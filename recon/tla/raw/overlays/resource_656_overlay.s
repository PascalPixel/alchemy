.syntax unified
	.thumb
	.section .text.x0200821c,"ax",%progbits
	.balign 4
	.global Func_0200021c
	.thumb_func
Func_0200021c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	sub sp, #8
	mov r9, r3
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r7, #0
	movs r5, #8
.L_0200823e:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008250
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008250:
	adds r5, #1
	cmp r5, #63
	bls .L_0200823e
	ldr r0, .L_0200831c
	movs r5, #0
	movs r1, #0
.L_0200825c:
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldrh r3, [r0, r1]
	cmp r2, r3
	bne .L_0200826e
	adds r7, r5, #0
.L_0200826e:
	adds r5, #1
	adds r1, #12
	cmp r5, #9
	bls .L_0200825c
	movs r0, #158
	bl Func_02002de0
	ldr r2, .L_02008320
	movs r3, #133
	mov r10, r2
	lsls r3, r3, #2
	add r10, r3
	mov r4, r10
	ldr r0, [r4]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	lsls r5, r7, #1
	ldr r2, .L_0200831c
	adds r5, r5, r7
	lsls r5, r5, #2
	adds r6, r5, #0
	mov r8, r2
	adds r6, #8
	ldrh r2, [r2, r6]
	movs r1, #1
	add r6, r8
	movs r0, #2
	ldrh r3, [r6, #2]
	str r1, [sp, #0]
	str r0, [sp, #4]
	movs r1, #0
	movs r0, #0
	bl Func_02002c30
	ldrh r2, [r6, #2]
	mov r3, r8
	adds r5, #4
	ldr r0, [r3, r5]
	adds r2, #60
	ldrh r1, [r6]
	bl Func_02002c28
	mov r4, r10
	ldr r0, [r4]
	movs r1, #2
	bl Object_SetModeById
	mov r2, r10
	ldr r0, [r2]
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	mov r3, r10
	ldr r0, [r3]
	cmp r7, #12
	bne .L_020082f2
	movs r2, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	b .L_020082fc
.L_020082f2:
	movs r2, #4
	movs r1, #2
	negs r2, r2
	bl ObjectMotion_CenterOnColumn
.L_020082fc:
	movs r0, #4
	bl Battle_WaitMode0
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl Func_02002d70
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200831c:
	.4byte Data_02003904
.L_02008320:
	.4byte gPartyState
	.section .text.x02008324,"ax",%progbits
	.global Func_02000324
	.thumb_func
Func_02000324:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #8
	ldr r7, [r3, #108]
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r6, #0
	movs r5, #8
.L_02008342:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008354
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008354:
	adds r5, #1
	cmp r5, #63
	bls .L_02008342
	ldr r0, .L_02008430
	movs r5, #0
	movs r1, #0
.L_02008360:
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldrh r3, [r0, r1]
	cmp r2, r3
	bne .L_02008372
	adds r6, r5, #0
.L_02008372:
	adds r5, #1
	adds r1, #12
	cmp r5, #3
	bls .L_02008360
	cmp r6, #1
	bhi .L_0200839c
	movs r0, #132
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200839c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #228
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200839c
	adds r6, #4
.L_0200839c:
	movs r0, #158
	bl Func_02002de0
	ldr r2, .L_02008434
	movs r3, #133
	mov r10, r2
	lsls r3, r3, #2
	add r10, r3
	mov r4, r10
	ldr r0, [r4]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	lsls r5, r6, #1
	ldr r2, .L_02008430
	adds r5, r5, r6
	lsls r5, r5, #2
	adds r6, r5, #0
	mov r8, r2
	adds r6, #8
	ldrh r2, [r2, r6]
	movs r1, #1
	add r6, r8
	movs r0, #2
	ldrh r3, [r6, #2]
	str r1, [sp, #0]
	str r0, [sp, #4]
	movs r1, #0
	movs r0, #0
	bl Func_02002c30
	ldrh r2, [r6, #2]
	mov r3, r8
	adds r5, #4
	ldr r0, [r3, r5]
	ldrh r1, [r6]
	adds r2, #60
	bl Func_02002c28
	mov r4, r10
	ldr r0, [r4]
	movs r1, #2
	bl Object_SetModeById
	mov r2, r10
	ldr r0, [r2]
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	mov r3, r10
	movs r2, #4
	negs r2, r2
	ldr r0, [r3]
	movs r1, #2
	bl ObjectMotion_CenterOnColumn
	movs r0, #4
	bl Battle_WaitMode0
	movs r4, #170
	lsls r4, r4, #1
	adds r3, r7, r4
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02002d70
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008430:
	.4byte Data_0200397c
.L_02008434:
	.4byte gPartyState
	.section .text.x02008870,"ax",%progbits
	.balign 4
	.global Func_02000870
	.thumb_func
Func_02000870:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #131
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200888c
	b .L_02008ad8
.L_0200888c:
	ldr r3, .L_02008ae4
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r7]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #210
	ldr r0, [r7]
	lsls r1, r1, #2
	movs r2, #72
	bl ObjectMotion_SetPositionAndReset
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	adds r5, #34
	mov r9, r0
	mov r10, r5
	cmp r0, #0
	beq .L_02008972
	movs r0, #243
	bl Func_02002de0
	movs r1, #129
	ldr r0, [r7]
	lsls r1, r1, #1
	bl Func_02002d48
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #0
	bl Func_02002c48
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02002c48
	movs r5, #3
	movs r1, #68
	movs r0, #68
	movs r2, #50
	movs r3, #64
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c30
	movs r3, #2
	mov r2, r10
	strb r3, [r2]
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, [r7]
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r7]
	ldr r1, .L_02008ae8
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #210
	lsls r1, r1, #2
	movs r2, #88
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #232
	bl Func_02002de0
	movs r2, #50
	movs r3, #64
	movs r0, #72
	movs r1, #68
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c30
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #1
	mov r2, r10
	strb r3, [r2]
	b .L_02008ad4
.L_02008972:
	movs r0, #9
	bl Object_GetById
	movs r1, #2
	adds r6, r0, #0
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	ldr r1, .L_02008aec
	ldr r2, .L_02008af0
	movs r0, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #206
	strb r3, [r0]
	lsls r1, r1, #2
	movs r2, #88
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
	movs r0, #243
	bl Func_02002de0
	movs r1, #129
	ldr r0, [r7]
	lsls r1, r1, #1
	bl Func_02002d48
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #0
	bl Func_02002c48
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02002c48
	movs r5, #3
	movs r1, #68
	movs r0, #68
	movs r2, #50
	movs r3, #64
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c30
	movs r3, #2
	mov r8, r3
	mov r2, r8
	mov r3, r10
	strb r2, [r3]
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	ldr r0, [r7]
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	ldr r0, [r7]
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r7]
	ldr r1, .L_02008ae8
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #210
	movs r2, #88
	ldr r0, [r7]
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #128
	ldr r0, [r7]
	lsls r1, r1, #8
	bl Func_02002d30
	ldr r0, [r7]
	movs r1, #9
	bl Object_LinkObjectAndSetCallback
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #72]
	mov r2, r8
	adds r6, #34
	strb r2, [r6]
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #2
	movs r2, #74
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #214
	movs r2, #88
	movs r0, #9
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #129
	lsls r1, r1, #1
	ldr r0, [r7]
	bl Func_02002d48
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #232
	bl Func_02002de0
	movs r1, #68
	movs r2, #50
	movs r3, #64
	movs r0, #72
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c30
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, [r7]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r3, #1
	mov r2, r10
	strb r3, [r2]
	mov r3, r9
	movs r0, #1
	strb r3, [r6]
	bl WaitFrames
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_02008ad4:
	bl Func_02002c70
.L_02008ad8:
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008ae4:
	.4byte gPartyState
.L_02008ae8:
	.4byte 0x00019999
.L_02008aec:
	.4byte 0x00026666
.L_02008af0:
	.4byte 0x00013333
	.section .text.x02008e2c,"ax",%progbits
	.balign 4
	.global Func_02000e2c
	.thumb_func
Func_02000e2c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	sub sp, #8
	bl GameFlag_Test
	mov r8, r0
	cmp r0, #0
	bne .L_02008f44
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f44
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	asrs r7, r3, #20
	cmp r7, #35
	bne .L_02008f44
	ldr r3, [r6, #16]
	asrs r5, r3, #20
	cmp r5, #9
	bne .L_02008f44
	ldr r3, .L_020090b4
	movs r1, #133
	lsls r1, r1, #2
	adds r1, r1, r3
	ldr r0, [r1]
	mov r9, r1
	bl Object_GetById
	mov r10, r0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #183
	bl Func_02002de0
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #68
	movs r2, #35
	movs r3, #69
	bl Func_02002c30
	movs r0, #35
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
	adds r3, r6, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	movs r1, #3
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	mov r1, r10
	ldr r2, [r6, #16]
	ldr r3, [r1, #16]
	cmp r2, r3
	ble .L_02008eda
	mov r2, r9
	ldr r0, [r2]
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_02008eda:
	movs r5, #0
.L_02008edc:
	ldr r3, [r6, #12]
	ldr r1, .L_020090b8
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r5, #1
	bl WaitFrames
	cmp r5, #127
	bls .L_02008edc
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r1, #142
	movs r2, #152
	lsls r1, r1, #18
	lsls r2, r2, #16
	movs r0, #10
	bl Func_02002cd0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	mov r1, r10
	ldr r2, [r6, #16]
	ldr r3, [r1, #16]
	cmp r2, r3
	ble .L_02008f40
	ldr r5, .L_020090b4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_02008f40:
	bl Func_02002c70
.L_02008f44:
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	mov r8, r0
	cmp r0, #0
	bne .L_02009052
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009052
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	asrs r7, r3, #20
	cmp r7, #29
	bne .L_02009052
	ldr r3, [r6, #16]
	asrs r5, r3, #20
	cmp r5, #9
	bne .L_02009052
	ldr r3, .L_020090b4
	movs r1, #133
	lsls r1, r1, #2
	adds r1, r1, r3
	ldr r0, [r1]
	mov r9, r1
	bl Object_GetById
	mov r10, r0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #183
	bl Func_02002de0
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #29
	movs r1, #68
	movs r2, #29
	movs r3, #69
	bl Func_02002c30
	movs r0, #35
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
	adds r3, r6, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	movs r1, #3
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	mov r1, r10
	ldr r2, [r6, #16]
	ldr r3, [r1, #16]
	cmp r2, r3
	ble .L_02008fe8
	mov r2, r9
	ldr r0, [r2]
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_02008fe8:
	movs r5, #0
.L_02008fea:
	ldr r3, [r6, #12]
	ldr r1, .L_020090b8
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r5, #1
	bl WaitFrames
	cmp r5, #127
	bls .L_02008fea
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r1, #236
	movs r2, #152
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #11
	bl Func_02002cd0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	mov r1, r10
	ldr r2, [r6, #16]
	ldr r3, [r1, #16]
	cmp r2, r3
	ble .L_0200904e
	ldr r5, .L_020090b4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_0200904e:
	bl Func_02002c70
.L_02009052:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090a6
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090a6
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r0, #188
	bl Func_02002de0
	movs r3, #3
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #124
	movs r2, #31
	movs r3, #64
	movs r0, #4
	bl Func_02002c30
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #130
	bl GameFlag_SetBit
	bl Func_02002c70
.L_020090a6:
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020090b4:
	.4byte gPartyState
.L_020090b8:
	.4byte 0xffffe000
	.section .text.x020092b4,"ax",%progbits
	.balign 4
	.global Func_020012b4
	.thumb_func
Func_020012b4:
	push {r5, r6, lr}
	ldr r5, .L_02009360
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r0, #128
	movs r1, #128
	movs r2, #248
	movs r3, #128
	lsls r3, r3, #17
	lsls r0, r0, #12
	lsls r1, r1, #13
	lsls r2, r2, #16
	bl Func_02002d68
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	ldr r6, .L_02009364
	ldr r0, [r5]
	adds r2, r2, r6
	bl Func_02002cd0
	movs r0, #64
	bl Object_GetById
	ldr r2, [r0, #16]
	ldr r1, [r0, #8]
	adds r2, r2, r6
	movs r0, #64
	bl Func_02002cd0
	movs r0, #64
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #10
	bl WaitFrames
	ldr r0, [r5]
	movs r1, #0
	bl Object_AttachWorkTargetToObject
	movs r0, #1
	bl WaitFrames
	movs r3, #6
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #2
	movs r2, #1
	movs r1, #0
	movs r0, #6
	bl Func_02002c38
	bl Func_02002c10
	movs r0, #1
	bl WaitFrames
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #228
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02002d88
	bl Func_02002c70
	add sp, #8
	pop {r5, r6, pc}
.L_02009360:
	.4byte gPartyState
.L_02009364:
	.4byte 0xfed00000
	.section .text.x02009e0c,"ax",%progbits
	.balign 4
	.global Func_02001e0c
	.thumb_func
Func_02001e0c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200a100
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	ldr r0, .L_0200a104
	bl Func_02002dc8
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009edc
	movs r0, #15
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r0, #15
	bl Object_GetById
	movs r1, #206
	movs r3, #144
	lsls r3, r3, #15
	lsls r1, r1, #18
	ldr r2, .L_0200a108
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_0200a108
	movs r0, #128
	lsls r0, r0, #2
	str r3, [r5, #20]
	adds r0, #18
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009eb0
	movs r3, #110
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #110
	movs r1, #1
	movs r2, #3
	movs r3, #1
	bl Func_02002c38
	b .L_02009ec4
.L_02009eb0:
	movs r3, #110
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #110
	movs r1, #0
	movs r2, #3
	movs r3, #1
	bl Func_02002c38
.L_02009ec4:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009edc
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
.L_02009edc:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #131
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009fbc
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009f36
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #68
	movs r1, #68
	movs r2, #50
	movs r3, #64
	bl Func_02002c30
	movs r3, #110
	movs r5, #4
	str r3, [sp, #0]
	movs r0, #102
	movs r1, #16
	movs r2, #3
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #50
	str r3, [sp, #0]
	movs r0, #50
	movs r1, #0
	movs r2, #3
	movs r3, #1
	b .L_02009fa4
.L_02009f36:
	movs r0, #15
	bl Object_GetById
	movs r1, #206
	movs r3, #144
	lsls r1, r1, #18
	ldr r2, .L_0200a108
	lsls r3, r3, #15
	bl Object_SetPositionAndResetMotion
	movs r0, #15
	bl Object_GetById
	ldr r3, .L_0200a108
	movs r1, #68
	str r3, [r0, #20]
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #72
	movs r2, #50
	movs r3, #64
	bl Func_02002c30
	movs r5, #4
	movs r0, #106
	movs r1, #16
	movs r2, #3
	movs r3, #2
	movs r6, #110
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #50
	str r3, [sp, #0]
	movs r0, #50
	movs r1, #1
	movs r2, #3
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009fac
	movs r0, #110
	movs r1, #1
	movs r2, #3
	movs r3, #1
	str r6, [sp, #0]
.L_02009fa4:
	str r5, [sp, #4]
	bl Func_02002c38
	b .L_02009fbc
.L_02009fac:
	movs r0, #110
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
.L_02009fbc:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #51
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a03a
	movs r3, #46
	str r3, [sp, #0]
	movs r5, #5
	movs r0, #42
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #45
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #47
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #43
	str r5, [sp, #4]
	bl Func_02002c38
	movs r0, #16
	bl Object_GetById
	movs r1, #186
	movs r3, #176
	lsls r3, r3, #15
	lsls r1, r1, #18
	ldr r2, .L_0200a108
	bl Object_SetPositionAndResetMotion
	movs r0, #16
	bl Object_GetById
	ldr r3, .L_0200a108
	str r3, [r0, #20]
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a040
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a040
.L_0200a03a:
	movs r0, #10
	bl Func_02001dc8
.L_0200a040:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #52
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a10c
	movs r0, #8
	bl Object_GetById
	movs r6, #47
	movs r5, #9
	adds r7, r0, #0
	movs r1, #0
	movs r0, #42
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #46
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #48
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #10
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002c38
	movs r3, #8
	str r3, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #43
	str r6, [sp, #0]
	bl Func_02002c38
	movs r0, #17
	bl Object_GetById
	movs r1, #190
	movs r3, #152
	lsls r3, r3, #16
	lsls r1, r1, #18
	ldr r2, .L_0200a108
	bl Object_SetPositionAndResetMotion
	movs r0, #17
	bl Object_GetById
	ldr r3, .L_0200a108
	str r3, [r0, #20]
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r3, #47
	bne .L_0200a0e6
	ldr r3, [r7, #16]
	asrs r3, r3, #20
	cmp r3, #8
	bne .L_0200a0e6
	ldr r0, .L_0200a104
	bl Func_02002dc8
.L_0200a0e6:
	movs r0, #137
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a112
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a112
	.2byte 0x0000
.L_0200a100:
	.4byte gPartyState
.L_0200a104:
	.4byte Data_02003010
.L_0200a108:
	.4byte 0xffe00000
.L_0200a10c:
	movs r0, #11
	bl Func_02001dc8
.L_0200a112:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #53
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a1de
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r6, #49
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	movs r5, #7
	movs r0, #42
	movs r1, #0
	movs r2, #1
	asrs r7, r3, #20
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #6
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002c38
	movs r3, #50
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #48
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #8
	str r3, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #43
	str r6, [sp, #0]
	bl Func_02002c38
	movs r0, #18
	bl Object_GetById
	movs r1, #198
	movs r3, #240
	lsls r3, r3, #15
	lsls r1, r1, #18
	ldr r2, .L_0200a3e4
	bl Object_SetPositionAndResetMotion
	movs r0, #18
	bl Object_GetById
	ldr r3, .L_0200a3e4
	str r3, [r0, #20]
	mov r3, r8
	cmp r3, #48
	bne .L_0200a1b4
	cmp r7, #7
	beq .L_0200a1be
.L_0200a1b4:
	mov r2, r8
	cmp r2, #49
	bne .L_0200a1c4
	cmp r7, #8
	bne .L_0200a1c4
.L_0200a1be:
	ldr r0, .L_0200a3e8
	bl Func_02002dc8
.L_0200a1c4:
	movs r0, #147
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a1e4
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a1e4
.L_0200a1de:
	movs r0, #12
	bl Func_02001dc8
.L_0200a1e4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #54
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a23e
	movs r3, #51
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #42
	bl Func_02002c38
	movs r0, #19
	bl Object_GetById
	movs r1, #206
	movs r3, #240
	lsls r3, r3, #15
	lsls r1, r1, #18
	ldr r2, .L_0200a3e4
	bl Object_SetPositionAndResetMotion
	movs r0, #19
	bl Object_GetById
	ldr r3, .L_0200a3e4
	str r3, [r0, #20]
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #38
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a244
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a244
.L_0200a23e:
	movs r0, #13
	bl Func_02001dc8
.L_0200a244:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #55
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2e6
	movs r6, #51
	movs r5, #9
	movs r0, #42
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #8
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002c38
	movs r3, #50
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #52
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #10
	str r3, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #43
	str r6, [sp, #0]
	bl Func_02002c38
	movs r0, #20
	bl Object_GetById
	movs r1, #206
	movs r3, #152
	lsls r3, r3, #16
	lsls r1, r1, #18
	ldr r2, .L_0200a3e4
	bl Object_SetPositionAndResetMotion
	movs r0, #20
	bl Object_GetById
	ldr r3, .L_0200a3e4
	str r3, [r0, #20]
	movs r0, #148
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a2ec
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a2ec
.L_0200a2e6:
	movs r0, #14
	bl Func_02001dc8
.L_0200a2ec:
	ldr r0, .L_0200a3ec
	bl Func_02002db8
	movs r1, #144
	ldr r0, .L_0200a3f0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a31e
	movs r0, #9
	movs r1, #15
	bl Func_02001944
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_ClearBit
	b .L_0200a3ba
.L_0200a31e:
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a338
	movs r0, #10
	movs r1, #16
	bl Func_02001944
	movs r0, #146
	b .L_0200a36e
.L_0200a338:
	movs r0, #137
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a356
	movs r0, #11
	movs r1, #17
	bl Func_02001944
	movs r0, #137
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	b .L_0200a3ba
.L_0200a356:
	movs r0, #147
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a378
	movs r0, #12
	movs r1, #18
	bl Func_02001944
	movs r0, #147
.L_0200a36e:
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_0200a3ba
.L_0200a378:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #38
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a39a
	movs r0, #13
	movs r1, #19
	bl Func_02001944
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #38
	bl GameFlag_ClearBit
	b .L_0200a3ba
.L_0200a39a:
	movs r0, #148
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a3ba
	movs r0, #14
	movs r1, #20
	bl Func_02001944
	movs r0, #148
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200a3ba:
	ldr r3, .L_0200a3f4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #26
	bne .L_0200a3da
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a3da
	bl Func_020019ac
.L_0200a3da:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a3e4:
	.4byte 0xffe00000
.L_0200a3e8:
	.4byte Data_02003010
.L_0200a3ec:
	.4byte Data_0200301e
.L_0200a3f0:
	.4byte Func_020013f0
.L_0200a3f4:
	.4byte gPartyState
