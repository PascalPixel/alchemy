@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080175c0
	.thumb_func
Func_080175c0:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #84]
	ldr	r2, [pc, #84]
	ldr	r5, [r3, #0]
	movs	r6, #0
	adds	r3, r5, r2
	adds	r2, #2
	strh	r6, [r3, #0]
	adds	r3, r5, r2
	strh	r6, [r3, #0]
	adds	r7, r0, #0
	adds	r0, r1, #0
	movs	r1, #1
	sub	sp, #16
	bl	Func_08018038
	movs	r2, #235
	adds	r1, r0, #0
	lsls	r3, r1, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r3, [r5, r3]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L0
	cmp	r7, #0
	beq.n	.L0
	movs	r3, #1
	str	r3, [sp, #4]
	adds	r0, r7, #0
	movs	r2, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl	Func_080165d8
	adds	r6, r0, #0
	movs	r0, #0
	cmp	r6, #0
	beq.n	.L0
	adds	r0, r6, #0
.L0:
	add	sp, #16
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e8c
	.4byte 0x000012f4
