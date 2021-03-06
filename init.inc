;##############################################################################
; � ���� ����� ������� ��������� ������������� ��������� � ��������� �������
;##############################################################################


init_prt:

	sbi		ddr_led1, bit_led1	; ���� �� �����	 led1
	sbi		ddr_led2, bit_led2	; ���� �� �����	 led2
	sbi		ddr_led3, bit_led3	; ���� �� �����	 led3
	sbi		ddr_led4, bit_led4	; ���� �� �����	 led4
	sbi		ddr_led5, bit_led5	; ���� �� �����	 led5

	sbi		prt_key1, bit_key1	; ��� ������������� ��������	 ������ 1
	sbi		prt_key2, bit_key2	; ��� ������������� ��������	 ������ 2

	
	sbi		ddr_disp,disp_RS	; ���������� ���� ������� RS �� �����
	cbi		prt_disp,disp_E		; �������� ��� E ����� DISPLAY
	sbi		ddr_disp,disp_E		; ���������� ���� ������� E �� �����


	sbi		ddr_disp,disp_D4	;|
	sbi		ddr_disp,disp_D5	;|	
	sbi		ddr_disp,disp_D6	;|	
	sbi		ddr_disp,disp_D7	;|��������� DATALINES �� �����


ret

;-----------------------------------------------------------------------------------
;TIMER COUNTER 1
init_TC1:					
	ldi16	tmpH, tmpL, cnst_OCR1A 
	
	sts		OCR1AH,tmpH
	sts		OCR1AL,tmpL

	ldi		tmpL, cnst_TCCR1B
	ldi		tmpH, cnst_TIMSK1
		
	sts		TCCR1B, tmpL		; ��������� ������������ � ������ ������
	sts		TIMSK1, tmpH		; ��������� ���������� ����������

ret

;-----------------------------------------------------------------------------------
;������������� �������
init_disp:

	sti8	ram_delay, tmpL, 20	;|
	rcall	delay_1ms			;| �������� 20��(=> 15) ����� ������ �������
	
	rcall	disp_wr_30			; ������������� 8-�� ������� ������ (1/3)
	sti8	ram_delay, tmpL, 5	;|
	rcall	delay_1ms			;| �������� 5��(=> 4.1) 

	rcall	disp_wr_30			; ������������� 8-�� ������� ������ (2/3)
	rcall	delay_100us			;| �������� 100���


	rcall	disp_wr_30			; ������������� 8-�� ������� ������ (3/3)
	rcall	delay_100us			;| �������� 100��� ��� ��������� �������� ��������

	rcall 	disp_wr_20			; ������������� 4-� ������� ������
	rcall	delay_100us			;| �������� 100��� ��� ��������� �������� ��������

;------------------------

	ldi		dsp,0x28
	rcall	disp_wr_cmd

	ldi		dsp,0x06
	rcall	disp_wr_cmd

	ldi		dsp,0x01
	rcall	disp_wr_cmd
	rcall	delay_1ms
	rcall	delay_1ms


	ldi		dsp,0x0C
	rcall	disp_wr_cmd

ret

;-----------------------------------------------------------------------------------
;������������� ���
init_ADC:
	ldi		tmpL, cnst_ADMUX	;|
	sts		ADMUX,tmpL			;|��������� ��������������

	ldi		tmpL, cnst_ADCSRA	;|
	sts		ADCSRA, tmpL		;|

	ldi		tmpL, cnst_ADCSRB	;|
	sts		ADCSRB, tmpL		;| 

	ldi		tmpL, cnst_DIDR0	;|
	sts		DIDR0, tmpL			;|

ret

;-----------------------------------------------------------------------------------
;������������� USART
init_USART:
	sti16	UBRR0H,UBRR0L, tmpL, cnst_UBRR0 

	sti8	UCSR0A, tmpL, cnst_UCSR0A
	sti8	UCSR0B, tmpL, cnst_UCSR0B
	sti8	UCSR0C, tmpL, cnst_UCSR0C

ret

;-----------------------------------------------------------------------------------
;������������� ����������
init_vars:

	ldi		tmpL, 0x00

	mov		var_ic, tmpL

ret	

;***********************************************************************************
