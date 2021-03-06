;***********************************************************************************
;������� �� BIN � ASCII-HEX (8bit)
;��. XL
;���. XL - ��.������, XH - ��.������
bin_hex:

	mov		XH, XL			; �������� ����� �� ������ �������

	andi	XL, 0x0F		; ��������� � XL ������� ��������
	subi	XL, -0x30		; ��������� 0x30 ��� ��������� ASCII-������ ����

	cpi		XL, 0x3A		;|
	brsh	bin_hex_L1		;|���������, ���� ����� ����� �� A �� F
	rjmp	bin_hex_H		; ���� ���� ���, �� ��������� � ������� ��������

bin_hex_L1:
	subi	XL,-0x07		; ��������� 0x07 ��� ��������� ASCII-������ ����

bin_hex_H:
	swap	XH				; ������ ��������� �������
	andi	XH, 0x0F		; ��������� � XH ������� ��������
	subi	XH, -0x30		; ��������� 0x30 ��� ��������� ASCII-������ ����

	cpi		XH, 0x3A		;|
	brsh	bin_hex_H1		;|���������, ���� ����� ����� �� A �� F
	rjmp	bin_hex_exit	; ���� ���� ���, �� ��������� ������

bin_hex_H1:
	subi	XH,-0x07		; ��������� 0x07 ��� ��������� ASCII-������ ����

bin_hex_exit:

	ret

;-----------------------------------------------------------------------------------
; ������� �� ��������� ���� � ��� ����� �������� ������� ����������� ����� 
;��. parametrL
;���. units - �������, tens - �������, century - �����(������ ��� - ��������)
bin_dec:
	clr		tmpL	; ����� �������� � ���������
	clr		tmpH	; ����� �������� � ���������
	clr		tmp		; ����� �������� � �������

	mov		tmpL,	parametrL

	cpi		tmpL, 10		;|
	brlo	bin_dec_exit	;| ���� ����� ������ ������, �� ������� �� ������������
	bin_dec_units_loop:
	
	subi	tmpl, 10		; ��������� ������ ������ �� 10 
	inc		tmpH			; ���������� � ������� �������� 1

	cpi		tmpL, 10			;|���� ����� ������ 10, ����� �������
	brsh	bin_dec_units_loop	;|
	// �����  ����� ����� ���������� ���������� ������ � �������� tmpL

	cpi		tmpH, 10		;|
	brlo	bin_dec_exit	;| ���� �������� ������ ������, �� ������� �� ������������
	bin_dec_tens_loop:
	
	subi	tmpH, 10		; ��������� ������ �������� �� 10 
	inc		tmp			; ���������� � ������� ����� 1

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
;������������ ��������� ������ ���������� 
;�� - ADC_out
;��� -||-
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
	
