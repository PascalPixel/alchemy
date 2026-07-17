@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0811108c
	.thumb_func
Func_0811108c:
	push	{r4, r5, r7, lr}
	push	{r1, r2, r4, r5, r7, lr}
	pop	{r3, r4, r5, r7, pc}
