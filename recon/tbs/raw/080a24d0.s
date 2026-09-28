.syntax unified
	.thumb
	.global RunAssetSelectionScreen
	.global Func_080a24d0
	.thumb_func
RunAssetSelectionScreen:
Func_080a24d0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #128
	lsls	r1, r1, #6
	mov	r9, r1
	mov	r0, r9
	sub	sp, #16
	bl	Runtime_BumpAllocateAlternatePool
	movs	r1, #167
	adds	r7, r0, #0
	lsls	r1, r1, #4
	movs	r0, #55
	bl	Runtime_AllocateHeapBlock
	ldr	r2, [pc, #280]
	mov	r8, r2
	ldr	r2, [r2, #0]
	movs	r3, #1
	movs	r1, #0
	adds	r5, r0, #0
	strh	r3, [r2, #4]
	movs	r0, #0
	movs	r2, #30
	movs	r3, #20
	bl	UiWindow_DrawFrameFar
	movs	r0, #1
	bl	WaitFrames
	movs	r0, #0
	bl	UiWindow_InitializeWork
	movs	r3, #130
	lsls	r3, r3, #2
	adds	r0, r5, r3
	bl	Party_ListActiveOwnersFar
	ldr	r1, [pc, #240]
	adds	r3, r5, r1
	movs	r2, #0
	movs	r1, #3
	strb	r0, [r3, #0]
	movs	r3, #7
	movs	r0, #0
	bl	ItemMenu_Init
	bl	Resource_LoadPairedBlocks
	movs	r0, #14
	bl	Palette_LightenBankHighlight
	ldr	r0, [pc, #216]
	bl	Link_DrawShiftedTilePairFar
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #3
	movs	r0, #13
	bl	UiWindow_CreateFar
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r5, r2
	str	r0, [r3, #0]
	bl	Scheduler_EnableOverlayCallbacksWithFlags
	ldr	r3, [pc, #184]
	ldr	r1, [pc, #188]
	mov	fp, r3
	mov	r2, r9
	adds	r0, r7, #0
	bl	_call_via_fp
	ldr	r3, [pc, #180]
	mov	r1, r9
	ldr	r2, [pc, #180]
	ldr	r0, [pc, #168]
	bl	_call_via_r3
	movs	r0, #1
	bl	Func_080153e0
	bl	Menu_CancelSoundReset
	add	r1, sp, #8
	add	r0, sp, #12
	add	r2, sp, #4
	bl	ItemMenu_RunCommands
	mov	sl, r0
	bl	Menu_EnsureCancelSound
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L_080a25c2
	mov	r2, r8
	ldr	r0, [r2, #84]
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #4]
	ldr	r2, [pc, #136]
	lsls	r1, r1, #10
	ands	r3, r2
	subs	r2, #127
	orrs	r1, r3
	adds	r3, r0, r2
	strh	r1, [r3, #0]
	movs	r1, #186
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	adds	r1, #38
	adds	r2, r0, r1
	strh	r3, [r2, #0]
.L_080a25c2:
	mov	r6, r8
	ldr	r0, [r5, #36]
	adds	r6, #36
	bl	RenderOutput_ClearListFar
	ldr	r5, [pc, #100]
	ldr	r2, [r6, #0]
	ldr	r3, [pc, #60]
	strb	r3, [r2, r5]
	bl	ItemMenu_Close
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	UiWindow_DrawFrameFar
	bl	Menu_ResetTwoResourceEntries
	movs	r0, #55
	bl	Runtime_ReleaseHeapBlock
	mov	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #0
	strh	r3, [r2, #4]
	bl	Func_080152a8
	movs	r0, #0
	bl	Func_080153e0
	mov	r2, r9
	adds	r1, r7, #0
	ldr	r0, [pc, #28]
	bl	_call_via_fp
	ldr	r3, [r6, #0]
	b.n	.L_080a2638
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x03001e68
	.4byte 0x00000219
	.4byte 0x06002500
	.4byte 0x03001388
	.4byte 0x06004000
	.4byte 0x03000168
	.4byte 0x33333333
	.4byte 0x000001ff
	.2byte 0x0ea6
	.2byte 0x0000
.L_080a2638:
	movs	r1, #0
	adds	r3, r3, r5
	strb	r1, [r3, #0]
	adds	r0, r7, #0
	bl	Party_Do
	movs	r0, #1
	bl	WaitFrames
	bl	Scheduler_DisableOverlayCallbacksWithFlags
	movs	r0, #1
	bl	WaitFrames
	movs	r0, #0
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	bl	UiWindow_EraseBorderRectFar
	ldr	r3, [r6, #0]
	movs	r2, #0
	adds	r3, r3, r5
	strb	r2, [r3, #0]
	bl	Event_ClearInvalidPackedValuesFar
	mov	r0, sl
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
