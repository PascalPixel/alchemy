.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_080072f0, 0x080072f0
	.global Overlay_08004a94
Overlay_08004a94:
	ldr	r3, [pc, #20]
	adds	r0, r3, #0
	movs	r1, #128
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	lsls	r1, r1, #9
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	bx	lr
	movs	r0, r0
	.2byte 0x0350
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	sub	sp, #48
	bl	sub_08002322
	mov	sl, r0
	ldr	r0, [r5, #0]
	bl	sub_0800231c
	mov	r9, r0
	ldr	r0, [r5, #4]
	bl	sub_08002322
	mov	r8, r0
	ldr	r0, [r5, #4]
	bl	sub_0800231c
	mov	fp, r0
	ldr	r0, [r5, #8]
	bl	sub_08002322
	adds	r6, r0, #0
	ldr	r0, [r5, #8]
	bl	sub_0800231c
	mov	lr, r0
	ldr	r3, [pc, #216]
	mov	r0, fp
	mov	r1, lr
	mov	ip, pc
	bx	r3
	mov	r5, sp
	str	r0, [r5, #0]
	adds	r1, r6, #0
	mov	r0, fp
	mov	ip, pc
	bx	r3
	mov	r1, r8
	negs	r2, r1
	str	r0, [r5, #4]
	str	r2, [r5, #8]
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r3
	mov	r1, lr
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r0, #0
	adds	r1, r6, #0
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r3
	subs	r4, r4, r0
	str	r4, [r5, #12]
	mov	r0, sl
	mov	r1, r8
	mov	ip, pc
	bx	r3
	adds	r1, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r0, #0
	mov	r1, lr
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r4, r0
	str	r4, [r5, #16]
	mov	r0, sl
	mov	r1, fp
	mov	ip, pc
	bx	r3
	str	r0, [r5, #20]
	mov	r1, r8
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r3
	mov	r1, lr
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r0, #0
	adds	r1, r6, #0
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r4, r0
	str	r4, [r5, #24]
	mov	r0, r9
	mov	r1, r8
	mov	ip, pc
	bx	r3
	adds	r1, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r0, #0
	mov	r1, lr
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r3
	subs	r4, r4, r0
	str	r4, [r5, #28]
	mov	r0, r9
	mov	r1, fp
	mov	ip, pc
	bx	r3
	movs	r3, #0
	str	r0, [r5, #32]
	str	r3, [r5, #36]
	str	r3, [r5, #40]
	str	r3, [r5, #44]
	adds	r0, r5, #0
	ldr	r3, [pc, #28]
	bl	sub_080072f0
	add	sp, #48
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03000118
