@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800c004
	.thumb_func
Func_0800c004:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	movs	r1, #92
	movs	r0, #6
	sub	sp, #4
	bl	Func_080048f4
	movs	r1, #224
	lsls	r1, r1, #5
	mov	r8, r0
	movs	r0, #5
	bl	Func_080048f4
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	Func_0800bb20
	movs	r5, #0
	mov	r4, sp
	str	r5, [r4, #0]
	ldr	r3, [pc, #104]
	adds	r0, r4, #0
	adds	r1, r6, #0
	ldr	r2, [pc, #104]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	str	r5, [r4, #0]
	adds	r0, r4, #0
	mov	r1, r8
	ldr	r2, [pc, #96]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	cmp	r7, #4
	bne.n	.L0
	ldr	r0, [pc, #88]
	ldr	r1, [pc, #92]
	bl	Func_080041d8
	b.n	.L1
.L0:
	ldr	r0, [pc, #88]
	ldr	r1, [pc, #80]
	bl	Func_080041d8
.L1:
	subs	r3, r7, #3
	cmp	r3, #1
	bhi.n	.L2
	movs	r1, #200
	ldr	r0, [pc, #76]
	lsls	r1, r1, #4
	bl	Func_080041d8
	b.n	.L3
.L2:
	movs	r1, #200
	ldr	r0, [pc, #68]
	lsls	r1, r1, #4
	bl	Func_080041d8
	ldr	r3, [pc, #64]
	movs	r2, #0
	str	r2, [r3, #0]
	ldr	r3, [pc, #60]
	str	r2, [r3, #0]
.L3:
	mov	r1, r8
	movs	r2, #0
	movs	r3, #15
	strb	r3, [r1, #6]
	strb	r2, [r1, #7]
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0x85000700
	.4byte 0x85000017
	.4byte 0x0800d341
	.4byte 0x00000c8a
	.4byte 0x0800cacd
	.4byte 0x0800c881
	.4byte 0x0800c62d
	.4byte 0x03001d1c
	.4byte 0x03001cc0
	bx	lr
