.syntax unified
	.thumb
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {r5, lr}
	movs r1, #128
	lsls r1, r1, #7
	adds r1, #132
	adds r5, r0, #0
	movs r0, #8
	bl Func_02002a34
	bl Func_02002a44
	movs r0, #5
	bl Object_GetById
	movs r1, #2
	bl Func_02002a5c
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl Func_020023a4
	movs r0, #5
	bl Object_GetById
	movs r1, #0
	bl Func_02002a5c
	bl Func_02002a54
	bl Func_02002a4c
	movs r0, #8
	bl Field_BeginPaletteTransition
	pop {r5, pc}
	.section .text.x0200809c,"ax",%progbits
	.global Func_0200009c
	.thumb_func
Func_0200009c:
	push {r5, lr}
	adds r5, r0, #0
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	ldr r1, [r5, #8]
	adds r5, #100
	bl Func_0200260c
	ldrh r2, [r5]
	movs r3, #224
	adds r2, #1
	strh r2, [r5]
	lsls r3, r3, #11
	lsls r2, r2, #16
	ands r3, r2
	cmp r3, #0
	bne .L_020080c4
	movs r0, #125
	bl Func_02002a64
.L_020080c4:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020080c8,"ax",%progbits
	.global Func_020000c8
	.thumb_func
Func_020000c8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	bl Object_GetById
	adds r6, r0, #0
	bl Func_020026f4
	ldr r3, .L_0200811c
	adds r7, r6, #0
	str r3, [r6, #108]
	movs r3, #0
	mov r8, r3
	mov r3, r8
	adds r7, #35
	movs r2, #96
	strb r3, [r7]
	adds r0, r5, #0
	movs r1, #96
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #96
	adds r0, r5, #0
	negs r1, r1
	movs r2, #96
	bl ObjectMotion_CommitPositionAndActivate
	mov r3, r8
	str r3, [r6, #108]
	movs r3, #1
	strb r3, [r7]
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_020027b0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200811c:
	.4byte Func_0200009c
	.section .text.x02008128,"ax",%progbits
	.global Func_02000128
	.thumb_func
Func_02000128:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r5, .L_02008300
	subs r2, #172
	str r2, [r3]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #5
	movs r0, #10
	bl Object_SetModeById
	movs r0, #10
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	movs r1, #5
	bl Object_SetModeById
	movs r0, #11
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #19
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #146
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081a4
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
.L_020081a4:
	movs r0, #137
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008214
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
.L_02008214:
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	bne .L_02008242
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #145
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008242
	bl Func_020028b4
	movs r0, #0
	bl Func_020029cc
	bl Func_0200030c
	bl Func_020028bc
.L_02008242:
	ldr r5, .L_02008300
	movs r3, #241
	lsls r3, r3, #1
	adds r6, r5, r3
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #98
	bne .L_02008268
	movs r3, #245
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
	movs r0, #1
	strh r3, [r6]
	bl Func_020028a4
	bl Func_02001c30
.L_02008268:
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #99
	bne .L_0200827a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #147
	bl GameFlag_SetBit
.L_0200827a:
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #90
	bne .L_020082a6
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	movs r2, #5
	str r2, [r3]
	movs r0, #4
	bl Party_RemoveActiveOwner
	movs r0, #6
	bl Party_RemoveActiveOwner
	movs r0, #7
	bl Party_RemoveActiveOwner
	ldr r0, .L_02008304
	movs r1, #1
	bl Func_0200299c
.L_020082a6:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #147
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020082c6
	movs r0, #22
	movs r1, #5
	bl Object_SetModeById
	movs r0, #22
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	b .L_020082fc
.L_020082c6:
	movs r1, #160
	movs r2, #248
	lsls r1, r1, #14
	lsls r2, r2, #16
	movs r0, #22
	bl Func_0200290c
	movs r0, #22
	bl Object_GetById
	movs r1, #128
	lsls r1, r1, #7
	strh r1, [r0, #6]
	movs r2, #0
	movs r0, #22
	bl Func_02002a04
	movs r1, #3
	movs r0, #22
	bl ObjectMotion_SetActionVariant
	movs r0, #22
	bl Object_GetById
	movs r1, #8
	bl Object_SetActionCallback
.L_020082fc:
	movs r0, #0
	pop {r5, r6, pc}
.L_02008300:
	.4byte gPartyState
.L_02008304:
	.4byte 0x00000004
	.section .text.x0200830c,"ax",%progbits
	.global Func_0200030c
	.thumb_func
Func_0200030c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #145
	bl GameFlag_SetBit
	movs r0, #10
	bl Object_GetById
	movs r2, #0
	adds r3, r0, #0
	mov r8, r2
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	movs r3, #200
	lsls r3, r3, #1
	adds r3, #255
	movs r6, #192
	movs r5, #128
	str r3, [r0, #72]
	lsls r6, r6, #9
	lsls r5, r5, #8
	movs r1, #216
	movs r2, #192
	movs r3, #176
	str r6, [r0, #48]
	str r5, [r0, #52]
	lsls r1, r1, #16
	lsls r2, r2, #15
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #11
	bl Object_GetById
	adds r3, r0, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	movs r3, #224
	lsls r3, r3, #3
	adds r3, #174
	str r3, [r0, #72]
	movs r1, #154
	movs r2, #192
	movs r3, #152
	str r6, [r0, #48]
	str r5, [r0, #52]
	lsls r2, r2, #15
	lsls r3, r3, #16
	lsls r1, r1, #18
	bl Object_SetPositionAndResetMotion
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_0200297c
	movs r0, #188
	movs r1, #1
	movs r2, #248
	lsls r2, r2, #16
	movs r3, #0
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #10
	bl Object_GetById
	movs r1, #4
	adds r5, r0, #0
	movs r0, #10
	bl Object_SetModeById
	movs r0, #165
	bl Func_02002a64
	movs r1, #176
	movs r2, #128
	movs r3, #176
	lsls r2, r2, #12
	lsls r3, r3, #16
	adds r0, r5, #0
	lsls r1, r1, #17
	bl Func_02002884
	ldr r6, .L_0200843c
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_02002864
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #188
	movs r1, #1
	movs r2, #208
	lsls r2, r2, #15
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #11
	bl Object_GetById
	movs r1, #4
	adds r5, r0, #0
	movs r0, #11
	bl Object_SetModeById
	movs r1, #208
	movs r2, #128
	movs r3, #152
	lsls r2, r2, #12
	lsls r3, r3, #16
	adds r0, r5, #0
	lsls r1, r1, #17
	bl Func_02002884
	adds r1, r6, #0
	adds r0, r5, #0
	bl Func_02002864
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #165
	bl Func_02002a64
	movs r0, #10
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_0200298c
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200843c:
	.4byte Data_0200b000
	.section .text.x02008440,"ax",%progbits
	.global Func_02000440
	.thumb_func
Func_02000440:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #146
	bl GameFlag_SetBit
	bl Func_020028b4
	movs r0, #0
	bl Func_020029cc
	movs r0, #165
	bl Func_02002a64
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #11
	bl Object_SetModeById
	movs r0, #21
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	movs r2, #0
	adds r5, r0, #0
	mov r9, r2
	adds r3, r5, #0
	mov r2, r9
	adds r3, #85
	strb r2, [r3]
	movs r6, #128
	movs r3, #128
	lsls r3, r3, #10
	lsls r6, r6, #8
	str r3, [r5, #48]
	str r6, [r5, #52]
	movs r0, #10
	movs r1, #4
	mov r10, r3
	bl Object_SetModeById
	movs r1, #154
	movs r2, #192
	movs r3, #176
	lsls r3, r3, #16
	adds r0, r5, #0
	lsls r1, r1, #18
	lsls r2, r2, #15
	bl Func_02002884
	ldr r2, .L_0200850c
	adds r0, r5, #0
	mov r8, r2
	mov r1, r8
	bl Func_02002864
	movs r0, #11
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	mov r2, r9
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	str r3, [r5, #48]
	str r6, [r5, #52]
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r1, #240
	movs r2, #192
	movs r3, #176
	adds r0, r5, #0
	lsls r2, r2, #15
	lsls r3, r3, #16
	lsls r1, r1, #15
	bl Func_02002884
	adds r0, r5, #0
	mov r1, r8
	bl Func_02002864
	bl Func_020028bc
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200850c:
	.4byte Data_02003040
	.section .text.x02008510,"ax",%progbits
	.global Func_02000510
	.thumb_func
Func_02000510:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #137
	lsls r0, r0, #4
	bl GameFlag_SetBit
	bl Func_020028b4
	movs r0, #0
	bl Func_020029cc
	ldr r0, .L_0200892c
	bl Func_0200293c
	movs r1, #128
	movs r2, #128
	movs r0, #12
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_0200294c
	movs r1, #188
	movs r2, #140
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #5
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #5
	bl Func_02002964
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #188
	movs r2, #180
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #5
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #188
	movs r1, #1
	movs r2, #212
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_0200298c
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #12
	bl Func_02002964
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #17
	bl Func_02002964
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #18
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #18
	movs r1, #6
	movs r2, #25
	bl ObjectMotion_Launch
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #19
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #19
	movs r1, #0
	bl Func_0200294c
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #20
	movs r1, #0
	bl Func_0200294c
	movs r1, #3
	movs r0, #19
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #15
	movs r1, #0
	bl Func_0200294c
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #16
	movs r1, #64
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #16
	bl Func_0200290c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #20
	movs r1, #0
	bl Func_0200294c
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #64
	movs r0, #21
	negs r1, r1
	movs r2, #64
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #21
	bl Func_0200290c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #188
	movs r1, #1
	movs r2, #180
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_0200298c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #16
	movs r2, #16
	movs r3, #128
	movs r0, #8
	negs r1, r1
	negs r2, r2
	lsls r3, r3, #7
	bl Func_020029d4
	movs r2, #16
	movs r3, #128
	lsls r3, r3, #7
	movs r1, #16
	negs r2, r2
	movs r0, #9
	bl Func_020029d4
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #16
	movs r1, #0
	movs r0, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_02002964
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200294c
	movs r1, #0
	movs r2, #16
	movs r0, #9
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #60
	b .L_02008930
.L_0200892c:
	.4byte 0x000015b7
.L_02008930:
	movs r0, #5
	bl Func_02002964
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02002964
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008a7c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_0200294c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008ac2
.L_02008a7c:
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #8
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_02008ac2:
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #156
	movs r1, #1
	movs r2, #212
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_0200298c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #18
	bl Func_02002964
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #188
	movs r1, #1
	movs r2, #212
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_0200298c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #188
	movs r1, #1
	movs r2, #180
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_0200298c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #5
	bl Func_02002964
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #188
	movs r1, #1
	movs r2, #204
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_0200298c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #240
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #240
	movs r0, #18
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #240
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #240
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #12
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_0200294c
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #144
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_02009220
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #13
	ldr r1, .L_02009220
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #26
	movs r0, #12
	negs r1, r1
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #16
	movs r2, #12
	movs r0, #13
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	movs r1, #0
	adds r0, #9
	bl Func_0200294c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	movs r0, #9
	bl Object_SetModeById
	movs r0, #50
	bl Battle_WaitMode0
	bl Func_02002a44
	movs r0, #9
	bl Object_GetById
	movs r1, #2
	bl Func_02002a5c
	movs r0, #178
	bl Func_02002a64
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	adds r1, #132
	movs r0, #8
	bl Func_02002a34
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02002a64
	movs r1, #0
	movs r0, #12
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	bl Func_020023a4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #17
	bl Func_02002964
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #18
	bl Func_02002964
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #19
	bl Func_02002964
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #20
	bl Func_02002964
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #13
	bl Func_02002964
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #14
	bl Func_02002964
	movs r1, #128
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #15
	bl Func_02002964
	movs r1, #1
	movs r0, #13
	bl ObjectMotion_SetActionVariant
	movs r0, #13
	bl Func_020023a4
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl Func_02002a5c
	bl Func_02002a54
	bl Func_02002a4c
	movs r0, #16
	bl Field_BeginPaletteTransition
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #9
	movs r0, #9
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #14
	bl Func_02002964
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #15
	movs r1, #0
	bl Func_0200294c
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #14
	lsls r1, r1, #1
	bl Func_0200296c
	movs r0, #15
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200296c
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #14
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #14
	movs r1, #6
	movs r2, #25
	bl ObjectMotion_Launch
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02002964
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #16
	movs r0, #9
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #180
	movs r1, #1
	movs r2, #204
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	bl Func_0200298c
	movs r0, #17
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #17
	lsls r1, r1, #1
	bl Func_0200296c
	movs r0, #18
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #18
	lsls r1, r1, #1
	bl Func_0200296c
	movs r0, #19
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #19
	lsls r1, r1, #1
	bl Func_0200296c
	movs r0, #20
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #20
	lsls r1, r1, #1
	bl Func_0200296c
	movs r0, #21
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl Func_0200296c
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #18
	bl Func_02002964
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_0200294c
	movs r0, #19
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #19
	movs r1, #0
	bl Func_0200294c
	movs r0, #20
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #20
	bl Func_0200296c
	movs r0, #55
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #18
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #19
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #20
	movs r1, #6
	movs r2, #25
	bl ObjectMotion_Launch
	bl Func_020026f4
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #20
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #19
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #18
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	adds r2, #102
	adds r1, #204
	movs r0, #17
	bl ObjectMotion_SetSpeedParameters
	movs r0, #20
	bl Object_GetById
	ldr r6, .L_02009224
	adds r7, r0, #0
	adds r3, r7, #0
	movs r5, #0
	adds r3, #100
	strh r5, [r3]
	movs r0, #19
	str r6, [r7, #108]
	bl Object_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #100
	strh r5, [r3]
	movs r0, #18
	str r6, [r7, #108]
	bl Object_GetById
	adds r7, r0, #0
	b .L_02009228
.L_02009220:
	.4byte 0x00013333
.L_02009224:
	.4byte Func_0200009c
.L_02009228:
	adds r3, r7, #0
	adds r3, #100
	strh r5, [r3]
	movs r0, #20
	str r6, [r7, #108]
	movs r1, #10
	bl Object_SetModeById
	movs r0, #19
	movs r1, #10
	bl Object_SetModeById
	movs r0, #18
	movs r1, #10
	bl Object_SetModeById
	movs r1, #24
	movs r0, #20
	negs r1, r1
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #24
	movs r0, #19
	negs r1, r1
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #24
	negs r1, r1
	movs r2, #24
	movs r0, #18
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #18
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #240
	movs r0, #20
	movs r1, #224
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPosition
	movs r2, #240
	movs r0, #19
	movs r1, #224
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPosition
	movs r2, #240
	movs r1, #224
	lsls r2, r2, #1
	movs r0, #18
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #20
	bl Object_GetById
	adds r7, r0, #0
	str r5, [r7, #108]
	movs r0, #19
	bl Object_GetById
	adds r7, r0, #0
	str r5, [r7, #108]
	movs r0, #18
	bl Object_GetById
	adds r7, r0, #0
	str r5, [r7, #108]
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r1, #6
	movs r2, #25
	movs r0, #17
	bl ObjectMotion_Launch
	movs r0, #17
	bl Object_GetById
	movs r2, #100
	adds r7, r0, #0
	adds r2, r2, r7
	mov r10, r2
	mov r3, r10
	strh r5, [r3]
	movs r0, #17
	str r6, [r7, #108]
	movs r1, #10
	bl Object_SetModeById
	movs r1, #20
	negs r1, r1
	movs r2, #20
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #17
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #20
	movs r2, #20
	negs r1, r1
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #1
	movs r0, #17
	bl Object_SetModeById
	movs r0, #133
	bl Func_02002a64
	ldr r3, [r7, #8]
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r2, .L_0200972c
	ldr r3, [r7, #16]
	str r5, [r7, #108]
	adds r3, r3, r2
	str r3, [r7, #16]
	movs r3, #192
	lsls r3, r3, #8
	mov r8, r3
	mov r2, r8
	strh r2, [r7, #6]
	movs r2, #160
	ldr r3, [r7, #80]
	lsls r2, r2, #8
	mov r11, r2
	mov r2, r11
	strh r2, [r3, #18]
	movs r0, #55
	bl Battle_WaitMode0
	ldr r3, [r7, #8]
	ldr r2, .L_02009730
	movs r0, #17
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r3, [r7, #16]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r7, #16]
	movs r3, #128
	lsls r3, r3, #7
	mov r9, r3
	ldr r3, [r7, #80]
	mov r2, r9
	strh r5, [r3, #18]
	strh r2, [r7, #6]
	movs r1, #6
	movs r2, #40
	bl ObjectMotion_Launch
	movs r1, #11
	movs r0, #17
	bl Object_SetModeById
	movs r0, #135
	bl Func_02002a64
	movs r0, #14
	bl Battle_WaitMode0
	movs r0, #135
	bl Func_02002a64
	movs r0, #14
	bl Battle_WaitMode0
	movs r0, #135
	bl Func_02002a64
	movs r0, #14
	bl Battle_WaitMode0
	movs r0, #135
	bl Func_02002a64
	movs r0, #14
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	mov r3, r10
	strh r5, [r3]
	movs r0, #17
	str r6, [r7, #108]
	movs r1, #10
	bl Object_SetModeById
	movs r2, #236
	movs r1, #224
	lsls r2, r2, #1
	movs r0, #17
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #30
	bl Battle_WaitMode0
	str r5, [r7, #108]
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r2, #0
	movs r1, #0
	movs r0, #17
	bl Func_0200290c
	bl Func_020027b0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #188
	movs r1, #1
	movs r2, #204
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_0200298c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	mov r1, r9
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	mov r1, r11
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	mov r2, r8
	movs r0, #14
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #32
	negs r1, r1
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #9
	bl Object_GetById
	movs r6, #128
	lsls r6, r6, #6
	strh r6, [r0, #6]
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #192
	mov r2, r8
	movs r0, #9
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #14
	bl ObjectMotion_SetSpeedParameters
	movs r0, #14
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #228
	movs r2, #212
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #14
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r8, r2
	mov r2, r8
	orrs r3, r2
	movs r1, #192
	strb r3, [r0]
	lsls r1, r1, #7
	movs r0, #14
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	lsls r1, r1, #9
	movs r0, #14
	bl ObjectMotion_SetSpeedParameters
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #48
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #5
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r6, #0
	movs r0, #5
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r6, #0
	movs r0, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #15
	bl ObjectMotion_SetSpeedParameters
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #228
	ands r5, r3
	movs r2, #220
	lsls r1, r1, #1
	lsls r2, r2, #1
	strb r5, [r0]
	movs r0, #15
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02002964
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #16
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_02002964
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r0, #14
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #15
	bl ObjectMotion_SetSpeedParameters
	movs r0, #14
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r6, #254
	adds r3, r6, #0
	ands r3, r2
	movs r1, #244
	movs r2, #212
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #14
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	movs r1, #244
	movs r2, #220
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #15
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	movs r1, #228
	movs r2, #216
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #9
	lsls r2, r2, #1
	b .L_02009734
	.2byte 0x0000
.L_0200972c:
	.4byte 0xfff80000
.L_02009730:
	.4byte 0xfffc0000
.L_02009734:
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #14
	lsls r1, r1, #1
	bl Func_0200296c
	movs r0, #15
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200296c
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #14
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	movs r1, #252
	movs r2, #212
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #14
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r2, #0
	movs r0, #14
	bl Func_0200290c
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #252
	ands r6, r3
	movs r2, #220
	lsls r1, r1, #1
	lsls r2, r2, #1
	strb r6, [r0]
	movs r0, #15
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r2, r3
	strb r2, [r0]
	movs r1, #0
	movs r0, #15
	mov r8, r2
	movs r2, #0
	bl Func_0200290c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #64
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl Func_0200290c
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #5
	bl Func_02002994
	bl Func_0200298c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02002964
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_0200294c
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #10
	movs r0, #8
	bl Func_0200294c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #6
	adds r1, #255
	movs r2, #50
	movs r0, #5
	bl Func_02002964
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02002964
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #10
	adds r1, #255
	movs r2, #80
	movs r0, #5
	bl Func_02002964
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #8
	bl Func_02002964
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #5
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200994c
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_0200294c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200996e
.L_0200994c:
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #8
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
.L_0200996e:
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_02009a28
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #2
	bl Object_SetModeById
	movs r0, #5
	bl Object_GetById
	cmp r0, #0
	beq .L_02009a00
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #8
	bl ObjectMotion_ResetAndSetPosition
.L_02009a00:
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl Func_0200290c
	movs r0, #5
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_020028bc
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009a28:
	.4byte 0x00013333
	.section .text.x02009a2c,"ax",%progbits
	.global Func_02001a2c
	.thumb_func
Func_02001a2c:
	push {r5, lr}
	bl Func_020028b4
	movs r0, #0
	bl Func_020029cc
	ldr r0, .L_02009ba4
	bl Func_0200293c
	movs r2, #133
	movs r0, #5
	movs r1, #72
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #22
	movs r1, #0
	bl Func_0200294c
	movs r1, #1
	movs r0, #22
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #22
	bl Func_02002964
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #22
	movs r1, #6
	movs r2, #35
	bl ObjectMotion_Launch
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #12
	bl Object_SetModeById
	movs r2, #0
	movs r0, #22
	movs r1, #0
	bl Func_0200294c
	movs r0, #22
	movs r1, #1
	bl Object_SetModeById
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #22
	bl Func_02002964
	movs r1, #0
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #22
	movs r1, #0
	bl Func_0200294c
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #22
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #16
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	movs r2, #128
	movs r0, #22
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #22
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #22
	movs r1, #6
	movs r2, #25
	bl ObjectMotion_Launch
	movs r0, #22
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #0
	movs r1, #26
	movs r0, #22
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #8
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #1
	bl Object_SetModeById
	ldr r5, .L_02009ba8
	movs r1, #99
	adds r0, r5, #0
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #98
	bl Party_SetFields1f2And1f4
	ldr r3, .L_02009bac
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r0, #9
	movs r1, #0
	bl Func_020029a4
	bl Func_020028bc
	pop {r5, pc}
.L_02009ba4:
	.4byte 0x00001604
.L_02009ba8:
	.4byte 0x00000004
.L_02009bac:
	.4byte gPartyState
	.section .text.x02009bb0,"ax",%progbits
	.global Func_02001bb0
	.thumb_func
Func_02001bb0:
	push {lr}
	bl Func_020028b4
	movs r0, #0
	bl Func_020029cc
	ldr r0, .L_02009c2c
	bl Func_0200293c
	movs r3, #128
	lsls r3, r3, #8
	movs r1, #16
	movs r2, #0
	movs r0, #8
	bl Func_020029d4
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #8
	movs r1, #2
	bl Object_SetModeById
	movs r0, #5
	bl Object_GetById
	cmp r0, #0
	beq .L_02009c04
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #8
	bl ObjectMotion_ResetAndSetPosition
.L_02009c04:
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl Func_0200290c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #16
	movs r0, #5
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	bl Func_020028bc
	pop {pc}
.L_02009c2c:
	.4byte 0x00001615
	.section .text.x02009c30,"ax",%progbits
	.global Func_02001c30
	.thumb_func
Func_02001c30:
	push {r5, lr}
	bl Func_020028b4
	movs r0, #0
	bl Func_020029cc
	movs r1, #182
	movs r2, #156
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_0200290c
	movs r1, #181
	movs r2, #142
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_0200290c
	movs r1, #195
	movs r2, #144
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_0200290c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #5
	bl Func_020029fc
	movs r0, #5
	bl Object_GetById
	movs r5, #192
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	lsls r5, r5, #18
	movs r1, #0
	movs r0, #5
	bl Func_02002994
	ldr r3, [r5, #108]
	movs r2, #218
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #60
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_02009fc8
	bl Func_0200293c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #8
	bl Func_02002964
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #188
	movs r2, #164
	lsls r2, r2, #16
	movs r0, #5
	lsls r1, r1, #17
	bl Func_0200290c
	movs r1, #0
	movs r0, #5
	bl Func_020029fc
	movs r0, #5
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #50
	movs r0, #5
	movs r1, #6
	bl ObjectMotion_Launch
	movs r1, #4
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #14
	movs r2, #2
	movs r0, #5
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #10
	movs r0, #9
	bl Func_0200294c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #240
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_0200294c
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #9
	bl Object_LinkObjectAndSetCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #0
	movs r2, #120
	movs r0, #9
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #24
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl Func_0200290c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02002964
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_0200294c
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r2, #0
	movs r1, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_02009fcc
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #2
	bl Object_SetModeById
	movs r0, #5
	bl Object_GetById
	cmp r0, #0
	beq .L_02009fa4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #8
	bl ObjectMotion_ResetAndSetPosition
.L_02009fa4:
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	ldr r3, [r5, #108]
	movs r2, #218
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #8
	str r2, [r3]
	bl Func_020028bc
	pop {r5, pc}
	.2byte 0x0000
.L_02009fc8:
	.4byte 0x000015f9
.L_02009fcc:
	.4byte 0x00013333
	.section .text.x02009fd0,"ax",%progbits
	.global Func_02001fd0
	.thumb_func
Func_02001fd0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	movs r0, #132
	lsls r0, r0, #4
	adds r3, r6, r0
	ldr r3, [r3]
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #68
	mov r10, r3
	adds r0, #20
	adds r3, r6, r2
	ldr r1, [r3]
	adds r3, r6, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #4
	cmp r3, #7
	bls .L_0200a004
	b .L_0200a1fc
.L_0200a004:
	ldr r2, .L_0200a194
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200a00c:
	.4byte .L_0200a02c
	.4byte .L_0200a040
	.4byte .L_0200a076
	.4byte .L_0200a08c
	.4byte .L_0200a0e4
	.4byte .L_0200a11e
	.4byte .L_0200a152
	.4byte .L_0200a19c
.L_0200a02c:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #86
	adds r1, r6, r3
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	beq .L_0200a03e
	b .L_0200a1fc
.L_0200a03e:
	b .L_0200a106
.L_0200a040:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #92
	adds r2, r6, r3
	ldrh r3, [r2]
	movs r0, #248
	adds r3, #4
	strh r3, [r2]
	lsls r0, r0, #13
	lsls r3, r3, #16
	cmp r3, r0
	bgt .L_0200a05a
	b .L_0200a1fc
.L_0200a05a:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #84
	adds r2, r6, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	strh r3, [r2]
	lsls r0, r0, #4
	movs r3, #255
	adds r0, #86
	lsls r3, r3, #8
	adds r2, r6, r0
	b .L_0200a14c
.L_0200a076:
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #86
	adds r1, r6, r2
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #90
	beq .L_0200a088
	b .L_0200a1fc
.L_0200a088:
	subs r2, #2
	b .L_0200a10c
.L_0200a08c:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #86
	adds r5, r6, r3
	movs r0, #0
	ldrsh r1, [r5, r0]
	cmp r1, #0
	bne .L_0200a0bc
	mov r2, r10
	adds r2, #85
	movs r3, #3
	movs r0, #128
	strb r3, [r2]
	lsls r0, r0, #4
	movs r3, #192
	lsls r3, r3, #13
	adds r0, #94
	mov r2, r10
	str r3, [r2, #40]
	adds r3, r6, r0
	strb r1, [r3]
	movs r0, #134
	bl Func_02002a64
.L_0200a0bc:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #92
	adds r2, r6, r3
	ldrh r3, [r2]
	movs r0, #158
	adds r3, #8
	strh r3, [r2]
	lsls r0, r0, #15
	lsls r3, r3, #16
	cmp r3, r0
	bgt .L_0200a0d6
	b .L_0200a1fc
.L_0200a0d6:
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #84
	adds r3, r6, r2
	ldrh r2, [r3]
	adds r2, #1
	b .L_0200a1f2
.L_0200a0e4:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #86
	adds r1, r6, r3
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #10
	bne .L_0200a0fc
	movs r3, #0
	mov r2, r10
	str r3, [r2, #16]
	str r3, [r2, #8]
.L_0200a0fc:
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #30
	beq .L_0200a106
	b .L_0200a1fc
.L_0200a106:
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #84
.L_0200a10c:
	adds r3, r6, r2
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r1]
	b .L_0200a1fc
.L_0200a11e:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #92
	adds r2, r6, r3
	ldrh r3, [r2]
	subs r3, #8
	strh r3, [r2]
	lsls r3, r3, #16
	cmp r3, #0
	bgt .L_0200a1fc
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #84
	adds r2, r6, r0
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #86
	adds r2, r6, r3
	movs r3, #255
	lsls r3, r3, #8
.L_0200a14c:
	adds r3, #255
	strh r3, [r2]
	b .L_0200a1fc
.L_0200a152:
	ldr r3, [r1, #24]
	cmp r3, #0
	ble .L_0200a15e
	ldr r0, .L_0200a198
	adds r3, r3, r0
	b .L_0200a160
.L_0200a15e:
	movs r3, #0
.L_0200a160:
	str r3, [r1, #24]
	str r3, [r1, #28]
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #86
	adds r5, r6, r2
	movs r0, #0
	ldrsh r3, [r5, r0]
	cmp r3, #10
	bne .L_0200a17c
	movs r0, #155
	lsls r0, r0, #1
	bl Func_02002a64
.L_0200a17c:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #90
	bne .L_0200a1fc
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #84
	adds r3, r6, r0
	ldrh r2, [r3]
	adds r2, #1
	b .L_0200a1f2
	.2byte 0x0000
.L_0200a194:
	.4byte .L_0200a00c
.L_0200a198:
	.4byte 0xfffffae2
.L_0200a19c:
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #86
	adds r5, r6, r2
	movs r0, #0
	ldrsh r3, [r5, r0]
	cmp r3, #0
	bne .L_0200a1c2
	movs r0, #144
	bl Func_02002a64
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02002894
.L_0200a1c2:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #30
	bne .L_0200a1fc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02002894
	movs r0, #0
	ldrsh r3, [r5, r0]
	cmp r3, #30
	bne .L_0200a1fc
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #84
	adds r3, r6, r2
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
.L_0200a1f2:
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r5]
.L_0200a1fc:
	mov r0, r10
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #92
	strh r3, [r0, #6]
	adds r3, r6, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	bgt .L_0200a21a
	b .L_0200a38c
.L_0200a21a:
	movs r2, #0
	mov r8, r2
	subs r3, #8
	movs r4, #0
	cmp r8, r3
	bge .L_0200a2c2
	movs r3, #128
	movs r2, #208
	lsls r3, r3, #3
	lsls r2, r2, #3
	adds r0, r6, r3
	adds r1, r6, r2
.L_0200a232:
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #72
	adds r3, r6, r2
	ldr r3, [r3]
	adds r5, r1, #0
	str r3, [r5]
	adds r2, #4
	adds r3, r6, r2
	ldr r3, [r3]
	lsls r2, r4, #16
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #4]
	movs r2, #133
	lsls r2, r2, #4
	adds r3, r6, r2
	ldr r3, [r3]
	adds r7, r0, #0
	str r3, [r5, #8]
	ldr r3, [r5, #24]
	movs r0, #128
	adds r3, #1
	str r3, [r5, #24]
	lsls r0, r0, #4
	adds r0, #88
	adds r1, r6, r0
	ldrh r1, [r1]
	movs r2, #3
	asrs r3, r3, #2
	ands r3, r2
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, .L_0200a2a8
	ldr r2, .L_0200a2ac
	ands r1, r3
	ldrh r3, [r7, #8]
	adds r0, r7, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r7, #8]
	adds r1, r5, #0
	str r4, [sp, #0]
	bl Func_02002a14
	movs r2, #1
	ldr r4, [sp, #0]
	add r8, r2
	adds r0, r7, #0
	adds r1, r5, #0
	mov r3, r8
	adds r0, #40
	adds r1, #28
	adds r4, #16
	cmp r3, #15
	bgt .L_0200a38c
	b .L_0200a2b0
.L_0200a2a8:
	.4byte 0x000003ff
.L_0200a2ac:
	.4byte 0xfffffc00
.L_0200a2b0:
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #92
	adds r3, r6, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #8
	cmp r4, r3
	blt .L_0200a232
.L_0200a2c2:
	mov r3, r8
	cmp r3, #15
	bgt .L_0200a38c
	mov r0, r8
	lsls r3, r3, #3
	subs r3, r3, r0
	movs r0, #128
	lsls r3, r3, #2
	movs r2, #208
	lsls r0, r0, #4
	adds r3, r6, r3
	lsls r2, r2, #3
	adds r0, #72
	adds r5, r3, r2
	adds r3, r6, r0
	ldr r3, [r3]
	movs r2, #128
	str r3, [r5]
	adds r0, #20
	lsls r2, r2, #4
	adds r3, r6, r0
	adds r2, #76
	adds r1, r6, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [r1]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5, #4]
	movs r2, #133
	lsls r2, r2, #4
	adds r3, r6, r2
	ldr r0, [r5, #24]
	ldr r3, [r3]
	adds r0, #1
	str r3, [r5, #8]
	str r0, [r5, #24]
	lsls r0, r0, #12
	bl Math_Sine
	ldr r3, .L_0200a380
	adds r1, r0, #0
	ldr r0, .L_0200a384
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r5, #4]
	mov r2, r8
	adds r3, r3, r0
	ldr r0, .L_0200a384
	adds r3, r3, r0
	str r3, [r5, #4]
	lsls r3, r2, #2
	add r3, r8
	lsls r3, r3, #3
	movs r0, #128
	adds r3, r6, r3
	lsls r0, r0, #3
	adds r7, r3, r0
	movs r2, #128
	ldr r3, [r5, #24]
	lsls r2, r2, #4
	adds r2, #90
	adds r1, r6, r2
	asrs r3, r3, #2
	movs r2, #3
	ands r3, r2
	ldrh r2, [r1]
	lsls r3, r3, #3
	adds r2, r2, r3
	ldr r3, .L_0200a37c
	ldrh r1, [r7, #8]
	ands r2, r3
	ldr r3, .L_0200a388
	adds r0, r7, #0
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #8]
	adds r1, r5, #0
	bl Func_02002a14
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #94
	adds r3, r6, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_0200a38c
	ldr r3, [r5, #4]
	mov r2, r10
	str r3, [r2, #12]
	b .L_0200a38c
.L_0200a37c:
	.4byte 0x000003ff
.L_0200a380:
	.4byte IwramMulQ16
.L_0200a384:
	.4byte 0xfffe0000
.L_0200a388:
	.4byte 0xfffffc00
.L_0200a38c:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #86
	adds r2, r6, r3
	ldrh r3, [r2]
	add sp, #4
	adds r3, #1
	strh r3, [r2]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.section .text.x0200a3a4,"ax",%progbits
	.global Func_020023a4
	.thumb_func
Func_020023a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #134
	adds r5, r0, #0
	lsls r1, r1, #4
	movs r0, #220
	sub sp, #4
	bl Runtime_AllocateHeapBlockFar
	adds r7, r0, #0
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r1, #132
	movs r3, #0
	adds r2, #91
	lsls r1, r1, #4
	strb r3, [r2]
	adds r3, r7, r1
	str r6, [r3]
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #72
	adds r2, r7, r3
	ldr r3, [r6, #8]
	adds r1, #12
	str r3, [r2]
	adds r2, r7, r1
	ldr r3, [r6, #12]
	movs r1, #128
	str r3, [r2]
	movs r3, #133
	lsls r3, r3, #4
	adds r2, r7, r3
	ldr r3, [r6, #16]
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r2]
	adds r1, r7, #0
	ldr r0, .L_0200a520
	bl Func_02002824
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #4
	adds r2, r7, #0
	mov r11, r0
	bl VramBlock_LoadCached
	movs r2, #128
	lsls r2, r2, #4
	mov r10, r0
	adds r2, #90
	adds r3, r7, r2
	mov r1, r10
	strh r1, [r3]
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #88
	mov r2, r10
	adds r3, r7, r1
	adds r2, #32
	strh r2, [r3]
	movs r2, #208
	lsls r2, r2, #3
	movs r3, #128
	adds r2, r2, r7
	lsls r3, r3, #3
	movs r1, #15
	mov r8, r2
	adds r5, r7, r3
	mov r9, r1
.L_0200a444:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	adds r0, r5, #0
	bl Func_02002a0c
	adds r0, r6, #0
	bl Func_02002a2c
	subs r0, #2
	strh r0, [r5, #30]
	adds r0, r6, #0
	bl Func_02002a24
	ldrb r2, [r5, #9]
	movs r1, #13
	movs r3, #3
	negs r1, r1
	ands r0, r3
	adds r3, r1, #0
	ands r2, r3
	ldrb r3, [r5, #5]
	movs r1, #32
	orrs r3, r1
	lsls r0, r0, #2
	strb r3, [r5, #5]
	orrs r2, r0
	movs r3, #15
	ands r2, r3
	subs r3, #16
	strb r2, [r5, #9]
	add r9, r3
	mov r2, r8
	str r3, [r2, #24]
	mov r1, r9
	movs r3, #28
	adds r5, #40
	add r8, r3
	cmp r1, #0
	bge .L_0200a444
	movs r0, #145
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	lsls r0, r0, #1
	bl Func_0200286c
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #68
	adds r3, r7, r2
	str r0, [r3]
	mov r8, r0
	movs r1, #1
	bl Func_0200285c
	mov r0, r8
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r2, r8
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r2, [r6, #80]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r2, #18]
	ldr r2, .L_0200a524
	ldr r3, [r6, #8]
	ldr r1, .L_0200a51c
	adds r3, r3, r2
	str r3, [r6, #8]
	adds r3, r6, #0
	adds r3, #85
	strb r1, [r3]
	movs r3, #128
	movs r1, #128
	lsls r3, r3, #4
	lsls r1, r1, #4
	movs r2, #128
	adds r1, #86
	adds r3, #84
	lsls r2, r2, #4
	movs r5, #0
	adds r6, r7, r3
	adds r2, #92
	adds r3, r7, r1
	strh r5, [r6]
	strh r5, [r3]
	adds r3, r7, r2
	strh r5, [r3]
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #94
	adds r2, r7, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #221
	b .L_0200a528
	.2byte 0x0000
.L_0200a51c:
	.4byte 0x00000000
.L_0200a520:
	.4byte Data_02002a6c
.L_0200a524:
	.4byte 0xfff40000
.L_0200a528:
	bl Func_02002a64
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a588
	bl Scheduler_AddOrUpdateCallback
	movs r2, #186
	movs r1, #0
	ldrsh r3, [r6, r1]
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	beq .L_0200a560
.L_0200a544:
	movs r0, #1
	bl WaitFrames
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #84
	adds r3, r7, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r1, #186
	lsls r1, r1, #2
	adds r1, #255
	cmp r3, r1
	bne .L_0200a544
.L_0200a560:
	ldr r0, .L_0200a588
	bl Scheduler_RemoveCallbackFar
	mov r0, r8
	bl Func_02002874
	mov r0, r11
	bl Resource_ResetEntry
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a588:
	.4byte Func_02001fd0
	.section .text.x0200a58c,"ax",%progbits
	.global Func_0200258c
	.thumb_func
Func_0200258c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	movs r2, #128
	lsls r2, r2, #4
	movs r3, #148
	adds r5, r7, r2
	lsls r3, r3, #5
	movs r2, #95
	adds r6, r7, r3
	mov r8, r2
.L_0200a5aa:
	ldr r3, [r5, #24]
	cmp r3, #15
	bhi .L_0200a5f4
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #130
	adds r1, r7, r2
	ldrh r1, [r1]
	movs r2, #7
	asrs r3, r3, #1
	ands r3, r2
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, .L_0200a5ec
	ldr r2, .L_0200a5f0
	ands r1, r3
	ldrh r3, [r6, #8]
	adds r0, r6, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #8]
	adds r1, r5, #0
	bl Func_02002a14
	adds r0, r5, #0
	movs r1, #64
	movs r2, #0
	bl Func_02002a1c
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	b .L_0200a5f4
.L_0200a5ec:
	.4byte 0x000003ff
.L_0200a5f0:
	.4byte 0xfffffc00
.L_0200a5f4:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	adds r6, #40
	adds r5, #28
	cmp r2, #0
	bge .L_0200a5aa
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a60c,"ax",%progbits
	.global Func_0200260c
	.thumb_func
Func_0200260c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r2, [sp, #4]
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r10, r0
	mov r8, r3
	adds r7, r1, #0
	bl Random16Far
	movs r1, #134
	lsls r1, r1, #6
	add r1, r8
	movs r3, #0
	ldrsh r2, [r1, r3]
	mov r9, r1
	lsls r5, r2, #2
	adds r5, r5, r2
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r5, r5, #3
	movs r1, #148
	mov r11, r0
	lsls r1, r1, #5
	lsls r3, r3, #2
	add r5, r8
	mov r0, r10
	adds r5, r5, r1
	add r8, r3
	bl Func_02002a24
	ldrb r2, [r5, #9]
	movs r3, #3
	ands r0, r3
	movs r3, #13
	negs r3, r3
	ands r3, r2
	lsls r0, r0, #2
	orrs r3, r0
	strb r3, [r5, #9]
	mov r0, r10
	bl Func_02002a2c
	movs r6, #128
	lsls r6, r6, #4
	add r6, r8
	movs r3, #0
	strh r0, [r5, #30]
	str r3, [r6, #24]
	str r7, [r6]
	ldr r1, [sp, #4]
	mov r10, r3
	str r1, [r6, #4]
	ldr r3, [sp, #0]
	str r3, [r6, #8]
	bl Random16Far
	movs r1, #128
	lsls r1, r1, #10
	lsls r0, r0, #1
	adds r2, r6, #0
	adds r0, r0, r1
	mov r1, r11
	bl Vector_AddPolarOffsetFar
	mov r3, r10
	str r3, [r6, #12]
	bl Random16Far
	movs r1, #192
	lsls r1, r1, #8
	lsrs r0, r0, #1
	mov r3, r10
	adds r0, r0, r1
	str r3, [r6, #20]
	str r0, [r6, #16]
	bl Random16Far
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #12
	add r8, r1
	mov r2, r8
	mov r1, r11
	lsrs r0, r0, #2
	bl Vector_AddPolarOffsetFar
	mov r3, r9
	ldrh r0, [r3]
	mov r1, r9
	adds r0, #1
	strh r0, [r1]
	lsls r0, r0, #16
	movs r1, #96
	asrs r0, r0, #16
	bl Engine_MathRemainder
	mov r3, r9
	strh r0, [r3]
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a6f4,"ax",%progbits
	.global Func_020026f4
	.thumb_func
Func_020026f4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #136
	movs r0, #220
	sub sp, #4
	bl Runtime_AllocateHeapBlockFar
	adds r6, r0, #0
	ldr r0, .L_0200a7a8
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_02002824
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #4
	adds r2, r6, #0
	adds r5, r0, #0
	bl VramBlock_LoadCached
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #130
	mov r10, r0
	adds r3, r6, r1
	mov r2, r10
	adds r1, #2
	strh r2, [r3]
	adds r3, r6, r1
	strh r5, [r3]
	movs r2, #128
	movs r3, #148
	lsls r2, r2, #4
	lsls r3, r3, #5
	movs r1, #95
	adds r7, r6, r2
	adds r5, r6, r3
	mov r8, r1
.L_0200a74c:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_02002a0c
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	movs r1, #13
	strb r3, [r5, #5]
	negs r1, r1
	movs r3, #15
	ands r3, r2
	adds r2, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	movs r3, #240
	strh r3, [r5, #30]
	subs r3, #241
	add r8, r3
	mov r2, r8
	str r3, [r7, #24]
	adds r5, #40
	adds r7, #28
	cmp r2, #0
	bge .L_0200a74c
	movs r1, #134
	lsls r1, r1, #6
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200a7ac
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a7a8:
	.4byte 0x000001e1
.L_0200a7ac:
	.4byte Func_0200258c
	.section .text.x0200a7b0,"ax",%progbits
	.global Func_020027b0
	.thumb_func
Func_020027b0:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r0, .L_0200a7d8
	ldr r5, [r3]
	bl Scheduler_RemoveCallbackFar
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #132
	adds r5, r5, r3
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Resource_ResetEntry
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	pop {r5, pc}
.L_0200a7d8:
	.4byte Func_0200258c
	.section .rodata.x0200aa6c,"a",%progbits
	.global Data_02002a6c
Data_02002a6c:
	.4byte 0xc13c0100
	.4byte 0xb9d2cf52
	.4byte 0x13465bb3
	.4byte 0x5afce9e8
	.4byte 0xafd1ba81
	.4byte 0x9a39eb47
	.4byte 0x7445b435
	.4byte 0x2ff63444
	.4byte 0x74759d7d
	.4byte 0x581ed0e8
	.4byte 0x499ece98
	.4byte 0x66cc9bb5
	.4byte 0x28a91658
	.4byte 0xd6468754
	.4byte 0xb8fc3ca3
	.4byte 0x0394251c
	.4byte 0xa3921e4e
	.4byte 0xed0a619d
	.4byte 0x290915e5
	.4byte 0xd0d9ce84
	.4byte 0xcb0f067c
	.4byte 0x3a788051
	.4byte 0x4d0804f7
	.4byte 0xe32e6224
	.4byte 0x26d300f1
	.4byte 0xc780ce20
	.4byte 0x883c5604
	.4byte 0x991f8fbc
	.4byte 0x64fd4ba6
	.4byte 0x5026593a
	.4byte 0x42e4728b
	.4byte 0x4d0a3424
	.4byte 0xbe74d99b
	.4byte 0xe6f939cd
	.4byte 0xb126f91e
	.4byte 0xf32807ec
	.4byte 0x7c84cbcd
	.4byte 0xdc069263
	.4byte 0x984841e2
	.4byte 0x998e2689
	.4byte 0xd98df5e0
	.4byte 0x7ce0c6f9
	.4byte 0x70784b43
	.4byte 0xf7f6bc36
	.4byte 0xc7b95e4d
	.4byte 0xe7719be3
	.4byte 0x17a9ad75
	.4byte 0x586f8f25
	.4byte 0x75fcd4d3
	.4byte Text_MessageContexts + 0x7d28
	.4byte 0x9c9a2b93
	.4byte 0xfbcfa216
	.4byte 0x7e458240
	.4byte 0x86f9f834
	.4byte 0xfc221ebe
	.4byte 0xef811f5e
	.4byte 0x813ce478
	.4byte 0x242f015d
	.4byte 0xc6de6c1d
	.4byte 0x6faf3beb
	.4byte 0x5013be3c
	.4byte 0xf8f0a8bc
	.4byte 0x78c181c6
	.4byte 0xcf386e27
	.4byte 0xde5c3c49
	.4byte 0x6677cfc8
	.4byte 0x2a0df303
	.4byte 0x8f8df3f1
	.4byte 0xdf1e77c1
	.4byte 0x3e78c038
	.4byte 0x3de7e014
	.4byte 0x638c3f8f
	.4byte 0x4481687e
	.4byte 0x401077ce
	.4byte 0xf831f014
	.4byte 0x36800cc6
	.4byte 0x7d8b83c0
	.4byte 0x80cfbf02
	.4byte 0xe1c3400a
	.4byte 0xb40d23c6
	.4byte 0xde213ce2
	.4byte 0x9e3c2f19
	.4byte 0xe39df5f3
	.4byte 0xa8c6f9ae
	.4byte 0x8291fe62
	.4byte 0x80556f9c
	.4byte 0x88e0f404
	.4byte 0x38c0bf22
	.4byte 0xe8dadf3f
	.4byte 0xe01df023
	.4byte 0x102cef93
	.4byte 0x12c8cc41
	.4byte 0xbe40f187
	.4byte 0x39f09e71
	.4byte 0xb7dfcef8
	.4byte 0x0b10cf5e
	.4byte 0x3e012227
	.4byte 0x0e4fbaf0
	.4byte 0x344d6c12
	.4byte 0xc30f8e72
	.4byte 0xf8225e77
	.4byte 0x3bebd6fb
	.4byte 0xbdc9e3ef
	.4byte 0x3cf1a54a
	.4byte 0x323dcfe0
	.4byte 0x859f5350
	.4byte 0x38339cab
	.4byte 0xf5e0510f
	.4byte 0x3410af12
	.4byte 0x1f8088f5
	.4byte 0x9fbaf811
	.4byte 0xf19223df
	.4byte 0x607f04a3
	.4byte 0xadf82f92
	.4byte 0x04abd09e
	.4byte 0xc14f8168
	.4byte 0xaf1efc75
	.4byte 0xf82f19f3
	.4byte 0x301f82fa
	.4byte 0xd7cc2978
	.4byte 0xe0bebc73
	.4byte 0xc17dfdfd
	.4byte 0x033e31e3
	.4byte 0x1f45f3f4
	.4byte 0x18ebc7bf
	.4byte 0x043e08c6
	.4byte 0x60fe70ff
	.4byte 0x904fe2be
	.4byte 0xbaf1735e
	.4byte 0xbb803205
	.4byte 0xa18451f8
	.4byte 0xc00d1f80
	.4byte 0xf0ba1e1e
	.4byte 0x9f9af819
	.4byte 0xc17d79e3
	.4byte 0x1f7008cf
	.4byte 0x2df20be7
	.4byte 0x31f1147f
	.4byte 0xc04f8120
	.4byte 0x32147c17
	.4byte 0x5e1eefe1
	.4byte 0x7c2640a1
	.4byte 0x181fd39d
	.4byte 0xefe15ebc
	.4byte 0x67053ea8
	.4byte 0x8e7809f0
	.4byte 0xd1fc1bc7
	.4byte 0xfc0bd7c0
	.4byte 0x4aebc0dd
	.4byte 0x39f48a5e
	.4byte 0xc10f8eb8
	.4byte 0xbe147c63
	.4byte 0x3e1defe0
	.4byte 0x49f24f08
	.4byte 0x00b382f9
	.4byte 0xf82f891f
	.4byte 0xe3240ab9
	.4byte 0x0c3ea7e7
	.4byte 0x2009f2ef
	.4byte 0xf907af8a
	.4byte 0xc688dc18
	.4byte 0x1e07e087
	.4byte 0xe754707f
	.4byte 0x3f13b043
	.4byte 0xb851f3b3
	.4byte 0x09008f99
	.4byte 0xebeb483c
	.4byte 0x4b053e49
	.4byte 0xf3f2c632
	.4byte 0x3e026724
	.4byte 0xa3f2ef0e
	.4byte 0xf09a5f2b
	.4byte 0x00003efe
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00000178
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000178
	.4byte 0x400000e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000180
	.4byte 0x40000188
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x00000070
	.4byte 0x40000110
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff005a
	.4byte 0x00000178
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0062
	.4byte 0x00000180
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x00000048
	.4byte 0x80000108
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
	.4byte 0x00000004
	.4byte 0x00105006
	.4byte 0x00201005
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff005c
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00020000
	.4byte 0xffff005c
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00028000
	.4byte 0xffff00e8
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0002a000
	.4byte 0xffff00e8
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0002a000
	.4byte 0xffff00e8
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0002a000
	.4byte 0xffff00e8
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0002a000
	.4byte 0xffff00e8
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0002a000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0002e000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0002e000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0002e000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0002e000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0002e000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneEvents
gSceneEvents:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte Resource_Data197 + 0x2e8c1
	.4byte Func_02000440
	.4byte 0x00000002
	.4byte Resource_Data197 + 0xe8c2
	.4byte Func_02000510
	.4byte 0x00000002
	.4byte Resource_Data197 + 0x3e8c3
	.4byte Func_02001a2c
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte Func_02001bb0
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001608
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_02000054
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_020000c8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200b000
Data_0200b000:
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000003
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02003040
Data_02003040:
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
