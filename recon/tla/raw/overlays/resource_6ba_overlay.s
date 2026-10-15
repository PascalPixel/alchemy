.syntax unified
	.thumb
	.section .text.x02008054,"ax",%progbits
	.balign 4
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	mov r8, r1
	movs r0, #206
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Func_02000d00
	movs r7, #0
	mov r10, r0
	cmp r7, r8
	bge .L_02008114
.L_02008074:
	movs r0, #1
	movs r1, #1
	bl Func_02000d08
	movs r0, #5
	movs r1, #2
	bl Func_02000d08
	movs r0, #241
	lsls r0, r0, #9
	adds r0, #64
	movs r1, #5
	bl Func_02000d08
	bl UiWork_FinalizePendingCore
	movs r2, #0
	movs r3, #34
	adds r0, r6, #0
	movs r1, #5
	bl UiText_OpenMessageWindow
	ldr r3, .L_02008128
	movs r2, #3
	ldr r3, [r3]
	movs r5, #0
	ands r3, r2
	cmp r3, #0
	bne .L_020080d6
.L_020080ae:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_020080d6
	ldr r3, .L_02008128
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020080ae
	b .L_020080d6
.L_020080c8:
	movs r3, #1
	str r3, [r1]
	movs r0, #1
	str r3, [r1, #4]
	str r3, [r1, #12]
	bl WaitFrames
.L_020080d6:
	bl UiWork_IsComplete
	ldr r1, .L_02008128
	cmp r0, #0
	beq .L_020080c8
	ldr r3, [r1]
	movs r2, #2
	ands r3, r2
	movs r5, #0
	b .L_020080fe
.L_020080ea:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #9
	bgt .L_0200810c
	ldr r1, .L_02008128
	movs r2, #2
	ldr r3, [r1]
	ands r3, r2
.L_020080fe:
	cmp r3, #0
	bne .L_02008114
	ldr r3, [r1]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020080ea
.L_0200810c:
	adds r7, #1
	adds r6, #1
	cmp r7, r8
	blt .L_02008074
.L_02008114:
	bl UiWork_FinalizePendingCore
	mov r0, r10
	movs r1, #2
	bl UiWork_Finalize
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008128:
	.4byte gInput
	.section .text.x02008520,"ax",%progbits
	.global Func_02000520
	.thumb_func
Func_02000520:
	push {r5, r6, r7, lr}
	ldr r0, .L_020085f8
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r1, #100
	negs r1, r1
	movs r0, #4
	bl Owner_AdjustFirstValue
	movs r1, #100
	negs r1, r1
	movs r0, #5
	bl Owner_AdjustFirstValue
	movs r1, #33
	negs r1, r1
	movs r0, #6
	bl Owner_AdjustFirstValue
	movs r1, #50
	negs r1, r1
	movs r0, #4
	bl UnitEpAdd
	movs r1, #40
	negs r1, r1
	movs r0, #5
	bl UnitEpAdd
	movs r1, #35
	negs r1, r1
	movs r0, #6
	bl UnitEpAdd
	movs r0, #4
	bl UnitGet
	movs r2, #160
	adds r7, r0, #0
	lsls r2, r2, #1
	movs r6, #50
	adds r3, r7, r2
	movs r5, #1
	adds r6, #255
	strb r5, [r7, r6]
	movs r0, #5
	strb r5, [r3]
	bl UnitGet
	movs r2, #152
	adds r7, r0, #0
	lsls r2, r2, #1
	adds r3, r7, r2
	strb r5, [r3]
	movs r3, #2
	strb r3, [r7, r6]
	movs r0, #4
	bl UnitGet
	movs r5, #0
	adds r7, r0, #0
	movs r6, #216
	b .L_020085a4
.L_020085a0:
	adds r6, #2
	adds r5, #1
.L_020085a4:
	cmp r5, #14
	bgt .L_020085c2
	ldrh r0, [r6, r7]
	bl Func_02000d48
	ldrh r3, [r6, r7]
	cmp r3, #0
	beq .L_020085c2
	ldrb r3, [r0, #12]
	cmp r3, #2
	bne .L_020085a0
	movs r0, #4
	adds r1, r5, #0
	bl Inventory_Break
.L_020085c2:
	movs r0, #5
	bl UnitGet
	movs r5, #0
	adds r7, r0, #0
	movs r6, #216
	b .L_020085d4
.L_020085d0:
	adds r6, #2
	adds r5, #1
.L_020085d4:
	cmp r5, #14
	bgt .L_020085f2
	ldrh r0, [r6, r7]
	bl Func_02000d48
	ldrh r3, [r6, r7]
	cmp r3, #0
	beq .L_020085f2
	ldrb r3, [r0, #12]
	cmp r3, #2
	bne .L_020085d0
	movs r0, #5
	adds r1, r5, #0
	bl Inventory_Break
.L_020085f2:
	bl Func_02000dc0
	pop {r5, r6, r7, pc}
.L_020085f8:
	.4byte MsgTheBodyIs
	.section .text.x02008688,"ax",%progbits
	.balign 4
	.global Func_02000688
	.thumb_func
Func_02000688:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r5, .L_020086fc
	subs r2, #172
	str r2, [r3]
	movs r3, #139
	lsls r3, r3, #2
	adds r2, r5, r3
	movs r3, #2
	strb r3, [r2]
	movs r0, #16
	movs r1, #5
	bl Object_SetModeById
	movs r0, #17
	movs r1, #5
	bl Object_SetModeById
	movs r0, #18
	movs r1, #5
	bl Object_SetModeById
	movs r0, #19
	movs r1, #5
	bl Object_SetModeById
	movs r0, #30
	movs r1, #6
	bl Object_SetModeById
	movs r0, #31
	movs r1, #6
	bl Object_SetModeById
	movs r0, #32
	movs r1, #6
	bl Object_SetModeById
	movs r0, #33
	movs r1, #6
	bl Object_SetModeById
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_020086f8
	bl Func_02000db0
.L_020086f8:
	movs r0, #0
	pop {r5, pc}
.L_020086fc:
	.4byte gPartyState
	.section .text.x02008704,"ax",%progbits
	.balign 4
	.global Func_02000704
	.thumb_func
Func_02000704:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #32
	mov r8, r0
	mov r0, sp
	bl Party_ListActiveOwners
	movs r6, #0
	adds r7, r0, #0
	cmp r6, r7
	bge .L_0200873e
.L_0200871c:
	lsls r3, r6, #1
	mov r2, sp
	ldrh r5, [r2, r3]
	adds r6, #1
	adds r0, r5, #0
	bl UnitGet
	ldrb r1, [r0, #15]
	adds r0, r5, #0
	add r1, r8
	bl Party_AdvanceOwnerCountToTarget
	adds r0, r5, #0
	bl UnitCalc
	cmp r6, r7
	blt .L_0200871c
.L_0200873e:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008748,"ax",%progbits
	.global Func_02000748
	.thumb_func
Func_02000748:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008838
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
	ldr r5, .L_0200883c
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
.L_0200879c:
	cmp r7, #0
	beq .L_020087d2
	adds r0, r6, #0
	bl RenderOutput_RedrawSavedRect
	mov r0, r8
	adds r1, r6, #0
	movs r2, #0
	movs r3, #48
	bl UiText_DrawStringAtOffset
	ldr r0, .L_02008840
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
.L_020087d2:
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_02008844
	movs r2, #8
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_020087ee
	ldr r3, [r1, #4]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	beq .L_020087fc
.L_020087ee:
	movs r0, #5
	bl Func_02000704
	movs r0, #93
	bl Func_02000e78
	movs r7, #1
.L_020087fc:
	ldr r5, .L_02008844
	movs r2, #1
	ldr r3, [r5, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_02008816
	movs r0, #1
	bl Func_02000704
	movs r0, #91
	bl Func_02000e78
	movs r7, #1
.L_02008816:
	ldr r3, [r5, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0200879c
	movs r0, #113
	bl Func_02000e78
	adds r0, r6, #0
	movs r1, #2
	bl UiWork_Finalize
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008838:
	.4byte gPartyState
.L_0200883c:
	.4byte MsgRaiseEveryoneS
.L_02008840:
	.4byte Data_02000e80
.L_02008844:
	.4byte gInput
	.section .text.x02008848,"ax",%progbits
	.global Func_02000848
	.thumb_func
Func_02000848:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r2, #0
	sub sp, #4
	movs r5, #2
	mov r8, r2
	movs r1, #0
	movs r2, #30
	movs r3, #7
	movs r0, #0
	str r5, [sp, #0]
	bl UiWindow_Create
	movs r1, #8
	adds r7, r0, #0
	movs r2, #13
	movs r3, #10
	movs r0, #0
	str r5, [sp, #0]
	bl UiWindow_Create
	movs r3, #128
	movs r2, #128
	movs r6, #1
	lsls r3, r3, #19
	lsls r2, r2, #24
	mov r9, r0
	mov r10, r6
	adds r3, #212
	ldr r0, .L_02008a88
	ldr r1, .L_02008a8c
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #24
	ldr r0, .L_02008a90
	adds r1, #28
	adds r2, #1
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #112
	bl Func_02000e78
	movs r0, #1
	bl WaitFrames
.L_020088ac:
	mov r3, r10
	cmp r3, #0
	beq .L_02008924
	movs r2, #0
	adds r0, r7, #0
	mov r10, r2
	bl RenderOutput_PrepareForRedraw
	mov r0, r9
	bl RenderOutput_PrepareForRedraw
	ldr r0, .L_02008a94
	adds r1, r7, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringInWindow
	mov r3, r10
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #0
	adds r2, r7, #0
	movs r3, #80
	bl UiText_DrawNumberAtOffset
	bl PartyInventory_HasSpace
	cmp r0, #0
	beq .L_02008918
	ldr r0, .L_02008a98
	adds r1, r7, #0
	movs r2, #0
	movs r3, #32
	bl UiText_DrawStringInWindow
	ldr r0, .L_02008a9c
	adds r1, r7, #0
	adds r0, r6, r0
	movs r2, #120
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r0, .L_02008aa0
	adds r1, r7, #0
	adds r0, r6, r0
	movs r2, #0
	movs r3, #16
	bl UiText_DrawResource
	mov r0, r9
	adds r1, r6, #0
	bl ItemMenu_DrawItemDetails
	b .L_02008924
.L_02008918:
	ldr r0, .L_02008aa4
	adds r1, r7, #0
	movs r2, #0
	movs r3, #32
	bl UiText_DrawStringInWindow
.L_02008924:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_02008aa8
	movs r2, #1
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_0200894a
	adds r0, r6, #0
	bl PartyInventory_Add
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_02008956
	movs r0, #175
	bl Func_02000e78
.L_0200894a:
	ldr r5, .L_02008aa8
	movs r2, #2
	ldr r3, [r5, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_0200895e
.L_02008956:
	movs r0, #113
	bl Func_02000e78
	b .L_02008a6c
.L_0200895e:
	ldr r3, [r5, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_02008976
	movs r3, #1
	movs r0, #111
	mov r8, r3
	adds r6, #1
	mov r10, r3
	bl Func_02000e78
.L_02008976:
	ldr r3, [r5, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_02008990
	movs r2, #255
	movs r3, #1
	movs r0, #111
	mov r8, r2
	subs r6, #1
	mov r10, r3
	bl Func_02000e78
.L_02008990:
	ldr r3, [r5, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_020089a8
	movs r2, #1
	movs r0, #111
	mov r8, r2
	adds r6, #10
	mov r10, r2
	bl Func_02000e78
.L_020089a8:
	ldr r3, [r5, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_020089c2
	movs r3, #255
	movs r2, #1
	movs r0, #111
	mov r8, r3
	subs r6, #10
	mov r10, r2
	bl Func_02000e78
.L_020089c2:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020089dc
	movs r3, #1
	movs r0, #111
	mov r8, r3
	adds r6, #30
	mov r10, r3
	bl Func_02000e78
.L_020089dc:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_020089f8
	movs r2, #255
	movs r3, #1
	movs r0, #111
	mov r8, r2
	subs r6, #30
	mov r10, r3
	bl Func_02000e78
.L_020089f8:
	mov r2, r8
	lsls r5, r2, #24
	movs r2, #1
	asrs r3, r5, #24
	negs r2, r2
	cmp r3, r2
	bne .L_02008a32
	movs r3, #250
	lsls r3, r3, #1
	movs r1, #250
	adds r0, r6, r3
	b .L_02008a18
.L_02008a10:
	movs r2, #244
	adds r2, #255
	movs r1, #250
	adds r0, r6, r2
.L_02008a18:
	lsls r1, r1, #1
	bl __modsi3
	adds r6, r0, #0
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r6
	bl Func_02000d48
	ldrh r3, [r0, #6]
	cmp r3, #0
	beq .L_02008a10
.L_02008a32:
	movs r3, #128
	lsls r3, r3, #17
	cmp r5, r3
	bne .L_02008a66
	movs r2, #250
	lsls r2, r2, #1
	movs r1, #250
	adds r0, r6, r2
	b .L_02008a4c
.L_02008a44:
	movs r3, #246
	adds r3, #255
	movs r1, #250
	adds r0, r6, r3
.L_02008a4c:
	lsls r1, r1, #1
	bl __modsi3
	adds r6, r0, #0
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r6
	bl Func_02000d48
	ldrh r3, [r0, #6]
	cmp r3, #0
	beq .L_02008a44
.L_02008a66:
	movs r2, #0
	mov r8, r2
	b .L_020088ac
.L_02008a6c:
	adds r0, r7, #0
	movs r1, #2
	bl UiWork_Finalize
	mov r0, r9
	movs r1, #2
	bl UiWork_Finalize
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008a88:
	.4byte 0x05000200
.L_02008a8c:
	.4byte 0x050001c0
.L_02008a90:
	.4byte 0x050001e8
.L_02008a94:
	.4byte Data_02000e84
.L_02008a98:
	.4byte Data_02000e90
.L_02008a9c:
	.4byte 0x0000025f
.L_02008aa0:
	.4byte 0x00000092
.L_02008aa4:
	.4byte Data_02000ea8
.L_02008aa8:
	.4byte gInput
	.section .text.x02008ba0,"ax",%progbits
	.balign 4
	.global Func_02000ba0
	.thumb_func
Func_02000ba0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	adds r7, r1, #0
	mov r8, r2
	mov r10, r3
	cmp r6, #1
	bne .L_02008bd6
	movs r0, #177
	lsls r3, r2, #16
	lsls r1, r7, #16
	lsls r0, r0, #1
	movs r2, #0
	bl Func_02000c88
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008bd6
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r2, r5, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_02008bd6:
	cmp r6, #2
	bne .L_02008c52
	mov r2, r8
	lsls r3, r2, #16
	lsls r1, r7, #16
	movs r0, #252
	movs r2, #0
	bl Func_02000c88
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008c52
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_02008c04:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r5, #24]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #30
	adds r2, r2, r3
	ldrh r3, [r5, #6]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r5, #6]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	str r2, [r5, #24]
	str r2, [r5, #28]
	cmp r2, r3
	ble .L_02008c04
	adds r3, #1
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldr r3, .L_02008c5c
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	strh r3, [r5, #6]
	mov r0, r10
	ldr r1, [sp, #24]
	bl Func_02000e20
.L_02008c52:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008c5c:
	.4byte gPartyState
