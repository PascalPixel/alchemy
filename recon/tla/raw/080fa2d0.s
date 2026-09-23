.syntax unified
	.thumb
	.set sub_08038010, 0x08038010
	.set sub_080380d0, 0x080380d0
	.set sub_08038268, 0x08038268
	.set sub_080ad008, 0x080ad008
	.set sub_080f88d0, 0x080f88d0
	.set sub_080f8ce8, 0x080f8ce8
	.set sub_08100738, 0x08100738
	.set sub_08100d40, 0x08100d40
	.set sub_08104fe0, 0x08104fe0
	.set sub_081c0010, 0x081c0010
	.global Func_080fa2d0
	.thumb_func
Func_080fa2d0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	movs	r2, #180
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	adds	r2, #175
	adds	r7, r5, r3
	adds	r2, r2, r5
	ldrb	r1, [r7, #0]
	mov	r8, r2
	ldrb	r2, [r2, #0]
	bl	sub_08100738
	movs	r3, #1
	adds	r6, r0, #0
	negs	r3, r3
	cmp	r6, r3
	bne.n	.L_080fa33a
	movs	r0, #114
	bl	sub_081c0010
	ldr	r0, [r5, #48]
	bl	sub_08038268
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #94
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #68]
	adds	r2, r6, #0
	adds	r0, r0, r3
	adds	r1, r6, #0
	bl	sub_080f8ce8
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #30
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r0, r6, #0
	b.n	.L_080fa35e
.L_080fa33a:
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	bl	sub_08100d40
	ldrb	r0, [r7, #0]
	bl	sub_080ad008
	mov	r3, r8
	ldrb	r0, [r3, #0]
	bl	sub_080ad008
	movs	r0, #1
.L_080fa35e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x1120
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	sub	sp, #4
	adds	r0, r7, #0
	bl	sub_080f88d0
	movs	r1, #2
	movs	r2, #2
	bl	sub_08104fe0
	movs	r0, #151
	ldr	r1, [pc, #8]
	lsls	r0, r0, #1
	movs	r2, #3
	adds	r3, r7, r0
	b.n	.L_080fa394
	movs	r0, r0
	.2byte 0x001a
	.2byte 0x0000
.L_080fa394:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_080fa394
	movs	r5, #0
	str	r5, [r7, #44]
	str	r5, [r7, #40]
	movs	r6, #2
	movs	r1, #17
	movs	r2, #30
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
	bl	sub_08038010
	adds	r3, r7, #0
	adds	r3, #244
	str	r0, [r7, #48]
	str	r5, [r7, #36]
	adds	r2, r7, #0
	strb	r5, [r3, #0]
	adds	r3, #1
	strb	r5, [r3, #0]
	adds	r2, #246
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r3, r7, #0
	adds	r3, #247
	strb	r6, [r3, #0]
	add	sp, #4
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	r8, r0
	movs	r3, #168
	mov	r6, r8
	sub	sp, #4
	adds	r7, r1, #0
	movs	r5, #0
	mov	sl, r3
	adds	r6, #76
.L_080fa3ec:
	mov	r3, sl
	str	r3, [sp, #0]
	adds	r1, r5, #0
	movs	r0, #2
	adds	r2, r7, #0
	movs	r3, #248
	bl	sub_080380d0
	adds	r5, #1
	stmia	r6!, {r0}
	cmp	r5, #7
	ble.n	.L_080fa3ec
	movs	r3, #168
	mov	r6, r8
	movs	r5, #8
	mov	sl, r3
	adds	r6, #108
.L_080fa40e:
	mov	r3, sl
	str	r3, [sp, #0]
	movs	r3, #128
	adds	r1, r5, #0
	movs	r0, #2
	adds	r2, r7, #0
	lsls	r3, r3, #1
	bl	sub_080380d0
	adds	r5, #1
	stmia	r6!, {r0}
	cmp	r5, #15
	ble.n	.L_080fa40e
	movs	r3, #168
	mov	r6, r8
	movs	r5, #16
	mov	sl, r3
	adds	r6, #140
.L_080fa432:
	mov	r3, sl
	str	r3, [sp, #0]
	movs	r3, #128
	adds	r1, r5, #0
	movs	r0, #2
	adds	r2, r7, #0
	lsls	r3, r3, #1
	bl	sub_080380d0
	adds	r5, #1
	stmia	r6!, {r0}
	cmp	r5, #31
	ble.n	.L_080fa432
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
