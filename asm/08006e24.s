@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08006e24
	.thumb_func
Func_08006e24:
	push	{r4, r5, r6, r7, lr}
	mov	r7, r9
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #96
	mov	r9, r1
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	mov	r8, r0
	cmp	r0, #15
	bls.n	.L0
	ldr	r0, [pc, #4]
	b.n	Func_08006f32
	movs	r0, r0
	.4byte 0x000080ff
.L0:
	ldr	r0, [pc, #28]
	ldr	r0, [r0, #0]
	ldrb	r0, [r0, #8]
	mov	r7, r8
	lsls	r7, r0
	movs	r0, #224
	lsls	r0, r0, #20
	adds	r7, r7, r0
	ldr	r1, [pc, #16]
	movs	r0, #1
	adds	r3, r1, #0
	eors	r3, r0
	mov	r2, sp
	ldr	r0, [pc, #12]
	subs	r0, r0, r1
	b.n	.L1
	.4byte 0x02004c08
	.4byte 0x08006f49
	.4byte 0x08006f6d
.L2:
	ldrh	r0, [r3, #0]
	strh	r0, [r2, #0]
	adds	r3, #2
	adds	r2, #2
	subs	r0, r1, #2
.L1:
	lsls	r0, r0, #16
	lsrs	r1, r0, #16
	cmp	r1, #0
	bne.n	.L2
	movs	r4, #0
	b.n	.L3
.L4:
	adds	r0, r4, #1
	.4byte 0x0e040600
	cmp	r4, #81
	beq.n	Func_08006f30
.L3:
	mov	r0, r8
	bl	Func_08006d50
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r5, #0
	bne.n	.L4
	adds	r0, r7, #0
	mov	r1, sp
	adds	r1, #1
	bl	Func_08006f6c
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r5, #0
	bne.n	.L4
	movs	r6, #1
	cmp	r4, #0
	beq.n	.L5
	movs	r6, #6
.L5:
	movs	r4, #1
	cmp	r4, r6
	bhi.n	.L6
.L7:
	mov	r0, r8
	bl	Func_08006d50
	adds	r0, r4, #1
	lsls	r0, r0, #24
	lsrs	r4, r0, #24
	cmp	r4, r6
	bls.n	.L7
.L6:
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r3, [pc, #24]
	ldrh	r1, [r3, #0]
	ldr	r0, [pc, #24]
	ands	r1, r0
	ldr	r0, [pc, #24]
	ldr	r2, [r0, #0]
	ldrh	r0, [r2, #16]
	orrs	r0, r1
	strh	r0, [r3, #0]
	ldr	r1, [pc, #20]
	ldr	r0, [r2, #4]
	strh	r0, [r1, #0]
	adds	r4, r1, #0
	b.n	.L8
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x02004c08
	.4byte 0x02004c0c
.L10:
	ldrh	r0, [r4, #0]
	subs	r0, #1
	strh	r0, [r4, #0]
	movs	r0, #1
	add	r9, r0
	adds	r7, #1
.L8:
	ldrh	r0, [r4, #0]
	cmp	r0, #0
	beq.n	.L9
	mov	r0, r9
	adds	r1, r7, #0
	bl	Func_08006dec
	lsls	r0, r0, #16
	.4byte 0x2d000c05
	beq.n	.L10
.L9:
	ldr	r2, [pc, #28]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #28]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	.4byte 0xb0181c28
	pop	{r3, r4}
	mov	r8, r3
	mov	r9, r4
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x04000204
	.4byte 0x0000fffc
