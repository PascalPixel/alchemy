@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080bf2b4
	.thumb_func
Func_080bf2b4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	bl	Func_08077008
	movs	r2, #154
	lsls	r2, r2, #1
	adds	r1, r0, #0
	adds	r5, r1, r2
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r3, #255
	strb	r3, [r5, #0]
	lsls	r3, r3, #24
	movs	r7, #0
	cmp	r3, #0
	bne.n	.L1
	ldr	r2, [pc, #52]
	adds	r3, r1, r2
	strb	r7, [r3, #0]
	movs	r0, #1
	b.n	.L2
.L1:
	ldr	r3, [pc, #44]
	adds	r6, r1, r3
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bge.n	.L0
	ldrb	r1, [r5, #0]
	mov	r0, r8
	movs	r2, #20
	bl	Func_080bf208
	cmp	r0, #0
	beq.n	.L0
	strb	r7, [r6, #0]
	movs	r0, #1
	strb	r7, [r5, #0]
	b.n	.L2
.L0:
	movs	r0, #0
.L2:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000135
