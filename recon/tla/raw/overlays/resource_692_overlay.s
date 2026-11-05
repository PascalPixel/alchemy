.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080ce,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008880,"ax",%progbits
	.global Func_02000880
	.thumb_func
Func_02000880:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	b .L_020088d0
.L_0200888a:
	ldrh r0, [r7]
	bl Actor_GetById
	movs r3, #34
	adds r6, r0, #0
	adds r3, r3, r6
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	mov r8, r3
	bl Map_GetTerrainHeight
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	adds r5, r0, #0
	movs r1, #0
	adds r0, r6, #0
	bl Actor_SetSingleChildFlags
	mov r2, r8
	ldrb r0, [r2]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	asrs r5, r5, #19
	adds r5, #4
	asrs r2, r2, #20
	adds r3, r5, #0
	asrs r1, r1, #20
	adds r7, #6
	bl Func_020005dc
.L_020088d0:
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200888a
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008ed0,"ax",%progbits
	.balign 4
	.global Func_02000ed0
	.thumb_func
Func_02000ed0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	mov r8, r0
	movs r0, #148
	adds r0, #255
	sub sp, #68
	bl Import_ActorCreate
	mov r1, r8
	adds r6, r0, #0
	ldrh r0, [r1, #6]
	bl Math_Cosine
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	lsls r0, r0, #1
	strb r3, [r2]
	movs r1, #1
	mov r10, r0
	adds r0, r6, #0
	bl Func_02004c98
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	ldr r3, [r6, #8]
	add r2, sp, #56
	str r3, [r2]
	ldr r3, [r6, #12]
	str r3, [r2, #4]
	ldr r3, [r6, #16]
	str r3, [r2, #8]
.L_02008f2e:
	add r7, sp, #56
	ldr r1, [r7]
	ldr r2, [r7, #8]
	ldr r3, [r7, #4]
	add r1, r10
	subs r2, r2, r3
	str r1, [r7]
	asrs r2, r2, #20
	asrs r1, r1, #20
	movs r0, #2
	bl Func_02000658
	asrs r0, r0, #8
	mov r11, r0
	cmp r0, #212
	beq .L_02008fc8
	cmp r0, #222
	beq .L_02008fc8
	movs r2, #8
	mov r9, r2
.L_02008f56:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	mov r0, r9
	lsls r3, r0, #2
	adds r3, #20
	ldr r5, [r2, r3]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02008fbc
	mov r1, r8
	ldr r2, [r1, #8]
	ldr r3, [r5, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008f90
	ldr r2, [r1, #12]
	ldr r3, [r5, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008f90
	ldr r2, [r1, #16]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_02008fbc
.L_02008f90:
	adds r0, r5, #0
	adds r0, #8
	adds r1, r7, #0
	bl Func_020005b0
	cmp r0, #8
	bgt .L_02008fbc
	ldr r2, [r5, #12]
	ldr r3, [r7, #4]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008fbc
	ldr r3, [r5, #80]
	movs r0, #136
	ldr r3, [r3, #40]
	lsls r0, r0, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, r0
	bne .L_020090b0
	b .L_02008fc8
.L_02008fbc:
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #64
	bne .L_02008f56
	b .L_02008f2e
.L_02008fc8:
	ldr r3, .L_02009134
	movs r5, #128
	lsls r5, r5, #10
	add r7, sp, #56
	str r5, [r6, #52]
	str r5, [r6, #48]
	ldr r2, [r7, #4]
	str r3, [r6, #108]
	ldr r1, [r7]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02004cc8
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	bl Func_02004e10
	ldr r1, [r7, #4]
	ldr r2, [r7, #8]
	movs r3, #1
	ldr r0, [r7]
	bl Camera_MoveWithinMapBounds
	adds r0, r6, #0
	bl Func_02004cd0
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	bl Func_02004cb0
	movs r3, #0
	mov r9, r3
.L_0200900c:
	ldr r3, .L_02009138
	movs r2, #1
	ldr r3, [r3]
	mov r10, r3
	mov r0, r10
	ands r0, r2
	mov r10, r0
	cmp r0, #0
	bne .L_0200906a
	add r1, sp, #16
	mov r8, r1
	movs r3, #148
	mov r0, r8
	adds r3, #255
	strh r3, [r0, #24]
	ldr r3, .L_0200913c
	str r2, [r0]
	str r3, [r0, #36]
	ldr r3, .L_02009140
	movs r6, #31
	str r3, [r0, #28]
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	ands r0, r6
	ldr r4, [r7]
	mov r3, r10
	ands r5, r6
	subs r0, #16
	ldr r1, [r7, #4]
	ldr r2, [r7, #8]
	lsls r0, r0, #13
	subs r5, #16
	str r3, [sp, #4]
	movs r3, #153
	lsls r3, r3, #17
	lsls r5, r5, #13
	str r0, [sp, #0]
	mov r0, r8
	str r3, [sp, #8]
	str r0, [sp, #12]
	adds r3, r5, #0
	adds r0, r4, #0
	bl Func_0200015c
.L_0200906a:
	movs r0, #1
	bl wait
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #16
	bne .L_0200900c
	movs r0, #24
	bl wait
	mov r3, r11
	cmp r3, #222
	bne .L_02009090
	bl Func_02001350
	bl Func_02004d48
	b .L_02009126
.L_02009090:
	ldr r3, .L_02009144
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	movs r1, #1
	bl Camera_FollowActor
	bl Func_02004e20
	movs r0, #10
	bl wait
	bl Func_02004d48
	b .L_02009126
.L_020090b0:
	ldr r3, .L_02009134
	movs r5, #128
	lsls r5, r5, #10
	str r5, [r6, #52]
	str r5, [r6, #48]
	ldr r2, [r7, #4]
	str r3, [r6, #108]
	ldr r1, [r7]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02004cc8
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	bl Func_02004e10
	ldr r2, [r7, #8]
	ldr r1, [r7, #4]
	movs r3, #1
	ldr r0, [r7]
	bl Camera_MoveWithinMapBounds
	adds r0, r6, #0
	bl Func_02004cd0
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	bl Func_02004cb0
	movs r0, #188
	lsls r0, r0, #2
	bl GameFlag_SetBit
	mov r1, r9
	lsls r0, r1, #16
	lsrs r0, r0, #16
	bl Func_02000ce4
	movs r0, #188
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	ldr r3, .L_02009144
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Camera_FollowActor
	bl Func_02004e20
	movs r0, #10
	bl wait
	bl Func_02004d48
.L_02009126:
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009134:
	.4byte Func_02000d1c
.L_02009138:
	.4byte gFrameCount
.L_0200913c:
	.4byte Func_02000ea4
.L_02009140:
	.4byte gData_0200502c
.L_02009144:
	.4byte gMainRoutineWork
	.section .text.x02009828,"ax",%progbits
	.global Func_02001828
	.thumb_func
Func_02001828:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r7, .L_02009a9c
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r7, r0
	ldr r0, [r5]
	sub sp, #92
	bl Actor_GetById
	ldrh r3, [r0, #6]
	movs r1, #128
	lsls r1, r1, #6
	adds r1, r3, r1
	movs r3, #192
	lsls r3, r3, #8
	movs r2, #0
	ands r1, r3
	str r1, [sp, #36]
	str r2, [sp, #32]
	str r2, [sp, #28]
	str r2, [sp, #24]
	mov r10, r0
	mov r11, r2
	mov r9, r2
	mov r8, r2
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	mov r3, r10
	ldr r1, [r3, #8]
	ldr r0, .L_02009aa0
	movs r3, #128
	lsls r3, r3, #12
	ands r1, r0
	add r6, sp, #80
	adds r1, r1, r3
	str r1, [r6]
	mov r4, r10
	ldr r2, [r4, #16]
	asrs r1, r1, #16
	ands r2, r0
	adds r2, r2, r3
	str r2, [r6, #8]
	asrs r2, r2, #16
	ldr r0, [r5]
	bl Actor_SetPositionAndReset
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #146
	lsls r0, r0, #2
	bl Func_02004f30
	mov r0, r10
	adds r0, #35
	str r0, [sp, #20]
	movs r3, #191
	ldrb r2, [r0]
	ands r3, r2
	strb r3, [r0]
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_02004d10
	movs r0, #107
	bl Func_02004f30
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldr r3, .L_02009aa4
	cmp r2, r3
	bne .L_02009948
	movs r0, #20
	movs r3, #26
	movs r4, #160
	str r3, [sp, #24]
	mov r9, r0
	movs r2, #84
	mov r0, r11
	lsls r4, r4, #17
	movs r3, #212
	str r2, [sp, #28]
	str r4, [sp, #32]
	lsls r3, r3, #17
	str r0, [r6, #4]
	movs r0, #192
	movs r1, #89
	str r3, [r6, #8]
	lsls r0, r0, #2
	mov r8, r1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009914
	movs r1, #212
	mov r11, r1
.L_02009914:
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r2, #0
	str r2, [sp, #36]
	movs r1, #15
	movs r2, #13
	movs r3, #64
	movs r0, #0
	bl Func_02000694
	movs r0, #15
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r6, #0
.L_02009936:
	adds r0, r6, #0
	adds r0, #9
	movs r1, #1
	adds r6, #1
	bl Actor_SetModeById
	cmp r6, #7
	bne .L_02009936
	b .L_02009a46
.L_02009948:
	ldr r1, .L_02009a9c
	movs r4, #240
	lsls r4, r4, #1
	adds r3, r1, r4
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009aa8
	cmp r2, r3
	bne .L_020099b8
	movs r3, #92
	movs r0, #224
	str r3, [sp, #28]
	lsls r0, r0, #17
	movs r3, #0
	movs r4, #21
	str r0, [sp, #32]
	str r4, [sp, #24]
	movs r0, #192
	str r3, [r6, #4]
	movs r3, #172
	lsls r3, r3, #17
	lsls r0, r0, #2
	movs r1, #28
	movs r2, #83
	str r3, [r6, #8]
	adds r0, #1
	mov r9, r1
	mov r8, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200998c
	movs r1, #212
	mov r11, r1
.L_0200998c:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_ClearBit
	movs r1, #24
	movs r2, #15
	movs r3, #64
	movs r0, #0
	bl Func_02000694
	movs r1, #9
	movs r0, #0
	movs r2, #16
	movs r3, #64
	bl Func_02000694
	movs r0, #11
	movs r1, #4
	bl Actor_SetModeById
	b .L_02009a46
.L_020099b8:
	ldr r3, .L_02009aac
	cmp r2, r3
	bne .L_02009a46
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #8
	bgt .L_02009a0a
	movs r3, #13
	movs r4, #198
	str r3, [sp, #24]
	movs r0, #49
	movs r3, #0
	movs r2, #113
	lsls r4, r4, #18
	str r2, [sp, #28]
	str r4, [sp, #32]
	mov r9, r0
	str r3, [r6, #4]
	movs r0, #192
	movs r3, #216
	lsls r3, r3, #16
	lsls r0, r0, #2
	movs r1, #75
	str r3, [r6, #8]
	adds r0, #2
	mov r8, r1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020099fe
	movs r0, #212
	mov r11, r0
.L_020099fe:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
	b .L_02009a46
.L_02009a0a:
	movs r3, #115
	movs r0, #206
	str r3, [sp, #28]
	lsls r0, r0, #18
	movs r3, #0
	movs r4, #42
	str r0, [sp, #32]
	str r4, [sp, #24]
	movs r0, #192
	str r3, [r6, #4]
	movs r3, #170
	lsls r3, r3, #18
	lsls r0, r0, #2
	movs r1, #51
	movs r2, #104
	str r3, [r6, #8]
	adds r0, #2
	mov r9, r1
	mov r8, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009a3c
	movs r1, #212
	mov r11, r1
.L_02009a3c:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
.L_02009a46:
	movs r0, #181
	bl Func_02004f30
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004d10
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004d10
	mov r2, r8
	subs r2, #1
	str r2, [sp, #16]
	movs r3, #3
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	ldr r3, [sp, #16]
	mov r0, r9
	mov r1, r8
	mov r2, r9
	bl Func_02004cd8
	mov r3, r11
	cmp r3, #0
	bne .L_02009ab0
	bl Func_02004d18
	movs r0, #20
	bl FieldEvent_Wait
	b .L_02009b90
	.2byte 0x0000
.L_02009a9c:
	.4byte gMainRoutineWork
.L_02009aa0:
	.4byte 0xfff00000
.L_02009aa4:
	.4byte 0x000000e3
.L_02009aa8:
	.4byte 0x000000e4
.L_02009aac:
	.4byte 0x000000e6
.L_02009ab0:
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_02004d10
	movs r3, #148
	add r7, sp, #40
	adds r3, #255
	strh r3, [r7, #24]
	movs r3, #2
	str r3, [r7]
	movs r3, #10
	str r3, [r7, #4]
	ldr r3, .L_02009b78
	movs r4, #0
	str r3, [r7, #28]
	mov r8, r4
.L_02009ad8:
	movs r6, #0
.L_02009ada:
	bl Random16Far
	movs r1, #48
	add r5, sp, #80
	bl __umodsi3
	mov r2, r8
	lsls r3, r2, #18
	ldr r2, [r5, #8]
	ldr r1, [sp, #32]
	adds r2, r2, r3
	movs r3, #184
	lsls r3, r3, #13
	adds r2, r2, r3
	lsls r0, r0, #16
	movs r3, #128
	adds r0, r1, r0
	lsls r3, r3, #9
	str r0, [r5]
	ldr r1, [r5, #4]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #204
	lsls r3, r3, #14
	str r3, [sp, #8]
	adds r6, #1
	movs r3, #0
	str r7, [sp, #12]
	bl Func_0200015c
	cmp r6, #6
	bne .L_02009ada
	movs r0, #3
	bl wait
	mov r4, r8
	lsrs r3, r4, #1
	movs r2, #3
	adds r3, #1
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #75
	movs r1, #64
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	bl Func_02004cd8
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #10
	bne .L_02009ad8
	movs r6, #0
.L_02009b46:
	ldr r3, .L_02009b70
	lsrs r1, r6, #2
	movs r0, #128
	subs r3, r3, r1
	lsls r2, r1, #8
	lsls r0, r0, #19
	orrs r2, r3
	adds r0, #82
	strh r2, [r0]
	ldr r3, .L_02009b74
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r0, #3
	adds r6, #1
	bl wait
	cmp r6, #64
	bne .L_02009b46
	b .L_02009b7c
.L_02009b70:
	.4byte 0x00000010
.L_02009b74:
	.4byte 0x00003f44
.L_02009b78:
	.4byte gData_0200502c
.L_02009b7c:
	movs r3, #3
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #78
	movs r1, #64
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	bl Func_02004cd8
.L_02009b90:
	ldr r5, .L_02009c94
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldr r3, .L_02009c98
	cmp r2, r3
	bne .L_02009bb2
	movs r0, #15
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_02009bb2:
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02004f30
	movs r0, #181
	bl Func_02004f30
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004d10
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004d10
	movs r3, #3
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	ldr r3, [sp, #16]
	movs r1, #63
	mov r2, r9
	movs r0, #72
	bl Func_02004cd8
	bl Func_02004d18
	movs r0, #146
	lsls r0, r0, #2
	bl Func_02004f30
	ldr r0, [sp, #20]
	movs r3, #64
	ldrb r2, [r0]
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #133
	lsls r1, r1, #2
	adds r6, r5, r1
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	mov r2, r10
	ldr r3, [r2, #8]
	ldr r1, .L_02009c9c
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	add r5, sp, #80
	adds r3, r3, r2
	str r3, [r5]
	mov r4, r10
	ldr r3, [r4, #12]
	str r3, [r5, #4]
	ldr r3, [r4, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r0, [sp, #36]
	movs r1, #128
	lsls r1, r1, #8
	eors r1, r0
	movs r0, #128
	adds r2, r5, #0
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
	add r1, sp, #36
	ldrh r1, [r1]
	mov r2, r10
	strh r1, [r2, #6]
	movs r2, #2
	ldrsh r1, [r5, r2]
	ldr r0, [r6]
	movs r3, #10
	ldrsh r2, [r5, r3]
	bl Actor_SetPositionAndReset
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	bl Func_02004d48
	add sp, #92
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c94:
	.4byte gMainRoutineWork
.L_02009c98:
	.4byte 0x000000e3
.L_02009c9c:
	.4byte 0xfff00000
	.section .text.x0200c2f8,"ax",%progbits
	.balign 4
	.global Func_020042f8
	.thumb_func
Func_020042f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #16
	ldr r5, .L_0200c454
	str r3, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r11, r0
	ldr r1, [r5]
	movs r0, #8
	bl Func_02004da8
	ldr r1, [r5]
	movs r0, #9
	bl Func_02004da8
	ldr r1, [r5]
	movs r0, #10
	bl Func_02004da8
	movs r0, #1
	bl wait
	movs r0, #8
	bl Func_020042e0
	movs r0, #9
	bl Func_020042e0
	movs r0, #10
	bl Func_020042e0
	movs r1, #0
	movs r0, #9
	bl Actor_SetModeById
	movs r0, #1
	bl wait
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_02004da0
	movs r0, #1
	bl wait
	b .L_0200c432
.L_0200c37e:
	mov r3, r11
	ldrh r3, [r3]
	mov r9, r3
	mov r0, r9
	bl Actor_GetById
	mov r2, r11
	ldrh r2, [r2, #2]
	adds r5, r0, #0
	str r2, [sp, #8]
	adds r7, r5, #0
	adds r7, #34
	adds r0, r2, #0
	ldrb r2, [r7]
	adds r0, #1
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #12]
	ldr r6, [r2, r3]
	ldr r2, .L_0200c458
	adds r3, r6, r2
	ldr r2, .L_0200c45c
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	adds r2, r5, #0
	adds r2, #92
	movs r3, #3
	strb r3, [r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c3d4
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
	b .L_0200c42e
.L_0200c3d4:
	adds r0, r5, #0
	bl Func_02004ed0
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r6, r6, r3
	str r6, [sp, #4]
	add r8, r10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r7]
	bl Func_02004d20
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	mov r10, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	mov r0, r9
	bl Func_02004df8
	ldr r6, [sp, #4]
	asrs r5, r5, #19
	movs r3, #128
	adds r5, #4
	adds r2, r5, #0
	strb r3, [r6, #3]
	ldrb r0, [r7]
	mov r1, r10
	bl Func_02004ef0
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c42e
	mov r0, r9
	movs r1, #9
	bl ObjectVisual_CopyAttributes
.L_0200c42e:
	movs r3, #4
	add r11, r3
.L_0200c432:
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200c37e
	movs r0, #10
	bl wait
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c454:
	.4byte gMainRoutineWork
.L_0200c458:
	.4byte 0xfdff0000
.L_0200c45c:
	.4byte gMapShapeGrid
	.section .text.x0200cb18,"ax",%progbits
	.balign 4
	.global Func_02004b18
	.thumb_func
Func_02004b18:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #176
	lsls r1, r1, #5
	adds r1, #8
	movs r0, #220
	sub sp, #4
	bl HeapSlot_AllocFar
	adds r6, r0, #0
	ldr r0, .L_0200cbcc
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_02004c58
	bl Resource_FindFreeEntry
	movs r1, #160
	lsls r1, r1, #3
	adds r2, r6, #0
	adds r5, r0, #0
	bl VramCache_Load
	movs r1, #176
	lsls r1, r1, #5
	adds r1, #2
	mov r10, r0
	adds r3, r6, r1
	mov r2, r10
	adds r1, #2
	strh r2, [r3]
	adds r3, r6, r1
	strh r5, [r3]
	movs r2, #160
	movs r3, #192
	lsls r2, r2, #3
	lsls r3, r3, #4
	movs r1, #63
	adds r7, r6, r2
	adds r5, r6, r3
	mov r8, r1
.L_0200cb70:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_02004f10
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
	bge .L_0200cb70
	movs r1, #176
	lsls r1, r1, #5
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200cbd0
	bl Task_Add
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200cbcc:
	.4byte 0x000001f0
.L_0200cbd0:
	.4byte Func_020049d8
	.section .bss,"aw",%nobits
	.global Data_020067ec
Data_020067ec:
	.space 0x00000004
	.global Data_020067f0
Data_020067f0:
	.space 0x00000004
	.global Data_020067f4
Data_020067f4:
	.space 0x00000004
	.global Data_020067f8
Data_020067f8:
