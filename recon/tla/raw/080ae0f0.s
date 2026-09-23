.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_080ad3f8, 0x080ad3f8
	.set sub_080afdbc, 0x080afdbc
	.set sub_080afdd8, 0x080afdd8
	.set sub_080afe1c, 0x080afe1c
	.set sub_080b0b78, 0x080b0b78
	.set sub_080b0c9c, 0x080b0c9c
	.set sub_080b0e40, 0x080b0e40
	.global Func_080ae0f0
	.thumb_func
Func_080ae0f0:
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #4
	str	r2, [r3, #0]
	movs	r0, #4
	bl	sub_080afdd8
	movs	r0, #5
	bl	sub_080afe1c
	movs	r0, #6
	bl	sub_080afe1c
	pop	{pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r6, #10
.L_080ae11c:
	adds	r0, r6, #0
	subs	r5, r6, #1
	adds	r0, #67
	bl	sub_08016cfc
	movs	r1, #1
	adds	r2, r5, #0
	movs	r0, #7
	bl	sub_080b0b78
	adds	r6, #1
	movs	r0, #7
	movs	r1, #1
	adds	r2, r5, #0
	bl	sub_080b0c9c
	cmp	r6, #11
	ble.n	.L_080ae11c
	movs	r0, #0
	bl	.L_080ae358
	bl	sub_080afdbc
	cmp	r0, #0
	ble.n	.L_080ae166
	ldr	r3, [pc, #24]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r5, r3, r2
	adds	r6, r0, #0
.L_080ae158:
	ldrb	r0, [r5, #0]
	subs	r6, #1
	adds	r5, #1
	bl	sub_080ad3f8
	cmp	r6, #0
	bne.n	.L_080ae158
.L_080ae166:
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	bl	sub_080afdbc
	movs	r6, #0
	adds	r7, r0, #0
	cmp	r6, r7
	bge.n	.L_080ae214
.L_080ae180:
	ldr	r2, [pc, #152]
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r6, r1
	ldrb	r0, [r2, r3]
	bl	sub_08016ca4
	adds	r5, r0, #0
	ldrh	r1, [r5, #52]
	ldrh	r3, [r5, #54]
	strh	r1, [r5, #56]
	strh	r3, [r5, #58]
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	lsls	r0, r1, #14
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080ae1b2
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080ae1b2
	adds	r3, r0, #0
.L_080ae1b2:
	strh	r3, [r5, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080ae1c6
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_080ae1c6
	movs	r3, #1
	strh	r3, [r5, #20]
.L_080ae1c6:
	movs	r3, #58
	ldrsh	r0, [r5, r3]
	movs	r2, #54
	ldrsh	r1, [r5, r2]
	lsls	r0, r0, #14
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080ae1e4
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080ae1e4
	adds	r3, r0, #0
.L_080ae1e4:
	strh	r3, [r5, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080ae1f8
	movs	r1, #58
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L_080ae1f8
	movs	r3, #1
	strh	r3, [r5, #22]
.L_080ae1f8:
	mov	r2, r8
	cmp	r2, #1
	bne.n	.L_080ae20e
	movs	r1, #50
	adds	r1, #255
	adds	r3, r5, r1
	movs	r2, #0
	adds	r1, #15
	strb	r2, [r3, #0]
	adds	r3, r5, r1
	strb	r2, [r3, #0]
.L_080ae20e:
	adds	r6, #1
	cmp	r6, r7
	blt.n	.L_080ae180
.L_080ae214:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	bl	sub_080afdbc
	movs	r7, #0
	mov	r8, r0
	cmp	r7, r8
	bge.n	.L_080ae2ec
.L_080ae232:
	ldr	r2, [pc, #192]
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldrb	r6, [r2, r3]
	ldr	r3, [pc, #184]
	movs	r5, #0
	ldrb	r3, [r3, r6]
	cmp	r3, #0
	bne.n	.L_080ae258
	movs	r0, #136
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ae270
	movs	r0, #137
	lsls	r0, r0, #1
	b.n	.L_080ae268
.L_080ae258:
	movs	r0, #18
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ae270
	movs	r0, #20
	adds	r0, #255
.L_080ae268:
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ae272
.L_080ae270:
	movs	r5, #1
.L_080ae272:
	cmp	r5, #0
	beq.n	.L_080ae2e6
	adds	r0, r6, #0
	bl	sub_08016ca4
	adds	r5, r0, #0
	ldrh	r3, [r5, #54]
	strh	r3, [r5, #58]
	movs	r2, #56
	ldrsh	r0, [r5, r2]
	movs	r3, #52
	ldrsh	r1, [r5, r3]
	lsls	r0, r0, #14
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080ae2a0
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080ae2a0
	adds	r3, r0, #0
.L_080ae2a0:
	strh	r3, [r5, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080ae2b4
	movs	r1, #56
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L_080ae2b4
	movs	r3, #1
	strh	r3, [r5, #20]
.L_080ae2b4:
	movs	r2, #58
	ldrsh	r0, [r5, r2]
	movs	r3, #54
	ldrsh	r1, [r5, r3]
	lsls	r0, r0, #14
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080ae2d2
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080ae2d2
	adds	r3, r0, #0
.L_080ae2d2:
	strh	r3, [r5, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080ae2e6
	movs	r1, #58
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L_080ae2e6
	movs	r3, #1
	strh	r3, [r5, #22]
.L_080ae2e6:
	adds	r7, #1
	cmp	r7, r8
	blt.n	.L_080ae232
.L_080ae2ec:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x127c
	.2byte 0x080b
.L_080ae2fc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	r8, r1
	mov	sl, r2
	bl	sub_08016ca4
	movs	r6, #1
	adds	r7, r0, #0
	negs	r6, r6
	movs	r3, #1
	adds	r7, #248
	adds	r5, r6, #0
	movs	r0, #0
	mov	ip, r3
.L_080ae31c:
	mov	r1, ip
	movs	r4, #0
	lsls	r1, r0
	adds	r2, r7, #0
.L_080ae324:
	ldmia	r2!, {r3}
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ae330
	adds	r5, r4, #0
	adds	r6, r0, #0
.L_080ae330:
	adds	r4, #1
	cmp	r4, #3
	ble.n	.L_080ae324
	adds	r0, #1
	cmp	r0, #19
	ble.n	.L_080ae31c
	movs	r3, #1
	negs	r3, r3
	adds	r0, r5, #0
	cmp	r5, r3
	beq.n	.L_080ae350
	mov	r3, r8
	str	r5, [r3, #0]
	mov	r3, sl
	str	r6, [r3, #0]
	movs	r0, #0
.L_080ae350:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
.L_080ae358:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	bl	sub_080afdbc
	mov	fp, r0
.L_080ae36e:
	movs	r2, #0
	movs	r4, #186
	lsls	r4, r4, #2
	mov	r9, r2
	mov	r8, r2
	adds	r4, #255
	mov	sl, r2
	cmp	r9, fp
	bge.n	.L_080ae3c4
	ldr	r3, [pc, #116]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r7, r3, r2
	mov	r6, fp
.L_080ae38a:
	ldrb	r5, [r7, #0]
	str	r4, [sp, #0]
	adds	r0, r5, #0
	bl	sub_08016ca4
	movs	r3, #140
	ldr	r4, [sp, #0]
	lsls	r3, r3, #1
	adds	r7, #1
	movs	r2, #0
	adds	r0, r0, r3
	movs	r1, #3
.L_080ae3a2:
	ldrb	r3, [r0, #0]
	subs	r1, #1
	adds	r0, #1
	adds	r2, r2, r3
	cmp	r1, #0
	bge.n	.L_080ae3a2
	cmp	r2, r8
	ble.n	.L_080ae3b6
	mov	r8, r2
	mov	sl, r5
.L_080ae3b6:
	cmp	r2, r4
	bge.n	.L_080ae3be
	adds	r4, r2, #0
	mov	r9, r5
.L_080ae3be:
	subs	r6, #1
	cmp	r6, #0
	bne.n	.L_080ae38a
.L_080ae3c4:
	mov	r2, r8
	subs	r3, r2, r4
	cmp	r3, #1
	ble.n	.L_080ae3ea
	add	r2, sp, #4
	mov	r0, sl
	add	r1, sp, #8
	bl	.L_080ae2fc
	adds	r2, r0, #0
	cmp	r2, #0
	bne.n	.L_080ae3ea
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #4]
	mov	r0, sl
	mov	r3, r9
	bl	sub_080b0e40
	b.n	.L_080ae36e
.L_080ae3ea:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	lsls	r3, r1, #20
	movs	r2, #7
	ands	r2, r1
	lsrs	r1, r3, #23
	ldrb	r0, [r0, r1]
	movs	r3, #1
	asrs	r0, r2
	ands	r0, r3
	bx	lr
	.align 2, 0
