.syntax unified
	.thumb
	.section .text.x020081a0,"ax",%progbits
	.balign 4
	.global Func_020001a0
	.thumb_func
Func_020001a0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #136
	mov r8, r1
	cmp r1, #14
	bne .L_0200824c
	movs r0, #14
	bl Actor_GetById
	adds r7, r0, #0
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008244
	ldr r3, [r7, #8]
	asrs r6, r3, #20
	cmp r6, #47
	bne .L_02008244
	ldr r3, [r7, #16]
	asrs r5, r3, #20
	cmp r5, #28
	bne .L_02008244
	movs r0, #14
	bl Func_0200015c
	movs r3, #1
	movs r2, #1
	movs r1, #28
	movs r0, #49
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl Func_0200106c
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #14
	movs r1, #2
	bl Actor_SetSpritePriority
	movs r0, #10
	bl Actor_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #27
	bne .L_02008218
	movs r0, #2
	adds r0, #255
	bl Func_0200119c
	movs r3, #220
	b .L_02008228
.L_02008218:
	subs r3, #28
	cmp r3, #1
	bhi .L_0200822c
	movs r0, #129
	lsls r0, r0, #1
	bl Func_0200119c
	movs r3, #236
.L_02008228:
	lsls r3, r3, #17
	str r3, [r5, #16]
.L_0200822c:
	ldr r2, [r7, #16]
	ldr r3, [r5, #16]
	cmp r2, r3
	bge .L_02008238
	movs r0, #129
	b .L_0200823a
.L_02008238:
	movs r0, #130
.L_0200823a:
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_0200824c
.L_02008244:
	mov r0, r8
	movs r1, #1
	bl Actor_SetSpritePriority
.L_0200824c:
	mov r2, r8
	cmp r2, #15
	beq .L_02008254
	b .L_0200835a
.L_02008254:
	movs r0, #15
	bl Actor_GetById
	adds r6, r0, #0
	movs r0, #137
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200835a
	ldr r3, [r6, #8]
	asrs r3, r3, #20
	cmp r3, #53
	bne .L_0200835a
	ldr r3, [r6, #16]
	asrs r3, r3, #20
	cmp r3, #36
	bne .L_0200835a
	movs r0, #15
	bl Func_0200015c
	movs r1, #2
	movs r0, #15
	bl Actor_SetSpritePriority
	movs r0, #9
	bl Actor_GetById
	adds r7, r0, #0
	ldr r1, [r6, #8]
	ldr r3, [r7, #8]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_020082a2
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_020082ac
	b .L_020082e8
.L_020082a2:
	movs r2, #128
	subs r3, r3, r1
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_020082e8
.L_020082ac:
	adds r3, r7, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	adds r0, r7, #0
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	bl Func_02001054
	adds r0, r7, #0
	bl Func_0200105c
	ldr r1, .L_02008364
	adds r0, r6, #0
	bl Func_02001044
	adds r0, r7, #0
	bl Func_0200104c
	movs r0, #128
	str r5, [r7, #8]
	str r5, [r7, #16]
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_0200835a
.L_020082e8:
	add r0, sp, #8
	adds r1, r7, #0
	movs r2, #128
	ldr r5, .L_02008368
	mov lr, r5
	.2byte 0xf800
	movs r0, #9
	bl Actor_ResetTargetsAndVelocity
	movs r1, #4
	movs r2, #0
	movs r0, #9
	bl Actor_Jump
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_0200111c
	movs r2, #0
	movs r1, #15
	movs r0, #9
	bl Actor_SetAngleToward
	movs r0, #60
	bl FieldEvent_Wait
	movs r1, #4
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #128
	add r1, sp, #8
	adds r0, r7, #0
	mov lr, r5
	.2byte 0xf800
	movs r3, #53
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #53
	movs r1, #35
	movs r2, #1
	movs r3, #1
	bl Func_0200106c
	movs r0, #137
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_0200835a:
	add sp, #136
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008364:
	.4byte gData_02001404
.L_02008368:
	.4byte IwramCopyWords
	.section .text.x0200863c,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_0200063c
	.thumb_func
Func_0200063c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #129
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	sub sp, #8
	bl Func_02001184
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	ldr r7, .L_ja_02008954
	orrs r3, r2
	movs r2, #240
	lsls r2, r2, #1
	strb r3, [r0]
	adds r3, r7, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_ja_02008958
	movs r6, #0
	cmp r2, r3
	beq .L_ja_0200867c
	b .L_ja_020087d8
.L_ja_0200867c:
	movs r0, #18
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r1, #1
	movs r0, #19
	bl Actor_SetSpritePriority
	movs r0, #19
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #19
	bl Actor_GetById
	adds r0, #89
	strb r6, [r0]
	movs r0, #19
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #12
	bl Actor_GetById
	adds r0, #89
	strb r6, [r0]
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020086f0
	movs r3, #34
	movs r5, #28
	str r3, [sp, #4]
	movs r0, #30
	movs r1, #35
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_0200106c
	movs r3, #33
	str r3, [sp, #4]
	movs r0, #30
	movs r1, #34
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_0200106c
.L_ja_020086f0:
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008712
	movs r3, #22
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #24
	movs r1, #32
	movs r2, #1
	movs r3, #1
	bl Func_0200106c
.L_ja_02008712:
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008736
	movs r3, #47
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #28
	movs r2, #1
	movs r3, #1
	bl Func_0200106c
	b .L_ja_0200873e
.L_ja_02008736:
	movs r0, #14
	movs r1, #1
	bl Actor_SetSpritePriority
.L_ja_0200873e:
	movs r0, #137
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200875e
	movs r3, #53
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #53
	movs r1, #35
	movs r2, #1
	movs r3, #1
	bl Func_0200106c
.L_ja_0200875e:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_ja_0200879e
	movs r0, #16
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	movs r0, #17
	bl Actor_GetById
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #64
	orrs r3, r2
	strb r3, [r1]
	movs r3, #224
	lsls r3, r3, #16
	str r3, [r0, #12]
	str r3, [r0, #20]
	adds r0, #89
	strb r5, [r0]
	b .L_ja_020087a8
.L_ja_0200879e:
	movs r1, #2
	movs r0, #10
	adds r1, #255
	bl Func_02001124
.L_ja_020087a8:
	movs r0, #17
	movs r1, #231
	bl Func_0200118c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #130
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020087ca
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_020010d4
	b .L_ja_0200894c
.L_ja_020087ca:
	movs r0, #13
	bl Actor_GetById
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r0, #28]
	b .L_ja_0200894c
.L_ja_020087d8:
	ldr r3, .L_ja_0200895c
	cmp r2, r3
	beq .L_ja_020087e0
	b .L_ja_0200894c
.L_ja_020087e0:
	movs r0, #8
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #9
	movs r1, #1
	bl Actor_SetSpritePriority
	bl Func_02001174
	movs r1, #144
	lsls r1, r1, #4
	movs r0, #0
	adds r1, #232
	movs r2, #8
	movs r3, #9
	bl Func_0200117c
	movs r0, #132
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008824
	movs r3, #4
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #6
	movs r1, #16
	movs r2, #1
	movs r3, #1
	bl Func_0200106c
.L_ja_02008824:
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008846
	movs r3, #10
	movs r2, #30
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #8
	movs r1, #30
	movs r2, #1
	movs r3, #1
	bl Func_0200106c
.L_ja_02008846:
	movs r0, #253
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008892
	movs r0, #20
	bl Actor_GetById
	movs r1, #144
	movs r2, #228
	adds r5, r0, #0
	lsls r1, r1, #15
	movs r0, #20
	lsls r2, r2, #17
	bl Func_020010d4
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	movs r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	movs r3, #4
	movs r2, #28
	str r0, [r5, #20]
	str r0, [r5, #12]
	movs r1, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #3
	movs r2, #1
	movs r3, #1
	bl Func_0200106c
.L_ja_02008892:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #232
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020088de
	movs r0, #13
	bl Actor_GetById
	movs r1, #228
	movs r2, #244
	adds r5, r0, #0
	lsls r1, r1, #17
	movs r0, #13
	lsls r2, r2, #17
	bl Func_020010d4
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	movs r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	movs r3, #28
	movs r2, #30
	str r0, [r5, #20]
	str r0, [r5, #12]
	movs r1, #30
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r2, #1
	movs r3, #2
	bl Func_0200106c
.L_ja_020088de:
	movs r3, #112
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #8
	movs r1, #15
	movs r2, #16
	movs r3, #4
	bl Func_0200106c
	movs r3, #120
	movs r2, #33
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #2
	movs r0, #20
	movs r1, #33
	movs r2, #8
	bl Func_0200106c
	movs r0, #14
	movs r1, #8
	movs r2, #17
	bl Func_020004f4
	movs r0, #15
	movs r1, #8
	movs r2, #17
	bl Func_020004f4
	movs r0, #16
	movs r1, #22
	movs r2, #33
	bl Func_020004f4
	movs r0, #17
	movs r1, #22
	movs r2, #33
	bl Func_020004f4
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200894c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #8
	bne .L_ja_0200894c
	bl Func_02000af0
.L_ja_0200894c:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_02008954:
	.4byte gMainRoutineWork
.L_ja_02008958:
	.4byte 0x0000006b
.L_ja_0200895c:
	.4byte 0x0000006c
	.endif
