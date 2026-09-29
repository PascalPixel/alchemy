.syntax unified
	.thumb
	.section .text.x02008214,"ax",%progbits
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r2, #0
	movs	r0, #112
	sub	sp, #4
	mov	r8, r2
	bl 0x020088c8
	movs	r5, #2
	movs	r1, #0
	movs	r2, #30
	movs	r3, #7
	movs	r0, #0
	str	r5, [sp, #0]
	bl 0x020087f0
	movs	r1, #8
	adds	r7, r0, #0
	movs	r2, #28
	movs	r3, #10
	movs	r0, #0
	str	r5, [sp, #0]
	bl 0x020087f0
	movs	r6, #1
	mov	r9, r0
	mov	sl, r6
.L_02000250:
	ldr	r3, [pc, #540]
	ldr	r0, [pc, #544]
	ldr	r1, [pc, #544]
	ldr	r2, [pc, #548]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #28
	ldr	r0, [pc, #544]
	ldr	r2, [pc, #544]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
.L_02000268:
	bl 0x020087e8
.L_0200026c:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_02000306
	movs	r3, #135
	lsls	r3, r3, #1
	movs	r1, #135
	movs	r2, #0
	adds	r0, r6, r3
	lsls	r1, r1, #1
	mov	sl, r2
	bl 0x020087e0
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl 0x02008830
	adds	r0, r7, #0
	bl 0x02008838
	ldr	r0, [pc, #500]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x02008818
	mov	r2, sl
	str	r2, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #0
	adds	r2, r7, #0
	movs	r3, #80
	bl 0x02008820
	bl 0x02008888
	cmp	r0, #0
	beq.n	.L_020002fa
	ldr	r5, [pc, #468]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #32
	ands	r5, r6
	ldr	r0, [pc, #460]
	bl 0x02008818
	adds	r0, r5, #0
	bl 0x02008860
	ldr	r0, [pc, #452]
	adds	r1, r7, #0
	adds	r0, r5, r0
	movs	r2, #120
	movs	r3, #0
	bl 0x02008808
	ldr	r3, [pc, #444]
	adds	r5, r5, r3
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl 0x02008808
	mov	r0, r9
	bl 0x02008830
	mov	r0, r9
	adds	r1, r6, #0
	bl 0x020088c0
	b.n	.L_02000306
.L_020002fa:
	ldr	r0, [pc, #416]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #32
	bl 0x02008818
.L_02000306:
	ldr	r5, [pc, #408]
	ldr	r3, [r5, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000326
	adds	r0, r6, #0
	bl 0x02008868
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_02000330
	movs	r0, #175
	bl 0x020088c8
.L_02000326:
	ldr	r3, [r5, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000338
.L_02000330:
	movs	r0, #113
	bl 0x020088c8
	b.n	.L_02000442
.L_02000338:
	ldr	r5, [pc, #360]
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000354
	movs	r2, #255
	movs	r3, #1
	movs	r0, #111
	mov	r8, r2
	subs	r6, #1
	mov	sl, r3
	bl 0x020088c8
.L_02000354:
	ldr	r3, [r5, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200036c
	movs	r2, #1
	movs	r0, #111
	mov	r8, r2
	adds	r6, #1
	mov	sl, r2
	bl 0x020088c8
.L_0200036c:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000384
	movs	r3, #1
	movs	r0, #111
	mov	r8, r3
	adds	r6, #10
	mov	sl, r3
	bl 0x020088c8
.L_02000384:
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200039e
	movs	r2, #255
	movs	r3, #1
	movs	r0, #111
	mov	r8, r2
	subs	r6, #10
	mov	sl, r3
	bl 0x020088c8
.L_0200039e:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020003b8
	movs	r2, #1
	movs	r0, #111
	mov	r8, r2
	adds	r6, #30
	mov	sl, r2
	bl 0x020088c8
.L_020003b8:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020003d4
	movs	r3, #255
	movs	r2, #1
	movs	r0, #111
	mov	r8, r3
	subs	r6, #30
	mov	sl, r2
	bl 0x020088c8
.L_020003d4:
	mov	r3, r8
	lsls	r5, r3, #24
	movs	r2, #1
	asrs	r3, r5, #24
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02000408
	movs	r3, #135
	lsls	r3, r3, #1
	movs	r1, #135
	adds	r0, r6, r3
	b.n	.L_020003f2
.L_020003ec:
	ldr	r2, [pc, #184]
	movs	r1, #135
	adds	r0, r6, r2
.L_020003f2:
	lsls	r1, r1, #1
	bl 0x020087e0
	adds	r6, r0, #0
	ldr	r0, [pc, #144]
	ands	r0, r6
	bl 0x02008860
	ldrh	r3, [r0, #6]
	cmp	r3, #0
	beq.n	.L_020003ec
.L_02000408:
	movs	r3, #128
	lsls	r3, r3, #17
	cmp	r5, r3
	bne.n	.L_02000436
	movs	r2, #135
	lsls	r2, r2, #1
	movs	r1, #135
	adds	r0, r6, r2
	b.n	.L_02000420
.L_0200041a:
	ldr	r3, [pc, #144]
	movs	r1, #135
	adds	r0, r6, r3
.L_02000420:
	lsls	r1, r1, #1
	bl 0x020087e0
	adds	r6, r0, #0
	ldr	r0, [pc, #96]
	ands	r0, r6
	bl 0x02008860
	ldrh	r3, [r0, #6]
	cmp	r3, #0
	beq.n	.L_0200041a
.L_02000436:
	movs	r2, #0
	movs	r0, #1
	mov	r8, r2
	bl 0x020087e8
	b.n	.L_0200026c
.L_02000442:
	adds	r0, r7, #0
	bl 0x02008830
	movs	r0, #1
	bl 0x020087e8
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x020087f8
	mov	r0, r9
	movs	r1, #1
	bl 0x020087f8
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x040000d4
	.4byte 0x05000200
	.4byte 0x050001c0
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x80000001
	.4byte 0x020088d4
	.4byte 0x000001ff
	.4byte 0x020088e0
	.4byte 0x00000182
	.4byte 0x00000075
	.4byte 0x020088f8
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x0000010d
	.2byte 0x010f
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r2, #0
	movs	r0, #112
	sub	sp, #4
	mov	r8, r2
	bl 0x020088c8
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #30
	movs	r3, #12
	movs	r0, #0
	bl 0x020087f0
	movs	r6, #1
	adds	r7, r0, #0
	mov	sl, r6
	ldr	r3, [pc, #476]
	ldr	r0, [pc, #480]
	ldr	r1, [pc, #480]
	ldr	r2, [pc, #484]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #28
	ldr	r0, [pc, #480]
	ldr	r2, [pc, #480]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl 0x020087e8
.L_020004f8:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_02000574
	movs	r3, #135
	lsls	r3, r3, #1
	movs	r1, #135
	movs	r2, #0
	adds	r0, r6, r3
	lsls	r1, r1, #1
	mov	sl, r2
	bl 0x020087e0
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl 0x02008830
	adds	r0, r7, #0
	bl 0x02008838
	ldr	r0, [pc, #436]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x02008818
	mov	r2, sl
	str	r2, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #0
	adds	r2, r7, #0
	movs	r3, #80
	bl 0x02008820
	ldr	r5, [pc, #412]
	ldr	r0, [pc, #412]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #72
	bl 0x02008818
	ldr	r0, [pc, #404]
	ands	r5, r6
	adds	r0, r5, r0
	adds	r1, r7, #0
	movs	r2, #120
	movs	r3, #0
	bl 0x02008808
	ldr	r3, [pc, #392]
	adds	r5, r5, r3
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #24
	bl 0x02008808
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #48
	bl 0x02008800
.L_02000574:
	ldr	r3, [pc, #368]
	ldr	r3, [r3, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020005b2
	movs	r0, #113
	bl 0x020088c8
	adds	r0, r7, #0
	bl 0x02008830
	movs	r0, #1
	bl 0x020087e8
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x020087f8
	mov	r0, r9
	movs	r1, #1
	bl 0x020087f8
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
.L_020005b2:
	ldr	r5, [pc, #312]
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020005ce
	movs	r3, #255
	movs	r2, #1
	movs	r0, #111
	mov	r8, r3
	subs	r6, #1
	mov	sl, r2
	bl 0x020088c8
.L_020005ce:
	ldr	r3, [r5, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020005e6
	movs	r3, #1
	movs	r0, #111
	mov	r8, r3
	adds	r6, #1
	mov	sl, r3
	bl 0x020088c8
.L_020005e6:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020005fe
	movs	r2, #1
	movs	r0, #111
	mov	r8, r2
	adds	r6, #10
	mov	sl, r2
	bl 0x020088c8
.L_020005fe:
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000618
	movs	r3, #255
	movs	r2, #1
	movs	r0, #111
	mov	r8, r3
	subs	r6, #10
	mov	sl, r2
	bl 0x020088c8
.L_02000618:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000632
	movs	r3, #1
	movs	r0, #111
	mov	r8, r3
	adds	r6, #30
	mov	sl, r3
	bl 0x020088c8
.L_02000632:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200064e
	movs	r2, #255
	movs	r3, #1
	movs	r0, #111
	mov	r8, r2
	subs	r6, #30
	mov	sl, r3
	bl 0x020088c8
.L_0200064e:
	mov	r2, r8
	lsls	r5, r2, #24
	movs	r2, #1
	asrs	r3, r5, #24
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02000682
	movs	r3, #135
	lsls	r3, r3, #1
	movs	r1, #135
	adds	r0, r6, r3
	b.n	.L_0200066c
.L_02000666:
	ldr	r2, [pc, #136]
	movs	r1, #135
	adds	r0, r6, r2
.L_0200066c:
	lsls	r1, r1, #1
	bl 0x020087e0
	adds	r6, r0, #0
	ldr	r0, [pc, #96]
	ands	r0, r6
	bl 0x02008870
	ldrb	r3, [r0, #4]
	cmp	r3, #0
	beq.n	.L_02000666
.L_02000682:
	movs	r3, #128
	lsls	r3, r3, #17
	cmp	r5, r3
	bne.n	.L_020006b0
	movs	r2, #135
	lsls	r2, r2, #1
	movs	r1, #135
	adds	r0, r6, r2
	b.n	.L_0200069a
.L_02000694:
	ldr	r3, [pc, #92]
	movs	r1, #135
	adds	r0, r6, r3
.L_0200069a:
	lsls	r1, r1, #1
	bl 0x020087e0
	adds	r6, r0, #0
	ldr	r0, [pc, #52]
	ands	r0, r6
	bl 0x02008870
	ldrb	r3, [r0, #4]
	cmp	r3, #0
	beq.n	.L_02000694
.L_020006b0:
	movs	r2, #0
	movs	r0, #1
	mov	r8, r2
	bl 0x020087e8
	b.n	.L_020004f8
	.4byte 0x040000d4
	.4byte 0x05000200
	.4byte 0x050001c0
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x80000001
	.4byte 0x0200890c
	.4byte 0x00003fff
	.4byte 0x02008914
	.4byte 0x00000333
	.4byte 0x0000053a
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x0000010d
	.2byte 0x010f
	.2byte 0x0000
	.section .rodata,"a",%progbits
	.global gItemLevelLvLabel
gItemLevelLvLabel:
	.4byte 0x0000764c
	.4byte 0x6d657449
	.4byte 0x3a6f4e20
	.4byte 0x00000000
	.4byte 0x65473a41
	.4byte 0x74492074
	.4byte 0x20206d65
	.4byte 0x65523a42
	.4byte 0x6e727574
	.4byte 0x00000000
	.4byte 0x4d455449
	.4byte 0x4c554620
	.4byte 0x2e2e2e4c
	.4byte 0x2e2e2e2e
	.4byte 0x0000002e
	.4byte 0x20797350
	.4byte 0x003a6f4e
	.4byte 0x65523a42
	.4byte 0x6e727574
	.4byte 0x00000000
	.global gItemLevelGlyphsUpper
gItemLevelGlyphsUpper:
	.4byte 0x20422041
	.4byte 0x20442043
	.4byte 0x20462045
	.4byte 0x20482047
	.4byte 0x204b204a
	.4byte 0x204d204c
	.4byte 0x004f204e
	.global gItemLevelGlyphsLower
gItemLevelGlyphsLower:
	.4byte 0x20512050
	.4byte 0x20532052
	.4byte 0x20552054
	.4byte 0x20572056
	.4byte 0x20592058
	.4byte 0x2061205a
	.4byte 0x00632062
	.global gItemLevelGlyphsMarks
gItemLevelGlyphsMarks:
	.4byte 0x203f2021
	.4byte 0x20242023
	.4byte 0x00000025
	.global gItemLevelEntrances
gItemLevelEntrances:
	.4byte 0xffff0000
	.4byte 0x00000338
	.4byte 0xc0000360
	.4byte 0x01180000
	.4byte 0x039001f8
	.4byte 0x00000380
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gItemLevelExits
gItemLevelExits:
	.4byte 0x000001ff
	.global gItemLevelPlacements
gItemLevelPlacements:
	.4byte 0xffff01f4
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x03600000
	.4byte 0x00002000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00002000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00002000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00000000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x03500000
	.4byte 0x00000000
	.4byte 0x03600000
	.4byte 0x00015000
	.4byte 0xffff0015
	.4byte 0x00000001
	.4byte 0x03500000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00025000
	.4byte 0xffff0015
	.4byte 0x00000001
	.4byte 0x03300000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00025000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gItemLevelEvents
gItemLevelEvents:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008215
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020087c9
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020087d5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020080ed
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020084b1
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200804d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008071
	.4byte 0x00008e15
	.4byte 0xffff000a
	.4byte 0x02008769
	.4byte 0x10008e15
	.4byte 0xffff000a
	.4byte 0x020087b9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
