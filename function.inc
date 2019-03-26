;***********************************************************************************
;Перевод из BIN в ASCII-HEX (8bit)
;Вх. XL
;Вых. XL - мл.разряд, XH - ст.разряд
bin_hex:

	mov		XH, XL			; Копируем число во второй регистр

	andi	XL, 0x0F		; Оставляем в XL младший полубайт
	subi	XL, -0x30		; Добавляем 0x30 для получения ASCII-адреса цифр

	cpi		XL, 0x3A		;|
	brsh	bin_hex_L1		;|Переходим, если нужны буквы от A до F
	rjmp	bin_hex_H		; Если букв нет, то переходим к анализу старшего

bin_hex_L1:
	subi	XL,-0x07		; Добавляем 0x07 для получения ASCII-адреса букв

bin_hex_H:
	swap	XH				; Меняем полубайты местами
	andi	XH, 0x0F		; Оставляем в XH младший полубайт
	subi	XH, -0x30		; Добавляем 0x30 для получения ASCII-адреса цифр

	cpi		XH, 0x3A		;|
	brsh	bin_hex_H1		;|Переходим, если нужны буквы от A до F
	rjmp	bin_hex_exit	; Если букв нет, то завершаем анализ

bin_hex_H1:
	subi	XH,-0x07		; Добавляем 0x07 для получения ASCII-адреса букв

bin_hex_exit:

	ret

;-----------------------------------------------------------------------------------
; Перевод из двоичного кода в три байта хранящие разряды десятичного числа 
;Вх. parametrL
;Вых. units - единицы, tens - десятки, century - сотни(первый бит - знаковый)
bin_dec:
	clr		tmpL	; Будет работать с единицами
	clr		tmpH	; Будет работать с десятками
	clr		tmp		; Будет работать с сотнями

	mov		tmpL,	parametrL

	cpi		tmpL, 10		;|
	brlo	bin_dec_exit	;| Если число меньше десяти, то выходим из подпрограммы
	bin_dec_units_loop:
	
	subi	tmpl, 10		; Уменьшаем разряд единиц на 10 
	inc		tmpH			; Прибавляем к разряду десятков 1

	cpi		tmpL, 10			;|Если число меньше 10, тогда выходим
	brsh	bin_dec_units_loop	;|
	// После  этого цикла образуется колчиество единиц в регистре tmpL

	cpi		tmpH, 10		;|
	brlo	bin_dec_exit	;| Если десятков меньше десяти, то выходим из подпрограммы
	bin_dec_tens_loop:
	
	subi	tmpH, 10		; Уменьшаем разряд десятков на 10 
	inc		tmp			; Прибавляем к разряду сотен 1

	cpi		tmpH, 10
	brsh	bin_dec_tens_loop

	bin_dec_exit:
	subi	tmpL, -0x30
	subi	tmpH, -0x30
	subi	tmp, -0x30
	sts		units,	tmpL
	sts		tens,	tmpH
	sts		century,tmp
ret
;-----------------------------------------------------------------------------------
;Светодиодная индикация уровня напряжения 
;Вх - ADC_out
;Вых -||-
LED_indicator:

	cpi		ADC_out, ((0xFF)/5)*1
	brsh	LED_indicator_1
	cbi		prt_led1,bit_led1

LED_indicator_cp2:
	cpi		ADC_out, ((0xFF)/5)*2
	brsh	LED_indicator_2
	cbi		prt_led2,bit_led2

LED_indicator_cp3:
	cpi		ADC_out, ((0xFF)/5)*3
	brsh	LED_indicator_3
	cbi		prt_led3,bit_led3

LED_indicator_cp4:
	cpi		ADC_out, ((0xFF)/5)*4
	brsh	LED_indicator_4
	cbi		prt_led4,bit_led4

LED_indicator_cp5:
	cpi		ADC_out, (0xFF)-5
	brsh	LED_indicator_5
	cbi		prt_led5,bit_led5

	rjmp	LED_indicator_exit


LED_indicator_1:
	sbi		prt_led1,bit_led1
	rjmp	LED_indicator_cp2

LED_indicator_2:
	sbi		prt_led2,bit_led2
	rjmp	LED_indicator_cp3

LED_indicator_3:
	sbi		prt_led3,bit_led3
	rjmp	LED_indicator_cp4

LED_indicator_4:
	sbi		prt_led4,bit_led4
	rjmp	LED_indicator_cp5

LED_indicator_5:
	sbi		prt_led5,bit_led5
	rjmp	LED_indicator_exit

LED_indicator_exit:

	ret

;***********************************************************************************	
	
