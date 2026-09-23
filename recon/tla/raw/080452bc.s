.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_080143f8, 0x080143f8
	.set sub_08014d00, 0x08014d00
	.set sub_0803da78, 0x0803da78
	.global Func_080452bc
	.thumb_func
Func_080452bc:
.L_080452bc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	adds	r6, r3, #0
	negs	r3, r6
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	adds	r7, r2, #0
	movs	r0, #0
	lsls	r3, r3, #2
	lsls	r2, r6, #2
	mov	ip, r0
	mov	r8, r3
	mov	lr, r2
.L_080452da:
	ldr	r0, [sp, #0]
	movs	r2, #0
	ldmia	r0!, {r4}
	adds	r3, r0, #0
	str	r3, [sp, #0]
	ldr	r0, [sp, #4]
	ldmia	r0!, {r1}
	adds	r3, r0, #0
	str	r3, [sp, #4]
	cmp	r6, #0
	bge.n	.L_080452f6
	mov	r3, r8
	lsrs	r4, r3
	b.n	.L_080452fa
.L_080452f6:
	mov	r0, lr
	lsls	r4, r0
.L_080452fa:
	ldr	r5, [pc, #48]
	movs	r0, #7
.L_080452fe:
	lsls	r2, r2, #4
	cmp	r4, r5
	bls.n	.L_08045308
	lsrs	r3, r4, #28
	b.n	.L_0804530a
.L_08045308:
	lsrs	r3, r1, #28
.L_0804530a:
	adds	r2, r2, r3
	subs	r0, #1
	lsls	r4, r4, #4
	lsls	r1, r1, #4
	cmp	r0, #0
	bge.n	.L_080452fe
	stmia	r7!, {r2}
	movs	r2, #1
	add	ip, r2
	mov	r3, ip
	cmp	r3, #7
	ble.n	.L_080452da
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0fff
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #56]
	adds	r7, r0, #0
	ldr	r3, [r3, #0]
	lsrs	r6, r3, #2
	movs	r3, #3
	ands	r6, r3
	cmp	r6, #2
	ble.n	.L_08045344
	movs	r6, #2
.L_08045344:
	cmp	r6, #0
	bgt.n	.L_0804534a
	movs	r6, #1
.L_0804534a:
	ldr	r5, [pc, #36]
	adds	r6, #1
	negs	r3, r6
	adds	r1, r5, #0
	adds	r2, r7, #0
	ldr	r0, [pc, #28]
	bl	.L_080452bc
	adds	r5, #32
	adds	r2, r7, #0
	ldr	r0, [pc, #24]
	adds	r2, #32
	adds	r1, r5, #0
	adds	r3, r6, #0
	bl	.L_080452bc
	pop	{r5, r6, r7, pc}
	.4byte 0x0300122c
	.4byte 0x0805f730
	.4byte 0x06000220
	.2byte 0x0240
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #64]
	adds	r6, r0, #0
	ldr	r3, [r3, #0]
	lsrs	r7, r3, #2
	movs	r3, #3
	ands	r7, r3
	cmp	r7, #2
	ble.n	.L_08045394
	movs	r7, #2
.L_08045394:
	cmp	r7, #0
	bgt.n	.L_0804539a
	movs	r7, #1
.L_0804539a:
	ldr	r3, [pc, #44]
	ldr	r5, [pc, #44]
	mov	r8, r3
	adds	r7, #1
	negs	r3, r7
	mov	r0, r8
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	.L_080452bc
	adds	r5, #32
	adds	r2, r6, #0
	adds	r2, #32
	mov	r0, r8
	adds	r1, r5, #0
	adds	r3, r7, #0
	bl	.L_080452bc
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0300122c
	.4byte 0x06000400
	.2byte 0xf730
	.2byte 0x0805
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	mov	r9, r0
	mov	sl, r2
	mov	r8, r2
	mov	fp, r2
.L_080453e8:
	movs	r3, #0
	mov	lr, r3
	mov	r3, fp
	add	r3, sl
	lsls	r7, r3, #6
.L_080453f2:
	mov	r2, r9
	adds	r6, r2, r7
	mov	r2, lr
	lsls	r3, r2, #5
	ldr	r2, [pc, #96]
	adds	r5, r3, r2
	movs	r3, #0
	mov	ip, r3
.L_08045402:
	ldrh	r1, [r5, #0]
	movs	r0, #0
	adds	r5, #2
	movs	r4, #0
.L_0804540a:
	adds	r3, r1, #0
	movs	r2, #15
	ands	r3, r2
	add	r3, r8
	lsls	r2, r3, #1
	ldr	r3, [pc, #72]
	lsrs	r1, r1, #4
	ldrh	r2, [r3, r2]
	lsls	r3, r4, #2
	lsls	r2, r3
	adds	r4, #1
	orrs	r0, r2
	cmp	r4, #3
	ble.n	.L_0804540a
	movs	r2, #1
	add	ip, r2
	mov	r3, ip
	strh	r0, [r6, #0]
	adds	r6, #2
	cmp	r3, #15
	ble.n	.L_08045402
	add	lr, r2
	mov	r2, lr
	adds	r7, #32
	cmp	r2, #9
	ble.n	.L_080453f2
	movs	r3, #16
	add	r8, r3
	movs	r3, #1
	movs	r2, #4
	add	sl, r3
	add	fp, r2
	mov	r2, sl
	cmp	r2, #1
	ble.n	.L_080453e8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x06000600
	.2byte 0xf770
	.2byte 0x0805
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	mov	r8, r1
	movs	r1, #193
	adds	r6, r0, #0
	lsls	r1, r1, #3
	movs	r0, #68
	bl	sub_08014d00
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_0803da78
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	adds	r1, r5, #0
	mov	r0, r8
	bl	sub_080143f8
	adds	r5, r0, #0
	movs	r0, #68
	bl	sub_0801314c
	adds	r0, r5, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.align 2, 0
