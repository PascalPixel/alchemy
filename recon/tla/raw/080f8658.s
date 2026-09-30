.syntax unified
	.thumb
	.global Func_080f8658
	.thumb_func
Func_080f8658:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	movs	r2, #0
	ldr	r0, [pc, #152]
	movs	r3, #40
	adds	r6, r1, #0
	bl	UiText_DrawStringAtOffsetFar
	ldr	r3, [pc, #144]
	adds	r1, r6, #0
	mov	r8, r3
	mov	r0, r8
	movs	r3, #40
	movs	r2, #48
	bl	0x08038098
	movs	r3, #52
	ldrsh	r5, [r7, r3]
	adds	r1, r6, #0
	movs	r3, #40
	adds	r0, r5, #0
	movs	r2, #88
	bl	0x080f8610
	movs	r3, #56
	ldrsh	r5, [r7, r3]
	ldrh	r3, [r7, #52]
	lsls	r3, r3, #16
	asrs	r3, r3, #18
	cmp	r5, r3
	bge.n	.L_080f86a0
	movs	r0, #4
	bl	0x080380b8
.L_080f86a0:
	cmp	r5, #0
	bne.n	.L_080f86aa
	movs	r0, #2
	bl	0x080380b8
.L_080f86aa:
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #48
	movs	r3, #40
	bl	0x080f8610
	movs	r0, #15
	bl	0x080380b8
	adds	r1, r6, #0
	ldr	r0, [pc, #68]
	movs	r2, #0
	movs	r3, #48
	bl	UiText_DrawStringAtOffsetFar
	mov	r0, r8
	adds	r1, r6, #0
	movs	r3, #48
	movs	r2, #48
	bl	0x08038098
	movs	r3, #58
	ldrsh	r5, [r7, r3]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r3, #48
	movs	r2, #48
	bl	0x080f8610
	movs	r3, #54
	ldrsh	r5, [r7, r3]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #88
	movs	r3, #48
	bl	0x080f8610
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0810593c
	.4byte 0x08105940
	.2byte 0x5944
	.2byte 0x0810
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r2, #0
	adds	r5, r1, #0
	mov	r8, r0
	adds	r1, r6, #0
	ldr	r0, [pc, #280]
	movs	r2, #0
	movs	r3, #32
	sub	sp, #4
	bl	0x08038080
	movs	r7, #40
	ldrh	r0, [r5, #60]
	adds	r2, r6, #0
	movs	r3, #16
	movs	r1, #3
	str	r7, [sp, #0]
	bl	UiText_DrawNumberAtOffsetFar
	mov	r3, r8
	ldrh	r2, [r3, #60]
	ldrh	r3, [r5, #60]
	cmp	r2, r3
	beq.n	.L_080f8770
	adds	r0, r2, #0
	movs	r3, #64
	adds	r2, r6, #0
	movs	r1, #3
	str	r7, [sp, #0]
	bl	UiText_DrawNumberAtOffsetFar
	mov	r3, r8
	ldrh	r2, [r3, #60]
	ldrh	r3, [r5, #60]
	cmp	r2, r3
	bls.n	.L_080f8764
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #36
	movs	r3, #0
	bl	Func_08104b58
	b.n	.L_080f8770
.L_080f8764:
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #36
	movs	r3, #1
	bl	Func_08104b58
.L_080f8770:
	ldr	r0, [pc, #196]
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #48
	bl	0x08038080
	movs	r7, #56
	ldrh	r0, [r5, #62]
	adds	r2, r6, #0
	movs	r3, #16
	movs	r1, #3
	str	r7, [sp, #0]
	bl	UiText_DrawNumberAtOffsetFar
	mov	r3, r8
	ldrh	r2, [r3, #62]
	ldrh	r3, [r5, #62]
	cmp	r2, r3
	beq.n	.L_080f87c8
	adds	r0, r2, #0
	movs	r3, #64
	adds	r2, r6, #0
	movs	r1, #3
	str	r7, [sp, #0]
	bl	UiText_DrawNumberAtOffsetFar
	mov	r3, r8
	ldrh	r2, [r3, #62]
	ldrh	r3, [r5, #62]
	cmp	r2, r3
	bls.n	.L_080f87bc
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #52
	movs	r3, #0
	bl	Func_08104b58
	b.n	.L_080f87c8
.L_080f87bc:
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #52
	movs	r3, #1
	bl	Func_08104b58
.L_080f87c8:
	ldr	r0, [pc, #112]
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #64
	bl	0x08038080
	adds	r7, r5, #0
	movs	r3, #72
	adds	r7, #64
	mov	r5, r8
	ldrh	r0, [r7, #0]
	adds	r2, r6, #0
	str	r3, [sp, #0]
	mov	sl, r3
	movs	r1, #3
	movs	r3, #16
	adds	r5, #64
	bl	UiText_DrawNumberAtOffsetFar
	ldrh	r2, [r5, #0]
	ldrh	r3, [r7, #0]
	cmp	r2, r3
	beq.n	.L_080f8828
	mov	r3, sl
	adds	r0, r2, #0
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r3, #64
	movs	r1, #3
	bl	UiText_DrawNumberAtOffsetFar
	ldrh	r2, [r5, #0]
	ldrh	r3, [r7, #0]
	cmp	r2, r3
	bls.n	.L_080f881c
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #68
	movs	r3, #0
	bl	Func_08104b58
	b.n	.L_080f8828
.L_080f881c:
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #68
	movs	r3, #1
	bl	Func_08104b58
.L_080f8828:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000104b
	.4byte 0x0000104c
	.2byte 0x104f
	.2byte 0x0000