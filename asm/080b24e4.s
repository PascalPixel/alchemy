@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b24e4
	.thumb_func
Func_080b24e4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r1, [sp, #8]
	ldr	r3, [pc, #416]
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #416]
	adds	r3, r3, r6
	ldrh	r4, [r3, #0]
	mov	r9, r0
	movs	r0, #1
	ldr	r1, [pc, #408]
	mov	fp, r0
	movs	r0, #142
	str	r4, [sp, #0]
	lsls	r0, r0, #1
	mov	r8, r3
	adds	r3, r1, r0
	movs	r2, #0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	sl, r2
	ldr	r2, [pc, #388]
	lsls	r3, r3, #1
	ldrsh	r4, [r2, r3]
	movs	r2, #140
	str	r4, [sp, #4]
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	movs	r7, #0
	cmp	r4, r3
	ble.n	.L0
	b.n	.L1
.L0:
	movs	r3, #228
	mov	r4, r8
	strh	r3, [r4, #0]
	movs	r1, #2
	movs	r0, #228
	bl	Func_08015120
	ldr	r5, [pc, #356]
	adds	r0, r5, #0
	bl	Func_080b0574
	mov	r2, r8
	ldrh	r0, [r2, #0]
	movs	r1, #2
	adds	r5, #1
	bl	Func_08015120
	adds	r0, r5, #0
	bl	Func_080b0574
.L6:
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L2
	ldr	r0, [pc, #328]
	adds	r3, r6, r0
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	movs	r4, #0
	adds	r0, r7, r1
	mov	fp, r4
	bl	Func_080022fc
	movs	r3, #219
	adds	r7, r0, #0
	lsls	r1, r7, #1
	lsls	r3, r3, #2
	adds	r2, r1, r3
	adds	r3, r6, #2
	adds	r1, r1, r7
	ldrsh	r4, [r3, r2]
	lsls	r1, r1, #3
	subs	r1, #12
	mov	r0, r9
	movs	r2, #0
	mov	sl, r4
	bl	Func_080b0a6c
	movs	r3, #234
	lsls	r3, r3, #2
	adds	r2, r6, r3
	ldr	r4, [pc, #256]
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r5, r6, r4
	ldrh	r2, [r5, #0]
	mov	r0, r9
	adds	r1, r7, #0
	bl	Func_080b11c4
	ldrh	r2, [r5, #0]
	ldr	r0, [sp, #8]
	mov	r1, sl
	bl	Func_080b1470
.L2:
	ldr	r1, [pc, #248]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L3
	ldr	r0, [pc, #216]
	adds	r5, r6, r0
	ldrh	r1, [r5, #0]
	mov	r0, sl
	bl	Func_08077028
	adds	r1, r0, #0
	cmp	r1, #0
	bge.n	.L4
	movs	r0, #113
	bl	Func_080f9010
	mov	r0, sl
	movs	r1, #1
	bl	Func_08015120
	ldrh	r0, [r5, #0]
	movs	r1, #2
	bl	Func_08015120
	mov	r0, sl
	bl	Func_08077248
	cmp	r0, #15
	bne.n	.L5
	ldr	r0, [pc, #192]
	bl	Func_080b04dc
	b.n	.L6
.L5:
	ldr	r0, [pc, #188]
	bl	Func_080b04dc
	b.n	.L6
.L4:
	mov	r0, sl
	bl	Func_08077058
	movs	r0, #101
	bl	Func_080f9010
	ldr	r0, [pc, #172]
	bl	Func_080b0574
	ldrh	r1, [r5, #0]
	mov	r0, sl
	bl	Func_08077028
	ldr	r2, [sp, #4]
	negs	r0, r2
	bl	Func_08077298
	movs	r0, #1
	bl	Func_080772a0
	b.n	.L7
.L3:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	ldr	r0, [pc, #136]
	bl	Func_080b0574
	movs	r0, #113
	bl	Func_080f9010
	b.n	.L7
.L8:
	ldr	r5, [pc, #124]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L9
	movs	r0, #111
	bl	Func_080f9010
	movs	r3, #1
	subs	r7, #1
	mov	fp, r3
.L9:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L10
	movs	r0, #111
	bl	Func_080f9010
	movs	r4, #1
	adds	r7, #1
	mov	fp, r4
.L10:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L6
.L7:
	ldr	r0, [pc, #32]
	mov	r2, sp
	ldrh	r2, [r2, #0]
	adds	r3, r6, r0
	strh	r2, [r3, #0]
.L1:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x0000039e
	.4byte 0x02000240
	.4byte 0x080b4146
	.4byte 0x00000cc3
	.4byte 0x000003a7
	.4byte 0x03001c94
	.4byte 0x00000c9e
	.4byte 0x00000ca6
	.4byte 0x00000ca1
	.4byte 0x00000cc5
	.4byte 0x03001b04
