@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_083a7df4
	.thumb_func
Func_083a7df4:
	push	{r1, r7, lr}
	pop	{r1, r4, r5, r6, pc}
