.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0801314c, 0x0801314c
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_08014d00, 0x08014d00
	.set sub_080380c8, 0x080380c8
	.set sub_080c8378, 0x080c8378
	.set sub_080c8390, 0x080c8390
	.set sub_081c0010, 0x081c0010
	.global Func_08108690
	.thumb_func
Func_08108690:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #184]
	movs	r3, #0
	mov	sl, r3
	ldr	r3, [r5, #12]
	ldr	r6, [r0, #0]
	adds	r7, r2, #0
	movs	r2, #32
	ands	r3, r2
	mov	fp, r0
	mov	r8, r1
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L_081086c2
	subs	r6, #1
	adds	r0, r6, r1
	bl	sub_08002064
	adds	r6, r0, #0
.L_081086c2:
	ldr	r3, [r5, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081086da
	adds	r6, #1
	mov	r3, r8
	adds	r0, r6, r3
	mov	r1, r8
	bl	sub_08002064
	adds	r6, r0, #0
.L_081086da:
	ldr	r3, [r5, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081086ec
	subs	r3, r6, r7
	cmp	r3, #0
	blt.n	.L_081086ec
	adds	r6, r3, #0
.L_081086ec:
	ldr	r3, [r5, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08108718
	mov	r3, r8
	adds	r0, r7, r3
	subs	r0, #1
	adds	r1, r7, #0
	bl	sub_08002054
	adds	r3, r7, #0
	muls	r3, r0
	adds	r5, r6, r7
	cmp	r5, r3
	bge.n	.L_0810870e
	adds	r6, r5, #0
.L_0810870e:
	mov	r1, r8
	subs	r1, #1
	cmp	r6, r1
	ble.n	.L_08108718
	adds	r6, r1, #0
.L_08108718:
	cmp	r9, r6
	beq.n	.L_08108726
	movs	r0, #111
	bl	sub_081c0010
	movs	r3, #1
	mov	sl, r3
.L_08108726:
	adds	r1, r7, #0
	mov	r0, r9
	bl	sub_08002054
	adds	r1, r7, #0
	adds	r5, r7, #0
	muls	r5, r0
	adds	r0, r6, #0
	bl	sub_08002054
	adds	r3, r7, #0
	muls	r3, r0
	cmp	r5, r3
	beq.n	.L_08108746
	movs	r3, #2
	mov	sl, r3
.L_08108746:
	mov	r3, fp
	str	r6, [r3, #0]
	mov	r0, sl
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	lsls	r3, r1, #4
	adds	r1, r3, #1
	cmp	r0, #0
	ble.n	.L_08108780
	ldr	r4, [pc, #28]
.L_08108768:
	ldrh	r3, [r4, #0]
	subs	r0, #1
	adds	r3, r2, r3
	adds	r4, #2
	strb	r1, [r3, #4]
	strb	r1, [r3, #8]
	strb	r1, [r3, #12]
	strb	r1, [r3, #16]
	strb	r1, [r3, #20]
	strb	r1, [r3, #24]
	cmp	r0, #0
	bne.n	.L_08108768
.L_08108780:
	pop	{pc}
	movs	r0, r0
	.2byte 0xc348
	.2byte 0x0810
.L_08108788:
	push	{lr}
	ldr	r3, [pc, #76]
	lsls	r0, r0, #5
	adds	r0, r0, r3
	ldr	r3, [pc, #72]
	lsls	r2, r2, #1
	ldrh	r3, [r3, r2]
	movs	r4, #3
	adds	r1, r1, r3
	adds	r1, #2
.L_0810879c:
	ldrb	r2, [r0, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_081087cc
	strb	r2, [r1, #0]
	adds	r0, #1
	ldrb	r2, [r0, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_081087cc
	strb	r2, [r1, #1]
	adds	r0, #1
	ldrb	r2, [r0, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_081087cc
	strb	r2, [r1, #30]
	adds	r0, #1
	ldrb	r2, [r0, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_081087cc
	strb	r2, [r1, #31]
	adds	r0, #1
.L_081087cc:
	subs	r4, #1
	adds	r1, #4
	cmp	r4, #0
	bge.n	.L_0810879c
	pop	{pc}
	movs	r0, r0
	.4byte 0x0810c008
	.2byte 0xc384
	.2byte 0x0810
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r1
	movs	r1, #128
	adds	r5, r0, #0
	lsls	r1, r1, #3
	movs	r0, #56
	mov	sl, r2
	adds	r7, r3, #0
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #0
	mov	r9, r3
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #192]
	adds	r1, r6, #0
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	movs	r1, #10
	bl	sub_08002064
	adds	r1, r6, #0
	movs	r2, #0
	bl	.L_08108788
	adds	r0, r5, #0
	movs	r1, #10
	bl	sub_08002054
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_08108896
	movs	r1, #10
	bl	sub_08002064
	adds	r1, r6, #0
	movs	r2, #1
	bl	.L_08108788
	adds	r0, r5, #0
	movs	r1, #10
	bl	sub_08002054
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_08108896
	movs	r1, #10
	bl	sub_08002064
	adds	r1, r6, #0
	movs	r2, #2
	bl	.L_08108788
	adds	r0, r5, #0
	movs	r1, #10
	bl	sub_08002054
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_08108896
	movs	r1, #10
	bl	sub_08002064
	adds	r1, r6, #0
	movs	r2, #3
	bl	.L_08108788
	adds	r0, r5, #0
	movs	r1, #10
	bl	sub_08002054
	cmp	r0, #0
	beq.n	.L_08108896
	movs	r1, #10
	bl	sub_08002064
	adds	r1, r6, #0
	movs	r2, #4
	bl	.L_08108788
.L_08108896:
	bl	sub_080143ac
	adds	r5, r0, #0
	cmp	r5, #96
	beq.n	.L_081088ba
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r2, r6, #0
	bl	sub_080142d4
	ldr	r1, [pc, #40]
	adds	r0, r5, #0
	mov	r2, r8
	mov	r3, sl
	str	r7, [sp, #0]
	bl	sub_080380c8
	mov	r9, r0
.L_081088ba:
	movs	r0, #56
	bl	sub_0801314c
	mov	r0, r9
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0810c148
	.2byte 0x8000
	.2byte 0x8000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	adds	r3, #128
	ldr	r5, [r3, #0]
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r4, r5, r2
	movs	r2, #128
	movs	r3, #128
	lsls	r2, r2, #2
	adds	r2, #34
	lsls	r3, r3, #19
	adds	r6, r0, #0
	adds	r1, r1, r2
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r2, [pc, #32]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #224
	lsls	r2, r2, #2
	adds	r1, r5, r2
	adds	r0, r4, #0
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_080c8378
	movs	r0, #16
	bl	sub_080c8390
	pop	{r5, r6, pc}
	.4byte 0x84000150
	.2byte 0x02a0
	.2byte 0x8400
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #108]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #34
	adds	r0, r0, r3
	movs	r1, #1
	bl	sub_080c8378
	movs	r0, #16
	bl	sub_080c8390
	pop	{pc}
	.2byte 0x0000
