@ Why assembly: the ARM loop uses svc 0x00030000, a software
@ interrupt, then branches back to the same svc instruction.
.syntax unified
	.arm
	.global Iwram_Halt
Iwram_Halt:
.L_030007ac:
	svc	0x00030000
	b	.L_030007ac
