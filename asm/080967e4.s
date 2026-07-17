@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080967e4
	.thumb_func
Func_080967e4:
	push	{lr}
	cmp	r0, #0
	bne.n	.L0
	bl	Func_08096810
	b.n	.L1
.L0:
	cmp	r0, #1
	bne.n	.L2
	bl	Func_08096960
	b.n	.L1
.L2:
	cmp	r0, #2
	bne.n	.L3
	bl	Func_08096ab0
	b.n	.L1
.L3:
	cmp	r0, #3
	bne.n	.L1
	bl	Func_08096af0
.L1:
	pop	{r0}
	bx	r0
