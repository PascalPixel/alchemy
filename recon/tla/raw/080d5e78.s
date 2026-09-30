.syntax unified
	.thumb
	.global ObjectEffect_EndContextEffect
	.thumb_func
ObjectEffect_EndContextEffect:
	.global ObjectEffect_EndContextEffect
	.thumb_func
ObjectEffect_EndContextEffect:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #136]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl	ObjectTable_Get
	adds	r5, r0, #0
	ldr	r2, [r5, #80]
	movs	r1, #12
	mov	r8, r2
	adds	r1, #255
	mov	r0, r8
	bl	ResourceMetadata_RegisterFar
	movs	r3, #0
	mov	sl, r3
	mov	r1, sl
	mov	r2, r8
	strb	r1, [r2, #26]
	movs	r3, #15
	ldr	r2, [pc, #96]
	strb	r3, [r0, #5]
	ldr	r3, [r5, #8]
	movs	r1, #128
	lsls	r1, r1, #12
	ands	r3, r2
	mov	r9, r1
	add	r3, r9
	str	r3, [r5, #8]
	ldr	r3, [r5, #16]
	adds	r1, r6, #0
	ands	r3, r2
	str	r3, [r5, #16]
	adds	r0, r5, #0
	bl	Object_SetMode
	movs	r0, #30
	bl	WaitFrames
	movs	r6, #1
	mov	r2, r8
	strb	r6, [r2, #27]
	ldr	r0, [r2, #44]
	bl	0x08020070
	mov	r1, r8
	strb	r6, [r1, #26]
	mov	r3, sl
	str	r3, [r1, #44]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	str	r3, [r5, #48]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	add	r3, r9
	bl	Object_SetPosition
	adds	r0, r5, #0
	bl	Object_CommitPosition
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0