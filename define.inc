;***********************************************************************************
;����������

.equ	prt_led1	= PORTC			;LED 1
.equ	ddr_led1	= DDRC
.equ    bit_led1	= PC2

.equ	prt_led2	= PORTC			;LED 2
.equ	ddr_led2	= DDRC
.equ    bit_led2    = PC3

.equ	prt_led3    = PORTC			;LED 3
.equ	ddr_led3	= DDRC
.equ    bit_led3    = PC4

.equ	prt_led4    = PORTC			;LED 4
.equ	ddr_led4	= DDRC
.equ    bit_led4    = PC5

.equ	prt_led5    = PORTD			;LED 5
.equ	ddr_led5	= DDRD
.equ    bit_led5    = PD0

;-----------------------------------------------------------------------------------
;������

.equ	prt_key1    = PORTC			;������ 1
.equ	pin_key1	= PINC
.equ    bit_key1    = PC1

.equ	prt_key2	= PORTC			;������ 2
.equ	pin_key2	= PINC
.equ    bit_key2	= PC0

;-----------------------------------------------------------------------------------
;�������(4 ����)

.equ	prt_disp	= PORTB			; ���� �������
.equ	disp_RS		= PB0			; ����� ������ ������(0-������� ������, 1-������� ������)
.equ	disp_E		= PB1			; ����� �������������(Latch)/�������������

.equ	disp_D4		= PB2			; DATA LINE 4
.equ	disp_D5		= PB3			; DATA LINE 5
.equ	disp_D6		= PB4			; DATA LINE 6
.equ	disp_D7		= PB5			; DATA LINE 7

.equ	ddr_disp	= DDRB	
;-----------------------------------------------------------------------------------

.def	parametrL		= r0	;| ����� ������� �������, ����� ��� ��������
.def	parametrH		= r1	;| ����� �������� ������� ��������
.def	instructionR	= r2	; �������, � ������� ����� ���������� ��������� ��� �������� �� ����������
;.def	----			= r3	;
;.def	----			= r4	;
;.def	----			= r5	;
;.def	----			= r6	;
.def	var_ic			= r7	; Interrupt Counter
;.def	----			= r8	;
;.def	----			= r9	;
;.def	----			= r10	;
;.def	----			= r11	;
;.def	AdrMinL			= r12	;����� ����� ������ � ������� �����
;.def	AdrMinH			= r13	;
;.def	----			= r14	;
;.def	SREG_int		= r15	;��������� �������� SREG � ����������. *** �� ������������ ��� ��������� ����������� ***

;---
.def	tmpL			= r16	;��������� 16-�� ������ �������, �� ����������� � �������������
.def	tmpH			= r17	;
.def	Cntr			= r18	;������� ��� ������
.def	flag			= r19	;�����
.def	Button			= r20	;������� ��������� ������
.def	dsp				= r21	;�������� ��� ������ �� �������
.def	tmp				= r22	;������� ��� ��������
.def	adc_out			= r23	; ��� ����� ���� ��������� ��������������
;.def	------			= r24	;���� 1
;.def	Bin24_01		= r25	;���� 2
;		XL				= r26
;		XH				= r27
;		YL				= r28
;		YH				= r29
;		ZL				= r30
;		ZH				= r31

;-----------------------------------------------------------------------------------
;����� �������� Flag

.equ	f_old_key		= 0		; ���� ������� ������
.equ	f_new_key		= 1		;
.equ	f_led_on		= 2		;

;-----------------------------------------------------------------------------------

; ��������� ������� 1

.equ	cnst_TCCR1B		= (0<<WGM13)|(1<<WGM12)|(1<<CS12)|(0<<CS11)|(1<<CS10)
.equ	cnst_TIMSK1		= (1<<OCIE1A)
.equ	cnst_OCR1A		= 0x2625				; $2625 = 0.5c

;-----------------------------------------------------------------------------------
;��������� ���

.equ	cnst_ADMUX		= (0<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(0<<MUX0)
.equ	cnst_ADCSRA		= (1<<ADEN)|(0<<ADSC)|(1<<ADATE)|(1<<ADIF)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)
.equ	cnst_ADCSRB		= (0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0)
.equ	cnst_DIDR0		= (0<<ADC5D)|(0<<ADC4D)|(0<<ADC3D)|(0<<ADC2D)|(0<<ADC1D)|(0<<ADC0D)

;-----------------------------------------------------------------------------------
;��������� USART
.equ 	XTAL = 20000000	
.equ 	baudrate = 9600  
.equ 	bauddivider = XTAL/(16*baudrate)-1

.equ	cnst_UBRR0		= 103	; �������� 9600 ���
.equ	cnst_UCSR0A		=(0<<U2X0)|(0<<MPCM0)
.equ	cnst_UCSR0B		=(0<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0)|(0<<UCSZ02)
.equ	cnst_UCSR0C		=(0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0)

;-----------------------------------------------------------------------------------
; ����������

.dseg					// ���������� � ������� ������
.org	SRAM_START		// � ������ ������ ������ 

.equ	buff_disp_sz	= 32			; ������ ������ �������

buff_disp:			.byte buff_disp_sz	; ����� ������� � ���
ram_delay:			.byte 1				; ���������� ��� delay_us, delay_ms	
units:				.byte 1				;| 
tens:				.byte 1				;| ���������� ��� bin_dec
century:			.byte 1				;| 


;-----------------------------------------------------------------------------------
; ���������

.equ	Ftakt	= 20000000		;�������� ������� � ��

;***********************************************************************************
.cseg