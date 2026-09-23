.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801489c, 0x0801489c
	.set sub_080148e8, 0x080148e8
	.set sub_080c8978, 0x080c8978
	.set sub_080eb01c, 0x080eb01c
	.set sub_080eb298, 0x080eb298
	.global Func_080e9ee4
	.thumb_func
Func_080e9ee4:
.L_080e9ee4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	ldr	r3, [r7, #0]
	ldr	r6, [r7, #12]
	ldr	r2, [r7, #16]
	subs	r6, r6, r3
	ldr	r3, [r7, #4]
	mov	sl, r1
	subs	r2, r2, r3
	mov	r9, r2
	adds	r1, r6, #0
	mov	r0, r9
	bl	sub_080148e8
	adds	r5, r0, #0
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r3, [pc, #76]
	mov	r1, sl
	mov	r8, r3
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x683b
	adds	r3, r3, r0
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002096
	mov	r1, sl
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x6879
	ldr	r4, [r7, #12]
	ldr	r3, [r7, #0]
	ldr	r2, [r7, #16]
	adds	r1, r1, r0
	subs	r3, r4, r3
	str	r1, [r7, #4]
	subs	r0, r2, r1
	eors	r3, r6
	movs	r1, #128
	lsls	r1, r1, #24
	cmp	r3, #0
	bge.n	.L_080e9f4c
	str	r4, [r7, #0]
.L_080e9f4c:
	mov	r3, r9
	eors	r3, r0
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080e9f5a
	ldr	r3, [r7, #16]
	str	r3, [r7, #4]
.L_080e9f5a:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #92]
	sub	sp, #12
	mov	r8, r3
	movs	r3, #232
	lsls	r3, r3, #5
	adds	r3, #85
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080ea01a
	movs	r3, #234
	lsls	r3, r3, #5
	add	r3, r8
	ldr	r3, [r3, #0]
	mov	r6, sp
	str	r3, [r6, #0]
	movs	r3, #232
	lsls	r3, r3, #5
	adds	r3, #68
	add	r3, r8
	ldr	r3, [r3, #0]
	movs	r5, #232
	str	r3, [r6, #8]
	movs	r3, #232
	lsls	r3, r3, #5
	lsls	r5, r5, #5
	adds	r3, #76
	adds	r5, #72
	add	r3, r8
	add	r5, r8
	ldr	r0, [r3, #0]
	adds	r2, r6, #0
	ldr	r1, [r5, #0]
	bl	sub_0801489c
	movs	r7, #132
	ldr	r3, [r6, #0]
	lsls	r7, r7, #5
	add	r7, r8
	str	r3, [r7, #12]
	movs	r2, #192
	ldr	r3, [r6, #8]
	lsls	r2, r2, #3
	str	r3, [r7, #16]
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	movs	r3, #0
	mov	sl, r3
.L_080e9fda:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	blt.n	.L_080ea00a
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L_080e9fee
	ldr	r3, [r6, #0]
	str	r3, [r7, #12]
	ldr	r3, [r6, #8]
	str	r3, [r7, #16]
.L_080e9fee:
	ldr	r3, [r7, #0]
	movs	r1, #0
	str	r3, [r6, #0]
	ldr	r3, [r7, #4]
	adds	r2, r6, #0
	str	r3, [r6, #8]
	movs	r3, #232
	lsls	r3, r3, #5
	adds	r3, #80
	add	r3, r8
	ldr	r0, [r3, #0]
	bl	sub_0801489c
	ldr	r3, [r7, #24]
.L_080ea00a:
	adds	r3, #1
	str	r3, [r7, #24]
	movs	r3, #1
	add	sl, r3
	mov	r2, sl
	adds	r7, #28
	cmp	r2, #47
	ble.n	.L_080e9fda
.L_080ea01a:
	movs	r7, #132
	movs	r5, #174
	lsls	r7, r7, #5
	lsls	r5, r5, #5
	movs	r3, #47
	add	r7, r8
	add	r5, r8
	mov	sl, r3
.L_080ea02a:
	movs	r3, #232
	lsls	r3, r3, #5
	adds	r3, #84
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080ea062
	ldr	r3, [r7, #24]
	cmp	r3, #0
	blt.n	.L_080ea05c
	movs	r1, #128
	adds	r0, r7, #0
	lsls	r1, r1, #11
	bl	.L_080e9ee4
	ldr	r3, [r7, #0]
	adds	r0, r5, #0
	str	r3, [r5, #12]
	ldr	r3, [r7, #4]
	str	r3, [r5, #16]
	bl	sub_080eb01c
	ldr	r3, [r7, #24]
.L_080ea05c:
	adds	r3, #1
	str	r3, [r7, #24]
	b.n	.L_080ea0bc
.L_080ea062:
	cmp	r3, #2
	bne.n	.L_080ea080
	movs	r1, #192
	adds	r0, r7, #0
	lsls	r1, r1, #11
	bl	.L_080e9ee4
	ldr	r3, [r7, #0]
	adds	r0, r5, #0
	str	r3, [r5, #12]
	ldr	r3, [r7, #4]
	str	r3, [r5, #16]
	bl	sub_080eb01c
	b.n	.L_080ea0bc
.L_080ea080:
	cmp	r3, #3
	bne.n	.L_080ea0bc
	movs	r3, #232
	lsls	r3, r3, #5
	adds	r3, #88
	add	r3, r8
	ldr	r2, [r7, #0]
	ldr	r3, [r3, #0]
	cmp	r2, r3
	bne.n	.L_080ea0a4
	movs	r3, #232
	lsls	r3, r3, #5
	adds	r3, #92
	add	r3, r8
	ldr	r2, [r7, #4]
	ldr	r3, [r3, #0]
	cmp	r2, r3
	beq.n	.L_080ea0bc
.L_080ea0a4:
	movs	r1, #192
	adds	r0, r7, #0
	lsls	r1, r1, #11
	bl	.L_080e9ee4
	ldr	r3, [r7, #0]
	adds	r0, r5, #0
	str	r3, [r5, #12]
	ldr	r3, [r7, #4]
	str	r3, [r5, #16]
	bl	sub_080eb01c
.L_080ea0bc:
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	mov	r3, sl
	adds	r5, #40
	adds	r7, #28
	cmp	r3, #0
	bge.n	.L_080ea02a
	movs	r6, #232
	movs	r7, #128
	lsls	r6, r6, #5
	movs	r5, #184
	lsls	r7, r7, #4
	adds	r6, #86
	lsls	r5, r5, #4
	movs	r2, #31
	add	r7, r8
	add	r6, r8
	add	r5, r8
	mov	sl, r2
.L_080ea0e4:
	ldr	r0, [r7, #24]
	cmp	r0, #19
	bhi.n	.L_080ea130
	movs	r1, #5
	bl	sub_08002054
	ldrh	r1, [r6, #0]
	movs	r3, #7
	ands	r3, r0
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #40]
	ands	r1, r3
	ldrh	r3, [r5, #8]
	adds	r0, r5, #0
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #8]
	adds	r1, r7, #0
	bl	sub_080eb298
	adds	r0, r7, #0
	movs	r1, #63
	ldr	r2, [pc, #20]
	bl	sub_080c8978
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
	b.n	.L_080ea130
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x8000
	.2byte 0xffff
.L_080ea130:
	.2byte 0x2301
	negs	r3, r3
	add	sl, r3
	mov	r2, sl
	adds	r5, #40
	adds	r7, #28
	cmp	r2, #0
	bge.n	.L_080ea0e4
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.align 2, 0
