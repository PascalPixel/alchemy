.syntax unified
	.thumb
	.section .text.x02008184,"ax",%progbits
	.balign 4
	.global Func_02000184
	.thumb_func
Func_02000184:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	bl Func_02001da0
	movs r0, #0
	bl Func_02001ee8
	movs r0, #26
	bl Func_02001f10
	movs r0, #212
	bl Func_02001f10
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_02001ec0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_02001eb8
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl Func_02001ec0
	movs r1, #1
	ldr r0, .L_020083b4
	bl Func_02001eb8
	movs r0, #40
	bl Func_02001ec8
	movs r0, #40
	bl WaitFrames
	movs r0, #197
	bl Func_02001f10
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_020083b8
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008222
	movs r5, #0
.L_020081f0:
	cmp r5, #20
	bne .L_02008200
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #0
	bl Func_02001e88
.L_02008200:
	ldr r6, .L_020083b8
	movs r1, #128
	ldr r2, [r6]
	lsls r1, r1, #8
	ldr r3, [r2, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r2, #12]
	adds r5, #1
	bl WaitFrames
	cmp r5, #79
	bls .L_020081f0
	ldr r3, [r6]
	movs r2, #4
	adds r3, #85
	strb r2, [r3]
.L_02008222:
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	ldr r3, .L_020083bc
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #179
	ldr r0, [r6]
	lsls r1, r1, #1
	movs r2, #108
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	ldr r1, [r6]
	movs r0, #9
	bl Func_02001e20
	movs r0, #1
	bl WaitFrames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #182
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #124
	bl ObjectMotion_SetPositionAndReset
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	ldr r0, .L_020083c0
	bl Func_02001e48
	bl Func_02000100
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	bl Func_02001e60
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl Func_02001e90
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	bl Func_02001e70
	movs r2, #20
	movs r0, #10
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	bl Func_02001e60
	bl Func_02000100
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	bl Func_02000100
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r6]
	bl Func_02001e88
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl Func_02001e88
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #6
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #10
	bl Object_GetById
	ldrh r3, [r0, #6]
	ldr r1, .L_020083b0
	strh r3, [r5, #6]
	movs r0, #1
	mov r8, r1
	bl WaitFrames
	movs r0, #14
	movs r1, #10
	bl Func_02001e20
	movs r2, #0
	movs r1, #0
	movs r0, #10
	bl Func_02001e10
	movs r0, #110
	bl Func_02001f10
	movs r1, #5
	movs r0, #14
	bl Object_SetModeById
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #176
	movs r2, #0
	b .L_020083c4
	.2byte 0x0000
.L_020083b0:
	.4byte 0x00000000
.L_020083b4:
	.4byte 0x00403108
.L_020083b8:
	.4byte gData_02002058
.L_020083bc:
	.4byte gAdventure
.L_020083c0:
	.4byte MsgOhYoungWitch
.L_020083c4:
	movs r0, #14
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r1, #3
	movs r0, #8
	bl Object_SetModeById
	movs r0, #110
	bl Func_02001f10
	movs r1, #6
	movs r0, #14
	bl Object_SetModeById
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r1, #1
	movs r0, #14
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	movs r0, #40
	bl FieldEvent_WaitFrames
	bl Func_02000100
	movs r0, #14
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #14
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #14
	movs r1, #0
	bl Func_02001e60
	ldr r1, .L_0200859c
	movs r0, #14
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #14
	movs r1, #0
	bl Func_02001e60
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_020085a0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #196
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #164
	bl ObjectMotion_SetPositionAndReset
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #14
	bl Func_02001e88
	movs r2, #20
	movs r0, #14
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #14
	movs r1, #0
	bl Func_02001e60
	movs r1, #196
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #220
	bl ObjectMotion_SetPositionAndReset
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02001e10
	ldr r0, [r6]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #12
	bl Object_GetById
	mov r2, r8
	adds r0, #85
	strb r2, [r0]
	ldr r1, .L_020085a4
	movs r0, #12
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #20
	bl FieldEvent_WaitFrames
	bl Func_02000100
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r6]
	bl Func_02001e88
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_02001e88
	movs r1, #224
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #80
	movs r0, #9
	bl ObjectMotion_ArmCallback
	bl Func_02000100
	movs r0, #20
	bl FieldEvent_WaitFrames
	bl Func_02000100
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #6
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	ldr r0, [r6]
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_02001ec0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	adds r0, #3
	bl Func_02001eb8
	movs r0, #10
	bl Func_02001ec8
	movs r0, #10
	bl WaitFrames
	ldr r5, .L_020085a8
	ldr r3, .L_02008598
	movs r7, #192
	strh r3, [r5]
	movs r0, #1
	lsls r7, r7, #18
	bl WaitFrames
	ldr r3, [r7, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r0, [r3]
	movs r1, #0
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r3]
	asrs r0, r0, #16
	movs r2, #2
	bl Func_02001d88
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl Func_02001eb8
	movs r0, #20
	bl Func_02001ec8
	movs r0, #20
	b .L_020085ac
.L_02008598:
	.4byte 0x00007fff
.L_0200859c:
	.4byte gData_02001f78
.L_020085a0:
	.4byte 0x00019999
.L_020085a4:
	.4byte gData_02001f9c
.L_020085a8:
	.4byte 0x0500021e
.L_020085ac:
	bl WaitFrames
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	movs r1, #224
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	ldr r0, [r6]
	bl Func_02001e88
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	bl Func_02000100
	movs r1, #224
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_02000100
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r6]
	bl Func_02001e88
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #9
	bl Func_02001e88
	bl Func_02000100
	ldr r0, [r6]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl ObjectMotion_ArmCallback
	ldr r1, .L_02008948
	movs r0, #12
	bl Object_SetActionCallbackAndRefreshById
	bl Func_02000100
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001e90
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl Func_02001e90
	movs r1, #224
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	bl Func_02000100
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	bl Func_02000100
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02001e70
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	movs r1, #224
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #13
	bl Func_02001e88
	bl Func_02000100
	movs r0, #9
	movs r1, #0
	movs r2, #40
	bl Func_02001e58
	bl Func_02000100
	movs r2, #10
	movs r0, #9
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	bl Func_02000100
	movs r1, #128
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02001e70
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	bl Func_02000100
	movs r1, #224
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02001e70
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r1, #0
	movs r0, #9
	bl Func_02001e60
	bl Func_02000100
	movs r0, #0
	bl Func_02001f10
	movs r0, #220
	bl Func_02001f10
	movs r1, #3
	movs r0, #8
	bl Object_SetModeById
	movs r0, #13
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r1, #212
	movs r2, #208
	lsls r2, r2, #15
	movs r0, #13
	lsls r1, r1, #17
	bl Func_02001e10
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02001eb8
	movs r0, #40
	bl Func_02001ec8
	movs r0, #40
	bl WaitFrames
	bl Func_02001094
	movs r2, #0
	movs r1, #0
	movs r0, #13
	bl Func_02001e10
	movs r0, #1
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_02001eb8
	movs r1, #1
	ldr r0, .L_0200894c
	bl Func_02001eb8
	movs r0, #60
	bl Func_02001ec8
	movs r0, #60
	bl WaitFrames
	bl Func_02000100
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_02001eb8
	movs r0, #20
	bl Func_02001ec8
	movs r0, #20
	bl WaitFrames
	ldr r5, .L_02008950
	movs r2, #3
	ldr r3, [r5]
	movs r0, #10
	adds r3, #85
	strb r2, [r3]
	bl FieldEvent_WaitFrames
	movs r0, #54
	adds r0, #255
	bl Func_02001f10
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #25
	bl Func_02001f10
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02001e70
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	ldr r0, [r6]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #196
	movs r2, #104
	ldr r0, [r6]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02001e70
	ldr r0, [r6]
	movs r1, #28
	bl Object_SetModeById
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02008868
	ldr r0, [r6]
	bl Object_GetById
	ldr r4, [r5]
	mov r2, r8
	adds r3, r4, #0
	adds r3, #85
	strb r2, [r3]
	movs r3, #144
	ldr r2, [r0, #12]
	lsls r3, r3, #14
	ldr r1, [r0, #8]
	adds r2, r2, r3
	ldr r3, [r0, #16]
	adds r0, r4, #0
	bl Func_02001d48
	ldr r0, [r5]
	bl Func_02001d50
.L_02008868:
	movs r0, #242
	movs r1, #0
	bl PartyInventory_GiveItem
	ldr r0, [r5]
	cmp r0, #0
	beq .L_0200887a
	bl Func_02001d30
.L_0200887a:
	ldr r0, [r6]
	movs r1, #1
	bl Object_SetModeById
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_020088ce
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r1, #0
	movs r0, #9
	bl Func_02001e60
	ldr r2, [r7, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020088ec
.L_020088ce:
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #9
	adds r3, #1
	movs r1, #3
	strh r3, [r2]
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
.L_020088ec:
	movs r1, #188
	movs r2, #124
	lsls r1, r1, #1
	movs r0, #9
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02001e70
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	ldr r3, .L_02008954
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_02008958
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008972
.L_02008948:
	.4byte gData_02001fe4
.L_0200894c:
	.4byte 0x00403108
.L_02008950:
	.4byte gData_02002058
.L_02008954:
	.4byte gAdventure
.L_02008958:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r0, #9
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02001e60
.L_02008972:
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	ldr r7, .L_02008a84
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r7, r2
	ldr r0, [r6]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	ldr r0, [r6]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	ldr r5, .L_02008a88
	movs r0, #9
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl FieldEvent_WaitFrames
	adds r1, r5, #0
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #0
	bl Func_02001eb8
	movs r6, #192
	movs r0, #60
	bl Func_02001ec8
	lsls r6, r6, #18
	movs r0, #64
	bl WaitFrames
	ldr r1, [r6, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r5, #218
	adds r3, #85
	str r3, [r2]
	lsls r5, r5, #1
	movs r3, #10
	str r3, [r1, r5]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02001eb8
	ldr r3, [r6, #108]
	movs r6, #1
	str r6, [r3, r5]
	movs r0, #1
	bl Func_02001ec8
	bl Func_02001d78
	movs r1, #0
	movs r2, #3
	ldr r0, .L_02008a8c
	bl Func_02001d88
	bl Func_02001d80
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #107
	bl GameFlag_ClearBit
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #242
	bl GameFlag_ClearBit
	ldr r2, .L_02008a90
	movs r1, #152
	lsls r1, r1, #2
	adds r3, r7, r1
	strh r2, [r3]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #98
	adds r3, r7, r2
	ldr r2, .L_02008a94
	subs r1, #124
	strh r6, [r3]
	adds r3, r7, r1
	strh r2, [r3]
	movs r3, #243
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #6
	strh r3, [r2]
	movs r0, #3
	bl Func_02001eb0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008a84:
	.4byte gAdventure
.L_02008a88:
	.4byte gData_0200200c
.L_02008a8c:
	.4byte MsgCarriedTheMessage
.L_02008a90:
	.4byte 0x0000006e
.L_02008a94:
	.4byte 0x00000070
	.section .text.x02008be8,"ax",%progbits
	.balign 4
	.global Func_02000be8
	.thumb_func
Func_02000be8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #132
	movs r2, #188
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r1, r1, r3
	adds r2, r2, r3
	mov r10, r1
	mov r8, r2
	bl Func_02001da0
	movs r0, #0
	bl Func_02001ee8
	bl Func_02001d60
	movs r7, #1
	movs r6, #0
.L_02008c18:
	adds r3, r6, #0
	subs r3, #17
	cmp r3, #62
	bhi .L_02008c3e
	adds r1, r7, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_02008c2a
	adds r3, r7, #4
.L_02008c2a:
	asrs r0, r3, #2
	movs r3, #16
	subs r3, r3, r0
	lsls r2, r0, #8
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	adds r7, r1, #0
.L_02008c3e:
	mov r1, r10
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r1, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	adds r6, #1
	adds r3, r3, r2
	movs r2, #176
	lsls r2, r2, #1
	str r3, [r1, #12]
	cmp r6, r2
	blt .L_02008c18
	bl Func_02001d40
	movs r0, #1
	bl WaitFrames
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r5, .L_02008d34
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #164
	lsls r1, r1, #1
	movs r2, #184
	ldr r0, [r5]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #154
	lsls r0, r0, #1
	bl Func_02001f10
	ldr r0, [r5]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	cmp r6, #0
	beq .L_02008cf2
.L_02008cae:
	adds r3, r6, #0
	subs r3, #33
	cmp r3, #62
	bhi .L_02008cd4
	adds r3, r7, #1
	adds r2, r3, #0
	cmp r3, #0
	bge .L_02008cc0
	adds r2, r7, #4
.L_02008cc0:
	asrs r1, r2, #2
	movs r3, #16
	subs r3, r3, r1
	lsls r2, r1, #8
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	subs r7, #1
.L_02008cd4:
	mov r1, r10
	ldr r3, [r1, #12]
	ldr r2, .L_02008d38
	movs r0, #1
	adds r3, r3, r2
	str r3, [r1, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	subs r6, #1
	adds r3, r3, r2
	str r3, [r1, #12]
	bl WaitFrames
	cmp r6, #0
	bne .L_02008cae
.L_02008cf2:
	ldr r3, .L_02008d34
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02001f10
	movs r0, #1
	bl WaitFrames
	movs r0, #125
	bl Func_02001f10
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
	bl Func_02001da8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d34:
	.4byte gAdventure
.L_02008d38:
	.4byte 0xffff0000
	.section .text.x02008d3c,"ax",%progbits
	.global Func_02000d3c
	.thumb_func
Func_02000d3c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #188
	adds r1, r1, r3
	lsls r2, r2, #1
	adds r7, r3, r2
	mov r8, r1
	bl Func_02001da0
	movs r0, #0
	bl Func_02001ee8
	bl Func_02001d60
	ldr r5, .L_02008e4c
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #8
	negs r2, r2
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_02001e70
	movs r0, #154
	lsls r0, r0, #1
	bl Func_02001f10
	ldr r0, [r5]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r6, #1
	movs r5, #0
.L_02008dae:
	adds r3, r5, #0
	subs r3, #17
	cmp r3, #62
	bhi .L_02008dd4
	adds r1, r6, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_02008dc0
	adds r3, r6, #4
.L_02008dc0:
	asrs r0, r3, #2
	movs r3, #16
	subs r3, r3, r0
	lsls r2, r0, #8
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	adds r6, r1, #0
.L_02008dd4:
	mov r1, r8
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r7, #12]
	bl WaitFrames
	movs r2, #96
	adds r5, #1
	adds r2, #255
	cmp r5, r2
	ble .L_02008dae
	ldr r5, .L_02008e4c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #1
	ldr r0, [r5]
	bl ObjectMotion_SetActionVariant
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02001f10
	movs r0, #1
	bl WaitFrames
	movs r0, #125
	bl Func_02001f10
	movs r0, #20
	bl FieldEvent_WaitFrames
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #1
	adds r0, #34
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02001da8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008e4c:
	.4byte gAdventure
	.section .text.x02009710,"ax",%progbits
	.global Func_02001710
	.thumb_func
Func_02001710:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl Func_02001da0
	movs r0, #0
	bl Func_02001ee8
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	movs r0, #1
	bl WaitFrames
	bl Func_02001010
	bl Func_02001d40
	movs r0, #1
	bl WaitFrames
	ldr r6, .L_02009a94
	movs r2, #133
	lsls r2, r2, #2
	movs r5, #224
	adds r6, r6, r2
	lsls r5, r5, #8
	movs r1, #180
	movs r2, #216
	adds r3, r5, #0
	ldr r0, [r6]
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02001e18
	movs r1, #188
	movs r2, #216
	adds r3, r5, #0
	movs r0, #7
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02001e18
	movs r1, #180
	movs r2, #200
	movs r3, #0
	movs r0, #11
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r5, #160
	mov r10, r3
	lsls r5, r5, #8
	bl Func_02001e18
	movs r1, #204
	movs r2, #216
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #16
	adds r3, r5, #0
	bl Func_02001e18
	movs r2, #128
	lsls r2, r2, #8
	mov r9, r2
	movs r1, #212
	movs r2, #200
	movs r0, #6
	lsls r1, r1, #17
	lsls r2, r2, #16
	mov r3, r9
	bl Func_02001e18
	movs r3, #176
	movs r1, #196
	movs r2, #200
	lsls r1, r1, #17
	lsls r3, r3, #8
	lsls r2, r2, #16
	movs r0, #12
	movs r7, #192
	bl Func_02001e18
	lsls r7, r7, #18
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	mov r8, r2
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r1, #208
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #12
	lsls r1, r1, #7
	bl Func_02001e70
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #12
	bl Func_02001e88
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	adds r1, r5, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #12
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #196
	lsls r1, r1, #1
	movs r2, #168
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #210
	bl Func_02001f10
	bl Func_02000f00
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #185
	bl Func_02001f10
	movs r0, #8
	bl Object_GetById
	mov r3, r10
	adds r0, #85
	strb r3, [r0]
	bl Func_02001064
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #8
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #212
	movs r2, #152
	movs r0, #8
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #12
	bl Func_02001e70
	ldr r0, .L_02009a98
	bl Func_02001e48
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02001e60
	movs r0, #12
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #196
	lsls r1, r1, #1
	movs r2, #140
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl WaitFrames
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl Func_02001e10
	movs r0, #1
	bl WaitFrames
	mov r1, r8
	movs r2, #20
	movs r0, #5
	bl Func_02001e88
	movs r0, #5
	mov r1, r9
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #5
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200993a
	movs r0, #7
	movs r1, #0
	bl Func_02001e70
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02001e60
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009960
.L_0200993a:
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #7
	adds r3, #1
	strh r3, [r2]
	mov r1, r9
	bl Func_02001e70
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02001e60
	bl Func_02001f08
.L_02009960:
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #6
	bl Func_02001e90
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Func_02001e60
	movs r0, #11
	movs r1, #0
	bl Func_02001e70
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #11
	movs r1, #0
	bl Func_02001e60
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	bl Func_02001e70
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #11
	movs r1, #0
	bl Func_02001e60
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	bl Func_02001e70
	movs r0, #11
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #11
	movs r1, #0
	bl Func_02001e60
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	ldr r3, .L_02009a94
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #11
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #7
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #6
	ldr r1, .L_02009a9c
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_02009aa0
	movs r0, #11
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #6
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #253
	bl GameFlag_SetBit
	bl Func_02001da8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009a94:
	.4byte gAdventure
.L_02009a98:
	.4byte MsgIMightBe
.L_02009a9c:
	.4byte 0x00013333
.L_02009aa0:
	.4byte gData_02001f4c
	.section .text.x02009ba8,"ax",%progbits
	.global Func_02001ba8
	.thumb_func
Func_02001ba8:
	push {r5, r6, r7, lr}
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009c28
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009c3c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	lsls r2, r2, #1
	adds r7, r3, r2
	adds r2, #112
	adds r6, r3, r2
	movs r5, #1
	bl Func_02001d60
	movs r4, #0
.L_02009bdc:
	adds r3, r4, #0
	subs r3, #17
	cmp r3, #62
	bhi .L_02009c02
	adds r1, r5, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_02009bee
	adds r3, r5, #4
.L_02009bee:
	asrs r0, r3, #2
	movs r3, #16
	subs r3, r3, r0
	lsls r2, r0, #8
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	adds r5, r1, #0
.L_02009c02:
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r7, #12]
	adds r4, #1
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r3, #176
	lsls r3, r3, #1
	cmp r4, r3
	blt .L_02009bdc
	bl Func_02001d40
	movs r0, #1
	bl WaitFrames
	b .L_02009c3c
.L_02009c28:
	ldr r3, .L_02009c40
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_02009c3c
	bl Func_02000be8
.L_02009c3c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c40:
	.4byte gAdventure
