.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.global Func_080fad48
	.thumb_func
Func_080fad48:
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	bl	sub_08016ca4
	movs	r7, #128
	lsls	r7, r7, #1
	movs	r4, #248
	movs	r5, #0
	adds	r7, #255
	lsls	r4, r4, #8
	adds	r0, #216
	movs	r1, #14
.L_080fad60:
	ldrh	r2, [r0, #0]
	adds	r0, #2
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080fad7c
	adds	r3, r7, #0
	ands	r3, r2
	cmp	r3, r6
	bne.n	.L_080fad7c
	adds	r3, r4, #0
	ands	r3, r2
	lsrs	r3, r3, #11
	adds	r5, r5, r3
	adds	r5, #1
.L_080fad7c:
	subs	r1, #1
	cmp	r1, #0
	bge.n	.L_080fad60
	adds	r0, r5, #0
	pop	{r5, r6, r7, pc}
	.align 2, 0
