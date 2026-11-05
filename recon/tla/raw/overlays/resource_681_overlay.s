.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080ce,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200842a,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.else
	.2byte 0x0000
	.endif
	.section .text.x02008f00,"ax",%progbits
	.global Func_02000f00
	.thumb_func
Func_02000f00:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #9
	bl Actor_GetById
	adds r7, r0, #0
	movs r0, #10
	bl Actor_GetById
	adds r5, r0, #0
	ldr r6, [r5, #80]
	ldr r3, .L_02008f8c
	ldr r1, [r6, #40]
	movs r2, #128
	mov r8, r1
	ldr r0, .L_02008f90
	adds r1, r5, #0
	mov lr, r3
	.2byte 0xf800
	mov r1, r8
	ldrb r3, [r1, #21]
	ldr r2, .L_02008f94
	str r3, [r2]
	ldr r2, .L_02008f98
	ldrb r3, [r6, #24]
	str r3, [r2]
	ldr r3, .L_02008f9c
	ldr r1, [r5, #8]
	movs r2, #0
	str r1, [r3]
	ldr r3, .L_02008fa0
	ldr r0, [r5, #16]
	str r2, [r5, #108]
	str r0, [r3]
	str r2, [r5, #44]
	ldr r3, [r5, #12]
	str r2, [r5, #40]
	str r2, [r5, #36]
	str r3, [r5, #60]
	str r1, [r5, #56]
	str r0, [r5, #64]
	movs r0, #10
	bl Actor_ResetTargetsAndVelocity
	adds r0, r5, #0
	movs r1, #10
	bl Actor_SetAnimationSpeed
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	ldr r2, [r7]
	ldr r3, .L_02008fa4
	cmp r2, r3
	beq .L_02008f76
	ldr r3, .L_02008fa8
	cmp r2, r3
	bne .L_02008f86
.L_02008f76:
	adds r2, r7, #0
	adds r2, #91
	movs r3, #1
	strb r3, [r2]
	adds r0, r7, #0
	movs r1, #0
	bl Actor_SetAnimationSpeed
.L_02008f86:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008f8c:
	.4byte IwramCopyWords
.L_02008f90:
	.4byte gData_0200646c
.L_02008f94:
	.4byte gData_020064f8
.L_02008f98:
	.4byte Data_020064f4
.L_02008f9c:
	.4byte gData_020064ec
.L_02008fa0:
	.4byte gData_020064f0
.L_02008fa4:
	.4byte gData_020055c0
.L_02008fa8:
	.4byte gData_0200559c
	.section .text.x020099cc,"ax",%progbits
	.global Func_020019cc
	.thumb_func
Func_020019cc:
	push {r5, r6, r7, lr}
	ldr r3, .L_02009b50
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #4
	bl Actor_GetById
	adds r5, r0, #0
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	ldr r7, .L_02009b54
	movs r1, #1
	adds r0, r7, #0
	bl FieldMessage_Show
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #1
	movs r2, #0
	bl Func_02004f2c
	ldr r3, [r5, #16]
	asrs r3, r3, #19
	cmp r3, #39
	bgt .L_02009a1c
	movs r1, #220
	movs r2, #164
	ldr r0, [r6]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_ResetAndSetPositionInMode2
	ldr r0, [r6]
	bl Actor_WaitForMoveById
.L_02009a1c:
	ldr r1, [r6]
	movs r0, #11
	bl Func_02004ec4
	movs r1, #235
	movs r2, #177
	ldr r0, [r6]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_ResetAndSetPositionInMode2
	movs r1, #223
	movs r2, #177
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #11
	bl Actor_ResetAndSetPositionInMode2
	ldr r0, [r6]
	bl Actor_WaitForMoveById
	movs r0, #11
	bl Actor_WaitForMoveById
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #11
	bl Actor_ArmCallback
	adds r0, r7, #1
	bl Func_02004f0c
	movs r1, #0
	movs r0, #11
	bl Func_02004f14
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #128
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r0, #10
	bl FieldEvent_Wait
	ldr r0, [r6]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl Actor_ArmCallback
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #233
	movs r2, #171
	lsls r2, r2, #1
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Actor_ResetAndSetPositionInMode2
	ldr r0, [r6]
	bl Actor_WaitForMoveById
	adds r0, r7, #2
	movs r1, #1
	bl FieldMessage_Show
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004e24
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02004e24
	bl Func_02004e2c
	bl Func_02004ddc
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	ldrh r2, [r3]
	mov r6, sp
	movs r3, #255
	adds r6, #2
	ands r3, r2
	adds r7, r6, #0
	strh r3, [r6]
	cmp r3, #0
	beq .L_02009b58
.L_02009b20:
	ldrh r3, [r7]
	ldr r2, .L_02009b4c
	movs r1, #128
	lsls r1, r1, #19
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	ldrh r3, [r7]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	strh r3, [r7]
	adds r5, r7, #0
	movs r0, #1
	bl wait
	ldrh r3, [r5]
	cmp r3, #0
	bne .L_02009b20
	b .L_02009b58
	.2byte 0x0000
.L_02009b4c:
	.4byte 0x00001000
.L_02009b50:
	.4byte gMainRoutineWork
.L_02009b54:
	.4byte MsgGaiaRockInside09
.L_02009b58:
	movs r5, #128
	lsls r5, r5, #19
	ldrh r2, [r5]
	movs r3, #253
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r5]
	movs r1, #4
	movs r0, #12
	bl Func_020015d0
	ldrh r3, [r5]
	ldr r2, .L_02009ba0
	movs r1, #128
	orrs r3, r2
	strh r3, [r5]
	ldr r2, .L_02009ba4
	ldrh r3, [r6]
	lsls r1, r1, #19
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	movs r2, #128
	ldr r3, .L_02009ba8
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldrh r3, [r6]
	cmp r3, #8
	beq .L_02009bc6
.L_02009b96:
	ldrh r3, [r6]
	ldr r2, .L_02009ba4
	movs r1, #128
	lsls r1, r1, #19
	b .L_02009bac
.L_02009ba0:
	.4byte 0x00000f00
.L_02009ba4:
	.4byte 0x00001000
.L_02009ba8:
	.4byte 0x00003f42
.L_02009bac:
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	adds r5, r6, #0
	ldrh r3, [r6]
	movs r0, #1
	adds r3, #1
	strh r3, [r6]
	bl wait
	ldrh r3, [r5]
	cmp r3, #8
	bne .L_02009b96
.L_02009bc6:
	bl Func_02004dd4
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl Func_02004ebc
	movs r1, #139
	movs r0, #4
	bl Party_LearnAbility
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #186
	bl GameFlag_SetBit
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #11
	ldr r1, .L_02009c3c
	bl Actor_SetSpeedParameters
	movs r0, #11
	movs r1, #2
	bl Actor_SetModeById
	ldr r3, .L_02009c40
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	cmp r0, #0
	beq .L_02009c1c
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #11
	bl Actor_ResetAndSetPosition
.L_02009c1c:
	movs r0, #11
	bl Actor_WaitForMoveById
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl Func_02004ebc
	movs r0, #20
	bl FieldEvent_Wait
	bl Func_02004e64
	add sp, #4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c3c:
	.4byte 0x00013333
.L_02009c40:
	.4byte gMainRoutineWork
	.section .text.x02009d14,"ax",%progbits
	.global Func_02001d14
	.thumb_func
Func_02001d14:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #238
	sub sp, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d2c
	b .L_02009eb2
.L_02009d2c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #186
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d3c
	b .L_02009eb2
.L_02009d3c:
	movs r0, #12
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	ldrh r3, [r3]
	mov r5, sp
	adds r5, #2
	strh r3, [r5]
	mov r8, r0
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r1, r1, #12
	lsls r2, r2, #9
	lsls r0, r0, #12
	bl Func_02004e24
	movs r0, #107
	bl Func_02005014
	bl Func_02004ddc
	ldrb r3, [r5]
	strh r3, [r5]
	cmp r3, #0
	beq .L_02009dac
.L_02009d7e:
	ldrh r3, [r5]
	ldr r2, .L_02009da8
	movs r1, #128
	lsls r1, r1, #19
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	strh r3, [r5]
	movs r0, #1
	bl wait
	ldrh r3, [r5]
	cmp r3, #0
	bne .L_02009d7e
	b .L_02009dac
	.2byte 0x0000
.L_02009da8:
	.4byte 0x00001000
.L_02009dac:
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #253
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	movs r0, #30
	movs r6, #232
	movs r5, #174
	bl FieldEvent_Wait
	lsls r5, r5, #17
	lsls r6, r6, #17
	movs r3, #68
	adds r3, #255
	adds r2, r5, #0
	ldr r1, .L_02009e88
	adds r0, r6, #0
	bl OverlayObject_PrepareObject
	adds r2, r5, #0
	adds r1, r6, #0
	adds r7, r0, #0
	movs r0, #12
	bl Func_02004ebc
	bl Func_02001c9c
	movs r5, #0
.L_02009dea:
	cmp r5, #10
	bne .L_02009df2
	bl Func_02001c9c
.L_02009df2:
	mov r2, r8
	ldr r3, [r2, #16]
	ldr r2, .L_02009e8c
	movs r0, #2
	adds r3, r3, r2
	mov r2, r8
	str r3, [r2, #16]
	adds r5, #1
	bl FieldEvent_Wait
	cmp r5, #19
	ble .L_02009dea
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #238
	bl GameFlag_SetBit
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004e24
	movs r3, #0
	str r3, [r7, #8]
	str r3, [r7, #16]
	movs r0, #1
	bl FieldEvent_Wait
	adds r0, r7, #0
	bl Func_02004dbc
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02005014
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_02009e7c
	mov r6, sp
	orrs r3, r2
	strh r3, [r1]
	adds r6, #2
	ldrh r3, [r6]
	ldr r2, .L_02009e80
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
	movs r2, #128
	ldr r3, .L_02009e84
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldrh r3, [r6]
	cmp r3, #8
	beq .L_02009eaa
.L_02009e70:
	ldrh r3, [r6]
	ldr r2, .L_02009e80
	movs r1, #128
	lsls r1, r1, #19
	b .L_02009e90
	.2byte 0x0000
.L_02009e7c:
	.4byte 0x00000f00
.L_02009e80:
	.4byte 0x00001000
.L_02009e84:
	.4byte 0x00003f42
.L_02009e88:
	.4byte 0xffe00000
.L_02009e8c:
	.4byte 0xffff0000
.L_02009e90:
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	adds r5, r6, #0
	ldrh r3, [r6]
	movs r0, #1
	adds r3, #1
	strh r3, [r6]
	bl wait
	ldrh r3, [r5]
	cmp r3, #8
	bne .L_02009e70
.L_02009eaa:
	bl Func_02004dd4
	bl Func_02004e64
.L_02009eb2:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a974,"ax",%progbits
	.global Func_02002974
	.thumb_func
Func_02002974:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #10
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r1, #0
	asrs r3, r3, #19
	mov r8, r3
	ldr r3, [r0, #16]
	movs r0, #11
	mov r9, r1
	mov r10, r1
	asrs r7, r3, #19
	bl Actor_GetById
	ldr r3, [r0, #8]
	ldr r2, .L_0200aa10
	asrs r6, r3, #19
	ldr r3, [r0, #16]
	movs r4, #1
	asrs r3, r3, #19
	mov r11, r3
	lsls r1, r4, #24
	mov r12, r2
.L_0200a9b0:
	mov r0, r12
	asrs r2, r1, #22
	ldrsh r3, [r0, r2]
	mov lr, r3
	cmp r8, lr
	bne .L_0200a9c8
	adds r3, r2, #2
	ldrsh r3, [r0, r3]
	cmp r7, r3
	bne .L_0200a9c8
	lsrs r1, r1, #24
	mov r9, r1
.L_0200a9c8:
	lsls r2, r4, #24
	asrs r1, r2, #22
	ldrsh r3, [r0, r1]
	cmp r6, r3
	bne .L_0200a9de
	adds r3, r1, #2
	ldrsh r3, [r0, r3]
	cmp r11, r3
	bne .L_0200a9de
	lsrs r1, r2, #24
	mov r10, r1
.L_0200a9de:
	movs r4, #128
	lsls r4, r4, #17
	adds r3, r2, r4
	lsrs r4, r3, #24
	movs r5, #176
	lsls r1, r4, #24
	lsls r5, r5, #20
	cmp r1, r5
	ble .L_0200a9b0
	mov r1, r10
	mov r2, r9
	lsls r3, r1, #28
	lsls r1, r2, #24
	orrs r1, r3
	movs r0, #132
	asrs r1, r1, #24
	lsls r0, r0, #2
	bl GameFlag_SetByte
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200aa10:
	.4byte gData_0200530c
	.section .text.x0200ab8c,"ax",%progbits
	.balign 4
	.global Func_02002b8c
	.thumb_func
Func_02002b8c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200ac64
	movs r2, #2
	mov r8, r1
	movs r3, #192
	movs r0, #131
	add r2, r8
	lsls r3, r3, #18
	lsls r0, r0, #1
	mov r10, r2
	ldr r7, [r3, #108]
	bl GameFlag_Test
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r6, r0, #0
	cmp r3, #0
	beq .L_0200abc0
	movs r3, #1
	orrs r6, r3
.L_0200abc0:
	movs r5, #8
.L_0200abc2:
	adds r0, r5, #0
	bl Actor_GetById
	adds r5, #1
	adds r0, #91
	strb r6, [r0]
	cmp r5, #16
	ble .L_0200abc2
	cmp r6, #0
	bne .L_0200acac
	mov r1, r10
	ldrh r3, [r1]
	movs r1, #171
	adds r2, r3, #1
	mov r3, r10
	strh r2, [r3]
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200acac
	lsls r3, r2, #16
	cmp r3, #0
	beq .L_0200acac
.L_0200abf4:
	mov r2, r8
	ldrh r3, [r2]
	cmp r3, #60
	beq .L_0200ac02
	cmp r3, #180
	beq .L_0200ac68
	b .L_0200ac7a
.L_0200ac02:
	movs r0, #8
	movs r1, #0
	movs r2, #3
	bl Func_02002b58
	movs r0, #9
	movs r1, #3
	movs r2, #0
	bl Func_02002b58
	movs r5, #3
	movs r0, #10
	movs r1, #0
	movs r2, #3
	bl Func_02002b58
	negs r5, r5
	movs r0, #11
	movs r1, #0
	movs r2, #3
	bl Func_02002b58
	movs r0, #12
	movs r1, #0
	adds r2, r5, #0
	bl Func_02002b58
	movs r0, #13
	movs r1, #0
	adds r2, r5, #0
	bl Func_02002b58
	movs r0, #14
	movs r1, #0
	movs r2, #3
	bl Func_02002b58
	movs r0, #15
	movs r1, #0
	adds r2, r5, #0
	bl Func_02002b58
	movs r0, #16
	movs r1, #3
	movs r2, #0
	bl Func_02002b58
	b .L_0200ac7a
	.2byte 0x0000
.L_0200ac64:
	.4byte gSceneState
.L_0200ac68:
	movs r5, #8
.L_0200ac6a:
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	adds r5, #1
	bl Func_02002b58
	cmp r5, #16
	ble .L_0200ac6a
.L_0200ac7a:
	mov r1, r8
	ldrh r3, [r1]
	mov r2, r8
	adds r3, #1
	movs r1, #240
	strh r3, [r2]
	lsls r1, r1, #16
	lsls r3, r3, #16
	cmp r3, r1
	bne .L_0200ac92
	ldr r3, .L_0200aca8
	strh r3, [r2]
.L_0200ac92:
	mov r1, r10
	ldrh r3, [r1]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200abf4
	b .L_0200acac
.L_0200aca8:
	.4byte 0x00000000
.L_0200acac:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.section .text.x0200acb4,"ax",%progbits
	.global Func_02002cb4
	.thumb_func
Func_02002cb4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsls r0, r0, #24
	lsrs r0, r0, #24
	ldr r5, .L_0200ad24
	mov r8, r0
	movs r0, #10
	adds r0, #255
	adds r6, r5, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ad14
	strh r0, [r5]
	strh r0, [r6]
	bl Func_02004fe4
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r7, #0
	movs r5, #8
	movs r6, #2
.L_0200acec:
	ldr r2, .L_0200ad28
	lsls r3, r7, #2
	ldrsh r1, [r2, r3]
	ldrsh r2, [r2, r6]
	adds r0, r5, #0
	lsls r1, r1, #19
	lsls r2, r2, #19
	bl Func_02004ebc
	mov r0, r8
	lsls r1, r0, #24
	asrs r1, r1, #24
	adds r0, r5, #0
	adds r5, #1
	bl Func_02002b04
	adds r6, #4
	adds r7, #1
	cmp r5, #16
	ble .L_0200acec
.L_0200ad14:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200ad2c
	bl Task_Add
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200ad24:
	.4byte gSceneState
.L_0200ad28:
	.4byte gData_0200533c
.L_0200ad2c:
	.4byte Func_02002b8c
	.section .text.x0200af30,"ax",%progbits
	.balign 4
	.ifdef TLA_EDITION_JA
	.global Func_02002f30
	.thumb_func
Func_02002f30:
	.4byte 0x465fb5e0
	.4byte 0x464d4656
	.4byte 0x4647b4e0
	.4byte 0x23c0b480
	.4byte 0x6edb049b
	.4byte 0x004020d6
	.4byte 0x181b2281
	.4byte 0x4faa0092
	.4byte 0x21f0601a
	.4byte 0x187b0049
	.4byte 0x5e1a2000
	.4byte 0xb0874ba7
	.4byte 0xd001429a
	.endif
	.section .text.x0200af64,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02002f64
	.thumb_func
Func_02002f64:
	.2byte 0xf000, 0xff17 @ bl 0x0200bb96
	.4byte 0x187d3102
	.4byte 0x5eab2200
	.4byte 0xdd072b08
	.4byte 0xf0012008
	.4byte 0x2100ff6f
	.4byte 0xff3cf001
	.4byte 0xfed6f7ff
	.4byte 0x5e2b2000
	.4byte 0xd0002b14
	.4byte 0xf001e0a7
	.4byte 0x2000ff57
	.4byte 0xf80cf002
	.4byte 0x21012001
	.4byte 0x42402201
	.4byte 0x42524249
	.4byte 0xf0012300
	.4byte 0x2185ffc3
	.4byte 0x187b0089
	.4byte 0x21006818
	.4byte 0xf0012200
	.4byte 0x2009ff73
	.4byte 0x22002100
	.4byte 0xff6ef001
	.4byte 0x22002100
	.4byte 0xf001200a
	.4byte 0x2009ff69
	.4byte 0xff52f001
	.4byte 0xf001200a
	.4byte 0xf001ff4f
	.4byte 0xf001ffd9
	.4byte 0x203cffdf
	.4byte 0xff28f001
	.4byte 0x00802082
	.4byte 0x251430ff
	.4byte 0xfeaef001
	.4byte 0xd00a2800
	.4byte 0x00802082
	.4byte 0xf00130ff
	.4byte 0x2090feaf
	.4byte 0x30bc0100
	.4byte 0xfea6f001
	.4byte 0x20c22515
	.4byte 0xf0010080
	.4byte 0x2800fe9d
	.4byte 0x20c2d009
	.4byte 0xf0010080
	.4byte 0x2090fe9f
	.4byte 0x30bd0100
	.4byte 0xfe96f001
	.4byte 0x20c02516
	.4byte 0x30090080
	.4byte 0xfe8cf001
	.4byte 0xd00a2800
	.4byte 0x008020c0
	.4byte 0xf0013009
	.4byte 0x2090fe8d
	.4byte 0x30be0100
	.4byte 0xfe84f001
	.4byte 0x20c02517
	.4byte 0x300a0080
	.4byte 0xfe7af001
	.4byte 0xd00a2800
	.4byte 0x008020c0
	.4byte 0xf001300a
	.4byte 0x208cfe7b
	.4byte 0x30ff0100
	.4byte 0xfe72f001
	.4byte 0x208d2518
	.4byte 0xf0010080
	.4byte 0xf7ffffb9
	.4byte 0x200ffe57
	.4byte 0xfed8f001
	.4byte 0x20082103
	.4byte 0xff10f001
	.4byte 0x00802080
	.4byte 0xf0013012
	.4byte 0x2080ffab
	.4byte 0x22802180
	.4byte 0x02890280
	.4byte 0xf0010252
	.4byte 0x22e6feaf
	.4byte 0x21012001
	.4byte 0x32660212
	.4byte 0x42404249
	.4byte 0xfea6f001
	.4byte 0xf0012078
	.4byte 0x2101febb
	.4byte 0xf0012008
	.4byte 0x203cfef3
	.4byte 0xfeb4f001
	.4byte 0x30ff200a
	.4byte 0xfe40f001
	.4byte 0xf0011c28
	.4byte 0x4b46ff39
	.4byte 0x005222f1
	.4byte 0x881b189b
	.4byte 0x3b092080
	.4byte 0x0240041b
	.4byte 0xd8374283
	.4byte 0x004020a2
	.4byte 0xfe2ef001
	.4byte 0x01002090
	.4byte 0xf0013027
	.4byte 0x2800fe25
	.4byte 0x2008d017
	.4byte 0xf0012104
	.4byte 0x231efecf
	.4byte 0x93002213
	.4byte 0x20559201
	.4byte 0x22052148
	.4byte 0xf0012302
	.4byte 0x2009fe61
	.4byte 0x22002100
	.4byte 0xfeb8f001
	.4byte 0x2100200a
	.4byte 0xf0012200
	.4byte 0x2090feb3
	.4byte 0x30ee0100
	.4byte 0xfe06f001
	.4byte 0xd00d2800
	.4byte 0x01002090
	.4byte 0xf00130ba
	.4byte 0x2800fdff
	.4byte 0x21e8d106
	.4byte 0x200c22a4
	.4byte 0x04520449
	.4byte 0xfe9ef001
	ldr r7, .L_ja_0200aff8
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r7, r1
	.2byte 0x2200
	ldrsh r3, [r3, r2]
	.4byte 0xd0002b09
	.4byte 0x2090e2be
	.4byte 0x30270100
	.4byte 0xfde8f001
	.4byte 0xd0012800
	.4byte 0xf857f001
	.4byte 0xf0012035
	.4byte 0x20c4ff31
	.4byte 0xf00130ff
	.4byte 0x2501fe4d
	.4byte 0x42a8426d
	.4byte 0xe28ad100
	.4byte 0x01002090
	.4byte 0xf00130ac
	.4byte 0x4682fdd3
	.4byte 0xd0272800
	.4byte 0x21022008
	.4byte 0xfe7cf001
	.4byte 0x21002009
	.4byte 0xf0012200
	.4byte 0x21d8fe6f
	.4byte 0x045222a0
	.4byte 0x0449200a
	.4byte 0xfe68f001
	.4byte 0x2103200a
	.4byte 0xfe98f001
	.4byte 0x2213231b
	.4byte 0x92019300
	.4byte 0x2113201e
	.4byte 0x23012201
	.4byte 0xfdfaf001
	.4byte 0x2105200a
	.4byte 0xfe5ef001
	.4byte 0xf81ff001
.L_ja_0200aff8:
	.4byte gMainRoutineWork
	.4byte 0x000000b1
	.4byte 0xf001200a
	.4byte 0x4683fe29
	.4byte 0xf0012008
	.4byte 0x4680fe25
	.4byte 0x004020c3
	.4byte 0xfeecf001
	.4byte 0x01002090
	.4byte 0xf00130ac
	.4byte 0x2090fd9b
	.4byte 0x30de0100
	.4byte 0xfd96f001
	.4byte 0xfe08f001
	.4byte 0xf0012000
	.4byte 0x2200febd
	.4byte 0x21002009
	.4byte 0xfe30f001
	.4byte 0x21012009
	.4byte 0xfe10f001
	.4byte 0x22b421e4
	.4byte 0x200a0452
	.4byte 0xf0010449
	.4byte 0x200afe25
	.4byte 0xf0012101
	.4byte 0x21e0fe05
	.4byte 0x200a2200
	.4byte 0xf0010209
	.4byte 0x2008fe4b
	.4byte 0xf0012102
	.4byte 0xf001fe1f
	.4byte 0xf001fe8d
	.4byte 0x20f4fe93
	.4byte 0x230122c0
	.4byte 0x04401c29
	.4byte 0xf0010452
	.4byte 0xf001fe53
	.4byte 0x2184fe55
	.4byte 0x00492200
	.4byte 0xf001200a
	.4byte 0x2028fe3b
	.4byte 0xfdccf001
	.4byte 0xf00148b2
	.4byte 0x200afe25
	.4byte 0xf0012100
	.4byte 0x200afe25
	.4byte 0xf0012103
	.4byte 0x200afe01
	.4byte 0xf0012100
	.4byte 0x200afe1d
	.4byte 0xf0012108
	.4byte 0x20c0fdf5
	.4byte 0x300a01c0
	.4byte 0xf0012100
	.4byte 0x23c0fe13
	.4byte 0x4658025b
	.4byte 0x22a421f4
	.4byte 0x00526303
	.4byte 0x200a0049
	.4byte 0xfdccf001
	.4byte 0xf0012005
	.4byte 0x2106fda5
	.4byte 0xf001200a
	.4byte 0x200afddd
	.4byte 0xfd9ef001
	.4byte 0x465a21c0
	.4byte 0x629102c9
	.4byte 0xf001200a
	.4byte 0x235bfd97
	.4byte 0x4699445b
	.4byte 0x23014647
	.4byte 0x4648375b
	.4byte 0x21007003
	.4byte 0x4658703b
	.4byte 0xfd32f001
	.4byte 0x21004640
	.4byte 0xfd2ef001
	.4byte 0x21064658
	.4byte 0xfd72f001
	.4byte 0x21064640
	.4byte 0xfd6ef001
	.4byte 0x2205251e
	.4byte 0x26132302
	.4byte 0x2055214c
	.4byte 0x96019500
	.4byte 0xfd4cf001
	.4byte 0x0080208f
	.4byte 0xfe4cf001
	.4byte 0x01c020fe
	.4byte 0x30ff2100
	.4byte 0xfe0af001
	.4byte 0xf001200a
	.4byte 0x200afe0f
	.4byte 0xfd64f001
	.4byte 0x21002080
	.4byte 0xf0010240
	.4byte 0x200afdff
	.4byte 0xfe04f001
	.4byte 0xf001200a
	.4byte 0x203cfd59
	.4byte 0xfd56f001
	.4byte 0x21004658
	.4byte 0xfd42f001
	.4byte 0x21004640
	.4byte 0xfd3ef001
	.4byte 0x20552302
	.4byte 0x2205214a
	.4byte 0x96019500
	.4byte 0xfd1ef001
	.4byte 0x4651464a
	.4byte 0x46587011
	.4byte 0x21107039
	.4byte 0xfce6f001
	.4byte 0x46402110
	.4byte 0xfce2f001
	.4byte 0xf001200a
	.4byte 0x305afd47
	.4byte 0x25fe7802
	.4byte 0x40131c2b
	.4byte 0x22b421e4
	.4byte 0x70030052
	.4byte 0x200a0049
	.4byte 0xf0012680
	.4byte 0x02f6fd51
	.4byte 0x2181465b
	.4byte 0x0049629e
	.4byte 0xf001200a
	.4byte 0x200afd91
	.4byte 0xfd4ef001
	.4byte 0x22142102
	.4byte 0x200a31ff
	.4byte 0xfd84f001
	.4byte 0xf001201e
	.4byte 0x2108fd15
	.4byte 0xf001200a
	.4byte 0x200afd4d
	.4byte 0xfd0ef001
	.4byte 0x024020c0
	.4byte 0x63084659
	.4byte 0x21f422a4
	.4byte 0x00520049
	.4byte 0xf001200a
	.4byte 0x200efd27
	.4byte 0xfd00f001
	.4byte 0x02d222c0
	.4byte 0x629a465b
	.4byte 0x200a2106
	.4byte 0xfd34f001
	.4byte 0xf001200a
	.4byte 0x208ffcf5
	.4byte 0xf0010080
	.4byte 0x200afdcd
	.4byte 0xfcfef001
	.4byte 0x7803305a
	.4byte 0x401d21e4
	.4byte 0x005222b4
	.4byte 0x00497005
	.4byte 0xf001200a
	.4byte 0x4658fd0b
	.4byte 0x00492181
	.4byte 0x200a6286
	.4byte 0xfd4cf001
	.4byte 0xf001200a
	.4byte 0x2100fd09
	.4byte 0xf001200a
	.4byte 0x200ffd35
	.4byte 0xfcd2f001
	.4byte 0x46402100
	.4byte 0xfc76f001
	.4byte 0xf001200f
	.4byte 0x20d7fccb
	.4byte 0xfda4f001
	.4byte 0x21f4201e
	.4byte 0x4a314b30
	.4byte 0x30ff0449
	.4byte 0xfc70f001
	.4byte 0x1c052100
	.4byte 0xfc9cf001
	.4byte 0x492d1c28
	.4byte 0xfc64f001
	.4byte 0x21011c28
	.4byte 0xfc58f001
	.4byte 0x21061c28
	.4byte 0xfca0f001
	.4byte 0x22002102
	.4byte 0xf001200a
	.4byte 0x2005fcef
	.4byte 0xfca8f001
	.4byte 0x00492181
	.4byte 0x200a2200
	.4byte 0xfd0ef001
	.4byte 0xf0012037
	.4byte 0x2005fc9f
	.4byte 0xfc9cf001
	.4byte 0xf0012088
	.4byte 0x20cefd75
	.4byte 0x4a1a21f4
	.4byte 0x04494b18
	.4byte 0xf0010040
	.4byte 0x2100fc41
	.4byte 0xf0011c05
	.4byte 0x4917fc6d
	.4byte 0xf0011c28
	.4byte 0x6d2afc35
	.4byte 0x490f2380
	.4byte 0x8253019b
	.4byte 0x1c2b22c0
	.4byte 0x33550252
	.4byte 0x7019632a
	.4byte 0x045b23e4
	.4byte 0x4b0f63ab
	.4byte 0x63eb200a
	.4byte 0x045b23b3
	.4byte 0x23c0642b
	.4byte 0x61ab01db
	.4byte 0xf00161eb
	.4byte 0x2110fc6f
	.4byte 0xf0014640
	.4byte 0x200afc13
	.4byte 0x0000e00e
	.4byte 0x00000000
	.4byte 0x00002430
	.4byte 0x01590000
	.4byte 0xfff00000
	.4byte gData_0200533c + 0x60
	.4byte gData_0200533c + 0x24
	.4byte 0xffe00000
	.4byte 0xfc58f001
	.4byte 0xf0012085
	.4byte 0x4653fd31
	.4byte 0x60ab2280
	.4byte 0x1c30612b
	.4byte 0x02521c31
	.4byte 0xfc34f001
	.4byte 0x21082700
	.4byte 0x2f014249
	.4byte 0x2108dd00
	.4byte 0x22082301
	.4byte 0x4252403b
	.4byte 0xd1002b00
	.4byte 0x20e42208
	.4byte 0x04130440
	.4byte 0x22b30409
	.4byte 0x04521809
	.4byte 0x189b2094
	.4byte 0x4a4430ff
	.4byte 0xfbe2f001
	.4byte 0xae0300bd
	.4byte 0x21025170
	.4byte 0xfbd0f001
	.4byte 0x21005970
	.4byte 0xfc08f001
	.4byte 0x59703701
	.4byte 0xf001493d
	.4byte 0x2f03fbcf
	.4byte 0x23a0ddd5
	.4byte 0x4658031b
	.4byte 0x200a6283
	.4byte 0xfc2af001
	.4byte 0x7802305a
	.4byte 0x401323fe
	.4byte 0x22a021d8
	.4byte 0x00497003
	.4byte 0x200a0052
	.4byte 0xfc36f001
	.4byte 0xf001200a
	.4byte 0x22e6fc0b
	.4byte 0x21012001
	.4byte 0x32660212
	.4byte 0x42494240
	.4byte 0xfbeaf001
	.4byte 0x200a2105
	.4byte 0xfc3af001
	.4byte 0xf001201e
	.4byte 0x200afbfb
	.4byte 0xf0012101
	.4byte 0xf001fc7b
	.4byte 0x4d26fc75
	.4byte 0x1c29200a
	.4byte 0xfc08f001
	.4byte 0xf001200a
	.4byte 0x2014fc09
	.4byte 0xfbeaf001
	.4byte 0x200a1c29
	.4byte 0xfbfef001
	.4byte 0xf0012014
	.4byte 0x20c0fbe3
	.4byte 0x300a01c0
	.4byte 0xf0012100
	.4byte 0x4b1bfc3d
	.4byte 0x00892185
	.4byte 0x6818185b
	.4byte 0xf0012101
	.4byte 0xf001fc49
	.4byte 0x200afc53
	.4byte 0xf0012103
	.4byte 0x231bfc37
	.4byte 0x93002213
	.4byte 0x201e9201
	.4byte 0x22012113
	.4byte 0xf0012301
	.4byte 0xf001fb99
	.4byte 0xf000fbcb
	.4byte 0x200afdc0
	.4byte 0xf00130ff
	.4byte 0x2800fb49
	.4byte 0xf000d001
	.4byte 0x21ecfdb8
	.4byte 0x200a22e4
	.4byte 0x04520449
	.4byte 0xfbe6f001
	.4byte 0x200a4906
	.4byte 0xfbc6f001
	.4byte 0xfdabf000
	.4byte 0xffe00000
	.4byte gData_0200533c + 0x60
	.4byte gData_02005e28
	.4byte gMainRoutineWork
	.4byte gData_02005728 + 0x64
	.4byte 0xd0002b0a
	.4byte 0x2007e2d0
	.4byte 0xfc78f001
	.4byte 0x01002090
	.4byte 0xf0013027
	.2byte 0xfb23
	mov r8, r0
	.4byte 0xd0012800
	.4byte 0xfd91f000
	.4byte 0xfb94f001
	.4byte 0xf0012000
	.4byte 0x2090fc49
	.4byte 0x30270100
	.4byte 0xfb18f001
	.4byte 0xf00148e3
	.4byte 0x2200fbe1
	.4byte 0x21002009
	.4byte 0xfbb4f001
	.4byte 0x20082103
	.4byte 0xfbb8f001
	.4byte 0xf001200b
	.4byte 0x2280fb89
	.4byte 0x63020252
	.4byte 0x4692200a
	.4byte 0xfb82f001
	.4byte 0x1c052680
	.4byte 0x21dc0236
	.2byte 0x229c
	.2byte 0x200b
	.4byte 0x04520449
	.2byte 0x632e
	.2byte 0xf001
	.2byte 0xfb9b
	movs r3, #133
	lsls r3, r3, #2
	adds r7, r7, r3
	.4byte 0x22a421e4
	.4byte 0x04496838
	.4byte 0xf0010452
	.4byte 0x21f4fb91
	.4byte 0x200a22d4
	.4byte 0x04520449
	.4byte 0xfb8af001
	.4byte 0x21006838
	.4byte 0xf0012200
	.4byte 0x200bfbb5
	.4byte 0x22002100
	.4byte 0xfbb0f001
	.4byte 0x200a21c0
	.4byte 0x22000209
	.4byte 0xfbaaf001
	.4byte 0xfbf0f001
	.4byte 0xfbf6f001
	.4byte 0x221e2181
	.4byte 0x200b0049
	.4byte 0xfba8f001
	.4byte 0x00802080
	.4byte 0xf0013012
	.4byte 0x2100fc13
	.4byte 0xf001200b
	.4byte 0x2080fb93
	.4byte 0x30120080
	.4byte 0xfc0af001
	.4byte 0x2100200a
	.4byte 0xfb8af001
	.4byte 0x00492180
	.4byte 0x68382200
	.4byte 0xfb90f001
	.4byte 0x00492180
	.4byte 0x200b2200
	.4byte 0xfb8af001
	.4byte 0xf001200f
	.4byte 0x2180fb1b
	.4byte 0x01c96838
	.4byte 0xf0012200
	.4byte 0x2180fb79
	.4byte 0x01c92200
	.4byte 0xf001200b
	.4byte 0x201efb73
	.4byte 0xfb0cf001
	.4byte 0x2101200a
	.4byte 0xfb8cf001
	.4byte 0xfb86f001
	.4byte 0x2102200a
	.4byte 0xfb4af001
	.4byte 0x2101200a
	.4byte 0xfb72f001
	.4byte 0x22bc21f4
	.4byte 0x00490052
	.4byte 0xf001200a
	.4byte 0x200afb1f
	.4byte 0xfb24f001
	.4byte 0x200a2102
	.4byte 0xfb38f001
	.4byte 0xf0012014
	.4byte 0x21ecfaed
	.4byte 0x004922b4
	.4byte 0x200a0052
	.4byte 0xfb0ef001
	.4byte 0xf001200a
	.4byte 0x6838fb13
	.4byte 0x2200210a
	.4byte 0xfb2ef001
	.4byte 0x210a200b
	.4byte 0xf0012200
	.4byte 0x200afb29
	.4byte 0x22002108
	.4byte 0xfb24f001
	.4byte 0x21012001
	.4byte 0x42522201
	.4byte 0x42402300
	.4byte 0xf0014249
	.4byte 0x200afb47
	.4byte 0xf0012108
	.4byte 0x200afb03
	.4byte 0xf0012100
	.4byte 0x23c0fb23
	.4byte 0x21f4025b
	.4byte 0x005222a4
	.4byte 0x0049632b
	.4byte 0xf001200a
	.4byte 0x2005fadd
	.4byte 0xfab6f001
	.4byte 0x200a2106
	.4byte 0xfaeef001
	.4byte 0xf001200a
	.4byte 0x23c0faaf
	.4byte 0x62ab02db
	.4byte 0xf001200a
	.4byte 0x208ffaa9
	.4byte 0xf0010080
	.4byte 0x1c2afb81
	.4byte 0x325b20fe
	.4byte 0x01c02301
	.4byte 0x21007013
	.4byte 0xf00130ff
	.4byte 0x200afb3b
	.4byte 0xfb40f001
	.4byte 0xf001200a
	.4byte 0x2080fa95
	.4byte 0x30120080
	.4byte 0xfb6cf001
	.4byte 0x46502100
	.4byte 0xfb2cf001
	.4byte 0xf001200a
	.4byte 0x200afb31
	.4byte 0xfa86f001
	.4byte 0x01c020fe
	.4byte 0x30ff2100
	.4byte 0xfb20f001
	.4byte 0xf001200a
	.4byte 0x200afb25
	.4byte 0xfa7af001
	.4byte 0x46502100
	.4byte 0xfb16f001
	.4byte 0xf001200a
	.4byte 0x200afb1b
	.4byte 0xfa70f001
	.4byte 0x01c020fe
	.4byte 0x30ff2100
	.4byte 0xfb0af001
	.4byte 0xf001200a
	.4byte 0x203cfb0f
	.4byte 0xfa64f001
	.4byte 0x21042008
	.4byte 0xfa9cf001
	.4byte 0x2213231e
	.4byte 0x92019300
	.4byte 0x21482055
	.4byte 0x23022205
	.4byte 0xfa2ef001
	.2byte 0x4b4b
	mov r0, r8
	.2byte 0x21e4
	.2byte 0x22b4
	str r3, [r5, #12]
	.2byte 0x0452
	str r0, [r5, #40]
	.2byte 0x0449
	movs r0, #10
	.2byte 0x632e
	.4byte 0xfa7ef001
	.4byte 0x2101200a
	.4byte 0xfa82f001
	.4byte 0x46502100
	.4byte 0xfae2f001
	.4byte 0xf001200a
	.4byte 0x200afae7
	.4byte 0xfa3cf001
	.4byte 0x68392200
	.4byte 0xf001200a
	.4byte 0x200afa87
	.4byte 0xf0012100
	.4byte 0x200afa93
	.4byte 0xf0012103
	.4byte 0x200afa6f
	.4byte 0xf0012100
	.4byte 0x2180fa8b
	.4byte 0x200a2200
	.4byte 0xf00101c9
	.4byte 0x200afa89
	.4byte 0xf0012102
	.4byte 0x21f4fa5d
	.4byte 0x005222c4
	.4byte 0x0049200a
	.4byte 0xfa42f001
	.4byte 0x200b2101
	.4byte 0xfa5ef001
	.4byte 0xf0012005
	.4byte 0x2100fa13
	.4byte 0xf001200b
	.4byte 0x200afa6f
	.4byte 0xfa3cf001
	.4byte 0x2100200a
	.4byte 0xfa44f001
	.4byte 0x210b2200
	.4byte 0xf001200a
	.4byte 0x200ffa53
	.4byte 0xfa00f001
	.4byte 0x2100200a
	.4byte 0xfa5cf001
	.4byte 0x2103200a
	.4byte 0xfa38f001
	.4byte 0x22002180
	.4byte 0x01c9200a
	.4byte 0xfa56f001
	.4byte 0x2102200a
	.4byte 0xfa2af001
	.4byte 0x228621f4
	.4byte 0x200a0092
	.4byte 0xf0010049
	.4byte 0x6838fa0f
	.4byte 0xf0012101
	.4byte 0x6838fa57
	.4byte 0x2200210b
	.4byte 0xfa2ef001
	.4byte 0x68392200
	.4byte 0xf001200b
	.4byte 0x201efa29
	.4byte 0xf9d6f001
	.4byte 0x2103200b
	.4byte 0xfa12f001
	.4byte 0x2102200b
	.4byte 0xfa0af001
	.4byte 0xf0016838
	.4byte 0x2800f9db
	.4byte 0x220ad00c
	.4byte 0x23125e81
	.4byte 0x200b5ec2
	.4byte 0xf9e6f001
	.4byte 0x0000e004
	.4byte 0x00002439
	.4byte 0xffe00000
	.4byte 0xf001200b
	.4byte 0x2100f9e9
	.4byte 0x200b2200
	.4byte 0xf9e8f001
	.4byte 0xf001200b
	.4byte 0x200af9e1
	.4byte 0x22002100
	.4byte 0xf9e0f001
	.4byte 0x22bc21f4
	.4byte 0x00520049
	.4byte 0xf0016838
	.4byte 0x6838f9cd
	.4byte 0xf9d2f001
	.4byte 0x21a020a0
	.4byte 0x465202c9
	.4byte 0xf00102c0
	.4byte 0x206bf983
	.4byte 0xfa74f001
	.4byte 0xf001201e
	.4byte 0x2180f995
	.4byte 0x22000049
	.4byte 0xf0016838
	.4byte 0x200ff9fb
	.4byte 0xf98cf001
	.4byte 0x020921c0
	.4byte 0x68382200
	.4byte 0xf9eaf001
	.4byte 0xf001200f
	.4byte 0x200cf983
	.4byte 0xf990f001
	.4byte 0x04db2380
	.4byte 0x881b3352
	.4byte 0x350a466d
	.4byte 0x21a04680
	.4byte 0x802b20a0
	.4byte 0x46520309
	.4byte 0xf0010300
	.4byte 0x206bf959
	.4byte 0xfa4af001
	.4byte 0xf930f001
	.4byte 0x802b782b
	.4byte 0xd0152b00
	.4byte 0x4a09882b
	.4byte 0x04c92180
	.4byte 0x31524313
	.4byte 0x20ff800b
	.4byte 0x0200882b
	.4byte 0x181b30ff
	.4byte 0x2001802b
	.4byte 0xf8c0f001
	.4byte 0x2b00882b
	.4byte 0xe001d1ec
	.4byte 0x00001000
	.4byte 0x04c92180
	.4byte 0x23fd880a
	.4byte 0x33ff021b
	.4byte 0x800b4013
	.4byte 0x26e8201e
	.4byte 0xf00125ae
	.4byte 0x046df945
	.4byte 0x23440476
	.4byte 0x492e1c2a
	.4byte 0x1c3033ff
	.4byte 0xfb60f7fc
	.4byte 0x1c071c2a
	.4byte 0x200c1c31
	.4byte 0xf96af001
	.4byte 0x1c282500
	.4byte 0xf0012105
	.4byte 0x2800f895
	.4byte 0xf7fed101
	.4byte 0x4641f85f
	.4byte 0x4a24690b
	.4byte 0x189b2002
	.4byte 0x3501610b
	.4byte 0xf924f001
	.4byte 0xddec2d13
	.4byte 0x01002090
	.4byte 0xf00130ee
	.4byte 0x22e6f8ad
	.4byte 0x21012001
	.4byte 0x42490212
	.4byte 0x42403266
	.4byte 0xf8fcf001
	.4byte 0x60bb2300
	.4byte 0x2001613b
	.4byte 0xf90ef001
	.4byte 0xf0011c38
	.4byte 0x20c3f8bf
	.4byte 0xf0010040
	.4byte 0x201ef9e3
	.4byte 0xf904f001
	.4byte 0x04c92180
	.4byte 0x4a0b880b
	.4byte 0x4313466e
	.4byte 0x360a800b
	.4byte 0x4a098833
	.4byte 0x43133152
	.4byte 0x2280800b
	.4byte 0x04d24b07
	.4byte 0x80133250
	.4byte 0x2b088833
	.4byte 0x8833d01b
	.4byte 0x21804a02
	.4byte 0xe00904c9
	.4byte 0x00000f00
	.4byte 0x00001000
	.4byte 0x00003f42
	.4byte 0xffe00000
	.4byte 0xffff0000
	.4byte 0x31524313
	.4byte 0x1c35800b
	.4byte 0x20018833
	.4byte 0x80333301
	.4byte 0xf83ef001
	.4byte 0x2b08882b
	.4byte 0xf001d1e3
	.4byte 0xf001f891
	.4byte 0xe2ccf8d7
	.4byte 0xd1342b0b
	.4byte 0xf8cef001
	.4byte 0xf0012000
	.2byte 0xf983
	.2byte 0x2009
	.4byte 0x22002100
	.4byte 0xf8f6f001
	.4byte 0x22ac21e0
	.4byte 0x0449200a
	.2byte 0x0452
	.2byte 0xf001
	.2byte 0xf8ef
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r7, r3
	.4byte 0x68292200
	.4byte 0xf001200a
	.4byte 0x2113f903
	.4byte 0xf0016828
	.4byte 0xf001f8eb
	.4byte 0xf001f959
	.4byte 0x48e9f95f
	bl Func_02004f0c
	movs r1, #0
	movs r0, #10
	bl Func_02004f14
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #242
	lsls r0, r0, #1
	movs r2, #243
	lsls r2, r2, #1
	adds r3, r7, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	adds r3, r7, r2
	.4byte 0x2b0ce03e
	.4byte 0xe292d000
	.4byte 0xf896f001
	.4byte 0xf0012000
	.4byte 0x2009f94b
	.4byte 0x22002100
	.4byte 0xf8bef001
	.4byte 0x22a021d7
	.4byte 0x200a0452
	.4byte 0xf0010449
	.4byte 0x200af8b7
	.4byte 0xf0012105
	.2byte 0xf8bb
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r7, r0
	.4byte 0x21136818
	.4byte 0xf8b4f001
	.4byte 0xf922f001
	.4byte 0xf928f001
	.4byte 0xf001201e
	.4byte 0x2102f871
	.4byte 0xf001200a
	.4byte 0x201ef8b5
	.4byte 0xf86af001
	.4byte 0xf00148c9
	.2byte 0xf8c3
	.2byte 0x200a
	movs r1, #0
	.2byte 0xf001
	.4byte 0xf001f8c3
	.4byte 0xf001f911
	.2byte 0xf913
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r7, r1
	adds r1, #2
	movs r2, #0
	ldrsh r0, [r3, r2]
	adds r3, r7, r1
	.4byte 0x5e992200
	.4byte 0xf8dcf001
	.4byte 0xf85af001
	.2byte 0xe24f
	ldr r3, .L_ja_0200be90
	cmp r2, r3
	bne .L_ja_0200bc80
	movs r0, #8
	bl Actor_GetById
	adds r2, r0, #0
	adds r2, #35
	ldrb r3, [r2]
	movs r5, #32
	orrs r3, r5
	strb r3, [r2]
	movs r1, #3
	bl Func_02004e3c
	movs r0, #9
	bl Actor_GetById
	adds r2, r0, #0
	adds r2, #35
	ldrb r3, [r2]
	movs r1, #3
	orrs r5, r3
	strb r5, [r2]
	bl Func_02004e3c
	movs r0, #8
	movs r1, #12
	bl Func_02002ac8
	movs r0, #9
	movs r1, #12
	bl Func_02002ac8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #98
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bc06
	movs r1, #220
	movs r2, #172
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02004ebc
	movs r1, #138
	movs r2, #172
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02004ebc
	b .L_ja_0200bc2c
.L_ja_0200bc06:
	movs r3, #89
	movs r5, #40
	str r3, [sp, #0]
	movs r0, #88
	movs r1, #40
	movs r2, #1
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02004df4
	movs r3, #100
	str r3, [sp, #0]
	movs r0, #99
	movs r1, #40
	movs r2, #1
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02004df4
.L_ja_0200bc2c:
	movs r0, #132
	lsls r0, r0, #2
	bl GameFlag_GetByte
	adds r5, r0, #0
	movs r0, #132
	movs r3, #15
	lsls r0, r0, #2
	ands r5, r3
	bl GameFlag_GetByte
	asrs r0, r0, #4
	lsls r0, r0, #24
	lsrs r6, r0, #24
	cmp r5, #0
	beq .L_ja_0200bc60
	ldr r2, .L_ja_0200be94
	lsls r3, r5, #2
	ldrsh r1, [r2, r3]
	adds r3, #2
	ldrsh r2, [r2, r3]
	lsls r1, r1, #19
	lsls r2, r2, #19
	movs r0, #10
	bl Func_02004ebc
.L_ja_0200bc60:
	lsls r3, r6, #24
	asrs r0, r3, #24
	cmp r0, #0
	bne .L_ja_0200bc6a
	b .L_ja_0200c036
.L_ja_0200bc6a:
	ldr r2, .L_ja_0200be94
	lsls r3, r0, #2
	ldrsh r1, [r2, r3]
	adds r3, #2
	ldrsh r2, [r2, r3]
	lsls r1, r1, #19
	lsls r2, r2, #19
	movs r0, #11
	bl Func_02004ebc
	b .L_ja_0200c036
.L_ja_0200bc80:
	ldr r3, .L_ja_0200be98
	cmp r2, r3
	bne .L_ja_0200bc9e
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #99
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200bc96
	b .L_ja_0200c036
.L_ja_0200bc96:
	movs r0, #1
	bl Func_02002cb4
	b .L_ja_0200c036
.L_ja_0200bc9e:
	ldr r3, .L_ja_0200be9c
	cmp r2, r3
	beq .L_ja_0200bca6
	b .L_ja_0200befa
.L_ja_0200bca6:
	movs r0, #10
	bl Actor_GetById
	movs r1, #3
	adds r6, r0, #0
	bl Func_02004e3c
	movs r0, #8
	movs r1, #10
	bl Func_02002ac8
	movs r0, #9
	movs r1, #11
	bl Func_02002ac8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #188
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200bce6
	movs r3, #26
	movs r2, #77
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #77
	movs r2, #1
	movs r3, #15
	bl Func_02004df4
.L_ja_0200bce6:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #189
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bd3a
	movs r1, #240
	movs r2, #176
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004ebc
	movs r3, #85
	str r3, [sp, #4]
	movs r5, #28
	movs r0, #2
	movs r1, #93
	movs r2, #1
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02004df4
	movs r6, #22
	movs r0, #27
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004dfc
	movs r3, #29
	str r3, [sp, #0]
	movs r0, #32
	movs r1, #16
	movs r2, #2
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02004dfc
.L_ja_0200bd3a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #190
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bd8e
	movs r1, #152
	movs r2, #176
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02004ebc
	movs r3, #85
	str r3, [sp, #4]
	movs r5, #36
	movs r0, #2
	movs r1, #93
	movs r2, #1
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02004df4
	movs r6, #22
	movs r0, #35
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004dfc
	movs r3, #37
	str r3, [sp, #0]
	movs r0, #32
	movs r1, #16
	movs r2, #2
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02004dfc
.L_ja_0200bd8e:
	movs r0, #140
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bdc4
	movs r3, #37
	movs r2, #26
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #61
	movs r1, #0
	movs r2, #3
	movs r3, #3
	bl Func_02004e04
	movs r3, #38
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02004dfc
.L_ja_0200bdc4:
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #6
	beq .L_ja_0200bdd4
	b .L_ja_0200bede
.L_ja_0200bdd4:
	movs r3, #60
	mov r8, r3
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Camera_MoveWithinMapBounds
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r7, r0
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	bl Event_StartScreenTransition
	movs r0, #15
	bl FieldEvent_Wait
	movs r1, #168
	movs r2, #192
	lsls r2, r2, #17
	lsls r1, r1, #18
	movs r0, #11
	bl Func_02004ebc
	movs r0, #11
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #11
	bl Actor_GetById
	adds r6, r0, #0
	movs r0, #162
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	lsls r0, r0, #1
	bl Import_ActorCreate
	adds r5, r0, #0
	cmp r5, #0
	beq .L_ja_0200be7c
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r1, .L_ja_0200bea0
	adds r0, r5, #0
	bl Func_02004dac
	adds r0, r5, #0
	movs r1, #1
	bl Func_02004d9c
	movs r1, #14
	adds r0, r5, #0
	bl Animation_ApplyChildValues
	ldr r1, [r5, #80]
	movs r2, #12
	ldrb r3, [r1, #9]
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r3, .L_ja_0200bea4
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_ja_0200be7c:
	ldr r3, [r6, #12]
	movs r1, #128
	lsls r1, r1, #16
	adds r3, r3, r1
	str r3, [r6, #12]
	b .L_ja_0200beae
	.4byte 0x0000242f
	.4byte 0x00002438
.L_ja_0200be90:
	.4byte 0x000000b2
.L_ja_0200be94:
	.4byte gData_0200530c
.L_ja_0200be98:
	.4byte 0x000000b4
.L_ja_0200be9c:
	.4byte 0x000000b3
.L_ja_0200bea0:
	.4byte gData_020053d8
.L_ja_0200bea4:
	.4byte 0x00013333
.L_ja_0200bea8:
	movs r2, #1
	negs r2, r2
	add r8, r2
.L_ja_0200beae:
	mov r3, r8
	cmp r3, #0
	beq .L_ja_0200bec4
	movs r0, #1
	bl wait
	ldr r3, [r6, #40]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	ble .L_ja_0200bea8
.L_ja_0200bec4:
	movs r0, #188
	bl Func_02005014
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #3
	bl Func_02004f6c
.L_ja_0200bede:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #196
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200beee
	b .L_ja_0200c036
.L_ja_0200beee:
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	b .L_ja_0200c036
.L_ja_0200befa:
	ldr r3, .L_ja_0200c04c
	cmp r2, r3
	beq .L_ja_0200bf02
	b .L_ja_0200c036
.L_ja_0200bf02:
	movs r0, #10
	movs r1, #1
	bl Func_02004fec
	movs r1, #9
	movs r0, #8
	bl Func_02002ac8
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	beq .L_ja_0200bf28
	cmp r3, #3
	beq .L_ja_0200bf28
	cmp r3, #5
	bne .L_ja_0200bf2c
.L_ja_0200bf28:
	bl Func_02004d00
.L_ja_0200bf2c:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bf4c
	movs r1, #192
	movs r2, #150
	movs r3, #170
	lsls r1, r1, #17
	lsls r2, r2, #15
	lsls r3, r3, #18
	movs r0, #11
	bl Func_02004a5c
.L_ja_0200bf4c:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bf68
	movs r2, #204
	movs r3, #170
	ldr r1, .L_ja_0200c050
	lsls r2, r2, #14
	lsls r3, r3, #18
	movs r0, #12
	bl Func_02004a5c
.L_ja_0200bf68:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bf86
	movs r2, #220
	movs r3, #170
	ldr r1, .L_ja_0200c054
	lsls r2, r2, #14
	lsls r3, r3, #18
	movs r0, #13
	bl Func_02004a5c
.L_ja_0200bf86:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #188
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bfd8
	movs r1, #192
	movs r2, #184
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004ebc
	movs r3, #85
	str r3, [sp, #4]
	movs r5, #25
	movs r0, #1
	movs r1, #85
	movs r2, #1
	movs r3, #4
	str r5, [sp, #0]
	bl Func_02004df4
	movs r6, #23
	movs r0, #27
	movs r1, #15
	movs r2, #2
	movs r3, #1
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004dfc
	movs r0, #25
	movs r1, #25
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004dfc
.L_ja_0200bfd8:
	movs r0, #15
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #196
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200c036
	movs r3, #104
	str r3, [sp, #0]
	movs r5, #20
	movs r0, #117
	movs r1, #20
	movs r2, #7
	movs r3, #8
	str r5, [sp, #4]
	bl Func_02004e04
	movs r3, #111
	str r3, [sp, #0]
	movs r0, #111
	movs r1, #12
	movs r2, #1
	movs r3, #6
	str r5, [sp, #4]
	bl Func_02004e04
	movs r3, #40
	movs r2, #84
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #84
	movs r2, #8
	movs r3, #8
	bl Func_02004dfc
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
.L_ja_0200c036:
	movs r0, #0
	bl Func_02004fc4
	movs r0, #0
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_ja_0200c04c:
	.4byte 0x000000b5
.L_ja_0200c050:
	.4byte 0x01cf0000
.L_ja_0200c054:
	.4byte 0x02350000
	.endif
	.section .text.x0200c280,"ax",%progbits
	.global Func_02004280
	.thumb_func
Func_02004280:
	push {r5, r6, r7, lr}
	ldr r1, .L_0200c334
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200c338
	sub sp, #4
	cmp r2, r3
	beq .L_0200c298
	b .L_0200c41c
.L_0200c298:
	movs r3, #192
	movs r2, #241
	lsls r3, r3, #18
	lsls r2, r2, #1
	ldr r7, [r3, #32]
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #4
	ble .L_0200c2ae
	b .L_0200c3ac
.L_0200c2ae:
	adds r2, #50
	adds r3, r1, r2
	ldr r0, [r3]
	bl Unit_Get
	adds r2, r0, #0
	movs r5, #0
	adds r2, #13
.L_0200c2be:
	ldrb r3, [r0]
	adds r0, #1
	adds r5, r5, r3
	cmp r0, r2
	ble .L_0200c2be
	adds r0, r5, #0
	bl Func_020044f8
	bl Func_02004fe4
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r3, #6
	adds r3, #255
	adds r2, r7, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #12
	ldrh r2, [r0]
	movs r3, #255
	lsls r3, r3, #8
	mov r1, sp
	adds r3, #252
	adds r1, #2
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200c330
	orrs r3, r2
	strh r3, [r0]
	bl Func_020048a0
	ldr r5, .L_0200c33c
	ldr r3, [r5]
	cmp r3, #3
	bne .L_0200c340
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #246
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c340
	movs r1, #208
	movs r2, #176
	movs r0, #64
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl Func_02004ebc
	b .L_0200c340
.L_0200c330:
	.4byte 0x00000001
.L_0200c334:
	.4byte gMainRoutineWork
.L_0200c338:
	.4byte 0x000000b1
.L_0200c33c:
	.4byte gData_020023c4 + 0x288
.L_0200c340:
	ldr r3, [r5]
	cmp r3, #9
	bne .L_0200c354
	movs r1, #208
	movs r2, #144
	movs r0, #65
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl Func_02004ebc
.L_0200c354:
	ldr r0, [r5]
	movs r2, #165
	lsls r2, r2, #4
	adds r0, r0, r2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c366
	b .L_0200c4c6
.L_0200c366:
	movs r0, #8
	bl Actor_GetById
	adds r6, r0, #0
	bl Func_02004904
	movs r1, #192
	adds r5, r0, #0
	lsls r1, r1, #8
	adds r7, r6, #0
	ands r1, r5
	movs r3, #0
	adds r7, #89
	lsrs r1, r1, #14
	strb r3, [r7]
	adds r1, #2
	adds r0, r6, #0
	bl Func_02004d9c
	movs r3, #128
	lsls r3, r3, #7
	ands r3, r5
	cmp r3, #0
	beq .L_0200c39a
	ldr r3, .L_0200c4cc
	b .L_0200c39c
.L_0200c39a:
	ldr r3, .L_0200c4d0
.L_0200c39c:
	str r3, [r6, #108]
	movs r3, #1
	movs r0, #128
	strb r3, [r7]
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_0200c4c6
.L_0200c3ac:
	cmp r3, #8
	bgt .L_0200c3b2
	b .L_0200c4c6
.L_0200c3b2:
	ldr r2, .L_0200c4d4
	movs r3, #1
	str r3, [r2]
	movs r3, #188
	lsls r3, r3, #1
	adds r5, r7, r3
	movs r3, #127
	strh r3, [r5, #40]
	movs r1, #128
	movs r3, #254
	movs r2, #0
	lsls r1, r1, #9
	lsls r3, r3, #6
	str r2, [r5, #24]
	str r2, [r5, #28]
	str r2, [r5, #32]
	str r2, [r5, #36]
	str r1, [r5, #16]
	str r1, [r5, #20]
	strh r3, [r5, #42]
	adds r3, r7, #0
	adds r3, #228
	ldr r0, [r3]
	ldr r6, .L_0200c4d8
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #8]
	ldr r1, [r5, #20]
	adds r0, r0, r3
	str r0, [r5]
	adds r3, r7, #0
	adds r3, #232
	ldr r0, [r3]
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #12]
	adds r0, r0, r3
	str r0, [r5, #4]
	bl Func_02004dc4
	movs r0, #1
	bl wait
	ldr r1, .L_0200c4dc
	ldr r3, .L_0200c4e0
	ldr r0, .L_0200c4e4
	movs r2, #24
	mov lr, r3
	.2byte 0xf800
	ldr r0, .L_0200c4e4
	bl Func_02004de4
	b .L_0200c4c6
.L_0200c41c:
	ldr r3, .L_0200c4e8
	cmp r2, r3
	bne .L_0200c474
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #9
	bhi .L_0200c4c6
	ldr r2, .L_0200c4ec
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0200c43c:
	.4byte .L_0200c4b0
	.4byte .L_0200c4b0
	.4byte .L_0200c46c
	.4byte .L_0200c46c
	.4byte .L_0200c4b0
	.4byte .L_0200c4b8
	.4byte .L_0200c4b8
	.4byte .L_0200c4b0
	.4byte .L_0200c464
	.4byte .L_0200c464
.L_0200c464:
	movs r0, #102
	bl Func_02004274
	b .L_0200c4c6
.L_0200c46c:
	movs r0, #103
	bl Func_02004274
	b .L_0200c4c6
.L_0200c474:
	ldr r3, .L_0200c4f0
	cmp r2, r3
	bne .L_0200c4c6
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #6
	bhi .L_0200c4c6
	ldr r2, .L_0200c4f4
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0200c494:
	.4byte .L_0200c4b0
	.4byte .L_0200c4b8
	.4byte .L_0200c4b0
	.4byte .L_0200c4b8
	.4byte .L_0200c4b0
	.4byte .L_0200c4c0
	.4byte .L_0200c4b8
.L_0200c4b0:
	movs r0, #100
	bl Func_02004274
	b .L_0200c4c6
.L_0200c4b8:
	movs r0, #101
	bl Func_02004274
	b .L_0200c4c6
.L_0200c4c0:
	movs r0, #102
	bl Func_02004274
.L_0200c4c6:
	movs r0, #0
	add sp, #4
	pop {r5, r6, r7, pc}
.L_0200c4cc:
	.4byte Func_02000aac
.L_0200c4d0:
	.4byte Func_02000a60
.L_0200c4d4:
	.4byte gData_020023c4 + 0x288
.L_0200c4d8:
	.4byte IwramMulQ16
.L_0200c4dc:
	.4byte gData_020053e4
.L_0200c4e0:
	.4byte IwramCopyWords
.L_0200c4e4:
	.4byte Data_0202de00
.L_0200c4e8:
	.4byte 0x000000b2
.L_0200c4ec:
	.4byte .L_0200c43c
.L_0200c4f0:
	.4byte 0x000000b5
.L_0200c4f4:
	.4byte .L_0200c494
	.section .text.x0200c4f8,"ax",%progbits
	.global Func_020044f8
	.thumb_func
Func_020044f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_0200c7d0
	adds r5, r0, #0
	ldr r0, [r6]
	ldr r1, .L_0200c7d4
	sub sp, #8
	str r0, [sp, #4]
	movs r0, #10
	adds r1, #4
	adds r0, #255
	mov r11, r1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c524
	b .L_0200c846
.L_0200c524:
	movs r0, #252
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c532
	b .L_0200c846
.L_0200c532:
	str r5, [r6]
.L_0200c534:
	movs r0, #196
	lsls r0, r0, #4
	bl Heap_AllocEwram
	movs r1, #196
	adds r5, r0, #0
	lsls r1, r1, #4
	ldr r3, .L_0200c7d8
	mov lr, r3
	.2byte 0xf800
	movs r2, #14
	adds r4, r5, #0
	mov r10, r2
	mov r8, r2
	movs r6, #0
	adds r4, #112
	movs r7, #28
	adds r0, r5, #0
	adds r1, r5, #0
.L_0200c55a:
	movs r3, #196
	lsls r3, r3, #4
	movs r2, #99
	adds r3, r3, r6
	subs r7, #1
	strb r2, [r0]
	adds r6, #4
	strb r2, [r4]
	adds r0, #112
	strb r2, [r1]
	adds r4, #112
	adds r1, #4
	strb r2, [r5, r3]
	cmp r7, #0
	bge .L_0200c55a
	movs r7, #1
.L_0200c57a:
	movs r0, #0
	mov r9, r0
.L_0200c57e:
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #16
	bgt .L_0200c534
	mov r1, r8
	str r1, [sp, #0]
	bl Random16Far
	lsls r0, r0, #2
	lsrs r0, r0, #16
	mov r6, r10
	ldr r1, [sp, #0]
	cmp r0, #0
	bne .L_0200c5a4
	cmp r7, #1
	beq .L_0200c57e
	cmp r7, #13
	beq .L_0200c57e
.L_0200c5a4:
	mov r2, r8
	lsls r3, r2, #3
	cmp r0, #1
	beq .L_0200c5f8
	cmp r0, #1
	bgt .L_0200c5b6
	cmp r0, #0
	beq .L_0200c5c0
	b .L_0200c6a4
.L_0200c5b6:
	cmp r0, #2
	beq .L_0200c62e
	cmp r0, #3
	beq .L_0200c668
	b .L_0200c6a4
.L_0200c5c0:
	mov r0, r8
	subs r3, r3, r0
	lsls r3, r3, #2
	add r3, r10
	lsls r2, r3, #2
	adds r3, r2, #0
	adds r3, #112
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	adds r3, #224
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	adds r3, #108
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	adds r3, #116
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	mov r1, r8
	adds r1, #1
	b .L_0200c6a4
.L_0200c5f8:
	mov r2, r8
	subs r3, r3, r2
	lsls r3, r3, #2
	add r3, r10
	lsls r2, r3, #2
	subs r3, r2, #4
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	subs r3, #8
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	subs r3, #116
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	adds r3, #108
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	mov r6, r10
	subs r6, #1
	b .L_0200c6a4
.L_0200c62e:
	mov r0, r8
	subs r3, r3, r0
	lsls r3, r3, #2
	add r3, r10
	lsls r2, r3, #2
	adds r3, r2, #4
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	adds r3, #8
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	subs r3, #108
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_0200c656
	b .L_0200c57e
.L_0200c656:
	adds r3, r2, #0
	adds r3, #116
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_0200c662
	b .L_0200c57e
.L_0200c662:
	mov r6, r10
	adds r6, #1
	b .L_0200c6a4
.L_0200c668:
	mov r1, r8
	subs r3, r3, r1
	lsls r3, r3, #2
	add r3, r10
	lsls r2, r3, #2
	adds r3, r2, #0
	subs r3, #112
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_0200c67e
	b .L_0200c57e
.L_0200c67e:
	adds r3, r2, #0
	subs r3, #224
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_0200c68a
	b .L_0200c57e
.L_0200c68a:
	adds r3, r2, #0
	subs r3, #116
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_0200c696
	b .L_0200c57e
.L_0200c696:
	adds r3, r2, #0
	subs r3, #108
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_0200c6a2
	b .L_0200c57e
.L_0200c6a2:
	subs r1, #1
.L_0200c6a4:
	mov r2, r8
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	add r3, r10
	lsls r3, r3, #2
	adds r3, r3, r5
	strb r7, [r3]
	adds r7, #1
	strb r6, [r3, #1]
	strb r1, [r3, #2]
	mov r10, r6
	mov r8, r1
	cmp r7, #14
	bgt .L_0200c6c4
	b .L_0200c57a
.L_0200c6c4:
	movs r3, #14
	mov r10, r3
	mov r8, r3
	movs r7, #1
.L_0200c6cc:
	mov r0, r8
	lsls r0, r0, #3
	mov r1, r8
	subs r3, r0, r1
	lsls r3, r3, #2
	add r3, r10
	lsls r3, r3, #2
	adds r2, r3, r5
	adds r3, #112
	ldrsb r1, [r5, r3]
	adds r4, r7, #1
	mov r12, r2
	mov lr, r0
	adds r2, r7, #0
	cmp r1, r4
	bne .L_0200c6f0
	adds r2, r4, #0
	b .L_0200c70c
.L_0200c6f0:
	subs r3, r7, #1
	mov r0, r12
	cmp r1, r3
	bne .L_0200c6fc
	adds r2, r1, #0
	b .L_0200c70c
.L_0200c6fc:
	subs r0, #112
	movs r1, #0
	ldrsb r1, [r0, r1]
	subs r3, r2, #1
	cmp r1, r3
	bne .L_0200c70c
	adds r2, r1, #0
	b .L_0200c6fc
.L_0200c70c:
	cmp r2, #0
	bne .L_0200c712
	movs r2, #1
.L_0200c712:
	lsls r3, r7, #3
	mov r0, lr
	mov r1, r8
	mov r9, r3
	add r3, r11
	strb r2, [r3, #2]
	subs r3, r0, r1
	lsls r3, r3, #2
	add r3, r10
	lsls r0, r3, #2
	adds r3, r0, #0
	subs r3, #112
	ldrsb r1, [r5, r3]
	adds r2, r7, #0
	cmp r1, r4
	bne .L_0200c736
	adds r2, r4, #0
	b .L_0200c752
.L_0200c736:
	subs r3, r7, #1
	adds r0, r0, r5
	cmp r1, r3
	bne .L_0200c742
	adds r2, r1, #0
	b .L_0200c752
.L_0200c742:
	adds r0, #112
	movs r1, #0
	ldrsb r1, [r0, r1]
	subs r3, r2, #1
	cmp r1, r3
	bne .L_0200c752
	adds r2, r1, #0
	b .L_0200c742
.L_0200c752:
	cmp r2, #0
	bne .L_0200c758
	movs r2, #1
.L_0200c758:
	mov r3, r9
	add r3, r11
	mov r1, r8
	mov r0, lr
	strb r2, [r3, #5]
	subs r3, r0, r1
	lsls r3, r3, #2
	add r3, r10
	lsls r3, r3, #2
	subs r3, #4
	ldrsb r1, [r5, r3]
	adds r2, r7, #0
	cmp r1, r4
	bne .L_0200c778
	adds r2, r4, #0
	b .L_0200c79e
.L_0200c778:
	subs r3, r7, #1
	mov r6, r10
	cmp r1, r3
	bne .L_0200c784
	adds r2, r1, #0
	b .L_0200c79e
.L_0200c784:
	mov r1, r8
	mov r0, lr
	subs r3, r0, r1
	lsls r3, r3, #2
	adds r6, #1
	adds r3, r3, r6
	lsls r3, r3, #2
	ldrsb r1, [r5, r3]
	subs r3, r2, #1
	cmp r1, r3
	bne .L_0200c79e
	adds r2, r1, #0
	b .L_0200c784
.L_0200c79e:
	cmp r2, #0
	bne .L_0200c7a4
	movs r2, #1
.L_0200c7a4:
	mov r3, r9
	add r3, r11
	strb r2, [r3, #3]
	mov r3, r8
	lsls r0, r3, #3
	subs r3, r0, r3
	lsls r3, r3, #2
	add r3, r10
	lsls r3, r3, #2
	adds r3, #4
	ldrsb r1, [r5, r3]
	adds r2, r7, #0
	cmp r1, r4
	bne .L_0200c7c4
	adds r2, r4, #0
	b .L_0200c7f4
.L_0200c7c4:
	subs r3, r7, #1
	mov r6, r10
	cmp r1, r3
	bne .L_0200c7dc
	adds r2, r1, #0
	b .L_0200c7f4
.L_0200c7d0:
	.4byte gRandomState
.L_0200c7d4:
	.4byte gData_020023c4 + 0x288
.L_0200c7d8:
	.4byte IwramClearWords
.L_0200c7dc:
	mov r1, r8
	subs r3, r0, r1
	lsls r3, r3, #2
	subs r6, #1
	adds r3, r3, r6
	lsls r3, r3, #2
	ldrsb r1, [r5, r3]
	subs r3, r2, #1
	cmp r1, r3
	bne .L_0200c7f4
	adds r2, r1, #0
	b .L_0200c7dc
.L_0200c7f4:
	cmp r2, #0
	bne .L_0200c7fa
	movs r2, #1
.L_0200c7fa:
	mov r3, r9
	add r3, r11
	strb r2, [r3, #4]
	strb r7, [r3]
	mov r2, r12
	mov r3, r12
	ldrb r2, [r2, #1]
	lsls r2, r2, #24
	asrs r2, r2, #24
	ldrb r3, [r3, #2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r7, r4, #0
	mov r10, r2
	mov r8, r3
	cmp r7, #14
	bgt .L_0200c81e
	b .L_0200c6cc
.L_0200c81e:
	mov r2, r11
	adds r2, #117
	ldr r1, .L_0200c854
	movs r3, #251
	strb r3, [r2]
	mov r0, r11
	movs r3, #250
	strb r3, [r0, #10]
	movs r3, #1
	str r3, [r1]
	adds r0, r5, #0
	bl Heap_Release
	movs r0, #252
	lsls r0, r0, #2
	bl GameFlag_SetBit
	ldr r3, .L_0200c858
	ldr r2, [sp, #4]
	str r2, [r3]
.L_0200c846:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c854:
	.4byte gData_020023c4 + 0x288
.L_0200c858:
	.4byte gRandomState
	.section .bss,"aw",%nobits
	.global gData_0200646c
gData_0200646c:
	.space 0x00000080
	.global gData_020064ec
gData_020064ec:
	.space 0x00000004
	.global gData_020064f0
gData_020064f0:
	.space 0x00000004
	.global Data_020064f4
Data_020064f4:
	.space 0x00000004
	.global gData_020064f8
gData_020064f8:
