.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013438, 0x08013438
	.set sub_08015f0c, 0x08015f0c
	.set sub_08016950, 0x08016950
	.set sub_0801713c, 0x0801713c
	.global Func_08016054
	.thumb_func
Func_08016054:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	ldr	r3, [r3, #0]
	movs	r2, #192
	lsls	r2, r2, #6
	adds	r2, #60
	sub	sp, #4
	adds	r6, r3, r2
	movs	r3, #0
	mov	sl, r3
	movs	r7, #0
	mov	r8, sp
.L_08016076:
	mov	r2, r8
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r3, #128
	movs	r2, #133
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	mov	r0, r8
	adds	r1, r6, #0
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r7, #0
	bl	sub_08015f0c
	adds	r5, r0, #0
	cmp	r5, #14
	bhi.n	.L_080160c0
	lsls	r0, r5, #16
	lsrs	r0, r0, #16
	movs	r1, #0
	adds	r2, r6, #0
	movs	r3, #64
	bl	sub_0801713c
	adds	r0, r5, #1
	lsls	r0, r0, #16
	adds	r2, r6, #0
	movs	r3, #4
	lsrs	r0, r0, #16
	adds	r2, #56
	movs	r1, #16
	bl	sub_0801713c
	movs	r3, #1
	add	sl, r3
.L_080160c0:
	adds	r7, #1
	adds	r6, #64
	cmp	r7, #2
	bls.n	.L_08016076
	mov	r0, sl
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	ldr	r7, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #6
	sub	sp, #4
	adds	r3, #252
	adds	r6, r7, r3
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r6, #0
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #3
	bl	sub_08015f0c
	adds	r5, r0, #0
	cmp	r5, #14
	bhi.n	.L_08016136
	lsls	r0, r5, #16
	lsrs	r0, r0, #16
	movs	r1, #0
	adds	r2, r6, #0
	movs	r3, #64
	bl	sub_0801713c
	movs	r3, #196
	adds	r0, r5, #1
	lsls	r3, r3, #6
	adds	r3, #52
	lsls	r0, r0, #16
	movs	r1, #136
	lsrs	r0, r0, #16
	adds	r2, r7, r3
	lsls	r1, r1, #1
	movs	r3, #4
	bl	sub_0801713c
	movs	r0, #1
	b.n	.L_08016138
.L_08016136:
	movs	r0, #0
.L_08016138:
	add	sp, #4
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl	sub_08013438
	movs	r0, #204
	bl	sub_0801314c
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	ldr	r1, [r3, #0]
	cmp	r1, #0
	beq.n	.L_0801617e
	movs	r2, #196
	lsls	r2, r2, #6
	adds	r2, #60
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r3, #197
	lsls	r3, r3, #6
	adds	r2, r1, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r3, #196
	lsls	r3, r3, #6
	adds	r3, #66
	adds	r2, r1, r3
	movs	r3, #3
	strh	r3, [r2, #0]
.L_0801617e:
	pop	{pc}
	push	{r5, r6, r7, lr}
	ldr	r6, [pc, #72]
	sub	sp, #4
	ldrh	r3, [r6, #0]
	adds	r7, r3, #0
	strh	r6, [r6, #0]
	ldr	r5, [pc, #64]
	movs	r0, #7
	movs	r1, #0
	adds	r2, r5, #0
	bl	sub_08013438
	movs	r0, #6
	movs	r1, #0
	adds	r2, r5, #0
	bl	sub_08013438
	ldr	r4, [pc, #36]
	adds	r3, r6, #0
	strh	r4, [r3, #0]
	ldr	r1, [pc, #40]
	movs	r3, #255
	ldrh	r2, [r1, #0]
	lsls	r3, r3, #8
	adds	r3, #63
	ands	r3, r2
	strh	r3, [r1, #0]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	movs	r0, #128
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080161d8
	strh	r0, [r1, #0]
	b.n	.L_080161d8
	.4byte 0x00000000
	.4byte 0x04000208
	.4byte 0x08016695
	.2byte 0x0200
	.2byte 0x0400
.L_080161d8:
	ldrh	r2, [r1, #0]
	movs	r0, #64
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080161e6
	strh	r0, [r1, #0]
.L_080161e6:
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #52]
	ldr	r1, [pc, #68]
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	strh	r4, [r2, #0]
	str	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #6
	strh	r4, [r2, #0]
	str	r3, [r1, #0]
	ldr	r2, [pc, #36]
	ldrh	r3, [r1, #0]
	mov	r0, sp
	orrs	r3, r2
	strh	r3, [r1, #0]
	ldr	r3, [pc, #28]
	ldr	r2, [pc, #40]
	strh	r3, [r6, #0]
	movs	r3, #0
	mov	ip, r2
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	b.n	.L_08016238
	.4byte 0x00008000
	.4byte 0x00004003
	.4byte 0x00000001
	.4byte 0x04000134
	.4byte 0x04000128
	.2byte 0x5360
	.2byte 0x0200
.L_08016238:
	mov	r1, ip
	adds	r2, #88
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #1
	mov	r2, ip
	negs	r3, r3
	str	r3, [r2, #20]
	mov	r3, ip
	adds	r3, #96
	str	r3, [r2, #40]
	mov	r4, ip
	adds	r3, #32
	str	r3, [r2, #44]
	adds	r4, #224
	adds	r3, #64
	adds	r2, #48
	adds	r1, #160
	movs	r0, #1
.L_0801625e:
	subs	r0, #1
	str	r1, [r2, #0]
	str	r3, [r2, #16]
	str	r4, [r2, #32]
	adds	r3, #96
	adds	r4, #96
	adds	r2, #4
	adds	r1, #96
	cmp	r0, #0
	bge.n	.L_0801625e
	ldr	r5, [pc, #68]
	movs	r0, #0
	strh	r0, [r5, #0]
	ldr	r1, [pc, #64]
	ldr	r2, [pc, #48]
	ldrh	r3, [r1, #0]
	ldr	r4, [pc, #48]
	orrs	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #56]
	strh	r2, [r5, #0]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #52]
	strb	r4, [r3, #0]
	ldr	r3, [pc, #52]
	str	r0, [r3, #0]
	ldr	r3, [pc, #52]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #52]
	str	r0, [r3, #0]
	ldr	r3, [pc, #52]
	strh	r0, [r3, #0]
	bl	sub_08016950
	strh	r7, [r5, #0]
	add	sp, #4
	b.n	.L_080162d8
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x04000208
	.4byte 0x04000200
	.4byte 0x030011b8
	.4byte 0x020054c0
	.4byte 0x020038d0
	.4byte 0x020036d4
	.4byte 0x020055d0
	.2byte 0x5354
	.2byte 0x0200
.L_080162d8:
	pop	{r5, r6, r7, pc}
	.align 2, 0
