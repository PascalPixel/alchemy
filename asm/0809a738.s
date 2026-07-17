@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0809a738
	.thumb_func
Func_0809a738:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	bl	Func_08004458
	ldrh	r2, [r7, #6]
	movs	r1, #128
	mov	r8, r2
	lsls	r1, r1, #10
	adds	r6, r0, #0
	mov	r0, r8
	adds	r6, r6, r1
	bl	Func_0800231c
	ldr	r5, [pc, #140]
	adds	r1, r0, #0
	adds	r0, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r5
	mov	sl, r0
	mov	r0, r8
	bl	Func_08002322
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	ip, pc
	bx	r5
	ldr	r3, [r7, #8]
	add	r3, sl
	str	r3, [r7, #8]
	ldr	r3, [r7, #16]
	adds	r3, r3, r0
	str	r3, [r7, #16]
	ldr	r1, [pc, #104]
	ldrh	r3, [r7, #6]
	adds	r3, r3, r1
	strh	r3, [r7, #6]
	adds	r5, r7, #0
	adds	r5, #102
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #0]
	cmp	r3, #0
	beq.n	.L0
	subs	r3, r2, #1
	strh	r3, [r5, #0]
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	strh	r3, [r7, #6]
	b.n	.L1
.L0:
	bl	Func_08004458
	lsls	r0, r0, #5
	lsrs	r0, r0, #16
	cmp	r0, #0
	bne.n	.L1
	bl	Func_08004458
	lsls	r0, r0, #4
	lsrs	r0, r0, #16
	adds	r0, #8
	strh	r0, [r5, #0]
.L1:
	adds	r2, r7, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	movs	r1, #202
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	cmp	r3, r1
	bne.n	.L2
	ldr	r1, [pc, #28]
	adds	r0, r7, #0
	bl	Func_08009098
.L2:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03000118
	.4byte 0x0000fff0
	.4byte 0x0809f0b0
