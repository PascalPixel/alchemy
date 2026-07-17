@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_081e1e34
	.thumb_func
Func_081e1e34:
	push	{r1, r3, r4, r6, r7, lr}
	adds	r4, #211
	cmp	r5, #94
	movs	r4, #36
	adds	r4, #94
	lsrs	r4, r4, #20
	pop	{r0, r1, r3, r4, r5, r6, r7, pc}
