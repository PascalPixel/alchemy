@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08099340
	.thumb_func
Func_08099340:
	push	{r5, r6, lr}
	ldr	r3, [pc, #100]
	adds	r6, r0, #0
	sub	sp, #12
	ldr	r1, [r3, #0]
	cmp	r6, #0
	beq.n	.L0
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	subs	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	cmp	r2, #0
	beq.n	.L1
	ldr	r3, [r1, #4]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r0, #160
	ldr	r3, [r1, #8]
	lsls	r0, r0, #12
	adds	r3, r3, r0
	str	r3, [r5, #4]
	ldr	r3, [r1, #12]
	str	r3, [r5, #8]
	adds	r3, r6, #0
	adds	r3, #102
	movs	r4, #0
	ldrsh	r1, [r3, r4]
	lsls	r3, r2, #11
	lsls	r0, r2, #16
	adds	r1, r1, r3
	adds	r2, r5, #0
	bl	Func_0800447c
	ldr	r3, [r5, #0]
	str	r3, [r6, #8]
	ldr	r3, [r5, #4]
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	b.n	.L0
.L1:
	ldr	r1, [pc, #20]
	adds	r0, r6, #0
	bl	Func_08009098
.L0:
	add	sp, #12
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
	.4byte 0x0809f0b0
