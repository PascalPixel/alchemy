@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b3210
	.thumb_func
Func_080b3210:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #92]
	ldr	r1, [pc, #96]
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #96]
	ldrsb	r0, [r3, r0]
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #0
	sub	sp, #4
	mov	sl, r0
	movs	r7, #0
	cmp	r2, r3
	bge.n	.L0
	adds	r3, r6, #2
	movs	r5, #219
	mov	r8, r3
	lsls	r5, r5, #2
.L2:
	mov	r1, r8
	ldrsh	r0, [r1, r5]
	str	r2, [sp, #0]
	bl	Func_08077008
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r2, [sp, #0]
	cmp	r3, #0
	beq.n	.L1
	adds	r2, #1
.L1:
	ldr	r1, [pc, #36]
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r7, #1
	adds	r5, #2
	cmp	r7, r3
	blt.n	.L2
.L0:
	mov	r0, sl
	muls	r0, r2
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x000003a7
	.4byte 0x080b4ab6
