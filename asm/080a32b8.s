@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a32b8
	.thumb_func
Func_080a32b8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #124]
	movs	r2, #186
	ldr	r5, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r0, [r3, #0]
	ldr	r3, [pc, #116]
	adds	r2, #167
	adds	r7, r5, r3
	adds	r2, r2, r5
	ldrb	r1, [r7, #0]
	mov	r8, r2
	ldrb	r2, [r2, #0]
	bl	Func_080a9e48
	movs	r3, #1
	adds	r6, r0, #0
	negs	r3, r3
	cmp	r6, r3
	bne.n	.L0
	movs	r0, #114
	bl	Func_080f9010
	ldr	r0, [r5, #44]
	bl	Func_08015278
	ldr	r2, [pc, #80]
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #76]
	adds	r2, r6, #0
	adds	r0, r0, r3
	adds	r1, r6, #0
	bl	Func_080a1d08
	ldr	r3, [pc, #68]
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r0, r6, #0
	b.n	.L1
.L0:
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #52]
	ands	r0, r3
	bl	Func_080aa448
	ldrb	r0, [r7, #0]
	bl	Func_08077010
	mov	r3, r8
	ldrb	r0, [r3, #0]
	bl	Func_08077010
	movs	r0, #1
.L1:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x0000021a
	.4byte 0x0000025a
	.4byte 0x00000bef
	.4byte 0x00000222
	.4byte 0x000001ff
