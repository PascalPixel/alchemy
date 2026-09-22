.syntax unified
	.thumb
	.set sub_08077008, 0x08077008
	.set sub_08077018, 0x08077018
	.set sub_08077080, 0x08077080
	.set sub_08077198, 0x08077198
	.set sub_080771a0, 0x080771a0
	.set sub_080b9a70, 0x080b9a70
	.set sub_080bad7c, 0x080bad7c
	.set sub_080bae40, 0x080bae40
	.set sub_080bd3c8, 0x080bd3c8
	.set sub_080bd3e4, 0x080bd3e4
	.global Overlay_080bd424
Overlay_080bd424:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	str	r1, [sp, #24]
	adds	r7, r0, #0
	movs	r1, #0
	ldrsh	r0, [r7, r1]
	bl	sub_08077008
	movs	r3, #0
	movs	r2, #1
	str	r3, [sp, #12]
	ldr	r3, [pc, #560]
	mov	fp, r0
	str	r2, [sp, #16]
	str	r2, [sp, #8]
	add	r3, fp
	movs	r0, #1
	ldrb	r3, [r3, #0]
	negs	r0, r0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_080bd45e
	b.n	.L_080bd792
.L_080bd45e:
	ldr	r1, [sp, #24]
	cmp	r1, #0
	beq.n	.L_080bd46e
	movs	r2, #6
	ldrsh	r3, [r7, r2]
	cmp	r3, #4
	beq.n	.L_080bd46e
	b.n	.L_080bd792
.L_080bd46e:
	movs	r3, #148
	lsls	r3, r3, #1
	add	r3, fp
	ldrb	r0, [r3, #0]
	bl	sub_08077198
	str	r0, [sp, #20]
	ldr	r1, [sp, #20]
	movs	r3, #144
	lsls	r3, r3, #1
	adds	r0, #54
	adds	r1, #55
	add	r3, fp
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	mov	r8, r3
.L_080bd48e:
	ldr	r2, [sp, #4]
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #6
	bhi.n	.L_080bd534
	ldr	r2, [pc, #480]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080bd4bc
	.4byte 0x080bd4c4
	.4byte 0x080bd4cc
	.4byte 0x080bd4da
	.4byte 0x080bd504
	.4byte 0x080bd530
	.2byte 0xd534
	.2byte 0x080b
	add	r3, sp, #28
	ldr	r0, [pc, #448]
	mov	r9, r3
	b.n	.L_080bd4d2
	add	r1, sp, #28
	ldr	r0, [pc, #444]
	mov	r9, r1
	b.n	.L_080bd4d2
	add	r2, sp, #28
	ldr	r0, [pc, #440]
	mov	r9, r2
.L_080bd4d2:
	bl	sub_080bd3e4
	mov	sl, r0
	b.n	.L_080bd534
	mov	r3, r8
	ldr	r2, [r3, #0]
	lsls	r3, r2, #31
	cmp	r3, #0
	bne.n	.L_080bd508
	bl	sub_080771a0
	mov	r1, r8
	movs	r3, #7
	ldrb	r2, [r1, #0]
	ands	r0, r3
	movs	r3, #15
	negs	r3, r3
	ands	r3, r2
	lsls	r0, r0, #1
	movs	r2, #1
	orrs	r3, r0
	orrs	r3, r2
	strb	r3, [r1, #0]
	ldr	r2, [r1, #0]
	b.n	.L_080bd508
	mov	r3, r8
	ldr	r2, [r3, #0]
.L_080bd508:
	lsls	r3, r2, #28
	ldr	r0, [sp, #24]
	lsrs	r3, r3, #29
	mov	sl, r3
	cmp	r0, #0
	beq.n	.L_080bd534
	mov	r2, sl
	movs	r3, #7
	adds	r2, #1
	ands	r2, r3
	mov	r3, r8
	ldrb	r1, [r3, #0]
	movs	r3, #15
	negs	r3, r3
	lsls	r2, r2, #1
	ands	r3, r1
	orrs	r3, r2
	mov	r0, r8
	strb	r3, [r0, #0]
	b.n	.L_080bd534
	movs	r1, #1
	add	sl, r1
.L_080bd534:
	ldr	r2, [sp, #0]
	ldrb	r6, [r2, #0]
	mov	r3, sl
	asrs	r6, r3
	mov	r1, sl
	movs	r3, #1
	ands	r6, r3
	ldr	r2, [sp, #20]
	lsls	r3, r1, #1
	adds	r3, #56
	ldr	r0, [sp, #16]
	ldrh	r3, [r2, r3]
	ands	r6, r0
	mov	r9, r3
	movs	r3, #4
	strh	r3, [r7, #6]
	cmp	r6, #0
	beq.n	.L_080bd5be
	ldr	r3, [sp, #24]
	cmp	r3, #0
	beq.n	.L_080bd5be
	mov	r1, fp
	adds	r1, #216
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #292]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080bd586
	ldr	r3, [sp, #20]
	adds	r3, #53
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L_080bd586
	movs	r3, #2
	strh	r3, [r7, #6]
	ldr	r3, [pc, #268]
	strh	r3, [r7, #8]
	b.n	.L_080bd792
.L_080bd586:
	cmp	r6, #0
	beq.n	.L_080bd5ba
	ldrh	r0, [r1, #0]
	bl	sub_08077018
	adds	r5, r0, #0
	ldrb	r3, [r5, #12]
	cmp	r3, #1
	bne.n	.L_080bd5b4
	ldrh	r0, [r5, #40]
	bl	sub_08077080
	movs	r3, #2
	ldrh	r5, [r5, #40]
	strh	r3, [r7, #6]
	ldrb	r3, [r0, #1]
	movs	r2, #0
	mov	r9, r5
	strh	r2, [r7, #8]
	cmp	r3, #2
	bgt.n	.L_080bd5b4
	cmp	r3, #1
	bge.n	.L_080bd5b6
.L_080bd5b4:
	movs	r6, #0
.L_080bd5b6:
	cmp	r6, #0
	bne.n	.L_080bd5be
.L_080bd5ba:
	movs	r0, #0
	str	r0, [sp, #16]
.L_080bd5be:
	ldr	r1, [sp, #8]
	cmp	r1, #0
	bne.n	.L_080bd5c6
	b.n	.L_080bd766
.L_080bd5c6:
	mov	r0, r9
	bl	sub_08077080
	adds	r5, r0, #0
	ldrb	r3, [r5, #3]
	cmp	r3, #47
	beq.n	.L_080bd5ee
	cmp	r3, #47
	bgt.n	.L_080bd5de
	cmp	r3, #46
	beq.n	.L_080bd5e4
	b.n	.L_080bd606
.L_080bd5de:
	cmp	r3, #49
	beq.n	.L_080bd5f8
	b.n	.L_080bd606
.L_080bd5e4:
	movs	r3, #3
	strh	r3, [r7, #6]
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	b.n	.L_080bd600
.L_080bd5ee:
	movs	r3, #7
	strh	r3, [r7, #6]
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	b.n	.L_080bd600
.L_080bd5f8:
	movs	r3, #99
	strh	r3, [r7, #6]
	movs	r1, #0
	ldrsh	r0, [r7, r1]
.L_080bd600:
	bl	sub_080b9a70
	strh	r0, [r7, #10]
.L_080bd606:
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L_080bd61e
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	ldrh	r2, [r7, #6]
	cmp	r3, #3
	beq.n	.L_080bd620
	cmp	r3, #7
	beq.n	.L_080bd61c
	b.n	.L_080bd792
.L_080bd61c:
	b.n	.L_080bd620
.L_080bd61e:
	ldrh	r2, [r7, #6]
.L_080bd620:
	movs	r1, #128
	lsls	r3, r2, #16
	lsls	r1, r1, #10
	cmp	r3, r1
	beq.n	.L_080bd69a
	mov	r0, r9
	bl	sub_080bd3c8
	cmp	r0, #0
	beq.n	.L_080bd674
	movs	r3, #1
	mov	r2, r9
	mov	r1, fp
	strh	r3, [r7, #6]
	strh	r2, [r7, #8]
	movs	r0, #58
	ldrsh	r3, [r1, r0]
	ldrb	r2, [r5, #9]
	cmp	r2, r3
	ble.n	.L_080bd658
	ldr	r3, [sp, #20]
	adds	r3, #53
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080bd658
	b.n	.L_080bd770
.L_080bd658:
	ldr	r3, [pc, #56]
	add	r3, fp
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bd698
	ldr	r3, [sp, #20]
	adds	r3, #53
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_080bd672
	b.n	.L_080bd770
.L_080bd672:
	b.n	.L_080bd698
.L_080bd674:
	ldrh	r2, [r7, #6]
	b.n	.L_080bd69a
	.4byte 0x00000129
	.4byte 0x080bd4a0
	.4byte 0x080c2b80
	.4byte 0x080c2b88
	.4byte 0x080c2b90
	.4byte 0x000001ff
	.4byte 0x000001fd
	.2byte 0x013d
	.2byte 0x0000
.L_080bd698:
	ldr	r2, [pc, #36]
.L_080bd69a:
	lsls	r3, r2, #16
	asrs	r2, r3, #16
	cmp	r2, #99
	bne.n	.L_080bd6ae
	movs	r3, #164
	lsls	r3, r3, #1
	add	r3, fp
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080bd770
.L_080bd6ae:
	ldr	r3, [sp, #24]
	cmp	r3, #0
	beq.n	.L_080bd6c4
	cmp	r2, #3
	beq.n	.L_080bd770
	cmp	r2, #7
	beq.n	.L_080bd770
	b.n	.L_080bd6c4
	movs	r0, r0
	.2byte 0x0001
	.2byte 0x0000
.L_080bd6c4:
	cmp	r2, #4
	bne.n	.L_080bd6d6
	mov	r0, r9
	mov	r1, r9
	movs	r3, #0
	strh	r0, [r7, #8]
	cmp	r1, #1
	bne.n	.L_080bd6d6
	strh	r3, [r7, #6]
.L_080bd6d6:
	ldrb	r3, [r5, #8]
	strh	r3, [r7, #12]
	ldrb	r3, [r5, #0]
	cmp	r3, #2
	beq.n	.L_080bd6f2
	cmp	r3, #2
	bgt.n	.L_080bd6ea
	cmp	r3, #1
	beq.n	.L_080bd722
	b.n	.L_080bd758
.L_080bd6ea:
	cmp	r3, #3
	beq.n	.L_080bd74c
	cmp	r3, #4
	bne.n	.L_080bd758
.L_080bd6f2:
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	bl	sub_080bae40
	movs	r3, #2
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_080bd712
	ldrh	r3, [r7, #0]
	movs	r0, #0
	cmp	r3, #7
	bhi.n	.L_080bd70e
	movs	r0, #1
.L_080bd70e:
	bl	sub_080bad7c
.L_080bd712:
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_080bd766
	movs	r2, #0
	strh	r0, [r7, #10]
	str	r2, [sp, #8]
	b.n	.L_080bd766
.L_080bd722:
	adds	r1, r5, #0
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	bl	sub_080bae40
	movs	r1, #2
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_080bd742
	ldrh	r3, [r7, #0]
	movs	r0, #0
	cmp	r3, #7
	bhi.n	.L_080bd73e
	movs	r0, #1
.L_080bd73e:
	bl	sub_080bad7c
.L_080bd742:
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_080bd766
	b.n	.L_080bd760
.L_080bd74c:
	movs	r1, #0
	ldrsh	r0, [r7, r1]
	bl	sub_080b9a70
	strh	r0, [r7, #10]
	b.n	.L_080bd766
.L_080bd758:
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	bl	sub_080b9a70
.L_080bd760:
	movs	r3, #0
	strh	r0, [r7, #10]
	str	r3, [sp, #8]
.L_080bd766:
	ldr	r0, [sp, #24]
	cmp	r0, #0
	bne.n	.L_080bd770
	movs	r1, #0
	str	r1, [sp, #8]
.L_080bd770:
	ldr	r2, [sp, #8]
	cmp	r2, #0
	beq.n	.L_080bd784
	ldr	r3, [sp, #12]
	cmp	r3, #16
	ble.n	.L_080bd784
	movs	r3, #3
	movs	r0, #0
	strh	r3, [r7, #6]
	str	r0, [sp, #8]
.L_080bd784:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #8]
	adds	r1, #1
	str	r1, [sp, #12]
	cmp	r2, #0
	beq.n	.L_080bd792
	b.n	.L_080bd48e
.L_080bd792:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
