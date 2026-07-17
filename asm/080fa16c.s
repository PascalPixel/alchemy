@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_080fa16c
	.thumb_func
Func_080fa16c:
	push	{r4, r5}
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	cmp	r3, #128
	bcs.n	.L0
	strb	r3, [r1, #5]
	adds	r2, #1
	str	r2, [r1, #64]
	b.n	.L1
.L0:
	ldrb	r3, [r1, #5]
.L1:
	ldr	r1, [r1, #32]
	cmp	r1, #0
	beq.n	.L2
	movs	r4, #131
	movs	r5, #64
.L4:
	ldrb	r2, [r1, #0]
	tst	r2, r4
	beq.n	.L3
	tst	r2, r5
	bne.n	.L3
	ldrb	r0, [r1, #17]
	cmp	r0, r3
	bne.n	.L3
	movs	r0, #64
	orrs	r2, r0
	strb	r2, [r1, #0]
	b.n	.L2
.L3:
	ldr	r1, [r1, #52]
	cmp	r1, #0
	bne.n	.L4
.L2:
	pop	{r4, r5}
	bx	lr
