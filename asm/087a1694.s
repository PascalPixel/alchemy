@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_087a1694
	.thumb_func
Func_087a1694:
	push	{r0, r1, r6, lr}
	asrs	r3, r4, #32
	ldmia	r0!, {r1, r3, r4, r5}
	movs	r1, #231
	pop	{r0, r5, pc}
