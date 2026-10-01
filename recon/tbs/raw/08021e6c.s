.syntax unified
	.thumb
	.global Ui_RunSelectionScreen
	.thumb_func
Ui_RunSelectionScreen:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #60
	str r0, [sp, #52]
	ldr r3, .L_08021f64
	movs r0, #128
	ldr r3, [r3]
	lsls r0, r0, #3
	str r3, [sp, #48]
	bl Resource_LoadIntoFreeSlot
	adds r7, r0, #0
	movs r0, #1
	str r0, [sp, #44]
	movs r0, #240
	movs r1, #0
	lsls r0, r0, #1
	str r1, [sp, #40]
	bl Runtime_BumpAllocateAlternatePool
	mov r9, r0
	mov r3, r9
	movs r2, #255
	adds r3, #255
	mov r12, r9
.L_08021ea8:
	strb r2, [r3]
	subs r3, #1
	cmp r3, r12
	bge .L_08021ea8
	movs r2, #128
	lsls r2, r2, #1
	add r2, r9
	movs r3, #1
	str r3, [r2]
	ldr r2, [sp, #52]
	cmp r2, #0
	bne .L_08021ece
	movs r3, #6
	str r3, [sp, #0]
	movs r0, #20
	movs r3, #3
	movs r1, #17
	movs r2, #10
	b .L_08021eda
.L_08021ece:
	movs r3, #6
	str r3, [sp, #0]
	movs r0, #22
	movs r3, #3
	movs r1, #17
	movs r2, #8
.L_08021eda:
	bl UiWindow_Create
	movs r3, #214
	lsls r3, r3, #1
	add r3, r9
	str r0, [r3]
	movs r3, #226
	lsls r3, r3, #1
	movs r0, #1
	add r3, r9
	negs r0, r0
	str r0, [r3]
	ldr r3, [sp, #52]
	cmp r3, #0
	bne .L_08021f6c
	movs r2, #228
	lsls r2, r2, #1
	mov r4, r9
	movs r3, #14
	str r3, [r4, r2]
	movs r1, #4
	adds r3, r2, #4
	str r1, [r4, r3]
	adds r1, r2, #0
	movs r3, #7
	adds r1, #8
	str r3, [r4, r1]
	ldr r3, .L_08021f68
	adds r2, #12
	str r0, [r4, r2]
	ldr r3, [r3]
	movs r2, #224
	lsls r2, r2, #1
	ldr r3, [r3, #60]
	add r2, r9
	str r3, [r2]
	b .L_08021ff0
.L_08021f24:
	movs r5, #2
	negs r5, r5
	mov r10, r5
	ldr r2, .L_08021f68
	b .L_0802260c
.L_08021f2e:
	movs r3, #224
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	movs r6, #228
	lsls r3, r3, #2
	lsls r6, r6, #1
	adds r3, r3, r6
	mov r7, r9
	ldr r3, [r7, r3]
	ldr r2, .L_08021f68
	mov r10, r3
	b .L_0802260c
.L_08021f48:
	movs r0, #113
	bl AudioCommand_PlayFar
	movs r0, #1
	negs r0, r0
	mov r10, r0
	ldr r2, .L_08021f68
	b .L_0802260c
.L_08021f58:
	movs r3, #228
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	mov r10, r3
	b .L_0802260c
.L_08021f64:
	.4byte gWindowWork
.L_08021f68:
	.4byte gLinkCountdownWork
.L_08021f6c:
	ldr r3, .L_0802213c
	ldr r3, [r3]
	movs r2, #224
	ldr r3, [r3, #64]
	lsls r2, r2, #1
	add r2, r9
	str r3, [r2]
	movs r3, #228
	mov r1, r9
	movs r5, #0
	lsls r3, r3, #1
	str r5, [r1, r3]
	adds r3, #4
	movs r5, #1
	str r5, [r1, r3]
	movs r0, #0
	bl Trade_GetOfferStateFar
	ldr r3, [r0]
	movs r5, #2
	cmp r3, #0
	beq .L_08021fce
	add r5, sp, #56
	movs r0, #0
	adds r1, r5, #0
	bl BattlePlacement_CountValidEntriesFar
	mov r2, sp
	adds r2, #59
.L_08021fa6:
	ldrb r3, [r5]
	ldr r4, [sp, #40]
	adds r5, #1
	adds r4, r4, r3
	str r4, [sp, #40]
	cmp r5, r2
	ble .L_08021fa6
	movs r2, #232
	mov r5, r9
	lsls r2, r2, #1
	movs r3, #15
	str r3, [r5, r2]
	movs r5, #3
	cmp r4, #0
	beq .L_08021fce
	adds r2, #4
	movs r3, #16
	mov r6, r9
	str r3, [r6, r2]
	movs r5, #4
.L_08021fce:
	movs r1, #228
	lsls r1, r1, #1
	lsls r3, r5, #2
	mov r0, r9
	adds r3, r3, r1
	movs r2, #2
	adds r5, #1
	str r2, [r0, r3]
	lsls r3, r5, #2
	adds r3, r3, r1
	movs r2, #3
	adds r5, #1
	str r2, [r0, r3]
	lsls r3, r5, #2
	adds r3, r3, r1
	subs r2, #4
	str r2, [r0, r3]
.L_08021ff0:
	movs r5, #136
	lsls r5, r5, #1
	add r5, r9
	movs r6, #228
	strh r7, [r5]
	lsls r6, r6, #1
	add r6, r9
	movs r5, #150
	ldr r2, [r6]
	movs r3, #1
	movs r6, #230
	movs r1, #0
	lsls r5, r5, #1
	lsls r6, r6, #1
	negs r3, r3
	mov r8, r1
	add r5, r9
	add r6, r9
	cmp r2, r3
	beq .L_08022038
.L_08022018:
	mov r0, r9
	mov r1, r8
	bl UiText_DrawCharacter
	movs r4, #1
	add r8, r4
	mov r0, r8
	cmp r0, #5
	bgt .L_08022038
	strh r7, [r5]
	movs r1, #1
	ldmia r6!, {r2}
	negs r1, r1
	adds r5, #28
	cmp r2, r1
	bne .L_08022018
.L_08022038:
	movs r0, #216
	lsls r0, r0, #1
	mov r2, r8
	add r0, r9
	str r2, [r0]
	movs r2, #218
	lsls r2, r2, #1
	movs r3, #160
	add r2, r9
	lsls r3, r3, #1
	strh r3, [r2]
	movs r2, #219
	lsls r2, r2, #1
	subs r3, #16
	add r2, r9
	strh r3, [r2]
	adds r3, #136
	movs r1, #0
	add r3, r9
	strh r1, [r3]
	ldr r3, [r0]
	mov r8, r1
	cmp r8, r3
	bge .L_080220a6
	movs r1, #140
	adds r4, r0, #0
	lsls r1, r1, #1
	movs r0, #138
	lsls r0, r0, #1
	movs r5, #136
	add r1, r9
.L_08022076:
	ldr r3, [r4]
	mov r6, r8
	subs r3, r6, r3
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #3
	adds r3, r2, #0
	adds r3, #155
	mov r7, r9
	str r3, [r0, r7]
	ldr r3, [sp, #52]
	cmp r3, #0
	beq .L_08022096
	adds r3, r2, #0
	adds r3, #171
	str r3, [r0, r7]
.L_08022096:
	str r5, [r1]
	movs r6, #1
	ldr r3, [r4]
	add r8, r6
	adds r1, #28
	adds r0, #28
	cmp r8, r3
	blt .L_08022076
.L_080220a6:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_08022140
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_08022144
	movs r0, #2
	movs r1, #136
	bl Runtime_SetIrqHandler
	movs r0, #216
	movs r2, #224
	lsls r0, r0, #1
	lsls r2, r2, #1
	add r0, r9
	add r2, r9
	adds r7, r0, #0
	adds r1, r2, #0
	str r7, [sp, #16]
	str r1, [sp, #24]
.L_080220ce:
	ldr r3, .L_08022148
	ldr r3, [r3]
	movs r2, #31
	lsls r3, r3, #1
	ldr r1, .L_0802214c
	ands r3, r2
	lsls r3, r3, #1
	ldrh r1, [r1, r3]
	ldr r4, .L_08022150
	adds r3, r1, r4
	str r1, [sp, #36]
	cmp r3, #0
	bge .L_080220ec
	adds r3, r1, #0
	subs r3, #253
.L_080220ec:
	movs r5, #152
	asrs r3, r3, #2
	lsls r5, r5, #1
	adds r5, r3, r5
	str r5, [sp, #36]
	movs r3, #218
	add r6, sp, #36
	ldrh r6, [r6]
	lsls r3, r3, #1
	add r3, r9
	strh r6, [r3]
	movs r3, #219
	lsls r3, r3, #1
	add r3, r9
	ldr r2, .L_08022154
	adds r7, r6, #0
	ldr r0, .L_08022138
	strh r7, [r3]
	movs r3, #32
	strh r0, [r2, #4]
	strh r3, [r2, #6]
	ldr r1, [sp, #44]
	cmp r1, #0
	bne .L_0802211e
	b .L_0802236c
.L_0802211e:
	ldr r3, [sp, #48]
	ldr r4, .L_08022158
	movs r2, #0
	str r2, [sp, #44]
	adds r2, r3, r4
	movs r3, #1
	strb r3, [r2]
	ldr r5, [sp, #52]
	cmp r5, #0
	bne .L_08022134
	b .L_08022264
.L_08022134:
	b .L_0802215c
	.2byte 0x0000
.L_08022138:
	.4byte 0x00000000
.L_0802213c:
	.4byte gLinkCountdownWork
.L_08022140:
	.4byte Graphics_SetBg1Priority3
.L_08022144:
	.4byte Graphics_ClearBg1ControlBit2
.L_08022148:
	.4byte gFrameCount
.L_0802214c:
	.4byte Data_080366f8
.L_08022150:
	.4byte 0xffffff00
.L_08022154:
	.4byte gBgScroll
.L_08022158:
	.4byte 0x00000ea6
.L_0802215c:
	ldr r7, [sp, #16]
	ldr r3, [r7]
	movs r6, #0
	movs r2, #6
	mov lr, r6
	subs r3, r2, r3
	cmp lr, r3
	bge .L_080221c2
	movs r3, #216
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	ldr r6, .L_08022194
	subs r2, r2, r3
	movs r7, #3
	mov r12, r2
	movs r4, #0
.L_0802217e:
	mov r1, lr
	adds r3, r4, r1
	ldr r2, [sp, #48]
	movs r0, #0
	lsls r3, r3, #1
	mov r8, r0
	adds r0, r3, r2
.L_0802218c:
	movs r2, #0
	adds r1, r0, #0
	b .L_08022198
	.2byte 0x0000
.L_08022194:
	.4byte 0x0000f07f
.L_08022198:
	adds r3, r2, #0
	ands r3, r7
	lsls r3, r3, #1
	movs r5, #137
	adds r3, r3, r1
	lsls r5, r5, #3
	adds r3, r3, r5
	adds r2, #1
	strh r6, [r3]
	cmp r2, #2
	ble .L_08022198
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r0, #64
	cmp r2, #2
	ble .L_0802218c
	add lr, r1
	adds r4, #2
	cmp lr, r12
	blt .L_0802217e
.L_080221c2:
	ldr r4, [sp, #16]
	movs r3, #0
	mov lr, r3
	ldr r3, [r4]
	cmp lr, r3
	blt .L_080221d0
	b .L_0802236c
.L_080221d0:
	movs r3, #216
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	mov r11, r3
	lsls r3, r3, #1
	add r3, r11
	mov r10, r3
	mov r5, r10
	lsls r5, r5, #1
	movs r6, #0
	str r5, [sp, #32]
	str r6, [sp, #8]
.L_080221ea:
	ldr r2, [sp, #8]
	ldr r0, [sp, #48]
	add r2, lr
	movs r7, #0
	lsls r3, r2, #1
	mov r1, lr
	mov r8, r7
	adds r4, r3, r0
	adds r6, r2, #0
	lsls r5, r1, #4
.L_080221fe:
	str r5, [sp, #4]
	movs r0, #0
	mov r12, r6
.L_08022204:
	adds r1, r0, #0
	movs r2, #3
	ands r1, r2
	mov r3, r12
	adds r2, r3, r1
	mov r7, r10
	subs r2, r2, r7
	ldr r3, .L_0802225c
	ldr r7, [sp, #4]
	lsls r2, r2, #1
	adds r2, r2, r3
	adds r3, r7, r0
	movs r7, #128
	lsls r7, r7, #1
	adds r3, r3, r7
	strh r3, [r2]
	ldr r2, [sp, #32]
	lsls r1, r1, #1
	adds r1, r4, r1
	ldr r3, .L_08022260
	subs r1, r1, r2
	ldr r7, .L_08022258
	adds r1, r1, r3
	adds r0, #1
	strh r7, [r1]
	cmp r0, #2
	ble .L_08022204
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r4, #64
	adds r6, #32
	adds r5, #4
	cmp r1, #2
	ble .L_080221fe
	ldr r2, [sp, #8]
	add lr, r0
	adds r2, #2
	str r2, [sp, #8]
	cmp lr, r11
	blt .L_080221ea
	b .L_0802236c
.L_08022258:
	.4byte 0x00000000
.L_0802225c:
	.4byte 0x0600fd6c
.L_08022260:
	.4byte 0x0000046c
.L_08022264:
	ldr r4, [sp, #16]
	movs r3, #0
	mov lr, r3
	ldr r3, [r4]
	movs r2, #6
	subs r3, r2, r3
	cmp lr, r3
	bge .L_080222cc
	movs r3, #216
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	ldr r6, .L_080222b0
	subs r2, r2, r3
	movs r7, #3
	mov r12, r2
	movs r4, #0
.L_08022286:
	mov r0, lr
	adds r3, r4, r0
	ldr r1, [sp, #48]
	movs r5, #0
	lsls r3, r3, #1
	mov r8, r5
	adds r0, r3, r1
.L_08022294:
	movs r2, #0
	adds r1, r0, #0
.L_08022298:
	adds r3, r2, #0
	ands r3, r7
	lsls r3, r3, #1
	ldr r5, .L_080222b4
	adds r3, r3, r1
	adds r3, r3, r5
	adds r2, #1
	strh r6, [r3]
	cmp r2, #2
	ble .L_08022298
	b .L_080222b8
	.2byte 0x0000
.L_080222b0:
	.4byte 0x0000f07f
.L_080222b4:
	.4byte 0x00000444
.L_080222b8:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r0, #64
	cmp r2, #2
	ble .L_08022294
	add lr, r1
	adds r4, #2
	cmp lr, r12
	blt .L_08022286
.L_080222cc:
	ldr r4, [sp, #16]
	movs r3, #0
	mov lr, r3
	ldr r3, [r4]
	cmp lr, r3
	bge .L_0802236c
	movs r3, #216
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	mov r11, r3
	lsls r3, r3, #1
	add r3, r11
	mov r10, r3
	mov r5, r10
	lsls r5, r5, #1
	movs r6, #0
	str r5, [sp, #28]
	str r6, [sp, #12]
.L_080222f2:
	ldr r2, [sp, #12]
	ldr r0, [sp, #48]
	add r2, lr
	movs r7, #0
	lsls r3, r2, #1
	mov r1, lr
	mov r8, r7
	adds r4, r3, r0
	adds r6, r2, #0
	lsls r5, r1, #4
.L_08022306:
	str r5, [sp, #4]
	movs r0, #0
	mov r12, r6
.L_0802230c:
	adds r1, r0, #0
	movs r2, #3
	ands r1, r2
	mov r3, r12
	adds r2, r3, r1
	mov r7, r10
	subs r2, r2, r7
	ldr r3, .L_08022368
	ldr r7, [sp, #4]
	lsls r2, r2, #1
	adds r2, r2, r3
	adds r3, r7, r0
	movs r7, #128
	lsls r7, r7, #1
	adds r3, r3, r7
	strh r3, [r2]
	ldr r2, [sp, #28]
	lsls r1, r1, #1
	adds r1, r4, r1
	movs r3, #141
	subs r1, r1, r2
	lsls r3, r3, #3
	ldr r7, .L_08022364
	adds r1, r1, r3
	adds r0, #1
	strh r7, [r1]
	cmp r0, #2
	ble .L_0802230c
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r4, #64
	adds r6, #32
	adds r5, #4
	cmp r1, #2
	ble .L_08022306
	ldr r2, [sp, #12]
	add lr, r0
	adds r2, #2
	str r2, [sp, #12]
	cmp lr, r11
	blt .L_080222f2
	b .L_0802236c
	.2byte 0x0000
.L_08022364:
	.4byte 0x00000000
.L_08022368:
	.4byte 0x0600fd68
.L_0802236c:
	movs r7, #226
	movs r6, #224
	lsls r7, r7, #1
	lsls r6, r6, #1
	add r7, r9
	add r6, r9
	ldr r2, [r7]
	ldr r3, [r6]
	cmp r2, r3
	beq .L_080223c4
	movs r5, #214
	lsls r5, r5, #1
	add r5, r9
	ldr r0, [r5]
	bl RenderOutput_PrepareForRedraw
	ldr r2, [r6]
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r4, #142
	lsls r4, r4, #1
	lsls r3, r3, #2
	adds r3, r3, r4
	mov r1, r9
	ldr r0, [r1, r3]
	ldr r3, .L_080223f8
	ldr r1, [r5]
	adds r0, r0, r3
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r3, [r6]
	str r3, [r7]
	ldr r1, [r6]
	movs r2, #228
	lsls r2, r2, #1
	lsls r3, r1, #2
	adds r3, r3, r2
	mov r4, r9
	ldr r2, [r4, r3]
	mov r0, r9
	bl UiText_DrawCharacter
.L_080223c4:
	movs r0, #218
	lsls r0, r0, #1
	add r0, r9
	bl AffineMatrix_BuildForEffect
	movs r3, #222
	lsls r3, r3, #1
	add r3, r9
	str r0, [r3]
	ldr r6, [sp, #16]
	movs r5, #0
	ldr r3, [r6]
	mov r8, r5
	cmp r8, r3
	bge .L_0802249e
	movs r5, #130
	movs r0, #63
	lsls r5, r5, #1
	negs r0, r0
	ldr r6, .L_080223f4
	add r5, r9
	adds r7, r0, #0
	b .L_080223fc
	.2byte 0x0000
.L_080223f4:
	.4byte 0xfffffe00
.L_080223f8:
	.4byte 0x0000001f
.L_080223fc:
	ldr r1, [sp, #24]
	ldr r3, [r1]
	cmp r8, r3
	bne .L_0802246c
	movs r3, #222
	lsls r3, r3, #1
	add r3, r9
	ldrb r2, [r3]
	movs r1, #31
	ldrb r3, [r5, #7]
	ands r2, r1
	lsls r2, r2, #1
	ands r3, r7
	orrs r3, r2
	strb r3, [r5, #7]
	ldrb r3, [r5, #5]
	movs r2, #3
	orrs r3, r2
	strb r3, [r5, #5]
	ldr r2, [sp, #36]
	lsls r3, r2, #3
	subs r2, r3, r2
	ldr r1, [r5, #16]
	cmp r2, #0
	bge .L_08022432
	ldr r3, .L_08022468
	adds r2, r2, r3
.L_08022432:
	asrs r2, r2, #9
	ldr r3, .L_08022464
	adds r2, r1, r2
	subs r2, #14
	ands r2, r3
	ldrh r3, [r5, #6]
	ands r3, r6
	orrs r3, r2
	strh r3, [r5, #6]
	ldr r4, [sp, #36]
	lsls r3, r4, #1
	adds r3, r3, r4
	ldr r2, [r5, #20]
	cmp r3, #0
	bge .L_08022452
	adds r3, #255
.L_08022452:
	asrs r3, r3, #8
	adds r3, r2, r3
	subs r3, #20
	strb r3, [r5, #4]
	adds r0, r5, #0
	movs r1, #241
	bl Runtime_PushSlotEntry
	b .L_08022490
.L_08022464:
	.4byte 0x000001ff
.L_08022468:
	.4byte 0x000001ff
.L_0802246c:
	ldr r3, .L_080224a4
	ldr r2, [r5, #16]
	ands r2, r3
	ldrh r3, [r5, #6]
	ands r3, r6
	orrs r3, r2
	strh r3, [r5, #6]
	ldr r3, [r5, #20]
	strb r3, [r5, #4]
	ldrb r3, [r5, #7]
	movs r0, #4
	ands r3, r7
	strb r3, [r5, #7]
	negs r0, r0
	ldrb r3, [r5, #5]
	adds r2, r0, #0
	ands r3, r2
	strb r3, [r5, #5]
.L_08022490:
	ldr r2, [sp, #16]
	movs r1, #1
	ldr r3, [r2]
	add r8, r1
	adds r5, #28
	cmp r8, r3
	blt .L_080223fc
.L_0802249e:
	ldr r3, .L_080224a8
	b .L_080224ac
	.2byte 0x0000
.L_080224a4:
	.4byte 0x000001ff
.L_080224a8:
	.4byte gKeyState
.L_080224ac:
	ldr r2, .L_08022734
	ldr r5, [r3]
	ldr r3, .L_08022738
	ldr r2, [r2]
	ldr r7, [r3]
	movs r3, #216
	adds r3, r3, r2
	ldr r1, [r3]
	mov r8, r3
	cmp r1, #0
	beq .L_08022574
	adds r6, r2, #0
	adds r6, #220
	ldr r3, [r6]
	movs r7, #0
	movs r5, #0
	cmp r3, #0
	bne .L_08022570
	adds r3, r2, #0
	adds r3, #224
	ldr r3, [r3]
	cmp r3, #1
	bne .L_080224fc
	ldr r4, [sp, #24]
	ldr r3, [r4]
	movs r5, #228
	lsls r3, r3, #2
	lsls r5, r5, #1
	adds r3, r3, r5
	mov r7, r9
	ldr r3, [r7, r3]
	cmp r3, #3
	bne .L_080224f4
	movs r7, #1
	movs r5, #1
	b .L_080224f8
.L_080224f4:
	movs r7, #32
	movs r5, #32
.L_080224f8:
	movs r3, #30
	b .L_08022572
.L_080224fc:
	cmp r3, #0
	bne .L_08022566
	ldr r0, [sp, #24]
	ldr r3, [r0]
	movs r2, #228
	lsls r3, r3, #2
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r4, r9
	ldr r3, [r4, r3]
	cmp r3, #16
	beq .L_0802251e
	ldr r0, [sp, #40]
	cmp r0, #0
	bne .L_0802255e
	cmp r3, #15
	bne .L_0802255e
.L_0802251e:
	cmp r1, #1
	bne .L_08022556
	cmp r3, #15
	bne .L_0802252a
	ldr r0, .L_0802273c
	b .L_08022530
.L_0802252a:
	cmp r3, #16
	bne .L_0802253a
	ldr r0, .L_08022740
.L_08022530:
	movs r1, #15
	movs r2, #8
	bl UiText_ShowMessageAndWaitComplete
	str r0, [sp, #20]
.L_0802253a:
	movs r1, #155
	movs r0, #102
	bl Func_080b5128
	movs r1, #1
	ldr r0, [sp, #20]
	bl UiWork_Finalize
	mov r1, r8
	ldr r3, [r1]
	adds r3, #1
	str r3, [r1]
	movs r3, #45
	b .L_08022572
.L_08022556:
	movs r3, #200
	movs r7, #1
	movs r5, #1
	b .L_08022572
.L_0802255e:
	movs r3, #40
	movs r7, #16
	movs r5, #16
	b .L_08022572
.L_08022566:
	movs r3, #60
	str r3, [r6]
	movs r7, #1
	movs r5, #1
	b .L_08022574
.L_08022570:
	subs r3, #1
.L_08022572:
	str r3, [r6]
.L_08022574:
	movs r3, #192
	lsls r3, r3, #2
	ands r3, r5
	cmp r3, #0
	beq .L_08022586
	ldr r2, [sp, #52]
	cmp r2, #0
	beq .L_08022586
	b .L_08021f24
.L_08022586:
	movs r3, #1
	ands r3, r5
	cmp r3, #0
	beq .L_08022590
	b .L_08021f2e
.L_08022590:
	ldr r3, [sp, #52]
	cmp r3, #0
	beq .L_080225a0
	movs r3, #2
	ands r3, r5
	cmp r3, #0
	beq .L_080225a0
	b .L_08021f48
.L_080225a0:
	movs r3, #144
	ands r3, r7
	cmp r3, #0
	beq .L_080225c2
	movs r0, #111
	bl AudioCommand_PlayFar
	ldr r4, [sp, #24]
	ldr r5, [sp, #16]
	ldr r0, [r4]
	ldr r1, [r5]
	adds r0, #1
	bl Math_Mod
	ldr r6, [sp, #24]
	str r0, [r6]
	b .L_080225f0
.L_080225c2:
	movs r3, #96
	ands r3, r7
	cmp r3, #0
	beq .L_080225e4
	movs r0, #111
	bl AudioCommand_PlayFar
	ldr r7, [sp, #24]
	ldr r2, [sp, #16]
	ldr r0, [r7]
	ldr r1, [r2]
	adds r0, r0, r1
	subs r0, #1
	bl Math_Mod
	str r0, [r7]
	b .L_080225f0
.L_080225e4:
	ldr r2, .L_08022734
	ldr r3, [r2]
	ldr r3, [r3, #76]
	cmp r3, #0
	bne .L_080225f0
	b .L_08021f58
.L_080225f0:
	movs r0, #128
	lsls r0, r0, #19
	ldr r1, .L_08022744
	bl QueueIoWriteDelay2
	ldr r5, .L_08022748
	ldr r4, [sp, #48]
	movs r6, #0
	adds r3, r4, r5
	strb r6, [r3]
	movs r0, #1
	bl WaitFrames
	b .L_080220ce
.L_0802260c:
	ldr r7, [sp, #52]
	cmp r7, #0
	beq .L_08022620
	ldr r3, [r2]
	movs r2, #224
	lsls r2, r2, #1
	add r2, r9
	ldr r2, [r2]
	str r2, [r3, #64]
	b .L_0802262c
.L_08022620:
	ldr r3, [r2]
	movs r2, #224
	lsls r2, r2, #1
	add r2, r9
	ldr r2, [r2]
	str r2, [r3, #60]
.L_0802262c:
	movs r3, #216
	lsls r3, r3, #1
	add r3, r9
	movs r0, #0
	ldr r3, [r3]
	mov r8, r0
	cmp r8, r3
	bge .L_0802265a
	movs r6, #216
	movs r5, #136
	lsls r6, r6, #1
	lsls r5, r5, #1
	add r6, r9
	add r5, r9
.L_08022648:
	ldrh r0, [r5]
	bl Resource_ResetEntry
	movs r1, #1
	ldr r3, [r6]
	add r8, r1
	adds r5, #28
	cmp r8, r3
	blt .L_08022648
.L_0802265a:
	ldr r3, [sp, #48]
	ldr r4, .L_08022748
	adds r2, r3, r4
	movs r3, #1
	strb r3, [r2]
	movs r3, #214
	lsls r3, r3, #1
	add r3, r9
	ldr r6, .L_0802274c
	movs r5, #3
	ldr r0, [r3]
	movs r1, #1
	ldr r7, .L_08022750
	bl UiWork_Finalize
	mov r12, r5
	movs r4, #0
	movs r5, #0
	mov lr, r6
.L_08022680:
	adds r3, r5, r4
	ldr r2, [sp, #48]
	movs r0, #0
	lsls r3, r3, #1
	mov r8, r0
	adds r1, r3, r2
.L_0802268c:
	movs r2, #0
	adds r0, r1, #0
.L_08022690:
	adds r3, r2, #0
	mov r6, r12
	ands r3, r6
	lsls r3, r3, #1
	adds r3, r3, r0
	adds r3, r3, r7
	mov r6, lr
	adds r2, #1
	strh r6, [r3]
	cmp r2, #2
	ble .L_08022690
	movs r0, #1
	add r8, r0
	mov r2, r8
	adds r1, #64
	cmp r2, #2
	ble .L_0802268c
	adds r4, #1
	adds r5, #2
	cmp r4, #6
	ble .L_08022680
	ldr r3, [sp, #48]
	ldr r4, .L_08022754
	adds r2, r3, r4
	movs r3, #1
	strb r3, [r2]
	bl WaitFrames
	ldr r0, .L_08022758
	bl Scheduler_RemoveCallback
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Runtime_SetIrqHandler
	ldr r1, .L_0802275c
	ldr r0, .L_08022760
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08022706
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	strh r2, [r1]
	ldr r2, .L_08022764
	adds r3, #4
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_08022706:
	strh r4, [r0]
	mov r0, r9
	bl Runtime_BumpFree
	ldr r6, .L_08022748
	ldr r5, [sp, #48]
	movs r3, #0
	adds r2, r5, r6
	strb r3, [r2]
	movs r0, #1
	bl WaitFrames
	mov r0, r10
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_08022734:
	.4byte gLinkCountdownWork
.L_08022738:
	.4byte gKeysRepeat
.L_0802273c:
	.4byte 0x00000c4a
.L_08022740:
	.4byte 0x00000c49
.L_08022744:
	.4byte 0x00001741
.L_08022748:
	.4byte 0x00000ea6
.L_0802274c:
	.4byte 0x0000f07f
.L_08022750:
	.4byte 0x0000044a
.L_08022754:
	.4byte 0x00000ea3
.L_08022758:
	.4byte Graphics_SetBg1Priority3
.L_0802275c:
	.4byte gIoWriteQueue
.L_08022760:
	.4byte 0x04000208
.L_08022764:
	.4byte 0x00001541
