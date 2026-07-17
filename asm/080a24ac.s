@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a24ac
	.thumb_func
Func_080a24ac:
	push	{lr}
	movs	r0, #15
	bl	Func_080150b8
	pop	{r0}
	bx	r0
