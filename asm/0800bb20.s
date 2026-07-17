@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800bb20
	.thumb_func
Func_0800bb20:
	push	{r5, r6, r7, lr}
	sub	sp, #4
	cmp	r0, #3
	bne.n	.L0
	movs	r1, #224
	lsls	r1, r1, #4
	movs	r0, #4
	bl	Func_080048f4
	movs	r1, #192
	adds	r7, r0, #0
	lsls	r1, r1, #3
	movs	r0, #3
	bl	Func_080048f4
	b.n	.L1
.L0:
	movs	r1, #224
	lsls	r1, r1, #4
	movs	r0, #4
	bl	Func_080048b0
	movs	r1, #192
	adds	r7, r0, #0
	lsls	r1, r1, #3
	movs	r0, #3
	bl	Func_080048b0
.L1:
	adds	r6, r0, #0
	bl	Func_08004838
	movs	r5, #0
	mov	r4, sp
	str	r5, [r4, #0]
	ldr	r3, [pc, #68]
	adds	r0, r4, #0
	adds	r1, r7, #0
	ldr	r2, [pc, #64]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	str	r5, [r4, #0]
	adds	r0, r4, #0
	adds	r1, r6, #0
	ldr	r2, [pc, #56]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #56]
	movs	r1, #128
	movs	r0, #93
	bl	Func_08003fa4
	ldr	r5, [pc, #48]
	movs	r0, #53
	adds	r1, r5, #0
	bl	Func_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #16]
	ldr	r0, [pc, #32]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #4
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x040000d4
	.4byte 0x85000380
	.4byte 0x85000180
	.4byte 0x08012f20
	.4byte 0x0000007c
	.4byte 0x0800a418
