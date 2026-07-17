@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c0edc
	.thumb_func
Func_080c0edc:
	push	{lr}
	cmp	r0, #0
	bge.n	.L0
	adds	r0, #15
.L0:
	asrs	r0, r0, #4
	pop	{r1}
	bx	r1
