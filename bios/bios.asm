cpu 	8086
bits 	16

	section .boot
boot:
	; 8086 instructions 00 - FF
	; Used to test 8086 emulator instruction decodinge n

	; 00 - ADD Eb Gb
	ADD		AH,		BH

	; 01 - ADD Ev Gv
	ADD		AX,		BX

	; 02 - ADD Gb Eb
	ADD		BH, 	[0xffff]

	; 03 - ADD Gv Ev
	ADD		BX, 	[0xffff]

	; 04 - ADD AL Ib
	ADD		AL,		3

	; 05 - ADD AL Iv
	ADD		AX,		256

	; 06 - PUSH ES
	PUSH 	ES

	; 07 - POP ES
	POP		ES

	; 08 - OR Eb Gb
	OR		AH,		BH

	; 09 - OR Ev Gv
	OR		AX,		BX

	; 0A - OR Gb Eb
	OR		BH, 	[0xffff]

	; 0B - OR Gv Ev
	OR		BX, 	[0xffff]

	; 0C - OR AL Ib
	OR		AL,		3

	; 0D - OR AL Iv
	OR		AX,		256

	; 0E - PUSH CS
	PUSH 	CS

	; 0F - POP CS
	POP		CS

	; 10 - ADC Eb Gb
	ADC		AH,		BH

	; 11 - ADC Ev Gv
	ADC		AX,		BX

	; 12 - ADC Gb Eb
	ADC		BH, 	[0xffff]

	; 13 - ADC Gv Ev
	ADC		BX, 	[0xffff]

	; 14 - ADC AL Ib
	ADC		AL,		3

	; 15 - ADC AL Iv
	ADC		AX,		256

	; 16 - PUSH SS
	PUSH 	SS

	; 17 - POP SS
	POP		SS

	; 18 - SBB Eb Gb
	SBB		AH,		BH

	; 19 - SBB Ev Gv
	SBB		AX,		BX

	; 1A - SBB Gb Eb
	SBB		BH, 	[0xffff]

	; 1B - SBB Gv Ev
	SBB		BX, 	[0xffff]

	; 1C - SBB AL Ib
	SBB		AL,		3

	; 1D - SBB AL Iv
	SBB		AX,		256

	; 1E - PUSH DS
	PUSH 	DS

	; 1F - POP DS
	POP		DS

	; 20 - AND Eb Gb
	AND		AH,		BH

	; 21 - AND Ev Gv
	AND		AX,		BX

	; 22 - AND Gb Eb
	AND		BH, 	[0xffff]

	; 23 - AND Gv Ev
	AND		BX, 	[0xffff]

	; 24 - AND AL Ib
	AND		AL,		3

	; 25 - AND AL Iv
	AND		AX,		256

	; 27 - DAA
	DAA

	; 28 - SUB Eb Gb
	SUB		AH,		BH

	; 29 - SUB Ev Gv
	SUB		AX,		BX

	; 2A - SUB Gb Eb
	SUB		BH, 	[0xffff]

	; 2B - SUB Gv Ev
	SUB		BX, 	[0xffff]

	; 2C - SUB AL Ib
	SUB		AL,		3

	; 2D - SUB AL Iv
	SUB		AX,		256

	; 2F - DAS
	DAS

	; 30 - XOR Eb Gb
	XOR		AH,		BH

	; 31 - XOR Ev Gv
	XOR		AX,		BX

	; 32 - XOR Gb Eb
	XOR		BH, 	[0xffff]

	; 33 - XOR Gv Ev
	XOR		BX, 	[0xffff]

	; 34 - XOR AL Ib
	XOR		AL,		3

	; 35 - XOR AL Iv
	XOR		AX,		256

	; 37 - AAA
	AAA

	; 38 - CMP Eb Gb
	CMP		AH,		BH

	; 39 - CMP Ev Gv
	CMP		AX,		BX

	; 3A - CMP Gb Eb
	CMP		BH, 	[0xffff]

	; 3B - CMP Gv Ev
	CMP		BX, 	[0xffff]

	; 3C - CMP AL Ib
	CMP		AL,		3

	; 3D - CMP AL Iv
	CMP		AX,		256

	; 3F - AAS
	AAS

	; 40 - INC AX
	INC		AX

	; 41 - INC CX
	INC		CX

	; 42 - INC DX
	INC		DX

	; 43 - INC BX
	INC		BX

	; 44 - INC SP
	INC		SP

	; 45 - INC BP
	INC		BP

	; 46 - INC SI
	INC		SI

	; 47 - INC DI
	INC		DI

	; 48 - DEC AX
	DEC		AX

	; 49 - DEC CX
	DEC		CX

	; 4A - DEC DX
	DEC		DX

	; 4B - DEC BX
	DEC		BX

	; 4C - DEC SP
	DEC		SP

	; 4D - DEC BP
	DEC		BP

	; 4E - DEC SI
	DEC		SI

	; 4F - DEC DI
	DEC		DI

	; 50 - PUSH AX
	PUSH	AX

	; 51 - PUSH CX
	PUSH	CX

	; 52 - PUSH DX
	PUSH	DX

	; 53 - PUSH BX
	PUSH	BX

	; 54 - PUSH SP
	PUSH	SP

	; 55 - PUSH BP
	PUSH	BP

	; 56 - PUSH SI
	PUSH	SI

	; 57 - PUSH DI
	PUSH	DI

	; 58 - POP AX
	POP		AX

	; 59 - POP CX
	POP		CX

	; 5A - POP DX
	POP		DX

	; 5B - POP BX
	POP		BX

	; 5C - POP SP
	POP		SP

	; 5D - POP BP
	POP		BP

	; 5E - POP SI
	POP		SI

	; 5F - POP DI
	POP		DI

	; 70 - JO Jbs
	JO		location

	; 71 - JNO Jbs
	JNO		location

	; 72 - JB Jbs
	JB		location

	; 73 - JNB Jbs
	JNB		location

	; 74 - JZ Jbs
	JZ		location

	; 75 - JNZ Jbs
	JNZ		location

	; 76 - JBE Jbs
	JBE		location

	; 77 - JNBE Jbs
	JNBE	location

	; 78 - JS Jbs
	JS		location

	; 79 - JNS Jbs
	JNS		location

	; 7A - JP Jbs
	JP		location

	; 7B - JNP Jbs
	JNP		location

	; 7C - JL Jbs
	JL		location

	; 7D - JNL Jbs
	JNL		location

	; 7E - JLE Jbs
	JLE		location

	; 7F - JNLE Jbs
	JNLE	location

	location:

	; 80 - ADD Eb Ib
	; 80 - OR Eb Ib
	; 80 - ADC Eb Ib
	; 80 - SBB Eb Ib
	; 80 - AND Eb Ib
	; 80 - SUB Eb Ib
	; 80 - XOR Eb Ib
	; 80 - CMP Eb Ib
	ADD		AH,		1
	OR		AH,		1
	ADC		AH,		1
	SBB		AH,		1
	AND		AH,		1
	SUB		AH,		1
	XOR		AH,		1
	CMP		AH,		1

	; 81 - ADD Ev Iv
	; 81 - OR Ev Iv
	; 81 - ADC Ev Iv
	; 81 - SBB Ev Iv
	; 81 - AND Ev Iv
	; 81 - SUB Ev Iv
	; 81 - XOR Ev Iv
	; 81 - CMP Ev Iv
	ADD		BX,		256
	OR		BX,		256
	ADC		BX,		256
	SBB		BX,		256
	AND		BX,		256
	SUB		BX,		256
	XOR		BX,		256
	CMP		BX,		256

	; Alias of 80
	; 82 - ADD Eb Ib
	; 82 - OR Eb Ib
	; 82 - ADC Eb Ib
	; 82 - SBB Eb Ib
	; 82 - AND Eb Ib
	; 82 - SUB Eb Ib
	; 82 - XOR Eb Ib
	; 82 - CMP Eb Ib
	db 0x82, 0xC4, 0x1 
	db 0x82, 0xCC, 0x1 
	db 0x82, 0xD4, 0x1 
	db 0x82, 0xDC, 0x1 
	db 0x82, 0xE4, 0x1 
	db 0x82, 0xEC, 0x1 
	db 0x82, 0xF4, 0x1 
	db 0x82, 0xFC, 0x1

	; 83 - ADD Ev Ibs
	; 83 - OR Ev Ibs
	; 83 - ADC Ev Ibs
	; 83 - SBB Ev Ibs
	; 83 - AND Ev Ibs
	; 83 - SUB Ev Ibs
	; 83 - XOR Ev Ibs
	; 83 - CMP Ev Ibs
	ADD		BX,		1
	;OR		BX,		1
	ADC		BX,		1
	SBB		BX,		1
	;AND		BX,		1
	SUB		BX,		1
	;XOR		BX,		1
	CMP		BX,		1

	; 84 - TEST Eb Gb
	TEST	[0xffff],	AH

	; 85 - TEST Ev Gv
	TEST	[0xffff],	AX

	; 86 - XCHG Eb Gb
	XCHG	[0xffff],	AH

	; 87 - XCHG Ev Gv
	XCHG	[0xffff],	AX

	; 88 - MOV Eb Gb
	MOV		[0xffff],	AH

	; 89 - MOV Ev Gv
	MOV		[0xffff],	BX

	; 8A - MOV Gb Eb
	MOV		AH,			[0xffff]

	; 8B - MOV Gv Ev
	MOV		BX,			[0xffff]

	; 8C - MOV Mw Sw
	MOV		[0xffff],	CS

	; 8D - LEA Gv M
	LEA		SI,			[0xffff]

	; 8E - MOV Sw Ew
	MOV		CS,			AX

	; 8F - POP Ev
	POP	WORD [0xffff]

	; 90 - XCHG AX AX
	XCHG	AX,			AX

	; 91 - XCHG CX AX
	XCHG	CX,			AX

	; 92 - XCHG DX AX
	XCHG	DX,			AX

	; 93 - XCHG BX AX
	XCHG	BX,			AX

	; 94 - XCHG SP AX
	XCHG	SP,			AX

	; 95 - XCHG BP AX
	XCHG	BP,			AX

	; 96 - XCHG SI AX
	XCHG	SI,			AX

	; 97 - XCHG DI AX
	XCHG	DI,			AX

	; 98 - CBW AX AL
	CBW

	; 99 - DX AX
	CWD

	; 9A - CALLF Ap
	CALL	0xf000:0

	; 9B - WAIT
	WAIT

	; 9C - PUSHF
	PUSHF

	; 9D - POPF
	POPF

	; 9E - SAHF
	SAHF

	; 9F - LAHF
	LAHF

	; A0 - MOV AL Ob
	MOV		AL, 		[0xffff]

	; A1 - MOV AX Ob
	MOV		AX, 		[0xffff]

	; A2 - MOV Ob AL
	MOV		[0xffff], 	AL

	; A3 - MOV Ob AX
	MOV		[0xffff], 	AX

	; A4 - MOVSB
	MOVSB

	; A5 - MOVSW
	MOVSW

	; A6 - CMPSB
	CMPSB

	; A7 - CMPSW
	CMPSW

	; A8 - TEST AL Ib
	TEST	AL,			1

	; A9 - TEST AX Iv
	TEST	AX,			1

	; AA - STOSB
	STOSB

	; AB - STOSW
	STOSW

	; AC - LODSB
	LODSB

	; AD - LODSW
	LODSW

	; AE - SCASB
	SCASB

	; AF - SCASW
	SCASW

	; B0 - MOV AL Ib
	MOV		AL,			1

	; B1 - MOV CL Ib
	MOV		CL,			2

	; B2 - MOV DL Ib
	MOV		DL,			3

	; B3 - MOV BL Ib
	MOV		BL,			4

	; B4 - MOV AH Ib
	MOV		AH,			1

	; B5 - MOV CH Ib
	MOV		CH,			2

	; B6 - MOV DH Ib
	MOV		DH,			3

	; B7 - MOV BH Ib
	MOV		BH,			4

	; B8 - MOV AX Iv
	MOV		AX,			1

	; B9 - MOV CX Iv
	MOV		CX,			2

	; BA - MOV DX Iv
	MOV		DX,			3

	; BB - MOV BX Iv
	MOV		BX,			4

	; BC - MOV SP Iv
	MOV		SP,			5

	; BD - MOV BP Iv
	MOV		BP,			6

	; BE - MOV SI Iv
	MOV		SI,			7

	; BF - MOV DI Iv
	MOV		DI,			8

	; C2 - RETN
	RETN	1

	; C3 - RETN
	RETN

	; C4 - LES Gv Mp
	LES		DI,			[0xffff]

	; C5 - LDS Gv Mp
	LDS		DI,			[0xffff]

	; C6 - MOV Eb Ib
	MOV BYTE [0xffff],	1

	; C7 - MOV Ev Iv
	MOV WORD [0xffff],	1

	; CA - RETF Iw
	RETF	0xf000

	; CB - RETF
	RETF

	; CC - INT 3
	db		0xcc

	; CD - INT Ib
	INT		11

	; CE - INTO
	INTO

	; CF - IRET
	IRET

	; D0 - ROL Eb 1
	; D0 - ROR Eb 1
	; D0 - RCL Eb 1
	; D0 - RCR Eb 1
	; D0 - SHL Eb 1
	; D0 - SHR Eb 1
	; D0 - SAL Eb 1
	; D0 - SAR Eb 1
	ROL		AH,			1
	ROR		AH,			1
	RCL		AH,			1
	RCR		AH,			1
	SHL		AH,			1
	SHR		AH,			1
	SAL		AH,			1
	SAR		AH,			1

	; D1 - ROL Ev 1
	; D1 - ROR Ev 1
	; D1 - RCL Ev 1
	; D1 - RCR Ev 1
	; D1 - SHL Ev 1
	; D1 - SHR Ev 1
	; D1 - SAL Ev 1
	; D1 - SAR Ev 1
	ROL		AX,			1
	ROR		AX,			1
	RCL		AX,			1
	RCR		AX,			1
	SHL		AX,			1
	SHR		AX,			1
	SAL		AX,			1
	SAR		AX,			1

	; D2 - ROL Eb CL
	; D2 - ROR Eb CL
	; D2 - RCL Eb CL
	; D2 - RCR Eb CL
	; D2 - SHL Eb CL
	; D2 - SHR Eb CL
	; D2 - SAL Eb CL
	; D2 - SAR Eb CL
	ROL		AH,			CL
	ROR		AH,			CL
	RCL		AH,			CL
	RCR		AH,			CL
	SHL		AH,			CL
	SHR		AH,			CL
	SAL		AH,			CL
	SAR		AH,			CL

	; D3 - ROL Ev CL
	; D3 - ROR Ev CL
	; D3 - RCL Ev CL
	; D3 - RCR Ev CL
	; D3 - SHL Ev CL
	; D3 - SHR Ev CL
	; D3 - SAL Ev CL
	; D3 - SAR Ev CL
	ROL		AX,			CL
	ROR		AX,			CL
	RCL		AX,			CL
	RCR		AX,			CL
	SHL		AX,			CL
	SHR		AX,			CL
	SAL		AX,			CL
	SAR		AX,			CL

	; D4 - AAM
	AAM

	; D5 - AAD
	AAD

	; D7 - XLAT
	XLAT

	; E0 - LOOPNZ
	LOOPNZ	testlbl

	; E1 - LOOPZ
	LOOPZ	testlbl

	; E2 - LOOP
	LOOP	testlbl

	; E3 - JCXZ
	JCXZ	testlbl

	testlbl:

	; E4 - IN AL Ib
	IN		AL,			1

	; E5 - IN AX Ib
	IN		AX,			1

	; E6 - OUT Ib AL
	OUT		1,			AL

	; E7 - OUT Ib AX
	OUT		1,			AX

	; E8 - CALL Jv
	CALL 	testproc

	; E9 - JMP Jv
	JMP		256

	; EA - JMPF Ap
	jmp		0xf000:0

	; EB - JMP
	jmp		testproc

	testproc:

	; EC - IN AL DX
	IN 		AL, 		DX

	; ED - IN AX DX
	IN		AX,			DX

	; EE - OUT DX AL
	OUT		DX,			AL

	; EF - OUT DX AX
	OUT		DX,			AX

	; F4 - HLT
	HLT

	; F5 - CMC
	CMC

	; F6 - TEST Eb Ib
	; F6 - TEST Eb Ib
	; F6 - NOT Eb
	; F6 - NEG Eb
	; F6 - MUL Eb
	; F6 - IMUL Eb
	; F6 - DIV Eb
	; F6 - IDIV Eb
	TEST	AH,			1
	db		0xf6, 0xcc, 0x01
	NOT		AH
	NEG		AH
	MUL		AH
	IMUL	AH
	DIV		AH
	IDIV	AH

	; F7 - TEST Ev Iv
	; F7 - TEST Ev Iv
	; F7 - NOT Ev
	; F7 - NEG Ev
	; F7 - MUL Ev
	; F7 - IMUL Ev
	; F7 - DIV Ev
	; F7 - IDIV Ev
	TEST	BX,			1
	db		0xf7, 0xcb, 0x01
	NOT		AX
	NEG		AX
	MUL		AX
	IMUL	AX
	DIV		AX
	IDIV	AX

	; F8 - CLC
	CLC

	; F9 - STC
	STC

	; FA - CLI
	CLI

	; FB - STI
	STI

	; FC - CLD
	CLD

	; FD - STD
	STD

	; FE - INC Eb
	; FE - DEC Eb
	INC		AH
	DEC		AH

	; FF - INC Ev
	; FF - DEC Ev
	; FF - CALL Ev
	; FF - CALLF Mp
	; FF - JMP Ev
	; FF - JMPF Mp
	; FF - PUSH Ev
	INC 	 WORD [0xabcd]
	DEC 	 WORD [0xabcd]
	CALL 	 WORD [0xabcd]
	CALL FAR WORD [0xabcd]
	JMP 	 WORD [0xabcd]
	JMP FAR  WORD [0xabcd]
	PUSH 	 WORD [0xabcd]

	section .reset
reset:
	jmp		0xf000:0