.syntax unified
	.thumb
	.set sub_080432a4, 0x080432a4
	.set sub_0804d0dc, 0x0804d0dc
	.set sub_0804d118, 0x0804d118
	.set sub_0804d16c, 0x0804d16c
	.set sub_0804d38c, 0x0804d38c
	.set sub_0804d3e8, 0x0804d3e8
	.global Menu_SelectSaveSlotAction
	.global Func_0804d5ac
	.thumb_func
Menu_SelectSaveSlotAction:
Func_0804d5ac:
	push	{r5, r6, r7, lr}
	movs	r6, #0
	movs	r5, #0
	bl	sub_080432a4
	cmp	r0, #0
	bge.n	.L_0804d5c0
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0804d668
.L_0804d5c0:
	cmp	r0, #0
	bne.n	.L_0804d5c8
	movs	r0, #0
	b.n	.L_0804d668
.L_0804d5c8:
	cmp	r0, #3
	bne.n	.L_0804d5d0
	movs	r6, #1
	b.n	.L_0804d5e2
.L_0804d5d0:
	cmp	r0, #103
	bne.n	.L_0804d5d8
	movs	r6, #2
	b.n	.L_0804d5e2
.L_0804d5d8:
	cmp	r0, #100
	ble.n	.L_0804d5e0
	movs	r6, #3
	b.n	.L_0804d5e2
.L_0804d5e0:
	movs	r5, #1
.L_0804d5e2:
	bl	sub_0804d0dc
	cmp	r6, #0
	beq.n	.L_0804d5ee
	cmp	r6, #3
	bne.n	.L_0804d5f4
.L_0804d5ee:
	movs	r0, #21
	bl	sub_0804d38c
.L_0804d5f4:
	cmp	r6, #1
	bhi.n	.L_0804d5fe
	movs	r0, #22
	bl	sub_0804d38c
.L_0804d5fe:
	cmp	r6, #0
	beq.n	.L_0804d606
	cmp	r6, #3
	bne.n	.L_0804d60c
.L_0804d606:
	movs	r0, #23
	bl	sub_0804d38c
.L_0804d60c:
	movs	r0, #24
	bl	sub_0804d38c
	ldr	r7, [pc, #88]
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_0804d622
	movs	r0, #29
	bl	sub_0804d38c
.L_0804d622:
	ldr	r3, [pc, #76]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0804d632
	movs	r0, #31
	bl	sub_0804d38c
.L_0804d632:
	movs	r0, #17
	movs	r1, #7
	movs	r2, #0
	bl	sub_0804d3e8
	adds	r0, r5, #0
	bl	sub_0804d16c
	adds	r5, r0, #0
	bl	sub_0804d118
	cmp	r5, #0
	blt.n	.L_0804d658
	lsls	r3, r6, #1
	ldr	r2, [pc, #36]
	adds	r3, r3, r6
	lsls	r3, r3, #1
	adds	r3, r5, r3
	ldrsb	r5, [r2, r3]
.L_0804d658:
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	bne.n	.L_0804d666
	cmp	r5, #4
	bne.n	.L_0804d666
	movs	r5, #5
.L_0804d666:
	adds	r0, r5, #0
.L_0804d668:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x020036d8
	.4byte 0x02005350
	.4byte 0x0805f8bf
