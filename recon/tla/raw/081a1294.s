.syntax unified
	.thumb
	.global Func_081a1294
	.thumb_func
Func_081a1294:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #136]
	mov	r8, r2
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #132]
	lsrs	r3, r3, #19
	lsls	r3, r3, #6
	mov	r9, r3
	ldr	r3, [pc, #128]
	adds	r7, r1, #0
	mov	r1, r8
	strh	r1, [r3, #0]
	mov	r3, r8
	strh	r3, [r2, #0]
	ldr	r2, [pc, #120]
	mov	r1, r8
	lsls	r3, r1, #16
	str	r3, [r2, #0]
	movs	r3, #128
	movs	r2, #0
	lsls	r3, r3, #5
	mov	ip, r0
	mov	sl, r2
	mov	fp, r3
.L_081a12d0:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_081a12d8
	adds	r3, #7
.L_081a12d8:
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r1, #255
	asrs	r3, r3, #3
	mov	r4, r9
	lsls	r5, r3, #6
	ands	r4, r1
	mov	lr, r1
	movs	r6, #30
.L_081a12ea:
	mov	r2, ip
	adds	r0, r2, r5
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r7, r4
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r4, #64
	mov	r3, lr
	subs	r6, #1
	adds	r5, #64
	ands	r4, r3
	cmp	r6, #0
	bge.n	.L_081a12ea
	movs	r2, #1
	movs	r1, #128
	add	sl, r2
	lsls	r1, r1, #4
	mov	r3, sl
	add	ip, fp
	adds	r7, r7, r1
	cmp	r3, #14
	ble.n	.L_081a12d0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02007518
	.4byte 0x02007522
	.4byte 0x02007520
	.4byte 0x0200751c
