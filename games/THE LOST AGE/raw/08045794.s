.syntax unified
	.thumb
	.set sub_08013438, 0x08013438
	.set sub_08013560, 0x08013560
	.set sub_0803a3b8, 0x0803a3b8
	.set sub_0803a69c, 0x0803a69c
	.global Overlay_08045794
Overlay_08045794:
	push	{lr}
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r3, #0
	adds	r2, #18
	strh	r3, [r2, #0]
	ldr	r2, [pc, #8]
	movs	r0, #2
	movs	r1, #136
	bl	sub_08013438
	pop	{pc}
	.2byte 0x5781
	.2byte 0x0804
	push	{r5, lr}
	movs	r3, #1
	bl	sub_0803a69c
	adds	r5, r0, #0
	b.n	.L_080457c2
.L_080457bc:
	movs	r0, #1
	bl	sub_08013560
.L_080457c2:
	bl	sub_0803a3b8
	cmp	r0, #0
	beq.n	.L_080457bc
	adds	r0, r5, #0
	pop	{r5, pc}
