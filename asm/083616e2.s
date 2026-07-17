@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_083616e2
	.thumb_func
Func_083616e2:
	push	{r0, r2, r3, r4, r5, r6, r7, lr}
	lsls	r4, r5, #1
	movs	r5, r2
	pop	{r0, r3, r5, r6, pc}
