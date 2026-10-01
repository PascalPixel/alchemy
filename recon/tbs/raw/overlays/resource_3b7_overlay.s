.syntax unified
	.thumb
	.section .text.x020084bc,"ax",%progbits
	.global Func_020004bc
	.thumb_func
Func_020004bc:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #8
	adds r5, r0, #0
	bl Engine_EventBegin
	movs r0, #30
	bl Engine_EventWait
	movs r0, #148
	bl Engine_AudioPlayCue
	movs r0, #100
	bl Engine_EventWait
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl Engine_ActorFaceDirection
	movs r0, #40
	bl Engine_EventWait
	movs r3, #8
	str r3, [sp, #4]
	movs r6, #3
	mov r8, r3
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #82
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #3
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #85
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #88
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #91
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #94
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #97
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #100
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #70
	movs r3, #0
	movs r0, #79
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #70
	movs r3, #0
	movs r0, #82
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #70
	movs r3, #0
	movs r0, #85
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #70
	movs r3, #0
	movs r0, #88
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #70
	movs r3, #0
	movs r0, #91
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #70
	movs r3, #0
	movs r0, #94
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #70
	movs r3, #0
	movs r0, #97
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r2, #70
	movs r3, #0
	movs r1, #29
	movs r0, #100
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #70
	bl Engine_EventWait
	movs r0, #126
	bl Engine_AudioPlayCue
	adds r0, r5, #0
	movs r1, #3
	bl Engine_ItemShowFound
	movs r1, #0
	adds r0, r5, #0
	bl Engine_PartyGiveItem
	movs r0, #20
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #70
	movs r3, #0
	movs r0, #97
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #70
	movs r3, #0
	movs r0, #94
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #70
	movs r3, #0
	movs r0, #91
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #70
	movs r3, #0
	movs r0, #88
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #70
	movs r3, #0
	movs r0, #85
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #70
	movs r3, #0
	movs r0, #82
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #100
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #97
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #94
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #91
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #88
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #85
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #82
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #20
	movs r2, #70
	movs r3, #0
	movs r0, #79
	str r6, [sp, #0]
	bl Map_CopyMetatileIndicesRect
	movs r0, #154
	bl Engine_AudioPlayCue
	movs r0, #8
	bl Engine_EventWait
	bl Engine_EventEnd
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.section .text.x02008ac8,"ax",%progbits
	.global TorebiIzumi_RunSpringGame
	.thumb_func
TorebiIzumi_RunSpringGame:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #0
	mov r10, r3
	sub sp, #4
	bl Engine_EventBegin
	bl Battle_ResetEffectCounter
	ldr r3, .L_02008d4c
	mov r9, r3
	ldr r3, .L_02008d50
	mov r11, r3
.L_02008aec:
	mov r3, r11
	ldr r3, [r3, #16]
	movs r0, #229
	mov r8, r3
	bl PartyInventory_CountItem
	adds r7, r0, #0
	mov r0, r9
	bl Engine_EventSetMessage
	movs r0, #1
	movs r1, #0
	negs r0, r0
	bl Engine_EventOpenMessage
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #0
	.ifdef TBS_EDITION_EN
	movs r2, #17
	.else
	.ifdef TBS_EDITION_JA
	movs r2, #17
	.else
	movs r2, #19
	.endif
	.endif
	movs r3, #4
	movs r0, #0
	bl UiWindow_Create
	ldr r5, .L_02008d54
	adds r6, r0, #0
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r3, #0
	str r3, [sp, #0]
	mov r0, r8
	movs r1, #6
	adds r2, r6, #0
	.ifdef TBS_EDITION_EN
	movs r3, #72
	.else
	.ifdef TBS_EDITION_JA
	movs r3, #72
	.else
	movs r3, #88
	.endif
	.endif
	bl UiText_DrawNumberInWindow
	adds r0, r5, #1
	adds r1, r6, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	movs r3, #8
	str r3, [sp, #0]
	adds r2, r6, #0
	.ifdef TBS_EDITION_EN
	movs r3, #72
	.else
	.ifdef TBS_EDITION_JA
	movs r3, #72
	.else
	movs r3, #88
	.endif
	.endif
	movs r1, #6
	adds r0, r7, #0
	bl UiText_DrawNumberInWindow
	mov r0, r10
	bl Menu_SelectEntry20To21
	movs r1, #2
	mov r10, r0
	adds r0, r6, #0
	bl UiWork_Finalize
	bl UiWork_FinalizePendingCore
	movs r3, #1
	negs r3, r3
	cmp r10, r3
	bne .L_02008b74
	b .L_02008d34
.L_02008b74:
	mov r3, r10
	cmp r3, #0
	bne .L_02008b86
	mov r3, r8
	cmp r3, #0
	bne .L_02008bdc
	mov r0, r9
	adds r0, #1
	b .L_02008b94
.L_02008b86:
	mov r3, r10
	cmp r3, #1
	bne .L_02008bdc
	cmp r7, #0
	bne .L_02008bb4
	mov r0, r9
	adds r0, #2
.L_02008b94:
	bl Engine_EventSetMessage
	movs r0, #1
	negs r0, r0
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #1
	bl Engine_TaskWait
	b .L_02008aec
.L_02008baa:
	movs r0, #112
	bl Engine_AudioPlayCue
	movs r5, #0
	b .L_02008c3c
.L_02008bb4:
	bl PartyInventory_CountFreeSlots
	cmp r0, #0
	bne .L_02008bdc
	mov r0, r9
	adds r0, #4
	bl Engine_EventSetMessage
	movs r0, #1
	movs r1, #0
	negs r0, r0
	bl Engine_EventOpenMessage
	movs r0, #0
	movs r1, #0
	bl Engine_EventChooseYesNo
	cmp r0, #0
	beq .L_02008bdc
	b .L_02008d34
.L_02008bdc:
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #15
	.ifdef TBS_EDITION_EN
	movs r2, #9
	.else
	.ifdef TBS_EDITION_JA
	movs r2, #9
	.else
	movs r2, #10
	.endif
	.endif
	movs r3, #4
	.ifdef TBS_EDITION_EN
	movs r0, #20
	.else
	.ifdef TBS_EDITION_JA
	movs r0, #20
	.else
	movs r0, #19
	.endif
	.endif
	bl UiWindow_Create
	ldr r5, .L_02008d58
	adds r6, r0, #0
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r5, #1
	adds r1, r6, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	movs r0, #5
	bl Engine_TaskWait
	movs r0, #116
	bl Engine_AudioPlayCue
	ldr r5, .L_02008d5c
	movs r7, #1
	b .L_02008c20
.L_02008c1a:
	movs r0, #1
	bl Engine_TaskWait
.L_02008c20:
	ldr r3, [r5]
	ands r3, r7
	cmp r3, #0
	bne .L_02008baa
	ldr r3, [r5]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_02008c1a
	movs r0, #113
	bl Engine_AudioPlayCue
	movs r5, #1
	negs r5, r5
.L_02008c3c:
	adds r0, r6, #0
	movs r1, #2
	bl UiWork_Finalize
	movs r3, #1
	negs r3, r3
	cmp r5, r3
	beq .L_02008d34
	mov r3, r10
	cmp r3, #0
	bne .L_02008c5c
	movs r0, #1
	negs r0, r0
	bl Party_AdjustSixDigitCounterA
	b .L_02008c68
.L_02008c5c:
	mov r3, r10
	cmp r3, #1
	bne .L_02008c68
	movs r0, #229
	bl PartyInventory_Remove
.L_02008c68:
	mov r0, r10
	bl TorebiIzumi_RunSpringRide
	mov r3, r10
	adds r5, r0, #0
	cmp r3, #0
	bne .L_02008cb2
	cmp r5, #4
	beq .L_02008ca4
	ldr r6, .L_02008d60
	lsls r5, r5, #1
	ldrh r0, [r6, r5]
	bl Party_AdjustSixDigitCounterA
	movs r0, #91
	bl Engine_AudioPlayCue
	movs r1, #5
	ldrh r0, [r6, r5]
	bl UiWork_PushValueSlot
	ldr r0, .L_02008d64
	bl Engine_EventSetMessage
	movs r0, #1
	negs r0, r0
	movs r1, #0
	bl Engine_EventShowMessage
	b .L_02008d34
.L_02008ca4:
	movs r0, #113
	bl Engine_AudioPlayCue
	movs r0, #10
	bl Engine_EventWait
	b .L_02008d34
.L_02008cb2:
	lsls r3, r5, #1
	adds r3, r3, r5
	movs r6, #0
	adds r0, r3, #3
	movs r7, #0
	cmp r6, r0
	bge .L_02008cd6
	ldr r2, .L_02008d68
	mov r12, r0
	add r2, r11
.L_02008cc6:
	ldrb r3, [r2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r6, #1
	adds r2, #1
	adds r7, r7, r3
	cmp r6, r12
	blt .L_02008cc6
.L_02008cd6:
	bl Engine_RandomNext
	adds r3, r7, #0
	muls r3, r0
	mov r1, r11
	lsrs r2, r3, #16
	movs r3, #142
	lsls r3, r3, #1
	adds r1, #1
	ldrsb r3, [r1, r3]
	subs r2, r2, r3
	movs r6, #0
	cmp r2, #0
	blt .L_02008d08
	ldr r1, .L_02008d68
	add r1, r11
.L_02008cf6:
	adds r6, #1
	cmp r6, #14
	bgt .L_02008d08
	adds r1, #1
	movs r3, #0
	ldrsb r3, [r1, r3]
	subs r2, r2, r3
	cmp r2, #0
	bge .L_02008cf6
.L_02008d08:
	cmp r6, #15
	bne .L_02008d0e
	movs r6, #14
.L_02008d0e:
	ldr r2, .L_02008d6c
	lsls r3, r6, #2
	ldr r0, [r2, r3]
	bl Func_020004bc
	movs r3, #142
	lsls r3, r3, #1
	mov r0, r11
	adds r1, r6, r3
	adds r0, #1
	ldrb r3, [r0, r1]
	lsls r3, r3, #24
	asrs r2, r3, #24
	cmp r2, #1
	ble .L_02008d34
	lsrs r3, r3, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	strb r3, [r0, r1]
.L_02008d34:
	bl Engine_EventEnd
	movs r0, #0
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_02008d4c:
	.4byte MsgTorebiThrow
.L_02008d50:
	.4byte gCell
.L_02008d54:
	.4byte MsgTorebiCoins
.L_02008d58:
	.4byte MsgTorebiToss
.L_02008d5c:
	.4byte gKeyState
.L_02008d60:
	.4byte Data_0200200c
.L_02008d64:
	.4byte MsgTorebiWonNumberCoin
.L_02008d68:
	.4byte 0x0000011d
.L_02008d6c:
	.4byte Data_02001fd0
	.section .text.x02008e5c,"ax",%progbits
	.global FieldScene_RunSecondaryScript
	.thumb_func
FieldScene_RunSecondaryScript:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, .L_02009074
	mov r9, r0
	movs r1, #3
	mov r2, r9
	sub sp, #24
	mov r8, r1
	adds r2, #28
.L_02008e74:
	ldr r3, [r2]
	str r3, [r2, #12]
	ldr r3, [r2, #4]
	str r3, [r2, #16]
	ldr r3, [r2, #8]
	str r3, [r2, #20]
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r4, r8
	subs r2, #12
	cmp r4, #0
	bne .L_02008e74
	mov r1, r9
	movs r0, #2
	ldrsh r3, [r1, r0]
	cmp r3, #31
	bgt .L_02008e9a
	b .L_02009224
.L_02008e9a:
	ldr r3, [r1, #4]
	ldr r2, [r1, #64]
	adds r3, r3, r2
	str r3, [r1, #4]
	mov r2, r9
	ldr r1, [r1, #8]
	ldr r0, [r2, #68]
	adds r1, r1, r0
	ldr r3, [r2, #12]
	str r1, [r2, #8]
	ldr r2, [r2, #72]
	mov r4, r9
	adds r3, r3, r2
	str r3, [r4, #12]
	cmp r1, #0
	ble .L_02008ebc
	b .L_0200921c
.L_02008ebc:
	mov r1, r8
	str r1, [r4, #8]
	cmp r0, #0
	beq .L_02008ee8
	str r1, [r4, #68]
	ldr r3, .L_02009078
	ldr r3, [r3]
	cmp r3, #1
	bne .L_02008edc
	movs r0, #17
	bl Object_GetById
	movs r1, #1
	bl Object_SetMode
	b .L_02008ee8
.L_02008edc:
	movs r0, #12
	bl Object_GetById
	movs r1, #1
	bl Object_SetMode
.L_02008ee8:
	mov r2, r9
	ldr r3, [r2, #76]
	cmp r3, #0
	ble .L_02008f72
	ldr r3, [r2, #4]
	movs r2, #240
	lsls r2, r2, #15
	subs r3, r2, r3
	mov r4, r9
	asrs r7, r3, #8
	movs r6, #142
	ldr r3, [r4, #12]
	lsls r6, r6, #15
	subs r6, r6, r3
	asrs r6, r6, #8
	adds r0, r7, #0
	muls r0, r7
	adds r3, r6, #0
	muls r3, r6
	adds r0, r0, r3
	ldr r3, .L_0200907c
	bl _call_via_r3
	mov r10, r0
	ldr r0, .L_02009080
	mov r8, r0
	mov r0, r8
	muls r0, r7
	mov r1, r10
	bl __divsi3
	mov r1, r9
	ldr r3, [r1, #64]
	adds r7, r3, r0
	str r7, [r1, #64]
	mov r0, r8
	muls r0, r6
	mov r1, r10
	bl __divsi3
	mov r2, r9
	ldr r3, [r2, #72]
	adds r2, r3, r0
	mov r3, r9
	str r2, [r3, #72]
	lsls r3, r7, #6
	subs r3, r3, r7
	lsls r3, r3, #2
	adds r3, r3, r7
	cmp r3, #0
	bge .L_02008f50
	adds r3, #255
.L_02008f50:
	asrs r3, r3, #8
	mov r4, r9
	str r3, [r4, #64]
	lsls r3, r2, #6
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r2
	cmp r3, #0
	bge .L_02008f64
	adds r3, #255
.L_02008f64:
	mov r0, r9
	asrs r3, r3, #8
	str r3, [r0, #72]
	ldr r3, [r0, #76]
	subs r3, #1
	str r3, [r0, #76]
	b .L_0200909a
.L_02008f72:
	mov r1, r9
	ldr r3, [r1, #64]
	movs r1, #220
	muls r3, r1
	cmp r3, #0
	bge .L_02008f80
	adds r3, #255
.L_02008f80:
	asrs r2, r3, #8
	mov r3, r9
	str r2, [r3, #64]
	ldr r3, [r3, #72]
	muls r3, r1
	cmp r3, #0
	bge .L_02008f90
	adds r3, #255
.L_02008f90:
	ldr r0, .L_02009084
	asrs r3, r3, #8
	mov r4, r9
	str r3, [r4, #72]
	adds r3, r2, r0
	ldr r2, .L_02009088
	cmp r3, r2
	bhi .L_02008fa4
	movs r3, #0
	str r3, [r4, #64]
.L_02008fa4:
	mov r1, r9
	ldr r3, [r1, #72]
	ldr r4, .L_02009084
	adds r3, r3, r4
	cmp r3, r2
	bhi .L_02008fb4
	movs r3, #0
	str r3, [r1, #72]
.L_02008fb4:
	mov r0, r9
	ldr r3, [r0, #64]
	cmp r3, #0
	bne .L_0200909a
	ldr r3, [r0, #72]
	cmp r3, #0
	bne .L_0200909a
	ldr r3, .L_02009078
	ldr r3, [r3]
	cmp r3, #1
	bne .L_02008ff0
	movs r0, #17
	bl Object_GetById
	movs r1, #2
	bl Object_SetMode
	movs r0, #15
	movs r1, #0
	bl OverlayObject_SetField54
	movs r0, #14
	movs r1, #0
	bl OverlayObject_SetField54
	movs r0, #13
	movs r1, #0
	bl OverlayObject_SetField54
	b .L_02009014
.L_02008ff0:
	movs r0, #12
	bl Object_GetById
	movs r1, #2
	bl Object_SetMode
	movs r0, #10
	movs r1, #0
	bl OverlayObject_SetField54
	movs r0, #9
	movs r1, #0
	bl OverlayObject_SetField54
	movs r0, #8
	movs r1, #0
	bl OverlayObject_SetField54
.L_02009014:
	mov r1, r9
	ldr r3, [r1, #4]
	movs r2, #240
	lsls r2, r2, #15
	ldr r1, [r1, #12]
	subs r2, r2, r3
	movs r3, #142
	lsls r3, r3, #15
	subs r3, r3, r1
	asrs r2, r2, #16
	asrs r3, r3, #16
	adds r4, r2, #0
	muls r4, r2
	adds r0, r3, #0
	muls r0, r3
	adds r2, r4, #0
	adds r3, r0, #0
	adds r2, r2, r3
	ldr r3, .L_0200908c
	movs r1, #1
	str r1, [r3]
	cmp r2, #224
	bgt .L_02009048
	ldr r2, .L_02009090
	movs r3, #0
	b .L_02009098
.L_02009048:
	movs r3, #156
	lsls r3, r3, #2
	cmp r2, r3
	bgt .L_02009056
	ldr r3, .L_02009090
	str r1, [r3]
	b .L_0200909a
.L_02009056:
	movs r4, #136
	lsls r4, r4, #3
	cmp r2, r4
	bgt .L_02009064
	ldr r2, .L_02009090
	movs r3, #2
	b .L_02009098
.L_02009064:
	movs r0, #210
	lsls r0, r0, #3
	cmp r2, r0
	bgt .L_02009094
	ldr r2, .L_02009090
	movs r3, #3
	b .L_02009098
	.2byte 0x0000
.L_02009074:
	.4byte TorebiIzumi_Ride
.L_02009078:
	.4byte TorebiIzumi_RideSide
.L_0200907c:
	.4byte IwramSqrt
.L_02009080:
	.4byte 0x00001999
.L_02009084:
	.4byte 0x000003ff
.L_02009088:
	.4byte 0x000007fe
.L_0200908c:
	.4byte TorebiIzumi_RideEnded
.L_02009090:
	.4byte TorebiIzumi_RideResult
.L_02009094:
	ldr r2, .L_0200936c
	movs r3, #4
.L_02009098:
	str r3, [r2]
.L_0200909a:
	movs r2, #240
	lsls r2, r2, #15
	mov r3, r9
	movs r1, #192
	mov r10, r2
	movs r0, #168
	ldr r2, [r3, #12]
	movs r7, #192
	lsls r1, r1, #16
	movs r4, #192
	lsls r0, r0, #14
	lsls r7, r7, #14
	mov r8, r1
	lsls r4, r4, #13
	adds r5, r2, #0
	cmp r2, r0
	bge .L_020090f2
	movs r3, #168
	lsls r3, r3, #14
	subs r3, r3, r2
	movs r2, #42
	adds r0, r3, #0
	muls r0, r2
	movs r1, #18
	str r4, [sp, #4]
	bl __divsi3
	movs r1, #192
	lsls r1, r1, #14
	movs r3, #180
	adds r7, r0, r1
	lsls r3, r3, #15
	ldr r4, [sp, #4]
	cmp r7, r3
	ble .L_020090e2
	adds r7, r3, #0
.L_020090e2:
	mov r2, r8
	subs r2, r2, r0
	movs r3, #150
	mov r8, r2
	lsls r3, r3, #16
	cmp r8, r3
	bge .L_020090f2
	mov r8, r3
.L_020090f2:
	movs r0, #204
	lsls r0, r0, #15
	cmp r5, r0
	ble .L_02009130
	movs r3, #42
	adds r0, r5, #0
	muls r0, r3
	ldr r1, .L_02009370
	adds r0, r0, r1
	movs r1, #18
	str r4, [sp, #4]
	bl __divsi3
	movs r2, #192
	lsls r2, r2, #14
	movs r3, #180
	adds r7, r0, r2
	lsls r3, r3, #15
	ldr r4, [sp, #4]
	cmp r7, r3
	ble .L_0200911e
	adds r7, r3, #0
.L_0200911e:
	movs r3, #192
	lsls r3, r3, #16
	subs r3, r3, r0
	mov r8, r3
	movs r3, #150
	lsls r3, r3, #16
	cmp r8, r3
	bge .L_02009130
	mov r8, r3
.L_02009130:
	mov r0, r9
	ldr r5, [r0, #4]
	movs r1, #180
	lsls r1, r1, #15
	adds r6, r5, #0
	cmp r5, r1
	bge .L_02009172
	movs r3, #180
	lsls r3, r3, #15
	subs r3, r3, r5
	lsls r0, r3, #3
	adds r0, r0, r3
	lsls r0, r0, #1
	movs r1, #42
	bl __divsi3
	movs r2, #192
	lsls r2, r2, #13
	movs r3, #168
	adds r4, r0, r2
	lsls r3, r3, #14
	cmp r4, r3
	ble .L_02009160
	adds r4, r3, #0
.L_02009160:
	movs r3, #240
	lsls r3, r3, #15
	subs r3, r3, r0
	mov r10, r3
	movs r3, #204
	lsls r3, r3, #15
	cmp r10, r3
	bge .L_02009172
	mov r10, r3
.L_02009172:
	movs r0, #150
	lsls r0, r0, #16
	cmp r6, r0
	ble .L_020091ac
	lsls r0, r6, #3
	ldr r1, .L_02009374
	adds r0, r0, r6
	lsls r0, r0, #1
	adds r0, r0, r1
	movs r1, #42
	bl __divsi3
	movs r2, #192
	lsls r2, r2, #13
	movs r3, #168
	adds r4, r0, r2
	lsls r3, r3, #14
	cmp r4, r3
	ble .L_0200919a
	adds r4, r3, #0
.L_0200919a:
	movs r3, #240
	lsls r3, r3, #15
	subs r3, r3, r0
	mov r10, r3
	movs r3, #204
	lsls r3, r3, #15
	cmp r10, r3
	bge .L_020091ac
	mov r10, r3
.L_020091ac:
	cmp r6, r7
	bge .L_020091c6
	mov r0, r9
	ldr r3, [r0, #64]
	str r7, [r0, #4]
	cmp r3, #0
	bge .L_020091c4
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r0, #64]
.L_020091c4:
	adds r5, r7, #0
.L_020091c6:
	cmp r5, r8
	ble .L_020091e2
	mov r2, r9
	ldr r3, [r2, #64]
	mov r1, r8
	str r1, [r2, #4]
	cmp r3, #0
	ble .L_020091e2
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r0, r9
	str r3, [r0, #64]
.L_020091e2:
	mov r1, r9
	ldr r2, [r1, #12]
	cmp r2, r4
	bge .L_020091fe
	ldr r3, [r1, #72]
	str r4, [r1, #12]
	cmp r3, #0
	bge .L_020091fc
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r1, #72]
.L_020091fc:
	adds r2, r4, #0
.L_020091fe:
	cmp r2, r10
	ble .L_02009224
	mov r3, r9
	mov r2, r10
	str r2, [r3, #12]
	ldr r3, [r3, #72]
	cmp r3, #0
	ble .L_02009224
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r4, r9
	str r3, [r4, #72]
	b .L_02009224
.L_0200921c:
	ldr r1, .L_02009378
	mov r2, r9
	adds r3, r0, r1
	str r3, [r2, #68]
.L_02009224:
	movs r3, #0
	mov r8, r3
.L_02009228:
	mov r4, r8
	lsls r3, r4, #1
	ldr r2, .L_0200937c
	add r3, r8
	lsls r3, r3, #3
	adds r6, r3, r2
	movs r0, #18
	ldrsh r3, [r6, r0]
	ldrh r2, [r6, #18]
	cmp r3, #0
	ble .L_02009242
	subs r3, r2, #1
	strh r3, [r6, #18]
.L_02009242:
	movs r1, #20
	ldrsh r3, [r6, r1]
	ldrh r2, [r6, #20]
	cmp r3, #0
	ble .L_02009250
	subs r3, r2, #1
	strh r3, [r6, #20]
.L_02009250:
	mov r2, r8
	cmp r2, #1
	bgt .L_020092f4
	movs r4, #16
	ldrsh r3, [r6, r4]
	movs r5, #128
	lsls r5, r5, #9
	cmp r3, #1
	bne .L_02009264
	lsls r5, r5, #1
.L_02009264:
	cmp r3, #2
	bne .L_0200926c
	lsls r3, r5, #1
	adds r5, r3, r5
.L_0200926c:
	movs r0, #18
	ldrsh r3, [r6, r0]
	cmp r3, #0
	ble .L_02009282
	mov r1, r8
	cmp r1, #0
	bne .L_0200927e
	movs r0, #18
	b .L_0200931a
.L_0200927e:
	movs r0, #19
	b .L_0200931a
.L_02009282:
	mov r2, r8
	cmp r2, #0
	bne .L_02009296
	movs r0, #18
	bl Object_GetById
	movs r1, #1
	bl Object_SetMode
	b .L_020092a2
.L_02009296:
	movs r0, #19
	bl Object_GetById
	movs r1, #1
	bl Object_SetMode
.L_020092a2:
	movs r4, #14
	ldrsh r3, [r6, r4]
	ldrh r2, [r6, #14]
	cmp r3, #0
	bne .L_020092f0
	movs r0, #12
	ldrsh r3, [r6, r0]
	cmp r3, #0
	bne .L_020092ba
	ldr r3, [r6]
	adds r3, r3, r5
	b .L_020092be
.L_020092ba:
	ldr r3, [r6]
	subs r3, r3, r5
.L_020092be:
	str r3, [r6]
	movs r1, #128
	ldr r2, [r6]
	lsls r1, r1, #15
	cmp r2, r1
	bgt .L_020092d8
	movs r3, #0
	strh r3, [r6, #12]
	mov r3, r8
	cmp r3, #1
	bne .L_020092d8
	movs r3, #30
	strh r3, [r6, #14]
.L_020092d8:
	ldr r4, .L_02009380
	cmp r2, r4
	bgt .L_020092e0
	b .L_02009410
.L_020092e0:
	movs r3, #1
	mov r0, r8
	strh r3, [r6, #12]
	cmp r0, #1
	beq .L_020092ec
	b .L_02009410
.L_020092ec:
	movs r3, #30
	b .L_0200940e
.L_020092f0:
	subs r3, r2, #1
	b .L_0200940e
.L_020092f4:
	mov r1, r8
	cmp r1, #2
	bne .L_02009384
	movs r2, #16
	ldrsh r3, [r6, r2]
	movs r5, #64
	negs r5, r5
	cmp r3, #1
	bne .L_02009308
	lsls r5, r5, #1
.L_02009308:
	cmp r3, #2
	bne .L_02009310
	lsls r3, r5, #1
	adds r5, r3, r5
.L_02009310:
	movs r4, #18
	ldrsh r3, [r6, r4]
	cmp r3, #0
	ble .L_02009326
	movs r0, #20
.L_0200931a:
	bl Object_GetById
	movs r1, #3
	bl Object_SetMode
	b .L_02009410
.L_02009326:
	movs r0, #20
	bl Object_GetById
	movs r1, #2
	bl Object_SetMode
	movs r1, #12
	ldrsh r0, [r6, r1]
	bl Engine_MathSin
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r2, #224
	lsls r2, r2, #15
	lsls r3, r3, #4
	adds r3, r3, r2
	str r3, [r6]
	movs r3, #12
	ldrsh r0, [r6, r3]
	bl Engine_MathCos
	lsls r3, r0, #2
	adds r3, r3, r0
	movs r4, #144
	lsls r3, r3, #3
	lsls r4, r4, #15
	adds r3, r3, r4
	str r3, [r6, #8]
	ldrh r3, [r6, #12]
	adds r2, r3, r5
	ldrh r3, [r6, #14]
	adds r3, #1
	strh r2, [r6, #12]
	b .L_0200940e
	.2byte 0x0000
.L_0200936c:
	.4byte TorebiIzumi_RideResult
.L_02009370:
	.4byte 0xef440000
.L_02009374:
	.4byte 0xf5740000
.L_02009378:
	.4byte 0xffffc000
.L_0200937c:
	.4byte TorebiIzumi_ActorRecords
.L_02009380:
	.4byte 0x00afffff
.L_02009384:
	ldr r3, .L_020093a0
	ldrh r2, [r6, #14]
	ands r2, r3
	movs r0, #16
	ldrsh r3, [r6, r0]
	movs r5, #64
	cmp r3, #1
	bne .L_02009396
	movs r5, #128
.L_02009396:
	cmp r3, #2
	bne .L_020093a4
	lsls r3, r5, #1
	adds r5, r3, r5
	b .L_020093a4
.L_020093a0:
	.4byte 0x000001ff
.L_020093a4:
	movs r1, #18
	ldrsh r3, [r6, r1]
	cmp r3, #0
	ble .L_020093ba
	movs r0, #21
	bl Object_GetById
	movs r1, #3
	bl Object_SetMode
	b .L_0200940a
.L_020093ba:
	ldr r3, .L_0200968c
	cmp r2, r3
	bgt .L_020093fe
	movs r4, #12
	ldrsh r0, [r6, r4]
	bl Engine_MathSin
	movs r3, #52
	muls r3, r0
	movs r0, #224
	lsls r0, r0, #15
	adds r3, r3, r0
	str r3, [r6]
	movs r1, #12
	ldrsh r0, [r6, r1]
	bl Engine_MathCos
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r2, #144
	lsls r2, r2, #15
	lsls r3, r3, #3
	adds r3, r3, r2
	str r3, [r6, #8]
	ldrh r3, [r6, #12]
	adds r3, r3, r5
	strh r3, [r6, #12]
	movs r0, #21
	bl Object_GetById
	movs r1, #2
	bl Object_SetMode
	b .L_0200940a
.L_020093fe:
	movs r0, #21
	bl Object_GetById
	movs r1, #3
	bl Object_SetMode
.L_0200940a:
	ldrh r3, [r6, #14]
	adds r3, #1
.L_0200940e:
	strh r3, [r6, #14]
.L_02009410:
	movs r4, #20
	ldrsh r3, [r6, r4]
	cmp r3, #0
	bne .L_020094ce
	mov r0, r9
	ldr r3, [r0, #8]
	cmp r3, #0
	bne .L_020094ce
	ldr r2, [r0, #4]
	ldr r3, [r6]
	subs r3, r3, r2
	asrs r7, r3, #16
	ldr r2, [r0, #12]
	ldr r3, [r6, #8]
	subs r3, r3, r2
	asrs r5, r3, #16
	adds r2, r7, #0
	muls r2, r7
	adds r3, r5, #0
	muls r3, r5
	adds r0, r2, r3
	cmp r0, #119
	bgt .L_020094ce
	mov r2, r9
	ldr r1, [r2, #76]
	cmp r1, #30
	ble .L_020094ce
	movs r4, #192
	mov r3, r8
	lsls r4, r4, #10
	cmp r3, #1
	bgt .L_0200947a
	movs r0, #12
	ldrsh r3, [r6, r0]
	cmp r3, #0
	bne .L_02009466
	ldr r3, [r2, #64]
	cmp r3, r4
	bge .L_020094b2
	adds r3, r1, #0
	subs r3, #100
	str r4, [r2, #64]
	b .L_020094b0
.L_02009466:
	negs r2, r4
	mov r4, r9
	ldr r3, [r4, #64]
	cmp r3, r2
	ble .L_020094b2
	adds r3, r1, #0
	subs r3, #100
	str r2, [r4, #64]
	str r3, [r4, #76]
	b .L_020094b2
.L_0200947a:
	str r4, [sp, #4]
	ldr r3, .L_02009690
	bl _call_via_r3
	ldr r4, [sp, #4]
	adds r2, r0, #0
	negs r3, r7
	adds r0, r3, #0
	muls r0, r4
	adds r1, r2, #0
	str r2, [sp, #8]
	bl __divsi3
	ldr r2, [sp, #8]
	ldr r4, [sp, #4]
	negs r3, r5
	mov r1, r9
	str r0, [r1, #64]
	adds r0, r3, #0
	muls r0, r4
	adds r1, r2, #0
	bl __divsi3
	mov r2, r9
	ldr r3, [r2, #76]
	subs r3, #100
	str r0, [r2, #72]
.L_020094b0:
	str r3, [r2, #76]
.L_020094b2:
	ldr r0, .L_02009694
	bl Engine_AudioPlayCue
	movs r3, #16
	ldrsh r0, [r6, r3]
	movs r1, #3
	adds r0, #1
	bl __modsi3
	movs r3, #36
	strh r3, [r6, #18]
	movs r3, #30
	strh r0, [r6, #16]
	strh r3, [r6, #20]
.L_020094ce:
	mov r4, r8
	cmp r4, #1
	beq .L_020094fc
	cmp r4, #1
	bgt .L_020094de
	cmp r4, #0
	beq .L_020094ea
	b .L_02009556
.L_020094de:
	mov r0, r8
	cmp r0, #2
	beq .L_02009516
	cmp r0, #3
	beq .L_02009538
	b .L_02009556
.L_020094ea:
	movs r1, #16
	ldrsh r2, [r6, r1]
	ldr r3, .L_02009698
	ldrb r3, [r3, r2]
	lsls r2, r2, #4
	adds r2, #16
	str r2, [sp, #0]
	movs r0, #18
	b .L_0200950c
.L_020094fc:
	movs r4, #16
	ldrsh r2, [r6, r4]
	ldr r3, .L_02009698
	ldrb r3, [r3, r2]
	lsls r2, r2, #4
	adds r2, #16
	str r2, [sp, #0]
	movs r0, #19
.L_0200950c:
	adds r1, r6, #0
	movs r2, #0
	bl TorebiIzumi_PlaceActor
	b .L_02009556
.L_02009516:
	movs r0, #12
	ldrsh r3, [r6, r0]
	movs r2, #128
	lsls r2, r2, #8
	movs r4, #16
	ldrsh r1, [r6, r4]
	subs r2, r2, r3
	ldr r3, .L_0200969c
	ldrb r3, [r3, r1]
	lsls r1, r1, #4
	adds r1, #16
	str r1, [sp, #0]
	movs r0, #20
	adds r1, r6, #0
	bl TorebiIzumi_PlaceActor
	b .L_02009556
.L_02009538:
	movs r0, #12
	ldrsh r3, [r6, r0]
	ldr r2, .L_020096a0
	movs r4, #16
	ldrsh r1, [r6, r4]
	subs r2, r2, r3
	ldr r3, .L_0200969c
	ldrb r3, [r3, r1]
	lsls r1, r1, #4
	adds r1, #16
	str r1, [sp, #0]
	movs r0, #21
	adds r1, r6, #0
	bl TorebiIzumi_PlaceActor
.L_02009556:
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #4
	beq .L_02009562
	b .L_02009228
.L_02009562:
	mov r2, r9
	ldr r3, [r2, #4]
	str r3, [r2, #52]
	movs r3, #0
	str r3, [r2, #56]
	ldr r3, [r2, #12]
	str r3, [r2, #60]
	ldr r3, .L_020096a4
	ldr r3, [r3]
	cmp r3, #1
	bne .L_020095f0
	mov r1, r9
	movs r6, #16
	adds r1, #4
	movs r0, #17
	movs r2, #0
	movs r3, #0
	str r6, [sp, #0]
	bl TorebiIzumi_PlaceActor
	mov r1, r9
	adds r1, #52
	movs r0, #16
	movs r2, #0
	movs r3, #0
	str r6, [sp, #0]
	bl TorebiIzumi_PlaceActor
	mov r1, r9
	adds r1, #16
	movs r0, #15
	movs r2, #0
	movs r3, #0
	str r6, [sp, #0]
	bl TorebiIzumi_PlaceActor
	mov r1, r9
	adds r1, #28
	movs r0, #14
	movs r2, #0
	movs r3, #0
	str r6, [sp, #0]
	bl TorebiIzumi_PlaceActor
	mov r1, r9
	movs r2, #0
	movs r3, #0
	adds r1, #40
	movs r0, #13
	str r6, [sp, #0]
	bl TorebiIzumi_PlaceActor
	movs r0, #15
	bl Object_GetById
	movs r1, #4
	bl Object_SetMode
	movs r0, #14
	bl Object_GetById
	movs r1, #4
	bl Object_SetMode
	movs r0, #13
	bl Object_GetById
	movs r1, #4
	bl Object_SetMode
	b .L_02009666
.L_020095f0:
	mov r1, r9
	movs r6, #16
	adds r1, #4
	movs r0, #12
	movs r2, #0
	movs r3, #0
	str r6, [sp, #0]
	bl TorebiIzumi_PlaceActor
	mov r1, r9
	adds r1, #52
	movs r0, #11
	movs r2, #0
	movs r3, #0
	str r6, [sp, #0]
	bl TorebiIzumi_PlaceActor
	mov r1, r9
	adds r1, #16
	movs r0, #10
	movs r2, #0
	movs r3, #0
	str r6, [sp, #0]
	bl TorebiIzumi_PlaceActor
	mov r1, r9
	adds r1, #28
	movs r0, #9
	movs r2, #0
	movs r3, #0
	str r6, [sp, #0]
	bl TorebiIzumi_PlaceActor
	mov r1, r9
	movs r2, #0
	movs r3, #0
	adds r1, #40
	movs r0, #8
	str r6, [sp, #0]
	bl TorebiIzumi_PlaceActor
	movs r0, #10
	bl Object_GetById
	movs r1, #4
	bl Object_SetMode
	movs r0, #9
	bl Object_GetById
	movs r1, #4
	bl Object_SetMode
	movs r0, #8
	bl Object_GetById
	movs r1, #4
	bl Object_SetMode
.L_02009666:
	mov r3, r9
	ldrh r2, [r3, #2]
	movs r0, #1
	movs r4, #2
	ldrsh r3, [r3, r4]
	negs r0, r0
	cmp r3, r0
	beq .L_0200967c
	adds r3, r2, #1
	mov r1, r9
	strh r3, [r1, #2]
.L_0200967c:
	add sp, #24
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_0200968c:
	.4byte 0x0000017f
.L_02009690:
	.4byte IwramSqrt
.L_02009694:
	.4byte 0x0000012d
.L_02009698:
	.4byte Data_02002054
.L_0200969c:
	.4byte Data_02002056 + 0x1
.L_020096a0:
	.4byte 0x0000ffff
.L_020096a4:
	.4byte TorebiIzumi_RideSide
	.section .rodata.x02009a08,"a",%progbits
	.global TorebiIzumi_SceneTableA
TorebiIzumi_SceneTableA:
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0xc00000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000080
	.4byte 0xc00000b0
	.4byte 0x00080000
	.4byte 0x00f80008
	.4byte 0x000000c8
	.4byte 0xffff000b
	.4byte 0x00000060
	.4byte 0xc00001b8
	.4byte 0x00080000
	.4byte 0x00f80120
	.4byte 0x000001c8
	.4byte 0xffff000c
	.4byte 0x00000090
	.4byte 0xc0000186
	.4byte 0x00080000
	.4byte 0x00f80120
	.4byte 0x000001c8
	.4byte 0xffff000d
	.4byte 0x00000078
	.4byte 0x80000098
	.4byte 0x00080000
	.4byte 0x00f80008
	.4byte 0x000000c8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global TorebiIzumi_SceneTableB
TorebiIzumi_SceneTableB:
	.4byte 0x000000bd
	.4byte 0x10114087
	.4byte 0xffffffff
	.4byte 0x00000089
	.4byte 0x1010d087
	.4byte 0xffffffff
	.4byte 0x10205087
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global gTorebiIzumiPlacements2
gTorebiIzumiPlacements2:
	.4byte 0xffff018e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0112
	.4byte 0x00000007
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte 0x00000007
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte 0x00000007
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte 0x00000007
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001c000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiIzumiPlacementsOther
gTorebiIzumiPlacementsOther:
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00015000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00520000
	.4byte 0x00015000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00520000
	.4byte 0x00015000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00015000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0001b000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00010000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00014000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00014000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00014000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00016000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiIzumiEventsOther
gTorebiIzumiEventsOther:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte FieldScene_RunIndexedStep0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgTorebiIzumiHmmIWonderIfTheseKids
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte TorebiIzumi_PayToPlay
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgTorebiIzumiReadTheSignIfYaDont
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte TorebiIzumi_AskIfFirstTime
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgTorebiIzumiWhoaGettingTripleDigitsIsReal
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgTorebiIzumiIDontWantToStopPlaying
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgTorebiIzumiWinningOrLosingIsPureLuck
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgTorebiIzumiTheRulesForLuckyDiceAint
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgTorebiIzumiSometimesPeopleWhoHaveNeverPlayed
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgTorebiIzumiThinkingWontHelpYouWinThis
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgTorebiIzumiMyLuckHasRunOutFor
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte SceneDialogue_RunMessage0e35
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte SceneDialogue_RunMessage0e34
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgTorebiIzumiLuckyDiceThrowTheDiceOn
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgTorebiIzumiLuckyDiceIfTwoNumbersMatch
	.4byte 0x00000000
	.4byte 0x09620011
	.4byte MsgTorebiIzumiYeahThatDiceGameLooksKinda
	.4byte 0x00008d15
	.4byte 0x09620011
	.4byte MsgTorebiIzumiILikePlayingLuckyDiceBut
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgTorebiIzumiOhThereAreAlwaysScalpersAt
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgTorebiIzumiTheresNoWayPoorSoldiersLike
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiIzumiEvents2
gTorebiIzumiEvents2:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgTorebiTossLuckyMedal
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgTorebiFaceAwayTolbi
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte MsgTorebiIzumiTolbiSpringHasMysteriousPowersStand
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte TorebiIzumi_AskForLuckyMedal
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte MsgTorebiIzumiTooBadIOnlyHaveOne
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgTorebiIzumiIGetNervousWhenIToss
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgTorebiIzumiWhyWontAnyoneGiveMeA
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte MsgTorebiIzumiThisIsALuckyMedalSo
	.4byte 0x00000003
	.4byte 0xffff0050
	.4byte TorebiIzumi_WalkLeaderToSpring
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global TorebiIzumi_AlphaSteps
TorebiIzumi_AlphaSteps:
	.4byte 0x0a08090a
	.4byte 0x0c040b06
	.4byte 0x0e020d03
	.4byte 0x10000f01
	.global Data_02001fd0
Data_02001fd0:
	.4byte 0x00000017
	.4byte 0x0000007c
	.4byte 0x00000051
	.4byte 0x00000098
	.4byte 0x00000025
	.4byte 0x0000006c
	.4byte 0x000000ab
	.4byte 0x0000008e
	.4byte 0x00000030
	.4byte 0x000000a0
	.4byte 0x00000085
	.4byte 0x00000061
	.4byte 0x000000b7
	.4byte 0x000000ba
	.4byte 0x000000bd
	.global Data_0200200c
Data_0200200c:
	.4byte 0x000a0014
	.4byte 0x00010002
	.4byte 0x00000000
	.global TorebiIzumi_TopicIds
TorebiIzumi_TopicIds:
	.4byte 0x000000fa
	.4byte 0x000000fb
	.4byte 0x000000fc
	.4byte 0x00000100
	.4byte 0x00000101
	.4byte 0x00000102
	.4byte 0x00000106
	.4byte 0x00000107
	.4byte 0x00000108
	.4byte 0x000000b7
	.4byte 0x000000b6
	.4byte 0x000000b5
	.4byte 0x000000bd
	.4byte 0x000000ba
	.4byte 0x000000bc
	.global Data_02002054
Data_02002054:
	.2byte 0x0100
	.global Data_02002056
Data_02002056:
	.2byte 0x0009
	.2byte 0x0403
	.global TorebiIzumi_ActorTileX
TorebiIzumi_ActorTileX:
	.2byte 0xa050
	.2byte 0x4850
	.global TorebiIzumi_ActorTileZ
TorebiIzumi_ActorTileZ:
	.2byte 0x6820
	.2byte 0x4844
	.global TorebiIzumi_ActorHeadings
TorebiIzumi_ActorHeadings:
	.2byte 0x0000
	.4byte 0x00000001
	.2byte 0x8000
