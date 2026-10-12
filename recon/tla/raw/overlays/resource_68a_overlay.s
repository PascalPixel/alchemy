.syntax unified
	.thumb
	.section .text.x02008110,"ax",%progbits
	.balign 4
	.global Func_02000110
	.thumb_func
Func_02000110:
	push {r5, r6, lr}
	sub sp, #12
	movs r3, #5
	movs r2, #13
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #7
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl Func_02001a24
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #108
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200813c
	b .L_020083ae
.L_0200813c:
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #108
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #110
	bl GameFlag_SetBit
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #176
	movs r2, #216
	movs r0, #12
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_0200195c
	movs r0, #12
	movs r1, #9
	movs r2, #0
	bl ObjectMotion_Launch
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r6, #29
.L_02008186:
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r5, #6]
	movs r2, #255
	ldr r3, [r5, #24]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bgt .L_020081a4
	movs r2, #160
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r5, #24]
.L_020081a4:
	ldr r3, [r5, #24]
	movs r0, #1
	str r3, [r5, #28]
	subs r6, #1
	bl WaitFrames
	cmp r6, #0
	bge .L_02008186
	movs r3, #192
	lsls r3, r3, #6
	strh r3, [r5, #6]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r0, #12
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #1
	movs r0, #12
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #9
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #120
	movs r2, #200
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r2, #200
	movs r1, #136
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #3
	bl Battle_WaitMode0
	ldr r5, .L_020083b4
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	movs r1, #224
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #0
	movs r2, #16
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl Func_0200195c
	movs r0, #13
	bl Object_GetById
	ldr r3, .L_020083b8
	movs r6, #0
	str r3, [r0, #108]
	movs r0, #13
	bl Object_GetById
	adds r0, #100
	strh r6, [r0]
.L_020083ae:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020083b4:
	.4byte gPartyState
.L_020083b8:
	.4byte Func_02000048
	.section .text.x02008468,"ax",%progbits
	.balign 4
	.global Func_02000468
	.thumb_func
Func_02000468:
	push {r5, r6, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008528
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #108
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008528
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #110
	bl GameFlag_ClearBit
	movs r0, #13
	bl Func_02000080
	movs r1, #135
	movs r2, #184
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_0200195c
	movs r3, #176
	lsls r3, r3, #13
	str r3, [r5, #12]
	movs r0, #12
	movs r1, #11
	movs r2, #0
	bl ObjectMotion_Launch
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r0, #12
	ldr r1, .L_0200852c
	ldr r2, .L_02008530
	bl ObjectMotion_SetSpeedParameters
	movs r1, #156
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r6, #23
.L_020084e6:
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r5, #6]
	movs r2, #255
	ldr r3, [r5, #24]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bgt .L_02008504
	movs r2, #160
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r5, #24]
.L_02008504:
	ldr r3, [r5, #24]
	movs r0, #1
	str r3, [r5, #28]
	subs r6, #1
	bl WaitFrames
	cmp r6, #0
	bge .L_020084e6
	movs r3, #192
	lsls r3, r3, #6
	strh r3, [r5, #6]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
.L_02008528:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200852c:
	.4byte 0x00023333
.L_02008530:
	.4byte 0x00011999
