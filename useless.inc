;***********************************************************************************
;Светодиоды бегают с заполнением
left_to_right:

	sti8	ram_delay, tmpl, 100

	sbi		prt_led1, bit_led1
	rcall 	delay_1x			

	cbi 	prt_led1, bit_led1


	sbi		prt_led2, bit_led2
	rcall 	delay_1x			

	cbi 	prt_led2, bit_led2


	sbi		prt_led3, bit_led3
	rcall 	delay_1x			

	cbi 	prt_led3, bit_led3


	sbi		prt_led4, bit_led4
	rcall 	delay_1x	

	cbi 	prt_led4, bit_led4


	sbi		prt_led5, bit_led5
	sti8	ram_delay, tmpL, 200
	rcall 	delay_1x	
	


	
	sti8	ram_delay, tmpl, 100
	
	sbi		prt_led1, bit_led1
	rcall 	delay_1x			

	cbi 	prt_led1, bit_led1


	sbi		prt_led2, bit_led2
	rcall 	delay_1x			

	cbi 	prt_led2, bit_led2


	sbi		prt_led3, bit_led3
	rcall 	delay_1x			

	cbi 	prt_led3, bit_led3


	sbi		prt_led4, bit_led4
	sti8	ram_delay, tmpL, 200
	rcall 	delay_1x	




	sti8	ram_delay, tmpl, 100

	sbi		prt_led1, bit_led1
	rcall 	delay_1x			

	cbi 	prt_led1, bit_led1


	sbi		prt_led2, bit_led2
	rcall 	delay_1x			

	cbi 	prt_led2, bit_led2


	sbi		prt_led3, bit_led3
	sti8	ram_delay, tmpL, 200
	rcall 	delay_1x	




	sti8	ram_delay, tmpl, 100

	sbi		prt_led1, bit_led1
	rcall 	delay_1x			

	cbi 	prt_led1, bit_led1


	sbi		prt_led2, bit_led2
	sti8	ram_delay, tmpL, 200
	rcall 	delay_1x		




	sbi		prt_led1, bit_led1
	rcall 	delay_1x
	rcall 	delay_1x
	rcall 	delay_1x			

	cbr		flag, (1<<f_led_on)

	cbi 	prt_led1, bit_led1
	cbi 	prt_led2, bit_led2
	cbi 	prt_led3, bit_led3
	cbi 	prt_led4, bit_led4
	cbi 	prt_led5, bit_led5

	ret

;-----------------------------------------------------------------------------------
;Светодиоды бегают с заполнением
right_to_left:

	sbi		prt_led5, bit_led5
	ldi		tmpL,150
	rcall 	delay_1x			

	cbi 	prt_led5, bit_led5


	sbi		prt_led4, bit_led4
	ldi		tmpL,150
	rcall 	delay_1x			

	cbi 	prt_led4, bit_led4


	sbi		prt_led3, bit_led3
	ldi		tmpL,150
	rcall 	delay_1x			

	cbi 	prt_led3, bit_led3


	sbi		prt_led2, bit_led2
	ldi		tmpL,150
	rcall 	delay_1x	

	cbi 	prt_led2, bit_led2

	sbi		prt_led1, bit_led1
	ldi		tmpL,200
	rcall 	delay_1x	
	
	
		
	
	sbi		prt_led5, bit_led5
	ldi		tmpL,150
	rcall 	delay_1x			

	cbi 	prt_led5, bit_led5


	sbi		prt_led4, bit_led4
	ldi		tmpL,150
	rcall 	delay_1x			

	cbi 	prt_led4, bit_led4


	sbi		prt_led3, bit_led3
	ldi		tmpL,150
	rcall 	delay_1x			

	cbi 	prt_led3, bit_led3


	sbi		prt_led2, bit_led2
	ldi		tmpL,200
	rcall 	delay_1x	





	sbi		prt_led5, bit_led5
	ldi		tmpL,150
	rcall 	delay_1x			

	cbi 	prt_led5, bit_led5


	sbi		prt_led4, bit_led4
	ldi		tmpL,150
	rcall 	delay_1x			

	cbi 	prt_led4, bit_led4


	sbi		prt_led3, bit_led3
	ldi		tmpL,200
	rcall 	delay_1x	






	sbi		prt_led5, bit_led5
	ldi		tmpL,150
	rcall 	delay_1x			

	cbi 	prt_led5, bit_led5


	sbi		prt_led4, bit_led4
	ldi		tmpL,200
	rcall 	delay_1x		





	sbi		prt_led5, bit_led5
	ldi		tmpL,200
	rcall 	delay_1x
	rcall 	delay_1x
	rcall 	delay_1x			

	cbi 	prt_led1, bit_led1
	cbi 	prt_led2, bit_led2
	cbi 	prt_led3, bit_led3
	cbi 	prt_led4, bit_led4
	cbi 	prt_led5, bit_led5

	cbr		flag, (1<<f_led_on)
	ret

;-----------------------------------------------------------------------------------
;Светодиоды бегут к центру и обратно
mv_to_cnt:

	sbi		prt_led1, bit_led1
	sbi		prt_led5, bit_led5
	ldi		tmpL,150
	rcall 	delay_1x			

	cbi 	prt_led1, bit_led1
	cbi 	prt_led5, bit_led5


	sbi		prt_led2, bit_led2
	sbi		prt_led4, bit_led4
	ldi		tmpL,150
	rcall 	delay_1x			

	cbi 	prt_led2, bit_led2
	cbi 	prt_led4, bit_led4


	sbi		prt_led3, bit_led3
	ldi		tmpL,150
	rcall 	delay_1x			

	cbi 	prt_led3, bit_led3

	sbi		prt_led2, bit_led2
	sbi		prt_led4, bit_led4
	ldi		tmpL,150
	rcall 	delay_1x			

	cbi 	prt_led2, bit_led2
	cbi 	prt_led4, bit_led4




	ret

;-----------------------------------------------------------------------------------
;Мигание третьего светодиода
 blink3:

 	sbi		prt_led3, bit_led3
	ldi		tmpL,50
	rcall 	delay_1x			

	cbi 	prt_led3, bit_led3

	cbr		flag, (1<<f_led_on)

	ret

;-----------------------------------------------------------------------------------
; Инверсия состояния светодиода 1
InvLed1:

	in		tmpL, prt_led1
	ldi		tmpH, (1<<bit_Led1)
	eor		tmpL, tmpH
	out		prt_led1, tmpL

	ret

;-----------------------------------------------------------------------------------
;Заставка
about:

	rcall	string_to_buff			; Загрузить строку в буфер Дисплея
	rcall	buff_to_disp
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_1x				;|
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_1x				;|
	sti8	ram_delay, tmpl, 255	;| Ждём 2,6 сек
	rcall 	delay_1x				;|
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_1x				;|
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_1x				;| 
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_1x

	rcall	buff_clr

	ret

;***********************************************************************************
