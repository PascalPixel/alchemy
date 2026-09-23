.syntax unified
	.thumb
	.set sub_08020090, 0x08020090
	.set sub_080200c0, 0x080200c0
	.set sub_080200c8, 0x080200c8
	.set sub_08020218, 0x08020218
	.global Func_080dc10c
	.thumb_func
Func_080dc10c:
	push	{r5, r6, lr}
	movs	r4, #192
	lsls	r4, r4, #18
	adds	r4, #224
	ldr	r6, [r4, #0]
	bl	sub_080200c0
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080dc15e
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080dc132
	bl	sub_080200c8
	movs	r0, #0
	b.n	.L_080dc160
.L_080dc132:
	ldr	r3, [r6, #16]
	adds	r2, r5, #0
	ldr	r3, [r3, #20]
	adds	r2, #85
	str	r3, [r5, #20]
	movs	r3, #4
	strb	r3, [r2, #0]
	subs	r2, #50
	strb	r3, [r2, #0]
	subs	r3, #17
	ldr	r1, [r5, #80]
	adds	r0, r5, #0
	ldrb	r2, [r1, #9]
	ands	r3, r2
	strb	r3, [r1, #9]
	movs	r1, #0
	bl	sub_08020218
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08020090
.L_080dc15e:
	adds	r0, r5, #0
.L_080dc160:
	pop	{r5, r6, pc}
	.align 2, 0
