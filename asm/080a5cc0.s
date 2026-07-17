@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a5cc0
	.thumb_func
Func_080a5cc0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	movs	r5, #0
	str	r0, [sp, #8]
	str	r2, [sp, #4]
	str	r5, [sp, #0]
	ldr	r3, [pc, #360]
	ldr	r7, [r3, #0]
	mov	fp, r5
	b.n	.L0
.L27:
	cmp	r5, #4
	bls.n	.L1
	b.n	.L2
.L1:
	ldr	r2, [pc, #352]
	lsls	r3, r5, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080a5d04
	.4byte 0x080a5d34
	.4byte 0x080a5e22
	.4byte 0x080a5dfa
	.4byte 0x080a5e92
	movs	r3, #186
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [pc, #316]
	movs	r0, #0
	bl	Func_080a3cf8
	movs	r0, #0
	bl	Func_080a602c
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L3
	movs	r2, #1
	str	r2, [sp, #0]
	mov	fp, r3
.L3:
	ldr	r0, [r7, #44]
	bl	Func_08015270
	movs	r5, #1
	b.n	.L0
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [pc, #276]
	adds	r3, r7, r2
	ldrb	r0, [r3, #0]
	bl	Func_08077008
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L4
	b.n	.L0
.L4:
	movs	r2, #154
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L5
	cmp	r3, #1
	bgt.n	.L6
	cmp	r3, #0
	beq.n	.L7
	b.n	.L8
.L6:
	cmp	r3, #2
	beq.n	.L9
	b.n	.L8
.L7:
	ldr	r1, [pc, #224]
	b.n	.L10
.L5:
	ldr	r1, [pc, #224]
.L10:
	movs	r0, #0
	bl	Func_080a3cf8
	b.n	.L8
.L9:
	ldr	r1, [pc, #220]
	movs	r0, #0
	bl	Func_080a3cf8
.L8:
	bl	Func_080a9cbc
	ldr	r3, [pc, #196]
	adds	r6, r7, r3
	ldrb	r1, [r6, #0]
	movs	r2, #0
	ldr	r0, [r7, #36]
	movs	r3, #0
	bl	Func_080a112c
	movs	r0, #0
	bl	Func_080a6ccc
	movs	r2, #1
	negs	r2, r2
	adds	r1, r0, #0
	mov	r8, r2
	movs	r5, #0
	cmp	r1, r8
	bne.n	.L11
	b.n	.L0
.L11:
	movs	r2, #154
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	movs	r5, #2
	cmp	r3, #0
	bne.n	.L12
	b.n	.L0
.L12:
	cmp	r3, #1
	bne.n	.L13
	movs	r2, #0
	ldrb	r0, [r6, #0]
	bl	Func_080a65e4
	ldr	r0, [r7, #44]
	bl	Func_08015278
	ldr	r0, [pc, #140]
	mov	r1, r8
	mov	r2, r8
	bl	Func_080a1d08
	b.n	.L14
.L13:
	movs	r2, #1
	ldrb	r0, [r6, #0]
	bl	Func_080a65e4
	ldr	r0, [r7, #44]
	bl	Func_08015278
	ldr	r0, [pc, #116]
	mov	r1, r8
	mov	r2, r8
	bl	Func_080a1d08
.L14:
	movs	r5, #0
	b.n	.L0
	ldr	r1, [pc, #108]
	movs	r0, #0
	bl	Func_080a3cf8
	movs	r0, #0
	bl	Func_080a63e4
	movs	r3, #1
	mov	sl, r0
	negs	r3, r3
	movs	r5, #4
	cmp	sl, r3
	beq.n	.L15
	b.n	.L0
.L15:
	movs	r2, #136
	lsls	r2, r2, #2
	adds	r1, r7, r2
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #32]
	b.n	.L16
	bl	Func_080a5fe0
	cmp	r0, #1
	bne.n	.L17
.L25:
	movs	r5, #3
	b.n	.L0
.L17:
	cmp	r0, #2
	bne.n	.L18
	ldr	r3, [pc, #56]
	adds	r2, r7, r3
	movs	r3, #9
	strb	r3, [r2, #0]
	movs	r5, #4
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x03001f2c
	.4byte 0x080a5cf0
	.4byte 0x00000ae9
	.4byte 0x0000021a
	.4byte 0x00000aea
	.4byte 0x00000af1
	.4byte 0x00000af0
	.4byte 0x00000ae2
	.4byte 0x00000ae3
	.4byte 0x00000aeb
	.4byte 0x0000021b
.L18:
	movs	r2, #1
	str	r2, [sp, #0]
	mov	fp, r2
	ldr	r2, [pc, #272]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	ldr	r2, [sp, #8]
	str	r3, [r2, #0]
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #256]
	ands	r3, r2
	ldr	r2, [sp, #4]
	str	r3, [r2, #0]
	b.n	.L0
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r2, r2, r7
	movs	r3, #0
	ldrh	r0, [r2, #0]
	mov	sl, r3
	mov	r8, r2
	ldr	r3, [pc, #228]
	ldr	r2, [pc, #236]
	adds	r5, r7, r3
	adds	r6, r7, r2
	movs	r3, #0
	ldrb	r1, [r5, #0]
	ldrb	r2, [r6, #0]
	bl	Func_080a9f10
	ldrb	r3, [r6, #0]
	mov	fp, r0
	cmp	r3, #9
	bne.n	.L19
	ldrb	r3, [r5, #0]
	strb	r3, [r6, #0]
	movs	r3, #9
	mov	sl, r3
.L19:
	movs	r2, #1
	negs	r2, r2
	mov	r9, r2
	cmp	fp, r9
	beq.n	.L20
	mov	r2, r8
	ldrh	r3, [r2, #0]
	ldr	r0, [pc, #184]
	ands	r0, r3
	bl	Func_08077080
	ldrb	r3, [r5, #0]
	ldrb	r1, [r0, #9]
	adds	r0, r3, #0
	negs	r1, r1
	bl	Func_08077120
.L20:
	ldrb	r0, [r5, #0]
	bl	Func_08077010
	cmp	fp, r9
	beq.n	.L21
	ldrb	r1, [r6, #0]
	ldr	r0, [r7, #36]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080a112c
	mov	r2, r8
	ldrh	r3, [r2, #0]
	ldr	r0, [pc, #140]
	ands	r0, r3
	bl	Func_080aa460
	ldr	r0, [r7, #44]
	bl	Func_08015278
	ldr	r2, [pc, #132]
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #128]
	movs	r1, #0
	adds	r0, r0, r3
	mov	r2, r9
	bl	Func_080a1d08
	b.n	.L22
.L21:
	movs	r0, #114
	bl	Func_080f9010
	ldr	r0, [r7, #44]
	bl	Func_08015278
	ldr	r2, [pc, #100]
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #96]
	mov	r1, fp
	adds	r0, r0, r3
	mov	r2, fp
	bl	Func_080a1d08
.L22:
	movs	r3, #1
	negs	r3, r3
	cmp	fp, r3
	beq.n	.L23
	movs	r3, #136
	lsls	r3, r3, #2
	adds	r1, r7, r3
	movs	r2, #1
	mov	fp, r2
	ldr	r3, [pc, #44]
	ldrh	r2, [r1, #0]
.L16:
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r5, #1
	b.n	.L0
.L23:
	ldr	r3, [pc, #56]
	adds	r2, r7, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	mov	r2, sl
	ldr	r1, [pc, #24]
	cmp	r2, #9
	beq.n	.L24
	b.n	.L25
.L24:
	movs	r3, #136
	lsls	r3, r3, #2
	adds	r2, r7, r3
	ldrh	r3, [r2, #0]
	orrs	r3, r1
	strh	r3, [r2, #0]
	movs	r5, #1
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x0000021a
	.4byte 0x00003fff
	.4byte 0x0000021b
	.4byte 0x0000025a
	.4byte 0x00000bef
	.4byte 0x00000222
.L2:
	movs	r2, #1
	str	r2, [sp, #0]
.L0:
	ldr	r3, [sp, #0]
	cmp	r3, #0
	bne.n	.L26
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L26
	b.n	.L27
.L26:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L28
	movs	r2, #1
	negs	r2, r2
	mov	fp, r2
.L28:
	mov	r0, fp
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
