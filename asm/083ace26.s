@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_083ace26
	.thumb_func
Func_083ace26:
	push	{r2, r3, r4, r5, lr}
	mvns	r4, r0
	cmp	r5, #187
	add	r6, sp, #436
	ldmia	r4, {r1, r2, r4, r6, r7}
	pop	{r2, r3, r4, pc}
