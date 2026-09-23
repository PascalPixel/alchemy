.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08009080, 0x08009080
	.set sub_08009088, 0x08009088
	.set sub_08077008, 0x08077008
	.set sub_08077018, 0x08077018
	.set sub_08077058, 0x08077058
	.set sub_08077060, 0x08077060
	.set sub_080771a0, 0x080771a0
	.set sub_080b7dd0, 0x080b7dd0
	.set sub_080b7f70, 0x080b7f70
	.set sub_080b8000, 0x080b8000
	.set sub_080b9d34, 0x080b9d34
	.set sub_080bb938, 0x080bb938
	.set sub_080bbabc, 0x080bbabc
	.set sub_080be02c, 0x080be02c
	.set sub_080c10e8, 0x080c10e8
	.set sub_080c1724, 0x080c1724
	.set sub_080c9008, 0x080c9008
	.set sub_080c9018, 0x080c9018
	.global Func_080ba6ac
	.thumb_func
Func_080ba6ac:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #564]
	ldr	r1, [r3, #0]
	ldrb	r3, [r0, #0]
	mov	r8, r2
	sub	sp, #88
	mov	sl, r0
	ldr	r2, [pc, #556]
	cmp	r3, #4
	bhi.n	.L_080ba6cc
	movs	r2, #128
	lsls	r2, r2, #6
.L_080ba6cc:
	ldr	r3, [r1, #0]
	cmp	r3, r2
	beq.n	.L_080ba6d4
	str	r2, [r1, #0]
.L_080ba6d4:
	add	r5, sp, #4
	adds	r1, r5, #0
	mov	r0, sl
	bl	sub_080b9d34
	movs	r1, #0
	movs	r0, #0
	bl	sub_080c10e8
	ldr	r0, [r5, #8]
	bl	sub_080b7dd0
	ldr	r0, [r0, #0]
	movs	r1, #3
	mov	r9, r0
	bl	sub_08009080
	movs	r1, #16
	mov	r0, r9
	bl	sub_08009088
	mov	r1, sl
	ldrb	r3, [r1, #2]
	cmp	r3, #7
	bhi.n	.L_080ba70a
	movs	r3, #1
	b.n	.L_080ba70c
.L_080ba70a:
	movs	r3, #0
.L_080ba70c:
	str	r3, [r5, #4]
	ldr	r3, [r5, #20]
	movs	r7, #0
	adds	r2, r5, #0
	cmp	r3, #0
	beq.n	.L_080ba75e
	movs	r6, #0
.L_080ba71a:
	lsls	r3, r7, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_080b7dd0
	movs	r1, #0
	ldr	r0, [r0, #0]
	bl	sub_080b7f70
	adds	r3, r0, #0
	adds	r3, #39
	ldrb	r3, [r3, #0]
	subs	r3, #1
	movs	r1, #0
	cmp	r3, #0
	beq.n	.L_080ba752
	mov	ip, r3
	adds	r3, r6, r5
	adds	r2, r3, #0
	adds	r2, #52
	adds	r0, #40
.L_080ba744:
	ldmia	r0!, {r3}
	ldrb	r3, [r3, #5]
	adds	r1, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r1, ip
	bne.n	.L_080ba744
.L_080ba752:
	ldr	r3, [r5, #20]
	adds	r7, #1
	adds	r6, #4
	adds	r2, r5, #0
	cmp	r7, r3
	bne.n	.L_080ba71a
.L_080ba75e:
	movs	r1, #200
	ldr	r0, [pc, #400]
	lsls	r1, r1, #4
	bl	sub_080041d8
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080ba7c0
	movs	r7, #0
	movs	r6, #0
.L_080ba772:
	ldr	r3, [pc, #388]
	ldr	r3, [r3, #0]
	cmp	r7, #19
	bgt.n	.L_080ba792
	ldr	r2, [pc, #384]
	ldr	r1, [pc, #384]
	adds	r0, r3, r2
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r1
	subs	r2, r2, r6
	str	r2, [r3, #0]
	ldr	r1, [pc, #376]
	movs	r3, #128
	bl	sub_080c1724
.L_080ba792:
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [pc, #364]
	adds	r7, #1
	adds	r6, r6, r2
	cmp	r7, #19
	ble.n	.L_080ba772
	mov	r6, sl
	ldr	r3, [r6, #88]
	movs	r2, #128
	lsls	r2, r2, #7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ba7b8
	adds	r0, r5, #0
	bl	sub_080c9008
	b.n	.L_080ba7c6
.L_080ba7b8:
	adds	r0, r5, #0
	bl	sub_080c9018
	b.n	.L_080ba7c6
.L_080ba7c0:
	movs	r0, #60
	bl	sub_080030f8
.L_080ba7c6:
	bl	sub_080be02c
	adds	r6, r5, #0
	mov	r0, r9
	movs	r1, #1
	bl	sub_08009080
	ldr	r3, [r6, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_080ba7f4
	movs	r2, #36
.L_080ba7de:
	ldrsh	r0, [r6, r2]
	str	r2, [sp, #0]
	bl	sub_080b8000
	adds	r5, r6, #0
	ldr	r2, [sp, #0]
	ldr	r3, [r5, #20]
	adds	r7, #1
	adds	r2, #2
	cmp	r7, r3
	bne.n	.L_080ba7de
.L_080ba7f4:
	mov	r3, r8
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_08077008
	mov	r1, r8
	movs	r7, #8
	ldrsh	r3, [r1, r7]
	lsls	r3, r3, #1
	adds	r3, #216
	adds	r6, r0, #0
	ldrh	r5, [r6, r3]
	adds	r0, r5, #0
	bl	sub_08077018
	ldrb	r2, [r0, #12]
	adds	r3, r2, #0
	cmp	r3, #1
	bne.n	.L_080ba87e
	mov	r3, r8
	movs	r6, #8
	ldrsh	r1, [r3, r6]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_08077058
	mov	r1, r8
	movs	r7, #8
	ldrsh	r5, [r1, r7]
	cmp	r0, #2
	bne.n	.L_080ba8da
	ldr	r3, [pc, #196]
	ldr	r0, [r3, #0]
	movs	r4, #0
.L_080ba838:
	movs	r2, #188
	lsls	r1, r4, #4
	lsls	r2, r2, #2
	adds	r3, r1, r2
	adds	r3, r0, r3
	movs	r6, #2
	ldrsh	r3, [r3, r6]
	cmp	r3, #2
	bne.n	.L_080ba876
	movs	r7, #187
	lsls	r7, r7, #2
	adds	r3, r1, r7
	ldrsh	r2, [r0, r3]
	mov	r6, r8
	movs	r7, #0
	ldrsh	r3, [r6, r7]
	cmp	r2, r3
	bne.n	.L_080ba876
	movs	r7, #189
	lsls	r7, r7, #2
	adds	r1, r1, r7
	ldrsh	r2, [r0, r1]
	ldrh	r3, [r0, r1]
	cmp	r2, r5
	bne.n	.L_080ba86e
	ldr	r3, [pc, #160]
	b.n	.L_080ba874
.L_080ba86e:
	cmp	r2, r5
	ble.n	.L_080ba876
	subs	r3, #1
.L_080ba874:
	strh	r3, [r0, r1]
.L_080ba876:
	adds	r4, #1
	cmp	r4, #19
	bls.n	.L_080ba838
	b.n	.L_080ba8da
.L_080ba87e:
	lsls	r3, r2, #24
	lsrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_080ba8c0
	bl	sub_080771a0
	movs	r3, #7
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_080ba8da
	mov	r1, r8
	movs	r7, #8
	ldrsh	r3, [r1, r7]
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r1, [r6, r3]
	movs	r0, #2
	bl	sub_080bbabc
	ldr	r1, [pc, #104]
	movs	r0, #4
	bl	sub_080bbabc
	mov	r3, r8
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	movs	r6, #8
	ldrsh	r1, [r3, r6]
	bl	sub_08077060
	bl	sub_080bb938
	b.n	.L_080ba8da
.L_080ba8c0:
	cmp	r3, #4
	bne.n	.L_080ba8da
	ldr	r3, [pc, #76]
	ands	r3, r5
	cmp	r3, #184
	bne.n	.L_080ba8ce
	movs	r5, #185
.L_080ba8ce:
	mov	r1, r8
	movs	r7, #8
	ldrsh	r3, [r1, r7]
	lsls	r3, r3, #1
	adds	r3, #216
	strh	r5, [r6, r3]
.L_080ba8da:
	movs	r0, #0
	add	sp, #88
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f00
	.4byte 0xffffe000
	.4byte 0x080bd899
	.4byte 0x03001e74
	.4byte 0x00000544
	.4byte 0x00000644
	.4byte 0x050000c0
	.4byte 0x00000444
	.4byte 0x0000ffff
	.4byte 0x0000081c
	.4byte 0x000001ff
