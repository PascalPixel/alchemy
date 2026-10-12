.syntax unified
	.thumb
	.section .text.x02008184,"ax",%progbits
	.balign 4
	.global Func_02000184
	.thumb_func
Func_02000184:
	push {r5, r6, r7, lr}
	movs r0, #17
	bl Object_GetById
	ldr r3, .L_02008308
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r7, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r2, r3, #19
	ldr r3, [r0, #16]
	asrs r1, r3, #19
	ldr r3, [r7, #8]
	asrs r3, r3, #19
	cmp r2, #36
	bgt .L_020081b2
	cmp r3, #37
	ble .L_020081b2
	b .L_02008306
.L_020081b2:
	cmp r2, #37
	ble .L_020081bc
	cmp r3, #36
	bgt .L_020081bc
	b .L_02008306
.L_020081bc:
	movs r6, #1
	cmp r2, #36
	ble .L_020081c4
	negs r6, r6
.L_020081c4:
	movs r5, #1
	negs r5, r5
	cmp r1, #95
	bgt .L_020081ce
	movs r5, #1
.L_020081ce:
	cmp r5, #1
	bne .L_020082a0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082a0
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081ee
	b .L_02008306
.L_020081ee:
	bl Func_02001080
	movs r0, #0
	bl Func_020011e0
	movs r0, #17
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	movs r2, #32
	movs r1, #0
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #17
	movs r1, #1
	bl Func_02001188
	movs r0, #17
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #8
	negs r1, r1
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #20
	movs r0, #17
	movs r1, #4
	bl ObjectMotion_Launch
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02001168
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #17
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #8
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #17
	bl Func_02001160
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02001088
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008306
.L_020082a0:
	bl Func_02001080
	movs r0, #0
	bl Func_020011e0
	lsls r5, r5, #5
	movs r0, #17
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	adds r2, r5, #0
	movs r1, #0
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	lsls r1, r6, #4
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	lsls r1, r6, #1
	adds r1, r1, r6
	lsls r1, r1, #4
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r7, #40]
	movs r0, #17
	negs r5, r5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #17
	movs r1, #0
	adds r2, r5, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02001088
.L_02008306:
	pop {r5, r6, r7, pc}
.L_02008308:
	.4byte gPartyState
