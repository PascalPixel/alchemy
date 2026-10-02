	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
.syntax unified
	.thumb
	.section .text.x0200ab14,"ax",%progbits
	.global Scene_RunScene3c8SequenceA
	.thumb_func
Scene_RunScene3c8SequenceA:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	movs r1, #0
	movs r0, #0
	str r1, [sp, #12]
	bl Object_GetById
	str r0, [sp, #8]
	bl Engine_EventBegin
	.ifndef TBS_EDITION_EN
	.if EDITION_INTERNATIONAL
	.ifndef TBS_EDITION_DE
	bl Battle_ResetEffectCounter
	.endif
	.endif
	.endif
	movs r3, #5
	movs r2, #48
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #69
	movs r1, #48
	movs r2, #4
	movs r3, #2
	bl Engine_MapCopyCellAttributes
	movs r3, #9
	movs r2, #37
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #73
	movs r2, #9
	movs r1, #37
	movs r3, #13
	bl Engine_MapCopyCellAttributes
	movs r2, #15
	mov r10, r2
.L_0200ab60:
	mov r0, r10
	bl Object_GetById
	movs r3, #35
	mov r8, r0
	add r3, r8
	mov r11, r3
	ldrb r3, [r3]
	cmp r3, #2
	beq .L_0200ab8e
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #72
	movs r1, #48
	movs r2, #1
	movs r3, #1
	bl Engine_MapCopyCellAttributes
	b .L_0200aba8
.L_0200ab8e:
	mov r1, r8
	ldr r2, [r1, #8]
	ldr r3, [r1, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #73
	movs r1, #48
	movs r2, #1
	movs r3, #1
	bl Engine_MapCopyCellAttributes
.L_0200aba8:
	mov r2, r8
	ldr r4, .L_0200aebc
	movs r6, #0
	ldr r0, [r2, #8]
	ldr r3, [r4, r6]
	asrs r2, r0, #20
	movs r5, #8
	cmp r2, r3
	bne .L_0200abd0
	mov r1, r8
	ldr r3, [r1, #16]
	ldr r2, [r4, #4]
	asrs r3, r3, #20
	cmp r3, r2
	bne .L_0200abd0
	ldr r3, [r1, #12]
	cmp r3, #0
	blt .L_0200abd0
	movs r5, #0
	b .L_0200abf8
.L_0200abd0:
	adds r6, #1
	cmp r6, #7
	bhi .L_0200abf8
	lsls r1, r6, #3
	ldr r3, [r4, r1]
	asrs r2, r0, #20
	cmp r2, r3
	bne .L_0200abd0
	mov r2, r8
	ldr r3, [r2, #16]
	adds r2, r1, #4
	ldr r2, [r4, r2]
	asrs r3, r3, #20
	cmp r3, r2
	bne .L_0200abd0
	mov r1, r8
	ldr r3, [r1, #12]
	cmp r3, #0
	blt .L_0200abd0
	adds r5, r6, #0
.L_0200abf8:
	cmp r5, #8
	bne .L_0200abfe
	b .L_0200ae98
.L_0200abfe:
	movs r6, #15
	b .L_0200ac04
.L_0200ac02:
	adds r6, #1
.L_0200ac04:
	cmp r6, #18
	bhi .L_0200ac30
	adds r0, r6, #0
	bl Object_GetById
	cmp r10, r6
	beq .L_0200ac02
	mov r3, r8
	ldr r2, [r3, #8]
	ldr r3, [r0, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200ac02
	mov r1, r8
	ldr r2, [r1, #16]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200ac02
	movs r5, #8
.L_0200ac30:
	cmp r5, #8
	bne .L_0200ac36
	b .L_0200ae98
.L_0200ac36:
	ldr r2, [sp, #8]
	ldr r3, [r2, #80]
	ldrb r3, [r3, #9]
	lsls r3, r3, #28
	lsrs r3, r3, #30
	lsls r7, r5, #3
	ldr r1, .L_0200aebc
	mov r9, r3
	ldr r2, [r2, #16]
	adds r3, r7, #4
	ldr r3, [r1, r3]
	asrs r2, r2, #20
	cmp r2, r3
	bhi .L_0200ac6e
	mov r2, r8
	ldr r1, [r2, #12]
	ldr r0, [r2, #8]
	ldr r3, .L_0200aec0
	ldr r2, [r2, #16]
	adds r2, r2, r3
	movs r3, #20
	bl OverlayObject_PrepareObjectWithCommand15
	movs r1, #3
	str r0, [sp, #12]
	movs r0, #0
	bl Engine_ActorSetSpritePriority
.L_0200ac6e:
	movs r6, #15
.L_0200ac70:
	adds r0, r6, #0
	bl Object_GetById
	cmp r10, r6
	beq .L_0200ac9e
	mov r1, r8
	ldr r2, [r1, #8]
	ldr r3, [r0, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200ac9e
	ldr r2, [r1, #16]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	subs r2, #1
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200ac9e
	adds r0, r6, #0
	movs r1, #3
	bl Engine_ActorSetSpritePriority
.L_0200ac9e:
	adds r6, #1
	cmp r6, #18
	bls .L_0200ac70
	mov r0, r10
	bl Object_GetById
	movs r1, #0
	bl Engine_ActorSetSpriteFlags
	mov r3, r8
	adds r3, #34
	movs r2, #0
	mov r6, r8
	strb r2, [r3]
	adds r6, #85
	movs r3, #3
	strb r3, [r6]
	ldr r3, .L_0200aec4
	mov r1, r8
	movs r2, #0
	str r3, [r1, #72]
	str r2, [r1, #68]
	ldr r1, .L_0200aebc
	adds r5, r7, #4
	ldr r3, [r1, r7]
	ldr r2, [r1, r5]
	movs r0, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #44
	movs r2, #1
	movs r3, #1
	bl Engine_MapCopyCellAttributes
	mov r0, r8
	bl OverlayObject_WaitUntilIdle
	movs r0, #188
	bl Engine_AudioPlayCue
	mov r3, r8
	movs r2, #0
	adds r3, #89
	strb r2, [r3]
	ldr r3, .L_0200aec8
	mov r1, r8
	strb r2, [r6]
	mov r0, r10
	str r3, [r1, #12]
	movs r1, #3
	bl Engine_ActorSetSpritePriority
	movs r3, #2
	mov r2, r11
	strb r3, [r2]
	ldr r1, .L_0200aebc
	ldr r3, [r1, r7]
	ldr r2, [r1, r5]
	movs r0, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r1, #48
	bl Engine_MapCopyCellAttributes
	movs r0, #0
	mov r1, r9
	bl Engine_ActorSetSpritePriority
	movs r0, #0
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r6, #15
.L_0200ad3c:
	adds r0, r6, #0
	bl Object_GetById
	cmp r10, r6
	beq .L_0200ad7c
	mov r3, r8
	ldr r2, [r3, #8]
	ldr r3, [r0, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200ad7c
	mov r1, r8
	ldr r2, [r1, #16]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	subs r2, #1
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200ad7c
	adds r0, r6, #0
	movs r1, #1
	bl Engine_ActorSetSpritePriority
	adds r0, r6, #0
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
.L_0200ad7c:
	adds r6, #1
	cmp r6, #18
	bls .L_0200ad3c
	ldr r0, [sp, #12]
	bl Engine_ObjectDispatchRelease
	movs r0, #194
	lsls r0, r0, #2
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_0200ad9a
	bl Engine_EventEnd
	b .L_0200aea8
.L_0200ad9a:
	movs r0, #15
	bl Object_GetById
	mov r8, r0
	movs r0, #16
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #17
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #18
	bl Object_GetById
	movs r2, #35
	add r8, r2
	mov r3, r8
	adds r5, #35
	ldrb r2, [r3]
	ldrb r3, [r5]
	adds r6, #35
	ands r3, r2
	ldrb r2, [r6]
	adds r0, #35
	ands r3, r2
	ldrb r2, [r0]
	ands r3, r2
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0200ae98
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Engine_CameraSetSpeed
	movs r0, #14
	movs r1, #1
	bl Object_PlaceCurrentWithinCameraBounds
	bl Engine_CameraWaitForMove
	movs r1, #194
	ldr r2, .L_0200aecc
	lsls r1, r1, #2
	movs r0, #136
	bl SceneEffect_SpawnEffect284AtCell
	adds r6, r0, #0
	movs r0, #30
	bl Engine_EventWait
	ldr r0, .L_0200aed0
	ldr r1, .L_0200aed4
	bl Engine_CameraSetSpeed
	movs r0, #216
	movs r1, #1
	movs r2, #158
	movs r3, #1
	lsls r2, r2, #18
	negs r1, r1
	lsls r0, r0, #16
	bl Engine_CameraMoveTo
	adds r0, r6, #0
	bl ObjectDispatch_WaitForValue16
	adds r0, r6, #0
	ldr r1, .L_0200aed8
	bl Engine_ObjectSetScript
	movs r1, #190
	lsls r1, r1, #2
	movs r0, #216
	ldr r2, .L_0200aedc
	bl SceneEffect_SpawnEffect284AtCell
	movs r1, #99
	ldr r3, [r6]
	adds r1, r1, r6
	adds r5, r0, #0
	mov r8, r1
	b .L_0200ae8e
.L_0200ae46:
	mov r1, r8
	ldrb r3, [r1]
	cmp r3, #0
	bne .L_0200ae58
	adds r3, r5, #0
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200ae86
.L_0200ae58:
	movs r0, #30
	bl Engine_EventWait
	ldr r0, .L_0200aee0
	movs r1, #77
	movs r2, #35
	bl Engine_MapAnimateCells
	movs r3, #13
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #13
	movs r1, #35
	movs r2, #1
	movs r3, #1
	bl Engine_MapCopyCellAttributes
	movs r0, #194
	lsls r0, r0, #2
	bl Engine_GameFlagSet
	b .L_0200ae98
.L_0200ae86:
	movs r0, #1
	bl Engine_TaskWait
	ldr r3, [r6]
.L_0200ae8e:
	cmp r3, #0
	bne .L_0200ae46
	ldr r3, [r5]
	cmp r3, #0
	bne .L_0200ae46
.L_0200ae98:
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #18
	bhi .L_0200aea4
	b .L_0200ab60
.L_0200aea4:
	bl Engine_EventEnd
.L_0200aea8:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_0200aebc:
	.4byte Data_02005164
.L_0200aec0:
	.4byte 0xfffc0000
.L_0200aec4:
	.4byte 0x00001999
.L_0200aec8:
	.4byte 0xfff00000
.L_0200aecc:
	.4byte Data_0200577c
.L_0200aed0:
	.4byte 0x00006666
.L_0200aed4:
	.4byte 0x00000ccc
.L_0200aed8:
	.4byte Data_020057c8
.L_0200aedc:
	.4byte Data_02005ac8
.L_0200aee0:
	.4byte Data_02005d3c
	.section .text.x0200cbd8,"ax",%progbits
	.global Func_02004bd8
	.thumb_func
Func_02004bd8:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r3, [r5, #8]
	sub sp, #12
	mov r0, sp
	str r3, [r0]
	ldr r1, .L_0200cd60
	ldr r3, [r5, #12]
	adds r3, r3, r1
	str r3, [r0, #4]
	ldr r3, [r5, #16]
	movs r1, #0
	str r3, [r0, #8]
	bl SceneData_FindSlotAtPosition
	adds r7, r0, #0
	ldr r6, [r7, #80]
	ldr r3, [r6, #40]
	movs r1, #128
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r1, r1, #1
	cmp r3, r1
	beq .L_0200cc0a
	b .L_0200cd4c
.L_0200cc0a:
	ldr r2, [r5, #36]
	adds r4, r2, #0
	cmp r2, #0
	bge .L_0200cc14
	negs r4, r2
.L_0200cc14:
	ldr r3, [r5, #44]
	adds r1, r3, #0
	cmp r3, #0
	bge .L_0200cc1e
	negs r1, r3
.L_0200cc1e:
	cmp r4, r1
	ble .L_0200cc38
	adds r3, r2, #0
	cmp r3, #0
	bge .L_0200cc2c
	ldr r2, .L_0200cd64
	adds r3, r3, r2
.L_0200cc2c:
	cmp r3, #0
	bge .L_0200cc34
	ldr r4, .L_0200cd68
	b .L_0200cc4a
.L_0200cc34:
	ldr r4, .L_0200cd6c
	b .L_0200cc4a
.L_0200cc38:
	cmp r3, #0
	bge .L_0200cc40
	ldr r1, .L_0200cd64
	adds r3, r3, r1
.L_0200cc40:
	cmp r3, #0
	bge .L_0200cc48
	ldr r4, .L_0200cd70
	b .L_0200cc4a
.L_0200cc48:
	ldr r4, .L_0200cd74
.L_0200cc4a:
	ldrb r1, [r4]
	adds r0, r1, #0
	cmp r0, #0
	beq .L_0200cc74
	adds r2, r6, #0
	adds r2, #36
	ldrb r3, [r2]
	cmp r3, r0
	beq .L_0200cc6e
	adds r6, r2, #0
.L_0200cc5e:
	adds r4, #1
	ldrb r1, [r4]
	adds r2, r1, #0
	cmp r2, #0
	beq .L_0200cc74
	ldrb r3, [r6]
	cmp r3, r2
	bne .L_0200cc5e
.L_0200cc6e:
	adds r3, r1, #0
	cmp r3, #0
	bne .L_0200cc7e
.L_0200cc74:
	adds r0, r5, #0
	ldr r1, .L_0200cd78
	bl Engine_ObjectSetScript
	b .L_0200cd54
.L_0200cc7e:
	ldr r3, .L_0200cd7c
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200cd80
	cmp r2, r3
	bne .L_0200cce8
	ldr r0, .L_0200cd84
	movs r4, #0
	ldr r6, [r5, #8]
	ldr r3, [r0, r4]
	asrs r2, r6, #20
	cmp r2, r3
	bne .L_0200cca8
	ldr r3, [r5, #16]
	ldr r2, [r0, #4]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_0200ccc4
.L_0200cca8:
	adds r4, #1
	cmp r4, #3
	bhi .L_0200ccc4
	lsls r1, r4, #3
	ldr r3, [r0, r1]
	asrs r2, r6, #20
	cmp r2, r3
	bne .L_0200cca8
	ldr r3, [r5, #16]
	adds r2, r1, #4
	ldr r2, [r0, r2]
	asrs r3, r3, #20
	cmp r3, r2
	bne .L_0200cca8
.L_0200ccc4:
	movs r6, #0
	lsls r4, r4, #2
	b .L_0200ccd0
.L_0200ccca:
	adds r3, r1, #1
	str r3, [r0, r4]
	adds r6, #1
.L_0200ccd0:
	ldr r0, .L_0200cd88
	ldr r1, [r0, r4]
	ldrb r2, [r1]
	cmp r2, #0
	beq .L_0200cc74
	ldr r3, [r7, #80]
	adds r3, #36
	ldrb r3, [r3]
	cmp r2, r3
	bne .L_0200ccca
	ldr r3, .L_0200cd8c
	b .L_0200cd3e
.L_0200cce8:
	ldr r0, .L_0200cd90
	movs r4, #0
	ldr r6, [r5, #8]
	ldr r3, [r0, r4]
	asrs r2, r6, #20
	cmp r2, r3
	bne .L_0200cd00
	ldr r3, [r5, #16]
	ldr r2, [r0, #4]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_0200cd1c
.L_0200cd00:
	adds r4, #1
	cmp r4, #7
	bhi .L_0200cd1c
	lsls r1, r4, #3
	ldr r3, [r0, r1]
	asrs r2, r6, #20
	cmp r2, r3
	bne .L_0200cd00
	ldr r3, [r5, #16]
	adds r2, r1, #4
	ldr r2, [r0, r2]
	asrs r3, r3, #20
	cmp r3, r2
	bne .L_0200cd00
.L_0200cd1c:
	movs r6, #0
	lsls r4, r4, #2
	b .L_0200cd28
.L_0200cd22:
	adds r3, r1, #1
	str r3, [r0, r4]
	adds r6, #1
.L_0200cd28:
	ldr r0, .L_0200cd94
	ldr r1, [r0, r4]
	ldrb r2, [r1]
	cmp r2, #0
	beq .L_0200cc74
	ldr r3, [r7, #80]
	adds r3, #36
	ldrb r3, [r3]
	cmp r2, r3
	bne .L_0200cd22
	ldr r3, .L_0200cd98
.L_0200cd3e:
	ldr r2, [r3, r4]
	lsls r3, r6, #2
	ldr r1, [r3, r2]
	adds r0, r5, #0
	bl Engine_ObjectSetScript
	b .L_0200cd54
.L_0200cd4c:
	ldr r1, .L_0200cd78
	adds r0, r5, #0
	bl Engine_ObjectSetScript
.L_0200cd54:
	movs r0, #0
	add sp, #12
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_0200cd60:
	.4byte 0xfff00000
.L_0200cd64:
	.4byte 0x0000ffff
.L_0200cd68:
	.4byte Data_020051a4
.L_0200cd6c:
	.4byte Data_020051a8
.L_0200cd70:
	.4byte Data_020051ac
.L_0200cd74:
	.4byte Data_020051b0
.L_0200cd78:
	.4byte Data_02005564
.L_0200cd7c:
	.4byte gCell
.L_0200cd80:
	.4byte 0x000000b9
.L_0200cd84:
	.4byte gVinasuSwitchCells
.L_0200cd88:
	.4byte Data_0200772c
.L_0200cd8c:
	.4byte Data_0200777c
.L_0200cd90:
	.4byte Data_02005164
.L_0200cd94:
	.4byte Data_0200778c
.L_0200cd98:
	.4byte Data_020077ec
	.section .rodata.x0200d040,"a",%progbits
.L_0200d040:
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
.L_0200d078:
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
.L_0200d0b0:
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
	.global StagedActor_DirectionSteps
StagedActor_DirectionSteps:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global gVinasuSwitchCells
gVinasuSwitchCells:
	.4byte 0x00000030
	.4byte 0x00000029
	.4byte 0x00000034
	.4byte 0x00000029
	.4byte 0x00000030
	.4byte 0x0000002b
	.4byte 0x00000034
	.4byte 0x0000002b
	.global gVinasuBlockHeights
gVinasuBlockHeights:
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xffe00000
	.4byte 0xffd00000
	.4byte 0xffc00000
	.4byte 0xffb00000
	.4byte 0xffb00000
	.global Data_02005164
Data_02005164:
	.4byte 0x0000000b
	.4byte 0x00000027
	.4byte 0x0000000e
	.4byte 0x00000027
	.4byte 0x0000000b
	.4byte 0x00000029
	.4byte 0x00000010
	.4byte 0x0000002a
	.4byte 0x0000000a
	.4byte 0x0000002b
	.4byte 0x0000000e
	.4byte 0x0000002b
	.4byte 0x0000000a
	.4byte 0x0000002e
	.4byte 0x00000010
	.4byte 0x0000002e
	.global Data_020051a4
Data_020051a4:
	.4byte 0x00070605
	.global Data_020051a8
Data_020051a8:
	.4byte 0x00080604
	.global Data_020051ac
Data_020051ac:
	.4byte 0x00080703
	.global Data_020051b0
Data_020051b0:
	.4byte 0x00050403
.L_0200d1b4:
	.4byte 0x00080706
.L_0200d1b8:
	.4byte 0x00080706
.L_0200d1bc:
	.4byte 0x00060504
.L_0200d1c0:
	.4byte 0x00060504
.L_0200d1c4:
	.2byte 0x0007
.L_0200d1c6:
	.2byte 0x0500
.L_0200d1c8:
	.2byte 0x0800
.L_0200d1ca:
	.2byte 0x0500
.L_0200d1cc:
	.4byte 0x00080700
.L_0200d1d0:
	.2byte 0x0003
.L_0200d1d2:
	.2byte 0x0003
	.global gEffectScripts
gEffectScripts:
	.4byte .L_0200d040
	.4byte .L_0200d078
	.4byte .L_0200d0b0
.L_0200d1e0:
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00002126
	.4byte 0x00000015
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte OverlayObject_ApplyLowNibbleOfField100
	.4byte 0x00000010
.L_0200d204:
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00002126
	.4byte 0x00000022
	.4byte OverlayObject_ApplyLowNibbleOfField100
	.4byte 0x00000010
	.global gVinasuLeaderApproachScript
gVinasuLeaderApproachScript:
	.4byte 0x0000001c
	.4byte 0x0000000c
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte OverlayObject_UpdateEveryFourFrames
	.4byte 0x80010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00011999
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00011999
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000e666
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000e666
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000c
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000010
.L_0200d29c:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00009999
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00009999
	.4byte 0x00000015
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte OverlayObject_ApplyZero
	.4byte 0x00000010
	.global gVinasuSprayScript
gVinasuSprayScript:
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000a000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00004000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000a000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x0000000c
	.4byte 0x00000004
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
.L_0200d37c:
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000010
.L_0200d388:
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000010
.L_0200d394:
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000010
.L_0200d3a0:
	.4byte 0x0000001c
	.4byte 0x00000006
	.4byte 0x00000010
.L_0200d3ac:
	.4byte 0x0000001c
	.4byte 0x00000007
	.4byte 0x00000010
.L_0200d3b8:
	.4byte 0x0000001c
	.4byte 0x00000008
	.4byte 0x00000010
	.global gVinasuPushScript
gVinasuPushScript:
	.4byte 0x00000022
	.4byte OverlayObject_ApplyZero
	.4byte 0x0000001c
	.4byte 0x00000007
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte SceneEffect_SpawnRandomizedParticleEveryFourFrames
	.4byte 0x00000003
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000003
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02e70000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000003
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02e70000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gVinasuSettleScriptA
gVinasuSettleScriptA:
	.4byte 0x00000003
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000003
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03480000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
	.global gVinasuSettleScriptB
gVinasuSettleScriptB:
	.4byte 0x00000003
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03480000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
	.global Data_02005564
Data_02005564:
.L_0200d564:
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001b
.L_0200d57c:
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000015
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000010
.L_0200d5d4:
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
.L_0200d60c:
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
.L_0200d644:
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03480000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
.L_0200d67c:
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
.L_0200d6b4:
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000015
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000010
.L_0200d70c:
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03480000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
.L_0200d744:
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
	.global Data_0200577c
Data_0200577c:
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000010
	.global Data_020057c8
Data_020057c8:
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x01080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02e80000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
.L_0200d824:
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00e80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
.L_0200d8a4:
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00b80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
.L_0200d924:
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x01080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
.L_0200d980:
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00b80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02780000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
.L_0200d9b8:
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00e80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02780000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
	.global Data_02005ac8
Data_02005ac8:
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00a80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02e80000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
.L_0200db90:
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00a80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
.L_0200dc34:
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00b80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte Func_02004bd8
	.4byte 0x00000010
.L_0200dc90:
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000015
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000010
	.global gVinasuPushCells
gVinasuPushCells:
	.4byte 0x0032007c
	.4byte 0x00020001
	.4byte 0x007c0006
	.4byte 0x0001002f
	.4byte 0x00060002
	.4byte 0x002c007c
	.4byte 0x00020001
	.4byte 0x007c0006
	.4byte 0x00010029
	.4byte 0x00060002
	.2byte 0xffff
	.global gVinasuSettleCells
gVinasuSettleCells:
	.2byte 0x0075
	.4byte 0x0001003b
	.4byte 0x00060002
	.4byte 0x003b0073
	.4byte 0x00020001
	.4byte 0x00710006
	.4byte 0x0001003b
	.4byte 0x00060002
	.4byte 0x003b006f
	.4byte 0x00020001
	.4byte 0xffff0006
	.global Data_02005d3c
Data_02005d3c:
	.4byte 0x003a0060
	.4byte 0x00020001
	.4byte 0x005f0006
	.4byte 0x0001003a
	.4byte 0x00060002
	.4byte 0x00380060
	.4byte 0x00020001
	.4byte 0x005f0006
	.4byte 0x00010038
	.4byte 0x00060002
	.4byte 0x0000ffff
	.global gVinasuHeyaEntrances1
gVinasuHeyaEntrances1:
	.4byte 0xffff0001
	.4byte 0x00000028
	.4byte 0x00000108
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000220
	.4byte 0xffff0002
	.4byte 0x00000178
	.4byte 0x40000058
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000220
	.4byte 0xffff0003
	.4byte 0x000001e8
	.4byte 0x800001a8
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000220
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEntrancesOther
gVinasuHeyaEntrancesOther:
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0xc00000c8
	.4byte 0x00000000
	.4byte 0x00f00038
	.4byte 0x000000d8
	.4byte 0xffff0002
	.4byte 0x00000078
	.4byte 0x40000068
	.4byte 0x00000000
	.4byte 0x00f00038
	.4byte 0x000000d8
	.4byte 0xffff0003
	.4byte 0x00000158
	.4byte 0xc00000e8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0004
	.4byte 0x000001d8
	.4byte 0x400000b8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0005
	.4byte 0x00000178
	.4byte 0x400000b8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0006
	.4byte 0x00000158
	.4byte 0x40000068
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0007
	.4byte 0x00000348
	.4byte 0xc00000f8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff0008
	.4byte 0x000003c8
	.4byte 0x400000a8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff0009
	.4byte 0x00000358
	.4byte 0x40000058
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000a
	.4byte 0x00000328
	.4byte 0x40000058
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000b
	.4byte 0x000002c8
	.4byte 0x400000a8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000c
	.4byte 0x000000c8
	.4byte 0xc0000198
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff000d
	.4byte 0x00000068
	.4byte 0xc00001f8
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff000e
	.4byte 0x00000068
	.4byte 0x400001d8
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff0014
	.4byte 0x00000208
	.4byte 0x400002b8
	.4byte 0x01300000
	.4byte 0x02400158
	.4byte 0x000002d8
	.4byte 0xffff0015
	.4byte 0x00000158
	.4byte 0x40000238
	.4byte 0x01300000
	.4byte 0x02400158
	.4byte 0x000002d8
	.4byte 0xffff0016
	.4byte 0x000002d8
	.4byte 0x400003a8
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0017
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0018
	.4byte 0x00000188
	.4byte 0x400003b8
	.4byte 0x01100000
	.4byte 0x02000328
	.4byte 0x000003d8
	.4byte 0xffff0019
	.4byte 0x00000188
	.4byte 0x40000358
	.4byte 0x01100000
	.4byte 0x02000328
	.4byte 0x000003d8
	.4byte 0xffff001a
	.4byte 0x000002f8
	.4byte 0x400003a8
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff001e
	.4byte 0x00000058
	.4byte 0x40000368
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff001f
	.4byte 0x000000b8
	.4byte 0x40000368
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff0020
	.4byte 0x000000b8
	.4byte 0x40000288
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEntrances3
gVinasuHeyaEntrances3:
	.4byte 0xffff0001
	.4byte 0x000001c8
	.4byte 0x40000228
	.4byte 0x01780000
	.4byte 0x02e00158
	.4byte 0x00000270
	.4byte 0xffff0002
	.4byte 0x000001c8
	.4byte 0xc0000250
	.4byte 0x01780000
	.4byte 0x02e00158
	.4byte 0x00000270
	.4byte 0xffff0003
	.4byte 0x000002b8
	.4byte 0x400001d8
	.4byte 0x01780000
	.4byte 0x02e00158
	.4byte 0x00000270
	.4byte 0xffff0004
	.4byte 0x000002b8
	.4byte 0xc0000200
	.4byte 0x01780000
	.4byte 0x02e00158
	.4byte 0x00000270
	.4byte 0xffff0005
	.4byte 0x000001a8
	.4byte 0x40000088
	.4byte 0x01580000
	.4byte 0x02500030
	.4byte 0x000000d8
	.4byte 0xffff0006
	.4byte 0x00000208
	.4byte 0xc00000b0
	.4byte 0x01580000
	.4byte 0x02500030
	.4byte 0x000000d8
	.4byte 0xffff0007
	.4byte 0x00000068
	.4byte 0xc00002f0
	.4byte 0x00380000
	.4byte 0x01300248
	.4byte 0x00000370
	.4byte 0xffff0008
	.4byte 0x000000c8
	.4byte 0x400002a8
	.4byte 0x00380000
	.4byte 0x01300248
	.4byte 0x00000370
	.4byte 0xffff0009
	.4byte 0x00000058
	.4byte 0x40000158
	.4byte 0x00280000
	.4byte 0x01200118
	.4byte 0x000001f0
	.4byte 0xffff000a
	.4byte 0x000000f8
	.4byte 0xc00001b0
	.4byte 0x00280000
	.4byte 0x01200118
	.4byte 0x000001f0
	.4byte 0xffff000b
	.4byte 0x00000298
	.4byte 0x40000108
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff000c
	.4byte 0x000002d8
	.4byte 0x40000088
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff000d
	.4byte 0x00000348
	.4byte 0x40000088
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff000e
	.4byte 0x000003b8
	.4byte 0x40000088
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff000f
	.4byte 0x00000348
	.4byte 0xc0000168
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff0010
	.4byte 0x00000058
	.4byte 0x40000098
	.4byte 0x00280000
	.4byte 0x01480028
	.4byte 0x000000d0
	.4byte 0xffff0011
	.4byte 0x000000b8
	.4byte 0x40000068
	.4byte 0x00280000
	.4byte 0x01480028
	.4byte 0x000000d0
	.4byte 0xffff0012
	.4byte 0x00000118
	.4byte 0x40000098
	.4byte 0x00280000
	.4byte 0x01480028
	.4byte 0x000000d0
	.4byte 0xffff0013
	.4byte 0x000001d8
	.4byte 0x40000308
	.4byte 0x01a80000
	.4byte 0x02a002b0
	.4byte 0x00000350
	.4byte 0xffff0014
	.4byte 0x00000258
	.4byte 0x40000308
	.4byte 0x01a80000
	.4byte 0x02a002b0
	.4byte 0x00000350
	.4byte 0xffff0015
	.4byte 0x00000348
	.4byte 0x40000088
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEntrances4
gVinasuHeyaEntrances4:
	.4byte 0xffff0001
	.4byte 0x00000068
	.4byte 0x40000078
	.4byte 0x00380000
	.4byte 0x01700038
	.4byte 0x00000110
	.4byte 0xffff0002
	.4byte 0x00000138
	.4byte 0x400000c8
	.4byte 0x00380000
	.4byte 0x01700038
	.4byte 0x00000110
	.4byte 0xffff0003
	.4byte 0x00000258
	.4byte 0xc00000d8
	.4byte 0x02280000
	.4byte 0x03500058
	.4byte 0x00000220
	.4byte 0xffff0004
	.4byte 0x000002b8
	.4byte 0x40000138
	.4byte 0x02280000
	.4byte 0x03500058
	.4byte 0x00000220
	.4byte 0xffff0005
	.4byte 0x00000288
	.4byte 0xc0000218
	.4byte 0x02280000
	.4byte 0x03500058
	.4byte 0x00000220
	.4byte 0xffff0006
	.4byte 0x00000308
	.4byte 0x40000218
	.4byte 0x02280000
	.4byte 0x03500058
	.4byte 0x00000220
	.4byte 0xffff0007
	.4byte 0x00000068
	.4byte 0x40000208
	.4byte 0x00380000
	.4byte 0x01600188
	.4byte 0x00000260
	.4byte 0xffff0008
	.4byte 0x00000138
	.4byte 0xc0000248
	.4byte 0x00380000
	.4byte 0x01600188
	.4byte 0x00000260
	.4byte 0xffff0009
	.4byte 0x000002e8
	.4byte 0x400002b8
	.4byte 0x02880000
	.4byte 0x03800278
	.4byte 0x00000320
	.4byte 0xffff000a
	.4byte 0x00000328
	.4byte 0x400002b8
	.4byte 0x02880000
	.4byte 0x03800278
	.4byte 0x00000320
	.4byte 0xffff000b
	.4byte 0x00000138
	.4byte 0x40000218
	.4byte 0x00380000
	.4byte 0x01600188
	.4byte 0x00000260
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEntrances5
gVinasuHeyaEntrances5:
	.4byte 0xffff0001
	.4byte 0x00000088
	.4byte 0x40000088
	.4byte 0x00080000
	.4byte 0x01000038
	.4byte 0x000000e0
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0xc00000b8
	.4byte 0x00080000
	.4byte 0x01000038
	.4byte 0x000000e0
	.4byte 0xffff0003
	.4byte 0x00000088
	.4byte 0xc0000188
	.4byte 0x00080000
	.4byte 0x01000108
	.4byte 0x000001b0
	.4byte 0xffff0004
	.4byte 0x00000138
	.4byte 0x400002a8
	.4byte 0x00f80000
	.4byte 0x02000258
	.4byte 0x00000300
	.4byte 0xffff0005
	.4byte 0x00000188
	.4byte 0x400002a8
	.4byte 0x00f80000
	.4byte 0x02000258
	.4byte 0x00000300
	.4byte 0xffff0006
	.4byte 0x000001d8
	.4byte 0x400002a8
	.4byte 0x00f80000
	.4byte 0x02000258
	.4byte 0x00000300
	.4byte 0xffff0007
	.4byte 0x00000088
	.4byte 0x40000318
	.4byte 0x00080000
	.4byte 0x010002d8
	.4byte 0x000003a0
	.4byte 0xffff0008
	.4byte 0x00000088
	.4byte 0xc0000388
	.4byte 0x00080000
	.4byte 0x010002d8
	.4byte 0x000003a0
	.4byte 0xffff0009
	.4byte 0x00000138
	.4byte 0xc00000e8
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000a
	.4byte 0x00000178
	.4byte 0x400000b8
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000b
	.4byte 0x000001f8
	.4byte 0x400000f8
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000c
	.4byte 0x000002a8
	.4byte 0x40000078
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000d
	.4byte 0x00000138
	.4byte 0x400001b8
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000e
	.4byte 0x00000278
	.4byte 0xc0000188
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000f
	.4byte 0x000002d8
	.4byte 0x40000278
	.4byte 0x02a80000
	.4byte 0x03a00238
	.4byte 0x00000300
	.4byte 0xffff0010
	.4byte 0x00000378
	.4byte 0x40000278
	.4byte 0x02a80000
	.4byte 0x03a00238
	.4byte 0x00000300
	.4byte 0xffff0011
	.4byte 0x00000088
	.4byte 0x40000238
	.4byte 0x00080000
	.4byte 0x010001e0
	.4byte 0x00000288
	.4byte 0xffff0012
	.4byte 0x00000088
	.4byte 0xc0000258
	.4byte 0x00080000
	.4byte 0x010001e0
	.4byte 0x00000288
	.4byte 0xffff0013
	.4byte 0x000001b8
	.4byte 0xc0000108
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff0014
	.4byte 0x00000238
	.4byte 0xc0000108
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEntrances6
gVinasuHeyaEntrances6:
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0x400000f8
	.4byte 0x00480000
	.4byte 0x01400038
	.4byte 0x000001a0
	.4byte 0xffff0002
	.4byte 0x00000118
	.4byte 0xc0000118
	.4byte 0x00480000
	.4byte 0x01400038
	.4byte 0x000001a0
	.4byte 0xffff0003
	.4byte 0x000001d8
	.4byte 0x40000148
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0004
	.4byte 0x00000218
	.4byte 0x400000d8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0005
	.4byte 0x00000298
	.4byte 0x40000058
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0006
	.4byte 0x000002e8
	.4byte 0x40000098
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0007
	.4byte 0x00000348
	.4byte 0x40000098
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0008
	.4byte 0x000003a8
	.4byte 0xc0000118
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0009
	.4byte 0x000001d8
	.4byte 0x400001d8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff000a
	.4byte 0x00000268
	.4byte 0xc00001c8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff000b
	.4byte 0x000002d8
	.4byte 0xc00001c8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff000c
	.4byte 0x00000338
	.4byte 0xc00001c8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff000d
	.4byte 0x00000318
	.4byte 0x40000288
	.4byte 0x02c80000
	.4byte 0x03d00248
	.4byte 0x00000360
	.4byte 0xffff000e
	.4byte 0x00000388
	.4byte 0x40000288
	.4byte 0x02c80000
	.4byte 0x03d00248
	.4byte 0x00000360
	.4byte 0xffff000f
	.4byte 0x00000068
	.4byte 0x40000308
	.4byte 0x00380000
	.4byte 0x01300218
	.4byte 0x00000350
	.4byte 0xffff0010
	.4byte 0x000000d8
	.4byte 0x40000258
	.4byte 0x00380000
	.4byte 0x01300218
	.4byte 0x00000350
	.4byte 0xffff0011
	.4byte 0x00000258
	.4byte 0x400002e8
	.4byte 0x01880000
	.4byte 0x02800288
	.4byte 0x00000340
	.4byte 0xffff0012
	.4byte 0x00000218
	.4byte 0x40000148
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0013
	.4byte 0x00000318
	.4byte 0x40000148
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0014
	.4byte 0x00000298
	.4byte 0x40000088
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPrimaryTable
gVinasuHeyaPrimaryTable:
	.4byte 0x000000b5
	.4byte 0x00120002
	.4byte 0x002010b6
	.4byte 0x0033d002
	.4byte 0x000000b6
	.4byte 0x001020b5
	.4byte 0x002030b6
	.4byte 0x003020b6
	.4byte 0x0041f0b6
	.4byte 0x0051e0b6
	.4byte 0x006070b6
	.4byte 0x007060b6
	.4byte 0x008200b6
	.4byte 0x009160b6
	.4byte 0x00a0c0b6
	.4byte 0x00b0d0b6
	.4byte 0x00c0a0b6
	.4byte 0x00d0b0b6
	.4byte 0x00e180b6
	.4byte 0x0141a0b6
	.4byte 0x015190b6
	.4byte 0x016090b6
	.4byte 0x017100b7
	.4byte 0x0180e0b6
	.4byte 0x019150b6
	.4byte 0x01a140b6
	.4byte 0x01e050b6
	.4byte 0x01f040b6
	.4byte 0x020080b6
	.4byte 0x000000b7
	.4byte 0x001130b7
	.4byte 0x002090b7
	.4byte 0x003010b8
	.4byte 0x004050b7
	.4byte 0x005040b7
	.4byte 0x0060d0b7
	.4byte 0x0070e0b7
	.4byte 0x008040b8
	.4byte 0x009020b7
	.4byte 0x00a0b0b7
	.4byte 0x00b0a0b7
	.4byte 0x00c070b8
	.4byte 0x00d060b7
	.4byte 0x00e070b7
	.4byte 0x00f110b7
	.4byte 0x010170b6
	.4byte 0x0110f0b7
	.4byte 0x012100ad
	.4byte 0x013010b7
	.4byte 0x014140b7
	.4byte 0x0150b0b8
	.4byte 0x000000b8
	.4byte 0x001030b7
	.4byte 0x002030b8
	.4byte 0x003020b8
	.4byte 0x004080b7
	.4byte 0x0050a0b8
	.4byte 0x006110b9
	.4byte 0x0070c0b7
	.4byte 0x008090b8
	.4byte 0x009080b8
	.4byte 0x00a050b8
	.4byte 0x00b150b7
	.4byte 0x000000b9
	.4byte 0x001010ba
	.4byte 0x002040b9
	.4byte 0x003050b9
	.4byte 0x004020b9
	.4byte 0x005030b9
	.4byte 0x006090b9
	.4byte 0x007050ba
	.4byte 0x0080b0b9
	.4byte 0x009060b9
	.4byte 0x00a040ba
	.4byte 0x00b080b9
	.4byte 0x00c070ba
	.4byte 0x00d090ba
	.4byte 0x00e0f0b9
	.4byte 0x00f0e0b9
	.4byte 0x010120b9
	.4byte 0x011060b8
	.4byte 0x012100b9
	.4byte 0x013120ba
	.4byte 0x014130ba
	.4byte 0x000000ba
	.4byte 0x001010b9
	.4byte 0x002030ba
	.4byte 0x003020ba
	.4byte 0x0040a0b9
	.4byte 0x005070b9
	.4byte 0x006110ba
	.4byte 0x0070c0b9
	.4byte 0x008100ba
	.4byte 0x0090d0b9
	.4byte 0x00a0d0ba
	.4byte 0x00b0e0ba
	.4byte 0x00c0f0ba
	.4byte 0x00d0a0ba
	.4byte 0x00e0b0ba
	.4byte 0x00f0c0ba
	.4byte 0x010080ba
	.4byte 0x011060ba
	.4byte 0x012130b9
	.4byte 0x013140b9
	.4byte 0x014140ba
	.4byte 0x015010bb
	.4byte 0x000001ff
	.global gVinasuHeyaPlacementsOther
gVinasuHeyaPlacementsOther:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPlacements1
gVinasuHeyaPlacements1:
	.4byte 0x09810098
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x010c0000
	.4byte 0x00024000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00024000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01ac0000
	.4byte 0x00024000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x019c0000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x016c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPlacements2
gVinasuHeyaPlacements2:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x00000070
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
	.4byte 0x00000071
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00024000
	.4byte 0x00000070
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0102c000
	.4byte 0x000001f4
	.4byte .L_0200d1e0
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00024000
	.4byte 0x00000114
	.4byte .L_0200d29c
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte .L_0200d1e0
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte .L_0200d1e0
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte .L_0200d1e0
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte .L_0200d1e0
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte .L_0200d1e0
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte .L_0200d1e0
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte .L_0200d1e0
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte .L_0200d1e0
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte .L_0200d1e0
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPlacements3
gVinasuHeyaPlacements3:
	.4byte 0x000000fd
	.4byte .L_0200d204
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0x000000fd
	.4byte .L_0200d204
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x01024000
	.4byte 0x000000fd
	.4byte .L_0200d204
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x01024000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0102c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPlacements4
gVinasuHeyaPlacements4:
	.4byte 0x000001f4
	.4byte .L_0200d1e0
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte .L_0200d37c
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x0002c000
	.4byte 0xffff011c
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPlacements5
gVinasuHeyaPlacements5:
	.4byte 0x000001f4
	.4byte .L_0200d1e0
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte .L_0200d3ac
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte .L_0200d3a0
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte .L_0200d388
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0002c000
	.4byte 0xffff011c
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPlacements6
gVinasuHeyaPlacements6:
	.4byte 0x00000101
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002c000
	.4byte 0x00000101
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0102c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0102c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0102c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0102c000
	.4byte 0x000001f4
	.4byte .L_0200d1e0
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte .L_0200d37c
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte .L_0200d3ac
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte .L_0200d3b8
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte .L_0200d394
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0002c000
	.4byte 0xffff011c
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0x000000f2
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEvents1
gVinasuHeyaEvents1:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte FieldScene_RunActorEightTenStepLoop
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgVinasuHeyaIodemBeHappyTheEntranceTo
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgVinasuHeyaOhhhHowCouldTheyAttackMere
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte SceneDialogue_RunActorElevenDialogue
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgVinasuHeyaHowHowDidTheyOpenThe
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgVinasuHeyaTheyAreAlreadyHeadedForBabi
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgVinasuHeyaBabisSoldiersAreAllColossoVictors
	.4byte 0x00008d15
	.4byte 0xffff0408
	.4byte FieldScene_RunActorEightTenStepLoop
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgVinasuHeyaUhnnnIfOnlyICouldMove
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgVinasuHeyaTheirPowersWereSoStrangeWho
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgVinasuHeyaIMustGetInsideVenusLighthouse
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgVinasuHeyaTheYoungBoySeemedToGlow
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgVinasuHeyaTheyWerentInVenusLighthouseVery
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgVinasuHeyaIThoughtIRecognizedOneOf
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEvents2
gVinasuHeyaEvents2:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
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
	.4byte 0x00000021
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000031
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000031
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000031
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000031
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000031
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000021
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x00000021
	.4byte 0xffff001a
	.4byte 0x0000001a
	.4byte 0x00000021
	.4byte 0xffff001e
	.4byte 0x0000001e
	.4byte 0x00000021
	.4byte 0xffff001f
	.4byte 0x0000001f
	.4byte 0x00000021
	.4byte 0xffff0020
	.4byte 0x00000020
	.4byte 0x00000002
	.4byte 0x0200002d
	.4byte FieldScene_RunLeaderSurpriseApproach
	.4byte 0x00000002
	.4byte 0xffff002e
	.4byte VinasuHeya_UpdateFloorSwitch
	.4byte 0x00000202
	.4byte 0xffff0032
	.4byte FieldScene_RunFiveCallSequence
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgVinasuHeyaKradenToldThemToStopBut
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgVinasuHeyaKradenWentInsideButThenCame
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte FieldScene_SetupActorTenCamera
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgVinasuHeyaIfKradenHadntBeenThereWe
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgVinasuHeyaKradenSaidSomethingAboutThisBeing
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgVinasuHeyaIWonderWhatMysteriesTheStatue
	.4byte 0x00000003
	.ifeq EDITION_INTERNATIONAL
	.4byte 0xffff0023
	.4byte SceneDialogue_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0024
	.4byte SceneDialogue_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0025
	.4byte SceneDialogue_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0026
	.4byte SceneDialogue_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0027
	.4byte SceneDialogue_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0028
	.4byte SceneDialogue_ReadRelief
	.4byte 0x00000003
	.else
	.4byte 0xffff0023
	.4byte SceneDialogue_ReadRelief
	.4byte 0x00000003
	.ifndef TBS_EDITION_EN
	.4byte 0xffff0028
	.4byte SceneDialogue_ReadRelief
	.4byte 0x00000003
	.endif
	.endif
	.4byte 0xffff0029
	.4byte FieldScene_RunStatueDialogueSequence
	.4byte 0x00000013
	.4byte 0x0f370064
	.4byte 0x001000a1
	.4byte 0x00000013
	.4byte 0x0f380065
	.4byte 0x001000ce
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte FieldScene_RunFlag986ActorOneScene
	.4byte 0x00008c15
	.4byte 0x0200000d
	.4byte SceneState_RunActor13AtColumn42Setup
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte FieldScene_SetFlag987AtActorTwelveTile
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte SceneState_ApplySixRectsAfterFlag161
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte SceneState_ApplySixRectsAfter161
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEvents3
gVinasuHeyaEvents3:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
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
	.4byte 0x00000021
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
	.4byte 0x00000021
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
	.4byte 0x00000021
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000031
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000021
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte SceneState_PassRange0To1
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte Scene_RunActorLeapSequence
	.4byte 0x00000202
	.4byte 0xffff001f
	.4byte SceneState_ApplyRectAt19_44AndRunThree
	.4byte 0x00000202
	.4byte 0xffff0020
	.4byte SceneState_ApplyRectAt19_44AndRunThree
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte SceneState_ClearWorkspaceWord24
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte SceneState_StoreLookupZeroToWord24
	.4byte 0x00000202
	.4byte 0xffff002d
	.4byte FieldScene_RunGuardedThreeStepSetup
	.4byte 0x00000013
	.4byte 0x0f340064
	.4byte 0x001000a2
	.4byte 0x00000003
	.4byte 0x0350006e
	.4byte 0x00300000
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte FieldScene_PlaceAndPinSlots8To10
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte FieldScene_PlaceAndPinSlots8To10
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte FieldScene_PlaceAndPinSlots8To10
	.4byte 0x10008c15
	.4byte 0xffff000b
	.4byte SceneActor_ApplyKind45AtActorsElevenAndTwelve
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte SceneActor_ApplyKind45AtActorsElevenAndTwelve
	.4byte 0x10009315
	.4byte 0xffff000b
	.4byte SceneActor_ApplyKind45AtActorsElevenAndTwelve
	.4byte 0x10009315
	.4byte 0xffff000c
	.4byte SceneActor_ApplyKind45AtActorsElevenAndTwelve
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte SceneActor_ApplyPositionsOfActors11And12
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte SceneActor_ApplyPositionsOfActors11And12
	.4byte 0x00009315
	.4byte 0xffff000b
	.4byte SceneActor_UpdateSlots11And12ByTile
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte SceneActor_UpdateSlots11And12ByTile
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEvents4
gVinasuHeyaEvents4:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte SceneEffect_RunObjectZeroColorSequence
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
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
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte VinasuHeya_ShiftBridge
	.4byte 0x00000202
	.4byte 0x03010024
	.4byte FieldScene_RunGuardedRectStep
	.4byte 0x00000003
	.4byte 0xffff0019
	.4byte SceneState_SetFlag953
	.4byte 0x00008c15
	.4byte 0x03010009
	.4byte VinasuHeya_RunCellPushScene
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEvents5
gVinasuHeyaEvents5:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte SceneEffect_RunObjectZeroColorSequence
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte SceneEffect_RunObjectZeroColorSequence
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte SceneEffect_RunObjectZeroColorSequence
	.4byte 0x00000021
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000021
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000031
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte SceneState_PassRange0To1
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte SceneState_PassRange0To1
	.4byte 0x00000202
	.4byte 0xffff0023
	.4byte SceneState_RunConditionalStep
	.4byte 0x00000003
	.4byte 0xffff0019
	.4byte SceneState_SetFlag953
	.4byte 0x00000013
	.4byte 0x0f350064
	.4byte 0x00100052
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte VinasuHeya_SettlePushedBlocks
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte VinasuHeya_SettlePushedBlocks
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte VinasuHeya_SettlePushedBlocks
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEvents6
gVinasuHeyaEvents6:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
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
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000031
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00004602
	.4byte 0xffff0012
	.4byte FieldScene_RunApproachAndSpawnEffect
	.4byte 0x00000202
	.4byte 0xffff0023
	.4byte FieldScene_RunThreeCallSequence
	.4byte 0x00008602
	.4byte 0xffff0024
	.4byte SceneActor_TryMoveActorZeroTwoTilesAhead
	.4byte 0x00000602
	.4byte 0xffff0024
	.4byte SceneActor_TryMoveActorZeroTwoTilesAhead
	.4byte 0x00000202
	.4byte 0xffff0024
	.4byte FieldScene_RunThreeCallSequence
	.4byte 0x00000602
	.4byte 0xffff0025
	.4byte SceneActor_TryMoveActorZeroTwoTilesAhead
	.4byte 0x00000202
	.4byte 0xffff0025
	.4byte FieldScene_RunThreeCallSequence
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte VinasuHeya_RetractBridge
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte VinasuHeya_ExtendBridge
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte SceneState_PassRange0To1
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte SceneState_PassZeroAndMinusOneRecord
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte SceneState_PassRangeNeg1To0
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte SceneState_CallHandlerWithFlagPair
	.4byte 0x00000202
	.4byte 0xffff002d
	.4byte FieldScene_DrawTilesWhenCheckClear
	.4byte 0x00000003
	.4byte 0xffff0019
	.4byte SceneState_SetFlag953
	.4byte 0x00000013
	.4byte 0x0f360064
	.4byte 0x00100009
	.4byte 0x00000003
	.4byte 0x0351006e
	.4byte 0x00300000
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x00009315
	.4byte 0xffff000a
	.4byte VinasuHeya_ResolveFloatingBlock
	.4byte 0x00009315
	.4byte 0xffff000b
	.4byte VinasuHeya_ResolveFloatingBlock
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte VinasuHeya_ResolveFloatingBlock
	.4byte 0x00009315
	.4byte 0xffff000d
	.4byte VinasuHeya_ResolveFloatingBlock
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte Scene_RunScene3c8SequenceA
	.4byte 0x00008c15
	.4byte 0xffff0010
	.4byte Scene_RunScene3c8SequenceA
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte Scene_RunScene3c8SequenceA
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte Scene_RunScene3c8SequenceA
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200772c
Data_0200772c:
	.4byte .L_0200d1b4
	.4byte .L_0200d1b8
	.4byte .L_0200d1bc
	.4byte .L_0200d1c0
.L_0200f73c:
	.4byte .L_0200d57c
	.4byte .L_0200d5d4
	.4byte .L_0200d564
	.4byte .L_0200d564
.L_0200f74c:
	.4byte .L_0200d60c
	.4byte .L_0200d644
	.4byte .L_0200d564
	.4byte .L_0200d564
.L_0200f75c:
	.4byte .L_0200d6b4
	.4byte .L_0200d67c
	.4byte .L_0200d6b4
	.4byte .L_0200d564
.L_0200f76c:
	.4byte .L_0200d744
	.4byte .L_0200d70c
	.4byte .L_0200d744
	.4byte .L_0200d564
	.global Data_0200777c
Data_0200777c:
	.4byte .L_0200f73c
	.4byte .L_0200f74c
	.4byte .L_0200f75c
	.4byte .L_0200f76c
	.global Data_0200778c
Data_0200778c:
	.4byte .L_0200d1c4
	.4byte .L_0200d1c6
	.4byte .L_0200d1c6 + 0x1
	.4byte .L_0200d1c8 + 0x1
	.4byte .L_0200d1ca + 0x1
	.4byte .L_0200d1cc + 0x1
	.4byte .L_0200d1d0
	.4byte .L_0200d1d2
.L_0200f7ac:
	.4byte .L_0200dc90
	.4byte .L_0200d564
.L_0200f7b4:
	.4byte .L_0200d564
.L_0200f7b8:
	.4byte .L_0200d980
	.4byte .L_0200d564
.L_0200f7c0:
	.4byte .L_0200d9b8
	.4byte .L_0200d564
.L_0200f7c8:
	.4byte .L_0200dc34
	.4byte .L_0200d564
.L_0200f7d0:
	.4byte .L_0200d924
	.4byte .L_0200d8a4
	.4byte .L_0200d564
.L_0200f7dc:
	.4byte .L_0200db90
	.4byte .L_0200d564
.L_0200f7e4:
	.4byte .L_0200d824
	.4byte .L_0200d564
	.global Data_020077ec
Data_020077ec:
	.4byte .L_0200f7ac
	.4byte .L_0200f7b4
	.4byte .L_0200f7b8
	.4byte .L_0200f7c0
	.4byte .L_0200f7c8
	.4byte .L_0200f7d0
	.4byte .L_0200f7dc
	.4byte .L_0200f7e4
