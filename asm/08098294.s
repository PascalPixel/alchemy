@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08098294
	.thumb_func
Func_08098294:
	push	{r5, r6, lr}
	ldr	r3, [pc, #64]
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	mov	ip, r3
	movs	r0, #63
.L1:
	mov	r2, ip
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L0
	mov	r3, ip
	adds	r3, #84
	ldrb	r4, [r3, #0]
	cmp	r4, #1
	bne.n	.L0
	ldr	r1, [r2, #80]
	ldr	r2, [r1, #40]
	movs	r6, #0
	ldrsh	r3, [r2, r6]
	cmp	r3, #200
	bne.n	.L0
	adds	r3, r1, #0
	adds	r3, #37
	strb	r5, [r2, #5]
	strb	r4, [r3, #0]
.L0:
	movs	r2, #112
	subs	r0, #1
	add	ip, r2
	cmp	r0, #0
	bge.n	.L1
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e64
