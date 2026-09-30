.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02000780
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, .L_02008048
	bx lr
.L_02008048:
	.4byte Data_02000828
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r3, .L_02008080
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_02008062
	ldr r0, .L_02008084
	b .L_0200807c
.L_02008062:
	cmp r3, #98
	bne .L_0200806a
	ldr r0, .L_02008088
	b .L_0200807c
.L_0200806a:
	cmp r3, #97
	bne .L_02008072
	ldr r0, .L_0200808c
	b .L_0200807c
.L_02008072:
	cmp r3, #96
	bne .L_0200807a
	ldr r0, .L_02008090
	b .L_0200807c
.L_0200807a:
	ldr r0, .L_02008094
.L_0200807c:
	pop {pc}
	.2byte 0x0000
.L_02008080:
	.4byte gPartyState
.L_02008084:
	.4byte Data_020008a4
.L_02008088:
	.4byte Data_02000a0c
.L_0200808c:
	.4byte Data_02000b74
.L_02008090:
	.4byte Data_02000cf4
.L_02008094:
	.4byte Data_0200082c
	.section .text.x02008098,"ax",%progbits
	.global Func_02000098
	.thumb_func
Func_02000098:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_020006e4
	movs r0, #0
	bl Func_0200072c
	ldr r3, .L_020080e8
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_020080ba
	ldr r0, .L_020080ec
	b .L_020080c8
.L_020080ba:
	cmp r3, #98
	bne .L_020080c2
	ldr r0, .L_020080f0
	b .L_020080c8
.L_020080c2:
	cmp r3, #97
	bne .L_020080d0
	ldr r0, .L_020080f4
.L_020080c8:
	adds r0, r5, r0
	bl Func_02000714
	b .L_020080d8
.L_020080d0:
	ldr r0, .L_020080f8
	adds r0, r5, r0
	bl Func_02000714
.L_020080d8:
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200071c
	bl Func_020006ec
	pop {r5, pc}
	.2byte 0x0000
.L_020080e8:
	.4byte gPartyState
.L_020080ec:
	.4byte 0x0000133a
.L_020080f0:
	.4byte 0x00001348
.L_020080f4:
	.4byte 0x0000135e
.L_020080f8:
	.4byte 0x00001365
	.section .text.x020080fc,"ax",%progbits
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_020006e4
	movs r0, #0
	bl Func_0200072c
	ldr r0, .L_02008134
	adds r0, r5, r0
	bl Func_02000714
	adds r0, r5, #0
	movs r1, #1
	bl Func_0200068c
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200071c
	cmp r5, #1
	bne .L_0200812e
	movs r0, #0
	movs r1, #0
	bl Func_0200071c
.L_0200812e:
	bl Func_020006ec
	pop {r5, pc}
.L_02008134:
	.4byte 0x0000135f
	.section .text.x02008138,"ax",%progbits
	.global Func_02000138
	.thumb_func
Func_02000138:
	push {lr}
	ldr r3, .L_02008154
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #94
	ble .L_0200814e
	ldr r0, .L_02008158
	b .L_02008150
.L_0200814e:
	ldr r0, .L_0200815c
.L_02008150:
	pop {pc}
	.2byte 0x0000
.L_02008154:
	.4byte gPartyState
.L_02008158:
	.4byte Data_02000f10
.L_0200815c:
	.4byte Data_02000ebc
	.section .text.x02008160,"ax",%progbits
	.global Func_02000160
	.thumb_func
Func_02000160:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	bl Owner_GetState
	ldrb r1, [r0, #15]
	adds r0, r5, #0
	adds r1, r1, r6
	bl Party_AdvanceOwnerCountToTarget
	adds r0, r5, #0
	bl Owner_RecalculateStats
	pop {r5, r6, pc}
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
	bl Owner_GetState
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
	bl Owner_RecalculateStats
	movs r0, #1
	bl Owner_RecalculateStats
	movs r0, #3
	bl Owner_RecalculateStats
	movs r0, #2
	bl Owner_RecalculateStats
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
	.4byte 0x00001151
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
	bl Engine_MathRemainder
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
	bl Func_02000734
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
	bl Engine_MathRemainder
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
	bl Engine_MathRemainder
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
	.section .text.x02008570,"ax",%progbits
	.global Func_02000570
	.thumb_func
Func_02000570:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #85
	str r3, [r2]
	subs r3, #77
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	bl Func_02000724
	pop {pc}
	.section .text.x02008590,"ax",%progbits
	.global Func_02000590
	.thumb_func
Func_02000590:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #88
	str r3, [r2]
	subs r3, #80
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	ldr r3, .L_020085d0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #89
	bgt .L_020085cc
	movs r0, #11
	bl Object_GetById
	ldr r5, .L_020085d4
	str r5, [r0, #28]
	movs r0, #11
	bl Object_GetById
	str r5, [r0, #24]
.L_020085cc:
	movs r0, #0
	pop {r5, pc}
.L_020085d0:
	.4byte gPartyState
.L_020085d4:
	.4byte 0x0001b333
	.section .text.x020085d8,"ax",%progbits
	.global Func_020005d8
	.thumb_func
Func_020005d8:
	push {lr}
	bl Func_020006e4
	movs r0, #0
	bl Func_0200072c
	movs r0, #10
	movs r1, #2
	bl Object_SetModeById
	movs r1, #4
	movs r0, #11
	bl Object_SetModeById
	movs r0, #10
	bl ObjectMotion_WaitForAnimationChange
	movs r1, #156
	movs r2, #208
	movs r0, #11
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_020006fc
	bl Func_020006ec
	pop {pc}
	.2byte 0x0000
	.section .text.x02008610,"ax",%progbits
	.global Func_02000610
	.thumb_func
Func_02000610:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #1
	adds r3, #53
	strb r2, [r3]
	bx lr
	.section .text.x02008620,"ax",%progbits
	.global Func_02000620
	.thumb_func
Func_02000620:
	movs r0, #0
	bx lr
	.section .text.x02008624,"ax",%progbits
	.global Func_02000624
	.thumb_func
Func_02000624:
	push {lr}
	bl Func_020006a4
	pop {pc}
	.section .text.x0200862c,"ax",%progbits
	.global Func_0200062c
	.thumb_func
Func_0200062c:
	push {lr}
	bl Func_020006ac
	pop {pc}
	.section .rodata.x02008744,"a",%progbits
	.global Data_02000744
Data_02000744:
	.4byte 0x0000764c
	.global Data_02000748
Data_02000748:
	.4byte 0x6d657449
	.4byte 0x3a6f4e20
	.4byte 0x00000000
	.global Data_02000754
Data_02000754:
	.4byte 0x65473a41
	.4byte 0x74492074
	.4byte 0x20206d65
	.4byte 0x65523a42
	.4byte 0x6e727574
	.4byte 0x00000000
	.global Data_0200076c
Data_0200076c:
	.4byte 0x4d455449
	.4byte 0x4c554620
	.4byte 0x2e2e2e4c
	.4byte 0x2e2e2e2e
	.4byte 0x0000002e
	.global Data_02000780
Data_02000780:
	.4byte 0xffff0000
	.4byte 0x00000050
	.4byte 0x40000048
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000050
	.4byte 0xc0000110
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0060
	.4byte 0x000000c0
	.4byte 0xc00000f0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0061
	.4byte 0x000000c0
	.4byte 0xc00000f0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0062
	.4byte 0x000000b0
	.4byte 0xc00000f0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x000000b0
	.4byte 0xc00000f0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000828
Data_02000828:
	.4byte 0x000001ff
	.global Data_0200082c
Data_0200082c:
	.4byte 0xffff0150
	.4byte 0x00000001
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00002000
	.4byte 0xffff00fc
	.4byte 0x00000001
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00002000
	.4byte 0xffff00fd
	.4byte 0x00000001
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00002000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00002000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020008a4
Data_020008a4:
	.4byte 0xffff0015
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff0012
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff0011
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff0018
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff0019
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00022000
	.4byte 0xffff001a
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff0010
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff0026
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff0024
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000a0c
Data_02000a0c:
	.4byte 0xffff001d
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff0022
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff0017
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00022000
	.4byte 0xffff003c
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff003d
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff002c
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff002d
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000b74
Data_02000b74:
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00022000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff003b
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff0040
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00022000
	.4byte 0xffff0041
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00022000
	.4byte 0xffff003f
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000cf4
Data_02000cf4:
	.4byte 0xffff00cd
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff00c6
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff007b
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff0068
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff0069
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff0081
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff0082
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00022000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00022000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00022000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00022000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00022000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000ebc
Data_02000ebc:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_020002cc
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000624
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_0200062c
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_020001a8
	.4byte 0x00008e15
	.4byte 0xffff000a
	.4byte Func_020005d8
	.4byte 0x10008e15
	.4byte 0xffff000a
	.4byte Func_02000610
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000f10
Data_02000f10:
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte Func_020000fc
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte Func_020000fc
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte Func_020000fc
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte Func_020000fc
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte Func_020000fc
	.4byte 0x00000000
	.4byte 0xffff0006
	.4byte Func_020000fc
	.4byte 0x00000000
	.4byte 0xffff0007
	.4byte Func_020000fc
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0020
	.4byte Func_02000098
	.4byte 0x00008e15
	.4byte 0xffff000a
	.4byte Func_020005d8
	.4byte 0x10008e15
	.4byte 0xffff000a
	.4byte Func_02000610
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
