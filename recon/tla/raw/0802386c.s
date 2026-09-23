.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_080142ac, 0x080142ac
	.set sub_08014cc0, 0x08014cc0
	.set sub_08021a84, 0x08021a84
	.set sub_080220f0, 0x080220f0
	.set sub_08022318, 0x08022318
	.set sub_0802d45c, 0x0802d45c
	.global Func_0802386c
	.thumb_func
Func_0802386c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #32]
	sub	sp, #144
	str	r0, [sp, #76]
	adds	r2, r0, #0
	adds	r2, #228
	ldr	r1, [r2, #0]
	ldr	r3, [pc, #868]
	ands	r1, r3
	str	r1, [sp, #72]
	ldr	r2, [r2, #4]
	ands	r2, r3
	str	r2, [sp, #68]
	ldr	r2, [r5, #24]
	str	r2, [sp, #64]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	str	r3, [sp, #52]
	ldr	r1, [pc, #848]
	movs	r0, #80
	bl	sub_08014cc0
	ldr	r2, [pc, #844]
	adds	r1, r0, #0
	ldr	r0, [pc, #844]
	movs	r4, #132
	subs	r2, r2, r0
	movs	r3, #128
	lsls	r4, r4, #24
	lsrs	r2, r2, #2
	lsls	r3, r3, #19
	adds	r3, #212
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #0
	ldr	r5, [r5, #20]
	ldr	r4, [sp, #64]
	mov	r9, r5
	mov	r1, r9
	mov	r2, r9
	movs	r0, #63
	adds	r1, #34
	adds	r2, #84
	strh	r3, [r4, #0]
	str	r0, [sp, #80]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
.L_080238dc:
	mov	r4, r9
	ldr	r3, [r4, #0]
	cmp	r3, #0
	bne.n	.L_080238e6
	b.n	.L_08023d44
.L_080238e6:
	ldr	r1, [r4, #8]
	adds	r5, r1, #0
	cmp	r1, #0
	bne.n	.L_080238f6
	ldr	r3, [r4, #16]
	cmp	r3, #0
	bne.n	.L_080238f6
	b.n	.L_08023d1c
.L_080238f6:
	ldr	r0, [sp, #4]
	movs	r6, #15
	str	r0, [sp, #12]
	ldrb	r3, [r0, #0]
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_08023906
	b.n	.L_08023d44
.L_08023906:
	cmp	r6, #1
	beq.n	.L_0802390c
	b.n	.L_08023d44
.L_0802390c:
	ldr	r2, [sp, #64]
	ldrh	r3, [r2, #4]
	cmp	r3, #0
	beq.n	.L_08023924
	ldrb	r2, [r0, #8]
	adds	r3, r6, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08023924
	mov	r3, r9
	ldr	r5, [r3, #80]
	b.n	.L_08023d3c
.L_08023924:
	mov	r4, r9
	ldr	r0, [sp, #72]
	ldr	r4, [r4, #80]
	subs	r0, r5, r0
	str	r4, [sp, #48]
	str	r0, [sp, #60]
	mov	r2, r9
	ldr	r7, [r2, #16]
	ldr	r3, [sp, #68]
	subs	r3, r7, r3
	str	r3, [sp, #56]
	ldr	r4, [sp, #56]
	ldr	r3, [r2, #12]
	subs	r2, r4, r3
	ldr	r4, [pc, #700]
	adds	r3, r0, r4
	ldr	r0, [pc, #700]
	cmp	r3, r0
	bls.n	.L_0802394c
	b.n	.L_08023ce2
.L_0802394c:
	ldr	r3, [pc, #696]
	cmp	r2, r3
	bgt.n	.L_08023954
	b.n	.L_08023ce2
.L_08023954:
	ldr	r4, [pc, #692]
	cmp	r2, r4
	ble.n	.L_0802395c
	b.n	.L_08023ce2
.L_0802395c:
	mov	r0, r9
	adds	r0, #35
	ldr	r3, [sp, #8]
	str	r0, [sp, #44]
	ldrb	r2, [r3, #1]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08023970
	b.n	.L_08023ce2
.L_08023970:
	ldr	r4, [sp, #8]
	movs	r0, #3
	str	r4, [sp, #36]
	ldrb	r3, [r4, #0]
	movs	r4, #156
	ands	r0, r3
	lsls	r3, r0, #3
	str	r0, [sp, #40]
	subs	r3, r3, r0
	ldr	r0, [sp, #76]
	lsls	r3, r3, #3
	lsls	r4, r4, #1
	adds	r3, r3, r4
	ldr	r3, [r0, r3]
	str	r3, [sp, #32]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08023998
	b.n	.L_08023c1e
.L_08023998:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r4, r9
	movs	r0, #22
	ldrsh	r4, [r4, r0]
	adds	r3, r3, r7
	ldr	r1, [pc, #616]
	mov	r8, r3
	movs	r3, #32
	movs	r0, #0
	ands	r3, r2
	adds	r6, r5, r1
	str	r4, [sp, #28]
	mov	fp, r0
	str	r0, [sp, #24]
	str	r0, [sp, #20]
	cmp	r3, #0
	beq.n	.L_080239be
	b.n	.L_08023afc
.L_080239be:
	ldr	r1, [sp, #76]
	movs	r2, #2
	adds	r2, #255
	adds	r2, r1, r2
	str	r2, [sp, #16]
	ldr	r0, [sp, #40]
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	adds	r1, r6, #0
	mov	r2, r8
	mov	fp, r3
	bl	sub_0802d45c
	mov	r4, r9
	ldr	r3, [r4, #20]
	cmp	r0, r3
	bne.n	.L_08023a32
	mov	r0, r8
	asrs	r0, r0, #20
	ldr	r1, [sp, #32]
	lsls	r3, r0, #7
	asrs	r2, r6, #20
	adds	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r3, [r3, r1]
	mov	sl, r0
	lsls	r3, r3, #16
	lsrs	r3, r3, #30
	str	r3, [sp, #24]
	movs	r3, #144
	lsls	r3, r3, #13
	adds	r2, r7, r3
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	bl	sub_0802d45c
	movs	r4, #136
	lsls	r4, r4, #14
	asrs	r5, r0, #16
	adds	r2, r7, r4
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	bl	sub_0802d45c
	asrs	r0, r0, #16
	subs	r0, #16
	cmp	r0, r5
	ble.n	.L_08023a20
	adds	r5, r0, #0
.L_08023a20:
	ldr	r0, [sp, #28]
	cmp	r5, r0
	ble.n	.L_08023a38
	ldr	r1, [sp, #16]
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	adds	r3, #1
	mov	fp, r3
	b.n	.L_08023a38
.L_08023a32:
	mov	r2, r8
	asrs	r2, r2, #20
	mov	sl, r2
.L_08023a38:
	movs	r3, #208
	lsls	r3, r3, #12
	adds	r6, r6, r3
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	mov	r2, r8
	bl	sub_0802d45c
	mov	r4, r9
	ldr	r3, [r4, #20]
	cmp	r0, r3
	bne.n	.L_08023aa4
	mov	r0, sl
	ldr	r1, [sp, #32]
	lsls	r3, r0, #7
	asrs	r2, r6, #20
	adds	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r3, [r3, r1]
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #13
	lsrs	r3, r3, #30
	add	r2, r8
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	str	r3, [sp, #20]
	bl	sub_0802d45c
	movs	r2, #128
	lsls	r2, r2, #14
	asrs	r5, r0, #16
	add	r2, r8
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	bl	sub_0802d45c
	asrs	r0, r0, #16
	subs	r0, #16
	cmp	r0, r5
	ble.n	.L_08023a8c
	adds	r5, r0, #0
.L_08023a8c:
	ldr	r2, [sp, #28]
	cmp	r5, r2
	ble.n	.L_08023aa4
	ldr	r4, [sp, #76]
	movs	r0, #2
	adds	r0, #255
	adds	r3, r4, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r3, #1
	mov	fp, r3
.L_08023aa4:
	ldr	r1, [pc, #364]
	mov	r2, sl
	adds	r6, r6, r1
	ldr	r4, [sp, #32]
	lsls	r3, r2, #7
	asrs	r2, r6, #20
	adds	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r3, [r3, r4]
	movs	r2, #128
	lsls	r2, r2, #13
	lsls	r3, r3, #16
	add	r2, r8
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	lsrs	r7, r3, #30
	bl	sub_0802d45c
	movs	r2, #128
	lsls	r2, r2, #14
	asrs	r5, r0, #16
	add	r2, r8
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	bl	sub_0802d45c
	asrs	r0, r0, #16
	subs	r0, #16
	cmp	r0, r5
	ble.n	.L_08023ae2
	adds	r5, r0, #0
.L_08023ae2:
	ldr	r0, [sp, #28]
	cmp	r5, r0
	ble.n	.L_08023b6c
	ldr	r1, [sp, #76]
	movs	r2, #2
	adds	r2, #255
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r3, #1
	mov	fp, r3
	b.n	.L_08023b6c
.L_08023afc:
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	mov	r2, r8
	bl	sub_0802d45c
	mov	r4, r9
	ldr	r3, [r4, #20]
	cmp	r3, r0
	bne.n	.L_08023b28
	mov	r0, r8
	asrs	r0, r0, #20
	lsls	r3, r0, #7
	asrs	r2, r6, #20
	ldr	r1, [sp, #32]
	adds	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r3, [r3, r1]
	mov	sl, r0
	lsls	r3, r3, #16
	lsrs	r3, r3, #30
	str	r3, [sp, #24]
	b.n	.L_08023b2e
.L_08023b28:
	mov	r2, r8
	asrs	r2, r2, #20
	mov	sl, r2
.L_08023b2e:
	movs	r3, #192
	lsls	r3, r3, #11
	adds	r6, r5, r3
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	mov	r2, r8
	bl	sub_0802d45c
	mov	r4, r9
	ldr	r3, [r4, #20]
	cmp	r3, r0
	bne.n	.L_08023b5a
	mov	r0, sl
	lsls	r3, r0, #7
	asrs	r2, r6, #20
	ldr	r1, [sp, #32]
	adds	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r3, [r3, r1]
	lsls	r3, r3, #16
	lsrs	r3, r3, #30
	str	r3, [sp, #20]
.L_08023b5a:
	mov	r2, sl
	lsls	r3, r2, #7
	ldr	r4, [sp, #32]
	asrs	r2, r5, #20
	adds	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r3, [r3, r4]
	lsls	r3, r3, #16
	lsrs	r7, r3, #30
.L_08023b6c:
	ldr	r0, [sp, #24]
	cmp	r0, #0
	bne.n	.L_08023b7c
	ldr	r1, [sp, #20]
	cmp	r1, #0
	bne.n	.L_08023b7c
	cmp	r7, #0
	beq.n	.L_08023ba0
.L_08023b7c:
	ldr	r2, [sp, #24]
	mov	fp, r2
	mov	r3, fp
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08023ba0
	ldr	r4, [sp, #20]
	cmp	r4, #0
	beq.n	.L_08023b96
	mov	fp, r4
	mov	r3, fp
	ands	r3, r2
.L_08023b96:
	cmp	r3, #0
	bne.n	.L_08023ba0
	cmp	r7, #0
	beq.n	.L_08023ba0
	mov	fp, r7
.L_08023ba0:
	mov	r0, fp
	cmp	r0, #0
	beq.n	.L_08023be6
	ldr	r1, [sp, #12]
	ldrb	r3, [r1, #0]
	cmp	r3, #1
	bne.n	.L_08023c18
	mov	r2, r9
	ldr	r4, [r2, #80]
	cmp	r0, #3
	bls.n	.L_08023bba
	movs	r3, #3
	mov	fp, r3
.L_08023bba:
	ldrb	r1, [r4, #9]
	movs	r2, #13
	movs	r3, #3
	mov	r0, fp
	negs	r2, r2
	ands	r0, r3
	adds	r3, r2, #0
	ands	r3, r1
	mov	fp, r0
	lsls	r0, r0, #2
	orrs	r3, r0
	strb	r3, [r4, #9]
	adds	r1, r4, #0
	adds	r1, #37
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r0
	strb	r2, [r1, #0]
	mov	r2, r9
	ldr	r1, [r2, #8]
	ldr	r7, [r2, #16]
	b.n	.L_08023c1e
.L_08023be6:
	mov	r3, r9
	ldr	r1, [r3, #8]
	ldr	r7, [r3, #16]
	b.n	.L_08023c1e
	movs	r0, r0
	.4byte 0xffff0000
	.4byte 0x000002c8
	.4byte 0x08020f90
	.4byte 0x08020da8
	.4byte 0x001fffff
	.4byte 0x012ffffe
	.4byte 0xffe00000
	.4byte 0x00dfffff
	.4byte 0xfff90000
	.2byte 0x0000
	.2byte 0xfffa
.L_08023c18:
	.2byte 0x464c
	ldr	r1, [r4, #8]
	ldr	r7, [r4, #16]
.L_08023c1e:
	asrs	r3, r7, #20
	asrs	r2, r1, #20
	lsls	r3, r3, #7
	ldr	r0, [sp, #32]
	adds	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r3, [r3, r0]
	lsls	r3, r3, #18
	lsrs	r2, r3, #30
	cmp	r2, #0
	beq.n	.L_08023c3c
	ldr	r1, [sp, #36]
	adds	r3, r2, #0
	adds	r3, #255
	strb	r3, [r1, #0]
.L_08023c3c:
	ldr	r3, [sp, #48]
	mov	r2, r9
	ldr	r1, [r3, #12]
	ldr	r5, [pc, #304]
	ldr	r0, [r2, #24]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9015
	ldr	r2, [sp, #48]
	mov	r4, r9
	ldr	r1, [r2, #12]
	ldr	r0, [r4, #28]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0xae15
	str	r0, [r6, #4]
	ldr	r3, [sp, #60]
	add	r1, sp, #92
	str	r3, [r1, #0]
	mov	r4, r9
	ldr	r0, [sp, #52]
	ldr	r3, [r4, #12]
	subs	r2, r3, r0
	str	r2, [r1, #4]
	ldr	r3, [sp, #56]
	subs	r5, r3, r0
	str	r5, [r1, #8]
	ldr	r3, [r4, #20]
	subs	r4, r3, r0
	str	r4, [r1, #12]
	ldr	r3, [sp, #44]
	ldrb	r0, [r3, #0]
	movs	r3, #2
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08023c96
	ldr	r3, [pc, #240]
	adds	r2, r2, r3
	str	r2, [r1, #4]
	adds	r2, r5, r3
	adds	r3, r4, r3
	str	r2, [r1, #8]
	str	r3, [r1, #12]
	ldr	r4, [sp, #44]
	ldrb	r0, [r4, #0]
.L_08023c96:
	movs	r3, #4
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08023cb8
	ldr	r3, [r1, #4]
	movs	r2, #160
	lsls	r2, r2, #17
	adds	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r3, [r1, #8]
	adds	r3, r3, r2
	str	r3, [r1, #8]
	ldr	r3, [r1, #12]
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r2, [sp, #44]
	ldrb	r0, [r2, #0]
.L_08023cb8:
	movs	r3, #8
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08023cd4
	ldr	r0, [sp, #4]
	mov	r4, r9
	ldrb	r2, [r0, #15]
	ldrh	r3, [r4, #6]
	ldr	r0, [sp, #48]
	str	r2, [sp, #0]
	adds	r2, r6, #0
	bl	sub_08022318
	b.n	.L_08023d44
.L_08023cd4:
	mov	r2, r9
	ldrh	r3, [r2, #6]
	ldr	r0, [sp, #48]
	adds	r2, r6, #0
	bl	sub_080220f0
	b.n	.L_08023d44
.L_08023ce2:
	ldr	r3, [sp, #4]
	ldrb	r2, [r3, #8]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08023cfa
	mov	r4, r9
	ldrh	r1, [r4, #6]
	ldr	r0, [sp, #48]
	bl	sub_08021a84
	b.n	.L_08023d44
.L_08023cfa:
	movs	r5, #1
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08023d44
	ldr	r0, [sp, #48]
	adds	r3, r5, #0
	ldrb	r2, [r0, #17]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08023d44
	ldrb	r0, [r0, #16]
	bl	sub_080142ac
	ldr	r1, [sp, #48]
	strb	r5, [r1, #25]
	b.n	.L_08023d44
.L_08023d1c:
	ldr	r2, [sp, #4]
	movs	r6, #15
	ldrb	r3, [r2, #0]
	ands	r6, r3
	cmp	r6, #1
	bne.n	.L_08023d44
	mov	r3, r9
	ldr	r5, [r3, #80]
	ldrb	r3, [r2, #8]
	cmp	r3, #0
	bne.n	.L_08023d44
	ldrb	r2, [r5, #17]
	adds	r3, r6, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08023d44
.L_08023d3c:
	ldrb	r0, [r5, #16]
	bl	sub_080142ac
	strb	r6, [r5, #25]
.L_08023d44:
	ldr	r4, [sp, #80]
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #4]
	subs	r4, #1
	adds	r0, #128
	adds	r1, #128
	movs	r2, #128
	str	r4, [sp, #80]
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	add	r9, r2
	cmp	r4, #0
	blt.n	.L_08023d60
	b.n	.L_080238dc
.L_08023d60:
	movs	r0, #80
	bl	sub_0801314c
	add	sp, #144
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.4byte 0xfec00000
