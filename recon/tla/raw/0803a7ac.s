.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0803939c, 0x0803939c
	.set sub_080397e0, 0x080397e0
	.set sub_08039ed0, 0x08039ed0
	.set sub_0803a3b8, 0x0803a3b8
	.set sub_0803a3e4, 0x0803a3e4
	.set sub_0803a69c, 0x0803a69c
	.set sub_0803b880, 0x0803b880
	.set sub_0803c548, 0x0803c548
	.set sub_080c82a8, 0x080c82a8
	.set sub_080c8778, 0x080c8778
	.global Func_0803a7ac
	.thumb_func
Func_0803a7ac:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	sub	sp, #28
	mov	r8, r3
	movs	r3, #0
	movs	r4, #0
	adds	r6, r1, #0
	str	r3, [sp, #4]
	str	r4, [sp, #8]
	movs	r3, #2
	movs	r5, #0
	movs	r2, #1
	adds	r7, r6, #0
	ands	r3, r6
	mov	sl, r0
	str	r5, [sp, #24]
	str	r5, [sp, #20]
	ands	r7, r2
	cmp	r3, #0
	beq.n	.L_0803a7e8
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #137
	add	r3, r8
	strb	r2, [r3, #0]
.L_0803a7e8:
	add	r0, sp, #12
	add	r2, sp, #20
	add	r3, sp, #16
	str	r0, [sp, #0]
	add	r1, sp, #24
	mov	r0, sl
	bl	sub_0803b880
	ldr	r2, [sp, #16]
	movs	r3, #30
	subs	r3, r3, r2
	ldr	r2, [sp, #12]
	asrs	r3, r3, #1
	str	r3, [sp, #24]
	movs	r3, #12
	subs	r3, r3, r2
	asrs	r2, r3, #1
	movs	r3, #8
	ands	r3, r6
	str	r2, [sp, #20]
	cmp	r3, #0
	beq.n	.L_0803a818
	adds	r3, r2, #4
	b.n	.L_0803a840
.L_0803a818:
	movs	r3, #64
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0803a826
	adds	r3, r2, #0
	adds	r3, #12
	b.n	.L_0803a840
.L_0803a826:
	bl	sub_080c8778
	add	r5, sp, #4
	adds	r1, r5, #0
	bl	sub_080c82a8
	ldr	r3, [r5, #4]
	asrs	r3, r3, #3
	cmp	r3, #9
	ble.n	.L_0803a83e
	subs	r3, #5
	b.n	.L_0803a840
.L_0803a83e:
	adds	r3, #4
.L_0803a840:
	str	r3, [sp, #20]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #20]
	mov	r0, sl
	adds	r3, r7, #0
	bl	sub_0803a69c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0803a898
	b.n	.L_0803a85c
.L_0803a856:
	movs	r0, #1
	bl	sub_08013560
.L_0803a85c:
	bl	sub_0803a3b8
	cmp	r0, #0
	beq.n	.L_0803a856
	movs	r3, #32
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0803a876
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #60]
	movs	r3, #1
	strb	r3, [r2, #6]
.L_0803a876:
	movs	r3, #4
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_0803a898
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	sub_0803939c
	b.n	.L_0803a88e
.L_0803a888:
	movs	r0, #1
	bl	sub_08013560
.L_0803a88e:
	adds	r0, r5, #0
	bl	sub_0803a3e4
	cmp	r0, #0
	beq.n	.L_0803a888
.L_0803a898:
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #137
	movs	r2, #0
	add	r3, r8
	strb	r2, [r3, #0]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #132
	add	r3, r8
	strh	r2, [r3, #0]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #134
	add	r3, r8
	strh	r2, [r3, #0]
	movs	r0, #3
	bl	sub_08013560
	add	sp, #28
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{lr}
	bl	sub_08039ed0
	bl	sub_080397e0
	bl	sub_0803c548
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0803a8f4
	bl	sub_08039ed0
	bl	sub_080397e0
	bl	sub_0803c548
	b.n	.L_0803a8f8
.L_0803a8f4:
	bl	sub_080397e0
.L_0803a8f8:
	pop	{pc}
	.2byte 0x0000
