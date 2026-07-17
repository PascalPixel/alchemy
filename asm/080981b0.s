@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080981b0
	.thumb_func
Func_080981b0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	movs	r0, #154
	bl	Func_080f9010
	ldr	r5, [pc, #192]
	movs	r2, #30
	mov	r8, r2
.L0:
	ldr	r3, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r7, #12]
	movs	r2, #128
	ldrh	r3, [r7, #6]
	lsls	r2, r2, #6
	adds	r3, r3, r2
	strh	r3, [r7, #6]
	ldr	r3, [r7, #24]
	adds	r3, r3, r5
	str	r3, [r7, #24]
	ldr	r3, [r7, #28]
	adds	r3, r3, r5
	str	r3, [r7, #28]
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L0
	movs	r2, #128
	movs	r3, #7
	lsls	r2, r2, #9
	mov	r8, r3
	mov	sl, r2
.L2:
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	ldr	r0, [pc, #124]
	bl	Func_08096c80
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L1
	ldr	r1, [pc, #116]
	bl	Func_08009098
	bl	Func_08004458
	mov	r3, sl
	adds	r2, r6, #0
	adds	r2, #85
	str	r3, [r6, #52]
	add	r0, sl
	movs	r3, #2
	str	r0, [r6, #48]
	strb	r3, [r2, #0]
	ldr	r3, [pc, #96]
	str	r3, [r6, #72]
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	subs	r5, r5, r0
	str	r5, [r6, #40]
	bl	Func_08004458
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r2, #128
	lsls	r2, r2, #12
	lsls	r5, r5, #3
	adds	r5, r5, r2
	bl	Func_08004458
	adds	r1, r5, #0
	adds	r2, r0, #0
	adds	r0, r6, #0
	bl	Func_08096bec
.L1:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L2
	movs	r0, #131
	bl	Func_080f9010
	adds	r0, r7, #0
	bl	Func_080090d0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0xfffff800
	.4byte 0x0000011d
	.4byte 0x0809f0d4
	.4byte 0x00000a3d
