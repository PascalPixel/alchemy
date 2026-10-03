@ Uncredited: complete FlippingBurst extent, including its literal pool.
@ The prior matching C reserved an unread frame word; ordinary draft retained.
	.code	16
.gcc2_compiled.:
.text
	.align	2
	.global	BattleFx_RunFlippingBurst
	.thumb_func
	.type	 BattleFx_RunFlippingBurst,function
BattleFx_RunFlippingBurst:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r7, .L24
	mov	r3, r7
	ldmia	r3!, {r1}
	ldr	r2, .L24+4
	mov	fp, r1
	ldr	r3, [r3]
	sub	sp, sp, #84
	add	r2, r2, fp
	str	r3, [sp, #56]
	str	r0, [r2]
	mov	r0, #0
	mov	sl, r2
	bl	BattleFx_BeginCanvasLayer
	ldr	r0, .L24+8
	mov	r1, fp
	mov	r2, #1
	mov	r3, #1
	bl	Resource_LoadAndDecompress
	mov	r5, #2
	mov	r1, #7
	mov	r2, #7
	mov	r3, #3
	mov	r0, #46
	str	r5, [sp]
	bl	BattleEffect_LoadWork
	mov	r3, #239
	lsl	r3, r3, #7
	ldr	r2, .L24+12
	add	r3, r3, fp
	add	r2, r2, fp
	str	r5, [r3]
	ldr	r7, [r7, #28]
	mov	r3, #75
	mov	r1, #144
	str	r3, [r2]
	lsl	r1, r1, #3
	ldr	r0, .L24+16
	str	r7, [sp, #48]
	bl	Scheduler_AddOrUpdateCallback
	mov	r4, sl
	ldr	r3, [r4]
	ldr	r0, [r3, #8]
	bl	GetBattleObjectSlotFar
	mov	r1, sl
	ldr	r3, [r1]
	mov	r5, r0
	mov	r2, #36
	ldrsh	r0, [r3, r2]
	bl	GetBattleObjectSlotFar
	ldr	r7, [r5]
	ldr	r0, [r0]
	mov	r3, #90
	ldr	r5, [r7, #8]
	mov	r8, r3
	ldr	r3, [r0, #8]
	sub	r3, r3, r5
	mov	r9, r0
	mov	r1, #100
	mov	r0, r8
	mul	r0, r0, r3
	bl	__divsi3
	add	r5, r5, r0
	str	r5, [sp, #44]
	mov	r4, r9
	ldr	r3, [r4, #16]
	ldr	r5, [r7, #16]
	sub	r3, r3, r5
	mov	r0, r8
	mul	r0, r0, r3
	mov	r1, #100
	bl	__divsi3
	mov	r1, sl
	ldr	r3, [r1]
	add	r5, r5, r0
	ldr	r0, [r3, #8]
	bl	Battle_GetObjectTableValueFar
	str	r0, [sp, #40]
	mov	r2, sl
	ldr	r3, [r2]
	mov	r4, #36
	ldrsh	r0, [r3, r4]
	bl	Battle_GetObjectTableValueFar
	str	r0, [sp, #36]
	mov	r0, r7
	bl	Object_ResetMotion
	mov	r3, r5
	ldr	r1, [sp, #44]
	mov	r2, #0
	mov	r0, r7
	bl	Object_SetPosition
	mov	r1, #2
	mov	r0, r7
	bl	Object_SetMode
	mov	r3, r7
	mov	r2, #1
	add	r3, r3, #88
	strb	r2, [r3]
	mov	r5, r7
	mov	r3, #128
	add	r5, r5, #90
	lsl	r3, r3, #10
	str	r5, [sp, #32]
	strb	r2, [r5]
	str	r3, [r7, #52]
	mov	r3, #128
	lsl	r3, r3, #12
	str	r3, [r7, #48]
	mov	r0, #20
	bl	WaitFrames
	mov	r5, #46
	mov	r2, sp
	mov	r3, sp
	mov	r4, #56
	add	r2, r2, #72
	add	r3, r3, #60
	neg	r4, r4
	neg	r5, r5
	mov	r1, #0
	str	r2, [sp, #20]
	str	r3, [sp, #24]
	str	r4, [sp, #16]
	str	r5, [sp, #12]
	mov	sl, r1
.L7:
	ldr	r3, .L24+20
	ldr	r5, [r3]
	mov	r6, r5
	add	r6, r6, #12
	bl	Render_ResetTransformState
	mov	r1, r6
	mov	r0, r5
	bl	Graphics_PrepareTransferInIwramWork
	mov	r1, sl
	cmp	r1, #0
	bne	.L8
	mov	r2, #240
	lsl	r2, r2, #12
	mov	r3, r9
	str	r2, [r3, #40]
	ldr	r3, .L24+24
	mov	r4, r9
	str	r3, [r4, #72]
	str	r2, [r7, #40]
	str	r3, [r7, #72]
.L8:
	mov	r1, sl
	cmp	r1, #11
	bne	.L9
	mov	r2, r9
	ldr	r3, [r2, #28]
	neg	r3, r3
	str	r3, [r2, #28]
	ldr	r3, [r7, #28]
	neg	r3, r3
	str	r3, [r7, #28]
	ldr	r4, [sp, #40]
	ldr	r3, [r7, #12]
	add	r3, r3, r4
	str	r3, [r7, #12]
	ldr	r1, [sp, #36]
	ldr	r3, [r2, #12]
	add	r3, r3, r1
	str	r3, [r2, #12]
.L9:
	mov	r2, sl
	cmp	r2, #54
	bne	.L10
	ldr	r3, .L24+4
	add	r3, r3, fp
	ldr	r3, [r3]
	mov	r4, #36
	ldrsh	r0, [r3, r4]
	mov	r3, #10
	str	r3, [sp]
	mov	r1, #7
	mov	r2, #5
	mov	r3, #0
	bl	ObjectGroup_UpdateMembers
	mov	r3, #128
	mov	r1, r9
	lsl	r3, r3, #12
	str	r3, [r1, #40]
	ldr	r3, .L24+28
	str	r3, [r1, #72]
	mov	r3, #160
	lsl	r3, r3, #11
	str	r3, [r7, #40]
	ldr	r3, .L24+32
	str	r3, [r7, #72]
	mov	r3, #128
	lsl	r3, r3, #9
	str	r3, [r7, #52]
	mov	r3, #128
	lsl	r3, r3, #10
	str	r3, [r7, #48]
	ldr	r2, [sp, #32]
	mov	r3, #0
	strb	r3, [r2]
	mov	r0, r7
	bl	Object_ResetMotion
	ldr	r3, [r7, #16]
	mov	r0, r7
	mov	r1, #0
	mov	r2, #0
	bl	Object_SetPosition
.L10:
	bl	Render_ResetTransformState
	mov	r0, r5
	mov	r1, r6
	bl	Graphics_PrepareTransferInIwramWork
	ldr	r3, [r7, #8]
	ldr	r4, [sp, #20]
	str	r3, [r4]
	ldr	r3, [r7, #12]
	str	r3, [r4, #4]
	ldr	r3, [r7, #16]
	str	r3, [r4, #8]
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #24]
	bl	EffectPosition_ApplyBaseAndYOffset
	ldr	r5, [sp, #24]
	ldr	r3, [r5]
	asr	r2, r3, #1
	mov	r3, sl
	sub	r3, r3, #54
	str	r2, [r5]
	cmp	r3, #1
	bhi	.L11
	ldr	r3, [r5, #4]
	mov	r1, #32
	str	r1, [sp]
	mov	r1, #64
	str	r1, [sp, #4]
	sub	r2, r2, #16
	sub	r3, r3, #16
	ldr	r0, [sp, #56]
	mov	r1, fp
	ldr	r4, [sp, #48]
	bl	_call_via_r4
.L11:
	ldr	r5, [sp, #16]
	cmp	r5, #11
	bhi	.L12
	lsr	r3, r5, #31
	add	r3, r5, r3
	asr	r3, r3, #1
	lsl	r3, r3, #11
	ldr	r6, [sp, #12]
	mov	r4, #0
	mov	r8, r3
.L16:
	lsl	r1, r4, #12
	mov	r0, r1
	str	r1, [sp, #28]
	str	r4, [sp, #8]
	bl	Trig_Sin
	mov	r3, r6
	mul	r3, r3, r0
	ldr	r2, [sp, #24]
	ldr	r5, [r2]
	asr	r3, r3, #16
	ldr	r0, [sp, #28]
	add	r5, r5, r3
	bl	Trig_Cos
	mov	r3, r6
	mul	r3, r3, r0
	mov	r1, sl
	mov	r2, #32
	asr	r3, r3, #16
	sub	r3, r3, r1
	sub	r5, r5, #16
	str	r2, [sp]
	mov	r1, fp
	mov	r2, #64
	str	r2, [sp, #4]
	add	r3, r3, #100
	mov	r2, r5
	ldr	r0, [sp, #56]
	add	r1, r1, r8
	ldr	r5, [sp, #48]
	bl	_call_via_r5
	ldr	r4, [sp, #8]
	add	r4, r4, #1
	cmp	r4, #16
	bne	.L16
.L12:
	mov	r1, sl
	cmp	r1, #64
	bne	.L18
	mov	r2, r9
	ldr	r3, [r2, #28]
	neg	r3, r3
	str	r3, [r2, #28]
	ldr	r3, [r7, #28]
	neg	r3, r3
	str	r3, [r7, #28]
	ldr	r4, [sp, #40]
	ldr	r3, [r7, #12]
	sub	r3, r3, r4
	str	r3, [r7, #12]
	ldr	r5, [sp, #36]
	ldr	r3, [r2, #12]
	sub	r3, r3, r5
	str	r3, [r2, #12]
	mov	r0, r7
	mov	r1, #0
	bl	Object_SetMode
.L18:
	mov	r1, sl
	cmp	r1, #54
	bne	.L19
	mov	r0, #134
	bl	BattleEventRuntime_BeginPhaseFar
.L19:
	mov	r2, sl
	cmp	r2, #0
	bne	.L20
	mov	r0, #136
	bl	Audio_PlayCue
	ldr	r2, .L24+36
	mov	r3, #6
	add	r2, r2, fp
	str	r3, [r2]
.L20:
	mov	r3, sl
	cmp	r3, #53
	bne	.L21
	ldr	r2, .L24+36
	mov	r3, #6
	add	r2, r2, fp
	str	r3, [r2]
.L21:
	mov	r1, #16
	mov	r0, #16
	bl	Camera_ApplyShake
	ldr	r2, .L24+40
	mov	r3, #1
	add	r2, r2, fp
	str	r3, [r2]
	mov	r0, #1
	bl	WaitFrames
	mov	r1, #1
	ldr	r4, [sp, #16]
	ldr	r5, [sp, #12]
	add	sl, sl, r1
	add	r4, r4, #1
	add	r5, r5, #1
	mov	r2, sl
	str	r4, [sp, #16]
	str	r5, [sp, #12]
	cmp	r2, #96
	beq	.LCB420
	b	.L7	@long jump
.LCB420:
	ldr	r0, .L24+16
	bl	Scheduler_RemoveCallback
	mov	r0, #46
	bl	Runtime_ReleaseHeapBlock
	bl	BattleFx_EndCanvasLayer
	add	sp, sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
.L25:
	.balign 4, 0
.L24:
	.word	gBattleFxWork
	.word	30760
	.word	ResourceId_FlashBurstSheet
	.word	30596
	.word	BattlePresentation_ProcessPendingGraphicsTransfer
	.word	gCameraWork
	.word	37355
	.word	43909
	.word	30801
	.word	30632
	.word	30756
.Lfe1:
	.size	 BattleFx_RunFlippingBurst,.Lfe1-BattleFx_RunFlippingBurst
