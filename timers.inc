;***********************************************************************************

OV1_TIMER:
/*
	push	tmpL				; ��������� � ���� tmpL
	in		tmpL, SREG			; �������� SREG � tmpL
	push	tmpL				; ��������� SREG � ����
	push	tmpH	

	sbr		flag, (1<<f_led_on)	; set flag

	inc		var_ic

;	ldi16	0xD9DA,tmpL, tmpH	;|
								;|
;	sts 	TCNT1H, tmpH		;| ������ ������ �� 0,5�
;	sts 	TCNT1L, tmpL		;|

	pop		tmpH
	pop		tmpL
	out		SREG, tmpL
	pop		tmpL
*/
reti						; ��������� �� ���������� 

;-----------------------------------------------------------------------------------

OC1A_TIMER:
/*
	push	tmpL				; ��������� � ���� tmpL
	in		tmpL, SREG			; �������� SREG � tmpL
	push	tmpL				; ��������� SREG � ����
	push	tmpH
		

	inc		var_ic

;	ldi16	0xD9DA,tmpL, tmpH	;|
								;|
;	sts 	TCNT1H, tmpH		;| ������ ������ �� 0,5�
;	sts 	TCNT1L, tmpL		;|

	pop		tmpH
	pop		tmpL
	out		SREG, tmpL
	pop		tmpL
*/
reti						; ��������� �� ���������� 


;***********************************************************************************
