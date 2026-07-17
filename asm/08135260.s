@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08135260
	.thumb_func
Func_08135260:
	push	{r0, r1, r4, r5, r7, lr}
	stmia	r3!, {r0, r1, r3, r4, r5, r7}
	pop	{r0, r1, r2, r6, r7, pc}
