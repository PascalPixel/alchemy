@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08300806
	.thumb_func
Func_08300806:
	push	{r0, r1, r4, lr}
	movs	r1, #191
	pop	{r4, pc}
