@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a23f4
	.thumb_func
Func_080a23f4:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	strh	r3, [r0, #8]
	ldr	r3, [sp, #4]
	strh	r1, [r0, #12]
	strh	r3, [r0, #10]
	strh	r2, [r0, #14]
.L0:
	pop	{r0}
	bx	r0
