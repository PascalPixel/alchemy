@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f7f30
	.thumb_func
Func_080f7f30:
	push	{r5, r6, lr}
	ldr	r3, [pc, #56]
	ldr	r1, [pc, #56]
	ldr	r2, [r3, #0]
	adds	r3, r2, r1
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [pc, #44]
	adds	r4, r2, r3
	subs	r3, #52
	adds	r5, r2, r1
	adds	r1, r2, r3
.L1:
	ldrb	r3, [r1, #0]
	ldr	r2, [r4, #0]
	strb	r3, [r6, r2]
	ldr	r3, [r4, #0]
	adds	r3, #1
	str	r3, [r4, #0]
	ldr	r3, [r5, #0]
	adds	r0, #1
	adds	r1, #1
	cmp	r0, r3
	bne.n	.L1
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02004c00
	.4byte 0x00004404
	.4byte 0x0000443c
