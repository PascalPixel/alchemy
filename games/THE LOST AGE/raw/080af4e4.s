.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080aec04, 0x080aec04
	.set sub_080b0288, 0x080b0288
	.set sub_080b02d4, 0x080b02d4
	.global Overlay_080af4e4
Overlay_080af4e4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	bl	sub_08016ca4
	movs	r5, #42
	mov	r9, r0
	adds	r5, #255
	movs	r0, #88
	add	r0, r9
	add	r5, r9
	mov	r8, r0
	ldrb	r0, [r5, #0]
	bl	sub_080b0288
	ldrb	r3, [r5, #0]
	movs	r4, #192
	mov	fp, r0
	lsls	r4, r4, #8
	ldr	r1, [pc, #12]
	mov	r2, r8
	movs	r6, #31
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_080af522
	b.n	.L_080af6e6
.L_080af522:
	b.n	.L_080af528
	.2byte 0x0000
	.2byte 0x0000
.L_080af528:
	ldrh	r3, [r2, #0]
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080af532
	strh	r1, [r2, #0]
.L_080af532:
	subs	r6, #1
	adds	r2, #4
	cmp	r6, #0
	bge.n	.L_080af528
	mov	r1, r8
	movs	r4, #31
	movs	r6, #31
	adds	r1, #124
.L_080af542:
	lsls	r3, r4, #2
	mov	r0, r8
	ldrh	r2, [r3, r0]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_080af552
	subs	r4, #1
	b.n	.L_080af55a
.L_080af552:
	strh	r2, [r1, #0]
	subs	r4, #1
	subs	r1, #4
	subs	r6, #1
.L_080af55a:
	cmp	r4, #0
	bge.n	.L_080af542
	cmp	r6, #0
	blt.n	.L_080af578
	ldr	r2, [pc, #16]
	lsls	r3, r6, #2
	add	r3, r8
.L_080af568:
	subs	r6, #1
	strh	r2, [r3, #0]
	subs	r3, #4
	cmp	r6, #0
	bge.n	.L_080af568
	b.n	.L_080af578
	.2byte 0x0000
	.2byte 0x0000
.L_080af578:
	movs	r1, #16
	add	r1, fp
	mov	lr, r1
	movs	r2, #16
	movs	r6, #0
	mov	sl, r2
	mov	r7, lr
.L_080af586:
	ldrh	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_080af5f4
	mov	r3, r9
	mov	r0, lr
	ldrb	r2, [r3, #15]
	ldrb	r3, [r0, #2]
	cmp	r2, r3
	bcc.n	.L_080af5f4
	mov	r1, r8
	ldrh	r5, [r1, #0]
	ldrh	r3, [r0, #0]
	mov	ip, r5
	movs	r4, #0
	cmp	ip, r3
	beq.n	.L_080af5bc
	mov	ip, sl
.L_080af5a8:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L_080af5bc
	adds	r1, #4
	mov	r3, fp
	mov	r0, ip
	ldrh	r2, [r1, #0]
	ldrh	r3, [r3, r0]
	cmp	r2, r3
	bne.n	.L_080af5a8
.L_080af5bc:
	cmp	r4, #32
	bne.n	.L_080af5f4
	adds	r3, r5, #0
	movs	r4, #0
	cmp	r3, #0
	bne.n	.L_080af5d4
	ldrh	r3, [r7, #0]
	movs	r1, #128
	lsls	r1, r1, #8
	orrs	r3, r1
	mov	r2, r8
	b.n	.L_080af5ee
.L_080af5d4:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L_080af5f0
	lsls	r3, r4, #2
	mov	r0, r8
	adds	r2, r3, r0
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080af5d4
	ldrh	r3, [r7, #0]
	movs	r1, #128
	lsls	r1, r1, #8
	orrs	r3, r1
.L_080af5ee:
	strh	r3, [r2, #0]
.L_080af5f0:
	cmp	r4, #32
	beq.n	.L_080af602
.L_080af5f4:
	movs	r2, #4
	adds	r6, #1
	add	lr, r2
	adds	r7, #4
	add	sl, r2
	cmp	r6, #15
	ble.n	.L_080af586
.L_080af602:
	movs	r3, #216
	movs	r6, #0
	mov	sl, r3
.L_080af608:
	mov	r0, sl
	mov	r1, r9
	ldrh	r2, [r0, r1]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080af694
	ldr	r3, [pc, #36]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080af694
	ldrh	r0, [r0, r1]
	bl	sub_080aec04
	ldrb	r3, [r0, #12]
	cmp	r3, #3
	bne.n	.L_080af694
	mov	r2, r8
	ldrh	r5, [r2, #0]
	ldr	r3, [pc, #16]
	ldrh	r0, [r0, #40]
	ands	r3, r5
	mov	ip, r0
	movs	r4, #0
	cmp	r3, ip
	beq.n	.L_080af65e
	b.n	.L_080af644
	.4byte 0x00000200
	.2byte 0x3fff
	.2byte 0x0000
.L_080af644:
	movs	r7, #252
	lsls	r7, r7, #6
	adds	r7, #255
	mov	r1, r8
.L_080af64c:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L_080af65e
	adds	r1, #4
	ldrh	r2, [r1, #0]
	adds	r3, r7, #0
	ands	r3, r2
	cmp	r3, ip
	bne.n	.L_080af64c
.L_080af65e:
	cmp	r4, #32
	bne.n	.L_080af694
	adds	r3, r5, #0
	movs	r4, #0
	cmp	r3, #0
	bne.n	.L_080af678
	ldr	r3, [pc, #8]
	orrs	r3, r0
	mov	r0, r8
	strh	r3, [r0, #0]
	b.n	.L_080af690
	.2byte 0x4000
	.2byte 0x0000
.L_080af678:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L_080af690
	lsls	r3, r4, #2
	mov	r1, r8
	adds	r2, r3, r1
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080af678
	ldr	r3, [pc, #40]
	orrs	r3, r0
	strh	r3, [r2, #0]
.L_080af690:
	cmp	r4, #32
	beq.n	.L_080af69e
.L_080af694:
	movs	r2, #2
	adds	r6, #1
	add	sl, r2
	cmp	r6, #14
	ble.n	.L_080af608
.L_080af69e:
	movs	r4, #0
	movs	r6, #0
	mov	r1, r8
.L_080af6a4:
	lsls	r3, r4, #2
	mov	r0, r8
	ldrh	r2, [r3, r0]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_080af6b8
	adds	r4, #1
	b.n	.L_080af6c0
	.2byte 0x4000
	.2byte 0x0000
.L_080af6b8:
	strh	r2, [r1, #0]
	adds	r4, #1
	adds	r1, #4
	adds	r6, #1
.L_080af6c0:
	cmp	r4, #31
	ble.n	.L_080af6a4
	cmp	r6, #31
	bgt.n	.L_080af6e4
	lsls	r3, r6, #2
	mov	r0, r8
	ldr	r1, [pc, #16]
	adds	r2, r3, r0
	movs	r3, #32
	subs	r6, r3, r6
.L_080af6d4:
	subs	r6, #1
	strh	r1, [r2, #0]
	adds	r2, #4
	cmp	r6, #0
	bne.n	.L_080af6d4
	b.n	.L_080af6e4
	.2byte 0x0000
	.2byte 0x0000
.L_080af6e4:
	movs	r0, #0
.L_080af6e6:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r7, r0, #0
	bl	sub_08016ca4
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	adds	r5, r0, #0
	movs	r0, #88
	mov	ip, r3
	ands	r6, r3
	ldrh	r3, [r0, r5]
	mov	r2, ip
	movs	r4, #1
	ands	r2, r3
	negs	r4, r4
	movs	r1, #0
	cmp	r2, r6
	bne.n	.L_080af722
	strh	r2, [r0, r5]
	movs	r4, #0
	b.n	.L_080af738
.L_080af722:
	adds	r1, #1
	adds	r0, #4
	cmp	r1, #30
	bgt.n	.L_080af738
	ldrh	r3, [r0, r5]
	mov	r2, ip
	ands	r2, r3
	cmp	r2, r6
	bne.n	.L_080af722
	strh	r2, [r0, r5]
	adds	r4, r1, #0
.L_080af738:
	cmp	r4, #0
	bge.n	.L_080af76c
	movs	r2, #88
	ldrh	r3, [r5, r2]
	movs	r1, #0
	cmp	r3, #0
	bne.n	.L_080af74c
	strh	r6, [r5, r2]
	movs	r4, #0
	b.n	.L_080af762
.L_080af74c:
	adds	r1, #1
	cmp	r1, #30
	bgt.n	.L_080af762
	lsls	r3, r1, #2
	adds	r2, r3, #0
	adds	r2, #88
	ldrh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L_080af74c
	strh	r6, [r5, r2]
	adds	r4, r1, #0
.L_080af762:
	cmp	r4, #0
	bge.n	.L_080af76c
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080af790
.L_080af76c:
	adds	r0, r7, #0
	bl	sub_080b02d4
	movs	r3, #88
	ldrh	r3, [r5, r3]
	movs	r1, #0
	cmp	r3, r6
	beq.n	.L_080af78e
	adds	r0, r5, #0
	adds	r0, #88
.L_080af780:
	adds	r1, #1
	cmp	r1, #31
	bgt.n	.L_080af78e
	adds	r0, #4
	ldrh	r3, [r0, #0]
	cmp	r3, r6
	bne.n	.L_080af780
.L_080af78e:
	adds	r0, r1, #0
.L_080af790:
	pop	{r5, r6, r7, pc}
