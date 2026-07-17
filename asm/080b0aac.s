@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b0aac
	.thumb_func
Func_080b0aac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #772]
	ldr	r3, [r3, #0]
	sub	sp, #36
	movs	r0, #0
	movs	r1, #0
	str	r0, [sp, #32]
	str	r0, [sp, #16]
	mov	sl, r3
	str	r1, [r3, #32]
	movs	r5, #2
	movs	r1, #7
	movs	r3, #4
	movs	r2, #12
	movs	r0, #18
	str	r5, [sp, #0]
	bl	Func_08015010
	mov	r2, sl
	str	r0, [r2, #12]
	bl	Func_080b10cc
	movs	r0, #0
	movs	r1, #8
	movs	r2, #15
	movs	r3, #4
	str	r5, [sp, #0]
	bl	Func_08015010
	str	r0, [sp, #32]
.L31:
	movs	r5, #2
	movs	r1, #12
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	ldr	r7, [sp, #16]
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r3, #224
	str	r0, [sp, #28]
	lsls	r3, r3, #2
	add	r3, sl
	ldr	r2, [r3, #0]
	movs	r3, #18
	strb	r3, [r2, #5]
	movs	r2, #234
	lsls	r2, r2, #2
	add	r2, sl
	movs	r3, #12
	strb	r3, [r2, #0]
	movs	r0, #0
	movs	r3, #3
	movs	r1, #17
	movs	r2, #30
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r3, #1
	str	r0, [sp, #24]
	mov	fp, r3
.L10:
	ldr	r3, [pc, #656]
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r0, fp
	mov	r9, r3
	cmp	r0, #0
	beq.n	.L0
	movs	r2, #155
	lsls	r2, r2, #2
	lsls	r3, r7, #1
	add	r2, sl
	ldrsh	r5, [r3, r2]
	adds	r0, r5, #0
	bl	Func_08077018
	movs	r2, #0
	adds	r6, r0, #0
	movs	r1, #7
	adds	r0, r7, #0
	mov	fp, r2
	bl	Func_080022fc
	adds	r1, r0, #0
	lsls	r1, r1, #5
	ldr	r0, [sp, #28]
	subs	r1, #8
	movs	r2, #8
	bl	Func_080b0a6c
	movs	r2, #234
	lsls	r2, r2, #2
	movs	r3, #4
	add	r2, sl
	strb	r3, [r2, #0]
	ldr	r0, [sp, #28]
	adds	r1, r7, #0
	bl	Func_080b0fa4
	ldr	r1, [pc, #580]
	ldr	r0, [sp, #24]
	adds	r1, r5, r1
	bl	Func_080b11a4
	ldr	r0, [sp, #32]
	bl	Func_08015270
	movs	r3, #0
	ldrsh	r2, [r6, r3]
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	movs	r3, #0
	bl	Func_080b110c
.L0:
	ldr	r1, [pc, #552]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	b.n	.L2
.L1:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	b.n	.L4
.L3:
	ldr	r0, [pc, #532]
	ldr	r3, [r0, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	mov	r1, r9
	mov	r8, r7
	subs	r7, #1
	adds	r0, r7, r1
	bl	Func_080022fc
	adds	r7, r0, #0
	cmp	r8, r7
	beq.n	.L5
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	mov	fp, r2
.L5:
	ldr	r0, [pc, #492]
	ldr	r3, [r0, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L6
	mov	r1, r9
	mov	r8, r7
	adds	r7, #1
	adds	r0, r7, r1
	bl	Func_080022fc
	adds	r7, r0, #0
	cmp	r8, r7
	beq.n	.L6
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	mov	fp, r2
.L6:
	ldr	r0, [pc, #452]
	ldr	r3, [r0, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	subs	r3, r7, #7
	cmp	r3, #0
	blt.n	.L7
	movs	r1, #1
	adds	r7, r3, #0
	mov	fp, r1
.L7:
	ldr	r2, [pc, #428]
	ldr	r3, [r2, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	mov	r0, r9
	adds	r0, #6
	movs	r1, #7
	bl	Func_080022ec
	lsls	r3, r0, #3
	adds	r5, r7, #7
	subs	r3, r3, r0
	cmp	r5, r3
	bge.n	.L9
	movs	r3, #1
	adds	r7, r5, #0
	mov	fp, r3
.L9:
	mov	r0, r9
	subs	r0, #1
	cmp	r7, r0
	ble.n	.L8
	adds	r7, r0, #0
.L8:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L10
.L32:
	ldr	r0, [sp, #24]
	movs	r1, #2
	bl	Func_08015018
	movs	r1, #2
	ldr	r0, [sp, #28]
	bl	Func_08015018
	movs	r0, #1
	bl	Func_080030f8
	mov	r0, r8
	cmp	r0, #0
	beq.n	.L11
	b.n	.L12
.L11:
	ldr	r1, [sp, #16]
	movs	r2, #155
	lsls	r3, r1, #1
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r0, sl
	ldr	r5, [pc, #332]
	ldrh	r3, [r0, r3]
	add	r5, sl
	strh	r3, [r5, #0]
	ldr	r0, [pc, #328]
	bl	Func_080b04dc
	ldrh	r0, [r5, #0]
	bl	Func_08077018
	movs	r1, #1
	str	r0, [sp, #8]
	str	r1, [sp, #12]
	movs	r5, #2
	movs	r1, #14
	movs	r2, #13
	movs	r3, #3
	movs	r6, #0
	movs	r0, #0
	str	r6, [sp, #4]
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r3, #224
	str	r0, [sp, #20]
	lsls	r3, r3, #2
	add	r3, sl
	ldr	r2, [r3, #0]
	movs	r3, #4
	strb	r3, [r2, #5]
	movs	r2, #234
	lsls	r2, r2, #2
	add	r2, sl
	movs	r3, #12
	strb	r3, [r2, #0]
	mov	r2, r8
	str	r2, [sp, #0]
	ldr	r0, [sp, #20]
	movs	r1, #2
	movs	r2, #0
	movs	r3, #8
	bl	Func_080a1028
	movs	r3, #9
	movs	r0, #16
	movs	r1, #11
	movs	r2, #14
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r3, #1
	movs	r7, #0
	mov	r9, r0
	mov	fp, r3
.L19:
	ldr	r0, [sp, #4]
	cmp	r0, #0
	beq.n	.L13
	movs	r1, #0
	ldr	r0, [pc, #220]
	str	r1, [sp, #4]
	bl	Func_080b04dc
	movs	r2, #1
	mov	fp, r2
.L13:
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L14
	ldr	r3, [pc, #208]
	add	r3, sl
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	movs	r0, #0
	mov	fp, r0
	adds	r0, r7, r1
	bl	Func_080022fc
	movs	r3, #219
	adds	r7, r0, #0
	lsls	r1, r7, #1
	lsls	r3, r3, #2
	adds	r2, r1, r3
	mov	r3, sl
	adds	r1, r1, r7
	adds	r3, #2
	lsls	r1, r1, #3
	ldrsh	r6, [r3, r2]
	subs	r1, #12
	ldr	r0, [sp, #20]
	movs	r2, #0
	bl	Func_080b0a6c
	movs	r2, #234
	lsls	r2, r2, #2
	add	r2, sl
	movs	r3, #3
	ldr	r5, [pc, #144]
	strb	r3, [r2, #0]
	add	r5, sl
	ldr	r0, [sp, #20]
	ldrh	r2, [r5, #0]
	adds	r1, r7, #0
	bl	Func_080b11c4
	ldrh	r0, [r5, #0]
	bl	Func_08077238
	cmp	r0, #0
	bne.n	.L15
	ldrh	r2, [r5, #0]
	mov	r0, r9
	adds	r1, r6, #0
	bl	Func_080b1470
	b.n	.L14
.L15:
	ldrh	r2, [r5, #0]
	mov	r0, r9
	adds	r1, r6, #0
	bl	Func_080b1260
.L14:
	ldr	r1, [pc, #88]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L16
	ldr	r5, [pc, #84]
	add	r5, sl
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	bl	Func_08077028
	adds	r1, r0, #0
	cmp	r1, #0
	bge.n	.L17
	movs	r0, #113
	bl	Func_080f9010
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08015120
	ldrh	r0, [r5, #0]
	movs	r1, #2
	bl	Func_08015120
	adds	r0, r6, #0
	bl	Func_08077248
	cmp	r0, #15
	bne.n	.L18
	ldr	r0, [pc, #44]
	bl	Func_080b04dc
	b.n	.L19
.L18:
	ldr	r0, [pc, #40]
	bl	Func_080b04dc
	b.n	.L19
	.4byte 0x03001f2c
	.4byte 0x000003a6
	.4byte 0x00000075
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x0000039e
	.4byte 0x00000c9d
	.4byte 0x000003a7
	.4byte 0x00000c9e
	.4byte 0x00000ca6
.L17:
	adds	r0, r6, #0
	bl	Func_08077058
	ldr	r2, [sp, #8]
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	ldr	r2, [pc, #396]
	ldr	r2, [r2, #16]
	cmp	r3, r2
	bls.n	.L20
	b.n	.L21
.L20:
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	bl	Func_08077220
	cmp	r0, #0
	bne.n	.L22
	movs	r1, #1
	adds	r0, r6, #0
	bl	Func_08015120
	ldr	r0, [pc, #372]
	bl	Func_080b04dc
	movs	r0, #0
	bl	Func_080b0634
	movs	r3, #1
	str	r3, [sp, #4]
	cmp	r0, #0
	beq.n	.L22
	b.n	.L19
.L22:
	ldr	r5, [pc, #352]
	movs	r0, #112
	bl	Func_080f9010
	add	r5, sl
	movs	r0, #1
	bl	Func_080030f8
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	bl	Func_080b153c
	str	r0, [sp, #12]
	movs	r2, #1
	ldr	r1, [sp, #12]
	movs	r0, #1
	negs	r2, r2
	str	r0, [sp, #4]
	cmp	r1, r2
	bne.n	.L23
	b.n	.L19
.L23:
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	ldr	r2, [sp, #12]
	bl	Func_080b17e4
	mov	r1, r9
	ldr	r0, [sp, #20]
	bl	Func_080b24e4
	movs	r3, #0
	mov	r8, r3
	b.n	.L24
.L16:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L25
	ldr	r5, [pc, #280]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L26
	movs	r0, #111
	bl	Func_080f9010
	movs	r0, #1
	subs	r7, #1
	mov	fp, r0
.L26:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L27
	movs	r0, #111
	bl	Func_080f9010
	movs	r1, #1
	adds	r7, #1
	mov	fp, r1
.L27:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L19
.L24:
	bl	Func_080a1030
	mov	r0, r9
	movs	r1, #2
	bl	Func_08015018
	movs	r1, #2
	ldr	r0, [sp, #20]
	bl	Func_08015018
	movs	r0, #1
	bl	Func_080030f8
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L28
	ldr	r3, [pc, #200]
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L28
	ldr	r3, [sp, #12]
	cmp	r8, r3
	bge.n	.L29
	ldr	r6, [pc, #172]
	adds	r5, r3, #0
	add	r6, sl
.L30:
	movs	r1, #1
	ldrh	r0, [r6, #0]
	negs	r1, r1
	subs	r5, #1
	bl	Func_08077240
	cmp	r5, #0
	bne.n	.L30
.L29:
	bl	Func_080b0070
	cmp	r0, #0
	beq.n	.L12
	ldr	r3, [pc, #152]
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r0, [sp, #16]
	subs	r3, #1
	cmp	r0, r3
	ble.n	.L28
	str	r3, [sp, #16]
.L28:
	ldr	r0, [pc, #136]
	bl	Func_080b04dc
	b.n	.L31
.L25:
	movs	r0, #113
	bl	Func_080f9010
	movs	r1, #1
	negs	r1, r1
	mov	r8, r1
	b.n	.L24
.L21:
	movs	r0, #113
	bl	Func_080f9010
	ldr	r0, [pc, #112]
	bl	Func_080b0574
	movs	r2, #1
	negs	r2, r2
	mov	r8, r2
	b.n	.L24
.L4:
	movs	r0, #113
	bl	Func_080f9010
	movs	r3, #1
	negs	r3, r3
	mov	r8, r3
	b.n	.L32
.L2:
	movs	r0, #112
	str	r7, [sp, #16]
	bl	Func_080f9010
	movs	r0, #0
	mov	r8, r0
	b.n	.L32
.L12:
	ldr	r0, [sp, #32]
	movs	r1, #2
	bl	Func_08015018
	mov	r1, sl
	ldr	r0, [r1, #12]
	movs	r1, #2
	bl	Func_08015018
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #0
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000c9f
	.4byte 0x0000039e
	.4byte 0x03001b04
	.4byte 0x000003aa
	.4byte 0x000003a6
	.4byte 0x00000ca8
	.4byte 0x00000c9c
