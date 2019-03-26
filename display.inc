;***********************************************************************************
;Константы 
.equ		cnst_char_disp_sz	= 16	;Количество символов в строке

;===================================================================================
;Запись 0x30 в Дисплей для установки 8-ми битного режима
disp_wr_30:
	
	cbi		prt_disp, disp_RS		; Настройка линии для записи команд
	sbi		prt_disp, disp_E		; Установка бита E

	cbi		prt_disp, disp_D7		; 0
	cbi		prt_disp, disp_D6		; 0
	sbi		prt_disp, disp_D5		; 1
	sbi		prt_disp, disp_D4		; 1	
 
	nop

	cbi		prt_disp, disp_E		; Сброс бита E, запись данных  в Дисплей
	
	ret

;-----------------------------------------------------------------------------------
;Запись 0x20 в Дисплей для установки 4-x битного режима
disp_wr_20:
	
	cbi		prt_disp, disp_RS		; Настройка линии для записи команд
	sbi		prt_disp, disp_E		; Установка бита E

	cbi		prt_disp, disp_D7		; 0
	cbi		prt_disp, disp_D6		; 0
	sbi		prt_disp, disp_D5		; 1
	cbi		prt_disp, disp_D4		; 0
 
	nop

	cbi		prt_disp, disp_E		; Сброс бита E, запись данных  в Дисплей

ret

;-----------------------------------------------------------------------------------
;Запись в Дисплей (4-х) битный режим (Без установки RS)
;Вх - dsp
;Вых -||-
disp_wr:
	
	sbi		prt_disp, disp_E		; Установка бита E

	cbi		prt_disp, disp_D7		;|
	cbi		prt_disp, disp_D6		;|  Сброс выходов
	cbi		prt_disp, disp_D5		;|
	cbi		prt_disp, disp_D4		;|
		
	sbrc	dsp, 7					;|	
	sbi		prt_disp, disp_D7		;| Установка 7-го бита

	sbrc	dsp, 6					;|	
	sbi		prt_disp, disp_D6		;| Установка 6-го бита

	sbrc	dsp, 5					;|	
	sbi		prt_disp, disp_D5		;| Установка 5-го бита

	sbrc	dsp, 4					;|	
	sbi		prt_disp, disp_D4		;| Установка 4-го бита

	nop
	nop

	cbi		prt_disp, disp_E		; Сброс бита E, запись данных  в дисплей


	cbi		prt_disp, disp_D7		;|
	cbi		prt_disp, disp_D6		;|  Сброс выходов
	cbi		prt_disp, disp_D5		;|
	cbi		prt_disp, disp_D4		;|

	sbi		prt_disp, disp_E		; Установка бита E(Время между сбросом и установкой =>230нс)
		
	sbrc	dsp, 3					;|	
	sbi		prt_disp, disp_D7		;| Установка 3-го бита

	sbrc	dsp, 2					;|	
	sbi		prt_disp, disp_D6		;| Установка 2-го бита

	sbrc	dsp, 1					;|	
	sbi		prt_disp, disp_D5		;| Установка 1-го бита

	sbrc	dsp, 0					;|	
	sbi		prt_disp, disp_D4		;| Установка 0-го бита

	nop
	nop

	cbi		prt_disp, disp_E		; Сброс бита E, запись данных  в дисплей

	rcall	delay_100us				; Ожидание обработки Дисплеем

ret

;-----------------------------------------------------------------------------------
;Запись КОМАНД в Дисплей
;Вх - dsp
disp_wr_cmd:
	
	cbi		prt_disp, disp_RS		; Настройка линии для записи команд

	nop

	rcall	disp_wr

	ret

;-----------------------------------------------------------------------------------
;Запись ДАННЫХ в Дисплей
;Вх - dsp
disp_wr_data:

	sbi		prt_disp, disp_RS		; Настройка линии для записи данных

	nop

	rcall	disp_wr

	ret

;-----------------------------------------------------------------------------------
;Функция очистки буфера Дисплея
buff_clr:
	
	push	YL
	push	YH

	ldi16	YH, YL, buff_disp
	ldi		tmpH,buff_disp_sz
	ldi		tmpL,' '				; Очистка буфера пробелами

buff_clr_loop:
	st		y+, tmpL

	dec		tmpH
	brne	buff_clr_loop

	pop		YH
	pop		YL

	ret

;-----------------------------------------------------------------------------------
;Функция загрузки любой строки в буфер Дисплея
;Указатель на строку передаётся через parametrL и parametrH 
load_string_to_buff:

	push	YL
	push	YH
	push	ZL
	push	ZH

	ldi16	YH, YL, buff_disp		; Загрузка указателя на буфер в Y
	mov		ZL, parametrL			;| ; Загрузка указателя на строку в Z
	mov		ZH, parametrH			;|


string_to_buff_loop:
	lpm		tmpL,z+					;|Загружаем символ строки 
									;|из Flash  в регистр
	tst		tmpL					; Проверка на конец строки(0x00)
	breq	exit					

	st		y+, tmpL				; Загрузка регистра в буфер по указателю
	rjmp	string_to_buff_loop	

exit:
	pop		ZH
	pop		ZL
	pop		YH
	pop		YL	

	ret


;-----------------------------------------------------------------------------------
;Вывод буфера на Дисплей
buff_to_disp:
	
	push	XL
	push	YL
	push	YH
	
	ldi		dsp,0x80				;| 0x80 - адрес начала первой строки
	rcall	disp_wr_cmd				;| 

	ldi16	YH, YL, buff_disp
	ldi		XL,cnst_char_disp_sz	; Счётчик

buff_to_disp_loop1:
	ld		dsp, y+
	rcall	disp_wr_data
	
	dec		XL
	brne	buff_to_disp_loop1		;Конец загрузки буфера в первую строку Дисплея
;------------------
	ldi		dsp,0xC0				;| 0xC0 - адрес начала второй строки
	rcall	disp_wr_cmd				;|

	ldi		XL,cnst_char_disp_sz

;------------------
buff_to_disp_loop2:
	ld		dsp, y+
	rcall	disp_wr_data
	
	dec		XL
	brne	buff_to_disp_loop2

	pop		YH
	pop		YL
	pop		XL

	ret

;-----------------------------------------------------------------------------------
;Сдвиг буфера дисплея вправо
buff_shift_right:
	push	YL
	push	YH
	push	ZL
	push	ZH
	
	ldi16	YH, YL, buff_disp
	ldi		tmpL,((buff_disp_sz/2)-1)

	lds		ZL,buff_disp+15
	adiw	y,1

buff_shift_right_loop:
	ld		ZH,y
	st		y+, ZL
	mov		ZL, ZH
	
	dec		tmpL

	brne	buff_shift_right_loop


	pop		ZH
	pop		ZL
	pop		YH
	pop		YL

	ret

;Заставка
about:

	ldi16	tmpH, tmpL, string*2	;| Передаём параметры (указатель на строку)
	mov		parametrL, tmpL			;| функции load_string_to_buff
	mov		parametrH, tmpH			;|

	rcall	load_string_to_buff		; Загрузить строку в буфер Дисплея
	rcall	buff_to_disp
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_ms				;|
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_ms				;|
	sti8	ram_delay, tmpl, 255	;| Ждём 2,6 сек
	rcall 	delay_ms				;|
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_ms				;|
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_ms				;| 
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_ms

	rcall	buff_clr				;| Выключаем заставку
	rcall	buff_to_disp			;|
	ret

;***********************************************************************************
