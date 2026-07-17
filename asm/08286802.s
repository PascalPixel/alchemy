@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08286802
	.thumb_func
Func_08286802:
	push	{r0, r6, r7, lr}
	pop	{r1, r4, r5, pc}
