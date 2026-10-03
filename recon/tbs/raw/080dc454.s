@ Uncredited: complete OrbitingSparks extent and both literal pools.
@ Its former C adoption reserved a second blitter word without a consumer.
	.code	16
.gcc2_compiled.:
.text
	.balign 4, 0
	.global	BattleFx_RunOrbitingSparks
	.thumb_func
	.type	 BattleFx_RunOrbitingSparks,function
BattleFx_RunOrbitingSparks:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, .L39+8
	mov	r3, r6
	ldmia	r3!, {r1}
	ldr	r3, [r3]
	sub	sp, sp, #88
	str	r3, [sp, #48]
	mov	r3, r6
	sub	r3, r3, #108
	ldr	r3, [r3]
	mov	r0, #1
	mov	r9, r1
	str	r3, [sp, #28]
	bl	BattleFx_BeginCanvasLayer
	ldr	r2, .L39+12
	ldrh	r3, .L39
	strh	r3, [r2]
	ldrh	r3, .L39+4
	add	r2, r2, #50
	strh	r3, [r2]
	ldr	r0, .L39+16
	bl	Resource_GetTableEntry
	mov	r5, r0
	mov	r0, #160
	ldr	r3, .L39+20
	mov	r1, r5
	mov	r2, #128
	add	r5, r5, #128
	lsl	r0, r0, #19
	bl	_call_via_r3
	mov	r0, r5
	mov	r1, r9
	bl	Resource_DecodeType01
	mov	r5, #2
	mov	r1, #7
	mov	r2, #7
	mov	r3, #3
	mov	r0, #46
	str	r5, [sp]
	bl	BattleEffect_LoadWork
	b	.L40
.L41:
	.balign 4, 0
.L39:
	.word	256
	.word	4112
	.word	gWorkSlot+156
	.word	67108896
	.word	ResourceId_SkullSheet
	.word	IwramCopyWords
.L40:
	mov	r1, #239
	lsl	r1, r1, #7
	ldr	r2, .L42
	add	r1, r1, r9
	mov	r3, #1
	str	r3, [r1]
	add	r2, r2, r9
	mov	r3, #0
	str	r3, [r2]
	ldr	r6, [r6, #28]
	str	r5, [r1]
	mov	r3, #75
	mov	r1, #144
	str	r3, [r2]
	ldr	r0, .L42+4
	lsl	r1, r1, #3
	str	r6, [sp, #36]
	bl	Scheduler_AddOrUpdateCallback
	ldr	r3, .L42+8
	add	r3, r3, r9
	ldr	r3, [r3]
	ldr	r3, [r3, #4]
	mov	r2, #0
	str	r2, [sp, #24]
	cmp	r3, #0
	beq	.L6
	mov	r3, #112
	neg	r3, r3
	str	r3, [sp, #24]
.L6:
	ldr	r5, [sp, #24]
	ldr	r2, .L42+12
	lsl	r3, r5, #8
	str	r3, [r2]
	mov	r4, #0
	mov	r3, #225
	lsl	r3, r3, #7
	mov	r2, #0
	mov	r1, #4
	add	r3, r3, r9
.L14:
	add	r4, r4, #1
	str	r2, [r3]
	str	r2, [r3, #4]
	str	r2, [r3, #12]
	str	r1, [r3, #8]
	add	r3, r3, #28
	cmp	r4, #64
	bne	.L14
	mov	r1, #0
	str	r1, [sp, #44]
	ldr	r2, .L42+8
	mov	r5, r9
	ldr	r3, [r5, r2]
	ldr	r3, [r3, #20]
	sub	r1, r1, #64
	lsl	r3, r3, #4
	cmp	r3, r1
	bne	.LCB139
	b	.L37	@long jump
.LCB139:
.L19:
	mov	r3, #0
	str	r3, [sp, #32]
	mov	r5, r9
	ldr	r3, [r5, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne	.LCB150
	b	.L38	@long jump
.LCB150:
	ldr	r3, [sp, #44]
	mov	r1, #52
	mov	r5, #36
	add	r1, r1, sp
	str	r3, [sp, #16]
	str	r5, [sp, #12]
	mov	fp, r1
.L23:
	mov	r1, r9
	add	r5, r1, r2
	ldr	r2, [sp, #32]
	lsl	r2, r2, #1
	ldr	r3, [r5]
	ldr	r1, [sp, #12]
	str	r2, [sp, #20]
	ldrsh	r0, [r3, r1]
	bl	GetBattleObjectSlotFar
	ldr	r3, [r5]
	ldr	r5, [sp, #12]
	ldr	r6, [r0]
	ldrsh	r0, [r3, r5]
	mov	r1, fp
	bl	EffectPosition_ApplyStepAndYOffset
	mov	r2, fp
	ldr	r3, [r2]
	ldr	r5, [sp, #24]
	add	r3, r3, r5
	str	r3, [r2]
	ldr	r1, [sp, #16]
	cmp	r1, #63
	bhi	.L22
	bl	Render_ResetTransformState
	ldr	r0, [sp, #28]
	mov	r1, r0
	add	r1, r1, #12
	bl	Graphics_PrepareTransferInIwramWork
	ldr	r3, [r6, #8]
	add	r0, sp, #76
	str	r3, [r0]
	ldr	r3, [r6, #12]
	str	r3, [r0, #4]
	ldr	r3, [r6, #16]
	add	r5, sp, #64
	str	r3, [r0, #8]
	mov	r1, r5
	bl	EffectPosition_ApplyBaseAndYOffset
	ldr	r3, [r5]
	ldr	r2, [sp, #24]
	add	r3, r3, r2
	str	r3, [r5]
	mov	r4, #0
	mov	sl, fp
	mov	r7, r9
	mov	r8, r4
.L32:
	ldr	r5, [sp, #20]
	ldr	r1, [sp, #32]
	add	r3, r5, r1
	add	r3, r3, r4
	lsl	r5, r3, #3
	sub	r5, r5, r3
	lsl	r5, r5, #2
	mov	r2, #225
	lsl	r2, r2, #7
	add	r5, r5, r9
	add	r5, r5, r2
	ldr	r0, [r5, #12]
	add	r0, r0, r8
	str	r4, [sp, #8]
	bl	Trig_Sin
	mov	r3, sl
	ldr	r6, [r3]
	lsl	r0, r0, #3
	asr	r0, r0, #16
	add	r6, r6, r0
	ldr	r0, [r5, #12]
	add	r0, r0, r8
	bl	Trig_Cos
	mov	r1, sl
	ldr	r3, [r1, #4]
	ldr	r2, [r5, #12]
	mov	r1, #128
	lsl	r1, r1, #2
	lsl	r0, r0, #3
	add	r2, r2, r1
	asr	r0, r0, #16
	str	r2, [r5, #12]
	add	r3, r3, r0
	mov	r2, #24
	sub	r6, r6, #12
	mov	r1, r7
	str	r2, [sp]
	str	r2, [sp, #4]
	sub	r3, r3, #28
	mov	r2, r6
	ldr	r0, [sp, #48]
	ldr	r5, [sp, #36]
	bl	_call_via_r5
	ldr	r4, [sp, #8]
	mov	r1, #144
	ldr	r2, .L42+16
	lsl	r1, r1, #2
	add	r4, r4, #1
	add	r7, r7, r1
	add	r8, r8, r2
	cmp	r4, #3
	bne	.L32
.L22:
	ldr	r3, [sp, #16]
	ldr	r5, [sp, #12]
	ldr	r1, [sp, #32]
	sub	r3, r3, #16
	add	r5, r5, #2
	add	r1, r1, #1
	str	r5, [sp, #12]
	str	r3, [sp, #16]
	str	r1, [sp, #32]
	ldr	r2, .L42+8
	mov	r5, r9
	ldr	r3, [r5, r2]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	bne	.L23
.L38:
	ldr	r2, .L42+20
	mov	r3, #1
	add	r2, r2, r9
	str	r3, [r2]
	mov	r0, #1
	bl	WaitFrames
	ldr	r1, [sp, #44]
	add	r1, r1, #1
	str	r1, [sp, #44]
	ldr	r2, .L42+8
	mov	r5, r9
	ldr	r3, [r5, r2]
	ldr	r3, [r3, #20]
	lsl	r3, r3, #4
	add	r3, r3, #64
	cmp	r1, r3
	beq	.LCB340
	b	.L19	@long jump
.LCB340:
.L37:
	ldr	r0, .L42+4
	bl	Scheduler_RemoveCallback
	mov	r0, #46
	bl	Runtime_ReleaseHeapBlock
	bl	BattleFx_EndCanvasLayer
	add	sp, sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
.L43:
	.balign 4, 0
.L42:
	.word	30596
	.word	BattlePresentation_ProcessPendingGraphicsTransfer
	.word	30760
	.word	67108904
	.word	21845
	.word	30756
.Lfe1:
	.size	 BattleFx_RunOrbitingSparks,.Lfe1-BattleFx_RunOrbitingSparks
