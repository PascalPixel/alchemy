.syntax unified
	.thumb
	.set sub_08015120, 0x08015120
	.set sub_08077008, 0x08077008
	.set sub_08077018, 0x08077018
	.set sub_08077230, 0x08077230
	.set sub_080772b0, 0x080772b0
	.set sub_080b0574, 0x080b0574
	.set sub_080b0634, 0x080b0634
	.set sub_080b10cc, 0x080b10cc
	.set sub_080b19cc, 0x080b19cc
	.set sub_080b1dec, 0x080b1dec
	.set sub_080f9010, 0x080f9010
	.global Func_080b1f4c
	.thumb_func
Func_080b1f4c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	ldr	r3, [pc, #336]
	ldr	r3, [r3, #0]
	adds	r7, r2, #0
	str	r3, [sp, #8]
	bl	sub_08077008
	ldr	r2, [sp, #12]
	lsls	r2, r2, #1
	str	r2, [sp, #4]
	adds	r5, r2, #0
	adds	r6, r0, #0
	adds	r5, #216
	ldrh	r3, [r6, r5]
	ldr	r2, [pc, #316]
	ands	r2, r3
	mov	sl, r2
	mov	r0, sl
	bl	sub_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #4
	ands	r3, r2
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	movs	r2, #1
	mov	fp, r3
	negs	r2, r2
	movs	r3, #0
	mov	r9, r0
	str	r3, [sp, #0]
	cmp	r7, r2
	bne.n	.L_080b1fa6
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r7, #1
.L_080b1fa6:
	ldrh	r0, [r6, r5]
	bl	sub_080b19cc
	adds	r2, r7, #0
	muls	r2, r0
	mov	r8, r2
	cmp	r2, #0
	bne.n	.L_080b1fc6
	mov	r0, sl
	movs	r1, #2
	bl	sub_08015120
	ldr	r0, [pc, #252]
	bl	sub_080b0574
	b.n	.L_080b20a0
.L_080b1fc6:
	ldrh	r2, [r6, r5]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b1fee
	mov	r3, r9
	ldrb	r2, [r3, #3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b1fee
	mov	r0, sl
	movs	r1, #2
	bl	sub_08015120
	ldr	r0, [pc, #216]
	bl	sub_080b0574
	b.n	.L_080b20a0
.L_080b1fee:
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L_080b1ff8
	ldr	r5, [pc, #204]
	b.n	.L_080b2020
.L_080b1ff8:
	ldr	r3, [sp, #4]
	adds	r3, #216
	ldrh	r2, [r6, r3]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b200c
	ldr	r5, [pc, #188]
	b.n	.L_080b2020
.L_080b200c:
	cmp	r7, #1
	ble.n	.L_080b2014
	ldr	r5, [pc, #184]
	b.n	.L_080b2020
.L_080b2014:
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_080b201e
	ldr	r5, [pc, #180]
	b.n	.L_080b2020
.L_080b201e:
	ldr	r5, [pc, #180]
.L_080b2020:
	mov	r0, sl
	movs	r1, #2
	bl	sub_08015120
	mov	r0, r8
	movs	r1, #5
	bl	sub_08015120
	adds	r0, r5, #0
	bl	sub_080b0574
	movs	r0, #0
	bl	sub_080b0634
	cmp	r0, #0
	beq.n	.L_080b205a
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L_080b204c
	ldr	r3, [sp, #0]
	cmp	r3, #0
	beq.n	.L_080b2050
.L_080b204c:
	ldr	r5, [pc, #136]
	b.n	.L_080b2052
.L_080b2050:
	ldr	r5, [pc, #136]
.L_080b2052:
	adds	r0, r5, #0
	bl	sub_080b0574
	b.n	.L_080b20a0
.L_080b205a:
	movs	r0, #102
	bl	sub_080f9010
	cmp	r7, #0
	ble.n	.L_080b2074
	adds	r5, r7, #0
.L_080b2066:
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #12]
	subs	r5, #1
	bl	sub_080772b0
	cmp	r5, #0
	bne.n	.L_080b2066
.L_080b2074:
	mov	r0, r8
	bl	sub_08077230
	bl	sub_080b10cc
	ldr	r2, [sp, #8]
	ldr	r1, [sp, #16]
	ldr	r0, [r2, #32]
	bl	sub_080b1dec
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L_080b2094
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L_080b2098
.L_080b2094:
	ldr	r5, [pc, #72]
	b.n	.L_080b209a
.L_080b2098:
	ldr	r5, [pc, #72]
.L_080b209a:
	adds	r0, r5, #0
	bl	sub_080b0574
.L_080b20a0:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x000001ff
	.4byte 0x00000cac
	.4byte 0x00000cab
	.4byte 0x00000cb2
	.4byte 0x00000cb1
	.4byte 0x00000cb0
	.4byte 0x00000caf
	.4byte 0x00000cae
	.4byte 0x00000cb6
	.4byte 0x00000cb4
	.4byte 0x00000cb5
	.4byte 0x00000cb3
