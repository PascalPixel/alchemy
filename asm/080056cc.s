@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080056cc
	.thumb_func
Func_080056cc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #136
	lsls	r1, r1, #5
	movs	r0, #51
	sub	sp, #24
	bl	Func_080048f4
	movs	r3, #0
	mov	fp, r0
	add	r0, sp, #4
	str	r3, [r0, #0]
	mov	r1, fp
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #44]
	movs	r0, #2
	bl	Func_080069c8
	movs	r7, #0
	b.n	.L0
.L2:
	movs	r0, #1
	bl	Func_080030f8
	adds	r7, #1
.L0:
	cmp	r7, #7
	bhi.n	.L1
	bl	Func_08006910
	lsls	r0, r0, #16
	cmp	r0, #0
	bne.n	.L2
	b.n	.L3
.L1:
	movs	r0, #1
	b.n	.L4
	.4byte 0x040000d4
	.4byte 0x85000440
	.4byte 0x030000f4
.L3:
	mov	r2, fp
	movs	r3, #8
	adds	r2, #64
	add	r3, sp
	mov	r6, fp
	str	r2, [sp, #0]
	mov	r8, r3
	movs	r2, #32
	movs	r3, #16
	adds	r2, r2, r6
	adds	r3, r3, r6
	movs	r7, #0
	mov	r9, r2
	mov	sl, r3
.L11:
	movs	r3, #0
	strb	r3, [r6, #0]
	mov	r2, sl
	movs	r3, #16
	strb	r3, [r2, #0]
	ldr	r3, [pc, #52]
	mov	r2, r9
	strh	r3, [r2, #0]
	adds	r0, r7, #0
	bl	Func_080058ac
	ldr	r3, [pc, #44]
	adds	r5, r0, #0
	add	r1, sp, #8
	ldr	r0, [sp, #0]
	ldr	r2, [pc, #40]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L5:
	ldr	r2, [pc, #28]
	ldr	r3, [r2, #8]
	movs	r2, #128
	lsls	r2, r2, #24
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L5
	mov	r0, r8
	ldr	r1, [pc, #20]
	movs	r2, #7
	bl	Func_08005c08
	b.n	.L6
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x040000d4
	.4byte 0x84000004
	.4byte 0x080079b0
.L6:
	cmp	r0, #0
	bne.n	.L7
	mov	r2, r8
	ldrh	r3, [r2, #10]
	mov	r2, r9
	strh	r3, [r2, #0]
	mov	r3, r8
	ldrb	r2, [r3, #7]
	adds	r1, r2, #0
	cmp	r1, #15
	bhi.n	.L7
	cmp	r5, #0
	bne.n	.L7
	movs	r3, #1
	strb	r3, [r6, #0]
	mov	r3, sl
	strb	r2, [r3, #0]
	cmp	r5, r7
	bcs.n	.L7
	mov	ip, r1
	mov	r1, fp
	adds	r0, r1, #0
	movs	r4, #0
	adds	r0, #32
.L10:
	ldrb	r3, [r1, #16]
	cmp	r3, ip
	bne.n	.L8
	mov	r3, r8
	ldrh	r3, [r3, #10]
	ldrh	r2, [r0, #0]
	mov	lr, r3
	cmp	r2, lr
	bcs.n	.L9
	strb	r4, [r1, #0]
	b.n	.L8
.L9:
	strb	r4, [r6, #0]
.L8:
	adds	r5, #1
	adds	r1, #1
	adds	r0, #2
	cmp	r5, r7
	bcc.n	.L10
.L7:
	movs	r2, #2
	movs	r3, #1
	adds	r7, #1
	adds	r6, #1
	add	r9, r2
	add	sl, r3
	cmp	r7, #15
	bls.n	.L11
	movs	r0, #0
.L4:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
