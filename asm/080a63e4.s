@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a63e4
	.thumb_func
Func_080a63e4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r0, [sp, #8]
	ldr	r3, [pc, #64]
	ldr	r6, [r3, #0]
	ldr	r2, [pc, #64]
	movs	r1, #29
	ldrsb	r1, [r6, r1]
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	mov	r8, r1
	movs	r1, #0
	str	r1, [sp, #4]
	str	r1, [sp, #0]
	mov	fp, r3
	movs	r3, #1
	mov	r9, r3
	movs	r3, #28
	ldrsb	r3, [r6, r3]
	subs	r2, #17
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r0, [r6, r3]
	bl	Func_08077008
	mov	r3, r8
	lsls	r3, r3, #1
	mov	sl, r3
	mov	r0, sl
	add	r0, r8
	lsls	r0, r0, #3
	subs	r0, #10
	movs	r1, #16
	bl	Func_080a1ac0
	b.n	.L0
	.4byte 0x03001f2c
	.4byte 0x00000219
.L10:
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L1
	mov	r0, r8
	movs	r2, #0
	mov	r1, fp
	add	r0, fp
	mov	r9, r2
	bl	Func_080022fc
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #1
	movs	r7, #130
	mov	sl, r3
	lsls	r7, r7, #2
	add	r7, sl
	ldrh	r0, [r6, r7]
	bl	Func_08077008
	ldr	r3, [r6, #16]
	mov	r1, sl
	ldrh	r2, [r3, #12]
	add	r1, r8
	adds	r2, r2, r1
	ldr	r5, [r6, #24]
	ldr	r3, [pc, #28]
	lsls	r2, r2, #3
	subs	r2, #2
	strh	r2, [r5, #6]
	ands	r2, r3
	ldr	r3, [pc, #24]
	ldr	r1, [pc, #24]
	ands	r2, r3
	ldrh	r3, [r5, #22]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #22]
	ldr	r1, [sp, #8]
	cmp	r1, #0
	bne.n	.L2
	b.n	.L3
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.4byte 0xfffffe00
.L3:
	ldr	r0, [r6, #36]
	ldrh	r1, [r6, r7]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080a112c
	adds	r0, r6, #0
	ldrh	r1, [r6, r7]
	bl	Func_080a1804
	ldr	r0, [pc, #64]
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L4
	ldr	r2, [sp, #0]
	cmp	r2, #0
	bne.n	.L4
	ldr	r0, [r6, #44]
	bl	Func_08015270
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #32]
	ands	r0, r3
	ldr	r3, [pc, #36]
	movs	r2, #0
	adds	r0, r0, r3
	ldr	r1, [r6, #44]
	movs	r3, #0
	bl	Func_08015080
	movs	r2, #1
	str	r2, [sp, #0]
	b.n	.L2
.L4:
	ldr	r0, [pc, #12]
	bl	Func_080770d0
	b.n	.L2
	movs	r0, r0
	.4byte 0x00003fff
	.4byte 0x00000151
	.4byte 0x0000053a
.L1:
	mov	r3, r8
	lsls	r3, r3, #1
	mov	sl, r3
.L2:
	mov	r0, sl
	add	r0, r8
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	Func_080a1a40
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #188]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	movs	r0, #112
	bl	Func_080f9010
	movs	r3, #130
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r3, [r6, r3]
	str	r3, [sp, #4]
	b.n	.L6
.L5:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	movs	r0, #113
	bl	Func_080f9010
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #4]
	b.n	.L6
.L7:
	ldr	r5, [pc, #136]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	negs	r2, r2
	movs	r3, #1
	add	r8, r2
	mov	r9, r3
.L8:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	movs	r0, #111
	bl	Func_080f9010
	movs	r1, #1
	add	r8, r1
	mov	r9, r1
.L0:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L9
	b.n	.L10
.L9:
	mov	r2, r8
	lsls	r2, r2, #1
	mov	sl, r2
.L6:
	ldr	r5, [r6, #24]
	mov	r3, r8
	strb	r3, [r6, #29]
	adds	r0, r5, #0
	bl	Func_080a17c4
	movs	r3, #13
	strb	r3, [r5, #5]
	movs	r0, #1
	bl	Func_080030f8
	mov	r1, r8
	movs	r2, #130
	strb	r1, [r6, #29]
	lsls	r2, r2, #2
	add	r2, sl
	ldrh	r3, [r6, r2]
	str	r3, [r6, #8]
	ldr	r1, [pc, #36]
	ldrh	r2, [r6, r2]
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	ldr	r0, [sp, #4]
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x0000021b
