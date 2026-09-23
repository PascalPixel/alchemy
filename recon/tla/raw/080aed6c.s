.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080aec04, 0x080aec04
	.global Inventory_Add
	.global Func_080aed6c
	.thumb_func
Inventory_Add:
Func_080aed6c:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	bl	sub_08016ca4
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080aec04
	ldrb	r2, [r0, #3]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080aedda
	movs	r3, #216
	ldrh	r3, [r6, r3]
	movs	r2, #128
	lsls	r2, r2, #1
	eors	r3, r5
	adds	r2, #255
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080aedb2
	adds	r1, r2, #0
	adds	r2, r6, #0
	adds	r2, #216
.L_080aeda0:
	adds	r0, #1
	cmp	r0, #14
	bgt.n	.L_080aedb2
	adds	r2, #2
	ldrh	r3, [r2, #0]
	eors	r3, r5
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_080aeda0
.L_080aedb2:
	cmp	r0, #15
	beq.n	.L_080aedda
	lsls	r3, r0, #1
	adds	r4, r3, #0
	adds	r4, #216
	ldrh	r1, [r6, r4]
	lsrs	r3, r1, #11
	adds	r2, r3, #1
	cmp	r2, #29
	bhi.n	.L_080aedf2
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, #255
	ands	r3, r1
	lsls	r2, r2, #11
	orrs	r3, r2
	strh	r3, [r6, r4]
	b.n	.L_080aedf6
.L_080aedd6:
	strh	r5, [r6, r1]
	b.n	.L_080aedf6
.L_080aedda:
	adds	r2, r6, #0
	movs	r0, #0
	adds	r2, #216
	movs	r1, #216
.L_080aede2:
	ldrh	r3, [r2, #0]
	adds	r2, #2
	cmp	r3, #0
	beq.n	.L_080aedd6
	adds	r0, #1
	adds	r1, #2
	cmp	r0, #14
	ble.n	.L_080aede2
.L_080aedf2:
	movs	r0, #1
	negs	r0, r0
.L_080aedf6:
	pop	{r5, r6, pc}
