.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080aec04, 0x080aec04
	.set sub_080aee98, 0x080aee98
	.set sub_080aeec8, 0x080aeec8
	.set sub_080af148, 0x080af148
	.set sub_080af1c8, 0x080af1c8
	.set sub_080af43c, 0x080af43c
	.global Func_080af298
	.thumb_func
Func_080af298:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	sub_080aeec8
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	movs	r0, #0
	cmp	r5, r3
	beq.n	.L_080af2be
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	sub_080aee98
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080af148
	movs	r0, #0
.L_080af2be:
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	sub_080aeec8
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	movs	r0, #0
	cmp	r5, r3
	beq.n	.L_080af2e6
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	sub_080aee98
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080af1c8
	movs	r0, #0
.L_080af2e6:
	pop	{r5, r6, pc}
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_08016ca4
	lsls	r5, r5, #1
	adds	r5, #216
	ldrh	r2, [r0, r5]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_080af302
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080af30a
.L_080af302:
	ldr	r3, [pc, #8]
	orrs	r3, r2
	strh	r3, [r0, r5]
	movs	r0, #0
.L_080af30a:
	pop	{r5, pc}
	.2byte 0x0400
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_08016ca4
	lsls	r5, r5, #1
	adds	r5, #216
	ldrh	r2, [r0, r5]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_080af32a
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080af336
.L_080af32a:
	movs	r3, #251
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r0, r5]
	movs	r0, #0
.L_080af336:
	pop	{r5, pc}
	push	{lr}
	bl	sub_080aec04
	ldrh	r0, [r0, #40]
	bl	sub_080af43c
	ldrb	r0, [r0, #0]
	pop	{pc}
.L_080af348:
	push	{lr}
	movs	r3, #150
	adds	r2, r0, #0
	lsls	r3, r3, #1
	ldr	r4, [pc, #32]
	movs	r0, #0
	cmp	r2, r3
	bge.n	.L_080af372
	ldrb	r3, [r4, r2]
	adds	r3, r3, r1
	cmp	r3, #0
	bge.n	.L_080af364
	movs	r3, #0
	b.n	.L_080af370
.L_080af364:
	cmp	r3, #99
	ble.n	.L_080af36e
	movs	r3, #99
	movs	r0, #99
	b.n	.L_080af370
.L_080af36e:
	adds	r0, r3, #0
.L_080af370:
	strb	r3, [r4, r2]
.L_080af372:
	pop	{pc}
	.2byte 0x208c
	.2byte 0x0200
	push	{lr}
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ldr	r2, [pc, #24]
	ands	r3, r0
	lsls	r3, r3, #1
	ldrh	r0, [r2, r3]
	movs	r4, #0
	cmp	r0, #0
	beq.n	.L_080af396
	subs	r0, #1
	bl	.L_080af348
	adds	r4, r0, #0
.L_080af396:
	adds	r0, r4, #0
	pop	{pc}
	movs	r0, r0
	.2byte 0x1f40
	.2byte 0x080b
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r1, #0
	sub	sp, #4
	bl	sub_08016ca4
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	mov	r8, r0
	movs	r1, #0
	ands	r6, r3
	movs	r7, #0
	movs	r5, #216
	mov	sl, r3
.L_080af3c2:
	mov	r2, r8
	ldrh	r3, [r5, r2]
	mov	r2, sl
	ands	r3, r2
	cmp	r3, r6
	bne.n	.L_080af3ee
	adds	r0, r6, #0
	str	r1, [sp, #0]
	bl	sub_080aec04
	ldrb	r2, [r0, #3]
	movs	r3, #16
	ands	r3, r2
	ldr	r1, [sp, #0]
	cmp	r3, #0
	beq.n	.L_080af3ec
	mov	r2, r8
	ldrh	r3, [r5, r2]
	lsrs	r3, r3, #11
	adds	r1, r3, #1
	b.n	.L_080af3f6
.L_080af3ec:
	adds	r1, #1
.L_080af3ee:
	adds	r7, #1
	adds	r5, #2
	cmp	r7, #14
	ble.n	.L_080af3c2
.L_080af3f6:
	adds	r0, r1, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.align 2, 0
