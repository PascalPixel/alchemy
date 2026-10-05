.syntax unified
	.arm

	.global IwramMulQ16
IwramMulQ16:
	smull	r2, r0, r1, r0
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	bx	lr
