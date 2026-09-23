.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_08015080, 0x08015080
	.set sub_08015270, 0x08015270
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_080770d0, 0x080770d0
	.set sub_080a112c, 0x080a112c
	.set sub_080a17c4, 0x080a17c4
	.set sub_080a1804, 0x080a1804
	.set sub_080a1a40, 0x080a1a40
	.set sub_080a1ac0, 0x080a1ac0
	.set sub_080f9010, 0x080f9010
	.global Func_080a63e4
	.thumb_func
Func_080a63e4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r0, [sp, #8]
	ldr	r3, [pc, #64]
	ldr	r6, [r3, #0]
	ldr	r2, [pc, #64]
	movs	r1, #29
	ldrsb	r1, [r6, r1]
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	mov	r8, r1
	movs	r1, #0
	str	r1, [sp, #4]
	str	r1, [sp, #0]
	mov	fp, r3
	movs	r3, #1
	mov	r9, r3
	movs	r3, #28
	ldrsb	r3, [r6, r3]
	subs	r2, #17
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r0, [r6, r3]
	bl	sub_08077008
	mov	r3, r8
	lsls	r3, r3, #1
	mov	sl, r3
	mov	r0, sl
	add	r0, r8
	lsls	r0, r0, #3
	subs	r0, #10
	movs	r1, #16
	bl	sub_080a1ac0
	b.n	.L_080a6582
	.4byte 0x03001f2c
	.2byte 0x0219
	.2byte 0x0000
.L_080a6440:
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L_080a6500
	mov	r0, r8
	movs	r2, #0
	mov	r1, fp
	add	r0, fp
	mov	r9, r2
	bl	sub_080022fc
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #1
	movs	r7, #130
	mov	sl, r3
	lsls	r7, r7, #2
	add	r7, sl
	ldrh	r0, [r6, r7]
	bl	sub_08077008
	ldr	r3, [r6, #16]
	mov	r1, sl
	ldrh	r2, [r3, #12]
	add	r1, r8
	adds	r2, r2, r1
	ldr	r5, [r6, #24]
	ldr	r3, [pc, #28]
	lsls	r2, r2, #3
	subs	r2, #2
	strh	r2, [r5, #6]
	ands	r2, r3
	ldr	r3, [pc, #24]
	ldr	r1, [pc, #24]
	ands	r2, r3
	ldrh	r3, [r5, #22]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #22]
	ldr	r1, [sp, #8]
	cmp	r1, #0
	bne.n	.L_080a6506
	b.n	.L_080a64a0
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
.L_080a64a0:
	ldr	r0, [r6, #36]
	ldrh	r1, [r6, r7]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080a112c
	adds	r0, r6, #0
	ldrh	r1, [r6, r7]
	bl	sub_080a1804
	ldr	r0, [pc, #64]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a64ea
	ldr	r2, [sp, #0]
	cmp	r2, #0
	bne.n	.L_080a64ea
	ldr	r0, [r6, #44]
	bl	sub_08015270
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #32]
	ands	r0, r3
	ldr	r3, [pc, #36]
	movs	r2, #0
	adds	r0, r0, r3
	ldr	r1, [r6, #44]
	movs	r3, #0
	bl	sub_08015080
	movs	r2, #1
	str	r2, [sp, #0]
	b.n	.L_080a6506
.L_080a64ea:
	ldr	r0, [pc, #12]
	bl	sub_080770d0
	b.n	.L_080a6506
	movs	r0, r0
	.4byte 0x00003fff
	.4byte 0x00000151
	.2byte 0x053a
	.2byte 0x0000
.L_080a6500:
	mov	r3, r8
	lsls	r3, r3, #1
	mov	sl, r3
.L_080a6506:
	mov	r0, sl
	add	r0, r8
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	sub_080a1a40
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #188]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a6538
	movs	r0, #112
	bl	sub_080f9010
	movs	r3, #130
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r3, [r6, r3]
	str	r3, [sp, #4]
	b.n	.L_080a6596
.L_080a6538:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a6550
	movs	r0, #113
	bl	sub_080f9010
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #4]
	b.n	.L_080a6596
.L_080a6550:
	ldr	r5, [pc, #136]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a656c
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	negs	r2, r2
	movs	r3, #1
	add	r8, r2
	mov	r9, r3
.L_080a656c:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a6582
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #1
	add	r8, r1
	mov	r9, r1
.L_080a6582:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a6590
	b.n	.L_080a6440
.L_080a6590:
	mov	r2, r8
	lsls	r2, r2, #1
	mov	sl, r2
.L_080a6596:
	ldr	r5, [r6, #24]
	mov	r3, r8
	strb	r3, [r6, #29]
	adds	r0, r5, #0
	bl	sub_080a17c4
	movs	r3, #13
	strb	r3, [r5, #5]
	movs	r0, #1
	bl	sub_080030f8
	mov	r1, r8
	movs	r2, #130
	strb	r1, [r6, #29]
	lsls	r2, r2, #2
	add	r2, sl
	ldrh	r3, [r6, r2]
	str	r3, [r6, #8]
	ldr	r1, [pc, #36]
	ldrh	r2, [r6, r2]
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	ldr	r0, [sp, #4]
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x0000021b
