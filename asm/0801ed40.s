@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801ed40
	.thumb_func
Func_0801ed40:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #116]
	adds	r6, r0, #0
	movs	r0, #32
	sub	sp, #16
	adds	r5, r1, #0
	mov	r8, r2
	ldr	r7, [r3, #0]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L0
	cmp	r5, #0
	bne.n	.L1
	movs	r5, #18
.L1:
	cmp	r5, #1
	bne.n	.L0
	movs	r5, #19
.L0:
	adds	r0, r5, #0
	bl	Func_08019d2c
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L2
	cmp	r6, #1
	bls.n	.L3
	ldr	r2, [pc, #68]
	adds	r3, r7, r2
	ldrh	r3, [r3, #0]
	movs	r6, #1
	cmp	r3, r0
	beq.n	.L3
	ldr	r2, [pc, #60]
	adds	r3, r7, r2
	ldrh	r3, [r3, #0]
	cmp	r3, r0
	bne.n	.L2
	movs	r6, #0
.L3:
	ldr	r2, [pc, #52]
	lsls	r3, r6, #1
	adds	r3, r3, r2
	adds	r1, r6, #0
	ldrh	r3, [r7, r3]
	adds	r1, #14
	str	r1, [sp, #0]
	movs	r1, #1
	str	r3, [sp, #12]
	str	r1, [sp, #4]
	add	r2, sp, #12
	add	r3, sp, #8
	mov	r1, r8
	bl	Func_0801a4fc
.L2:
	add	sp, #16
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x000012ee
	.4byte 0x000012ec
	.4byte 0x000012f0
