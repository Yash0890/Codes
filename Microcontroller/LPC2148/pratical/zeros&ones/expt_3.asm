; ZEROS AND ONES
	AREA RESET,CODE,READONLY
START
	MOV R0,#0X42	; VALUE
	MOV R1,#0X00	; ONES COUNTER
	MOV R2,#0X00	; ZEROS COUNTER
	MOV R7,#0X08	; LOOP COUNTER

LOOP
	RORS R0,#0X01
	BCC ZERO
	ADD R1,R1,#0X01
	B SKIP
	
ZERO
	ADD R2,R2,#0X01

SKIP
	SUB R7,R7,#0X01
	CMP R7,#0X00
	BNE LOOP

WAIT B WAIT
	END