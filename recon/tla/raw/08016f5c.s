.syntax unified
	.thumb
	.global Func_08016f5c
	.thumb_func
Func_08016f5c:
	adds	r2, r1, #0
	lsls	r0, r0, #24
	lsrs	r1, r0, #24
	cmp	r1, #3
	bhi.n	.L_08016f90
	ldr	r0, [pc, #24]
	strb	r1, [r0, #0]
	ldr	r1, [pc, #24]
	ldrb	r0, [r0, #0]
	lsls	r0, r0, #2
	ldr	r3, [pc, #20]
	adds	r0, r0, r3
	str	r0, [r1, #0]
	ldr	r0, [pc, #20]
	str	r0, [r2, #0]
	movs	r0, #0
	b.n	.L_08016f92
	movs	r0, r0
	.4byte 0x02007420
	.4byte 0x02007428
	.4byte 0x04000100
	.2byte 0x6f39
	.2byte 0x0801
.L_08016f90:
	movs	r0, #1
.L_08016f92:
	bx	lr
	push	{r4, r5, lr}
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	ldr	r1, [pc, #80]
	lsls	r2, r0, #1
	adds	r2, r2, r0
	lsls	r2, r2, #1
	ldr	r0, [r1, #0]
	adds	r2, r2, r0
	ldr	r1, [pc, #72]
	ldr	r3, [pc, #72]
	ldrh	r0, [r3, #0]
	strh	r0, [r1, #0]
	movs	r5, #0
	strh	r5, [r3, #0]
	ldr	r4, [pc, #68]
	ldr	r0, [pc, #68]
	ldrb	r0, [r0, #0]
	movs	r1, #8
	lsls	r1, r0
	ldrh	r0, [r4, #0]
	orrs	r0, r1
	strh	r0, [r4, #0]
	movs	r0, #1
	strh	r0, [r3, #0]
	ldr	r0, [pc, #56]
	strb	r5, [r0, #0]
	ldr	r1, [pc, #56]
	ldrh	r0, [r2, #0]
	strh	r0, [r1, #0]
	adds	r2, #2
	ldr	r3, [pc, #52]
	ldr	r0, [r3, #0]
	ldrh	r1, [r2, #0]
	strh	r1, [r0, #0]
	adds	r0, #2
	str	r0, [r3, #0]
	ldrh	r1, [r2, #2]
	strh	r1, [r0, #0]
	subs	r0, #2
	str	r0, [r3, #0]
	pop	{r4, r5}
	pop	{r0}
	bx	r0
	.4byte 0x02007418
	.4byte 0x0200742c
	.4byte 0x04000208
	.4byte 0x04000200
	.4byte 0x02007420
	.4byte 0x02007424
	.4byte 0x02007422
	.2byte 0x7428
	.2byte 0x0200
	ldr	r1, [pc, #44]
	ldr	r0, [r1, #0]
	movs	r2, #0
	strh	r2, [r0, #0]
	adds	r0, #2
	str	r0, [r1, #0]
	strh	r2, [r0, #0]
	subs	r0, #2
	str	r0, [r1, #0]
	ldr	r3, [pc, #32]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #32]
	ldr	r0, [pc, #32]
	ldrb	r0, [r0, #0]
	movs	r1, #8
	lsls	r1, r0
	ldrh	r0, [r2, #0]
	bics	r0, r1
	strh	r0, [r2, #0]
	ldr	r0, [pc, #24]
	ldrh	r0, [r0, #0]
	strh	r0, [r3, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x02007428
	.4byte 0x04000208
	.4byte 0x04000200
	.4byte 0x02007420
	.2byte 0x742c
	.2byte 0x0200
	ldrb	r0, [r0, #0]
	bx	lr
