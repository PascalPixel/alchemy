@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080284dc
	.thumb_func
Func_080284dc:
	push	{r5, lr}
	movs	r1, #152
	movs	r0, #58
	sub	sp, #4
	bl	Func_080048f4
	movs	r3, #0
	adds	r5, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	adds	r1, r5, #0
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #24]
	ldr	r0, [pc, #24]
	bl	Func_080041d8
	adds	r0, r5, #0
	add	sp, #4
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x040000d4
	.4byte 0x85000026
	.4byte 0x00000c76
	.4byte 0x08028195
