.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_08015080, 0x08015080
	.set sub_080150d0, 0x080150d0
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_080a10d0, 0x080a10d0
	.set sub_080a112c, 0x080a112c
	.set sub_080a1a40, 0x080a1a40
	.set sub_080a1e38, 0x080a1e38
	.set sub_080a33d4, 0x080a33d4
	.set sub_080a38a8, 0x080a38a8
	.set sub_080a3d6c, 0x080a3d6c
	.set sub_080a3ddc, 0x080a3ddc
	.set sub_080a3e88, 0x080a3e88
	.set sub_080f9010, 0x080f9010
	.global Overlay_080a35f8
Overlay_080a35f8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r0, [sp, #28]
	ldr	r3, [pc, #276]
	ldr	r3, [r3, #0]
	movs	r1, #28
	ldrsb	r1, [r3, r1]
	movs	r2, #30
	ldrsb	r2, [r3, r2]
	mov	r8, r1
	movs	r1, #0
	str	r2, [sp, #24]
	str	r1, [sp, #20]
	str	r1, [sp, #12]
	str	r1, [sp, #8]
	mov	r2, r8
	lsls	r7, r2, #1
	mov	sl, r3
	ldrh	r0, [r7, r0]
	movs	r3, #1
	mov	fp, r3
	bl	sub_08077008
	mov	r5, sl
	movs	r3, #10
	adds	r5, #32
	str	r0, [sp, #16]
	str	r3, [sp, #0]
	movs	r7, #2
	adds	r0, r5, #0
	movs	r1, #13
	movs	r2, #3
	movs	r3, #17
	str	r7, [sp, #4]
	bl	sub_080a10d0
	cmp	r0, #0
	beq.n	.L_080a3658
	ldr	r1, [r5, #0]
	mov	r0, sl
	bl	sub_080a33d4
.L_080a3658:
	mov	r6, sl
	movs	r3, #4
	adds	r6, #40
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #13
	movs	r2, #13
	movs	r3, #17
	str	r7, [sp, #4]
	bl	sub_080a10d0
	cmp	r0, #0
	beq.n	.L_080a368e
	ldr	r3, [sp, #20]
	ldr	r2, [r6, #0]
	movs	r0, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r3, #0
	bl	sub_080150d0
	movs	r3, #135
	lsls	r3, r3, #2
	add	r3, sl
	str	r0, [r3, #0]
	movs	r3, #13
	strb	r3, [r0, #5]
.L_080a368e:
	ldr	r5, [pc, #148]
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	sub_08015080
	ldr	r1, [r6, #0]
	movs	r2, #0
	movs	r3, #8
	adds	r0, r5, #0
	bl	sub_08015080
	mov	r1, sl
	ldr	r3, [r1, #20]
	mov	r2, fp
	strb	r2, [r3, #5]
	b.n	.L_080a3864
.L_080a36b4:
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #24]
	add	r0, r8
	bl	sub_080022fc
	lsls	r7, r0, #1
	mov	r8, r0
	adds	r0, r7, r0
	lsls	r0, r0, #3
	subs	r0, #10
	movs	r1, #16
	bl	sub_080a1a40
	mov	r3, fp
	mov	r9, r7
	cmp	r3, #0
	beq.n	.L_080a375c
	movs	r1, #0
	ldr	r3, [sp, #28]
	str	r1, [sp, #12]
	adds	r5, r7, r3
	mov	r2, sl
	ldrh	r0, [r5, #0]
	mov	fp, r1
	ldr	r6, [r2, #36]
	bl	sub_08077008
	ldr	r1, [sp, #8]
	str	r0, [sp, #16]
	cmp	r1, #0
	beq.n	.L_080a3728
	ldrh	r0, [r5, #0]
	bl	sub_08077008
	movs	r1, #228
	lsls	r1, r1, #1
	add	r1, sl
	movs	r2, #0
	bl	sub_080a3ddc
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, sl
	strb	r0, [r3, #0]
	ldrh	r0, [r5, #0]
	bl	sub_080a38a8
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_080a112c
	b.n	.L_080a373c
	.4byte 0x03001f2c
	.2byte 0x0b87
	.2byte 0x0000
.L_080a3728:
	ldrh	r0, [r5, #0]
	movs	r1, #0
	bl	sub_080a3e88
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_080a112c
.L_080a373c:
	movs	r3, #165
	lsls	r3, r3, #1
	ldr	r1, [pc, #48]
	movs	r2, #3
	add	r3, sl
.L_080a3746:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_080a3746
	movs	r2, #162
	lsls	r2, r2, #1
	ldr	r3, [pc, #32]
	add	r2, r9
	mov	r1, sl
	strh	r3, [r1, r2]
.L_080a375c:
	movs	r0, #1
	bl	sub_080030f8
	ldr	r6, [pc, #24]
	ldr	r3, [r6, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a37f0
	ldr	r3, [pc, #16]
	movs	r2, #128
	b.n	.L_080a3784
	.4byte 0x0000001e
	.4byte 0x0000001a
	.4byte 0x03001c94
	.2byte 0x1ae8
	.2byte 0x0300
.L_080a3784:
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a37d0
	ldr	r2, [sp, #12]
	adds	r2, #4
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080a379c
	ldr	r3, [sp, #12]
	adds	r3, #7
.L_080a379c:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	ldr	r0, [sp, #16]
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	adds	r1, r3, #0
	adds	r0, #216
	str	r3, [sp, #12]
	bl	sub_080a1e38
	ldr	r3, [sp, #12]
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	str	r3, [sp, #12]
	ldr	r3, [sp, #28]
	mov	r2, r9
	ldrh	r0, [r2, r3]
	movs	r1, #0
	bl	sub_080a3e88
	movs	r0, #112
	bl	sub_080f9010
	b.n	.L_080a37f0
.L_080a37d0:
	ldr	r5, [sp, #28]
	add	r5, r9
	ldrh	r0, [r5, #0]
	bl	sub_080a3d6c
	cmp	r0, #0
	beq.n	.L_080a37ea
	movs	r0, #112
	bl	sub_080f9010
	ldrh	r5, [r5, #0]
	str	r5, [sp, #20]
	b.n	.L_080a3876
.L_080a37ea:
	movs	r0, #114
	bl	sub_080f9010
.L_080a37f0:
	ldr	r3, [r6, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a3808
	movs	r0, #113
	bl	sub_080f9010
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #20]
	b.n	.L_080a3876
.L_080a3808:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a381a
	movs	r3, #1
	str	r3, [sp, #8]
	mov	fp, r3
.L_080a381a:
	ldr	r3, [pc, #128]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080a3832
	ldr	r1, [sp, #8]
	cmp	r1, #1
	bne.n	.L_080a3832
	movs	r2, #0
	movs	r3, #1
	str	r2, [sp, #8]
	mov	fp, r3
.L_080a3832:
	ldr	r5, [pc, #108]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a384e
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #1
	negs	r1, r1
	movs	r2, #1
	add	r8, r1
	mov	fp, r2
.L_080a384e:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a3864
	movs	r0, #111
	bl	sub_080f9010
	movs	r3, #1
	add	r8, r3
	mov	fp, r3
.L_080a3864:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a3872
	b.n	.L_080a36b4
.L_080a3872:
	mov	r1, r8
	lsls	r7, r1, #1
.L_080a3876:
	mov	r3, sl
	mov	r2, r8
	strb	r2, [r3, #28]
	ldr	r1, [sp, #28]
	ldrh	r2, [r7, r1]
	str	r2, [r3, #8]
	ldr	r3, [pc, #32]
	add	r3, sl
	strb	r2, [r3, #0]
	ldr	r0, [sp, #20]
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001ae8
	.4byte 0x03001b04
	.4byte 0x0000021a
