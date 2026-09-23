.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_0800352c, 0x0800352c
	.set sub_08015418, 0x08015418
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080a1fd4
	.global Func_080a1fd4
	.thumb_func
Unnamed_080a1fd4:
Func_080a1fd4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r1
	ldr	r1, [sp, #28]
	adds	r5, r0, #0
	adds	r7, r2, #0
	movs	r0, #1
	mov	r2, r8
	adds	r6, r3, #0
	mov	sl, r1
	negs	r0, r0
	cmp	r2, #0
	bne.n	.L_080a1ff6
	b.n	.L_080a212e
.L_080a1ff6:
	ldr	r0, [pc, #324]
	bl	sub_08015418
	adds	r1, r7, #0
	mov	r0, r8
	bl	sub_080022ec
	adds	r1, r7, #0
	mov	r9, r0
	mov	r0, r8
	bl	sub_080022fc
	cmp	r0, #0
	beq.n	.L_080a2016
	movs	r3, #1
	add	r9, r3
.L_080a2016:
	cmp	r5, #0
	beq.n	.L_080a2034
	ldr	r2, [pc, #292]
	ldr	r4, [r2, #0]
	movs	r3, #16
	ldr	r1, [r2, #0]
	ands	r4, r3
	ldr	r5, [r2, #0]
	movs	r3, #32
	ands	r1, r3
	ldr	r2, [r2, #0]
	movs	r3, #64
	ands	r5, r3
	movs	r3, #128
	b.n	.L_080a204c
.L_080a2034:
	ldr	r2, [pc, #264]
	ldr	r4, [r2, #0]
	movs	r3, #128
	ldr	r1, [r2, #0]
	ands	r4, r3
	ldr	r5, [r2, #0]
	movs	r3, #64
	ands	r1, r3
	ldr	r2, [r2, #0]
	movs	r3, #32
	ands	r5, r3
	movs	r3, #16
.L_080a204c:
	ands	r2, r3
	cmp	r5, #0
	beq.n	.L_080a2084
	movs	r0, #111
	bl	sub_080f9010
	mov	r1, sl
	ldr	r3, [r1, #0]
	subs	r3, #1
	str	r3, [r1, #0]
	cmp	r3, #0
	bge.n	.L_080a206a
	mov	r3, r9
	subs	r3, #1
	str	r3, [r1, #0]
.L_080a206a:
	mov	r2, sl
	ldr	r3, [r2, #0]
	adds	r0, r7, #0
	muls	r0, r3
	ldr	r3, [r6, #0]
	mov	r2, r8
	adds	r3, r3, r0
	subs	r2, #1
	cmp	r3, r2
	ble.n	.L_080a20c6
	mov	r1, r8
	subs	r3, r1, r0
	b.n	.L_080a20ba
.L_080a2084:
	cmp	r2, #0
	beq.n	.L_080a20ce
	movs	r0, #111
	bl	sub_080f9010
	mov	r2, sl
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	mov	r2, r9
	subs	r2, #1
	cmp	r3, r2
	ble.n	.L_080a20a2
	mov	r3, sl
	str	r5, [r3, #0]
.L_080a20a2:
	mov	r1, sl
	ldr	r3, [r1, #0]
	adds	r0, r7, #0
	muls	r0, r3
	ldr	r3, [r6, #0]
	mov	r2, r8
	adds	r3, r3, r0
	subs	r2, #1
	cmp	r3, r2
	ble.n	.L_080a20c6
	mov	r2, r8
	subs	r3, r2, r0
.L_080a20ba:
	subs	r3, #1
	subs	r1, r7, #1
	str	r3, [r6, #0]
	cmp	r3, r1
	ble.n	.L_080a20c6
	str	r1, [r6, #0]
.L_080a20c6:
	bl	sub_0800352c
	movs	r0, #1
	b.n	.L_080a212e
.L_080a20ce:
	cmp	r1, #0
	beq.n	.L_080a20fc
	movs	r0, #111
	bl	sub_080f9010
	ldr	r3, [r6, #0]
	subs	r3, #1
	str	r3, [r6, #0]
	cmp	r3, #0
	bge.n	.L_080a212c
	subs	r2, r7, #1
	str	r2, [r6, #0]
	mov	r1, sl
	ldr	r3, [r1, #0]
	muls	r3, r7
	mov	r1, r8
	subs	r3, r1, r3
	subs	r3, #1
	str	r3, [r6, #0]
	cmp	r3, r2
	ble.n	.L_080a212c
	str	r2, [r6, #0]
	b.n	.L_080a212c
.L_080a20fc:
	movs	r0, #1
	negs	r0, r0
	cmp	r4, #0
	beq.n	.L_080a212e
	movs	r0, #111
	bl	sub_080f9010
	ldr	r2, [r6, #0]
	adds	r2, #1
	str	r2, [r6, #0]
	mov	r1, sl
	ldr	r3, [r1, #0]
	muls	r3, r7
	mov	r1, r8
	subs	r3, r1, r3
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_080a2122
	str	r0, [r6, #0]
.L_080a2122:
	ldr	r3, [r6, #0]
	subs	r2, r7, #1
	cmp	r3, r2
	ble.n	.L_080a212c
	str	r0, [r6, #0]
.L_080a212c:
	movs	r0, #0
.L_080a212e:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x06002500
	.4byte 0x03001b04
