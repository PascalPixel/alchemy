	.set OVERLAY_657_EUROPEAN, 0
	.ifdef TLA_EDITION_DE
	.set OVERLAY_657_EUROPEAN, 1
	.endif
	.ifdef TLA_EDITION_ES
	.set OVERLAY_657_EUROPEAN, 1
	.endif
	.ifdef TLA_EDITION_FR
	.set OVERLAY_657_EUROPEAN, 1
	.endif
	.ifdef TLA_EDITION_IT
	.set OVERLAY_657_EUROPEAN, 1
	.endif
.syntax unified
	.thumb
	.section .text.x02008ce0,"ax",%progbits
	.global Func_02000ce0
	.thumb_func
Func_02000ce0:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008cfa
	movs r0, #152
	lsls r0, r0, #4
	bl GameFlag_SetBit
	b .L_02009048
.L_02008cfa:
	bl Func_02001f5c
	movs r0, #0
	bl Func_020020ac
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #4
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #136
	movs r0, #4
	bl ObjectMotion_SetPositionAndReset
	ldr r6, .L_0200904c
	adds r0, r6, #0
	bl Func_0200201c
	movs r2, #16
	movs r3, #128
	movs r0, #5
	movs r1, #16
	negs r2, r2
	lsls r3, r3, #7
	bl Func_020020b4
	movs r2, #16
	movs r3, #128
	movs r0, #9
	movs r1, #32
	negs r2, r2
	lsls r3, r3, #7
	bl Func_020020b4
	movs r1, #16
	movs r2, #16
	movs r3, #128
	lsls r3, r3, #7
	negs r1, r1
	negs r2, r2
	movs r0, #6
	bl Func_020020b4
	movs r0, #80
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #9
	bl Func_0200204c
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02009050
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #150
	lsls r1, r1, #1
	movs r2, #152
	movs r0, #9
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_02009054
	adds r1, #153
	bl Func_0200205c
	movs r0, #200
	movs r1, #1
	movs r2, #172
	lsls r2, r2, #17
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_0200206c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #152
	lsls r1, r1, #6
	ldr r0, .L_02009058
	adds r1, #102
	bl Func_0200205c
	movs r0, #150
	movs r1, #1
	movs r2, #172
	lsls r2, r2, #17
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_0200206c
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #204
	lsls r1, r1, #8
	ldr r0, .L_0200905c
	adds r1, #204
	bl Func_0200205c
	movs r0, #132
	movs r1, #1
	movs r2, #128
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	bl Func_0200206c
	movs r0, #5
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002034
	movs r0, #6
	movs r1, #0
	bl Func_02002044
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02002044
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #9
	bl Func_0200204c
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r5, .L_02009060
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	ldr r5, [r5]
	cmp r0, #0
	bne .L_02008ece
	adds r0, r5, #0
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008ee2
.L_02008ece:
	adds r0, r5, #0
	bl ObjectMotion_WaitForAnimationChange
	adds r0, r6, #5
	bl Func_0200201c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
.L_02008ee2:
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002034
	movs r0, #6
	movs r1, #0
	bl Func_02002044
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	movs r0, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #144
	strb r3, [r0]
	lsls r1, r1, #1
	movs r2, #144
	movs r0, #9
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	bl Func_02002034
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02009058
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_02009058
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02009058
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02008fd8
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
.L_02008fd8:
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02008ff8
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_02008ff8:
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02009018
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_02009018:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02001fc4
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02001fc4
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02001fc4
	bl Func_02001f64
	movs r0, #152
	lsls r0, r0, #4
	bl GameFlag_SetBit
.L_02009048:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200904c:
	.4byte MsgLookItS
.L_02009050:
	.4byte 0x00019999
.L_02009054:
	.4byte 0x0004cccc
.L_02009058:
	.4byte 0x00013333
.L_0200905c:
	.4byte 0x00066666
.L_02009060:
	.4byte gPartyState
	.section .text.x020094c0,"ax",%progbits
	.global Func_020014c0
	.thumb_func
Func_020014c0:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020094d4
	bl .L_02009d50
.L_020094d4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #137
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094e6
	bl .L_02009d50
.L_020094e6:
	bl Func_02001f5c
	movs r0, #0
	bl Func_020020ac
	ldr r0, .L_02009770
	bl Func_0200201c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #176
	movs r0, #4
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r3, #128
	movs r2, #8
	lsls r3, r3, #7
	movs r0, #7
	movs r1, #16
	bl Func_020020b4
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_0200205c
	movs r0, #144
	movs r1, #1
	movs r2, #208
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl Motion_CamBounds
	movs r3, #128
	movs r0, #9
	movs r1, #32
	movs r2, #0
	lsls r3, r3, #7
	bl Func_020020b4
	movs r2, #8
	movs r3, #128
	movs r0, #6
	movs r1, #48
	negs r2, r2
	lsls r3, r3, #7
	bl Func_020020b4
	movs r1, #16
	movs r2, #8
	movs r3, #128
	lsls r3, r3, #7
	negs r1, r1
	negs r2, r2
	movs r0, #5
	bl Func_020020b4
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #7
	ldr r1, .L_02009774
	ldr r2, .L_02009778
	bl ObjectMotion_SetSpeedParameters
	movs r1, #140
	lsls r1, r1, #1
	movs r2, #224
	movs r0, #7
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_0200204c
	movs r0, #7
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #20
	movs r0, #7
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_0200205c
	movs r0, #144
	movs r1, #1
	movs r2, #184
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #7
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #16
	negs r2, r2
	movs r0, #7
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #129
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #9
	bl Func_0200204c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	bl Func_02002044
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r1, #2
	adds r1, #255
	movs r2, #20
	movs r0, #7
	bl Func_0200204c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #5
	bl Func_0200204c
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #7
	bl Func_0200204c
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r1, #224
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02002054
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02002034
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #224
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02002044
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	ldr r3, .L_0200977c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009780
	movs r1, #8
	movs r2, #20
	adds r1, #255
	movs r0, #5
	bl Func_0200204c
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_020097ca
.L_02009770:
	.4byte MsgMyShipAt
.L_02009774:
	.4byte 0x00026666
.L_02009778:
	.4byte 0x00013333
.L_0200977c:
	.4byte gPartyState
.L_02009780:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #160
	adds r3, #2
	strh r3, [r2]
	movs r0, #7
	movs r2, #0
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02002034
.L_020097ca:
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #9
	bl Func_0200204c
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #144
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	ldr r5, .L_02009c04
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_0200204c
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #6
	bl Func_0200204c
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	bl Func_02002044
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #7
	bl Func_0200204c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02002054
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #7
	bl Func_0200204c
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_0200204c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #6
	bl Func_0200204c
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_0200204c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_0200204c
	movs r2, #20
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #0
	bl Func_02002044
	movs r1, #128
	movs r2, #40
	movs r0, #9
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02002044
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #3
	bl Motion_SetVarCbAndRefresh
	movs r2, #0
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02002054
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #7
	bl Func_0200204c
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02002044
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #6
	bl Func_0200204c
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #4
	bl Func_0200204c
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #5
	bl Func_0200204c
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02002034
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #16
	movs r0, #7
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #6
	adds r1, #255
	movs r2, #20
	movs r0, #7
	bl Func_0200204c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #5
	bl Func_0200204c
	movs r2, #16
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #20
	movs r0, #7
	movs r1, #0
	bl Func_0200202c
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r2, #0
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r1, #0
	b .L_02009c08
	.2byte 0x0000
.L_02009c04:
	.4byte gPartyState
.L_02009c08:
	movs r0, #9
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_02009c3e
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009c60
.L_02009c3e:
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #9
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02002034
.L_02009c60:
	ldr r5, .L_02009d54
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #128
	ldr r0, [r5]
	movs r2, #0
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02002044
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #132
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #7
	bl Func_0200204c
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r2, #0
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02009d58
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02009d58
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_02009d58
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #7
	ldr r1, .L_02009d58
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_02009d5c
	movs r0, #5
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #9
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #6
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #7
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #129
	bl GameFlag_SetBit
	bl Func_02001f64
.L_02009d50:
	pop {r5, pc}
	.2byte 0x0000
.L_02009d54:
	.4byte gPartyState
.L_02009d58:
	.4byte 0x00013333
.L_02009d5c:
	.4byte Data_020020e4
	.section .bss,"aw",%nobits
	.global Data_020024b0
Data_020024b0:
	.space 0x00000004
	.global Data_020024b4
Data_020024b4:
