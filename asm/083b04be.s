@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_083b04be
	.thumb_func
Func_083b04be:
	push	{r2, r4, r6, lr}
	str	r2, [r4, #72]
	cmp	r6, #136
	pop	{r2, r3, r4, r5, pc}
