.syntax unified
	.thumb
	.global Func_08049154
	.thumb_func
Func_08049154:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #372
	str r2, [sp, #84]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #60]
	movs r1, #1
	str r0, [sp, #72]
	negs r1, r1
	movs r0, #128
	str r1, [sp, #68]
	mov r9, r1
	bl Resource_LoadIntoFreeSlot
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #42
	str r0, [sp, #64]
	str r3, [sp, #0]
	movs r1, #4
	movs r2, #30
	movs r3, #4
	movs r0, #0
	bl UiWindow_Create
	movs r6, #6
	str r0, [sp, #60]
	movs r1, #8
	movs r2, #10
	movs r3, #3
	movs r0, #20
	str r6, [sp, #0]
	bl UiWindow_Create
	movs r2, #0
	str r0, [sp, #56]
	str r2, [sp, #52]
	adds r5, #228
	ldr r3, [r5]
	ldr r4, [r3, #52]
	ldr r0, [r3, #48]
	ldr r3, [r3, #56]
	mov r11, r4
	mov r10, r0
	str r3, [sp, #48]
	str r6, [sp, #0]
	movs r1, #11
	movs r0, #13
	movs r2, #17
	movs r3, #9
	bl UiWindow_Create
	movs r1, #156
	lsls r1, r1, #1
	add r1, sp
	str r0, [sp, #76]
	str r1, [sp, #28]
	ldr r7, .L_08049224
	movs r6, #128
	movs r4, #0
	adds r5, r1, #0
	lsls r6, r6, #23
.L_080491dc:
	movs r3, #0
	lsls r0, r4, #1
	str r6, [r5, #4]
	str r3, [r5, #8]
	ldr r1, [sp, #76]
	adds r4, #1
	movs r3, #12
	ldrsh r2, [r1, r3]
	ldr r3, .L_08049220
	ldrh r1, [r5, #6]
	lsls r2, r2, #3
	adds r2, #8
	ands r2, r3
	adds r3, r7, #0
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #6]
	ldr r1, [sp, #76]
	movs r2, #14
	ldrsh r3, [r1, r2]
	adds r0, r0, r3
	lsls r0, r0, #3
	adds r0, #4
	strb r0, [r5, #4]
	adds r5, #12
	cmp r4, #3
	ble .L_080491dc
	ldr r2, .L_08049228
	ldr r7, [sp, #28]
	movs r5, #8
	add r6, sp, #96
	mov r8, r2
	movs r4, #3
	b .L_0804922c
.L_08049220:
	.4byte 0x000001ff
.L_08049224:
	.4byte 0xfffffe00
.L_08049228:
	.4byte 0xfffffc00
.L_0804922c:
	movs r0, #128
	str r4, [sp, #4]
	bl Resource_LoadIntoFreeSlot
	movs r1, #1
	negs r1, r1
	stmia r6!, {r0}
	bl Resource_GetBuffer
	ldr r3, .L_08049258
	ldr r4, [sp, #4]
	ands r0, r3
	ldrh r3, [r5, r7]
	mov r1, r8
	ands r3, r1
	orrs r3, r0
	subs r4, #1
	strh r3, [r5, r7]
	adds r5, #12
	cmp r4, #0
	bge .L_0804922c
	b .L_0804925c
.L_08049258:
	.4byte 0x000003ff
.L_0804925c:
	movs r2, #138
	lsls r2, r2, #1
	add r2, sp
	mov r8, r2
	mov r0, r8
	bl Func_080ad180
	str r0, [sp, #80]
	movs r7, #0
	adds r3, r0, #0
	subs r3, #1
	str r3, [sp, #20]
	cmp r3, #0
	blt .L_080492c0
	mov r4, sp
	adds r4, #240
	str r4, [sp, #32]
	adds r5, r3, #0
	add r5, r8
.L_08049282:
	ldrb r6, [r5]
	adds r0, r6, #0
	bl SummonDefinition_Get
	ldr r1, [sp, #84]
	adds r0, #4
	ldrb r2, [r0]
	ldrb r3, [r1]
	movs r4, #0
	cmp r2, r3
	bhi .L_080492aa
.L_08049298:
	adds r4, #1
	cmp r4, #3
	bgt .L_080492aa
	adds r0, #1
	adds r1, #1
	ldrb r2, [r0]
	ldrb r3, [r1]
	cmp r2, r3
	bls .L_08049298
.L_080492aa:
	cmp r4, #4
	bne .L_080492b8
	ldr r2, [sp, #32]
	movs r3, #32
	strb r6, [r2, r7]
	strb r3, [r5]
	adds r7, #1
.L_080492b8:
	subs r5, #1
	cmp r5, r8
	bge .L_08049282
	b .L_080492c6
.L_080492c0:
	mov r3, sp
	adds r3, #240
	str r3, [sp, #32]
.L_080492c6:
	ldr r4, [sp, #80]
	cmp r4, #0
	ble .L_080492e8
	ldr r2, [sp, #32]
	mov r0, r8
	adds r1, r7, r2
	adds r2, r4, #0
.L_080492d4:
	ldrb r3, [r0]
	adds r0, #1
	cmp r3, #32
	beq .L_080492e2
	strb r3, [r1]
	adds r7, #1
	adds r1, #1
.L_080492e2:
	subs r2, #1
	cmp r2, #0
	bne .L_080492d4
.L_080492e8:
	ldr r4, [sp, #32]
	movs r3, #32
	strb r3, [r4, r7]
	ldr r2, [sp, #64]
	movs r0, #180
	lsls r0, r0, #1
	mov r1, r10
	add r0, sp
	lsls r1, r1, #1
	lsls r2, r2, #16
	str r0, [sp, #24]
	str r1, [sp, #16]
	str r2, [sp, #12]
.L_08049302:
	cmp r11, r9
	bne .L_0804930e
	ldr r3, [sp, #68]
	cmp r10, r3
	bne .L_0804930e
	b .L_080495ce
.L_0804930e:
	ldr r0, [sp, #72]
	movs r4, #1
	strb r4, [r0, #6]
	ldr r2, [sp, #76]
	ldr r4, [sp, #68]
	movs r1, #12
	ldrsh r0, [r2, r1]
	movs r3, #14
	ldrsh r1, [r2, r3]
	ldrh r2, [r2, #8]
	lsls r3, r4, #1
	adds r1, r1, r3
	movs r3, #15
	str r3, [sp, #0]
	subs r2, #2
	adds r1, #1
	movs r3, #1
	adds r0, #1
	bl Func_08046134
	bl Ui_FillVramBlockPattern
	ldr r1, [sp, #32]
	mov r3, r11
	add r3, r10
	ldrb r0, [r1, r3]
	bl SummonDefinition_Get
	adds r6, r0, #0
	ldrh r0, [r6]
	ldr r3, .L_080493c8
	add r5, sp, #112
	adds r0, r0, r3
	adds r1, r5, #0
	movs r2, #52
	bl UiText_CopyMessageString
	movs r2, #0
	ldr r1, [sp, #60]
	movs r3, #4
	adds r0, r5, #0
	bl UiText_RenderWideStringAtOffset
	movs r3, #0
	str r3, [sp, #52]
	mov r2, r10
	str r2, [sp, #68]
	movs r1, #1
	movs r2, #0
	adds r6, #4
.L_08049372:
	ldrb r3, [r6]
	adds r6, #1
	cmp r3, #0
	beq .L_08049384
	ldr r4, [sp, #52]
	adds r3, r1, #0
	lsls r3, r2
	orrs r4, r3
	str r4, [sp, #52]
.L_08049384:
	adds r2, #1
	cmp r2, #3
	ble .L_08049372
	cmp r11, r9
	bne .L_08049390
	b .L_08049546
.L_08049390:
	ldr r0, [sp, #76]
	bl RenderOutput_RedrawSavedRect
	movs r7, #0
	ldr r5, [sp, #84]
	movs r0, #0
	mov r8, r0
	movs r6, #0
.L_080493a0:
	movs r2, #160
	lsls r2, r2, #7
	adds r2, #1
	mov r3, r8
	adds r1, r7, r2
	str r3, [sp, #0]
	ldr r0, [sp, #56]
	movs r3, #0
	adds r2, r6, #0
	bl UiWindow_SetTilemapEntry
	ldrb r3, [r5]
	cmp r3, #9
	bls .L_080493cc
	movs r4, #241
	lsls r4, r4, #8
	adds r4, #150
	adds r1, r3, r4
	b .L_080493d8
	.2byte 0x0000
.L_080493c8:
	.4byte 0x00000885
.L_080493cc:
	ldrb r3, [r5]
	adds r1, r3, #0
	movs r3, #240
	adds r1, #48
	lsls r3, r3, #8
	orrs r1, r3
.L_080493d8:
	mov r0, r8
	adds r2, r6, #1
	str r0, [sp, #0]
	movs r3, #0
	ldr r0, [sp, #56]
	adds r7, #1
	bl UiWindow_SetTilemapEntry
	adds r5, #1
	adds r6, #2
	cmp r7, #3
	ble .L_080493a0
	ldr r1, [sp, #32]
	mov r2, r11
	ldrb r6, [r1, r2]
	movs r4, #0
	cmp r6, #32
	bne .L_080493fe
	b .L_08049528
.L_080493fe:
	mov r3, sp
	adds r3, #88
	str r3, [sp, #8]
.L_08049404:
	adds r0, r6, #0
	str r4, [sp, #4]
	bl SummonDefinition_Get
	str r0, [sp, #36]
	adds r1, r0, #0
	ldr r0, [sp, #84]
	adds r1, #4
	ldrb r2, [r1]
	ldrb r3, [r0]
	movs r7, #0
	ldr r4, [sp, #4]
	cmp r2, r3
	bhi .L_08049432
.L_08049420:
	adds r7, #1
	cmp r7, #3
	bgt .L_08049432
	adds r1, #1
	adds r0, #1
	ldrb r2, [r1]
	ldrb r3, [r0]
	cmp r2, r3
	bls .L_08049420
.L_08049432:
	ldr r2, [sp, #36]
	movs r3, #4
	eors r3, r7
	negs r5, r3
	orrs r5, r3
	ldr r0, .L_08049470
	ldrh r3, [r2]
	movs r1, #1
	ands r0, r3
	add r2, sp, #96
	lsls r3, r4, #2
	lsrs r5, r5, #31
	adds r2, r2, r3
	str r1, [sp, #0]
	ldr r3, [sp, #8]
	subs r5, r1, r5
	movs r1, #0
	str r4, [sp, #4]
	bl Ability_LoadGlyph
	ldr r4, [sp, #4]
	ldr r2, [sp, #28]
	lsls r3, r4, #1
	adds r1, r3, r4
	mov r8, r3
	ldr r0, [sp, #88]
	ldr r3, .L_08049474
	lsls r1, r1, #2
	adds r1, #8
	ands r0, r3
	b .L_08049478
.L_08049470:
	.4byte 0x00003fff
.L_08049474:
	.4byte 0x000003ff
.L_08049478:
	ldrh r3, [r2, r1]
	ldr r2, .L_080494b4
	ands r3, r2
	orrs r3, r0
	ldr r0, [sp, #28]
	strh r3, [r0, r1]
	cmp r5, #0
	bne .L_08049490
	movs r0, #2
	bl Func_08041f70
	ldr r4, [sp, #4]
.L_08049490:
	adds r0, r6, #0
	str r4, [sp, #4]
	bl SummonDefinition_Get
	ldr r3, .L_080494b8
	ldr r4, [sp, #4]
	ldrh r0, [r0]
	ldr r1, [sp, #76]
	adds r0, r0, r3
	movs r2, #16
	lsls r3, r4, #4
	bl UiText_DrawCharacterAtOffset
	movs r1, #0
	ldr r6, [sp, #36]
	lsls r3, r1, #1
	b .L_080494bc
	.2byte 0x0000
.L_080494b4:
	.4byte 0xfffffc00
.L_080494b8:
	.4byte 0x000005a7
.L_080494bc:
	ldr r4, [sp, #4]
	adds r5, r3, #0
	movs r7, #0
	mov r9, r1
	adds r6, #4
	adds r5, #11
.L_080494c8:
	ldrb r3, [r6]
	cmp r3, #0
	beq .L_080494fc
	movs r2, #160
	lsls r2, r2, #7
	adds r2, #1
	mov r3, r9
	adds r1, r7, r2
	str r3, [sp, #0]
	ldr r0, [sp, #76]
	adds r2, r5, #0
	mov r3, r8
	str r4, [sp, #4]
	bl UiWindow_SetTilemapEntry
	ldrb r1, [r6]
	mov r0, r9
	adds r2, r5, #1
	str r0, [sp, #0]
	adds r1, #48
	ldr r0, [sp, #76]
	mov r3, r8
	bl Func_0803c274
	ldr r4, [sp, #4]
	adds r5, #2
.L_080494fc:
	adds r7, #1
	adds r6, #1
	cmp r7, #3
	ble .L_080494c8
	movs r0, #15
	str r4, [sp, #4]
	bl Func_08041f70
	ldr r4, [sp, #4]
	add r3, sp, #92
	movs r1, #1
	strb r1, [r3, r4]
	adds r4, #1
	cmp r4, #3
	bgt .L_08049544
	ldr r0, [sp, #32]
	mov r2, r11
	adds r3, r2, r4
	ldrb r6, [r0, r3]
	cmp r6, #32
	beq .L_08049528
	b .L_08049404
.L_08049528:
	cmp r4, #3
	bgt .L_08049544
	ldr r0, .L_08049554
	add r2, sp, #372
	adds r3, r4, r2
	adds r2, r3, r0
	movs r3, #4
	movs r1, #0
	subs r4, r3, r4
.L_0804953a:
	subs r4, #1
	strb r1, [r2]
	adds r2, #1
	cmp r4, #0
	bne .L_0804953a
.L_08049544:
	mov r9, r11
.L_08049546:
	ldr r1, [sp, #80]
	cmp r1, #4
	ble .L_080495a4
	movs r4, #0
	adds r5, r1, #0
	adds r5, #3
	b .L_08049594
.L_08049554:
	.4byte 0xfffffee8
.L_08049558:
	movs r2, #243
	lsls r2, r2, #8
	adds r2, #1
	mov r3, r11
	adds r1, r4, r2
	cmp r3, #0
	bge .L_08049568
	adds r3, #3
.L_08049568:
	asrs r3, r3, #2
	cmp r4, r3
	bne .L_08049576
	movs r3, #243
	lsls r3, r3, #8
	adds r3, #11
	adds r1, r4, r3
.L_08049576:
	ldr r3, [sp, #76]
	str r4, [sp, #4]
	ldrh r2, [r3, #8]
	subs r2, r2, r0
	movs r0, #0
	adds r2, r2, r4
	str r0, [sp, #0]
	adds r0, r3, #0
	movs r3, #1
	subs r2, #2
	negs r3, r3
	bl UiWindow_SetTilemapEntry
	ldr r4, [sp, #4]
	adds r4, #1
.L_08049594:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_0804959e
	ldr r3, [sp, #80]
	adds r3, #6
.L_0804959e:
	asrs r0, r3, #2
	cmp r4, r0
	blt .L_08049558
.L_080495a4:
	ldr r2, [sp, #76]
	ldr r4, [sp, #16]
	movs r1, #12
	ldrsh r0, [r2, r1]
	movs r3, #14
	ldrsh r1, [r2, r3]
	ldrh r2, [r2, #8]
	adds r1, r1, r4
	movs r3, #14
	adds r0, #1
	adds r1, #1
	subs r2, #2
	str r3, [sp, #0]
	movs r3, #1
	bl Func_08046134
	ldr r1, [sp, #72]
	movs r0, #1
	movs r2, #0
	strb r0, [r1, #3]
	strb r2, [r1, #6]
.L_080495ce:
	ldr r6, [sp, #28]
	movs r4, #0
	add r5, sp, #92
.L_080495d4:
	ldrb r3, [r5]
	adds r5, #1
	cmp r3, #0
	beq .L_080495e8
	adds r0, r6, #0
	movs r1, #240
	str r4, [sp, #4]
	bl Runtime_PushSlotEntry
	ldr r4, [sp, #4]
.L_080495e8:
	adds r4, #1
	adds r6, #12
	cmp r4, #3
	ble .L_080495d4
	ldr r0, [sp, #76]
	ldr r2, [sp, #16]
	movs r4, #12
	ldrsh r3, [r0, r4]
	ldr r4, [sp, #24]
	lsls r3, r3, #3
	subs r3, #2
	str r3, [sp, #40]
	movs r1, #14
	ldrsh r3, [r0, r1]
	movs r0, #0
	adds r3, r2, r3
	lsls r3, r3, #3
	adds r3, #20
	str r3, [sp, #44]
	movs r3, #128
	lsls r3, r3, #23
	str r3, [r4, #4]
	str r0, [r4, #8]
	ldr r1, [sp, #12]
	movs r5, #0
	lsrs r0, r1, #16
	ldr r1, .L_08049668
	bl Resource_GetBuffer
	ldr r3, .L_08049658
	ldr r2, [sp, #24]
	ands r0, r3
	ldrh r3, [r2, #8]
	ldr r2, .L_0804965c
	ldr r1, .L_0804966c
	ldr r4, [sp, #24]
	ands r3, r2
	orrs r3, r0
	ldr r0, [r1]
	strh r3, [r4, #8]
	movs r3, #4
	ands r0, r3
	ldr r3, [sp, #40]
	lsrs r2, r0, #1
	movs r4, #255
	ldr r1, [sp, #24]
	adds r2, r3, r2
	lsls r4, r4, #8
	ldr r3, .L_08049660
	adds r4, #252
	adds r2, r2, r4
	ands r2, r3
	ldrh r3, [r1, #6]
	ldr r1, .L_08049664
	b .L_08049670
	.2byte 0x0000
.L_08049658:
	.4byte 0x000003ff
.L_0804965c:
	.4byte 0xfffffc00
.L_08049660:
	.4byte 0x000001ff
.L_08049664:
	.4byte 0xfffffe00
.L_08049668:
	.4byte Data_080597f8
.L_0804966c:
	.4byte Data_0300122c
.L_08049670:
	lsrs r0, r0, #2
	ands r3, r1
	orrs r3, r2
	ldr r2, [sp, #24]
	movs r1, #242
	strh r3, [r2, #6]
	ldr r3, [sp, #44]
	subs r0, r3, r0
	adds r0, #248
	strb r0, [r2, #4]
	ldr r0, [sp, #24]
	bl Runtime_PushSlotEntry
	ldr r4, .L_080499ac
	movs r3, #8
	ldr r6, [r4]
	ands r6, r3
.L_08049692:
	negs r3, r6
	orrs r3, r6
	lsrs r3, r3, #31
	adds r2, r3, #0
	ldr r0, [sp, #52]
	movs r3, #15
	subs r2, r3, r2
	movs r3, #1
	lsls r3, r5
	ands r3, r0
	cmp r3, #0
	bne .L_080496ac
	movs r2, #15
.L_080496ac:
	ldr r3, [sp, #56]
	movs r1, #12
	ldrsh r0, [r3, r1]
	lsls r3, r5, #1
	adds r0, r0, r3
	ldr r3, [sp, #56]
	adds r0, #1
	movs r4, #14
	ldrsh r1, [r3, r4]
	adds r5, #1
	str r2, [sp, #0]
	adds r1, #1
	movs r2, #2
	movs r3, #1
	bl Func_08046134
	cmp r5, #3
	ble .L_08049692
	ldr r4, [sp, #80]
	cmp r4, #4
	ble .L_0804978c
	movs r4, #0
	ldr r5, [sp, #80]
	adds r5, #3
	b .L_08049730
.L_080496de:
	ldr r2, .L_080499ac
	movs r0, #243
	ldr r3, [r2]
	lsls r0, r0, #8
	movs r2, #15
	adds r0, #1
	ands r3, r2
	adds r1, r4, r0
	cmp r3, #11
	bhi .L_08049708
	mov r3, r11
	cmp r3, #0
	bge .L_080496fa
	adds r3, #3
.L_080496fa:
	asrs r3, r3, #2
	cmp r4, r3
	bne .L_08049708
	movs r3, #243
	lsls r3, r3, #8
	adds r3, #11
	adds r1, r4, r3
.L_08049708:
	ldr r0, [sp, #76]
	adds r2, r5, #0
	ldrh r3, [r0, #8]
	cmp r5, #0
	bge .L_08049716
	ldr r2, [sp, #80]
	adds r2, #6
.L_08049716:
	asrs r2, r2, #2
	subs r2, r3, r2
	adds r2, r2, r4
	movs r3, #0
	str r3, [sp, #0]
	subs r2, #2
	ldr r0, [sp, #76]
	subs r3, #1
	str r4, [sp, #4]
	bl UiWindow_SetTilemapEntry
	ldr r4, [sp, #4]
	adds r4, #1
.L_08049730:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_0804973a
	ldr r3, [sp, #80]
	adds r3, #6
.L_0804973a:
	asrs r2, r3, #2
	cmp r4, r2
	blt .L_080496de
	ldr r4, [sp, #76]
	movs r5, #1
	ldrh r3, [r4, #8]
	movs r1, #243
	negs r5, r5
	subs r2, r3, r2
	movs r0, #0
	lsls r1, r1, #8
	str r0, [sp, #0]
	adds r3, r5, #0
	ldr r0, [sp, #76]
	subs r2, #3
	adds r1, #52
	bl UiWindow_SetTilemapEntry
	ldr r1, [sp, #76]
	movs r3, #0
	ldrh r2, [r1, #8]
	adds r0, r1, #0
	movs r1, #243
	lsls r1, r1, #8
	str r3, [sp, #0]
	subs r2, #2
	adds r1, #53
	adds r3, r5, #0
	bl UiWindow_SetTilemapEntry
	ldr r0, [sp, #76]
	movs r2, #2
	movs r4, #14
	ldrsh r3, [r0, r4]
	ldr r1, [sp, #72]
	subs r3, #1
	lsrs r3, r3, #2
	lsls r2, r3
	ldrb r3, [r1, #3]
	orrs r2, r3
	strb r2, [r1, #3]
.L_0804978c:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	mov r4, r10
	mov r3, r11
	str r3, [r2, #52]
	str r4, [r2, #48]
	ldr r0, [sp, #48]
	str r0, [r2, #56]
	ldr r3, .L_080499b0
	ldr r1, [r3, #4]
	ldr r0, [r3, #12]
	adds r3, r2, #0
	adds r3, #216
	ldr r3, [r3]
	cmp r3, #0
	beq .L_080497ca
	adds r2, #220
	ldr r3, [r2]
	movs r0, #0
	movs r1, #0
	cmp r3, #0
	bne .L_080497c6
	movs r3, #120
	str r3, [r2]
	movs r0, #1
	movs r1, #1
	b .L_080497ca
.L_080497c6:
	subs r3, #1
	str r3, [r2]
.L_080497ca:
	adds r3, r1, #0
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080497de
	ldr r4, [sp, #32]
	mov r3, r11
	add r3, r10
	ldrb r6, [r4, r3]
	b .L_0804995a
.L_080497de:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r3, [r3]
	ldr r3, [r3, #76]
	cmp r3, #0
	beq .L_080497f4
	movs r3, #2
	ands r3, r1
	cmp r3, #0
	beq .L_08049800
.L_080497f4:
	movs r0, #113
	movs r6, #1
	bl Audio_PlayCue
	negs r6, r6
	b .L_0804995a
.L_08049800:
	movs r3, #128
	ands r3, r0
	cmp r3, #0
	beq .L_08049832
	movs r0, #111
	bl Audio_PlayCue
	movs r0, #1
	add r10, r0
	mov r1, r10
	cmp r1, #4
	beq .L_08049822
	ldr r2, [sp, #80]
	mov r3, r11
	add r3, r10
	cmp r3, r2
	bne .L_08049826
.L_08049822:
	movs r3, #0
	mov r10, r3
.L_08049826:
	mov r0, r10
	mov r4, r10
	lsls r0, r0, #1
	str r4, [sp, #48]
	str r0, [sp, #16]
	b .L_08049952
.L_08049832:
	movs r3, #64
	ands r3, r0
	cmp r3, #0
	beq .L_0804987a
	movs r0, #111
	bl Audio_PlayCue
	movs r1, #1
	negs r1, r1
	add r10, r1
	mov r2, r10
	cmp r2, #0
	bge .L_0804986e
	ldr r3, [sp, #20]
	cmp r3, #0
	bge .L_08049856
	ldr r3, [sp, #80]
	adds r3, #2
.L_08049856:
	asrs r3, r3, #2
	lsls r3, r3, #2
	cmp r11, r3
	bne .L_0804986a
	ldr r4, [sp, #80]
	mov r0, r11
	subs r3, r4, r0
	subs r3, #1
	mov r10, r3
	b .L_0804986e
.L_0804986a:
	movs r1, #3
	mov r10, r1
.L_0804986e:
	mov r3, r10
	mov r2, r10
	lsls r3, r3, #1
	str r2, [sp, #48]
	str r3, [sp, #16]
	b .L_08049952
.L_0804987a:
	movs r3, #16
	ands r3, r0
	cmp r3, #0
	beq .L_080498d8
	movs r0, #111
	bl Audio_PlayCue
	bl Runtime_SetMainState19
	ldr r4, [sp, #80]
	mov r3, r11
	adds r3, #4
	cmp r3, r4
	blt .L_080498ac
	mov r0, r11
	cmp r0, #0
	beq .L_08049952
	ldr r2, [sp, #48]
	movs r1, #0
	mov r10, r2
	mov r3, r10
	lsls r3, r3, #1
	mov r11, r1
	str r3, [sp, #16]
	b .L_08049952
.L_080498ac:
	mov r11, r3
	ldr r4, [sp, #48]
	ldr r3, [sp, #20]
	mov r10, r4
	cmp r3, #0
	bge .L_080498bc
	ldr r3, [sp, #80]
	adds r3, #2
.L_080498bc:
	asrs r3, r3, #2
	lsls r3, r3, #2
	cmp r11, r3
	bne .L_08049934
	ldr r0, [sp, #80]
	mov r1, r11
	subs r3, r0, r1
	ldr r2, [sp, #48]
	subs r3, #1
	mov r10, r3
	cmp r10, r2
	ble .L_0804993c
	mov r10, r2
	b .L_08049934
.L_080498d8:
	movs r3, #32
	ands r3, r0
	cmp r3, #0
	beq .L_08049952
	movs r0, #111
	bl Audio_PlayCue
	bl Runtime_SetMainState19
	mov r4, r11
	cmp r4, #0
	beq .L_08049902
	ldr r1, [sp, #48]
	movs r0, #4
	mov r10, r1
	mov r2, r10
	negs r0, r0
	lsls r2, r2, #1
	add r11, r0
	str r2, [sp, #16]
	b .L_08049952
.L_08049902:
	ldr r3, [sp, #20]
	cmp r3, #0
	bge .L_0804990c
	ldr r3, [sp, #80]
	adds r3, #2
.L_0804990c:
	asrs r3, r3, #2
	lsls r3, r3, #2
	mov r11, r3
	ldr r3, [sp, #48]
	mov r4, r11
	mov r10, r3
	cmp r4, #0
	beq .L_08049944
	ldr r0, [sp, #80]
	ldr r1, [sp, #48]
	subs r3, r0, r4
	subs r3, #1
	mov r10, r3
	cmp r10, r1
	ble .L_0804994c
	mov r10, r1
	mov r2, r10
	lsls r2, r2, #1
	str r2, [sp, #16]
	b .L_08049952
.L_08049934:
	mov r3, r10
	lsls r3, r3, #1
	str r3, [sp, #16]
	b .L_08049952
.L_0804993c:
	mov r4, r10
	lsls r4, r4, #1
	str r4, [sp, #16]
	b .L_08049952
.L_08049944:
	mov r0, r10
	lsls r0, r0, #1
	str r0, [sp, #16]
	b .L_08049952
.L_0804994c:
	mov r1, r10
	lsls r1, r1, #1
	str r1, [sp, #16]
.L_08049952:
	movs r0, #1
	bl WaitFrames
	b .L_08049302
.L_0804995a:
	movs r0, #1
	bl WaitFrames
	movs r4, #3
	add r5, sp, #96
.L_08049964:
	ldmia r5!, {r0}
	str r4, [sp, #4]
	bl Resource_ResetEntry
	ldr r4, [sp, #4]
	subs r4, #1
	cmp r4, #0
	bge .L_08049964
	ldr r2, [sp, #12]
	lsrs r0, r2, #16
	bl Resource_ResetEntry
	movs r1, #1
	ldr r0, [sp, #56]
	bl UiWork_Finalize
	movs r1, #1
	ldr r0, [sp, #60]
	bl UiWork_Finalize
	movs r1, #1
	ldr r0, [sp, #76]
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	adds r0, r6, #0
	add sp, #372
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080499ac:
	.4byte Data_0300122c
.L_080499b0:
	.4byte gInput
