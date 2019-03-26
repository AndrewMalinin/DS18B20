;-----------------------------------------------------------------------------------
.equ	prt_DS18B20		=	PORTD	
.equ	ddr_DS18B20		=	DDRD
.equ	line_DS18B20	=	PD7
.equ	pin_DS18B20		=	PIND
.dseg
	DS18B20_low_temp_byte:	.byte 1
	DS18B20_high_temp_byte:	.byte 1
	integer_part:			.byte 1
	fractional_part:		.byte 1
.cseg
;-----------------------------------------------------------------------------------

init_DS18B20:
			cbi		prt_DS18B20, line_DS18B20	; Сброс линии
ret
;----------------------- Импульс присутствия ---------------------------------------
OneWire_reset_impulse:		
	sbi		ddr_DS18B20, line_DS18B20	; Линия 1-Wire на вывыод 
	//cbi		prt_DS18B20, line_DS18B20	; Сброc линии в логический ноль

	sti8	ram_delay, tmpL, 250	;|
	rcall	delay_us				;|
	sti8	ram_delay, tmpL, 250	;|
	rcall	delay_us				;|
	sti8	ram_delay, tmpL, 100	;| Держим низкий уровень 600мкс.
	rcall	delay_us				;| (минимум - 480мкс.)

	cbi		ddr_DS18B20, line_DS18B20	; Линия 1-Wire на ввод 
//	cbi		prt_DS18B20, line_DS18B20	; вкл Z-состояние входа
ret
;--------------------- Ответный импульс --------------------------------------------
OneWire_presense_impulse:
	sti8	ram_delay, tmpL, 70		;| нужно ждать 15мкс после импульса присутствия
	rcall	delay_us				;| и импульс ответа длится от 60 до 240мкс

	sbic	pin_DS18B20, line_DS18B20	;| Если на шине 0 - присутствуют датчики
		rjmp	slave_not_found			;| иначе - ERROR

/*	rcall	buff_clr
	ldi16	tmpH, tmpL, DS18B20_detected*2		;| Передаём параметры (указатель на строку)
	mov		parametrL, tmpL						;| функции load_string_to_buff
	mov		parametrH, tmpH						;|
	rcall	load_string_to_buff					; Загрузить строку в буфер Дисплея
	rcall	buff_to_disp
*/
	sti8	ram_delay, tmpL, 255	;|
	rcall	delay_us				;|
	sti8	ram_delay, tmpL, 145	;| задержка 400мкс
	rcall	delay_us				;|

ret
;------------------ Функции выставления  1 и 0 на линию 1-Wire ---------------------
OneWire_write_one:
	sbi		ddr_DS18B20, line_DS18B20	; Сброс линии

	sti8	ram_delay, tmpL, 6			;| 
	rcall	delay_us					;| отпускаем шину в течение 15мкс

	cbi		ddr_DS18B20, line_DS18B20	;| Включаем высокоомный вход, 
										;| и на шине состояние лог. 1
	sti8	ram_delay, tmpL, 63		;| 
	rcall	delay_us					;| Задержка 70 мкс (минимум - 60)

ret

OneWire_write_zero:
	sbi		ddr_DS18B20, line_DS18B20	; Шина 1-Wire на выход


	sti8	ram_delay, tmpL, 63		;| 
	rcall	delay_us					;| Задержка 85мкс (15мкс + 70мкс это установка 0)

ret

OneWire_recovery_impulse:
	cbi		ddr_DS18B20, line_DS18B20	;| Включаем высокоомный вход, 
										;| и на шине состояние лог. 1
	sti8	ram_delay, tmpL, 3			;| 
	rcall	delay_us	

ret
;---------------------- Функция отправки команд датчику ----------------------------
DS18B20_wr_cmd:
	cli			; Запрет прерываний
	ldi		Cntr, 8
	DS18B20_wr_cmd_loop:		; цикличное считывание битов


		sbrs	instructionR, 0	
			rcall	OneWire_write_zero

		sbrc	instructionR, 0	
			rcall	OneWire_write_one
			
		rcall	OneWire_recovery_impulse
		lsr		instructionR


		dec		Cntr					; Cntr--
		brne	DS18B20_wr_cmd_loop		; перейти, если Cntr!=0

	sei			; Разрешение прерываний
ret
;---------------------------------- SKIP ROM (0xCC) --------------------------------
DS18B20_skip_ROM:
	ldi		tmpL, 0xCC
	mov		instructionR, tmpL
	rcall	DS18B20_wr_cmd
ret
;------------------------------- CONVERT T (0x44) ----------------------------------
DS18B20_convertT:
	ldi		tmpL, 0x44
	mov		instructionR, tmpL
	rcall	DS18B20_wr_cmd
ret
;-------------------------------- READ SCRATCH (0xBE) ------------------------------
DS18B20_read_scratch:
	ldi		tmpL, 0xBE
	mov		instructionR, tmpL
	rcall	DS18B20_wr_cmd
ret
;-------------------------- Чтение информации от DS18B20 ---------------------------
DS18B20_read_one_byte:
	clr		tmpH
	ldi		Cntr, 8

	DS18B20_read_data_loop:
		sbi		ddr_DS18B20, line_DS18B20	;|
		sti8	ram_delay, tmpL, 3			;| Сброс 3мкс
		rcall	delay_us					;| 

		cbi		ddr_DS18B20, line_DS18B20	;|  Возвращаем высокое состояние линии

		sti8	ram_delay, tmpL, 19		;| 
		rcall	delay_us					;|
		sbic	pin_DS18B20, line_DS18B20	;| Ждем 25мкс и читаем состояние линии
			sbr		tmpH, 0b10000000
		lsr		tmpH

		sti8	ram_delay, tmpL, 42			;| 
		rcall	delay_us					;|

		dec Cntr
	brne	DS18B20_read_data_loop
	
	rol		tmpH

ret
;----------------------- Cчитать 2 байта температуры -------------------------------
DS18B20_read_temperature:


	rcall	DS18B20_read_one_byte			; Cчитываем младший байт
	sts		DS18B20_low_temp_byte, tmpH		; Пишем в память

	rcall	DS18B20_read_one_byte			; Cчитываем старший  байт
	sts		DS18B20_high_temp_byte, tmpH	; Пишем в память


ret
;-----------------------------------------------------------------------------------
convert_temperature:
	clr		tmpL
	lds		tmpH, DS18B20_high_temp_byte	;| Загружаем старший байт
	ldi		Cntr, 4
	convert_temperature_1Loop:				;| В этих циклах переписываем байт 
		sbrc	tmpH, 3						;| со значением температуры из двух принятых 
			sbr		tmpL, 0b00000001		;| байтов, в один регистр tmpL
											;| и сохраняем далее в integer_part
		lsl		tmpH
		lsl		tmpL
	dec		Cntr
	brne	convert_temperature_1Loop

	lds		tmpH, DS18B20_low_temp_byte
	ldi		Cntr, 4

	convert_temperature_2Loop:
		sbrc	tmpH, 7
			sbr		tmpL, 0b00000001

		lsl		tmpH
		lsl		tmpL
	dec		Cntr
	brne	convert_temperature_2Loop
	ROR		tmpL


	cbr		tmpL, 0b10000000
	sts		integer_part, tmpL

	; ################ Дробная часть ##################
	clr		tmpL
	lds		tmpH, DS18B20_low_temp_byte	;| Загружаем младший байт
	ldi		Cntr, 4						;| где последняя тетрада - дробная часть температуры
	convert_temperature_3Loop:
		sbrc	tmpH, 3
			sbr		tmpL, 0b00000001

		lsl		tmpH
		lsl		tmpL
	dec		Cntr
	brne	convert_temperature_3Loop
	ROR		tmpL

	// Приведем дробную часть к десятичному числу
	mov		tmpH, tmpL
	clr		tmpL
	sbrc	tmpH, 0
		subi	tmpL, -7
	sbrc	tmpH, 1
		subi	tmpL, -13
	sbrc	tmpH, 2
		subi	tmpL, -25
	sbrc	tmpH, 3
		subi	tmpL, -50

	sts		fractional_part, tmpL	;| Загружаем дробную часть в переменную

		

ret

;-----------------------------------------------------------------------------------
temperatute_to_buff:
	lds		parametrL, integer_part
	rcall	bin_dec
	lds		tmpL, units
	sts		buff_disp+5, tmpL
	lds		tmpL, tens
	tst		tmpL
	breq	temperatute_to_buff_zero_tens
	sts		buff_disp+4, tmpL
	temperatute_to_buff_zero_tens:
	lds		tmpL, century
	subi	tmpL, 0x30
	tst		tmpL
	breq	temperatute_to_buff_zero_century
	sts		buff_disp+3, tmpL
	temperatute_to_buff_zero_century:
	lds		parametrL, fractional_part
	rcall	bin_dec
	lds		tmpL, tens
	sts		buff_disp+7, tmpL
	ldi		tmpL,0x2E				;| Запятая
	sts		buff_disp+6, tmpL
	ldi		tmpL,0xDF				;| Значок градуса
	sts		buff_disp+8, tmpL
	ldi		tmpL,0x43				;| Буква C
	sts		buff_disp+9, tmpL		
ret
;----------------------------------- ERRORS ----------------------------------------
//ERRORS
slave_not_found:
	rcall	buff_clr

	ldi16	tmpH, tmpL, error_slave_not_found*2 ;| Передаём параметры (указатель на строку)
	mov		parametrL, tmpL						;| функции load_string_to_buff
	mov		parametrH, tmpH						;|

	rcall	load_string_to_buff		; Загрузить строку в буфер Дисплея
	rcall	buff_to_disp
ret
	
.cseg
error_slave_not_found:	.db "     ERROR:     "				;| В строке - 16 символов
						.db "SLAVE NOT FOUND ",0x00,0x00	;|

DS18B20_detected:		.db "  DS18B20       "				;| В строке - 16 символов
						.db "    DETECTED    ",0x00,0x00	;|