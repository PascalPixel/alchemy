.syntax unified
	.thumb
	.balign 4
	.global Func_080dfcf8
	.thumb_func
Func_080dfcf8:
	push	{lr}
	bl	.L_080dfd00
	pop	{pc}
.L_080dfd00:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #40
	ldr	r1, [r3, #20]
	mov	sl, r3
	ldr	r2, [r3, #16]
	str	r1, [sp, #0]
	movs	r3, #0
	mov	fp, r3
	mov	r3, sl
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r1, [r1, #80]
	cmp	r3, #0
	beq.n	.L_080dfd36
	b.n	.L_080dffd6
.L_080dfd36:
	mov	r3, sl
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080dfd66
	movs	r3, #1
	mov	fp, r3
	ldr	r3, [r1, #40]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r1, #186
	lsls	r1, r1, #1
	cmp	r3, r1
	bne.n	.L_080dfd5a
	movs	r1, #2
	mov	fp, r1
.L_080dfd5a:
	movs	r1, #118
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_080dfd66
	movs	r3, #3
	mov	fp, r3
.L_080dfd66:
	ldr	r3, [r2, #8]
	add	r1, sp, #16
	str	r3, [r1, #0]
	add	r5, sp, #4
	ldr	r3, [r2, #12]
	mov	r8, r1
	str	r3, [r1, #4]
	movs	r0, #140
	ldr	r3, [r2, #16]
	mov	r2, sl
	str	r3, [r1, #8]
	ldr	r1, [pc, #612]
	ldr	r3, [r2, #4]
	lsls	r0, r0, #1
	str	r3, [r5, #0]
	ldr	r3, [r2, #8]
	adds	r3, r3, r1
	str	r3, [r5, #4]
	movs	r1, #0
	ldr	r3, [r2, #12]
	movs	r2, #0
	str	r3, [r5, #8]
	movs	r3, #0
	bl	Func_080dc10c
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_080dfda0
	b.n	.L_080dffd6
.L_080dfda0:
	bl	0x080dc294
	adds	r0, r7, #0
	movs	r1, #2
	bl	Object_SetMode
	mov	r9, r8
	movs	r6, #0
	mov	r8, r5
.L_080dfdb2:
	mov	r2, r8
	mov	r1, r9
	ldr	r3, [r2, #0]
	ldr	r5, [r1, #0]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r6, #0
	muls	r0, r3
	bl	Math_Div
	adds	r5, r5, r0
	str	r5, [r7, #8]
	mov	r2, r8
	mov	r1, r9
	ldr	r3, [r2, #4]
	ldr	r5, [r1, #4]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r6, #0
	muls	r0, r3
	bl	Math_Div
	adds	r5, r5, r0
	str	r5, [r7, #12]
	mov	r2, r8
	mov	r1, r9
	ldr	r3, [r2, #8]
	ldr	r5, [r1, #8]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r6, #0
	muls	r0, r3
	bl	Math_Div
	movs	r3, #134
	lsls	r3, r3, #9
	adds	r3, #204
	adds	r5, r5, r0
	movs	r1, #10
	adds	r0, r6, #0
	muls	r0, r3
	str	r5, [r7, #16]
	bl	Math_Div
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r0, r0, r2
	str	r0, [r7, #24]
	str	r0, [r7, #28]
	adds	r6, #1
	movs	r0, #1
	bl	WaitFrames
	cmp	r6, #11
	blt.n	.L_080dfdb2
	ldr	r3, [pc, #452]
	movs	r0, #207
	str	r3, [r7, #24]
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	str	r3, [r7, #28]
	bl	Audio_PlayCue
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_080dfed6
	mov	r1, fp
	cmp	r1, #1
	bne.n	.L_080dfef8
	movs	r0, #20
	bl	WaitFrames
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L_080dfe4e
	ldr	r3, [pc, #416]
	str	r3, [r2, #108]
.L_080dfe4e:
	movs	r3, #0
	mov	r8, r3
	add	r6, sp, #28
.L_080dfe54:
	ldr	r3, [r7, #8]
	movs	r1, #128
	str	r3, [r6, #0]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	mov	r2, r8
	muls	r2, r3
	ldr	r3, [r7, #12]
	lsls	r1, r1, #11
	adds	r3, r3, r2
	adds	r3, r3, r1
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
	bl	Random16
	movs	r2, #192
	lsls	r5, r0, #2
	lsls	r2, r2, #10
	adds	r5, r5, r0
	adds	r5, r5, r2
	bl	Random16
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_0801489c
	movs	r0, #154
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	lsls	r0, r0, #1
	bl	Func_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080dfeba
	ldr	r3, [pc, #332]
	movs	r2, #0
	str	r3, [r5, #108]
	adds	r3, r5, #0
	adds	r3, #100
	str	r7, [r5, #104]
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	bl	Random16
	strh	r0, [r5, #6]
.L_080dfeba:
	movs	r0, #6
	bl	WaitFrames
	movs	r3, #1
	add	r8, r3
	mov	r1, r8
	cmp	r1, #15
	ble.n	.L_080dfe54
	movs	r0, #20
	bl	WaitFrames
	movs	r0, #120
	bl	WaitFrames
.L_080dfed6:
	movs	r1, #1
	adds	r0, r7, #0
	bl	Object_SetMode
	movs	r0, #30
	bl	WaitFrames
	movs	r0, #136
	bl	Audio_PlayCue
	adds	r0, r7, #0
	bl	Func_08020340
	adds	r0, r7, #0
	bl	0x080200c8
	b.n	.L_080dffd2
.L_080dfef8:
	mov	r3, fp
	subs	r3, #2
	cmp	r3, #1
	bhi.n	.L_080dffd2
	movs	r0, #5
	bl	WaitFrames
	adds	r0, r7, #0
	movs	r1, #1
	bl	Object_SetMode
	mov	r2, fp
	cmp	r2, #2
	bne.n	.L_080dffa2
	movs	r1, #192
	movs	r3, #0
	lsls	r1, r1, #10
	movs	r2, #20
	mov	r8, r3
	add	r6, sp, #28
	mov	fp, r1
	mov	r9, r2
.L_080dff24:
	ldr	r3, [r7, #8]
	movs	r1, #128
	str	r3, [r6, #0]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	mov	r2, r8
	muls	r2, r3
	ldr	r3, [r7, #12]
	lsls	r1, r1, #11
	adds	r3, r3, r2
	adds	r3, r3, r1
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
	bl	Random16
	lsls	r5, r0, #2
	adds	r5, r5, r0
	bl	Random16
	add	r5, fp
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	Func_0801489c
	movs	r0, #154
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	lsls	r0, r0, #1
	bl	Func_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080dff92
	ldr	r3, [pc, #132]
	mov	r1, r9
	str	r3, [r5, #108]
	adds	r3, r5, #0
	adds	r3, #100
	strh	r1, [r3, #0]
	movs	r2, #0
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	mov	r2, fp
	str	r3, [r5, #72]
	str	r2, [r5, #40]
	bl	Random16
	strh	r0, [r5, #6]
.L_080dff92:
	movs	r1, #1
	movs	r3, #2
	add	r8, r1
	negs	r3, r3
	mov	r2, r8
	add	r9, r3
	cmp	r2, #15
	ble.n	.L_080dff24
.L_080dffa2:
	mov	r3, sl
	movs	r1, #0
	ldr	r0, [r3, #16]
	bl	Func_080e1420
	bl	0x080dc384
	adds	r2, r7, #0
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r3, [pc, #60]
	mov	r2, sl
	str	r3, [r7, #108]
	movs	r1, #26
	ldrsh	r0, [r2, r1]
	bl	Func_080db0b0
	adds	r0, r7, #0
	bl	Func_08020340
	adds	r0, r7, #0
	bl	0x080200c8
.L_080dffd2:
	bl	0x080dc384
.L_080dffd6:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfffc0000
	.4byte 0x0001b333
	.4byte 0x080dfcd5
	.4byte 0x080dfc3d
	.4byte 0x080dfb89
	.2byte 0xfffd
	.2byte 0x080d
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r5, #100
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #0]
	cmp	r3, #80
	bne.n	.L_080e0018
	movs	r3, #0
	str	r3, [r0, #108]
	movs	r0, #136
	bl	Audio_PlayCue
	ldrh	r2, [r5, #0]
.L_080e0018:
	adds	r3, r2, #1
	strh	r3, [r5, #0]
	pop	{r5, pc}
	.2byte 0x0000