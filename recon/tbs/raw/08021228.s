.syntax unified
	.thumb
	.global Djinn_ShowJoinedMessage
	.global Func_08021228
	.thumb_func
Djinn_ShowJoinedMessage:
Func_08021228:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r2, [sp, #8]
	ldr	r3, [pc, #256]
	adds	r6, r1, #0
	ldr	r5, [r3, #0]
	movs	r3, #3
	ldr	r2, [pc, #252]
	ands	r3, r6
	lsls	r3, r3, #1
	mov	fp, r0
	movs	r0, #0
	mov	r9, r0
	mov	sl, r0
	ldrsh	r1, [r2, r3]
	mov	r8, r1
	mov	r1, r9
	str	r1, [sp, #0]
	movs	r0, #2
	movs	r1, #1
	movs	r2, #26
	movs	r3, #5
	add	r7, sp, #20
	bl	UiWindow_Create
	mov	r9, r0
	cmp	r0, #0
	beq.n	.L_0802132a
	movs	r3, #4
	movs	r1, #4
	movs	r2, #0
	str	r3, [sp, #0]
	bl	UiWindow_DrawDividerLine
	ldr	r3, [pc, #200]
	adds	r2, r5, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r0, r8
	bl	Localization_LookupEntryId
	movs	r1, #14
	add	r2, sp, #16
	add	r3, sp, #12
	str	r1, [sp, #0]
	mov	r1, sl
	str	r1, [sp, #4]
	bl	UiGlyph_LoadEntryWithPalette
	mov	r2, sl
	ldr	r3, [pc, #172]
	str	r2, [r7, #0]
	str	r3, [sp, #24]
	movs	r2, #224
	ldr	r3, [sp, #12]
	lsls	r2, r2, #8
	ldr	r0, [pc, #164]
	orrs	r3, r2
	ldr	r2, [pc, #164]
	str	r3, [sp, #28]
	mov	r1, sl
	adds	r3, r5, r0
	strh	r1, [r3, #0]
	mov	r0, sl
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	movs	r1, #1
	mov	r0, fp
	bl	UiWork_PushValueSlot
	lsls	r0, r6, #2
	ldr	r1, [sp, #8]
	adds	r0, r0, r6
	lsls	r0, r0, #2
	movs	r2, #150
	lsls	r2, r2, #1
	adds	r0, r0, r1
	adds	r0, r0, r2
	movs	r1, #4
	bl	UiWork_PushValueSlot
	ldr	r0, [pc, #124]
	adds	r0, r6, r0
	bl	UiText_BuildRenderEntriesMode1
	mov	r3, sl
	adds	r1, r0, #0
	str	r3, [sp, #0]
	movs	r2, #36
	movs	r3, #2
	mov	r0, r9
	bl	UiText_QueueRenderEntries
	movs	r0, #81
	bl	Audio_PlayCue
	ldr	r5, [pc, #96]
	ldr	r6, [pc, #100]
.L_080212f8:
	adds	r0, r7, #0
	movs	r1, #250
	bl	Runtime_PushSlotEntry
	movs	r0, #1
	bl	WaitFrames
	bl	Audio_Check
	cmp	r0, #0
	beq.n	.L_08021316
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080212f8
.L_08021316:
	mov	r0, r9
	movs	r1, #2
	bl	UiWork_Finalize
	movs	r0, #1
	bl	WaitFrames
	ldr	r0, [sp, #16]
	bl	Resource_ResetEntry
.L_0802132a:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x080371fe
	.4byte 0x00000ea3
	.4byte 0x8014000c
	.4byte 0x000012f4
	.4byte 0x000012f6
	.4byte 0x00000980
	.4byte 0x00000303
	.4byte 0x03001c94
