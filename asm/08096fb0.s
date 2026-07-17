@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08096fb0
	.thumb_func
Func_08096fb0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r0
	ldr	r0, [pc, #52]
	adds	r3, r0, #0
	subs	r3, #76
	ldr	r2, [r0, #0]
	ldr	r7, [r3, #0]
	ldr	r3, [pc, #48]
	mov	sl, r2
	add	r3, sl
	movs	r6, #0
	ldrsb	r6, [r3, r6]
	sub	sp, #4
	mov	r9, r1
	cmp	r6, #0
	bne.n	.L0
	movs	r1, #228
	lsls	r1, r1, #3
	movs	r0, #56
	bl	Func_080048b0
	adds	r5, r0, #0
	mov	r0, sp
	str	r6, [r0, #0]
	ldr	r3, [pc, #16]
	adds	r1, r5, #0
	ldr	r2, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L1
	.4byte 0x03001ebc
	.4byte 0x00000cc6
	.4byte 0x040000d4
	.4byte 0x850001c8
.L0:
	ldr	r5, [r0, #116]
.L1:
	mov	r3, r8
	strh	r3, [r5, #28]
	mov	r0, r8
	bl	Func_08077080
	ldrb	r3, [r0, #12]
	strh	r3, [r5, #30]
	ldr	r3, [pc, #80]
	add	r3, sl
	movs	r6, #0
	ldrsb	r6, [r3, r6]
	cmp	r6, #0
	bne.n	.L2
	bl	Func_08096c24
	ldr	r3, [pc, #60]
	adds	r2, r5, #0
	subs	r3, r3, r0
	adds	r2, #74
	strh	r3, [r2, #0]
	adds	r3, r5, #0
	adds	r3, #33
	mov	r4, r9
	strb	r4, [r3, #0]
	movs	r0, #1
	adds	r3, #1
	strb	r0, [r3, #0]
	ldr	r2, [pc, #44]
	subs	r3, #2
	strb	r0, [r3, #0]
	adds	r3, #3
	strb	r0, [r3, #0]
	adds	r3, r5, r2
	strb	r0, [r3, #0]
	ldr	r3, [r7, #4]
	str	r3, [r5, #76]
	ldr	r3, [r7, #8]
	str	r3, [r5, #80]
	ldr	r3, [r7, #12]
	str	r3, [r5, #84]
	ldr	r3, [pc, #24]
	mov	ip, r3
	movs	r3, #237
	lsls	r3, r3, #1
	add	r3, ip
	b.n	.L3
	movs	r0, r0
	.4byte 0x00000200
	.4byte 0x00000cc6
	.4byte 0x0000071c
	.4byte 0x02000240
.L3:
	ldrh	r1, [r3, #0]
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	ldr	r3, [pc, #108]
	cmp	r2, r3
	bne.n	.L4
	adds	r3, r5, #0
	adds	r3, #69
	strb	r0, [r3, #0]
.L4:
	lsls	r3, r1, #16
	ldr	r2, [pc, #96]
	asrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L5
	adds	r3, r5, #0
	adds	r3, #69
	strb	r0, [r3, #0]
.L5:
	movs	r3, #250
	lsls	r3, r3, #1
	add	r3, ip
	movs	r1, #1
	ldr	r0, [r3, #0]
	negs	r1, r1
	bl	Func_080970f8
	movs	r2, #30
	ldrsh	r3, [r5, r2]
	cmp	r3, #8
	beq.n	.L6
	movs	r3, #204
	lsls	r3, r3, #4
	add	r3, sl
	strh	r6, [r3, #0]
.L6:
	bl	Func_08004080
	adds	r3, r5, #0
	adds	r3, #70
	strh	r0, [r3, #0]
	movs	r1, #128
	lsls	r0, r0, #16
	lsls	r1, r1, #1
	asrs	r0, r0, #16
	ldr	r2, [pc, #36]
	bl	Func_08003fa4
	movs	r1, #200
	ldr	r0, [pc, #32]
	lsls	r1, r1, #4
	bl	Func_080041d8
.L2:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000035
	.4byte 0x00000037
	.4byte 0x0809c410
	.4byte 0x08096f8d
