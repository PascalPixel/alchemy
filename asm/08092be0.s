@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08092be0
	.thumb_func
Func_08092be0:
	push	{r5, lr}
	ldr	r3, [pc, #88]
	ldr	r4, [r3, #0]
	ldr	r2, [r4, #52]
	movs	r5, #1
	negs	r5, r5
	movs	r1, #8
	cmp	r2, #0
	beq.n	.L0
	adds	r3, r2, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L0
	ldr	r3, [r2, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, r0
	bne.n	.L0
	movs	r5, #8
	b.n	.L1
.L0:
	adds	r1, #1
	cmp	r1, #65
	bgt.n	.L1
	lsls	r3, r1, #2
	adds	r3, #20
	ldr	r2, [r4, r3]
	cmp	r2, #0
	beq.n	.L0
	adds	r3, r2, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L0
	ldr	r3, [r2, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, r0
	bne.n	.L0
	adds	r5, r1, #0
.L1:
	adds	r0, r5, #0
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x03001ebc
