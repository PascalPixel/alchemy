.syntax unified
	.thumb
	.section .text.x020089d4,"ax",%progbits
	.global Func_020009d4
	.thumb_func
Func_020009d4:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r0, #9
	bl Actor_GetById
	mov r9, r0
	movs r0, #64
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r6, r0, #0
	mov r10, r3
	bl Func_0200119c
	movs r0, #0
	bl Func_0200125c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02001244
	movs r1, #1
	ldr r0, .L_02008d7c
	bl Func_0200123c
	movs r0, #60
	bl Func_0200124c
	movs r0, #168
	movs r1, #1
	movs r2, #158
	lsls r2, r2, #16
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	movs r0, #26
	bl Func_02001284
	movs r0, #60
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #8
	bl Actor_SetModeById
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02001284
	movs r0, #60
	bl FieldEvent_Wait
	ldr r0, .L_02008d80
	bl Func_020011e4
	movs r2, #226
	lsls r2, r2, #1
	add r10, r2
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	movs r1, #0
	negs r2, r2
	bl Func_0200118c
	movs r1, #168
	movs r2, #152
	lsls r1, r1, #16
	movs r0, #64
	lsls r2, r2, #16
	bl Func_020011cc
	adds r2, r6, #0
	movs r3, #0
	mov r8, r3
	adds r2, #85
	movs r3, #6
	strb r3, [r2]
	movs r2, #128
	ldr r3, [r6, #20]
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r6, #20]
	movs r0, #220
	bl Func_02001284
	movs r0, #206
	movs r2, #0
	movs r3, #0
	movs r1, #0
	lsls r0, r0, #1
	bl Func_0200115c
	ldr r1, .L_02008d84
	adds r5, r0, #0
	bl Func_02001154
	adds r0, r5, #0
	movs r1, #1
	bl Animation_ApplyChildValues
	adds r3, r5, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	movs r2, #128
	ldr r3, [r6, #8]
	lsls r2, r2, #12
	str r3, [r5, #8]
	movs r0, #15
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r6, #16]
	str r3, [r5, #16]
	ldr r3, [r6, #20]
	movs r6, #128
	str r3, [r5, #20]
	ldr r3, .L_02008d88
	lsls r6, r6, #9
	str r3, [r5, #108]
	bl FieldEvent_Wait
	mov r3, r8
	str r3, [r5, #8]
	str r3, [r5, #12]
	str r3, [r5, #16]
	mov r2, r10
	ldrh r0, [r2]
	str r3, [r5, #108]
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	movs r1, #0
	negs r2, r2
	bl Func_0200118c
	ldr r3, .L_02008d8c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #166
	movs r2, #188
	ldr r0, [r5]
	bl Actor_ResetAndSetPosition
	ldr r0, [r5]
	bl Actor_WaitForMoveById
	ldr r1, [r5]
	movs r0, #9
	bl Func_020011d4
	mov r3, r9
	str r6, [r3, #48]
	movs r1, #184
	movs r2, #188
	movs r0, #9
	bl Actor_ResetAndSetPosition
	movs r0, #9
	bl Actor_WaitForMoveById
	movs r1, #128
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r0, #9
	movs r1, #0
	bl Func_020011ec
	movs r2, #0
	ldr r0, [r5]
	movs r1, #0
	bl Actor_ArmCallback
	ldr r0, [r5]
	movs r1, #3
	bl Actor_SetModeById
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	mov r2, r10
	ldrh r0, [r2]
	movs r1, #0
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	negs r2, r2
	bl Func_0200118c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl Func_020011fc
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	bl Func_020011ec
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	movs r1, #0
	negs r2, r2
	bl Func_0200118c
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_020011fc
	movs r0, #30
	bl FieldEvent_Wait
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	movs r1, #0
	negs r2, r2
	bl Func_0200118c
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #9
	bl Func_020011fc
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	bl Func_020011ec
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	movs r1, #0
	negs r2, r2
	bl Func_0200118c
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r5]
	bl Func_020011fc
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_020011fc
	movs r0, #60
	bl FieldEvent_Wait
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	movs r1, #0
	negs r2, r2
	bl Func_0200118c
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #6
	adds r1, #255
	movs r2, #0
	ldr r0, [r5]
	bl Func_020011fc
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_020011fc
	movs r0, #60
	bl FieldEvent_Wait
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	negs r2, r2
	movs r1, #0
	asrs r0, r0, #16
	bl Func_0200118c
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #3
	ldr r0, [r5]
	bl Actor_SetModeById
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_020011ec
	movs r0, #8
	movs r1, #2
	bl Actor_SetModeById
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	negs r2, r2
	movs r1, #0
	asrs r0, r0, #16
	bl Func_0200118c
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #8
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #9
	movs r1, #2
	bl Actor_SetModeById
	ldr r0, [r5]
	bl Actor_GetById
	cmp r0, #0
	beq .L_02008d2e
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl Actor_ResetAndSetPosition
.L_02008d2e:
	movs r0, #9
	bl Actor_WaitForMoveById
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl Func_020011cc
	movs r0, #9
	bl Actor_WaitForMoveById
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #206
	bl GameFlag_SetBit
	bl Func_02001264
	adds r0, r6, #0
	movs r1, #0
	bl Func_0200123c
	movs r0, #60
	bl Func_0200124c
	ldr r0, [r5]
	movs r1, #1
	bl Func_02001214
	movs r0, #60
	bl FieldEvent_Wait
	bl Func_020011a4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_02008d7c:
	.4byte 0x00403108
.L_02008d80:
	.4byte MsgByReachingThis
.L_02008d84:
	.4byte gData_020012a4
.L_02008d88:
	.4byte Func_020009bc
.L_02008d8c:
	.4byte gAdventure
	.section .text.x02008ecc,"ax",%progbits
	.global Func_02000ecc
	.thumb_func
Func_02000ecc:
	.ifdef TLA_EDITION_JA
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	movs r2, #129
	lsls r0, r0, #1
	lsls r2, r2, #1
	adds r3, r3, r0
	adds r2, #255
	str r2, [r3, #0]
	movs r0, #0
	sub sp, #8
	bl Func_02001254
	ldr r5, .L_ja_0200911c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_ja_02009120
	cmp r2, r3
	bne .L_ja_02008f50
	adds r1, #2
	adds r3, r5, r1
	movs r2, #0
	ldrsh r6, [r3, r2]
	cmp r6, #1
	bne .L_ja_02008f50
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008f2c
	ldr r3, .L_ja_02009124
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r2, r5, r0
	adds r1, #98
	strh r3, [r2, #0]
	adds r3, r5, r1
	strh r6, [r3, #0]
	b .L_ja_02008f50
.L_ja_02008f2c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008f50
	ldr r2, .L_ja_02009128
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r3, r5, r0
	adds r1, #98
	strh r2, [r3, #0]
	adds r2, r5, r1
	movs r3, #3
	strh r3, [r2, #0]
.L_ja_02008f50:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_02008f86
	ldr r1, .L_ja_0200911c
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_ja_0200912c
	cmp r2, r3
	bne .L_ja_02008f86
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #2
	ble .L_ja_02008f86
	adds r2, #50
	adds r3, r1, r2
	ldr r0, [r3, #0]
	bl Func_0200126c
.L_ja_02008f86:
	ldr r3, .L_ja_0200911c
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_ja_02009130
	cmp r2, r3
	bne .L_ja_02009080
	adds r0, #32
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008fb6
	movs r3, #8
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #6
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_ja_02008fb6:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008fd8
	movs r3, #8
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_ja_02008fd8:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008ffa
	movs r3, #9
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_ja_02008ffa:
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200901c
	movs r3, #8
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_ja_0200901c:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200903c
	movs r3, #6
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_ja_0200903c:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200905e
	movs r3, #5
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_ja_0200905e:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009080
	movs r3, #6
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_ja_02009080:
	ldr r3, .L_ja_0200911c
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_ja_02009134
	cmp r2, r3
	bne .L_ja_020090b2
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020090b2
	movs r3, #17
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #18
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_ja_020090b2:
	ldr r3, .L_ja_0200911c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_ja_02009138
	cmp r2, r3
	bne .L_ja_02009114
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #206
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009114
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #147
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_02009114
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_02009114
	movs r0, #64
	bl Actor_GetById
	movs r1, #168
	movs r2, #152
	adds r5, r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #64
	bl Func_020011cc
	adds r2, r5, #0
	movs r3, #6
	adds r2, #85
	strb r3, [r2, #0]
	movs r1, #128
	ldr r3, [r5, #20]
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r5, #20]
.L_ja_02009114:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_ja_0200911c:
	.4byte gAdventure
.L_ja_02009120:
	.4byte 0x0000007e
.L_ja_02009124:
	.4byte 0x0000007b
.L_ja_02009128:
	.4byte 0x00000078
.L_ja_0200912c:
	.4byte 0x0000007f
.L_ja_02009130:
	.4byte 0x00000080
.L_ja_02009134:
	.4byte 0x00000082
.L_ja_02009138:
	.4byte 0x00000081
	.else
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	movs r0, #0
	sub sp, #8
	bl Func_02001254
	ldr r5, .L_02009118
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200911c
	cmp r2, r3
	bne .L_02008f4e
	adds r1, #2
	adds r3, r5, r1
	movs r2, #0
	ldrsh r6, [r3, r2]
	cmp r6, #1
	bne .L_02008f4e
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f2a
	ldr r3, .L_02009120
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r2, r5, r0
	adds r1, #98
	strh r3, [r2]
	adds r3, r5, r1
	strh r6, [r3]
	b .L_02008f4e
.L_02008f2a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f4e
	ldr r2, .L_02009124
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r3, r5, r0
	adds r1, #98
	strh r2, [r3]
	adds r2, r5, r1
	movs r3, #3
	strh r3, [r2]
.L_02008f4e:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008f84
	ldr r1, .L_02009118
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009128
	cmp r2, r3
	bne .L_02008f84
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #2
	ble .L_02008f84
	adds r2, #50
	adds r3, r1, r2
	ldr r0, [r3]
	bl Func_0200126c
.L_02008f84:
	ldr r3, .L_02009118
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200912c
	cmp r2, r3
	bne .L_0200907e
	adds r0, #32
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008fb4
	movs r3, #8
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #6
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_02008fb4:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008fd6
	movs r3, #8
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_02008fd6:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ff8
	movs r3, #9
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_02008ff8:
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200901a
	movs r3, #8
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_0200901a:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200903a
	movs r3, #6
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_0200903a:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200905c
	movs r3, #5
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_0200905c:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200907e
	movs r3, #6
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_0200907e:
	ldr r3, .L_02009118
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009130
	cmp r2, r3
	bne .L_020090b0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090b0
	movs r3, #17
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #18
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_020090b0:
	ldr r3, .L_02009118
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009134
	cmp r2, r3
	bne .L_02009112
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #206
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009112
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #147
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009112
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009112
	movs r0, #64
	bl Actor_GetById
	movs r1, #168
	movs r2, #152
	adds r5, r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #64
	bl Func_020011cc
	adds r2, r5, #0
	movs r3, #6
	adds r2, #85
	strb r3, [r2]
	movs r1, #128
	ldr r3, [r5, #20]
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r5, #20]
.L_02009112:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
.L_02009118:
	.4byte gAdventure
.L_0200911c:
	.4byte 0x0000007e
.L_02009120:
	.4byte 0x0000007b
.L_02009124:
	.4byte 0x00000078
.L_02009128:
	.4byte 0x0000007f
.L_0200912c:
	.4byte 0x00000080
.L_02009130:
	.4byte 0x00000082
.L_02009134:
	.4byte 0x00000081
	.endif
