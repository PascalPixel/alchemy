.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08002096, 0x08002096
	.set sub_08013560, 0x08013560
	.set sub_080138a8, 0x080138a8
	.set sub_0801401c, 0x0801401c
	.set sub_08014128, 0x08014128
	.set sub_08014274, 0x08014274
	.set sub_080143e0, 0x080143e0
	.set sub_080143f8, 0x080143f8
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_08039418, 0x08039418
	.set sub_080396a0, 0x080396a0
	.set sub_080396bc, 0x080396bc
	.set sub_0803aaa4, 0x0803aaa4
	.set sub_0803aae4, 0x0803aae4
	.set sub_0803c378, 0x0803c378
	.set sub_0803c9bc, 0x0803c9bc
	.set sub_0803d98c, 0x0803d98c
	.set sub_08041f70, 0x08041f70
	.set sub_08042010, 0x08042010
	.set sub_080420fc, 0x080420fc
	.set sub_08042214, 0x08042214
	.set sub_08042244, 0x08042244
	.set sub_0804524c, 0x0804524c
	.set sub_08045330, 0x08045330
	.set sub_080454a0, 0x080454a0
	.set sub_080455b0, 0x080455b0
	.set sub_08046134, 0x08046134
	.set sub_080499b4, 0x080499b4
	.set sub_08049a04, 0x08049a04
	.set sub_080ad010, 0x080ad010
	.set sub_080ad078, 0x080ad078
	.set sub_080ad280, 0x080ad280
	.set sub_081180b8, 0x081180b8
	.set sub_081180d0, 0x081180d0
	.set sub_081180e0, 0x081180e0
	.set sub_081c0010, 0x081c0010
	.global Func_08049a30
	.thumb_func
Func_08049a30:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #304
	str	r0, [sp, #80]
	str	r2, [sp, #72]
	str	r1, [sp, #76]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r1, [r5, #60]
	movs	r2, #1
	negs	r2, r2
	movs	r0, #128
	str	r1, [sp, #68]
	str	r2, [sp, #64]
	str	r2, [sp, #60]
	bl	sub_080143e0
	movs	r3, #42
	str	r0, [sp, #56]
	str	r3, [sp, #0]
	movs	r1, #5
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	bl	sub_08039260
	movs	r3, #0
	str	r0, [sp, #52]
	str	r3, [sp, #48]
	adds	r5, #228
	ldr	r3, [r5, #0]
	ldr	r1, [r3, #52]
	ldr	r2, [r3, #48]
	ldr	r3, [r3, #56]
	mov	r9, r1
	mov	r8, r2
	str	r3, [sp, #44]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r0, #15
	movs	r3, #11
	movs	r1, #9
	movs	r2, #15
	bl	sub_08039260
	mov	r3, sp
	adds	r3, #84
	str	r3, [sp, #28]
	ldr	r6, [pc, #84]
	movs	r5, #128
	mov	fp, r0
	movs	r7, #0
	adds	r4, r3, #0
	lsls	r5, r5, #23
.L_08049aa6:
	movs	r3, #0
	lsls	r0, r7, #1
	str	r5, [r4, #4]
	str	r3, [r4, #8]
	mov	r3, fp
	movs	r1, #12
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #52]
	ldrh	r1, [r4, #6]
	lsls	r2, r2, #3
	adds	r2, #8
	ands	r2, r3
	adds	r3, r6, #0
	ands	r3, r1
	orrs	r3, r2
	mov	r2, fp
	strh	r3, [r4, #6]
	movs	r1, #14
	ldrsh	r3, [r2, r1]
	adds	r7, #1
	adds	r0, r0, r3
	lsls	r0, r0, #3
	adds	r0, #4
	strb	r0, [r4, #4]
	adds	r4, #12
	cmp	r7, #4
	ble.n	.L_08049aa6
	mov	r3, sp
	adds	r3, #144
	ldr	r1, [pc, #16]
	str	r3, [sp, #24]
	ldr	r6, [sp, #28]
	str	r3, [sp, #8]
	movs	r5, #8
	b.n	.L_08049af8
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0xfc00
	.2byte 0xffff
.L_08049af8:
	mov	sl, r1
	movs	r7, #4
.L_08049afc:
	movs	r0, #128
	bl	sub_080143e0
	ldr	r3, [sp, #8]
	movs	r1, #1
	stmia	r3!, {r0}
	negs	r1, r1
	adds	r2, r3, #0
	str	r2, [sp, #8]
	bl	sub_080143f8
	ldr	r3, [pc, #24]
	mov	r1, sl
	ands	r0, r3
	ldrh	r3, [r5, r6]
	subs	r7, #1
	ands	r3, r1
	orrs	r3, r0
	strh	r3, [r5, r6]
	adds	r5, #12
	cmp	r7, #0
	bge.n	.L_08049afc
	b.n	.L_08049b30
	movs	r0, r0
	.2byte 0x03ff
	.2byte 0x0000
.L_08049b30:
	movs	r5, #240
	lsls	r5, r5, #8
	adds	r5, #24
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #2
	bl	sub_08049a04
	movs	r1, #129
	lsls	r1, r1, #1
	adds	r0, r5, #0
	adds	r1, #255
	bl	sub_08049a04
	adds	r5, #1
	movs	r1, #132
	lsls	r1, r1, #2
	adds	r0, r5, #0
	bl	sub_08049a04
	movs	r1, #137
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r0, r5, #0
	bl	sub_08049a04
	movs	r2, #146
	lsls	r2, r2, #1
	mov	r3, r8
	mov	r1, sp
	add	r2, sp
	lsls	r3, r3, #1
	adds	r1, #164
	str	r2, [sp, #20]
	str	r3, [sp, #16]
	str	r1, [sp, #32]
.L_08049b78:
	ldr	r2, [sp, #64]
	cmp	r9, r2
	bne.n	.L_08049b86
	ldr	r3, [sp, #60]
	cmp	r8, r3
	bne.n	.L_08049b86
	b.n	.L_08049d5c
.L_08049b86:
	ldr	r1, [sp, #68]
	movs	r3, #1
	strb	r3, [r1, #6]
	mov	r3, fp
	movs	r2, #12
	ldrsh	r0, [r3, r2]
	movs	r2, #14
	ldrsh	r1, [r3, r2]
	ldr	r2, [sp, #60]
	adds	r0, #1
	lsls	r3, r2, #1
	adds	r1, r1, r3
	mov	r3, fp
	ldrh	r2, [r3, #8]
	movs	r3, #15
	adds	r1, #1
	str	r3, [sp, #0]
	subs	r2, #2
	movs	r3, #1
	bl	sub_08046134
	bl	sub_080396bc
	ldr	r1, [sp, #72]
	cmp	r1, #0
	beq.n	.L_08049bfc
	mov	r3, r9
	ldr	r2, [sp, #76]
	add	r3, r8
	lsls	r3, r3, #1
	adds	r5, r3, r2
	ldrh	r1, [r5, #0]
	ldr	r0, [sp, #80]
	bl	sub_080499b4
	cmp	r0, #2
	bne.n	.L_08049bd8
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #32]
	adds	r1, r5, #0
	b.n	.L_08049be6
.L_08049bd8:
	ldrh	r3, [r5, #0]
	ldr	r0, [pc, #20]
	ldr	r5, [sp, #32]
	ands	r0, r3
	ldr	r3, [pc, #20]
	adds	r1, r5, #0
	adds	r0, r0, r3
.L_08049be6:
	movs	r2, #52
	bl	sub_0803c9bc
	b.n	.L_08049c08
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x00000d4f
	.2byte 0x0092
	.2byte 0x0000
.L_08049bfc:
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #100]
	adds	r1, r5, #0
	movs	r2, #52
	bl	sub_0803c9bc
.L_08049c08:
	ldr	r1, [sp, #52]
	movs	r3, #4
	adds	r0, r5, #0
	movs	r2, #0
	bl	sub_0803aae4
	ldr	r1, [sp, #64]
	mov	r3, r8
	str	r3, [sp, #60]
	cmp	r9, r1
	beq.n	.L_08049cde
	mov	r0, fp
	bl	sub_08039418
	ldr	r1, [sp, #76]
	mov	r2, r9
	lsls	r3, r2, #1
	adds	r3, r3, r1
	ldrh	r5, [r3, #0]
	movs	r7, #0
	cmp	r5, #0
	beq.n	.L_08049cd8
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	adds	r6, r3, #0
	movs	r3, #8
	str	r3, [sp, #12]
	str	r1, [sp, #4]
	mov	sl, r2
.L_08049c42:
	adds	r0, r5, #0
	bl	sub_080ad010
	movs	r0, #15
	bl	sub_08041f70
	adds	r1, r5, #0
	ldr	r0, [sp, #80]
	bl	sub_080499b4
	cmp	r0, #0
	beq.n	.L_08049c68
	movs	r0, #4
	bl	sub_08041f70
	b.n	.L_08049c78
	movs	r0, r0
	.2byte 0x0d46
	.2byte 0x0000
.L_08049c68:
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08049c78
	movs	r0, #2
	bl	sub_08041f70
.L_08049c78:
	movs	r0, #128
	ldr	r3, [pc, #88]
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r5
	adds	r0, r0, r3
	mov	r1, fp
	lsls	r3, r7, #4
	movs	r2, #16
	bl	sub_08042010
	movs	r0, #15
	bl	sub_08041f70
	ldr	r3, [sp, #4]
	adds	r0, r5, #0
	ldmia	r3!, {r1}
	adds	r7, #1
	adds	r2, r3, #0
	str	r2, [sp, #4]
	bl	sub_080454a0
	ldr	r3, [pc, #36]
	ldr	r1, [sp, #12]
	mov	r2, sl
	ands	r0, r3
	ldrh	r3, [r1, r2]
	ldr	r2, [pc, #32]
	ands	r3, r2
	orrs	r3, r0
	mov	r2, sl
	strh	r3, [r1, r2]
	adds	r1, #12
	str	r1, [sp, #12]
	cmp	r7, #4
	bgt.n	.L_08049cd8
	adds	r6, #2
	ldrh	r5, [r6, #0]
	cmp	r5, #0
	bne.n	.L_08049c42
	b.n	.L_08049cd8
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x025f
	.2byte 0x0000
.L_08049cd8:
	mov	r3, r9
	str	r7, [sp, #48]
	str	r3, [sp, #64]
.L_08049cde:
	ldr	r1, [sp, #72]
	cmp	r1, #5
	ble.n	.L_08049d30
	movs	r7, #0
	adds	r1, #4
	mov	sl, r1
	b.n	.L_08049d22
.L_08049cec:
	movs	r2, #243
	lsls	r2, r2, #8
	adds	r2, #1
	mov	r0, r9
	movs	r1, #5
	adds	r6, r7, r2
	bl	sub_08002054
	cmp	r7, r0
	bne.n	.L_08049d08
	movs	r3, #243
	lsls	r3, r3, #8
	adds	r3, #11
	adds	r6, r7, r3
.L_08049d08:
	mov	r1, fp
	ldrh	r2, [r1, #8]
	movs	r3, #0
	subs	r2, r2, r5
	adds	r2, r2, r7
	str	r3, [sp, #0]
	subs	r2, #2
	mov	r0, fp
	adds	r1, r6, #0
	subs	r3, #1
	bl	sub_0803c378
	adds	r7, #1
.L_08049d22:
	mov	r0, sl
	movs	r1, #5
	bl	sub_08002054
	adds	r5, r0, #0
	cmp	r7, r5
	blt.n	.L_08049cec
.L_08049d30:
	mov	r2, fp
	movs	r1, #12
	ldrsh	r0, [r2, r1]
	movs	r3, #14
	ldrsh	r1, [r2, r3]
	ldr	r2, [sp, #16]
	mov	r3, fp
	adds	r1, r1, r2
	ldrh	r2, [r3, #8]
	movs	r3, #14
	adds	r1, #1
	subs	r2, #2
	str	r3, [sp, #0]
	adds	r0, #1
	movs	r3, #1
	bl	sub_08046134
	ldr	r1, [sp, #68]
	movs	r3, #1
	movs	r2, #0
	strb	r3, [r1, #3]
	strb	r2, [r1, #6]
.L_08049d5c:
	ldr	r3, [sp, #72]
	cmp	r3, #5
	ble.n	.L_08049e10
	movs	r7, #0
	adds	r3, #4
	mov	sl, r3
	b.n	.L_08049db8
.L_08049d6a:
	ldr	r3, [pc, #304]
	movs	r1, #243
	ldr	r3, [r3, #0]
	lsls	r1, r1, #8
	movs	r2, #15
	adds	r1, #1
	ands	r3, r2
	adds	r6, r7, r1
	cmp	r3, #11
	bhi.n	.L_08049d92
	mov	r0, r9
	movs	r1, #5
	bl	sub_08002054
	cmp	r7, r0
	bne.n	.L_08049d92
	movs	r2, #243
	lsls	r2, r2, #8
	adds	r2, #11
	adds	r6, r7, r2
.L_08049d92:
	mov	r3, fp
	movs	r1, #5
	mov	r0, sl
	ldrh	r5, [r3, #8]
	bl	sub_08002054
	subs	r5, r5, r0
	adds	r5, r5, r7
	movs	r1, #0
	subs	r5, #2
	movs	r3, #1
	str	r1, [sp, #0]
	mov	r0, fp
	adds	r1, r6, #0
	adds	r2, r5, #0
	negs	r3, r3
	bl	sub_0803c378
	adds	r7, #1
.L_08049db8:
	mov	r0, sl
	movs	r1, #5
	bl	sub_08002054
	cmp	r7, r0
	blt.n	.L_08049d6a
	mov	r3, fp
	ldrh	r2, [r3, #8]
	movs	r1, #0
	str	r1, [sp, #0]
	movs	r5, #1
	movs	r1, #243
	negs	r5, r5
	subs	r2, r2, r0
	lsls	r1, r1, #8
	mov	r0, fp
	adds	r3, r5, #0
	subs	r2, #3
	adds	r1, #52
	bl	sub_0803c378
	mov	r3, fp
	ldrh	r2, [r3, #8]
	movs	r1, #0
	str	r1, [sp, #0]
	movs	r1, #243
	lsls	r1, r1, #8
	subs	r2, #2
	mov	r0, fp
	adds	r1, #53
	adds	r3, r5, #0
	bl	sub_0803c378
	mov	r1, fp
	movs	r2, #14
	ldrsh	r3, [r1, r2]
	ldr	r1, [sp, #68]
	subs	r3, #1
	lsrs	r3, r3, #2
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r1, #3]
	orrs	r2, r3
	strb	r2, [r1, #3]
.L_08049e10:
	ldr	r2, [sp, #48]
	cmp	r2, #0
	ble.n	.L_08049e2a
	ldr	r5, [sp, #28]
	adds	r7, r2, #0
.L_08049e1a:
	adds	r0, r5, #0
	movs	r1, #240
	subs	r7, #1
	bl	sub_08014128
	adds	r5, #12
	cmp	r7, #0
	bne.n	.L_08049e1a
.L_08049e2a:
	mov	r2, fp
	movs	r1, #12
	ldrsh	r3, [r2, r1]
	lsls	r3, r3, #3
	subs	r3, #2
	str	r3, [sp, #36]
	movs	r1, #14
	ldrsh	r3, [r2, r1]
	ldr	r2, [sp, #16]
	ldr	r1, [sp, #20]
	adds	r3, r2, r3
	lsls	r3, r3, #3
	adds	r3, #20
	str	r3, [sp, #40]
	movs	r3, #128
	lsls	r3, r3, #23
	movs	r2, #0
	str	r3, [r1, #4]
	str	r2, [r1, #8]
	ldr	r0, [sp, #56]
	ldr	r1, [pc, #76]
	bl	sub_080143f8
	ldr	r3, [pc, #48]
	ldr	r1, [sp, #20]
	ands	r0, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r1, #8]
	ands	r3, r2
	orrs	r3, r0
	adds	r2, r1, #0
	strh	r3, [r2, #8]
	ldr	r3, [pc, #48]
	movs	r1, #255
	ldr	r0, [r3, #0]
	movs	r3, #4
	ands	r0, r3
	ldr	r3, [sp, #36]
	lsrs	r2, r0, #1
	lsls	r1, r1, #8
	adds	r2, r3, r2
	adds	r1, #252
	adds	r2, r2, r1
	ldr	r3, [pc, #16]
	ldr	r1, [sp, #20]
	ands	r2, r3
	ldrh	r3, [r1, #6]
	ldr	r1, [pc, #12]
	b.n	.L_08049ea4
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x0300122c
	.2byte 0x97f8
	.2byte 0x0805
.L_08049ea4:
	lsrs	r0, r0, #2
	ands	r3, r1
	orrs	r3, r2
	ldr	r2, [sp, #20]
	strh	r3, [r2, #6]
	ldr	r3, [sp, #40]
	subs	r0, r3, r0
	adds	r0, #248
	strb	r0, [r2, #4]
	ldr	r1, [sp, #72]
	cmp	r1, #0
	beq.n	.L_08049ec4
	ldr	r0, [sp, #20]
	movs	r1, #242
	bl	sub_08014128
.L_08049ec4:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r1, [r3, #0]
	mov	r2, r9
	mov	r3, r8
	str	r2, [r1, #52]
	str	r3, [r1, #48]
	ldr	r2, [sp, #44]
	str	r2, [r1, #56]
	ldr	r0, [pc, #584]
	movs	r2, #1
	ldr	r3, [r0, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08049f6a
	ldr	r3, [sp, #72]
	cmp	r3, #0
	beq.n	.L_08049f62
	ldr	r2, [sp, #76]
	mov	r1, r9
	add	r1, r8
	lsls	r3, r1, #1
	adds	r5, r3, r2
	ldrh	r0, [r5, #0]
	mov	sl, r1
	movs	r7, #128
	bl	sub_080ad010
	ldrh	r2, [r5, #0]
	lsls	r7, r7, #3
	adds	r3, r7, #0
	ands	r3, r2
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L_08049f1c
	ldrh	r1, [r5, #0]
	ldr	r0, [sp, #80]
	bl	sub_080499b4
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_08049f1c
	b.n	.L_0804a0e0
.L_08049f1c:
	movs	r0, #114
	bl	sub_081c0010
	cmp	r6, #2
	bne.n	.L_08049f2c
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #508]
	b.n	.L_08049f3a
.L_08049f2c:
	ldrh	r2, [r5, #0]
	adds	r3, r7, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08049f44
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #496]
.L_08049f3a:
	adds	r1, r5, #0
	movs	r2, #32
	bl	sub_0803c9bc
	b.n	.L_08049f50
.L_08049f44:
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #488]
	adds	r1, r5, #0
	movs	r2, #32
	bl	sub_0803c9bc
.L_08049f50:
	bl	sub_080396bc
	adds	r0, r5, #0
	ldr	r1, [sp, #52]
	movs	r2, #0
	movs	r3, #4
	bl	sub_0803aae4
	b.n	.L_08049f88
.L_08049f62:
	movs	r3, #1
	negs	r3, r3
	mov	sl, r3
	b.n	.L_0804a0e0
.L_08049f6a:
	ldr	r3, [r1, #76]
	cmp	r3, #0
	beq.n	.L_08049f7a
	ldr	r3, [r0, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08049f88
.L_08049f7a:
	movs	r0, #113
	bl	sub_081c0010
	movs	r1, #1
	negs	r1, r1
	mov	sl, r1
	b.n	.L_0804a0e0
.L_08049f88:
	ldr	r2, [sp, #72]
	cmp	r2, #0
	bne.n	.L_08049f90
	b.n	.L_0804a0d8
.L_08049f90:
	ldr	r1, [pc, #400]
	movs	r2, #128
	ldr	r3, [r1, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08049fc6
	movs	r0, #111
	bl	sub_081c0010
	movs	r3, #1
	add	r8, r3
	mov	r1, r8
	cmp	r1, #5
	beq.n	.L_08049fb6
	ldr	r2, [sp, #72]
	mov	r3, r9
	add	r3, r8
	cmp	r3, r2
	bne.n	.L_08049fba
.L_08049fb6:
	movs	r3, #0
	mov	r8, r3
.L_08049fba:
	mov	r2, r8
	mov	r1, r8
	lsls	r2, r2, #1
	str	r1, [sp, #44]
	str	r2, [sp, #16]
	b.n	.L_0804a0d8
.L_08049fc6:
	ldr	r3, [r1, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804a010
	movs	r0, #111
	bl	sub_081c0010
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r1, r8
	cmp	r1, #0
	bge.n	.L_0804a004
	ldr	r0, [sp, #72]
	movs	r1, #5
	subs	r0, #1
	bl	sub_08002054
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	r9, r3
	bne.n	.L_0804a000
	ldr	r2, [sp, #72]
	mov	r1, r9
	subs	r3, r2, r1
	subs	r3, #1
	mov	r8, r3
	b.n	.L_0804a004
.L_0804a000:
	movs	r2, #4
	mov	r8, r2
.L_0804a004:
	mov	r1, r8
	mov	r3, r8
	lsls	r1, r1, #1
	str	r3, [sp, #44]
	str	r1, [sp, #16]
	b.n	.L_0804a0d8
.L_0804a010:
	ldr	r3, [r1, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804a06e
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080138a8
	ldr	r2, [sp, #72]
	mov	r3, r9
	adds	r3, #5
	cmp	r3, r2
	blt.n	.L_0804a042
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_0804a0d8
	ldr	r2, [sp, #44]
	movs	r1, #0
	mov	r8, r2
	mov	r3, r8
	lsls	r3, r3, #1
	mov	r9, r1
	b.n	.L_0804a0d6
.L_0804a042:
	ldr	r1, [sp, #44]
	ldr	r0, [sp, #72]
	mov	r8, r1
	subs	r0, #1
	movs	r1, #5
	mov	r9, r3
	bl	sub_08002054
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	r9, r3
	bne.n	.L_0804a0d2
	ldr	r2, [sp, #72]
	mov	r1, r9
	subs	r3, r2, r1
	ldr	r2, [sp, #44]
	subs	r3, #1
	mov	r8, r3
	cmp	r8, r2
	ble.n	.L_0804a0c2
	mov	r8, r2
	b.n	.L_0804a0d2
.L_0804a06e:
	ldr	r3, [r1, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804a0d8
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080138a8
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L_0804a09a
	ldr	r3, [sp, #44]
	movs	r2, #5
	mov	r8, r3
	mov	r1, r8
	negs	r2, r2
	lsls	r1, r1, #1
	add	r9, r2
	str	r1, [sp, #16]
	b.n	.L_0804a0d8
.L_0804a09a:
	ldr	r0, [sp, #72]
	movs	r1, #5
	subs	r0, #1
	bl	sub_08002054
	ldr	r2, [sp, #44]
	lsls	r3, r0, #2
	adds	r3, r3, r0
	mov	r9, r3
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_0804a0ca
	ldr	r1, [sp, #72]
	subs	r3, r1, r3
	subs	r3, #1
	mov	r8, r3
	cmp	r8, r2
	ble.n	.L_0804a0d2
	mov	r8, r2
	b.n	.L_0804a0cc
.L_0804a0c2:
	mov	r1, r8
	lsls	r1, r1, #1
	str	r1, [sp, #16]
	b.n	.L_0804a0d8
.L_0804a0ca:
	mov	r2, r8
.L_0804a0cc:
	lsls	r2, r2, #1
	str	r2, [sp, #16]
	b.n	.L_0804a0d8
.L_0804a0d2:
	mov	r3, r8
	lsls	r3, r3, #1
.L_0804a0d6:
	str	r3, [sp, #16]
.L_0804a0d8:
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_08049b78
.L_0804a0e0:
	ldr	r0, [sp, #52]
	movs	r1, #1
	bl	sub_0803939c
	movs	r1, #1
	mov	r0, fp
	bl	sub_0803939c
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #56]
	bl	sub_08014274
	ldr	r5, [sp, #24]
	movs	r7, #4
.L_0804a100:
	ldmia	r5!, {r0}
	subs	r7, #1
	bl	sub_08014274
	cmp	r7, #0
	bge.n	.L_0804a100
	movs	r0, #1
	bl	sub_08013560
	mov	r0, sl
	add	sp, #304
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001150
	.4byte 0x00000d4f
	.4byte 0x00000d4d
	.2byte 0x0d4c
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #300
	str	r1, [sp, #72]
	str	r2, [sp, #68]
	movs	r5, #192
	lsls	r5, r5, #18
	adds	r6, r0, #0
	ldr	r0, [r5, #60]
	movs	r1, #1
	str	r0, [sp, #64]
	negs	r1, r1
	movs	r0, #128
	str	r1, [sp, #60]
	str	r1, [sp, #56]
	bl	sub_080143e0
	str	r0, [sp, #52]
	adds	r0, r6, #0
	bl	sub_08016ca4
	movs	r3, #42
	str	r0, [sp, #48]
	str	r3, [sp, #0]
	movs	r1, #5
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	bl	sub_08039260
	movs	r2, #5
	str	r0, [sp, #44]
	str	r2, [sp, #40]
	adds	r5, #228
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #52]
	ldr	r1, [r3, #48]
	ldr	r3, [r3, #56]
	mov	fp, r0
	mov	r8, r1
	str	r3, [sp, #36]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r2, #21
	movs	r0, #9
	movs	r1, #9
	movs	r3, #11
	bl	sub_08039260
	mov	r2, sp
	adds	r2, #80
	str	r2, [sp, #20]
	ldr	r6, [pc, #84]
	movs	r5, #128
	mov	r9, r0
	movs	r7, #0
	adds	r4, r2, #0
	lsls	r5, r5, #23
.L_0804a1b2:
	movs	r3, #0
	lsls	r0, r7, #1
	str	r5, [r4, #4]
	str	r3, [r4, #8]
	mov	r1, r9
	movs	r3, #12
	ldrsh	r2, [r1, r3]
	ldr	r3, [pc, #52]
	ldrh	r1, [r4, #6]
	lsls	r2, r2, #3
	adds	r2, #8
	ands	r2, r3
	adds	r3, r6, #0
	ands	r3, r1
	orrs	r3, r2
	mov	r1, r9
	strh	r3, [r4, #6]
	movs	r2, #14
	ldrsh	r3, [r1, r2]
	adds	r7, #1
	adds	r0, r0, r3
	lsls	r0, r0, #3
	adds	r0, #4
	strb	r0, [r4, #4]
	adds	r4, #12
	cmp	r7, #4
	ble.n	.L_0804a1b2
	mov	r2, sp
	adds	r2, #140
	ldr	r3, [pc, #16]
	str	r2, [sp, #16]
	ldr	r6, [sp, #20]
	str	r2, [sp, #4]
	movs	r5, #8
	b.n	.L_0804a204
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0xfc00
	.2byte 0xffff
.L_0804a204:
	mov	sl, r3
	movs	r7, #4
.L_0804a208:
	movs	r0, #128
	bl	sub_080143e0
	ldr	r2, [sp, #4]
	subs	r7, #1
	stmia	r2!, {r0}
	adds	r1, r2, #0
	str	r1, [sp, #4]
	movs	r1, #1
	negs	r1, r1
	bl	sub_080143f8
	ldr	r3, [pc, #20]
	mov	r1, sl
	ands	r0, r3
	ldrh	r3, [r5, r6]
	ands	r3, r1
	orrs	r3, r0
	strh	r3, [r5, r6]
	adds	r5, #12
	cmp	r7, #0
	bge.n	.L_0804a208
	b.n	.L_0804a23c
	movs	r0, r0
	.2byte 0x03ff
	.2byte 0x0000
.L_0804a23c:
	movs	r5, #240
	lsls	r5, r5, #8
	adds	r5, #24
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #2
	bl	sub_08049a04
	movs	r1, #129
	lsls	r1, r1, #1
	adds	r0, r5, #0
	adds	r1, #255
	bl	sub_08049a04
	adds	r5, #1
	movs	r1, #132
	lsls	r1, r1, #2
	adds	r0, r5, #0
	bl	sub_08049a04
	movs	r1, #137
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r0, r5, #0
	bl	sub_08049a04
	movs	r2, #144
	lsls	r2, r2, #1
	mov	r3, r8
	add	r2, sp
	lsls	r3, r3, #1
	str	r2, [sp, #12]
	str	r3, [sp, #8]
.L_0804a27e:
	ldr	r0, [sp, #60]
	cmp	fp, r0
	bne.n	.L_0804a28c
	ldr	r1, [sp, #56]
	cmp	r8, r1
	bne.n	.L_0804a28c
	b.n	.L_0804a4dc
.L_0804a28c:
	ldr	r2, [sp, #64]
	movs	r3, #1
	strb	r3, [r2, #6]
	mov	r1, r9
	movs	r3, #12
	ldrsh	r0, [r1, r3]
	movs	r2, #14
	ldrsh	r1, [r1, r2]
	ldr	r2, [sp, #56]
	adds	r0, #1
	lsls	r3, r2, #1
	adds	r1, r1, r3
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r3, #15
	str	r3, [sp, #0]
	adds	r1, #1
	subs	r2, #2
	movs	r3, #1
	bl	sub_08046134
	bl	sub_080396bc
	ldr	r0, [sp, #68]
	cmp	r0, #0
	beq.n	.L_0804a2e8
	ldr	r1, [sp, #72]
	mov	r3, fp
	add	r3, r8
	lsls	r3, r3, #1
	ldrh	r3, [r3, r1]
	ldr	r0, [pc, #20]
	add	r5, sp, #160
	ands	r0, r3
	ldr	r3, [pc, #16]
	adds	r1, r5, #0
	adds	r0, r0, r3
	movs	r2, #52
	bl	sub_0803c9bc
	b.n	.L_0804a2f4
	movs	r0, r0
	.4byte 0x00003fff
	.2byte 0x0885
	.2byte 0x0000
.L_0804a2e8:
	add	r5, sp, #160
	ldr	r0, [pc, #192]
	adds	r1, r5, #0
	movs	r2, #52
	bl	sub_0803c9bc
.L_0804a2f4:
	movs	r2, #0
	movs	r3, #4
	adds	r0, r5, #0
	ldr	r1, [sp, #44]
	bl	sub_0803aae4
	ldr	r3, [sp, #60]
	mov	r2, r8
	str	r2, [sp, #56]
	cmp	fp, r3
	bne.n	.L_0804a30c
	b.n	.L_0804a45c
.L_0804a30c:
	mov	r0, r9
	bl	sub_08039418
	ldr	r1, [sp, #72]
	mov	r0, fp
	lsls	r3, r0, #1
	ldrh	r5, [r3, r1]
	movs	r7, #0
	cmp	r5, #0
	bne.n	.L_0804a322
	b.n	.L_0804a456
.L_0804a322:
	mov	r2, sp
	adds	r2, #76
	str	r2, [sp, #24]
.L_0804a328:
	adds	r0, r5, #0
	bl	sub_080ad078
	movs	r1, #240
	adds	r6, r0, #0
	lsls	r1, r1, #8
	movs	r0, #0
	str	r0, [sp, #0]
	lsls	r3, r7, #1
	mov	r0, r9
	adds	r1, #31
	movs	r2, #11
	mov	sl, r3
	bl	sub_0803c378
	movs	r1, #0
	str	r1, [sp, #0]
	movs	r1, #240
	lsls	r1, r1, #8
	mov	r0, r9
	adds	r1, #30
	movs	r2, #12
	mov	r3, sl
	bl	sub_0803c378
	ldr	r3, [sp, #16]
	movs	r0, #252
	lsls	r0, r0, #6
	lsls	r2, r7, #2
	adds	r2, r3, r2
	adds	r0, #255
	movs	r3, #1
	ands	r0, r5
	str	r3, [sp, #0]
	movs	r1, #0
	ldr	r3, [sp, #24]
	bl	sub_0803d98c
	mov	r0, sl
	ldr	r2, [sp, #20]
	adds	r1, r0, r7
	ldr	r3, [pc, #40]
	ldr	r0, [sp, #76]
	lsls	r1, r1, #2
	adds	r1, #8
	ands	r0, r3
	ldrh	r3, [r2, r1]
	ldr	r2, [pc, #32]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [sp, #20]
	ldrb	r2, [r6, #1]
	strh	r3, [r0, r1]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0804a3b0
	movs	r0, #4
	bl	sub_08041f70
	b.n	.L_0804a3d8
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x0d48
	.2byte 0x0000
.L_0804a3b0:
	ldr	r0, [sp, #48]
	ldrb	r2, [r6, #9]
	movs	r1, #58
	ldrsh	r3, [r0, r1]
	cmp	r2, r3
	ble.n	.L_0804a3c4
	movs	r0, #2
	bl	sub_08041f70
	b.n	.L_0804a3d8
.L_0804a3c4:
	ldr	r1, [sp, #48]
	movs	r2, #62
	adds	r2, #255
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804a3d8
	movs	r0, #9
	bl	sub_08041f70
.L_0804a3d8:
	ldr	r0, [sp, #64]
	movs	r3, #5
	strb	r3, [r0, #7]
	ldr	r0, [pc, #268]
	mov	r1, r9
	adds	r0, r5, r0
	lsls	r5, r7, #4
	movs	r2, #16
	adds	r3, r5, #0
	bl	sub_08042010
	ldrb	r0, [r6, #9]
	movs	r1, #2
	movs	r3, #104
	mov	r2, r9
	str	r5, [sp, #0]
	bl	sub_08042214
	movs	r0, #15
	bl	sub_08041f70
	ldr	r1, [sp, #64]
	movs	r3, #15
	strb	r3, [r1, #7]
	ldrb	r3, [r6, #2]
	cmp	r3, #4
	beq.n	.L_0804a426
	movs	r2, #160
	lsls	r2, r2, #7
	adds	r1, r3, #0
	adds	r2, #1
	movs	r3, #0
	adds	r1, r1, r2
	str	r3, [sp, #0]
	mov	r0, r9
	movs	r2, #15
	mov	r3, sl
	bl	sub_0803c378
.L_0804a426:
	ldrb	r3, [r6, #8]
	cmp	r3, #255
	bne.n	.L_0804a430
	movs	r3, #11
	b.n	.L_0804a432
.L_0804a430:
	subs	r3, #1
.L_0804a432:
	movs	r0, #0
	str	r0, [sp, #0]
	movs	r1, #16
	mov	r0, r9
	mov	r2, sl
	adds	r7, #1
	bl	sub_0804524c
	cmp	r7, #4
	bgt.n	.L_0804a456
	mov	r1, fp
	ldr	r2, [sp, #72]
	adds	r3, r1, r7
	lsls	r3, r3, #1
	ldrh	r5, [r3, r2]
	cmp	r5, #0
	beq.n	.L_0804a456
	b.n	.L_0804a328
.L_0804a456:
	mov	r3, fp
	str	r7, [sp, #40]
	str	r3, [sp, #60]
.L_0804a45c:
	ldr	r0, [sp, #68]
	cmp	r0, #5
	ble.n	.L_0804a4b0
	movs	r7, #0
	adds	r0, #4
	mov	sl, r0
	b.n	.L_0804a4a2
.L_0804a46a:
	movs	r1, #243
	lsls	r1, r1, #8
	adds	r1, #1
	adds	r6, r7, r1
	mov	r0, fp
	movs	r1, #5
	bl	sub_08002054
	cmp	r7, r0
	bne.n	.L_0804a486
	movs	r2, #243
	lsls	r2, r2, #8
	adds	r2, #11
	adds	r6, r7, r2
.L_0804a486:
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r0, #0
	subs	r2, r2, r5
	adds	r2, r2, r7
	movs	r3, #1
	str	r0, [sp, #0]
	subs	r2, #2
	mov	r0, r9
	adds	r1, r6, #0
	negs	r3, r3
	bl	sub_0803c378
	adds	r7, #1
.L_0804a4a2:
	mov	r0, sl
	movs	r1, #5
	bl	sub_08002054
	adds	r5, r0, #0
	cmp	r7, r5
	blt.n	.L_0804a46a
.L_0804a4b0:
	mov	r2, r9
	movs	r1, #12
	ldrsh	r0, [r2, r1]
	movs	r3, #14
	ldrsh	r1, [r2, r3]
	ldr	r2, [sp, #8]
	mov	r3, r9
	adds	r1, r1, r2
	ldrh	r2, [r3, #8]
	movs	r3, #14
	adds	r0, #1
	adds	r1, #1
	str	r3, [sp, #0]
	subs	r2, #2
	movs	r3, #1
	bl	sub_08046134
	ldr	r0, [sp, #64]
	movs	r3, #1
	movs	r1, #0
	strb	r3, [r0, #3]
	strb	r1, [r0, #6]
.L_0804a4dc:
	ldr	r2, [sp, #68]
	cmp	r2, #5
	ble.n	.L_0804a596
	movs	r7, #0
	adds	r2, #4
	mov	sl, r2
	b.n	.L_0804a53e
	movs	r0, r0
	.2byte 0x05a7
	.2byte 0x0000
.L_0804a4f0:
	movs	r3, #243
	lsls	r3, r3, #8
	adds	r3, #1
	adds	r6, r7, r3
	ldr	r3, [pc, #288]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L_0804a518
	mov	r0, fp
	movs	r1, #5
	bl	sub_08002054
	cmp	r7, r0
	bne.n	.L_0804a518
	movs	r0, #243
	lsls	r0, r0, #8
	adds	r0, #11
	adds	r6, r7, r0
.L_0804a518:
	mov	r1, r9
	ldrh	r5, [r1, #8]
	mov	r0, sl
	movs	r1, #5
	bl	sub_08002054
	subs	r5, r5, r0
	adds	r5, r5, r7
	movs	r2, #0
	subs	r5, #2
	movs	r3, #1
	str	r2, [sp, #0]
	mov	r0, r9
	adds	r1, r6, #0
	adds	r2, r5, #0
	negs	r3, r3
	bl	sub_0803c378
	adds	r7, #1
.L_0804a53e:
	mov	r0, sl
	movs	r1, #5
	bl	sub_08002054
	cmp	r7, r0
	blt.n	.L_0804a4f0
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r5, #1
	movs	r1, #243
	negs	r5, r5
	subs	r2, r2, r0
	lsls	r1, r1, #8
	movs	r0, #0
	str	r0, [sp, #0]
	adds	r3, r5, #0
	mov	r0, r9
	subs	r2, #3
	adds	r1, #52
	bl	sub_0803c378
	mov	r1, r9
	ldrh	r2, [r1, #8]
	movs	r1, #243
	movs	r3, #0
	lsls	r1, r1, #8
	str	r3, [sp, #0]
	subs	r2, #2
	mov	r0, r9
	adds	r1, #53
	adds	r3, r5, #0
	bl	sub_0803c378
	mov	r1, r9
	movs	r0, #14
	ldrsh	r3, [r1, r0]
	ldr	r0, [sp, #64]
	subs	r3, #1
	lsrs	r3, r3, #2
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r0, #3]
	orrs	r2, r3
	strb	r2, [r0, #3]
.L_0804a596:
	ldr	r1, [sp, #40]
	cmp	r1, #0
	ble.n	.L_0804a5b0
	ldr	r5, [sp, #20]
	adds	r7, r1, #0
.L_0804a5a0:
	adds	r0, r5, #0
	movs	r1, #240
	subs	r7, #1
	bl	sub_08014128
	adds	r5, #12
	cmp	r7, #0
	bne.n	.L_0804a5a0
.L_0804a5b0:
	mov	r0, r9
	movs	r2, #12
	ldrsh	r3, [r0, r2]
	ldr	r2, [sp, #8]
	lsls	r3, r3, #3
	subs	r3, #4
	str	r3, [sp, #28]
	movs	r1, #14
	ldrsh	r3, [r0, r1]
	ldr	r0, [sp, #12]
	adds	r3, r2, r3
	lsls	r3, r3, #3
	adds	r3, #20
	str	r3, [sp, #32]
	movs	r3, #128
	lsls	r3, r3, #23
	movs	r1, #0
	str	r3, [r0, #4]
	str	r1, [r0, #8]
	ldr	r0, [sp, #52]
	ldr	r1, [pc, #68]
	bl	sub_080143f8
	ldr	r3, [pc, #48]
	ldr	r2, [sp, #12]
	ands	r0, r3
	ldrh	r3, [r2, #8]
	ldr	r2, [pc, #44]
	ldr	r1, [sp, #28]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [sp, #12]
	strh	r3, [r0, #8]
	ldr	r3, [pc, #40]
	ldr	r0, [r3, #0]
	movs	r3, #4
	ands	r0, r3
	movs	r3, #255
	lsrs	r2, r0, #1
	lsls	r3, r3, #8
	adds	r2, r1, r2
	adds	r3, #252
	ldr	r1, [sp, #12]
	adds	r2, r2, r3
	ldr	r3, [pc, #12]
	lsrs	r0, r0, #2
	ands	r2, r3
	b.n	.L_0804a624
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0x0300122c
	.2byte 0x97f8
	.2byte 0x0805
.L_0804a624:
	ldrh	r3, [r1, #6]
	ldr	r1, [pc, #56]
	ands	r3, r1
	orrs	r3, r2
	ldr	r2, [sp, #12]
	strh	r3, [r2, #6]
	ldr	r3, [sp, #32]
	subs	r0, r3, r0
	adds	r0, #248
	strb	r0, [r2, #4]
	ldr	r0, [sp, #68]
	cmp	r0, #0
	beq.n	.L_0804a646
	ldr	r0, [sp, #12]
	movs	r1, #242
	bl	sub_08014128
.L_0804a646:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r1, [r3, #0]
	mov	r2, fp
	mov	r3, r8
	str	r2, [r1, #52]
	str	r3, [r1, #48]
	ldr	r0, [sp, #36]
	str	r0, [r1, #56]
	ldr	r0, [pc, #8]
	movs	r2, #1
	b.n	.L_0804a668
	.4byte 0xfffffe00
	.2byte 0x1150
	.2byte 0x0300
.L_0804a668:
	ldr	r3, [r0, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804a698
	ldr	r1, [sp, #68]
	cmp	r1, #0
	beq.n	.L_0804a692
	ldr	r2, [sp, #72]
	mov	r6, fp
	add	r6, r8
	lsls	r3, r6, #1
	ldrh	r0, [r3, r2]
	bl	sub_080ad078
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804a690
	b.n	.L_0804a816
.L_0804a690:
	b.n	.L_0804a6b4
.L_0804a692:
	movs	r6, #1
	negs	r6, r6
	b.n	.L_0804a816
.L_0804a698:
	ldr	r3, [r1, #76]
	cmp	r3, #0
	beq.n	.L_0804a6a8
	ldr	r3, [r0, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804a6b4
.L_0804a6a8:
	movs	r0, #113
	movs	r6, #1
	bl	sub_081c0010
	negs	r6, r6
	b.n	.L_0804a816
.L_0804a6b4:
	ldr	r3, [sp, #68]
	cmp	r3, #0
	bne.n	.L_0804a6bc
	b.n	.L_0804a80e
.L_0804a6bc:
	ldr	r1, [pc, #408]
	movs	r2, #128
	ldr	r3, [r1, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804a6f2
	movs	r0, #111
	bl	sub_081c0010
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #5
	beq.n	.L_0804a6e2
	ldr	r2, [sp, #68]
	mov	r3, fp
	add	r3, r8
	cmp	r3, r2
	bne.n	.L_0804a6e6
.L_0804a6e2:
	movs	r3, #0
	mov	r8, r3
.L_0804a6e6:
	mov	r1, r8
	mov	r0, r8
	lsls	r1, r1, #1
	str	r0, [sp, #36]
	str	r1, [sp, #8]
	b.n	.L_0804a80e
.L_0804a6f2:
	ldr	r3, [r1, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804a73c
	movs	r0, #111
	bl	sub_081c0010
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_0804a730
	ldr	r0, [sp, #68]
	movs	r1, #5
	subs	r0, #1
	bl	sub_08002054
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	fp, r3
	bne.n	.L_0804a72c
	ldr	r0, [sp, #68]
	mov	r1, fp
	subs	r3, r0, r1
	subs	r3, #1
	mov	r8, r3
	b.n	.L_0804a730
.L_0804a72c:
	movs	r2, #4
	mov	r8, r2
.L_0804a730:
	mov	r0, r8
	mov	r3, r8
	lsls	r0, r0, #1
	str	r3, [sp, #36]
	str	r0, [sp, #8]
	b.n	.L_0804a80e
.L_0804a73c:
	ldr	r3, [r1, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804a79c
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080138a8
	ldr	r1, [sp, #68]
	mov	r3, fp
	adds	r3, #5
	cmp	r3, r1
	blt.n	.L_0804a770
	mov	r2, fp
	cmp	r2, #0
	beq.n	.L_0804a80e
	ldr	r0, [sp, #36]
	movs	r3, #0
	mov	r8, r0
	mov	r1, r8
	lsls	r1, r1, #1
	mov	fp, r3
	str	r1, [sp, #8]
	b.n	.L_0804a80e
.L_0804a770:
	ldr	r0, [sp, #68]
	ldr	r2, [sp, #36]
	subs	r0, #1
	movs	r1, #5
	mov	fp, r3
	mov	r8, r2
	bl	sub_08002054
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	fp, r3
	bne.n	.L_0804a7f0
	ldr	r0, [sp, #68]
	mov	r1, fp
	subs	r3, r0, r1
	ldr	r2, [sp, #36]
	subs	r3, #1
	mov	r8, r3
	cmp	r8, r2
	ble.n	.L_0804a7f8
	mov	r8, r2
	b.n	.L_0804a7f0
.L_0804a79c:
	ldr	r3, [r1, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804a80e
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080138a8
	mov	r0, fp
	cmp	r0, #0
	beq.n	.L_0804a7c8
	ldr	r2, [sp, #36]
	movs	r1, #5
	mov	r8, r2
	mov	r3, r8
	negs	r1, r1
	lsls	r3, r3, #1
	add	fp, r1
	str	r3, [sp, #8]
	b.n	.L_0804a80e
.L_0804a7c8:
	ldr	r0, [sp, #68]
	movs	r1, #5
	subs	r0, #1
	bl	sub_08002054
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r0, [sp, #36]
	mov	fp, r3
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_0804a800
	ldr	r1, [sp, #68]
	subs	r3, r1, r3
	subs	r3, #1
	mov	r8, r3
	cmp	r8, r0
	ble.n	.L_0804a808
	mov	r8, r0
	b.n	.L_0804a808
.L_0804a7f0:
	mov	r3, r8
	lsls	r3, r3, #1
	str	r3, [sp, #8]
	b.n	.L_0804a80e
.L_0804a7f8:
	mov	r0, r8
	lsls	r0, r0, #1
	str	r0, [sp, #8]
	b.n	.L_0804a80e
.L_0804a800:
	mov	r1, r8
	lsls	r1, r1, #1
	str	r1, [sp, #8]
	b.n	.L_0804a80e
.L_0804a808:
	mov	r2, r8
	lsls	r2, r2, #1
	str	r2, [sp, #8]
.L_0804a80e:
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_0804a27e
.L_0804a816:
	ldr	r0, [sp, #44]
	movs	r1, #1
	bl	sub_0803939c
	mov	r0, r9
	movs	r1, #1
	bl	sub_0803939c
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [sp, #16]
	movs	r7, #4
.L_0804a830:
	ldmia	r5!, {r0}
	subs	r7, #1
	bl	sub_08014274
	cmp	r7, #0
	bge.n	.L_0804a830
	ldr	r0, [sp, #52]
	bl	sub_08014274
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r6, #0
	add	sp, #300
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #324
	str	r2, [sp, #84]
	str	r3, [sp, #80]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	mov	sl, r0
	mov	r8, r1
	movs	r0, #0
	movs	r1, #255
	str	r0, [sp, #64]
	lsls	r1, r1, #8
	movs	r0, #128
	adds	r1, #255
	lsls	r0, r0, #1
	str	r3, [sp, #76]
	str	r1, [sp, #56]
	bl	sub_080143e0
	ldr	r3, [sp, #84]
	movs	r2, #0
	str	r0, [sp, #52]
	mov	r9, r2
	cmp	r3, #0
	bne.n	.L_0804a8a0
	movs	r4, #1
	str	r4, [sp, #84]
.L_0804a8a0:
	mov	r5, r8
	cmp	r5, #2
	beq.n	.L_0804a8aa
	cmp	r5, #4
	bne.n	.L_0804a8b8
.L_0804a8aa:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	b.n	.L_0804a8c2
.L_0804a8b8:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	movs	r3, #16
.L_0804a8c2:
	str	r3, [r2, #40]
	mov	r6, sp
	adds	r6, #212
	str	r6, [sp, #28]
	mov	r3, sp
	movs	r2, #0
	movs	r7, #5
	adds	r3, #234
.L_0804a8d2:
	subs	r7, #1
	strb	r2, [r3, #0]
	subs	r3, #4
	cmp	r7, #0
	bge.n	.L_0804a8d2
	movs	r7, #1
	negs	r7, r7
	mov	r0, r8
	str	r7, [sp, #68]
	cmp	r0, #2
	bne.n	.L_0804a926
	ldr	r1, [sp, #76]
	movs	r3, #88
	ldrsh	r3, [r1, r3]
	movs	r7, #0
	cmp	r3, #255
	beq.n	.L_0804a97c
	movs	r3, #154
	lsls	r3, r3, #1
	add	r3, sp
	str	r3, [sp, #20]
	ldr	r4, [sp, #64]
	ldr	r5, [sp, #20]
	adds	r2, r1, #0
	lsls	r3, r4, #1
	adds	r2, #88
	adds	r1, r3, r5
.L_0804a908:
	ldrh	r3, [r2, #0]
	adds	r7, #1
	strh	r3, [r1, #0]
	ldr	r6, [sp, #64]
	adds	r1, #2
	adds	r6, #1
	str	r6, [sp, #64]
	adds	r2, #2
	cmp	r7, #5
	bgt.n	.L_0804a996
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	cmp	r3, #255
	bne.n	.L_0804a908
	b.n	.L_0804a996
.L_0804a926:
	mov	r1, r8
	cmp	r1, #4
	bne.n	.L_0804a940
	movs	r2, #154
	lsls	r2, r2, #1
	add	r2, sp
	mov	r3, sl
	adds	r4, r2, #0
	movs	r5, #1
	str	r2, [sp, #20]
	strh	r3, [r4, #0]
	str	r5, [sp, #64]
	b.n	.L_0804a996
.L_0804a940:
	ldr	r1, [sp, #76]
	movs	r3, #100
	adds	r1, #2
	ldrsh	r3, [r1, r3]
	movs	r7, #0
	cmp	r3, #255
	beq.n	.L_0804a98e
	movs	r0, #154
	lsls	r0, r0, #1
	add	r0, sp
	str	r0, [sp, #20]
	ldr	r2, [sp, #64]
	ldr	r4, [sp, #20]
	lsls	r3, r2, #1
	movs	r0, #100
	adds	r2, r3, r4
.L_0804a960:
	ldrh	r3, [r1, r0]
	adds	r7, #1
	strh	r3, [r2, #0]
	ldr	r5, [sp, #64]
	adds	r2, #2
	adds	r5, #1
	str	r5, [sp, #64]
	adds	r0, #2
	cmp	r7, #5
	bgt.n	.L_0804a996
	ldrsh	r3, [r1, r0]
	cmp	r3, #255
	bne.n	.L_0804a960
	b.n	.L_0804a996
.L_0804a97c:
	movs	r7, #154
	lsls	r7, r7, #1
	add	r7, sp
	str	r7, [sp, #20]
	b.n	.L_0804a996
.L_0804a986:
	ldr	r0, [sp, #20]
	ldrh	r5, [r0, r5]
	mov	sl, r5
	b.n	.L_0804aaca
.L_0804a98e:
	movs	r1, #154
	lsls	r1, r1, #1
	add	r1, sp
	str	r1, [sp, #20]
.L_0804a996:
	ldr	r3, [sp, #64]
	ldr	r4, [sp, #20]
	lsls	r2, r3, #1
	ldr	r3, [pc, #40]
	mov	r6, r8
	strh	r3, [r4, r2]
	ldr	r5, [sp, #64]
	str	r5, [sp, #60]
	cmp	r6, #2
	beq.n	.L_0804a9ac
	b.n	.L_0804aaf4
.L_0804a9ac:
	ldr	r7, [sp, #84]
	cmp	r7, #255
	bne.n	.L_0804a9b4
	b.n	.L_0804aaca
.L_0804a9b4:
	ldr	r0, [sp, #80]
	cmp	r0, #0
	bne.n	.L_0804a9bc
	b.n	.L_0804aaca
.L_0804a9bc:
	movs	r6, #0
	movs	r7, #0
	cmp	r6, r5
	blt.n	.L_0804a9c6
	b.n	.L_0804aaca
.L_0804a9c6:
	b.n	.L_0804a9cc
	.2byte 0x00ff
	.2byte 0x0000
.L_0804a9cc:
	ldr	r1, [sp, #20]
	lsls	r5, r6, #1
	ldrh	r3, [r1, r5]
	cmp	r3, #254
	beq.n	.L_0804aac0
	adds	r0, r3, #0
	bl	sub_08016ca4
	ldr	r3, [sp, #80]
	adds	r1, r0, #0
	subs	r3, #3
	cmp	r3, #5
	bhi.n	.L_0804aaba
	ldr	r2, [pc, #428]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0804aa1a
	.4byte 0x0804aa2c
	.4byte 0x0804aa08
	.4byte 0x0804aa5a
	.4byte 0x0804aaba
	.2byte 0xaaa0
	.2byte 0x0804
	movs	r2, #56
	ldrsh	r3, [r1, r2]
	cmp	r3, #0
	bne.n	.L_0804aaba
	ldr	r3, [sp, #20]
	movs	r7, #1
	ldrh	r3, [r3, r5]
	mov	sl, r3
	b.n	.L_0804aaba
	movs	r4, #50
	adds	r4, #255
	adds	r3, r1, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0804aaba
	b.n	.L_0804aab8
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	movs	r2, #255
	lsls	r2, r2, #24
	adds	r2, #255
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0804aab8
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0804aab8
	movs	r4, #66
	adds	r4, #255
	adds	r3, r1, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804aaba
	b.n	.L_0804aab8
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	movs	r2, #255
	lsls	r2, r2, #24
	adds	r2, #255
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0804aab8
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0804aab8
	movs	r4, #66
	adds	r4, #255
	adds	r3, r1, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0804aab8
	subs	r0, #7
	adds	r3, r1, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0804aab8
	adds	r2, #4
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804aaba
	b.n	.L_0804aab8
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r3, [r3, #0]
	movs	r4, #134
	ldr	r0, [sp, #20]
	lsls	r4, r4, #1
	adds	r3, r3, r4
	ldrh	r2, [r0, r5]
	ldr	r3, [r3, #0]
	cmp	r3, r2
	bne.n	.L_0804aaba
.L_0804aab8:
	movs	r7, #1
.L_0804aaba:
	cmp	r7, #0
	beq.n	.L_0804aac0
	b.n	.L_0804a986
.L_0804aac0:
	ldr	r1, [sp, #64]
	adds	r6, #1
	cmp	r6, r1
	bge.n	.L_0804aaca
	b.n	.L_0804a9cc
.L_0804aaca:
	ldr	r2, [sp, #64]
	movs	r6, #0
	cmp	r6, r2
	bge.n	.L_0804aaec
	ldr	r4, [sp, #20]
	ldrh	r3, [r4, #0]
	cmp	r3, sl
	beq.n	.L_0804aaec
	adds	r2, r4, #0
.L_0804aadc:
	ldr	r5, [sp, #64]
	adds	r6, #1
	cmp	r6, r5
	bge.n	.L_0804aaec
	adds	r2, #2
	ldrh	r3, [r2, #0]
	cmp	r3, sl
	bne.n	.L_0804aadc
.L_0804aaec:
	ldr	r7, [sp, #64]
	cmp	r6, r7
	beq.n	.L_0804aaf4
	str	r6, [sp, #68]
.L_0804aaf4:
	ldr	r0, [sp, #68]
	cmp	r0, #0
	bge.n	.L_0804ab1c
	ldr	r3, [sp, #64]
	subs	r3, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #68]
	b.n	.L_0804ab1c
.L_0804ab08:
	ldr	r1, [sp, #68]
	ldr	r2, [sp, #64]
	adds	r3, r1, r2
	subs	r3, #1
	adds	r0, r3, #0
	adds	r1, r2, #0
	str	r3, [sp, #68]
	bl	sub_08002064
	str	r0, [sp, #68]
.L_0804ab1c:
	ldr	r3, [sp, #68]
	ldr	r4, [sp, #20]
	lsls	r3, r3, #1
	str	r3, [sp, #16]
	ldrh	r3, [r4, r3]
	cmp	r3, #254
	beq.n	.L_0804ab08
	movs	r0, #182
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0804ab4e
	mov	r5, r8
	cmp	r5, #1
	bne.n	.L_0804ab4e
	ldr	r6, [sp, #20]
	ldr	r7, [sp, #16]
	ldrh	r0, [r6, r7]
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	beq.n	.L_0804ab08
.L_0804ab4e:
	mov	r2, r8
	cmp	r2, #2
	beq.n	.L_0804ab6c
	add	r5, sp, #200
	mov	r0, sl
	adds	r1, r5, #0
	bl	sub_081180b8
	ldr	r4, [sp, #28]
	movs	r3, #8
	strb	r3, [r4, #2]
	ldr	r3, [r5, #0]
	strb	r3, [r4, #0]
	movs	r3, #128
	strb	r3, [r4, #1]
.L_0804ab6c:
	movs	r3, #74
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #12
	movs	r2, #30
	movs	r3, #4
	bl	sub_08039260
	movs	r6, #152
	mov	r5, sp
	mov	r7, sp
	adds	r5, #236
	add	r6, sp
	adds	r7, #88
	str	r0, [sp, #72]
	str	r5, [sp, #24]
	mov	fp, r6
	str	r7, [sp, #36]
	b.n	.L_0804ab9e
	movs	r0, r0
	.2byte 0xa9f0
	.2byte 0x0804
.L_0804ab98:
	ldr	r0, [sp, #68]
	lsls	r0, r0, #1
	str	r0, [sp, #16]
.L_0804ab9e:
	movs	r1, #0
	str	r1, [sp, #48]
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #20]
	mov	r1, fp
	ldrh	r0, [r2, r3]
	bl	sub_081180b8
	ldr	r3, [pc, #76]
	ldr	r4, [sp, #24]
	str	r3, [r4, #4]
	ldr	r5, [sp, #48]
	str	r5, [r4, #8]
	ldr	r5, [pc, #68]
	movs	r3, #31
	ldr	r1, [r5, #0]
	ldr	r0, [sp, #52]
	lsrs	r1, r1, #2
	ands	r1, r3
	ldr	r3, [pc, #60]
	lsls	r1, r1, #8
	adds	r1, r1, r3
	bl	sub_080143f8
	ldr	r3, [pc, #40]
	ldr	r6, [sp, #24]
	ands	r0, r3
	ldrh	r2, [r6, #8]
	ldr	r3, [pc, #48]
	adds	r7, r6, #0
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [r5, #0]
	strh	r3, [r7, #8]
	lsls	r0, r0, #12
	bl	sub_08002096
	cmp	r0, #0
	bge.n	.L_0804ac0c
	movs	r1, #254
	lsls	r1, r1, #7
	adds	r1, #255
	adds	r0, r0, r1
	b.n	.L_0804ac0c
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x40002000
	.4byte 0x0300122c
	.4byte 0x0805c9c4
	.2byte 0xfc00
	.2byte 0xffff
.L_0804ac0c:
	mov	r4, fp
	ldr	r3, [r4, #4]
	asrs	r2, r0, #15
	adds	r0, r3, r2
	str	r0, [r4, #4]
	ldr	r5, [sp, #28]
	movs	r1, #1
	ldrb	r2, [r5, #2]
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804ac5e
	ldr	r4, [r4, #0]
	ldrb	r3, [r5, #0]
	adds	r3, r4, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r1, r3, #1
	ldrb	r3, [r5, #1]
	adds	r3, r0, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r0, r3, #1
	subs	r3, r4, r1
	cmp	r3, #0
	blt.n	.L_0804ac46
	cmp	r3, #7
	ble.n	.L_0804ac4c
	b.n	.L_0804ac50
.L_0804ac46:
	subs	r3, r1, r4
	cmp	r3, #7
	bgt.n	.L_0804ac50
.L_0804ac4c:
	movs	r6, #1
	str	r6, [sp, #48]
.L_0804ac50:
	mov	r7, fp
	str	r1, [r7, #0]
	ldr	r2, [sp, #28]
	str	r0, [r7, #4]
	strb	r1, [r2, #0]
	strb	r0, [r2, #1]
	b.n	.L_0804ac96
.L_0804ac5e:
	movs	r4, #192
	lsls	r3, r2, #24
	lsls	r4, r4, #18
	cmp	r3, r4
	bhi.n	.L_0804ac78
	mov	r5, fp
	ldr	r6, [sp, #28]
	ldr	r3, [r5, #0]
	str	r0, [r5, #4]
	strb	r3, [r6, #0]
	strb	r0, [r6, #1]
	strb	r1, [r6, #2]
	b.n	.L_0804ac96
.L_0804ac78:
	ldr	r7, [sp, #28]
	mov	r0, fp
	ldrb	r3, [r7, #0]
	str	r3, [r0, #0]
	ldrb	r3, [r7, #1]
	str	r3, [r0, #4]
	adds	r3, r2, #0
	adds	r3, #252
	movs	r2, #192
	strb	r3, [r7, #2]
	lsls	r2, r2, #18
	lsls	r3, r3, #24
	cmp	r3, r2
	bhi.n	.L_0804ac96
	strb	r1, [r7, #2]
.L_0804ac96:
	mov	r3, fp
	ldr	r2, [r3, #0]
	ldr	r4, [sp, #24]
	ldr	r3, [pc, #52]
	subs	r2, #8
	ldrh	r1, [r4, #6]
	ands	r2, r3
	ldr	r3, [pc, #48]
	adds	r5, r4, #0
	ands	r3, r1
	orrs	r3, r2
	mov	r6, fp
	strh	r3, [r5, #6]
	ldr	r3, [r6, #4]
	movs	r1, #240
	subs	r3, #16
	strb	r3, [r5, #4]
	ldr	r0, [sp, #24]
	bl	sub_08014128
	ldr	r7, [sp, #84]
	cmp	r7, #255
	bne.n	.L_0804acec
	ldr	r2, [pc, #20]
	ldr	r3, [sp, #88]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #1
	orrs	r3, r2
	movs	r2, #255
	b.n	.L_0804ace0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0x0000
	.2byte 0xffff
.L_0804ace0:
	.2byte 0x0212
	adds	r2, #255
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #17
	b.n	.L_0804ad02
.L_0804acec:
	ldr	r2, [pc, #276]
	ldr	r3, [sp, #88]
	ands	r3, r2
	movs	r2, #176
	orrs	r3, r2
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r3, r2
	movs	r2, #176
	lsls	r2, r2, #16
.L_0804ad02:
	orrs	r3, r2
	str	r3, [sp, #88]
	ldr	r0, [sp, #36]
	ldr	r3, [pc, #248]
	ldr	r2, [r0, #4]
	ands	r2, r3
	str	r2, [r0, #4]
	ldr	r0, [sp, #36]
	bl	sub_0801401c
	ldr	r1, [sp, #56]
	movs	r3, #1
	ands	r3, r1
	str	r0, [sp, #44]
	cmp	r3, #0
	bne.n	.L_0804ad24
	b.n	.L_0804b3b6
.L_0804ad24:
	movs	r2, #0
	str	r2, [sp, #64]
	ldr	r1, [sp, #28]
	movs	r0, #253
	movs	r7, #5
.L_0804ad2e:
	ldrb	r2, [r1, #2]
	adds	r3, r0, #0
	ands	r3, r2
	subs	r7, #1
	strb	r3, [r1, #2]
	adds	r1, #4
	cmp	r7, #0
	bge.n	.L_0804ad2e
	ldr	r3, [sp, #84]
	movs	r7, #0
	cmp	r7, r3
	bcs.n	.L_0804ae08
	ldr	r6, [sp, #64]
	add	r4, sp, #172
	ldr	r1, [sp, #64]
	mov	sl, r4
	ldr	r5, [sp, #20]
	add	r0, sp, #324
	ldr	r4, [sp, #28]
	adds	r3, r6, r0
	adds	r6, r3, #0
	mov	r2, sl
	lsls	r3, r1, #1
	mov	lr, r5
	adds	r0, r4, #0
	adds	r5, r3, r2
	movs	r3, #254
	subs	r6, #160
	mov	r8, r3
	adds	r0, #24
.L_0804ad6a:
	ldr	r1, [sp, #68]
	ldr	r2, [sp, #60]
	adds	r3, r1, r7
	cmp	r3, r2
	bge.n	.L_0804adaa
	lsls	r3, r3, #1
	mov	r1, lr
	ldrh	r2, [r1, r3]
	adds	r3, r2, #0
	cmp	r3, #254
	beq.n	.L_0804adaa
	ldrb	r3, [r4, #2]
	strh	r2, [r5, #0]
	movs	r2, #2
	orrs	r2, r3
	movs	r3, #0
	orrs	r2, r3
	movs	r3, #3
	ldrsb	r3, [r4, r3]
	strb	r2, [r4, #2]
	cmp	r3, r7
	beq.n	.L_0804ad9e
	mov	r1, r8
	ands	r2, r1
	strb	r2, [r4, #2]
	strb	r7, [r4, #3]
.L_0804ad9e:
	strb	r7, [r6, #0]
	ldr	r2, [sp, #64]
	adds	r6, #1
	adds	r2, #1
	str	r2, [sp, #64]
	adds	r5, #2
.L_0804adaa:
	cmp	r7, #0
	beq.n	.L_0804adf6
	ldr	r1, [sp, #68]
	subs	r3, r1, r7
	cmp	r3, #0
	blt.n	.L_0804adf6
	lsls	r3, r3, #1
	mov	r1, lr
	ldrh	r2, [r1, r3]
	adds	r3, r2, #0
	cmp	r3, #254
	beq.n	.L_0804adf6
	ldrb	r3, [r0, #2]
	strh	r2, [r5, #0]
	movs	r2, #6
	subs	r2, r2, r7
	mov	ip, r2
	movs	r2, #2
	orrs	r2, r3
	movs	r3, #0
	orrs	r2, r3
	movs	r3, #3
	ldrsb	r3, [r0, r3]
	negs	r1, r7
	strb	r2, [r0, #2]
	cmp	r3, r1
	beq.n	.L_0804ade8
	mov	r3, r8
	ands	r2, r3
	strb	r2, [r0, #2]
	strb	r1, [r0, #3]
.L_0804ade8:
	mov	r1, ip
	strb	r1, [r6, #0]
	ldr	r2, [sp, #64]
	adds	r6, #1
	adds	r2, #1
	str	r2, [sp, #64]
	adds	r5, #2
.L_0804adf6:
	ldr	r3, [sp, #84]
	adds	r7, #1
	adds	r4, #4
	subs	r0, #4
	cmp	r7, r3
	bcc.n	.L_0804ad6a
	b.n	.L_0804ae0c
	.2byte 0x0000
	.2byte 0xffff
.L_0804ae08:
	.2byte 0xac2b
	mov	sl, r4
.L_0804ae0c:
	ldr	r1, [sp, #28]
	movs	r4, #2
	movs	r0, #6
	movs	r7, #5
.L_0804ae14:
	ldrb	r2, [r1, #2]
	adds	r3, r4, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0804ae20
	strb	r0, [r1, #3]
.L_0804ae20:
	subs	r7, #1
	adds	r1, #4
	cmp	r7, #0
	bge.n	.L_0804ae14
	ldr	r5, [sp, #64]
	ldr	r3, [pc, #52]
	lsls	r2, r5, #1
	mov	r6, sl
	strh	r3, [r6, r2]
	mov	r0, sl
	movs	r1, #1
	bl	sub_081180e0
	movs	r3, #192
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #16]
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r3, [r3, #0]
	movs	r7, #134
	ldrh	r0, [r1, r2]
	lsls	r7, r7, #1
	adds	r3, r3, r7
	str	r0, [r3, #0]
	cmp	r0, #7
	bls.n	.L_0804ae56
	b.n	.L_0804b2b0
.L_0804ae56:
	ldr	r3, [sp, #84]
	cmp	r3, #255
	bne.n	.L_0804ae5e
	b.n	.L_0804b3ac
.L_0804ae5e:
	b.n	.L_0804ae64
	.2byte 0x00ff
	.2byte 0x0000
.L_0804ae64:
	ldr	r4, [sp, #80]
	cmp	r4, #0
	bne.n	.L_0804ae6c
	b.n	.L_0804b3ac
.L_0804ae6c:
	bl	sub_08016ca4
	ldr	r5, [sp, #20]
	ldr	r7, [sp, #16]
	adds	r6, r0, #0
	mov	r1, fp
	ldrh	r0, [r5, r7]
	bl	sub_081180b8
	mov	r0, r9
	cmp	r0, #0
	beq.n	.L_0804ae8a
	movs	r1, #1
	bl	sub_0803939c
.L_0804ae8a:
	ldr	r3, [sp, #80]
	subs	r3, #1
	cmp	r3, #6
	bls.n	.L_0804ae94
	b.n	.L_0804b3ac
.L_0804ae94:
	ldr	r2, [pc, #624]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0804aee8
	.4byte 0x0804af42
	.4byte 0x0804afd8
	.4byte 0x0804b01e
	.4byte 0x0804afa4
	.4byte 0x0804b138
	.2byte 0xaeb8
	.2byte 0x0804
	mov	r1, fp
	ldr	r3, [r1, #0]
	cmp	r3, #0
	bge.n	.L_0804aec2
	adds	r3, #7
.L_0804aec2:
	asrs	r3, r3, #3
	subs	r0, r3, #4
	adds	r3, #4
	cmp	r3, #29
	ble.n	.L_0804aece
	movs	r0, #22
.L_0804aece:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #9
	movs	r3, #3
	bl	sub_08039260
	mov	r9, r0
	movs	r0, #2
	bl	sub_08041f70
	ldr	r0, [pc, #548]
	b.n	.L_0804b29e
	mov	r2, fp
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_0804aef2
	adds	r3, #7
.L_0804aef2:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #6
	cmp	r3, #29
	ble.n	.L_0804aefe
	movs	r0, #17
.L_0804aefe:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #13
	movs	r3, #3
	bl	sub_08039260
	mov	r9, r0
	movs	r3, #0
	ldr	r0, [pc, #508]
	mov	r1, r9
	movs	r2, #0
	bl	sub_080420fc
	movs	r5, #0
	movs	r3, #56
	ldrsh	r0, [r6, r3]
	movs	r1, #4
	mov	r2, r9
	movs	r3, #16
	str	r5, [sp, #0]
	bl	sub_08042244
	ldr	r0, [pc, #484]
	mov	r1, r9
	movs	r2, #48
	movs	r3, #0
	bl	sub_080420fc
	movs	r4, #52
	ldrsh	r0, [r6, r4]
	movs	r1, #4
	mov	r2, r9
	b.n	.L_0804af9a
	mov	r5, fp
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L_0804af4c
	adds	r3, #7
.L_0804af4c:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #6
	cmp	r3, #29
	ble.n	.L_0804af58
	movs	r0, #17
.L_0804af58:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #13
	movs	r3, #3
	bl	sub_08039260
	mov	r9, r0
	mov	r1, r9
	ldr	r0, [pc, #428]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080420fc
	movs	r5, #0
	movs	r7, #58
	ldrsh	r0, [r6, r7]
	movs	r1, #4
	mov	r2, r9
	movs	r3, #16
	str	r5, [sp, #0]
	bl	sub_08042244
	mov	r1, r9
	ldr	r0, [pc, #392]
	movs	r2, #48
	movs	r3, #0
	bl	sub_080420fc
	movs	r1, #54
	ldrsh	r0, [r6, r1]
	mov	r2, r9
	movs	r1, #4
.L_0804af9a:
	movs	r3, #56
	str	r5, [sp, #0]
	bl	sub_08042244
	b.n	.L_0804b3ac
	mov	r2, fp
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_0804afae
	adds	r3, #7
.L_0804afae:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #5
	cmp	r3, #29
	ble.n	.L_0804afba
	movs	r0, #18
.L_0804afba:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r3, #3
	movs	r2, #12
	bl	sub_08039260
	movs	r4, #56
	ldrsh	r3, [r6, r4]
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_0804afd4
	b.n	.L_0804b296
.L_0804afd4:
	ldr	r0, [pc, #324]
	b.n	.L_0804b012
	mov	r5, fp
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L_0804afe2
	adds	r3, #7
.L_0804afe2:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #5
	cmp	r3, #29
	ble.n	.L_0804afee
	movs	r0, #18
.L_0804afee:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r3, #3
	movs	r2, #12
	bl	sub_08039260
	movs	r7, #50
	adds	r7, #255
	adds	r3, r6, r7
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r9, r0
	cmp	r3, #0
	bne.n	.L_0804b010
	b.n	.L_0804b296
.L_0804b010:
	ldr	r0, [pc, #268]
.L_0804b012:
	mov	r1, r9
	movs	r2, #0
	movs	r3, #0
	bl	sub_08042010
	b.n	.L_0804b3ac
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r7, r6, r0
	ldrb	r3, [r7, #0]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0804b02e
	movs	r5, #1
.L_0804b02e:
	movs	r1, #60
	adds	r1, #255
	adds	r1, r1, r6
	ldrb	r3, [r1, #0]
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_0804b03e
	adds	r5, #1
.L_0804b03e:
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r2, r2, r6
	ldrb	r3, [r2, #0]
	mov	sl, r2
	cmp	r3, #0
	beq.n	.L_0804b04e
	adds	r5, #1
.L_0804b04e:
	movs	r3, #62
	adds	r3, #255
	adds	r3, r6, r3
	str	r3, [sp, #40]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b05e
	adds	r5, #1
.L_0804b05e:
	movs	r4, #66
	adds	r4, #255
	adds	r6, r6, r4
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_0804b06c
	adds	r5, #1
.L_0804b06c:
	cmp	r5, #0
	bne.n	.L_0804b072
	movs	r5, #1
.L_0804b072:
	movs	r3, #9
	subs	r1, r3, r5
	cmp	r1, #3
	bgt.n	.L_0804b07c
	movs	r1, #4
.L_0804b07c:
	mov	r0, fp
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bge.n	.L_0804b086
	adds	r3, #7
.L_0804b086:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #9
	cmp	r3, #29
	ble.n	.L_0804b092
	movs	r0, #14
.L_0804b092:
	movs	r2, #6
	adds	r3, r5, #2
	str	r2, [sp, #0]
	movs	r2, #16
	bl	sub_08039260
	ldrb	r3, [r7, #0]
	mov	r9, r0
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0804b0b6
	ldr	r0, [pc, #120]
	mov	r1, r9
	movs	r2, #0
	movs	r3, #0
	bl	sub_08042010
	movs	r5, #1
.L_0804b0b6:
	mov	r1, r8
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_0804b0cc
	lsls	r3, r5, #3
	ldr	r0, [pc, #100]
	mov	r1, r9
	movs	r2, #0
	bl	sub_08042010
	adds	r5, #1
.L_0804b0cc:
	mov	r2, sl
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_0804b0e2
	lsls	r3, r5, #3
	ldr	r0, [pc, #84]
	mov	r1, r9
	movs	r2, #0
	bl	sub_08042010
	adds	r5, #1
.L_0804b0e2:
	ldr	r4, [sp, #40]
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_0804b0f8
	lsls	r3, r5, #3
	ldr	r0, [pc, #64]
	mov	r1, r9
	movs	r2, #0
	bl	sub_08042010
	adds	r5, #1
.L_0804b0f8:
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_0804b100
	b.n	.L_0804b290
.L_0804b100:
	lsls	r3, r5, #3
	ldr	r0, [pc, #48]
	b.n	.L_0804b286
	movs	r0, r0
	.4byte 0x0804ae9c
	.4byte 0x00000d0c
	.4byte 0x0805f88c
	.4byte 0x0805f890
	.4byte 0x0805f894
	.4byte 0x00000d0b
	.4byte 0x00000d04
	.4byte 0x00000d05
	.4byte 0x00000d06
	.4byte 0x00000d07
	.4byte 0x00000d08
	.2byte 0x0d09
	.2byte 0x0000
	movs	r7, #50
	adds	r7, #255
	adds	r3, r6, r7
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0804b14c
	movs	r5, #1
.L_0804b14c:
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b15a
	adds	r5, #1
.L_0804b15a:
	movs	r1, #60
	adds	r1, #255
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b168
	adds	r5, #1
.L_0804b168:
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b176
	adds	r5, #1
.L_0804b176:
	movs	r4, #62
	adds	r4, #255
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b184
	adds	r5, #1
.L_0804b184:
	movs	r7, #66
	adds	r7, #255
	adds	r3, r6, r7
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b192
	adds	r5, #1
.L_0804b192:
	movs	r0, #160
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b1a0
	adds	r5, #1
.L_0804b1a0:
	cmp	r5, #0
	bne.n	.L_0804b1a6
	movs	r5, #1
.L_0804b1a6:
	movs	r3, #9
	subs	r1, r3, r5
	cmp	r1, #3
	bgt.n	.L_0804b1b0
	movs	r1, #4
.L_0804b1b0:
	mov	r2, fp
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_0804b1ba
	adds	r3, #7
.L_0804b1ba:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #9
	cmp	r3, #29
	ble.n	.L_0804b1c6
	movs	r0, #14
.L_0804b1c6:
	movs	r2, #6
	adds	r3, r5, #2
	str	r2, [sp, #0]
	movs	r2, #16
	bl	sub_08039260
	movs	r4, #50
	adds	r4, #255
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r9, r0
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0804b1f4
	ldr	r0, [pc, #340]
	mov	r1, r9
	movs	r2, #0
	movs	r3, #0
	bl	sub_08042010
	movs	r5, #1
.L_0804b1f4:
	movs	r7, #156
	lsls	r7, r7, #1
	adds	r3, r6, r7
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b20e
	lsls	r3, r5, #3
	ldr	r0, [pc, #316]
	mov	r1, r9
	movs	r2, #0
	bl	sub_08042010
	adds	r5, #1
.L_0804b20e:
	movs	r0, #60
	adds	r0, #255
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b228
	lsls	r3, r5, #3
	ldr	r0, [pc, #292]
	mov	r1, r9
	movs	r2, #0
	bl	sub_08042010
	adds	r5, #1
.L_0804b228:
	movs	r1, #158
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b242
	lsls	r3, r5, #3
	ldr	r0, [pc, #272]
	mov	r1, r9
	movs	r2, #0
	bl	sub_08042010
	adds	r5, #1
.L_0804b242:
	movs	r2, #62
	adds	r2, #255
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b25c
	lsls	r3, r5, #3
	ldr	r0, [pc, #248]
	mov	r1, r9
	movs	r2, #0
	bl	sub_08042010
	adds	r5, #1
.L_0804b25c:
	movs	r4, #66
	adds	r4, #255
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b276
	lsls	r3, r5, #3
	ldr	r0, [pc, #228]
	mov	r1, r9
	movs	r2, #0
	bl	sub_08042010
	adds	r5, #1
.L_0804b276:
	movs	r7, #160
	lsls	r7, r7, #1
	adds	r3, r6, r7
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b290
	lsls	r3, r5, #3
	ldr	r0, [pc, #204]
.L_0804b286:
	mov	r1, r9
	movs	r2, #0
	bl	sub_08042010
	adds	r5, #1
.L_0804b290:
	cmp	r5, #0
	beq.n	.L_0804b296
	b.n	.L_0804b3ac
.L_0804b296:
	movs	r0, #2
	bl	sub_08041f70
	ldr	r0, [pc, #184]
.L_0804b29e:
	mov	r1, r9
	movs	r2, #0
	movs	r3, #0
	bl	sub_08042010
	movs	r0, #15
	bl	sub_08041f70
	b.n	.L_0804b3ac
.L_0804b2b0:
	ldr	r1, [sp, #84]
	cmp	r1, #255
	bne.n	.L_0804b2b8
	b.n	.L_0804b3ac
.L_0804b2b8:
	bl	sub_08016ca4
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #20]
	add	r5, sp, #108
	mov	r8, r0
	adds	r1, r5, #0
	ldrh	r0, [r2, r3]
	bl	sub_081180b8
	ldr	r3, [pc, #140]
	ldr	r0, [r3, #0]
	lsls	r0, r0, #12
	bl	sub_08002096
	cmp	r0, #0
	bge.n	.L_0804b2e2
	movs	r4, #254
	lsls	r4, r4, #7
	adds	r4, #255
	adds	r0, r0, r4
.L_0804b2e2:
	ldr	r3, [r5, #4]
	asrs	r2, r0, #15
	adds	r3, r3, r2
	str	r3, [r5, #4]
	movs	r3, #165
	lsls	r3, r3, #1
	add	r3, r8
	ldrh	r3, [r3, #0]
	movs	r6, #100
	adds	r6, #255
	cmp	r3, r6
	beq.n	.L_0804b308
	movs	r7, #176
	lsls	r7, r7, #1
	cmp	r3, r7
	beq.n	.L_0804b308
	movs	r7, #0
	add	r6, sp, #120
	b.n	.L_0804b328
.L_0804b308:
	movs	r3, #165
	lsls	r3, r3, #1
	add	r3, r8
	ldrh	r3, [r3, #0]
	movs	r1, #100
	adds	r1, #255
	ldr	r0, [pc, #72]
	cmp	r3, r1
	bne.n	.L_0804b31c
	adds	r0, #1
.L_0804b31c:
	add	r6, sp, #120
	adds	r1, r6, #0
	movs	r2, #14
	bl	sub_0803c9bc
	b.n	.L_0804b36a
.L_0804b328:
	cmp	r7, #13
	bgt.n	.L_0804b364
	mov	r4, r8
	ldrb	r3, [r4, r7]
	lsls	r2, r7, #1
	strh	r3, [r6, r2]
	adds	r7, #1
	cmp	r3, #0
	bne.n	.L_0804b328
	b.n	.L_0804b366
	.4byte 0x00000d04
	.4byte 0x00000d05
	.4byte 0x00000d06
	.4byte 0x00000d07
	.4byte 0x00000d08
	.4byte 0x00000d09
	.4byte 0x00000d0a
	.4byte 0x00000d03
	.4byte 0x0300122c
	.2byte 0x0c5a
	.2byte 0x0000
.L_0804b364:
	lsls	r2, r7, #1
.L_0804b366:
	ldr	r3, [pc, #48]
	strh	r3, [r6, r2]
.L_0804b36a:
	adds	r0, r6, #0
	bl	sub_0803aaa4
	ldr	r3, [r5, #0]
	lsrs	r2, r0, #31
	adds	r2, r0, r2
	asrs	r2, r2, #1
	subs	r3, r3, r2
	subs	r3, #8
	str	r3, [r5, #0]
	adds	r3, r3, r0
	cmp	r3, #224
	ble.n	.L_0804b38a
	movs	r3, #224
	subs	r3, r3, r0
	str	r3, [r5, #0]
.L_0804b38a:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L_0804b39c
	movs	r3, #0
	str	r3, [r5, #0]
	b.n	.L_0804b39c
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_0804b39c:
	bl	sub_080396a0
	ldr	r2, [r5, #0]
	adds	r0, r6, #0
	ldr	r1, [sp, #72]
	movs	r3, #4
	bl	sub_0803aae4
.L_0804b3ac:
	ldr	r5, [sp, #56]
	movs	r3, #2
	negs	r3, r3
	ands	r5, r3
	str	r5, [sp, #56]
.L_0804b3b6:
	ldr	r6, [sp, #48]
	cmp	r6, #0
	bne.n	.L_0804b3be
	b.n	.L_0804b500
.L_0804b3be:
	ldr	r0, [sp, #64]
	movs	r7, #1
	cmp	r7, r0
	blt.n	.L_0804b3c8
	b.n	.L_0804b500
.L_0804b3c8:
	mov	r2, sp
	adds	r2, #164
	movs	r3, #96
	ldr	r5, [sp, #24]
	movs	r6, #2
	movs	r1, #172
	add	r3, sp
	str	r2, [sp, #32]
	str	r6, [sp, #12]
	add	r1, sp
	mov	r8, r3
	mov	sl, r1
	mov	r4, r8
	adds	r5, #12
.L_0804b3e4:
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #28]
	ldrb	r3, [r0, r7]
	ldr	r2, [sp, #12]
	lsls	r3, r3, #2
	adds	r3, r1, r3
	str	r3, [sp, #4]
	mov	r3, sl
	ldrh	r0, [r2, r3]
	adds	r1, r4, #0
	str	r4, [sp, #8]
	bl	sub_081180b8
	ldr	r3, [pc, #188]
	ldr	r0, [r3, #0]
	lsls	r0, r0, #12
	bl	sub_08002096
	ldr	r4, [sp, #8]
	cmp	r0, #0
	bge.n	.L_0804b416
	movs	r6, #254
	lsls	r6, r6, #7
	adds	r6, #255
	adds	r0, r0, r6
.L_0804b416:
	ldr	r3, [r4, #4]
	asrs	r2, r0, #15
	adds	r3, r3, r2
	str	r3, [r4, #4]
	ldr	r0, [sp, #24]
	mov	lr, r5
	mov	ip, r0
	mov	r1, lr
	mov	r2, ip
	ldmia	r2!, {r0, r3, r6}
	stmia	r1!, {r0, r3, r6}
	ldr	r1, [sp, #4]
	ldrb	r2, [r1, #2]
	movs	r1, #1
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804b460
	ldr	r2, [sp, #4]
	ldr	r1, [r4, #0]
	ldrb	r3, [r2, #0]
	ldrb	r2, [r2, #1]
	adds	r1, r1, r3
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	ldr	r3, [r4, #4]
	asrs	r1, r1, #1
	adds	r3, r3, r2
	str	r1, [r4, #0]
	ldr	r6, [sp, #4]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	strb	r1, [r6, #0]
	str	r3, [r4, #4]
	strb	r3, [r6, #1]
	b.n	.L_0804b476
.L_0804b460:
	ldrh	r3, [r5, #6]
	ldrb	r2, [r5, #4]
	ldr	r0, [sp, #4]
	lsls	r3, r3, #23
	lsrs	r3, r3, #23
	adds	r2, #8
	strb	r1, [r0, #2]
	str	r3, [r4, #0]
	strb	r3, [r0, #0]
	str	r2, [r4, #4]
	strb	r2, [r0, #1]
.L_0804b476:
	ldrb	r2, [r5, #5]
	movs	r1, #13
	negs	r1, r1
	adds	r3, r1, #0
	adds	r0, r2, #0
	mov	r2, r8
	ands	r0, r3
	ldr	r1, [r2, #0]
	movs	r3, #4
	orrs	r0, r3
	ldr	r3, [pc, #40]
	subs	r1, #8
	ands	r1, r3
	ldr	r2, [pc, #36]
	ldrh	r3, [r5, #6]
	mov	r6, r8
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #6]
	ldr	r3, [r6, #4]
	strb	r0, [r5, #5]
	subs	r3, #12
	strb	r3, [r5, #4]
	ldr	r1, [sp, #84]
	cmp	r1, #255
	bne.n	.L_0804b4c0
	movs	r2, #4
	negs	r2, r2
	ands	r0, r2
	b.n	.L_0804b4ca
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0x122c
	.2byte 0x0300
.L_0804b4c0:
	movs	r3, #4
	negs	r3, r3
	ands	r0, r3
	movs	r3, #1
	orrs	r0, r3
.L_0804b4ca:
	strb	r0, [r5, #5]
	ldr	r2, [sp, #44]
	movs	r3, #31
	ands	r2, r3
	movs	r6, #63
	ldrb	r3, [r5, #7]
	negs	r6, r6
	adds	r1, r6, #0
	lsls	r2, r2, #1
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r5, #7]
	adds	r0, r5, #0
	movs	r1, #240
	str	r4, [sp, #8]
	bl	sub_08014128
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #64]
	adds	r0, #2
	adds	r7, #1
	adds	r5, #12
	str	r0, [sp, #12]
	ldr	r4, [sp, #8]
	cmp	r7, r1
	bge.n	.L_0804b500
	b.n	.L_0804b3e4
.L_0804b500:
	ldr	r3, [pc, #408]
	ldr	r6, [r3, #4]
	ldr	r5, [r3, #12]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	adds	r3, r2, #0
	adds	r3, #216
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b532
	adds	r2, #220
	ldr	r3, [r2, #0]
	movs	r5, #0
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L_0804b52e
	movs	r3, #60
	str	r3, [r2, #0]
	movs	r5, #1
	movs	r6, #1
	b.n	.L_0804b532
.L_0804b52e:
	subs	r3, #1
	str	r3, [r2, #0]
.L_0804b532:
	movs	r3, #1
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0804b5b4
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #16]
	movs	r5, #0
	ldrh	r4, [r2, r3]
	ldr	r7, [sp, #76]
	str	r5, [sp, #56]
	movs	r3, #88
	ldrsh	r3, [r7, r3]
	movs	r0, #1
	negs	r0, r0
	movs	r1, #0
	cmp	r3, #255
	beq.n	.L_0804b57a
	cmp	r3, r4
	bne.n	.L_0804b55e
	movs	r0, #128
	lsls	r0, r0, #1
	b.n	.L_0804b57a
.L_0804b55e:
	adds	r1, #1
	cmp	r1, #5
	bgt.n	.L_0804b57a
	ldr	r5, [sp, #76]
	lsls	r3, r1, #1
	adds	r3, #88
	ldrsh	r3, [r5, r3]
	cmp	r3, #255
	beq.n	.L_0804b57a
	cmp	r3, r4
	bne.n	.L_0804b55e
	movs	r0, #128
	lsls	r0, r0, #1
	orrs	r0, r1
.L_0804b57a:
	cmp	r0, #0
	bge.n	.L_0804b5b0
	ldr	r2, [sp, #76]
	movs	r5, #192
	adds	r2, #102
	movs	r7, #0
	ldrsh	r3, [r2, r7]
	movs	r1, #0
	lsls	r5, r5, #1
	cmp	r3, #255
	beq.n	.L_0804b5b0
	cmp	r3, r4
	bne.n	.L_0804b598
	adds	r0, r5, #0
	b.n	.L_0804b5b0
.L_0804b598:
	adds	r1, #1
	adds	r2, #2
	cmp	r1, #5
	bgt.n	.L_0804b5b0
	movs	r7, #0
	ldrsh	r3, [r2, r7]
	cmp	r3, #255
	beq.n	.L_0804b5b0
	cmp	r3, r4
	bne.n	.L_0804b598
	adds	r0, r5, #0
	orrs	r0, r1
.L_0804b5b0:
	str	r0, [sp, #68]
	b.n	.L_0804b61c
.L_0804b5b4:
	ldr	r0, [sp, #84]
	cmp	r0, #255
	beq.n	.L_0804b61c
	movs	r3, #144
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0804b5ea
	movs	r0, #111
	bl	sub_081c0010
.L_0804b5c8:
	ldr	r1, [sp, #68]
	adds	r1, #1
	str	r1, [sp, #68]
	adds	r0, r1, #0
	ldr	r1, [sp, #60]
	bl	sub_08002064
	str	r0, [sp, #68]
	ldr	r4, [sp, #20]
	lsls	r2, r0, #1
	ldrh	r3, [r4, r2]
	cmp	r3, #254
	beq.n	.L_0804b5c8
	ldr	r7, [sp, #56]
	movs	r3, #1
	orrs	r7, r3
	str	r7, [sp, #56]
.L_0804b5ea:
	movs	r3, #96
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0804b61c
	movs	r0, #111
	bl	sub_081c0010
.L_0804b5f8:
	ldr	r0, [sp, #68]
	ldr	r1, [sp, #60]
	adds	r3, r0, r1
	subs	r3, #1
	adds	r0, r3, #0
	str	r3, [sp, #68]
	bl	sub_08002064
	str	r0, [sp, #68]
	ldr	r2, [sp, #20]
	lsls	r3, r0, #1
	ldrh	r3, [r2, r3]
	cmp	r3, #254
	beq.n	.L_0804b5f8
	ldr	r4, [sp, #56]
	movs	r3, #1
	orrs	r4, r3
	str	r4, [sp, #56]
.L_0804b61c:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	beq.n	.L_0804b632
	movs	r3, #2
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0804b640
.L_0804b632:
	movs	r0, #113
	bl	sub_081c0010
	movs	r5, #1
	negs	r5, r5
	str	r5, [sp, #68]
	b.n	.L_0804b650
.L_0804b640:
	movs	r0, #1
	bl	sub_08013560
	ldr	r6, [sp, #56]
	cmp	r6, #0
	beq.n	.L_0804b650
	bl	.L_0804ab98
.L_0804b650:
	movs	r0, #1
	bl	sub_08013560
	mov	r7, r9
	ldr	r0, [sp, #52]
	bl	sub_08014274
	cmp	r7, #0
	beq.n	.L_0804b66a
	mov	r0, r9
	movs	r1, #1
	bl	sub_0803939c
.L_0804b66a:
	ldr	r0, [sp, #72]
	movs	r1, #1
	bl	sub_0803939c
	movs	r1, #0
	ldr	r0, [sp, #20]
	bl	sub_081180e0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #40]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #68]
	add	sp, #324
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r7, [r3, #0]
	sub	sp, #8
	cmp	r7, #0
	bne.n	.L_0804b6b2
	b.n	.L_0804b7b4
.L_0804b6b2:
	ldr	r0, [r7, #40]
	ldr	r5, [r7, #44]
	cmp	r0, r5
	beq.n	.L_0804b6d8
	subs	r6, r0, r5
	adds	r0, r6, #0
	movs	r1, #3
	bl	sub_08002054
	cmp	r0, #0
	bne.n	.L_0804b6d0
	subs	r0, #1
	cmp	r6, #0
	blt.n	.L_0804b6d0
	movs	r0, #1
.L_0804b6d0:
	adds	r0, r5, r0
	str	r0, [r7, #44]
	bl	sub_081180d0
.L_0804b6d8:
	adds	r5, r7, #0
	adds	r6, r7, #0
	adds	r5, #36
	movs	r2, #2
.L_0804b6e0:
	ldrb	r3, [r5, #0]
	adds	r5, #1
	cmp	r3, #0
	beq.n	.L_0804b6f4
	adds	r0, r6, #0
	movs	r1, #240
	str	r2, [sp, #4]
	bl	sub_08014128
	ldr	r2, [sp, #4]
.L_0804b6f4:
	subs	r2, #1
	adds	r6, #12
	cmp	r2, #0
	bge.n	.L_0804b6e0
	ldr	r0, [pc, #184]
	bl	sub_08045330
	ldr	r3, [r7, #80]
	cmp	r3, #0
	beq.n	.L_0804b7b4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #36]
	adds	r3, r2, #0
	adds	r3, #82
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804b734
	movs	r3, #0
	str	r3, [r7, #76]
	b.n	.L_0804b7b4
.L_0804b71e:
	ldrh	r3, [r2, #12]
	cmp	r3, #86
	bne.n	.L_0804b75a
	ldrh	r3, [r2, #14]
	cmp	r3, #83
	bne.n	.L_0804b75a
	movs	r3, #225
	lsls	r3, r3, #2
	str	r3, [r7, #76]
	adds	r6, r3, #0
	b.n	.L_0804b75a
.L_0804b734:
	ldr	r6, [r7, #76]
	cmp	r6, #0
	bge.n	.L_0804b75e
	adds	r3, r2, #0
	adds	r3, #80
	ldrb	r2, [r3, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #112]
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldrh	r3, [r2, #8]
	cmp	r3, #69
	bne.n	.L_0804b75a
	ldrh	r3, [r2, #10]
	cmp	r3, #68
	beq.n	.L_0804b71e
.L_0804b75a:
	cmp	r6, #0
	blt.n	.L_0804b7b4
.L_0804b75e:
	ldr	r3, [r7, #68]
	cmp	r3, #0
	bne.n	.L_0804b772
	ldr	r3, [r7, #72]
	cmp	r3, #0
	bne.n	.L_0804b772
	bl	sub_080455b0
	ldr	r6, [r7, #76]
	str	r0, [r7, #68]
.L_0804b772:
	cmp	r6, #0
	ble.n	.L_0804b77c
	subs	r3, r6, #1
	str	r3, [r7, #76]
	adds	r6, r3, #0
.L_0804b77c:
	cmp	r6, #0
	blt.n	.L_0804b7b4
	adds	r0, r6, #0
	adds	r0, #59
	movs	r1, #60
	bl	sub_08002054
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0804b7a0
	lsls	r3, r5, #4
	subs	r3, r3, r5
	lsls	r3, r3, #2
	cmp	r3, r6
	bne.n	.L_0804b7a0
	movs	r0, #108
	bl	sub_081c0010
.L_0804b7a0:
	ldr	r2, [r7, #68]
	cmp	r2, #0
	beq.n	.L_0804b7b4
	movs	r3, #8
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #2
	movs	r3, #16
	bl	sub_08042244
.L_0804b7b4:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0x06006680
	.2byte 0x3874
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #36]
	movs	r1, #166
	ldr	r3, [pc, #156]
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	sub	sp, #132
	cmp	r3, #2
	beq.n	.L_0804b7ec
	cmp	r3, #2
	ble.n	.L_0804b7ec
	movs	r1, #1
	mov	r8, r1
	cmp	r3, #4
	ble.n	.L_0804b7f0
.L_0804b7ec:
	movs	r3, #0
	mov	r8, r3
.L_0804b7f0:
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L_0804b80e
	adds	r3, r2, #0
	adds	r3, #67
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0804b808
	movs	r3, #1
	mov	r8, r3
.L_0804b808:
	mov	r1, r8
	cmp	r1, #0
	beq.n	.L_0804b864
.L_0804b80e:
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r3, #4
	movs	r1, #7
	movs	r2, #30
	movs	r0, #0
	bl	sub_08039260
	mov	sl, r0
	bl	sub_080396bc
	add	r5, sp, #4
	adds	r1, r5, #0
	movs	r2, #52
	ldr	r0, [pc, #72]
	bl	sub_0803c9bc
	adds	r0, r5, #0
	mov	r1, sl
	movs	r2, #0
	movs	r3, #4
	bl	sub_0803aae4
	movs	r5, #192
	ldr	r7, [pc, #56]
	lsls	r5, r5, #18
	movs	r6, #3
	adds	r5, #228
.L_0804b846:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r7, #4]
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_0804b85c
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	bne.n	.L_0804b846
.L_0804b85c:
	mov	r0, sl
	movs	r1, #1
	bl	sub_0803939c
.L_0804b864:
	mov	r0, r8
	add	sp, #132
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x00000c9a
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, lr}
	ldrb	r3, [r0, #1]
	movs	r2, #15
	ands	r2, r3
	movs	r5, #0
	cmp	r2, #1
	bne.n	.L_0804b88c
	movs	r5, #1
.L_0804b88c:
	cmp	r2, #11
	bne.n	.L_0804b892
	movs	r5, #2
.L_0804b892:
	ldrb	r3, [r0, #3]
	cmp	r3, #3
	bne.n	.L_0804b89a
	movs	r5, #3
.L_0804b89a:
	cmp	r3, #4
	bne.n	.L_0804b8a0
	movs	r5, #4
.L_0804b8a0:
	cmp	r3, #64
	bne.n	.L_0804b8a6
	movs	r5, #6
.L_0804b8a6:
	ldrb	r0, [r0, #3]
	bl	sub_080ad280
	cmp	r0, #0
	beq.n	.L_0804b8b2
	movs	r5, #5
.L_0804b8b2:
	adds	r0, r5, #0
	pop	{r5, pc}
	.align 2, 0
