.syntax unified
	.thumb
	.global sub_08124af8
	.thumb_func
sub_08124af8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r0
	bl	OwnerState_Get
	movs	r7, #159
	mov	r8, r0
	lsls	r7, r7, #1
	add	r7, r8
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_08124b92
	bl	Func_081234a4
	mov	r2, r8
	movs	r3, #52
	ldrsh	r6, [r2, r3]
	ldrb	r3, [r7, #0]
	movs	r1, #56
	ldrsh	r5, [r2, r1]
	adds	r3, #1
	adds	r0, r3, #0
	muls	r0, r6
	movs	r1, #10
	bl	Math_Div
	mov	r9, r5
	adds	r5, r5, r0
	cmp	r5, r6
	ble.n	.L_08124b3c
	adds	r5, r6, #0
.L_08124b3c:
	mov	r1, r9
	subs	r6, r5, r1
	cmp	r6, #0
	beq.n	.L_08124b7a
	mov	r1, sl
	movs	r0, #0
	bl	0x08120360
	mov	r1, r8
	movs	r2, #52
	ldrsh	r3, [r1, r2]
	cmp	r5, r3
	bne.n	.L_08124b60
	ldr	r1, [pc, #72]
	movs	r0, #4
	bl	0x08120360
	b.n	.L_08124b70
.L_08124b60:
	movs	r0, #1
	adds	r1, r6, #0
	bl	0x08120360
	ldr	r1, [pc, #56]
	movs	r0, #4
	bl	0x08120360
.L_08124b70:
	mov	r2, r8
	strh	r5, [r2, #56]
	mov	r0, sl
	bl	0x080ad0d0
.L_08124b7a:
	bl	Func_081201c4
	movs	r2, #159
	lsls	r2, r2, #1
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r0, #1
	adds	r3, #255
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08124b94
.L_08124b92:
	movs	r0, #0
.L_08124b94:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000c6c
	.2byte 0x0c69
	.2byte 0x0000
	push	{lr}
	bl	OwnerState_Get
	movs	r3, #64
	adds	r3, #255
	adds	r1, r0, r3
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08124bc8
	adds	r3, #255
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L_08124bca
.L_08124bc8:
	movs	r0, #0
.L_08124bca:
	pop	{pc}
	push	{lr}
	bl	OwnerState_Get
	movs	r3, #163
	lsls	r3, r3, #1
	adds	r1, r0, r3
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08124bf8
	adds	r3, #255
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_08124bf8
	movs	r1, #72
	adds	r1, #255
	adds	r2, r0, r1
	strb	r3, [r2, #0]
	movs	r0, #1
	b.n	.L_08124bfa
.L_08124bf8:
	movs	r0, #0
.L_08124bfa:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #0
	sub	sp, #4
	bl	Func_080ad000
	movs	r1, #148
	adds	r3, r0, #0
	lsls	r1, r1, #1
	adds	r7, r3, #0
	adds	r3, r3, r1
	ldr	r3, [r3, #0]
	movs	r2, #0
	adds	r7, #8
	mov	r8, r2
	cmp	r2, r3
	bge.n	.L_08124c54
	adds	r5, r7, #0
.L_08124c22:
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	ble.n	.L_08124c42
	ldrb	r0, [r5, #2]
	str	r2, [sp, #0]
	bl	OwnerState_Get
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r2, [sp, #0]
	cmp	r3, #0
	beq.n	.L_08124c42
	ldrb	r3, [r5, #3]
	subs	r3, #1
	strb	r3, [r5, #3]
.L_08124c42:
	movs	r1, #144
	movs	r3, #1
	lsls	r1, r1, #1
	add	r8, r3
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	adds	r5, #4
	cmp	r8, r3
	blt.n	.L_08124c22
.L_08124c54:
	movs	r1, #144
	movs	r3, #0
	lsls	r1, r1, #1
	mov	r8, r3
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	cmp	r8, r3
	bge.n	.L_08124ca0
	adds	r6, r7, #0
.L_08124c66:
	movs	r3, #3
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L_08124c8e
	ldrb	r5, [r6, #2]
	ldrb	r1, [r6, #0]
	ldrb	r2, [r6, #1]
	adds	r0, r5, #0
	bl	0x080ad158
	ldrb	r2, [r6, #1]
	ldrb	r1, [r6, #0]
	adds	r0, r5, #0
	bl	0x080ad168
	adds	r0, r5, #0
	bl	0x080ad008
	movs	r2, #1
	b.n	.L_08124c94
.L_08124c8e:
	movs	r3, #1
	adds	r6, #4
	add	r8, r3
.L_08124c94:
	movs	r1, #144
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	cmp	r8, r3
	blt.n	.L_08124c66
.L_08124ca0:
	adds	r0, r2, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
