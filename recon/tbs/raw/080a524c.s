.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_08004938, 0x08004938
	.set sub_080072f0, 0x080072f0
	.set sub_08015010, 0x08015010
	.set sub_08015018, 0x08015018
	.set sub_08015068, 0x08015068
	.set sub_08015080, 0x08015080
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_08077018, 0x08077018
	.set sub_08077050, 0x08077050
	.set sub_080770c0, 0x080770c0
	.set sub_080772c0, 0x080772c0
	.set sub_080a1a40, 0x080a1a40
	.set sub_080a1ac0, 0x080a1ac0
	.set sub_080a3ef0, 0x080a3ef0
	.set sub_080f9010, 0x080f9010
	.global Func_080a524c
	.thumb_func
Func_080a524c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #3
	movs	r2, #17
	adds	r5, r0, #0
	movs	r3, #10
	movs	r0, #13
	bl	sub_08015010
	ldr	r3, [pc, #264]
	ands	r5, r3
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_08077018
	ldr	r3, [pc, #256]
	adds	r5, r5, r3
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #24
	movs	r3, #0
	bl	sub_08015080
	ldr	r5, [pc, #244]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	adds	r5, #1
	bl	sub_08015080
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #24
	bl	sub_08015080
	ldr	r5, [pc, #220]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #24
	movs	r3, #40
	adds	r5, #1
	bl	sub_08015080
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #24
	movs	r3, #56
	bl	sub_08015080
	movs	r6, #1
	movs	r0, #104
	movs	r1, #86
	mov	r8, r6
	bl	sub_080a1ac0
	b.n	.L_080a5306
.L_080a52c8:
	lsls	r1, r6, #4
	adds	r1, #70
	movs	r0, #104
	bl	sub_080a1a40
	ldr	r5, [pc, #172]
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a52ea
	movs	r2, #1
	movs	r0, #111
	subs	r6, #1
	mov	r8, r2
	bl	sub_080f9010
.L_080a52ea:
	ldr	r3, [r5, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a5300
	movs	r3, #1
	movs	r0, #111
	adds	r6, #1
	mov	r8, r3
	bl	sub_080f9010
.L_080a5300:
	movs	r0, #1
	bl	sub_080030f8
.L_080a5306:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a534c
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_080a5326
	movs	r3, #0
	adds	r0, r6, #2
	movs	r1, #2
	mov	r8, r3
	bl	sub_080022fc
	adds	r6, r0, #0
.L_080a5326:
	ldr	r1, [pc, #92]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a533a
	movs	r0, #112
	bl	sub_080f9010
	b.n	.L_080a534c
.L_080a533a:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a52c8
	movs	r0, #113
	bl	sub_080f9010
	movs	r6, #1
.L_080a534c:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080a535a
	movs	r6, #1
.L_080a535a:
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08015018
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x000001ff
	.4byte 0x00000182
	.4byte 0x00000ad4
	.4byte 0x00000b2c
	.4byte 0x03001b04
	.2byte 0x1c94
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #0
	sub	sp, #12
	mov	r8, r3
	movs	r3, #1
	str	r3, [sp, #8]
	ldr	r3, [pc, #372]
	ldr	r3, [r3, #0]
	ldr	r6, [pc, #372]
	mov	r9, r3
	add	r6, r9
	ldrb	r0, [r6, #0]
	bl	sub_08077008
	movs	r3, #187
	str	r0, [sp, #4]
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	mov	sl, r3
	ldrb	r3, [r6, #0]
	movs	r5, #166
	adds	r0, r3, #0
	movs	r2, #0
	lsls	r5, r5, #1
	bl	sub_080a3ef0
	adds	r0, r5, #0
	bl	sub_08004938
	ldr	r3, [pc, #332]
	ldr	r1, [sp, #4]
	adds	r2, r5, #0
	mov	fp, r0
	bl	sub_080072f0
	movs	r3, #134
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r7, [r3, #0]
	mov	r3, sl
	ldrb	r0, [r6, #0]
	ldrh	r1, [r3, #0]
	bl	sub_08077050
	adds	r0, #2
	cmp	r0, #1
	bhi.n	.L_080a53fe
	b.n	.L_080a54c6
.L_080a53f6:
	movs	r0, #175
	bl	sub_080f9010
	b.n	.L_080a54ca
.L_080a53fe:
	ldr	r5, [pc, #292]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #24
	movs	r3, #24
	adds	r5, #1
	bl	sub_08015080
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #72
	movs	r3, #24
	bl	sub_08015080
	movs	r3, #24
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r1, #16
	movs	r2, #16
	movs	r3, #96
	bl	sub_08015068
	adds	r1, r7, #0
	ldr	r0, [pc, #248]
	movs	r2, #0
	movs	r3, #16
	bl	sub_08015080
	movs	r0, #110
	movs	r1, #32
	bl	sub_080a1ac0
	b.n	.L_080a5488
.L_080a5440:
	mov	r3, r8
	lsls	r0, r3, #1
	add	r0, r8
	lsls	r0, r0, #4
	adds	r0, #110
	movs	r1, #32
	bl	sub_080a1a40
	ldr	r5, [pc, #216]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a546c
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	movs	r0, #111
	movs	r3, #1
	str	r3, [sp, #8]
	bl	sub_080f9010
.L_080a546c:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a5482
	movs	r3, #1
	movs	r0, #111
	add	r8, r3
	str	r3, [sp, #8]
	bl	sub_080f9010
.L_080a5482:
	movs	r0, #1
	bl	sub_080030f8
.L_080a5488:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a54ca
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080a54aa
	mov	r0, r8
	movs	r3, #0
	adds	r0, #2
	movs	r1, #2
	str	r3, [sp, #8]
	bl	sub_080022fc
	mov	r8, r0
.L_080a54aa:
	ldr	r1, [pc, #132]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080a53f6
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a5440
	movs	r0, #113
	bl	sub_080f9010
.L_080a54c6:
	movs	r3, #1
	mov	r8, r3
.L_080a54ca:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080a54da
	movs	r3, #1
	mov	r8, r3
.L_080a54da:
	mov	r3, r8
	cmp	r3, #1
	bne.n	.L_080a54ee
	movs	r2, #166
	ldr	r3, [pc, #60]
	ldr	r0, [sp, #4]
	mov	r1, fp
	lsls	r2, r2, #1
	bl	sub_080072f0
.L_080a54ee:
	ldr	r5, [pc, #44]
	mov	r0, fp
	add	r5, r9
	bl	sub_08002df0
	ldrb	r0, [r5, #0]
	bl	sub_08077010
	ldrb	r0, [r5, #0]
	bl	sub_080772c0
	mov	r0, r8
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x0000021b
	.4byte 0x03001388
	.4byte 0x00000b2c
	.4byte 0x00000ad6
	.4byte 0x03001b04
	.4byte 0x03001c94
