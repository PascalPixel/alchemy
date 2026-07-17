@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800412c
	.thumb_func
Func_0800412c:
	push	{lr}
	lsrs	r2, r2, #2
	movs	r4, #0
	cmp	r4, r2
	bcs.n	.L0
.L1:
	ldmia	r1!, {r3}
	adds	r4, #1
	stmia	r0!, {r3}
	cmp	r4, r2
	bcc.n	.L1
.L0:
	pop	{r0}
	bx	r0
