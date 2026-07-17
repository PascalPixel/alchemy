@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b1f4c
	.thumb_func
Func_080b1f4c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	ldr	r3, [pc, #336]
	ldr	r3, [r3, #0]
	adds	r7, r2, #0
	str	r3, [sp, #8]
	bl	Func_08077008
	ldr	r2, [sp, #12]
	lsls	r2, r2, #1
	str	r2, [sp, #4]
	adds	r5, r2, #0
	adds	r6, r0, #0
	adds	r5, #216
	ldrh	r3, [r6, r5]
	ldr	r2, [pc, #316]
	ands	r2, r3
	mov	sl, r2
	mov	r0, sl
	bl	Func_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #4
	ands	r3, r2
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	movs	r2, #1
	mov	fp, r3
	negs	r2, r2
	movs	r3, #0
	mov	r9, r0
	str	r3, [sp, #0]
	cmp	r7, r2
	bne.n	.L0
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r7, #1
.L0:
	ldrh	r0, [r6, r5]
	bl	Func_080b19cc
	adds	r2, r7, #0
	muls	r2, r0
	mov	r8, r2
	cmp	r2, #0
	bne.n	.L1
	mov	r0, sl
	movs	r1, #2
	bl	Func_08015120
	ldr	r0, [pc, #252]
	bl	Func_080b0574
	b.n	.L2
.L1:
	ldrh	r2, [r6, r5]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	mov	r3, r9
	ldrb	r2, [r3, #3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	mov	r0, sl
	movs	r1, #2
	bl	Func_08015120
	ldr	r0, [pc, #216]
	bl	Func_080b0574
	b.n	.L2
.L3:
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L4
	ldr	r5, [pc, #204]
	b.n	.L5
.L4:
	ldr	r3, [sp, #4]
	adds	r3, #216
	ldrh	r2, [r6, r3]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L6
	ldr	r5, [pc, #188]
	b.n	.L5
.L6:
	cmp	r7, #1
	ble.n	.L7
	ldr	r5, [pc, #184]
	b.n	.L5
.L7:
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L8
	ldr	r5, [pc, #180]
	b.n	.L5
.L8:
	ldr	r5, [pc, #180]
.L5:
	mov	r0, sl
	movs	r1, #2
	bl	Func_08015120
	mov	r0, r8
	movs	r1, #5
	bl	Func_08015120
	adds	r0, r5, #0
	bl	Func_080b0574
	movs	r0, #0
	bl	Func_080b0634
	cmp	r0, #0
	beq.n	.L9
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L10
	ldr	r3, [sp, #0]
	cmp	r3, #0
	beq.n	.L11
.L10:
	ldr	r5, [pc, #136]
	b.n	.L12
.L11:
	ldr	r5, [pc, #136]
.L12:
	adds	r0, r5, #0
	bl	Func_080b0574
	b.n	.L2
.L9:
	movs	r0, #102
	bl	Func_080f9010
	cmp	r7, #0
	ble.n	.L13
	adds	r5, r7, #0
.L14:
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #12]
	subs	r5, #1
	bl	Func_080772b0
	cmp	r5, #0
	bne.n	.L14
.L13:
	mov	r0, r8
	bl	Func_08077230
	bl	Func_080b10cc
	ldr	r2, [sp, #8]
	ldr	r1, [sp, #16]
	ldr	r0, [r2, #32]
	bl	Func_080b1dec
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L15
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L16
.L15:
	ldr	r5, [pc, #72]
	b.n	.L17
.L16:
	ldr	r5, [pc, #72]
.L17:
	adds	r0, r5, #0
	bl	Func_080b0574
.L2:
	add	sp, #20
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
	.4byte 0x000001ff
	.4byte 0x00000cac
	.4byte 0x00000cab
	.4byte 0x00000cb2
	.4byte 0x00000cb1
	.4byte 0x00000cb0
	.4byte 0x00000caf
	.4byte 0x00000cae
	.4byte 0x00000cb6
	.4byte 0x00000cb4
	.4byte 0x00000cb5
	.4byte 0x00000cb3
