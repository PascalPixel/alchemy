@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080fa484
	.thumb_func
Func_080fa484:
	push	{lr}
	bl	Func_080fa264
	pop	{r0}
	bx	r0
