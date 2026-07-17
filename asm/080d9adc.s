@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080d9adc
	.thumb_func
Func_080d9adc:
	push	{lr}
	movs	r1, #3
	bl	Func_080d9ae8
	pop	{r0}
	bx	r0
