@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a355c
	.thumb_func
Func_080a355c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #136]
	mov	sl, r0
	ldr	r7, [r3, #0]
	mov	r6, sl
	movs	r1, #0
	adds	r6, #28
	ldr	r0, [r7, #44]
	mov	r8, r1
	ldrsb	r5, [r7, r6]
	bl	Func_08015278
	ldr	r1, [pc, #120]
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	adds	r2, r7, #2
	strb	r3, [r2, r6]
	movs	r2, #1
	negs	r2, r2
	cmp	r5, r2
	bne.n	.L0
	mov	r3, r8
	strb	r3, [r7, r6]
	movs	r6, #0
	b.n	.L1
.L0:
	lsls	r6, r5, #1
	adds	r0, r6, r5
	lsls	r0, r0, #3
	subs	r0, #10
	movs	r1, #16
	bl	Func_080a1ac0
.L1:
	movs	r5, #130
	lsls	r5, r5, #2
	adds	r3, r6, r5
	ldrh	r0, [r7, r3]
	bl	Func_08077008
	movs	r1, #228
	lsls	r1, r1, #1
	adds	r6, r7, r1
	adds	r1, r6, #0
	movs	r2, #0
	bl	Func_080a3ddc
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r3, r7, r2
	adds	r5, r7, r5
	strb	r0, [r3, #0]
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	Func_080a35f8
	mov	r1, sl
	lsls	r3, r1, #2
	adds	r3, #20
	mov	r8, r0
	ldr	r0, [r7, r3]
	bl	Func_080a17c4
	movs	r0, #1
	bl	Func_080030f8
	mov	r0, r8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x00000219
