.syntax unified
	.thumb
	.global Func_080ab314
	.thumb_func
Func_080ab314:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #660]
	ldr	r1, [r3, #0]
	sub	sp, #32
	str	r1, [sp, #20]
	subs	r3, #160
	ldr	r3, [r3, #0]
	movs	r2, #0
	str	r3, [sp, #16]
	str	r2, [sp, #12]
	str	r2, [sp, #8]
	ldr	r0, [r1, #48]
	bl	RenderOutput_ClearListFar
	movs	r0, #1
	bl	WaitFrames
	movs	r1, #134
	ldr	r3, [sp, #20]
	lsls	r1, r1, #1
	adds	r6, r3, r1
	ldr	r0, [r6, #0]
	bl	UiWindow_Clear
	ldr	r5, [pc, #616]
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	UiText_DrawAt
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	UiText_DrawAt
	movs	r5, #6
	movs	r0, #1
	movs	r1, #1
	movs	r2, #11
	movs	r3, #3
	str	r5, [sp, #0]
	bl	UiWindow_SetRectPalette
	ldr	r2, [sp, #20]
	movs	r3, #10
	ldr	r0, [r2, #48]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #0
	movs	r3, #28
	str	r5, [sp, #4]
	bl	UiWindow_ApplyRectAtObjectOrigin
	movs	r1, #9
	movs	r2, #8
	movs	r3, #10
	movs	r0, #0
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	movs	r5, #2
	movs	r1, #12
	movs	r2, #22
	movs	r3, #7
	adds	r6, r0, #0
	movs	r0, #8
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	movs	r3, #3
	str	r0, [sp, #28]
	movs	r1, #9
	movs	r2, #22
	movs	r0, #8
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	str	r0, [sp, #24]
	bl	Func_080152a8
	ldr	r3, [pc, #504]
	movs	r7, #0
	movs	r5, #0
	mov	r8, r3
.L_080ab3ce:
	mov	r1, r8
	adds	r0, r5, r1
	lsls	r3, r5, #3
	adds	r1, r6, #0
	movs	r2, #0
	adds	r5, #1
	bl	UiText_DrawAt
	cmp	r5, #6
	ble.n	.L_080ab3ce
	ldr	r3, [pc, #480]
	movs	r2, #1
	movs	r1, #0
	mov	fp, r2
	mov	r9, r3
	mov	r8, r1
.L_080ab3ee:
	ldr	r0, [sp, #24]
	bl	UiWindow_Clear
	ldr	r0, [pc, #456]
	ldr	r1, [sp, #24]
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, r0
	bl	UiText_DrawMessageAt
	ldr	r1, [pc, #452]
	ldr	r0, [sp, #28]
	adds	r1, r7, r1
	bl	0x080153f8
	mov	r2, fp
	movs	r3, #15
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r2, [sp, #8]
	movs	r1, #0
	movs	r3, #6
	mov	sl, r0
	adds	r0, r6, #0
	bl	Menu_DrawAtWindowOffset
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r3, #14
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #0
	adds	r2, r7, #0
	movs	r3, #6
	bl	Menu_DrawAtWindowOffset
	str	r7, [sp, #8]
	b.n	.L_080ab4b8
.L_080ab43a:
	mov	r1, r9
	ldr	r2, [r1, #0]
	movs	r3, #96
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ab45a
	subs	r7, #1
	adds	r0, r7, #0
	movs	r1, #7
	bl	Menu_GetModuloOfSum
	adds	r7, r0, #0
	movs	r0, #111
	bl	Audio_PlayCue
	b.n	.L_080ab4ec
.L_080ab45a:
	ldr	r1, [pc, #368]
	ldr	r2, [r1, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ab474
	movs	r0, #113
	bl	Audio_PlayCue
	movs	r2, #2
	negs	r2, r2
	str	r2, [sp, #12]
	b.n	.L_080ab4ec
.L_080ab474:
	ldr	r2, [r1, #0]
	movs	r3, #6
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ab48c
	movs	r0, #113
	bl	Audio_PlayCue
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #12]
	b.n	.L_080ab4ec
.L_080ab48c:
	ldr	r3, [r1, #0]
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ab4b8
	bl	UiWork_IsCompleteFar
	cmp	r0, #0
	beq.n	.L_080ab4b2
	adds	r7, #1
	adds	r0, r7, #0
	movs	r1, #7
	bl	Menu_GetModuloOfSum
	adds	r7, r0, #0
	movs	r0, #112
	bl	Audio_PlayCue
	b.n	.L_080ab4ec
.L_080ab4b2:
	movs	r0, #111
	bl	Audio_PlayCue
.L_080ab4b8:
	ldrh	r1, [r6, #14]
	adds	r1, r1, r7
	lsls	r1, r1, #3
	movs	r0, #12
	adds	r1, #8
	negs	r0, r0
	bl	UiMenu_PositionCursor
	movs	r0, #1
	bl	WaitFrames
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #144
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ab43a
	adds	r7, #1
	adds	r0, r7, #0
	movs	r1, #7
	bl	Menu_GetModuloOfSum
	adds	r7, r0, #0
	movs	r0, #111
	bl	Audio_PlayCue
.L_080ab4ec:
	ldr	r1, [sp, #16]
	ldr	r2, [pc, #224]
	adds	r5, r1, r2
	ldrh	r3, [r5, #0]
	cmp	r3, #99
	beq.n	.L_080ab502
	adds	r0, r3, #0
	bl	Resource_ResetEntry
	movs	r3, #99
	strh	r3, [r5, #0]
.L_080ab502:
	ldr	r5, [pc, #208]
	ldr	r1, [pc, #208]
	ldr	r3, [r5, #0]
	mov	r2, r8
	adds	r3, r3, r1
	strb	r2, [r3, #0]
	ldr	r0, [sp, #28]
	bl	UiWindow_Clear
	mov	r1, sl
	ldr	r3, [r1, #0]
	mov	r2, r8
	mov	r1, r8
	strh	r1, [r3, #26]
	strh	r2, [r3, #24]
	strh	r2, [r3, #20]
	mov	r1, sl
	mov	r3, r8
	str	r3, [r1, #0]
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L_080ab530
	b.n	.L_080ab3ee
.L_080ab530:
	ldr	r1, [pc, #168]
	ldr	r3, [r5, #0]
	movs	r2, #1
	adds	r3, r3, r1
	strb	r2, [r3, #0]
	ldr	r0, [sp, #24]
	bl	RenderOutput_ClearListFar
	ldr	r0, [sp, #28]
	bl	RenderOutput_ClearListFar
	movs	r0, #1
	bl	WaitFrames
	movs	r1, #1
	ldr	r0, [sp, #24]
	bl	UiWork_FinalizeFar
	adds	r0, r6, #0
	movs	r1, #1
	bl	UiWork_FinalizeFar
	movs	r1, #1
	ldr	r0, [sp, #28]
	bl	UiWork_FinalizeFar
	bl	Func_080152a8
	movs	r3, #2
	ldr	r2, [sp, #12]
	negs	r3, r3
	cmp	r2, r3
	bne.n	.L_080ab59a
	ldr	r1, [sp, #20]
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	bl	UiWindow_Clear
	ldr	r3, [sp, #20]
	ldr	r0, [r3, #48]
	bl	UiWindow_Clear
	ldr	r1, [sp, #20]
	ldr	r0, [r1, #16]
	bl	UiWindow_Clear
	ldr	r2, [pc, #72]
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	movs	r2, #0
	strb	r2, [r3, #0]
.L_080ab59a:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #64]
	bl	Scheduler_AddOrUpdateCallback
	ldr	r0, [sp, #12]
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x00000c30
	.4byte 0x00000c32
	.4byte 0x03001b04
	.4byte 0x00000c39
	.4byte 0x03001c94
	.4byte 0x000012b6
	.4byte 0x03001e8c
	.4byte 0x000012f8
	.4byte 0x00000ea6
	.4byte 0x080a19a1
