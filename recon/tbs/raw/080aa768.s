.syntax unified
	.thumb
	.global Func_080aa768
Func_080aa768:
	.global Unnamed_080aa768
	.thumb_func
Unnamed_080aa768:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_080aaa8c
	ldr r7, [r3]
	movs r4, #0
	ldr r2, [r7, #20]
	mov r10, r4
	movs r3, #13
	strb r3, [r2, #5]
	mov r3, r10
	sub sp, #8
	movs r1, #0
	strh r3, [r2, #12]
	str r4, [sp, #0]
	mov r8, r1
	bl Menu_OpenBackdropScreen
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #0]
	movs r5, #2
.L_080aa798:
	cmp r5, #15
	bls .L_080aa79e
	b .L_080aac52
.L_080aa79e:
	ldr r2, .L_080aaa90
	lsls r3, r5, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080aa7a8:
	.4byte .L_080aa7e8
	.4byte .L_080aac56
	.4byte .L_080aa7fa
	.4byte .L_080aa8b0
	.4byte .L_080aabc0
	.4byte .L_080aabec
	.4byte .L_080aaaa4
	.4byte .L_080aa9be
	.4byte .L_080aa880
	.4byte .L_080aaaec
	.4byte .L_080aa83c
	.4byte .L_080aac06
	.4byte .L_080aaac0
	.4byte .L_080aa9da
	.4byte .L_080aab08
	.4byte .L_080aa86a
.L_080aa7e8:
	cmp r4, #0
	blt .L_080aa7ee
	b .L_080aabbc
.L_080aa7ee:
	movs r1, #1
	negs r1, r1
	movs r2, #1
	mov r10, r1
	mov r8, r2
	b .L_080aabbc
.L_080aa7fa:
	movs r0, #0
	bl Menu_SetFirstObjectRowCoordinates
	movs r1, #0
	movs r2, #200
	movs r3, #0
	movs r0, #1
	bl FourObjectMotion_SetSlotPosition
	movs r0, #0
	bl DjinnMenu_SelectDjinn
	adds r4, r0, #0
	movs r5, #15
	cmp r4, #10
	bne .L_080aa81c
	b .L_080aac56
.L_080aa81c:
	movs r5, #0
	cmp r4, #0
	bge .L_080aa824
	b .L_080aac56
.L_080aa824:
	movs r1, #187
	movs r3, #28
	ldrsb r3, [r7, r3]
	lsls r1, r1, #1
	adds r2, r7, r1
	strh r3, [r2]
	movs r5, #10
	cmp r4, #7
	bne .L_080aa838
	b .L_080aac56
.L_080aa838:
	movs r5, #3
	b .L_080aac56
.L_080aa83c:
	movs r3, #28
	ldrsb r3, [r7, r3]
	movs r2, #130
	lsls r2, r2, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r2, [r7, r3]
	ldr r1, .L_080aaa94
	str r2, [r7, #8]
	ldrh r2, [r7, r3]
	adds r3, r7, r1
	strb r2, [r3]
	bl Unnamed_080ae2f4
	movs r2, #2
	adds r4, r0, #0
	negs r2, r2
	cmp r4, r2
	beq .L_080aa864
	b .L_080aabbc
.L_080aa864:
	movs r3, #1
	mov r8, r3
	b .L_080aabbc
.L_080aa86a:
	bl DjinnMenu_ShowHelp
	movs r1, #2
	adds r4, r0, #0
	negs r1, r1
	cmp r4, r1
	beq .L_080aa87a
	b .L_080aabbc
.L_080aa87a:
	movs r2, #1
	mov r8, r2
	b .L_080aabbc
.L_080aa880:
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r7, r1
	ldrb r3, [r3]
	movs r5, #0
	cmp r3, #0
	bne .L_080aa890
	b .L_080aac56
.L_080aa890:
	movs r0, #1
	bl DjinnMenu_SelectDjinn
	movs r2, #2
	adds r4, r0, #0
	negs r2, r2
	cmp r4, r2
	bne .L_080aa8a4
	movs r3, #1
	mov r8, r3
.L_080aa8a4:
	movs r5, #4
	cmp r4, #0
	bge .L_080aa8ac
	b .L_080aac56
.L_080aa8ac:
	movs r5, #9
	b .L_080aac56
.L_080aa8b0:
	movs r1, #194
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r0, [r3]
	bl DjinnMenu_DrawElementList
	movs r0, #8
	negs r0, r0
	bl Menu_SetFirstObjectRowCoordinates
	movs r3, #28
	ldrsb r3, [r7, r3]
	movs r2, #130
	lsls r2, r2, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r2, [r7, r3]
	ldr r1, .L_080aaa94
	str r2, [r7, #8]
	ldrh r2, [r7, r3]
	adds r3, r7, r1
	strb r2, [r3]
	movs r3, #28
	ldrsb r3, [r7, r3]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #3
	adds r1, #48
	movs r2, #54
	movs r3, #0
	movs r0, #0
	bl FourObjectMotion_SetSlotPosition
	movs r0, #1
	bl DjinnMenu_SelectDjinn
	ldr r2, .L_080aaa98
	adds r3, r7, r2
	ldrb r3, [r3]
	movs r1, #0
	adds r4, r0, #0
	cmp r1, r3
	bge .L_080aa91a
	adds r0, r7, r2
	subs r2, #213
.L_080aa90a:
	ldrh r3, [r2, r7]
	adds r3, #8
	strh r3, [r2, r7]
	ldrb r3, [r0]
	adds r1, #1
	adds r2, #2
	cmp r1, r3
	blt .L_080aa90a
.L_080aa91a:
	movs r3, #2
	negs r3, r3
	cmp r4, r3
	bne .L_080aa926
	movs r1, #1
	mov r8, r1
.L_080aa926:
	cmp r4, #0
	bge .L_080aa92c
	b .L_080aabbc
.L_080aa92c:
	subs r3, r4, #3
	cmp r3, #1
	bls .L_080aa93a
	cmp r4, #8
	beq .L_080aa93a
	cmp r4, #9
	bne .L_080aa94e
.L_080aa93a:
	movs r3, #29
	ldrsb r3, [r7, r3]
	movs r2, #130
	lsls r2, r2, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	ldr r1, .L_080aaa9c
	ldrh r2, [r7, r3]
	adds r3, r7, r1
	strb r2, [r3]
.L_080aa94e:
	cmp r4, #0
	bge .L_080aa954
	b .L_080aabbc
.L_080aa954:
	cmp r4, #1
	bne .L_080aa95c
	movs r5, #5
	b .L_080aac56
.L_080aa95c:
	cmp r4, #2
	bne .L_080aa964
	movs r5, #6
	b .L_080aac56
.L_080aa964:
	cmp r4, #3
	bne .L_080aa976
	movs r3, #136
	lsls r3, r3, #2
	adds r2, r7, r3
	movs r3, #2
	strh r3, [r2]
	movs r5, #7
	b .L_080aac56
.L_080aa976:
	cmp r4, #4
	bne .L_080aa988
	movs r1, #136
	lsls r1, r1, #2
	adds r2, r7, r1
	movs r3, #2
	strh r3, [r2]
	movs r5, #9
	b .L_080aac56
.L_080aa988:
	cmp r4, #5
	bne .L_080aa990
	movs r5, #11
	b .L_080aac56
.L_080aa990:
	cmp r4, #6
	bne .L_080aa998
	movs r5, #12
	b .L_080aac56
.L_080aa998:
	cmp r4, #8
	bne .L_080aa9aa
	movs r3, #136
	lsls r3, r3, #2
	adds r2, r7, r3
	movs r3, #2
	strh r3, [r2]
	movs r5, #13
	b .L_080aac56
.L_080aa9aa:
	cmp r4, #9
	beq .L_080aa9b0
	b .L_080aac56
.L_080aa9b0:
	movs r1, #136
	lsls r1, r1, #2
	adds r2, r7, r1
	movs r3, #2
	strh r3, [r2]
	movs r5, #14
	b .L_080aac56
.L_080aa9be:
	movs r0, #1
	bl OwnerAction_RunCompareLoop
	movs r2, #2
	adds r4, r0, #0
	negs r2, r2
	cmp r4, r2
	bne .L_080aa9d2
	movs r3, #1
	mov r8, r3
.L_080aa9d2:
	movs r5, #3
	cmp r4, #0
	bge .L_080aa9da
	b .L_080aac56
.L_080aa9da:
	movs r0, #126
	bl AudioCommand_PlayFar
	ldr r2, .L_080aaaa0
	ldr r1, .L_080aaa94
	adds r3, r7, r2
	subs r2, #2
	adds r6, r7, r1
	ldrb r1, [r3]
	adds r3, r7, r2
	ldrb r2, [r3]
	ldr r3, .L_080aaa9c
	adds r5, r7, r3
	ldrb r3, [r5]
	ldrb r0, [r6]
	bl Djinn_TransferFar
	adds r4, r0, #0
	ldrb r0, [r6]
	str r4, [sp, #0]
	bl Owner_RecalculateStatsFar
	ldrb r0, [r5]
	bl Owner_RecalculateStatsFar
	ldr r2, [r7, #20]
	movs r3, #13
	strb r3, [r2, #5]
	ldr r0, [r7, #48]
	bl RenderOutput_ClearListFar
	movs r1, #194
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r0, [r3]
	bl Menu_ComputeEntryValues
	movs r2, #187
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrh r0, [r3]
	movs r1, #10
	bl __umodsi3
	movs r1, #188
	movs r3, #0
	lsls r1, r1, #1
	mov r12, r3
	adds r3, r7, r1
	lsls r0, r0, #16
	ldrb r6, [r3]
	lsrs r0, r0, #16
	movs r1, #0
	adds r5, r0, #0
	adds r5, #160
	ldr r4, [sp, #0]
	b .L_080aaa4e
.L_080aaa4c:
	adds r1, #1
.L_080aaa4e:
	movs r2, #194
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r2, [r3]
	ldrsb r3, [r2, r5]
	cmp r1, r3
	bge .L_080aaa6e
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	adds r3, r3, r1
	lsls r3, r3, #1
	ldrb r3, [r2, r3]
	cmp r6, r3
	bne .L_080aaa4c
	mov r12, r1
.L_080aaa6e:
	mov r1, r12
	lsls r3, r1, #2
	add r3, r12
	movs r1, #186
	lsls r1, r1, #1
	lsls r3, r3, #1
	adds r3, r0, r3
	adds r2, r7, r1
	strh r3, [r2]
	ldr r2, [r7, #20]
	movs r3, #1
	strb r3, [r2, #5]
	movs r5, #0
	b .L_080aac56
	.2byte 0x0000
.L_080aaa8c:
	.4byte gMenuWork
.L_080aaa90:
	.4byte .L_080aa7a8
.L_080aaa94:
	.4byte 0x0000021a
.L_080aaa98:
	.4byte 0x00000219
.L_080aaa9c:
	.4byte 0x0000021b
.L_080aaaa0:
	.4byte 0x00000256
.L_080aaaa4:
	movs r0, #2
	bl OwnerAction_RunCompareLoop
	movs r2, #2
	adds r4, r0, #0
	negs r2, r2
	cmp r4, r2
	bne .L_080aaab8
	movs r3, #1
	mov r8, r3
.L_080aaab8:
	movs r5, #3
	cmp r4, #0
	bge .L_080aaac0
	b .L_080aac56
.L_080aaac0:
	movs r0, #175
	bl AudioCommand_PlayFar
	ldr r2, .L_080aac70
	ldr r1, .L_080aac74
	adds r3, r7, r2
	subs r2, #2
	adds r6, r7, r1
	adds r5, r7, r2
	ldrb r1, [r3]
	ldrb r2, [r5]
	ldrb r0, [r6]
	str r3, [sp, #4]
	bl Djinn_DeactivateFar
	ldr r3, [sp, #4]
	ldrb r2, [r5]
	ldrb r1, [r3]
	ldrb r0, [r6]
	bl Trade_AddOfferFar
	b .L_080aac30
.L_080aaaec:
	movs r0, #0
	bl OwnerAction_RunCompareLoop
	movs r3, #2
	adds r4, r0, #0
	negs r3, r3
	cmp r4, r3
	bne .L_080aab00
	movs r1, #1
	mov r8, r1
.L_080aab00:
	movs r5, #3
	cmp r4, #0
	bge .L_080aab08
	b .L_080aac56
.L_080aab08:
	movs r0, #126
	bl AudioCommand_PlayFar
	ldr r2, .L_080aac74
	ldr r1, .L_080aac70
	adds r6, r7, r2
	adds r3, r7, r1
	adds r2, #58
	ldrb r1, [r3]
	adds r3, r7, r2
	ldrb r2, [r3]
	ldr r3, .L_080aac78
	adds r5, r7, r3
	ldrb r3, [r5]
	ldrb r0, [r6]
	bl Djinn_TransferFar
	ldr r1, .L_080aac7c
	ldr r2, .L_080aac80
	adds r3, r7, r1
	ldrb r1, [r3]
	adds r3, r7, r2
	ldrb r2, [r3]
	ldrb r0, [r5]
	ldrb r3, [r6]
	bl Djinn_TransferFar
	adds r4, r0, #0
	ldrb r0, [r6]
	str r4, [sp, #0]
	bl Owner_RecalculateStatsFar
	ldrb r0, [r5]
	bl Owner_RecalculateStatsFar
	movs r1, #194
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r0, [r3]
	bl Menu_ComputeEntryValues
	movs r2, #187
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrh r0, [r3]
	movs r1, #10
	bl __umodsi3
	movs r1, #188
	movs r3, #0
	lsls r1, r1, #1
	mov r12, r3
	adds r3, r7, r1
	lsls r0, r0, #16
	ldrb r6, [r3]
	lsrs r0, r0, #16
	movs r1, #0
	adds r5, r0, #0
	adds r5, #160
	ldr r4, [sp, #0]
	b .L_080aab84
.L_080aab82:
	adds r1, #1
.L_080aab84:
	movs r2, #194
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r2, [r3]
	ldrsb r3, [r2, r5]
	cmp r1, r3
	bge .L_080aaba4
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	adds r3, r3, r1
	lsls r3, r3, #1
	ldrb r3, [r2, r3]
	cmp r6, r3
	bne .L_080aab82
	mov r12, r1
.L_080aaba4:
	mov r1, r12
	lsls r3, r1, #2
	add r3, r12
	movs r1, #186
	lsls r1, r1, #1
	lsls r3, r3, #1
	adds r3, r0, r3
	adds r2, r7, r1
	strh r3, [r2]
	ldr r2, [r7, #20]
	movs r3, #1
	strb r3, [r2, #5]
.L_080aabbc:
	movs r5, #2
	b .L_080aac56
.L_080aabc0:
	movs r2, #1
	negs r2, r2
	cmp r4, r2
	bne .L_080aabcc
	mov r10, r4
	b .L_080aabbc
.L_080aabcc:
	movs r1, #136
	lsls r1, r1, #2
	adds r3, r7, r1
	ldrh r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080aabe0
	movs r5, #8
	b .L_080aac56
.L_080aabe0:
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080aac56
	movs r5, #7
	b .L_080aac56
.L_080aabec:
	movs r0, #3
	bl OwnerAction_RunCompareLoop
	movs r2, #2
	adds r4, r0, #0
	negs r2, r2
	cmp r4, r2
	bne .L_080aac00
	movs r3, #1
	mov r8, r3
.L_080aac00:
	movs r5, #3
	cmp r4, #0
	blt .L_080aac56
.L_080aac06:
	movs r0, #139
	bl AudioCommand_PlayFar
	ldr r2, .L_080aac70
	ldr r1, .L_080aac74
	adds r3, r7, r2
	subs r2, #2
	adds r6, r7, r1
	adds r5, r7, r2
	ldrb r1, [r3]
	ldrb r2, [r5]
	ldrb r0, [r6]
	str r3, [sp, #4]
	bl Djinn_ActivateFar
	ldr r3, [sp, #4]
	ldrb r2, [r5]
	ldrb r1, [r3]
	ldrb r0, [r6]
	bl Trade_RemoveOfferFar
.L_080aac30:
	adds r4, r0, #0
	ldrb r0, [r6]
	str r4, [sp, #0]
	bl Owner_RecalculateStatsFar
	ldr r2, [r7, #20]
	movs r3, #13
	strb r3, [r2, #5]
	ldr r0, [r7, #48]
	bl RenderOutput_ClearListFar
	ldr r2, [r7, #20]
	movs r3, #1
	strb r3, [r2, #5]
	movs r5, #2
	ldr r4, [sp, #0]
	b .L_080aac56
.L_080aac52:
	movs r3, #1
	mov r8, r3
.L_080aac56:
	mov r1, r8
	cmp r1, #0
	bne .L_080aac5e
	b .L_080aa798
.L_080aac5e:
	mov r0, r10
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_080aac70:
	.4byte 0x00000256
.L_080aac74:
	.4byte 0x0000021a
.L_080aac78:
	.4byte 0x0000021b
.L_080aac7c:
	.4byte 0x00000257
.L_080aac80:
	.4byte 0x00000255
