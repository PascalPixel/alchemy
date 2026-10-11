.syntax unified
	.thumb
	.section .text.x02008058,"ax",%progbits
	.global Func_02000058
	.thumb_func
Func_02000058:
	push {r5, r6, lr}
	adds r2, r0, #0
	adds r5, r2, #0
	adds r5, #98
	ldrb r3, [r5]
	movs r0, #63
	adds r1, r2, #0
	ands r0, r3
	adds r1, #85
	movs r3, #3
	ldr r6, [r2, #80]
	strb r3, [r1]
	cmp r0, #0
	bne .L_0200807a
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r2, #40]
.L_0200807a:
	cmp r0, #16
	bne .L_02008084
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2, #40]
.L_02008084:
	cmp r0, #24
	bne .L_0200808e
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r2, #40]
.L_0200808e:
	lsls r0, r0, #12
	bl Math_Cosine
	cmp r0, #0
	bge .L_0200809a
	adds r0, #63
.L_0200809a:
	asrs r3, r0, #6
	strh r3, [r6, #18]
	ldrb r3, [r5]
	movs r0, #1
	adds r3, #1
	strb r3, [r5]
	negs r0, r0
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020080ac,"ax",%progbits
	.global Func_020000ac
	.thumb_func
Func_020000ac:
	push {lr}
	ldr r3, .L_020080d4
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020080d8
	cmp r2, r3
	bne .L_020080c4
	ldr r0, .L_020080dc
	b .L_020080d0
.L_020080c4:
	ldr r3, .L_020080e0
	cmp r2, r3
	bne .L_020080ce
	ldr r0, .L_020080e4
	b .L_020080d0
.L_020080ce:
	ldr r0, .L_020080e8
.L_020080d0:
	pop {pc}
	.2byte 0x0000
.L_020080d4:
	.4byte gPartyState
.L_020080d8:
	.4byte 0x0000006b
.L_020080dc:
	.4byte Data_020015f0
.L_020080e0:
	.4byte 0x0000006c
.L_020080e4:
	.4byte Data_020018c8
.L_020080e8:
	.4byte Data_02001294
	.section .text.x020080ec,"ax",%progbits
	.global Func_020000ec
	.thumb_func
Func_020000ec:
	push {lr}
	movs r0, #11
	sub sp, #8
	bl Object_GetById
	adds r2, r0, #0
	movs r3, #0
	adds r2, #35
	adds r0, #85
	strb r3, [r2]
	strb r3, [r0]
	movs r2, #32
	movs r3, #22
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #32
	movs r2, #1
	movs r3, #1
	movs r0, #24
	bl Func_0200106c
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.section .text.x02008124,"ax",%progbits
	.global Func_02000124
	.thumb_func
Func_02000124:
	push {r5, lr}
	sub sp, #8
	movs r3, #34
	str r3, [sp, #4]
	movs r5, #28
	movs r0, #30
	movs r1, #35
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_0200106c
	movs r3, #33
	str r3, [sp, #4]
	movs r1, #34
	movs r2, #1
	movs r3, #1
	movs r0, #30
	str r5, [sp, #0]
	bl Func_0200106c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_SetBit
	add sp, #8
	pop {r5, pc}
	.section .text.x0200815c,"ax",%progbits
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {r5, r6, r7, lr}
	bl Object_GetById
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #85
	movs r3, #3
	movs r6, #60
	strb r3, [r7]
	b .L_02008172
.L_02008170:
	subs r6, #1
.L_02008172:
	cmp r6, #0
	beq .L_02008182
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5, #40]
	cmp r3, #0
	bne .L_02008170
.L_02008182:
	movs r0, #10
	bl WaitFrames
	movs r3, #0
	strb r3, [r7]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020081a0,"ax",%progbits
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
	bl Object_GetById
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
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	bl Object_GetById
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
	bl ObjectMotion_SetActionVariant
.L_0200824c:
	mov r2, r8
	cmp r2, #15
	beq .L_02008254
	b .L_0200835a
.L_02008254:
	movs r0, #15
	bl Object_GetById
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
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	bl Object_GetById
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
	bl ObjectMotion_ResetTargetsAndVelocity
	movs r1, #4
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_0200111c
	movs r2, #0
	movs r1, #15
	movs r0, #9
	bl ObjectMotion_SetAngleToward
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
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
	.4byte Data_02001404
.L_02008368:
	.4byte IwramCopyWords
	.section .text.x0200836c,"ax",%progbits
	.global Func_0200036c
	.thumb_func
Func_0200036c:
	push {lr}
	bl Func_02001084
	movs r0, #0
	bl Func_02001164
	movs r1, #191
	movs r2, #192
	movs r0, #17
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPosition
	movs r1, #128
	lsls r1, r1, #2
	movs r2, #190
	lsls r2, r2, #1
	adds r1, #234
	movs r0, #16
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #17
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #17
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008420
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008420
	movs r0, #10
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #191
	movs r2, #194
	movs r0, #10
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #191
	movs r2, #188
	movs r0, #17
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPosition
	movs r1, #191
	movs r2, #190
	lsls r2, r2, #1
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r1, #2
	movs r0, #10
	adds r1, #255
	bl Func_02001124
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
.L_02008420:
	bl Func_0200108c
	pop {pc}
	.2byte 0x0000
	.section .text.x02008440,"ax",%progbits
	.global Func_02000440
	.thumb_func
Func_02000440:
	push {lr}
	movs r0, #18
	sub sp, #8
	bl Object_GetById
	adds r2, r0, #0
	movs r3, #0
	adds r2, #35
	adds r0, #85
	strb r3, [r2]
	strb r3, [r0]
	movs r2, #16
	movs r3, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #16
	movs r2, #1
	movs r3, #1
	movs r0, #6
	bl Func_0200106c
	movs r0, #132
	lsls r0, r0, #2
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008478,"ax",%progbits
	.global Func_02000478
	.thumb_func
Func_02000478:
	push {lr}
	movs r0, #19
	sub sp, #8
	bl Object_GetById
	adds r2, r0, #0
	movs r3, #0
	adds r2, #35
	adds r0, #85
	strb r3, [r2]
	strb r3, [r0]
	movs r2, #30
	movs r3, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #30
	movs r2, #1
	movs r3, #1
	movs r0, #8
	bl Func_0200106c
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.section .text.x020084b0,"ax",%progbits
	.global Func_020004b0
	.thumb_func
Func_020004b0:
	push {r5, lr}
	adds r5, r1, #0
	adds r3, r5, #0
	subs r3, #16
	sub sp, #8
	cmp r3, #1
	bhi .L_020084d2
	movs r3, #20
	movs r2, #33
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #120
	movs r1, #33
	movs r2, #8
	movs r3, #2
	bl Func_0200106c
.L_020084d2:
	adds r3, r5, #0
	subs r3, #14
	cmp r3, #1
	bhi .L_020084ee
	movs r3, #8
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #112
	movs r1, #15
	movs r2, #16
	movs r3, #4
	bl Func_0200106c
.L_020084ee:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200851c,"ax",%progbits
	.global Func_0200051c
	.thumb_func
Func_0200051c:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	sub sp, #8
	cmp r7, #20
	bne .L_02008566
	movs r0, #20
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #253
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008566
	ldr r3, [r5, #8]
	asrs r5, r3, #20
	cmp r5, #4
	bne .L_02008566
	movs r0, #20
	bl Func_0200015c
	movs r3, #28
	str r3, [sp, #4]
	movs r0, #3
	movs r1, #28
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_0200106c
	movs r0, #253
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
.L_02008566:
	cmp r7, #13
	bne .L_020085b0
	movs r0, #13
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #232
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085b0
	ldr r3, [r5, #8]
	asrs r6, r3, #20
	cmp r6, #28
	bne .L_020085b0
	ldr r3, [r5, #16]
	asrs r5, r3, #20
	cmp r5, #30
	bne .L_020085b0
	movs r0, #13
	bl Func_0200015c
	movs r0, #32
	movs r1, #30
	movs r2, #1
	movs r3, #2
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_0200106c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #232
	bl GameFlag_SetBit
.L_020085b0:
	adds r3, r7, #0
	subs r3, #14
	cmp r3, #1
	bhi .L_020085cc
	movs r0, #14
	movs r1, #8
	movs r2, #17
	bl Func_020004f4
	movs r0, #15
	movs r1, #8
	movs r2, #17
	bl Func_020004f4
.L_020085cc:
	adds r3, r7, #0
	subs r3, #16
	cmp r3, #1
	bhi .L_020085e8
	movs r0, #16
	movs r1, #22
	movs r2, #33
	bl Func_020004f4
	movs r0, #17
	movs r1, #22
	movs r2, #33
	bl Func_020004f4
.L_020085e8:
	add sp, #8
	pop {r5, r6, r7, pc}
	.section .text.x020085ec,"ax",%progbits
	.global Func_020005ec
	.thumb_func
Func_020005ec:
	push {lr}
	ldr r3, .L_0200861c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008620
	cmp r2, r3
	bne .L_02008604
	ldr r0, .L_02008624
	b .L_0200861a
.L_02008604:
	ldr r3, .L_02008628
	cmp r2, r3
	bne .L_0200860e
	ldr r0, .L_0200862c
	b .L_0200861a
.L_0200860e:
	ldr r3, .L_02008630
	cmp r2, r3
	bne .L_02008618
	ldr r0, .L_02008634
	b .L_0200861a
.L_02008618:
	ldr r0, .L_02008638
.L_0200861a:
	pop {pc}
.L_0200861c:
	.4byte gPartyState
.L_02008620:
	.4byte 0x0000006b
.L_02008624:
	.4byte Data_02001ae4
.L_02008628:
	.4byte 0x0000006c
.L_0200862c:
	.4byte Data_02001c58
.L_02008630:
	.4byte 0x0000006d
.L_02008634:
	.4byte Data_02001db4
.L_02008638:
	.4byte Data_02001ad8
	.section .text.x0200863c,"ax",%progbits
	.global Func_0200063c
	.thumb_func
Func_0200063c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	sub sp, #8
	bl Func_02001184
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	ldr r7, .L_02008950
	orrs r3, r2
	movs r2, #240
	lsls r2, r2, #1
	strb r3, [r0]
	adds r3, r7, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008954
	movs r6, #0
	cmp r2, r3
	beq .L_0200867a
	b .L_020087d6
.L_0200867a:
	movs r0, #18
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #1
	movs r0, #19
	bl ObjectMotion_SetActionVariant
	movs r0, #19
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #19
	bl Object_GetById
	adds r0, #89
	strb r6, [r0]
	movs r0, #19
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	adds r0, #89
	strb r6, [r0]
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086ee
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
.L_020086ee:
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008710
	movs r3, #22
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #24
	movs r1, #32
	movs r2, #1
	movs r3, #1
	bl Func_0200106c
.L_02008710:
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008734
	movs r3, #47
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #28
	movs r2, #1
	movs r3, #1
	bl Func_0200106c
	b .L_0200873c
.L_02008734:
	movs r0, #14
	movs r1, #1
	bl ObjectMotion_SetActionVariant
.L_0200873c:
	movs r0, #137
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200875c
	movs r3, #53
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #53
	movs r1, #35
	movs r2, #1
	movs r3, #1
	bl Func_0200106c
.L_0200875c:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200879c
	movs r0, #16
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	movs r0, #17
	bl Object_GetById
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
	b .L_020087a6
.L_0200879c:
	movs r1, #2
	movs r0, #10
	adds r1, #255
	bl Func_02001124
.L_020087a6:
	movs r0, #17
	movs r1, #231
	bl Func_0200118c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #130
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020087c8
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_020010d4
	b .L_0200894a
.L_020087c8:
	movs r0, #13
	bl Object_GetById
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r0, #28]
	b .L_0200894a
.L_020087d6:
	ldr r3, .L_02008958
	cmp r2, r3
	beq .L_020087de
	b .L_0200894a
.L_020087de:
	movs r0, #8
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	movs r1, #1
	bl ObjectMotion_SetActionVariant
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
	beq .L_02008822
	movs r3, #4
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #6
	movs r1, #16
	movs r2, #1
	movs r3, #1
	bl Func_0200106c
.L_02008822:
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008844
	movs r3, #10
	movs r2, #30
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #8
	movs r1, #30
	movs r2, #1
	movs r3, #1
	bl Func_0200106c
.L_02008844:
	movs r0, #253
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008890
	movs r0, #20
	bl Object_GetById
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
.L_02008890:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #232
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020088dc
	movs r0, #13
	bl Object_GetById
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
.L_020088dc:
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
	bne .L_0200894a
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #8
	bne .L_0200894a
	bl Func_02000af0
.L_0200894a:
	movs r0, #0
	add sp, #8
	pop {r5, r6, r7, pc}
.L_02008950:
	.4byte gPartyState
.L_02008954:
	.4byte 0x0000006b
.L_02008958:
	.4byte 0x0000006c
	.section .text.x02008960,"ax",%progbits
	.global Func_02000960
	.thumb_func
Func_02000960:
	push {lr}
	bl Func_02001084
	movs r0, #0
	bl Func_02001164
	ldr r0, .L_02008ac8
	bl Func_02001104
	movs r1, #156
	movs r2, #152
	lsls r2, r2, #17
	movs r0, #27
	lsls r1, r1, #17
	bl Func_020010d4
	movs r0, #27
	movs r1, #0
	bl Func_0200110c
	movs r0, #128
	movs r1, #1
	movs r2, #196
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02001134
	movs r0, #11
	bl ObjectMotion_ResetTargetsAndVelocity
	movs r0, #11
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #132
	movs r2, #156
	movs r0, #27
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #200
	lsls r2, r2, #1
	movs r0, #27
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #11
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #11
	movs r1, #0
	bl Func_0200110c
	movs r0, #11
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #11
	movs r1, #0
	bl Func_0200110c
	movs r0, #27
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #27
	movs r1, #0
	bl Func_0200110c
	movs r0, #27
	movs r1, #0
	bl Func_0200110c
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #11
	bl Func_0200111c
	movs r0, #11
	movs r1, #0
	bl Func_0200110c
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #27
	bl Func_0200111c
	movs r0, #27
	movs r1, #0
	bl Func_0200110c
	movs r0, #11
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #11
	movs r1, #0
	bl Func_0200110c
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #27
	bl Func_0200111c
	movs r0, #27
	movs r1, #0
	bl Func_0200110c
	movs r1, #132
	movs r2, #156
	movs r0, #27
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #180
	movs r2, #156
	lsls r2, r2, #1
	movs r0, #27
	lsls r1, r1, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #11
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #11
	movs r1, #0
	bl Func_0200110c
	ldr r3, .L_02008acc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #11
	bl Object_LinkObjectAndSetCallback
	movs r0, #30
	bl WaitFrames
	movs r0, #11
	movs r1, #0
	bl Func_0200110c
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #60
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #27
	movs r1, #0
	bl Func_020010d4
	ldr r1, .L_02008ad0
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	bl Func_0200108c
	movs r0, #136
	lsls r0, r0, #4
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
.L_02008ac8:
	.4byte MsgIsTheWay
.L_02008acc:
	.4byte gPartyState
.L_02008ad0:
	.4byte Data_020017f8
	.section .text.x02008af0,"ax",%progbits
	.global Func_02000af0
	.thumb_func
Func_02000af0:
	push {r5, lr}
	ldr r3, .L_02008d70
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r5, [r3]
	bl Func_02001084
	movs r0, #0
	bl Func_02001164
	movs r1, #248
	movs r2, #236
	movs r0, #23
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_020010d4
	movs r1, #132
	movs r2, #236
	movs r0, #24
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_020010d4
	movs r1, #235
	movs r0, #25
	lsls r1, r1, #16
	ldr r2, .L_02008d74
	bl Func_020010d4
	movs r1, #232
	movs r2, #248
	movs r0, #26
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_020010d4
	movs r0, #27
	ldr r1, .L_02008d78
	ldr r2, .L_02008d74
	bl Func_020010d4
	movs r1, #140
	movs r2, #248
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #28
	bl Func_020010d4
	adds r0, r5, #0
	bl Func_02000ad4
	movs r0, #23
	bl Func_02000ad4
	movs r0, #24
	bl Func_02000ad4
	movs r0, #25
	bl Func_02000ad4
	movs r0, #26
	bl Func_02000ad4
	movs r0, #27
	bl Func_02000ad4
	movs r0, #28
	bl Func_02000ad4
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r0, .L_02008d7c
	bl Func_02001104
	movs r1, #4
	movs r2, #30
	adds r1, #255
	movs r0, #27
	bl Func_0200111c
	movs r0, #27
	movs r1, #0
	bl Func_0200110c
	movs r2, #0
	movs r1, #25
	movs r0, #27
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	adds r1, r5, #0
	movs r2, #0
	movs r0, #27
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #27
	bl Func_0200111c
	movs r0, #27
	movs r1, #0
	bl Func_0200110c
	movs r0, #27
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #27
	movs r1, #0
	bl Func_0200110c
	movs r0, #25
	movs r1, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #25
	bl Func_0200111c
	movs r0, #25
	movs r1, #0
	bl Func_0200110c
	movs r2, #134
	movs r1, #248
	lsls r2, r2, #2
	movs r0, #23
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #1
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #134
	lsls r2, r2, #2
	movs r0, #24
	lsls r1, r1, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	adds r1, r5, #0
	movs r0, #25
	bl Object_LinkObjectAndSetCallback
	adds r1, r5, #0
	movs r0, #26
	bl Object_LinkObjectAndSetCallback
	adds r1, r5, #0
	movs r0, #27
	bl Object_LinkObjectAndSetCallback
	adds r1, r5, #0
	movs r0, #28
	bl Object_LinkObjectAndSetCallback
	movs r0, #2
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #128
	movs r2, #138
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #2
	adds r0, r5, #0
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	movs r0, #23
	bl Object_SetModeById
	movs r1, #1
	movs r0, #24
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #23
	bl Object_SetModeById
	movs r0, #3
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #244
	movs r1, #248
	lsls r2, r2, #1
	movs r0, #23
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #2
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #244
	movs r0, #24
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #220
	movs r0, #23
	movs r1, #248
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #132
	movs r2, #220
	movs r0, #24
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r2, #228
	movs r0, #25
	movs r1, #232
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r2, #236
	movs r0, #26
	movs r1, #232
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #140
	movs r2, #228
	movs r0, #27
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #140
	movs r2, #236
	movs r0, #28
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_020010d4
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Func_020010d4
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl Func_020010d4
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_020010d4
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020010d4
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020010d4
	bl Func_0200108c
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_SetBit
	pop {r5, pc}
	.2byte 0x0000
.L_02008d70:
	.4byte gPartyState
.L_02008d74:
	.4byte 0x01e30000
.L_02008d78:
	.4byte 0x01150000
.L_02008d7c:
	.4byte MsgHereTheIntruder
	.section .text.x02008d80,"ax",%progbits
	.global Func_02000d80
	.thumb_func
Func_02000d80:
	push {lr}
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	ldr r3, .L_02008da4
	cmp r0, #0
	beq .L_02008d9a
	adds r0, r3, #0
	movs r1, #9
	bl Func_0200113c
	b .L_02008da2
.L_02008d9a:
	adds r0, r3, #0
	movs r1, #8
	bl Func_0200113c
.L_02008da2:
	pop {pc}
.L_02008da4:
	.4byte 0x0000006c
	.section .text.x02008da8,"ax",%progbits
	.global Func_02000da8
	.thumb_func
Func_02000da8:
	push {r5, r6, lr}
	ldr r3, .L_02008e20
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r6, [r3]
	adds r5, r0, #0
	bl Func_02001084
	movs r0, #0
	bl Func_02001164
	adds r0, r5, #0
	bl ObjectMotion_ResetTargetsAndVelocity
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #1
	movs r2, #30
	bl Func_0200111c
	movs r2, #20
	adds r1, r6, #0
	adds r0, r5, #0
	bl ObjectMotion_SetAngleToward
	ldr r0, .L_02008e24
	bl Func_02001104
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200110c
	movs r1, #2
	adds r0, r6, #0
	adds r1, #255
	bl Func_02001124
	adds r0, r5, #0
	movs r1, #4
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200110c
	movs r1, #128
	adds r0, r6, #0
	lsls r1, r1, #1
	bl Func_02001124
	bl Func_02000d80
	bl Func_0200108c
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008e20:
	.4byte gPartyState
.L_02008e24:
	.4byte MsgGuardsAnIntruder
	.section .text.x02008e28,"ax",%progbits
	.global Func_02000e28
	.thumb_func
Func_02000e28:
	push {r5, r6, lr}
	ldr r3, .L_02008ea0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r6, [r3]
	adds r5, r0, #0
	bl Func_02001084
	movs r0, #0
	bl Func_02001164
	adds r0, r5, #0
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #1
	movs r2, #30
	bl Func_0200111c
	movs r2, #20
	adds r1, r6, #0
	adds r0, r5, #0
	bl ObjectMotion_SetAngleToward
	ldr r0, .L_02008ea4
	bl Func_02001104
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200110c
	movs r1, #2
	adds r0, r6, #0
	adds r1, #255
	bl Func_02001124
	adds r0, r5, #0
	movs r1, #4
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	bl Func_0200110c
	movs r1, #128
	adds r0, r6, #0
	lsls r1, r1, #1
	bl Func_02001124
	bl Func_02000d80
	bl Func_0200108c
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008ea0:
	.4byte gPartyState
.L_02008ea4:
	.4byte MsgGrrrrrRrrruffRrufff
	.section .text.x02008ea8,"ax",%progbits
	.global Func_02000ea8
	.thumb_func
Func_02000ea8:
	push {r5, lr}
	ldr r3, .L_02008ef0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #14
	bl Object_GetById
	ldr r3, [r5, #16]
	ldr r1, [r0, #16]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_02008ed4
	movs r3, #128
	lsls r3, r3, #13
	cmp r2, r3
	blt .L_02008ede
	b .L_02008ee6
.L_02008ed4:
	movs r2, #128
	subs r3, r3, r1
	lsls r2, r2, #13
	cmp r3, r2
	bge .L_02008ee6
.L_02008ede:
	ldr r2, [r0, #8]
	ldr r3, [r5, #8]
	cmp r2, r3
	bgt .L_02008eec
.L_02008ee6:
	movs r0, #10
	bl Func_02000da8
.L_02008eec:
	pop {r5, pc}
	.2byte 0x0000
.L_02008ef0:
	.4byte gPartyState
	.section .text.x02008ef4,"ax",%progbits
	.global Func_02000ef4
	.thumb_func
Func_02000ef4:
	push {r5, lr}
	ldr r3, .L_02008f3c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #15
	bl Object_GetById
	ldr r3, [r5, #16]
	ldr r1, [r0, #16]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_02008f20
	movs r3, #128
	lsls r3, r3, #13
	cmp r2, r3
	blt .L_02008f2a
	b .L_02008f32
.L_02008f20:
	movs r2, #128
	subs r3, r3, r1
	lsls r2, r2, #13
	cmp r3, r2
	bge .L_02008f32
.L_02008f2a:
	ldr r2, [r0, #8]
	ldr r3, [r5, #8]
	cmp r2, r3
	blt .L_02008f38
.L_02008f32:
	movs r0, #10
	bl Func_02000da8
.L_02008f38:
	pop {r5, pc}
	.2byte 0x0000
.L_02008f3c:
	.4byte gPartyState
