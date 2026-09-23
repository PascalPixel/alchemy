.syntax unified
	.thumb
	.set sub_08002df0, 0x08002df0
	.set sub_080045d4, 0x080045d4
	.set sub_08004938, 0x08004938
	.set sub_080072f0, 0x080072f0
	.set sub_080072fc, 0x080072fc
	.global Func_0800d14c
	.thumb_func
Func_0800d14c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	mov	r9, r3
	ldr	r3, [r7, #8]
	subs	r0, r1, r3
	mov	r8, r1
	mov	sl, r2
	cmp	r0, #0
	bge.n	.L_0800d16a
	ldr	r2, [pc, #396]
	adds	r0, r0, r2
.L_0800d16a:
	ldr	r3, [r7, #12]
	mov	r2, sl
	asrs	r1, r0, #16
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_0800d17a
	ldr	r3, [pc, #380]
	adds	r0, r0, r3
.L_0800d17a:
	ldr	r3, [r7, #16]
	mov	r2, r9
	asrs	r5, r0, #16
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_0800d18a
	ldr	r3, [pc, #364]
	adds	r0, r0, r3
.L_0800d18a:
	adds	r3, r5, #0
	muls	r3, r5
	asrs	r6, r0, #16
	adds	r0, r1, #0
	muls	r0, r1
	adds	r2, r6, #0
	muls	r2, r6
	adds	r0, r0, r3
	adds	r0, r0, r2
	ldr	r3, [pc, #344]
	bl	sub_080072f0
	movs	r2, #128
	lsls	r5, r0, #16
	lsls	r2, r2, #13
	cmp	r5, r2
	bge.n	.L_0800d1ec
	ldr	r3, [r7, #8]
	mov	r2, r8
	subs	r1, r2, r3
	ldr	r3, [r7, #12]
	mov	r2, sl
	subs	r5, r2, r3
	ldr	r3, [r7, #16]
	mov	r2, r9
	subs	r6, r2, r3
	adds	r0, r1, #0
	ldr	r3, [pc, #312]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r0, #0
	adds	r1, r5, #0
	adds	r0, r5, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r5, r0, #0
	adds	r1, r6, #0
	adds	r0, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r4, r5
	adds	r4, r4, r0
	adds	r0, r4, #0
	bl	sub_080045d4
	adds	r5, r0, #0
.L_0800d1ec:
	movs	r3, #128
	lsls	r3, r3, #9
	cmp	r5, r3
	bge.n	.L_0800d20c
	mov	r3, sl
	mov	r2, r8
	str	r3, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r2, [r7, #8]
	mov	r2, r9
	str	r2, [r7, #16]
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	str	r3, [r7, #64]
	b.n	.L_0800d2e4
.L_0800d20c:
	adds	r3, r7, #0
	adds	r3, #88
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0800d27c
	ldr	r1, [r7, #48]
	ldr	r3, [pc, #224]
	adds	r0, r1, #0
	mov	ip, pc
	bx	r3
	adds	r1, r0, #0
	ldr	r3, [pc, #220]
	ldr	r0, [r7, #52]
	bl	sub_080072f0
	adds	r1, r0, #0
	cmp	r5, r1
	ble.n	.L_0800d23a
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	subs	r1, r5, r3
	b.n	.L_0800d240
.L_0800d23a:
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r1, r3, #1
.L_0800d240:
	ldr	r3, [pc, #188]
	adds	r0, r5, #0
	bl	sub_080072f0
	ldr	r4, [r7, #8]
	adds	r5, r0, #0
	mov	r2, r8
	ldr	r3, [pc, #172]
	subs	r0, r2, r4
	adds	r1, r5, #0
	mov	ip, pc
	bx	r3
	adds	r4, r4, r0
	mov	r8, r4
	ldr	r4, [r7, #12]
	mov	r2, sl
	subs	r0, r2, r4
	adds	r1, r5, #0
	mov	ip, pc
	bx	r3
	adds	r4, r4, r0
	mov	sl, r4
	ldr	r4, [r7, #16]
	mov	r2, r9
	subs	r0, r2, r4
	adds	r1, r5, #0
	mov	ip, pc
	bx	r3
	adds	r4, r4, r0
	mov	r9, r4
.L_0800d27c:
	mov	r3, r8
	str	r3, [r7, #56]
	mov	r3, r9
	mov	r2, sl
	str	r3, [r7, #64]
	ldr	r3, [r7, #8]
	str	r2, [r7, #60]
	mov	r2, r8
	subs	r1, r2, r3
	ldr	r3, [r7, #12]
	mov	r2, sl
	subs	r5, r2, r3
	ldr	r3, [r7, #16]
	mov	r2, r9
	subs	r6, r2, r3
	movs	r3, #86
	adds	r3, r3, r7
	mov	ip, r3
	mov	r2, ip
	movs	r3, #16
	strb	r3, [r2, #0]
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_0800d2ae
	negs	r2, r1
.L_0800d2ae:
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_0800d2b6
	negs	r3, r6
.L_0800d2b6:
	cmp	r2, r3
	bge.n	.L_0800d2c2
	movs	r3, #18
	mov	r2, ip
	strb	r3, [r2, #0]
	adds	r1, r6, #0
.L_0800d2c2:
	adds	r3, r7, #0
	adds	r3, #85
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0800d2e4
	cmp	r1, #0
	bge.n	.L_0800d2d2
	negs	r1, r1
.L_0800d2d2:
	adds	r0, r5, #0
	cmp	r0, #0
	bge.n	.L_0800d2da
	negs	r0, r0
.L_0800d2da:
	cmp	r1, r0
	bge.n	.L_0800d2e4
	movs	r3, #17
	mov	r2, ip
	strb	r3, [r2, #0]
.L_0800d2e4:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000ffff
	.4byte 0x030001d8
	.4byte 0x03000118
	.2byte 0x013c
	.2byte 0x0300
	push	{r5, r6, lr}
	ldr	r5, [pc, #44]
	adds	r0, r5, #0
	bl	sub_08004938
	movs	r2, #132
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	ldr	r3, [pc, #32]
	ldr	r0, [pc, #32]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_080072fc
	adds	r0, r6, #0
	bl	sub_08002df0
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000004e8
	.4byte 0x040000d4
	.4byte 0x0800a494
