.syntax unified
	.arm
	.global IwramHalt
IwramHalt:
.L_030007ac:
	svc	0x00030000
	b	.L_030007ac
