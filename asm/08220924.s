@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08220924
	.thumb_func
Func_08220924:
	push	{r2, r5, r6, lr}
	str	r5, [r7, #72]
	add	pc, r4
