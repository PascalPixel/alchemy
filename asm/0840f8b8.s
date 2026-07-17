@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0840f8b8
	.thumb_func
Func_0840f8b8:
	push	{r4, r6, lr}
	str	r1, [r3, #96]
	add	pc, r1
