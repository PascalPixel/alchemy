@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0802899c
	.thumb_func
Func_0802899c:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r0, #0
	bl	Func_0801c2d0
	bl	Func_080284dc
	movs	r0, #1
	bl	Func_080287a8
	movs	r0, #15
	bl	Func_080287a8
	movs	r0, #2
	bl	Func_080287a8
	movs	r0, #7
	bl	Func_080287a8
	movs	r2, #0
	movs	r0, #17
	movs	r1, #7
	bl	Func_08028808
	subs	r1, r6, #1
	adds	r0, r5, #0
	bl	Func_080286a0
	adds	r6, r0, #0
	bl	Func_0802851c
	bl	Func_0801c2e4
	adds	r0, r6, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
