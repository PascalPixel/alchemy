.syntax unified
	.thumb
	.set sub_08016ce4, 0x08016ce4
	.set sub_0803a7ac, 0x0803a7ac
	.set sub_080400e8, 0x080400e8
	.set sub_08040798, 0x08040798
	.set sub_08043690, 0x08043690
	.global Func_0804e0d0
	.thumb_func
Func_0804e0d0:
	push	{r5, r6, lr}
	movs	r5, #0
.L_0804e0d4:
	adds	r0, r5, #0
	bl	sub_08040798
	movs	r6, #1
	adds	r5, r0, #0
	negs	r6, r6
	adds	r0, r6, #0
	cmp	r5, r6
	beq.n	.L_0804e13a
	cmp	r5, #0
	bne.n	.L_0804e11a
	movs	r0, #190
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0804e0fa
	ldr	r0, [pc, #68]
	b.n	.L_0804e108
.L_0804e0fa:
	movs	r0, #126
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0804e110
	ldr	r0, [pc, #56]
.L_0804e108:
	movs	r1, #1
	bl	sub_0803a7ac
	b.n	.L_0804e0d4
.L_0804e110:
	bl	sub_08043690
	cmp	r0, r6
	bne.n	.L_0804e138
	b.n	.L_0804e0d4
.L_0804e11a:
	cmp	r5, #1
	bne.n	.L_0804e12c
	ldr	r0, [pc, #36]
	movs	r1, #1
	bl	sub_0803a7ac
	ldr	r3, [pc, #32]
	strb	r5, [r3, #0]
	b.n	.L_0804e138
.L_0804e12c:
	cmp	r5, #2
	bne.n	.L_0804e138
	bl	sub_080400e8
	cmp	r0, r6
	beq.n	.L_0804e0d4
.L_0804e138:
	movs	r0, #0
.L_0804e13a:
	pop	{r5, r6, pc}
	.4byte 0x00000022
	.4byte 0x00000023
	.4byte 0x00001160
	.2byte 0x11d0
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	adds	r6, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	ldr	r7, [sp, #20]
	mov	ip, r3
	movs	r5, #12
	ldrsh	r3, [r0, r5]
	ldr	r4, [sp, #16]
	adds	r3, r1, r3
	adds	r1, r3, #1
	movs	r5, #14
	ldrsh	r3, [r0, r5]
	adds	r3, r2, r3
	adds	r2, r3, #1
	movs	r3, #1
	ands	r7, r3
	lsls	r7, r7, #12
	cmp	r1, #0
	bge.n	.L_0804e17a
	adds	r6, r6, r1
	movs	r1, #0
.L_0804e17a:
	adds	r3, r1, r6
	cmp	r3, #29
	ble.n	.L_0804e184
	movs	r3, #30
	subs	r6, r3, r1
.L_0804e184:
	cmp	r2, #0
	bge.n	.L_0804e18c
	adds	r4, r4, r2
	movs	r2, #0
.L_0804e18c:
	adds	r3, r2, r4
	cmp	r3, #29
	ble.n	.L_0804e196
	movs	r3, #20
	subs	r4, r3, r2
.L_0804e196:
	cmp	r6, #0
	ble.n	.L_0804e1d0
	cmp	r4, #0
	ble.n	.L_0804e1d0
	lsls	r3, r1, #1
	lsls	r2, r2, #6
	add	r3, ip
	adds	r0, r2, r3
.L_0804e1a6:
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r1, #8
	cmp	r2, #0
	beq.n	.L_0804e1c2
	ldr	r5, [pc, #32]
.L_0804e1b2:
	ldrh	r3, [r1, #0]
	subs	r2, #1
	ands	r3, r5
	orrs	r3, r7
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r2, #0
	bne.n	.L_0804e1b2
.L_0804e1c2:
	subs	r4, #1
	adds	r0, #64
	cmp	r4, #0
	bne.n	.L_0804e1a6
	movs	r3, #1
	mov	r2, ip
	strb	r3, [r2, #3]
.L_0804e1d0:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffffefff
