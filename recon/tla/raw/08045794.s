.syntax unified
	.thumb
	.global Func_08045794
	.thumb_func
Func_08045794:
	push	{lr}
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r3, #0
	adds	r2, #18
	strh	r3, [r2, #0]
	ldr	r2, [pc, #8]
	movs	r0, #2
	movs	r1, #136
	bl	0x08013438
	pop	{pc}
	.2byte 0x5781
	.2byte 0x0804
	push	{r5, lr}
	movs	r3, #1
	bl	0x0803a69c
	adds	r5, r0, #0
	b.n	.L_080457c2
.L_080457bc:
	movs	r0, #1
	bl	WaitFrames
.L_080457c2:
	bl	0x0803a3b8
	cmp	r0, #0
	beq.n	.L_080457bc
	adds	r0, r5, #0
	pop	{r5, pc}
	.2byte 0x0000
