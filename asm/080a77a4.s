@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a77a4
	.thumb_func
Func_080a77a4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #28
	adds	r2, r2, r0
	ldr	r3, [pc, #112]
	lsls	r0, r0, #2
	mov	sl, r0
	ldr	r5, [r3, #0]
	mov	r3, sl
	adds	r3, #20
	ldr	r0, [r5, r3]
	movs	r6, #0
	movs	r3, #1
	strb	r3, [r0, #5]
	strh	r6, [r0, #12]
	ldr	r0, [r5, #16]
	sub	sp, #4
	mov	r8, r2
	ldrsb	r7, [r5, r2]
	bl	Func_08015270
	movs	r0, #185
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #3
	ldr	r0, [r5, #16]
	movs	r1, #9
	str	r3, [sp, #0]
	movs	r2, #1
	movs	r3, #9
	bl	Func_08015070
.L0:
	movs	r3, #1
	negs	r3, r3
	cmp	r7, r3
	bne.n	.L1
	ldr	r3, [pc, #40]
	mov	r2, r8
	strb	r3, [r5, r2]
	b.n	.L2
.L1:
	lsls	r0, r7, #1
	adds	r0, r0, r7
	lsls	r0, r0, #3
	subs	r0, #10
	movs	r1, #16
	bl	Func_080a1ac0
.L2:
	movs	r2, #136
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #3
	bne.n	.L3
	bl	Func_080a7d68
	b.n	.L4
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001f2c
.L3:
	bl	Func_080a7a34
.L4:
	adds	r6, r0, #0
	mov	r3, sl
	adds	r3, #20
	ldr	r0, [r5, r3]
	bl	Func_080a17c4
	movs	r0, #1
	bl	Func_080030f8
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
