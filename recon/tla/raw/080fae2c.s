.syntax unified
	.thumb
	.global Func_080fae2c
	.thumb_func
Func_080fae2c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	mov	r8, r1
	adds	r6, r0, #0
	bl	Owner_GetState
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r5, r7, r2
	adds	r1, r5, #0
	movs	r2, #0
	bl	Func_080fad88
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r7, r2
	strb	r0, [r3, #0]
	ldr	r0, [r7, #36]
	bl	0x08038260
	mov	r0, r8
	bl	0x080f8c94
	adds	r0, r5, #0
	movs	r1, #0
	bl	0x080fadd0
	adds	r0, r6, #0
	bl	Func_080fad1c
	cmp	r0, #0
	bne.n	.L_080fae82
	ldr	r0, [pc, #16]
	ldr	r1, [r7, #36]
	movs	r2, #8
	movs	r3, #24
	bl	0x08038080
.L_080fae82:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x1006
	.2byte 0x0000
	.global Func_080fae8c
	.thumb_func
Func_080fae8c:
.L_080fae8c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	mov	r9, r1
	movs	r1, #0
	sub	sp, #4
	mov	fp, r2
	ldr	r7, [r3, #0]
	mov	sl, r1
	adds	r5, r0, #0
	bl	Owner_GetState
	mov	r2, r9
	str	r0, [sp, #0]
	lsls	r3, r2, #1
	adds	r3, #216
	ldrh	r3, [r0, r3]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	mov	r8, r3
	bl	Item_Get
	mov	r3, fp
	cmp	r3, #1
	bne.n	.L_080faed8
	movs	r1, #128
	lsls	r1, r1, #1
	mov	sl, r1
.L_080faed8:
	ldrb	r0, [r0, #2]
	cmp	r0, #11
	bls.n	.L_080faee0
	b.n	.L_080fb08a
.L_080faee0:
	ldr	r2, [pc, #436]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080fb004
	.4byte 0x080faf70
	.4byte 0x080faf70
	.4byte 0x080faf70
	.4byte 0x080faf70
	.4byte 0x080faf70
	.4byte 0x080faff8
	.4byte 0x080faf70
	.4byte 0x080faf70
	.4byte 0x080faf70
	.4byte 0x080faf18
	.2byte 0xb004
	.2byte 0x080f
	cmp	r5, r6
	bne.n	.L_080faf20
	movs	r3, #9
	b.n	.L_080faffe
.L_080faf20:
	adds	r0, r6, #0
	bl	Owner_GetState
	str	r0, [sp, #0]
	movs	r0, #166
	lsls	r0, r0, #1
	bl	Func_08014d78
	movs	r2, #166
	lsls	r2, r2, #1
	ldr	r3, [pc, #356]
	ldr	r1, [sp, #0]
	mov	fp, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c30
	bl	Inventory_RemoveFirstUnflagged
	adds	r2, r0, #0
	cmp	r2, #0
	beq.n	.L_080fafd6
	ldr	r3, [pc, #340]
	mov	r1, r8
	ands	r1, r3
	adds	r0, r6, #0
	mov	r8, r1
	bl	0x080ad020
	movs	r3, #1
	adds	r2, r0, #0
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_080fafc8
	movs	r3, #9
	mov	r1, sl
	orrs	r1, r3
	mov	sl, r1
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	b.n	.L_080fafce
	cmp	r5, r6
	bne.n	.L_080faf78
	movs	r3, #2
	b.n	.L_080faffe
.L_080faf78:
	adds	r0, r6, #0
	bl	Owner_GetState
	str	r0, [sp, #0]
	movs	r0, #166
	lsls	r0, r0, #1
	bl	Func_08014d78
	movs	r2, #166
	lsls	r2, r2, #1
	ldr	r3, [pc, #268]
	ldr	r1, [sp, #0]
	mov	fp, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c30
	bl	Inventory_RemoveFirstUnflagged
	adds	r2, r0, #0
	cmp	r2, #0
	beq.n	.L_080fafd6
	ldr	r3, [pc, #252]
	mov	r1, r8
	ands	r1, r3
	adds	r0, r6, #0
	mov	r8, r1
	bl	0x080ad020
	movs	r3, #1
	adds	r2, r0, #0
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_080fafc8
	movs	r3, #2
	mov	r1, sl
	orrs	r1, r3
	mov	sl, r1
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	b.n	.L_080fafce
.L_080fafc8:
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	mov	r2, r9
.L_080fafce:
	mov	r3, sl
	bl	0x080f8170
	b.n	.L_080fafe2
.L_080fafd6:
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	0x080f8170
.L_080fafe2:
	movs	r2, #166
	ldr	r3, [pc, #180]
	ldr	r0, [sp, #0]
	mov	r1, fp
	lsls	r2, r2, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4658
	bl	Func_08013164
	b.n	.L_080fb08a
	cmp	r6, r5
	bne.n	.L_080fb012
	movs	r3, #4
.L_080faffe:
	mov	r2, sl
	orrs	r2, r3
	mov	sl, r2
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	0x080f8170
	b.n	.L_080fb08a
.L_080fb012:
	adds	r0, r6, #0
	bl	Owner_GetState
	str	r0, [sp, #0]
	movs	r0, #166
	lsls	r0, r0, #1
	bl	Func_08014d78
	movs	r2, #166
	lsls	r2, r2, #1
	ldr	r3, [pc, #116]
	ldr	r1, [sp, #0]
	mov	fp, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c30
	bl	Inventory_RemoveFirstUnflagged
	adds	r2, r0, #0
	cmp	r2, #0
	beq.n	.L_080fb06a
	adds	r0, r6, #0
	mov	r1, r8
	bl	0x080ad020
	movs	r3, #1
	adds	r2, r0, #0
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_080fb05c
	movs	r3, #4
	mov	r1, sl
	orrs	r1, r3
	mov	sl, r1
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	b.n	.L_080fb062
.L_080fb05c:
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	mov	r2, r9
.L_080fb062:
	mov	r3, sl
	bl	0x080f8170
	b.n	.L_080fb076
.L_080fb06a:
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	0x080f8170
.L_080fb076:
	movs	r2, #166
	ldr	r3, [pc, #32]
	ldr	r0, [sp, #0]
	mov	r1, fp
	lsls	r2, r2, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4658
	bl	Func_08013164
.L_080fb08a:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x080faee8
	.4byte 0x03000730
	.2byte 0xfdff
	.2byte 0xffff
