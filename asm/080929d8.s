@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080929d8
	.thumb_func
Func_080929d8:
	push	{lr}
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L0
	ldr	r0, [r0, #80]
	mov	ip, r0
	mov	r3, ip
	adds	r3, #39
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L1
	mov	r4, ip
	adds	r4, #40
	adds	r0, r3, #0
.L3:
	ldmia	r4!, {r2}
	cmp	r2, #0
	beq.n	.L2
	ldr	r3, [r2, #16]
	cmp	r3, #0
	beq.n	.L2
	strb	r1, [r2, #5]
.L2:
	subs	r0, #1
	cmp	r0, #0
	bne.n	.L3
.L1:
	mov	r2, ip
	adds	r2, #37
	movs	r3, #1
	strb	r3, [r2, #0]
.L0:
	pop	{r0}
	bx	r0
