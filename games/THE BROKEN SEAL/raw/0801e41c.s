.syntax unified
	.thumb
	.set sub_0801e260, 0x0801e260
	.global Overlay_0801e41c
Overlay_0801e41c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r3
	ldr	r3, [pc, #676]
	ldr	r3, [r3, #0]
	adds	r7, r1, #0
	adds	r5, r0, #0
	mov	r8, r2
	ldr	r6, [sp, #28]
	mov	r9, r3
	cmp	r7, sl
	beq.n	.L_0801e43c
	b.n	.L_0801e572
.L_0801e43c:
	cmp	r8, r6
	bne.n	.L_0801e442
	b.n	.L_0801e6c2
.L_0801e442:
	cmp	r8, r6
	bls.n	.L_0801e44c
	mov	r4, r8
	mov	r8, r6
	adds	r6, r4, #0
.L_0801e44c:
	ldrh	r0, [r5, #12]
	ldrh	r1, [r5, #14]
	mov	r2, r8
	subs	r3, r6, r2
	add	r0, sl
	movs	r2, #1
	add	r1, r8
	bl	sub_0801e260
	ldrh	r3, [r5, #14]
	ldrh	r2, [r5, #12]
	add	r3, r8
	lsls	r3, r3, #5
	adds	r3, r3, r2
	add	r3, sl
	lsls	r3, r3, #1
	mov	r2, r9
	mov	r4, r8
	adds	r0, r3, r2
	cmp	r4, r6
	bls.n	.L_0801e478
	b.n	.L_0801e6c2
.L_0801e478:
	ldrh	r1, [r0, #0]
	cmp	r4, r8
	bne.n	.L_0801e4d8
	ldr	r2, [pc, #596]
	adds	r3, r1, r2
	cmp	r3, #15
	bhi.n	.L_0801e564
	ldr	r2, [pc, #592]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0801e566
	.4byte 0x0801e560
	.4byte 0x0801e566
	.4byte 0x0801e566
	.4byte 0x0801e566
	.4byte 0x0801e4d4
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e4d0
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.2byte 0xe566
	.2byte 0x0801
	ldr	r1, [pc, #520]
	b.n	.L_0801e566
	ldr	r1, [pc, #520]
	b.n	.L_0801e566
.L_0801e4d8:
	cmp	r4, r6
	bne.n	.L_0801e538
	ldr	r2, [pc, #500]
	adds	r3, r1, r2
	cmp	r3, #16
	bhi.n	.L_0801e564
	ldr	r2, [pc, #508]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0801e560
	.4byte 0x0801e566
	.4byte 0x0801e566
	.4byte 0x0801e566
	.4byte 0x0801e566
	.4byte 0x0801e534
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e530
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.2byte 0xe566
	.2byte 0x0801
	ldr	r1, [pc, #436]
	b.n	.L_0801e566
	ldr	r1, [pc, #436]
	b.n	.L_0801e566
.L_0801e538:
	ldr	r2, [pc, #408]
	adds	r3, r1, r2
	cmp	r3, #5
	bhi.n	.L_0801e564
	ldr	r2, [pc, #428]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0801e560
	.4byte 0x0801e560
	.4byte 0x0801e566
	.4byte 0x0801e566
	.4byte 0x0801e566
	.2byte 0xe560
	.2byte 0x0801
	ldr	r1, [pc, #400]
	b.n	.L_0801e566
.L_0801e564:
	ldr	r1, [pc, #400]
.L_0801e566:
	adds	r4, #1
	strh	r1, [r0, #0]
	adds	r0, #64
	cmp	r4, r6
	bls.n	.L_0801e478
	b.n	.L_0801e6c2
.L_0801e572:
	cmp	r8, r6
	beq.n	.L_0801e578
	b.n	.L_0801e6c2
.L_0801e578:
	cmp	r7, sl
	bne.n	.L_0801e57e
	b.n	.L_0801e6c2
.L_0801e57e:
	cmp	r7, sl
	bls.n	.L_0801e588
	adds	r4, r7, #0
	mov	r7, sl
	mov	sl, r4
.L_0801e588:
	ldrh	r0, [r5, #12]
	ldrh	r1, [r5, #14]
	mov	r3, sl
	subs	r2, r3, r7
	adds	r0, r0, r7
	movs	r3, #1
	add	r1, r8
	bl	sub_0801e260
	ldrh	r3, [r5, #14]
	ldrh	r2, [r5, #12]
	add	r3, r8
	lsls	r3, r3, #5
	adds	r3, r3, r2
	adds	r3, r3, r7
	lsls	r3, r3, #1
	mov	r2, r9
	adds	r4, r7, #0
	adds	r0, r3, r2
	cmp	r4, sl
	bls.n	.L_0801e5b4
	b.n	.L_0801e6c2
.L_0801e5b4:
	ldrh	r1, [r0, #0]
	cmp	r4, r7
	bne.n	.L_0801e61c
	ldr	r2, [pc, #280]
	adds	r3, r1, r2
	cmp	r3, #17
	bhi.n	.L_0801e6b4
	ldr	r2, [pc, #312]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0801e6b6
	.4byte 0x0801e6b6
	.4byte 0x0801e6b6
	.4byte 0x0801e6b0
	.4byte 0x0801e6b6
	.4byte 0x0801e6b4
	.4byte 0x0801e618
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e614
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.2byte 0xe6b6
	.2byte 0x0801
	ldr	r1, [pc, #232]
	b.n	.L_0801e6b6
	ldr	r1, [pc, #232]
	b.n	.L_0801e6b6
.L_0801e61c:
	cmp	r4, sl
	bne.n	.L_0801e684
	ldr	r2, [pc, #176]
	adds	r3, r1, r2
	cmp	r3, #18
	bhi.n	.L_0801e6b4
	ldr	r2, [pc, #220]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0801e6b6
	.4byte 0x0801e6b6
	.4byte 0x0801e6b0
	.4byte 0x0801e6b6
	.4byte 0x0801e6b6
	.4byte 0x0801e6b4
	.4byte 0x0801e680
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e67c
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.2byte 0xe6b6
	.2byte 0x0801
	ldr	r1, [pc, #140]
	b.n	.L_0801e6b6
	ldr	r1, [pc, #140]
	b.n	.L_0801e6b6
.L_0801e684:
	ldr	r2, [pc, #76]
	adds	r3, r1, r2
	cmp	r3, #6
	bhi.n	.L_0801e6b4
	ldr	r2, [pc, #132]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0801e6b6
	.4byte 0x0801e6b6
	.4byte 0x0801e6b0
	.4byte 0x0801e6b0
	.4byte 0x0801e6b6
	.4byte 0x0801e6b4
	.2byte 0xe6b0
	.2byte 0x0801
	ldr	r1, [pc, #64]
	b.n	.L_0801e6b6
.L_0801e6b4:
	ldr	r1, [pc, #96]
.L_0801e6b6:
	adds	r4, #1
	strh	r1, [r0, #0]
	adds	r0, #2
	cmp	r4, sl
	bhi.n	.L_0801e6c2
	b.n	.L_0801e5b4
.L_0801e6c2:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0xffff0ff7
	.4byte 0x0801e490
	.4byte 0x0000f018
	.4byte 0x0000f009
	.4byte 0x0801e4ec
	.4byte 0x0000f019
	.4byte 0x0000f00a
	.4byte 0x0801e548
	.4byte 0x0000f00d
	.4byte 0x0000f00f
	.4byte 0x0801e5cc
	.4byte 0x0000f01a
	.4byte 0x0000f00b
	.4byte 0x0801e630
	.4byte 0x0000f01b
	.4byte 0x0000f00c
	.4byte 0x0801e694
	.4byte 0x0000f00e
