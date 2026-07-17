@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800ea54
	.thumb_func
Func_0800ea54:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	str	r1, [r0, #108]
.L0:
	pop	{r0}
	bx	r0
