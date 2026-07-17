@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a24b8
	.thumb_func
Func_080a24b8:
	push	{lr}
	movs	r0, #2
	bl	Func_080150b8
	pop	{r0}
	bx	r0
