.syntax unified
	.thumb
	.global Func_0803a448
	.thumb_func
Func_0803a448:
	push	{r5, lr}
	movs	r3, #192
	movs	r1, #128
	lsls	r3, r3, #18
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #95
	ldr	r5, [r3, #60]
	bl	VramBlock_LoadCached
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #72
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	movs	r3, #154
	lsls	r3, r3, #5
	adds	r2, r5, r3
	movs	r3, #9
	strh	r3, [r2, #0]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #56
	adds	r2, r5, r3
	movs	r3, #10
	strh	r3, [r2, #0]
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r2, #60
	adds	r3, r5, r2
	movs	r1, #0
	strh	r1, [r3, #0]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #62
	adds	r2, r5, r3
	movs	r3, #15
	strh	r3, [r2, #0]
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #66
	adds	r5, r5, r2
	strh	r1, [r5, #0]
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #8]
	bl	Func_080145a8
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0xa8c9
	.2byte 0x0803
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r6, r0, #0
	ldr	r5, [r3, #60]
	cmp	r6, #0
	beq.n	.L_0803a4d4
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #95
	bl	VramBlock_LoadCached
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #72
	adds	r3, r5, r2
	strh	r0, [r3, #0]
.L_0803a4d4:
	movs	r3, #154
	lsls	r3, r3, #5
	adds	r2, r5, r3
	movs	r3, #9
	strh	r3, [r2, #0]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #56
	adds	r2, r5, r3
	movs	r3, #10
	strh	r3, [r2, #0]
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r2, #60
	adds	r3, r5, r2
	movs	r1, #0
	strh	r1, [r3, #0]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #62
	adds	r2, r5, r3
	movs	r3, #15
	strh	r3, [r2, #0]
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #66
	adds	r3, r5, r2
	strh	r1, [r3, #0]
	cmp	r6, #0
	beq.n	.L_0803a51c
	movs	r1, #144
	ldr	r0, [pc, #20]
	lsls	r1, r1, #3
	bl	Func_080145a8
	b.n	.L_0803a526
.L_0803a51c:
	movs	r1, #144
	ldr	r0, [pc, #12]
	lsls	r1, r1, #3
	bl	Func_080145a8
.L_0803a526:
	pop	{r5, r6, pc}
	.4byte 0x0803a8c9
	.2byte 0xa8d9
	.2byte 0x0803
	.global Func_0803a530
	.thumb_func
Func_0803a530:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #148
	ldr	r5, [r3, #0]
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_0803a54a
	movs	r1, #1
	bl	UiWork_Finalize
	movs	r3, #0
	str	r3, [r5, #0]
.L_0803a54a:
	pop	{r5, pc}
	.global Func_0803a54c
	.thumb_func
Func_0803a54c:
.L_0803a54c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #60]
	adds	r3, #148
	ldr	r6, [r3, #0]
	movs	r3, #2
	strb	r3, [r5, #5]
	movs	r1, #1
	sub	sp, #4
	bl	UiText_BuildRenderEntries
	movs	r2, #1
	mov	sl, r2
	mov	r3, sl
	strb	r3, [r5, #5]
	movs	r2, #244
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r3, [r5, r3]
	movs	r7, #0
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_0803a5d6
	ldr	r0, [r6, #0]
	cmp	r0, #0
	bne.n	.L_0803a5b2
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #15
	movs	r2, #30
	movs	r3, #6
	movs	r0, #0
	bl	0x08039260
	mov	r3, sl
	adds	r5, r0, #0
	str	r5, [r6, #0]
	movs	r0, #0
	str	r3, [sp, #0]
	movs	r1, #15
	movs	r2, #30
	movs	r3, #6
	bl	Func_0803a2b0
	str	r7, [r6, #8]
	b.n	.L_0803a5b4
.L_0803a5b2:
	adds	r5, r0, #0
.L_0803a5b4:
	cmp	r5, #0
	beq.n	.L_0803a5d6
	ldr	r2, [r6, #8]
	adds	r0, r5, #0
	mov	r1, r8
	bl	Func_080395fc
	movs	r3, #0
	adds	r7, r0, #0
	str	r7, [r6, #4]
	str	r3, [r6, #8]
	cmp	r7, #0
	bne.n	.L_0803a5d6
	adds	r0, r5, #0
	movs	r1, #1
	bl	UiWork_Finalize
.L_0803a5d6:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.global Func_0803a5e0
	.thumb_func
Func_0803a5e0:
	push	{r5, lr}
	adds	r5, r0, #0
	b.n	.L_0803a5ec
.L_0803a5e6:
	movs	r0, #1
	bl	WaitFrames
.L_0803a5ec:
	bl	UiWork_IsComplete
	cmp	r0, #0
	beq.n	.L_0803a5e6
	adds	r0, r5, #0
	bl	Func_0803a54c
	b.n	.L_0803a602
.L_0803a5fc:
	movs	r0, #1
	bl	WaitFrames
.L_0803a602:
	bl	UiWork_IsComplete
	cmp	r0, #0
	beq.n	.L_0803a5fc
	pop	{r5, pc}
	.global Func_0803a60c
	.thumb_func
Func_0803a60c:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #60]
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #132
	adds	r3, r5, r2
	movs	r6, #0
	adds	r2, #2
	strh	r6, [r3, #0]
	adds	r3, r5, r2
	strh	r6, [r3, #0]
	adds	r7, r0, #0
	adds	r0, r1, #0
	movs	r1, #1
	sub	sp, #16
	bl	UiText_BuildRenderEntries
	movs	r2, #244
	adds	r1, r0, #0
	lsls	r3, r1, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r3, [r5, r3]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_0803a662
	cmp	r7, #0
	beq.n	.L_0803a662
	movs	r3, #1
	str	r3, [sp, #4]
	adds	r0, r7, #0
	movs	r2, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl	0x0803954c
	adds	r6, r0, #0
	movs	r0, #0
	cmp	r6, #0
	beq.n	.L_0803a662
	adds	r0, r6, #0
.L_0803a662:
	add	sp, #16
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
