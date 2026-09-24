.syntax unified
	.thumb
	.set sub_08002f40, 0x08002f40
	.set sub_08005340, 0x08005340
	.set sub_08185000, 0x08185000
	.global ResourceSlot_Load
	.global Func_0800b6b8
	.thumb_func
ResourceSlot_Load:
Func_0800b6b8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	sub	sp, #4
	adds	r7, r1, #0
	adds	r4, r2, #0
	mov	r8, r3
	cmp	r6, #7
	bls.n	.L_0800b6d2
.L_0800b6ce:
	movs	r0, #0
	b.n	.L_0800b77e
.L_0800b6d2:
	ldr	r3, [pc, #184]
	ldr	r5, [r3, #0]
	adds	r0, r4, #0
	lsls	r3, r6, #3
	adds	r5, r5, r3
	str	r4, [sp, #0]
	bl	sub_08185000
	ldr	r4, [sp, #0]
	ldr	r2, [pc, #168]
	lsls	r3, r6, #12
	adds	r5, #28
	orrs	r3, r4
	str	r3, [r5, #0]
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	mov	sl, r0
	str	r7, [r5, #4]
	ldrh	r0, [r2, #0]
	adds	r1, r2, #6
	movs	r5, #0
.L_0800b700:
	adds	r2, #4
	cmp	r3, #0
	beq.n	.L_0800b6ce
	cmp	r3, r4
	beq.n	.L_0800b71e
	adds	r5, #1
	cmp	r5, #255
	bhi.n	.L_0800b71e
	movs	r6, #0
	ldrsh	r3, [r1, r6]
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	ldrh	r0, [r2, #0]
	adds	r1, #4
	b.n	.L_0800b700
.L_0800b71e:
	bl	sub_08002f40
	adds	r1, r7, #0
	bl	sub_08005340
	ldr	r3, [r7, #0]
	adds	r4, r7, #0
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0800b746
	adds	r2, r3, #0
.L_0800b734:
	adds	r3, r2, r7
	adds	r5, #1
	stmia	r4!, {r3}
	cmp	r5, #255
	bhi.n	.L_0800b746
	ldr	r3, [r4, #0]
	adds	r2, r3, #0
	cmp	r3, #0
	bne.n	.L_0800b734
.L_0800b746:
	mov	r1, r8
	cmp	r1, #0
	beq.n	.L_0800b774
	mov	r2, r8
	subs	r2, #1
	adds	r5, r4, #4
	adds	r0, r7, r0
	cmp	r2, #4
	bls.n	.L_0800b75a
	movs	r2, #0
.L_0800b75a:
	ldr	r3, [pc, #56]
	lsls	r2, r2, #8
	adds	r2, r2, r3
	cmp	r5, r0
	bcs.n	.L_0800b774
.L_0800b764:
	ldrb	r4, [r5, #0]
	cmp	r4, #223
	bhi.n	.L_0800b76e
	ldrb	r4, [r2, r4]
	strb	r4, [r5, #0]
.L_0800b76e:
	adds	r5, #1
	cmp	r5, r0
	bcc.n	.L_0800b764
.L_0800b774:
	mov	r2, sl
	ldrb	r3, [r2, #0]
	ldrb	r2, [r2, #1]
	adds	r0, r2, #0
	muls	r0, r3
.L_0800b77e:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e68
	.4byte 0x08012fa0
	.4byte 0x080092b8
