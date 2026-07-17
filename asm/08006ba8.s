@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08006ba8
	.thumb_func
Func_08006ba8:
	push	{r4, r5, r6, r7, lr}
	sub	sp, #128
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r7, r3, #0
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	ldr	r2, [pc, #32]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #32]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	ldr	r3, [pc, #24]
	movs	r0, #1
	eors	r3, r0
	mov	r2, sp
	ldr	r0, [pc, #20]
	ldr	r1, [pc, #16]
	subs	r0, r0, r1
	lsls	r0, r0, #15
	b.n	.L0
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x08006b85
	.4byte 0x08006ba9
.L1:
	ldrh	r0, [r3, #0]
	strh	r0, [r2, #0]
	adds	r3, #2
	adds	r2, #2
	subs	r0, r1, #1
	lsls	r0, r0, #16
.L0:
	lsrs	r1, r0, #16
	cmp	r1, #0
	bne.n	.L1
	mov	r3, sp
	adds	r3, #1
	ldr	r0, [pc, #32]
	ldrb	r0, [r0, #28]
	lsls	r4, r0
	adds	r0, r4, #0
	movs	r2, #224
	lsls	r2, r2, #20
	adds	r1, r5, r2
	adds	r0, r0, r1
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	Func_080072f0
	add	sp, #128
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x08007abc
