@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080cb1a4
	.thumb_func
Func_080cb1a4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #780]
	adds	r3, r7, #0
	ldmia	r3!, {r1}
	ldr	r2, [pc, #776]
	mov	fp, r1
	ldr	r3, [r3, #0]
	sub	sp, #84
	add	r2, fp
	str	r3, [sp, #56]
	str	r0, [r2, #0]
	movs	r0, #0
	mov	sl, r2
	bl	Func_080cd594
	ldr	r0, [pc, #760]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #732]
	add	r3, fp
	add	r2, fp
	str	r5, [r3, #0]
	ldr	r7, [r7, #28]
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #716]
	str	r7, [sp, #48]
	bl	Func_080041d8
	mov	r4, sl
	ldr	r3, [r4, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5098
	mov	r1, sl
	ldr	r3, [r1, #0]
	adds	r5, r0, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	Func_080b5098
	ldr	r7, [r5, #0]
	ldr	r0, [r0, #0]
	movs	r3, #90
	ldr	r5, [r7, #8]
	mov	r8, r3
	ldr	r3, [r0, #8]
	subs	r3, r3, r5
	mov	r9, r0
	movs	r1, #100
	mov	r0, r8
	muls	r0, r3
	bl	Func_080022ec
	adds	r5, r5, r0
	str	r5, [sp, #44]
	mov	r4, r9
	ldr	r3, [r4, #16]
	ldr	r5, [r7, #16]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #100
	bl	Func_080022ec
	mov	r1, sl
	ldr	r3, [r1, #0]
	adds	r5, r5, r0
	ldr	r0, [r3, #8]
	bl	Func_080b5070
	str	r0, [sp, #40]
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	bl	Func_080b5070
	str	r0, [sp, #36]
	adds	r0, r7, #0
	bl	Func_08009140
	adds	r3, r5, #0
	ldr	r1, [sp, #44]
	movs	r2, #0
	adds	r0, r7, #0
	bl	Func_08009150
	movs	r1, #2
	adds	r0, r7, #0
	bl	Func_08009080
	adds	r3, r7, #0
	movs	r2, #1
	adds	r3, #88
	strb	r2, [r3, #0]
	adds	r5, r7, #0
	movs	r3, #128
	adds	r5, #90
	lsls	r3, r3, #10
	str	r5, [sp, #32]
	strb	r2, [r5, #0]
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r7, #48]
	movs	r0, #20
	bl	Func_080030f8
	movs	r5, #46
	mov	r2, sp
	mov	r3, sp
	movs	r4, #56
	adds	r2, #72
	adds	r3, #60
	negs	r4, r4
	negs	r5, r5
	movs	r1, #0
	str	r2, [sp, #20]
	str	r3, [sp, #24]
	str	r4, [sp, #16]
	str	r5, [sp, #12]
	mov	sl, r1
.L11:
	ldr	r3, [pc, #524]
	ldr	r5, [r3, #0]
	adds	r6, r5, #0
	adds	r6, #12
	bl	Func_080049ac
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	Func_080051d8
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L0
	movs	r2, #240
	lsls	r2, r2, #12
	mov	r3, r9
	str	r2, [r3, #40]
	ldr	r3, [pc, #496]
	mov	r4, r9
	str	r3, [r4, #72]
	str	r2, [r7, #40]
	str	r3, [r7, #72]
.L0:
	mov	r1, sl
	cmp	r1, #11
	bne.n	.L1
	mov	r2, r9
	ldr	r3, [r2, #28]
	negs	r3, r3
	str	r3, [r2, #28]
	ldr	r3, [r7, #28]
	negs	r3, r3
	str	r3, [r7, #28]
	ldr	r4, [sp, #40]
	ldr	r3, [r7, #12]
	adds	r3, r3, r4
	str	r3, [r7, #12]
	ldr	r1, [sp, #36]
	ldr	r3, [r2, #12]
	adds	r3, r3, r1
	str	r3, [r2, #12]
.L1:
	mov	r2, sl
	cmp	r2, #54
	bne.n	.L2
	ldr	r3, [pc, #424]
	add	r3, fp
	ldr	r3, [r3, #0]
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	Func_080d6888
	movs	r3, #128
	mov	r1, r9
	lsls	r3, r3, #12
	str	r3, [r1, #40]
	ldr	r3, [pc, #416]
	str	r3, [r1, #72]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r7, #40]
	ldr	r3, [pc, #408]
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	ldr	r2, [sp, #32]
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r7, #0
	bl	Func_08009140
	ldr	r3, [r7, #16]
	adds	r0, r7, #0
	movs	r1, #0
	movs	r2, #0
	bl	Func_08009150
.L2:
	bl	Func_080049ac
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_080051d8
	ldr	r3, [r7, #8]
	ldr	r4, [sp, #20]
	str	r3, [r4, #0]
	ldr	r3, [r7, #12]
	str	r3, [r4, #4]
	ldr	r3, [r7, #16]
	str	r3, [r4, #8]
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #24]
	bl	Func_080e3944
	ldr	r5, [sp, #24]
	ldr	r3, [r5, #0]
	asrs	r2, r3, #1
	mov	r3, sl
	subs	r3, #54
	str	r2, [r5, #0]
	cmp	r3, #1
	bhi.n	.L3
	ldr	r3, [r5, #4]
	movs	r1, #32
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	subs	r2, #16
	subs	r3, #16
	ldr	r0, [sp, #56]
	mov	r1, fp
	ldr	r4, [sp, #48]
	bl	Func_080072f4
.L3:
	ldr	r5, [sp, #16]
	cmp	r5, #11
	bhi.n	.L4
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #11
	ldr	r6, [sp, #12]
	movs	r4, #0
	mov	r8, r3
.L5:
	lsls	r1, r4, #12
	adds	r0, r1, #0
	str	r1, [sp, #28]
	str	r4, [sp, #8]
	bl	Func_08002322
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r2, [sp, #24]
	ldr	r5, [r2, #0]
	asrs	r3, r3, #16
	ldr	r0, [sp, #28]
	adds	r5, r5, r3
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	mov	r1, sl
	movs	r2, #32
	asrs	r3, r3, #16
	subs	r3, r3, r1
	subs	r5, #16
	str	r2, [sp, #0]
	mov	r1, fp
	movs	r2, #64
	str	r2, [sp, #4]
	adds	r3, #100
	adds	r2, r5, #0
	ldr	r0, [sp, #56]
	add	r1, r8
	ldr	r5, [sp, #48]
	bl	Func_080072f8
	ldr	r4, [sp, #8]
	adds	r4, #1
	cmp	r4, #16
	bne.n	.L5
.L4:
	mov	r1, sl
	cmp	r1, #64
	bne.n	.L6
	mov	r2, r9
	ldr	r3, [r2, #28]
	negs	r3, r3
	str	r3, [r2, #28]
	ldr	r3, [r7, #28]
	negs	r3, r3
	str	r3, [r7, #28]
	ldr	r4, [sp, #40]
	ldr	r3, [r7, #12]
	subs	r3, r3, r4
	str	r3, [r7, #12]
	ldr	r5, [sp, #36]
	ldr	r3, [r2, #12]
	subs	r3, r3, r5
	str	r3, [r2, #12]
	adds	r0, r7, #0
	movs	r1, #0
	bl	Func_08009080
.L6:
	mov	r1, sl
	cmp	r1, #54
	bne.n	.L7
	movs	r0, #134
	bl	Func_080b50e8
.L7:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L8
	movs	r0, #136
	bl	Func_080f9010
	ldr	r2, [pc, #136]
	movs	r3, #6
	add	r2, fp
	str	r3, [r2, #0]
.L8:
	mov	r3, sl
	cmp	r3, #53
	bne.n	.L9
	ldr	r2, [pc, #124]
	movs	r3, #6
	add	r2, fp
	str	r3, [r2, #0]
.L9:
	movs	r1, #16
	movs	r0, #16
	bl	Func_080e155c
	ldr	r2, [pc, #112]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #1
	ldr	r4, [sp, #16]
	ldr	r5, [sp, #12]
	add	sl, r1
	adds	r4, #1
	adds	r5, #1
	mov	r2, sl
	str	r4, [sp, #16]
	str	r5, [sp, #12]
	cmp	r2, #96
	beq.n	.L10
	b.n	.L11
.L10:
	ldr	r0, [pc, #48]
	bl	Func_08004278
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
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x0000007d
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e80
	.4byte 0x000091eb
	.4byte 0x0000ab85
	.4byte 0x00007851
	.4byte 0x000077a8
	.4byte 0x00007824
