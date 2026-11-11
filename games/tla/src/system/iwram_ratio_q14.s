@ Why assembly: both smull halves are consumed from nonconsecutive r3/r0;
@ compiler DI multiply patterns require consecutive halves, highpart
@ patterns discard the low half. Its Thumb entry's bx pc switches to the ARM code that follows, a mode switch inside a function;
@ the halfword after bx pc is never run.
.syntax unified
	.thumb
	.global IwramRatioMulQ14
IwramRatioMulQ14:
	bx	pc
AlchemyUncredited_080007ea:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080007ea:
	push	{r1, lr}
	mov	r1, r0
	mov	r0, #0x40000000
	bl	IwramDivide
	pop	{r1, lr}
	smull	r3, r0, r1, r0
	lsl	r0, r0, #18
	orr	r0, r0, r3, lsr #14
	bx	lr
