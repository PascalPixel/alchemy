/* Draft: Torebi spring counter layout.
 * 2026-10-01: This international owner omits the Japanese unit marker
 * beside each counter and is 28 bytes shorter than the Japanese owner.
 * Its old Japanese-number/window choices are preserved as an uncredited
 * assemblable attempt, including the complete literal pool.
 */
	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
.syntax unified
.thumb
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
	.ifeq EDITION_INTERNATIONAL
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
	.ifeq EDITION_INTERNATIONAL
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
	.ifeq EDITION_INTERNATIONAL
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
	.ifeq EDITION_INTERNATIONAL
	movs r2, #9
	.else
	movs r2, #10
	.endif
	.endif
	movs r3, #4
	.ifdef TBS_EDITION_EN
	movs r0, #20
	.else
	.ifeq EDITION_INTERNATIONAL
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
