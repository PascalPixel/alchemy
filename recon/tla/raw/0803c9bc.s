.syntax unified
	.thumb
	.balign 4
	.global UiText_CopyMessageString
	.thumb_func
UiText_CopyMessageString:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #60]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #66
	adds	r5, r2, #0
	adds	r2, r6, r3
	movs	r3, #0
	adds	r7, r1, #0
	strh	r3, [r2, #0]
	movs	r1, #1
	bl	UiText_BuildRenderEntries
	subs	r5, #1
	movs	r0, #0
	cmp	r0, r5
	bcs.n	.L_0803ca0e
	movs	r2, #244
	lsls	r2, r2, #4
	ldrh	r3, [r6, r2]
	strh	r3, [r7, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_0803ca0e
	mov	ip, r5
	adds	r2, r6, r2
	movs	r4, #0
.L_0803c9f6:
	adds	r0, #1
	adds	r4, #2
	cmp	r0, ip
	bcs.n	.L_0803ca12
	adds	r2, #2
	ldrh	r3, [r2, #0]
	adds	r1, r4, #0
	strh	r3, [r1, r7]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_0803c9f6
	b.n	.L_0803ca14
.L_0803ca0e:
	movs	r1, #0
	b.n	.L_0803ca14
.L_0803ca12:
	lsls	r1, r0, #1
.L_0803ca14:
	ldr	r3, [pc, #4]
	strh	r3, [r1, r7]
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
	.global UiText_DecodeMessage
	.thumb_func
UiText_DecodeMessage:
.L_0803ca20:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r2, #0
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r2, #200
	ldr	r3, [r2, #0]
	sub	sp, #12
	mov	r9, r0
	adds	r6, r1, #0
	mov	r8, r2
	mov	sl, r3
	cmp	r3, #0
	bne.n	.L_0803ca66
	ldr	r5, [pc, #192]
	movs	r0, #200
	adds	r1, r5, #0
	bl	Runtime_AllocateHeapBlock
	movs	r2, #132
	movs	r3, #128
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r1, r0, #0
	adds	r3, #212
	ldr	r0, [pc, #172]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r2, r8
	ldr	r3, [r2, #0]
.L_0803ca66:
	mov	r5, sp
	mov	r1, r9
	adds	r0, r5, #0
	mov	r8, r3
	bl	0x0803d178
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	mov	r9, r3
	b.n	.L_0803cad8
.L_0803ca7c:
	cmp	r0, #14
	beq.n	.L_0803ca94
	cmp	r0, #14
	bhi.n	.L_0803ca8e
	cmp	r0, #12
	bhi.n	.L_0803cace
	cmp	r0, #8
	bcc.n	.L_0803cace
	b.n	.L_0803cab4
.L_0803ca8e:
	cmp	r0, #15
	beq.n	.L_0803cab4
	b.n	.L_0803cace
.L_0803ca94:
	subs	r7, #3
	cmp	r7, #0
	ble.n	.L_0803cae2
	strh	r0, [r6, #0]
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x3602
	add	r0, r9
	strh	r0, [r6, #0]
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x3602
	add	r0, r9
	b.n	.L_0803cad4
.L_0803cab4:
	subs	r7, #1
	cmp	r7, #0
	ble.n	.L_0803cae2
	strh	r0, [r6, #0]
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x22ff
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r6, #2
	adds	r0, r0, r2
	b.n	.L_0803cad4
.L_0803cace:
	subs	r7, #1
	cmp	r7, #0
	ble.n	.L_0803cae2
.L_0803cad4:
	strh	r0, [r6, #0]
	adds	r6, #2
.L_0803cad8:
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x2800
	bne.n	.L_0803ca7c
.L_0803cae2:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_0803caee
	movs	r0, #200
	bl	Runtime_ReleaseHeapBlock
.L_0803caee:
	ldr	r3, [pc, #16]
	add	sp, #12
	strh	r3, [r6, #0]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00000144
	.2byte 0x8438
	.2byte 0x0803
	push	{lr}
	ldr	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_0803cb18
	movs	r3, #0
	str	r3, [r0, #0]
.L_0803cb18:
	pop	{pc}
	.2byte 0x0000