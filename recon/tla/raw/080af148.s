.syntax unified
	.thumb
	.global Inventory_Remove
	.thumb_func
Inventory_Remove:
.L_080af148:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r0, #0
	bl	Owner_GetState
	lsls	r5, r5, #1
	adds	r5, #216
	ldrh	r3, [r0, r5]
	movs	r6, #1
	negs	r6, r6
	cmp	r3, #0
	beq.n	.L_080af1be
	movs	r2, #248
	lsls	r2, r2, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080af178
	ldr	r2, [pc, #8]
	movs	r6, #1
	adds	r3, r3, r2
	strh	r3, [r0, r5]
	b.n	.L_080af1be
	.2byte 0xf800
	.2byte 0xffff
.L_080af178:
	adds	r6, r0, #0
	adds	r6, #216
	strh	r2, [r0, r5]
	adds	r4, r6, #0
	movs	r5, #0
	adds	r1, r6, #0
	movs	r0, #14
.L_080af186:
	ldrh	r2, [r4, #0]
	adds	r4, #2
	lsls	r3, r2, #16
	cmp	r3, #0
	beq.n	.L_080af196
	strh	r2, [r1, #0]
	adds	r5, #1
	adds	r1, #2
.L_080af196:
	subs	r0, #1
	cmp	r0, #0
	bge.n	.L_080af186
	cmp	r5, #14
	bgt.n	.L_080af1bc
	lsls	r3, r5, #1
	ldr	r2, [pc, #20]
	adds	r0, r3, r6
	movs	r3, #15
	subs	r5, r3, r5
.L_080af1aa:
	subs	r5, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r5, #0
	bne.n	.L_080af1aa
	b.n	.L_080af1bc
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_080af1bc:
	movs	r6, #2
.L_080af1be:
	adds	r0, r7, #0
	bl	Owner_RecalculateStats
	adds	r0, r6, #0
	pop	{r5, r6, r7, pc}
