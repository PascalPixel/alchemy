@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a4800
	.thumb_func
Func_080a4800:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #1
	mov	sl, r3
	ldr	r3, [pc, #260]
	ldr	r3, [r3, #0]
	mov	r8, r3
	movs	r3, #135
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r3, [r3, #0]
	movs	r5, #13
	sub	sp, #4
	strb	r5, [r3, #5]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #30
	movs	r3, #10
	mov	r9, r0
	movs	r0, #0
	bl	Func_08015010
	adds	r7, r0, #0
	ldr	r0, [pc, #224]
	bl	Func_08004278
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	strb	r5, [r3, #5]
	bl	Func_080a22f4
	movs	r0, #1
	movs	r6, #0
	bl	Func_080030f8
	b.n	.L0
.L5:
	ldr	r1, [pc, #196]
	ldr	r3, [r1, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #1
	subs	r6, #1
	mov	sl, r3
.L1:
	ldr	r3, [r1, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	movs	r3, #1
	adds	r6, #1
	mov	sl, r3
.L2:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L3
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L4
	movs	r3, #0
	adds	r0, r6, #5
	movs	r1, #5
	mov	sl, r3
	bl	Func_080022fc
	mov	r1, r9
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	Func_080a4924
.L4:
	ldr	r1, [pc, #120]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L3
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	movs	r6, #1
	negs	r6, r6
.L3:
	adds	r0, r7, #0
	bl	Func_08015270
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #1
	adds	r0, r7, #0
	bl	Func_08015018
	mov	r3, r8
	ldr	r0, [r3, #16]
	bl	Func_08015270
	movs	r0, #14
	bl	Func_080a2144
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #48]
	bl	Func_080041d8
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #1
	strb	r3, [r2, #5]
	movs	r0, #13
	movs	r1, #0
	movs	r2, #17
	movs	r3, #10
	bl	Func_08015408
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x080a19a1
	.4byte 0x03001b04
	.4byte 0x03001c94
