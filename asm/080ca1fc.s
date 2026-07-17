@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080ca1fc
	.thumb_func
Func_080ca1fc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	ldr	r2, [pc, #804]
	str	r1, [sp, #40]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	str	r1, [sp, #36]
	ldr	r3, [r3, #0]
	str	r3, [sp, #32]
	adds	r3, r2, #0
	subs	r3, #108
	ldr	r3, [r3, #0]
	str	r3, [sp, #20]
	ldr	r2, [r2, #8]
	str	r2, [sp, #16]
	ldr	r2, [pc, #784]
	adds	r3, r1, r2
	str	r0, [r3, #0]
	ldr	r3, [sp, #40]
	cmp	r3, #0
	bne.n	.L0
	movs	r0, #0
	bl	Func_080cd594
	b.n	.L1
.L0:
	movs	r0, #1
	bl	Func_080cd594
.L1:
	movs	r6, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	Func_080ed408
	ldr	r5, [pc, #744]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #24]
	movs	r2, #7
	movs	r3, #11
	movs	r0, #47
	str	r6, [sp, #0]
	bl	Func_080ed408
	adds	r5, #188
	ldr	r5, [r5, #0]
	ldr	r0, [pc, #720]
	ldr	r1, [sp, #16]
	movs	r2, #0
	movs	r3, #0
	str	r5, [sp, #28]
	bl	Func_080e0524
	ldr	r4, [sp, #40]
	cmp	r4, #0
	bne.n	.L2
	ldr	r0, [pc, #704]
	b.n	.L3
.L2:
	ldr	r0, [pc, #704]
.L3:
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #700]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	movs	r3, #239
	ldr	r1, [sp, #36]
	ldr	r4, [pc, #688]
	lsls	r3, r3, #7
	adds	r2, r1, r3
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #672]
	bl	Func_080041d8
	ldr	r2, [pc, #640]
	ldr	r1, [sp, #36]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5098
	ldr	r0, [r0, #0]
	movs	r3, #0
	str	r0, [sp, #12]
	ldr	r7, [pc, #652]
	mov	r8, r3
.L5:
	bl	Func_08004458
	ldr	r5, [pc, #648]
	ands	r5, r0
	bl	Func_08004458
	ldr	r3, [pc, #644]
	ldr	r4, [sp, #12]
	adds	r6, r0, #0
	ands	r6, r3
	ldr	r3, [r4, #8]
	str	r3, [r7, #0]
	movs	r1, #160
	ldr	r3, [r4, #12]
	lsls	r1, r1, #11
	adds	r3, r3, r1
	str	r3, [r7, #4]
	ldr	r3, [r4, #16]
	adds	r0, r6, #0
	str	r3, [r7, #8]
	bl	Func_08002322
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	bl	Func_08004458
	movs	r3, #255
	ands	r3, r0
	subs	r3, #32
	lsls	r3, r3, #9
	str	r3, [r7, #16]
	adds	r0, r6, #0
	bl	Func_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #8
	str	r3, [r7, #20]
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #48
	str	r3, [r7, #24]
	ldr	r2, [sp, #40]
	cmp	r2, #0
	bne.n	.L4
	ldr	r3, [r7, #12]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #12]
	ldr	r3, [r7, #20]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #20]
.L4:
	movs	r3, #1
	movs	r4, #128
	add	r8, r3
	lsls	r4, r4, #1
	adds	r7, #28
	cmp	r8, r4
	bne.n	.L5
	ldr	r2, [sp, #20]
	movs	r3, #56
	adds	r2, #12
	movs	r1, #0
	add	r3, sp
	str	r2, [sp, #8]
	mov	fp, r1
	mov	r9, r3
.L22:
	bl	Func_080049ac
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #8]
	bl	Func_080051d8
	ldr	r6, [pc, #480]
	movs	r4, #0
	mov	r8, r4
.L14:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L6
	adds	r3, #31
.L6:
	asrs	r3, r3, #5
	lsls	r3, r3, #3
	cmp	fp, r3
	blt.n	.L7
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L7
	mov	r1, r8
	lsls	r0, r1, #2
	adds	r0, r0, r3
	lsls	r0, r0, #10
	bl	Func_08002322
	ldr	r3, [r6, #0]
	lsls	r0, r0, #4
	mov	r2, r9
	adds	r3, r3, r0
	str	r3, [r2, #0]
	ldr	r3, [r6, #4]
	str	r3, [r2, #4]
	ldr	r3, [r6, #8]
	add	r5, sp, #44
	str	r3, [r2, #8]
	mov	r0, r9
	adds	r1, r5, #0
	bl	Func_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	ldr	r4, [pc, #420]
	ldr	r3, [r5, #8]
	cmp	r3, r4
	bgt.n	.L8
	movs	r3, #157
	lsls	r3, r3, #1
	str	r3, [r5, #8]
.L8:
	ldr	r2, [pc, #408]
	cmp	r3, r2
	ble.n	.L9
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L9:
	ldr	r1, [pc, #404]
	adds	r2, r3, r1
	cmp	r2, #0
	bge.n	.L10
	adds	r2, r3, #0
	subs	r2, #251
.L10:
	asrs	r3, r2, #6
	movs	r0, #6
	subs	r0, r0, r3
	lsls	r4, r0, #1
	ldr	r2, [pc, #388]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #16]
	lsrs	r3, r0, #31
	adds	r1, r2, r1
	adds	r3, r0, r3
	ldr	r2, [r5, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #32]
	ldr	r4, [sp, #24]
	bl	Func_080072f4
	movs	r2, #128
	movs	r1, #62
	adds	r0, r6, #0
	lsls	r2, r2, #3
	bl	Func_080e38b8
	ldr	r1, [sp, #40]
	cmp	r1, #1
	bne.n	.L11
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #8]
	cmp	r3, #0
	bge.n	.L12
	ldr	r3, [r6, #12]
	movs	r4, #128
	lsls	r4, r4, #6
	adds	r3, r3, r4
	b.n	.L13
.L12:
	ldr	r3, [r6, #12]
	ldr	r1, [pc, #316]
	adds	r3, r3, r1
.L13:
	str	r3, [r6, #12]
.L11:
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L7:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r6, #28
	cmp	r3, #128
	bne.n	.L14
	ldr	r4, [sp, #40]
	cmp	r4, #1
	bne.n	.L15
	ldr	r3, [sp, #36]
	ldr	r4, [pc, #224]
	adds	r2, r3, r4
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L16
	adds	r5, r2, #0
	movs	r7, #36
	movs	r6, #48
.L18:
	cmp	fp, r6
	bne.n	.L17
	movs	r0, #1
	negs	r0, r0
	bl	Func_080b50e8
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r7]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r8
	bl	Func_080d6888
.L17:
	ldr	r3, [r5, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	r8, r2
	adds	r7, #2
	adds	r6, #8
	cmp	r8, r3
	bne.n	.L18
	b.n	.L16
.L15:
	ldr	r4, [sp, #36]
	ldr	r1, [pc, #152]
	movs	r3, #0
	adds	r2, r4, r1
	mov	r8, r3
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L16
	movs	r3, #1
	negs	r3, r3
	mov	sl, r3
	adds	r5, r2, #0
	movs	r7, #36
	movs	r6, #48
.L20:
	cmp	fp, r6
	bne.n	.L19
	movs	r0, #126
	bl	Func_080f9010
	mov	r0, sl
	bl	Func_080b50e8
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r7]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r2, sl
	mov	r3, r8
	bl	Func_080d6888
.L19:
	ldr	r3, [r5, #0]
	movs	r1, #1
	ldr	r3, [r3, #20]
	add	r8, r1
	adds	r7, #2
	adds	r6, #8
	cmp	r8, r3
	bne.n	.L20
.L16:
	bl	Func_080cd52c
	ldr	r4, [pc, #136]
	ldr	r3, [sp, #36]
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #128
	beq.n	.L21
	b.n	.L22
.L21:
	ldr	r0, [pc, #72]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x03001e50
	.4byte 0x00000073
	.4byte 0x0000007c
	.4byte 0x0000007b
	.4byte 0x03001388
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x02010000
	.4byte 0x000003ff
	.4byte 0x0000ffff
	.4byte 0x00000139
	.4byte 0x0000027a
	.4byte 0xfffffec6
	.4byte 0x080ede48
	.4byte 0xffffe000
	.4byte 0x00007824
