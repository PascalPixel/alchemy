@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a9e34
	.thumb_func
Func_080a9e34:
	push	{lr}
	bl	Func_080a22f4
	movs	r0, #13
	bl	Func_080a2144
	pop	{r0}
	bx	r0
	bx	lr
