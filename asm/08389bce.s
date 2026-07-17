@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08389bce
	.thumb_func
Func_08389bce:
	push	{r1, r2, r3, r5, lr}
	asrs	r3, r0, #16
	ldr	r5, [sp, #980]
	pop	{r0, r1, r3, pc}
