.syntax unified
	.thumb
	.set sub_08014878, 0x08014878
	.set sub_08016ca4, 0x08016ca4
	.set sub_0810a70c, 0x0810a70c
	.set sub_0810a748, 0x0810a748
	.global Func_0810a760
	.thumb_func
Func_0810a760:
	push	{r5, r6, r7, lr}
	bl	sub_0810a70c
	adds	r5, r0, #0
	movs	r6, #0
	movs	r7, #0
	cmp	r5, #0
	beq.n	.L_0810a7d8
	movs	r2, #4
	ldrsh	r3, [r5, r2]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_0810a794
	adds	r2, r5, #4
	movs	r1, #20
.L_0810a77e:
	ldrsh	r3, [r1, r5]
	adds	r0, #1
	adds	r6, r6, r3
	adds	r1, #2
	cmp	r0, #7
	bgt.n	.L_0810a794
	adds	r2, #2
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	cmp	r3, #0
	bne.n	.L_0810a77e
.L_0810a794:
	bl	sub_08014878
	adds	r3, r6, #0
	muls	r3, r0
	lsrs	r1, r3, #16
	movs	r2, #4
	ldrsh	r3, [r5, r2]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_0810a7ce
	movs	r4, #20
	ldrsh	r3, [r5, r4]
	subs	r1, r1, r3
	cmp	r1, #0
	blt.n	.L_0810a7ce
	adds	r2, r5, #4
.L_0810a7b4:
	adds	r0, #1
	adds	r2, #2
	cmp	r0, #7
	bgt.n	.L_0810a7ce
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	cmp	r3, #0
	beq.n	.L_0810a7ce
	movs	r4, #16
	ldrsh	r3, [r2, r4]
	subs	r1, r1, r3
	cmp	r1, #0
	bge.n	.L_0810a7b4
.L_0810a7ce:
	cmp	r0, #8
	beq.n	.L_0810a7d8
	lsls	r3, r0, #1
	adds	r3, #4
	ldrsh	r7, [r5, r3]
.L_0810a7d8:
	adds	r0, r7, #0
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	bl	sub_08016ca4
	adds	r5, r0, #0
	movs	r6, #0
	adds	r5, #216
.L_0810a7e8:
	ldrh	r0, [r5, #0]
	adds	r5, #2
	bl	sub_0810a748
	cmp	r0, #0
	beq.n	.L_0810a7f8
	movs	r0, #1
	b.n	.L_0810a800
.L_0810a7f8:
	adds	r6, #1
	cmp	r6, #14
	ble.n	.L_0810a7e8
	movs	r0, #0
.L_0810a800:
	pop	{r5, r6, pc}
	.2byte 0x0000
