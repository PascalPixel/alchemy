.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08013560, 0x08013560
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08038060, 0x08038060
	.set sub_08038068, 0x08038068
	.set sub_08038080, 0x08038080
	.set sub_08038090, 0x08038090
	.set sub_08038098, 0x08038098
	.set sub_080380a8, 0x080380a8
	.set sub_080380b0, 0x080380b0
	.set sub_08038260, 0x08038260
	.set sub_08038268, 0x08038268
	.set sub_08038270, 0x08038270
	.set sub_080ad010, 0x080ad010
	.set sub_080ad078, 0x080ad078
	.set sub_080f811c, 0x080f811c
	.set sub_080f8888, 0x080f8888
	.set sub_080f88c4, 0x080f88c4
	.set sub_080f8a44, 0x080f8a44
	.set sub_080f8e08, 0x080f8e08
	.set sub_080f8f9c, 0x080f8f9c
	.set sub_080f9170, 0x080f9170
	.set sub_080f9224, 0x080f9224
	.set sub_080f92dc, 0x080f92dc
	.set sub_080fa3d4, 0x080fa3d4
	.set sub_080fa458, 0x080fa458
	.set sub_080fad88, 0x080fad88
	.set sub_080fadd0, 0x080fadd0
	.set sub_080fd6b0, 0x080fd6b0
	.set sub_080fd6f0, 0x080fd6f0
	.set sub_080ff7b4, 0x080ff7b4
	.set sub_08100700, 0x08100700
	.set sub_081c0010, 0x081c0010
	.global Func_080ff850
	.thumb_func
Func_080ff850:
.L_080ff850:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r2, [pc, #116]
	ldr	r3, [pc, #120]
	movs	r7, #0
	mov	r8, r2
	mov	sl, r3
.L_080ff862:
	lsls	r3, r7, #1
	ldr	r2, [pc, #112]
	adds	r3, r3, r7
	lsls	r3, r3, #7
	movs	r6, #0
	adds	r5, r3, r2
.L_080ff86e:
	adds	r0, r5, #0
	movs	r1, #64
	ldr	r2, [pc, #104]
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x2401
	adds	r0, r5, #4
.L_080ff87c:
	adds	r1, r6, #0
	cmp	r7, #1
	bne.n	.L_080ff886
	cmp	r4, #1
	ble.n	.L_080ff8b0
.L_080ff886:
	cmp	r7, #0
	bne.n	.L_080ff898
	subs	r3, r4, #2
	cmp	r6, r3
	ble.n	.L_080ff898
	adds	r1, r3, #0
	cmp	r1, #0
	bge.n	.L_080ff898
	movs	r1, #0
.L_080ff898:
	lsls	r1, r1, #3
	mov	r3, r8
	ldr	r3, [r3, r1]
	ldr	r2, [r0, #0]
	adds	r1, #4
	eors	r2, r3
	str	r2, [r0, #0]
	mov	r3, r8
	ldr	r2, [r0, #32]
	ldr	r1, [r3, r1]
	eors	r2, r1
	str	r2, [r0, #32]
.L_080ff8b0:
	adds	r4, #1
	adds	r0, #4
	cmp	r4, #7
	ble.n	.L_080ff87c
	adds	r6, #1
	adds	r5, #64
	cmp	r6, #5
	ble.n	.L_080ff86e
	adds	r7, #1
	cmp	r7, #1
	ble.n	.L_080ff862
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08105984
	.4byte 0x03000260
	.4byte 0x06006000
	.2byte 0x4444
	.2byte 0x4444
.L_080ff8e0:
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	lsls	r5, r3, #1
	movs	r3, #243
	lsls	r3, r3, #8
	mov	r8, r1
	adds	r3, #1
	movs	r1, #128
	mov	r9, r2
	adds	r3, r3, r5
	lsls	r1, r1, #3
	sub	sp, #4
	movs	r6, #0
	mov	fp, r3
	orrs	r1, r3
	mov	r2, r8
	mov	r3, r9
	str	r6, [sp, #0]
	mov	sl, r0
	bl	sub_08038270
	movs	r3, #243
	lsls	r3, r3, #8
	adds	r5, r5, r3
	mov	r2, r8
	mov	r0, sl
	adds	r1, r5, #0
	mov	r3, r9
	adds	r2, #1
	str	r6, [sp, #0]
	bl	sub_08038270
	movs	r3, #2
	add	r8, r3
	mov	r0, sl
	mov	r1, fp
	mov	r2, r8
	mov	r3, r9
	str	r6, [sp, #0]
	bl	sub_08038270
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}
.L_080ff94c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	mov	fp, r2
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #8]
	mov	r1, fp
	mov	r8, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r1, #16]
	adds	r7, r0, #0
	adds	r3, r3, r2
	mov	r2, r8
	str	r3, [r1, #24]
	ldr	r0, [r2, #48]
	sub	sp, #8
	bl	sub_08038260
	movs	r0, #1
	bl	sub_08013560
	mov	r1, fp
	ldr	r3, [r1, #24]
	movs	r2, #226
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	mov	r1, r8
	ldrh	r2, [r1, r3]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080ffa3a
	movs	r5, #252
	lsls	r5, r5, #6
	ldr	r3, [pc, #424]
	adds	r5, #255
	adds	r0, r5, #0
	ands	r0, r2
	adds	r0, r0, r3
	ldr	r1, [r1, #48]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	mov	r2, fp
	ldr	r3, [r2, #24]
	movs	r1, #226
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r3, r1
	mov	r2, r8
	ldrh	r3, [r2, r3]
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_080ad078
	movs	r3, #104
	adds	r5, r0, #0
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r3, #224
	movs	r1, #0
	movs	r2, #96
	bl	sub_08038068
	movs	r3, #0
	mov	sl, r3
	ldrb	r3, [r5, #6]
	cmp	r3, #0
	bne.n	.L_080ff9f4
	ldrb	r0, [r5, #1]
	movs	r3, #64
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080ff9fa
	b.n	.L_080ff9f6
.L_080ff9f4:
	ldrb	r0, [r5, #1]
.L_080ff9f6:
	movs	r1, #2
	mov	sl, r1
.L_080ff9fa:
	movs	r3, #128
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080ffa0a
	mov	r2, sl
	movs	r3, #1
	orrs	r2, r3
	mov	sl, r2
.L_080ffa0a:
	mov	r3, sl
	cmp	r3, #3
	bne.n	.L_080ffa14
	ldr	r0, [pc, #316]
	b.n	.L_080ffa1c
.L_080ffa14:
	mov	r1, sl
	cmp	r1, #2
	bne.n	.L_080ffa28
	ldr	r0, [pc, #312]
.L_080ffa1c:
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #96
	bl	sub_08038080
	b.n	.L_080ffa3a
.L_080ffa28:
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L_080ffa3a
	ldr	r0, [pc, #296]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #96
	bl	sub_08038080
.L_080ffa3a:
	mov	r3, fp
	ldr	r2, [r3, #8]
	movs	r1, #0
	lsls	r3, r2, #2
	adds	r3, r3, r2
	mov	sl, r1
	lsls	r3, r3, #1
	movs	r1, #226
	add	r3, r8
	lsls	r1, r1, #1
	movs	r2, #1
	adds	r1, r1, r3
	mov	r9, r2
	movs	r6, #2
	mov	r8, r1
.L_080ffa58:
	mov	r2, fp
	ldr	r3, [r2, #16]
	cmp	sl, r3
	bne.n	.L_080ffabe
	mov	r1, r8
	ldrh	r3, [r1, #0]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	bl	sub_080ad078
	adds	r5, r0, #0
	ldrb	r3, [r5, #2]
	cmp	r3, #4
	beq.n	.L_080ffaa8
	adds	r1, r3, #0
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r1, #1
	adds	r0, r7, #0
	movs	r2, #24
	adds	r3, r6, #0
	bl	sub_08038270
	mov	r2, r9
	movs	r3, #14
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r7, #0
	movs	r1, #9
	adds	r2, r6, #0
	movs	r3, #15
	bl	sub_080f9224
	mov	r1, r9
	movs	r2, #14
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	b.n	.L_080ffb04
.L_080ffaa8:
	mov	r3, r9
	movs	r1, #14
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	adds	r0, r7, #0
	movs	r1, #9
	adds	r2, r6, #0
	movs	r3, #19
	bl	sub_080f9224
	b.n	.L_080ffb26
.L_080ffabe:
	mov	r2, r8
	ldrh	r3, [r2, #0]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	bl	sub_080ad078
	adds	r5, r0, #0
	ldrb	r3, [r5, #2]
	cmp	r3, #4
	beq.n	.L_080ffb12
	adds	r1, r3, #0
	movs	r3, #4
	str	r3, [sp, #0]
	adds	r1, #1
	adds	r0, r7, #0
	movs	r2, #24
	adds	r3, r6, #0
	bl	sub_08038270
	mov	r3, r9
	movs	r1, #15
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	adds	r0, r7, #0
	movs	r1, #9
	adds	r2, r6, #0
	movs	r3, #15
	bl	sub_080f9224
	mov	r2, r9
	movs	r3, #15
	str	r2, [sp, #0]
	str	r3, [sp, #4]
.L_080ffb04:
	adds	r0, r7, #0
	movs	r1, #25
	adds	r2, r6, #0
	movs	r3, #3
	bl	sub_080f9224
	b.n	.L_080ffb26
.L_080ffb12:
	mov	r1, r9
	movs	r2, #15
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	adds	r0, r7, #0
	movs	r1, #9
	adds	r2, r6, #0
	movs	r3, #19
	bl	sub_080f9224
.L_080ffb26:
	movs	r1, #1
	add	sl, r1
	movs	r3, #2
	mov	r2, sl
	adds	r6, #2
	add	r8, r3
	cmp	r2, #4
	ble.n	.L_080ffa58
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #1
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00000885
	.4byte 0x00001044
	.4byte 0x00001043
	.2byte 0x1042
	.2byte 0x0000
.L_080ffb5c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	r8, r0
	mov	fp, r3
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, fp
	ldrb	r0, [r3, #0]
	adds	r5, r2, #0
	sub	sp, #8
	bl	sub_08016ca4
	str	r0, [sp, #4]
	mov	r0, r8
	bl	sub_08038260
	ldr	r2, [r5, #8]
	lsls	r3, r2, #2
	adds	r6, r3, r2
	ldr	r3, [r5, #20]
	subs	r3, r3, r6
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	r9, r3
	cmp	r3, #5
	bls.n	.L_080ffba8
	movs	r1, #5
	mov	r9, r1
.L_080ffba8:
	movs	r3, #58
	str	r3, [sp, #0]
	movs	r0, #5
	adds	r1, r6, #0
	mov	r2, r8
	movs	r3, #80
	bl	sub_080f92dc
	movs	r2, #28
	ldr	r1, [r5, #20]
	ldr	r3, [r5, #8]
	mov	r0, r8
	str	r2, [sp, #0]
	movs	r2, #5
	bl	sub_080f9170
	movs	r2, #176
	movs	r3, #0
	ldr	r0, [pc, #240]
	mov	r1, r8
	bl	sub_08038080
	movs	r2, #0
	mov	r3, r9
	mov	sl, r2
	cmp	r3, #0
	bls.n	.L_080ffc56
	movs	r1, #226
	lsls	r3, r6, #1
	lsls	r1, r1, #1
	adds	r7, r3, r1
.L_080ffbe6:
	mov	r2, fp
	ldrh	r3, [r7, r2]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	bl	sub_080ad078
	mov	r1, fp
	ldrh	r3, [r7, r1]
	adds	r6, r0, #0
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	mov	r2, sl
	ldr	r3, [pc, #188]
	lsls	r5, r2, #4
	adds	r5, #16
	adds	r0, r0, r3
	mov	r1, r8
	movs	r2, #88
	adds	r3, r5, #0
	bl	sub_08038080
	ldrb	r0, [r6, #9]
	movs	r1, #2
	mov	r2, r8
	movs	r3, #176
	str	r5, [sp, #0]
	bl	sub_080380a8
	ldrb	r4, [r6, #8]
	cmp	r4, #255
	bne.n	.L_080ffc30
	movs	r4, #11
	b.n	.L_080ffc32
.L_080ffc30:
	subs	r4, #1
.L_080ffc32:
	mov	r3, sl
	lsls	r2, r3, #1
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r2, #2
	adds	r3, r4, #0
	mov	r0, r8
	movs	r1, #25
	bl	.L_080ff8e0
	mov	r3, sl
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	sl, r3
	adds	r7, #2
	cmp	r9, sl
	bhi.n	.L_080ffbe6
.L_080ffc56:
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, fp
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080ffc6e
	ldr	r0, [pc, #100]
	mov	r1, r8
	movs	r2, #96
	movs	r3, #17
	bl	sub_08038080
.L_080ffc6e:
	ldr	r0, [sp, #4]
	mov	r1, r8
	movs	r2, #40
	movs	r3, #0
	bl	sub_08038090
	ldr	r1, [sp, #4]
	movs	r2, #42
	adds	r2, #255
	adds	r3, r1, r2
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #68]
	mov	r1, r8
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #32
	bl	sub_08038080
	mov	r1, r8
	ldr	r0, [pc, #56]
	movs	r2, #0
	movs	r3, #48
	bl	sub_08038098
	ldr	r3, [sp, #4]
	movs	r1, #2
	ldrb	r0, [r3, #15]
	movs	r3, #48
	str	r3, [sp, #0]
	mov	r2, r8
	movs	r3, #24
	bl	sub_080380b0
	movs	r0, #1
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0000101c
	.4byte 0x000005a7
	.4byte 0x0000101e
	.4byte 0x00000b63
	.2byte 0x5974
	.2byte 0x0810
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	sub	sp, #48
	ldr	r2, [r7, #40]
	movs	r3, #0
	str	r3, [sp, #16]
	mov	sl, r2
	movs	r3, #5
	movs	r2, #1
	adds	r0, r7, #0
	movs	r4, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r3, #30
	mov	fp, r2
	adds	r0, #48
	movs	r2, #0
	mov	r8, r4
	str	r4, [sp, #4]
	bl	sub_080f811c
	adds	r3, r7, #0
	adds	r3, #240
	str	r3, [sp, #12]
	adds	r0, r7, #0
	ldr	r1, [r3, #0]
	bl	sub_080fa3d4
	ldr	r5, [pc, #444]
	movs	r6, #24
	negs	r6, r6
	adds	r0, r5, #0
	mov	r1, sl
	movs	r2, #0
	adds	r3, r6, #0
	subs	r5, #2
	bl	sub_08038080
	adds	r0, r5, #0
	mov	r1, sl
	movs	r2, #64
	adds	r3, r6, #0
	bl	sub_08038080
	bl	.L_080ff850
	movs	r4, #226
	lsls	r4, r4, #1
	adds	r4, r4, r7
	mov	r9, r4
	b.n	.L_080ffeaa
.L_080ffd4c:
	mov	r2, r8
	add	r6, sp, #20
	cmp	r2, #0
	beq.n	.L_080ffdba
	cmp	r2, #2
	bne.n	.L_080ffd96
	movs	r4, #128
	lsls	r4, r4, #2
	adds	r4, #22
	adds	r3, r7, r4
	ldrb	r0, [r3, #0]
	bl	sub_08016ca4
	mov	r1, r9
	movs	r2, #0
	bl	sub_080fd6f0
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r7, r2
	add	r6, sp, #20
	strb	r0, [r3, #0]
	movs	r1, #0
	adds	r0, r6, #0
	bl	sub_080ff7b4
	bl	sub_080fa458
	mov	r0, r9
	bl	sub_080fd6b0
	movs	r3, #1
	movs	r0, #1
	mov	fp, r3
	bl	sub_08013560
	b.n	.L_080ffd98
.L_080ffd96:
	add	r6, sp, #20
.L_080ffd98:
	mov	r4, fp
	cmp	r4, #0
	beq.n	.L_080ffdac
	movs	r2, #0
	mov	fp, r2
	mov	r0, sl
	movs	r1, #0
	adds	r2, r6, #0
	bl	.L_080ffb5c
.L_080ffdac:
	mov	r0, sl
	movs	r1, #0
	adds	r2, r6, #0
	bl	.L_080ff94c
	movs	r3, #0
	mov	r8, r3
.L_080ffdba:
	movs	r0, #1
	bl	sub_08013560
	add	r3, sp, #28
	ldr	r1, [r6, #20]
	movs	r0, #0
	str	r3, [sp, #0]
	movs	r2, #5
	add	r3, sp, #36
	bl	sub_080f8f9c
	ldr	r1, [r6, #16]
	adds	r5, r0, #0
	lsls	r1, r1, #4
	adds	r1, #60
	movs	r0, #55
	bl	sub_080f8a44
	cmp	r5, #1
	bne.n	.L_080ffde8
	movs	r4, #1
	mov	r8, r4
	mov	fp, r4
.L_080ffde8:
	cmp	r5, #0
	bne.n	.L_080ffdf0
	movs	r2, #1
	mov	r8, r2
.L_080ffdf0:
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L_080ffdfc
	movs	r4, #0
	mov	r8, r4
.L_080ffdfc:
	ldr	r5, [pc, #224]
	movs	r2, #1
	ldr	r3, [r5, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ffe14
	movs	r0, #112
	bl	sub_081c0010
	movs	r2, #1
	str	r2, [sp, #16]
	b.n	.L_080ffeb8
.L_080ffe14:
	ldr	r3, [r5, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ffe2c
	movs	r0, #113
	bl	sub_081c0010
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #16]
	b.n	.L_080ffeb8
.L_080ffe2c:
	ldr	r3, [r5, #12]
	movs	r1, #128
	lsls	r1, r1, #1
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_080ffe44
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ffeaa
.L_080ffe44:
	movs	r0, #111
	str	r1, [sp, #8]
	bl	sub_081c0010
	movs	r0, #28
	ldrsb	r0, [r7, r0]
	movs	r4, #129
	lsls	r3, r0, #1
	lsls	r4, r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r7, r3]
	movs	r2, #153
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r6, #24]
	ldr	r1, [sp, #8]
	strb	r2, [r7, r3]
	ldr	r3, [r5, #12]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ffe72
	adds	r0, #1
	b.n	.L_080ffe74
.L_080ffe72:
	subs	r0, #1
.L_080ffe74:
	movs	r4, #139
	lsls	r4, r4, #1
	adds	r4, #255
	adds	r3, r7, r4
	ldrb	r1, [r3, #0]
	adds	r0, r0, r1
	bl	sub_08002064
	movs	r3, #129
	lsls	r2, r0, #1
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrh	r3, [r7, r2]
	movs	r4, #128
	str	r3, [r7, #8]
	ldrh	r1, [r7, r2]
	lsls	r4, r4, #2
	adds	r4, #22
	adds	r3, r7, r4
	strb	r1, [r3, #0]
	strb	r0, [r7, #28]
	adds	r0, r7, #0
	ldrh	r1, [r7, r2]
	bl	sub_080f88c4
	movs	r2, #2
	mov	r8, r2
.L_080ffeaa:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ffeb8
	b.n	.L_080ffd4c
.L_080ffeb8:
	ldr	r0, [r7, #48]
	bl	sub_08038060
	mov	r0, sl
	bl	sub_08038260
	ldr	r3, [sp, #12]
	ldr	r0, [r3, #0]
	bl	sub_08038268
	ldr	r0, [sp, #16]
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00001037
	.2byte 0x1150
	.2byte 0x0300
.L_080ffee4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	mov	sl, r2
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #8]
	mov	r8, r3
	lsls	r3, r2, #2
	mov	r1, sl
	adds	r3, r3, r2
	mov	r9, r3
	ldr	r3, [r1, #16]
	mov	r2, r8
	add	r3, r9
	str	r3, [r1, #24]
	adds	r6, r0, #0
	ldr	r0, [r2, #48]
	sub	sp, #8
	bl	sub_08038260
	movs	r0, #1
	bl	sub_08013560
	mov	r1, sl
	ldr	r3, [r1, #24]
	movs	r2, #226
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	mov	r1, r8
	ldrh	r2, [r1, r3]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080fff4c
	movs	r0, #128
	ldr	r3, [pc, #416]
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r2
	adds	r0, r0, r3
	ldr	r1, [r1, #48]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
.L_080fff4c:
	movs	r2, #1
	mov	r1, r9
	mov	fp, r2
	lsls	r3, r1, #1
	movs	r2, #226
	add	r3, r8
	lsls	r2, r2, #1
	adds	r2, r2, r3
	movs	r7, #0
	movs	r5, #1
	mov	r9, r2
.L_080fff62:
	mov	r1, sl
	ldr	r3, [r1, #16]
	cmp	r7, r3
	bne.n	.L_080fffb4
	ldr	r3, [r1, #24]
	movs	r2, #226
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	mov	r1, r8
	ldrh	r3, [r1, r3]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	bl	sub_080ad010
	ldr	r0, [r0, #20]
	cmp	r0, #4
	beq.n	.L_080fffa0
	movs	r3, #0
	adds	r1, r0, #1
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r2, #27
	adds	r3, r5, #0
	bl	sub_08038270
	mov	r2, fp
	movs	r3, #14
	b.n	.L_080fffde
.L_080fffa0:
	mov	r3, fp
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r3, #14
	movs	r1, #14
	adds	r2, r5, #0
	str	r3, [sp, #4]
	bl	sub_080f9224
	b.n	.L_08100004
.L_080fffb4:
	mov	r1, r9
	ldrh	r3, [r1, #0]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	bl	sub_080ad010
	ldr	r0, [r0, #20]
	cmp	r0, #4
	beq.n	.L_080ffff0
	movs	r3, #4
	adds	r1, r0, #1
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r2, #27
	adds	r3, r5, #0
	bl	sub_08038270
	mov	r2, fp
	movs	r3, #15
.L_080fffde:
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #14
	adds	r2, r5, #0
	movs	r3, #13
	bl	sub_080f9224
	b.n	.L_08100004
.L_080ffff0:
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r3, #15
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #14
	adds	r2, r5, #0
	movs	r3, #14
	bl	sub_080f9224
.L_08100004:
	movs	r1, #2
	adds	r7, #1
	adds	r5, #2
	add	r9, r1
	cmp	r7, #4
	ble.n	.L_080fff62
	movs	r2, #1
	mov	r9, r2
	movs	r5, #1
	movs	r6, #15
	movs	r7, #3
.L_0810001a:
	mov	r3, r8
	mov	r1, r9
	ldr	r0, [r3, #52]
	adds	r2, r5, #0
	str	r1, [sp, #0]
	movs	r3, #9
	movs	r1, #2
	subs	r7, #1
	str	r6, [sp, #4]
	adds	r5, #2
	bl	sub_080f9224
	cmp	r7, #0
	bge.n	.L_0810001a
	mov	r2, sl
	ldr	r3, [r2, #24]
	movs	r1, #226
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r3, r1
	mov	r1, r8
	ldrh	r2, [r1, r3]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081000c6
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r2
	bl	sub_080ad010
	ldrb	r1, [r0, #2]
	cmp	r1, #2
	beq.n	.L_081000b0
	cmp	r1, #2
	bgt.n	.L_0810006c
	cmp	r1, #1
	beq.n	.L_08100076
	b.n	.L_081000c6
.L_0810006c:
	cmp	r1, #3
	beq.n	.L_08100098
	cmp	r1, #4
	beq.n	.L_08100086
	b.n	.L_081000c6
.L_08100076:
	mov	r2, r8
	movs	r3, #14
	ldr	r0, [r2, #52]
	str	r1, [sp, #0]
	str	r3, [sp, #4]
	movs	r1, #2
	movs	r2, #1
	b.n	.L_081000a8
.L_08100086:
	mov	r3, r8
	ldr	r0, [r3, #52]
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #14
	str	r3, [sp, #4]
	movs	r1, #2
	movs	r2, #3
	b.n	.L_081000a8
.L_08100098:
	mov	r1, r8
	movs	r3, #1
	ldr	r0, [r1, #52]
	str	r3, [sp, #0]
	movs	r3, #14
	str	r3, [sp, #4]
	movs	r1, #2
	movs	r2, #5
.L_081000a8:
	movs	r3, #9
	bl	sub_080f9224
	b.n	.L_081000c6
.L_081000b0:
	mov	r2, r8
	movs	r3, #1
	ldr	r0, [r2, #52]
	str	r3, [sp, #0]
	movs	r3, #14
	str	r3, [sp, #4]
	movs	r1, #2
	movs	r2, #7
	movs	r3, #9
	bl	sub_080f9224
.L_081000c6:
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #1
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0092
	.2byte 0x0000
.L_081000e0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	mov	r9, r3
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r0, [r3, #0]
	adds	r5, r2, #0
	sub	sp, #4
	bl	sub_08016ca4
	movs	r3, #96
	str	r3, [sp, #0]
	movs	r2, #8
	movs	r3, #224
	mov	fp, r0
	movs	r1, #128
	adds	r0, r7, #0
	bl	sub_08038068
	ldr	r2, [r5, #8]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	mov	r8, r3
	ldr	r3, [r5, #20]
	mov	r2, r8
	subs	r3, r3, r2
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	sl, r3
	cmp	r3, #5
	bls.n	.L_0810013a
	movs	r3, #5
	mov	sl, r3
.L_0810013a:
	movs	r3, #52
	str	r3, [sp, #0]
	movs	r0, #5
	mov	r1, r8
	adds	r2, r7, #0
	movs	r3, #123
	bl	sub_080f92dc
	movs	r2, #28
	ldr	r3, [r5, #8]
	ldr	r1, [r5, #20]
	adds	r0, r7, #0
	str	r2, [sp, #0]
	movs	r2, #5
	bl	sub_080f9170
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08100174
	ldr	r0, [pc, #168]
	adds	r1, r7, #0
	movs	r2, #120
	movs	r3, #8
	bl	sub_08038080
	b.n	.L_081001b2
.L_08100174:
	mov	r2, sl
	movs	r6, #0
	cmp	r2, #0
	bls.n	.L_081001b2
	mov	r2, r8
	lsls	r3, r2, #1
	movs	r2, #226
	add	r3, r9
	lsls	r2, r2, #1
	adds	r5, r3, r2
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	mov	r8, r3
.L_08100190:
	ldrh	r3, [r5, #0]
	mov	r0, r8
	ands	r0, r3
	ldr	r3, [pc, #124]
	adds	r1, r7, #0
	adds	r0, r0, r3
	lsls	r3, r6, #4
	adds	r3, #8
	movs	r2, #136
	bl	sub_08038080
	adds	r3, r6, #1
	lsls	r3, r3, #24
	lsrs	r6, r3, #24
	adds	r5, #2
	cmp	sl, r6
	bhi.n	.L_08100190
.L_081001b2:
	mov	r0, fp
	adds	r1, r7, #0
	movs	r2, #40
	movs	r3, #0
	bl	sub_08038090
	ldr	r5, [pc, #88]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #32
	movs	r3, #16
	adds	r5, #1
	bl	sub_08038080
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #32
	movs	r3, #24
	bl	sub_08038080
	mov	r2, fp
	movs	r3, #16
	ldrh	r0, [r2, #60]
	movs	r1, #3
	str	r3, [sp, #0]
	adds	r2, r7, #0
	movs	r3, #72
	bl	sub_080380b0
	mov	r3, fp
	ldrh	r0, [r3, #62]
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r1, #3
	adds	r2, r7, #0
	movs	r3, #72
	bl	sub_080380b0
	movs	r0, #1
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00001006
	.4byte 0x0000025f
	.2byte 0x1026
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	sub	sp, #80
	ldr	r1, [r7, #40]
	movs	r4, #1
	mov	r9, r1
	movs	r2, #0
	movs	r1, #128
	str	r2, [sp, #16]
	str	r4, [sp, #12]
	lsls	r1, r1, #2
	adds	r1, #22
	movs	r3, #2
	mov	sl, r3
	adds	r3, r7, r1
	ldrb	r0, [r3, #0]
	bl	sub_08016ca4
	mov	r2, sl
	mov	fp, r0
	movs	r3, #10
	adds	r0, r7, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #10
	movs	r3, #15
	adds	r0, #52
	bl	sub_080f811c
	adds	r3, r7, #0
	adds	r3, #240
	str	r3, [sp, #8]
	adds	r0, r7, #0
	ldr	r1, [r3, #0]
	bl	sub_080fa3d4
	ldr	r5, [pc, #564]
	movs	r6, #24
	negs	r6, r6
	adds	r0, r5, #0
	mov	r1, r9
	adds	r3, r6, #0
	movs	r2, #0
	subs	r5, #3
	bl	sub_08038080
	adds	r0, r5, #0
	mov	r1, r9
	movs	r2, #64
	adds	r3, r6, #0
	bl	sub_08038080
	b.n	.L_0810047e
.L_0810029a:
	add	r4, sp, #52
	mov	r1, sl
	mov	r8, r4
	cmp	r1, #0
	beq.n	.L_08100374
	cmp	r1, #2
	bne.n	.L_08100330
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #22
	adds	r3, r7, r2
	ldrb	r0, [r3, #0]
	bl	sub_08016ca4
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r1, r7, r3
	movs	r2, #0
	mov	fp, r0
	bl	sub_080fad88
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r3, r7, r4
	strb	r0, [r3, #0]
	mov	r0, r9
	bl	sub_08038260
	mov	r2, fp
	add	r4, sp, #20
	adds	r2, #216
	movs	r0, #0
	movs	r1, #14
.L_081002dc:
	ldrh	r3, [r2, #0]
	subs	r1, #1
	strh	r3, [r0, r4]
	adds	r2, #2
	adds	r0, #2
	cmp	r1, #0
	bge.n	.L_081002dc
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r5, r7, r1
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_080f8e08
	add	r2, sp, #52
	mov	r8, r2
	movs	r1, #0
	mov	r0, r8
	bl	sub_080ff7b4
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_080fadd0
	ldr	r0, [r7, #52]
	bl	sub_08038260
	movs	r4, #128
	lsls	r4, r4, #2
	adds	r4, #22
	adds	r3, r7, r4
	ldr	r0, [r7, #52]
	ldrb	r1, [r3, #0]
	movs	r2, #1
	bl	.L_081004b8
	movs	r1, #1
	movs	r0, #1
	str	r1, [sp, #12]
	bl	sub_08013560
	b.n	.L_08100334
.L_08100330:
	add	r2, sp, #52
	mov	r8, r2
.L_08100334:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq.n	.L_08100348
	movs	r4, #0
	mov	r0, r9
	movs	r1, #0
	mov	r2, r8
	str	r4, [sp, #12]
	bl	.L_081000e0
.L_08100348:
	movs	r1, #0
	mov	r2, r8
	mov	r0, r9
	bl	.L_080ffee4
	ldr	r5, [pc, #348]
	movs	r6, #24
	negs	r6, r6
	adds	r0, r5, #0
	mov	r1, r9
	movs	r2, #0
	adds	r3, r6, #0
	bl	sub_08038080
	mov	r1, r9
	subs	r0, r5, #3
	movs	r2, #64
	adds	r3, r6, #0
	bl	sub_08038080
	movs	r1, #0
	mov	sl, r1
.L_08100374:
	movs	r0, #1
	bl	sub_08013560
	mov	r2, r8
	add	r3, sp, #60
	ldr	r1, [r2, #20]
	movs	r0, #0
	str	r3, [sp, #0]
	movs	r2, #5
	add	r3, sp, #68
	bl	sub_080f8f9c
	mov	r3, r8
	ldr	r1, [r3, #16]
	adds	r5, r0, #0
	lsls	r1, r1, #4
	adds	r1, #52
	movs	r0, #96
	bl	sub_080f8a44
	cmp	r5, #1
	bne.n	.L_081003a6
	movs	r4, #1
	str	r4, [sp, #12]
	mov	sl, r4
.L_081003a6:
	cmp	r5, #0
	bne.n	.L_081003ae
	movs	r1, #1
	mov	sl, r1
.L_081003ae:
	movs	r2, #1
	negs	r2, r2
	cmp	r5, r2
	bne.n	.L_081003ba
	movs	r3, #0
	mov	sl, r3
.L_081003ba:
	ldr	r5, [pc, #248]
	movs	r2, #1
	ldr	r3, [r5, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081003d2
	movs	r0, #112
	bl	sub_081c0010
	movs	r4, #1
	str	r4, [sp, #16]
	b.n	.L_0810048c
.L_081003d2:
	ldr	r3, [r5, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081003ea
	movs	r0, #113
	bl	sub_081c0010
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #16]
	b.n	.L_0810048c
.L_081003ea:
	ldr	r3, [r5, #12]
	movs	r6, #128
	lsls	r6, r6, #1
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_08100402
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810047e
.L_08100402:
	movs	r0, #111
	bl	sub_081c0010
	movs	r0, #28
	ldrsb	r0, [r7, r0]
	movs	r2, #129
	lsls	r2, r2, #2
	lsls	r3, r0, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	mov	r1, r8
	movs	r4, #153
	ldr	r2, [r1, #24]
	lsls	r4, r4, #2
	adds	r3, r3, r4
	strb	r2, [r7, r3]
	ldr	r3, [r5, #12]
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0810042e
	adds	r0, #1
	b.n	.L_08100430
.L_0810042e:
	subs	r0, #1
.L_08100430:
	mov	r2, fp
	add	r5, sp, #20
	adds	r2, #216
	movs	r4, #0
	movs	r1, #14
.L_0810043a:
	ldrh	r3, [r4, r5]
	subs	r1, #1
	strh	r3, [r2, #0]
	adds	r4, #2
	adds	r2, #2
	cmp	r1, #0
	bge.n	.L_0810043a
	movs	r2, #139
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r7, r2
	ldrb	r1, [r3, #0]
	adds	r0, r0, r1
	bl	sub_08002064
	movs	r3, #129
	lsls	r2, r0, #1
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrh	r3, [r7, r2]
	movs	r4, #128
	str	r3, [r7, #8]
	ldrh	r1, [r7, r2]
	lsls	r4, r4, #2
	adds	r4, #22
	adds	r3, r7, r4
	strb	r1, [r3, #0]
	strb	r0, [r7, #28]
	adds	r0, r7, #0
	ldrh	r1, [r7, r2]
	bl	sub_080f88c4
	movs	r1, #2
	mov	sl, r1
.L_0810047e:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_0810048c
	b.n	.L_0810029a
.L_0810048c:
	ldr	r0, [r7, #48]
	bl	sub_08038060
	mov	r0, r9
	bl	sub_08038260
	ldr	r2, [sp, #8]
	ldr	r0, [r2, #0]
	bl	sub_08038268
	ldr	r0, [sp, #16]
	add	sp, #80
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00001038
	.2byte 0x1150
	.2byte 0x0300
.L_081004b8:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	mov	sl, r2
	mov	r8, r3
	bl	sub_08100700
	bl	sub_080fa458
	ldr	r5, [pc, #100]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	adds	r0, r5, #1
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #32
	bl	sub_08038080
	adds	r0, r5, #2
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #16
	adds	r5, #3
	bl	sub_08038080
	adds	r0, r5, #0
	movs	r5, #226
	lsls	r5, r5, #1
	movs	r3, #48
	adds	r1, r6, #0
	movs	r2, #0
	add	r5, r8
	bl	sub_08038080
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	.L_08100540
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_08100532
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_080fadd0
	adds	r0, r5, #0
	bl	.L_08100660
.L_08100532:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x1053
	.2byte 0x0000
.L_08100540:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #56]
	adds	r6, r0, #0
	mov	r8, r3
	movs	r3, #14
	adds	r7, r1, #0
	mov	sl, r3
.L_08100554:
	ldrh	r0, [r7, #0]
	ldr	r3, [pc, #36]
	adds	r7, #2
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_081005d0
	ldr	r3, [pc, #28]
	adds	r5, r3, #0
	ands	r5, r0
	adds	r0, r5, #0
	bl	sub_080ad010
	ldrb	r3, [r0, #2]
	cmp	r3, #2
	beq.n	.L_081005a2
	cmp	r3, #2
	bgt.n	.L_08100588
	cmp	r3, #1
	beq.n	.L_08100592
	b.n	.L_081005d0
	.4byte 0x00000200
	.4byte 0x000001ff
	.2byte 0x025f
	.2byte 0x0000
.L_08100588:
	cmp	r3, #3
	beq.n	.L_081005b2
	cmp	r3, #4
	beq.n	.L_081005c2
	b.n	.L_081005d0
.L_08100592:
	mov	r3, r8
	adds	r0, r5, r3
	adds	r1, r6, #0
	movs	r2, #16
	movs	r3, #8
	bl	sub_08038080
	b.n	.L_081005d0
.L_081005a2:
	mov	r3, r8
	adds	r0, r5, r3
	adds	r1, r6, #0
	movs	r2, #16
	movs	r3, #56
	bl	sub_08038080
	b.n	.L_081005d0
.L_081005b2:
	mov	r3, r8
	adds	r0, r5, r3
	adds	r1, r6, #0
	movs	r2, #16
	movs	r3, #40
	bl	sub_08038080
	b.n	.L_081005d0
.L_081005c2:
	mov	r3, r8
	adds	r0, r5, r3
	adds	r1, r6, #0
	movs	r2, #16
	movs	r3, #24
	bl	sub_08038080
.L_081005d0:
	movs	r3, #1
	negs	r3, r3
	add	sl, r3
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_08100554
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #4
	adds	r6, r3, #0
	mov	sl, r0
	mov	r8, r1
	adds	r7, r2, #0
	movs	r5, #0
	adds	r6, #76
.L_08100602:
	ldmia	r6!, {r0}
	cmp	r0, #0
	beq.n	.L_08100614
	adds	r1, r5, #0
	mov	r2, sl
	mov	r3, r8
	str	r7, [sp, #0]
	bl	.L_08100624
.L_08100614:
	adds	r5, #1
	cmp	r5, #31
	ble.n	.L_08100602
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
.L_08100624:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r1, #0
	adds	r6, r0, #0
	mov	r8, r2
	adds	r7, r3, #0
	cmp	r5, #31
	ble.n	.L_08100638
	movs	r5, #0
.L_08100638:
	ldr	r1, [sp, #20]
	adds	r0, r5, #0
	bl	sub_08002054
	lsls	r0, r0, #4
	adds	r0, r0, r7
	strh	r0, [r6, #8]
	ldr	r1, [sp, #20]
	adds	r0, r5, #0
	bl	sub_08002064
	lsls	r0, r0, #4
	add	r0, r8
	strh	r0, [r6, #6]
	adds	r0, r6, #0
	bl	sub_080f8888
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_08100660:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	adds	r5, r0, #0
	adds	r7, r6, #0
	sub	sp, #4
	bl	sub_08100700
	movs	r3, #14
	movs	r1, #216
	adds	r7, #76
	adds	r6, r5, #0
	mov	r8, r3
.L_08100682:
	ldrh	r2, [r6, #0]
	adds	r6, #2
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_081006e8
	ldr	r3, [pc, #36]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081006e8
	ldr	r5, [r7, #0]
	cmp	r5, #0
	beq.n	.L_081006e8
	ldr	r0, [pc, #28]
	str	r1, [sp, #0]
	ands	r0, r2
	bl	sub_080ad010
	ldrb	r3, [r0, #2]
	ldr	r1, [sp, #0]
	cmp	r3, #2
	beq.n	.L_081006d0
	cmp	r3, #2
	bgt.n	.L_081006c2
	b.n	.L_081006bc
	movs	r0, r0
	.4byte 0x00000200
	.2byte 0x01ff
	.2byte 0x0000
.L_081006bc:
	cmp	r3, #1
	beq.n	.L_081006cc
	b.n	.L_081006de
.L_081006c2:
	cmp	r3, #3
	beq.n	.L_081006d4
	cmp	r3, #4
	beq.n	.L_081006d8
	b.n	.L_081006de
.L_081006cc:
	movs	r3, #32
	b.n	.L_081006da
.L_081006d0:
	movs	r3, #80
	b.n	.L_081006da
.L_081006d4:
	movs	r3, #64
	b.n	.L_081006da
.L_081006d8:
	movs	r3, #48
.L_081006da:
	strh	r1, [r5, #6]
	strh	r3, [r5, #8]
.L_081006de:
	adds	r0, r5, #0
	str	r1, [sp, #0]
	bl	sub_080f8888
	ldr	r1, [sp, #0]
.L_081006e8:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r3, r8
	adds	r7, #4
	cmp	r3, #0
	bge.n	.L_08100682
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
