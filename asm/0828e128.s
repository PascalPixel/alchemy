@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0828e128
	.thumb_func
Func_0828e128:
	push	{r0, r2, r3, r4, lr}
	ldrb	r6, [r3, #4]
	asrs	r3, r7, #7
	pop	{r0, r1, r2, r7, pc}
