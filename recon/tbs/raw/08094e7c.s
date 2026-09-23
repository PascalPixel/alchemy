.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08003dec, 0x08003dec
	.set sub_08003fa4, 0x08003fa4
	.set sub_08004080, 0x08004080
	.set sub_080041d8, 0x080041d8
	.set sub_08004458, 0x08004458
	.set sub_080048f4, 0x080048f4
	.set sub_080053e8, 0x080053e8
	.set sub_080091a8, 0x080091a8
	.set sub_080770c0, 0x080770c0
	.global Func_08094e7c
	.thumb_func
Func_08094e7c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #244]
	ldr	r0, [r3, #0]
	sub	sp, #28
	movs	r1, #0
	str	r0, [sp, #24]
	str	r1, [sp, #20]
	subs	r3, #84
	ldr	r3, [r3, #0]
	str	r3, [sp, #12]
	adds	r3, #228
	adds	r7, r0, #0
	str	r3, [sp, #8]
	str	r1, [sp, #4]
	str	r1, [sp, #0]
	str	r1, [sp, #16]
	adds	r7, #8
	mov	r9, r1
	mov	fp, r1
	mov	r8, r1
.L_08094eb0:
	ldrh	r3, [r7, #28]
	cmp	r3, #0
	bne.n	.L_08094eb8
	b.n	.L_08094fc8
.L_08094eb8:
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #8]
	ldr	r2, [r2, #0]
	ldr	r3, [r3, #4]
	movs	r0, #179
	lsls	r0, r0, #1
	mov	r8, r2
	mov	sl, r3
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_08094ed6
	ldrh	r3, [r7, #28]
	adds	r3, #1
	strh	r3, [r7, #28]
.L_08094ed6:
	ldrh	r3, [r7, #28]
	ldr	r2, [pc, #168]
	lsrs	r3, r3, #1
	lsls	r3, r3, #2
	adds	r6, r3, r2
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	movs	r3, #1
	ldr	r2, [r7, #12]
	ands	r0, r3
	mov	r1, r8
	ands	r3, r5
	subs	r2, r2, r1
	adds	r3, r3, r0
	asrs	r2, r2, #16
	lsrs	r3, r3, #1
	adds	r1, r2, r3
	subs	r2, r1, #1
	mov	fp, r2
	ldr	r3, [r7, #20]
	ldr	r2, [r7, #16]
	mov	r0, sl
	subs	r3, r3, r2
	subs	r3, r3, r0
	cmp	r3, #0
	bge.n	.L_08094f14
	ldr	r2, [pc, #116]
	adds	r3, r3, r2
.L_08094f14:
	movs	r0, #0
	ldrsh	r2, [r6, r0]
	asrs	r3, r3, #16
	adds	r3, r3, r2
	mov	r8, r3
	adds	r3, r1, #0
	adds	r3, #15
	adds	r6, #2
	cmp	r3, #255
	bhi.n	.L_08094fb2
	movs	r1, #32
	negs	r1, r1
	cmp	r8, r1
	blt.n	.L_08094fb2
	mov	r2, r8
	cmp	r2, #159
	bgt.n	.L_08094fb2
	ldr	r3, [sp, #24]
	ldr	r1, [r3, #4]
	ldrh	r3, [r6, #0]
	adds	r1, r1, r3
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #48]
	ands	r1, r3
	ldrh	r3, [r7, #8]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #8]
	ldr	r3, [pc, #40]
	mov	r1, fp
	ands	r1, r3
	ldr	r2, [pc, #40]
	ldrh	r3, [r7, #6]
	ands	r3, r2
	orrs	r3, r1
	ldrb	r2, [r7, #5]
	movs	r1, #63
	strh	r3, [r7, #6]
	adds	r3, r1, #0
	ands	r3, r2
	strb	r3, [r7, #5]
	ldrb	r3, [r7, #7]
	ands	r1, r3
	movs	r3, #64
	b.n	.L_08094f8c
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x03001ec4
	.4byte 0x0809f024
	.2byte 0xffff
	.2byte 0x0000
.L_08094f8c:
	orrs	r1, r3
	ldr	r3, [pc, #48]
	mov	r0, r8
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #36]
	strb	r0, [r7, #4]
	movs	r0, #63
	negs	r0, r0
	lsrs	r3, r3, #1
	ands	r3, r2
	adds	r2, r0, #0
	lsls	r3, r3, #4
	ands	r1, r2
	orrs	r1, r3
	strb	r1, [r7, #7]
	adds	r0, r7, #0
	movs	r1, #240
	bl	sub_08003dec
.L_08094fb2:
	ldrh	r3, [r7, #28]
	ldr	r1, [pc, #12]
	adds	r3, r3, r1
	strh	r3, [r7, #28]
	b.n	.L_08094fc8
	.4byte 0x00000001
	.4byte 0x03001e40
	.2byte 0xffff
	.2byte 0x0000
.L_08094fc8:
	ldr	r2, [sp, #20]
	cmp	r2, #3
	bhi.n	.L_08095078
	ldrh	r3, [r7, #28]
	cmp	r3, #0
	bne.n	.L_08095078
	ldr	r0, [sp, #24]
	ldr	r1, [pc, #68]
	adds	r3, r0, r1
	ldr	r5, [r3, #0]
	cmp	r5, #0
	bne.n	.L_08095078
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L_08095020
	ldr	r3, [sp, #4]
	str	r3, [r7, #12]
	ldr	r0, [sp, #0]
	mov	r3, r8
	mov	r2, fp
	asrs	r1, r2, #16
	str	r0, [r7, #20]
	asrs	r2, r3, #16
	movs	r0, #0
	bl	sub_080091a8
	ldr	r3, [pc, #24]
	lsls	r0, r0, #16
	str	r0, [r7, #16]
	mov	r0, r9
	subs	r3, r3, r0
	strh	r3, [r7, #28]
	str	r5, [r7, #24]
	ldr	r1, [sp, #20]
	movs	r2, #4
	adds	r1, #1
	str	r1, [sp, #20]
	add	r9, r2
	b.n	.L_08095078
	movs	r0, r0
	.4byte 0x0000003e
	.2byte 0x040c
	.2byte 0x0000
.L_08095020:
	bl	sub_08004458
	movs	r3, #255
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_08095078
	ldr	r3, [sp, #12]
	ldr	r6, [r3, #0]
	bl	sub_08004458
	ldr	r3, [r6, #0]
	lsls	r0, r0, #8
	ldr	r5, [pc, #92]
	adds	r3, r3, r0
	adds	r3, r3, r5
	str	r3, [sp, #4]
	bl	sub_08004458
	ldr	r3, [r6, #8]
	lsls	r0, r0, #8
	adds	r3, r3, r0
	adds	r3, r3, r5
	ldr	r0, [sp, #4]
	str	r3, [sp, #0]
	mov	r2, fp
	str	r3, [r7, #20]
	mov	r3, r8
	asrs	r1, r2, #16
	str	r0, [r7, #12]
	asrs	r2, r3, #16
	movs	r0, #0
	bl	sub_080091a8
	lsls	r0, r0, #16
	str	r0, [r7, #16]
	movs	r3, #30
	mov	r0, r9
	strh	r3, [r7, #28]
	str	r0, [r7, #24]
	ldr	r1, [sp, #20]
	adds	r1, #1
	movs	r2, #4
	str	r1, [sp, #20]
	mov	r9, r2
.L_08095078:
	ldr	r3, [sp, #16]
	adds	r3, #1
	str	r3, [sp, #16]
	adds	r7, #32
	cmp	r3, #31
	bhi.n	.L_08095086
	b.n	.L_08094eb0
.L_08095086:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.2byte 0xff80
	.2byte 0xb5e0
	movs	r1, #130
	lsls	r1, r1, #3
	movs	r0, #29
	sub	sp, #4
	bl	sub_080048f4
	adds	r5, r0, #0
	adds	r7, r5, #0
	movs	r1, #0
	mov	r0, sp
	str	r1, [r0, #0]
	adds	r7, #8
	ldr	r3, [pc, #136]
	adds	r1, r5, #0
	ldr	r2, [pc, #136]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #14
	bl	sub_080048f4
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [pc, #120]
	bl	sub_080053e8
	bl	sub_08004080
	movs	r1, #128
	adds	r2, r6, #0
	str	r0, [r5, #0]
	lsls	r1, r1, #2
	bl	sub_08003fa4
	str	r0, [r5, #4]
	movs	r0, #14
	bl	sub_08002dd8
	movs	r5, #0
	movs	r6, #0
.L_080950f0:
	ldr	r3, [pc, #88]
	ldr	r3, [r3, #0]
	adds	r1, r7, #0
	stmia	r1!, {r6}
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #84]
	stmia	r1!, {r3}
	movs	r3, #212
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	asrs	r1, r1, #16
	str	r6, [r7, #12]
	str	r6, [r7, #20]
	asrs	r2, r2, #16
	movs	r0, #0
	bl	sub_080091a8
	ldr	r2, [pc, #36]
	adds	r3, r5, #0
	ands	r3, r2
	lsls	r0, r0, #16
	adds	r3, #1
	adds	r5, #1
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	adds	r7, #32
	cmp	r5, #31
	bls.n	.L_080950f0
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #32]
	bl	sub_080041d8
	add	sp, #4
	b.n	.L_08095158
	movs	r0, r0
	.4byte 0x0000000f
	.4byte 0x040000d4
	.4byte 0x85000104
	.4byte 0x080a00b8
	.4byte 0x03001e70
	.4byte 0x40000400
	.2byte 0x4e7d
	.2byte 0x0809
.L_08095158:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
