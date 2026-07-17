@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_081eccfc
	.thumb_func
Func_081eccfc:
	push	{r1, r4, lr}
	lsrs	r3, r3, #28
	subs	r0, #28
	asrs	r5, r7, #25
	cmp	r4, #9
	pop	{r0, r1, r2, r3, r4, r5, r6, pc}
