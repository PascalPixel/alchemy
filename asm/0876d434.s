@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0876d434
	.thumb_func
Func_0876d434:
	push	{r0, r6, r7, lr}
	lsls	r0, r0, #29
	str	r0, [sp, #912]
	movs	r3, #192
	bx	r1
