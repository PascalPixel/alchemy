.syntax unified
	.thumb
	.global Func_080de060
	.thumb_func
Func_080de060:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	movs	r1, #64
	adds	r1, r1, r7
	sub	sp, #12
	mov	sl, r3
	mov	r8, r1
.L_080de07c:
	mov	r2, r8
	movs	r6, #0
	ldrsb	r6, [r2, r6]
	cmp	r6, #0
	bne.n	.L_080de0be
	ldr	r3, [r7, #20]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r7, #24]
	str	r3, [r5, #8]
	bl	Random16
	adds	r1, r0, #0
	lsls	r1, r1, #16
	movs	r0, #200
	lsrs	r1, r1, #16
	lsls	r0, r0, #13
	adds	r2, r5, #0
	bl	Func_0801489c
	ldr	r3, [r5, #0]
	mov	r1, r8
	str	r3, [r7, #12]
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r7, #36]
	str	r3, [r7, #32]
	adds	r3, r7, #0
	adds	r3, #66
	strb	r6, [r3, #0]
	b.n	.L_080de12e
.L_080de0be:
	cmp	r6, #1
	bne.n	.L_080de0d6
	adds	r0, r7, #0
	bl	Func_080ebe70
	cmp	r0, #0
	bne.n	.L_080de14a
	mov	r2, r8
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_080de07c
.L_080de0d6:
	cmp	r6, #2
	bne.n	.L_080de136
	mov	r3, sl
	ldr	r2, [r3, #16]
	mov	r5, sp
	ldr	r3, [r2, #8]
	movs	r1, #128
	str	r3, [r5, #0]
	lsls	r1, r1, #13
	ldr	r3, [r2, #12]
	movs	r0, #128
	adds	r3, r3, r1
	str	r3, [r5, #4]
	lsls	r0, r0, #12
	ldr	r3, [r2, #16]
	mov	r2, sl
	str	r3, [r5, #8]
	ldrh	r1, [r2, #0]
	adds	r2, r5, #0
	bl	Func_0801489c
	adds	r0, r5, #0
	bl	Func_080dc390
	bl	Random16
	adds	r1, r0, #0
	movs	r0, #128
	adds	r2, r5, #0
	lsls	r0, r0, #11
	bl	Func_0801489c
	ldr	r3, [r5, #0]
	adds	r2, r7, #0
	str	r3, [r7, #12]
	adds	r2, #66
	ldr	r3, [r5, #8]
	mov	r1, r8
	str	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #4
	strh	r3, [r7, #50]
	movs	r3, #1
	strb	r3, [r2, #0]
.L_080de12e:
	ldrb	r3, [r1, #0]
	adds	r3, #1
	strb	r3, [r1, #0]
	b.n	.L_080de14a
.L_080de136:
	cmp	r6, #3
	bne.n	.L_080de14a
	adds	r0, r7, #0
	bl	Func_080ebe70
	cmp	r0, #0
	bne.n	.L_080de14a
	adds	r0, r7, #0
	bl	Func_080ebf68
.L_080de14a:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r0, #134
	sub	sp, #12
	bl	Audio_PlayCue
	ldr	r1, [r5, #8]
	mov	r6, sp
	str	r1, [r6, #0]
	ldr	r4, [pc, #152]
	ldr	r2, [r5, #12]
	movs	r0, #208
	str	r2, [r6, #4]
	lsls	r0, r0, #1
	ldr	r3, [r5, #16]
	adds	r0, #255
	str	r3, [r6, #8]
	adds	r2, r2, r4
	bl	Func_080dc10c
	cmp	r0, #0
	beq.n	.L_080de19a
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #9
	movs	r3, #20
	strh	r3, [r2, #0]
	ldr	r1, [pc, #116]
	bl	Object_SetCallback
.L_080de19a:
	movs	r0, #128
	lsls	r0, r0, #9
	mov	r8, r6
	movs	r7, #11
	mov	sl, r0
.L_080de1a4:
	movs	r0, #209
	mov	r3, r8
	lsls	r0, r0, #1
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	adds	r0, #255
	ldr	r3, [r3, #8]
	bl	Func_080dc10c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080de1f4
	ldr	r1, [pc, #80]
	bl	Object_SetCallback
	bl	Random16
	adds	r2, r6, #0
	adds	r2, #85
	mov	r4, sl
	movs	r3, #0
	add	r0, sl
	str	r4, [r6, #52]
	str	r0, [r6, #48]
	strb	r3, [r2, #0]
	bl	Random16
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r0, #128
	lsls	r0, r0, #12
	lsls	r5, r5, #3
	adds	r5, r5, r0
	bl	Random16
	adds	r1, r5, #0
	adds	r2, r0, #0
	adds	r0, r6, #0
	bl	0x080db974
.L_080de1f4:
	subs	r7, #1
	cmp	r7, #0
	bge.n	.L_080de1a4
	movs	r0, #0
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffe00000
	.4byte 0x080f0e54
	.2byte 0x0e78
	.2byte 0x080f
	push	{lr}
	bl	.L_080de21c
	pop	{pc}
.L_080de21c:
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
	ldr	r0, [r3, #20]
	mov	r9, r3
	str	r0, [sp, #8]
	bl	0x080dc294
	movs	r0, #130
	bl	Audio_PlayCue
	add	r1, sp, #12
	mov	r5, r9
	mov	sl, r1
	movs	r2, #11
	adds	r5, #80
	mov	r6, sl
	mov	r8, r2
.L_080de252:
	mov	r3, r9
	ldr	r2, [r3, #16]
	movs	r4, #128
	ldr	r3, [r2, #8]
	lsls	r4, r4, #13
	str	r3, [r6, #0]
	adds	r0, r6, #0
	ldr	r3, [r2, #12]
	adds	r3, r3, r4
	str	r3, [r6, #4]
	ldr	r3, [r2, #16]
	str	r3, [r6, #8]
	bl	Func_080dc390
	movs	r1, #168
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #8]
	adds	r0, r5, #0
	lsls	r1, r1, #2
	bl	Func_080ebec8
	adds	r0, r5, #0
	ldr	r1, [pc, #116]
	bl	Func_080ebeb4
	adds	r0, r5, #0
	movs	r1, #7
	bl	Func_080ebea8
	ldr	r0, [r5, #0]
	movs	r1, #9
	bl	0x08020280
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r5, #44]
	str	r3, [r5, #40]
	movs	r0, #2
	bl	WaitFrames
	movs	r0, #1
	negs	r0, r0
	add	r8, r0
	mov	r1, r8
	adds	r5, #72
	cmp	r1, #0
	bge.n	.L_080de252
	mov	r3, r9
	ldr	r2, [r3, #16]
	mov	r4, sl
	ldr	r3, [r2, #8]
	movs	r0, #128
	str	r3, [r4, #0]
	lsls	r0, r0, #13
	ldr	r3, [r2, #12]
	adds	r3, r3, r0
	str	r3, [r4, #4]
	movs	r0, #128
	ldr	r3, [r2, #16]
	mov	r2, r9
	str	r3, [r4, #8]
	lsls	r0, r0, #12
	ldrh	r1, [r2, #0]
	mov	r2, sl
	bl	Func_0801489c
	mov	r3, sl
	movs	r0, #139
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	lsls	r0, r0, #1
	ldr	r3, [r3, #8]
	bl	Func_080dc10c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_080de2f8
	bl	0x080dc384
	b.n	.L_080de53a
	.2byte 0xe061
	.2byte 0x080d
.L_080de2f8:
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r6, #28]
	str	r3, [r6, #24]
	mov	r4, r9
	ldrh	r3, [r4, #0]
	ldr	r2, [pc, #44]
	strh	r3, [r6, #6]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #5
	bl	Object_SetMode
	adds	r0, r6, #0
	movs	r1, #3
	bl	0x08020278
	ldr	r3, [r6, #24]
	movs	r0, #128
	lsls	r0, r0, #9
	cmp	r3, r0
	bge.n	.L_080de354
	b.n	.L_080de338
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_080de338:
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r3, r3, r1
	str	r3, [r6, #28]
	str	r3, [r6, #24]
	movs	r0, #1
	bl	WaitFrames
	movs	r2, #255
	ldr	r3, [r6, #24]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	ble.n	.L_080de338
.L_080de354:
	movs	r0, #3
	bl	WaitFrames
	mov	r0, sp
	adds	r0, #24
	str	r0, [sp, #4]
	movs	r3, #0
	movs	r4, #2
	mov	fp, r3
	mov	r8, r4
	add	r7, sp, #32
.L_080de36a:
	movs	r0, #139
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	lsls	r0, r0, #1
	bl	Func_080dc10c
	adds	r5, r0, #0
	str	r0, [r7, #0]
	subs	r7, #4
	cmp	r5, #0
	beq.n	.L_080de3b8
	movs	r3, #240
	lsls	r3, r3, #8
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	mov	r1, r9
	ldrh	r3, [r1, #0]
	movs	r2, #0
	strh	r3, [r5, #6]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r1, #5
	bl	Object_SetMode
	adds	r0, r5, #0
	movs	r1, #2
	bl	0x08020278
	mov	r1, fp
	ldr	r0, [r5, #80]
	bl	Func_080dc0d8
	mov	fp, r0
.L_080de3b8:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r4, r8
	cmp	r4, #0
	bge.n	.L_080de36a
	mov	r3, r9
	mov	r0, fp
	adds	r3, #32
	ldrb	r0, [r0, #16]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	fp, r0
	cmp	r3, #0
	beq.n	.L_080de3fe
	mov	r1, r9
	ldr	r2, [r1, #16]
	mov	r4, sl
	ldr	r3, [r2, #8]
	movs	r0, #128
	str	r3, [r4, #0]
	lsls	r0, r0, #13
	ldr	r3, [r2, #12]
	adds	r3, r3, r0
	str	r3, [r4, #4]
	movs	r0, #224
	ldr	r3, [r2, #16]
	lsls	r0, r0, #14
	str	r3, [r4, #8]
	mov	r2, sl
	ldrh	r1, [r1, #0]
	bl	Func_0801489c
	b.n	.L_080de414
.L_080de3fe:
	mov	r1, r9
	ldr	r3, [r1, #4]
	mov	r2, sl
	str	r3, [r2, #0]
	movs	r4, #128
	ldr	r3, [r1, #8]
	lsls	r4, r4, #13
	adds	r3, r3, r4
	str	r3, [r2, #4]
	ldr	r3, [r1, #12]
	str	r3, [r2, #8]
.L_080de414:
	mov	r0, sl
	ldr	r2, [r0, #4]
	ldr	r1, [r0, #0]
	ldr	r3, [r0, #8]
	adds	r0, r6, #0
	bl	Object_SetPosition
	ldr	r1, [pc, #292]
	adds	r0, r6, #0
	bl	Object_SetCallback
	ldr	r1, [sp, #4]
	movs	r2, #2
	str	r1, [sp, #0]
	mov	r7, sl
	mov	r8, r2
.L_080de434:
	ldr	r4, [sp, #0]
	ldmia	r4!, {r5}
	adds	r3, r4, #0
	str	r3, [sp, #0]
	cmp	r5, #0
	beq.n	.L_080de45a
	movs	r0, #3
	bl	WaitFrames
	ldr	r1, [r7, #0]
	adds	r0, r5, #0
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	bl	Object_SetPosition
	adds	r0, r5, #0
	ldr	r1, [pc, #244]
	bl	Object_SetCallback
.L_080de45a:
	movs	r0, #1
	negs	r0, r0
	add	r8, r0
	mov	r1, r8
	cmp	r1, #0
	bge.n	.L_080de434
	ldr	r3, [r6, #0]
	movs	r2, #0
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_080de486
.L_080de470:
	movs	r0, #1
	bl	WaitFrames
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #59
	bgt.n	.L_080de486
	ldr	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_080de470
.L_080de486:
	ldr	r0, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080de4fa
	mov	r3, r9
	adds	r3, #53
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080de4fa
	mov	r3, r9
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080de4ae
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r0, #40]
.L_080de4ae:
	ldr	r1, [sp, #8]
	mov	r2, sl
	ldr	r3, [r1, #8]
	movs	r0, #128
	str	r3, [r2, #0]
	lsls	r0, r0, #13
	ldr	r3, [r1, #12]
	str	r3, [r2, #4]
	ldr	r3, [r1, #16]
	str	r3, [r2, #8]
	mov	r3, r9
	ldrh	r1, [r3, #0]
	bl	Func_0801489c
	mov	r1, sl
	ldr	r0, [sp, #8]
	bl	Func_08020210
	cmp	r0, #0
	bne.n	.L_080de4fa
	ldr	r0, [sp, #8]
	mov	r1, sl
	bl	0x08020298
	cmp	r0, #0
	bne.n	.L_080de4fa
	ldr	r4, [sp, #8]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r4, #52]
	str	r3, [r4, #48]
	mov	r0, sl
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	ldr	r3, [r0, #8]
	ldr	r0, [sp, #8]
	bl	Object_SetPosition
.L_080de4fa:
	movs	r0, #4
	bl	0x080ce31c
	adds	r2, r0, #0
	movs	r0, #160
	lsls	r0, r0, #23
	adds	r0, #5
	movs	r1, #4
	bl	Func_080ce458
	cmp	r0, #0
	beq.n	.L_080de520
	mov	r3, r9
	movs	r2, #24
	ldrsh	r1, [r3, r2]
	movs	r4, #26
	ldrsh	r2, [r3, r4]
	bl	0x080ceafc
.L_080de520:
	movs	r0, #10
	bl	WaitFrames
	bl	0x080dc384
	movs	r0, #20
	bl	WaitFrames
	mov	r0, fp
	cmp	r0, #96
	beq.n	.L_080de53a
	bl	Func_08014274
.L_080de53a:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x080f0ef8
	.2byte 0x0e58
	.2byte 0x080f
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r2, [r3, #0]
	movs	r3, #7
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080de566
	movs	r1, #2
	bl	0x08020278
	b.n	.L_080de570
.L_080de566:
	cmp	r2, #2
	bne.n	.L_080de570
	movs	r1, #0
	bl	0x08020278
.L_080de570:
	movs	r0, #0
	pop	{pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	cmp	r0, #0
	beq.n	.L_080de59a
	ldr	r1, [pc, #28]
	ldr	r2, [r0, #28]
	ldr	r3, [r0, #24]
	adds	r2, r2, r1
	str	r2, [r0, #28]
	movs	r2, #128
	adds	r3, r3, r1
	lsls	r2, r2, #5
	str	r3, [r0, #24]
	cmp	r3, r2
	bgt.n	.L_080de59a
	ldr	r1, [pc, #8]
	bl	Object_SetCallback
.L_080de59a:
	pop	{pc}
	.4byte 0xfffff000
	.2byte 0x0e54
	.2byte 0x080f
	push	{r5, r6, lr}
	adds	r6, r0, #0
	sub	sp, #12
	cmp	r6, #0
	beq.n	.L_080de5f4
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	subs	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	cmp	r2, #0
	beq.n	.L_080de5f0
	ldr	r3, [r6, #56]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r6, #60]
	lsls	r0, r2, #17
	str	r3, [r5, #4]
	ldr	r3, [r6, #64]
	str	r3, [r5, #8]
	adds	r3, r6, #0
	adds	r3, #102
	movs	r4, #0
	ldrsh	r1, [r3, r4]
	lsls	r3, r2, #11
	adds	r1, r1, r3
	adds	r2, r5, #0
	bl	Func_0801489c
	ldr	r3, [r5, #0]
	str	r3, [r6, #8]
	ldr	r3, [r5, #4]
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	b.n	.L_080de5f4
.L_080de5f0:
	ldr	r3, [pc, #4]
	str	r3, [r6, #108]
.L_080de5f4:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0xe579
	.2byte 0x080d
	push	{r5, r6, lr}
	adds	r6, r0, #0
	sub	sp, #12
	cmp	r6, #0
	beq.n	.L_080de64c
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	subs	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	cmp	r2, #0
	beq.n	.L_080de648
	ldr	r3, [r6, #56]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r6, #60]
	lsls	r0, r2, #17
	str	r3, [r5, #4]
	ldr	r3, [r6, #64]
	str	r3, [r5, #8]
	adds	r3, r6, #0
	adds	r3, #102
	movs	r4, #0
	ldrsh	r1, [r3, r4]
	lsls	r3, r2, #11
	subs	r1, r1, r3
	adds	r2, r5, #0
	bl	Func_0801489c
	ldr	r3, [r5, #0]
	str	r3, [r6, #8]
	ldr	r3, [r5, #4]
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	b.n	.L_080de64c
.L_080de648:
	ldr	r3, [pc, #4]
	str	r3, [r6, #108]
.L_080de64c:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0xe579
	.2byte 0x080d
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r2, [r3, #0]
	ldr	r1, [r2, #20]
	cmp	r1, #0
	beq.n	.L_080de686
	adds	r3, r2, #0
	adds	r3, #53
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080de678
	adds	r2, #32
	movs	r3, #1
	strb	r3, [r2, #0]
.L_080de678:
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	bl	.L_080de688
.L_080de686:
	pop	{pc}
.L_080de688:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #12
	ldr	r2, [r3, #20]
	mov	r8, r3
	mov	r9, r2
	bl	0x080dc294
	movs	r0, #115
	bl	Audio_PlayCue
	movs	r3, #15
	mov	r7, sp
	mov	sl, r3
.L_080de6b2:
	movs	r0, #213
	lsls	r0, r0, #1
	adds	r0, #255
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_080dc10c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080de742
	bl	Random16
	movs	r4, #128
	lsls	r4, r4, #8
	lsrs	r0, r0, #1
	adds	r0, r0, r4
	str	r0, [r6, #28]
	str	r0, [r6, #24]
	bl	Random16
	movs	r3, #1
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_080de6e8
	ldr	r3, [pc, #292]
	b.n	.L_080de6ea
.L_080de6e8:
	ldr	r3, [pc, #292]
.L_080de6ea:
	str	r3, [r6, #108]
	bl	Random16
	adds	r2, r6, #0
	adds	r2, #100
	movs	r3, #60
	strh	r0, [r6, #6]
	strh	r3, [r2, #0]
	bl	Random16
	adds	r3, r6, #0
	adds	r3, #102
	movs	r1, #9
	strh	r0, [r3, #0]
	adds	r0, r6, #0
	bl	0x08020278
	mov	r2, r8
	ldr	r3, [r2, #4]
	str	r3, [r7, #0]
	ldr	r3, [r2, #8]
	str	r3, [r7, #4]
	ldr	r3, [r2, #12]
	str	r3, [r7, #8]
	bl	Random16
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r3, r3, #10
	lsls	r5, r5, #2
	adds	r5, r5, r3
	bl	Random16
	adds	r2, r7, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_0801489c
	ldr	r3, [r7, #0]
	str	r3, [r6, #56]
	ldr	r3, [r7, #4]
	str	r3, [r6, #60]
	ldr	r3, [r7, #8]
	str	r3, [r6, #64]
.L_080de742:
	movs	r0, #3
	bl	WaitFrames
	movs	r4, #1
	negs	r4, r4
	add	sl, r4
	mov	r2, sl
	cmp	r2, #0
	bge.n	.L_080de6b2
	movs	r0, #10
	bl	WaitFrames
	movs	r0, #115
	bl	Audio_PlayCue
	movs	r0, #50
	bl	WaitFrames
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_080de7fa
	mov	r3, r8
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080de7fa
	movs	r0, #212
	bl	Audio_PlayCue
	movs	r4, #7
	mov	sl, r4
.L_080de784:
	movs	r1, #7
	mov	r0, r9
	bl	0x08020278
	movs	r0, #1
	bl	WaitFrames
	mov	r0, r9
	movs	r1, #0
	bl	0x08020278
	movs	r0, #4
	bl	WaitFrames
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_080de784
	mov	r3, r8
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080de7c8
	movs	r0, #220
	bl	Audio_PlayCue
	mov	r0, r9
	movs	r1, #2
	bl	Object_SetMode
.L_080de7c8:
	ldr	r3, [pc, #72]
	mov	r4, r9
	str	r3, [r4, #108]
	movs	r0, #6
	bl	0x080ce31c
	adds	r2, r0, #0
	movs	r0, #160
	lsls	r0, r0, #23
	adds	r0, #5
	movs	r1, #6
	bl	Func_080ce458
	cmp	r0, #0
	beq.n	.L_080de7f4
	mov	r3, r8
	movs	r2, #24
	ldrsh	r1, [r3, r2]
	movs	r4, #26
	ldrsh	r2, [r3, r4]
	bl	0x080ceafc
.L_080de7f4:
	movs	r0, #20
	bl	WaitFrames
.L_080de7fa:
	bl	0x080dc384
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080de5a5
	.4byte 0x080de5fd
	.2byte 0xe551
	.2byte 0x080d
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r5, #0
	adds	r7, #100
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	movs	r6, #128
	lsls	r0, r0, #9
	bl	Math_Sine
	lsls	r6, r6, #11
	adds	r1, r0, #0
	ldr	r3, [pc, #44]
	adds	r0, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6bab
	adds	r3, r3, r0
	str	r3, [r5, #8]
	ldrh	r3, [r7, #0]
	adds	r3, #1
	strh	r3, [r7, #0]
	lsls	r3, r3, #16
	asrs	r1, r3, #16
	adds	r2, r1, #0
	adds	r2, #128
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080de856
	adds	r3, r1, #0
	adds	r3, #255
.L_080de856:
	asrs	r3, r3, #7
	lsls	r3, r3, #7
	subs	r3, r2, r3
	strh	r3, [r7, #0]
	pop	{r5, r6, r7, pc}
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r6, r0, #0
	sub	sp, #12
	ldr	r1, [r3, #0]
	cmp	r6, #0
	beq.n	.L_080de8c6
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	subs	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	cmp	r2, #0
	beq.n	.L_080de8be
	ldr	r3, [r1, #4]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r0, #160
	ldr	r3, [r1, #8]
	lsls	r0, r0, #12
	adds	r3, r3, r0
	str	r3, [r5, #4]
	lsls	r0, r2, #16
	ldr	r3, [r1, #12]
	str	r3, [r5, #8]
	adds	r3, r6, #0
	adds	r3, #102
	movs	r4, #0
	ldrsh	r1, [r3, r4]
	lsls	r3, r2, #11
	adds	r1, r1, r3
	adds	r2, r5, #0
	bl	Func_0801489c
	ldr	r3, [r5, #0]
	str	r3, [r6, #8]
	ldr	r3, [r5, #4]
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	b.n	.L_080de8c6
.L_080de8be:
	ldr	r1, [pc, #12]
	adds	r0, r6, #0
	bl	Object_SetCallback
.L_080de8c6:
	add	sp, #12
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0e54
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	adds	r7, r0, #0
	lsls	r3, r3, #18
	movs	r2, #100
	adds	r2, r2, r7
	adds	r3, #224
	ldr	r6, [r3, #0]
	movs	r3, #0
	ldrsh	r0, [r2, r3]
	mov	r8, r2
	movs	r2, #1
	negs	r2, r2
	sub	sp, #12
	cmp	r0, r2
	beq.n	.L_080de940
	lsls	r0, r0, #10
	bl	Math_Sine
	movs	r5, #192
	lsls	r5, r5, #11
	adds	r1, r0, #0
	ldr	r3, [pc, #216]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6873
	movs	r2, #128
	adds	r3, r3, r0
	str	r3, [r7, #8]
	lsls	r2, r2, #13
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r7, #12]
	mov	r2, r8
	ldr	r3, [r6, #12]
	str	r3, [r7, #16]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r1, r3, #16
	adds	r2, r1, #0
	adds	r2, #64
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080de936
	adds	r3, r1, #0
	adds	r3, #127
.L_080de936:
	asrs	r3, r3, #6
	lsls	r3, r3, #6
	subs	r3, r2, r3
	mov	r2, r8
	strh	r3, [r2, #0]
.L_080de940:
	ldr	r3, [pc, #156]
	movs	r1, #3
	ldr	r0, [r3, #0]
	bl	0x0800206c
	cmp	r0, #0
	bne.n	.L_080de9d4
	ldr	r3, [r7, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
	bl	Random16
	lsls	r5, r0, #1
	adds	r5, r5, r0
	bl	Random16
	lsls	r5, r5, #1
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	Func_0801489c
	movs	r0, #209
	lsls	r0, r0, #1
	adds	r0, #255
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl	Func_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080de9d4
	ldr	r3, [pc, #80]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	movs	r3, #153
	lsls	r3, r3, #8
	adds	r3, #153
	adds	r2, #85
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #229
	lsls	r3, r3, #1
	str	r3, [r5, #72]
	bl	Random16
	adds	r3, r5, #0
	lsrs	r0, r0, #9
	adds	r3, #100
	strh	r0, [r3, #0]
	movs	r1, #9
	ldr	r3, [r5, #8]
	adds	r0, r5, #0
	str	r3, [r5, #56]
	bl	0x08020278
	adds	r2, r5, #0
	adds	r2, #94
	movs	r3, #72
	strh	r3, [r2, #0]
	ldr	r1, [pc, #24]
	adds	r0, r5, #0
	bl	Object_SetCallback
.L_080de9d4:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.4byte 0x0300122c
	.4byte 0x080de819
	.2byte 0x0e54
	.2byte 0x080f
	push	{lr}
	bl	.L_080de9f8
	bl	Func_080dc7cc
	pop	{pc}
.L_080de9f8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r7, #0
	mov	sl, r3
	ldr	r6, [r3, #16]
	bl	0x080dc294
	movs	r2, #0
	mov	r8, r2
.L_080dea18:
	ldr	r2, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #14
	movs	r0, #148
	adds	r2, r2, r3
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	lsls	r0, r0, #1
	bl	Func_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080dea5e
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	ldr	r3, [pc, #348]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #100
	movs	r3, #120
	strh	r3, [r2, #0]
	lsls	r3, r7, #13
	adds	r2, #2
	strh	r3, [r2, #0]
	subs	r2, #17
	movs	r3, #4
	strb	r3, [r2, #0]
	mov	r1, r8
	ldr	r0, [r5, #80]
	bl	Func_080dc0d8
	mov	r8, r0
.L_080dea5e:
	movs	r0, #1
	adds	r7, #1
	bl	WaitFrames
	cmp	r7, #7
	ble.n	.L_080dea18
	mov	r2, r8
	ldrb	r2, [r2, #16]
	movs	r0, #130
	mov	r9, r2
	bl	Audio_PlayCue
	movs	r0, #110
	bl	WaitFrames
	movs	r0, #148
	lsls	r0, r0, #1
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_080dc10c
	adds	r6, r0, #0
	adds	r5, r6, #0
	cmp	r6, #0
	beq.n	.L_080deac0
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r6, #28]
	str	r3, [r6, #24]
	mov	r2, sl
	ldr	r3, [r2, #4]
	movs	r1, #7
	str	r3, [r6, #8]
	ldr	r3, [r2, #8]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r6, #12]
	mov	r2, sl
	ldr	r3, [r2, #12]
	adds	r2, r6, #0
	str	r3, [r6, #16]
	adds	r2, #85
	movs	r3, #4
	strb	r3, [r2, #0]
	bl	0x08020278
.L_080deac0:
	movs	r0, #131
	bl	Audio_PlayCue
	movs	r0, #12
	bl	WaitFrames
	cmp	r6, #0
	beq.n	.L_080deafe
	movs	r3, #3
	movs	r7, #0
	mov	r8, r3
.L_080dead6:
	adds	r3, r7, #0
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080deaea
	adds	r0, r5, #0
	movs	r1, #9
	bl	0x08020278
	b.n	.L_080deaf2
.L_080deaea:
	adds	r0, r5, #0
	movs	r1, #10
	bl	0x08020278
.L_080deaf2:
	movs	r0, #2
	adds	r7, #1
	bl	WaitFrames
	cmp	r7, #29
	ble.n	.L_080dead6
.L_080deafe:
	adds	r0, r5, #0
	movs	r1, #0
	bl	0x08020278
	movs	r0, #84
	bl	Audio_PlayCue
	cmp	r5, #0
	beq.n	.L_080deb38
	ldr	r3, [pc, #140]
	adds	r2, r6, #0
	str	r3, [r6, #108]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	mov	r3, sl
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080deb32
	movs	r0, #128
	bl	WaitFrames
	b.n	.L_080deb38
.L_080deb32:
	movs	r0, #192
	bl	WaitFrames
.L_080deb38:
	cmp	r6, #0
	beq.n	.L_080deb76
	movs	r3, #255
	adds	r2, r6, #0
	lsls	r3, r3, #8
	adds	r2, #100
	adds	r3, #255
	strh	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r6, #48]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r6, #52]
	subs	r2, #10
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #192
	movs	r2, #232
	lsls	r1, r1, #16
	lsls	r2, r2, #8
	adds	r0, r6, #0
	bl	0x080db974
	adds	r0, r6, #0
	bl	Object_CommitPosition
	adds	r0, r6, #0
	bl	0x080200c8
.L_080deb76:
	mov	r3, r9
	cmp	r3, #96
	beq.n	.L_080deb82
	mov	r0, r9
	bl	Func_08014274
.L_080deb82:
	mov	r2, sl
	ldr	r3, [r2, #36]
	cmp	r3, #0
	beq.n	0x080deb8e
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf7fd
	.2byte 0xfbf9
	.2byte 0xbc68
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x080de865
	.2byte 0xe8d1
	.2byte 0x080d
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl	Func_080cdf5c
	bl	ObjectTable_Get
	ldr	r3, [r0, #8]
	ldr	r1, [r0, #16]
	adds	r0, #34
	ldrb	r2, [r0, #0]
	adds	r0, r3, #0
	bl	0x080dbca8
	cmp	r0, #0
	bne.n	.L_080debd4
	movs	r3, #179
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r3, #146
	strh	r3, [r2, #0]
.L_080debd4:
	pop	{r5, pc}
	.2byte 0x0000
