.syntax unified
	.thumb
	.global IwramMulQ16
IwramMulQ16:
	bx	pc
AlchemyUncredited_080007d6:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080007d6:
	smull	r2, r0, r1, r0
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	bx	lr
