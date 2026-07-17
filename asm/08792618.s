@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08792618
	.thumb_func
Func_08792618:
	push	{r0, r2, r3, r4, r5, r7, lr}
	pop	{r1, r2, r4, r5, pc}
