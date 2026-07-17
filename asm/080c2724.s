@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c2724
	.thumb_func
Func_080c2724:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #660]
	movs	r0, #166
	ldr	r3, [r3, #0]
	lsls	r0, r0, #3
	adds	r0, r3, r0
	sub	sp, #56
	str	r0, [sp, #20]
	ldr	r0, [r0, #4]
	cmp	r0, #0
	beq.n	.L0
	movs	r1, #5
	bl	Func_08015120
	ldr	r0, [pc, #636]
	bl	Func_080151c8
	bl	Func_080bb65c
.L0:
	mov	r2, sp
	adds	r2, #24
	adds	r1, r2, #0
	movs	r0, #1
	str	r2, [sp, #12]
	bl	Func_080b6b40
	str	r0, [sp, #16]
	movs	r0, #166
	lsls	r0, r0, #1
	bl	Func_08004970
	mov	fp, r0
	ldr	r0, [sp, #16]
	movs	r3, #0
	str	r3, [sp, #8]
	cmp	r3, r0
	blt.n	.L1
	b.n	.L2
.L1:
	add	r2, sp, #40
	str	r3, [sp, #0]
	mov	r8, r2
.L15:
	ldr	r0, [sp, #12]
	ldr	r3, [sp, #0]
	ldrh	r3, [r3, r0]
	adds	r0, r3, #0
	str	r3, [sp, #4]
	bl	Func_08077008
	movs	r3, #146
	mov	sl, r0
	lsls	r3, r3, #1
	ldr	r0, [sp, #20]
	add	r3, sl
	ldr	r2, [r3, #0]
	ldr	r1, [r0, #4]
	adds	r2, r2, r1
	str	r2, [r3, #0]
	b.n	.L3
.L14:
	movs	r0, #89
	bl	Func_080f9010
	bl	Func_08015118
	ldr	r3, [pc, #544]
	add	r3, sl
	ldrb	r0, [r3, #0]
	movs	r1, #3
	bl	Func_08015120
	ldr	r2, [sp, #0]
	ldr	r3, [sp, #12]
	movs	r1, #1
	ldrh	r0, [r2, r3]
	bl	Func_08015120
	mov	r2, sl
	ldrb	r0, [r2, #15]
	movs	r1, #5
	bl	Func_08015120
	ldr	r0, [pc, #512]
	bl	Func_080151c8
	bl	Func_080bb65c
	ldr	r3, [pc, #508]
	mov	r6, sl
	mov	r9, r3
	adds	r6, #88
	movs	r7, #31
.L7:
	ldrh	r5, [r6, #0]
	mov	r0, r9
	adds	r3, r5, #0
	ands	r3, r0
	adds	r6, #4
	cmp	r3, #0
	beq.n	.L4
	lsrs	r3, r5, #15
	cmp	r3, #0
	beq.n	.L4
	movs	r3, #88
	mov	r2, fp
	ldrh	r3, [r2, r3]
	movs	r1, #0
	cmp	r5, r3
	beq.n	.L5
	adds	r2, #88
.L6:
	adds	r1, #1
	cmp	r1, #31
	bgt.n	.L5
	adds	r2, #4
	ldrh	r3, [r2, #0]
	cmp	r5, r3
	bne.n	.L6
.L5:
	cmp	r1, #32
	bne.n	.L4
	bl	Func_08015118
	ldr	r3, [pc, #432]
	add	r3, sl
	ldrb	r0, [r3, #0]
	movs	r1, #3
	bl	Func_08015120
	movs	r1, #1
	ldr	r0, [sp, #4]
	bl	Func_08015120
	mov	r3, r9
	ands	r5, r3
	movs	r1, #4
	adds	r0, r5, #0
	bl	Func_08015120
	movs	r0, #154
	bl	Func_080f9010
	ldr	r0, [pc, #408]
	bl	Func_080151c8
	bl	Func_080bb65c
.L4:
	subs	r7, #1
	cmp	r7, #0
	bge.n	.L7
	mov	r3, r8
	movs	r2, #4
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L8
	movs	r1, #5
	bl	Func_08015120
	ldr	r0, [pc, #380]
	bl	Func_080151c8
	bl	Func_080bb65c
.L8:
	mov	r3, r8
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L9
	movs	r1, #5
	bl	Func_08015120
	ldr	r0, [pc, #356]
	bl	Func_080151c8
	bl	Func_080bb65c
.L9:
	mov	r3, r8
	movs	r2, #8
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L10
	movs	r1, #5
	bl	Func_08015120
	ldr	r0, [pc, #336]
	bl	Func_080151c8
	bl	Func_080bb65c
.L10:
	mov	r3, r8
	movs	r2, #10
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L11
	movs	r1, #5
	bl	Func_08015120
	ldr	r0, [pc, #312]
	bl	Func_080151c8
	bl	Func_080bb65c
.L11:
	mov	r3, r8
	movs	r2, #12
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L12
	movs	r1, #5
	bl	Func_08015120
	ldr	r0, [pc, #292]
	bl	Func_080151c8
	bl	Func_080bb65c
.L12:
	mov	r3, r8
	movs	r2, #14
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L3
	movs	r1, #5
	bl	Func_08015120
	ldr	r0, [pc, #268]
	bl	Func_080151c8
	bl	Func_080bb65c
.L3:
	movs	r2, #166
	mov	r1, sl
	ldr	r3, [pc, #260]
	mov	r0, fp
	lsls	r2, r2, #1
	bl	Func_080072f0
	mov	r1, r8
	ldr	r0, [sp, #4]
	bl	Func_080770b8
	cmp	r0, #0
	beq.n	.L13
	b.n	.L14
.L13:
	ldr	r0, [sp, #0]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #16]
	adds	r0, #2
	adds	r2, #1
	str	r0, [sp, #0]
	str	r2, [sp, #8]
	cmp	r2, r3
	bge.n	.L2
	b.n	.L15
.L2:
	mov	r0, fp
	bl	Func_08002df0
	ldr	r2, [sp, #20]
	ldr	r0, [r2, #0]
	cmp	r0, #0
	beq.n	.L16
	movs	r1, #5
	bl	Func_08015120
	ldr	r0, [pc, #200]
	bl	Func_080151c8
	ldr	r3, [sp, #20]
	ldr	r0, [r3, #0]
	bl	Func_08077230
	bl	Func_080bb65c
.L16:
	ldr	r0, [pc, #184]
	mov	sl, r0
.L21:
	movs	r7, #1
	negs	r7, r7
	mov	r8, r7
	movs	r6, #0
	movs	r5, #12
.L18:
	ldr	r2, [sp, #20]
	ldrh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L17
	adds	r0, r3, #0
	bl	Func_080c2470
	cmp	r0, r8
	blt.n	.L17
	mov	r8, r0
	adds	r7, r6, #0
.L17:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, #3
	ble.n	.L18
	movs	r6, #1
	negs	r6, r6
	cmp	r7, r6
	beq.n	.L19
	lsls	r3, r7, #1
	adds	r5, r3, #0
	ldr	r3, [sp, #20]
	adds	r5, #12
	ldrh	r0, [r3, r5]
	movs	r1, #2
	bl	Func_08015120
	ldr	r0, [pc, #120]
	bl	Func_080151c8
	bl	Func_080bb65c
	ldr	r2, [sp, #20]
	ldrh	r0, [r2, r5]
	bl	Func_08077030
	cmp	r0, r6
	bne.n	.L20
	ldr	r0, [sp, #20]
	ldrh	r3, [r0, r5]
	mov	r2, sl
	strh	r3, [r2, #0]
	b.n	.L19
.L20:
	ldr	r3, [pc, #24]
	ldr	r0, [sp, #20]
	strh	r3, [r0, r5]
	b.n	.L21
.L19:
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001e74
	.4byte 0x0000083a
	.4byte 0x00000129
	.4byte 0x0000089a
	.4byte 0x00003fff
	.4byte 0x0000089b
	.4byte 0x0000089c
	.4byte 0x0000089d
	.4byte 0x0000089e
	.4byte 0x0000089f
	.4byte 0x000008a0
	.4byte 0x000008a1
	.4byte 0x03001388
	.4byte 0x0000083b
	.4byte 0x0200047c
	.4byte 0x0000083c
