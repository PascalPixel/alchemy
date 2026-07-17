@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a90bc
	.thumb_func
Func_080a90bc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #108]
	ldr	r3, [r3, #0]
	sub	sp, #44
	movs	r1, #0
	mov	r8, r3
	str	r1, [sp, #12]
	mov	fp, r1
	bl	Func_080a8c2c
	movs	r3, #134
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r0, [r3, #0]
	bl	Func_08015270
	movs	r3, #5
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r1, #0
	movs	r2, #0
	movs	r3, #30
	adds	r0, #44
	bl	Func_080a10d0
	ldr	r3, [pc, #60]
	ldr	r1, [pc, #48]
	movs	r2, #3
	add	r3, r8
.L0:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L0
	mov	r1, r8
	movs	r0, #245
	adds	r1, #72
	movs	r2, #31
.L2:
	ldmia	r1!, {r3}
	cmp	r3, #0
	beq.n	.L1
	strb	r0, [r3, #15]
.L1:
	subs	r2, #1
	cmp	r2, #0
	bge.n	.L2
	ldr	r0, [pc, #24]
	bl	Func_08004278
	movs	r3, #134
	lsls	r3, r3, #1
	b.n	.L3
	movs	r0, r0
	.4byte 0xfffffff0
	.4byte 0x03001f2c
	.4byte 0x00000242
	.4byte 0x080a19a1
.L3:
	add	r3, r8
	ldr	r1, [r3, #0]
	mov	r0, r8
	bl	Func_080a33d4
	movs	r6, #24
	ldr	r5, [pc, #516]
	mov	r2, r8
	negs	r6, r6
	ldr	r1, [r2, #36]
	adds	r0, r5, #0
	movs	r2, #80
	adds	r3, r6, #0
	bl	Func_08015080
	adds	r5, #2
	mov	r3, r8
	ldr	r1, [r3, #36]
	adds	r0, r5, #0
	movs	r2, #0
	adds	r3, r6, #0
	bl	Func_08015080
	ldr	r1, [pc, #488]
	add	r1, r8
	mov	r9, r1
	b.n	.L4
.L10:
	movs	r0, #112
	bl	Func_080f9010
	movs	r2, #1
	str	r2, [sp, #12]
	mov	fp, r2
	b.n	.L4
.L11:
	movs	r0, #113
	bl	Func_080f9010
	movs	r1, #1
	movs	r3, #1
	mov	fp, r1
	movs	r1, #200
	negs	r3, r3
	ldr	r0, [pc, #452]
	lsls	r1, r1, #4
	str	r3, [sp, #12]
	bl	Func_080041d8
	b.n	.L4
.L16:
	cmp	r4, #0
	beq.n	.L5
	mov	r2, sl
	movs	r4, #0
	cmp	r2, #0
	beq.n	.L6
	mov	r3, r8
	ldr	r0, [r3, #36]
	movs	r1, #0
	adds	r2, r6, #0
	mov	sl, r4
	str	r4, [sp, #8]
	bl	Func_080a8f40
	ldr	r4, [sp, #8]
.L6:
	mov	r1, r8
	ldr	r0, [r1, #36]
	movs	r1, #0
	adds	r2, r6, #0
	str	r4, [sp, #8]
	bl	Func_080a8d34
	movs	r0, #1
	bl	Func_080030f8
	ldr	r4, [sp, #8]
.L5:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	Func_080030f8
	add	r3, sp, #24
	ldr	r1, [r6, #20]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r0, #0
	add	r3, sp, #32
	bl	Func_080a1fd4
	mov	r2, r8
	ldr	r1, [r6, #16]
	ldr	r3, [r2, #20]
	movs	r7, #1
	lsls	r1, r1, #4
	adds	r5, r0, #0
	strb	r7, [r3, #5]
	adds	r1, #60
	movs	r0, #55
	bl	Func_080a1a40
	ldr	r4, [sp, #8]
	cmp	r5, #1
	bne.n	.L7
	movs	r3, #1
	mov	sl, r3
	movs	r4, #1
.L7:
	cmp	r5, #0
	bne.n	.L8
	movs	r4, #1
.L8:
	movs	r1, #1
	negs	r1, r1
	cmp	r5, r1
	bne.n	.L9
	movs	r4, #0
.L9:
	ldr	r2, [pc, #320]
	ldr	r3, [r2, #0]
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L10
	ldr	r2, [r2, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L11
	ldr	r5, [pc, #304]
	movs	r7, #128
	ldr	r3, [r5, #0]
	lsls	r7, r7, #1
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L12
	ldr	r2, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L13
.L12:
	movs	r0, #111
	bl	Func_080f9010
	mov	r2, r8
	movs	r0, #28
	ldrsb	r0, [r2, r0]
	movs	r1, #130
	lsls	r1, r1, #2
	lsls	r3, r0, #1
	adds	r3, r3, r1
	ldrh	r3, [r2, r3]
	movs	r2, #152
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r6, #24]
	mov	r1, r8
	strb	r2, [r1, r3]
	ldr	r3, [r5, #0]
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L14
	adds	r0, #1
	b.n	.L15
.L14:
	subs	r0, #1
.L15:
	ldr	r3, [pc, #232]
	add	r3, r8
	ldrb	r1, [r3, #0]
	adds	r0, r0, r1
	bl	Func_080022fc
	movs	r3, #130
	lsls	r2, r0, #1
	lsls	r3, r3, #2
	mov	r1, r8
	adds	r2, r2, r3
	ldrh	r3, [r1, r2]
	str	r3, [r1, #8]
	ldrh	r3, [r1, r2]
	mov	r1, r9
	strb	r3, [r1, #0]
	mov	r3, r8
	strb	r0, [r3, #28]
	mov	r0, r8
	ldrh	r1, [r3, r2]
	bl	Func_080a1804
	b.n	.L4
.L13:
	movs	r0, #168
	lsls	r0, r0, #1
	str	r4, [sp, #8]
	bl	Func_080770c0
	ldr	r4, [sp, #8]
	cmp	r0, #0
	bne.n	.L4
	b.n	.L16
.L4:
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L17
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L17
	bl	Func_080a9cbc
	mov	r2, r8
	ldr	r0, [r2, #36]
	bl	Func_08015270
	mov	r3, r9
	ldrb	r0, [r3, #0]
	bl	Func_08077008
	movs	r1, #228
	lsls	r1, r1, #1
	movs	r2, #0
	add	r1, r8
	bl	Func_080a68ec
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r8
	strb	r0, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	add	r6, sp, #16
	movs	r1, #0
	adds	r0, r6, #0
	bl	Func_080a8b8c
	mov	r1, r8
	mov	r2, r9
	ldr	r0, [r1, #36]
	ldrb	r1, [r2, #0]
	bl	Func_080a9374
	movs	r4, #1
	mov	sl, r4
	b.n	.L13
.L17:
	mov	r3, r8
	ldr	r0, [r3, #44]
	bl	Func_08015278
	mov	r1, r8
	ldr	r0, [r1, #44]
	bl	Func_08015270
	bl	Func_080a345c
	movs	r3, #134
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r0, [r3, #0]
	bl	Func_08015278
	mov	r2, r8
	ldr	r0, [r2, #36]
	bl	Func_08015270
	ldr	r0, [sp, #12]
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000b06
	.4byte 0x0000021a
	.4byte 0x080a19a1
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x00000219
	movs	r0, #1
	bx	lr
