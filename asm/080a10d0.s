@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a10d0
	.thumb_func
Func_080a10d0:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r0, [r6, #0]
	sub	sp, #4
	adds	r5, r3, #0
	ldr	r4, [sp, #20]
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L1
	movs	r0, #0
	b.n	.L2
.L1:
	bl	Func_08015270
	movs	r0, #0
	b.n	.L2
.L0:
	movs	r3, #255
	ands	r4, r3
	adds	r0, r1, #0
	ldr	r3, [sp, #16]
	adds	r1, r2, #0
	adds	r2, r5, #0
	str	r4, [sp, #0]
	bl	Func_08015010
	str	r0, [r6, #0]
	movs	r0, #1
.L2:
	add	sp, #4
	pop	{r5, r6}
	pop	{r1}
	bx	r1
