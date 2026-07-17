@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b04c4
	.thumb_func
Func_080b04c4:
	push	{lr}
	b.n	.L0
.L1:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	bl	Func_080f9048
	cmp	r0, #0
	bne.n	.L1
	pop	{r0}
	bx	r0
