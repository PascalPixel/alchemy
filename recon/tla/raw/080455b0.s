.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_080143f8, 0x080143f8
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d78, 0x08014d78
	.set sub_0801591c, 0x0801591c
	.set sub_08039260, 0x08039260
	.set sub_08042188, 0x08042188
	.global Func_080455b0
	.thumb_func
Func_080455b0:
	push	{r5, lr}
	sub	sp, #4
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #6
	movs	r3, #4
	movs	r0, #0
	bl	sub_08039260
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #12]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08042188
	adds	r0, r5, #0
	add	sp, #4
	pop	{r5, pc}
	.2byte 0xf7b0
	.2byte 0x0805
.L_080455dc:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r5, r1, #0
	movs	r1, #193
	mov	r8, r0
	lsls	r1, r1, #3
	movs	r0, #68
	bl	sub_08014cc0
	adds	r6, r0, #0
	ldr	r0, [pc, #52]
	bl	sub_08013300
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #4
	lsls	r5, r5, #1
	adds	r2, r6, r3
	ldrh	r3, [r5, r0]
	adds	r1, r6, #0
	adds	r0, r0, r3
	str	r0, [r2, #0]
	bl	sub_0801591c
	adds	r1, r6, #0
	mov	r0, r8
	bl	sub_080143f8
	adds	r5, r0, #0
	movs	r0, #68
	bl	sub_0801314c
	adds	r0, r5, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x01d7
	.2byte 0x0000
.L_0804562c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r1, #0
	movs	r1, #193
	lsls	r1, r1, #3
	adds	r7, r0, #0
	movs	r0, #68
	mov	r8, r2
	bl	sub_08014cc0
	adds	r6, r0, #0
	ldr	r0, [pc, #160]
	bl	sub_08013300
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #4
	lsls	r5, r5, #1
	adds	r2, r6, r3
	ldrh	r3, [r5, r0]
	adds	r1, r6, #0
	adds	r0, r0, r3
	str	r0, [r2, #0]
	bl	sub_0801591c
	movs	r0, #128
	lsls	r0, r0, #3
	bl	sub_08014d78
	movs	r2, #0
	mov	lr, r0
	mov	r5, lr
	mov	ip, r2
.L_08045672:
	ldrb	r4, [r6, #0]
	adds	r6, #1
	ldrb	r2, [r7, r4]
	adds	r3, r2, #0
	cmp	r3, #255
	bne.n	.L_080456ae
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r0, r7, r3
	ldr	r3, [r0, #0]
	strb	r3, [r7, r4]
	ldr	r1, [r0, #0]
	cmp	r1, #63
	bgt.n	.L_080456ac
	movs	r3, #160
	lsls	r2, r1, #1
	lsls	r3, r3, #19
	adds	r3, r3, r2
	ldr	r2, [pc, #84]
	mov	sl, r3
	lsls	r3, r4, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	mov	r2, sl
	strh	r3, [r2, #0]
	adds	r3, r1, #1
	str	r3, [r0, #0]
	ldrb	r2, [r7, r4]
	b.n	.L_080456ae
.L_080456ac:
	adds	r2, r3, #0
.L_080456ae:
	strb	r2, [r5, #0]
	movs	r3, #1
	movs	r2, #128
	add	ip, r3
	lsls	r2, r2, #3
	adds	r5, #1
	cmp	ip, r2
	blt.n	.L_08045672
	mov	r3, r8
	ldr	r2, [pc, #44]
	lsls	r1, r3, #6
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r1, r1, r2
	adds	r3, #212
	mov	r0, lr
	ldr	r2, [pc, #36]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, lr
	bl	sub_08013164
	movs	r0, #68
	bl	sub_0801314c
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x000001d7
	.4byte 0x05000200
	.4byte 0x06004000
	.2byte 0x0100
	.2byte 0x8400
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r3, r1, #0
	lsls	r5, r3, #3
	subs	r5, r5, r3
	adds	r4, r0, #0
	mov	r8, r2
	lsls	r5, r5, #2
	movs	r2, #130
	adds	r6, r4, r5
	lsls	r3, r3, #4
	lsls	r2, r2, #1
	sub	sp, #4
	adds	r6, r6, r2
	mov	r1, r8
	adds	r2, r3, #0
	str	r4, [sp, #0]
	bl	.L_0804562c
	movs	r2, #142
	ldr	r4, [sp, #0]
	lsls	r2, r2, #1
	adds	r3, r5, r2
	mov	r2, r8
	str	r2, [r4, r3]
	ldr	r3, [pc, #40]
	str	r3, [r6, #4]
	movs	r3, #0
	str	r3, [r6, #8]
	movs	r3, #136
	lsls	r3, r3, #1
	adds	r5, r5, r3
	ldrh	r0, [r4, r5]
	mov	r1, r8
	bl	.L_080455dc
	ldr	r3, [pc, #16]
	ldrh	r2, [r6, #8]
	ands	r0, r3
	ldr	r3, [pc, #16]
	add	sp, #4
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r6, #8]
	b.n	.L_08045760
	.4byte 0x000003ff
	.4byte 0x80002000
	.2byte 0xfc00
	.2byte 0xffff
.L_08045760:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.align 2, 0
