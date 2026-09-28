.syntax unified
	.thumb
	.global Party_ShowJoinedMessage
	.global Func_08021390
	.thumb_func
Party_ShowJoinedMessage:
Func_08021390:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #208]
	sub	sp, #28
	movs	r2, #0
	ldr	r5, [r3, #0]
	mov	sl, r0
	str	r2, [sp, #0]
	movs	r0, #2
	movs	r1, #1
	movs	r2, #26
	movs	r3, #5
	add	r7, sp, #16
	bl	UiWindow_Create
	movs	r6, #0
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L_0802145c
	movs	r1, #4
	movs	r3, #4
	movs	r2, #0
	str	r3, [sp, #0]
	bl	UiWindow_DrawDividerLine
	ldr	r3, [pc, #168]
	adds	r2, r5, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r0, sl
	bl	Party_LookupCharacterValueByFlag32
	bl	Localization_LookupEntryId
	movs	r1, #14
	add	r2, sp, #12
	add	r3, sp, #8
	str	r1, [sp, #0]
	movs	r1, #0
	str	r6, [sp, #4]
	bl	UiGlyph_LoadEntryWithPalette
	ldr	r3, [pc, #136]
	str	r6, [r7, #0]
	movs	r2, #224
	str	r3, [sp, #20]
	ldr	r3, [sp, #8]
	lsls	r2, r2, #8
	orrs	r3, r2
	ldr	r2, [pc, #128]
	str	r3, [sp, #24]
	adds	r3, r5, r2
	adds	r2, #2
	strh	r6, [r3, #0]
	adds	r3, r5, r2
	strh	r6, [r3, #0]
	movs	r1, #1
	mov	r0, sl
	bl	UiWork_PushValueSlot
	ldr	r0, [pc, #108]
	bl	UiText_BuildRenderEntriesMode1
	movs	r2, #36
	adds	r1, r0, #0
	movs	r3, #2
	mov	r0, r8
	str	r6, [sp, #0]
	bl	UiText_QueueRenderEntries
	movs	r0, #81
	bl	Audio_PlayCue
	ldr	r5, [pc, #88]
	ldr	r6, [pc, #88]
.L_0802142a:
	adds	r0, r7, #0
	movs	r1, #250
	bl	Runtime_PushSlotEntry
	movs	r0, #1
	bl	WaitFrames
	bl	Audio_Check
	cmp	r0, #0
	beq.n	.L_08021448
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0802142a
.L_08021448:
	mov	r0, r8
	movs	r1, #2
	bl	UiWork_Finalize
	movs	r0, #1
	bl	WaitFrames
	ldr	r0, [sp, #12]
	bl	Resource_ResetEntry
.L_0802145c:
	add	sp, #28
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0x8014000c
	.4byte 0x000012f4
	.4byte 0x0000001b
	.4byte 0x00000303
	.2byte 0x1c94
	.2byte 0x0300
