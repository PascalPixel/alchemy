.syntax unified
	.thumb
	.set sub_080142d4, 0x080142d4
	.global Func_080143ac
	.thumb_func
Func_080143ac:
.L_080143ac:
	push	{lr}
	ldr	r1, [pc, #44]
	movs	r4, #255
	ldrh	r3, [r1, #2]
	lsls	r4, r4, #8
	adds	r4, #255
	movs	r0, #96
	movs	r2, #0
	cmp	r3, r4
	bne.n	.L_080143c4
	movs	r0, #0
	b.n	.L_080143da
.L_080143c4:
	adds	r2, #1
	adds	r1, #4
	cmp	r2, #95
	bgt.n	.L_080143da
	movs	r4, #255
	ldrh	r3, [r1, #2]
	lsls	r4, r4, #8
	adds	r4, #255
	cmp	r3, r4
	bne.n	.L_080143c4
	adds	r0, r2, #0
.L_080143da:
	pop	{pc}
	.2byte 0x36e0
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	.L_080143ac
	adds	r1, r6, #0
	movs	r2, #0
	adds	r5, r0, #0
	bl	sub_080142d4
	adds	r0, r5, #0
	pop	{r5, r6, pc}
	.align 2, 0
