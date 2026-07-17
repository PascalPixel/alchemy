@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080aaf58
	.thumb_func
Func_080aaf58:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #80]
	ldr	r3, [r3, #0]
	mov	r8, r3
	ldr	r3, [pc, #76]
	add	r3, r8
	ldrb	r3, [r3, #0]
	movs	r4, #0
	sub	sp, #4
	cmp	r4, r3
	bge.n	.L0
	movs	r6, #130
	adds	r7, r0, #0
	lsls	r6, r6, #2
	adds	r7, #160
	add	r6, r8
	adds	r5, r0, #0
.L1:
	movs	r2, #1
	ldrh	r1, [r6, #0]
	adds	r0, r5, #0
	negs	r2, r2
	str	r4, [sp, #0]
	bl	Func_080ac8fc
	ldr	r3, [pc, #36]
	ldr	r4, [sp, #0]
	add	r3, r8
	ldrb	r3, [r3, #0]
	adds	r4, #1
	strb	r0, [r7, #0]
	adds	r6, #2
	adds	r7, #1
	adds	r5, #20
	cmp	r4, r3
	blt.n	.L1
.L0:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x00000219
