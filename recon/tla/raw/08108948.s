.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.global Func_08108948
	.thumb_func
Func_08108948:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_081089da
	movs	r1, #13
	ldrsb	r1, [r7, r1]
	mov	sl, r1
	cmp	r1, #0
	beq.n	.L_081089da
	ldr	r2, [r7, #0]
	ldrb	r6, [r7, #12]
	mov	r8, r2
	movs	r1, #8
	ldrsh	r3, [r7, r1]
	movs	r1, #4
	ldrsh	r2, [r7, r1]
	adds	r6, #1
	strb	r6, [r7, #12]
	lsls	r6, r6, #24
	subs	r3, r3, r2
	asrs	r6, r6, #24
	adds	r0, r6, #0
	muls	r0, r3
	mov	r1, sl
	bl	sub_08002054
	ldrh	r5, [r7, #4]
	mov	r3, r8
	adds	r5, r5, r0
	strh	r5, [r3, #6]
	ldr	r2, [pc, #56]
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r1, r8
	adds	r3, #255
	ands	r5, r3
	mov	r9, r2
	ldr	r3, [pc, #48]
	ldrh	r2, [r1, #22]
	ands	r3, r2
	orrs	r3, r5
	mov	r2, r8
	strh	r3, [r2, #22]
	movs	r1, #6
	ldrsh	r2, [r7, r1]
	movs	r1, #10
	ldrsh	r3, [r7, r1]
	mov	r1, sl
	subs	r3, r3, r2
	adds	r0, r6, #0
	muls	r0, r3
	bl	sub_08002054
	ldrh	r5, [r7, #6]
	mov	r2, r8
	adds	r5, r5, r0
	strh	r5, [r2, #8]
	strb	r5, [r2, #20]
	b.n	.L_081089d0
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0xfe00
	.2byte 0xffff
.L_081089d0:
	cmp	r6, sl
	bne.n	.L_081089da
	mov	r3, r9
	strb	r3, [r7, #13]
	strb	r3, [r7, #12]
.L_081089da:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r4, [r5, #0]
	cmp	r4, #0
	beq.n	.L_08108a82
	movs	r1, #8
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r4, #6]
	subs	r0, r2, r3
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_081089fe
	adds	r3, r0, #3
.L_081089fe:
	asrs	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_08108a06
	negs	r3, r3
.L_08108a06:
	cmp	r0, #0
	ble.n	.L_08108a1c
	cmp	r3, #0
	beq.n	.L_08108a12
	subs	r3, r2, r3
	b.n	.L_08108a2a
.L_08108a12:
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r2, r1
	b.n	.L_08108a2a
.L_08108a1c:
	cmp	r0, #0
	bge.n	.L_08108a40
	cmp	r3, #0
	beq.n	.L_08108a28
	adds	r3, r2, r3
	b.n	.L_08108a2a
.L_08108a28:
	adds	r3, r2, #1
.L_08108a2a:
	strh	r3, [r4, #6]
	ldrh	r3, [r4, #6]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r2, r3
	ldrh	r1, [r4, #22]
	ldr	r3, [pc, #72]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r4, #22]
.L_08108a40:
	movs	r1, #10
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r4, #8]
	subs	r0, r2, r3
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_08108a50
	adds	r3, r0, #3
.L_08108a50:
	asrs	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_08108a58
	negs	r3, r3
.L_08108a58:
	cmp	r0, #0
	ble.n	.L_08108a6e
	cmp	r3, #0
	beq.n	.L_08108a64
	subs	r3, r2, r3
	b.n	.L_08108a7c
.L_08108a64:
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r2, r1
	b.n	.L_08108a7c
.L_08108a6e:
	cmp	r0, #0
	bge.n	.L_08108a82
	cmp	r3, #0
	beq.n	.L_08108a7a
	adds	r3, r2, r3
	b.n	.L_08108a7c
.L_08108a7a:
	adds	r3, r2, #1
.L_08108a7c:
	strh	r3, [r4, #8]
	ldrh	r3, [r4, #8]
	strb	r3, [r4, #20]
.L_08108a82:
	pop	{r5, pc}
	.2byte 0xfe00
	.2byte 0xffff
	push	{r5, r6, lr}
	ldr	r5, [r0, #0]
	ldr	r6, [pc, #16]
	ldrh	r4, [r5, #6]
	strh	r1, [r0, #8]
	strh	r4, [r0, #4]
	ldrh	r4, [r5, #8]
	strh	r2, [r0, #10]
	strh	r4, [r0, #6]
	strb	r3, [r0, #13]
	strb	r6, [r0, #12]
	b.n	.L_08108aa4
	.2byte 0x0000
	.2byte 0x0000
.L_08108aa4:
	pop	{r5, r6, pc}
	.2byte 0x0000
