.syntax unified
	.thumb
	.set sub_08002df0, 0x08002df0
	.set sub_08004938, 0x08004938
	.set sub_080072f0, 0x080072f0
	.set sub_08077008, 0x08077008
	.set sub_08077018, 0x08077018
	.set sub_08077028, 0x08077028
	.set sub_080a112c, 0x080a112c
	.set sub_080a40ac, 0x080a40ac
	.global Func_080a3ef0
	.thumb_func
Func_080a3ef0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r3, #0
	ldr	r3, [pc, #404]
	mov	r9, r1
	movs	r1, #0
	sub	sp, #4
	adds	r5, r2, #0
	ldr	r7, [r3, #0]
	mov	r8, r0
	mov	sl, r1
	bl	sub_08077008
	mov	r2, r9
	lsls	r3, r2, #1
	adds	r3, #216
	ldrh	r3, [r0, r3]
	mov	fp, r0
	str	r3, [sp, #0]
	cmp	r5, #1
	bne.n	.L_080a3f2a
	movs	r3, #128
	lsls	r3, r3, #1
	mov	sl, r3
.L_080a3f2a:
	ldr	r1, [sp, #0]
	ldr	r0, [pc, #364]
	ands	r0, r1
	bl	sub_08077018
	ldrb	r3, [r0, #2]
	cmp	r3, #9
	bls.n	.L_080a3f3c
	b.n	.L_080a4086
.L_080a3f3c:
	ldr	r2, [pc, #352]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080a4000
	.4byte 0x080a3f6c
	.4byte 0x080a3f6c
	.4byte 0x080a3f6c
	.4byte 0x080a3f6c
	.4byte 0x080a3f6c
	.4byte 0x080a3ff4
	.4byte 0x080a3f6c
	.4byte 0x080a3f6c
	.2byte 0x3f6c
	.2byte 0x080a
	cmp	r8, r6
	bne.n	.L_080a3f74
	movs	r3, #2
	b.n	.L_080a3ffa
.L_080a3f74:
	adds	r0, r6, #0
	bl	sub_08077008
	movs	r5, #166
	lsls	r5, r5, #1
	mov	fp, r0
	adds	r0, r5, #0
	bl	sub_08004938
	adds	r2, r5, #0
	ldr	r3, [pc, #280]
	mov	r1, fp
	mov	r8, r0
	bl	sub_080072f0
	adds	r0, r6, #0
	bl	sub_080a40ac
	adds	r2, r0, #0
	cmp	r2, #0
	beq.n	.L_080a3fd2
	ldr	r3, [pc, #264]
	ldr	r1, [sp, #0]
	adds	r0, r6, #0
	ands	r1, r3
	str	r1, [sp, #0]
	bl	sub_08077028
	movs	r3, #1
	adds	r2, r0, #0
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_080a3fc4
	movs	r3, #2
	mov	r1, sl
	orrs	r1, r3
	mov	sl, r1
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	b.n	.L_080a3fca
.L_080a3fc4:
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	mov	r2, r9
.L_080a3fca:
	mov	r3, sl
	bl	sub_080a112c
	b.n	.L_080a3fde
.L_080a3fd2:
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	sub_080a112c
.L_080a3fde:
	movs	r2, #166
	ldr	r3, [pc, #192]
	mov	r0, fp
	mov	r1, r8
	lsls	r2, r2, #1
	bl	sub_080072f0
	mov	r0, r8
	bl	sub_08002df0
	b.n	.L_080a4086
	cmp	r6, r8
	bne.n	.L_080a400e
	movs	r3, #4
.L_080a3ffa:
	mov	r2, sl
	orrs	r2, r3
	mov	sl, r2
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	sub_080a112c
	b.n	.L_080a4086
.L_080a400e:
	adds	r0, r6, #0
	bl	sub_08077008
	movs	r5, #166
	lsls	r5, r5, #1
	mov	fp, r0
	adds	r0, r5, #0
	bl	sub_08004938
	adds	r2, r5, #0
	ldr	r3, [pc, #128]
	mov	r1, fp
	mov	r8, r0
	bl	sub_080072f0
	adds	r0, r6, #0
	bl	sub_080a40ac
	adds	r2, r0, #0
	cmp	r2, #0
	beq.n	.L_080a4066
	adds	r0, r6, #0
	ldr	r1, [sp, #0]
	bl	sub_08077028
	movs	r3, #1
	adds	r2, r0, #0
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_080a4058
	movs	r3, #4
	mov	r1, sl
	orrs	r1, r3
	mov	sl, r1
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	b.n	.L_080a405e
.L_080a4058:
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	mov	r2, r9
.L_080a405e:
	mov	r3, sl
	bl	sub_080a112c
	b.n	.L_080a4072
.L_080a4066:
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	sub_080a112c
.L_080a4072:
	movs	r2, #166
	ldr	r3, [pc, #44]
	mov	r0, fp
	mov	r1, r8
	lsls	r2, r2, #1
	bl	sub_080072f0
	mov	r0, r8
	bl	sub_08002df0
.L_080a4086:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x000001ff
	.4byte 0x080a3f44
	.4byte 0x03001388
	.4byte 0xfffffdff
