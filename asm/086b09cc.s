@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_086b09cc
	.thumb_func
Func_086b09cc:
	push	{r4, r6, lr}
	strh	r5, [r5, #48]
	cmp	r6, #96
	add	r0, pc, #540
	ldr	r6, [sp, #432]
	strb	r4, [r4, #19]
	pop	{r2, r5, pc}
