@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080ad508
	.thumb_func
Func_080ad508:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #136]
	movs	r5, #137
	ldr	r6, [r3, #0]
	sub	sp, #4
	movs	r7, #0
	lsls	r5, r5, #2
	movs	r2, #3
.L1:
	ldr	r0, [r5, r6]
	cmp	r0, #0
	beq.n	.L0
	str	r2, [sp, #0]
	bl	Func_08009038
	str	r7, [r5, r6]
	ldr	r2, [sp, #0]
.L0:
	subs	r2, #1
	adds	r5, #4
	cmp	r2, #0
	bge.n	.L1
	ldr	r1, [pc, #104]
	movs	r3, #141
	lsls	r3, r3, #2
	mov	sl, r1
	movs	r1, #137
	adds	r7, r6, r3
	lsls	r1, r1, #2
	movs	r3, #0
	adds	r6, r6, r1
	mov	r8, r3
	movs	r2, #3
.L3:
	mov	r1, r8
	mov	r3, sl
	ldr	r0, [r1, r3]
	str	r2, [sp, #0]
	bl	Func_08009030
	adds	r5, r0, #0
	ldr	r2, [sp, #0]
	cmp	r5, #0
	beq.n	.L2
	movs	r1, #2
	bl	Func_08009020
	ldr	r2, [sp, #0]
.L2:
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #32]
	ldr	r3, [pc, #36]
	str	r5, [r6, #0]
	strh	r3, [r7, #0]
	ldr	r3, [pc, #32]
	movs	r1, #4
	subs	r2, #1
	strh	r3, [r7, #8]
	adds	r6, #4
	adds	r7, #2
	add	r8, r1
	cmp	r2, #0
	bge.n	.L3
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #24]
	bl	Func_080041d8
	add	sp, #4
	b.n	.L4
	.4byte 0x00000010
	.4byte 0x000000c8
	.4byte 0x03001f2c
	.4byte 0x080af304
	.4byte 0x080ad40d
.L4:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
