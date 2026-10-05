.syntax unified
	.thumb
	.global IwramRatioMulQ14
IwramRatioMulQ14:
AlchemyUncredited_080007e8:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080007e8:
	push	{r1, lr}
	mov	r1, r0
	mov	r0, #0x40000000
	bl	IwramDivide
	pop	{r1, lr}
	smull	r3, r0, r1, r0
	lsl	r0, r0, #18
	orr	r0, r0, r3, lsr #14
	bx	lr
