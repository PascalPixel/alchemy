@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_083871e8
	.thumb_func
Func_083871e8:
	push	{r6, r7, lr}
	movs	r3, #244
	lsrs	r1, r2, #4
	lsls	r0, r2, #3
	pop	{r2, r4, r6, r7, pc}
