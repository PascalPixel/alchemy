@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080217a4
	.thumb_func
Func_080217a4:
	push	{r5, lr}
	ldr	r3, [pc, #128]
	ldr	r3, [r3, #0]
	movs	r2, #7
	lsrs	r3, r3, #1
	ldr	r1, [pc, #124]
	ands	r3, r2
	lsls	r3, r3, #2
	ldr	r1, [r1, r3]
	sub	sp, #8
	adds	r5, r0, #0
	cmp	r1, #0
	bge.n	.L0
	adds	r1, #255
.L0:
	asrs	r1, r1, #8
	cmp	r5, #0
	beq.n	.L1
	ldr	r3, [sp, #0]
	ldr	r4, [pc, #100]
	lsls	r1, r1, #16
	ldr	r2, [pc, #100]
	lsrs	r1, r1, #16
	ands	r3, r4
	orrs	r3, r1
	ands	r3, r2
	lsls	r1, r1, #16
	orrs	r3, r1
	str	r3, [sp, #0]
	mov	r0, sp
	ldr	r3, [r0, #4]
	ands	r3, r4
	str	r3, [r0, #4]
	bl	Func_08003d28
	movs	r3, #31
	ldrb	r2, [r5, #23]
	ands	r0, r3
	movs	r3, #63
	negs	r3, r3
	ands	r3, r2
	lsls	r0, r0, #1
	orrs	r3, r0
	strb	r3, [r5, #23]
	ldrb	r3, [r5, #21]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r5, #21]
	ldrh	r2, [r5, #6]
	ldr	r3, [pc, #48]
	adds	r2, r2, r3
	ldr	r3, [pc, #24]
	ldrh	r1, [r5, #22]
	ands	r2, r3
	ldr	r3, [pc, #44]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #22]
	ldrb	r3, [r5, #8]
	adds	r3, #240
	strb	r3, [r5, #20]
	movs	r3, #252
	strb	r3, [r5, #15]
	b.n	.L1
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x03001800
	.4byte 0x08037230
	.4byte 0xffff0000
	.4byte 0x0000ffff
	.4byte 0x0000fff0
	.4byte 0xfffffe00
.L1:
	add	sp, #8
	pop	{r5}
	pop	{r0}
	bx	r0
