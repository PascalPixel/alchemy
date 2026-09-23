.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08016ca4, 0x08016ca4
	.set sub_080b0028, 0x080b0028
	.set sub_080b039c, 0x080b039c
	.set sub_080b0514, 0x080b0514
	.set sub_080b06a8, 0x080b06a8
	.set sub_080b06c8, 0x080b06c8
	.set sub_080b06e8, 0x080b06e8
	.set sub_080b0740, 0x080b0740
	.global Func_080b075c
	.thumb_func
Func_080b075c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r1
	sub	sp, #4
	adds	r5, r0, #0
	mov	r0, fp
	mov	r8, r3
	str	r2, [sp, #0]
	bl	sub_08016ca4
	mov	r1, r8
	adds	r6, r0, #0
	movs	r0, #1
	mov	r9, r0
	cmp	r1, #90
	bne.n	.L_080b0794
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrh	r0, [r3, #0]
	bl	sub_080b06a8
	b.n	.L_080b0a82
.L_080b0794:
	mov	r3, r8
	cmp	r3, #91
	bne.n	.L_080b07a8
	movs	r4, #165
	lsls	r4, r4, #1
	adds	r3, r6, r4
	ldrh	r0, [r3, #0]
	bl	sub_080b06c8
	b.n	.L_080b0a82
.L_080b07a8:
	mov	r0, r8
	bl	sub_080b0740
	cmp	r0, #0
	beq.n	.L_080b07be
	movs	r7, #56
	ldrsh	r3, [r6, r7]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080b07be
	b.n	.L_080b0a82
.L_080b07be:
	mov	r0, r8
	cmp	r0, #3
	bne.n	.L_080b0808
	movs	r1, #50
	adds	r1, #255
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080b0808
	b.n	.L_080b0a80
.L_080b07d6:
	movs	r2, #60
	adds	r2, #255
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b0832
	movs	r4, #158
	lsls	r4, r4, #1
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b0832
	movs	r7, #62
	adds	r7, #255
	adds	r3, r6, r7
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b0832
	movs	r0, #66
	adds	r0, #255
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b0832
	b.n	.L_080b0a80
.L_080b0808:
	mov	r1, r8
	cmp	r1, #4
	bne.n	.L_080b0832
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b0832
	movs	r4, #58
	adds	r4, #255
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b0832
	movs	r7, #157
	lsls	r7, r7, #1
	adds	r3, r6, r7
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080b07d6
.L_080b0832:
	movs	r1, #156
	movs	r4, #58
	lsls	r1, r1, #1
	adds	r4, #255
	movs	r0, #50
	movs	r7, #157
	adds	r0, #255
	lsls	r7, r7, #1
	adds	r2, r6, r1
	adds	r1, r6, r4
	adds	r4, #2
	adds	r4, r4, r6
	adds	r3, r6, r0
	adds	r0, r6, r7
	adds	r7, #2
	mov	ip, r4
	adds	r4, r6, r7
	mov	r7, r8
	cmp	r7, #64
	bne.n	.L_080b08ac
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080b08ac
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080b08ac
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_080b08ac
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_080b08ac
	mov	r0, ip
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_080b08ac
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	bne.n	.L_080b08ac
	movs	r1, #62
	adds	r1, #255
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b08ac
	movs	r2, #66
	adds	r2, #255
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b08ac
	movs	r4, #160
	lsls	r4, r4, #1
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_080b08ac
	b.n	.L_080b0a82
.L_080b08ac:
	mov	r7, r8
	cmp	r7, #28
	bne.n	.L_080b08c2
	movs	r0, #66
	adds	r0, #255
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	movs	r0, #0
	cmp	r3, #1
	bne.n	.L_080b08c2
	b.n	.L_080b0a82
.L_080b08c2:
	movs	r1, #66
	adds	r1, r1, r6
	ldrb	r3, [r1, #0]
	mov	sl, r1
	cmp	r3, #39
	bhi.n	.L_080b08d0
	b.n	.L_080b09e8
.L_080b08d0:
	mov	r3, r8
	subs	r3, #18
	cmp	r3, #64
	bls.n	.L_080b08da
	b.n	.L_080b09e8
.L_080b08da:
	ldr	r2, [pc, #436]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080b0a80
	.4byte 0x080b0a80
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b0a80
	.4byte 0x080b0a80
	.4byte 0x080b0a80
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b0a80
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b09e8
	.4byte 0x080b0a80
	.4byte 0x080b09e8
	.2byte 0x0a80
	.2byte 0x080b
.L_080b09e8:
	movs	r2, #42
	adds	r2, #255
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080b09fc
	mov	r3, r8
	movs	r0, #0
	cmp	r3, #82
	beq.n	.L_080b0a82
.L_080b09fc:
	mov	r0, r8
	bl	sub_080b0514
	adds	r7, r0, #0
	cmp	r7, #0
	ble.n	.L_080b0a3e
	ldr	r1, [sp, #0]
	adds	r0, r5, #0
	bl	sub_080b0028
	adds	r5, r0, #0
	ldr	r1, [sp, #0]
	mov	r0, fp
	bl	sub_080b0028
	mov	r4, sl
	ldrb	r3, [r4, #0]
	subs	r5, r5, r0
	lsrs	r3, r3, #1
	subs	r5, r5, r3
	lsls	r3, r5, #1
	adds	r3, r3, r5
	adds	r0, r6, #0
	mov	r1, r8
	adds	r7, r7, r3
	bl	sub_080b06e8
	cmp	r0, #0
	beq.n	.L_080b0a40
	adds	r7, #25
	b.n	.L_080b0a40
.L_080b0a3a:
	movs	r0, #1
	b.n	.L_080b0a82
.L_080b0a3e:
	negs	r7, r7
.L_080b0a40:
	mov	r0, r8
	cmp	r0, #67
	bne.n	.L_080b0a4a
	movs	r1, #3
	mov	r9, r1
.L_080b0a4a:
	mov	r2, r8
	cmp	r2, #80
	bne.n	.L_080b0a54
	movs	r3, #3
	mov	r9, r3
.L_080b0a54:
	mov	r4, r8
	cmp	r4, #85
	bne.n	.L_080b0a5e
	movs	r0, #3
	mov	r9, r0
.L_080b0a5e:
	movs	r6, #0
	cmp	r6, r9
	bge.n	.L_080b0a80
	ldr	r3, [sp, #36]
	muls	r7, r3
.L_080b0a68:
	adds	r0, r7, #0
	movs	r1, #100
	bl	sub_08002054
	adds	r5, r0, #0
	bl	sub_080b039c
	cmp	r5, r0
	bge.n	.L_080b0a3a
	adds	r6, #1
	cmp	r6, r9
	blt.n	.L_080b0a68
.L_080b0a80:
	movs	r0, #0
.L_080b0a82:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x080b08e4
