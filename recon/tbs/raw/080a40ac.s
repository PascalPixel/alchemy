.syntax unified
	.thumb
	.set sub_08077008, 0x08077008
	.set sub_08077058, 0x08077058
	.global Unnamed_080a40ac
	.global Func_080a40ac
	.thumb_func
Unnamed_080a40ac:
Func_080a40ac:
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	bl	sub_08077008
	movs	r3, #216
	ldrh	r3, [r0, r3]
	movs	r1, #0
	movs	r6, #0
	adds	r0, #216
	b.n	.L_080a4102
.L_080a40c0:
	ldrh	r2, [r0, #0]
	ldr	r3, [pc, #40]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080a40f8
	lsrs	r3, r2, #11
	adds	r2, r3, #1
	cmp	r3, #0
	bne.n	.L_080a40d4
	movs	r2, #1
.L_080a40d4:
	cmp	r2, #0
	beq.n	.L_080a40f0
	adds	r5, r2, #0
.L_080a40da:
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	sub_08077058
	subs	r5, #1
	adds	r1, r0, #0
	cmp	r5, #0
	bne.n	.L_080a40da
	b.n	.L_080a40f0
	.2byte 0x0200
	.2byte 0x0000
.L_080a40f0:
	movs	r0, #0
	cmp	r1, #2
	bne.n	.L_080a410a
	b.n	.L_080a4106
.L_080a40f8:
	adds	r6, #1
	adds	r0, #2
	cmp	r6, #14
	bgt.n	.L_080a4108
	ldrh	r3, [r0, #0]
.L_080a4102:
	cmp	r3, #0
	bne.n	.L_080a40c0
.L_080a4106:
	movs	r1, #1
.L_080a4108:
	adds	r0, r1, #0
.L_080a410a:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
