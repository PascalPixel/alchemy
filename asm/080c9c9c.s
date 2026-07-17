@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c9c9c
	.thumb_func
Func_080c9c9c:
	push	{lr}
	movs	r1, #1
	bl	Func_080c9ca8
	pop	{r0}
	bx	r0
