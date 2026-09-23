.syntax unified
	.thumb
	.set sub_0801489c, 0x0801489c
	.set sub_080148e8, 0x080148e8
	.set sub_080149e0, 0x080149e0
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_080227b0, 0x080227b0
	.set sub_08022b04, 0x08022b04
	.set sub_08022bac, 0x08022bac
	.set sub_08023220, 0x08023220
	.set sub_080233a8, 0x080233a8
	.set sub_080233d0, 0x080233d0
	.set sub_08024738, 0x08024738
	.set sub_08024f20, 0x08024f20
	.set sub_0802d45c, 0x0802d45c
	.set sub_0802d87c, 0x0802d87c
	.set sub_0802d958, 0x0802d958
	.set sub_081c0010, 0x081c0010
	.global Func_0802632c
	.thumb_func
Func_0802632c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	adds	r2, r6, #0
	sub	sp, #96
	movs	r1, #0
	adds	r2, #85
	movs	r3, #4
	movs	r0, #2
	str	r1, [sp, #12]
	str	r0, [sp, #8]
	strb	r3, [r2, #0]
	ldr	r3, [pc, #824]
	ldr	r1, [pc, #824]
	movs	r2, #143
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026374
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r6, #48]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r6, #52]
	movs	r3, #5
	str	r3, [sp, #8]
	b.n	.L_08026380
.L_08026374:
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r6, #48]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r6, #52]
.L_08026380:
	ldr	r5, [pc, #776]
	ldr	r1, [pc, #780]
	ldr	r3, [r5, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
	movs	r2, #255
	lsls	r3, r3, #16
	lsls	r2, r2, #8
	lsrs	r1, r3, #16
	adds	r2, #255
	str	r3, [sp, #4]
	cmp	r1, r2
	bne.n	.L_080263aa
	ldr	r0, [sp, #12]
	movs	r3, #4
	orrs	r0, r3
	str	r0, [sp, #12]
	b.n	.L_08026632
.L_080263aa:
	add	r3, sp, #84
	mov	fp, r3
	ldr	r3, [r6, #8]
	mov	r0, fp
	movs	r2, #0
	str	r2, [sp, #12]
	str	r3, [r0, #0]
	mov	r2, fp
	ldr	r3, [r6, #12]
	str	r3, [r0, #4]
	ldr	r3, [r6, #16]
	str	r3, [r0, #8]
	movs	r0, #128
	lsls	r0, r0, #12
	bl	sub_0801489c
	ldr	r3, [pc, #712]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080263e0
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080263e0
	b.n	.L_08026632
.L_080263e0:
	adds	r0, r6, #0
	mov	r1, fp
	bl	sub_0802d958
	cmp	r0, #0
	bne.n	.L_08026492
	ldr	r3, [r6, #8]
	add	r5, sp, #72
	str	r3, [r5, #0]
	movs	r1, #128
	ldr	r3, [r6, #12]
	lsls	r1, r1, #12
	str	r3, [r5, #4]
	mov	r8, r1
	ldr	r3, [r6, #16]
	mov	r0, r8
	str	r3, [r5, #8]
	ldr	r2, [sp, #4]
	movs	r3, #128
	lsrs	r7, r2, #16
	lsls	r3, r3, #5
	adds	r1, r7, r3
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d958
	cmp	r0, #0
	bne.n	.L_08026492
	ldr	r3, [r6, #8]
	ldr	r0, [pc, #628]
	str	r3, [r5, #0]
	adds	r1, r7, r0
	ldr	r3, [r6, #12]
	mov	r0, r8
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d958
	cmp	r0, #0
	bne.n	.L_08026492
	ldr	r3, [r6, #8]
	movs	r2, #128
	str	r3, [r5, #0]
	lsls	r2, r2, #6
	ldr	r3, [r6, #12]
	adds	r1, r7, r2
	str	r3, [r5, #4]
	mov	r0, r8
	ldr	r3, [r6, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d958
	cmp	r0, #0
	bne.n	.L_08026492
	ldr	r3, [r6, #8]
	mov	r0, r8
	str	r3, [r5, #0]
	adds	r2, r5, #0
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	ldr	r3, [pc, #544]
	adds	r1, r7, r3
	bl	sub_0801489c
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_0802d958
	ldr	r1, [sp, #4]
	str	r1, [sp, #0]
	cmp	r0, #0
	bne.n	.L_08026492
	b.n	.L_080265c2
.L_08026492:
	ldr	r0, [sp, #4]
	movs	r2, #16
	movs	r1, #128
	lsrs	r3, r0, #16
	add	r2, sp
	lsls	r1, r1, #5
	mov	r9, r2
	adds	r2, r3, r1
	ldr	r1, [pc, #500]
	mov	r0, r9
	strh	r2, [r0, #0]
	adds	r2, r3, r1
	movs	r1, #128
	lsls	r1, r1, #6
	strh	r2, [r0, #2]
	adds	r2, r3, r1
	ldr	r1, [pc, #488]
	strh	r2, [r0, #4]
	adds	r2, r3, r1
	movs	r1, #192
	lsls	r1, r1, #6
	strh	r2, [r0, #6]
	adds	r2, r3, r1
	ldr	r1, [pc, #476]
	strh	r2, [r0, #8]
	adds	r3, r3, r1
	mov	r2, r9
	strh	r3, [r2, #10]
	movs	r3, #0
	mov	sl, r3
	mov	r8, fp
.L_080264d0:
	mov	r0, sl
	lsls	r3, r0, #1
	mov	r1, r9
	ldrsh	r2, [r1, r3]
	ldr	r3, [r6, #8]
	mov	r1, r8
	str	r3, [r1, #0]
	lsls	r2, r2, #16
	ldr	r3, [r6, #12]
	lsrs	r7, r2, #16
	str	r3, [r1, #4]
	movs	r0, #128
	ldr	r3, [r6, #16]
	lsls	r0, r0, #12
	str	r3, [r1, #8]
	str	r2, [sp, #0]
	adds	r1, r7, #0
	mov	r2, r8
	bl	sub_0801489c
	adds	r0, r6, #0
	mov	r1, r8
	bl	sub_0802d958
	cmp	r0, #0
	bne.n	.L_080265a2
	ldr	r3, [r6, #8]
	add	r5, sp, #72
	str	r3, [r5, #0]
	movs	r2, #128
	ldr	r3, [r6, #12]
	lsls	r2, r2, #5
	str	r3, [r5, #4]
	movs	r0, #128
	ldr	r3, [r6, #16]
	adds	r1, r7, r2
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d958
	cmp	r0, #0
	bne.n	.L_080265a2
	ldr	r3, [r6, #8]
	movs	r0, #128
	str	r3, [r5, #0]
	lsls	r0, r0, #12
	ldr	r3, [r6, #12]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	ldr	r3, [pc, #340]
	adds	r1, r7, r3
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d958
	cmp	r0, #0
	bne.n	.L_080265a2
	ldr	r3, [r6, #8]
	movs	r0, #128
	str	r3, [r5, #0]
	lsls	r0, r0, #6
	ldr	r3, [r6, #12]
	adds	r1, r7, r0
	str	r3, [r5, #4]
	movs	r0, #128
	ldr	r3, [r6, #16]
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d958
	cmp	r0, #0
	bne.n	.L_080265a2
	ldr	r3, [r6, #8]
	ldr	r2, [pc, #284]
	str	r3, [r5, #0]
	movs	r0, #128
	ldr	r3, [r6, #12]
	adds	r1, r7, r2
	str	r3, [r5, #4]
	lsls	r0, r0, #12
	ldr	r3, [r6, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d958
	cmp	r0, #0
	beq.n	.L_080265c2
.L_080265a2:
	movs	r3, #1
	add	sl, r3
	mov	r0, sl
	cmp	r0, #6
	blt.n	.L_080264d0
	ldr	r3, [r6, #8]
	mov	r1, fp
	str	r3, [r1, #0]
	ldr	r3, [r6, #12]
	str	r3, [r1, #4]
	ldr	r3, [r6, #16]
	str	r3, [r1, #8]
	ldr	r2, [sp, #12]
	movs	r3, #1
	orrs	r2, r3
	str	r2, [sp, #12]
.L_080265c2:
	add	r3, sp, #60
	mov	r8, r3
	ldr	r3, [r6, #8]
	mov	r0, r8
	str	r3, [r0, #0]
	ldr	r3, [r6, #12]
	str	r3, [r0, #4]
	ldr	r3, [r6, #16]
	str	r3, [r0, #8]
	ldr	r2, [sp, #0]
	movs	r0, #128
	lsrs	r1, r2, #16
	lsls	r0, r0, #11
	mov	r2, r8
	bl	sub_0801489c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r3, #63
	adds	r7, r5, #0
	mov	sl, r3
	adds	r7, #89
.L_080265f0:
	ldrh	r3, [r6, #32]
	subs	r1, r3, #2
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_08026622
	ldrb	r2, [r7, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026622
	cmp	r5, r6
	beq.n	.L_08026622
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	mov	r2, r8
	bl	.L_08026f80
	cmp	r0, #0
	blt.n	.L_08026622
	ldr	r0, [sp, #12]
	movs	r3, #2
	orrs	r0, r3
	str	r0, [sp, #12]
.L_08026622:
	movs	r1, #1
	negs	r1, r1
	add	sl, r1
	mov	r2, sl
	adds	r7, #128
	adds	r5, #128
	cmp	r2, #0
	bge.n	.L_080265f0
.L_08026632:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	cmp	r3, #0
	beq.n	.L_0802665c
	ldr	r0, [sp, #12]
	movs	r2, #3
	ands	r2, r0
	cmp	r2, #0
	beq.n	.L_08026654
	movs	r1, #194
	lsls	r1, r1, #1
	adds	r2, r3, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0802665c
.L_08026654:
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r3, r3, r0
	strh	r2, [r3, #0]
.L_0802665c:
	ldr	r1, [sp, #12]
	cmp	r1, #0
	beq.n	.L_080266a4
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	movs	r5, #9
	cmp	r3, #0
	bne.n	.L_0802667c
	movs	r5, #22
.L_0802667c:
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080233d0
	b.n	.L_080266ac
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x03001150
	.4byte 0x0802ec5c
	.4byte 0x03001238
	.4byte 0xfffff000
	.4byte 0xffffe000
	.2byte 0xd000
	.2byte 0xffff
.L_080266a4:
	.2byte 0x1c30
	ldr	r1, [sp, #8]
	bl	sub_080233d0
.L_080266ac:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L_080266f6
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #56]
	str	r3, [r6, #60]
	str	r3, [r6, #64]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080266e6
	ldr	r0, [sp, #4]
	ldrh	r1, [r6, #6]
	lsrs	r3, r0, #16
	subs	r3, r3, r1
	lsls	r3, r3, #16
	movs	r2, #128
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_080266da
	adds	r3, r2, #0
.L_080266da:
	ldr	r2, [pc, #212]
	cmp	r3, r2
	bge.n	.L_080266e2
	adds	r3, r2, #0
.L_080266e2:
	adds	r3, r1, r3
	strh	r3, [r6, #6]
.L_080266e6:
	adds	r3, r6, #0
	adds	r3, #100
	movs	r2, #0
	strh	r2, [r3, #0]
	adds	r2, r6, #0
	adds	r2, #102
	movs	r3, #2
	b.n	.L_08026714
.L_080266f6:
	add	r3, sp, #84
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	adds	r0, r6, #0
	ldr	r3, [r3, #8]
	bl	sub_08024738
	adds	r2, r6, #0
	adds	r2, #100
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	ldrh	r1, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08026716
	subs	r3, r1, #1
.L_08026714:
	strh	r3, [r2, #0]
.L_08026716:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r4, #0
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080267a2
	ldr	r1, [pc, #140]
	ldr	r0, [pc, #144]
	movs	r2, #141
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0802677c
	movs	r2, #140
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0802677c
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #54
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0802677c
	movs	r2, #142
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0802677c
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #58
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802677e
.L_0802677c:
	movs	r4, #1
.L_0802677e:
	cmp	r4, #0
	beq.n	.L_0802678a
	movs	r0, #113
	bl	sub_081c0010
	b.n	.L_080267a0
.L_0802678a:
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	bl	sub_0802d45c
	ldrh	r3, [r6, #4]
	str	r0, [r6, #20]
	adds	r3, #1
	strh	r3, [r6, #4]
.L_080267a0:
	movs	r0, #1
.L_080267a2:
	add	sp, #96
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfffff000
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100
	movs	r1, #0
	mov	r8, r0
	ldr	r3, [pc, #828]
	movs	r0, #2
	str	r1, [sp, #16]
	str	r1, [sp, #12]
	str	r0, [sp, #8]
	ldr	r1, [pc, #824]
	movs	r2, #143
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026802
	movs	r3, #128
	lsls	r3, r3, #9
	mov	r0, r8
	str	r3, [r0, #48]
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #204
	movs	r1, #5
	str	r3, [r0, #52]
	str	r1, [sp, #8]
	b.n	.L_08026812
.L_08026802:
	movs	r3, #128
	lsls	r3, r3, #9
	mov	r2, r8
	str	r3, [r2, #48]
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #204
	str	r3, [r2, #52]
.L_08026812:
	ldr	r3, [pc, #768]
	ldr	r1, [pc, #768]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
	movs	r2, #255
	lsls	r3, r3, #16
	lsls	r2, r2, #8
	lsrs	r1, r3, #16
	adds	r2, #255
	str	r3, [sp, #4]
	cmp	r1, r2
	bne.n	.L_0802683c
	ldr	r0, [sp, #16]
	movs	r3, #4
	orrs	r0, r3
	str	r0, [sp, #16]
	b.n	.L_08026ccc
.L_0802683c:
	movs	r2, #0
	str	r2, [sp, #16]
	movs	r3, #88
	mov	r0, r8
	add	r3, sp
	mov	fp, r3
	ldr	r3, [r0, #8]
	mov	r2, fp
	str	r3, [r2, #0]
	movs	r5, #128
	ldr	r3, [r0, #12]
	lsls	r5, r5, #12
	str	r3, [r2, #4]
	ldr	r3, [r0, #16]
	adds	r0, r5, #0
	str	r3, [r2, #8]
	bl	sub_0801489c
	mov	r3, r8
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026916
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #20]
	ldr	r4, [pc, #676]
	movs	r6, #0
	mov	r9, r6
	adds	r0, r5, #0
.L_0802687c:
	ldr	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_080268f4
	adds	r3, r7, #0
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080268f4
	cmp	r7, r8
	beq.n	.L_080268f4
	mov	r3, r8
	ldr	r2, [r3, #12]
	ldr	r3, [r7, #12]
	ldr	r1, [pc, #644]
	subs	r2, r2, r3
	adds	r3, r2, r1
	cmp	r3, #0
	bge.n	.L_080268aa
	movs	r3, #128
	lsls	r3, r3, #13
	subs	r3, r3, r2
.L_080268aa:
	cmp	r3, r4
	bgt.n	.L_080268f4
	mov	r2, r8
	ldr	r3, [r2, #8]
	ldr	r1, [r7, #8]
	subs	r3, r3, r1
	adds	r3, r3, r0
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_080268d2
	ldr	r3, [r2, #16]
	ldr	r2, [r7, #16]
	subs	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_080268d2
	movs	r6, #1
.L_080268d2:
	mov	r2, fp
	ldr	r3, [r2, #0]
	subs	r3, r3, r1
	adds	r3, r3, r0
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_080268f4
	ldr	r3, [r2, #8]
	ldr	r2, [r7, #16]
	movs	r1, #128
	subs	r3, r3, r2
	lsls	r1, r1, #12
	adds	r3, r3, r1
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_080268f4
	b.n	.L_08026b08
.L_080268f4:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r7, #128
	cmp	r3, #63
	ble.n	.L_0802687c
	cmp	r6, #0
	beq.n	.L_08026916
	mov	r0, r8
	mov	r1, fp
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08026916
	movs	r0, #0
	str	r0, [sp, #16]
	b.n	.L_08026ccc
.L_08026916:
	ldr	r3, [pc, #524]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802692e
	ldr	r3, [pc, #500]
	movs	r2, #128
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802692e
	b.n	.L_08026b08
.L_0802692e:
	mov	r0, r8
	mov	r1, fp
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080269e6
	mov	r2, r8
	ldr	r3, [r2, #8]
	add	r5, sp, #76
	str	r3, [r5, #0]
	movs	r7, #128
	ldr	r3, [r2, #12]
	movs	r0, #128
	str	r3, [r5, #4]
	lsls	r0, r0, #5
	ldr	r3, [r2, #16]
	lsls	r7, r7, #12
	str	r3, [r5, #8]
	ldr	r3, [sp, #4]
	adds	r2, r5, #0
	lsrs	r6, r3, #16
	adds	r1, r6, r0
	adds	r0, r7, #0
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080269e6
	mov	r1, r8
	ldr	r3, [r1, #8]
	ldr	r2, [pc, #436]
	str	r3, [r5, #0]
	adds	r0, r7, #0
	ldr	r3, [r1, #12]
	str	r3, [r5, #4]
	ldr	r3, [r1, #16]
	adds	r1, r6, r2
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080269e6
	mov	r0, r8
	ldr	r3, [r0, #8]
	movs	r2, #128
	str	r3, [r5, #0]
	lsls	r2, r2, #6
	ldr	r3, [r0, #12]
	adds	r1, r6, r2
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r0, #16]
	adds	r0, r7, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080269e6
	mov	r0, r8
	ldr	r3, [r0, #8]
	ldr	r2, [pc, #364]
	str	r3, [r5, #0]
	adds	r1, r6, r2
	ldr	r3, [r0, #12]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	adds	r0, r7, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	ldr	r3, [sp, #4]
	str	r3, [sp, #0]
	cmp	r0, #0
	bne.n	.L_080269e6
	b.n	.L_08026b4c
.L_080269e6:
	ldr	r1, [sp, #4]
	add	r0, sp, #20
	mov	sl, r0
	movs	r0, #128
	lsrs	r3, r1, #16
	lsls	r0, r0, #5
	adds	r2, r3, r0
	ldr	r0, [pc, #304]
	mov	r1, sl
	strh	r2, [r1, #0]
	adds	r2, r3, r0
	movs	r0, #128
	lsls	r0, r0, #6
	strh	r2, [r1, #2]
	adds	r2, r3, r0
	ldr	r0, [pc, #292]
	strh	r2, [r1, #4]
	adds	r2, r3, r0
	movs	r0, #192
	lsls	r0, r0, #6
	strh	r2, [r1, #6]
	adds	r2, r3, r0
	strh	r2, [r1, #8]
	ldr	r2, [pc, #280]
	mov	r0, sl
	adds	r3, r3, r2
	strh	r3, [r0, #10]
	movs	r1, #0
	mov	r9, r1
	mov	r7, fp
.L_08026a22:
	mov	r2, r9
	lsls	r3, r2, #1
	mov	r0, sl
	ldrsh	r2, [r0, r3]
	mov	r0, r8
	ldr	r3, [r0, #8]
	lsls	r2, r2, #16
	str	r3, [r7, #0]
	lsrs	r6, r2, #16
	ldr	r3, [r0, #12]
	adds	r1, r6, #0
	str	r3, [r7, #4]
	ldr	r3, [r0, #16]
	movs	r0, #128
	str	r3, [r7, #8]
	lsls	r0, r0, #12
	str	r2, [sp, #0]
	adds	r2, r7, #0
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r7, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08026afc
	mov	r1, r8
	ldr	r3, [r1, #8]
	add	r5, sp, #76
	str	r3, [r5, #0]
	movs	r2, #128
	ldr	r3, [r1, #12]
	lsls	r2, r2, #5
	str	r3, [r5, #4]
	movs	r0, #128
	ldr	r3, [r1, #16]
	lsls	r0, r0, #12
	adds	r1, r6, r2
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08026afc
	mov	r0, r8
	ldr	r3, [r0, #8]
	ldr	r2, [pc, #160]
	str	r3, [r5, #0]
	adds	r1, r6, r2
	ldr	r3, [r0, #12]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	movs	r0, #128
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08026afc
	mov	r0, r8
	ldr	r3, [r0, #8]
	movs	r2, #128
	str	r3, [r5, #0]
	lsls	r2, r2, #6
	ldr	r3, [r0, #12]
	adds	r1, r6, r2
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r0, #16]
	movs	r0, #128
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08026afc
	mov	r0, r8
	ldr	r3, [r0, #8]
	ldr	r2, [pc, #80]
	str	r3, [r5, #0]
	adds	r1, r6, r2
	ldr	r3, [r0, #12]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	movs	r0, #128
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	beq.n	.L_08026b4c
.L_08026afc:
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #6
	blt.n	.L_08026a22
	b.n	.L_08026b34
.L_08026b08:
	movs	r1, #0
	str	r1, [sp, #16]
	b.n	.L_08026ccc
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x03001150
	.4byte 0x0802ec5c
	.4byte 0x0007ffff
	.4byte 0xfff00000
	.4byte 0x03001238
	.4byte 0xfffff000
	.4byte 0xffffe000
	.2byte 0xd000
	.2byte 0xffff
.L_08026b34:
	.2byte 0x4642
	ldr	r3, [r2, #8]
	mov	r0, fp
	str	r3, [r0, #0]
	ldr	r3, [r2, #12]
	str	r3, [r0, #4]
	ldr	r3, [r2, #16]
	str	r3, [r0, #8]
	ldr	r1, [sp, #16]
	movs	r3, #1
	orrs	r1, r3
	str	r1, [sp, #16]
.L_08026b4c:
	mov	r0, r8
	ldr	r3, [r0, #8]
	add	r2, sp, #64
	str	r3, [r2, #0]
	mov	fp, r2
	ldr	r3, [r0, #12]
	str	r3, [r2, #4]
	ldr	r3, [r0, #16]
	movs	r0, #128
	str	r3, [r2, #8]
	ldr	r2, [sp, #0]
	lsls	r0, r0, #11
	lsrs	r1, r2, #16
	mov	r2, fp
	bl	sub_0801489c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #20]
	movs	r3, #63
	adds	r6, r7, #0
	mov	r9, r3
	adds	r6, #8
.L_08026b7a:
	mov	r0, r8
	ldrh	r3, [r0, #32]
	subs	r1, r3, #2
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bne.n	.L_08026b88
	b.n	.L_08026ca0
.L_08026b88:
	adds	r3, r7, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08026b98
	b.n	.L_08026ca0
.L_08026b98:
	cmp	r7, r8
	bne.n	.L_08026b9e
	b.n	.L_08026ca0
.L_08026b9e:
	ldrh	r3, [r6, #24]
	adds	r0, r6, #0
	subs	r3, #2
	mov	r2, fp
	bl	.L_08026f80
	cmp	r0, #0
	blt.n	.L_08026ca0
	ldr	r3, [r6, #80]
	ldr	r2, [pc, #528]
	movs	r1, #128
	ands	r3, r2
	lsls	r1, r1, #2
	cmp	r3, r1
	bne.n	.L_08026c98
	mov	r2, r8
	ldr	r3, [r2, #16]
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #0]
	subs	r0, r0, r3
	ldr	r3, [r2, #8]
	subs	r1, r1, r3
	bl	sub_080148e8
	ldr	r3, [r6, #0]
	add	r5, sp, #76
	str	r3, [r5, #0]
	lsls	r0, r0, #16
	ldr	r3, [r6, #4]
	lsrs	r0, r0, #16
	str	r3, [r5, #4]
	mov	sl, r0
	ldr	r3, [r6, #8]
	movs	r0, #128
	lsls	r0, r0, #7
	mov	r1, sl
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_08024f20
	cmp	r0, #0
	bne.n	.L_08026c98
	ldr	r3, [r6, #0]
	movs	r0, #160
	str	r3, [r5, #0]
	lsls	r0, r0, #12
	ldr	r3, [r6, #4]
	mov	r1, sl
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r6, #8]
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08026c98
	ldr	r3, [r6, #0]
	movs	r1, #128
	str	r3, [r5, #0]
	lsls	r1, r1, #5
	ldr	r3, [r6, #4]
	movs	r0, #160
	str	r3, [r5, #4]
	add	r1, sl
	ldr	r3, [r6, #8]
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08026c98
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08026c98
	ldr	r3, [r6, #0]
	ldr	r1, [pc, #368]
	str	r3, [r5, #0]
	movs	r0, #160
	ldr	r3, [r6, #4]
	add	r1, sl
	str	r3, [r5, #4]
	lsls	r0, r0, #12
	ldr	r3, [r6, #8]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08026c98
	movs	r0, #128
	lsls	r0, r0, #7
	mov	r1, sl
	adds	r2, r6, #0
	bl	sub_0801489c
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	str	r3, [r6, #56]
	ldr	r3, [sp, #12]
	movs	r0, #1
	orrs	r3, r0
	str	r3, [sp, #12]
	b.n	.L_08026ca0
.L_08026c98:
	ldr	r1, [sp, #16]
	movs	r3, #2
	orrs	r1, r3
	str	r1, [sp, #16]
.L_08026ca0:
	movs	r2, #1
	negs	r2, r2
	add	r9, r2
	mov	r3, r9
	adds	r6, #128
	adds	r7, #128
	cmp	r3, #0
	blt.n	.L_08026cb2
	b.n	.L_08026b7a
.L_08026cb2:
	ldr	r0, [sp, #16]
	cmp	r0, #0
	bne.n	.L_08026ccc
	ldr	r1, [sp, #12]
	cmp	r1, #0
	beq.n	.L_08026ccc
	movs	r3, #128
	lsls	r3, r3, #7
	mov	r2, r8
	str	r3, [r2, #48]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r2, #52]
.L_08026ccc:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	cmp	r3, #0
	beq.n	.L_08026cf6
	ldr	r0, [sp, #16]
	movs	r2, #3
	ands	r2, r0
	cmp	r2, #0
	beq.n	.L_08026cee
	movs	r1, #194
	lsls	r1, r1, #1
	adds	r2, r3, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_08026cf6
.L_08026cee:
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r3, r3, r0
	strh	r2, [r3, #0]
.L_08026cf6:
	ldr	r1, [sp, #12]
	cmp	r1, #0
	beq.n	.L_08026d06
	mov	r0, r8
	movs	r1, #8
	bl	sub_080233d0
	b.n	.L_08026d38
.L_08026d06:
	ldr	r2, [sp, #16]
	cmp	r2, #0
	beq.n	.L_08026d30
	ldr	r3, [pc, #188]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	movs	r5, #9
	cmp	r3, #0
	bne.n	.L_08026d26
	movs	r5, #22
.L_08026d26:
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_080233d0
	b.n	.L_08026d38
.L_08026d30:
	mov	r0, r8
	ldr	r1, [sp, #8]
	bl	sub_080233d0
.L_08026d38:
	ldr	r2, [sp, #16]
	cmp	r2, #0
	beq.n	.L_08026d86
	movs	r3, #128
	lsls	r3, r3, #24
	mov	r0, r8
	str	r3, [r0, #56]
	str	r3, [r0, #60]
	str	r3, [r0, #64]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026d76
	ldr	r2, [sp, #4]
	ldrh	r1, [r0, #6]
	lsrs	r3, r2, #16
	subs	r3, r3, r1
	lsls	r3, r3, #16
	movs	r2, #128
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_08026d68
	adds	r3, r2, #0
.L_08026d68:
	ldr	r2, [pc, #92]
	cmp	r3, r2
	bge.n	.L_08026d70
	adds	r3, r2, #0
.L_08026d70:
	adds	r3, r1, r3
	mov	r0, r8
	strh	r3, [r0, #6]
.L_08026d76:
	mov	r3, r8
	adds	r3, #100
	movs	r2, #0
	strh	r2, [r3, #0]
	mov	r2, r8
	adds	r2, #102
	movs	r3, #2
	b.n	.L_08026da4
.L_08026d86:
	add	r3, sp, #88
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	mov	r0, r8
	ldr	r3, [r3, #8]
	bl	sub_08024738
	mov	r2, r8
	adds	r2, #100
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	ldrh	r1, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08026da6
	subs	r3, r1, #1
.L_08026da4:
	strh	r3, [r2, #0]
.L_08026da6:
	bl	.L_08026e60
	mov	r1, r8
	ldrh	r3, [r1, #4]
	mov	r2, r8
	adds	r3, #1
	movs	r0, #1
	strh	r3, [r2, #4]
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xff000200
	.4byte 0xfffff000
	.2byte 0x0240
	.2byte 0x0200
.L_08026dd0:
	push	{r5, r6, r7, lr}
	movs	r7, #252
	lsls	r7, r7, #6
	adds	r7, #255
	movs	r3, #192
	lsrs	r6, r0, #14
	ands	r7, r0
	movs	r0, #8
	lsls	r3, r3, #18
	adds	r0, #255
	ldr	r5, [r3, #108]
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08026dfa
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #250
	strh	r3, [r2, #0]
	b.n	.L_08026e58
.L_08026dfa:
	movs	r2, #197
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_08026e42
	ldr	r1, [pc, #80]
	subs	r2, #138
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026e26
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #252
	lsls	r3, r3, #8
	adds	r3, #136
	strh	r3, [r2, #0]
	b.n	.L_08026e58
.L_08026e26:
	ldr	r3, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026e58
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #252
	lsls	r3, r3, #8
	adds	r3, #135
	strh	r3, [r2, #0]
	b.n	.L_08026e58
.L_08026e42:
	cmp	r6, #0
	beq.n	.L_08026e4c
	cmp	r6, #1
	beq.n	.L_08026e50
	b.n	.L_08026e58
.L_08026e4c:
	movs	r2, #179
	b.n	.L_08026e52
.L_08026e50:
	movs	r2, #180
.L_08026e52:
	lsls	r2, r2, #1
	adds	r3, r5, r2
	strh	r7, [r3, #0]
.L_08026e58:
	adds	r0, r7, #0
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
.L_08026e60:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #0
	mov	ip, r3
	cmp	r3, #0
	bne.n	.L_08026e72
	b.n	.L_08026f76
.L_08026e72:
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #172
	add	r3, ip
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08026eec
	ldr	r1, [pc, #240]
	ldr	r0, [pc, #244]
	movs	r2, #141
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08026ede
	movs	r2, #140
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08026ede
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #54
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08026ede
	movs	r2, #142
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08026ede
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #58
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #4]
	movs	r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026f76
.L_08026ede:
	movs	r2, #179
	movs	r3, #128
	lsls	r2, r2, #1
	lsls	r3, r3, #6
	add	r2, ip
	adds	r3, #155
	b.n	.L_08026f32
.L_08026eec:
	ldr	r1, [pc, #136]
	ldr	r4, [pc, #140]
	movs	r2, #141
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r4, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026f04
	movs	r2, #173
	b.n	.L_08026f2c
.L_08026f04:
	movs	r2, #140
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r4, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026f18
	movs	r2, #174
	b.n	.L_08026f2c
.L_08026f18:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #54
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r4, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026f38
	movs	r2, #175
.L_08026f2c:
	lsls	r2, r2, #1
	add	r2, ip
	movs	r3, #1
.L_08026f32:
	strh	r3, [r2, #0]
	movs	r0, #1
	b.n	.L_08026f76
.L_08026f38:
	movs	r2, #142
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r4, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026f56
	movs	r2, #144
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r0, [r3, #0]
	bl	.L_08026dd0
	b.n	.L_08026f76
.L_08026f56:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #58
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r4, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026f76
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #66
	adds	r3, r1, r2
	ldrh	r0, [r3, #0]
	bl	.L_08026dd0
.L_08026f76:
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
.L_08026f80:
	push	{r5, r6, lr}
	ldmia	r0!, {r4}
	ldmia	r2!, {r5}
	adds	r1, r1, r3
	subs	r4, r4, r5
	asrs	r6, r4, #16
	ldmia	r2!, {r5}
	ldmia	r0!, {r4}
	ldr	r2, [r2, #0]
	ldr	r0, [r0, #0]
	movs	r3, #128
	subs	r4, r4, r5
	subs	r0, r0, r2
	lsls	r3, r3, #15
	asrs	r4, r4, #16
	asrs	r0, r0, #16
	cmp	r6, r3
	bgt.n	.L_08026fc2
	cmp	r0, r3
	bgt.n	.L_08026fc2
	adds	r2, r4, #0
	muls	r2, r4
	adds	r3, r6, #0
	muls	r3, r6
	adds	r3, r3, r2
	adds	r2, r0, #0
	muls	r2, r0
	adds	r3, r3, r2
	adds	r2, r1, #0
	muls	r2, r1
	movs	r0, #0
	cmp	r3, r2
	blt.n	.L_08026fc6
.L_08026fc2:
	movs	r0, #1
	negs	r0, r0
.L_08026fc6:
	pop	{r5, r6, pc}
.L_08026fc8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	ldr	r2, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #11
	adds	r2, r2, r3
	mov	sl, r1
	ldr	r3, [r7, #16]
	ldr	r1, [r7, #8]
	movs	r0, #14
	bl	sub_08023220
	ldr	r2, [r7, #80]
	adds	r3, r7, #0
	adds	r3, #102
	mov	r8, r2
	ldrh	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	adds	r6, r0, #0
	cmp	r3, #0
	bne.n	.L_08027004
	movs	r0, #184
	lsls	r0, r0, #1
	adds	r0, #255
	bl	sub_081c0010
.L_08027004:
	cmp	r6, #0
	beq.n	.L_08027056
	ldr	r3, [r7, #20]
	ldr	r5, [r6, #80]
	str	r3, [r6, #20]
	adds	r0, r6, #0
	ldr	r1, [pc, #76]
	bl	sub_080233a8
	adds	r3, r6, #0
	adds	r3, #85
	movs	r6, #0
	strb	r6, [r3, #0]
	cmp	r5, #0
	beq.n	.L_08027056
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_08022b04
	strb	r6, [r5, #26]
	mov	r2, r8
	ldrb	r3, [r2, #9]
	ldrb	r1, [r5, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r5, #9]
	mov	r3, sl
	cmp	r3, #2
	bne.n	.L_08027056
	adds	r0, r5, #0
	movs	r1, #13
	bl	sub_080227b0
	adds	r0, r5, #0
	movs	r1, #8
	bl	sub_08022bac
.L_08027056:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xec88
	.2byte 0x0802
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	ldr	r0, [r0, #80]
	sub	sp, #120
	movs	r2, #0
	movs	r1, #2
	str	r0, [sp, #28]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #108]
	ldr	r1, [pc, #716]
	str	r4, [sp, #8]
	movs	r0, #143
	ldr	r3, [r3, #32]
	str	r2, [sp, #36]
	str	r3, [sp, #4]
	ldr	r3, [pc, #708]
	str	r2, [sp, #32]
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldrh	r2, [r3, #0]
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080270ba
	movs	r3, #192
	lsls	r3, r3, #9
	mov	r1, r8
	str	r3, [r1, #48]
	movs	r3, #128
	lsls	r3, r3, #7
	movs	r2, #5
	str	r3, [r1, #52]
	str	r2, [sp, #16]
	b.n	.L_080270c8
.L_080270ba:
	movs	r3, #128
	lsls	r3, r3, #9
	mov	r4, r8
	str	r3, [r4, #48]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r4, #52]
.L_080270c8:
	ldr	r3, [pc, #652]
	ldr	r1, [pc, #660]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
	movs	r1, #255
	lsls	r3, r3, #16
	lsls	r1, r1, #8
	lsrs	r7, r3, #16
	adds	r1, #255
	str	r3, [sp, #0]
	cmp	r7, r1
	bne.n	.L_080270f2
	ldr	r2, [sp, #36]
	movs	r3, #4
	orrs	r2, r3
	str	r2, [sp, #36]
	b.n	.L_08027664
.L_080270f2:
	movs	r3, #0
	str	r3, [sp, #36]
	mov	r0, r8
	ldr	r3, [r0, #8]
	add	r4, sp, #108
	str	r3, [r4, #0]
	mov	sl, r4
	ldr	r3, [r0, #12]
	mov	r2, sl
	str	r3, [r4, #4]
	adds	r1, r7, #0
	ldr	r3, [r0, #16]
	movs	r0, #128
	str	r3, [r4, #8]
	lsls	r0, r0, #12
	bl	sub_0801489c
	mov	r3, r8
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08027124
	b.n	.L_0802726e
.L_08027124:
	mov	r1, sl
	ldr	r3, [r1, #0]
	add	r6, sp, #96
	str	r3, [r6, #0]
	movs	r5, #128
	ldr	r3, [r1, #4]
	movs	r2, #128
	str	r3, [r6, #4]
	lsls	r5, r5, #10
	ldr	r3, [r1, #8]
	lsls	r2, r2, #7
	adds	r1, r7, r2
	adds	r0, r5, #0
	str	r3, [r6, #8]
	adds	r2, r6, #0
	bl	sub_0801489c
	movs	r3, #84
	mov	r4, sl
	add	r3, sp
	mov	r9, r3
	ldr	r3, [r4, #0]
	mov	r0, r9
	str	r3, [r0, #0]
	ldr	r2, [pc, #524]
	ldr	r3, [r4, #4]
	adds	r1, r7, r2
	str	r3, [r0, #4]
	mov	r2, r9
	ldr	r3, [r4, #8]
	mov	fp, r9
	str	r3, [r0, #8]
	adds	r0, r5, #0
	bl	sub_0801489c
	movs	r3, #0
	str	r3, [sp, #20]
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r4, #128
	ldr	r7, [r3, #20]
	ldr	r5, [pc, #496]
	lsls	r4, r4, #12
	mov	ip, r4
.L_0802717c:
	ldr	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_0802723c
	adds	r3, r7, #0
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802723c
	cmp	r7, r8
	beq.n	.L_0802723c
	mov	r0, r8
	ldr	r3, [r7, #12]
	ldr	r2, [r0, #12]
	ldr	r1, [pc, #464]
	subs	r2, r2, r3
	adds	r3, r2, r1
	cmp	r3, #0
	bge.n	.L_080271aa
	movs	r3, #128
	lsls	r3, r3, #13
	subs	r3, r3, r2
.L_080271aa:
	cmp	r3, r5
	bgt.n	.L_0802723c
	mov	r2, r8
	ldr	r4, [r2, #8]
	ldr	r1, [r7, #8]
	subs	r3, r4, r1
	add	r3, ip
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_080271d4
	ldr	r3, [r2, #16]
	ldr	r2, [r7, #16]
	movs	r0, #128
	subs	r3, r3, r2
	lsls	r0, r0, #12
	adds	r3, r3, r0
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_080271d4
	movs	r2, #1
	str	r2, [sp, #12]
.L_080271d4:
	mov	r0, sl
	ldr	r3, [r0, #0]
	subs	r3, r3, r1
	add	r3, ip
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_0802723c
	ldr	r3, [r0, #8]
	ldr	r2, [r7, #16]
	movs	r0, #128
	subs	r3, r3, r2
	lsls	r0, r0, #12
	adds	r3, r3, r0
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_0802723c
	ldr	r3, [r6, #0]
	subs	r3, r3, r1
	adds	r3, r3, r0
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_0802723c
	ldr	r3, [r6, #8]
	subs	r3, r3, r2
	adds	r3, r3, r0
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_0802723c
	mov	r0, fp
	ldr	r3, [r0, #0]
	subs	r3, r3, r1
	movs	r1, #128
	lsls	r1, r1, #12
	adds	r3, r3, r1
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_0802723c
	ldr	r3, [r0, #8]
	subs	r3, r3, r2
	adds	r3, r3, r1
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_0802723c
	ldr	r2, [r7, #56]
	movs	r3, #128
	lsls	r3, r3, #24
	cmp	r2, r3
	bne.n	.L_0802723c
	ldr	r3, [r7, #64]
	cmp	r3, r2
	bne.n	.L_0802723c
	b.n	.L_080274c6
.L_0802723c:
	ldr	r4, [sp, #20]
	adds	r7, #128
	adds	r4, #1
	str	r4, [sp, #20]
	cmp	r4, #63
	ble.n	.L_0802717c
	ldr	r0, [sp, #12]
	cmp	r0, #0
	beq.n	.L_0802726e
	mov	r0, r8
	mov	r1, sl
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_0802726e
	ldr	r2, [sp, #0]
	movs	r1, #0
	asrs	r2, r2, #16
	str	r2, [sp, #24]
	str	r1, [sp, #36]
	mov	r3, r8
	lsls	r2, r2, #16
	ldr	r4, [r3, #8]
	mov	fp, r2
	b.n	.L_080274e0
.L_0802726e:
	ldr	r3, [pc, #256]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08027290
	ldr	r3, [pc, #224]
	movs	r2, #128
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08027290
	ldr	r0, [sp, #0]
	movs	r4, #0
	asrs	r0, r0, #16
	str	r4, [sp, #36]
	str	r0, [sp, #24]
	b.n	.L_08027664
.L_08027290:
	mov	r0, r8
	mov	r1, sl
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_0802737c
	mov	r1, r8
	ldr	r3, [r1, #8]
	add	r5, sp, #96
	str	r3, [r5, #0]
	movs	r7, #128
	ldr	r3, [r1, #12]
	lsls	r7, r7, #12
	str	r3, [r5, #4]
	adds	r0, r7, #0
	ldr	r3, [r1, #16]
	str	r3, [r5, #8]
	ldr	r2, [sp, #0]
	movs	r3, #128
	lsrs	r6, r2, #16
	lsls	r3, r3, #5
	adds	r1, r6, r3
	adds	r2, r5, #0
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_0802737c
	mov	r4, r8
	ldr	r3, [r4, #8]
	ldr	r0, [pc, #160]
	str	r3, [r5, #0]
	adds	r1, r6, r0
	ldr	r3, [r4, #12]
	adds	r0, r7, #0
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r4, #16]
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_0802737c
	mov	r1, r8
	ldr	r3, [r1, #8]
	movs	r2, #128
	str	r3, [r5, #0]
	lsls	r2, r2, #6
	ldr	r3, [r1, #12]
	adds	r0, r7, #0
	str	r3, [r5, #4]
	ldr	r3, [r1, #16]
	adds	r1, r6, r2
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_0802737c
	mov	r4, r8
	ldr	r3, [r4, #8]
	ldr	r0, [pc, #84]
	str	r3, [r5, #0]
	adds	r1, r6, r0
	ldr	r3, [r4, #12]
	adds	r0, r7, #0
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r4, #16]
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_0802737c
	ldr	r1, [sp, #0]
	movs	r3, #84
	asrs	r1, r1, #16
	str	r1, [sp, #24]
	mov	r2, r8
	add	r3, sp
	lsls	r1, r1, #16
	ldr	r4, [r2, #8]
	mov	r9, r3
	mov	fp, r1
	b.n	.L_080274e0
	.4byte 0x03001150
	.4byte 0x02000240
	.4byte 0x0802ec5c
	.4byte 0xffffc000
	.4byte 0x0007ffff
	.4byte 0xfff00000
	.4byte 0x03001238
	.4byte 0xfffff000
	.2byte 0xe000
	.2byte 0xffff
.L_0802737c:
	.2byte 0x9800
	movs	r4, #40
	lsrs	r3, r0, #16
	movs	r1, #128
	ldr	r0, [pc, #844]
	add	r4, sp
	lsls	r1, r1, #5
	mov	r9, r4
	adds	r2, r3, r1
	strh	r2, [r4, #0]
	mov	r1, r9
	adds	r2, r3, r0
	movs	r4, #128
	strh	r2, [r1, #2]
	lsls	r4, r4, #6
	ldr	r1, [pc, #828]
	adds	r2, r3, r4
	mov	r0, r9
	strh	r2, [r0, #4]
	movs	r0, #192
	adds	r2, r3, r1
	mov	r4, r9
	lsls	r0, r0, #6
	strh	r2, [r4, #6]
	mov	r1, r9
	adds	r2, r3, r0
	strh	r2, [r1, #8]
	ldr	r2, [pc, #808]
	movs	r0, #0
	adds	r3, r3, r2
	strh	r3, [r4, #10]
	str	r0, [sp, #20]
	mov	r7, sl
.L_080273be:
	ldr	r1, [sp, #20]
	mov	r2, r9
	lsls	r3, r1, #1
	ldrsh	r2, [r2, r3]
	mov	r0, r8
	str	r2, [sp, #24]
	lsls	r2, r2, #16
	ldr	r3, [r0, #8]
	lsrs	r6, r2, #16
	str	r3, [r7, #0]
	adds	r1, r6, #0
	ldr	r3, [r0, #12]
	mov	fp, r2
	str	r3, [r7, #4]
	adds	r2, r7, #0
	ldr	r3, [r0, #16]
	movs	r0, #128
	lsls	r0, r0, #12
	str	r3, [r7, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r7, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_0802749a
	mov	r1, r8
	ldr	r3, [r1, #8]
	add	r5, sp, #96
	str	r3, [r5, #0]
	movs	r2, #128
	ldr	r3, [r1, #12]
	lsls	r2, r2, #5
	str	r3, [r5, #4]
	movs	r0, #128
	ldr	r3, [r1, #16]
	lsls	r0, r0, #12
	adds	r1, r6, r2
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_0802749a
	mov	r4, r8
	ldr	r3, [r4, #8]
	ldr	r0, [pc, #684]
	str	r3, [r5, #0]
	adds	r1, r6, r0
	ldr	r3, [r4, #12]
	movs	r0, #128
	str	r3, [r5, #4]
	lsls	r0, r0, #12
	ldr	r3, [r4, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_0802749a
	mov	r1, r8
	ldr	r3, [r1, #8]
	movs	r2, #128
	str	r3, [r5, #0]
	lsls	r2, r2, #6
	ldr	r3, [r1, #12]
	movs	r0, #128
	str	r3, [r5, #4]
	lsls	r0, r0, #12
	ldr	r3, [r1, #16]
	adds	r1, r6, r2
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_0802749a
	mov	r4, r8
	ldr	r3, [r4, #8]
	ldr	r0, [pc, #608]
	str	r3, [r5, #0]
	adds	r1, r6, r0
	ldr	r3, [r4, #12]
	movs	r0, #128
	str	r3, [r5, #4]
	lsls	r0, r0, #12
	ldr	r3, [r4, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	beq.n	.L_080274d6
.L_0802749a:
	ldr	r1, [sp, #20]
	adds	r1, #1
	str	r1, [sp, #20]
	cmp	r1, #6
	blt.n	.L_080273be
	mov	r3, r8
	ldr	r2, [r3, #8]
	mov	r4, sl
	str	r2, [r4, #0]
	mov	r0, r8
	ldr	r3, [r3, #12]
	str	r3, [r4, #4]
	ldr	r3, [r0, #16]
	str	r3, [r4, #8]
	ldr	r1, [sp, #36]
	movs	r3, #1
	adds	r4, r2, #0
	movs	r2, #84
	orrs	r1, r3
	add	r2, sp
	str	r1, [sp, #36]
	b.n	.L_080274de
.L_080274c6:
	ldr	r0, [sp, #0]
	movs	r3, #0
	asrs	r0, r0, #16
	str	r0, [sp, #24]
	lsls	r0, r0, #16
	str	r3, [sp, #36]
	mov	fp, r0
	b.n	.L_080274e0
.L_080274d6:
	mov	r1, r8
	movs	r2, #84
	ldr	r4, [r1, #8]
	add	r2, sp
.L_080274de:
	mov	r9, r2
.L_080274e0:
	mov	r3, r9
	str	r4, [r3, #0]
	mov	r4, r8
	ldr	r3, [r4, #12]
	mov	r0, r9
	str	r3, [r0, #4]
	mov	r2, fp
	ldr	r3, [r4, #16]
	lsrs	r1, r2, #16
	str	r3, [r0, #8]
	movs	r0, #128
	lsls	r0, r0, #11
	mov	r2, r9
	bl	sub_0801489c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #20]
	movs	r3, #63
	str	r3, [sp, #20]
	adds	r6, r7, #0
	adds	r6, #8
.L_0802750c:
	mov	r4, r8
	ldrh	r3, [r4, #32]
	subs	r1, r3, #2
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bne.n	.L_0802751a
	b.n	.L_0802763a
.L_0802751a:
	adds	r3, r7, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r0, #1
	mov	fp, r0
	mov	r3, fp
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0802752e
	b.n	.L_0802763a
.L_0802752e:
	cmp	r7, r8
	bne.n	.L_08027534
	b.n	.L_0802763a
.L_08027534:
	ldrh	r3, [r6, #24]
	adds	r0, r6, #0
	subs	r3, #2
	mov	r2, r9
	bl	.L_08026f80
	cmp	r0, #0
	blt.n	.L_0802763a
	ldr	r3, [r6, #80]
	ldr	r2, [pc, #408]
	movs	r1, #128
	ands	r3, r2
	lsls	r1, r1, #2
	cmp	r3, r1
	bne.n	.L_08027632
	mov	r2, r8
	ldr	r3, [r2, #16]
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #0]
	subs	r0, r0, r3
	ldr	r3, [r2, #8]
	subs	r1, r1, r3
	bl	sub_080148e8
	ldr	r3, [r6, #0]
	add	r5, sp, #96
	str	r3, [r5, #0]
	lsls	r0, r0, #16
	ldr	r3, [r6, #4]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r6, #8]
	str	r3, [r5, #8]
	asrs	r3, r0, #16
	lsrs	r0, r0, #16
	mov	sl, r0
	movs	r0, #128
	lsls	r0, r0, #7
	mov	r1, sl
	str	r3, [sp, #24]
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_08024f20
	cmp	r0, #0
	bne.n	.L_08027632
	ldr	r3, [r6, #0]
	movs	r0, #160
	str	r3, [r5, #0]
	lsls	r0, r0, #12
	ldr	r3, [r6, #4]
	mov	r1, sl
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r6, #8]
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08027632
	ldr	r3, [r6, #0]
	movs	r1, #128
	str	r3, [r5, #0]
	lsls	r1, r1, #5
	ldr	r3, [r6, #4]
	movs	r0, #160
	str	r3, [r5, #4]
	add	r1, sl
	ldr	r3, [r6, #8]
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08027632
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08027632
	ldr	r3, [r6, #0]
	ldr	r1, [pc, #228]
	str	r3, [r5, #0]
	movs	r0, #160
	ldr	r3, [r6, #4]
	add	r1, sl
	str	r3, [r5, #4]
	lsls	r0, r0, #12
	ldr	r3, [r6, #8]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08027632
	movs	r0, #128
	lsls	r0, r0, #7
	mov	r1, sl
	adds	r2, r6, #0
	bl	sub_0801489c
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	str	r3, [r6, #56]
	ldr	r4, [sp, #32]
	mov	r0, fp
	orrs	r4, r0
	str	r4, [sp, #32]
	b.n	.L_0802763a
.L_08027632:
	ldr	r1, [sp, #36]
	movs	r3, #2
	orrs	r1, r3
	str	r1, [sp, #36]
.L_0802763a:
	ldr	r2, [sp, #20]
	adds	r6, #128
	subs	r2, #1
	str	r2, [sp, #20]
	adds	r7, #128
	cmp	r2, #0
	blt.n	.L_0802764a
	b.n	.L_0802750c
.L_0802764a:
	ldr	r3, [sp, #36]
	cmp	r3, #0
	bne.n	.L_08027664
	ldr	r4, [sp, #32]
	cmp	r4, #0
	beq.n	.L_08027664
	movs	r3, #128
	lsls	r3, r3, #7
	mov	r0, r8
	str	r3, [r0, #48]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r0, #52]
.L_08027664:
	ldr	r1, [sp, #8]
	cmp	r1, #0
	beq.n	.L_0802769a
	ldr	r3, [sp, #36]
	movs	r2, #3
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08027682
	movs	r4, #194
	lsls	r4, r4, #1
	adds	r2, r1, r4
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0802768c
.L_08027682:
	ldr	r0, [sp, #8]
	movs	r1, #194
	lsls	r1, r1, #1
	adds	r3, r0, r1
	strh	r2, [r3, #0]
.L_0802768c:
	ldr	r3, [pc, #84]
	ldr	r4, [sp, #8]
	ldr	r3, [r3, #0]
	movs	r0, #195
	lsls	r0, r0, #1
	adds	r2, r4, r0
	strh	r3, [r2, #0]
.L_0802769a:
	ldr	r1, [sp, #32]
	cmp	r1, #0
	beq.n	.L_080276aa
	mov	r0, r8
	movs	r1, #8
	bl	sub_080233d0
	b.n	.L_080276f4
.L_080276aa:
	ldr	r2, [sp, #36]
	cmp	r2, #0
	beq.n	.L_080276ec
	ldr	r3, [pc, #52]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	movs	r5, #9
	cmp	r3, #0
	bne.n	.L_080276ca
	movs	r5, #22
.L_080276ca:
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_080233d0
	b.n	.L_080276f4
	.4byte 0xfffff000
	.4byte 0xffffe000
	.4byte 0xffffd000
	.4byte 0xff000200
	.4byte 0x03001150
	.2byte 0x0240
	.2byte 0x0200
.L_080276ec:
	mov	r0, r8
	ldr	r1, [sp, #16]
	bl	sub_080233d0
.L_080276f4:
	ldr	r2, [sp, #36]
	cmp	r2, #0
	beq.n	.L_0802774e
	movs	r3, #128
	mov	r4, r8
	lsls	r3, r3, #24
	str	r3, [r4, #56]
	str	r3, [r4, #60]
	str	r3, [r4, #64]
	movs	r3, #0
	str	r3, [r4, #36]
	str	r3, [r4, #44]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08027738
	ldr	r0, [sp, #0]
	ldrh	r1, [r4, #6]
	lsrs	r3, r0, #16
	subs	r3, r3, r1
	lsls	r3, r3, #16
	movs	r2, #128
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_0802772a
	adds	r3, r2, #0
.L_0802772a:
	ldr	r2, [pc, #368]
	cmp	r3, r2
	bge.n	.L_08027732
	adds	r3, r2, #0
.L_08027732:
	adds	r3, r1, r3
	mov	r1, r8
	strh	r3, [r1, #6]
.L_08027738:
	movs	r2, #100
	add	r2, r8
	mov	sl, r2
	movs	r3, #0
	mov	r4, sl
	mov	r2, r8
	strh	r3, [r4, #0]
	adds	r2, #102
	movs	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_080277a8
.L_0802774e:
	add	r3, sp, #108
	ldr	r2, [r3, #4]
	ldr	r1, [r3, #0]
	mov	r0, r8
	ldr	r3, [r3, #8]
	bl	sub_08024738
	mov	r0, r8
	ldr	r1, [r0, #36]
	ldr	r6, [pc, #316]
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	ldr	r1, [r2, #44]
	adds	r5, r0, #0
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	bl	sub_080149e0
	ldr	r3, [sp, #36]
	mov	r4, r8
	str	r3, [r4, #36]
	str	r3, [r4, #44]
	ldr	r2, [sp, #24]
	lsls	r1, r2, #16
	mov	r2, r8
	adds	r2, #36
	lsrs	r1, r1, #16
	bl	sub_0801489c
	movs	r3, #100
	add	r3, r8
	mov	sl, r3
	ldrh	r2, [r3, #0]
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	beq.n	.L_080277a8
	subs	r3, r2, #1
	mov	r0, sl
	strh	r3, [r0, #0]
.L_080277a8:
	ldr	r1, [sp, #4]
	movs	r3, #12
	ldrb	r2, [r1, #23]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080277b6
	b.n	.L_080278c4
.L_080277b6:
	mov	r2, r8
	adds	r2, #34
	ldrb	r3, [r2, #0]
	cmp	r3, #2
	bhi.n	.L_080277d2
	adds	r2, r3, #0
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [r1, r3]
	b.n	.L_080277d4
.L_080277d2:
	ldr	r2, [pc, #208]
.L_080277d4:
	mov	r4, r8
	ldr	r3, [r4, #8]
	cmp	r3, #0
	bge.n	.L_080277e0
	ldr	r0, [pc, #200]
	adds	r3, r3, r0
.L_080277e0:
	mov	r4, r8
	asrs	r1, r3, #20
	ldr	r3, [r4, #16]
	cmp	r3, #0
	bge.n	.L_080277ee
	ldr	r0, [pc, #188]
	adds	r3, r3, r0
.L_080277ee:
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	ldr	r1, [sp, #28]
	adds	r2, r2, r3
	ldrb	r2, [r2, #3]
	ldrb	r5, [r1, #26]
	movs	r3, #16
	ands	r2, r3
	adds	r1, r5, #0
	cmp	r2, #0
	beq.n	.L_08027872
	movs	r6, #8
	adds	r3, r5, #0
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_0802782e
	ldr	r3, [sp, #28]
	ldr	r4, [sp, #28]
	ldrb	r2, [r3, #17]
	movs	r3, #3
	ands	r3, r2
	movs	r2, #16
	orrs	r3, r2
	strb	r3, [r4, #17]
	movs	r2, #4
	adds	r3, r5, #0
	orrs	r3, r2
	movs	r2, #254
	ands	r3, r2
	strb	r3, [r4, #26]
.L_0802782e:
	mov	r1, sl
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_0802788c
	ldr	r2, [sp, #36]
	cmp	r2, #0
	bne.n	.L_08027892
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne.n	.L_080278ac
	ldr	r4, [sp, #4]
	mov	r0, r8
	ldrb	r1, [r4, #23]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl	.L_08026fc8
	ldr	r0, [sp, #16]
	cmp	r0, #5
	bne.n	.L_0802785e
	mov	r1, sl
	strh	r6, [r1, #0]
	b.n	.L_08027864
.L_0802785e:
	movs	r3, #12
	mov	r2, sl
	strh	r3, [r2, #0]
.L_08027864:
	mov	r1, r8
	adds	r1, #102
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #44]
	eors	r3, r2
	strh	r3, [r1, #0]
	b.n	.L_080278ac
.L_08027872:
	movs	r3, #8
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_080278b0
	movs	r3, #1
	ldr	r4, [sp, #28]
	adds	r2, r5, #0
	orrs	r2, r3
	movs	r3, #251
	ands	r2, r3
	strb	r2, [r4, #26]
	ldrb	r1, [r4, #26]
	b.n	.L_080278b0
.L_0802788c:
	ldr	r0, [sp, #28]
	ldrb	r1, [r0, #26]
	b.n	.L_080278b0
.L_08027892:
	ldr	r2, [sp, #28]
	ldrb	r1, [r2, #26]
	b.n	.L_080278b0
	.4byte 0x00000001
	.4byte 0xfffff000
	.4byte 0x0300021c
	.4byte 0x02010000
	.2byte 0xffff
	.2byte 0x000f
.L_080278ac:
	ldr	r3, [sp, #28]
	ldrb	r1, [r3, #26]
.L_080278b0:
	cmp	r5, r1
	beq.n	.L_080278c0
	ldr	r4, [sp, #28]
	movs	r3, #1
	strb	r3, [r4, #25]
	ldr	r0, [sp, #4]
	ldrb	r2, [r0, #23]
	b.n	.L_080278c4
.L_080278c0:
	ldr	r1, [sp, #4]
	ldrb	r2, [r1, #23]
.L_080278c4:
	ldr	r3, [sp, #0]
	lsrs	r7, r3, #16
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08027990
	mov	r0, sl
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	bne.n	.L_08027990
	ldr	r1, [sp, #36]
	cmp	r1, #0
	bne.n	.L_08027990
	mov	r2, r8
	mov	r4, r8
	movs	r0, #135
	ldr	r1, [r2, #8]
	lsls	r0, r0, #1
	ldr	r2, [r2, #12]
	ldr	r3, [r4, #16]
	bl	sub_08023220
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_08027990
	mov	r0, r8
	ldr	r3, [r0, #20]
	ldr	r1, [pc, #120]
	str	r3, [r6, #20]
	adds	r0, r6, #0
	ldr	r5, [r6, #80]
	bl	sub_080233a8
	adds	r2, r6, #0
	add	r1, sp, #36
	movs	r3, #2
	adds	r2, #35
	ldrb	r1, [r1, #0]
	strb	r3, [r2, #0]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	cmp	r5, #0
	beq.n	.L_0802793c
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08022b04
	movs	r4, #128
	add	r2, sp, #36
	lsls	r4, r4, #7
	ldrb	r2, [r2, #0]
	adds	r3, r7, r4
	strh	r3, [r5, #18]
	ldrb	r3, [r5, #9]
	strb	r2, [r5, #26]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r5, #9]
.L_0802793c:
	mov	r7, r8
	adds	r7, #102
	movs	r0, #0
	ldrsh	r3, [r7, r0]
	ldrh	r2, [r7, #0]
	cmp	r3, #2
	bne.n	.L_0802795a
	movs	r1, #2
	adds	r0, r5, #0
	bl	sub_08022b04
	add	r1, sp, #36
	ldrh	r1, [r1, #0]
	ldr	r2, [pc, #28]
	strh	r1, [r7, #0]
.L_0802795a:
	lsls	r3, r2, #16
	cmp	r3, #0
	beq.n	.L_08027966
	movs	r3, #128
	lsls	r3, r3, #8
	strh	r3, [r6, #6]
.L_08027966:
	ldr	r2, [sp, #16]
	cmp	r2, #5
	bne.n	.L_0802797c
	movs	r3, #12
	mov	r4, sl
	strh	r3, [r4, #0]
	b.n	.L_08027982
	.4byte 0x00000000
	.2byte 0xec7c
	.2byte 0x0802
.L_0802797c:
	movs	r3, #18
	mov	r0, sl
	strh	r3, [r0, #0]
.L_08027982:
	ldrh	r3, [r7, #0]
	ldr	r2, [pc, #4]
	eors	r3, r2
	strh	r3, [r7, #0]
	b.n	.L_08027990
	.2byte 0x0001
	.2byte 0x0000
.L_08027990:
	bl	.L_08026e60
	mov	r1, r8
	ldrh	r3, [r1, #4]
	mov	r2, r8
	adds	r3, #1
	movs	r0, #1
	strh	r3, [r2, #4]
	add	sp, #120
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
