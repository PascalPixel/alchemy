.syntax unified
	.thumb
	.global Menu_DrawModeIndicator
	.global Func_0804d754
	.thumb_func
Menu_DrawModeIndicator:
Func_0804d754:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #232
	ldr	r6, [r3, #0]
	adds	r0, r6, #0
	adds	r5, r6, #0
	adds	r5, #140
	adds	r0, #150
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	ldrh	r1, [r5, #0]
	cmp	r2, r3
	beq.n	.L_0804d7ee
	strh	r1, [r0, #0]
	ldr	r0, [r6, #124]
	bl	0x080393fc
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_0804d78a
	cmp	r3, #1
	beq.n	.L_0804d7a8
	b.n	.L_0804d7d2
.L_0804d78a:
	ldr	r5, [pc, #100]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #4
	adds	r5, #1
	bl	UiText_DrawResource
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	UiText_DrawResource
	b.n	.L_0804d7ee
.L_0804d7a8:
	ldr	r5, [pc, #72]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #4
	bl	UiText_DrawResource
	adds	r0, r5, #1
	ldr	r1, [r6, #124]
	movs	r2, #0
	movs	r3, #16
	adds	r5, #2
	bl	UiText_DrawResource
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #28
	bl	UiText_DrawResource
	b.n	.L_0804d7ee
.L_0804d7d2:
	ldr	r5, [pc, #36]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #4
	adds	r5, #1
	bl	UiText_DrawResource
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	UiText_DrawResource
.L_0804d7ee:
	pop	{r5, r6, pc}
	.4byte 0x00001171
	.4byte 0x00001173
	.2byte 0x1176
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	r8, r0
	movs	r3, #1
	mov	r2, r8
	ands	r2, r3
	mov	r8, r2
	sub	sp, #4
	bl	0x0804d0dc
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #232
	ldr	r7, [r3, #0]
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_0804d830
	movs	r0, #44
	bl	0x0804d38c
	movs	r0, #45
	bl	0x0804d38c
	b.n	.L_0804d842
.L_0804d830:
	movs	r0, #46
	bl	0x0804d38c
	movs	r0, #47
	bl	0x0804d38c
	movs	r0, #48
	bl	0x0804d38c
.L_0804d842:
	movs	r2, #0
	movs	r0, #17
	movs	r1, #7
	bl	0x0804d3e8
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_0804d8be
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #118
	ldr	r0, [pc, #252]
	bl	0x080145a8
	movs	r3, #255
	adds	r2, r7, #0
	lsls	r3, r3, #8
	adds	r2, #150
	adds	r3, #255
	strh	r3, [r2, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	mov	sl, r3
	movs	r1, #0
	movs	r2, #14
	movs	r3, #4
	movs	r0, #8
	bl	0x08039260
	ldr	r5, [pc, #220]
	adds	r6, r7, #0
	adds	r1, r0, #0
	adds	r6, #128
	adds	r0, r5, #0
	str	r1, [r6, #0]
	movs	r2, #0
	movs	r3, #4
	bl	UiText_DrawResource
	mov	r2, sl
	str	r2, [sp, #0]
	movs	r1, #4
	movs	r2, #22
	movs	r3, #12
	movs	r0, #4
	bl	0x08039260
	adds	r1, r0, #0
	str	r1, [r7, #124]
	adds	r0, r5, #1
	movs	r2, #0
	movs	r3, #4
	adds	r5, #2
	bl	UiText_DrawResource
	ldr	r1, [r7, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	UiText_DrawResource
	b.n	.L_0804d908
.L_0804d8be:
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #118
	ldr	r0, [pc, #152]
	bl	0x080145a8
	movs	r3, #255
	adds	r2, r7, #0
	lsls	r3, r3, #8
	adds	r2, #150
	adds	r3, #255
	strh	r3, [r2, #0]
	movs	r5, #2
	movs	r1, #0
	movs	r2, #18
	movs	r3, #4
	movs	r0, #6
	str	r5, [sp, #0]
	bl	0x08039260
	adds	r6, r7, #0
	adds	r1, r0, #0
	adds	r6, #128
	str	r1, [r6, #0]
	ldr	r0, [pc, #116]
	movs	r2, #12
	movs	r3, #4
	bl	UiText_DrawResource
	movs	r0, #5
	movs	r1, #5
	movs	r2, #21
	movs	r3, #7
	str	r5, [sp, #0]
	bl	0x08039260
	str	r0, [r7, #124]
.L_0804d908:
	movs	r0, #0
	bl	0x0804d16c
	mov	r3, r8
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L_0804d91e
	ldr	r0, [pc, #64]
	bl	Func_08014644
	b.n	.L_0804d924
.L_0804d91e:
	ldr	r0, [pc, #64]
	bl	Func_08014644
.L_0804d924:
	ldr	r0, [r6, #0]
	bl	0x080393fc
	ldr	r0, [r7, #124]
	bl	0x080393fc
	ldr	r0, [r6, #0]
	movs	r1, #2
	bl	UiWork_Finalize
	movs	r1, #2
	ldr	r0, [r7, #124]
	bl	UiWork_Finalize
	movs	r0, #1
	bl	0x08013560
	bl	0x0804d118
	adds	r0, r5, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0804d679
	.4byte 0x00001179
	.4byte 0x0804d755
	.2byte 0x1178
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl	0x0804d0dc
	movs	r0, #17
	bl	0x0804d38c
	movs	r0, #18
	bl	0x0804d38c
	movs	r0, #19
	bl	0x0804d38c
	movs	r0, #20
	bl	0x0804d38c
	movs	r1, #7
	movs	r2, #0
	movs	r0, #17
	bl	0x0804d3e8
	adds	r0, r5, #0
	bl	0x0804d16c
	adds	r5, r0, #0
	bl	0x0804d118
	adds	r0, r5, #0
	pop	{r5, pc}
	.2byte 0x0000
