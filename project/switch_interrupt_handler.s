switch_interrupt_handler:
	mov   #switch_sense, r15
	call    #r15
	cmp   	#SW1, r12
	jnz     SW2_PRESSED
	bic.b   pause
	bic.b   siren2
	bic.b   siren3
	mov   #1, siren1
	jmp     END_HANDLER
SW2_PRESSED:
	cmp   	#SW2, r12
	jnz     SW3_PRESSED
	bic.b   pause
	bic.b   siren1
	bic.b   siren3
	mov.b   #1, siren2
	jmp     END_HANDLER
SW3_PRESSED:
	cmp   	#SW3, r12
	jz      SW4_PRESSED
	bic.b  	pause
	bic.b   siren1
	bic.b   siren2
	mov.b   #1, siren3
	jmp     END_HANDLER
SW4_PRESSED:
	cmp   	#SW4, r12
	jnz     END_HANDLER
	bic.b   siren1
	bic.b   siren
	bic.b   siren3
	mov.b   #1, pause
END_HANDLER:
	RETI
