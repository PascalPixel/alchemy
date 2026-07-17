@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0802281c
	.thumb_func
Func_0802281c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #148]
	adds	r5, r0, #0
	movs	r1, #0
	movs	r0, #1
	sub	sp, #4
	ldr	r7, [r3, #0]
	bl	Func_080b5090
	mov	r8, r0
	lsls	r3, r0, #1
	add	r3, r8
	lsls	r3, r3, #1
	movs	r0, #29
	subs	r0, r0, r3
	movs	r3, #15
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r3, #5
	movs	r2, #25
	bl	Func_08022768
	ldrh	r3, [r5, #0]
	movs	r6, #0
	cmp	r3, #255
	beq.n	.L0
	movs	r0, #0
.L5:
	movs	r3, #88
	ldrsh	r2, [r7, r3]
	ldrh	r3, [r0, r5]
	movs	r1, #0
	b.n	.L1
.L3:
	adds	r1, #1
	cmp	r1, #3
	bgt.n	.L2
	lsls	r3, r1, #1
	adds	r3, #88
	ldrsh	r2, [r7, r3]
	ldrh	r3, [r0, r5]
.L1:
	cmp	r2, r3
	beq.n	.L2
	cmp	r2, #255
	bne.n	.L3
	movs	r1, #4
.L2:
	cmp	r1, #4
	beq.n	.L4
	mov	r3, r8
	subs	r2, r3, r1
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	movs	r0, #29
	subs	r0, r0, r3
	movs	r3, #14
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #7
	movs	r3, #5
	bl	Func_08022768
.L4:
	adds	r6, #1
	cmp	r6, #3
	bgt.n	.L0
	lsls	r3, r6, #1
	adds	r0, r3, #0
	ldrh	r3, [r0, r5]
	cmp	r3, #255
	bne.n	.L5
.L0:
	movs	r0, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e74
