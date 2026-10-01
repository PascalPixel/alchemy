.syntax unified
	.thumb
	.global Func_080ccec8
	.thumb_func
Func_080ccec8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	mov r11, r0
	bl BattleAction_FindDescriptor
	movs r1, #1
	negs r1, r1
	movs r3, #128
	str r1, [sp, #28]
	lsls r3, r3, #5
	mov r1, r11
	movs r2, #0
	ands r1, r3
	str r0, [sp, #32]
	str r2, [sp, #20]
	str r2, [sp, #16]
	str r2, [sp, #12]
	str r1, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	mov r2, r11
	ands r2, r3
	mov r11, r2
	mov r0, r11
	bl Object_GetById
	mov r3, r11
	movs r5, #0
	str r0, [sp, #24]
	cmp r3, #63
	bgt .L_080ccf1c
	b .L_080cd3b2
.L_080ccf1c:
	cmp r3, #79
	beq .L_080ccf5a
	bl Func_080ceba8
	cmp r0, #0
	beq .L_080ccf76
	ldr r2, .L_080cd150
	mov r1, r11
	lsls r3, r1, #3
	adds r3, r0, r3
	adds r0, r3, r2
	movs r2, #0
	ldrsh r1, [r0, r2]
	mov r10, r11
	mov r8, r1
	ldr r1, .L_080cd154
	movs r5, #1
	adds r0, r3, r1
	movs r1, #0
	ldrsh r2, [r0, r1]
	str r2, [sp, #4]
	ldr r2, .L_080cd158
	adds r0, r3, r2
	movs r2, #0
	ldrsh r1, [r0, r2]
	mov r9, r1
	ldr r1, .L_080cd15c
	adds r0, r3, r1
	movs r2, #0
	ldrsh r7, [r0, r2]
	b .L_080ccf76
.L_080ccf5a:
	ldr r1, [sp, #28]
	movs r2, #208
	str r1, [sp, #4]
	lsls r2, r2, #4
	movs r3, #131
	adds r2, #52
	mov r8, r3
	adds r3, r6, r2
	mov r9, r1
	movs r1, #0
	ldrsh r7, [r3, r1]
	movs r2, #79
	mov r10, r2
	movs r5, #1
.L_080ccf76:
	cmp r5, #0
	bne .L_080ccf7c
	b .L_080cd3b2
.L_080ccf7c:
	mov r5, r8
	adds r3, r5, #0
	subs r3, #128
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #9
	cmp r3, r1
	bhi .L_080ccf8e
	movs r5, #1
.L_080ccf8e:
	mov r3, r8
	subs r3, #130
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #10
	cmp r3, r2
	bhi .L_080ccf9e
	movs r5, #5
.L_080ccf9e:
	mov r3, r8
	cmp r3, #132
	bne .L_080ccfa6
	movs r5, #26
.L_080ccfa6:
	mov r1, r8
	cmp r1, #133
	bne .L_080ccfae
	movs r5, #32
.L_080ccfae:
	bl Func_080d22a8
	ldr r2, [sp, #24]
	movs r3, #1
	adds r2, #91
	str r2, [sp, #0]
	strb r3, [r2]
	ldr r3, [sp, #8]
	cmp r3, #0
	bne .L_080ccfcc
	ldr r0, .L_080cd160
	movs r1, #1
	adds r0, r5, r0
	bl UiText_ShowPositionedMessageAndWaitFar
.L_080ccfcc:
	ldr r3, .L_080cd164
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_080cd168
	cmp r2, r3
	bne .L_080ccffe
	ldr r2, [sp, #4]
	cmp r2, #0
	bne .L_080ccffe
	mov r0, r9
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ccffe
	adds r0, r7, #0
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r0, .L_080cd16c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
.L_080ccffe:
	mov r0, r9
	bl GameFlag_IsConditionActive
	cmp r0, #0
	bne .L_080cd00a
	b .L_080cd394
.L_080cd00a:
	mov r3, r8
	cmp r3, #128
	bne .L_080cd022
	cmp r7, #0
	bne .L_080cd01c
	mov r0, r10
	bl Func_080cefe0
	b .L_080cd022
.L_080cd01c:
	mov r0, r10
	bl Func_080cefb4
.L_080cd022:
	mov r1, r8
	cmp r1, #129
	bne .L_080cd02e
	mov r0, r10
	bl Func_080cefb4
.L_080cd02e:
	mov r2, r8
	cmp r2, #130
	bne .L_080cd0ca
	mov r0, r10
	bl Func_080ceffc
	ldr r3, .L_080cd170
	adds r5, r0, #0
	str r3, [r5, #108]
	bl Func_080cf350
	movs r0, #83
	bl Audio_PlayCue
	adds r0, r7, #0
	movs r1, #5
	bl UiText_DrawQuantity
	ldr r6, .L_080cd174
	movs r1, #3
	adds r0, r6, #0
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r0, #218
	lsls r0, r0, #3
	movs r1, #0
	adds r0, #255
	bl Func_080cb6c8
	movs r0, #1
	bl Func_08038128
	movs r0, #126
	bl Audio_PlayCue
	bl Party_CountActiveOwnersFar
	cmp r0, #1
	bne .L_080cd086
	adds r0, r6, #6
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080cd08e
.L_080cd086:
	adds r0, r6, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
.L_080cd08e:
	bl Func_08038138
	movs r3, #0
	str r3, [r5, #108]
	movs r1, #2
	adds r0, r5, #0
	bl Object_SetMode
	movs r0, #246
	bl Audio_PlayCue
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, .L_080cd178
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	mov r0, r10
	bl Func_080cf004
	movs r3, #1
	negs r3, r3
	cmp r9, r3
	bne .L_080cd0c2
	b .L_080cd39e
.L_080cd0c2:
	mov r0, r9
	bl GameFlag_SetBit
	b .L_080cd39e
.L_080cd0ca:
	mov r1, r8
	cmp r1, #132
	bne .L_080cd106
	bl Func_080d22a8
	movs r0, #0
	bl Func_080cded4
	mov r0, r10
	bl Func_080ea14c
	adds r0, r7, #0
	bl Func_08038340
	adds r0, r7, #0
	bl Func_080ad2f8
	mov r0, r10
	bl Func_080cf004
	movs r2, #1
	negs r2, r2
	cmp r9, r2
	beq .L_080cd100
	mov r0, r9
	bl GameFlag_SetBit
.L_080cd100:
	bl Func_080d2350
	b .L_080cd39e
.L_080cd106:
	mov r3, r8
	cmp r3, #129
	bne .L_080cd1bc
	movs r3, #192
	lsls r3, r3, #18
	mov r0, r10
	ldr r5, [r3, #108]
	bl Func_080cef84
	movs r1, #1
	negs r1, r1
	cmp r9, r1
	beq .L_080cd134
	ldr r3, .L_080cd164
	ldr r2, .L_080cd14c
	movs r1, #149
	lsls r1, r1, #2
	adds r3, r3, r1
	mov r1, r9
	orrs r1, r2
	mov r9, r1
	mov r2, r9
	strh r2, [r3]
.L_080cd134:
	cmp r7, #7
	ble .L_080cd17c
	adds r1, r7, #0
	subs r1, #8
	movs r0, #105
	bl Func_080ca18c
	movs r1, #178
	lsls r1, r1, #1
	adds r3, r5, r1
	b .L_080cd18a
	.2byte 0x0000
.L_080cd14c:
	.4byte 0x00001000
.L_080cd150:
	.4byte 0xfffffe00
.L_080cd154:
	.4byte 0xfffffe02
.L_080cd158:
	.4byte 0xfffffe04
.L_080cd15c:
	.4byte 0xfffffe06
.L_080cd160:
	.4byte 0x00000dc4
.L_080cd164:
	.4byte gPartyState
.L_080cd168:
	.4byte 0x00000005
.L_080cd16c:
	.4byte 0x000015b5
.L_080cd170:
	.4byte BattleFx_EmitRandomParticleFromEmitter
.L_080cd174:
	.4byte 0x00000e17
.L_080cd178:
	.4byte 0x00000e19
.L_080cd17c:
	movs r0, #104
	adds r1, r7, #0
	bl Func_080ca18c
	movs r2, #178
	lsls r2, r2, #1
	adds r3, r5, r2
.L_080cd18a:
	strh r0, [r3]
	ldr r5, .L_080cd49c
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r5, r3
	movs r3, #2
	strb r3, [r2]
	movs r0, #104
	adds r1, r7, #0
	bl Func_080ca5d8
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #14
	adds r5, r5, r1
	movs r2, #0
	ldrsh r0, [r5, r2]
	bl Audio_PlayCue
	ldr r0, .L_080cd4a0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080cd39e
.L_080cd1bc:
	mov r3, r8
	cmp r3, #133
	bne .L_080cd1ca
	ldr r0, .L_080cd4a4
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
.L_080cd1ca:
	mov r0, r10
	bl Func_080ceeac
	cmp r0, #0
	beq .L_080cd1fe
	movs r1, #1
	ldr r0, .L_080cd4a8
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r0, #136
	bl Func_080ad2e8
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_080cd1ec
	b .L_080cd39e
.L_080cd1ec:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
	b .L_080cd39e
.L_080cd1fe:
	movs r3, #128
	lsls r3, r3, #8
	ands r3, r7
	cmp r3, #0
	beq .L_080cd290
	mov r2, r8
	cmp r2, #131
	bne .L_080cd21c
	mov r0, r10
	bl Func_080ceffc
	ldr r3, .L_080cd4ac
	adds r6, r0, #0
	str r3, [r6, #108]
	b .L_080cd238
.L_080cd21c:
	movs r1, #0
	mov r0, r10
	bl BattleFx_StartRandomParticleEmitter
	adds r6, r0, #0
	movs r0, #30
	bl WaitFrames
	mov r3, r8
	cmp r3, #128
	bne .L_080cd238
	mov r0, r10
	bl Func_080cefd0
.L_080cd238:
	adds r0, r6, #0
	bl Func_080cf350
	movs r0, #83
	bl Audio_PlayCue
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	ands r7, r3
	adds r0, r7, #0
	movs r1, #5
	bl UiText_DrawQuantity
	movs r1, #3
	ldr r0, .L_080cd4b0
	bl UiText_ShowPositionedMessageAndWaitFar
	adds r0, r7, #0
	bl Party_AdjustSixDigitCounterAFar
	movs r1, #1
	negs r1, r1
	cmp r9, r1
	beq .L_080cd270
	mov r0, r9
	bl GameFlag_SetBit
.L_080cd270:
	mov r2, r8
	cmp r2, #131
	bne .L_080cd288
	movs r3, #0
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	bl Object_SetPositionAndResetMotionFar
	movs r3, #0
	str r3, [r6, #108]
	b .L_080cd39e
.L_080cd288:
	adds r0, r6, #0
	bl Object_Destroy
	b .L_080cd39e
.L_080cd290:
	cmp r7, #0
	bne .L_080cd2a4
	movs r3, #1
	negs r3, r3
	cmp r9, r3
	beq .L_080cd394
	mov r0, r9
	bl GameFlag_SetBit
	b .L_080cd394
.L_080cd2a4:
	mov r1, r8
	cmp r1, #131
	bne .L_080cd2b8
	mov r0, r10
	bl Func_080ceffc
	ldr r3, .L_080cd4ac
	adds r6, r0, #0
	str r3, [r6, #108]
	b .L_080cd2c8
.L_080cd2b8:
	mov r0, r10
	adds r1, r7, #0
	bl BattleFx_StartRandomParticleEmitter
	adds r6, r0, #0
	movs r0, #30
	bl WaitFrames
.L_080cd2c8:
	adds r0, r7, #0
	bl PartyInventory_AddFar
	movs r2, #1
	adds r5, r0, #0
	negs r2, r2
	cmp r5, r2
	bne .L_080cd30e
	adds r0, r7, #0
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r5, .L_080cd4b4
	movs r1, #1
	adds r0, r5, #0
	adds r5, #4
	bl UiText_ShowPositionedMessageAndWaitFar
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	mov r3, r8
	cmp r3, #131
	beq .L_080cd300
	adds r0, r6, #0
	bl Func_080cf340
.L_080cd300:
	mov r1, r8
	cmp r1, #128
	bne .L_080cd39e
	mov r0, r10
	bl Func_080cef68
	b .L_080cd39e
.L_080cd30e:
	mov r2, r8
	cmp r2, #128
	bne .L_080cd31a
	mov r0, r10
	bl Func_080cefd0
.L_080cd31a:
	adds r0, r6, #0
	bl Func_080cf350
	movs r0, #83
	bl Audio_PlayCue
	movs r1, #2
	adds r0, r7, #0
	bl UiText_DrawQuantity
	ldr r3, .L_080cd49c
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r3, [r3]
	cmp r5, r3
	bne .L_080cd346
	ldr r0, .L_080cd4b8
	movs r1, #3
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080cd356
.L_080cd346:
	adds r0, r5, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_080cd4bc
	movs r1, #3
	bl UiText_ShowPositionedMessageAndWaitFar
.L_080cd356:
	movs r2, #1
	negs r2, r2
	cmp r9, r2
	beq .L_080cd364
	mov r0, r9
	bl GameFlag_SetBit
.L_080cd364:
	mov r3, r8
	cmp r3, #131
	bne .L_080cd37c
	movs r3, #0
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	bl Object_SetPositionAndResetMotionFar
	movs r3, #0
	str r3, [r6, #108]
	b .L_080cd382
.L_080cd37c:
	adds r0, r6, #0
	bl Object_Destroy
.L_080cd382:
	mov r1, r8
	cmp r1, #133
	bne .L_080cd38e
	mov r0, r10
	bl Func_080cf004
.L_080cd38e:
	movs r2, #0
	str r2, [sp, #28]
	b .L_080cd39e
.L_080cd394:
	ldr r0, .L_080cd4c0
	movs r1, #1
	adds r0, r5, r0
	bl UiText_ShowPositionedMessageAndWaitFar
.L_080cd39e:
	ldr r1, [sp, #0]
	movs r3, #0
	strb r3, [r1]
	bl Func_080d2350
	ldr r2, [sp, #28]
	adds r0, r2, #0
	cmp r2, #0
	beq .L_080cd3b2
	b .L_080cd568
.L_080cd3b2:
	ldr r3, .L_080cd49c
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #106
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, r11
	bne .L_080cd416
	movs r3, #1
	movs r0, #7
	mov r1, r11
	str r3, [sp, #16]
	bl Func_080ccd78
	adds r6, r0, #0
	cmp r6, #0
	bne .L_080cd426
	mov r1, r11
	movs r0, #0
	bl Func_080ccd78
	movs r1, #1
	adds r6, r0, #0
	str r1, [sp, #12]
	cmp r6, #0
	bne .L_080cd3ea
	b .L_080cd566
.L_080cd3ea:
	ldr r3, [r6, #8]
	movs r2, #128
	lsls r2, r2, #9
	cmp r3, r2
	blt .L_080cd420
	mov r0, r11
	bl BattleFx_GetFlags
	adds r5, r0, #0
	bl Random16
	ldr r3, .L_080cd4c4
	lsls r0, r0, #1
	lsrs r0, r0, #16
	lsls r5, r5, #1
	adds r5, r5, r0
	adds r5, r5, r3
	mov r0, r11
	adds r1, r5, #0
	bl Func_080cdea8
	b .L_080cd546
.L_080cd416:
	movs r0, #0
	mov r1, r11
	bl Func_080ccd78
	adds r6, r0, #0
.L_080cd420:
	cmp r6, #0
	bne .L_080cd426
	b .L_080cd546
.L_080cd426:
	ldr r3, [r6, #8]
	cmp r3, #0
	bne .L_080cd42e
	b .L_080cd546
.L_080cd42e:
	ldr r1, [sp, #16]
	cmp r1, #0
	bne .L_080cd484
	ldr r2, [sp, #24]
	movs r3, #1
	adds r2, #91
	strb r3, [r2]
	movs r1, #0
	ldr r0, [sp, #24]
	bl ObjectDispatch_ApplyValueToChildrenFar
	ldr r2, [sp, #24]
	ldr r1, [sp, #32]
	ldrh r2, [r2, #6]
	str r2, [sp, #20]
	movs r2, #128
	ldrb r3, [r1, #22]
	lsls r2, r2, #17
	lsls r3, r3, #24
	cmp r3, r2
	bls .L_080cd460
	movs r1, #192
	lsls r1, r1, #18
	cmp r3, r1
	bne .L_080cd482
.L_080cd460:
	ldr r5, .L_080cd49c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	bl Object_SetPositionAndResetMotionFar
	ldr r1, [r5]
	mov r0, r11
	movs r2, #0
	bl Object_LinkPair
.L_080cd482:
	ldr r3, [r6, #8]
.L_080cd484:
	movs r1, #128
	lsls r1, r1, #9
	cmp r3, r1
	bge .L_080cd4c8
	ldr r0, [sp, #12]
	bl UiWork_SetBusyFlagsFar
	ldr r1, [r6, #8]
	mov r0, r11
	bl Func_080cdea8
	b .L_080cd4d4
.L_080cd49c:
	.4byte gPartyState
.L_080cd4a0:
	.4byte 0x00000e1a
.L_080cd4a4:
	.4byte 0x00000de5
.L_080cd4a8:
	.4byte 0x00000e1c
.L_080cd4ac:
	.4byte BattleFx_EmitRandomParticleFromEmitter
.L_080cd4b0:
	.4byte 0x00000e10
.L_080cd4b4:
	.4byte 0x00000e0f
.L_080cd4b8:
	.4byte 0x00000e11
.L_080cd4bc:
	.4byte 0x00000e12
.L_080cd4c0:
	.4byte 0x00000def
.L_080cd4c4:
	.4byte 0x0000137f
.L_080cd4c8:
	bl Func_080d2260
	ldr r3, [r6, #8]
	mov r0, r11
	mov lr, r3
	.2byte 0xf800
.L_080cd4d4:
	ldr r2, [sp, #16]
	cmp r2, #0
	bne .L_080cd542
	ldr r2, [sp, #24]
	movs r1, #4
	ldrsh r3, [r2, r1]
	ldr r2, [r2]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	cmp r3, #17
	bne .L_080cd532
	ldr r3, [sp, #32]
	movs r0, #22
	ldrsb r0, [r3, r0]
	cmp r0, #3
	bne .L_080cd51c
	ldr r3, .L_080cd578
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r2, [sp, #24]
	movs r3, #1
	str r0, [r2, #104]
	adds r1, r2, #0
	adds r1, #90
	ldrb r2, [r1]
	orrs r3, r2
	strb r3, [r1]
	ldr r0, [sp, #24]
	ldr r1, .L_080cd57c
	bl Object_SetActionCallback
	b .L_080cd532
.L_080cd51c:
	cmp r0, #1
	bne .L_080cd532
	ldr r3, [sp, #24]
	add r1, sp, #20
	ldrh r1, [r1]
	adds r3, #100
	strh r1, [r3]
	ldr r0, [sp, #24]
	ldr r1, .L_080cd580
	bl Object_SetCallback
.L_080cd532:
	ldr r2, [sp, #24]
	movs r3, #0
	adds r2, #91
	strb r3, [r2]
	ldr r0, [sp, #24]
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
.L_080cd542:
	movs r2, #0
	str r2, [sp, #28]
.L_080cd546:
	ldr r3, [sp, #16]
	cmp r3, #0
	beq .L_080cd566
	ldr r5, .L_080cd578
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #106
	adds r5, r5, r1
	movs r2, #0
	ldrsh r0, [r5, r2]
	bl Func_080e035c
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r5]
.L_080cd566:
	ldr r0, [sp, #28]
.L_080cd568:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080cd578:
	.4byte gPartyState
.L_080cd57c:
	.4byte Object_LinkedMotionScript
.L_080cd580:
	.4byte ObjectMotion_StepAngleScript
