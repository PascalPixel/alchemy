.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_08015010, 0x08015010
	.set sub_08015018, 0x08015018
	.set sub_08015120, 0x08015120
	.set sub_08015270, 0x08015270
	.set sub_08077018, 0x08077018
	.set sub_08077028, 0x08077028
	.set sub_08077058, 0x08077058
	.set sub_08077220, 0x08077220
	.set sub_08077238, 0x08077238
	.set sub_08077240, 0x08077240
	.set sub_08077248, 0x08077248
	.set sub_080a1028, 0x080a1028
	.set sub_080a1030, 0x080a1030
	.set sub_080b0070, 0x080b0070
	.set sub_080b04dc, 0x080b04dc
	.set sub_080b0574, 0x080b0574
	.set sub_080b0634, 0x080b0634
	.set sub_080b0a6c, 0x080b0a6c
	.set sub_080b0fa4, 0x080b0fa4
	.set sub_080b10cc, 0x080b10cc
	.set sub_080b110c, 0x080b110c
	.set sub_080b11a4, 0x080b11a4
	.set sub_080b11c4, 0x080b11c4
	.set sub_080b1260, 0x080b1260
	.set sub_080b1470, 0x080b1470
	.set sub_080b153c, 0x080b153c
	.set sub_080b17e4, 0x080b17e4
	.set sub_080b24e4, 0x080b24e4
	.set sub_080f9010, 0x080f9010
	.global Shop_SelBuy
	.global Func_080b0aac
	.thumb_func
Shop_SelBuy:
Func_080b0aac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #772]
	ldr	r3, [r3, #0]
	sub	sp, #36
	movs	r0, #0
	movs	r1, #0
	str	r0, [sp, #32]
	str	r0, [sp, #16]
	mov	sl, r3
	str	r1, [r3, #32]
	movs	r5, #2
	movs	r1, #7
	movs	r3, #4
	movs	r2, #12
	movs	r0, #18
	str	r5, [sp, #0]
	bl	sub_08015010
	mov	r2, sl
	str	r0, [r2, #12]
	bl	sub_080b10cc
	movs	r0, #0
	movs	r1, #8
	movs	r2, #15
	movs	r3, #4
	str	r5, [sp, #0]
	bl	sub_08015010
	str	r0, [sp, #32]
.L_080b0af4:
	movs	r5, #2
	movs	r1, #12
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	ldr	r7, [sp, #16]
	str	r5, [sp, #0]
	bl	sub_08015010
	movs	r3, #224
	str	r0, [sp, #28]
	lsls	r3, r3, #2
	add	r3, sl
	ldr	r2, [r3, #0]
	movs	r3, #18
	strb	r3, [r2, #5]
	movs	r2, #234
	lsls	r2, r2, #2
	add	r2, sl
	movs	r3, #12
	strb	r3, [r2, #0]
	movs	r0, #0
	movs	r3, #3
	movs	r1, #17
	movs	r2, #30
	str	r5, [sp, #0]
	bl	sub_08015010
	movs	r3, #1
	str	r0, [sp, #24]
	mov	fp, r3
.L_080b0b32:
	ldr	r3, [pc, #656]
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r0, fp
	mov	r9, r3
	cmp	r0, #0
	beq.n	.L_080b0ba0
	movs	r2, #155
	lsls	r2, r2, #2
	lsls	r3, r7, #1
	add	r2, sl
	ldrsh	r5, [r3, r2]
	adds	r0, r5, #0
	bl	sub_08077018
	movs	r2, #0
	adds	r6, r0, #0
	movs	r1, #7
	adds	r0, r7, #0
	mov	fp, r2
	bl	sub_080022fc
	adds	r1, r0, #0
	lsls	r1, r1, #5
	ldr	r0, [sp, #28]
	subs	r1, #8
	movs	r2, #8
	bl	sub_080b0a6c
	movs	r2, #234
	lsls	r2, r2, #2
	movs	r3, #4
	add	r2, sl
	strb	r3, [r2, #0]
	ldr	r0, [sp, #28]
	adds	r1, r7, #0
	bl	sub_080b0fa4
	ldr	r1, [pc, #580]
	ldr	r0, [sp, #24]
	adds	r1, r5, r1
	bl	sub_080b11a4
	ldr	r0, [sp, #32]
	bl	sub_08015270
	movs	r3, #0
	ldrsh	r2, [r6, r3]
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	movs	r3, #0
	bl	sub_080b110c
.L_080b0ba0:
	ldr	r1, [pc, #552]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0bae
	b.n	.L_080b0f48
.L_080b0bae:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0bba
	b.n	.L_080b0f3a
.L_080b0bba:
	ldr	r0, [pc, #532]
	ldr	r3, [r0, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0be2
	mov	r1, r9
	mov	r8, r7
	subs	r7, #1
	adds	r0, r7, r1
	bl	sub_080022fc
	adds	r7, r0, #0
	cmp	r8, r7
	beq.n	.L_080b0be2
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	mov	fp, r2
.L_080b0be2:
	ldr	r0, [pc, #492]
	ldr	r3, [r0, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0c0a
	mov	r1, r9
	mov	r8, r7
	adds	r7, #1
	adds	r0, r7, r1
	bl	sub_080022fc
	adds	r7, r0, #0
	cmp	r8, r7
	beq.n	.L_080b0c0a
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	mov	fp, r2
.L_080b0c0a:
	ldr	r0, [pc, #452]
	ldr	r3, [r0, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0c22
	subs	r3, r7, #7
	cmp	r3, #0
	blt.n	.L_080b0c22
	movs	r1, #1
	adds	r7, r3, #0
	mov	fp, r1
.L_080b0c22:
	ldr	r2, [pc, #428]
	ldr	r3, [r2, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0c52
	mov	r0, r9
	adds	r0, #6
	movs	r1, #7
	bl	sub_080022ec
	lsls	r3, r0, #3
	adds	r5, r7, #7
	subs	r3, r3, r0
	cmp	r5, r3
	bge.n	.L_080b0c48
	movs	r3, #1
	adds	r7, r5, #0
	mov	fp, r3
.L_080b0c48:
	mov	r0, r9
	subs	r0, #1
	cmp	r7, r0
	ble.n	.L_080b0c52
	adds	r7, r0, #0
.L_080b0c52:
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_080b0b32
.L_080b0c5a:
	ldr	r0, [sp, #24]
	movs	r1, #2
	bl	sub_08015018
	movs	r1, #2
	ldr	r0, [sp, #28]
	bl	sub_08015018
	movs	r0, #1
	bl	sub_080030f8
	mov	r0, r8
	cmp	r0, #0
	beq.n	.L_080b0c78
	b.n	.L_080b0f56
.L_080b0c78:
	ldr	r1, [sp, #16]
	movs	r2, #155
	lsls	r3, r1, #1
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r0, sl
	ldr	r5, [pc, #332]
	ldrh	r3, [r0, r3]
	add	r5, sl
	strh	r3, [r5, #0]
	ldr	r0, [pc, #328]
	bl	sub_080b04dc
	ldrh	r0, [r5, #0]
	bl	sub_08077018
	movs	r1, #1
	str	r0, [sp, #8]
	str	r1, [sp, #12]
	movs	r5, #2
	movs	r1, #14
	movs	r2, #13
	movs	r3, #3
	movs	r6, #0
	movs	r0, #0
	str	r6, [sp, #4]
	str	r5, [sp, #0]
	bl	sub_08015010
	movs	r3, #224
	str	r0, [sp, #20]
	lsls	r3, r3, #2
	add	r3, sl
	ldr	r2, [r3, #0]
	movs	r3, #4
	strb	r3, [r2, #5]
	movs	r2, #234
	lsls	r2, r2, #2
	add	r2, sl
	movs	r3, #12
	strb	r3, [r2, #0]
	mov	r2, r8
	str	r2, [sp, #0]
	ldr	r0, [sp, #20]
	movs	r1, #2
	movs	r2, #0
	movs	r3, #8
	bl	sub_080a1028
	movs	r3, #9
	movs	r0, #16
	movs	r1, #11
	movs	r2, #14
	str	r5, [sp, #0]
	bl	sub_08015010
	movs	r3, #1
	movs	r7, #0
	mov	r9, r0
	mov	fp, r3
.L_080b0cf0:
	ldr	r0, [sp, #4]
	cmp	r0, #0
	beq.n	.L_080b0d04
	movs	r1, #0
	ldr	r0, [pc, #220]
	str	r1, [sp, #4]
	bl	sub_080b04dc
	movs	r2, #1
	mov	fp, r2
.L_080b0d04:
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_080b0d72
	ldr	r3, [pc, #208]
	add	r3, sl
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	movs	r0, #0
	mov	fp, r0
	adds	r0, r7, r1
	bl	sub_080022fc
	movs	r3, #219
	adds	r7, r0, #0
	lsls	r1, r7, #1
	lsls	r3, r3, #2
	adds	r2, r1, r3
	mov	r3, sl
	adds	r1, r1, r7
	adds	r3, #2
	lsls	r1, r1, #3
	ldrsh	r6, [r3, r2]
	subs	r1, #12
	ldr	r0, [sp, #20]
	movs	r2, #0
	bl	sub_080b0a6c
	movs	r2, #234
	lsls	r2, r2, #2
	add	r2, sl
	movs	r3, #3
	ldr	r5, [pc, #144]
	strb	r3, [r2, #0]
	add	r5, sl
	ldr	r0, [sp, #20]
	ldrh	r2, [r5, #0]
	adds	r1, r7, #0
	bl	sub_080b11c4
	ldrh	r0, [r5, #0]
	bl	sub_08077238
	cmp	r0, #0
	bne.n	.L_080b0d68
	ldrh	r2, [r5, #0]
	mov	r0, r9
	adds	r1, r6, #0
	bl	sub_080b1470
	b.n	.L_080b0d72
.L_080b0d68:
	ldrh	r2, [r5, #0]
	mov	r0, r9
	adds	r1, r6, #0
	bl	sub_080b1260
.L_080b0d72:
	ldr	r1, [pc, #88]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080b0e6a
	ldr	r5, [pc, #84]
	add	r5, sl
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	bl	sub_08077028
	adds	r1, r0, #0
	cmp	r1, #0
	bge.n	.L_080b0de8
	movs	r0, #113
	bl	sub_080f9010
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08015120
	ldrh	r0, [r5, #0]
	movs	r1, #2
	bl	sub_08015120
	adds	r0, r6, #0
	bl	sub_08077248
	cmp	r0, #15
	bne.n	.L_080b0db8
	ldr	r0, [pc, #44]
	bl	sub_080b04dc
	b.n	.L_080b0cf0
.L_080b0db8:
	ldr	r0, [pc, #40]
	bl	sub_080b04dc
	b.n	.L_080b0cf0
	.4byte 0x03001f2c
	.4byte 0x000003a6
	.4byte 0x00000075
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x0000039e
	.4byte 0x00000c9d
	.4byte 0x000003a7
	.4byte 0x00000c9e
	.2byte 0x0ca6
	.2byte 0x0000
.L_080b0de8:
	adds	r0, r6, #0
	bl	sub_08077058
	ldr	r2, [sp, #8]
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	ldr	r2, [pc, #396]
	ldr	r2, [r2, #16]
	cmp	r3, r2
	bls.n	.L_080b0dfe
	b.n	.L_080b0f26
.L_080b0dfe:
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	bl	sub_08077220
	cmp	r0, #0
	bne.n	.L_080b0e28
	movs	r1, #1
	adds	r0, r6, #0
	bl	sub_08015120
	ldr	r0, [pc, #372]
	bl	sub_080b04dc
	movs	r0, #0
	bl	sub_080b0634
	movs	r3, #1
	str	r3, [sp, #4]
	cmp	r0, #0
	beq.n	.L_080b0e28
	b.n	.L_080b0cf0
.L_080b0e28:
	ldr	r5, [pc, #352]
	movs	r0, #112
	bl	sub_080f9010
	add	r5, sl
	movs	r0, #1
	bl	sub_080030f8
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	bl	sub_080b153c
	str	r0, [sp, #12]
	movs	r2, #1
	ldr	r1, [sp, #12]
	movs	r0, #1
	negs	r2, r2
	str	r0, [sp, #4]
	cmp	r1, r2
	bne.n	.L_080b0e52
	b.n	.L_080b0cf0
.L_080b0e52:
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	ldr	r2, [sp, #12]
	bl	sub_080b17e4
	mov	r1, r9
	ldr	r0, [sp, #20]
	bl	sub_080b24e4
	movs	r3, #0
	mov	r8, r3
	b.n	.L_080b0eaa
.L_080b0e6a:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080b0f18
	ldr	r5, [pc, #280]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0e8c
	movs	r0, #111
	bl	sub_080f9010
	movs	r0, #1
	subs	r7, #1
	mov	fp, r0
.L_080b0e8c:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0ea2
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #1
	adds	r7, #1
	mov	fp, r1
.L_080b0ea2:
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_080b0cf0
.L_080b0eaa:
	bl	sub_080a1030
	mov	r0, r9
	movs	r1, #2
	bl	sub_08015018
	movs	r1, #2
	ldr	r0, [sp, #20]
	bl	sub_08015018
	movs	r0, #1
	bl	sub_080030f8
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080b0f10
	ldr	r3, [pc, #200]
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_080b0f10
	ldr	r3, [sp, #12]
	cmp	r8, r3
	bge.n	.L_080b0ef4
	ldr	r6, [pc, #172]
	adds	r5, r3, #0
	add	r6, sl
.L_080b0ee4:
	movs	r1, #1
	ldrh	r0, [r6, #0]
	negs	r1, r1
	subs	r5, #1
	bl	sub_08077240
	cmp	r5, #0
	bne.n	.L_080b0ee4
.L_080b0ef4:
	bl	sub_080b0070
	cmp	r0, #0
	beq.n	.L_080b0f56
	ldr	r3, [pc, #152]
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r0, [sp, #16]
	subs	r3, #1
	cmp	r0, r3
	ble.n	.L_080b0f10
	str	r3, [sp, #16]
.L_080b0f10:
	ldr	r0, [pc, #136]
	bl	sub_080b04dc
	b.n	.L_080b0af4
.L_080b0f18:
	movs	r0, #113
	bl	sub_080f9010
	movs	r1, #1
	negs	r1, r1
	mov	r8, r1
	b.n	.L_080b0eaa
.L_080b0f26:
	movs	r0, #113
	bl	sub_080f9010
	ldr	r0, [pc, #112]
	bl	sub_080b0574
	movs	r2, #1
	negs	r2, r2
	mov	r8, r2
	b.n	.L_080b0eaa
.L_080b0f3a:
	movs	r0, #113
	bl	sub_080f9010
	movs	r3, #1
	negs	r3, r3
	mov	r8, r3
	b.n	.L_080b0c5a
.L_080b0f48:
	movs	r0, #112
	str	r7, [sp, #16]
	bl	sub_080f9010
	movs	r0, #0
	mov	r8, r0
	b.n	.L_080b0c5a
.L_080b0f56:
	ldr	r0, [sp, #32]
	movs	r1, #2
	bl	sub_08015018
	mov	r1, sl
	ldr	r0, [r1, #12]
	movs	r1, #2
	bl	sub_08015018
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #0
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000c9f
	.4byte 0x0000039e
	.4byte 0x03001b04
	.4byte 0x000003aa
	.4byte 0x000003a6
	.4byte 0x00000ca8
	.4byte 0x00000c9c
