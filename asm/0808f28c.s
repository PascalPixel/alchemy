@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808f28c
	.thumb_func
Func_0808f28c:
	push	{r5, r6, lr}
	sub	sp, #12
	adds	r5, r0, #0
	bl	Func_08004458
	movs	r3, #100
	muls	r3, r0
	lsrs	r3, r3, #16
	cmp	r3, #9
	bhi.n	.L0
	ldr	r3, [r5, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	lsls	r5, r5, #4
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	Func_0800447c
	ldr	r0, [pc, #52]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl	Func_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	ldr	r1, [pc, #40]
	bl	Func_08009098
	movs	r1, #0
	adds	r0, r5, #0
	bl	Func_08009080
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
.L0:
	add	sp, #12
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x0000011d
	.4byte 0x0809e87c
