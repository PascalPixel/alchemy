.syntax unified
	.thumb
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	bl Func_02001fcc
	movs r0, #0
	bl Func_020020cc
	movs r0, #158
	bl Func_02002124
	ldrh r1, [r5, #4]
	ldrh r2, [r5, #6]
	ldr r0, [r5]
	bl Func_02001fb4
	ldr r5, .L_020080c4
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r2, r2, #7
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r2, #8
	movs r1, #2
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_SnapHeadingAndOffset
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r6, #0
	bl Func_020020b4
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02001fd4
	pop {r5, r6, pc}
.L_020080c4:
	.4byte gPartyState
	.section .text.x020080c8,"ax",%progbits
	.global Func_020000c8
	.thumb_func
Func_020000c8:
	push {lr}
	adds r1, r0, #0
	ldr r0, .L_020080e4
	cmp r1, #3
	bne .L_020080d4
	adds r0, #8
.L_020080d4:
	cmp r1, #16
	bne .L_020080da
	ldr r0, .L_020080e8
.L_020080da:
	movs r2, #0
	bl Func_02000054
	pop {pc}
	.2byte 0x0000
.L_020080e4:
	.4byte Data_02002368
.L_020080e8:
	.4byte Data_02002378
	.section .text.x020080f4,"ax",%progbits
	.global Func_020000f4
	.thumb_func
Func_020000f4:
	push {r5, lr}
	ldr r3, .L_02008140
	movs r1, #6
	ldr r0, [r3]
	bl Engine_MathModulo
	cmp r0, #0
	bne .L_02008126
	ldr r3, .L_02008144
	movs r0, #14
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r1, r3, #16
.L_0200810e:
	ldr r4, .L_02008148
	lsls r3, r0, #1
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r0, #1
	strh r3, [r2]
	cmp r0, #1
	bne .L_0200810e
	ldr r3, .L_0200814c
	strh r1, [r3]
.L_02008126:
	ldr r3, .L_02008140
	movs r1, #12
	ldr r0, [r3]
	ldr r5, .L_02008150
	lsrs r0, r0, #2
	bl Engine_MathModulo
	lsls r0, r0, #1
	ldrsh r1, [r5, r0]
	ldr r3, .L_02008154
	strh r1, [r3]
	pop {r5, pc}
	.2byte 0x0000
.L_02008140:
	.4byte Data_0300122c
.L_02008144:
	.4byte 0x0500019c
.L_02008148:
	.4byte 0x05000180
.L_0200814c:
	.4byte 0x05000182
.L_02008150:
	.4byte Data_02002608
.L_02008154:
	.4byte 0x0500019e
	.section .text.x02008158,"ax",%progbits
	.global Func_02000158
	.thumb_func
Func_02000158:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #129
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	movs r1, #200
	str r2, [r3]
	lsls r1, r1, #4
	ldr r0, .L_020081cc
	bl Scheduler_AddOrUpdateCallback
	movs r1, #3
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	ldr r2, .L_020081d0
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #6
	bne .L_020081a6
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
.L_020081a6:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #106
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081c8
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02002024
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02002024
.L_020081c8:
	movs r0, #0
	pop {pc}
.L_020081cc:
	.4byte Func_020000f4
.L_020081d0:
	.4byte gPartyState
	.section .text.x020081d8,"ax",%progbits
	.global Func_020001d8
	.thumb_func
Func_020001d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #13
	bl Object_GetById
	movs r1, #5
	adds r6, r0, #0
	movs r0, #13
	bl Object_SetModeById
	movs r0, #28
	bl Battle_WaitMode0
	movs r0, #112
	bl Func_02002124
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	movs r3, #128
	lsls r3, r3, #13
	movs r7, #160
	movs r0, #128
	mov r11, r3
	lsls r7, r7, #12
	lsls r0, r0, #2
	adds r1, r1, r7
	add r2, r11
	ldr r3, [r6, #16]
	adds r0, #162
	bl Object_Spawn
	movs r2, #128
	adds r5, r0, #0
	lsls r2, r2, #10
	str r2, [r5, #48]
	str r2, [r5, #52]
	mov r8, r2
	movs r2, #0
	mov r9, r2
	adds r3, r5, #0
	mov r2, r9
	adds r3, #85
	strb r2, [r3]
	movs r3, #192
	ldr r1, [r5, #8]
	lsls r3, r3, #11
	adds r1, r1, r3
	ldr r2, [r5, #12]
	movs r3, #128
	lsls r3, r3, #12
	mov r10, r3
	add r2, r10
	ldr r3, [r5, #16]
	bl Func_02001fa4
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	movs r0, #128
	lsls r0, r0, #2
	adds r1, r1, r7
	add r2, r11
	ldr r3, [r6, #16]
	adds r0, #162
	bl Object_Spawn
	adds r6, r0, #0
	mov r2, r8
	adds r3, r6, #0
	str r2, [r6, #48]
	str r2, [r6, #52]
	adds r3, #85
	mov r2, r9
	strb r2, [r3]
	ldr r3, .L_020082b0
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	add r1, r10
	adds r2, r2, r3
	ldr r3, [r5, #16]
	bl Func_02001fa4
	adds r0, r5, #0
	bl Func_02001fac
	adds r0, r5, #0
	bl Func_02001f9c
	adds r0, r6, #0
	bl Func_02001fac
	adds r0, r6, #0
	bl Func_02001f9c
	movs r0, #30
	bl Battle_WaitMode0
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020082b0:
	.4byte 0xfffc0000
	.section .text.x020082b4,"ax",%progbits
	.global Func_020002b4
	.thumb_func
Func_020002b4:
	push {r5, r6, lr}
	ldr r3, .L_02008380
	sub sp, #12
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #1
	bne .L_020082e8
	ldr r4, .L_02008384
	ldr r1, .L_02008388
	ldr r0, [r4]
	movs r2, #15
	asrs r3, r0, #3
	ands r3, r2
	ldrb r1, [r1, r3]
	lsls r2, r1, #8
	movs r3, #16
	subs r3, r3, r1
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	adds r0, #1
	str r0, [r4]
	b .L_02008338
.L_020082e8:
	cmp r3, #2
	bne .L_02008338
	ldr r6, .L_02008384
	movs r1, #28
	ldr r0, [r6]
	asrs r0, r0, #2
	bl Engine_MathRemainder
	ldr r2, .L_0200838c
	adds r5, r0, #0
	lsls r3, r5, #1
	ldrh r2, [r2, r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	cmp r5, #0
	bne .L_0200831c
	ldr r3, .L_02008390
	movs r2, #1
	str r2, [r3]
	ldr r3, .L_02008394
	movs r0, #153
	str r5, [r3]
	bl Func_02002124
.L_0200831c:
	cmp r5, #14
	bne .L_02008332
	ldr r2, .L_02008390
	movs r3, #0
	str r3, [r2]
	ldr r2, .L_02008394
	movs r3, #1
	str r3, [r2]
	movs r0, #153
	bl Func_02002124
.L_02008332:
	ldr r3, [r6]
	adds r3, #1
	str r3, [r6]
.L_02008338:
	movs r3, #208
	mov r5, sp
	lsls r3, r3, #18
	str r3, [r5]
	movs r3, #0
	str r3, [r5, #4]
	movs r3, #224
	lsls r3, r3, #17
	str r3, [r5, #8]
	adds r0, r5, #0
	bl Func_020020d4
	ldr r3, .L_02008390
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008366
	ldr r3, [r5]
	ldr r0, .L_02008398
	str r3, [r0, #12]
	ldr r3, [r5, #8]
	str r3, [r0, #16]
	bl Func_0200210c
.L_02008366:
	ldr r3, .L_02008394
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200837c
	ldr r3, [r5]
	ldr r0, .L_0200839c
	str r3, [r0, #12]
	ldr r3, [r5, #8]
	str r3, [r0, #16]
	bl Func_0200210c
.L_0200837c:
	add sp, #12
	pop {r5, r6, pc}
.L_02008380:
	.4byte gOverlayArea + 0x308c
.L_02008384:
	.4byte gOverlayArea + 0x3098
.L_02008388:
	.4byte Data_02002fd6 + 0x1
.L_0200838c:
	.4byte Data_02002fe8
.L_02008390:
	.4byte gOverlayArea + 0x3090
.L_02008394:
	.4byte gOverlayArea + 0x3024
.L_02008398:
	.4byte gOverlayArea + 0x3060
.L_0200839c:
	.4byte gOverlayArea + 0x3030
	.section .text.x020083a0,"ax",%progbits
	.global Func_020003a0
	.thumb_func
Func_020003a0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #128
	lsls r0, r0, #5
	sub sp, #4
	bl Runtime_BumpAllocate
	mov r9, r0
	movs r2, #253
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #6
	adds r3, #80
	strh r2, [r3]
	adds r3, #2
	movs r1, #128
	lsls r1, r1, #5
	strh r1, [r3]
	ldr r0, .L_020084b4
	movs r7, #32
	mov r1, r9
	bl Func_02001f6c
	ldr r5, .L_020084b8
	bl Resource_FindFreeEntry
	movs r1, #128
	mov r2, r9
	str r0, [r5]
	lsls r1, r1, #5
	ldr r5, .L_020084bc
	bl VramBlock_LoadCached
	ldr r6, .L_020084c0
	movs r3, #192
	str r0, [r5]
	movs r1, #0
	str r0, [sp, #0]
	movs r2, #0
	adds r0, r6, #0
	lsls r3, r3, #24
	bl Func_02002104
	movs r3, #1
	strh r3, [r6, #30]
	movs r5, #13
	ldrb r3, [r6, #9]
	negs r5, r5
	adds r2, r5, #0
	ands r2, r3
	movs r3, #8
	mov r8, r3
	mov r1, r8
	orrs r2, r1
	ldrb r1, [r6, #5]
	adds r3, r5, #0
	ands r3, r1
	movs r1, #4
	mov r10, r1
	mov r1, r10
	orrs r3, r1
	orrs r3, r7
	strb r3, [r6, #5]
	movs r3, #15
	ands r2, r3
	strb r2, [r6, #9]
	mov r1, r9
	ldr r0, .L_020084c4
	mov r11, r3
	bl Func_02001f6c
	ldr r6, .L_020084c8
	bl Resource_FindFreeEntry
	movs r1, #128
	mov r2, r9
	str r0, [r6]
	lsls r1, r1, #5
	ldr r6, .L_020084cc
	bl VramBlock_LoadCached
	str r0, [r6]
	ldr r6, .L_020084d0
	movs r3, #192
	str r0, [sp, #0]
	movs r1, #0
	adds r0, r6, #0
	movs r2, #0
	lsls r3, r3, #24
	bl Func_02002104
	ldrb r2, [r6, #9]
	movs r3, #2
	strh r3, [r6, #30]
	adds r3, r5, #0
	mov r1, r8
	ands r3, r2
	ldrb r2, [r6, #5]
	orrs r3, r1
	mov r1, r11
	ands r3, r1
	strb r3, [r6, #9]
	ands r5, r2
	ldr r3, .L_020084d4
	mov r2, r10
	orrs r5, r2
	orrs r5, r7
	movs r2, #0
	strb r5, [r6, #5]
	str r2, [r3]
	ldr r3, .L_020084d8
	movs r1, #0
	str r2, [r3]
	ldr r3, .L_020084dc
	ldr r0, .L_020084e0
	str r2, [r3]
	ldr r3, .L_020084e4
	strb r1, [r3]
	movs r1, #144
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	mov r0, r9
	bl Sys_Free
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020084b4:
	.4byte Data_02002620
.L_020084b8:
	.4byte gOverlayArea + 0x3020
.L_020084bc:
	.4byte gOverlayArea + 0x3088
.L_020084c0:
	.4byte gOverlayArea + 0x3060
.L_020084c4:
	.4byte Data_02002af8
.L_020084c8:
	.4byte gOverlayArea + 0x3094
.L_020084cc:
	.4byte gOverlayArea + 0x3058
.L_020084d0:
	.4byte gOverlayArea + 0x3030
.L_020084d4:
	.4byte gOverlayArea + 0x3090
.L_020084d8:
	.4byte gOverlayArea + 0x3024
.L_020084dc:
	.4byte gOverlayArea + 0x3098
.L_020084e0:
	.4byte Func_020002b4
.L_020084e4:
	.4byte gOverlayArea + 0x308c
	.section .text.x020084e8,"ax",%progbits
	.global Func_020004e8
	.thumb_func
Func_020004e8:
	push {lr}
	ldr r0, .L_02008504
	bl Scheduler_RemoveCallbackFar
	ldr r3, .L_02008508
	ldr r0, [r3]
	bl Resource_ResetEntry
	ldr r3, .L_0200850c
	ldr r0, [r3]
	bl Resource_ResetEntry
	pop {pc}
	.2byte 0x0000
.L_02008504:
	.4byte Func_020002b4
.L_02008508:
	.4byte gOverlayArea + 0x3020
.L_0200850c:
	.4byte gOverlayArea + 0x3094
	.section .text.x02008510,"ax",%progbits
	.global Func_02000510
	.thumb_func
Func_02000510:
	push {r5, r6, r7, lr}
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #5
	adds r3, #82
	strh r2, [r3]
	ldr r2, .L_0200854c
	movs r3, #0
	strb r3, [r2]
	movs r5, #0
.L_02008526:
	movs r7, #16
	subs r3, r7, r5
	movs r6, #128
	lsls r3, r3, #8
	lsls r6, r6, #19
	orrs r3, r5
	adds r6, #82
	strh r3, [r6]
	movs r0, #4
	adds r5, #1
	bl WaitFrames
	cmp r5, #15
	ble .L_02008526
	strh r7, [r6]
	ldr r2, .L_0200854c
	movs r3, #1
	strb r3, [r2]
	pop {r5, r6, r7, pc}
.L_0200854c:
	.4byte gOverlayArea + 0x308c
	.section .text.x02008550,"ax",%progbits
	.global Func_02000550
	.thumb_func
Func_02000550:
	push {r5, r6, lr}
	movs r3, #128
	lsls r3, r3, #19
	movs r2, #16
	adds r3, #82
	strh r2, [r3]
	ldr r2, .L_02008588
	movs r3, #0
	strb r3, [r2]
	movs r5, #0
.L_02008564:
	movs r3, #16
	movs r6, #128
	lsls r2, r5, #8
	subs r3, r3, r5
	lsls r6, r6, #19
	orrs r2, r3
	adds r6, #82
	strh r2, [r6]
	movs r0, #3
	adds r5, #1
	bl WaitFrames
	cmp r5, #15
	ble .L_02008564
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r6]
	pop {r5, r6, pc}
.L_02008588:
	.4byte gOverlayArea + 0x308c
	.section .text.x0200858c,"ax",%progbits
	.global Func_0200058c
	.thumb_func
Func_0200058c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl Func_02001fcc
	movs r0, #0
	bl Func_020020cc
	ldr r0, .L_02008630
	bl Func_0200205c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #16
	ldr r1, .L_02008634
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #24
	movs r2, #0
	movs r0, #16
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r0, #16
	lsls r1, r1, #7
	bl Func_0200207c
	ldr r3, .L_02008638
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r1, [r5]
	movs r2, #0
	movs r0, #16
	bl ObjectMotion_SetAngleToward
	movs r1, #0
	movs r0, #16
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200863c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #16
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	ldr r0, [r5]
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #16
	movs r1, #24
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r0, #16
	lsls r1, r1, #7
	bl Func_0200207c
	bl Func_02001fd4
	bl .L_02009f20
.L_02008630:
	.4byte 0x00002804
.L_02008634:
	.4byte 0x00013333
.L_02008638:
	.4byte gPartyState
.L_0200863c:
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #24
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	ldr r1, [r5]
	movs r0, #16
	bl Object_LinkObjectAndSetCallback
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #106
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #107
	bl GameFlag_SetBit
	movs r0, #7
	bl Party_AddActiveOwner
	movs r0, #64
	bl Object_GetById
	movs r1, #0
	str r7, [r0, #28]
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #13
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #14
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #7
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #218
	movs r2, #132
	movs r0, #4
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #218
	movs r1, #1
	movs r2, #204
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_020020ac
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #45
	lsls r1, r1, #1
	movs r0, #13
	bl Func_0200208c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #6
	strh r3, [r2]
	adds r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #15
	bl Func_0200207c
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #8
	bl Func_0200207c
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #15
	bl Func_0200208c
	movs r0, #15
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #13
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #15
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #216
	movs r2, #222
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #13
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #212
	movs r2, #222
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #208
	movs r2, #222
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #7
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #13
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #13
	movs r1, #1
	bl Object_SetModeById
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #14
	movs r1, #1
	bl Object_SetModeById
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #1
	bl Object_SetModeById
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	bl Func_0200207c
	movs r0, #218
	movs r1, #1
	movs r2, #248
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_020020ac
	movs r0, #5
	bl Battle_WaitMode0
	movs r3, #176
	movs r0, #17
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #8
	bl Func_020020dc
	movs r1, #16
	movs r3, #192
	movs r0, #5
	negs r1, r1
	movs r2, #0
	lsls r3, r3, #8
	bl Func_020020dc
	movs r1, #32
	movs r3, #192
	lsls r3, r3, #8
	movs r0, #6
	negs r1, r1
	movs r2, #0
	bl Func_020020dc
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #0
	movs r0, #7
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #7
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #160
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #13
	lsls r1, r1, #7
	bl Func_0200207c
	movs r2, #8
	negs r2, r2
	movs r1, #0
	movs r0, #6
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	bl Func_0200207c
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #13
	lsls r1, r1, #7
	bl Func_0200207c
	movs r2, #8
	negs r2, r2
	movs r1, #0
	movs r0, #5
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	bl Func_0200207c
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	bl Func_0200207c
	movs r2, #8
	negs r2, r2
	movs r1, #0
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	bl Func_0200207c
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #17
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	mov r10, r3
	ands r3, r2
	movs r2, #8
	strb r3, [r0]
	movs r1, #0
	negs r2, r2
	movs r0, #17
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #17
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r9, r2
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200206c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #17
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02002094
	movs r0, #37
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #17
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_0200206c
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #13
	bl Func_0200207c
	movs r0, #15
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_0200206c
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #7
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200206c
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #13
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #17
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #17
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	bl Func_020001d8
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #8
	movs r0, #32
	bl Func_02002114
	movs r0, #32
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_020003a0
	ldr r5, .L_02008fe0
	ldr r6, .L_02008fe4
	movs r3, #1
	str r3, [r6]
	movs r0, #153
	str r7, [r5]
	mov r8, r3
	bl Func_02002124
	bl Func_02000510
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_0200208c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #17
	bl Func_0200208c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_0200208c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #55
	movs r0, #5
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r0, #15
	movs r1, #0
	movs r2, #10
	bl Func_0200206c
	movs r1, #6
	adds r1, #255
	movs r2, #50
	movs r0, #17
	bl Func_0200208c
	movs r0, #13
	bl Func_020001d8
	bl Func_02000550
	movs r0, #30
	bl Battle_WaitMode0
	mov r2, r8
	movs r0, #153
	str r7, [r6]
	str r2, [r5]
	bl Func_02002124
	bl Func_02000510
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #10
	bl Func_0200206c
	movs r1, #0
	movs r2, #5
	movs r0, #14
	bl Func_0200206c
	movs r0, #17
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r10
	ands r2, r3
	strb r2, [r0]
	mov r10, r2
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #17
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #17
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r9
	orrs r2, r3
	strb r2, [r0]
	movs r0, #25
	mov r9, r2
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02002094
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #0
	movs r0, #6
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_0200208c
	movs r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r2, #16
	movs r0, #17
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #17
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #17
	bl Func_0200208c
	movs r0, #17
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #5
	bl Func_0200208c
	movs r1, #0
	movs r0, #5
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #5
	movs r1, #0
	bl Func_0200206c
	movs r0, #14
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #176
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #6
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	movs r2, #5
	adds r0, #13
	bl Func_0200206c
	movs r0, #13
	bl Func_020001d8
	bl Func_02000550
	movs r0, #10
	bl Battle_WaitMode0
	ldr r3, .L_02008fe8
	ldr r2, .L_02008fec
	str r7, [r3]
	movs r3, #2
	strb r3, [r2]
	movs r0, #150
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02002094
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_0200206c
	movs r0, #15
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r2, #128
	movs r0, #15
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #222
	movs r2, #222
	lsls r2, r2, #1
	movs r0, #15
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #15
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #218
	movs r1, #1
	movs r2, #236
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_020020ac
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #15
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #0
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #13
	bl Func_0200208c
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	bl Func_020001d8
	movs r0, #153
	bl Func_02002124
	movs r5, #0
	b .L_02008ff2
	.2byte 0x0000
.L_02008fe0:
	.4byte gOverlayArea + 0x3024
.L_02008fe4:
	.4byte gOverlayArea + 0x3090
.L_02008fe8:
	.4byte gOverlayArea + 0x3098
.L_02008fec:
	.4byte gOverlayArea + 0x308c
.L_02008ff0:
	adds r5, #1
.L_02008ff2:
	cmp r5, #119
	bgt .L_0200900c
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_020093b4
	movs r1, #14
	ldr r0, [r3]
	asrs r0, r0, #2
	bl Engine_MathRemainder
	cmp r0, #0
	bne .L_02008ff0
.L_0200900c:
	bl Func_020004e8
	movs r0, #32
	bl Field_BeginPaletteTransition
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #15
	bl Motion_SetModeAndWaitAnimation
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #254
	lsls r1, r1, #7
	adds r1, #255
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #0
	movs r0, #7
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #13
	bl Func_0200208c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #7
	bl Func_0200208c
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #14
	bl Func_0200208c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #13
	bl Func_0200208c
	movs r2, #0
	movs r0, #14
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #13
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #17
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #4
	bl Func_0200207c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #17
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #17
	bl Func_0200208c
	movs r2, #5
	movs r0, #17
	movs r1, #0
	bl Func_0200206c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #17
	bl Func_0200207c
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #17
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #17
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #17
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #17
	bl Func_0200208c
	movs r2, #5
	movs r0, #17
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200208c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #15
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #2
	movs r2, #50
	adds r1, #255
	movs r0, #15
	bl Func_0200208c
	movs r0, #15
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r2, #45
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200208c
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #15
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #15
	bl Func_0200208c
	movs r1, #0
	movs r0, #15
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020093b8
	movs r0, #15
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020093da
	.2byte 0x0000
.L_020093b4:
	.4byte gOverlayArea + 0x3098
.L_020093b8:
	movs r0, #35
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #15
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
.L_020093da:
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #15
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #15
	bl Func_0200208c
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #0
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #15
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #15
	bl ObjectMotion_SetSpeedParameters
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #32
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r0, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #4
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r0, #15
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #15
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_0200208c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_0200208c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_0200208c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #17
	bl Func_0200208c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #7
	bl Func_0200208c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #15
	bl Func_0200208c
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #15
	ldr r1, .L_0200997c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #50
	movs r2, #24
	movs r0, #15
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #13
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #14
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #4
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #17
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #15
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #8
	movs r0, #15
	negs r1, r1
	movs r2, #32
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #4
	lsls r1, r1, #1
	bl Func_02002094
	movs r0, #5
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02002094
	movs r0, #37
	bl Battle_WaitMode0
	movs r0, #4
	ldr r1, .L_02009980
	ldr r2, .L_02009984
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	ldr r1, .L_02009980
	ldr r2, .L_02009984
	bl ObjectMotion_SetSpeedParameters
	movs r1, #8
	movs r0, #5
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #8
	movs r2, #16
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r2, #0
	movs r0, #4
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #5
	bl Func_0200207c
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r2, #64
	movs r1, #0
	movs r0, #15
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #15
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #40
	movs r0, #15
	bl Func_0200208c
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #15
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #16
	bl Func_0200207c
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #16
	bl Func_0200207c
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #5
	bl Object_SetModeById
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r1, #0
	movs r2, #96
	movs r0, #15
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02002024
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #16
	movs r0, #5
	movs r1, #8
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #8
	movs r2, #16
	negs r1, r1
	negs r2, r2
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #218
	movs r1, #1
	movs r2, #240
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_020020ac
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #16
	movs r1, #1
	bl Object_SetModeById
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #17
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #17
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #4
	bl Func_0200207c
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #13
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #6
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #13
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	movs r0, #13
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #14
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #24
	movs r2, #26
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #8
	movs r1, #0
	movs r0, #14
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	movs r5, #15
	b .L_02009988
	.2byte 0x0000
.L_0200997c:
	.4byte 0x00013333
.L_02009980:
	.4byte 0x00023333
.L_02009984:
	.4byte 0x00011999
.L_02009988:
	movs r0, #64
	bl Object_GetById
	ldr r3, [r0, #28]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r0, #28]
	subs r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02009988
	movs r0, #64
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #28]
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_0200208c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #17
	bl Func_0200208c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_0200208c
	movs r1, #2
	adds r1, #255
	movs r2, #60
	movs r0, #4
	bl Func_0200208c
	movs r1, #24
	movs r2, #26
	movs r0, #14
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #8
	negs r2, r2
	movs r0, #14
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #4
	bl Func_0200207c
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_0200207c
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #0
	movs r0, #7
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_0200208c
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200206c
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #8
	bl Func_0200207c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r2, #0
	movs r0, #14
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #13
	bl Func_0200207c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #4
	bl Func_0200207c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #13
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #216
	movs r2, #196
	lsls r2, r2, #1
	movs r0, #13
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	bl Func_0200207c
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200206c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200206c
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #4
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #17
	ldr r1, .L_02009f2c
	bl ObjectMotion_SetSpeedParameters
	movs r0, #17
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_02009f30
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009dbc
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #17
	bl ObjectMotion_ResetAndSetPosition
.L_02009dbc:
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02002024
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02009f2c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009dfa
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_02009dfa:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02002024
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_02009f2c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009e38
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_02009e38:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #6
	bl Func_02002024
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #32
	movs r2, #48
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_02009f2c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009ece
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02009ece:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #7
	bl Func_02002024
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #210
	movs r2, #204
	lsls r2, r2, #1
	movs r0, #14
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	bl Func_0200207c
	movs r1, #160
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02001fd4
.L_02009f20:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009f2c:
	.4byte 0x00013333
.L_02009f30:
	.4byte gPartyState
	.section .rodata.x0200a12c,"a",%progbits
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x00000120
	.4byte 0xc00001c6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x000000cc
	.4byte 0x1010b0ca
	.4byte 0xffffffff
	.4byte 0x102060cc
	.4byte 0xffffffff
	.4byte 0x103070cc
	.4byte 0xffffffff
	.4byte 0x104080cc
	.4byte 0xffffffff
	.4byte 0x105090cc
	.4byte 0xffffffff
	.4byte 0x106020cc
	.4byte 0xffffffff
	.4byte 0x107030cc
	.4byte 0xffffffff
	.4byte 0x108040cc
	.4byte 0xffffffff
	.4byte 0x109050cc
	.4byte 0xffffffff
	.4byte 0x10a0a0ca
	.4byte 0xffffffff
	.4byte 0x10b0d0cc
	.4byte 0xffffffff
	.4byte 0x10c0e0cc
	.4byte 0xffffffff
	.4byte 0x10d0b0cc
	.4byte 0xffffffff
	.4byte 0x10e0c0cc
	.4byte 0xffffffff
	.4byte 0x10f100cc
	.4byte 0xffffffff
	.4byte 0x1100f0cc
	.4byte 0xffffffff
	.4byte 0x111110ca
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x03900000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00015000
	.4byte 0xffff009c
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00013000
	.4byte 0xffff00a0
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00030000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00010000
	.4byte 0xffff00a4
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001d000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00013000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00013000
	.4byte 0xffff0022
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00018000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x02300000
	.4byte 0x00015000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0002e000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200a33c:
	.4byte 0x00340001
	.4byte 0x00020001
	.4byte 0x00020006
	.4byte 0x00010034
	.4byte 0x00060002
	.4byte 0x0002ffff
	.4byte 0x00020036
	.4byte 0x00060002
	.4byte 0x00360004
	.4byte 0x00020002
	.4byte 0xffff0006
	.global Data_02002368
Data_02002368:
	.4byte .L_0200a33c
	.4byte 0x00170005
	.4byte .L_0200a33c
	.4byte 0x0017001e
	.global Data_02002378
Data_02002378:
	.4byte .L_0200a33c
	.4byte 0x00040038
	.global gSceneEvents
gSceneEvents:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_020000c8
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_020000c8
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x0000c602
	.4byte 0xffff0010
	.4byte Func_020000c8
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000002
	.4byte 0x096a0014
	.4byte Func_0200058c
	.4byte 0x00000000
	.4byte 0x196a0008
	.4byte 0x0000286e
	.4byte 0x00000000
	.4byte 0x196a0009
	.4byte 0x0000286f
	.4byte 0x00000000
	.4byte 0x196a000a
	.4byte 0x00002870
	.4byte 0x00000000
	.4byte 0x196a000b
	.4byte 0x00002871
	.4byte 0x00000000
	.4byte 0x196a000c
	.4byte 0x00002872
	.4byte 0x00008d15
	.4byte 0x196a0008
	.4byte 0x00002874
	.4byte 0x00008d15
	.4byte 0x196a0009
	.4byte 0x00002875
	.4byte 0x00008d15
	.4byte 0x196a000a
	.4byte 0x00002876
	.4byte 0x00008d15
	.4byte 0x196a000b
	.4byte 0x00002877
	.4byte 0x00008d15
	.4byte 0x196a000c
	.4byte 0x00002878
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000027f6
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000027f7
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000027f8
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000027f9
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000027fa
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000027fc
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000027fd
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000027fe
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000027ff
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002800
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000287a
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000287b
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0000287c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000287d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000287e
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000287f
	.4byte 0x000000f3
	.4byte 0xffff00c8
	.4byte 0x00403064
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x00403065
	.4byte 0x000000f3
	.4byte 0xffff00ca
	.4byte 0x00403066
	.4byte 0x000000f3
	.4byte 0xffff00cb
	.4byte 0x00403067
	.4byte 0x000000f3
	.4byte 0xffff00cc
	.4byte 0x00403068
	.4byte 0x000000f3
	.4byte 0xffff00cd
	.4byte 0x00403069
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x0040306a
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x0040306b
	.4byte 0x000001c3
	.4byte 0xffff00d0
	.4byte 0x0040306d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002608
Data_02002608:
	.4byte 0x76c07b00
	.4byte 0x6e407280
	.4byte 0x65c06a00
	.4byte 0x65c06180
	.4byte 0x6e406a00
	.4byte 0x76c07280
	.global Data_02002620
Data_02002620:
	.4byte 0x687c0300
	.4byte 0x534f1241
	.4byte 0xb9b78bbb
	.4byte 0x85cb2760
	.4byte 0x3359ae11
	.4byte 0xf46b8462
	.4byte 0x05e6d358
	.4byte 0x170a174f
	.4byte 0xa817e020
	.4byte 0x8626b17d
	.4byte 0x23369a1e
	.4byte 0x618b21ec
	.4byte 0xf4602e22
	.4byte 0x7a6d3c19
	.4byte 0x61b793d0
	.4byte 0xda7de3cf
	.4byte 0x9a0500c4
	.4byte 0x91e4f014
	.4byte 0x0a313d01
	.4byte 0x276721e5
	.4byte 0x47a0a429
	.4byte 0x3c913c8f
	.4byte 0x882e2d8f
	.4byte 0xc7df42e4
	.4byte 0xbf4090e1
	.4byte 0x04190087
	.4byte 0xc300f26d
	.4byte 0x8609081a
	.4byte 0x33350a7b
	.4byte 0x63c4500d
	.4byte 0x03dfa703
	.4byte 0x4ba1622a
	.4byte 0x51eeda92
	.4byte 0x1ba45c24
	.4byte 0x7e501f87
	.4byte 0x65005f8f
	.4byte 0x0fc7843e
	.4byte 0xe7dedaca
	.4byte 0xb54d69c2
	.4byte 0x88412493
	.4byte 0x24e5a72e
	.4byte 0x92841a1c
	.4byte 0xa2c0e7c3
	.4byte 0xc071f037
	.4byte 0x69c5e7eb
	.4byte 0xc2e0373f
	.4byte 0x59c2223d
	.4byte 0xddf8566c
	.4byte 0xd33e3c0a
	.4byte 0x3c19f1ac
	.4byte 0x59c312e8
	.4byte 0x38f22a81
	.4byte 0x511e79cc
	.4byte 0xfc42b9cd
	.4byte 0xc7186e30
	.4byte 0xa1e46258
	.4byte 0x96f23f41
	.4byte 0x6003c9f7
	.4byte 0x985339a4
	.4byte 0xf7ab0e53
	.4byte 0x9226eaf1
	.4byte 0x704841e7
	.4byte 0x02c2804c
	.4byte 0x992b09f8
	.4byte 0x4b01b09a
	.4byte 0x2b547b36
	.4byte 0x1ee84d50
	.4byte 0xa63d9903
	.4byte 0x9ccd48b2
	.4byte 0x91684ccd
	.4byte 0x4022d03c
	.4byte 0x103a2613
	.4byte 0x4580f7e6
	.4byte 0x9b21911a
	.4byte 0x304d9780
	.4byte 0x82107df0
	.4byte 0x1c3e471a
	.4byte 0x335e95e1
	.4byte 0xde6cb747
	.4byte 0xfc84fbce
	.4byte 0xd80b1210
	.4byte 0x0e07c343
	.4byte 0x09c71efc
	.4byte 0x888f0ff1
	.4byte 0xf810587d
	.4byte 0x805451f9
	.4byte 0xf1321171
	.4byte 0x4284cd3c
	.4byte 0x1a1f79f7
	.4byte 0x7c69e3cf
	.4byte 0x41f3ea0c
	.4byte 0xf9821205
	.4byte 0x338605c9
	.4byte 0x6e6c0d1b
	.4byte 0xe108e856
	.4byte 0xbedd48f7
	.4byte 0xd017458f
	.4byte 0x0bc2339f
	.4byte 0xe3cb8e3f
	.4byte 0x0d7136a6
	.4byte 0x49087a5a
	.4byte 0x69c3b1e7
	.4byte 0x38f928af
	.4byte 0xf1a73f1e
	.4byte 0xe1ea0720
	.4byte 0xe20640dc
	.4byte 0xbce108ff
	.4byte 0xcf9029af
	.4byte 0xefe359e0
	.4byte 0xf15c1e34
	.4byte 0xc53837c3
	.4byte 0xebd8f0a7
	.4byte 0xec0b15b1
	.4byte 0xe1de1162
	.4byte 0xc34ebe04
	.4byte 0x1e6c9ee4
	.4byte 0x4c7df182
	.4byte 0x02087404
	.4byte 0x61c9241c
	.4byte 0x9c78080c
	.4byte 0x5fca1127
	.4byte Resource_Data012 + 0xf4cc
	.4byte 0x78cfaf46
	.4byte 0xe8e64de1
	.4byte 0x021f0821
	.4byte 0xd808f8c5
	.4byte 0xa4790ece
	.4byte 0x98978dff
	.4byte 0x25080806
	.4byte 0xb059aa0f
	.4byte 0x11123c07
	.4byte 0xd0102494
	.4byte 0x92484700
	.4byte 0x30fbc97c
	.4byte 0x26688e38
	.4byte 0x80467059
	.4byte 0x1e5200fb
	.4byte 0x82c0f227
	.4byte 0xc40603df
	.4byte 0x0709b670
	.4byte 0x3ffa5f98
	.4byte 0xdf3f0292
	.4byte 0x34e02b91
	.4byte 0x000200d6
	.4byte 0x4f01978f
	.4byte 0x819f1f7b
	.4byte 0xed0b8a0b
	.4byte 0xab1ee355
	.4byte 0xe7664900
	.4byte 0xf71cf208
	.4byte 0x0f7666d4
	.4byte 0xb4ed2830
	.4byte 0x1eeb6647
	.4byte 0xe89e3c8b
	.4byte 0xae69c804
	.4byte 0x67606c9f
	.4byte 0x6411df1e
	.4byte 0x511e2cf8
	.4byte 0xbc4d079b
	.4byte 0xf1e2040f
	.4byte 0x80083d1e
	.4byte 0x9bd98d79
	.4byte 0xc0ba11f7
	.4byte 0x0e20e622
	.4byte 0xe4003c75
	.4byte 0x4172cf70
	.4byte 0x1a271802
	.4byte 0x7844e901
	.4byte 0x2103f1ee
	.4byte 0xb25dc7bf
	.4byte 0x1f04f513
	.4byte 0xdd93643e
	.4byte 0x9221c7b9
	.4byte Text_MessageContexts + 0x2032e
	.4byte 0xd78fbcfa
	.4byte 0x43079051
	.4byte 0xedcc7b1f
	.4byte 0x80b80f24
	.4byte 0xce9cd1d4
	.4byte 0x9d76c8fb
	.4byte 0x8d2048ab
	.4byte 0xef2ba004
	.4byte 0x2a303323
	.4byte 0x8f71af30
	.4byte 0x01d18589
	.4byte 0x91dc2174
	.4byte 0xa28040f3
	.4byte 0x01b31a28
	.4byte 0x839f0363
	.4byte 0x1e02a1d3
	.4byte 0x3c7bf410
	.4byte 0x49c8e08c
	.4byte 0x4355e3e8
	.4byte 0x209a4e3d
	.4byte 0xd8bc7b20
	.4byte 0x08f8f7ed
	.4byte 0x73cfd7dc
	.4byte 0xc5e978d6
	.4byte 0xeebc79d7
	.4byte 0x91f90003
	.4byte 0x780b9fa0
	.4byte 0x6bdfd718
	.4byte 0x3ceb9c3c
	.4byte 0x389e11c7
	.4byte 0xf8f0a7c7
	.4byte 0x7c4026bd
	.4byte 0x739f8b69
	.4byte 0x25c139f1
	.4byte 0xf522b087
	.4byte 0xee183740
	.4byte 0x8f708f90
	.4byte 0x20a18eac
	.4byte 0x1910f91e
	.4byte 0x10783d10
	.4byte 0xc1c847bf
	.4byte 0x10e19750
	.4byte 0x80600dd9
	.4byte 0x15d21a53
	.4byte 0xc63c8186
	.4byte 0xa6d63dc2
	.4byte 0xa0662935
	.4byte 0x47c06046
	.4byte 0x883978f0
	.4byte 0x280f2f79
	.4byte 0x37f0d492
	.4byte 0x4c74ee44
	.4byte 0x97889223
	.4byte 0x976c1ce1
	.4byte 0x2f112ee1
	.4byte 0x4863e8f7
	.4byte Field_Map146 + 0x23e7
	.4byte 0x51cb5c00
	.4byte 0x0e68359f
	.4byte 0x87300f61
	.4byte 0xf70f603c
	.4byte 0x0dc3ebb0
	.4byte 0x3399e478
	.4byte 0x5e0a0204
	.4byte 0x0605e4c3
	.4byte 0x030822ce
	.4byte Summon_WingedKnightATiles + 0x1258
	.4byte 0x92906610
	.4byte 0x2f906b10
	.4byte 0x06624f30
	.4byte 0x48f0a7c0
	.4byte 0x269a3900
	.4byte 0xc0626289
	.4byte 0xfc25c127
	.4byte 0x6fd785c4
	.4byte 0x8523c738
	.4byte 0x4f741a06
	.4byte 0x1cbae717
	.4byte 0x8e36f3f5
	.4byte 0xf0d39a00
	.4byte 0x587bce46
	.4byte 0xb07833e4
	.4byte 0xe9c75e1d
	.4byte 0x037d06cb
	.4byte 0xf030071f
	.4byte 0xc7860685
	.4byte 0x0ecc54a4
	.4byte 0xd01e6110
	.4byte 0xf02e0a21
	.4byte 0x64e02c38
	.4byte 0x9cc1fe01
	.4byte 0xb8fb4786
	.4byte 0x28f9c887
	.4byte 0xf31e53d3
	.4byte 0x79e42024
	.4byte 0x1efc6544
	.4byte 0x61442d8f
	.4byte 0x188d39f9
	.4byte 0x3184f486
	.4byte 0x81ece41d
	.4byte 0xae0f70ab
	.4byte 0xe53ca3c0
	.4byte 0x81316c0b
	.4byte 0x489b46a1
	.4byte 0x02a38114
	.4byte 0xace82b5e
	.4byte 0x13341f86
	.4byte 0xc430720e
	.4byte 0x0b41711e
	.4byte 0xc2f0099c
	.4byte 0xd0019f1b
	.4byte 0x8e1a73f3
	.4byte 0x92407a1e
	.4byte 0xc0940088
	.4byte 0x1811c8fb
	.4byte 0x78038477
	.4byte 0x61478058
	.4byte 0x8b7c0cf9
	.4byte 0x0b2351f1
	.4byte 0x14fce0c6
	.4byte 0x93c1cfab
	.4byte 0x13e92b39
	.4byte 0x41af3c78
	.4byte 0x861a732d
	.4byte 0x86b8fc2e
	.4byte 0xf55803df
	.4byte 0x64ce443e
	.4byte 0x8e0cc040
	.4byte 0xd1a8fd7a
	.4byte Resource_Data4C9 + 0x1629
	.4byte 0x7c0ff3e0
	.4byte 0x3fcf81fe
	.4byte 0x3e07f9f0
	.4byte 0x1fe7c0ff
	.4byte 0x1f03fcf8
	.4byte 0x0003e05b
	.global Data_02002af8
Data_02002af8:
	.4byte 0x967c0300
	.4byte 0x6e2becc1
	.4byte 0xb073b739
	.4byte 0xda769f35
	.4byte 0x60de6573
	.4byte 0x06e73c05
	.4byte 0x011e10fb
	.4byte 0x8b12f1c0
	.4byte 0x08f30497
	.4byte 0x5798e9ce
	.4byte 0x6663f0b8
	.4byte 0x91c88212
	.4byte 0x0cf7e045
	.4byte 0x556c8966
	.4byte 0x3ad1d4e7
	.4byte 0x50583efb
	.4byte 0xf021c0a4
	.4byte 0x3f9f0421
	.4byte 0x59db0c67
	.4byte 0x3cd4ea4d
	.4byte 0x81a30bab
	.4byte 0x8fd9756f
	.4byte 0x0f0d00a6
	.4byte 0xb9c6a357
	.4byte 0x2c366878
	.4byte 0x01a44870
	.4byte 0x02fc1a41
	.4byte 0x7b3f9d7d
	.4byte 0x61abd6de
	.4byte 0x56440682
	.4byte 0xac568eaf
	.4byte 0x87a266e1
	.4byte 0xf00f3cc7
	.4byte 0xa5797bcc
	.4byte 0x20494636
	.4byte 0x9d854917
	.4byte 0xa76cf0f3
	.4byte 0x0411f028
	.4byte 0xce671cea
	.4byte 0xefcb2329
	.4byte 0x78c831f1
	.4byte 0x0050b0ce
	.4byte 0xf93f3b08
	.4byte 0x1b79163d
	.4byte 0x3673a21e
	.4byte 0xe44f0cc9
	.4byte 0xd39c0d43
	.4byte 0x10f9ce6a
	.4byte 0xc157bf1e
	.4byte 0xaebc8f27
	.4byte 0xa5713840
	.4byte 0xc7bf5873
	.4byte 0x6bf91730
	.4byte 0x7db68cc6
	.4byte 0x9128d69e
	.4byte 0x3c99e08f
	.4byte 0xd3585a36
	.4byte 0x9985209d
	.4byte 0xa83457c0
	.4byte 0xb4f239f4
	.4byte 0x70d10444
	.4byte 0x27bf6a58
	.4byte 0x69b4cb1a
	.4byte 0x8e75aad1
	.4byte 0x68b4c446
	.4byte 0x27bf6463
	.4byte 0x6144fea3
	.4byte 0x45830327
	.4byte 0xd31fa98c
	.4byte 0x1ab81298
	.4byte 0x30b49346
	.4byte 0xe0083141
	.4byte 0x6ab320fb
	.4byte 0xd11368d1
	.4byte 0x215902a0
	.4byte 0x39d3dfa3
	.4byte 0x61b0a2d1
	.4byte 0x9c34c391
	.4byte 0x704ec8b0
	.4byte 0x59f490d1
	.4byte 0x8ba98cb1
	.4byte 0xf81705c1
	.4byte 0x3e320a3d
	.4byte 0x3108b8e4
	.4byte 0x764b5dae
	.4byte 0xf036116c
	.4byte 0x3642d33e
	.4byte 0x0f800dc3
	.4byte 0x840099ec
	.4byte 0x91e13c42
	.4byte 0x44f0581a
	.4byte 0x3843c200
	.4byte 0xc2c2e0c8
	.4byte 0x08fabec5
	.4byte 0x6921f01e
	.4byte 0x225173d8
	.4byte 0x87a2eeb5
	.4byte 0x929d837f
	.4byte 0xc8f4ee14
	.4byte 0x38ba078f
	.4byte 0x30c39c16
	.4byte 0x2860e1b2
	.4byte 0x1f7879e7
	.4byte 0x33d64cf0
	.4byte 0xc1fac698
	.4byte 0x5982de7e
	.4byte 0xc7c78d3e
	.4byte 0x7e58f810
	.4byte 0x38e0539f
	.4byte 0x72f14d02
	.4byte 0x80e75e1d
	.4byte 0x87cf1ec7
	.4byte 0xeb5c7908
	.4byte 0xece9e79c
	.4byte 0x5ece8065
	.4byte 0x904a00d0
	.4byte 0x1f91d493
	.4byte 0x1619e0d3
	.4byte 0x5c045b4c
	.4byte 0x673a7cc8
	.4byte 0x7c46a45c
	.4byte 0x91fe23c7
	.4byte 0x3ee3f0e4
	.4byte 0xab6a9e12
	.4byte 0xc53efb3b
	.4byte 0x0d111890
	.4byte 0x34e7e002
	.4byte 0x761b988a
	.4byte 0x4e1add06
	.4byte 0x35a66283
	.4byte 0x1aa7b7a4
	.4byte 0xad3c43c2
	.4byte 0x28a44862
	.4byte 0x0d44b1e3
	.4byte 0x8036018f
	.4byte 0x1b607b93
	.4byte 0x401201a9
	.4byte 0x1e923f23
	.4byte 0x6b49f518
	.4byte 0x5a1e12e0
	.4byte 0xf5852fe7
	.4byte 0x1f037f38
	.4byte 0xf40d81d4
	.4byte 0x0dc795f2
	.4byte 0x7ace82b1
	.4byte 0x1a1e0180
	.4byte 0x1aaa0436
	.4byte 0xc780e39f
	.4byte 0x68178008
	.4byte 0x207f8d3c
	.4byte 0x902f7430
	.4byte 0x879b01e8
	.4byte 0x20223c89
	.4byte 0x2c9ec2ae
	.4byte 0x978030f2
	.4byte 0xcf1ae03b
	.4byte 0xf7b673b3
	.4byte 0xb82a0739
	.4byte 0xe0ae1e68
	.4byte 0x8f1efd09
	.4byte 0x3e3fe00b
	.4byte 0xf3f5610f
	.4byte 0xe3c69f42
	.4byte 0x02d8fbf7
	.4byte 0x3ef1a73f
	.4byte 0xf23c7991
	.4byte 0x3cc3c030
	.4byte 0x7b978fbe
	.4byte 0xfcc7bcfc
	.4byte 0x46c041c7
	.4byte 0x99590da4
	.4byte 0xdfb3f1b6
	.4byte 0xc584eb03
	.4byte 0xb26232d8
	.4byte 0x46c43a62
	.4byte 0xf23b274a
	.4byte 0x1978e63c
	.4byte 0xf2696a9d
	.4byte 0x0c3086ea
	.4byte 0x10e111f8
	.4byte 0xec06c64c
	.4byte 0xd423cce9
	.4byte 0x03c8d513
	.4byte 0xb231fd9f
	.4byte 0x98f48901
	.4byte 0xa67ea4dc
	.4byte 0x2e51e8dd
	.4byte 0x7de2ae68
	.4byte 0x61c78380
	.4byte 0x48d41691
	.4byte 0xa81ee4c6
	.4byte 0x3c556aa6
	.4byte 0x5a34d8e1
	.4byte 0x881e4828
	.4byte 0x483dfa47
	.4byte 0xa8cc5540
	.4byte 0x24c19caa
	.4byte 0xec3e3538
	.4byte 0x8d1b8c7d
	.4byte 0xf91efc1b
	.4byte 0x0f07d3a3
	.4byte 0xf90d18e3
	.4byte 0x4217ae59
	.4byte 0x429007a0
	.4byte 0x22b199a5
	.4byte 0x1950538f
	.4byte 0xdc300a87
	.4byte 0x5d5ad923
	.4byte 0xa1593acf
	.4byte 0x460f47bc
	.4byte 0xd623ee60
	.4byte 0x1e23fe70
	.4byte 0x3a1038f0
	.4byte 0x3eaccfe0
	.4byte 0xcf8840f0
	.4byte 0x7c60f4e3
	.4byte 0xc007833e
	.4byte 0x53b6f221
	.4byte 0xc256a7cf
	.4byte 0xc5079e3d
	.4byte 0x3c42a7e7
	.4byte 0x35e3cc4a
	.4byte 0xc62e027c
	.4byte 0x3cf8f1c7
	.4byte 0xf8059402
	.4byte 0x8d39ac22
	.4byte 0x981e89d4
	.4byte 0x523cf3bf
	.4byte 0xc30df85e
	.4byte 0x50bdc67d
	.4byte 0xe9ed0e13
	.4byte 0x7130f311
	.4byte 0x9c0767e3
	.4byte 0x100411bd
	.4byte 0xfe66b92c
	.4byte 0x8ab658eb
	.4byte 0xbc7d0515
	.4byte 0x0287b48f
	.4byte 0x06e089a4
	.4byte 0x98f7224d
	.4byte 0x549400e1
	.4byte 0xc8517903
	.4byte 0x54868087
	.4byte 0x1ff0c320
	.4byte 0x91d5b180
	.4byte 0x00183e91
	.4byte 0x7eb09f71
	.4byte 0x7122998f
	.4byte 0x20d4a711
	.4byte 0xbf008827
	.4byte 0x051ab187
	.4byte 0x0c3c483e
	.4byte 0xf0df8016
	.4byte 0x07a93062
	.4byte 0xde7db08b
	.4byte 0xb3a1f447
	.4byte 0x4818ef17
	.4byte 0x1ee10c30
	.4byte 0x8758efcf
	.4byte 0x7a3e3dc3
	.4byte 0x0788e1de
	.4byte 0x0d71c34e
	.4byte 0xc52011d4
	.4byte 0x8f220f89
	.4byte 0x53be7406
	.4byte 0xc7c3d611
	.4byte 0x1dfccac1
	.4byte 0x394a0794
	.4byte 0xe7f72967
	.4byte Resource_Data012 + 0x97e1
	.4byte 0x0de41669
	.4byte 0x9fac26f4
	.4byte 0x2238f0c8
	.4byte 0x0e4f8239
	.4byte 0x6533390a
	.4byte 0x4c1e4034
	.4byte 0x5c81e840
	.4byte 0x80c2ba0f
	.4byte 0xa203df85
	.4byte 0x2443fe30
	.4byte 0xc1e12581
	.4byte 0xa15c06a6
	.4byte 0xfc0b36c7
	.4byte 0x8997869c
	.4byte 0x0d69e23a
	.4byte 0x3e43c8dc
	.4byte 0x10180a4d
	.4byte 0xb03c8b00
	.4byte 0xb03dabe0
	.4byte 0x119bc7ce
	.4byte 0x1336da07
	.4byte 0xd8e1a738
	.4byte 0x5e3b63a9
	.4byte 0x02bce3bf
	.4byte 0x3c0f40a4
	.4byte 0x28413a40
	.4byte 0xfbd7abe3
	.4byte 0xcabe3df8
	.4byte 0x20869cc4
	.4byte 0x8a5c4387
	.4byte 0xdc7ff1ac
	.4byte 0xac1e8d6b
	.4byte 0xcc43c797
	.4byte 0x4203aa73
	.4byte 0x5da0f3e1
	.4byte 0x8020a33e
	.4byte 0xf9f03fcf
	.4byte 0xc0ff3e07
	.4byte 0xfcf81fe7
	.4byte 0xe07f9f03
	.4byte 0x6c7c0ff3
	.2byte 0x0f81
	.global Data_02002fd6
Data_02002fd6:
	.2byte 0x0000
	.4byte 0x01000000
	.4byte 0x03020201
	.4byte 0x03030303
	.4byte 0x00010203
	.global Data_02002fe8
Data_02002fe8:
	.4byte 0x0c040e02
	.4byte Text_MessageContexts + 0x1fdd6
	.4byte 0x040c060a
	.4byte 0x00100010
	.4byte 0x060a040c
	.4byte 0x0a060808
	.4byte 0x0e020c04
	.4byte 0x0c040e02
	.4byte Text_MessageContexts + 0x1fdd6
	.4byte 0x040c060a
	.4byte 0x00100010
	.4byte 0x060a040c
	.4byte 0x0a060808
	.4byte 0x0e020c04
