.syntax unified
	.thumb
	.set sub_0803b094, 0x0803b094
	.set sub_080416cc, 0x080416cc
	.global UiText_DrawCharacterAtOffset
	.global Func_08042010
	.thumb_func
UiText_DrawCharacterAtOffset:
Func_08042010:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #60]
	mov	sl, r1
	movs	r1, #152
	mov	r9, r2
	lsls	r1, r1, #5
	movs	r2, #0
	mov	r8, r2
	adds	r1, #66
	adds	r5, r7, r1
	mov	r3, r8
	movs	r1, #1
	strh	r3, [r5, #0]
	bl	sub_0803b094
	ldrh	r3, [r5, #0]
	movs	r4, #244
	lsls	r4, r4, #4
	lsls	r3, r3, #1
	adds	r3, r3, r4
	mov	r0, r8
	strh	r0, [r7, r3]
	ldr	r2, [pc, #56]
	ldrh	r3, [r5, #0]
	lsrs	r6, r6, #3
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r5, #0]
	mov	r2, sl
	movs	r1, #14
	ldrsh	r3, [r2, r1]
	movs	r4, #12
	ldrsh	r2, [r2, r4]
	adds	r3, r3, r6
	mov	r0, r9
	lsrs	r1, r0, #3
	adds	r3, #1
	adds	r2, r2, r1
	lsls	r3, r3, #5
	adds	r3, r3, r2
	movs	r2, #160
	adds	r1, r3, #1
	lsls	r2, r2, #2
	cmp	r1, r2
	bcs.n	.L_0804209e
	ldr	r3, [pc, #12]
	movs	r4, #244
	lsls	r1, r1, #1
	lsls	r4, r4, #4
	b.n	.L_0804208c
	movs	r0, r0
	.4byte 0x000001ff
	.2byte 0x2000
	.2byte 0x0600
.L_0804208c:
	adds	r2, r1, r3
	adds	r0, r7, r4
	adds	r1, r7, r1
	movs	r3, #7
	mov	r4, r9
	adds	r1, #8
	ands	r3, r4
	bl	sub_080416cc
.L_0804209e:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
