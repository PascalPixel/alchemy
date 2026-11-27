@ Why assembly: svc 42 is a BIOS software interrupt.
.syntax unified
	.thumb
	.global BiosSoundGetJumpList
	.thumb_func
BiosSoundGetJumpList:
	svc	42
	bx	lr
