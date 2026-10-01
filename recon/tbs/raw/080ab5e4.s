.syntax unified
	.thumb
	.global DjinnMenu_SelectDjinn
	.thumb_func
DjinnMenu_SelectDjinn:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #108
	str r0, [sp, #80]
	ldr r3, .L_080ab7e8
	ldr r3, [r3]
	movs r0, #194
	str r3, [sp, #76]
	ldr r2, [sp, #80]
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r3, [r3]
	lsls r2, r2, #1
	movs r1, #1
	movs r5, #186
	str r1, [sp, #72]
	str r2, [sp, #52]
	ldr r7, [sp, #76]
	lsls r5, r5, #1
	mov r9, r3
	adds r3, r2, r5
	ldrh r5, [r7, r3]
	movs r1, #10
	adds r0, r5, #0
	bl IwramUnsignedRemainderEntry
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r0, [sp, #56]
	movs r1, #10
	adds r0, r5, #0
	bl Math_DivU
	mov r2, sp
	adds r2, #100
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r2, [sp, #28]
	movs r1, #1
	str r0, [sp, #48]
	negs r1, r1
	movs r0, #0
	ldr r5, [sp, #28]
	mov r3, sp
	str r0, [sp, #44]
	str r0, [sp, #40]
	str r0, [sp, #36]
	str r0, [sp, #32]
	str r1, [sp, #60]
	movs r2, #0
	adds r3, #107
	mov r12, r5
.L_080ab656:
	strb r2, [r3]
	subs r3, #1
	cmp r3, r12
	bge .L_080ab656
	ldr r7, [sp, #80]
	cmp r7, #0
	bne .L_080ab6d8
	mov r0, r9
	bl DjinnMenu_DrawElementList
	ldr r3, .L_080ab7ec
	movs r0, #0
	ldr r1, [sp, #76]
	str r0, [sp, #68]
	adds r2, r1, r3
	ldrb r3, [r2]
	cmp r7, r3
	bge .L_080ab69e
	adds r0, r2, #0
	ldr r1, [sp, #28]
	mov r2, r9
	movs r4, #4
	adds r2, #160
.L_080ab684:
	ldrb r3, [r2]
	lsls r3, r3, #24
	adds r2, #1
	cmp r3, #0
	bne .L_080ab690
	strb r4, [r1]
.L_080ab690:
	ldr r5, [sp, #68]
	adds r5, #1
	str r5, [sp, #68]
	ldrb r3, [r0]
	adds r1, #1
	cmp r5, r3
	blt .L_080ab684
.L_080ab69e:
	ldr r7, [sp, #48]
	movs r0, #0
	ldr r1, [sp, #76]
	ldr r2, .L_080ab7ec
	str r7, [sp, #44]
	str r0, [sp, #68]
	adds r3, r1, r2
	ldrb r3, [r3]
	cmp r0, r3
	bge .L_080ab746
	adds r5, r1, r2
.L_080ab6b4:
	ldr r7, [sp, #28]
	ldr r0, [sp, #56]
	ldrsb r3, [r7, r0]
	cmp r3, #4
	bne .L_080ab6ca
	adds r0, #1
	str r0, [sp, #56]
	ldrb r1, [r5]
	bl Menu_GetModuloOfSum
	str r0, [sp, #56]
.L_080ab6ca:
	ldr r1, [sp, #68]
	adds r1, #1
	str r1, [sp, #68]
	ldrb r3, [r5]
	cmp r1, r3
	blt .L_080ab6b4
	b .L_080ab746
.L_080ab6d8:
	ldr r2, [sp, #76]
	add r5, sp, #84
	movs r1, #28
	ldrsb r1, [r2, r1]
	adds r0, r5, #0
	bl Djinn_MarkBalancedEntries
	movs r3, #0
	ldr r7, [sp, #76]
	ldr r0, .L_080ab7ec
	str r3, [sp, #68]
	adds r2, r7, r0
	ldrb r3, [r2]
	movs r1, #0
	cmp r1, r3
	bge .L_080ab746
	ldr r0, [sp, #28]
	mov r1, r9
	adds r4, r2, #0
	adds r1, #160
	adds r2, r0, #0
	movs r6, #7
.L_080ab704:
	ldr r7, [sp, #76]
	movs r3, #28
	ldrsb r3, [r7, r3]
	ldr r7, [sp, #68]
	cmp r7, r3
	bne .L_080ab714
	strb r6, [r2]
	b .L_080ab732
.L_080ab714:
	ldr r7, [sp, #68]
	ldrb r3, [r5, r7]
	cmp r3, #0
	beq .L_080ab722
	movs r3, #0
	strb r3, [r2]
	b .L_080ab732
.L_080ab722:
	movs r3, #3
	strb r3, [r2]
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, #0
	bne .L_080ab732
	movs r3, #7
	strb r3, [r0]
.L_080ab732:
	ldr r3, [sp, #68]
	adds r3, #1
	str r3, [sp, #68]
	ldr r7, [sp, #68]
	ldrb r3, [r4]
	adds r0, #1
	adds r1, #1
	adds r2, #1
	cmp r7, r3
	blt .L_080ab704
.L_080ab746:
	ldr r0, [sp, #80]
	cmp r0, #1
	bne .L_080ab818
	ldr r1, [sp, #76]
	movs r2, #186
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r6, [r3]
	movs r1, #10
	adds r0, r6, #0
	bl IwramUnsignedRemainderEntry
	movs r1, #10
	adds r5, r0, #0
	adds r0, r6, #0
	bl Math_DivU
	lsls r5, r5, #16
	lsrs r5, r5, #16
	ldr r3, [sp, #76]
	adds r2, r0, #0
	lsls r6, r5, #3
	subs r6, r6, r5
	lsls r2, r2, #16
	ldr r0, [r3, #48]
	ldr r5, [sp, #80]
	adds r6, #1
	movs r3, #14
	lsrs r2, r2, #16
	str r3, [sp, #4]
	adds r2, #2
	adds r1, r6, #0
	movs r3, #6
	str r5, [sp, #0]
	bl Menu_DrawAtWindowOffset
	ldr r7, [sp, #76]
	movs r3, #7
	ldr r0, [r7, #48]
	adds r1, r6, #0
	str r3, [sp, #0]
	movs r2, #2
	movs r3, #6
	str r3, [sp, #4]
	bl UiWindow_ApplyRectAtObjectOrigin
	ldr r0, .L_080ab7ec
	adds r3, r7, r0
	ldrb r3, [r3]
	movs r5, #0
	cmp r5, r3
	bge .L_080ab818
	movs r6, #8
.L_080ab7b0:
	ldr r1, [sp, #76]
	movs r3, #28
	ldrsb r3, [r1, r3]
	cmp r5, r3
	bne .L_080ab7d2
	movs r2, #188
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	ldr r2, .L_080ab7e4
	ands r3, r2
	cmp r3, #0
	beq .L_080ab7ce
	ldr r0, .L_080ab7f0
	b .L_080ab7fe
.L_080ab7ce:
	ldr r0, .L_080ab7f4
	b .L_080ab7fe
.L_080ab7d2:
	ldr r3, [sp, #28]
	ldrb r2, [r3, r5]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080ab7fc
	ldr r0, .L_080ab7f8
	b .L_080ab7fe
	.2byte 0x0000
.L_080ab7e4:
	.4byte 0x00008000
.L_080ab7e8:
	.4byte gMenuWork
.L_080ab7ec:
	.4byte 0x00000219
.L_080ab7f0:
	.4byte 0x00000bb0
.L_080ab7f4:
	.4byte 0x00000baf
.L_080ab7f8:
	.4byte 0x00000bae
.L_080ab7fc:
	ldr r0, .L_080abb48
.L_080ab7fe:
	ldr r7, [sp, #76]
	adds r2, r6, #0
	movs r3, #8
	ldr r1, [r7, #48]
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, .L_080abb4c
	adds r3, r7, r0
	ldrb r3, [r3]
	adds r5, #1
	adds r6, #56
	cmp r5, r3
	blt .L_080ab7b0
.L_080ab818:
	ldr r1, [sp, #76]
	movs r2, #134
	lsls r2, r2, #1
	adds r3, r1, r2
	ldr r0, [r3]
	bl RenderOutput_RedrawSavedRectFar
	ldr r3, [sp, #76]
	ldr r2, [r3, #20]
	movs r3, #1
	strb r3, [r2, #5]
	ldr r5, [sp, #56]
	lsls r5, r5, #3
	str r5, [sp, #24]
.L_080ab834:
	ldr r7, [sp, #72]
	cmp r7, #0
	bne .L_080ab83c
	b .L_080abc9a
.L_080ab83c:
	movs r1, #1
	movs r0, #0
	negs r1, r1
	ldr r3, [sp, #28]
	str r0, [sp, #72]
	str r1, [sp, #60]
	ldr r5, [sp, #56]
	ldrb r2, [r3, r5]
	movs r3, #1
	ands r3, r2
	mov r10, r7
	cmp r3, #0
	bne .L_080ab85a
	ldr r7, [sp, #48]
	str r7, [sp, #60]
.L_080ab85a:
	ldr r1, [sp, #56]
	movs r2, #130
	ldr r0, [sp, #76]
	lsls r3, r1, #1
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r6, [r0, #16]
	ldrh r0, [r0, r3]
	bl Owner_GetStateFar
	ldr r7, .L_080abb50
	adds r5, r0, #0
	adds r0, r6, #0
	bl RenderOutput_RedrawSavedRectFar
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringAtOffsetFar
	adds r3, r5, r7
	ldrb r0, [r3]
	ldr r3, .L_080abb54
	adds r1, r6, #0
	adds r0, r0, r3
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, .L_080abb58
	adds r1, r6, #0
	movs r2, #48
	movs r3, #0
	bl UiText_DrawStringAtOffsetFar
	ldr r1, [sp, #72]
	ldrb r0, [r5, #15]
	adds r2, r6, #0
	str r1, [sp, #0]
	movs r3, #72
	movs r1, #2
	bl UiNumber_DrawAt
	ldr r2, [sp, #80]
	cmp r2, #0
	bne .L_080ab8c4
	ldr r0, .L_080abb5c
	adds r1, r6, #0
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
.L_080ab8c4:
	movs r7, #1
	ldr r3, [sp, #60]
	negs r7, r7
	cmp r3, r7
	beq .L_080ab8e2
	ldr r5, [sp, #56]
	lsls r3, r5, #2
	adds r3, r3, r5
	ldr r0, [sp, #60]
	lsls r3, r3, #1
	adds r3, r3, r0
	lsls r3, r3, #1
	mov r1, r9
	ldrh r3, [r1, r3]
	str r3, [sp, #40]
.L_080ab8e2:
	ldr r2, [sp, #76]
	movs r3, #134
	lsls r3, r3, #1
	adds r2, r2, r3
	ldr r0, [r2]
	mov r8, r2
	bl RenderOutput_RedrawSavedRectFar
	ldr r5, [sp, #80]
	cmp r5, #1
	bne .L_080ab980
	ldr r0, [sp, #76]
	ldr r1, .L_080abb60
	adds r3, r0, r1
	ldrb r0, [r3]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r4, .L_080abb64
	mov r2, r8
	adds r0, r4, #0
	ldr r1, [r2]
	movs r3, #0
	movs r2, #0
	str r4, [sp, #8]
	bl UiText_DrawCharacterAtOffsetFar
	movs r5, #188
	ldr r3, [sp, #76]
	lsls r5, r5, #1
	adds r6, r3, r5
	ldrh r2, [r6]
	movs r5, #224
	adds r3, r5, #0
	ands r3, r2
	lsrs r3, r3, #5
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r3, #31
	ands r3, r2
	lsls r0, r0, #2
	movs r1, #150
	adds r0, r0, r3
	lsls r1, r1, #1
	adds r0, r0, r1
	movs r1, #4
	bl UiText_DrawQuantity
	ldrh r3, [r6]
	ands r5, r3
	ldr r3, .L_080abb68
	ldr r1, [sp, #72]
	lsrs r5, r5, #5
	mov r2, r8
	adds r5, r5, r3
	ldr r0, [r2]
	movs r3, #0
	str r1, [sp, #0]
	movs r2, #6
	adds r1, r5, #0
	bl UiWindow_SetTilemapEntryFar
	ldr r4, [sp, #8]
	mov r2, r8
	adds r0, r4, #1
	ldr r1, [r2]
	movs r3, #0
	movs r2, #56
	bl UiText_DrawCharacterAtOffsetFar
	ldr r4, [sp, #8]
	mov r3, r8
	adds r4, #2
	ldr r1, [r3]
	adds r0, r4, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
.L_080ab980:
	ldr r5, [sp, #60]
	cmp r5, r7
	bne .L_080ab994
	ldr r0, [sp, #80]
	movs r1, #0
	movs r2, #200
	movs r3, #0
	bl FourObjectMotion_SetSlotPosition
	b .L_080abc14
.L_080ab994:
	ldr r7, [sp, #80]
	cmp r7, #0
	beq .L_080ab99c
	b .L_080abb7c
.L_080ab99c:
	ldr r0, [sp, #36]
	cmp r0, #0
	beq .L_080aba6a
	ldr r1, [sp, #32]
	cmp r1, #0
	bne .L_080ab9b8
	mov r2, r8
	ldr r1, [r2]
	ldr r0, .L_080abb6c
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080ab9c6
.L_080ab9b8:
	mov r3, r8
	ldr r1, [r3]
	ldr r0, .L_080abb70
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_080ab9c6:
	ldr r5, [sp, #40]
	movs r3, #240
	lsls r3, r3, #4
	ands r3, r5
	lsrs r6, r3, #8
	ldr r0, [sp, #40]
	movs r3, #224
	ands r3, r5
	movs r7, #31
	ands r7, r0
	lsrs r5, r3, #5
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	bl Trade_CanOfferDjinnFar
	cmp r0, #0
	bne .L_080ab9f8
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	bl Djinn_IsActiveFar
	cmp r0, #0
	beq .L_080aba32
.L_080ab9f8:
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	bl Trade_CanOfferDjinnFar
	cmp r0, #0
	beq .L_080aba12
	ldr r0, [sp, #80]
	adds r1, r5, #0
	movs r2, #1
	bl FourObjectMotion_ReplaceSlot
	b .L_080aba1c
.L_080aba12:
	ldr r0, [sp, #80]
	adds r1, r5, #0
	movs r2, #2
	bl FourObjectMotion_ReplaceSlot
.L_080aba1c:
	ldr r2, [sp, #24]
	ldr r3, [sp, #56]
	subs r1, r2, r3
	lsls r1, r1, #3
	adds r1, #48
	ldr r0, [sp, #80]
	movs r2, #62
	movs r3, #0
	bl FourObjectMotion_SetSlotPosition
	b .L_080aba64
.L_080aba32:
	ldr r7, [sp, #76]
	movs r1, #134
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r1, [r3]
	ldr r0, .L_080abb74
	movs r3, #16
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	adds r1, r5, #0
	movs r2, #1
	ldr r0, [sp, #80]
	bl FourObjectMotion_ReplaceSlot
	ldr r2, [sp, #24]
	ldr r3, [sp, #56]
	subs r1, r2, r3
	lsls r1, r1, #3
	adds r1, #48
	ldr r0, [sp, #80]
	movs r2, #62
	movs r3, #1
	bl FourObjectMotion_SetSlotPosition
.L_080aba64:
	mov r5, r10
	lsrs r3, r5, #1
	b .L_080abb3a
.L_080aba6a:
	ldr r4, .L_080abb78
	mov r7, r8
	adds r0, r4, #0
	ldr r1, [r7]
	movs r2, #0
	movs r3, #0
	str r4, [sp, #8]
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #240
	ldr r0, [sp, #40]
	lsls r3, r3, #4
	ands r3, r0
	lsrs r5, r3, #8
	movs r3, #224
	ands r3, r0
	movs r6, #31
	lsrs r7, r3, #5
	ands r6, r0
	adds r1, r7, #0
	adds r0, r5, #0
	adds r2, r6, #0
	bl Trade_CanOfferDjinnFar
	ldr r4, [sp, #8]
	cmp r0, #0
	bne .L_080abab0
	adds r0, r5, #0
	adds r1, r7, #0
	adds r2, r6, #0
	bl Djinn_IsActiveFar
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_080abb0a
.L_080abab0:
	adds r0, r5, #0
	adds r1, r7, #0
	adds r2, r6, #0
	str r4, [sp, #8]
	bl Trade_CanOfferDjinnFar
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_080abadc
	mov r2, r8
	ldr r1, [r2]
	adds r0, r4, #3
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #0
	adds r1, r7, #0
	movs r2, #1
	bl FourObjectMotion_ReplaceSlot
	b .L_080abaf4
.L_080abadc:
	mov r3, r8
	ldr r1, [r3]
	adds r0, r4, #2
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #0
	adds r1, r7, #0
	movs r2, #2
	bl FourObjectMotion_ReplaceSlot
.L_080abaf4:
	ldr r5, [sp, #24]
	ldr r7, [sp, #56]
	subs r1, r5, r7
	lsls r1, r1, #3
	adds r1, #48
	ldr r0, [sp, #80]
	movs r2, #62
	movs r3, #0
	bl FourObjectMotion_SetSlotPosition
	b .L_080abb36
.L_080abb0a:
	mov r2, r8
	adds r0, r4, #4
	ldr r1, [r2]
	movs r3, #16
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	adds r1, r7, #0
	movs r2, #1
	movs r0, #0
	bl FourObjectMotion_ReplaceSlot
	ldr r3, [sp, #24]
	ldr r5, [sp, #56]
	subs r1, r3, r5
	lsls r1, r1, #3
	adds r1, #48
	movs r0, #0
	movs r2, #62
	movs r3, #1
	bl FourObjectMotion_SetSlotPosition
.L_080abb36:
	mov r7, r10
	lsrs r3, r7, #1
.L_080abb3a:
	cmp r3, #0
	beq .L_080abc14
	ldr r0, [sp, #80]
	movs r1, #0
	bl FourObjectMotion_SetSlotPhase
	b .L_080abc14
.L_080abb48:
	.4byte 0x00000bb1
.L_080abb4c:
	.4byte 0x00000219
.L_080abb50:
	.4byte 0x00000129
.L_080abb54:
	.4byte 0x00000741
.L_080abb58:
	.4byte Data_080af28c
.L_080abb5c:
	.4byte 0x00000ba9
.L_080abb60:
	.4byte 0x0000021a
.L_080abb64:
	.4byte 0x00000bb2
.L_080abb68:
	.4byte 0x00005001
.L_080abb6c:
	.4byte 0x00000b98
.L_080abb70:
	.4byte 0x00000b99
.L_080abb74:
	.4byte 0x00000b9e
.L_080abb78:
	.4byte 0x00000b9a
.L_080abb7c:
	ldr r0, [sp, #40]
	movs r3, #240
	lsls r3, r3, #4
	ands r3, r0
	lsrs r6, r3, #8
	movs r3, #224
	ands r3, r0
	movs r7, #31
	ands r7, r0
	lsrs r5, r3, #5
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	bl Trade_CanOfferDjinnFar
	cmp r0, #0
	bne .L_080abbac
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	bl Djinn_IsActiveFar
	cmp r0, #0
	beq .L_080abbe6
.L_080abbac:
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	bl Trade_CanOfferDjinnFar
	cmp r0, #0
	beq .L_080abbc6
	ldr r0, [sp, #80]
	adds r1, r5, #0
	movs r2, #1
	bl FourObjectMotion_ReplaceSlot
	b .L_080abbd0
.L_080abbc6:
	ldr r0, [sp, #80]
	adds r1, r5, #0
	movs r2, #2
	bl FourObjectMotion_ReplaceSlot
.L_080abbd0:
	ldr r2, [sp, #24]
	ldr r3, [sp, #56]
	subs r1, r2, r3
	lsls r1, r1, #3
	adds r1, #48
	ldr r0, [sp, #80]
	movs r2, #54
	movs r3, #0
	bl FourObjectMotion_SetSlotPosition
	b .L_080abc04
.L_080abbe6:
	adds r1, r5, #0
	movs r2, #1
	ldr r0, [sp, #80]
	bl FourObjectMotion_ReplaceSlot
	ldr r5, [sp, #24]
	ldr r7, [sp, #56]
	subs r1, r5, r7
	lsls r1, r1, #3
	adds r1, #48
	ldr r0, [sp, #80]
	movs r2, #54
	movs r3, #1
	bl FourObjectMotion_SetSlotPosition
.L_080abc04:
	mov r0, r10
	lsrs r3, r0, #1
	cmp r3, #0
	beq .L_080abc14
	ldr r0, [sp, #80]
	movs r1, #0
	bl FourObjectMotion_SetSlotPhase
.L_080abc14:
	ldr r1, [sp, #76]
	ldr r0, [r1, #48]
	bl RenderOutput_ClearListFar
	movs r3, #1
	ldr r2, [sp, #60]
	negs r3, r3
	cmp r2, r3
	beq .L_080abc66
	ldr r5, [sp, #76]
	ldr r0, .L_080abfb8
	ldr r1, [r5, #48]
	movs r2, #0
	movs r3, #80
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #104
	ldr r0, [r5, #48]
	movs r1, #0
	str r3, [sp, #0]
	movs r2, #96
	movs r3, #224
	bl UiWindow_ClearInteriorTilesFar
	ldr r7, [sp, #40]
	movs r3, #224
	ands r3, r7
	lsrs r3, r3, #5
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r3, #31
	ands r3, r7
	lsls r0, r0, #2
	adds r0, r0, r3
	ldr r3, .L_080abfbc
	ldr r1, [r5, #48]
	adds r0, r0, r3
	movs r2, #0
	movs r3, #96
	bl UiText_DrawCharacterAtOffsetFar
.L_080abc66:
	ldr r0, [sp, #28]
	ldr r1, [sp, #56]
	movs r5, #1
	ldrb r2, [r0, r1]
	adds r3, r5, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080abc90
	ldr r2, [sp, #76]
	ldr r3, [sp, #24]
	ldr r0, [r2, #48]
	ldr r2, [sp, #48]
	subs r1, r3, r1
	movs r3, #14
	str r3, [sp, #4]
	adds r1, #1
	adds r2, #2
	movs r3, #6
	str r5, [sp, #0]
	bl Menu_DrawAtWindowOffset
.L_080abc90:
	ldr r3, .L_080abfc0
	ldr r7, .L_080abfc4
	ldr r3, [r3]
	adds r3, r3, r7
	strb r5, [r3]
.L_080abc9a:
	ldr r0, [sp, #28]
	ldr r1, [sp, #56]
	ldrb r2, [r0, r1]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080abcb8
	ldr r2, [sp, #24]
	subs r0, r2, r1
	lsls r0, r0, #3
	subs r0, #8
	movs r1, #52
	bl UiMenu_PositionCursor
	b .L_080abccc
.L_080abcb8:
	ldr r3, [sp, #24]
	ldr r5, [sp, #56]
	ldr r7, [sp, #48]
	subs r0, r3, r5
	lsls r0, r0, #3
	lsls r1, r7, #3
	subs r0, #8
	adds r1, #60
	bl UiMenu_PositionCursor
.L_080abccc:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_080abfc8
	movs r2, #128
	ldr r3, [r3]
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080abcea
	ldr r3, .L_080abfcc
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_080abcfa
.L_080abcea:
	ldr r0, [sp, #36]
	cmp r0, #0
	beq .L_080abcf4
	movs r1, #1
	str r1, [sp, #72]
.L_080abcf4:
	movs r2, #0
	str r2, [sp, #36]
	str r2, [sp, #32]
.L_080abcfa:
	ldr r3, .L_080abfd0
	ldr r3, [r3]
	mov r11, r3
	ldr r3, .L_080abfd4
	ldr r4, [r3]
	ldr r3, .L_080abfd8
	add r3, r9
	ldr r1, [r3]
	cmp r1, #0
	bne .L_080abd10
	b .L_080ac1b8
.L_080abd10:
	ldr r2, .L_080abfdc
	add r2, r9
	ldr r3, [r2]
	adds r3, #1
	movs r4, #0
	str r3, [r2]
	subs r3, r1, #1
	mov r11, r4
	cmp r3, #27
	bls .L_080abd26
	b .L_080ac1b8
.L_080abd26:
	ldr r2, .L_080abfe0
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080abd30:
	.4byte .L_080abe82
	.4byte .L_080abede
	.4byte .L_080abefc
	.4byte .L_080abede
	.4byte .L_080ac1b8
	.4byte .L_080abf1c
	.4byte .L_080abf1c
	.4byte .L_080abede
	.4byte .L_080abede
	.4byte .L_080ac1b8
	.4byte .L_080ac1b8
	.4byte .L_080ac1b8
	.4byte .L_080abf78
	.4byte .L_080abf9a
	.4byte .L_080abff8
	.4byte .L_080abf9a
	.4byte .L_080abf9a
	.4byte .L_080ac160
	.4byte .L_080ac1b8
	.4byte .L_080ac160
	.4byte .L_080ac17e
	.4byte .L_080abede
	.4byte .L_080ac1b8
	.4byte .L_080ac19c
	.4byte .L_080ac1b8
	.4byte .L_080ac1b8
	.4byte .L_080abdcc
	.4byte .L_080abda0
.L_080abda0:
	ldr r1, .L_080abfd4
	ldr r2, [r1]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	bne .L_080abdc6
	adds r6, r1, #0
	movs r5, #1
.L_080abdb0:
	movs r0, #150
	movs r1, #26
	bl UiMenu_PositionCursor
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6]
	ands r3, r5
	cmp r3, #0
	beq .L_080abdb0
.L_080abdc6:
	movs r4, #2
	mov r11, r4
	b .L_080ac1b8
.L_080abdcc:
	ldr r3, .L_080abfdc
	add r3, r9
	ldr r3, [r3]
	cmp r3, #60
	beq .L_080abdd8
	b .L_080ac1b8
.L_080abdd8:
	movs r2, #9
	movs r3, #1
	ldr r0, .L_080abfe4
	movs r1, #9
	bl UiText_OpenMessageWindowFar
	ldr r2, .L_080abfe8
	movs r3, #131
	lsls r3, r3, #2
	adds r2, r2, r3
	adds r5, r0, #0
	movs r3, #1
	strb r3, [r2]
	b .L_080abdfa
.L_080abdf4:
	movs r0, #1
	bl WaitFrames
.L_080abdfa:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_080abdf4
	movs r1, #1
	adds r0, r5, #0
	bl UiWork_FinalizeFar
	mov r0, r9
	bl DjinnMenu_DrawElementList
	movs r0, #1
	bl WaitFrames
	movs r2, #9
	movs r3, #1
	ldr r0, .L_080abfec
	movs r1, #9
	bl UiText_OpenMessageWindowFar
	movs r7, #131
	ldr r2, .L_080abfe8
	lsls r7, r7, #2
	adds r2, r2, r7
	movs r3, #1
	adds r5, r0, #0
	strb r3, [r2]
	b .L_080abe38
.L_080abe32:
	movs r0, #1
	bl WaitFrames
.L_080abe38:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_080abe32
	movs r1, #1
	adds r0, r5, #0
	bl UiWork_FinalizeFar
	mov r0, r9
	bl DjinnMenu_DrawElementList
	ldr r2, .L_080abfdc
	movs r3, #0
	add r2, r9
	str r3, [r2]
	bl BattlePlacement_UpdateTimedEntriesFar
	bl BattlePlacement_UpdateTimedEntriesFar
	bl BattlePlacement_UpdateTimedEntriesFar
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl Djinn_DeactivateFar
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl Trade_AddOfferFar
	movs r0, #0
	bl Owner_RecalculateStatsFar
	movs r0, #2
	mov r11, r0
	b .L_080ac1b6
.L_080abe82:
	ldr r3, .L_080abfdc
	add r3, r9
	ldr r3, [r3]
	cmp r3, #60
	beq .L_080abe8e
	b .L_080ac1b8
.L_080abe8e:
	movs r1, #9
	movs r2, #9
	movs r3, #1
	ldr r0, .L_080abff0
	str r4, [sp, #8]
	bl UiText_OpenMessageWindowFar
	ldr r2, .L_080abfe8
	movs r1, #131
	lsls r1, r1, #2
	adds r2, r2, r1
	movs r3, #1
	adds r5, r0, #0
	strb r3, [r2]
	b .L_080abeb4
.L_080abeac:
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
.L_080abeb4:
	ldr r4, [sp, #8]
	str r4, [sp, #8]
	bl UiWork_IsCompleteFar
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_080abeac
	adds r0, r5, #0
	movs r1, #1
	bl UiWork_FinalizeFar
	mov r0, r9
	bl DjinnMenu_DrawElementList
	ldr r2, .L_080abfdc
	movs r3, #0
	add r2, r9
	str r3, [r2]
	ldr r2, .L_080abfd8
	movs r3, #2
	b .L_080ac158
.L_080abede:
	ldr r2, .L_080abfdc
	add r2, r9
	ldr r3, [r2]
	cmp r3, #90
	beq .L_080abeea
	b .L_080ac1b8
.L_080abeea:
	movs r3, #1
	mov r11, r3
	movs r3, #0
	str r3, [r2]
	ldr r2, .L_080abfd8
	add r2, r9
	ldr r3, [r2]
	adds r3, #1
	b .L_080ac178
.L_080abefc:
	ldr r2, .L_080abfdc
	add r2, r9
	ldr r3, [r2]
	cmp r3, #90
	beq .L_080abf08
	b .L_080ac1b8
.L_080abf08:
	movs r3, #0
	str r3, [r2]
	ldr r2, .L_080abfd8
	movs r5, #16
	add r2, r9
	movs r3, #4
	mov r11, r5
	movs r4, #16
	str r3, [r2]
	b .L_080ac1b8
.L_080abf1c:
	ldr r3, .L_080abfdc
	add r3, r9
	ldr r3, [r3]
	cmp r3, #60
	beq .L_080abf28
	b .L_080ac1b8
.L_080abf28:
	movs r1, #9
	movs r2, #9
	movs r3, #1
	ldr r0, .L_080abff4
	str r4, [sp, #8]
	bl UiText_OpenMessageWindowFar
	movs r7, #131
	ldr r2, .L_080abfe8
	lsls r7, r7, #2
	adds r2, r2, r7
	movs r3, #1
	adds r5, r0, #0
	strb r3, [r2]
	b .L_080abf4e
.L_080abf46:
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
.L_080abf4e:
	ldr r4, [sp, #8]
	str r4, [sp, #8]
	bl UiWork_IsCompleteFar
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_080abf46
	adds r0, r5, #0
	movs r1, #1
	bl UiWork_FinalizeFar
	mov r0, r9
	bl DjinnMenu_DrawElementList
	ldr r2, .L_080abfdc
	movs r3, #0
	add r2, r9
	str r3, [r2]
	ldr r2, .L_080abfd8
	movs r3, #8
	b .L_080ac158
.L_080abf78:
	ldr r2, .L_080abfdc
	add r2, r9
	ldr r3, [r2]
	cmp r3, #40
	beq .L_080abf84
	b .L_080ac1b8
.L_080abf84:
	movs r3, #0
	str r3, [r2]
	ldr r2, .L_080abfd8
	add r2, r9
	ldr r3, [r2]
	movs r0, #2
	adds r3, #1
	mov r11, r0
	movs r4, #2
	str r3, [r2]
	b .L_080ac1b8
.L_080abf9a:
	ldr r2, .L_080abfdc
	add r2, r9
	ldr r3, [r2]
	cmp r3, #40
	beq .L_080abfa6
	b .L_080ac1b8
.L_080abfa6:
	movs r3, #0
	str r3, [r2]
	ldr r2, .L_080abfd8
	add r2, r9
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	b .L_080ac1b8
	.2byte 0x0000
.L_080abfb8:
	.4byte 0x00000bad
.L_080abfbc:
	.4byte 0x00000666
.L_080abfc0:
	.4byte gWindowWork
.L_080abfc4:
	.4byte 0x00000ea3
.L_080abfc8:
	.4byte gKeysHeld
.L_080abfcc:
	.4byte gKeysPressedLatch
.L_080abfd0:
	.4byte gKeysRepeat
.L_080abfd4:
	.4byte gKeyState
.L_080abfd8:
	.4byte 0x0000212c
.L_080abfdc:
	.4byte 0x00002128
.L_080abfe0:
	.4byte .L_080abd30
.L_080abfe4:
	.4byte 0x00000c4c
.L_080abfe8:
	.4byte gCell
.L_080abfec:
	.4byte 0x00000c4d
.L_080abff0:
	.4byte 0x00000c40
.L_080abff4:
	.4byte 0x00000c41
.L_080abff8:
	ldr r3, .L_080ac348
	add r3, r9
	ldr r3, [r3]
	cmp r3, #60
	beq .L_080ac004
	b .L_080ac1b8
.L_080ac004:
	ldr r3, .L_080ac34c
	movs r1, #131
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	strb r2, [r3]
	movs r1, #9
	movs r2, #9
	movs r3, #1
	ldr r0, .L_080ac350
	str r4, [sp, #8]
	bl UiText_OpenMessageWindowFar
	movs r1, #146
	adds r5, r0, #0
	movs r0, #2
	bl UiMenu_SlideCursor
	b .L_080ac032
.L_080ac02a:
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
.L_080ac032:
	ldr r4, [sp, #8]
	str r4, [sp, #8]
	bl UiWork_IsCompleteFar
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_080ac02a
	ldr r1, .L_080ac354
	ldr r2, [r1]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	bne .L_080ac06a
	adds r7, r1, #0
	movs r6, #1
.L_080ac050:
	movs r0, #2
	movs r1, #146
	str r4, [sp, #8]
	bl UiMenu_PositionCursor
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7]
	ands r3, r6
	ldr r4, [sp, #8]
	cmp r3, #0
	beq .L_080ac050
.L_080ac06a:
	movs r1, #1
	adds r0, r5, #0
	str r4, [sp, #8]
	bl UiWork_FinalizeFar
	mov r0, r9
	bl DjinnMenu_DrawElementList
	movs r0, #1
	bl WaitFrames
	movs r1, #9
	movs r2, #9
	movs r3, #1
	ldr r0, .L_080ac358
	bl UiText_OpenMessageWindowFar
	adds r5, r0, #0
	b .L_080ac098
.L_080ac090:
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
.L_080ac098:
	ldr r4, [sp, #8]
	str r4, [sp, #8]
	bl UiWork_IsCompleteFar
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_080ac090
	ldr r1, .L_080ac354
	ldr r2, [r1]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	bne .L_080ac0d0
	adds r7, r1, #0
	movs r6, #1
.L_080ac0b6:
	movs r0, #2
	movs r1, #146
	str r4, [sp, #8]
	bl UiMenu_PositionCursor
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7]
	ands r3, r6
	ldr r4, [sp, #8]
	cmp r3, #0
	beq .L_080ac0b6
.L_080ac0d0:
	movs r1, #1
	adds r0, r5, #0
	str r4, [sp, #8]
	bl UiWork_FinalizeFar
	mov r0, r9
	bl DjinnMenu_DrawElementList
	movs r0, #1
	bl WaitFrames
	movs r1, #9
	movs r2, #9
	movs r3, #1
	ldr r0, .L_080ac35c
	bl UiText_OpenMessageWindowFar
	adds r5, r0, #0
	b .L_080ac0fe
.L_080ac0f6:
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
.L_080ac0fe:
	ldr r4, [sp, #8]
	str r4, [sp, #8]
	bl UiWork_IsCompleteFar
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_080ac0f6
	ldr r1, .L_080ac354
	ldr r2, [r1]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	bne .L_080ac136
	adds r7, r1, #0
	movs r6, #1
.L_080ac11c:
	movs r0, #2
	movs r1, #146
	str r4, [sp, #8]
	bl UiMenu_PositionCursor
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7]
	ands r3, r6
	ldr r4, [sp, #8]
	cmp r3, #0
	beq .L_080ac11c
.L_080ac136:
	movs r1, #1
	adds r0, r5, #0
	str r4, [sp, #8]
	bl UiWork_FinalizeFar
	mov r0, r9
	bl DjinnMenu_DrawElementList
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_080ac348
	movs r3, #0
	add r2, r9
	str r3, [r2]
	ldr r2, .L_080ac360
	movs r3, #16
.L_080ac158:
	add r2, r9
	str r3, [r2]
	ldr r4, [sp, #8]
	b .L_080ac1b8
.L_080ac160:
	ldr r2, .L_080ac348
	add r2, r9
	ldr r3, [r2]
	cmp r3, #90
	bne .L_080ac1b8
	movs r3, #1
	mov r11, r3
	movs r3, #0
	str r3, [r2]
	ldr r2, .L_080ac360
	movs r3, #21
	add r2, r9
.L_080ac178:
	movs r4, #1
	str r3, [r2]
	b .L_080ac1b8
.L_080ac17e:
	ldr r2, .L_080ac348
	add r2, r9
	ldr r3, [r2]
	cmp r3, #90
	bne .L_080ac1b8
	movs r3, #0
	str r3, [r2]
	ldr r2, .L_080ac360
	movs r5, #32
	add r2, r9
	movs r3, #22
	mov r11, r5
	movs r4, #32
	str r3, [r2]
	b .L_080ac1b8
.L_080ac19c:
	ldr r2, .L_080ac348
	add r2, r9
	ldr r3, [r2]
	cmp r3, #60
	bne .L_080ac1b8
	movs r3, #0
	str r3, [r2]
	ldr r2, .L_080ac360
	movs r3, #25
	add r2, r9
	movs r7, #2
	str r3, [r2]
	mov r11, r7
.L_080ac1b6:
	movs r4, #2
.L_080ac1b8:
	ldr r0, [sp, #80]
	cmp r0, #0
	beq .L_080ac1c0
	b .L_080ac304
.L_080ac1c0:
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r4
	cmp r3, #0
	bne .L_080ac1cc
	b .L_080ac2ea
.L_080ac1cc:
	movs r2, #1
	ldr r1, [sp, #60]
	negs r2, r2
	cmp r1, r2
	bne .L_080ac1d8
	b .L_080ac33c
.L_080ac1d8:
	movs r3, #0
	ldr r5, [sp, #40]
	str r3, [sp, #68]
	movs r3, #240
	lsls r3, r3, #4
	ands r3, r5
	ldr r0, [sp, #40]
	lsrs r7, r3, #8
	movs r3, #224
	ands r3, r5
	movs r6, #31
	lsrs r5, r3, #5
	ands r6, r0
	adds r1, r5, #0
	adds r0, r7, #0
	adds r2, r6, #0
	str r4, [sp, #8]
	bl Trade_CanOfferDjinnFar
	ldr r4, [sp, #8]
	cmp r0, #0
	bne .L_080ac214
	adds r0, r7, #0
	adds r1, r5, #0
	adds r2, r6, #0
	bl Djinn_IsActiveFar
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_080ac218
.L_080ac214:
	movs r1, #1
	str r1, [sp, #68]
.L_080ac218:
	movs r2, #1
	str r2, [sp, #36]
	ldr r2, .L_080ac364
	movs r3, #0
	str r3, [r2]
	ldr r3, [sp, #68]
	cmp r3, #0
	bne .L_080ac25a
	movs r0, #114
	bl AudioCommand_PlayFar
	ldr r5, [sp, #76]
	ldr r0, [r5, #48]
	bl RenderOutput_ClearListFar
	movs r3, #104
	ldr r0, [r5, #48]
	movs r1, #0
	str r3, [sp, #0]
	movs r2, #80
	movs r3, #216
	bl UiWindow_ClearInteriorTilesFar
	ldr r0, .L_080ac368
	ldr r1, [r5, #48]
	movs r2, #0
	movs r3, #96
	bl UiText_DrawResourceFar
	movs r7, #1
	str r7, [sp, #72]
	bl .L_080ab834
.L_080ac25a:
	ldr r0, [sp, #40]
	lsrs r3, r0, #15
	cmp r3, #0
	beq .L_080ac298
	movs r0, #175
	str r4, [sp, #8]
	bl AudioCommand_PlayFar
	movs r5, #240
	ldr r1, [sp, #40]
	lsls r5, r5, #4
	movs r6, #224
	ands r5, r1
	ands r6, r1
	movs r3, #31
	ands r3, r1
	lsrs r5, r5, #8
	lsrs r6, r6, #5
	adds r2, r3, #0
	adds r1, r6, #0
	adds r0, r5, #0
	str r3, [sp, #12]
	bl Djinn_DeactivateFar
	ldr r3, [sp, #12]
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r3, #0
	bl Trade_AddOfferFar
	b .L_080ac2cc
.L_080ac298:
	movs r0, #139
	str r4, [sp, #8]
	bl AudioCommand_PlayFar
	movs r5, #240
	ldr r2, [sp, #40]
	lsls r5, r5, #4
	movs r6, #224
	ands r5, r2
	ands r6, r2
	movs r3, #31
	ands r3, r2
	lsrs r5, r5, #8
	lsrs r6, r6, #5
	adds r2, r3, #0
	adds r1, r6, #0
	adds r0, r5, #0
	str r3, [sp, #12]
	bl Djinn_ActivateFar
	ldr r3, [sp, #12]
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r3, #0
	bl Trade_RemoveOfferFar
.L_080ac2cc:
	ldr r4, [sp, #8]
	ldr r3, [sp, #40]
	movs r0, #240
	lsls r0, r0, #4
	ands r0, r3
	lsrs r0, r0, #8
	str r4, [sp, #8]
	bl Owner_RecalculateStatsFar
	mov r0, r9
	bl DjinnMenu_DrawElementList
	movs r5, #1
	str r5, [sp, #72]
	ldr r4, [sp, #8]
.L_080ac2ea:
	ldr r7, [sp, #80]
	cmp r7, #0
	bne .L_080ac304
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r4
	cmp r3, #0
	beq .L_080ac304
	movs r0, #112
	movs r5, #7
	bl AudioCommand_PlayFar
	b .L_080ac86a
.L_080ac304:
	movs r3, #1
	ands r3, r4
	cmp r3, #0
	bne .L_080ac320
	ldr r0, [sp, #80]
	cmp r0, #1
	beq .L_080ac314
	b .L_080ac41e
.L_080ac314:
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r4
	cmp r3, #0
	bne .L_080ac320
	b .L_080ac41e
.L_080ac320:
	movs r1, #1
	ldr r3, [sp, #28]
	str r1, [sp, #68]
	ldr r5, [sp, #56]
	ldrb r2, [r3, r5]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080ac3a4
	movs r0, #1
	ldr r7, [sp, #60]
	negs r0, r0
	cmp r7, r0
	bne .L_080ac36c
.L_080ac33c:
	movs r0, #114
	bl AudioCommand_PlayFar
	bl .L_080ab834
	.2byte 0x0000
.L_080ac348:
	.4byte 0x00002128
.L_080ac34c:
	.4byte gCell
.L_080ac350:
	.4byte 0x00000c44
.L_080ac354:
	.4byte gKeyState
.L_080ac358:
	.4byte 0x00000c45
.L_080ac35c:
	.4byte 0x00000c46
.L_080ac360:
	.4byte 0x0000212c
.L_080ac364:
	.4byte gKeysPressedLatch
.L_080ac368:
	.4byte 0x00000bbe
.L_080ac36c:
	ldr r2, [sp, #40]
	movs r3, #240
	lsls r3, r3, #4
	ands r3, r2
	lsrs r7, r3, #8
	movs r3, #224
	ands r3, r2
	movs r5, #31
	movs r1, #0
	lsrs r6, r3, #5
	ands r5, r2
	str r1, [sp, #68]
	adds r0, r7, #0
	adds r1, r6, #0
	adds r2, r5, #0
	bl Trade_CanOfferDjinnFar
	cmp r0, #0
	bne .L_080ac3a0
	adds r0, r7, #0
	adds r1, r6, #0
	adds r2, r5, #0
	bl Djinn_IsActiveFar
	cmp r0, #0
	beq .L_080ac3a4
.L_080ac3a0:
	movs r3, #1
	str r3, [sp, #68]
.L_080ac3a4:
	ldr r5, [sp, #68]
	cmp r5, #0
	bne .L_080ac3d8
	movs r0, #114
	bl AudioCommand_PlayFar
	ldr r7, [sp, #76]
	ldr r0, [r7, #48]
	bl RenderOutput_ClearListFar
	movs r3, #104
	ldr r0, [r7, #48]
	movs r1, #0
	str r3, [sp, #0]
	movs r2, #80
	movs r3, #216
	bl UiWindow_ClearInteriorTilesFar
	ldr r0, .L_080ac708
	ldr r1, [r7, #48]
	movs r2, #0
	movs r3, #96
	bl UiText_DrawResourceFar
	bl .L_080ab834
.L_080ac3d8:
	ldr r0, [sp, #80]
	cmp r0, #1
	bne .L_080ac414
	ldr r3, [sp, #56]
	ldr r1, [sp, #28]
	ldrb r2, [r1, r3]
	adds r3, r0, #0
	ands r3, r2
	movs r5, #4
	cmp r3, #0
	beq .L_080ac416
	ldr r5, [sp, #76]
	ldr r7, [sp, #56]
	movs r3, #28
	ldrsb r3, [r5, r3]
	cmp r7, r3
	bne .L_080ac410
	movs r0, #188
	lsls r0, r0, #1
	adds r3, r5, r0
	ldrh r2, [r3]
	movs r3, #128
	lsls r3, r3, #8
	ands r3, r2
	movs r5, #2
	cmp r3, #0
	bne .L_080ac416
	b .L_080ac414
.L_080ac410:
	movs r5, #3
	b .L_080ac416
.L_080ac414:
	movs r5, #1
.L_080ac416:
	movs r0, #112
	bl AudioCommand_PlayFar
	b .L_080ac86a
.L_080ac41e:
	movs r3, #8
	ands r3, r4
	cmp r3, #0
	beq .L_080ac42c
	movs r0, #113
	movs r5, #2
	b .L_080ac438
.L_080ac42c:
	movs r3, #2
	ands r3, r4
	cmp r3, #0
	beq .L_080ac440
	movs r0, #113
	movs r5, #1
.L_080ac438:
	bl AudioCommand_PlayFar
	negs r5, r5
	b .L_080ac86a
.L_080ac440:
	ldr r1, [sp, #80]
	cmp r1, #0
	beq .L_080ac448
	b .L_080ac5d2
.L_080ac448:
	movs r3, #4
	ands r3, r4
	cmp r3, #0
	bne .L_080ac452
	b .L_080ac5d2
.L_080ac452:
	ldr r2, [sp, #36]
	cmp r2, #0
	bne .L_080ac45a
	b .L_080ac5aa
.L_080ac45a:
	ldr r5, [sp, #32]
	movs r3, #1
	eors r5, r3
	str r5, [sp, #32]
	cmp r5, #0
	beq .L_080ac46e
	movs r0, #139
	bl AudioCommand_PlayFar
	b .L_080ac474
.L_080ac46e:
	movs r0, #175
	bl AudioCommand_PlayFar
.L_080ac474:
	movs r7, #0
	ldr r0, [sp, #76]
	ldr r1, .L_080ac70c
	str r7, [sp, #68]
	adds r3, r0, r1
	ldrb r3, [r3]
	cmp r7, r3
	blt .L_080ac486
	b .L_080ac59e
.L_080ac486:
	movs r2, #160
	movs r3, #0
	str r2, [sp, #20]
	str r3, [sp, #16]
.L_080ac48e:
	movs r5, #0
	str r5, [sp, #64]
	ldr r7, [sp, #20]
	mov r0, r9
	ldrsb r3, [r7, r0]
	cmp r5, r3
	bge .L_080ac57e
	ldr r1, [sp, #16]
	lsls r3, r1, #1
	add r3, r9
	mov r10, r3
.L_080ac4a4:
	mov r2, r10
	ldrh r7, [r2]
	movs r3, #2
	movs r0, #240
	lsls r0, r0, #4
	add r10, r3
	adds r3, r7, #0
	ands r3, r0
	lsrs r4, r3, #8
	movs r5, #0
	movs r1, #224
	adds r3, r7, #0
	ands r3, r1
	movs r2, #31
	mov r8, r5
	adds r5, r7, #0
	lsrs r6, r3, #5
	ands r5, r2
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r5, #0
	str r4, [sp, #8]
	bl Trade_CanOfferDjinnFar
	ldr r4, [sp, #8]
	cmp r0, #0
	bne .L_080ac4e8
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r5, #0
	bl Djinn_IsActiveFar
	cmp r0, #0
	beq .L_080ac4ec
.L_080ac4e8:
	movs r3, #1
	mov r8, r3
.L_080ac4ec:
	mov r5, r8
	cmp r5, #0
	beq .L_080ac56e
	ldr r0, [sp, #32]
	cmp r0, #0
	beq .L_080ac534
	lsrs r3, r7, #15
	cmp r3, #0
	bne .L_080ac56e
	movs r1, #240
	lsls r1, r1, #4
	movs r2, #224
	adds r5, r7, #0
	adds r6, r7, #0
	ands r5, r1
	ands r6, r2
	movs r3, #31
	ands r3, r7
	lsrs r5, r5, #8
	lsrs r6, r6, #5
	adds r2, r3, #0
	adds r1, r6, #0
	adds r0, r5, #0
	str r3, [sp, #12]
	bl Djinn_ActivateFar
	ldr r3, [sp, #12]
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r3, #0
	bl Trade_RemoveOfferFar
	adds r0, r5, #0
	bl Owner_RecalculateStatsFar
	b .L_080ac56e
.L_080ac534:
	lsrs r3, r7, #15
	cmp r3, #0
	beq .L_080ac56e
	movs r3, #240
	lsls r3, r3, #4
	movs r0, #224
	adds r5, r7, #0
	adds r6, r7, #0
	ands r5, r3
	ands r6, r0
	movs r3, #31
	ands r3, r7
	lsrs r5, r5, #8
	lsrs r6, r6, #5
	adds r2, r3, #0
	adds r1, r6, #0
	adds r0, r5, #0
	str r3, [sp, #12]
	bl Djinn_DeactivateFar
	ldr r3, [sp, #12]
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r3, #0
	bl Trade_AddOfferFar
	adds r0, r5, #0
	bl Owner_RecalculateStatsFar
.L_080ac56e:
	ldr r1, [sp, #64]
	adds r1, #1
	str r1, [sp, #64]
	ldr r2, [sp, #20]
	mov r5, r9
	ldrsb r3, [r2, r5]
	cmp r1, r3
	blt .L_080ac4a4
.L_080ac57e:
	ldr r7, [sp, #20]
	ldr r0, [sp, #16]
	ldr r1, [sp, #68]
	adds r7, #1
	adds r0, #10
	adds r1, #1
	ldr r2, [sp, #76]
	ldr r5, .L_080ac70c
	str r7, [sp, #20]
	str r0, [sp, #16]
	str r1, [sp, #68]
	adds r3, r2, r5
	ldrb r3, [r3]
	cmp r1, r3
	bge .L_080ac59e
	b .L_080ac48e
.L_080ac59e:
	mov r0, r9
	bl DjinnMenu_DrawElementList
	movs r7, #1
	str r7, [sp, #72]
	b .L_080ac5d2
.L_080ac5aa:
	ldr r1, [sp, #76]
	ldr r2, [sp, #24]
	ldr r3, [sp, #56]
	ldr r0, [r1, #48]
	subs r1, r2, r3
	ldr r2, [sp, #48]
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #15
	str r3, [sp, #4]
	adds r1, #1
	adds r2, #2
	movs r3, #6
	bl Menu_DrawAtWindowOffset
	movs r0, #112
	movs r5, #10
	bl AudioCommand_PlayFar
	b .L_080ac86a
.L_080ac5d2:
	movs r3, #64
	mov r5, r11
	ands r3, r5
	cmp r3, #0
	beq .L_080ac682
	movs r0, #111
	bl AudioCommand_PlayFar
	ldr r7, [sp, #28]
	ldr r0, [sp, #56]
	movs r5, #4
	ldrb r2, [r7, r0]
	adds r3, r5, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080ac5f6
	bl .L_080ab834
.L_080ac5f6:
	ldr r1, [sp, #76]
	ldr r2, [sp, #24]
	ldr r3, [sp, #56]
	ldr r0, [r1, #48]
	subs r1, r2, r3
	ldr r2, [sp, #48]
	movs r3, #15
	adds r2, #2
	str r3, [sp, #4]
	adds r1, #1
	movs r3, #6
	movs r6, #1
	str r6, [sp, #0]
	bl Menu_DrawAtWindowOffset
	ldr r0, [sp, #56]
	ldrb r2, [r7, r0]
	adds r3, r5, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080ac624
	bl .L_080ab834
.L_080ac624:
	adds r3, r6, #0
	ands r3, r2
	movs r1, #1
	cmp r3, #0
	beq .L_080ac63c
	movs r3, #2
	negs r3, r3
	ands r3, r2
	movs r1, #0
	strb r3, [r7, r0]
	str r1, [sp, #48]
	b .L_080ac65c
.L_080ac63c:
	ldr r3, [sp, #48]
	cmp r3, #0
	bne .L_080ac65c
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080ac65c
	adds r3, r2, #0
	ldr r5, [sp, #28]
	ldr r7, [sp, #56]
	orrs r3, r1
	movs r0, #2
	strb r3, [r5, r7]
	str r0, [sp, #72]
	bl .L_080ab834
.L_080ac65c:
	ldr r1, [sp, #48]
	ldr r3, [sp, #56]
	subs r1, #1
	str r1, [sp, #48]
	adds r3, #160
	mov r2, r9
	ldrsb r1, [r2, r3]
	cmp r1, #0
	bne .L_080ac670
	movs r1, #1
.L_080ac670:
	ldr r0, [sp, #48]
	bl Menu_GetModuloOfSum
	movs r3, #2
	str r0, [sp, #48]
	str r0, [sp, #44]
	str r3, [sp, #72]
	bl .L_080ab834
.L_080ac682:
	movs r3, #128
	mov r5, r11
	ands r3, r5
	cmp r3, #0
	beq .L_080ac738
	movs r0, #111
	bl AudioCommand_PlayFar
	ldr r7, [sp, #28]
	ldr r0, [sp, #56]
	movs r5, #4
	ldrb r2, [r7, r0]
	adds r3, r5, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080ac6a6
	bl .L_080ab834
.L_080ac6a6:
	ldr r1, [sp, #76]
	ldr r2, [sp, #24]
	ldr r3, [sp, #56]
	ldr r0, [r1, #48]
	subs r1, r2, r3
	ldr r2, [sp, #48]
	movs r3, #15
	adds r1, #1
	str r3, [sp, #4]
	adds r2, #2
	movs r3, #6
	movs r6, #1
	str r6, [sp, #0]
	bl Menu_DrawAtWindowOffset
	ldr r7, [sp, #48]
	ldr r3, [sp, #56]
	adds r7, #1
	str r7, [sp, #48]
	adds r3, #160
	mov r0, r9
	ldrsb r1, [r0, r3]
	cmp r1, #0
	bne .L_080ac6d8
	movs r1, #1
.L_080ac6d8:
	ldr r0, [sp, #48]
	bl Menu_GetModuloOfSum
	ldr r3, [sp, #56]
	str r0, [sp, #48]
	ldr r1, [sp, #28]
	ldrb r2, [r1, r3]
	adds r3, r6, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080ac710
	adds r3, r5, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080ac710
	movs r3, #2
	negs r3, r3
	ldr r5, [sp, #56]
	ands r3, r2
	movs r7, #0
	strb r3, [r1, r5]
	str r7, [sp, #48]
	b .L_080ac72c
	.2byte 0x0000
.L_080ac708:
	.4byte 0x00000bbe
.L_080ac70c:
	.4byte 0x00000219
.L_080ac710:
	ldr r0, [sp, #48]
	cmp r0, #0
	bne .L_080ac72c
	ldr r3, [sp, #56]
	ldr r1, [sp, #28]
	ldrb r2, [r1, r3]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080ac72c
	movs r3, #1
	ldr r5, [sp, #56]
	orrs r3, r2
	strb r3, [r1, r5]
.L_080ac72c:
	ldr r7, [sp, #48]
	movs r0, #2
	str r7, [sp, #44]
	str r0, [sp, #72]
	bl .L_080ab834
.L_080ac738:
	movs r3, #32
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_080ac7bc
	movs r0, #111
	bl AudioCommand_PlayFar
	ldr r3, [sp, #28]
	ldr r5, [sp, #56]
	ldrb r2, [r3, r5]
	movs r3, #4
	ands r3, r2
	cmp r3, #0
	bne .L_080ac772
	ldr r2, [sp, #24]
	ldr r7, [sp, #76]
	subs r1, r2, r5
	movs r3, #1
	ldr r2, [sp, #48]
	ldr r0, [r7, #48]
	str r3, [sp, #0]
	movs r3, #15
	str r3, [sp, #4]
	adds r1, #1
	adds r2, #2
	movs r3, #6
	bl Menu_DrawAtWindowOffset
.L_080ac772:
	ldr r3, [sp, #56]
	ldr r7, [sp, #76]
	subs r3, #1
	ldr r0, .L_080ac8f8
	str r3, [sp, #56]
	adds r5, r7, r0
	ldrb r1, [r5]
	adds r0, r3, #0
	bl Menu_GetModuloOfSum
	ldr r1, [sp, #80]
	str r0, [sp, #56]
	cmp r1, #0
	bne .L_080ac842
	movs r2, #0
	str r2, [sp, #68]
	ldrb r3, [r5]
	cmp r1, r3
	bge .L_080ac842
.L_080ac798:
	ldr r7, [sp, #28]
	ldr r0, [sp, #56]
	ldrsb r3, [r7, r0]
	cmp r3, #4
	bne .L_080ac7ae
	subs r0, #1
	str r0, [sp, #56]
	ldrb r1, [r5]
	bl Menu_GetModuloOfSum
	str r0, [sp, #56]
.L_080ac7ae:
	ldr r1, [sp, #68]
	adds r1, #1
	str r1, [sp, #68]
	ldrb r3, [r5]
	cmp r1, r3
	blt .L_080ac798
	b .L_080ac842
.L_080ac7bc:
	movs r3, #16
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	bne .L_080ac7ca
	bl .L_080ab834
.L_080ac7ca:
	movs r0, #111
	bl AudioCommand_PlayFar
	ldr r3, [sp, #28]
	ldr r5, [sp, #56]
	ldrb r2, [r3, r5]
	movs r3, #4
	ands r3, r2
	cmp r3, #0
	bne .L_080ac7fa
	ldr r2, [sp, #24]
	ldr r7, [sp, #76]
	subs r1, r2, r5
	movs r3, #1
	ldr r2, [sp, #48]
	ldr r0, [r7, #48]
	str r3, [sp, #0]
	movs r3, #15
	str r3, [sp, #4]
	adds r1, #1
	adds r2, #2
	movs r3, #6
	bl Menu_DrawAtWindowOffset
.L_080ac7fa:
	ldr r3, [sp, #56]
	ldr r7, [sp, #76]
	adds r3, #1
	ldr r0, .L_080ac8f8
	str r3, [sp, #56]
	adds r5, r7, r0
	ldrb r1, [r5]
	adds r0, r3, #0
	bl Menu_GetModuloOfSum
	ldr r1, [sp, #80]
	str r0, [sp, #56]
	cmp r1, #0
	bne .L_080ac842
	movs r2, #0
	str r2, [sp, #68]
	ldrb r3, [r5]
	cmp r1, r3
	bge .L_080ac842
.L_080ac820:
	ldr r7, [sp, #28]
	ldr r0, [sp, #56]
	ldrsb r3, [r7, r0]
	cmp r3, #4
	bne .L_080ac836
	adds r0, #1
	str r0, [sp, #56]
	ldrb r1, [r5]
	bl Menu_GetModuloOfSum
	str r0, [sp, #56]
.L_080ac836:
	ldr r1, [sp, #68]
	adds r1, #1
	str r1, [sp, #68]
	ldrb r3, [r5]
	cmp r1, r3
	blt .L_080ac820
.L_080ac842:
	ldr r2, [sp, #44]
	ldr r3, [sp, #56]
	str r2, [sp, #48]
	adds r3, #160
	mov r5, r9
	ldrsb r1, [r5, r3]
	cmp r1, #0
	bne .L_080ac854
	movs r1, #1
.L_080ac854:
	ldr r0, [sp, #48]
	bl Menu_GetModuloOfSum
	str r0, [sp, #48]
	ldr r0, [sp, #56]
	movs r7, #2
	lsls r0, r0, #3
	str r7, [sp, #72]
	str r0, [sp, #24]
	bl .L_080ab834
.L_080ac86a:
	ldr r3, [sp, #80]
	add r1, sp, #56
	ldrb r2, [r1]
	ldr r1, [sp, #76]
	adds r3, #28
	strb r2, [r1, r3]
	ldr r2, [sp, #60]
	movs r3, #1
	negs r3, r3
	cmp r2, r3
	beq .L_080ac8cc
	ldr r2, [sp, #56]
	ldr r7, [sp, #52]
	movs r0, #188
	lsls r0, r0, #1
	lsls r3, r2, #2
	adds r1, r7, r0
	adds r3, r3, r2
	ldr r7, [sp, #60]
	lsls r3, r3, #1
	adds r3, r3, r7
	lsls r3, r3, #1
	mov r0, r9
	ldrh r2, [r0, r3]
	ldr r3, [sp, #76]
	strh r2, [r3, r1]
	ldr r7, [sp, #80]
	movs r1, #149
	lsls r1, r1, #2
	adds r0, r7, r1
	movs r3, #31
	ldr r7, [sp, #76]
	ands r3, r2
	strb r3, [r7, r0]
	movs r3, #224
	adds r1, r7, #0
	ands r3, r2
	adds r1, #2
	lsrs r3, r3, #5
	strb r3, [r1, r0]
	ldr r0, [sp, #80]
	movs r3, #150
	lsls r3, r3, #2
	adds r1, r0, r3
	movs r3, #240
	lsls r3, r3, #4
	ands r3, r2
	lsrs r3, r3, #8
	strb r3, [r7, r1]
.L_080ac8cc:
	ldr r1, [sp, #48]
	ldr r7, [sp, #52]
	movs r0, #186
	lsls r0, r0, #1
	lsls r3, r1, #2
	adds r2, r7, r0
	adds r3, r3, r1
	ldr r7, [sp, #56]
	ldr r0, [sp, #76]
	lsls r3, r3, #1
	adds r3, r7, r3
	strh r3, [r0, r2]
	add sp, #108
	adds r0, r5, #0
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080ac8f8:
	.4byte 0x00000219
