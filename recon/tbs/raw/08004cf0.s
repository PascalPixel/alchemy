.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_080045d4, 0x080045d4
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_08007310, 0x08007310
	.global Func_08004cf0
	.thumb_func
Func_08004cf0:
	push	{r5, r6, lr}
	sub	sp, #48
	adds	r6, r0, #0
	mov	r5, sp
	adds	r0, r5, #0
	movs	r1, #128
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	lsls	r1, r1, #9
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	ldr	r3, [r6, #0]
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #16]
	ldr	r3, [r6, #8]
	adds	r0, r5, #0
	str	r3, [r5, #32]
	ldr	r3, [pc, #12]
	bl	sub_080072f0
	add	sp, #48
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.2byte 0x02c0
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	adds	r7, r1, #0
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
	movs	r0, r0
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
	str	r0, [r5, #32]
	ldr	r3, [r7, #0]
	str	r3, [r5, #36]
	ldr	r3, [r7, #4]
	str	r3, [r5, #40]
	ldr	r3, [r7, #8]
	adds	r0, r5, #0
	str	r3, [r5, #44]
	ldr	r3, [pc, #24]
	bl	sub_080072f0
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03000118
	.2byte 0x02c0
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
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
	ldr	r2, [sp, #0]
	mov	lr, r0
	ldr	r7, [r2, #0]
	ldr	r3, [pc, #316]
	mov	r0, fp
	mov	r1, lr
	mov	ip, pc
	bx	r3
	adds	r1, r0, #0
	adds	r0, r7, #0
	mov	ip, pc
	bx	r3
	add	r5, sp, #8
	str	r0, [r5, #0]
	adds	r1, r6, #0
	mov	r0, fp
	mov	ip, pc
	bx	r3
	adds	r1, r0, #0
	adds	r0, r7, #0
	mov	ip, pc
	bx	r3
	mov	r2, r8
	str	r0, [r5, #4]
	negs	r1, r2
	adds	r0, r7, #0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #8]
	ldr	r2, [sp, #0]
	mov	r0, sl
	ldr	r7, [r2, #4]
	mov	r1, r8
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
	adds	r1, r4, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #12]
	mov	r1, r8
	mov	r0, sl
	movs	r0, r0
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
	adds	r1, r4, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #16]
	mov	r1, fp
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r1, r0, #0
	adds	r0, r7, #0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #20]
	ldr	r2, [sp, #0]
	mov	r0, r9
	ldr	r7, [r2, #8]
	mov	r1, r8
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
	adds	r1, r4, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #24]
	mov	r1, r8
	mov	r0, r9
	movs	r0, r0
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
	adds	r1, r4, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #28]
	mov	r1, fp
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r1, r0, #0
	adds	r0, r7, #0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #32]
	ldr	r2, [sp, #4]
	ldr	r3, [r2, #0]
	str	r3, [r5, #36]
	ldr	r3, [r2, #4]
	str	r3, [r5, #40]
	ldr	r3, [r2, #8]
	adds	r0, r5, #0
	str	r3, [r5, #44]
	ldr	r3, [pc, #28]
	bl	sub_080072f0
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03000118
	.2byte 0x02c0
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #64
	str	r0, [sp, #32]
	mov	r0, sp
	adds	r0, #36
	str	r0, [sp, #12]
	adds	r4, r1, #0
	mov	r9, r2
	ldr	r3, [pc, #444]
	ldr	r0, [pc, #448]
	ldr	r1, [sp, #12]
	ldr	r2, [pc, #448]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [sp, #32]
	ldr	r2, [r4, #0]
	ldr	r3, [r1, #0]
	subs	r2, r2, r3
	str	r2, [sp, #28]
	ldr	r3, [r1, #4]
	ldr	r2, [r4, #4]
	subs	r2, r2, r3
	str	r2, [sp, #24]
	ldr	r3, [r1, #8]
	ldr	r2, [r4, #8]
	subs	r2, r2, r3
	str	r2, [sp, #20]
	ldr	r4, [sp, #24]
	ldr	r2, [sp, #28]
	ldr	r0, [sp, #20]
	asrs	r1, r2, #8
	asrs	r3, r4, #8
	asrs	r2, r0, #8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #12]
	adds	r2, r3, #0
	adds	r0, r1, #0
	bl	sub_080072f4
	ldr	r3, [pc, #392]
	bl	sub_080072f0
	adds	r1, r0, #0
	ldr	r0, [pc, #388]
	mov	fp, r0
	movs	r0, #128
	lsls	r0, r0, #24
	bl	sub_08007310
	lsrs	r3, r0, #15
	negs	r3, r3
	ldr	r5, [pc, #376]
	ldr	r0, [sp, #28]
	adds	r1, r3, #0
	movs	r0, r0
	mov	ip, pc
	bx	r5
	str	r0, [sp, #28]
	adds	r1, r3, #0
	ldr	r0, [sp, #24]
	movs	r0, r0
	mov	ip, pc
	bx	r5
	str	r0, [sp, #24]
	adds	r1, r3, #0
	ldr	r0, [sp, #20]
	movs	r0, r0
	mov	ip, pc
	bx	r5
	ldr	r1, [sp, #28]
	negs	r1, r1
	mov	r8, r1
	str	r0, [sp, #20]
	ldr	r0, [sp, #24]
	adds	r1, r0, #0
	mov	ip, pc
	bx	r5
	movs	r3, #128
	lsls	r3, r3, #9
	subs	r0, r3, r0
	cmp	r0, #0
	ble.n	.L_080050a6
	bl	sub_080045d4
	adds	r1, r0, #0
	movs	r0, #128
	lsls	r0, r0, #24
	bl	sub_08007310
	lsls	r3, r0, #1
.L_080050a6:
	ldr	r0, [sp, #20]
	adds	r1, r3, #0
	movs	r0, r0
	mov	ip, pc
	bx	r5
	str	r0, [sp, #16]
	adds	r1, r3, #0
	mov	r0, r8
	movs	r0, r0
	mov	ip, pc
	bx	r5
	mov	r8, r0
	mov	r1, r8
	ldr	r0, [sp, #24]
	movs	r0, r0
	mov	ip, pc
	bx	r5
	mov	sl, r0
	ldr	r1, [sp, #16]
	ldr	r0, [sp, #20]
	movs	r0, r0
	mov	ip, pc
	bx	r5
	adds	r3, r0, #0
	mov	r1, r8
	ldr	r0, [sp, #28]
	movs	r0, r0
	mov	ip, pc
	bx	r5
	subs	r6, r3, r0
	ldr	r1, [sp, #16]
	ldr	r0, [sp, #24]
	movs	r0, r0
	mov	ip, pc
	bx	r5
	negs	r7, r0
	adds	r3, r6, #0
	str	r7, [sp, #0]
	str	r7, [sp, #4]
	mov	r1, sl
	adds	r2, r6, #0
	ldr	r4, [sp, #12]
	mov	r0, sl
	bl	sub_080072f4
	bl	sub_080045d4
	adds	r1, r0, #0
	movs	r0, #128
	lsls	r0, r0, #24
	bl	sub_08007310
	lsls	r3, r0, #1
	adds	r1, r3, #0
	mov	r0, sl
	mov	ip, pc
	bx	r5
	mov	sl, r0
	adds	r1, r3, #0
	adds	r0, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r5
	adds	r6, r0, #0
	adds	r1, r3, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r5
	ldr	r1, [sp, #32]
	adds	r7, r0, #0
	ldr	r1, [r1, #4]
	ldr	r0, [sp, #32]
	ldr	r2, [sp, #32]
	ldr	r0, [r0, #0]
	str	r1, [sp, #8]
	ldr	r3, [sp, #16]
	ldr	r5, [r2, #8]
	mov	fp, r0
	mov	r4, r9
	movs	r0, #0
	mov	r1, r8
	str	r3, [r4, #0]
	str	r0, [r4, #12]
	str	r1, [r4, #24]
	ldr	r1, [sp, #16]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #12]
	adds	r2, r5, #0
	mov	r3, r8
	mov	r0, fp
	bl	sub_080072f4
	mov	r1, r9
	mov	r2, sl
	negs	r0, r0
	str	r0, [r1, #36]
	str	r6, [r1, #16]
	str	r7, [r1, #28]
	str	r2, [r1, #4]
	ldr	r2, [sp, #8]
	str	r5, [sp, #0]
	str	r7, [sp, #4]
	ldr	r4, [sp, #12]
	mov	r1, sl
	adds	r3, r6, #0
	mov	r0, fp
	bl	sub_080072f4
	mov	r1, r9
	negs	r0, r0
	str	r0, [r1, #40]
	ldr	r2, [sp, #28]
	str	r2, [r1, #8]
	ldr	r3, [sp, #24]
	str	r3, [r1, #20]
	ldr	r4, [sp, #20]
	str	r4, [r1, #32]
	ldr	r1, [sp, #28]
	str	r5, [sp, #0]
	str	r4, [sp, #4]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #24]
	ldr	r4, [sp, #12]
	mov	r0, fp
	bl	sub_080072f4
	mov	r1, r9
	negs	r0, r0
	str	r0, [r1, #44]
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0x08007994
	.4byte 0x84000007
	.4byte 0x030001d8
	.4byte 0x030003f0
	.4byte 0x03000118
