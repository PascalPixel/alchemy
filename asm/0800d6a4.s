@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800d6a4
	.thumb_func
Func_0800d6a4:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r1, #4
	ldrsh	r6, [r5, r1]
	ldr	r2, [r5, #0]
	lsls	r3, r6, #2
	adds	r3, r3, r2
	ldr	r3, [r3, #4]
	bl	Func_080072f0
	cmp	r0, #0
	beq.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	movs	r1, #4
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #4]
	cmp	r3, r6
	bne.n	.L2
	adds	r3, r2, #2
	strh	r3, [r5, #4]
.L2:
	movs	r0, #1
.L1:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
