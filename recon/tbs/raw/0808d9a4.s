.syntax unified
	.thumb
	.global Func_0808d9a4
	.thumb_func
Func_0808d9a4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	ldr	r3, [pc, #444]
	movs	r0, #250
	lsls	r0, r0, #1
	mov	r5, r8
	adds	r3, r3, r0
	subs	r5, #242
	ldr	r6, [r3, #0]
	cmp	r5, #5
	bhi.n	.L_0808d9e6
	bl	Battle_InitializeRenderObject
	ldr	r3, [pc, #428]
	ldrb	r3, [r3, r5]
	ldr	r0, [pc, #428]
	mov	r8, r3
	add	r0, r8
	movs	r1, #1
	bl	UiText_DrawMessage
	ldr	r0, [pc, #420]
	movs	r1, #1
	add	r0, r8
	bl	UiText_DrawMessage
	b.n	.L_0808dd72
.L_0808d9e6:
	movs	r0, #3
	mov	r1, r8
	bl	BattleFx_FindDescriptor
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_0808d9f6
	b.n	.L_0808dd5a
.L_0808d9f6:
	ldr	r3, [r7, #0]
	asrs	r5, r3, #4
	movs	r3, #31
	movs	r2, #6
	ldrsh	r1, [r7, r2]
	ands	r5, r3
	ldrh	r2, [r7, #4]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	mov	sl, r1
	cmp	r3, #0
	bne.n	.L_0808da2c
	cmp	r5, #0
	beq.n	.L_0808da2c
	bl	Battle_InitializeRenderObject
	ldr	r0, [pc, #352]
	movs	r1, #1
	adds	r0, r5, r0
	bl	UiText_DrawMessage
	movs	r0, #161
	lsls	r0, r0, #1
	bl	GameFlag_SetBitFar
	b.n	.L_0808da34
.L_0808da2c:
	movs	r0, #161
	lsls	r0, r0, #1
	bl	GameFlag_ClearBitFar
.L_0808da34:
	ldr	r2, [r7, #8]
	movs	r3, #240
	lsls	r3, r3, #20
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0808da4e
	ldr	r3, [pc, #320]
	movs	r0, #128
	ands	r3, r2
	lsls	r0, r0, #15
	cmp	r3, r0
	bne.n	.L_0808da9a
	b.n	.L_0808da82
.L_0808da4e:
	mov	r0, sl
	bl	GameFlag_IsConditionActive
	cmp	r0, #0
	beq.n	.L_0808da68
	ldr	r3, [pc, #280]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	ldr	r3, [r7, #8]
	bl	_call_via_r3
.L_0808da68:
	movs	r0, #161
	lsls	r0, r0, #1
	bl	GameFlag_IsSet
	cmp	r0, #0
	bne.n	.L_0808da76
	b.n	.L_0808dd6a
.L_0808da76:
	ldr	r0, [pc, #264]
	movs	r1, #1
	adds	r0, r5, r0
	bl	UiText_DrawMessage
	b.n	.L_0808dd6a
.L_0808da82:
	mov	r0, sl
	bl	GameFlag_IsConditionActive
	cmp	r0, #0
	beq.n	.L_0808da90
	ldrh	r0, [r7, #8]
	b.n	.L_0808da92
.L_0808da90:
	ldr	r0, [pc, #244]
.L_0808da92:
	movs	r1, #1
	bl	UiText_DrawMessage
	b.n	.L_0808dd6a
.L_0808da9a:
	bl	Battle_Reset
	mov	r0, sl
	bl	GameFlag_IsConditionActive
	cmp	r0, #0
	bne.n	.L_0808daaa
	b.n	.L_0808dd46
.L_0808daaa:
	ldr	r1, [r7, #8]
	movs	r3, #240
	lsls	r3, r3, #12
	movs	r0, #128
	ands	r3, r1
	lsls	r0, r0, #9
	movs	r2, #1
	cmp	r3, r0
	bne.n	.L_0808dac2
	cmp	r6, #7
	bgt.n	.L_0808dac2
	movs	r2, #0
.L_0808dac2:
	cmp	r2, #0
	bne.n	.L_0808dac8
	b.n	.L_0808dd3c
.L_0808dac8:
	ldr	r2, [pc, #192]
	ldr	r3, [r7, #0]
	ands	r3, r2
	mov	fp, r2
	cmp	r3, #19
	bne.n	.L_0808dadc
	mov	r0, r8
	bl	EffectRuntime_SetMode4AndPlayCue
	ldr	r1, [r7, #8]
.L_0808dadc:
	ldr	r3, [pc, #164]
	movs	r0, #192
	ands	r3, r1
	lsls	r0, r0, #14
	cmp	r3, r0
	bne.n	.L_0808db98
	ldr	r3, [r7, #0]
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #19
	bne.n	.L_0808daf8
	mov	r0, r8
	bl	EffectRuntime_SetMode2
.L_0808daf8:
	mov	r0, r8
	bl	EffectRuntime_GetCurrentObject
	adds	r6, r0, #0
	bl	EffectRuntime_PrepareRisingObject
	movs	r0, #83
	bl	Audio_PlayCue
	ldrh	r0, [r7, #8]
	movs	r1, #5
	bl	UiText_DrawQuantity
	ldr	r5, [pc, #124]
	movs	r1, #3
	adds	r0, r5, #0
	bl	UiText_DrawMessage
	movs	r1, #0
	ldr	r0, [pc, #116]
	bl	BattleParty_ApplyDrain
	movs	r0, #1
	bl	0x08015128
	movs	r0, #126
	bl	Audio_PlayCue
	adds	r0, r5, #1
	movs	r1, #1
	bl	UiText_DrawMessage
	bl	0x08015138
	movs	r1, #2
	adds	r0, r6, #0
	bl	Engine_ObjectSetAnimation
	movs	r0, #246
	bl	Audio_PlayCue
	adds	r5, #2
	movs	r0, #30
	bl	Battle_WaitMode0
	adds	r0, r5, #0
	movs	r1, #1
	bl	UiText_DrawMessage
	mov	r0, r8
	bl	EffectRuntime_ClearCurrentFlags
	movs	r2, #1
	negs	r2, r2
	cmp	sl, r2
	bne.n	.L_0808db6a
	b.n	.L_0808dd50
.L_0808db6a:
	mov	r0, sl
	bl	GameFlag_SetBitFar
	b.n	.L_0808dd50
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x0809e680
	.4byte 0x00000928
	.4byte 0x00000948
	.4byte 0xfff00000
	.4byte 0x00000976
	.4byte 0x000001ff
	.4byte 0x00000970
	.2byte 0x03e7
	.2byte 0x0000
.L_0808db98:
	movs	r0, #160
	lsls	r0, r0, #15
	cmp	r3, r0
	bne.n	.L_0808dc18
	ldr	r3, [pc, #88]
	ldr	r5, [r3, #0]
	ldr	r3, [r7, #0]
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #19
	bne.n	.L_0808dbb4
	mov	r0, r8
	bl	EffectRuntime_SetMode7AndLaunch
.L_0808dbb4:
	movs	r2, #1
	negs	r2, r2
	cmp	sl, r2
	beq.n	.L_0808dbd0
	ldr	r2, [pc, #56]
	mov	r1, sl
	orrs	r1, r2
	ldr	r3, [pc, #60]
	movs	r0, #141
	lsls	r0, r0, #2
	mov	sl, r1
	adds	r3, r3, r0
	mov	r2, sl
	strh	r2, [r3, #0]
.L_0808dbd0:
	ldrh	r1, [r7, #8]
	movs	r0, #99
	bl	BattleFx_GetWeightedResult
	movs	r1, #190
	lsls	r1, r1, #1
	adds	r3, r5, r1
	strh	r0, [r3, #0]
	ldr	r5, [pc, #28]
	ldr	r3, [pc, #32]
	adds	r2, r5, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	ldrh	r1, [r7, #8]
	movs	r0, #99
	bl	BattleFx_SelectBattleCue
	movs	r0, #247
	b.n	.L_0808dc08
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.2byte 0x022b
	.2byte 0x0000
.L_0808dc08:
	lsls	r0, r0, #1
	adds	r3, r5, r0
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	Audio_PlayCue
	ldr	r0, [pc, #364]
	b.n	.L_0808dd3e
.L_0808dc18:
	movs	r2, #128
	lsls	r2, r2, #14
	cmp	r3, r2
	bne.n	.L_0808dc80
	ldr	r3, [pc, #356]
	movs	r0, #250
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl	BattleFx_StartRandomParticleEmitter
	adds	r5, r0, #0
	movs	r0, #30
	bl	WaitFrames
	ldr	r3, [r7, #0]
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #19
	bne.n	.L_0808dc48
	mov	r0, r8
	bl	EffectRuntime_SetMode2
.L_0808dc48:
	adds	r0, r5, #0
	bl	EffectRuntime_PrepareRisingObject
	movs	r0, #83
	bl	Audio_PlayCue
	ldrh	r0, [r7, #8]
	movs	r1, #5
	bl	UiText_DrawQuantity
	ldr	r0, [pc, #300]
	movs	r1, #3
	bl	UiText_DrawMessage
	ldrh	r0, [r7, #8]
	bl	Party_AdjustSixDigitCounterAFar
	movs	r2, #1
	negs	r2, r2
	cmp	sl, r2
	beq.n	.L_0808dc78
	mov	r0, sl
	bl	GameFlag_SetBitFar
.L_0808dc78:
	adds	r0, r5, #0
	bl	Object_Destroy
	b.n	.L_0808dd50
.L_0808dc80:
	ldr	r3, [pc, #268]
	ldr	r2, [pc, #272]
	ldr	r0, [r3, #0]
	ands	r1, r2
	bl	BattleFx_StartRandomParticleEmitter
	mov	r9, r0
	movs	r0, #30
	bl	WaitFrames
	ldrh	r0, [r7, #8]
	bl	PartyInventory_AddFar
	movs	r3, #1
	adds	r6, r0, #0
	negs	r3, r3
	ldr	r5, [pc, #244]
	cmp	r6, r3
	bne.n	.L_0808dcde
	ldr	r0, [r7, #8]
	ldr	r1, [pc, #232]
	ands	r0, r1
	movs	r1, #2
	bl	UiText_DrawQuantity
	ldr	r5, [pc, #232]
	movs	r1, #1
	adds	r0, r5, #0
	adds	r5, #4
	bl	UiText_DrawMessage
	adds	r0, r5, #0
	movs	r1, #1
	bl	UiText_DrawMessage
	mov	r0, r9
	bl	Object_DestroyIfPresent
	ldr	r3, [r7, #0]
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #19
	bne.n	.L_0808dd50
	mov	r0, r8
	bl	EffectRuntime_SetMode5AndPlayCue
	b.n	.L_0808dd50
.L_0808dcde:
	ldr	r3, [r7, #0]
	mov	r0, fp
	ands	r3, r0
	cmp	r3, #19
	bne.n	.L_0808dcee
	mov	r0, r8
	bl	EffectRuntime_SetMode2
.L_0808dcee:
	mov	r0, r9
	bl	EffectRuntime_PrepareRisingObject
	movs	r0, #83
	bl	Audio_PlayCue
	ldr	r0, [r7, #8]
	movs	r1, #2
	ands	r0, r5
	bl	UiText_DrawQuantity
	ldr	r1, [pc, #136]
	ldr	r3, [r1, #0]
	cmp	r6, r3
	bne.n	.L_0808dd16
	ldr	r0, [pc, #144]
	movs	r1, #3
	bl	UiText_DrawMessage
	b.n	.L_0808dd26
.L_0808dd16:
	adds	r0, r6, #0
	movs	r1, #1
	bl	UiText_DrawQuantity
	ldr	r0, [pc, #132]
	movs	r1, #3
	bl	UiText_DrawMessage
.L_0808dd26:
	movs	r2, #1
	negs	r2, r2
	cmp	sl, r2
	beq.n	.L_0808dd34
	mov	r0, sl
	bl	GameFlag_SetBitFar
.L_0808dd34:
	mov	r0, r9
	bl	Object_Destroy
	b.n	.L_0808dd50
.L_0808dd3c:
	ldr	r0, [pc, #104]
.L_0808dd3e:
	movs	r1, #1
	bl	UiText_DrawMessage
	b.n	.L_0808dd50
.L_0808dd46:
	ldr	r0, [pc, #100]
	movs	r1, #1
	adds	r0, r5, r0
	bl	UiText_DrawMessage
.L_0808dd50:
	bl	BattleFx_FinishAction
	bl	BattleFx_PlayQueuedSound
	b.n	.L_0808dd6a
.L_0808dd5a:
	ldr	r0, [pc, #84]
	movs	r1, #1
	bl	UiText_DrawMessage
	ldr	r0, [pc, #80]
	movs	r1, #1
	bl	UiText_DrawMessage
.L_0808dd6a:
	movs	r0, #161
	lsls	r0, r0, #1
	bl	GameFlag_ClearBitFar
.L_0808dd72:
	movs	r0, #0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000973
	.4byte 0x02000240
	.4byte 0x00000969
	.4byte 0x02000434
	.4byte 0x00000fff
	.4byte 0x0000ffff
	.4byte 0x00000968
	.4byte 0x0000096a
	.4byte 0x0000096b
	.4byte 0x0000096f
	.4byte 0x00000948
	.4byte 0x0000092d
	.4byte 0x0000094d
