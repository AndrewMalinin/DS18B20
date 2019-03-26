; Created: 28.01.2019 0:33:09
; Author : Андрей

.device	ATmega48

.include "m48def.inc"	
.include "macro.inc"	
	

;********* ВЕКТОР ПРЕРЫВАНИЙ *************
.cseg						
							
.org	0x00				
	rjmp	RESET			
							
.org	OVF1addr			
	rjmp	OV1_TIMER		

.org	OC1Aaddr
	rjmp	OC1A_TIMER

.org	ADCCaddr
	rjmp	ADC_complete

.org	INT_VECTORS_SIZE

;*****************************************


RESET:
;============ Инициализация программного стека =================
	ldi		tmpL, low(RAMEND)		; Загружаем указатель на последний байт
	ldi		tmpH, high(RAMEND)		; оперативной памяти в регистры
	out		SPL, tmpL				; Установить стек	
	out		SPH, tmpH				; если SRAM больше 256 байт

;=================== Инициализация периферии =================
	rcall	init_prt				; Инициализация портов
	rcall	init_TC1				; Инициализация таймера 1
	rcall	init_ADC				; Инициализация АЦП
	rcall	init_USART				; Инициализация USART
	rcall	init_vars				; Инициализация значений переменных

	rcall 	init_disp				; Инициализация Дисплея
	rcall	buff_clr				; Очистка буфера Дисплея
	rcall	about					; Вызов заставки
	rcall	init_DS18B20
	
	sei								; Разрешить прерывания
	
main:
	rcall	OneWire_reset_impulse
	rcall	OneWire_presense_impulse
	
	rcall	DS18B20_skip_ROM

	rcall	DS18B20_convertT


	sti8	ram_delay, tmpL, 255	;| 
	rcall	delay_ms		
	sti8	ram_delay, tmpL, 255	;| 
	rcall	delay_ms		
	sti8	ram_delay, tmpL, 255	;| 
	rcall	delay_ms		


	rcall	OneWire_reset_impulse
	rcall	OneWire_presense_impulse
	
	rcall	DS18B20_skip_ROM
	rcall	DS18B20_read_scratch

	rcall	DS18B20_read_temperature
//	rcall	OneWire_reset_impulse
	rcall	convert_temperature
	rcall	buff_clr
	rcall	temperatute_to_buff

	rcall	buff_to_disp

		
	rcall	uart_send_byte
	inc		Button

rjmp	main


.cseg
string:	.db "   Andrew AVR   "
		.db " Project v.2.1  ",0x00,0x00



.include "define.inc"	
.include "init.inc"	
.include "adc.inc"	
.include "delay.inc"	
.include "timers.inc"	
.include "display.inc"	
.include "18B20.inc"
.include "function.inc"	
.include "uart.inc"	
