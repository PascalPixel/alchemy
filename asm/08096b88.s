@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08096b88
	.thumb_func
Func_08096b88:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	cmp	r2, #1
	bne.n	.L0
	ldr	r0, [r0, #80]
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L0
	ldrb	r3, [r0, #29]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L0
	mov	r3, r8
	adds	r3, #39
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L1
	ldr	r1, [pc, #48]
	mov	r7, r8
	mov	sl, r1
	adds	r7, #40
	adds	r6, r3, #0
.L2:
	mov	r2, sl
	ldr	r0, [r2, #0]
	movs	r1, #6
	bl	Func_08002304
	ldmia	r7!, {r5}
	subs	r6, #1
	strb	r0, [r5, #5]
	cmp	r6, #0
	bne.n	.L2
.L1:
	mov	r2, r8
	adds	r2, #37
	movs	r3, #1
	strb	r3, [r2, #0]
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e40
