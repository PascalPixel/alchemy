.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08016ca4, 0x08016ca4
	.set sub_08038118, 0x08038118
	.set sub_080381c8, 0x080381c8
	.set sub_08038220, 0x08038220
	.set sub_0811c66c, 0x0811c66c
	.set sub_0811c6cc, 0x0811c6cc
	.set sub_0811c710, 0x0811c710
	.set sub_0811ca54, 0x0811ca54
	.set sub_0811cd30, 0x0811cd30
	.set sub_08126548, 0x08126548
	.global Overlay_0811c538
Overlay_0811c538:
	push	{lr}
	movs	r3, #192
	ldr	r0, [pc, #76]
	lsls	r3, r3, #18
	ldr	r1, [r3, #48]
	adds	r3, #176
	ldr	r4, [r3, #0]
	ldr	r3, [r0, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	sub	sp, #4
	cmp	r3, #0
	beq.n	.L_0811c55a
	ldrh	r3, [r1, #54]
	adds	r3, r3, r2
	strh	r3, [r1, #54]
.L_0811c55a:
	ldr	r3, [r0, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0811c56e
	ldrh	r3, [r1, #54]
	ldr	r2, [pc, #36]
	adds	r3, r3, r2
	strh	r3, [r1, #54]
.L_0811c56e:
	ldr	r3, [r4, #20]
	cmp	r3, #0
	bne.n	.L_0811c588
	movs	r1, #240
	movs	r3, #128
	lsls	r3, r3, #9
	lsls	r1, r1, #15
	str	r3, [sp, #0]
	adds	r0, r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08126548
.L_0811c588:
	add	sp, #4
	pop	{pc}
	.4byte 0x03001150
	.2byte 0xfe00
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	movs	r1, #0
	ldrsh	r0, [r7, r1]
	bl	sub_08016ca4
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L_0811c5ae
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811c646
.L_0811c5ae:
	adds	r0, r7, #0
	bl	sub_0811cd30
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	ldr	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	strh	r0, [r7, #10]
	ldr	r1, [pc, #132]
	cmp	r3, #4
	bgt.n	.L_0811c5cc
	movs	r1, #128
	lsls	r1, r1, #6
.L_0811c5cc:
	movs	r3, #60
	str	r1, [r2, #0]
	str	r3, [r2, #4]
	bl	sub_08038118
	movs	r2, #6
	ldrsh	r3, [r7, r2]
	cmp	r3, #2
	beq.n	.L_0811c616
	cmp	r3, #2
	bgt.n	.L_0811c5ec
	cmp	r3, #0
	beq.n	.L_0811c624
	cmp	r3, #1
	beq.n	.L_0811c63a
	b.n	.L_0811c624
.L_0811c5ec:
	cmp	r3, #3
	beq.n	.L_0811c608
	cmp	r3, #99
	bne.n	.L_0811c624
	ldr	r0, [pc, #84]
	bl	sub_080381c8
	adds	r0, r7, #0
	bl	sub_0811c66c
	cmp	r0, #0
	beq.n	.L_0811c640
	movs	r0, #1
	b.n	.L_0811c646
.L_0811c608:
	movs	r0, #45
	bl	sub_08013560
	adds	r0, r7, #0
	bl	sub_0811c6cc
	b.n	.L_0811c640
.L_0811c616:
	movs	r0, #45
	bl	sub_08013560
	adds	r0, r7, #0
	bl	sub_0811ca54
	b.n	.L_0811c640
.L_0811c624:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	ldr	r6, [r3, #0]
	movs	r5, #0
	str	r5, [r6, #20]
	adds	r0, r7, #0
	bl	sub_0811ca54
	str	r5, [r6, #20]
	b.n	.L_0811c640
.L_0811c63a:
	adds	r0, r7, #0
	bl	sub_0811c710
.L_0811c640:
	bl	sub_08038220
	movs	r0, #0
.L_0811c646:
	pop	{r5, r6, r7, pc}
	.4byte 0xffffe000
	.2byte 0x0c98
	.2byte 0x0000
	push	{lr}
	cmp	r0, #7
	bhi.n	.L_0811c65a
	movs	r0, #0
	b.n	.L_0811c668
.L_0811c65a:
	adds	r3, r0, #0
	subs	r3, #128
	movs	r0, #0
	cmp	r3, #5
	bls.n	.L_0811c668
	movs	r0, #1
	negs	r0, r0
.L_0811c668:
	pop	{pc}
