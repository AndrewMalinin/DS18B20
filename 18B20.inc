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
			cbi		prt_DS18B20, line_DS18B20	; ����� �����
ret
;----------------------- ������� ����������� ---------------------------------------
OneWire_reset_impulse:		
	sbi		ddr_DS18B20, line_DS18B20	; ����� 1-Wire �� ������ 
	//cbi		prt_DS18B20, line_DS18B20	; ����c ����� � ���������� ����

	sti8	ram_delay, tmpL, 250	;|
	rcall	delay_us				;|
	sti8	ram_delay, tmpL, 250	;|
	rcall	delay_us				;|
	sti8	ram_delay, tmpL, 100	;| ������ ������ ������� 600���.
	rcall	delay_us				;| (������� - 480���.)

	cbi		ddr_DS18B20, line_DS18B20	; ����� 1-Wire �� ���� 
//	cbi		prt_DS18B20, line_DS18B20	; ��� Z-��������� �����
ret
;--------------------- �������� ������� --------------------------------------------
OneWire_presense_impulse:
	sti8	ram_delay, tmpL, 70		;| ����� ����� 15��� ����� �������� �����������
	rcall	delay_us				;| � ������� ������ ������ �� 60 �� 240���

	sbic	pin_DS18B20, line_DS18B20	;| ���� �� ���� 0 - ������������ �������
		rjmp	slave_not_found			;| ����� - ERROR

/*	rcall	buff_clr
	ldi16	tmpH, tmpL, DS18B20_detected*2		;| ������� ��������� (��������� �� ������)
	mov		parametrL, tmpL						;| ������� load_string_to_buff
	mov		parametrH, tmpH						;|
	rcall	load_string_to_buff					; ��������� ������ � ����� �������
	rcall	buff_to_disp
*/
	sti8	ram_delay, tmpL, 255	;|
	rcall	delay_us				;|
	sti8	ram_delay, tmpL, 145	;| �������� 400���
	rcall	delay_us				;|

ret
;------------------ ������� �����������  1 � 0 �� ����� 1-Wire ---------------------
OneWire_write_one:
	sbi		ddr_DS18B20, line_DS18B20	; ����� �����

	sti8	ram_delay, tmpL, 6			;| 
	rcall	delay_us					;| ��������� ���� � ������� 15���

	cbi		ddr_DS18B20, line_DS18B20	;| �������� ����������� ����, 
										;| � �� ���� ��������� ���. 1
	sti8	ram_delay, tmpL, 63		;| 
	rcall	delay_us					;| �������� 70 ��� (������� - 60)

ret

OneWire_write_zero:
	sbi		ddr_DS18B20, line_DS18B20	; ���� 1-Wire �� �����


	sti8	ram_delay, tmpL, 63		;| 
	rcall	delay_us					;| �������� 85��� (15��� + 70��� ��� ��������� 0)

ret

OneWire_recovery_impulse:
	cbi		ddr_DS18B20, line_DS18B20	;| �������� ����������� ����, 
										;| � �� ���� ��������� ���. 1
	sti8	ram_delay, tmpL, 3			;| 
	rcall	delay_us	

ret
;---------------------- ������� �������� ������ ������� ----------------------------
DS18B20_wr_cmd:
	cli			; ������ ����������
	ldi		Cntr, 8
	DS18B20_wr_cmd_loop:		; ��������� ���������� �����


		sbrs	instructionR, 0	
			rcall	OneWire_write_zero

		sbrc	instructionR, 0	
			rcall	OneWire_write_one
			
		rcall	OneWire_recovery_impulse
		lsr		instructionR


		dec		Cntr					; Cntr--
		brne	DS18B20_wr_cmd_loop		; �������, ���� Cntr!=0

	sei			; ���������� ����������
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
;-------------------------- ������ ���������� �� DS18B20 ---------------------------
DS18B20_read_one_byte:
	clr		tmpH
	ldi		Cntr, 8

	DS18B20_read_data_loop:
		sbi		ddr_DS18B20, line_DS18B20	;|
		sti8	ram_delay, tmpL, 3			;| ����� 3���
		rcall	delay_us					;| 

		cbi		ddr_DS18B20, line_DS18B20	;|  ���������� ������� ��������� �����

		sti8	ram_delay, tmpL, 19		;| 
		rcall	delay_us					;|
		sbic	pin_DS18B20, line_DS18B20	;| ���� 25��� � ������ ��������� �����
			sbr		tmpH, 0b10000000
		lsr		tmpH

		sti8	ram_delay, tmpL, 42			;| 
		rcall	delay_us					;|

		dec Cntr
	brne	DS18B20_read_data_loop
	
	rol		tmpH

ret
;----------------------- C������ 2 ����� ����������� -------------------------------
DS18B20_read_temperature:


	rcall	DS18B20_read_one_byte			; C�������� ������� ����
	sts		DS18B20_low_temp_byte, tmpH		; ����� � ������

	rcall	DS18B20_read_one_byte			; C�������� �������  ����
	sts		DS18B20_high_temp_byte, tmpH	; ����� � ������


ret
;-----------------------------------------------------------------------------------
convert_temperature:
	clr		tmpL
	lds		tmpH, DS18B20_high_temp_byte	;| ��������� ������� ����
	ldi		Cntr, 4
	convert_temperature_1Loop:				;| � ���� ������ ������������ ���� 
		sbrc	tmpH, 3						;| �� ��������� ����������� �� ���� �������� 
			sbr		tmpL, 0b00000001		;| ������, � ���� ������� tmpL
											;| � ��������� ����� � integer_part
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

	; ################ ������� ����� ##################
	clr		tmpL
	lds		tmpH, DS18B20_low_temp_byte	;| ��������� ������� ����
	ldi		Cntr, 4						;| ��� ��������� ������� - ������� ����� �����������
	convert_temperature_3Loop:
		sbrc	tmpH, 3
			sbr		tmpL, 0b00000001

		lsl		tmpH
		lsl		tmpL
	dec		Cntr
	brne	convert_temperature_3Loop
	ROR		tmpL

	// �������� ������� ����� � ����������� �����
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

	sts		fractional_part, tmpL	;| ��������� ������� ����� � ����������

		

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
	ldi		tmpL,0x2E				;| �������
	sts		buff_disp+6, tmpL
	ldi		tmpL,0xDF				;| ������ �������
	sts		buff_disp+8, tmpL
	ldi		tmpL,0x43				;| ����� C
	sts		buff_disp+9, tmpL		
ret
;----------------------------------- ERRORS ----------------------------------------
//ERRORS
slave_not_found:
	rcall	buff_clr

	ldi16	tmpH, tmpL, error_slave_not_found*2 ;| ������� ��������� (��������� �� ������)
	mov		parametrL, tmpL						;| ������� load_string_to_buff
	mov		parametrH, tmpH						;|

	rcall	load_string_to_buff		; ��������� ������ � ����� �������
	rcall	buff_to_disp
ret
	
.cseg
error_slave_not_found:	.db "     ERROR:     "				;| � ������ - 16 ��������
						.db "SLAVE NOT FOUND ",0x00,0x00	;|

DS18B20_detected:		.db "  DS18B20       "				;| � ������ - 16 ��������
						.db "    DETECTED    ",0x00,0x00	;|