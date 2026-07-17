@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08354f04
	.thumb_func
Func_08354f04:
	push	{r0, r1, r6, r7, lr}
	movs	r3, r0
	bx	sl
