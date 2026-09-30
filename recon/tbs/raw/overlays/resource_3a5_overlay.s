.syntax unified
	.thumb
	.section .text.x02008e2c,"ax",%progbits
	.global Func_02000e2c
	.thumb_func
Func_02000e2c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009028
	movs r2, #240
	ldr r3, [r3]
	movs r0, #128
	sub sp, #104
	movs r1, #60
	lsls r2, r2, #16
	lsls r0, r0, #2
	str r3, [sp, #20]
	str r1, [sp, #16]
	mov r11, r2
	bl Engine_GameFlagSet
	movs r0, #1
	bl SceneState_SetHalfwordB030
	ldr r3, .L_0200902c
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009030
	cmp r2, r3
	bne .L_02008e74
	ldr r2, .L_02009034
	movs r7, #3
	mov r8, r2
	b .L_02008e88
.L_02008e74:
	ldr r3, .L_02009038
	cmp r2, r3
	bne .L_02008e82
	ldr r3, .L_0200903c
	movs r7, #5
	mov r8, r3
	b .L_02008e88
.L_02008e82:
	ldr r1, .L_02009040
	movs r7, #2
	mov r8, r1
.L_02008e88:
	mov r9, r7
	mov r5, r8
	cmp r7, #0
	beq .L_02008eb8
	movs r6, #0
.L_02008e92:
	movs r0, #0
	bl Object_GetById
	adds r1, r5, #0
	adds r0, #8
	bl RamakanSabaku_CalculatePlanarDistance
	cmp r0, r11
	bgt .L_02008eac
	mov r2, r9
	subs r2, r2, r7
	mov r11, r0
	mov r10, r2
.L_02008eac:
	adds r6, #8
	mov r3, r8
	subs r7, #1
	adds r5, r3, r6
	cmp r7, #0
	bne .L_02008e92
.L_02008eb8:
	mov r1, r10
	lsls r1, r1, #1
	mov r10, r1
	movs r2, #128
	movs r1, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #0
	bl Engine_ActorSetSpeed
	movs r0, #0
	bl Object_GetById
	mov r2, r10
	lsls r3, r2, #2
	add r3, r8
	movs r2, #0
	ldr r1, [r3]
	ldr r3, [r3, #4]
	bl Engine_ObjectSetPosition
	movs r0, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r0, #152
	bl Engine_AudioPlayCue
	movs r0, #0
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #0
	bl Object_GetById
	ldr r1, [r0, #12]
	adds r0, r5, #0
	bl OverlayObject_WaitUntilField12BelowLimit
	movs r0, #241
	bl Engine_AudioPlayCue
	movs r0, #0
	bl Object_GetById
	movs r3, #214
	add r4, sp, #64
	strh r3, [r4, #24]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r4, #8]
	ldr r3, .L_02009044
	str r3, [r4, #12]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r4, #16]
	ldr r3, .L_02009048
	str r3, [r4, #20]
	movs r3, #224
	ldr r5, [r0, #8]
	lsls r3, r3, #13
	ldr r1, [r0, #12]
	ldr r2, [r0, #16]
	movs r6, #0
	str r3, [sp, #8]
	adds r0, r5, #0
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Effect_Spawn
	movs r1, #130
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #18
	movs r0, #0
	bl Engine_ActorSetAnimation
	ldr r1, .L_0200904c
	ldr r3, [sp, #20]
	ldr r5, .L_02009050
	adds r6, r3, r1
	movs r7, #0
.L_02008f6a:
	movs r3, #150
	lsls r3, r3, #2
	strh r3, [r6]
	ldr r2, [sp, #16]
	subs r2, #1
	str r2, [sp, #16]
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldrh r2, [r5]
	cmp r3, #0
	beq .L_02008f98
	subs r3, r2, #5
	strh r3, [r5]
	lsls r3, r3, #16
	cmp r3, #0
	bgt .L_02008f8e
	strh r7, [r5]
	b .L_02008f98
.L_02008f8e:
	ldr r2, [sp, #16]
	cmp r2, #0
	bne .L_02008f98
	movs r3, #1
	str r3, [sp, #16]
.L_02008f98:
	movs r0, #1
	bl Engine_TaskWait
	ldr r1, [sp, #16]
	cmp r1, #0
	bne .L_02008f6a
	movs r0, #0
	bl Object_GetById
	movs r3, #214
	add r4, sp, #24
	strh r3, [r4, #24]
	ldr r3, .L_02009044
	movs r2, #128
	str r3, [r4, #12]
	ldr r3, .L_02009048
	lsls r2, r2, #8
	str r2, [r4, #8]
	str r2, [r4, #16]
	str r3, [r4, #20]
	ldr r3, [sp, #16]
	ldr r2, [r0, #16]
	ldr r1, [r0, #12]
	ldr r5, [r0, #8]
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r3, #224
	lsls r3, r3, #13
	str r3, [sp, #8]
	adds r0, r5, #0
	movs r3, #0
	str r4, [sp, #12]
	bl Effect_Spawn
	movs r0, #144
	lsls r0, r0, #1
	bl Engine_AudioPlayCue
	movs r0, #152
	bl Engine_AudioPlayCue
	movs r0, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r1, #1
	movs r0, #0
	bl Engine_ActorSetAnimation
	movs r0, #10
	bl Engine_EventWait
	ldr r2, .L_0200904c
	ldr r1, [sp, #20]
	adds r3, r1, r2
	add r1, sp, #16
	ldrh r1, [r1]
	movs r0, #0
	strh r1, [r3]
	bl SceneState_SetHalfwordB030
	add sp, #104
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_02009028:
	.4byte Data_03001ebc
.L_0200902c:
	.4byte gCell
.L_02009030:
	.4byte 0x00000059
.L_02009034:
	.4byte RamakanSabaku_SafePoints1
.L_02009038:
	.4byte 0x0000005a
.L_0200903c:
	.4byte RamakanSabaku_SafePoints2
.L_02009040:
	.4byte RamakanSabaku_SafePointsOther
.L_02009044:
	.4byte 0x0000cccc
.L_02009048:
	.4byte 0x00013333
.L_0200904c:
	.4byte 0x00000cba
.L_02009050:
	.4byte Data_0200046b + 0x7
	.section .text.x020098a2,"ax",%progbits
	.2byte 0x0000
	.section .text.x020098a4,"ax",%progbits
	.global Func_020018a4
	.thumb_func
Func_020018a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_020098fc
	movs r0, #0
	ldrsh r3, [r3, r0]
	ldr r2, .L_02009900
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #12
	lsrs r3, r3, #5
	str r3, [sp, #8]
	ldr r3, .L_02009904
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_020098da
	ldr r2, .L_02009908
	ldr r3, .L_020098f8
	adds r4, r2, #0
	strh r3, [r2]
	b .L_02009932
.L_020098da:
	movs r0, #130
	lsls r0, r0, #1
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_0200990c
	ldr r4, .L_02009908
	movs r5, #0
	ldrsh r3, [r4, r5]
	ldrh r2, [r4]
	cmp r3, #0
	ble .L_02009932
	subs r3, r2, #1
	strh r3, [r4]
	b .L_02009932
.L_020098f8:
	.4byte 0x00000002
.L_020098fc:
	.4byte RamakanSabaku_SandVramSlot
.L_02009900:
	.4byte ResourceTableEntries
.L_02009904:
	.4byte RamakanSabaku_SandPhase
.L_02009908:
	.4byte RamakanSabaku_SandCounter
.L_0200990c:
	ldr r4, .L_02009948
	movs r0, #0
	ldrsh r3, [r4, r0]
	ldrh r2, [r4]
	cmp r3, #1
	bgt .L_02009932
	adds r3, r2, #1
	movs r1, #128
	strh r3, [r4]
	lsls r1, r1, #9
	lsls r3, r3, #16
	cmp r3, r1
	bne .L_02009932
	ldr r3, .L_0200994c
	ldr r0, .L_02009950
	ldr r1, .L_02009954
	ldr r2, .L_02009958
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_02009932:
	movs r2, #0
	ldrsh r0, [r4, r2]
	cmp r0, #0
	bne .L_02009960
	ldr r3, .L_0200995c
	movs r5, #0
	ldrsh r0, [r3, r5]
	bl Resource_ActivateEntry
	b .L_02009be4
	.2byte 0x0000
.L_02009948:
	.4byte RamakanSabaku_SandCounter
.L_0200994c:
	.4byte 0x040000d4
.L_02009950:
	.4byte Data_02001f80
.L_02009954:
	.4byte 0x050003c0
.L_02009958:
	.4byte 0x80000010
.L_0200995c:
	.4byte RamakanSabaku_SandVramSlot
.L_02009960:
	ldr r3, .L_020099e8
	ldr r1, [r3]
	cmp r1, #0
	beq .L_0200998c
	ldr r2, .L_020099ec
	adds r3, r1, r2
	ldrb r2, [r3]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #5
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r1, r1, r3
	lsls r3, r0, #19
	asrs r2, r3, #16
	adds r1, #38
	movs r4, #0
.L_02009982:
	adds r4, #1
	strh r2, [r1]
	adds r1, #4
	cmp r4, #143
	bls .L_02009982
.L_0200998c:
	movs r0, #144
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	mov r9, r0
	ldr r3, .L_020099f0
	ldr r0, .L_020099f4
	mov r1, r9
	ldr r2, .L_020099f8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r3, r9
	adds r3, #12
	ldr r5, .L_020099e4
	str r3, [sp, #4]
	mov r10, r3
	movs r4, #6
	mov r11, r5
.L_020099b0:
	mov r1, r10
	movs r0, #0
	ldrsh r2, [r1, r0]
	movs r3, #248
	lsls r2, r2, #16
	lsls r3, r3, #13
	ands r3, r2
	lsrs r3, r3, #16
	mov r8, r3
	lsrs r5, r2, #21
	mov r3, r11
	lsrs r2, r2, #26
	ands r2, r3
	ands r5, r3
	ldr r3, .L_020099fc
	movs r0, #0
	ldrsh r6, [r3, r0]
	movs r1, #3
	adds r0, r6, #0
	adds r7, r2, #0
	str r4, [sp, #0]
	bl __divsi3
	movs r1, #6
	add r8, r0
	b .L_02009a00
.L_020099e4:
	.4byte 0x0000001f
.L_020099e8:
	.4byte Data_03001ecc
.L_020099ec:
	.4byte 0x00000539
.L_020099f0:
	.4byte 0x040000d4
.L_020099f4:
	.4byte Data_02001f80
.L_020099f8:
	.4byte 0x80000010
.L_020099fc:
	.4byte Data_020026bc
.L_02009a00:
	adds r0, r6, #0
	bl __divsi3
	subs r7, #20
	subs r0, r7, r0
	adds r7, r0, #0
	adds r7, #20
	ldr r4, [sp, #0]
	cmp r6, #60
	ble .L_02009a30
	ldr r3, .L_02009ae8
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02009a30
	lsls r0, r6, #6
	movs r1, #120
	bl __divsi3
	adds r0, r5, r0
	adds r5, r0, #0
	ldr r4, [sp, #0]
	subs r5, #32
.L_02009a30:
	mov r1, r8
	cmp r1, #31
	bls .L_02009a3a
	movs r2, #31
	mov r8, r2
.L_02009a3a:
	cmp r5, #31
	bls .L_02009a40
	movs r5, #31
.L_02009a40:
	cmp r7, #31
	bls .L_02009a46
	movs r7, #31
.L_02009a46:
	lsls r2, r5, #5
	lsls r3, r7, #10
	orrs r3, r2
	mov r5, r8
	mov r0, r10
	orrs r3, r5
	movs r1, #2
	adds r4, #1
	strh r3, [r0]
	add r10, r1
	cmp r4, #11
	bls .L_020099b0
	ldr r2, [sp, #4]
	ldr r5, .L_02009aec
	mov r6, r9
	ldr r1, [r2]
	adds r0, r5, #0
	adds r6, #16
	adds r5, #4
	bl QueueIoWriteDelay3
	adds r0, r5, #0
	ldr r1, [r6]
	adds r5, #4
	adds r6, #4
	bl QueueIoWriteDelay3
	ldr r1, [r6]
	adds r0, r5, #0
	bl QueueIoWriteDelay3
	ldr r2, .L_02009af0
	ldr r0, .L_02009af4
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	lsls r0, r3, #4
	subs r0, r0, r3
	movs r3, #139
	lsls r3, r3, #2
	adds r2, r2, r3
	movs r3, #0
	ldrsh r1, [r2, r3]
	lsls r0, r0, #3
	bl __divsi3
	ldr r5, .L_02009af8
	movs r1, #236
	strh r0, [r5]
	lsls r1, r1, #15
	lsls r0, r0, #16
	cmp r0, r1
	ble .L_02009ab6
	ldr r2, .L_02009afc
	ldr r3, .L_02009ae0
	strh r3, [r2]
.L_02009ab6:
	ldr r1, .L_02009afc
	movs r0, #0
	ldrsh r3, [r1, r0]
	ldrh r2, [r1]
	cmp r3, #0
	beq .L_02009ad4
	adds r3, r2, #0
	subs r3, #8
	strh r2, [r5]
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bgt .L_02009ad4
	ldr r3, .L_02009ae4
	strh r3, [r1]
.L_02009ad4:
	ldr r3, .L_02009b00
	ldr r0, .L_02009b04
	mov r1, r9
	ldr r2, .L_02009b08
	b .L_02009b0c
	.2byte 0x0000
.L_02009ae0:
	.4byte 0x00000077
.L_02009ae4:
	.4byte 0x00000000
.L_02009ae8:
	.4byte gFrameCount
.L_02009aec:
	.4byte 0x050003cc
.L_02009af0:
	.4byte gCell
.L_02009af4:
	.4byte 0x00000232
.L_02009af8:
	.4byte Data_020026bc
.L_02009afc:
	.4byte Data_020026c0
.L_02009b00:
	.4byte 0x040000d4
.L_02009b04:
	.4byte RamakanSabaku_SandTileBuffer
.L_02009b08:
	.4byte 0x84000240
.L_02009b0c:
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_02009bf8
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #118
	bgt .L_02009b52
	ldr r3, .L_02009bfc
	movs r5, #0
	ldrsh r2, [r3, r5]
	movs r3, #128
	mov r1, r9
	movs r4, #12
	subs r3, r3, r2
	adds r1, #80
	ldr r0, .L_02009c00
	cmp r4, r3
	bcs .L_02009b48
	movs r2, #7
	mov r12, r3
.L_02009b34:
	adds r3, r4, #0
	ands r3, r2
	str r0, [r1, #32]
	stmia r1!, {r0}
	cmp r3, #7
	bne .L_02009b42
	adds r1, #32
.L_02009b42:
	adds r4, #1
	cmp r4, r12
	bcc .L_02009b34
.L_02009b48:
	mov r0, r9
	ldr r3, [r0]
	str r3, [r1]
	ldr r3, [r0, #32]
	str r3, [r1, #32]
.L_02009b52:
	movs r2, #144
	lsls r2, r2, #3
	ldr r0, .L_02009c04
	mov r1, r9
	add r2, r9
	movs r4, #0
.L_02009b5e:
	ldrb r3, [r2]
	adds r2, #1
	cmp r3, #0
	beq .L_02009b68
	strb r3, [r1]
.L_02009b68:
	adds r4, #1
	adds r1, #1
	cmp r4, r0
	bls .L_02009b5e
	ldr r3, .L_02009c08
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #144
	mov r2, r9
	lsls r1, r1, #3
	bl Engine_VramLoad
	ldr r2, .L_02009c0c
	ldr r5, .L_02009c10
	mov r8, r2
	movs r4, #0
	movs r7, #0
	movs r6, #8
.L_02009b8c:
	ldr r3, .L_02009c14
	movs r0, #0
	ldrsh r3, [r3, r0]
	lsls r3, r3, #3
	adds r2, r3, #0
	ldr r3, .L_02009c18
	subs r2, #16
	ands r2, r3
	cmp r4, #4
	bne .L_02009ba6
	movs r1, #128
	lsls r1, r1, #23
	mov r8, r1
.L_02009ba6:
	movs r3, #0
	str r3, [r5]
	lsls r3, r2, #16
	orrs r3, r6
	mov r2, r8
	orrs r3, r2
	str r3, [r5, #4]
	ldr r0, [sp, #8]
	movs r3, #228
	lsls r3, r3, #8
	orrs r3, r0
	ldr r0, .L_02009c10
	str r3, [r5, #8]
	adds r0, r7, r0
	movs r1, #255
	str r4, [sp, #0]
	bl Runtime_PushSlotEntry
	ldr r1, [sp, #8]
	ldr r4, [sp, #0]
	adds r1, #8
	adds r4, #1
	adds r5, #12
	str r1, [sp, #8]
	adds r7, #12
	adds r6, #32
	cmp r4, #4
	bls .L_02009b8c
	mov r0, r9
	bl Sys_Free
.L_02009be4:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_02009bf8:
	.4byte Data_020026c0
.L_02009bfc:
	.4byte Data_020026bc
.L_02009c00:
	.4byte 0xeeeeeeee
.L_02009c04:
	.4byte 0x0000047f
.L_02009c08:
	.4byte RamakanSabaku_SandVramSlot
.L_02009c0c:
	.4byte 0x80008000
.L_02009c10:
	.4byte RamakanSabaku_SandWork
.L_02009c14:
	.4byte RamakanSabaku_SandCounter
.L_02009c18:
	.4byte 0x000001ff
	.section .rodata.x02009e88,"a",%progbits
.L_02009e88:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
.L_02009ec0:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
.L_02009ef8:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global RamakanSabaku_SafePoints1
RamakanSabaku_SafePoints1:
	.4byte 0x01800000
	.4byte 0x00800000
	.4byte 0x01200000
	.4byte 0x02b00000
	.4byte 0x02600000
	.4byte 0x02d00000
	.global RamakanSabaku_SafePoints2
RamakanSabaku_SafePoints2:
	.4byte 0x02c00000
	.4byte 0x00600000
	.4byte 0x01c00000
	.4byte 0x01d00000
	.4byte 0x03400000
	.4byte 0x02500000
	.4byte 0x00800000
	.4byte 0x02f00000
	.4byte 0x01d00000
	.4byte 0x03800000
	.global RamakanSabaku_SafePointsOther
RamakanSabaku_SafePointsOther:
	.4byte 0x00c00000
	.4byte 0x00e00000
	.4byte 0x00c00000
	.4byte 0x01600000
	.global Data_02001f80
Data_02001f80:
	.4byte 0x377f10a0
	.4byte 0x121722bb
	.4byte 0x0ccc1172
	.4byte 0x7df07ef7
	.4byte 0x610b7ce7
	.4byte 0x50007c00
	.4byte 0x00147fff
	.4byte 0x2f1f0000
	.global RamakanSabaku_SandEffectTiles
RamakanSabaku_SandEffectTiles:
	.4byte 0x5c020100
	.4byte 0x3fc01eef
	.4byte 0x9de2abb8
	.4byte 0xd1dc7b0e
	.4byte 0x20cfafbd
	.4byte 0x4957900f
	.4byte 0xdef71d33
	.4byte 0xeb6f047a
	.4byte 0xebcc0813
	.4byte 0xe429f3ef
	.4byte 0xd2cbcc01
	.4byte 0x47c3de7b
	.4byte 0x38a3e160
	.4byte 0x1c7c1cfc
	.4byte 0x87dfcf87
	.4byte 0x9f07dfcf
	.4byte 0x7f3e0fbf
	.4byte 0x7dfcf81f
	.4byte 0x81f633e0
	.4byte 0xb881f027
	.4byte 0x041f87c0
	.4byte 0xfeb0433a
	.4byte 0xe91188ff
	.4byte 0x94499517
	.4byte 0x00a9cd3e
	.4byte 0xa6710fe9
	.4byte 0xa87fe42f
	.4byte 0x623ec04f
	.4byte 0xa923a250
	.4byte 0x755804c6
	.4byte 0x1a402388
	.4byte 0xe8cbe91d
	.4byte 0xfa4fe7b0
	.4byte 0xf281322f
	.4byte 0x452f81b2
	.4byte 0x5fffa48a
	.4byte 0x21c70ce4
	.4byte 0x02ab32ae
	.4byte 0xd8139124
	.4byte 0xff9c0981
	.4byte 0xfffc0d83
	.4byte 0x97fcbe53
	.4byte 0x0020673f
	.4byte 0xfe2534ae
	.4byte 0x435a2670
	.4byte 0x7046447c
	.4byte 0x039f8c62
	.4byte 0x330e38f8
	.4byte 0x71c61f00
	.4byte 0x3e3f9ede
	.4byte 0x38f01f18
	.4byte 0x09c38f80
	.4byte 0xe0be607c
	.4byte 0x7f01f223
	.4byte 0xfb49f0cf
	.4byte 0xcfc06c84
	.4byte 0x93e3dc08
	.4byte 0xbf9f0df3
	.4byte 0x7dfcf8cf
	.4byte 0x33efe7c6
	.4byte 0xaaca477e
	.4byte 0xad4cd315
	.4byte 0xe1c7cf1a
	.4byte 0x9c3d9e33
	.4byte 0xbb7da652
	.4byte 0x55126398
	.4byte 0x014000c5
	.4byte 0x394900b0
	.4byte 0x58ad26ae
	.4byte 0x88d26ef7
	.4byte 0xd55e23de
	.4byte 0x45eb29bb
	.4byte 0x5bfedbad
	.4byte 0x9acf33d4
	.4byte 0xef139d45
	.4byte 0x573ad45d
	.4byte 0x73ad5623
	.4byte 0xbb8eaaf7
	.4byte 0x4e6b83de
	.4byte 0xa30f8aca
	.4byte 0xef1a9aa6
	.4byte 0x1595a261
	.4byte 0xb549a5df
	.4byte 0x449a6ab2
	.4byte 0x81a556a9
	.4byte 0x0478c0f8
	.4byte 0xc0d14283
	.4byte 0x4d462a07
	.4byte 0xcd623113
	.4byte 0x4eab0f33
	.4byte 0x0d81432a
	.4byte 0xf0e8b55a
	.4byte 0x65c2ee19
	.4byte 0xb0593ea0
	.4byte 0x7c0b0405
	.4byte 0x0000ff00
	.global gEffectScripts
gEffectScripts:
	.4byte .L_02009e88
	.4byte .L_02009ec0
	.4byte .L_02009ef8
	.global gRamakanSabakuEntrancesOther
gRamakanSabakuEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x00000100
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000100
	.4byte 0x40000064
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gRamakanSabakuEntrances1
gRamakanSabakuEntrances1:
	.4byte 0xffff0000
	.4byte 0x00000100
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000003d8
	.4byte 0x800003b8
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x000003f8
	.4byte 0xffff0002
	.4byte 0x00000190
	.4byte 0x40000018
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x000003f8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gRamakanSabakuEntrances2
gRamakanSabakuEntrances2:
	.4byte 0xffff0000
	.4byte 0x00000100
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000390
	.4byte 0xc00003e8
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x000003f8
	.4byte 0xffff0002
	.4byte 0x00000018
	.4byte 0x00000248
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x000003f8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gRamakanSabakuEntrances3
gRamakanSabakuEntrances3:
	.4byte 0xffff0000
	.4byte 0x00000100
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000005e7
	.4byte 0x80000068
	.4byte 0x00000000
	.4byte 0x05f80008
	.4byte 0x000001f8
	.4byte 0xffff0002
	.4byte 0x000002d7
	.4byte 0x40000068
	.4byte 0x00000000
	.4byte 0x05f80008
	.4byte 0x000001f8
	.4byte 0xffff0003
	.4byte 0x00000257
	.4byte 0x40000128
	.4byte 0x00000000
	.4byte 0x05f80008
	.4byte 0x000001f8
	.4byte 0xffff0004
	.4byte 0x000000a7
	.4byte 0x40000078
	.4byte 0x00000000
	.4byte 0x05f80008
	.4byte 0x000001f8
	.4byte 0xffff0005
	.4byte 0x000000a7
	.4byte 0xc0000088
	.4byte 0x00000000
	.4byte 0x05f80008
	.4byte 0x000001f8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gRamakanSabakuEntrances4
gRamakanSabakuEntrances4:
	.4byte 0xffff0000
	.4byte 0x000001a8
	.4byte 0x80000338
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001a8
	.4byte 0xc00003c8
	.4byte 0x01300000
	.4byte 0x02700230
	.4byte 0x00000400
	.4byte 0xffff0002
	.4byte 0x00000228
	.4byte 0xc0000308
	.4byte 0x01300000
	.4byte 0x02700230
	.4byte 0x00000400
	.4byte 0xffff0003
	.4byte 0x000001b8
	.4byte 0xc0000138
	.4byte 0x00200000
	.4byte 0x01f00010
	.4byte 0x00000140
	.4byte 0xffff0004
	.4byte 0x00000088
	.4byte 0xc0000128
	.4byte 0x00200000
	.4byte 0x01f00010
	.4byte 0x00000140
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global RamakanSabaku_Exits
RamakanSabaku_Exits:
	.4byte 0x00000059
	.4byte 0x00112002
	.4byte 0x0020105a
	.4byte 0x0000005a
	.4byte 0x00102059
	.4byte 0x0020105b
	.4byte 0x0000005b
	.4byte 0x0010205a
	.4byte 0x0020205c
	.4byte 0x0030105c
	.4byte 0x0040305c
	.4byte 0x0000005c
	.4byte 0x0010205b
	.4byte 0x0020305b
	.4byte 0x00335002
	.4byte 0x0040405b
	.4byte 0x000001ff
	.global gRamakanSabakuPlacementsOther
gRamakanSabakuPlacementsOther:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gRamakanSabakuPlacements1
gRamakanSabakuPlacements1:
	.4byte 0x090a00c3
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff00c1
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x016c0000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gRamakanSabakuPlacements2
gRamakanSabakuPlacements2:
	.4byte 0x090a00c3
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff00c1
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00004000
	.4byte 0xffff00c1
	.4byte 0x00000001
	.4byte 0x02e00000
	.4byte 0x00000000
	.4byte 0x018c0000
	.4byte 0x00004000
	.4byte 0xffff00c1
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00004000
	.4byte 0xffff00c1
	.4byte 0x00000001
	.4byte 0x02400000
	.4byte 0x00000000
	.4byte 0x02cc0000
	.4byte 0x00004000
	.4byte 0x006f005d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gRamakanSabakuPlacements3
gRamakanSabakuPlacements3:
	.4byte 0x090a00c3
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff00c1
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x013c0000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global RamakanSabaku_Events
RamakanSabaku_Events:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte FieldScene_RunFlags8B2And8B3Steps
	.4byte 0x00000002
	.4byte 0x02000014
	.4byte Func_02000e2c
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte RamakanSabaku_UpdateTravelDust
	.4byte 0x00000002
	.4byte 0x02000016
	.4byte RamakanSabaku_FaceNearestActor
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte RamakanSabaku_UpdateDustAndProbe
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte FieldScene_RunScene3a5_020014b0
	.4byte 0x00000000
	.4byte 0x090a0008
	.4byte RamakanSabaku_ReturnToArea3
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte FieldScene_ApplyActor13Values3And3
	.4byte 0x00000013
	.4byte 0x0f7b0064
	.4byte 0x00100024
	.4byte 0x00000003
	.4byte 0x03500065
	.4byte 0x00300000
	.4byte 0x00000013
	.4byte 0x0f7c0066
	.4byte 0x001000c4
	.4byte 0x00000003
	.4byte 0x03510067
	.4byte 0x00300000
	.4byte 0x00000013
	.4byte 0x0f7d0069
	.4byte 0x00200309
	.4byte 0x00000013
	.4byte 0x0f7e006a
	.4byte 0x001000b7
	.4byte 0x00000013
	.4byte 0x0f7f006b
	.4byte 0x001000c3
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte RamakanSabaku_ConfigureAreaLayout
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte RamakanSabaku_ApplyEntryState
	.4byte 0x00000006
	.4byte 0x02010001
	.4byte FieldScene_RunScene3a5_02000c6c
	.4byte 0x00000006
	.4byte 0x02010002
	.4byte FieldScene_RunScene3a5_02000c6c
	.4byte 0x00000006
	.4byte 0x02010003
	.4byte FieldScene_RunScene3a5_02000c6c
	.4byte 0x00000002
	.4byte 0xffff0064
	.4byte RamakanSabaku_UpdateTravelDust
	.4byte 0x00000002
	.4byte 0xffff0065
	.4byte RamakanSabaku_UpdateTravelDust
	.4byte 0x00000002
	.4byte 0xffff0066
	.4byte RamakanSabaku_UpdateTravelDust
	.4byte 0x00000002
	.4byte 0xffff0067
	.4byte RamakanSabaku_UpdateTravelDust
	.4byte 0x00000002
	.4byte 0xffff0068
	.4byte RamakanSabaku_UpdateTravelDust
	.4byte 0x00000002
	.4byte 0xffff0069
	.4byte RamakanSabaku_UpdateTravelDust
	.4byte 0x00000002
	.4byte 0xffff006a
	.4byte RamakanSabaku_UpdateTravelDust
	.4byte 0x00000002
	.4byte 0xffff006b
	.4byte RamakanSabaku_UpdateTravelDust
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020026bc
Data_020026bc:
	.2byte 0x0000
	.global RamakanSabaku_SandCounter
RamakanSabaku_SandCounter:
	.2byte 0x0000
	.global Data_020026c0
Data_020026c0:
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global RamakanSabaku_SandVramSlot
RamakanSabaku_SandVramSlot:
	.space 16
	.global RamakanSabaku_SandWork
RamakanSabaku_SandWork:
	.space 0x50
	.global RamakanSabaku_SandTileBuffer
RamakanSabaku_SandTileBuffer:
	.space 0x900
	.global RamakanSabaku_SandPhase
RamakanSabaku_SandPhase:
	.space 2
