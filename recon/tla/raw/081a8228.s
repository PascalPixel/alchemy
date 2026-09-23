.syntax unified
	.thumb
	.set sub_081a7870, 0x081a7870
	.global Func_081a8228
	.thumb_func
Func_081a8228:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r4, [r3, #0]
	adds	r5, r0, #0
	cmp	r4, #0
	beq.n	.L_081a8262
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r1, #1
	adds	r3, r4, r1
	adds	r1, #1
	movs	r2, #0
	strb	r5, [r3, #0]
	adds	r3, r4, r1
	strb	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	movs	r2, #128
	adds	r1, r4, r3
	movs	r3, #224
	lsls	r2, r2, #3
	lsls	r3, r3, #5
	adds	r0, r4, r2
	adds	r2, r4, r3
	adds	r3, r5, #0
	bl	sub_081a7870
.L_081a8262:
	pop	{r5, pc}
	push	{lr}
	cmp	r0, #31
	ble.n	.L_081a826e
	movs	r0, #31
	b.n	.L_081a8274
.L_081a826e:
	cmp	r0, #0
	bge.n	.L_081a8274
	movs	r0, #0
.L_081a8274:
	pop	{pc}
	.align 2, 0
