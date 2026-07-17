@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800c0f4
	.thumb_func
Func_0800c0f4:
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	sub	sp, #4
	cmp	r7, #0
	beq.n	.L0
	adds	r3, r7, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	movs	r2, #15
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L1
	cmp	r2, #2
	beq.n	.L2
	b.n	.L3
.L1:
	ldr	r0, [r7, #80]
	bl	Func_0800bdd4
	b.n	.L3
.L2:
	ldr	r5, [r7, #80]
	movs	r6, #3
.L5:
	ldmia	r5!, {r0}
	cmp	r0, #0
	beq.n	.L4
	bl	Func_0800bdd4
.L4:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L5
.L3:
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	adds	r1, r7, #0
	ldr	r3, [pc, #16]
	ldr	r2, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L0:
	add	sp, #4
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0x8500001c
