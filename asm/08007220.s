@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08007220
	.thumb_func
Func_08007220:
	push	{r4, r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #64
	adds	r7, r1, #0
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #15
	bls.n	.L0
	ldr	r0, [pc, #4]
	b.n	.L1
	movs	r0, r0
	.4byte 0x000080ff
.L0:
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r2, [pc, #32]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #32]
	ands	r0, r1
	ldr	r1, [pc, #32]
	ldrh	r1, [r1, #36]
	orrs	r0, r1
	strh	r0, [r2, #0]
	lsls	r0, r4, #21
	lsrs	r5, r0, #16
	ldr	r1, [pc, #24]
	ldr	r0, [pc, #24]
	ldr	r0, [r0, #24]
	strh	r0, [r1, #0]
	adds	r0, r1, #0
	mov	r8, r0
	b.n	.L2
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x08007c10
	.4byte 0x02004c0c
	.4byte 0x08007be4
.L7:
	ldr	r0, [pc, #32]
	ldr	r1, [r0, #24]
	mov	r2, r8
	ldrh	r2, [r2, #0]
	subs	r0, r2, r1
	mov	r3, r8
	strh	r0, [r3, #0]
	adds	r7, r7, r1
	adds	r0, r5, #1
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
.L2:
	mov	r1, r8
	ldrh	r0, [r1, #0]
	cmp	r0, #0
	beq.n	.L3
	movs	r4, #2
	b.n	.L4
	movs	r0, r0
	.4byte 0x08007c10
.L6:
	subs	r0, r4, #1
	lsls	r0, r0, #16
	.4byte 0x2c000c04
	beq.n	.L5
.L4:
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	Func_080071a8
	lsls	r0, r0, #16
	lsrs	r6, r0, #16
	cmp	r6, #0
	bne.n	.L6
.L5:
	cmp	r6, #0
	beq.n	.L7
.L3:
	ldr	r2, [pc, #28]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #28]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	adds	r0, r6, #0
.L1:
	add	sp, #64
	pop	{r3}
	mov	r8, r3
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
