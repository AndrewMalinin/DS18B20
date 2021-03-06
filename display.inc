;***********************************************************************************
;��������� 
.equ		cnst_char_disp_sz	= 16	;���������� �������� � ������

;===================================================================================
;������ 0x30 � ������� ��� ��������� 8-�� ������� ������
disp_wr_30:
	
	cbi		prt_disp, disp_RS		; ��������� ����� ��� ������ ������
	sbi		prt_disp, disp_E		; ��������� ���� E

	cbi		prt_disp, disp_D7		; 0
	cbi		prt_disp, disp_D6		; 0
	sbi		prt_disp, disp_D5		; 1
	sbi		prt_disp, disp_D4		; 1	
 
	nop

	cbi		prt_disp, disp_E		; ����� ���� E, ������ ������  � �������
	
	ret

;-----------------------------------------------------------------------------------
;������ 0x20 � ������� ��� ��������� 4-x ������� ������
disp_wr_20:
	
	cbi		prt_disp, disp_RS		; ��������� ����� ��� ������ ������
	sbi		prt_disp, disp_E		; ��������� ���� E

	cbi		prt_disp, disp_D7		; 0
	cbi		prt_disp, disp_D6		; 0
	sbi		prt_disp, disp_D5		; 1
	cbi		prt_disp, disp_D4		; 0
 
	nop

	cbi		prt_disp, disp_E		; ����� ���� E, ������ ������  � �������

ret

;-----------------------------------------------------------------------------------
;������ � ������� (4-�) ������ ����� (��� ��������� RS)
;�� - dsp
;��� -||-
disp_wr:
	
	sbi		prt_disp, disp_E		; ��������� ���� E

	cbi		prt_disp, disp_D7		;|
	cbi		prt_disp, disp_D6		;|  ����� �������
	cbi		prt_disp, disp_D5		;|
	cbi		prt_disp, disp_D4		;|
		
	sbrc	dsp, 7					;|	
	sbi		prt_disp, disp_D7		;| ��������� 7-�� ����

	sbrc	dsp, 6					;|	
	sbi		prt_disp, disp_D6		;| ��������� 6-�� ����

	sbrc	dsp, 5					;|	
	sbi		prt_disp, disp_D5		;| ��������� 5-�� ����

	sbrc	dsp, 4					;|	
	sbi		prt_disp, disp_D4		;| ��������� 4-�� ����

	nop
	nop

	cbi		prt_disp, disp_E		; ����� ���� E, ������ ������  � �������


	cbi		prt_disp, disp_D7		;|
	cbi		prt_disp, disp_D6		;|  ����� �������
	cbi		prt_disp, disp_D5		;|
	cbi		prt_disp, disp_D4		;|

	sbi		prt_disp, disp_E		; ��������� ���� E(����� ����� ������� � ���������� =>230��)
		
	sbrc	dsp, 3					;|	
	sbi		prt_disp, disp_D7		;| ��������� 3-�� ����

	sbrc	dsp, 2					;|	
	sbi		prt_disp, disp_D6		;| ��������� 2-�� ����

	sbrc	dsp, 1					;|	
	sbi		prt_disp, disp_D5		;| ��������� 1-�� ����

	sbrc	dsp, 0					;|	
	sbi		prt_disp, disp_D4		;| ��������� 0-�� ����

	nop
	nop

	cbi		prt_disp, disp_E		; ����� ���� E, ������ ������  � �������

	rcall	delay_100us				; �������� ��������� ��������

ret

;-----------------------------------------------------------------------------------
;������ ������ � �������
;�� - dsp
disp_wr_cmd:
	
	cbi		prt_disp, disp_RS		; ��������� ����� ��� ������ ������

	nop

	rcall	disp_wr

	ret

;-----------------------------------------------------------------------------------
;������ ������ � �������
;�� - dsp
disp_wr_data:

	sbi		prt_disp, disp_RS		; ��������� ����� ��� ������ ������

	nop

	rcall	disp_wr

	ret

;-----------------------------------------------------------------------------------
;������� ������� ������ �������
buff_clr:
	
	push	YL
	push	YH

	ldi16	YH, YL, buff_disp
	ldi		tmpH,buff_disp_sz
	ldi		tmpL,' '				; ������� ������ ���������

buff_clr_loop:
	st		y+, tmpL

	dec		tmpH
	brne	buff_clr_loop

	pop		YH
	pop		YL

	ret

;-----------------------------------------------------------------------------------
;������� �������� ����� ������ � ����� �������
;��������� �� ������ ��������� ����� parametrL � parametrH 
load_string_to_buff:

	push	YL
	push	YH
	push	ZL
	push	ZH

	ldi16	YH, YL, buff_disp		; �������� ��������� �� ����� � Y
	mov		ZL, parametrL			;| ; �������� ��������� �� ������ � Z
	mov		ZH, parametrH			;|


string_to_buff_loop:
	lpm		tmpL,z+					;|��������� ������ ������ 
									;|�� Flash  � �������
	tst		tmpL					; �������� �� ����� ������(0x00)
	breq	exit					

	st		y+, tmpL				; �������� �������� � ����� �� ���������
	rjmp	string_to_buff_loop	

exit:
	pop		ZH
	pop		ZL
	pop		YH
	pop		YL	

	ret


;-----------------------------------------------------------------------------------
;����� ������ �� �������
buff_to_disp:
	
	push	XL
	push	YL
	push	YH
	
	ldi		dsp,0x80				;| 0x80 - ����� ������ ������ ������
	rcall	disp_wr_cmd				;| 

	ldi16	YH, YL, buff_disp
	ldi		XL,cnst_char_disp_sz	; �������

buff_to_disp_loop1:
	ld		dsp, y+
	rcall	disp_wr_data
	
	dec		XL
	brne	buff_to_disp_loop1		;����� �������� ������ � ������ ������ �������
;------------------
	ldi		dsp,0xC0				;| 0xC0 - ����� ������ ������ ������
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
;����� ������ ������� ������
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

;��������
about:

	ldi16	tmpH, tmpL, string*2	;| ������� ��������� (��������� �� ������)
	mov		parametrL, tmpL			;| ������� load_string_to_buff
	mov		parametrH, tmpH			;|

	rcall	load_string_to_buff		; ��������� ������ � ����� �������
	rcall	buff_to_disp
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_ms				;|
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_ms				;|
	sti8	ram_delay, tmpl, 255	;| ��� 2,6 ���
	rcall 	delay_ms				;|
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_ms				;|
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_ms				;| 
	sti8	ram_delay, tmpl, 255	;|
	rcall 	delay_ms

	rcall	buff_clr				;| ��������� ��������
	rcall	buff_to_disp			;|
	ret

;***********************************************************************************
