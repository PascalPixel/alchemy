.syntax unified
	.thumb
	.set sub_08038080, 0x08038080
	.set sub_080380a8, 0x080380a8
	.set sub_080f8888, 0x080f8888
	.global Func_080f9224
	.thumb_func
Func_080f9224:
	push	{r5, r6, r7, lr}
	adds	r6, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	ldr	r7, [sp, #20]
	mov	ip, r3
	movs	r5, #12
	ldrsh	r3, [r0, r5]
	ldr	r4, [sp, #16]
	adds	r3, r1, r3
	adds	r1, r3, #1
	movs	r5, #14
	ldrsh	r3, [r0, r5]
	lsls	r7, r7, #12
	adds	r3, r2, r3
	adds	r2, r3, #1
	cmp	r1, #0
	bge.n	.L_080f924e
	adds	r6, r6, r1
	movs	r1, #0
.L_080f924e:
	adds	r3, r1, r6
	cmp	r3, #29
	ble.n	.L_080f9258
	movs	r3, #30
	subs	r6, r3, r1
.L_080f9258:
	cmp	r2, #0
	bge.n	.L_080f9260
	adds	r4, r4, r2
	movs	r2, #0
.L_080f9260:
	adds	r3, r2, r4
	cmp	r3, #29
	ble.n	.L_080f926a
	movs	r3, #20
	subs	r4, r3, r2
.L_080f926a:
	cmp	r6, #0
	ble.n	.L_080f92a4
	cmp	r4, #0
	ble.n	.L_080f92a4
	lsls	r3, r1, #1
	lsls	r2, r2, #6
	add	r3, ip
	adds	r0, r2, r3
.L_080f927a:
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r1, #8
	cmp	r2, #0
	beq.n	.L_080f9296
	ldr	r5, [pc, #32]
.L_080f9286:
	ldrh	r3, [r1, #0]
	subs	r2, #1
	ands	r3, r5
	orrs	r3, r7
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r2, #0
	bne.n	.L_080f9286
.L_080f9296:
	subs	r4, #1
	adds	r0, #64
	cmp	r4, #0
	bne.n	.L_080f927a
	movs	r3, #1
	mov	r2, ip
	strb	r3, [r2, #3]
.L_080f92a4:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xefff
	.2byte 0xffff
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #24]
	ldr	r1, [pc, #24]
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #24
	ldr	r0, [pc, #16]
	adds	r1, #28
	adds	r2, #1
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.4byte 0x05000200
	.4byte 0x050001c0
	.2byte 0x01e8
	.2byte 0x0500
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r9, r3
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #4
	mov	r8, r3
	movs	r2, #13
	adds	r3, #76
	movs	r6, #31
.L_080f92fa:
	ldmia	r3!, {r5}
	cmp	r5, #0
	beq.n	.L_080f9302
	strb	r2, [r5, #5]
.L_080f9302:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L_080f92fa
	adds	r6, r1, #0
	adds	r0, r0, r6
	cmp	r6, r0
	bge.n	.L_080f9368
	lsls	r2, r6, #2
	adds	r3, r2, #0
	adds	r3, #76
	mov	r1, r8
	ldr	r5, [r1, r3]
	cmp	r5, #0
	beq.n	.L_080f9368
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, r8
	ldrb	r3, [r3, #0]
	subs	r3, #1
	cmp	r6, r3
	bgt.n	.L_080f9368
	adds	r3, r2, r1
	ldr	r7, [sp, #32]
	adds	r2, r3, #0
	mov	sl, r0
	adds	r2, #76
.L_080f9336:
	mov	r3, r9
	strh	r3, [r5, #6]
	strh	r7, [r5, #8]
	adds	r0, r5, #0
	str	r2, [sp, #0]
	bl	sub_080f8888
	adds	r6, #1
	movs	r3, #1
	strb	r3, [r5, #5]
	adds	r7, #16
	ldr	r2, [sp, #0]
	cmp	r6, sl
	bge.n	.L_080f9368
	adds	r2, #4
	ldr	r5, [r2, #0]
	cmp	r5, #0
	beq.n	.L_080f9368
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, r8
	ldrb	r3, [r3, #0]
	subs	r3, #1
	cmp	r6, r3
	ble.n	.L_080f9336
.L_080f9368:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	ldr	r3, [pc, #36]
	sub	sp, #4
	adds	r5, r0, #0
	ldr	r0, [r3, #16]
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r2, r5, #0
	movs	r1, #7
	movs	r3, #8
	bl	sub_080380a8
	ldr	r0, [pc, #16]
	adds	r1, r5, #0
	movs	r2, #64
	movs	r3, #0
	bl	sub_08038080
	add	sp, #4
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0x103a
	.2byte 0x0000
	push	{lr}
	cmp	r0, #0
	beq.n	.L_080f93b4
	strh	r3, [r0, #8]
	ldr	r3, [sp, #4]
	strh	r1, [r0, #12]
	strh	r3, [r0, #10]
	strh	r2, [r0, #14]
.L_080f93b4:
	pop	{pc}
	.align 2, 0
