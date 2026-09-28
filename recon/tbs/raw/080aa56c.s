.syntax unified
	.thumb
	.global Func_080aa56c
	.thumb_func
Func_080aa56c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r1, #167
	lsls	r1, r1, #4
	movs	r0, #55
	sub	sp, #4
	bl	Runtime_AllocateHeapBlock
	ldr	r3, [pc, #136]
	movs	r2, #131
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrb	r2, [r3, #0]
	mov	r9, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	ldr	r3, [pc, #124]
	movs	r2, #1
	ldr	r3, [r3, #0]
	mov	sl, r2
	mov	r2, sl
	movs	r1, #0
	strh	r2, [r3, #4]
	adds	r7, r0, #0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	UiWindow_DrawFrameFar
	movs	r0, #1
	bl	WaitFrames
	movs	r0, #0
	bl	UiWindow_InitializeWork
	ldr	r0, [pc, #88]
	bl	Runtime_BumpAllocateAlternatePool
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r7, r2
	str	r0, [r3, #0]
	ldr	r6, [pc, #80]
	ldr	r3, [pc, #80]
	mov	r8, r0
	movs	r5, #0
	add	r3, r8
	add	r6, r8
	movs	r0, #183
	str	r5, [r3, #0]
	str	r5, [r6, #0]
	lsls	r0, r0, #1
	bl	GameFlag_IsSet
	cmp	r0, #0
	beq.n	.L_080aa62c
	ldr	r0, [pc, #60]
	bl	GameFlag_IsSet
	cmp	r0, #0
	bne.n	.L_080aa5fe
	ldr	r0, [pc, #52]
	bl	GameFlag_IsSet
	cmp	r0, #0
	bne.n	.L_080aa5fa
	mov	r3, sl
	b.n	.L_080aa62a
.L_080aa5fa:
	movs	r3, #14
	b.n	.L_080aa62a
.L_080aa5fe:
	ldr	r0, [pc, #36]
	bl	GameFlag_IsSet
	cmp	r0, #0
	bne.n	.L_080aa628
	movs	r3, #27
	b.n	.L_080aa62a
	.4byte 0x02000240
	.4byte 0x03001e68
	.4byte 0x00002130
	.4byte 0x0000212c
	.4byte 0x00002128
	.4byte 0x0000016f
	.2byte 0x0171
	.2byte 0x0000
.L_080aa628:
	movs	r3, #28
.L_080aa62a:
	str	r3, [r6, #0]
.L_080aa62c:
	bl	Scheduler_EnableOverlayCallbacksWithFlags
	movs	r0, #1
	bl	Func_080153e0
	ldr	r0, [pc, #140]
	bl	Link_DrawShiftedTilePairFar
	movs	r2, #130
	lsls	r2, r2, #2
	adds	r0, r7, r2
	bl	Party_ListActiveOwnersFar
	ldr	r2, [pc, #128]
	adds	r3, r7, r2
	strb	r0, [r3, #0]
	bl	Resource_LoadPairedBlocksIfAvailable
	movs	r1, #3
	movs	r2, #0
	movs	r3, #7
	movs	r0, #0
	bl	ItemMenu_Init
	movs	r0, #0
	bl	Menu_SetFirstObjectRowCoordinates
	movs	r0, #14
	bl	Palette_LightenBankHighlight
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #5
	movs	r0, #13
	bl	UiWindow_CreateFar
	movs	r2, #188
	lsls	r2, r2, #1
	movs	r3, #134
	adds	r1, r7, r2
	lsls	r3, r3, #1
	ldr	r2, [pc, #60]
	adds	r4, r7, r3
	movs	r3, #255
	str	r0, [r4, #0]
	strh	r3, [r1, #0]
	strb	r2, [r7, #28]
	strb	r2, [r7, #29]
	movs	r2, #186
	lsls	r2, r2, #1
	movs	r5, #0
	adds	r3, r7, r2
	adds	r2, #2
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	strh	r5, [r3, #0]
	movs	r1, #0
	ldr	r0, [r4, #0]
	bl	FourObjectMotion_InitializeBottomRow
	bl	Unnamed_080aa768
	bl	FourObjectMotion_ClearSlotsAndScheduleAlt
	bl	Menu_ResetTwoResourceEntries
	movs	r0, #1
	bl	WaitFrames
	bl	ItemMenu_Close
	b.n	.L_080aa6cc
	.4byte 0x00000000
	.4byte 0x06002500
	.2byte 0x0219
	.2byte 0x0000
.L_080aa6cc:
	movs	r1, #0
	movs	r2, #30
	movs	r0, #0
	movs	r3, #20
	bl	UiWindow_DrawFrameFar
	ldr	r3, [pc, #116]
	ldr	r3, [r3, #0]
	strh	r5, [r3, #4]
	bl	Func_080152a8
	movs	r0, #0
	bl	Func_080153e0
	mov	r1, r8
	movs	r2, #128
	ldr	r5, [pc, #100]
	adds	r1, #168
	lsls	r2, r2, #6
	ldr	r0, [pc, #100]
	bl	_call_via_r5
	ldr	r1, [pc, #96]
	movs	r2, #128
	add	r1, r8
	ldr	r0, [pc, #96]
	bl	_call_via_r5
	movs	r0, #1
	bl	WaitFrames
	bl	Scheduler_DisableOverlayCallbacksWithFlags
	movs	r1, #0
	movs	r0, #0
	movs	r2, #30
	movs	r3, #20
	bl	UiWindow_EraseBorderRectFar
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl	Party_Do
	movs	r0, #55
	bl	Runtime_ReleaseHeapBlock
	bl	Event_ClearInvalidPackedValuesFar
	ldr	r3, [pc, #48]
	movs	r2, #131
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r2, r9
	movs	r0, #1
	strb	r2, [r3, #0]
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e68
	.4byte 0x03001388
	.4byte 0x06004000
	.4byte 0x000020a8
	.4byte 0x05000080
	.4byte 0x02000240
