	AREA RESET,CODE,READONLY
START
	LDR R0,=0X40000000
	LDR R1,=0X40000005
	ADD R2,R1,R0
	END