.syntax unified
	.thumb
	.section .text.x0200817c,"ax",%progbits
	.global Func_0200017c
	.thumb_func
Func_0200017c:
	push {r5, r6, r7, lr}
	sub sp, #32
	adds r7, r0, #0
	mov r0, sp
	bl Party_ListActiveOwners
	movs r5, #0
	adds r6, r0, #0
	cmp r5, r6
	bge .L_020081a2
.L_02008190:
	lsls r2, r5, #1
	mov r3, sp
	ldrh r0, [r3, r2]
	adds r1, r7, #0
	adds r5, #1
	bl Func_02000160
	cmp r5, r6
	blt .L_02008190
.L_020081a2:
	add sp, #32
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020081a8,"ax",%progbits
	.global Func_020001a8
	.thumb_func
Func_020001a8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_020082bc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #4
	bl UnitGet
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #30
	movs r3, #9
	mov r8, r0
	movs r0, #0
	bl UiWindow_Create
	ldr r5, .L_020082c0
	adds r6, r0, #0
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawResource
	adds r0, r5, #1
	adds r1, r6, #0
	movs r2, #0
	movs r3, #16
	adds r5, #2
	bl UiText_DrawResource
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #0
	movs r3, #32
	movs r7, #1
	bl UiText_DrawResource
.L_020081fc:
	cmp r7, #0
	beq .L_02008232
	adds r0, r6, #0
	bl RenderOutput_RedrawSavedRect
	mov r0, r8
	adds r1, r6, #0
	movs r2, #0
	movs r3, #48
	bl UiText_DrawStringAtOffset
	ldr r0, .L_020082c4
	adds r1, r6, #0
	movs r2, #48
	movs r3, #48
	bl UiText_DrawStringInWindow
	mov r3, r8
	ldrb r0, [r3, #15]
	movs r3, #48
	str r3, [sp, #0]
	movs r1, #0
	adds r2, r6, #0
	movs r3, #72
	movs r7, #0
	bl UiText_DrawNumberInWindow
.L_02008232:
	ldr r1, .L_020082c8
	movs r2, #8
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_02008248
	ldr r3, [r1, #4]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	beq .L_02008256
.L_02008248:
	movs r0, #5
	bl Func_0200017c
	movs r0, #93
	bl Func_0200073c
	movs r7, #1
.L_02008256:
	ldr r5, .L_020082c8
	movs r2, #1
	ldr r3, [r5, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_02008270
	movs r0, #1
	bl Func_0200017c
	movs r0, #91
	bl Func_0200073c
	movs r7, #1
.L_02008270:
	ldr r3, [r5, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_020082b4
	movs r0, #113
	bl Func_0200073c
	adds r0, r6, #0
	bl RenderOutput_RedrawSavedRect
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	adds r0, r6, #0
	bl UiWork_Finalize
	movs r0, #0
	bl UnitCalc
	movs r0, #1
	bl UnitCalc
	movs r0, #3
	bl UnitCalc
	movs r0, #2
	bl UnitCalc
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020082b4:
	movs r0, #1
	bl WaitFrames
	b .L_020081fc
.L_020082bc:
	.4byte gPartyState
.L_020082c0:
	.4byte MsgRaiseEveryoneS
.L_020082c4:
	.4byte Data_02000744
.L_020082c8:
	.4byte gInput
	.section .text.x020082cc,"ax",%progbits
	.global Func_020002cc
	.thumb_func
Func_020002cc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #0
	movs r0, #112
	sub sp, #4
	mov r10, r2
	bl Func_0200073c
	movs r5, #2
	movs r1, #0
	movs r2, #30
	movs r3, #7
	movs r0, #0
	str r5, [sp, #0]
	bl UiWindow_Create
	.ifdef TLA_EDITION_DE
	movs r1, #9
	.else
	.ifdef TLA_EDITION_ES
	movs r1, #9
	.else
	.ifdef TLA_EDITION_FR
	movs r1, #9
	.else
	.ifdef TLA_EDITION_IT
	movs r1, #9
	.else
	movs r1, #8
	.endif
	.endif
	.endif
	.endif
	adds r7, r0, #0
	.ifdef TLA_EDITION_DE
	movs r2, #18
	.else
	.ifdef TLA_EDITION_ES
	movs r2, #18
	.else
	.ifdef TLA_EDITION_FR
	movs r2, #18
	.else
	.ifdef TLA_EDITION_IT
	movs r2, #18
	.else
	movs r2, #13
	.endif
	.endif
	.endif
	.endif
	movs r3, #10
	movs r0, #0
	str r5, [sp, #0]
	bl UiWindow_Create
	movs r3, #128
	movs r2, #128
	movs r6, #1
	lsls r3, r3, #19
	lsls r2, r2, #24
	mov r11, r0
	mov r9, r6
	adds r3, #212
	ldr r0, .L_0200854c
	ldr r1, .L_02008550
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #24
	ldr r0, .L_02008554
	adds r1, #28
	adds r2, #1
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	bl WaitFrames
.L_02008334:
	mov r3, r9
	cmp r3, #0
	beq .L_020083d2
	movs r3, #128
	lsls r3, r3, #2
	movs r1, #128
	movs r2, #0
	adds r0, r6, r3
	lsls r1, r1, #2
	mov r9, r2
	bl __modsi3
	adds r6, r0, #0
	adds r0, r7, #0
	bl RenderOutput_RedrawSavedRect
	adds r0, r7, #0
	bl RenderOutput_ClearList
	ldr r0, .L_02008558
	adds r1, r7, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringInWindow
	mov r2, r9
	str r2, [sp, #0]
	adds r0, r6, #0
	movs r1, #0
	adds r2, r7, #0
	movs r3, #80
	bl UiText_DrawNumberAtOffset
	bl PartyInventory_HasSpace
	cmp r0, #0
	beq .L_020083c6
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	adds r1, r7, #0
	movs r2, #0
	movs r3, #32
	ands r5, r6
	ldr r0, .L_0200855c
	bl UiText_DrawStringInWindow
	adds r0, r5, #0
	bl Func_020006bc
	ldr r0, .L_02008560
	adds r1, r7, #0
	adds r0, r5, r0
	movs r2, #120
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r3, .L_02008564
	adds r1, r7, #0
	adds r5, r5, r3
	adds r0, r5, #0
	movs r2, #0
	movs r3, #16
	bl UiText_DrawResource
	mov r0, r11
	bl RenderOutput_RedrawSavedRect
	mov r0, r11
	adds r1, r6, #0
	bl ItemMenu_DrawItemDetails
	b .L_020083d2
.L_020083c6:
	ldr r0, .L_02008568
	adds r1, r7, #0
	movs r2, #0
	movs r3, #32
	bl UiText_DrawStringInWindow
.L_020083d2:
	ldr r3, .L_0200856c
	movs r2, #1
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_020083f2
	adds r0, r6, #0
	bl PartyInventory_Add
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_020083fe
	movs r0, #175
	bl Func_0200073c
.L_020083f2:
	ldr r5, .L_0200856c
	movs r2, #2
	ldr r3, [r5, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_02008406
.L_020083fe:
	movs r0, #113
	bl Func_0200073c
	b .L_02008520
.L_02008406:
	ldr r3, [r5, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_02008420
	movs r2, #255
	movs r3, #1
	movs r0, #111
	mov r10, r2
	subs r6, #1
	mov r9, r3
	bl Func_0200073c
.L_02008420:
	ldr r3, [r5, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_02008438
	movs r2, #1
	movs r0, #111
	mov r10, r2
	adds r6, #1
	mov r9, r2
	bl Func_0200073c
.L_02008438:
	ldr r3, [r5, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_02008450
	movs r3, #1
	movs r0, #111
	mov r10, r3
	adds r6, #10
	mov r9, r3
	bl Func_0200073c
.L_02008450:
	ldr r3, [r5, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0200846a
	movs r2, #255
	movs r3, #1
	movs r0, #111
	mov r10, r2
	subs r6, #10
	mov r9, r3
	bl Func_0200073c
.L_0200846a:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02008484
	movs r2, #1
	movs r0, #111
	mov r10, r2
	adds r6, #30
	mov r9, r2
	bl Func_0200073c
.L_02008484:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	mov r8, r2
	cmp r3, #0
	beq .L_020084a2
	movs r3, #255
	movs r2, #1
	movs r0, #111
	mov r10, r3
	subs r6, #30
	mov r9, r2
	bl Func_0200073c
.L_020084a2:
	mov r3, r10
	lsls r5, r3, #24
	movs r2, #1
	asrs r3, r5, #24
	negs r2, r2
	cmp r3, r2
	bne .L_020084de
	movs r3, #128
	lsls r3, r3, #2
	adds r0, r6, r3
	mov r1, r8
	b .L_020084c6
.L_020084ba:
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	movs r1, #128
	adds r0, r6, r2
	lsls r1, r1, #2
.L_020084c6:
	bl __modsi3
	adds r6, r0, #0
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r6
	bl Func_020006bc
	ldrh r3, [r0, #6]
	cmp r3, #0
	beq .L_020084ba
.L_020084de:
	movs r3, #128
	lsls r3, r3, #17
	cmp r5, r3
	bne .L_02008514
	movs r2, #128
	lsls r2, r2, #2
	movs r1, #128
	adds r0, r6, r2
	b .L_020084fa
.L_020084f0:
	movs r3, #129
	lsls r3, r3, #1
	adds r3, #255
	movs r1, #128
	adds r0, r6, r3
.L_020084fa:
	lsls r1, r1, #2
	bl __modsi3
	adds r6, r0, #0
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r6
	bl Func_020006bc
	ldrh r3, [r0, #6]
	cmp r3, #0
	beq .L_020084f0
.L_02008514:
	movs r2, #0
	movs r0, #1
	mov r10, r2
	bl WaitFrames
	b .L_02008334
.L_02008520:
	adds r0, r7, #0
	bl RenderOutput_RedrawSavedRect
	movs r0, #1
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #1
	bl UiWork_Finalize
	mov r0, r11
	movs r1, #1
	bl UiWork_Finalize
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200854c:
	.4byte 0x05000200
.L_02008550:
	.4byte 0x050001c0
.L_02008554:
	.4byte 0x050001e8
.L_02008558:
	.4byte Data_02000748
.L_0200855c:
	.4byte Data_02000754
.L_02008560:
	.4byte 0x0000025f
.L_02008564:
	.4byte 0x00000092
.L_02008568:
	.4byte Data_0200076c
.L_0200856c:
	.4byte gInput
