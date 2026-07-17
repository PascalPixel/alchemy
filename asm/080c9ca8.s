@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c9ca8
	.thumb_func
Func_080c9ca8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #84
	ldr	r6, [pc, #168]
	str	r1, [sp, #68]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	str	r1, [sp, #64]
	ldr	r3, [r3, #0]
	str	r3, [sp, #60]
	adds	r3, r6, #0
	ldr	r2, [pc, #156]
	subs	r3, #108
	ldr	r3, [r3, #0]
	adds	r5, r1, r2
	str	r3, [sp, #44]
	str	r0, [r5, #0]
	movs	r0, #1
	bl	Func_080cd594
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L0
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #11
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [r6, #28]
	movs	r0, #47
	str	r3, [sp, #48]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	b.n	.L1
.L0:
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r4, [r6, #28]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	str	r4, [sp, #48]
.L1:
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r6, [r6, #32]
	str	r6, [sp, #52]
	ldr	r0, [pc, #68]
	ldr	r1, [sp, #64]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [sp, #68]
	cmp	r0, #4
	bhi.n	.L2
	ldr	r3, [pc, #52]
	lsls	r1, r0, #2
	ldr	r3, [r1, r3]
	mov	pc, r3
	.4byte 0x080c9d54
	.4byte 0x080c9d58
	.4byte 0x080c9d5c
	.4byte 0x080c9d60
	.4byte 0x080c9d84
	ldr	r0, [pc, #28]
	b.n	.L3
	ldr	r0, [pc, #28]
	b.n	.L3
	ldr	r0, [pc, #28]
	b.n	.L3
	ldr	r0, [pc, #28]
	b.n	.L3
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00000058
	.4byte 0x080c9d40
	.4byte 0x000000b4
	.4byte 0x000000a0
	.4byte 0x000000cb
	.4byte 0x00000086
.L2:
	ldr	r0, [pc, #384]
.L3:
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #380]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	movs	r4, #239
	ldr	r3, [sp, #64]
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #64]
	ldr	r1, [pc, #360]
	movs	r3, #50
	adds	r2, r0, r1
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #352]
	bl	Func_080041d8
	ldr	r3, [pc, #348]
	ldr	r2, [sp, #64]
	adds	r5, r2, r3
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r1, [r3, #20]
	movs	r4, #3
	adds	r2, r1, #0
	muls	r2, r4
	lsls	r3, r2, #1
	ldr	r0, [r0, #0]
	adds	r3, r3, r2
	lsls	r3, r3, #1
	mov	sl, r0
	adds	r3, #48
	movs	r0, #0
	str	r3, [sp, #40]
	str	r0, [sp, #56]
	cmp	r1, #0
	beq.n	.L4
	mov	r9, r0
.L6:
	ldr	r1, [sp, #64]
	ldr	r2, [pc, #300]
	ldr	r4, [sp, #56]
	adds	r3, r1, r2
	ldr	r2, [r3, #0]
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	Func_080b5098
	ldr	r0, [r0, #0]
	mov	r4, r9
	mov	r8, r0
	ldr	r0, [sp, #64]
	lsls	r3, r4, #2
	movs	r1, #225
	movs	r2, #0
	adds	r3, r3, r0
	lsls	r1, r1, #7
	mov	fp, r2
	adds	r7, r3, r1
.L5:
	mov	r2, sl
	ldr	r3, [r2, #8]
	str	r3, [r7, #0]
	movs	r4, #160
	ldr	r5, [r2, #12]
	lsls	r4, r4, #13
	adds	r5, r5, r4
	str	r5, [r7, #4]
	ldr	r6, [r2, #16]
	str	r6, [r7, #8]
	mov	r1, r8
	ldr	r0, [r1, #8]
	movs	r1, #24
	subs	r0, r0, r3
	bl	Func_080022ec
	str	r0, [r7, #12]
	mov	r2, r8
	ldr	r0, [r2, #12]
	movs	r3, #160
	lsls	r3, r3, #13
	adds	r0, r0, r3
	movs	r1, #24
	subs	r0, r0, r5
	bl	Func_080022ec
	str	r0, [r7, #16]
	mov	r4, r8
	ldr	r0, [r4, #16]
	movs	r1, #24
	subs	r0, r0, r6
	bl	Func_080022ec
	str	r0, [r7, #20]
	movs	r0, #1
	add	fp, r0
	movs	r3, #0
	mov	r1, fp
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r1, #3
	bne.n	.L5
	movs	r2, #3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	add	r9, r3
	ldr	r3, [sp, #56]
	ldr	r4, [sp, #64]
	adds	r3, #1
	ldr	r0, [pc, #160]
	str	r3, [sp, #56]
	adds	r3, r4, r0
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #56]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	bne.n	.L6
.L4:
	ldr	r3, [sp, #40]
	movs	r2, #0
	mov	sl, r2
	cmp	r3, #0
	bne.n	.L7
	b.n	.L8
.L7:
	ldr	r4, [sp, #44]
	subs	r3, #16
	adds	r4, #12
	str	r3, [sp, #24]
	str	r4, [sp, #28]
.L32:
	ldr	r0, [sp, #64]
	movs	r1, #211
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r7, #128
	mov	r3, sl
	str	r2, [sp, #56]
	adds	r6, r0, r1
	lsls	r7, r7, #12
	lsls	r5, r3, #12
.L9:
	adds	r0, r5, #0
	bl	Func_08002322
	lsls	r0, r0, #1
	subs	r0, r7, r0
	asrs	r0, r0, #10
	stmia	r6!, {r0}
	ldr	r0, [sp, #56]
	movs	r4, #128
	lsls	r4, r4, #5
	adds	r0, #1
	adds	r5, r5, r4
	str	r0, [sp, #56]
	cmp	r0, #160
	bne.n	.L9
	ldr	r1, [sp, #24]
	cmp	sl, r1
	ble.n	.L10
	ldr	r4, [sp, #40]
	mov	r0, sl
	ldr	r1, [pc, #44]
	ldr	r3, [pc, #64]
	subs	r2, r4, r0
	orrs	r2, r1
	strh	r2, [r3, #0]
.L10:
	bl	Func_080049ac
	ldr	r1, [sp, #28]
	ldr	r0, [sp, #44]
	bl	Func_080051d8
	movs	r1, #0
	ldr	r3, [sp, #64]
	ldr	r4, [pc, #36]
	str	r1, [sp, #56]
	adds	r2, r3, r4
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L11
	b.n	.L12
.L11:
	b.n	.L13
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x000000a3
	.4byte 0x03001388
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x04000052
.L13:
	mov	r1, sl
	str	r2, [sp, #36]
	movs	r0, #36
	movs	r2, #0
	subs	r1, #30
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r2, [sp, #8]
.L31:
	ldr	r3, [sp, #12]
	cmp	sl, r3
	bge.n	.L14
	b.n	.L15
.L14:
	ldr	r0, [sp, #68]
	lsls	r0, r0, #2
	movs	r4, #0
	str	r0, [sp, #32]
	mov	fp, r4
	mov	r9, r3
.L22:
	cmp	sl, r9
	blt.n	.L16
	ldr	r3, [sp, #8]
	add	r3, fp
	lsls	r2, r3, #3
	ldr	r1, [sp, #64]
	subs	r2, r2, r3
	lsls	r2, r2, #2
	movs	r3, #225
	adds	r2, r1, r2
	lsls	r3, r3, #7
	adds	r3, r3, r2
	mov	r8, r3
	add	r6, sp, #72
	mov	r0, r8
	adds	r1, r6, #0
	bl	Func_080e3944
	ldr	r3, [r6, #0]
	asrs	r7, r3, #1
	str	r7, [r6, #0]
	mov	r4, r8
	ldr	r5, [r4, #24]
	cmp	r5, #0
	bge.n	.L17
	adds	r5, #7
.L17:
	asrs	r2, r5, #3
	cmp	r2, #5
	ble.n	.L18
	movs	r2, #5
.L18:
	ldr	r3, [pc, #584]
	ldr	r0, [sp, #32]
	ldrsb	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L19
	mov	r1, sl
	lsls	r5, r2, #1
	lsrs	r0, r1, #31
	adds	r5, r5, r2
	add	r0, sl
	movs	r1, #3
	lsls	r5, r5, #3
	asrs	r0, r0, #1
	adds	r5, r5, r2
	bl	Func_080022fc
	lsls	r2, r0, #2
	adds	r2, r2, r0
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r3, r3, #6
	ldr	r2, [sp, #64]
	lsls	r5, r5, #5
	adds	r5, r5, r3
	ldr	r3, [r6, #4]
	adds	r5, r2, r5
	movs	r4, #20
	movs	r0, #40
	adds	r2, r7, #0
	subs	r2, #10
	subs	r3, #40
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	adds	r1, r5, #0
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #60]
	bl	Func_080072f4
	ldr	r2, [r6, #0]
	movs	r0, #20
	movs	r1, #40
	ldr	r3, [r6, #4]
	subs	r2, #10
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r0, [sp, #60]
	adds	r1, r5, #0
	ldr	r4, [sp, #52]
	bl	Func_080072f4
	b.n	.L20
.L19:
	lsls	r5, r2, #1
	adds	r5, r5, r2
	lsls	r5, r5, #3
	ldr	r0, [sp, #64]
	adds	r5, r5, r2
	lsls	r5, r5, #5
	movs	r1, #150
	ldr	r3, [r6, #4]
	adds	r5, r0, r5
	lsls	r1, r1, #6
	adds	r5, r5, r1
	movs	r4, #20
	movs	r0, #40
	adds	r2, r7, #0
	subs	r2, #10
	subs	r3, #40
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	adds	r1, r5, #0
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #60]
	bl	Func_080072f4
	ldr	r2, [r6, #0]
	movs	r0, #20
	movs	r1, #40
	ldr	r3, [r6, #4]
	subs	r2, #10
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r0, [sp, #60]
	adds	r1, r5, #0
	ldr	r4, [sp, #52]
	bl	Func_080072f4
.L20:
	mov	r0, r8
	movs	r1, #64
	movs	r2, #0
	bl	Func_080e38b8
	mov	r0, r8
	ldr	r3, [r0, #24]
	adds	r3, #1
	str	r3, [r0, #24]
.L16:
	movs	r2, #1
	add	fp, r2
	movs	r1, #6
	mov	r3, fp
	add	r9, r1
	cmp	r3, #3
	beq.n	.L21
	b.n	.L22
.L21:
	ldr	r3, [sp, #32]
	ldr	r1, [pc, #376]
	adds	r3, #3
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L23
	ldr	r3, [sp, #12]
	adds	r3, #30
	cmp	sl, r3
	blt.n	.L23
	ldr	r3, [sp, #12]
	adds	r3, #62
	cmp	sl, r3
	bge.n	.L23
	ldr	r4, [sp, #36]
	ldr	r1, [sp, #20]
	ldr	r3, [r4, #0]
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	ldr	r4, [sp, #12]
	mov	r3, sl
	subs	r2, r3, r4
	ldr	r3, [sp, #16]
	ldr	r0, [r0, #0]
	ldr	r1, [pc, #332]
	cmp	r3, #0
	bge.n	.L24
	adds	r3, r2, #0
	subs	r3, #23
.L24:
	ldr	r2, [sp, #16]
	asrs	r3, r3, #3
	lsls	r3, r3, #3
	subs	r3, r2, r3
	ldrsb	r3, [r1, r3]
	ldr	r2, [r0, #8]
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r0, #8]
	cmp	r2, #0
	ble.n	.L25
	movs	r4, #128
	lsls	r4, r4, #8
	adds	r3, r2, r4
	b.n	.L26
.L25:
	ldr	r1, [pc, #296]
	adds	r3, r2, r1
.L26:
	str	r3, [r0, #8]
	ldr	r2, [sp, #36]
	ldr	r3, [r2, #0]
	ldr	r4, [sp, #20]
	ldrsh	r0, [r3, r4]
	movs	r3, #0
	movs	r1, #1
	str	r3, [sp, #0]
	negs	r1, r1
	movs	r2, #5
	subs	r3, #1
	bl	Func_080d6888
	ldr	r1, [pc, #256]
.L23:
	ldr	r3, [sp, #32]
	adds	r3, #1
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L27
	ldr	r3, [sp, #12]
	adds	r3, #24
	cmp	sl, r3
	bne.n	.L28
	movs	r0, #133
	bl	Func_080f9010
	ldr	r2, [sp, #56]
	cmp	r2, #0
	bne.n	.L29
	movs	r0, #1
	negs	r0, r0
	bl	Func_080b50e8
.L29:
	ldr	r4, [sp, #36]
	ldr	r1, [sp, #20]
	ldr	r3, [r4, #0]
	ldrsh	r0, [r3, r1]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	ldr	r3, [sp, #56]
	bl	Func_080d6888
.L28:
	ldr	r3, [sp, #12]
	adds	r3, #40
	cmp	sl, r3
	bne.n	.L30
	ldr	r4, [sp, #36]
	ldr	r3, [r4, #0]
	ldr	r1, [sp, #20]
	ldrsh	r0, [r3, r1]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	ldr	r3, [sp, #56]
	bl	Func_080d6888
.L30:
	ldr	r1, [pc, #164]
.L27:
	ldr	r3, [sp, #32]
	adds	r3, #2
	ldrsb	r1, [r1, r3]
	movs	r3, #1
	negs	r3, r3
	cmp	r1, r3
	beq.n	.L15
	ldr	r3, [sp, #12]
	adds	r3, #24
	cmp	sl, r3
	bne.n	.L15
	ldr	r4, [sp, #64]
	ldr	r0, [pc, #152]
	movs	r3, #4
	adds	r2, r4, r0
	str	r3, [r2, #0]
	ldr	r2, [sp, #36]
	ldr	r4, [sp, #20]
	ldr	r3, [r2, #0]
	ldrsh	r0, [r3, r4]
	bl	Func_080b5088
.L15:
	ldr	r3, [sp, #20]
	ldr	r4, [sp, #16]
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #56]
	adds	r3, #2
	subs	r4, #32
	adds	r0, #32
	adds	r1, #3
	adds	r2, #1
	str	r4, [sp, #16]
	str	r3, [sp, #20]
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #56]
	ldr	r4, [sp, #36]
	ldr	r3, [r4, #0]
	ldr	r3, [r3, #20]
	cmp	r2, r3
	beq.n	.L12
	b.n	.L31
.L12:
	movs	r0, #8
	movs	r1, #8
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r1, [pc, #80]
	ldr	r0, [sp, #64]
	movs	r3, #1
	adds	r2, r0, r1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #1
	ldr	r3, [sp, #40]
	add	sl, r2
	cmp	sl, r3
	beq.n	.L8
	b.n	.L32
.L8:
	ldr	r0, [pc, #56]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080edee8
	.4byte 0x080edefc
	.4byte 0xffff8000
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte 0x080cd261
