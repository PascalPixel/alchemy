@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08098c08
	.thumb_func
Func_08098c08:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r0, #134
	sub	sp, #12
	bl	Func_080f9010
	ldr	r1, [r5, #8]
	mov	r6, sp
	str	r1, [r6, #0]
	ldr	r2, [r5, #12]
	str	r2, [r6, #4]
	ldr	r4, [pc, #144]
	ldr	r3, [r5, #16]
	ldr	r0, [pc, #144]
	str	r3, [r6, #8]
	adds	r2, r2, r4
	bl	Func_08096c80
	cmp	r0, #0
	beq.n	.L0
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #9
	movs	r3, #20
	strh	r3, [r2, #0]
	ldr	r1, [pc, #120]
	bl	Func_08009098
.L0:
	movs	r0, #128
	lsls	r0, r0, #9
	mov	r8, r6
	movs	r7, #11
	mov	sl, r0
.L2:
	mov	r3, r8
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	ldr	r0, [pc, #104]
	ldr	r3, [r3, #8]
	bl	Func_08096c80
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L1
	ldr	r1, [pc, #92]
	bl	Func_08009098
	bl	Func_08004458
	adds	r2, r6, #0
	adds	r2, #85
	mov	r4, sl
	movs	r3, #0
	add	r0, sl
	str	r4, [r6, #52]
	str	r0, [r6, #48]
	strb	r3, [r2, #0]
	bl	Func_08004458
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r0, #128
	lsls	r0, r0, #12
	lsls	r5, r5, #3
	adds	r5, r5, r0
	bl	Func_08004458
	adds	r1, r5, #0
	adds	r2, r0, #0
	adds	r0, r6, #0
	bl	Func_08096bec
.L1:
	subs	r7, #1
	cmp	r7, #0
	bge.n	.L2
	movs	r0, #0
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0xffe00000
	.4byte 0x0000011b
	.4byte 0x0809f0b0
	.4byte 0x0000011d
	.4byte 0x0809f0d4
