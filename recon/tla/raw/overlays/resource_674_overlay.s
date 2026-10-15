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
	bl Motion_CamBounds
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
	bl Motion_CamBounds
	ldr r3, .L_0200896c
	movs r0, #133
	lsls r0, r0, #2
	adds r7, r3, r0
	ldr r1, [r7]
	movs r0, #10
	movs r2, #0
	bl ObjectMotion_SetAngleToward
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
	bl Inventory_PromptAndSetObjectMode
	mov r9, r0
	cmp r0, #0
	beq .L_02008794
	b .L_02008944
.L_02008794:
	ldr r0, [r7]
	bl Object_GetById
	ldr r1, [r7]
	mov r8, r0
	movs r0, #10
	bl Object_LinkObjectAndSetCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r7]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #168
	ldr r0, [r7]
	movs r1, #98
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r7]
	lsls r1, r1, #8
	bl Func_02003f3c
	ldr r0, [r7]
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
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
	bl ObjectDispatch_SetSingleChildFlags
	adds r3, r5, #0
	adds r3, #85
	adds r2, r5, #0
	strb r6, [r3]
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
.L_0200880e:
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02003fbc
	movs r0, #10
	bl ObjectMotion_EnableActionAndResetMotion
	ldr r0, [r7]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
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
	bl ObjectMotion_SetSpeedParameters
	movs r2, #164
	movs r0, #10
	movs r1, #128
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #10
	bl Func_02003f3c
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_02008974
	adds r2, #204
	movs r0, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	mov r10, r3
	ands r3, r2
	strb r3, [r0]
	movs r1, #108
	movs r2, #168
	movs r0, #10
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
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
	bl Object_SetModeById
	adds r3, r6, #0
	adds r3, #85
	mov r0, r9
	movs r1, #227
	strb r0, [r3]
	lsls r1, r1, #1
	adds r0, r6, #0
	bl Func_02003e3c
	movs r0, #20
	bl Battle_WaitMode0
.L_020088f0:
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	movs r1, #116
	strb r3, [r0]
	movs r2, #164
	movs r0, #10
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
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
	.4byte gPartyState
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
	bl Motion_CamBounds
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
	bl Motion_CamBounds
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
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
	bl ObjectMotion_SetSpeedParameters
	movs r2, #190
	ldr r0, [r6]
	movs r1, #120
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	ldr r0, [r6]
	bl Func_02003f3c
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	mov r10, r3
	ands r3, r2
	strb r3, [r0]
	movs r1, #132
	movs r2, #190
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
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
	bl ObjectDispatch_SetSingleChildFlags
	adds r3, r5, #0
	adds r3, #85
	adds r2, r5, #0
	strb r7, [r3]
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
.L_02008a5a:
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #120
	movs r2, #190
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	movs r1, #192
	strb r3, [r0]
	lsls r1, r1, #6
	movs r0, #10
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #10
	movs r0, #10
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #0
	bl Func_02003f2c
	ldr r1, [r6]
	movs r0, #10
	bl Object_LinkObjectAndSetCallback
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #132
	movs r2, #190
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
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
	bl Object_SetPositionAndResetMotion
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #120
	movs r2, #190
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, [r6]
	movs r1, #98
	movs r2, #168
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r6]
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	movs r1, #208
	movs r2, #192
	movs r3, #160
	lsls r2, r2, #13
	lsls r3, r3, #16
	lsls r1, r1, #15
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02003fc4
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	bl Func_02003f3c
	mov r0, r9
	adds r0, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
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
	bl ObjectMotion_Launch
	mov r0, r9
	adds r0, #3
	bl Func_02003f24
	movs r1, #0
	movs r0, #10
	bl Func_02003f2c
	movs r0, #10
	bl ObjectMotion_EnableActionAndResetMotion
	ldr r0, [r6]
	movs r1, #0
	bl Func_02003f3c
	bl Func_02003fc4
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_02008bba
	b .L_02008d04
.L_02008bba:
	ldr r0, [r6]
	bl Object_GetById
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
	bl ObjectMotion_SetSpeedParameters
	movs r2, #164
	movs r0, #10
	movs r1, #128
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #10
	bl Func_02003f3c
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_02008c98
	adds r2, #204
	movs r0, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #108
	movs r2, #168
	movs r0, #10
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
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
	bl Object_SetModeById
	ldr r3, .L_02008c8c
	adds r2, r5, #0
	adds r2, #85
	movs r1, #227
	adds r0, r5, #0
	strb r3, [r2]
	lsls r1, r1, #1
	bl Func_02003e3c
	movs r0, #20
	bl Battle_WaitMode0
	b .L_02008c9c
.L_02008c8c:
	.4byte 0x00000000
.L_02008c90:
	.4byte MsgOhThatRed
.L_02008c94:
	.4byte gPartyState
.L_02008c98:
	.4byte 0x00019999
.L_02008c9c:
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	movs r1, #116
	strb r3, [r0]
	movs r2, #164
	movs r0, #10
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
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
	bl Object_SetModeById
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
	bl ObjectMotion_Launch
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
	bl Object_SetModeById
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
	.section .text.x02008e38,"ax",%progbits
	.global Func_02000e38
	.thumb_func
Func_02000e38:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #227
	lsls r0, r0, #1
	bl PartyInventory_FindOwner
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_02008e64
	b .L_020091f2
.L_02008e64:
	ldr r3, .L_02009230
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #11
	bl Object_GetById
	movs r3, #0
	str r3, [sp, #0]
	mov r10, r0
	ldr r1, [r5]
	movs r0, #11
	movs r2, #0
	mov r9, r3
	bl ObjectMotion_SetAngleToward
	ldr r7, .L_02009234
	mov r1, r10
	str r7, [r1, #108]
	movs r0, #11
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	mov r2, r9
	mov r3, r10
	str r2, [r3, #108]
	ldr r0, .L_02009238
	bl Func_02003f24
	movs r1, #0
	movs r0, #11
	bl Func_02003f2c
	mov r1, r10
	ldr r2, [r6, #8]
	ldr r3, [r1, #8]
	cmp r2, r3
	ble .L_02008ebe
	movs r2, #1
	str r2, [sp, #0]
.L_02008ebe:
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_0200923c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #11
	ldr r1, .L_0200923c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r3, [sp, #0]
	cmp r3, #0
	beq .L_02008f16
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #148
	movs r2, #214
	lsls r2, r2, #2
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r1, #227
	lsls r1, r1, #1
	adds r0, r6, #0
	bl Func_02000da0
	mov r1, r10
	str r7, [r1, #108]
	mov r8, r0
	ldr r1, .L_02009240
	b .L_02008f4a
.L_02008f16:
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #132
	movs r2, #214
	lsls r2, r2, #2
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #0
	bl Func_02003f3c
	movs r1, #227
	lsls r1, r1, #1
	adds r0, r6, #0
	bl Func_02000da0
	mov r1, r10
	str r7, [r1, #108]
	mov r8, r0
	ldr r1, .L_02009244
.L_02008f4a:
	movs r0, #11
	bl Object_SetActionCallbackAndRefreshById
	mov r2, r9
	mov r3, r10
	str r2, [r3, #108]
	movs r0, #11
	movs r1, #0
	bl Func_02003f2c
	ldr r3, .L_02009230
	movs r1, #133
	lsls r1, r1, #2
	adds r7, r3, r1
	ldr r1, [r7]
	movs r0, #11
	bl Object_LinkObjectAndSetCallback
	mov r2, r8
	cmp r2, #0
	bne .L_02008f76
	b .L_02009142
.L_02008f76:
	adds r2, #89
	movs r3, #9
	strb r3, [r2]
	movs r1, #208
	movs r3, #210
	lsls r1, r1, #15
	lsls r3, r3, #18
	mov r0, r8
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_02009234
	ldr r1, [sp, #0]
	mov r11, r3
	cmp r1, #0
	beq .L_0200905a
	movs r1, #148
	movs r2, #210
	str r3, [r6, #108]
	ldr r0, [r7]
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	movs r1, #128
	str r2, [r6, #108]
	lsls r1, r1, #8
	ldr r0, [r7]
	bl Func_02003f3c
	mov r3, r11
	str r3, [r6, #108]
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #144
	movs r2, #210
	lsls r1, r1, #1
	lsls r2, r2, #2
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #1
	mov r9, r1
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
	movs r1, #140
	movs r3, #210
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #18
	mov r0, r8
	bl Object_SetPositionAndResetMotion
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #148
	movs r2, #210
	lsls r1, r1, #1
	lsls r2, r2, #2
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r1, r9
	orrs r3, r1
	strb r3, [r0]
	movs r2, #0
	str r2, [r6, #108]
	ldr r0, [r7]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	mov r3, r11
	mov r1, r10
	str r3, [r1, #108]
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #146
	b .L_0200911c
.L_0200905a:
	mov r3, r11
	movs r1, #132
	movs r2, #210
	str r3, [r6, #108]
	lsls r2, r2, #2
	ldr r0, [r7]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r1, [sp, #0]
	ldr r0, [r7]
	str r1, [r6, #108]
	movs r1, #0
	bl Func_02003f3c
	mov r2, r11
	str r2, [r6, #108]
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #136
	movs r2, #210
	lsls r1, r1, #1
	lsls r2, r2, #2
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #1
	mov r9, r1
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
	movs r1, #140
	movs r3, #210
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #18
	mov r0, r8
	bl Object_SetPositionAndResetMotion
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #132
	movs r2, #210
	lsls r1, r1, #1
	lsls r2, r2, #2
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r1, r9
	orrs r3, r1
	strb r3, [r0]
	ldr r2, [sp, #0]
	ldr r0, [r7]
	str r2, [r6, #108]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	mov r3, r11
	mov r1, r10
	str r3, [r1, #108]
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #134
.L_0200911c:
	ands r5, r3
	movs r2, #211
	lsls r2, r2, #2
	lsls r1, r1, #1
	strb r5, [r0]
	movs r0, #11
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
.L_02009142:
	movs r7, #200
	adds r7, #255
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02000da0
	mov r9, r0
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #140
	movs r2, #214
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #11
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r5, #0
	mov r3, r10
	str r5, [r3, #108]
	bl Func_02003fc4
	movs r1, #1
	ldr r0, .L_02009248
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #227
	lsls r0, r0, #1
	bl PartyInventory_Remove
	movs r1, #0
	adds r0, r7, #0
	bl PartyInventory_GiveItem
	bl Func_02003fbc
	ldr r3, .L_02009230
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r3, r1
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #7
	mov r2, r9
	strh r3, [r0, #6]
	cmp r2, #0
	beq .L_020091d2
	mov r0, r9
	bl Func_02003dfc
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
.L_020091d2:
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #161
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #22
	bl GameFlag_SetBit
	b .L_02009262
.L_020091f2:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #161
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009254
	ldr r3, .L_02009230
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r0, #11
	ldr r1, [r3]
	bl Object_LinkObjectAndSetCallback
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #22
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009226
	ldr r0, .L_0200924c
	bl Func_02003f24
	b .L_0200925a
.L_02009226:
	ldr r0, .L_02009250
	bl Func_02003f24
	b .L_0200925a
	.2byte 0x0000
.L_02009230:
	.4byte gPartyState
.L_02009234:
	.4byte Func_02000df8
.L_02009238:
	.4byte MsgSniffSniffI
.L_0200923c:
	.4byte 0x00019999
.L_02009240:
	.4byte Data_020043f8
.L_02009244:
	.4byte Data_02004300
.L_02009248:
	.4byte MsgTradedHisMilk
.L_0200924c:
	.4byte MsgIDonTLike
.L_02009250:
	.4byte MsgIDonTWantTo
.L_02009254:
	ldr r0, .L_02009274
	bl Func_02003f24
.L_0200925a:
	movs r0, #11
	movs r1, #0
	bl Func_02003f2c
.L_02009262:
	bl Func_02003e6c
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009274:
	.4byte MsgTheWaterHere
	.section .text.x0200929c,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_0200129c
	.thumb_func
Func_0200129c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_ja_020094c4
	adds r5, r0, #0
	movs r0, #133
	lsls r0, r0, #2
	adds r6, r6, r0
	ldr r0, [r6]
	sub sp, #36
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #9
	bl Object_GetById
	ldr r3, .L_ja_020094c8
	mov r8, sp
	mov r2, r8
	mov r11, r0
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	bl Func_02003fbc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #9
	bl Func_02003f54
	ldr r1, .L_ja_020094cc
	mov r2, r11
	str r1, [r2, #108]
	movs r0, #9
	mov r1, r8
	bl Object_SetActionCallbackAndRefreshById
	mov r4, r11
	movs r3, #0
	str r3, [r4, #108]
	ldr r0, .L_ja_020094d0
	mov r8, r3
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_ja_020094d4
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, .L_ja_020094cc
	mov r1, r11
	str r0, [r1, #108]
	movs r2, #226
	movs r1, #148
	movs r0, #9
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	mov r2, r8
	mov r3, r11
	movs r1, #160
	str r2, [r3, #108]
	movs r0, #9
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	ldr r1, .L_ja_020094cc
	movs r0, #204
	ldr r4, .L_ja_020094d4
	lsls r0, r0, #8
	adds r0, #204
	mov r10, r0
	str r0, [r5, #52]
	str r1, [r5, #108]
	adds r0, r5, #0
	movs r1, #2
	mov r9, r4
	str r4, [r5, #48]
	bl Func_02003de4
	movs r1, #139
	movs r3, #226
	lsls r3, r3, #17
	movs r2, #0
	adds r0, r5, #0
	lsls r1, r1, #17
	bl Func_02003e0c
	adds r0, r5, #0
	bl Func_02003e14
	adds r0, r5, #0
	movs r1, #1
	bl Func_02003de4
	movs r0, #200
	mov r2, r8
	adds r0, #255
	str r2, [r5, #108]
	bl PartyInventory_Remove
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #166
	lsls r1, r1, #2
	movs r0, #88
	bl Runtime_AllocateBlock
	movs r3, #203
	lsls r3, r3, #1
	adds r3, #255
	adds r0, r0, r3
	movs r3, #99
	strb r3, [r0]
	movs r1, #3
	ldr r0, [r6]
	bl Motion_SetModeAndWaitAnimation
	ldr r0, [r6]
	mov r1, r9
	mov r2, r10
	bl ObjectMotion_SetSpeedParameters
	ldr r4, .L_ja_020094cc
	movs r1, #156
	movs r2, #223
	str r4, [r7, #108]
	ldr r0, [r6]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	mov r0, r8
	str r0, [r7, #108]
	movs r1, #6
	ldr r0, [r6]
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #148
	movs r2, #226
	lsls r2, r2, #1
	ldr r0, [r6]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	ldr r3, .L_ja_020094d8
	ldr r0, [r6]
	str r3, [r7, #108]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r2, #204
	lsls r2, r2, #7
	mov r1, r10
	movs r0, #9
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r3, #204
	ldr r2, .L_ja_020094cc
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r5, #52]
	mov r3, r11
	str r2, [r3, #108]
	mov r1, r10
	movs r2, #128
	str r1, [r5, #48]
	lsls r2, r2, #2
	movs r1, #144
	adds r2, #26
	lsls r1, r1, #1
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02003fc4
	bl Func_020034ec
	ldr r4, .L_ja_020094cc
	adds r0, r5, #0
	movs r1, #2
	str r4, [r5, #108]
	bl Func_02003de4
	movs r1, #144
	lsls r1, r1, #17
	adds r0, r5, #0
	movs r2, #0
	ldr r3, .L_ja_020094dc
	bl Func_02003e0c
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	movs r2, #133
	lsls r0, r0, #1
	lsls r2, r2, #1
	adds r3, r3, r0
	adds r2, #255
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #3
	bl Func_02003f7c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #165
	bl GameFlag_SetBit
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_ja_020094c4:
	.4byte gPartyState
.L_ja_020094c8:
	.4byte Data_02004004
.L_ja_020094cc:
	.4byte Func_02000df8
.L_ja_020094d0:
	.4byte MsgAtLongLast
.L_ja_020094d4:
	.4byte 0x00019999
.L_ja_020094d8:
	.4byte Func_02001278
.L_ja_020094dc:
	.4byte 0x021a0000
	.else
	.global Func_0200129c
	.thumb_func
Func_0200129c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_020094c4
	adds r5, r0, #0
	movs r0, #133
	lsls r0, r0, #2
	adds r6, r6, r0
	ldr r0, [r6]
	sub sp, #36
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #9
	bl Object_GetById
	ldr r3, .L_020094c8
	mov r8, sp
	mov r2, r8
	mov r11, r0
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	bl Func_02003fbc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #9
	bl Func_02003f54
	ldr r1, .L_020094cc
	mov r2, r11
	str r1, [r2, #108]
	movs r0, #9
	mov r1, r8
	bl Object_SetActionCallbackAndRefreshById
	mov r4, r11
	movs r3, #0
	str r3, [r4, #108]
	ldr r0, .L_020094d0
	mov r8, r3
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_020094d4
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, .L_020094cc
	mov r1, r11
	str r0, [r1, #108]
	movs r2, #226
	movs r1, #148
	movs r0, #9
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	mov r2, r8
	mov r3, r11
	movs r1, #160
	str r2, [r3, #108]
	movs r0, #9
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	ldr r1, .L_020094cc
	movs r0, #204
	ldr r4, .L_020094d4
	lsls r0, r0, #8
	adds r0, #204
	mov r10, r0
	str r0, [r5, #52]
	str r1, [r5, #108]
	adds r0, r5, #0
	movs r1, #2
	mov r9, r4
	str r4, [r5, #48]
	bl Func_02003de4
	movs r1, #139
	movs r3, #226
	lsls r3, r3, #17
	movs r2, #0
	adds r0, r5, #0
	lsls r1, r1, #17
	bl Func_02003e0c
	adds r0, r5, #0
	bl Func_02003e14
	adds r0, r5, #0
	movs r1, #1
	bl Func_02003de4
	movs r0, #200
	mov r2, r8
	adds r0, #255
	str r2, [r5, #108]
	bl PartyInventory_Remove
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #166
	lsls r1, r1, #2
	movs r0, #88
	bl Runtime_AllocateBlock
	movs r3, #203
	lsls r3, r3, #1
	adds r3, #255
	adds r0, r0, r3
	movs r3, #99
	strb r3, [r0]
	movs r1, #3
	ldr r0, [r6]
	bl Motion_SetModeAndWaitAnimation
	ldr r0, [r6]
	mov r1, r9
	mov r2, r10
	bl ObjectMotion_SetSpeedParameters
	ldr r4, .L_020094cc
	movs r1, #156
	movs r2, #223
	str r4, [r7, #108]
	ldr r0, [r6]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	mov r0, r8
	str r0, [r7, #108]
	movs r1, #6
	ldr r0, [r6]
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #148
	movs r2, #226
	lsls r2, r2, #1
	ldr r0, [r6]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	ldr r3, .L_020094d8
	ldr r0, [r6]
	str r3, [r7, #108]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r2, #204
	lsls r2, r2, #7
	mov r1, r10
	movs r0, #9
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r3, #204
	ldr r2, .L_020094cc
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r5, #52]
	mov r3, r11
	str r2, [r3, #108]
	mov r1, r10
	movs r2, #128
	str r1, [r5, #48]
	lsls r2, r2, #2
	movs r1, #144
	adds r2, #26
	lsls r1, r1, #1
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02003fc4
	bl Func_020034ec
	ldr r4, .L_020094cc
	adds r0, r5, #0
	movs r1, #2
	str r4, [r5, #108]
	bl Func_02003de4
	movs r1, #144
	lsls r1, r1, #17
	adds r0, r5, #0
	movs r2, #0
	ldr r3, .L_020094dc
	bl Func_02003e0c
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	movs r2, #129
	lsls r0, r0, #1
	lsls r2, r2, #1
	adds r3, r3, r0
	adds r2, #255
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #3
	bl Func_02003f7c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #165
	bl GameFlag_SetBit
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020094c4:
	.4byte gPartyState
.L_020094c8:
	.4byte Data_02004004
.L_020094cc:
	.4byte Func_02000df8
.L_020094d0:
	.4byte MsgAtLongLast
.L_020094d4:
	.4byte 0x00019999
.L_020094d8:
	.4byte Func_02001278
.L_020094dc:
	.4byte 0x021a0000
	.endif
	.section .text.x020094e0,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_020014e0
	.thumb_func
Func_020014e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #200
	adds r0, #255
	bl PartyInventory_FindOwner
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_ja_0200950a
	b .L_ja_020098ac
.L_ja_0200950a:
	ldr r3, .L_ja_02009894
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Object_GetById
	mov r8, r0
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	mov r10, r0
	cmp r0, #0
	bne .L_ja_0200952c
	b .L_ja_02009694
.L_ja_0200952c:
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	ldr r7, .L_ja_02009898
	mov r3, r8
	movs r1, #159
	movs r2, #212
	str r7, [r3, #108]
	ldr r0, [r6]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r3, #0
	mov r2, r8
	movs r1, #128
	str r3, [r2, #108]
	ldr r0, [r6]
	movs r2, #0
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02003f3c
	ldr r5, .L_ja_0200989c
	adds r0, r5, #0
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	mov r10, r0
	cmp r0, #0
	bne .L_ja_02009688
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_ja_020098a0
	adds r2, #204
	ldr r0, [r6]
	bl ObjectMotion_SetSpeedParameters
	mov r3, r8
	str r7, [r3, #108]
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	mov r11, r3
	ands r3, r2
	strb r3, [r0]
	movs r1, #151
	movs r2, #212
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r9, r2
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
	movs r1, #148
	movs r3, #212
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #17
	movs r0, #163
	bl Func_02003df4
	movs r1, #148
	movs r3, #212
	adds r7, r0, #0
	lsls r1, r1, #17
	movs r0, #14
	movs r2, #0
	lsls r3, r3, #17
	bl Func_02003df4
	adds r5, r0, #0
	cmp r7, #0
	beq .L_ja_0200961e
	adds r3, r7, #0
	adds r3, #85
	mov r2, r10
	strb r2, [r3]
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
.L_ja_0200961e:
	cmp r5, #0
	beq .L_ja_02009640
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #28]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Func_02003de4
	ldr r1, .L_ja_020098a4
	adds r0, r5, #0
	bl Func_02003dec
.L_ja_02009640:
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r11
	ands r3, r2
	strb r3, [r0]
	movs r1, #159
	movs r2, #212
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
	mov r2, r8
	mov r3, r10
	movs r1, #192
	str r3, [r2, #108]
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_ja_0200982c
.L_ja_02009688:
	bl Func_02003fbc
	subs r0, r5, #1
	bl Func_02003f24
	b .L_ja_02009a1c
.L_ja_02009694:
	ldr r0, .L_ja_020098a8
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #20
	ldr r0, [r6]
	bl Func_02003f54
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	ldr r3, .L_ja_02009898
	mov r2, r8
	str r3, [r2, #108]
	movs r1, #159
	movs r2, #212
	lsls r2, r2, #1
	ldr r0, [r6]
	lsls r1, r1, #1
	mov r9, r3
	movs r7, #0
	bl ObjectMotion_SetPositionAndReset
	mov r3, r8
	movs r1, #128
	str r7, [r3, #108]
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_ja_020098a0
	ldr r0, [r6]
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	mov r2, r9
	mov r3, r8
	str r2, [r3, #108]
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #151
	movs r2, #212
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r11, r2
	mov r2, r11
	orrs r3, r2
	strb r3, [r0]
	movs r1, #148
	movs r3, #212
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #17
	movs r0, #163
	bl Func_02003df4
	movs r1, #148
	movs r3, #212
	adds r7, r0, #0
	lsls r1, r1, #17
	movs r0, #14
	movs r2, #0
	lsls r3, r3, #17
	bl Func_02003df4
	adds r5, r0, #0
	cmp r7, #0
	beq .L_ja_0200976a
	adds r3, r7, #0
	adds r3, #85
	mov r2, r10
	strb r2, [r3]
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
.L_ja_0200976a:
	cmp r5, #0
	beq .L_ja_0200978c
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #28]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Func_02003de4
	ldr r1, .L_ja_020098a4
	adds r0, r5, #0
	bl Func_02003dec
.L_ja_0200978c:
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #159
	movs r2, #212
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r11
	orrs r3, r2
	strb r3, [r0]
	mov r2, r8
	mov r3, r10
	movs r1, #128
	str r3, [r2, #108]
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02003f54
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02003f3c
	movs r0, #9
	bl Object_GetById
	mov r3, r9
	adds r5, r0, #0
	str r3, [r5, #108]
	movs r0, #9
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	mov r2, r10
	str r2, [r5, #108]
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_ja_02009834
.L_ja_0200982c:
	adds r0, r7, #0
	bl Func_0200129c
	b .L_ja_02009a3e
.L_ja_02009834:
	bl Func_02003fbc
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #9
	bl Func_02003f54
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #160
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02003f3c
	mov r3, r9
	mov r2, r8
	str r3, [r2, #108]
	movs r1, #151
	movs r2, #212
	lsls r2, r2, #1
	ldr r0, [r6]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	mov r3, r10
	mov r2, r8
	str r3, [r2, #108]
	cmp r7, #0
	beq .L_ja_02009886
	adds r0, r7, #0
	bl Func_02003dfc
.L_ja_02009886:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_ja_02009a3e
	.2byte 0x0000
.L_ja_02009894:
	.4byte gPartyState
.L_ja_02009898:
	.4byte Func_02000df8
.L_ja_0200989c:
	.4byte MsgOhYouRe
.L_ja_020098a0:
	.4byte 0x00019999
.L_ja_020098a4:
	.4byte Data_020044f0
.L_ja_020098a8:
	.4byte MsgIMSoVery
.L_ja_020098ac:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #165
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_020098bc
	b .L_ja_02009a30
.L_ja_020098bc:
	ldr r3, .L_ja_02009a50
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r1, [r7]
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_SetAngleToward
	ldr r0, .L_ja_02009a54
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
	ldr r0, [r7]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_ja_020098ee
	b .L_ja_02009a18
.L_ja_020098ee:
	ldr r0, [r7]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003fbc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r5, #226
	lsls r5, r5, #1
	adds r2, r2, r5
	mov r8, r3
	ldrh r3, [r2]
	movs r0, #9
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02003f2c
	movs r1, #166
	lsls r1, r1, #2
	movs r0, #88
	bl Runtime_AllocateBlock
	movs r2, #203
	lsls r2, r2, #1
	adds r2, #255
	movs r3, #99
	adds r0, r0, r2
	strb r3, [r0]
	movs r1, #3
	ldr r0, [r7]
	bl Motion_SetModeAndWaitAnimation
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r7]
	ldr r1, .L_ja_02009a58
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r7]
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #148
	adds r2, r5, #0
	lsls r1, r1, #1
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndCommit
	ldr r3, .L_ja_02009a5c
	ldr r0, [r7]
	str r3, [r6, #108]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	movs r0, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	bl Object_GetById
	ldr r5, .L_ja_02009a60
	movs r2, #128
	movs r1, #144
	lsls r2, r2, #2
	lsls r1, r1, #1
	adds r2, #26
	str r5, [r0, #108]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02003fc4
	bl Func_020034ec
	movs r0, #10
	bl Object_GetById
	movs r2, #128
	movs r1, #144
	lsls r2, r2, #2
	lsls r1, r1, #1
	adds r2, #26
	str r5, [r0, #108]
	movs r0, #10
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl Battle_WaitMode0
	mov r2, r8
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02003fbc
	movs r0, #3
	bl Func_02003f7c
	b .L_ja_02009a3e
.L_ja_02009a18:
	bl Func_02003fbc
.L_ja_02009a1c:
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	b .L_ja_02009a3e
.L_ja_02009a30:
	ldr r0, .L_ja_02009a64
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
.L_ja_02009a3e:
	bl Func_02003e6c
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_02009a50:
	.4byte gPartyState
.L_ja_02009a54:
	.4byte MsgWantToGo
.L_ja_02009a58:
	.4byte 0x00019999
.L_ja_02009a5c:
	.4byte Func_02001278
.L_ja_02009a60:
	.4byte Func_02000df8
.L_ja_02009a64:
	.4byte MsgIMSoVeryVery
	.else
	.global Func_020014e0
	.thumb_func
Func_020014e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #200
	adds r0, #255
	bl PartyInventory_FindOwner
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_0200950a
	b .L_020098ac
.L_0200950a:
	ldr r3, .L_02009894
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Object_GetById
	mov r8, r0
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	mov r10, r0
	cmp r0, #0
	bne .L_0200952c
	b .L_02009694
.L_0200952c:
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	ldr r7, .L_02009898
	mov r3, r8
	movs r1, #159
	movs r2, #212
	str r7, [r3, #108]
	ldr r0, [r6]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r3, #0
	mov r2, r8
	movs r1, #128
	str r3, [r2, #108]
	ldr r0, [r6]
	movs r2, #0
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02003f3c
	ldr r5, .L_0200989c
	adds r0, r5, #0
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	mov r10, r0
	cmp r0, #0
	bne .L_02009688
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_020098a0
	adds r2, #204
	ldr r0, [r6]
	bl ObjectMotion_SetSpeedParameters
	mov r3, r8
	str r7, [r3, #108]
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	mov r11, r3
	ands r3, r2
	strb r3, [r0]
	movs r1, #151
	movs r2, #212
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r9, r2
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
	movs r1, #148
	movs r3, #212
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #17
	movs r0, #163
	bl Func_02003df4
	movs r1, #148
	movs r3, #212
	adds r7, r0, #0
	lsls r1, r1, #17
	movs r0, #14
	movs r2, #0
	lsls r3, r3, #17
	bl Func_02003df4
	adds r5, r0, #0
	cmp r7, #0
	beq .L_0200961e
	adds r3, r7, #0
	adds r3, #85
	mov r2, r10
	strb r2, [r3]
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
.L_0200961e:
	cmp r5, #0
	beq .L_02009640
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #28]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Func_02003de4
	ldr r1, .L_020098a4
	adds r0, r5, #0
	bl Func_02003dec
.L_02009640:
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r11
	ands r3, r2
	strb r3, [r0]
	movs r1, #159
	movs r2, #212
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
	mov r2, r8
	mov r3, r10
	movs r1, #192
	str r3, [r2, #108]
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_0200982c
.L_02009688:
	bl Func_02003fbc
	subs r0, r5, #1
	bl Func_02003f24
	b .L_02009a1c
.L_02009694:
	ldr r0, .L_020098a8
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #20
	ldr r0, [r6]
	bl Func_02003f54
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	ldr r3, .L_02009898
	mov r2, r8
	str r3, [r2, #108]
	movs r1, #159
	movs r2, #212
	lsls r2, r2, #1
	ldr r0, [r6]
	lsls r1, r1, #1
	mov r9, r3
	movs r7, #0
	bl ObjectMotion_SetPositionAndReset
	mov r3, r8
	movs r1, #128
	str r7, [r3, #108]
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_020098a0
	ldr r0, [r6]
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	mov r2, r9
	mov r3, r8
	str r2, [r3, #108]
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #151
	movs r2, #212
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r11, r2
	mov r2, r11
	orrs r3, r2
	strb r3, [r0]
	movs r1, #148
	movs r3, #212
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #17
	movs r0, #163
	bl Func_02003df4
	movs r1, #148
	movs r3, #212
	adds r7, r0, #0
	lsls r1, r1, #17
	movs r0, #14
	movs r2, #0
	lsls r3, r3, #17
	bl Func_02003df4
	adds r5, r0, #0
	cmp r7, #0
	beq .L_0200976a
	adds r3, r7, #0
	adds r3, #85
	mov r2, r10
	strb r2, [r3]
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
.L_0200976a:
	cmp r5, #0
	beq .L_0200978c
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #28]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Func_02003de4
	ldr r1, .L_020098a4
	adds r0, r5, #0
	bl Func_02003dec
.L_0200978c:
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #159
	movs r2, #212
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r11
	orrs r3, r2
	strb r3, [r0]
	mov r2, r8
	mov r3, r10
	movs r1, #128
	str r3, [r2, #108]
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02003f54
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02003f3c
	movs r0, #9
	bl Object_GetById
	mov r3, r9
	adds r5, r0, #0
	str r3, [r5, #108]
	movs r0, #9
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	mov r2, r10
	str r2, [r5, #108]
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009834
.L_0200982c:
	adds r0, r7, #0
	bl Func_0200129c
	b .L_02009a3e
.L_02009834:
	bl Func_02003fbc
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #9
	bl Func_02003f54
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #160
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02003f3c
	mov r3, r9
	mov r2, r8
	str r3, [r2, #108]
	movs r1, #151
	movs r2, #212
	lsls r2, r2, #1
	ldr r0, [r6]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	mov r3, r10
	mov r2, r8
	str r3, [r2, #108]
	cmp r7, #0
	beq .L_02009886
	adds r0, r7, #0
	bl Func_02003dfc
.L_02009886:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02009a3e
	.2byte 0x0000
.L_02009894:
	.4byte gPartyState
.L_02009898:
	.4byte Func_02000df8
.L_0200989c:
	.4byte MsgOhYouRe
.L_020098a0:
	.4byte 0x00019999
.L_020098a4:
	.4byte Data_020044f0
.L_020098a8:
	.4byte MsgIMSoVery
.L_020098ac:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #165
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020098bc
	b .L_02009a30
.L_020098bc:
	ldr r3, .L_02009a50
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r1, [r7]
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_SetAngleToward
	ldr r0, .L_02009a54
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
	ldr r0, [r7]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_020098ee
	b .L_02009a18
.L_020098ee:
	ldr r0, [r7]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003fbc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r5, #226
	lsls r5, r5, #1
	adds r2, r2, r5
	mov r8, r3
	ldrh r3, [r2]
	movs r0, #9
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02003f2c
	movs r1, #166
	lsls r1, r1, #2
	movs r0, #88
	bl Runtime_AllocateBlock
	movs r2, #203
	lsls r2, r2, #1
	adds r2, #255
	movs r3, #99
	adds r0, r0, r2
	strb r3, [r0]
	movs r1, #3
	ldr r0, [r7]
	bl Motion_SetModeAndWaitAnimation
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r7]
	ldr r1, .L_02009a58
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r7]
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #148
	adds r2, r5, #0
	lsls r1, r1, #1
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndCommit
	ldr r3, .L_02009a5c
	ldr r0, [r7]
	str r3, [r6, #108]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	movs r0, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	bl Object_GetById
	ldr r5, .L_02009a60
	movs r2, #128
	movs r1, #144
	lsls r2, r2, #2
	lsls r1, r1, #1
	adds r2, #26
	str r5, [r0, #108]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02003fc4
	bl Func_020034ec
	movs r0, #10
	bl Object_GetById
	movs r2, #128
	movs r1, #144
	lsls r2, r2, #2
	lsls r1, r1, #1
	adds r2, #26
	str r5, [r0, #108]
	movs r0, #10
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl Battle_WaitMode0
	mov r2, r8
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02003fbc
	movs r0, #3
	bl Func_02003f7c
	b .L_02009a3e
.L_02009a18:
	bl Func_02003fbc
.L_02009a1c:
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	b .L_02009a3e
.L_02009a30:
	ldr r0, .L_02009a64
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
.L_02009a3e:
	bl Func_02003e6c
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009a50:
	.4byte gPartyState
.L_02009a54:
	.4byte MsgWantToGo
.L_02009a58:
	.4byte 0x00019999
.L_02009a5c:
	.4byte Func_02001278
.L_02009a60:
	.4byte Func_02000df8
.L_02009a64:
	.4byte MsgIMSoVeryVery
	.endif
	.section .text.x02009a68,"ax",%progbits
	.global Func_02001a68
	.thumb_func
Func_02001a68:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009bd8
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	mov r10, r0
	movs r0, #9
	bl Object_GetById
	mov r8, r0
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r2, #0
	ldr r1, [r5]
	movs r0, #9
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	ldr r6, .L_02009bdc
	adds r0, r6, #0
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	adds r7, r0, #0
	cmp r7, #0
	bne .L_02009bae
	bl Func_02003fbc
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #166
	lsls r1, r1, #2
	movs r0, #88
	bl Runtime_AllocateBlock
	movs r3, #203
	lsls r3, r3, #1
	adds r3, #255
	adds r0, r0, r3
	movs r3, #99
	strb r3, [r0]
	movs r1, #6
	ldr r0, [r5]
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02009be0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #192
	ldr r0, [r5]
	movs r1, #152
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndCommit
	ldr r3, .L_02009be4
	mov r2, r10
	str r3, [r2, #108]
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02003fc4
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02009be0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #10
	ldr r1, .L_02009be0
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_02009be8
	mov r6, r8
	mov r3, r8
	adds r6, #99
	ldr r1, .L_02009bec
	strb r7, [r6]
	movs r0, #9
	str r5, [r3, #108]
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	ldr r1, .L_02009bf0
	str r5, [r0, #108]
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
.L_02009b74:
	movs r0, #1
	bl WaitFrames
	ldrb r3, [r6]
	cmp r3, #0
	beq .L_02009b74
	bl Func_020034ec
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02003fbc
	movs r0, #4
	bl Func_02003f7c
	b .L_02009bca
.L_02009bae:
	bl Func_02003fbc
	adds r0, r6, #4
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
.L_02009bca:
	bl Func_02003e6c
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009bd8:
	.4byte gPartyState
.L_02009bdc:
	.4byte MsgYouAllSet
.L_02009be0:
	.4byte 0x00019999
.L_02009be4:
	.4byte Func_02001278
.L_02009be8:
	.4byte Func_02000df8
.L_02009bec:
	.4byte Data_020045a4
.L_02009bf0:
	.4byte Data_020045f0
	.section .text.x02009cfc,"ax",%progbits
	.global Func_02001cfc
	.thumb_func
Func_02001cfc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #188
	lsls r1, r1, #1
	adds r1, r1, r3
	ldr r3, .L_02009d84
	sub sp, #8
	ldrb r2, [r3]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #2
	adds r2, r2, r3
	ldr r3, .L_02009d88
	lsls r2, r2, #6
	adds r2, r2, r3
	add r7, sp, #4
	movs r3, #0
	str r3, [r7]
	mov r10, r1
	mov r8, r2
.L_02009d2e:
	ldr r3, .L_02009d8c
	ldr r0, [r7]
	ldrb r3, [r3]
	mov r1, r10
	adds r0, r0, r3
	movs r2, #6
	ldrsh r3, [r1, r2]
	adds r0, r0, r3
	lsls r0, r0, #9
	bl Math_Sine
	str r0, [sp, #0]
	mov r3, r10
	movs r2, #2
	ldrsh r5, [r3, r2]
	ldr r6, [r7]
	asrs r0, r0, #15
	adds r5, r5, r0
	movs r1, #3
	adds r0, r6, #0
	bl __modsi3
	adds r5, r5, r0
	mov r1, r8
	subs r5, #1
	movs r2, #2
	adds r6, #1
	strh r5, [r1]
	add r8, r2
	str r6, [r7]
	cmp r6, #160
	bne .L_02009d2e
	ldr r3, .L_02009d84
	movs r1, #1
	ldrb r2, [r3]
	add sp, #8
	eors r2, r1
	strb r2, [r3]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009d84:
	.4byte Data_02004f04
.L_02009d88:
	.4byte Data_02004f10
.L_02009d8c:
	.4byte gFrameCount
	.section .text.x02009db0,"ax",%progbits
	.global Func_02001db0
	.thumb_func
Func_02001db0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r0, #19
	bl Object_GetById
	ldr r2, .L_02009f4c
	ldr r3, [r0, #8]
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	cmp r3, r2
	bls .L_02009dce
	b .L_02009f44
.L_02009dce:
	ldr r0, [r0, #16]
	movs r3, #164
	lsls r3, r3, #16
	cmp r0, r3
	bge .L_02009dda
	b .L_02009f44
.L_02009dda:
	movs r2, #172
	lsls r2, r2, #16
	cmp r0, r2
	ble .L_02009de4
	b .L_02009f44
.L_02009de4:
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #19
	movs r1, #1
	bl Object_SetModeById
	ldr r3, .L_02009f50
	movs r2, #133
	mov r8, r3
	lsls r2, r2, #2
	add r8, r2
	mov r3, r8
	ldr r0, [r3]
	movs r1, #216
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	mov r2, r8
	ldr r0, [r2]
	movs r1, #9
	bl Func_02003fac
	ldr r0, .L_02009f54
	bl Func_02003f24
	movs r0, #9
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	mov r3, r8
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #10
	movs r0, #9
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #9
	bl Func_02003f2c
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r2, #0
	strb r3, [r0]
	movs r1, #222
	mov r10, r2
	movs r0, #9
	movs r2, #186
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #186
	ands r5, r3
	movs r1, #232
	strb r5, [r0]
	movs r0, #9
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #128
	orrs r6, r3
	strb r6, [r0]
	mov r3, r8
	ldr r0, [r3]
	lsls r1, r1, #7
	bl Func_02003f3c
	mov r2, r8
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #20
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #85
	mov r2, r10
	movs r1, #226
	strb r2, [r3]
	lsls r1, r1, #1
	bl Func_02003e3c
	ldr r2, [r6, #12]
	movs r3, #192
	lsls r3, r3, #13
	adds r2, r2, r3
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02003fc4
	movs r0, #226
	movs r1, #0
	lsls r0, r0, #1
	bl PartyInventory_GiveItem
	bl Func_02003fbc
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02003edc
	mov r2, r8
	ldr r0, [r2]
	movs r1, #1
	bl Object_SetModeById
	bl Func_02003fb4
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #162
	bl GameFlag_SetBit
	bl Func_02003e6c
.L_02009f44:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_02009f4c:
	.4byte 0xff1c0000
.L_02009f50:
	.4byte gPartyState
.L_02009f54:
	.4byte MsgThankYouSo
	.section .text.x0200a144,"ax",%progbits
	.global Func_02002144
	.thumb_func
Func_02002144:
	push {r5, r6, lr}
	movs r0, #12
	bl Object_GetById
	ldr r5, .L_0200a1c0
	ldr r3, [r0, #8]
	ldr r6, .L_0200a1c4
	str r3, [r5]
	ldr r3, [r0, #16]
	movs r0, #13
	str r3, [r6]
	bl Object_GetById
	ldr r3, .L_0200a1c8
	ldr r4, [r0, #8]
	str r4, [r3]
	ldr r3, .L_0200a1cc
	ldr r2, [r0, #16]
	str r2, [r3]
	ldr r0, [r5]
	asrs r3, r0, #20
	cmp r3, #7
	bne .L_0200a18e
	ldr r1, [r6]
	asrs r3, r1, #20
	cmp r3, #29
	bne .L_0200a18e
	asrs r3, r4, #20
	cmp r3, #7
	bne .L_0200a18e
	asrs r3, r2, #20
	cmp r3, #28
	bne .L_0200a18e
	movs r2, #2
	movs r3, #0
	bl Func_020020ec
.L_0200a18e:
	ldr r3, .L_0200a1c0
	ldr r3, [r3]
	asrs r3, r3, #20
	cmp r3, #12
	bne .L_0200a1be
	ldr r3, .L_0200a1c4
	ldr r3, [r3]
	asrs r3, r3, #20
	cmp r3, #25
	bne .L_0200a1be
	ldr r3, .L_0200a1c8
	ldr r0, [r3]
	asrs r3, r0, #20
	cmp r3, #12
	bne .L_0200a1be
	ldr r3, .L_0200a1cc
	ldr r1, [r3]
	asrs r3, r1, #20
	cmp r3, #26
	bne .L_0200a1be
	movs r2, #2
	movs r3, #0
	bl Func_020020ec
.L_0200a1be:
	pop {r5, r6, pc}
.L_0200a1c0:
	.4byte Data_0200519c
.L_0200a1c4:
	.4byte Data_02005194
.L_0200a1c8:
	.4byte Data_02005198
.L_0200a1cc:
	.4byte Data_02005190
	.section .text.x0200a1d0,"ax",%progbits
	.global Func_020021d0
	.thumb_func
Func_020021d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #12
	sub sp, #16
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #13
	bl Object_GetById
	ldr r3, [r7, #8]
	ldr r5, .L_0200a420
	asrs r3, r3, #20
	mov r11, r3
	ldr r3, [r7, #16]
	mov r8, r0
	asrs r3, r3, #20
	mov r9, r3
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	str r3, [sp, #12]
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	str r3, [sp, #8]
	ldr r0, [r5]
	asrs r3, r0, #20
	cmp r3, #12
	bne .L_0200a232
	ldr r6, .L_0200a424
	ldr r1, [r6]
	asrs r3, r1, #20
	cmp r3, #24
	bne .L_0200a232
	movs r2, #1
	movs r3, #21
	bl Func_020020ec
	ldr r0, [r5]
	ldr r1, [r6]
	movs r2, #2
	movs r3, #0
	bl Func_020020ec
	b .L_0200a240
.L_0200a232:
	ldr r3, .L_0200a424
	ldr r0, [r5]
	ldr r1, [r3]
	movs r2, #3
	movs r3, #0
	bl Func_020020ec
.L_0200a240:
	ldr r5, .L_0200a428
	ldr r0, [r5]
	asrs r3, r0, #20
	cmp r3, #7
	bne .L_0200a26a
	ldr r6, .L_0200a42c
	ldr r1, [r6]
	asrs r3, r1, #20
	cmp r3, #27
	bne .L_0200a26a
	movs r2, #1
	movs r3, #21
	bl Func_020020ec
	ldr r0, [r5]
	ldr r1, [r6]
	movs r2, #2
	movs r3, #0
	bl Func_020020ec
	b .L_0200a278
.L_0200a26a:
	ldr r3, .L_0200a42c
	ldr r0, [r5]
	ldr r1, [r3]
	movs r2, #3
	movs r3, #0
	bl Func_020020ec
.L_0200a278:
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	movs r2, #3
	movs r3, #255
	bl Func_020020ec
	mov r2, r8
	ldr r0, [r2, #8]
	ldr r1, [r2, #16]
	movs r3, #255
	movs r2, #3
	bl Func_020020ec
	movs r0, #132
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a352
	mov r3, r11
	cmp r3, #12
	bne .L_0200a352
	mov r2, r9
	cmp r2, #26
	bne .L_0200a352
	movs r3, #0
	mov r10, r3
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	ldr r2, [sp, #12]
	cmp r2, #12
	bne .L_0200a2dc
	ldr r3, [sp, #8]
	cmp r3, #26
	bne .L_0200a2dc
	mov r2, r11
	mov r3, r9
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r0, #7
	movs r1, #2
	movs r3, #1
	bl Func_02003e24
	movs r2, #1
	mov r10, r2
.L_0200a2dc:
	adds r6, r7, #0
	movs r3, #3
	adds r6, #85
	strb r3, [r6]
	movs r0, #10
	bl WaitFrames
	movs r0, #161
	bl Func_02003ffc
	movs r0, #10
	bl WaitFrames
	movs r5, #0
	movs r0, #132
	strb r5, [r6]
	lsls r0, r0, #2
	bl GameFlag_SetBit
	mov r3, r10
	cmp r3, #0
	beq .L_0200a34e
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	movs r2, #3
	movs r3, #0
	bl Func_020020ec
	mov r2, r8
	ldr r0, [r2, #8]
	ldr r1, [r2, #16]
	movs r3, #0
	movs r2, #3
	bl Func_020020ec
	movs r3, #12
	movs r2, #26
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #7
	movs r1, #3
	bl Func_02003e24
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_SetBit
	adds r2, r7, #0
	adds r2, #98
	movs r3, #1
	strb r3, [r2]
	mov r2, r8
	adds r2, #98
	strb r3, [r2]
.L_0200a34e:
	bl Func_02003e6c
.L_0200a352:
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a412
	ldr r3, [sp, #12]
	cmp r3, #7
	bne .L_0200a412
	ldr r2, [sp, #8]
	cmp r2, #29
	bne .L_0200a412
	movs r3, #0
	mov r10, r3
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	mov r2, r11
	cmp r2, #7
	bne .L_0200a39a
	mov r3, r9
	cmp r3, #29
	bne .L_0200a39a
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r0, #7
	movs r1, #2
	movs r3, #1
	bl Func_02003e24
	movs r2, #1
	mov r10, r2
.L_0200a39a:
	mov r6, r8
	movs r3, #3
	adds r6, #85
	strb r3, [r6]
	movs r0, #10
	bl WaitFrames
	movs r0, #161
	bl Func_02003ffc
	movs r0, #10
	bl WaitFrames
	movs r0, #137
	movs r5, #0
	lsls r0, r0, #1
	strb r5, [r6]
	adds r0, #255
	bl GameFlag_SetBit
	mov r3, r10
	cmp r3, #0
	beq .L_0200a40e
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	movs r2, #3
	movs r3, #0
	bl Func_020020ec
	mov r2, r8
	ldr r0, [r2, #8]
	ldr r1, [r2, #16]
	movs r3, #0
	movs r2, #3
	bl Func_020020ec
	movs r3, #7
	movs r2, #29
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #7
	movs r1, #3
	bl Func_02003e24
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_SetBit
	adds r2, r7, #0
	adds r2, #98
	movs r3, #1
	strb r3, [r2]
	mov r2, r8
	adds r2, #98
	strb r3, [r2]
.L_0200a40e:
	bl Func_02003e6c
.L_0200a412:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a420:
	.4byte Data_0200519c
.L_0200a424:
	.4byte Data_02005194
.L_0200a428:
	.4byte Data_02005198
.L_0200a42c:
	.4byte Data_02005190
	.section .text.x0200a430,"ax",%progbits
	.global Func_02002430
	.thumb_func
Func_02002430:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #13
	bl Object_GetById
	ldr r3, [r5, #8]
	ldr r7, .L_0200a4b8
	ldr r6, .L_0200a4bc
	str r3, [r7]
	ldr r2, .L_0200a4c0
	ldr r3, [r5, #16]
	mov r8, r2
	str r3, [r6]
	ldr r3, [r0, #8]
	str r3, [r2]
	ldr r3, .L_0200a4c4
	mov r10, r3
	ldr r3, [r0, #16]
	mov r2, r10
	str r3, [r2]
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	ldr r3, [r7]
	asrs r3, r3, #20
	cmp r3, #7
	bne .L_0200a4a4
	ldr r3, [r6]
	asrs r3, r3, #20
	cmp r3, #29
	bne .L_0200a4a4
	mov r2, r8
	ldr r3, [r2]
	asrs r3, r3, #20
	cmp r3, #7
	bne .L_0200a4a4
	mov r2, r10
	ldr r3, [r2]
	asrs r3, r3, #20
	cmp r3, #28
	bne .L_0200a4a4
	adds r2, r5, #0
	adds r2, #98
	movs r3, #0
	strb r3, [r2]
	ldr r0, [r7]
	ldr r1, [r6]
	movs r2, #2
	bl Func_020020ec
.L_0200a4a4:
	bl Func_02003fe4
	bl Func_020021d0
	bl Func_02003e6c
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a4b8:
	.4byte Data_0200519c
.L_0200a4bc:
	.4byte Data_02005194
.L_0200a4c0:
	.4byte Data_02005198
.L_0200a4c4:
	.4byte Data_02005190
	.section .text.x0200a53c,"ax",%progbits
	.global Func_0200253c
	.thumb_func
Func_0200253c:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200a598
	movs r2, #128
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200a596
	ldr r3, .L_0200a59c
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	ldr r5, .L_0200a5a0
	bl Object_GetById
	ldrh r3, [r5]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #246
	adds r3, r3, r2
	movs r2, #200
	lsls r3, r3, #16
	lsls r2, r2, #16
	adds r6, r0, #0
	cmp r3, r2
	bls .L_0200a596
	bl Func_02003e64
	adds r5, r6, #0
	movs r0, #0
	bl Func_02003fa4
	adds r5, #85
	movs r3, #0
	strb r3, [r5]
	movs r2, #148
	ldr r0, [r7]
	movs r1, #72
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r3, #3
	strb r3, [r5]
	bl Func_02003e6c
.L_0200a596:
	pop {r5, r6, r7, pc}
.L_0200a598:
	.4byte gInput
.L_0200a59c:
	.4byte gPartyState
.L_0200a5a0:
	.4byte gSceneState
	.section .text.x0200a830,"ax",%progbits
	.global Func_02002830
	.thumb_func
Func_02002830:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	ldr r5, .L_0200a9ac
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	ldr r0, [r5]
	bl Object_GetById
	mov r10, r0
	movs r0, #9
	bl Object_GetById
	ldr r1, [r5]
	mov r8, r0
	movs r0, #9
	bl Func_02003eec
	movs r2, #130
	ldr r1, .L_0200a9b0
	lsls r2, r2, #18
	movs r0, #10
	bl Func_02003edc
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	mov r2, r8
	lsls r3, r3, #8
	strh r3, [r2, #6]
	ldr r3, .L_0200a9b4
	mov r1, r10
	str r3, [r1, #108]
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	movs r2, #192
	lsls r2, r2, #18
	movs r3, #214
	mov r9, r2
	lsls r3, r3, #1
	ldr r2, [r2, #108]
	mov r11, r3
	mov r1, r11
	adds r3, #85
	str r3, [r2, r1]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #153
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #153
	adds r2, #204
	movs r0, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	bl Object_GetById
	ldr r3, .L_0200a9b8
	mov r2, r8
	str r3, [r0, #108]
	movs r1, #140
	str r3, [r2, #108]
	movs r2, #234
	movs r0, #10
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #148
	movs r2, #226
	lsls r2, r2, #1
	movs r0, #9
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #10
	movs r1, #1
	bl Object_SetModeById
	movs r6, #0
	mov r3, r10
	mov r1, r8
	str r6, [r3, #108]
	movs r0, #10
	str r6, [r1, #108]
	bl Object_GetById
	movs r1, #9
	str r6, [r0, #108]
	ldr r0, [r5]
	bl Func_02003fac
	ldr r0, .L_0200a9bc
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r0, #9
	movs r1, #1
	bl Object_SetModeById
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	ldr r0, [r5]
	ldr r1, .L_0200a9c0
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	ldr r0, [r5]
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #156
	movs r2, #220
	lsls r2, r2, #1
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fb4
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	mov r3, r9
	ldr r2, [r3, #108]
	movs r3, #128
	lsls r3, r3, #1
	mov r1, r11
	str r3, [r2, r1]
	bl Func_020034d8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a9ac:
	.4byte gPartyState
.L_0200a9b0:
	.4byte 0x01110000
.L_0200a9b4:
	.4byte Func_02001278
.L_0200a9b8:
	.4byte Func_02000df8
.L_0200a9bc:
	.4byte MsgHereWeAre
.L_0200a9c0:
	.4byte 0x00019999
	.section .text.x0200a9c4,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_020029c4
	.thumb_func
Func_020029c4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_ja_0200a980
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #9
	bl Object_GetById
	ldr r1, [r5]
	adds r7, r0, #0
	movs r0, #9
	bl Func_02003eec
	movs r1, #240
	movs r2, #135
	lsls r2, r2, #18
	lsls r1, r1, #16
	movs r0, #10
	bl Func_02003edc
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_ja_0200a97c
	mov r8, r3
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r7, #6]
	ldr r3, .L_ja_0200a984
	ldr r0, [r5]
	str r3, [r6, #108]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	b .L_ja_0200a988
.L_ja_0200a97c:
	.4byte 0x00000000
.L_ja_0200a980:
	.4byte gPartyState
.L_ja_0200a984:
	.4byte Func_02001278
.L_ja_0200a988:
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	movs r0, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	bl Object_GetById
	ldr r3, .L_ja_0200aa6c
	adds r5, r7, #0
	str r3, [r0, #108]
	adds r5, #99
	str r3, [r7, #108]
	mov r3, r8
	ldr r1, .L_ja_0200aa70
	adds r0, r7, #0
	strb r3, [r5]
	bl Func_02003dec
	ldr r1, .L_ja_0200aa74
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
.L_ja_0200a9c4:
	movs r0, #1
	bl WaitFrames
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_ja_0200a9c4
	movs r5, #0
	str r5, [r6, #108]
	movs r0, #10
	str r5, [r7, #108]
	bl Object_GetById
	movs r1, #192
	str r5, [r0, #108]
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02003f3c
	ldr r5, .L_ja_0200aa78
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #9
	bl Func_02003fac
	ldr r0, .L_ja_0200aa7c
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r0, #9
	movs r1, #1
	bl Object_SetModeById
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	ldr r0, [r5]
	ldr r1, .L_ja_0200aa80
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	ldr r0, [r5]
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #230
	ldr r0, [r5]
	movs r1, #152
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fb4
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	bl Func_020034d8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200aa6c:
	.4byte Func_02000df8
.L_ja_0200aa70:
	.4byte Data_02004508
.L_ja_0200aa74:
	.4byte Data_02004554
.L_ja_0200aa78:
	.4byte gPartyState
.L_ja_0200aa7c:
	.4byte MsgWellHereWe
.L_ja_0200aa80:
	.4byte 0x00019999
	.else
	.global Func_020029c4
	.thumb_func
Func_020029c4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200aa40
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #9
	bl Object_GetById
	ldr r1, [r5]
	adds r7, r0, #0
	movs r0, #9
	bl Func_02003eec
	movs r1, #240
	movs r2, #135
	lsls r2, r2, #18
	lsls r1, r1, #16
	movs r0, #10
	bl Func_02003edc
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0200aa3c
	mov r8, r3
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r7, #6]
	ldr r3, .L_0200aa44
	ldr r0, [r5]
	str r3, [r6, #108]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_StartScreenTransition
	b .L_0200aa48
.L_0200aa3c:
	.4byte 0x00000000
.L_0200aa40:
	.4byte gPartyState
.L_0200aa44:
	.4byte Func_02001278
.L_0200aa48:
	bl Event_WaitTransitionFrames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	movs r0, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	bl Object_GetById
	ldr r3, .L_0200ab4c
	adds r5, r7, #0
	str r3, [r0, #108]
	adds r5, #99
	str r3, [r7, #108]
	mov r3, r8
	ldr r1, .L_0200ab50
	adds r0, r7, #0
	strb r3, [r5]
	bl Func_02003dec
	ldr r1, .L_0200ab54
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
.L_0200aa94:
	movs r0, #1
	bl WaitFrames
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_0200aa94
	movs r5, #0
	str r5, [r6, #108]
	movs r0, #10
	str r5, [r7, #108]
	bl Object_GetById
	movs r1, #192
	str r5, [r0, #108]
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02003f3c
	ldr r5, .L_0200ab58
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #9
	bl Func_02003fac
	ldr r0, .L_0200ab5c
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r0, #9
	movs r1, #1
	bl Object_SetModeById
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	ldr r0, [r5]
	ldr r1, .L_0200ab60
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	ldr r0, [r5]
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #230
	ldr r0, [r5]
	movs r1, #152
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fb4
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	bl Func_020034d8
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ab4c:
	.4byte Func_02000df8
.L_0200ab50:
	.4byte Data_02004508
.L_0200ab54:
	.4byte Data_02004554
.L_0200ab58:
	.4byte gPartyState
.L_0200ab5c:
	.4byte MsgWellHereWe
.L_0200ab60:
	.4byte 0x00019999
	.endif
	.section .text.x0200ad7c,"ax",%progbits
	.balign 4
	.global Func_02002d7c
	.thumb_func
Func_02002d7c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, .L_0200aec0
	ldr r6, [r3, #108]
	movs r2, #179
	lsls r2, r2, #1
	adds r1, r7, #2
	adds r3, r6, r2
	mov r8, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200adaa
	movs r0, #134
	lsls r0, r0, #2
	bl GameFlag_Test
	movs r5, #11
	cmp r0, #0
	beq .L_0200adce
.L_0200adaa:
	movs r5, #11
.L_0200adac:
	adds r0, r5, #0
	bl Object_GetById
	adds r5, #1
	adds r0, #91
	movs r3, #1
	strb r3, [r0]
	cmp r5, #18
	ble .L_0200adac
	b .L_0200af14
.L_0200adc0:
	adds r0, r5, #0
	bl Object_GetById
	movs r3, #0
	adds r0, #91
	strb r3, [r0]
	adds r5, #1
.L_0200adce:
	cmp r5, #18
	ble .L_0200adc0
	ldr r3, .L_0200aec4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r1, .L_0200aec8
	ldr r3, [r0, #12]
	cmp r3, r1
	bge .L_0200ae0e
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	movs r5, #11
.L_0200adf0:
	adds r0, r5, #0
	bl Object_GetById
	adds r5, #1
	adds r0, #91
	movs r3, #1
	strb r3, [r0]
	cmp r5, #18
	ble .L_0200adf0
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #200
	strh r3, [r2]
	b .L_0200af14
.L_0200ae0e:
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #171
	adds r2, r3, #1
	mov r3, r8
	strh r2, [r3]
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_0200ae28
	b .L_0200af14
.L_0200ae28:
	lsls r3, r2, #16
	cmp r3, #0
	beq .L_0200af14
.L_0200ae2e:
	ldrh r3, [r7]
	cmp r3, #100
	beq .L_0200aeae
	cmp r3, #100
	bgt .L_0200ae3e
	cmp r3, #0
	beq .L_0200ae48
	b .L_0200aee6
.L_0200ae3e:
	cmp r3, #130
	beq .L_0200aecc
	cmp r3, #220
	beq .L_0200aeae
	b .L_0200aee6
.L_0200ae48:
	movs r5, #11
.L_0200ae4a:
	adds r0, r5, #0
	movs r1, #2
	adds r5, #1
	bl Object_SetModeById
	cmp r5, #18
	ble .L_0200ae4a
	movs r5, #1
	negs r5, r5
	movs r0, #11
	movs r1, #0
	movs r2, #1
	bl Func_02002d48
	movs r0, #12
	movs r1, #0
	adds r2, r5, #0
	bl Func_02002d48
	movs r0, #13
	movs r1, #0
	movs r2, #1
	bl Func_02002d48
	movs r0, #14
	movs r1, #0
	movs r2, #1
	bl Func_02002d48
	movs r0, #15
	adds r1, r5, #0
	movs r2, #0
	bl Func_02002d48
	movs r0, #16
	movs r1, #0
	movs r2, #1
	bl Func_02002d48
	movs r0, #17
	movs r1, #1
	movs r2, #0
	bl Func_02002d48
	movs r0, #18
	movs r1, #0
	adds r2, r5, #0
	bl Func_02002d48
	b .L_0200aee6
.L_0200aeae:
	movs r5, #11
.L_0200aeb0:
	adds r0, r5, #0
	movs r1, #1
	adds r5, #1
	bl Object_SetModeById
	cmp r5, #18
	ble .L_0200aeb0
	b .L_0200aee6
.L_0200aec0:
	.4byte gSceneState
.L_0200aec4:
	.4byte gPartyState
.L_0200aec8:
	.4byte 0xfff40000
.L_0200aecc:
	movs r5, #11
.L_0200aece:
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetModeById
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	adds r5, #1
	bl Func_02002d48
	cmp r5, #18
	ble .L_0200aece
.L_0200aee6:
	ldrh r3, [r7]
	movs r2, #130
	adds r3, #1
	strh r3, [r7]
	lsls r2, r2, #17
	lsls r3, r3, #16
	cmp r3, r2
	bne .L_0200aefa
	ldr r3, .L_0200af10
	strh r3, [r7]
.L_0200aefa:
	mov r1, r8
	ldrh r3, [r1]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200ae2e
	b .L_0200af14
.L_0200af10:
	.4byte 0x00000000
.L_0200af14:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
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
	bl Object_GetById
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
	bl ObjectMotion_SetActionVariant
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
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #13
	bl Object_GetById
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
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #13
	bl Object_GetById
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
	bl Object_GetById
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
	.4byte gPartyState
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
	bl Object_GetById
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
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #144
	ldr r0, .L_ja_0200b320
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
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
	bl WaitFrames
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
	.4byte gPartyState
.L_ja_0200b320:
	.4byte Func_02001c4c
.L_ja_0200b324:
	.4byte 0x03090000
	.else
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
	ldr r3, .L_0200b400
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_0200b2d4
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200b2d4:
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
	beq .L_0200b332
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200b332
	movs r1, #140
	movs r2, #234
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #10
	bl Func_02003edc
	movs r0, #10
	bl Object_GetById
	movs r1, #148
	adds r0, #85
	movs r2, #226
	strb r5, [r0]
	lsls r1, r1, #17
	movs r0, #9
	lsls r2, r2, #17
	bl Func_02003edc
.L_0200b332:
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #144
	ldr r0, .L_0200b404
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0200b400
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #4
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200b3b0
	movs r0, #140
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b3b0
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
	ldr r2, .L_0200b408
	bl Func_02003edc
	movs r0, #1
	bl WaitFrames
.L_0200b3b0:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b3ee
	ldr r3, .L_0200b400
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_0200b3d2
	bl Func_02002830
	b .L_0200b3fa
.L_0200b3d2:
	cmp r3, #4
	bne .L_0200b3dc
	bl Func_020029c4
	b .L_0200b3fa
.L_0200b3dc:
	cmp r3, #5
	beq .L_0200b3fa
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_020034d8
	b .L_0200b3fa
.L_0200b3ee:
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_020034d8
.L_0200b3fa:
	bl Func_02003e6c
	pop {r5, pc}
.L_0200b400:
	.4byte gPartyState
.L_0200b404:
	.4byte Func_02001c4c
.L_0200b408:
	.4byte 0x03090000
	.endif
	.section .text.x0200b410,"ax",%progbits
	.global Func_02003410
	.thumb_func
Func_02003410:
	push {r5, lr}
	movs r0, #112
	movs r1, #180
	bl Runtime_AllocateBlock
	movs r5, #0
	adds r4, r0, #0
.L_0200b41e:
	movs r3, #0
	str r3, [r4]
	strh r3, [r4, #4]
	strh r3, [r4, #6]
	strh r3, [r4, #8]
	strh r3, [r4, #10]
	movs r2, #0
.L_0200b42c:
	lsls r3, r2, #16
	lsrs r3, r3, #16
	ldr r1, .L_0200b45c
	lsls r2, r3, #1
	adds r2, #12
	strh r1, [r4, r2]
	adds r3, #1
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #13
	asrs r2, r3, #16
	cmp r3, r1
	bne .L_0200b42c
	movs r2, #128
	lsls r3, r5, #16
	lsls r2, r2, #9
	movs r1, #128
	adds r3, r3, r2
	lsls r1, r1, #11
	adds r4, #44
	asrs r5, r3, #16
	cmp r3, r1
	bne .L_0200b41e
	b .L_0200b460
.L_0200b45c:
	.4byte 0x00000000
.L_0200b460:
	adds r2, r0, #0
	adds r2, #176
	movs r3, #0
	strh r3, [r2]
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200b4fc,"ax",%progbits
	.balign 4
	.global Func_020034fc
	.thumb_func
Func_020034fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #192
	lsls r1, r1, #18
	ldr r2, [r1, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	sub sp, #32
	cmp r3, #0
	bne .L_0200b610
	movs r0, #173
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	bne .L_0200b610
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200b610
	ldr r1, [r1, #112]
	movs r0, #0
	mov r9, r1
	mov r3, r9
	adds r3, #176
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	lsls r1, r3, #16
	mov r8, r0
	mov r11, r1
	cmp r8, r3
	bcs .L_0200b610
.L_0200b556:
	movs r3, #44
	mov r2, r8
	muls r2, r3
	mov r0, r9
	adds r3, r2, #0
	adds r5, r0, r3
	ldrh r2, [r5, #6]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_0200b5f4
	movs r1, #4
	ldrsh r6, [r5, r1]
	movs r3, #10
	ldrsh r2, [r5, r3]
	ldr r0, [r5]
	subs r3, r2, r6
	lsls r7, r2, #16
	lsls r3, r3, #24
	lsrs r1, r7, #16
	mov r10, r0
	adds r4, r5, #0
	lsrs r0, r3, #24
	mov r12, r1
	adds r4, #12
	cmp r0, r12
	bcs .L_0200b59e
.L_0200b58a:
	ldrh r3, [r4]
	lsls r2, r0, #1
	mov r1, sp
	strh r3, [r1, r2]
	adds r3, r0, #1
	lsls r3, r3, #24
	lsrs r0, r3, #24
	adds r4, #2
	cmp r0, r12
	bcc .L_0200b58a
.L_0200b59e:
	lsls r6, r6, #16
	lsrs r2, r7, #16
	mov lr, r6
	lsrs r6, r6, #16
	movs r0, #0
	subs r3, r2, r6
	mov r12, r2
	cmp r0, r3
	bge .L_0200b5c8
.L_0200b5b0:
	ldrh r3, [r4]
	mov r1, sp
	lsls r2, r0, #1
	strh r3, [r1, r2]
	adds r3, r0, #1
	lsls r3, r3, #24
	mov r1, r12
	lsrs r0, r3, #24
	subs r3, r1, r6
	adds r4, #2
	cmp r0, r3
	blt .L_0200b5b0
.L_0200b5c8:
	movs r2, #128
	movs r3, #128
	lsrs r4, r7, #16
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	mov r0, sp
	mov r1, r10
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #128
	lsls r3, r3, #9
	add r3, lr
	asrs r6, r3, #16
	lsrs r3, r3, #16
	cmp r3, r4
	bcc .L_0200b5ee
	movs r6, #0
.L_0200b5ee:
	ldrh r3, [r5, #8]
	strh r6, [r5, #4]
	b .L_0200b5fc
.L_0200b5f4:
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
.L_0200b5fc:
	strh r3, [r5, #6]
	mov r3, r8
	adds r3, #1
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r1, r11
	mov r8, r3
	lsrs r3, r1, #16
	cmp r8, r3
	bcc .L_0200b556
.L_0200b610:
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
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
	bl Object_GetById
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
	.4byte Data_02004070
.L_0200b78c:
	.4byte Data_0200407c
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
	.4byte Data_0200407c
.L_0200b938:
	.4byte Data_02004094
.L_0200b93c:
	.4byte 0xffff0000
	.section .text.x0200b940,"ax",%progbits
	.global Func_02003940
	.thumb_func
Func_02003940:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r0, [sp, #8]
	str r1, [sp, #4]
	str r2, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_0200ba4c
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	adds r7, r0, #0
	ldrh r3, [r7, #6]
	ldr r1, [sp, #8]
	lsrs r3, r3, #12
	str r3, [r1]
	movs r2, #8
	adds r5, #52
	mov r11, r2
	mov lr, r5
.L_0200b97c:
	mov r3, lr
	ldr r6, [r3]
	movs r5, #0
.L_0200b982:
	ldr r3, [r6, #80]
	ldr r2, .L_0200ba50
	ldr r3, [r3, #40]
	movs r0, #0
	ldrsh r1, [r3, r0]
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	cmp r1, r3
	bne .L_0200ba26
	ldr r0, [sp, #8]
	movs r2, #10
	ldrsh r1, [r7, r2]
	ldr r3, [r0]
	ldr r2, .L_0200ba54
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	ldr r4, .L_0200ba58
	asrs r2, r3, #16
	adds r1, r1, r2
	asrs r1, r1, #4
	mov r9, r1
	movs r1, #18
	ldrsh r2, [r7, r1]
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r2, r2, r3
	asrs r2, r2, #4
	mov r8, r2
	movs r2, #10
	ldrsh r0, [r6, r2]
	lsls r2, r5, #2
	ldrsb r3, [r4, r2]
	adds r3, r0, r3
	asrs r3, r3, #4
	mov r10, r3
	movs r3, #18
	ldrsh r1, [r6, r3]
	adds r3, r2, #1
	ldrsb r3, [r4, r3]
	adds r3, r1, r3
	asrs r3, r3, #4
	mov r12, r3
	adds r3, r2, #2
	ldrsb r3, [r4, r3]
	adds r2, #3
	adds r0, r0, r3
	ldrsb r3, [r4, r2]
	asrs r0, r0, #4
	adds r1, r1, r3
	asrs r1, r1, #4
	cmp r10, r9
	bgt .L_0200ba26
	cmp r9, r0
	bge .L_0200ba26
	cmp r12, r8
	bgt .L_0200ba26
	cmp r8, r1
	bge .L_0200ba26
	ldr r0, [sp, #0]
	movs r3, #1
	ands r3, r5
	str r5, [r0]
	cmp r3, #0
	beq .L_0200ba14
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r10, r3
	beq .L_0200ba26
	ldr r2, [sp, #4]
	mov r1, r11
	str r1, [r2]
	adds r0, r6, #0
	b .L_0200ba3c
.L_0200ba14:
	ldr r3, [r7, #16]
	asrs r3, r3, #20
	cmp r12, r3
	beq .L_0200ba26
	ldr r0, [sp, #4]
	mov r3, r11
	str r3, [r0]
	adds r0, r6, #0
	b .L_0200ba3c
.L_0200ba26:
	adds r5, #1
	cmp r5, #5
	bls .L_0200b982
	movs r2, #1
	add r11, r2
	movs r1, #4
	mov r3, r11
	add lr, r1
	cmp r3, #63
	bls .L_0200b97c
	movs r0, #0
.L_0200ba3c:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ba4c:
	.4byte gPartyState
.L_0200ba50:
	.4byte Data_02004070
.L_0200ba54:
	.4byte Data_02004094
.L_0200ba58:
	.4byte Data_0200407c
	.section .text.x0200ba5c,"ax",%progbits
	.global Func_02003a5c
	.thumb_func
Func_02003a5c:
	sub sp, #16
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r0, [sp, #88]
	str r1, [sp, #92]
	str r2, [sp, #96]
	str r3, [sp, #100]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r0, #133
	str r3, [sp, #28]
	ldr r3, .L_0200bcfc
	lsls r0, r0, #2
	adds r0, r0, r3
	mov r10, r0
	ldr r0, [r0]
	bl Object_GetById
	mov r8, r0
	ldr r0, [sp, #104]
	bl Object_GetById
	mov r3, r8
	ldr r3, [r3, #48]
	mov r4, r8
	str r3, [sp, #20]
	adds r6, r0, #0
	ldr r4, [r4, #52]
	mov r0, sp
	adds r0, #32
	str r0, [sp, #12]
	str r4, [sp, #16]
	ldr r2, [sp, #100]
	ldr r3, [r6, #8]
	movs r1, #0
	str r3, [r0]
	mov r9, r1
	ldr r3, [r6, #16]
	mov r1, sp
	adds r1, #44
	str r3, [r0, #8]
	ldr r5, .L_0200bd00
	str r1, [sp, #8]
	lsls r7, r2, #2
	ldrsb r1, [r5, r7]
	ldr r3, [r6, #8]
	lsls r2, r1, #16
	adds r3, r3, r2
	ldr r2, [sp, #8]
	asrs r3, r3, #20
	str r3, [r2]
	mov lr, r3
	adds r3, r7, #1
	ldrsb r4, [r5, r3]
	ldr r3, [r6, #16]
	ldr r0, [sp, #8]
	lsls r2, r4, #16
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [r0, #8]
	adds r0, r1, #0
	mov r12, r3
	cmp r0, #0
	bge .L_0200baec
	negs r0, r0
.L_0200baec:
	adds r3, r7, #2
	ldrsb r1, [r5, r3]
	cmp r1, #0
	bge .L_0200baf6
	negs r1, r1
.L_0200baf6:
	adds r3, r0, r1
	asrs r3, r3, #4
	adds r1, r4, #0
	str r3, [sp, #24]
	cmp r1, #0
	bge .L_0200bb04
	negs r1, r1
.L_0200bb04:
	adds r3, r7, #3
	ldrsb r2, [r5, r3]
	cmp r2, #0
	bge .L_0200bb0e
	negs r2, r2
.L_0200bb0e:
	adds r3, r1, r2
	asrs r3, r3, #4
	str r3, [sp, #0]
	mov r11, r3
	movs r3, #0
	str r3, [sp, #4]
	mov r1, lr
	mov r2, r12
	ldr r3, [sp, #24]
	movs r0, #0
	bl Func_02003790
	mov r1, r10
	movs r2, #200
	ldr r0, [r1]
	lsls r2, r2, #5
	movs r1, #128
	lsls r1, r1, #8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	mov r2, r10
	ldr r0, [r2]
	movs r1, #8
	bl Object_SetModeById
	movs r0, #15
	bl WaitFrames
	ldr r4, [sp, #12]
	ldr r1, [sp, #88]
	ldr r3, [r4]
	ldr r2, [sp, #96]
	subs r1, r1, r3
	ldr r3, [r4, #8]
	asrs r1, r1, #17
	subs r2, r2, r3
	mov r3, r10
	asrs r2, r2, #17
	ldr r0, [r3]
	bl ObjectMotion_OffsetPositionAndResetMotion
	mov r4, r10
	ldr r0, [r4]
	bl Object_GetById
	ldr r3, .L_0200bd04
	str r3, [r0, #108]
	movs r0, #4
	bl WaitFrames
	movs r1, #2
	adds r0, r6, #0
	bl Func_02003de4
	movs r0, #239
	bl Func_02003ffc
	movs r2, #200
	movs r1, #128
	lsls r2, r2, #5
	ldr r0, [sp, #104]
	lsls r1, r1, #8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	adds r0, r6, #0
	ldr r1, [sp, #88]
	ldr r2, [sp, #92]
	ldr r3, [sp, #96]
	bl Func_02003e0c
	ldr r3, .L_0200bcfc
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r3, r0
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #152
	movs r2, #200
	lsls r1, r1, #7
	lsls r2, r2, #5
	ldr r0, [r5]
	adds r1, #204
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	ldr r2, .L_0200bd08
	mov r1, r9
	lsls r3, r1, #2
	ldr r2, [r2, r3]
	ldr r0, [r5]
	lsls r2, r2, #16
	asrs r1, r2, #31
	asrs r2, r2, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r3, [sp, #108]
	cmp r3, #0
	beq .L_0200bbe4
	mov lr, r3
	.2byte 0xf800
.L_0200bbe4:
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #1
	ldr r0, [r5]
	bl Object_SetModeById
	mov r3, r8
	movs r2, #0
	str r2, [r3, #108]
	ldr r4, [sp, #20]
	movs r5, #255
	str r4, [r3, #48]
	ldr r0, [sp, #16]
	str r0, [r3, #52]
	adds r0, r6, #0
	bl Func_02003e14
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02003ffc
	movs r0, #213
	bl Func_02003ffc
	ldr r2, [r6, #12]
	ldr r1, [sp, #88]
	ldr r3, [sp, #96]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotion
	adds r0, r6, #0
	movs r1, #1
	bl Func_02003de4
	ldr r1, .L_0200bd00
	ldr r0, [sp, #88]
	ldrsb r3, [r1, r7]
	adds r2, r7, #1
	lsls r3, r3, #16
	adds r0, r0, r3
	ldrsb r3, [r1, r2]
	mov r10, r1
	ldr r1, [sp, #96]
	lsls r3, r3, #16
	adds r1, r1, r3
	ldr r4, [sp, #28]
	asrs r0, r0, #20
	asrs r1, r1, #20
	str r0, [sp, #88]
	str r1, [sp, #96]
	mov r9, r2
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r4, r2
	ldr r3, [r3]
	adds r2, #4
	asrs r3, r3, #20
	mov r8, r3
	adds r3, r4, r2
	ldr r6, [r3]
	mov r4, r8
	asrs r6, r6, #20
	adds r3, r4, r0
	adds r2, r6, r1
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r3, r11
	ldr r2, [sp, #24]
	bl Func_02003e24
	mov r0, r11
	ldr r1, [sp, #88]
	ldr r2, [sp, #96]
	str r0, [sp, #0]
	ldr r3, [sp, #24]
	movs r0, #0
	str r5, [sp, #4]
	bl Func_02003790
	mov r3, r11
	ldr r1, [sp, #88]
	ldr r2, [sp, #96]
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #24]
	str r5, [sp, #4]
	bl Func_02003790
	ldr r0, [sp, #12]
	mov r4, r10
	ldrsb r3, [r4, r7]
	ldr r1, [r0]
	ldr r2, [sp, #8]
	lsls r3, r3, #16
	adds r1, r1, r3
	asrs r1, r1, #20
	str r1, [r2]
	mov r3, r9
	ldrsb r2, [r4, r3]
	ldr r3, [r0, #8]
	ldr r4, [sp, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [r4, #8]
	add r8, r1
	adds r6, r6, r3
	str r1, [sp, #0]
	str r3, [sp, #4]
	ldr r2, [sp, #24]
	mov r0, r8
	adds r1, r6, #0
	mov r3, r11
	bl Func_02003e24
	ldr r0, [sp, #8]
	mov r3, r11
	ldr r1, [r0]
	ldr r2, [r0, #8]
	movs r4, #0
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #24]
	str r4, [sp, #4]
	bl Func_02003790
	bl Func_02003fd4
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r3}
	add sp, #16
	bx r3
	.2byte 0x0000
.L_0200bcfc:
	.4byte gPartyState
.L_0200bd00:
	.4byte Data_0200407c
.L_0200bd04:
	.4byte Func_02003d0c
.L_0200bd08:
	.4byte Data_02004094
	.section .text.x0200bd0c,"ax",%progbits
	.global Func_02003d0c
	.thumb_func
Func_02003d0c:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldrh r3, [r5, #6]
	movs r2, #12
	lsrs r1, r3, #12
	adds r3, r1, #2
	ands r3, r2
	lsls r1, r3, #12
	ldr r3, [r5, #8]
	sub sp, #12
	mov r6, sp
	str r3, [r6]
	ldr r3, [r5, #12]
	movs r0, #128
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	lsls r0, r0, #13
	adds r2, r6, #0
	str r3, [r6, #8]
	bl Vector_AddPolarOffsetFar
	adds r0, r6, #0
	movs r1, #1
	bl Func_02003fdc
	cmp r0, #0
	beq .L_0200bd68
	movs r4, #0
.L_0200bd44:
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r2, .L_0200bd90
	lsls r3, r4, #1
	ldrh r3, [r2, r3]
	cmp r1, r3
	beq .L_0200bd8c
	adds r4, #1
	cmp r4, #5
	bls .L_0200bd44
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
.L_0200bd68:
	ldr r3, [r5, #8]
	adds r0, r5, #0
	str r3, [r6]
	ldr r3, [r5, #12]
	adds r1, r6, #0
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl Func_02003e2c
	cmp r0, #0
	ble .L_0200bd8c
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
.L_0200bd8c:
	add sp, #12
	pop {r5, r6, pc}
.L_0200bd90:
	.4byte Data_02004070
	.section .bss,"aw",%nobits
	.global Data_02004f04
Data_02004f04:
	.space 0x0000000c
	.ifdef TLA_EDITION_JA
	.space 0x00000004 @ the next object sits on 16 bytes, and the Japanese image ends 4 short of that
	.endif
	.global Data_02004f10
Data_02004f10:
	.space 0x00000280
	.global Data_02005190
Data_02005190:
	.space 0x00000004
	.global Data_02005194
Data_02005194:
	.space 0x00000004
	.global Data_02005198
Data_02005198:
	.space 0x00000004
	.global Data_0200519c
Data_0200519c:
