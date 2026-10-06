.syntax unified
	.arm

	.global IwramRatioMulQ14
IwramRatioMulQ14:
	stmfd	sp!, {lr}
	mov	r4, r1
	mov	r1, r0
	mov	r0, #0x40000000
	bl	IwramSignedDivide
	smull	r3, r0, r4, r0
	ldmfd	sp!, {lr}
	lsl	r0, r0, #18
	orr	r0, r0, r3, lsr #14
	bx	lr
