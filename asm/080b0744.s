@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b0744
	.thumb_func
Func_080b0744:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r1
	movs	r1, #128
	adds	r5, r0, #0
	lsls	r1, r1, #3
	movs	r0, #14
	mov	sl, r2
	adds	r7, r3, #0
	sub	sp, #4
	bl	Func_080048f4
	movs	r3, #0
	adds	r6, r0, #0
	mov	r9, r3
	ldr	r0, [pc, #196]
	ldr	r3, [pc, #200]
	adds	r1, r6, #0
	ldr	r2, [pc, #200]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022fc
	adds	r1, r6, #0
	movs	r2, #0
	bl	Func_080b06ec
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	movs	r1, #10
	bl	Func_080022fc
	adds	r1, r6, #0
	movs	r2, #1
	bl	Func_080b06ec
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	movs	r1, #10
	bl	Func_080022fc
	adds	r1, r6, #0
	movs	r2, #2
	bl	Func_080b06ec
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	movs	r1, #10
	bl	Func_080022fc
	adds	r1, r6, #0
	movs	r2, #3
	bl	Func_080b06ec
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	cmp	r0, #0
	beq.n	.L0
	movs	r1, #10
	bl	Func_080022fc
	adds	r1, r6, #0
	movs	r2, #4
	bl	Func_080b06ec
.L0:
	bl	Func_08004080
	adds	r5, r0, #0
	cmp	r5, #96
	beq.n	.L1
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r2, r6, #0
	bl	Func_08003fa4
	ldr	r1, [pc, #52]
	adds	r0, r5, #0
	mov	r2, r8
	mov	r3, sl
	str	r7, [sp, #0]
	bl	Func_080150c8
	mov	r9, r0
.L1:
	movs	r0, #14
	bl	Func_08002dd8
	mov	r0, r9
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x080b3e80
	.4byte 0x040000d4
	.4byte 0x84000040
	.4byte 0x80008000
