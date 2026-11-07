.syntax unified
	.thumb
	.section .text.x020086f4,"ax",%progbits
	.global Func_020006f4
	.thumb_func
Func_020006f4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #198
	adds r0, #255
	bl PartyInventory_FindOwner
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	bne .L_0200871a
	b .L_02008d5c
.L_0200871a:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	adds r7, r0, #0
	cmp r7, #0
	bne .L_0200872c
	b .L_02008978
.L_0200872c:
	adds r2, r5, #0
	movs r3, #0
	adds r0, r5, #0
	adds r1, r5, #0
	bl Camera_MoveWithinMapBounds
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02003f64
	movs r0, #140
	movs r2, #190
	lsls r0, r0, #16
	adds r1, r5, #0
	lsls r2, r2, #16
	movs r3, #1
	bl Camera_MoveWithinMapBounds
	ldr r3, .L_0200896c
	movs r0, #133
	lsls r0, r0, #2
	adds r7, r3, r0
	ldr r1, [r7]
	movs r0, #10
	movs r2, #0
	bl Actor_SetAngleToward
	ldr r2, .L_02008970
	mov r10, r2
	mov r0, r10
	bl Func_02003f24
	movs r0, #10
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003f74
	bl Func_02003fc4
	ldr r0, [r7]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	mov r9, r0
	cmp r0, #0
	beq .L_02008794
	b .L_02008944
.L_02008794:
	ldr r0, [r7]
	bl Actor_GetById
	ldr r1, [r7]
	mov r8, r0
	movs r0, #10
	bl Actor_WatchAndSetScript
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r7]
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r2, #168
	ldr r0, [r7]
	movs r1, #98
	bl Actor_SetPositionAndReset
	movs r1, #192
	ldr r0, [r7]
	lsls r1, r1, #8
	bl Func_02003f3c
	ldr r0, [r7]
	movs r1, #4
	movs r2, #10
	bl Actor_Jump
	movs r0, #234
	movs r1, #208
	movs r2, #192
	movs r3, #160
	adds r0, #255
	lsls r1, r1, #15
	lsls r2, r2, #13
	lsls r3, r3, #16
	bl Func_02003df4
	adds r5, r0, #0
	movs r6, #0
	cmp r5, #0
	beq .L_0200880e
	movs r1, #198
	adds r1, #255
	bl Func_02003e3c
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r3, r5, #0
	adds r3, #85
	adds r2, r5, #0
	strb r6, [r3]
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
.L_0200880e:
	movs r0, #10
	bl FieldEvent_Wait
	bl Func_02003fbc
	movs r0, #10
	bl Actor_Stop
	ldr r0, [r7]
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02003f54
	mov r0, r10
	adds r0, #1
	bl Func_02003f24
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #10
	movs r1, #0
	bl Func_02003f2c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r2, #164
	movs r0, #10
	movs r1, #128
	bl Actor_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #10
	bl Func_02003f3c
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_02008974
	adds r2, #204
	movs r0, #10
	bl Actor_SetSpeedParameters
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	mov r10, r3
	ands r3, r2
	strb r3, [r0]
	movs r1, #108
	movs r2, #168
	movs r0, #10
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	mov r0, r8
	ldr r2, [r0, #12]
	movs r3, #192
	lsls r3, r3, #13
	ldr r1, [r0, #8]
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #234
	adds r0, #255
	bl Func_02003df4
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020088f0
	ldr r0, [r7]
	movs r1, #28
	bl Actor_SetModeById
	adds r3, r6, #0
	adds r3, #85
	mov r0, r9
	movs r1, #227
	strb r0, [r3]
	lsls r1, r1, #1
	adds r0, r6, #0
	bl Func_02003e3c
	movs r0, #20
	bl FieldEvent_Wait
.L_020088f0:
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	movs r1, #116
	strb r3, [r0]
	movs r2, #164
	movs r0, #10
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	bl Func_02003fc4
	movs r0, #198
	adds r0, #255
	bl PartyInventory_Remove
	movs r0, #227
	lsls r0, r0, #1
	movs r1, #0
	bl PartyInventory_GiveItem
	cmp r6, #0
	bne .L_0200893a
	b .L_02008cf4
.L_0200893a:
	adds r0, r6, #0
	bl Func_02003dfc
	ldr r0, [r7]
	b .L_02008cee
.L_02008944:
	bl Func_02003fbc
	movs r1, #10
	adds r1, #255
	movs r2, #20
	movs r0, #10
	bl Func_02003f54
	mov r0, r10
	adds r0, #4
	bl Func_02003f24
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #10
	movs r1, #0
	bl Func_02003f2c
	b .L_02008d80
	.2byte 0x0000
.L_0200896c:
	.4byte gMainRoutineWork
.L_02008970:
	.4byte MsgAreYouGiving
.L_02008974:
	.4byte 0x00019999
.L_02008978:
	ldr r2, .L_02008c90
	mov r9, r2
	mov r0, r9
	bl Func_02003f24
	movs r0, #10
	movs r1, #0
	bl Func_02003f2c
	adds r2, r5, #0
	adds r0, r5, #0
	adds r1, r5, #0
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02003f64
	movs r0, #140
	movs r2, #190
	adds r1, r5, #0
	movs r3, #1
	lsls r0, r0, #16
	lsls r2, r2, #16
	bl Camera_MoveWithinMapBounds
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	ldr r3, .L_02008c94
	movs r0, #133
	lsls r0, r0, #2
	movs r1, #204
	movs r2, #204
	adds r6, r3, r0
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r2, #190
	ldr r0, [r6]
	movs r1, #120
	bl Actor_SetPositionAndReset
	movs r1, #0
	ldr r0, [r6]
	bl Func_02003f3c
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	mov r10, r3
	ands r3, r2
	strb r3, [r0]
	movs r1, #132
	movs r2, #190
	ldr r0, [r6]
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r8, r2
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r1, #140
	movs r0, #234
	movs r3, #190
	adds r0, #255
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #16
	bl Func_02003df4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008a5a
	movs r1, #198
	adds r1, #255
	bl Func_02003e3c
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r3, r5, #0
	adds r3, #85
	adds r2, r5, #0
	strb r7, [r3]
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
.L_02008a5a:
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #120
	movs r2, #190
	ldr r0, [r6]
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	movs r1, #192
	strb r3, [r0]
	lsls r1, r1, #6
	movs r0, #10
	movs r2, #0
	bl Actor_ArmCallback
	movs r2, #10
	movs r0, #10
	movs r1, #4
	bl Actor_Jump
	movs r0, #10
	movs r1, #0
	bl Func_02003f2c
	ldr r1, [r6]
	movs r0, #10
	bl Actor_WatchAndSetScript
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #132
	movs r2, #190
	ldr r0, [r6]
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r1, #195
	movs r3, #230
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #15
	adds r0, r5, #0
	bl Actor_SetPositionAndResetMotion
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #120
	movs r2, #190
	ldr r0, [r6]
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl FieldEvent_Wait
	ldr r0, [r6]
	movs r1, #98
	movs r2, #168
	bl Actor_SetPositionAndReset
	ldr r0, [r6]
	movs r1, #4
	movs r2, #10
	bl Actor_Jump
	movs r1, #208
	movs r2, #192
	movs r3, #160
	lsls r2, r2, #13
	lsls r3, r3, #16
	lsls r1, r1, #15
	adds r0, r5, #0
	bl Actor_SetPositionAndResetMotion
	movs r0, #10
	bl FieldEvent_Wait
	bl Func_02003fc4
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	bl Func_02003f3c
	mov r0, r9
	adds r0, #2
	movs r1, #1
	bl FieldMessage_Show
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	bl Func_02003fbc
	movs r2, #10
	movs r1, #6
	movs r0, #10
	bl Actor_Jump
	mov r0, r9
	adds r0, #3
	bl Func_02003f24
	movs r1, #0
	movs r0, #10
	bl Func_02003f2c
	movs r0, #10
	bl Actor_Stop
	ldr r0, [r6]
	movs r1, #0
	bl Func_02003f3c
	bl Func_02003fc4
	ldr r0, [r6]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	beq .L_02008bba
	b .L_02008d04
.L_02008bba:
	ldr r0, [r6]
	bl Actor_GetById
	adds r5, r0, #0
	bl Func_02003fbc
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02003f54
	mov r0, r9
	adds r0, #4
	bl Func_02003f24
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #10
	movs r1, #0
	bl Func_02003f2c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r2, #164
	movs r0, #10
	movs r1, #128
	bl Actor_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #10
	bl Func_02003f3c
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_02008c98
	adds r2, #204
	movs r0, #10
	bl Actor_SetSpeedParameters
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #108
	movs r2, #168
	movs r0, #10
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r3, #192
	ldr r2, [r5, #12]
	lsls r3, r3, #13
	movs r0, #234
	ldr r1, [r5, #8]
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r5, #16]
	bl Func_02003df4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008c9c
	ldr r0, [r6]
	movs r1, #28
	bl Actor_SetModeById
	ldr r3, .L_02008c8c
	adds r2, r5, #0
	adds r2, #85
	movs r1, #227
	adds r0, r5, #0
	strb r3, [r2]
	lsls r1, r1, #1
	bl Func_02003e3c
	movs r0, #20
	bl FieldEvent_Wait
	b .L_02008c9c
.L_02008c8c:
	.4byte 0x00000000
.L_02008c90:
	.4byte MsgOhThatRed
.L_02008c94:
	.4byte gMainRoutineWork
.L_02008c98:
	.4byte 0x00019999
.L_02008c9c:
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	movs r1, #116
	strb r3, [r0]
	movs r2, #164
	movs r0, #10
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	bl Func_02003fc4
	movs r0, #198
	adds r0, #255
	bl PartyInventory_Remove
	movs r0, #227
	lsls r0, r0, #1
	movs r1, #0
	bl PartyInventory_GiveItem
	cmp r5, #0
	beq .L_02008cf4
	adds r0, r5, #0
	bl Func_02003dfc
	ldr r0, [r6]
.L_02008cee:
	movs r1, #1
	bl Actor_SetModeById
.L_02008cf4:
	bl Func_02003fbc
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #163
	bl GameFlag_SetBit
	b .L_02008d80
.L_02008d04:
	bl Func_02003fbc
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #10
	bl Func_02003f54
	mov r0, r9
	adds r0, #7
	bl Func_02003f24
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #10
	movs r1, #0
	bl Func_02003f2c
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02003f3c
	ldr r0, [r6]
	movs r1, #4
	movs r2, #10
	bl Actor_Jump
	cmp r5, #0
	beq .L_02008d46
	adds r0, r5, #0
	bl Func_02003dfc
.L_02008d46:
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02008d80
.L_02008d5c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #163
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008d72
	ldr r0, .L_02008d98
	bl Func_02003f24
	b .L_02008d78
.L_02008d72:
	ldr r0, .L_02008d9c
	bl Func_02003f24
.L_02008d78:
	movs r0, #10
	movs r1, #0
	bl Func_02003f2c
.L_02008d80:
	movs r0, #10
	movs r1, #4
	bl Actor_SetModeById
	bl Func_02003e6c
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d98:
	.4byte MsgYupANice
.L_02008d9c:
	.4byte MsgIVeBeenFeeling
	.section .text.x0200b164,"ax",%progbits
	.global Func_02003164
	.thumb_func
Func_02003164:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #10
	ldr r5, [r3, #32]
	sub sp, #8
	bl Actor_GetById
	movs r2, #200
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r6, #0
	adds r2, #4
	str r6, [r3]
	adds r3, r5, r2
	str r6, [r3]
	ldr r3, .L_0200b298
	adds r2, #78
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #11
	bne .L_0200b19c
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200b19c:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b1b6
	adds r3, r7, #0
	adds r3, #85
	strb r6, [r3]
	movs r0, #10
	movs r1, #1
	bl Actor_SetSpritePriority
.L_0200b1b6:
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl Func_02003edc
	movs r0, #12
	bl Func_02003f4c
	movs r0, #13
	bl Func_02003f4c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_Test
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200b216
	movs r0, #12
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #13
	bl Actor_GetById
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
	adds r0, #98
	movs r3, #1
	adds r5, #98
	strb r3, [r5]
	movs r2, #29
	strb r3, [r0]
	movs r3, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #7
	movs r1, #3
	movs r2, #1
	movs r3, #1
	bl Func_02003e24
	b .L_0200b24a
.L_0200b216:
	movs r0, #12
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #13
	bl Actor_GetById
	adds r3, r5, #0
	adds r6, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, r6, #0
	adds r3, #85
	strb r7, [r3]
	movs r2, #3
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r3, #255
	bl Func_020020ec
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	movs r2, #3
	movs r3, #255
	bl Func_020020ec
.L_0200b24a:
	movs r0, #14
	bl Func_02003f4c
	movs r0, #14
	bl Actor_GetById
	movs r3, #1
	adds r1, r0, #0
	adds r0, #98
	strb r3, [r0]
	adds r3, r1, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	movs r2, #1
	ldr r0, [r1, #8]
	movs r3, #255
	ldr r1, [r1, #16]
	bl Func_020020ec
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b294
	movs r3, #5
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #5
	movs r1, #2
	movs r2, #1
	movs r3, #2
	bl Func_02003e24
.L_0200b294:
	add sp, #8
	pop {r5, r6, r7, pc}
.L_0200b298:
	.4byte gMainRoutineWork
	.section .text.x0200b29c,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_0200329c
	.thumb_func
Func_0200329c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	ldrb r2, [r5, #23]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #23]
	ldr r3, .L_ja_0200b31c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_ja_0200b1f4
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
.L_ja_0200b1f4:
	bl Func_02003410
	movs r1, #12
	movs r2, #5
	movs r3, #4
	movs r0, #3
	bl PaletteQueue_Add
	movs r0, #4
	movs r1, #12
	movs r2, #5
	movs r3, #4
	bl PaletteQueue_Add
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #165
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200b252
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_ja_0200b252
	movs r1, #140
	movs r2, #234
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #10
	bl Func_02003edc
	movs r0, #10
	bl Actor_GetById
	movs r1, #148
	adds r0, #85
	movs r2, #226
	strb r5, [r0]
	lsls r1, r1, #17
	movs r0, #9
	lsls r2, r2, #17
	bl Func_02003edc
.L_ja_0200b252:
	movs r0, #9
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #11
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #12
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r1, #144
	ldr r0, .L_ja_0200b320
	lsls r1, r1, #3
	bl Task_Add
	ldr r3, .L_ja_0200b31c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #4
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_ja_0200b2d0
	movs r0, #140
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200b2d0
	movs r3, #128
	movs r1, #152
	movs r2, #192
	lsls r3, r3, #7
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02003ee4
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #16
	ldr r2, .L_ja_0200b324
	bl Func_02003edc
	movs r0, #1
	bl wait
.L_ja_0200b2d0:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200b30a
	ldr r3, .L_ja_0200b31c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_ja_0200b2f2
	bl Func_02002830
	b .L_ja_0200b316
.L_ja_0200b2f2:
	cmp r3, #4
	bne .L_ja_0200b2fc
	bl Func_020029c4
	b .L_ja_0200b316
.L_ja_0200b2fc:
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_020034d8
	b .L_ja_0200b316
.L_ja_0200b30a:
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_020034d8
.L_ja_0200b316:
	bl Func_02003e6c
	pop {r5, pc}
.L_ja_0200b31c:
	.4byte gMainRoutineWork
.L_ja_0200b320:
	.4byte Func_02001c4c
.L_ja_0200b324:
	.4byte 0x03090000
	.endif
	.section .text.x0200b684,"ax",%progbits
	.balign 4
	.global Func_02003684
	.thumb_func
Func_02003684:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #8
	mov r8, r3
	bl Actor_GetById
	ldr r3, [r0, #80]
	ldr r5, .L_0200b788
	ldr r3, [r3, #40]
	movs r1, #0
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r4, r3, #16
	ldrh r3, [r5, r1]
	lsrs r2, r4, #16
	cmp r2, r3
	beq .L_0200b6ca
.L_0200b6b0:
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	lsrs r2, r3, #16
	asrs r1, r3, #16
	cmp r2, #5
	bhi .L_0200b6ca
	lsls r3, r2, #1
	ldrh r3, [r5, r3]
	lsrs r2, r4, #16
	cmp r2, r3
	bne .L_0200b6b0
.L_0200b6ca:
	lsls r3, r1, #16
	lsrs r2, r3, #16
	cmp r2, #6
	bne .L_0200b6d6
	movs r0, #0
	b .L_0200b77e
.L_0200b6d6:
	ldr r6, .L_0200b78c
	lsls r2, r2, #2
	ldrsb r4, [r6, r2]
	adds r1, r4, #0
	cmp r4, #0
	bge .L_0200b6e4
	negs r1, r4
.L_0200b6e4:
	adds r3, r2, #2
	ldrsb r3, [r6, r3]
	cmp r3, #0
	bge .L_0200b6ee
	negs r3, r3
.L_0200b6ee:
	adds r3, r1, r3
	asrs r7, r3, #4
	adds r3, r2, #1
	ldrsb r1, [r6, r3]
	adds r5, r1, #0
	cmp r1, #0
	bge .L_0200b6fe
	negs r5, r1
.L_0200b6fe:
	adds r3, r2, #3
	ldrsb r2, [r6, r3]
	cmp r2, #0
	bge .L_0200b708
	negs r2, r2
.L_0200b708:
	adds r5, r5, r2
	mov r10, r5
	ldr r6, [r0, #8]
	mov r3, r10
	ldr r5, [r0, #16]
	asrs r3, r3, #4
	mov r10, r3
	lsls r3, r4, #16
	adds r6, r6, r3
	lsls r3, r1, #16
	adds r5, r5, r3
	movs r3, #164
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	asrs r6, r6, #20
	asrs r1, r3, #20
	movs r3, #166
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	lsls r2, r1, #16
	asrs r3, r3, #20
	lsls r3, r3, #16
	asrs r5, r5, #20
	lsrs r2, r2, #16
	lsrs r3, r3, #16
	adds r2, r6, r2
	adds r3, r5, r3
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	mov r3, r10
	bl Func_02003e24
	movs r3, #255
	mov r2, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r8, r3
	movs r0, #0
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Func_02003790
	mov r2, r10
	mov r3, r8
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #2
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Func_02003790
	movs r0, #1
.L_0200b77e:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b788:
	.4byte gData_02004070
.L_0200b78c:
	.4byte gData_0200407c
	.section .text.x0200b7d8,"ax",%progbits
	.global Func_020037d8
	.thumb_func
Func_020037d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r5, r6, #0
	sub sp, #40
	adds r1, r6, #0
	adds r5, #12
	add r0, sp, #24
	adds r1, #16
	adds r2, r5, #0
	bl Func_02003940
	adds r4, r0, #0
	cmp r4, #0
	bne .L_0200b802
	b .L_0200b922
.L_0200b802:
	ldr r5, [r5]
	ldr r0, .L_0200b934
	str r5, [sp, #20]
	lsls r1, r5, #2
	ldrsb r2, [r0, r1]
	cmp r2, #0
	bge .L_0200b812
	negs r2, r2
.L_0200b812:
	adds r3, r1, #2
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_0200b81c
	negs r3, r3
.L_0200b81c:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #16]
	adds r3, r1, #1
	ldrsb r2, [r0, r3]
	cmp r2, #0
	bge .L_0200b82c
	negs r2, r2
.L_0200b82c:
	adds r3, r1, #3
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_0200b836
	negs r3, r3
.L_0200b836:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #12]
	ldr r3, [sp, #24]
	ldr r2, .L_0200b938
	ldr r1, .L_0200b93c
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	mov r9, r1
	mov r2, r9
	ands r2, r3
	lsls r3, r3, #16
	mov r10, r3
	movs r3, #0
	str r3, [r6, #20]
	mov r11, r3
	adds r3, r4, #0
	adds r3, #34
	str r3, [sp, #8]
	ldr r1, [sp, #8]
	movs r3, #2
	strb r3, [r1]
	mov r9, r2
	ldr r3, [r4, #8]
	add r3, r9
	str r3, [r6]
	ldr r3, [r4, #16]
	add r3, r10
	str r3, [r6, #8]
	ldr r3, [r4, #12]
	str r3, [sp, #32]
.L_0200b874:
	ldr r3, [sp, #20]
	ldr r2, .L_0200b934
	lsls r3, r3, #2
	str r3, [sp, #4]
	adds r3, #1
	ldrsb r2, [r2, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	ldr r2, [sp, #12]
	movs r1, #0
	mov r8, r1
	str r3, [sp, #36]
	cmp r8, r2
	bge .L_0200b8e2
.L_0200b892:
	ldr r3, .L_0200b934
	ldr r1, [sp, #4]
	add r5, sp, #28
	ldrsb r2, [r3, r1]
	ldr r3, [r6]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [sp, #16]
	movs r7, #0
	cmp r7, r2
	bge .L_0200b8cc
.L_0200b8aa:
	adds r0, r4, #0
	add r1, sp, #28
	str r4, [sp, #0]
	bl Func_02003e2c
	ldr r4, [sp, #0]
	cmp r0, #2
	beq .L_0200b8f4
	ldr r3, [r5]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r5]
	ldr r2, [sp, #16]
	adds r7, #1
	cmp r7, r2
	blt .L_0200b8aa
.L_0200b8cc:
	add r2, sp, #28
	ldr r3, [r2, #8]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r2, #8]
	ldr r3, [sp, #12]
	movs r2, #1
	add r8, r2
	cmp r8, r3
	blt .L_0200b892
.L_0200b8e2:
	ldr r3, [r6]
	movs r1, #1
	add r3, r9
	str r3, [r6]
	ldr r3, [r6, #8]
	add r11, r1
	add r3, r10
	str r3, [r6, #8]
	b .L_0200b874
.L_0200b8f4:
	ldr r2, [sp, #8]
	movs r3, #0
	strb r3, [r2]
	mov r3, r11
	movs r0, #0
	cmp r3, #0
	beq .L_0200b924
	mov r1, r9
	ldr r3, [r4, #8]
	mov r2, r11
	muls r2, r1
	adds r3, r3, r2
	str r3, [r6]
	movs r0, #1
	ldr r3, [r4, #12]
	str r3, [r6, #4]
	mov r3, r10
	mov r2, r11
	muls r2, r3
	ldr r3, [r4, #16]
	adds r3, r3, r2
	str r3, [r6, #8]
	b .L_0200b924
.L_0200b922:
	movs r0, #0
.L_0200b924:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b934:
	.4byte gData_0200407c
.L_0200b938:
	.4byte gData_02004094
.L_0200b93c:
	.4byte 0xffff0000
	.section .bss,"aw",%nobits
	.global gData_02004f04
gData_02004f04:
	.space 0x0000000c
	.ifdef TLA_EDITION_JA
	.space 0x00000004 @ the next object sits on 16 bytes, and the Japanese image ends 4 short of that
	.endif
	.global gData_02004f10
gData_02004f10:
	.space 0x00000280
	.global gData_02005190
gData_02005190:
	.space 0x00000004
	.global gData_02005194
gData_02005194:
	.space 0x00000004
	.global gData_02005198
gData_02005198:
	.space 0x00000004
	.global gData_0200519c
gData_0200519c:
