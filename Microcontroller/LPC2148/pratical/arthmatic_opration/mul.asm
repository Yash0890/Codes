	AREA RESET,CODE,READONLY
START
	LDR R0,=0X4000
	LDR R1,=0X4400
	MUL R2,R1,R0
	END