.syntax unified
	.thumb
	.global Shop_SelBuy
	.global Func_080b0aac
	.thumb_func
Shop_SelBuy:
Func_080b0aac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #772]
	ldr	r3, [r3, #0]
	sub	sp, #36
	movs	r0, #0
	movs	r1, #0
	str	r0, [sp, #32]
	str	r0, [sp, #16]
	mov	sl, r3
	str	r1, [r3, #32]
	movs	r5, #2
	movs	r1, #7
	movs	r3, #4
	movs	r2, #12
	movs	r0, #18
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	mov	r2, sl
	str	r0, [r2, #12]
	bl	Shop_DrawMoney
	movs	r0, #0
	movs	r1, #8
	movs	r2, #15
	movs	r3, #4
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	str	r0, [sp, #32]
.L_080b0af4:
	movs	r5, #2
	movs	r1, #12
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	ldr	r7, [sp, #16]
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	movs	r3, #224
	str	r0, [sp, #28]
	lsls	r3, r3, #2
	add	r3, sl
	ldr	r2, [r3, #0]
	movs	r3, #18
	strb	r3, [r2, #5]
	movs	r2, #234
	lsls	r2, r2, #2
	add	r2, sl
	movs	r3, #12
	strb	r3, [r2, #0]
	movs	r0, #0
	movs	r3, #3
	movs	r1, #17
	movs	r2, #30
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	movs	r3, #1
	str	r0, [sp, #24]
	mov	fp, r3
.L_080b0b32:
	ldr	r3, [pc, #656]
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r0, fp
	mov	r9, r3
	cmp	r0, #0
	beq.n	.L_080b0ba0
	movs	r2, #155
	lsls	r2, r2, #2
	lsls	r3, r7, #1
	add	r2, sl
	ldrsh	r5, [r3, r2]
	adds	r0, r5, #0
	bl	Item_Get
	movs	r2, #0
	adds	r6, r0, #0
	movs	r1, #7
	adds	r0, r7, #0
	mov	fp, r2
	bl	__modsi3
	adds	r1, r0, #0
	lsls	r1, r1, #5
	ldr	r0, [sp, #28]
	subs	r1, #8
	movs	r2, #8
	bl	Shop_PlaceCursor
	movs	r2, #234
	lsls	r2, r2, #2
	movs	r3, #4
	add	r2, sl
	strb	r3, [r2, #0]
	ldr	r0, [sp, #28]
	adds	r1, r7, #0
	bl	Shop_DrawStock
	ldr	r1, [pc, #580]
	ldr	r0, [sp, #24]
	adds	r1, r5, r1
	bl	Shop_DrawMsg
	ldr	r0, [sp, #32]
	bl	RenderOutput_RedrawSavedRectFar
	movs	r3, #0
	ldrsh	r2, [r6, r3]
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	movs	r3, #0
	bl	Shop_DrawItemPrice
.L_080b0ba0:
	ldr	r1, [pc, #552]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0bae
	b.n	.L_080b0f48
.L_080b0bae:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0bba
	b.n	.L_080b0f3a
.L_080b0bba:
	ldr	r0, [pc, #532]
	ldr	r3, [r0, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0be2
	mov	r1, r9
	mov	r8, r7
	subs	r7, #1
	adds	r0, r7, r1
	bl	__modsi3
	adds	r7, r0, #0
	cmp	r8, r7
	beq.n	.L_080b0be2
	movs	r0, #111
	bl	Audio_PlayCue
	movs	r2, #1
	mov	fp, r2
.L_080b0be2:
	ldr	r0, [pc, #492]
	ldr	r3, [r0, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0c0a
	mov	r1, r9
	mov	r8, r7
	adds	r7, #1
	adds	r0, r7, r1
	bl	__modsi3
	adds	r7, r0, #0
	cmp	r8, r7
	beq.n	.L_080b0c0a
	movs	r0, #111
	bl	Audio_PlayCue
	movs	r2, #1
	mov	fp, r2
.L_080b0c0a:
	ldr	r0, [pc, #452]
	ldr	r3, [r0, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0c22
	subs	r3, r7, #7
	cmp	r3, #0
	blt.n	.L_080b0c22
	movs	r1, #1
	adds	r7, r3, #0
	mov	fp, r1
.L_080b0c22:
	ldr	r2, [pc, #428]
	ldr	r3, [r2, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0c52
	mov	r0, r9
	adds	r0, #6
	movs	r1, #7
	bl	__divsi3
	lsls	r3, r0, #3
	adds	r5, r7, #7
	subs	r3, r3, r0
	cmp	r5, r3
	bge.n	.L_080b0c48
	movs	r3, #1
	adds	r7, r5, #0
	mov	fp, r3
.L_080b0c48:
	mov	r0, r9
	subs	r0, #1
	cmp	r7, r0
	ble.n	.L_080b0c52
	adds	r7, r0, #0
.L_080b0c52:
	movs	r0, #1
	bl	WaitFrames
	b.n	.L_080b0b32
.L_080b0c5a:
	ldr	r0, [sp, #24]
	movs	r1, #2
	bl	UiWork_FinalizeFar
	movs	r1, #2
	ldr	r0, [sp, #28]
	bl	UiWork_FinalizeFar
	movs	r0, #1
	bl	WaitFrames
	mov	r0, r8
	cmp	r0, #0
	beq.n	.L_080b0c78
	b.n	.L_080b0f56
.L_080b0c78:
	ldr	r1, [sp, #16]
	movs	r2, #155
	lsls	r3, r1, #1
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r0, sl
	ldr	r5, [pc, #332]
	ldrh	r3, [r0, r3]
	add	r5, sl
	strh	r3, [r5, #0]
	ldr	r0, [pc, #328]
	bl	UiMessage_ShowAndWait
	ldrh	r0, [r5, #0]
	bl	Item_Get
	movs	r1, #1
	str	r0, [sp, #8]
	str	r1, [sp, #12]
	movs	r5, #2
	movs	r1, #14
	movs	r2, #13
	movs	r3, #3
	movs	r6, #0
	movs	r0, #0
	str	r6, [sp, #4]
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	movs	r3, #224
	str	r0, [sp, #20]
	lsls	r3, r3, #2
	add	r3, sl
	ldr	r2, [r3, #0]
	movs	r3, #4
	strb	r3, [r2, #5]
	movs	r2, #234
	lsls	r2, r2, #2
	add	r2, sl
	movs	r3, #12
	strb	r3, [r2, #0]
	mov	r2, r8
	str	r2, [sp, #0]
	ldr	r0, [sp, #20]
	movs	r1, #2
	movs	r2, #0
	movs	r3, #8
	bl	PsynergyMenu_InitializeEntryObjectsFar
	movs	r3, #9
	movs	r0, #16
	movs	r1, #11
	movs	r2, #14
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	movs	r3, #1
	movs	r7, #0
	mov	r9, r0
	mov	fp, r3
.L_080b0cf0:
	ldr	r0, [sp, #4]
	cmp	r0, #0
	beq.n	.L_080b0d04
	movs	r1, #0
	ldr	r0, [pc, #220]
	str	r1, [sp, #4]
	bl	UiMessage_ShowAndWait
	movs	r2, #1
	mov	fp, r2
.L_080b0d04:
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_080b0d72
	ldr	r3, [pc, #208]
	add	r3, sl
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	movs	r0, #0
	mov	fp, r0
	adds	r0, r7, r1
	bl	__modsi3
	movs	r3, #219
	adds	r7, r0, #0
	lsls	r1, r7, #1
	lsls	r3, r3, #2
	adds	r2, r1, r3
	mov	r3, sl
	adds	r1, r1, r7
	adds	r3, #2
	lsls	r1, r1, #3
	ldrsh	r6, [r3, r2]
	subs	r1, #12
	ldr	r0, [sp, #20]
	movs	r2, #0
	bl	Shop_PlaceCursor
	movs	r2, #234
	lsls	r2, r2, #2
	add	r2, sl
	movs	r3, #3
	ldr	r5, [pc, #144]
	strb	r3, [r2, #0]
	add	r5, sl
	ldr	r0, [sp, #20]
	ldrh	r2, [r5, #0]
	adds	r1, r7, #0
	bl	Shop_DrawParty
	ldrh	r0, [r5, #0]
	bl	Item_GetEquipmentGroupFar
	cmp	r0, #0
	bne.n	.L_080b0d68
	ldrh	r2, [r5, #0]
	mov	r0, r9
	adds	r1, r6, #0
	bl	Shop_DrawUnitItem
	b.n	.L_080b0d72
.L_080b0d68:
	ldrh	r2, [r5, #0]
	mov	r0, r9
	adds	r1, r6, #0
	bl	Shop_DrawEquipComparison
.L_080b0d72:
	ldr	r1, [pc, #88]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080b0e6a
	ldr	r5, [pc, #84]
	add	r5, sl
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	bl	Inventory_AddItemFar
	adds	r1, r0, #0
	cmp	r1, #0
	bge.n	.L_080b0de8
	movs	r0, #113
	bl	Audio_PlayCue
	adds	r0, r6, #0
	movs	r1, #1
	bl	UiText_DrawQuantity
	ldrh	r0, [r5, #0]
	movs	r1, #2
	bl	UiText_DrawQuantity
	adds	r0, r6, #0
	bl	Ability_GetAvailability
	cmp	r0, #15
	bne.n	.L_080b0db8
	ldr	r0, [pc, #44]
	bl	UiMessage_ShowAndWait
	b.n	.L_080b0cf0
.L_080b0db8:
	ldr	r0, [pc, #40]
	bl	UiMessage_ShowAndWait
	b.n	.L_080b0cf0
	.4byte 0x03001f2c
	.4byte 0x000003a6
	.4byte 0x00000075
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x0000039e
	.4byte 0x00000c9d
	.4byte 0x000003a7
	.4byte 0x00000c9e
	.2byte 0x0ca6
	.2byte 0x0000
.L_080b0de8:
	adds	r0, r6, #0
	bl	Inventory_RemoveFar
	ldr	r2, [sp, #8]
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	ldr	r2, [pc, #396]
	ldr	r2, [r2, #16]
	cmp	r3, r2
	bls.n	.L_080b0dfe
	b.n	.L_080b0f26
.L_080b0dfe:
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	bl	Item_IsCompatibleWithOwnerFar
	cmp	r0, #0
	bne.n	.L_080b0e28
	movs	r1, #1
	adds	r0, r6, #0
	bl	UiText_DrawQuantity
	ldr	r0, [pc, #372]
	bl	UiMessage_ShowAndWait
	movs	r0, #0
	bl	UiMessage_ShowChoice
	movs	r3, #1
	str	r3, [sp, #4]
	cmp	r0, #0
	beq.n	.L_080b0e28
	b.n	.L_080b0cf0
.L_080b0e28:
	ldr	r5, [pc, #352]
	movs	r0, #112
	bl	Audio_PlayCue
	add	r5, sl
	movs	r0, #1
	bl	WaitFrames
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	bl	Shop_SelBuyNum
	str	r0, [sp, #12]
	movs	r2, #1
	ldr	r1, [sp, #12]
	movs	r0, #1
	negs	r2, r2
	str	r0, [sp, #4]
	cmp	r1, r2
	bne.n	.L_080b0e52
	b.n	.L_080b0cf0
.L_080b0e52:
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	ldr	r2, [sp, #12]
	bl	Shop_BuyDone
	mov	r1, r9
	ldr	r0, [sp, #20]
	bl	Shop_BuySpecialItem
	movs	r3, #0
	mov	r8, r3
	b.n	.L_080b0eaa
.L_080b0e6a:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080b0f18
	ldr	r5, [pc, #280]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0e8c
	movs	r0, #111
	bl	Audio_PlayCue
	movs	r0, #1
	subs	r7, #1
	mov	fp, r0
.L_080b0e8c:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b0ea2
	movs	r0, #111
	bl	Audio_PlayCue
	movs	r1, #1
	adds	r7, #1
	mov	fp, r1
.L_080b0ea2:
	movs	r0, #1
	bl	WaitFrames
	b.n	.L_080b0cf0
.L_080b0eaa:
	bl	Menu_ReleaseEntryObjectsFar
	mov	r0, r9
	movs	r1, #2
	bl	UiWork_FinalizeFar
	movs	r1, #2
	ldr	r0, [sp, #20]
	bl	UiWork_FinalizeFar
	movs	r0, #1
	bl	WaitFrames
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080b0f10
	ldr	r3, [pc, #200]
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_080b0f10
	ldr	r3, [sp, #12]
	cmp	r8, r3
	bge.n	.L_080b0ef4
	ldr	r6, [pc, #172]
	adds	r5, r3, #0
	add	r6, sl
.L_080b0ee4:
	movs	r1, #1
	ldrh	r0, [r6, #0]
	negs	r1, r1
	subs	r5, #1
	bl	Ability_GetMaximum
	cmp	r5, #0
	bne.n	.L_080b0ee4
.L_080b0ef4:
	bl	AbilityMenu_BuildAvailableList
	cmp	r0, #0
	beq.n	.L_080b0f56
	ldr	r3, [pc, #152]
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r0, [sp, #16]
	subs	r3, #1
	cmp	r0, r3
	ble.n	.L_080b0f10
	str	r3, [sp, #16]
.L_080b0f10:
	ldr	r0, [pc, #136]
	bl	UiMessage_ShowAndWait
	b.n	.L_080b0af4
.L_080b0f18:
	movs	r0, #113
	bl	Audio_PlayCue
	movs	r1, #1
	negs	r1, r1
	mov	r8, r1
	b.n	.L_080b0eaa
.L_080b0f26:
	movs	r0, #113
	bl	Audio_PlayCue
	ldr	r0, [pc, #112]
	bl	UiMessage_ShowAndRestoreState
	movs	r2, #1
	negs	r2, r2
	mov	r8, r2
	b.n	.L_080b0eaa
.L_080b0f3a:
	movs	r0, #113
	bl	Audio_PlayCue
	movs	r3, #1
	negs	r3, r3
	mov	r8, r3
	b.n	.L_080b0c5a
.L_080b0f48:
	movs	r0, #112
	str	r7, [sp, #16]
	bl	Audio_PlayCue
	movs	r0, #0
	mov	r8, r0
	b.n	.L_080b0c5a
.L_080b0f56:
	ldr	r0, [sp, #32]
	movs	r1, #2
	bl	UiWork_FinalizeFar
	mov	r1, sl
	ldr	r0, [r1, #12]
	movs	r1, #2
	bl	UiWork_FinalizeFar
	movs	r0, #1
	bl	WaitFrames
	movs	r0, #0
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000c9f
	.4byte 0x0000039e
	.4byte 0x03001b04
	.4byte 0x000003aa
	.4byte 0x000003a6
	.4byte 0x00000ca8
	.4byte 0x00000c9c
