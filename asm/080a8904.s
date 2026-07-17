@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a8904
	.thumb_func
Func_080a8904:
	push	{lr}
	movs	r3, #255
.L0:
	subs	r3, #1
	cmp	r3, #0
	bge.n	.L0
	pop	{r0}
	bx	r0
