;***********************************************************************************

OV1_TIMER:
/*
	push	tmpL				; поместить в стек tmpL
	in		tmpL, SREG			; записать SREG в tmpL
	push	tmpL				; поместить SREG в стек
	push	tmpH	

	sbr		flag, (1<<f_led_on)	; set flag

	inc		var_ic

;	ldi16	0xD9DA,tmpL, tmpH	;|
								;|
;	sts 	TCNT1H, tmpH		;| Таймер заведён на 0,5с
;	sts 	TCNT1L, tmpL		;|

	pop		tmpH
	pop		tmpL
	out		SREG, tmpL
	pop		tmpL
*/
reti						; вернуться из прерывания 

;-----------------------------------------------------------------------------------

OC1A_TIMER:
/*
	push	tmpL				; поместить в стек tmpL
	in		tmpL, SREG			; записать SREG в tmpL
	push	tmpL				; поместить SREG в стек
	push	tmpH
		

	inc		var_ic

;	ldi16	0xD9DA,tmpL, tmpH	;|
								;|
;	sts 	TCNT1H, tmpH		;| Таймер заведён на 0,5с
;	sts 	TCNT1L, tmpL		;|

	pop		tmpH
	pop		tmpL
	out		SREG, tmpL
	pop		tmpL
*/
reti						; вернуться из прерывания 


;***********************************************************************************
