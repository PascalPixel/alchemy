@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f40d0
	.thumb_func
Func_080f40d0:
	push	{lr}
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	asrs	r0, r0, #8
	bl	Func_080022ec
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	pop	{r1}
	bx	r1
