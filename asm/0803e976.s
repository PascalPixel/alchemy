@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0803e976
	.thumb_func
Func_0803e976:
	push	{r0, r1, r2, r4, r5, r6, lr}
	stmia	r1!, {r0, r3, r4, r6}
	pop	{r3, r7, pc}
