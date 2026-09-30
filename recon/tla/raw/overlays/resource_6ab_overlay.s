.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {lr}
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.section .text.x02008060,"ax",%progbits
	.global Func_02000060
	.thumb_func
Func_02000060:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #56
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3]
	bx lr
	.section .text.x02008074,"ax",%progbits
	.global Func_02000074
	.thumb_func
Func_02000074:
	push {lr}
	bl Func_020014f4
	pop {pc}
	.global Data_0200007c
Data_0200007c:
	.4byte 0x00004770
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	bl Object_GetById
	movs r1, #5
	mov r9, r0
	mov r11, r1
.L_02008098:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r9
	ldr r2, [r2, #8]
	lsls r5, r5, #4
	mov r8, r2
	add r8, r5
	lsls r0, r0, #4
	mov r3, r8
	subs r3, r3, r0
	mov r8, r3
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r1, r9
	ldr r2, [r1, #12]
	ldr r3, [r1, #16]
	lsls r6, r6, #3
	adds r6, r6, r2
	lsls r5, r5, #4
	movs r2, #128
	lsls r0, r0, #4
	lsls r2, r2, #11
	adds r3, r3, r5
	subs r3, r3, r0
	adds r6, r6, r2
	movs r0, #70
	adds r0, #255
	mov r1, r8
	adds r2, r6, #0
	bl Func_020041e4
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02008168
	bl Random16Far
	mov r8, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #7
	adds r0, r7, #0
	ldr r1, .L_02008180
	lsrs r5, r5, #1
	adds r5, r5, r3
	bl Func_020041dc
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r1, r8
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r1, #2
	adds r3, r3, r2
	str r3, [r7, #40]
	mov r0, r8
	bl Math_Cosine
	ldr r3, .L_02008184
	lsls r6, r6, #3
	adds r1, r0, #0
	mov r10, r3
	adds r0, r6, #0
	mov lr, r10
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r8
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r10
	.2byte 0xf800
	movs r3, #0
	str r3, [r7, #52]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #8
	str r0, [r7, #36]
	str r5, [r7, #24]
	str r5, [r7, #28]
	str r3, [r7, #68]
	adds r0, r7, #0
	movs r1, #1
	bl Animation_ApplyChildValues
.L_02008168:
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	cmp r2, #0
	bge .L_02008098
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008180:
	.4byte Data_02004474
.L_02008184:
	.4byte IwramMulQ16
	.section .text.x02008188,"ax",%progbits
	.global Func_02000188
	.thumb_func
Func_02000188:
	push {r5, lr}
	movs r0, #10
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #142
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081ac
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #75
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020081e6
.L_020081ac:
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_0200446c
	adds r3, r5, #0
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r0, #160
	lsls r3, r3, #16
	str r3, [r5, #8]
	adds r3, r5, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r0, r0, #4
	lsls r3, r3, #16
	str r3, [r5, #16]
	adds r0, #74
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020081e6
	movs r3, #153
	lsls r3, r3, #8
	adds r3, #153
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_020081e6:
	pop {r5, pc}
	.section .text.x020081e8,"ax",%progbits
	.global Func_020001e8
	.thumb_func
Func_020001e8:
	push {r5, lr}
	movs r0, #10
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #142
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200820c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #75
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200824a
.L_0200820c:
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_0200446c
	movs r0, #154
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	adds r3, r5, #0
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r0, #160
	lsls r3, r3, #16
	str r3, [r5, #8]
	adds r3, r5, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r0, r0, #4
	lsls r3, r3, #16
	str r3, [r5, #16]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	adds r0, #74
	bl GameFlag_SetBit
.L_0200824a:
	pop {r5, pc}
	.section .text.x0200824c,"ax",%progbits
	.global Func_0200024c
	.thumb_func
Func_0200024c:
	push {r5, lr}
	adds r5, r1, #0
	cmp r0, #1
	bne .L_02008294
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #62
	bl Func_0200446c
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02004404
	movs r0, #60
	bl Func_02004414
	bl Func_02000d88
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl Func_0200432c
	movs r0, #142
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #20
	strh r3, [r2]
.L_02008294:
	adds r3, r5, #0
	subs r3, #97
	cmp r3, #174
	bhi .L_020082ae
	adds r0, r5, #0
	movs r1, #30
	bl Engine_MathRemainder
	cmp r0, #0
	bne .L_020082ae
	movs r0, #10
	bl Func_02000080
.L_020082ae:
	movs r3, #138
	lsls r3, r3, #1
	cmp r5, r3
	bne .L_020082c8
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02004404
	movs r0, #20
	bl Func_02004414
.L_020082c8:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020082d4,"ax",%progbits
	.global Func_020002d4
	.thumb_func
Func_020002d4:
	push {r5, lr}
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020082f2
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r5, #0
	adds r3, #89
	movs r2, #0
	strb r2, [r3]
	subs r3, #4
	strb r2, [r3]
.L_020082f2:
	pop {r5, pc}
	.section .text.x020082f4,"ax",%progbits
	.global Func_020002f4
	.thumb_func
Func_020002f4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_02008548
	movs r3, #240
	mov r8, r1
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r5, [r3, r2]
	movs r3, #241
	lsls r3, r3, #1
	movs r2, #192
	lsls r2, r2, #18
	add r3, r8
	movs r1, #0
	ldrsh r7, [r3, r1]
	ldr r3, [r2, #108]
	movs r1, #214
	lsls r1, r1, #1
	mov r10, r2
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	ldr r2, .L_0200854c
	movs r3, #0
	movs r0, #137
	str r3, [r2]
	lsls r0, r0, #1
	sub sp, #8
	bl GameFlag_SetBit
	bl Func_02001d7c
	movs r3, #88
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02001df0
	ldr r3, .L_02008550
	cmp r5, r3
	beq .L_02008350
	b .L_0200853a
.L_02008350:
	movs r0, #0
	bl Func_0200443c
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083ac
	movs r5, #38
	movs r6, #24
	movs r0, #65
	movs r1, #65
	movs r2, #9
	movs r3, #21
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_0200423c
	movs r0, #65
	movs r1, #65
	movs r2, #9
	movs r3, #21
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004244
	movs r3, #87
	str r3, [sp, #4]
	movs r0, #76
	movs r1, #64
	movs r2, #9
	movs r3, #22
	str r5, [sp, #0]
	bl Func_02004244
	movs r3, #102
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #76
	movs r1, #64
	movs r2, #9
	movs r3, #22
	bl Func_02004244
.L_020083ac:
	cmp r7, #11
	bne .L_0200840a
	movs r3, #133
	lsls r3, r3, #2
	add r3, r8
	ldr r0, [r3]
	bl Object_GetById
	mov r8, r0
	movs r0, #208
	mov r2, r10
	lsls r0, r0, #2
	ldr r6, [r2, #108]
	bl GameFlag_GetByte
	adds r5, r0, #0
	movs r0, #210
	lsls r0, r0, #2
	bl GameFlag_GetByte
	movs r3, #128
	lsls r3, r3, #12
	lsls r5, r5, #20
	adds r5, r5, r3
	ldr r2, .L_02008554
	movs r3, #230
	lsls r3, r3, #1
	adds r6, r6, r3
	lsls r0, r0, #20
	adds r0, r0, r2
	ldr r2, [r6]
	mov r1, r8
	str r0, [r1, #16]
	str r5, [r1, #8]
	str r5, [r2, #8]
	ldr r3, [r1, #16]
	str r3, [r2, #16]
	bl Func_020041fc
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200840a
	bl Func_02003e04
.L_0200840a:
	adds r3, r7, #0
	subs r3, #9
	cmp r3, #1
	bhi .L_02008422
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008422
	bl Func_02003e04
.L_02008422:
	cmp r7, #1
	beq .L_0200842c
	cmp r7, #20
	beq .L_0200842c
	b .L_0200853a
.L_0200842c:
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008452
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	bl Object_GetById
	ldr r3, .L_02008558
	str r3, [r0, #24]
.L_02008452:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #75
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008470
	bl Func_02000d88
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200432c
	b .L_02008494
.L_02008470:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #74
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200848a
	movs r0, #154
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02008494
.L_0200848a:
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200432c
.L_02008494:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #59
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008532
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084c2
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_0200432c
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_0200432c
.L_020084c2:
	bl Func_02000d88
	movs r0, #10
	bl WaitFrames
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #58
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084f8
	movs r0, #0
	bl Func_02000ae4
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084f8
	movs r0, #13
	bl Func_02000db8
	movs r0, #14
	bl Func_02000db8
.L_020084f8:
	movs r0, #13
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r1, #200
	lsls r1, r1, #17
	cmp r3, r1
	ble .L_0200853a
	movs r3, #43
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #39
	movs r1, #22
	movs r2, #2
	movs r3, #1
	bl Func_0200423c
	movs r3, #40
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #31
	movs r1, #21
	movs r2, #2
	movs r3, #2
	bl Func_0200423c
	b .L_0200853a
.L_02008532:
	cmp r7, #20
	bne .L_0200853a
	bl Func_02000714
.L_0200853a:
	movs r0, #0
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008548:
	.4byte gPartyState
.L_0200854c:
	.4byte Data_02004cec
.L_02008550:
	.4byte 0x0000011e
.L_02008554:
	.4byte 0xfed80000
.L_02008558:
	.4byte 0xffff0000
	.section .text.x0200855c,"ax",%progbits
	.global Func_0200055c
	.thumb_func
Func_0200055c:
	push {lr}
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008586
	ldr r3, .L_020085a4
	movs r2, #253
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r3, .L_020085a8
	ldr r2, .L_020085ac
	movs r1, #160
	subs r3, r3, r2
	adds r0, r0, r3
	lsls r1, r1, #19
	bl Func_02004294
.L_02008586:
	ldr r3, .L_020085a4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #20
	bne .L_020085a0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #75
	bl GameFlag_SetBit
.L_020085a0:
	movs r0, #0
	pop {pc}
.L_020085a4:
	.4byte gPartyState
.L_020085a8:
	.4byte 0x00000121
.L_020085ac:
	.4byte 0x0000010e
	.section .text.x020085b0,"ax",%progbits
	.global Func_020005b0
	.thumb_func
Func_020005b0:
	push {r5, r6, lr}
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	bl Func_02000d88
	movs r0, #0
	bl Func_0200446c
	movs r0, #40
	bl Battle_WaitMode0
	ldr r6, .L_02008704
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r6, r1
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #129
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_020043bc
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02008708
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #0
	movs r2, #8
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r0, [r5]
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	ldr r0, [r5]
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r2, #20
	ldr r0, [r5]
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #53
	bl Func_0200446c
	movs r0, #11
	movs r1, #2
	bl Object_SetModeById
	movs r1, #2
	movs r0, #12
	bl Object_SetModeById
	movs r0, #24
	bl Battle_WaitMode0
	movs r1, #174
	movs r2, #232
	movs r0, #11
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_0200432c
	movs r1, #166
	movs r2, #232
	lsls r2, r2, #16
	movs r0, #12
	lsls r1, r1, #18
	bl Func_0200432c
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #12
	bl Object_SetModeById
	movs r0, #107
	bl Func_0200446c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02004264
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #195
	lsls r0, r0, #1
	bl Func_0200446c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_02004264
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r1, r1
	negs r0, r0
	bl Func_02004264
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #148
	bl Func_0200446c
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r1, #4
	movs r0, #12
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r6, r3
	movs r3, #2
	strb r3, [r2]
	ldr r0, .L_0200870c
	movs r1, #20
	bl Party_SetFields1eeAnd1f0
	ldr r3, .L_02008710
	movs r1, #251
	lsls r1, r1, #1
	adds r2, r6, r1
	strh r3, [r2]
	movs r0, #102
	movs r1, #0
	bl Func_020043e4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008704:
	.4byte gPartyState
.L_02008708:
	.4byte 0x00019999
.L_0200870c:
	.4byte 0x0000011e
.L_02008710:
	.4byte 0x00000068
	.section .text.x02008714,"ax",%progbits
	.global Func_02000714
	.thumb_func
Func_02000714:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #11
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #15
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #16
	bl Object_GetById
	mov r8, r0
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02000d88
	movs r0, #13
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #15
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #16
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5, #16]
	ldr r2, .L_02008a98
	ldr r1, [r5, #8]
	adds r3, r3, r2
	adds r0, r5, #0
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	adds r3, r6, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	mov r1, r8
	movs r3, #128
	lsls r3, r3, #7
	adds r1, #85
	str r3, [r6, #24]
	str r3, [r6, #28]
	strb r2, [r1]
	mov r2, r8
	str r3, [r2, #24]
	str r3, [r2, #28]
	movs r0, #170
	movs r1, #1
	movs r2, #228
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #0
	bl Motion_CamBounds
	ldr r3, .L_02008a9c
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	movs r1, #170
	movs r2, #132
	lsls r2, r2, #17
	lsls r1, r1, #18
	ldr r0, [r6]
	bl Func_0200432c
	movs r0, #1
	bl WaitFrames
	bl Func_020041fc
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #78
	bl Func_0200446c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #151
	bl Func_0200446c
	movs r0, #11
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #12
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #13
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #14
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #190
	bl Func_0200446c
	movs r0, #15
	movs r1, #11
	bl Func_02004334
	movs r1, #12
	movs r0, #16
	bl Func_02004334
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_02008aa0
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02008aa4
	movs r0, #12
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r5, .L_02008aa8
	movs r0, #15
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #16
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #13
	bl Func_02000db8
	movs r0, #14
	bl Func_02000db8
	ldr r5, .L_02008aac
	movs r0, #15
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #16
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #80
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #5
	movs r0, #13
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_02008ab0
	bl Func_0200437c
	movs r2, #80
	movs r0, #13
	movs r1, #0
	bl Func_0200438c
	movs r1, #1
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #34
	bl Func_0200446c
	movs r2, #20
	movs r0, #13
	movs r1, #0
	bl Func_0200438c
	movs r0, #14
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r1, #5
	movs r0, #14
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #20
	movs r0, #14
	movs r1, #0
	bl Func_0200438c
	movs r0, #14
	movs r1, #0
	bl Func_02004394
	movs r0, #13
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #13
	movs r1, #6
	bl Object_SetModeById
	movs r0, #13
	movs r1, #0
	bl Func_02004394
	movs r0, #14
	movs r1, #6
	bl Object_SetModeById
	movs r0, #14
	movs r1, #0
	movs r2, #20
	bl Func_0200438c
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #13
	bl Func_020043b4
	movs r0, #13
	movs r1, #0
	bl Func_02004394
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #14
	bl Func_020043b4
	movs r0, #14
	movs r1, #0
	bl Func_02004394
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #13
	bl Func_020043b4
	movs r2, #20
	movs r0, #13
	movs r1, #0
	bl Func_0200438c
	movs r0, #13
	movs r1, #0
	bl Func_02004394
	movs r0, #14
	movs r1, #7
	bl Object_SetModeById
	movs r0, #14
	movs r1, #0
	bl Func_02004394
	movs r0, #13
	movs r1, #0
	bl Func_02004394
	movs r0, #14
	movs r1, #0
	movs r2, #40
	bl Func_0200438c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #13
	bl Func_020043b4
	movs r1, #0
	movs r0, #13
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008a04
	ldr r0, [r6]
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #13
	movs r1, #7
	bl Object_SetModeById
	movs r0, #13
	movs r1, #0
	bl Func_02004394
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008a2c
.L_02008a04:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	ldr r0, [r6]
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #13
	movs r1, #6
	bl Object_SetModeById
	movs r0, #13
	movs r1, #0
	bl Func_02004394
.L_02008a2c:
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #0
	bl Func_02004394
	movs r0, #14
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #14
	movs r1, #0
	bl Func_02004394
	movs r1, #0
	movs r0, #13
	bl Func_02004394
	movs r0, #1
	bl Func_02000ae4
	movs r0, #14
	movs r1, #0
	bl Func_02004394
	movs r0, #13
	movs r1, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #13
	movs r1, #0
	bl Func_02004394
	movs r0, #13
	movs r1, #10
	bl Object_SetModeById
	movs r1, #9
	movs r0, #14
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #59
	bl GameFlag_SetBit
	bl Func_0200445c
	bl Func_020042e4
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02008a98:
	.4byte 0xfff80000
.L_02008a9c:
	.4byte gPartyState
.L_02008aa0:
	.4byte Data_020049a0
.L_02008aa4:
	.4byte Data_020049dc
.L_02008aa8:
	.4byte Data_0200491c
.L_02008aac:
	.4byte Data_02004958
.L_02008ab0:
	.4byte 0x00002d6a
	.section .text.x02008ab4,"ax",%progbits
	.global Func_02000ab4
	.thumb_func
Func_02000ab4:
	push {r5, lr}
	ldr r3, .L_02008ae0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r4, [r5, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r4, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	pop {r5, pc}
	.2byte 0x0000
.L_02008ae0:
	.4byte gPartyState
	.section .text.x02008ae4,"ax",%progbits
	.global Func_02000ae4
	.thumb_func
Func_02000ae4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008b98
	adds r7, r0, #0
	mov r8, r3
	movs r0, #234
	movs r3, #224
	lsls r3, r3, #16
	adds r0, #255
	ldr r1, .L_02008b9c
	movs r2, #0
	bl Func_020041e4
	mov r3, r8
	movs r5, #0
	str r0, [r3]
	cmp r0, #0
	beq .L_02008b92
	ldr r6, [r0, #80]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r2, r0, #0
	strb r3, [r6, #9]
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	adds r2, #7
	movs r3, #1
	strb r3, [r2]
	ldr r3, .L_02008ba0
	movs r1, #193
	str r3, [r0, #108]
	lsls r1, r1, #3
	strb r5, [r6, #26]
	strb r5, [r6, #27]
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r5, r0, #0
	movs r0, #247
	bl Func_020042ac
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	ldrb r0, [r6, #16]
	movs r1, #128
	adds r2, r5, #0
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	cmp r7, #0
	beq .L_02008b92
	movs r0, #1
	bl WaitFrames
	mov r3, r8
	ldr r2, [r3]
	movs r3, #160
	lsls r3, r3, #10
	str r3, [r2, #40]
	movs r0, #4
	bl WaitFrames
	movs r0, #135
	bl Func_0200446c
	movs r0, #8
	bl WaitFrames
	movs r0, #135
	bl Func_0200446c
.L_02008b92:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008b98:
	.4byte Data_02004cec
.L_02008b9c:
	.4byte 0x02be0000
.L_02008ba0:
	.4byte Func_02000ab4
	.section .text.x02008ba4,"ax",%progbits
	.global Func_02000ba4
	.thumb_func
Func_02000ba4:
	push {lr}
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	movs r1, #5
	movs r0, #14
	bl Object_SetModeById
	ldr r0, .L_02008bd4
	bl Func_0200437c
	movs r0, #14
	movs r1, #0
	bl Func_02004394
	movs r0, #14
	movs r1, #9
	bl Object_SetModeById
	bl Func_020042e4
	pop {pc}
.L_02008bd4:
	.4byte 0x00002d80
	.section .text.x02008bd8,"ax",%progbits
	.global Func_02000bd8
	.thumb_func
Func_02000bd8:
	push {r5, r6, r7, lr}
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	movs r1, #5
	movs r0, #13
	bl Object_SetModeById
	ldr r0, .L_02008c84
	bl Func_0200437c
	movs r0, #13
	movs r1, #0
	bl Func_02004394
	movs r0, #13
	movs r1, #10
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #58
	bl GameFlag_Test
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02008c14
	b .L_02008d80
.L_02008c14:
	ldr r5, .L_02008c88
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #128
	adds r6, r0, #0
	lsls r1, r1, #7
	ldr r0, [r5]
	bl Func_020043a4
	ldr r0, [r5]
	movs r1, #28
	bl Object_SetModeById
	ldr r5, .L_02008c8c
	ldr r0, [r5]
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r0, #52]
	movs r3, #128
	ldr r2, [r6, #12]
	lsls r3, r3, #14
	adds r2, r2, r3
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	bl Func_02004204
	ldr r0, [r5]
	bl Func_0200420c
	movs r0, #83
	bl Func_0200446c
	movs r0, #224
	bl PartyInventory_Remove
	movs r1, #222
	movs r0, #4
	bl Inventory_AddItem
	ldr r0, .L_02008c90
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r5, #0
	movs r6, #216
	b .L_02008c98
.L_02008c84:
	.4byte 0x00002d7f
.L_02008c88:
	.4byte gPartyState
.L_02008c8c:
	.4byte Data_02004cec
.L_02008c90:
	.4byte 0x00002e6c
.L_02008c94:
	adds r6, #2
	adds r5, #1
.L_02008c98:
	cmp r5, #14
	bgt .L_02008cb4
	movs r0, #4
	bl Owner_GetState
	ldr r3, .L_02008cdc
	ldrh r2, [r0, r6]
	ands r3, r2
	cmp r3, #222
	bne .L_02008c94
	movs r0, #4
	adds r1, r5, #0
	bl Func_020042c4
.L_02008cb4:
	ldr r3, .L_02008ce0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #58
	bl GameFlag_SetBit
	ldr r5, .L_02008ce4
	movs r2, #0
	movs r3, #0
	movs r1, #0
	ldr r0, [r5]
	b .L_02008ce8
	.2byte 0x0000
.L_02008cdc:
	.4byte 0x000001ff
.L_02008ce0:
	.4byte gPartyState
.L_02008ce4:
	.4byte Data_02004cec
.L_02008ce8:
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
	ldr r0, [r5]
	bl Func_020041ec
	movs r0, #1
	bl WaitFrames
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_02008d74
	adds r1, #153
	bl Func_020043c4
	movs r0, #170
	movs r1, #1
	movs r2, #146
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	bl Motion_CamBounds
	bl Func_020043d4
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_0200440c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02004404
	movs r0, #20
	bl Func_02004414
	movs r0, #20
	bl WaitFrames
	ldr r3, .L_02008d78
	ldr r2, .L_02008d70
	movs r0, #1
	strh r2, [r3]
	bl WaitFrames
	movs r2, #0
	ldr r0, .L_02008d7c
	movs r1, #0
	bl Func_020042a4
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02004404
	movs r0, #20
	bl Func_02004414
	movs r0, #20
	bl WaitFrames
	b .L_02008d80
.L_02008d70:
	.4byte 0x00007fff
.L_02008d74:
	.4byte 0x0004cccc
.L_02008d78:
	.4byte 0x0500021e
.L_02008d7c:
	.4byte 0x00002d83
.L_02008d80:
	bl Func_020042e4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008d88,"ax",%progbits
	.global Func_02000d88
	.thumb_func
Func_02000d88:
	push {lr}
	sub sp, #8
	movs r3, #5
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #68
	movs r2, #40
	movs r3, #73
	bl Func_02004214
	movs r3, #40
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #104
	movs r1, #9
	movs r2, #5
	movs r3, #6
	bl Func_0200423c
	add sp, #8
	pop {pc}
	.section .text.x02008db8,"ax",%progbits
	.global Func_02000db8
	.thumb_func
Func_02000db8:
	push {r5, r6, lr}
	sub sp, #8
	adds r6, r0, #0
	bl Object_GetById
	adds r5, r0, #0
	adds r0, r6, #0
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r6, #0
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	adds r3, r5, #0
	adds r3, #85
	movs r1, #0
	strb r1, [r3]
	adds r0, r5, #0
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	adds r3, r5, #0
	adds r3, #89
	strb r1, [r3]
	cmp r6, #13
	bne .L_02008e20
	movs r1, #175
	movs r2, #224
	lsls r2, r2, #16
	movs r0, #13
	lsls r1, r1, #18
	bl Func_0200432c
	movs r0, #13
	movs r1, #10
	bl Object_SetModeById
	movs r3, #43
	str r3, [sp, #0]
	movs r0, #39
	movs r1, #22
	movs r2, #2
	movs r3, #1
	str r6, [sp, #4]
	bl Func_0200423c
	b .L_02008e4a
.L_02008e20:
	movs r1, #165
	movs r2, #216
	lsls r2, r2, #16
	movs r0, #14
	lsls r1, r1, #18
	bl Func_0200432c
	movs r0, #14
	movs r1, #9
	bl Object_SetModeById
	movs r3, #40
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #31
	movs r1, #21
	movs r2, #2
	movs r3, #2
	bl Func_0200423c
.L_02008e4a:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008e50,"ax",%progbits
	.global Func_02000e50
	.thumb_func
Func_02000e50:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	ldr r0, .L_02008e7c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r3, #173
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
	bl Func_020042e4
	pop {r5, pc}
	.2byte 0x0000
.L_02008e7c:
	.4byte 0x00002e6b
	.section .text.x02008e80,"ax",%progbits
	.global Func_02000e80
	.thumb_func
Func_02000e80:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #158
	sub sp, #8
	bl GameFlag_Test
	mov r8, r0
	cmp r0, #0
	bne .L_02008ea2
	b .L_0200905c
.L_02008ea2:
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	movs r1, #1
	ldr r0, .L_0200904c
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #78
	bl Func_0200446c
	movs r0, #1
	bl WaitFrames
	movs r0, #182
	bl Func_0200446c
	movs r0, #234
	movs r1, #170
	movs r2, #128
	movs r3, #147
	adds r0, #255
	lsls r1, r1, #18
	lsls r2, r2, #12
	lsls r3, r3, #16
	ldr r7, .L_02009050
	bl Func_020041e4
	str r0, [r7]
	cmp r0, #0
	beq .L_02008f48
	ldr r6, [r0, #80]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r6, #9]
	adds r3, r0, #0
	movs r1, #0
	adds r3, #85
	adds r2, r0, #0
	strb r1, [r3]
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
	strb r1, [r6, #26]
	strb r1, [r6, #27]
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r1, #193
	lsls r1, r1, #3
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r5, r0, #0
	movs r0, #247
	bl Func_020042ac
	movs r2, #128
	lsls r2, r2, #3
	adds r5, r5, r2
	ldrb r0, [r6, #16]
	movs r1, #128
	adds r2, r5, #0
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
.L_02008f48:
	movs r0, #20
	bl Battle_WaitMode0
	ldr r3, .L_02009054
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #170
	ldr r0, [r6]
	lsls r1, r1, #2
	movs r2, #186
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #80
	bl ObjectMotion_ArmCallback
	movs r1, #2
	ldr r0, [r6]
	adds r1, #255
	movs r2, #40
	bl Func_020043b4
	ldr r0, [r7]
	cmp r0, #0
	beq .L_02008fe6
	adds r2, r0, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #10
	movs r5, #128
	str r3, [r0, #40]
	str r3, [r0, #48]
	lsls r5, r5, #9
	movs r1, #168
	movs r3, #160
	lsls r1, r1, #18
	movs r2, #0
	lsls r3, r3, #16
	str r5, [r0, #52]
	bl Func_02004204
	ldr r0, [r7]
	bl Func_0200420c
	movs r0, #135
	bl Func_0200446c
	ldr r0, [r7]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r1, #166
	movs r3, #158
	str r5, [r0, #48]
	lsls r1, r1, #18
	movs r2, #0
	lsls r3, r3, #16
	bl Func_02004204
	ldr r0, [r7]
	bl Func_0200420c
.L_02008fe6:
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_02009058
	adds r2, #204
	ldr r0, [r6]
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #128
	lsls r1, r1, #2
	movs r2, #170
	adds r1, #158
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	ldr r0, [r7]
	cmp r0, #0
	beq .L_02009038
	bl Func_020041ec
.L_02009038:
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	bl Func_020043a4
	bl Func_0200445c
	bl Func_020042e4
	b .L_020093b0
.L_0200904c:
	.4byte 0x00002e6d
.L_02009050:
	.4byte Data_02004cec
.L_02009054:
	.4byte gPartyState
.L_02009058:
	.4byte 0x00019999
.L_0200905c:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r3, [r3]
	mov r11, r3
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	movs r1, #1
	ldr r0, .L_020093c0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #78
	bl Func_0200446c
	movs r0, #1
	bl WaitFrames
	movs r0, #182
	bl Func_0200446c
	ldr r3, .L_020093c4
	movs r0, #234
	mov r9, r3
	movs r1, #170
	movs r2, #128
	movs r3, #147
	lsls r2, r2, #12
	adds r0, #255
	lsls r1, r1, #18
	lsls r3, r3, #16
	bl Func_020041e4
	mov r2, r9
	str r0, [r2]
	cmp r0, #0
	beq .L_02009112
	ldr r6, [r0, #80]
	mov r3, r8
	ldrb r2, [r6, #5]
	strb r3, [r6, #26]
	strb r3, [r6, #27]
	movs r3, #33
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r6, #9]
	adds r3, r0, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	adds r2, r0, #0
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r1, #193
	lsls r1, r1, #3
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r5, r0, #0
	movs r0, #247
	bl Func_020042ac
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	ldrb r0, [r6, #16]
	movs r1, #128
	adds r2, r5, #0
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
.L_02009112:
	movs r0, #20
	bl Battle_WaitMode0
	ldr r2, .L_020093c8
	movs r7, #133
	mov r10, r2
	lsls r7, r7, #2
	movs r1, #204
	movs r2, #204
	add r7, r10
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r7]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #170
	ldr r0, [r7]
	lsls r1, r1, #2
	movs r2, #186
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #40
	ldr r0, [r7]
	bl ObjectMotion_ArmCallback
	movs r0, #0
	bl Func_0200446c
	movs r0, #141
	bl Func_0200446c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #10
	lsls r0, r0, #10
	bl Func_02004264
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	ldr r0, [r7]
	lsls r1, r1, #1
	bl Func_020043bc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #9
	bl Func_02004264
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_0200440c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #6
	bl Func_02004404
	movs r0, #80
	bl Func_02004414
	movs r0, #80
	bl Battle_WaitMode0
	ldr r5, .L_020093cc
	movs r1, #144
	adds r0, r5, #0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_020093d0
	adds r1, #153
	bl Func_020043c4
	movs r0, #170
	movs r1, #1
	movs r2, #240
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_020043d4
	movs r0, #80
	bl Battle_WaitMode0
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r1, r1
	negs r0, r0
	bl Func_02004264
	movs r0, #195
	lsls r0, r0, #1
	bl Func_0200446c
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02004404
	movs r0, #80
	bl Func_02004414
	movs r0, #60
	bl WaitFrames
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	movs r0, #20
	bl WaitFrames
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl Func_02004264
	movs r0, #20
	bl Battle_WaitMode0
	movs r5, #38
	movs r6, #24
	movs r0, #65
	movs r1, #65
	movs r2, #9
	movs r3, #21
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_0200423c
	movs r0, #65
	movs r1, #65
	movs r2, #9
	movs r3, #21
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004244
	movs r3, #87
	str r3, [sp, #4]
	movs r0, #76
	movs r1, #64
	movs r2, #9
	movs r3, #22
	str r5, [sp, #0]
	bl Func_02004244
	movs r3, #102
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #76
	movs r2, #9
	movs r1, #64
	movs r3, #22
	bl Func_02004244
	movs r3, #253
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r2, .L_020093d4
	ldr r3, .L_020093d8
	mov r1, r11
	subs r3, r3, r2
	adds r0, r0, r3
	bl Func_02004294
	mov r3, r8
	mov r2, r11
	movs r0, #128
	strh r3, [r2]
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02004404
	movs r0, #120
	bl Func_02004414
	movs r0, #160
	bl WaitFrames
	movs r0, #170
	movs r1, #1
	movs r2, #186
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	bl Func_020043d4
	movs r0, #40
	bl Battle_WaitMode0
	mov r3, r9
	ldr r0, [r3]
	cmp r0, #0
	beq .L_0200932e
	adds r2, r0, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #10
	movs r5, #128
	str r3, [r0, #40]
	str r3, [r0, #48]
	lsls r5, r5, #9
	movs r1, #168
	movs r3, #160
	lsls r1, r1, #18
	lsls r3, r3, #16
	str r5, [r0, #52]
	movs r2, #0
	bl Func_02004204
	mov r2, r9
	ldr r0, [r2]
	bl Func_0200420c
	movs r0, #135
	bl Func_0200446c
	mov r3, r9
	ldr r0, [r3]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r1, #166
	movs r3, #158
	str r5, [r0, #48]
	movs r2, #0
	lsls r1, r1, #18
	lsls r3, r3, #16
	bl Func_02004204
	mov r2, r9
	ldr r0, [r2]
	bl Func_0200420c
.L_0200932e:
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_020093dc
	adds r2, #204
	ldr r0, [r7]
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #128
	strb r3, [r0]
	lsls r1, r1, #2
	movs r2, #170
	adds r1, #158
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	mov r3, r9
	ldr r0, [r3]
	cmp r0, #0
	beq .L_02009382
	bl Func_020041ec
.L_02009382:
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, [r7]
	bl Func_020043a4
	bl Func_0200445c
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #158
	bl GameFlag_SetBit
	ldr r3, .L_020093e0
	movs r2, #251
	lsls r2, r2, #1
	add r2, r10
	strh r3, [r2]
	bl Func_020042e4
.L_020093b0:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020093c0:
	.4byte 0x00002e6d
.L_020093c4:
	.4byte Data_02004cec
.L_020093c8:
	.4byte gPartyState
.L_020093cc:
	.4byte Func_02001424
.L_020093d0:
	.4byte 0x0004cccc
.L_020093d4:
	.4byte 0x0000010e
.L_020093d8:
	.4byte 0x00000121
.L_020093dc:
	.4byte 0x00019999
.L_020093e0:
	.4byte 0x00000069
	.section .text.x020093e4,"ax",%progbits
	.global Func_020013e4
	.thumb_func
Func_020013e4:
	push {lr}
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #12]
	movs r2, #160
	ldr r3, [r0, #24]
	lsls r2, r2, #3
	adds r2, #30
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	ldr r3, [r0, #104]
	subs r3, #1
	str r3, [r0, #104]
	cmp r3, #1
	bne .L_02009418
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Object_SetPositionAndResetMotion
	b .L_02009420
.L_02009418:
	cmp r3, #0
	bne .L_02009420
	bl Func_020041ec
.L_02009420:
	pop {pc}
	.2byte 0x0000
	.section .text.x02009424,"ax",%progbits
	.global Func_02001424
	.thumb_func
Func_02001424:
	push {r5, lr}
	ldr r3, .L_020094ec
	movs r1, #1
	ldr r2, [r3]
	adds r3, r2, #0
	ands r3, r1
	cmp r3, #0
	bne .L_020094e8
	lsrs r3, r2, #1
	ands r3, r1
	cmp r3, #0
	beq .L_02009454
	bl Random16Far
	lsls r5, r0, #3
	subs r5, r5, r0
	bl Random16Far
	lsls r5, r5, #2
	lsls r3, r0, #4
	subs r3, r3, r0
	lsrs r5, r5, #16
	movs r2, #155
	b .L_0200946a
.L_02009454:
	bl Random16Far
	lsls r5, r0, #3
	subs r5, r5, r0
	bl Random16Far
	lsls r5, r5, #2
	lsls r3, r0, #4
	subs r3, r3, r0
	lsrs r5, r5, #16
	movs r2, #178
.L_0200946a:
	lsls r2, r2, #18
	lsls r3, r3, #3
	lsls r5, r5, #16
	adds r5, r5, r2
	lsrs r3, r3, #16
	movs r2, #230
	lsls r2, r2, #17
	lsls r3, r3, #16
	adds r3, r3, r2
	movs r0, #30
	movs r2, #128
	adds r1, r5, #0
	adds r0, #255
	lsls r2, r2, #14
	bl Func_020041e4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020094e8
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
	adds r3, r5, #0
	movs r1, #0
	adds r3, #85
	strb r1, [r3]
	adds r3, #15
	strh r1, [r3]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r3, #13
	ldr r1, [r5, #80]
	negs r3, r3
	ldrb r2, [r1, #9]
	adds r0, r5, #0
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #9]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #60
	str r3, [r5, #104]
	ldr r3, .L_020094f0
	adds r0, r5, #0
	movs r1, #5
	str r3, [r5, #108]
	bl Func_020041cc
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
.L_020094e8:
	pop {r5, pc}
	.2byte 0x0000
.L_020094ec:
	.4byte Data_0300122c
.L_020094f0:
	.4byte Func_020013e4
	.section .text.x020094f4,"ax",%progbits
	.global Func_020014f4
	.thumb_func
Func_020014f4:
	push {lr}
	ldr r3, .L_02009520
	sub sp, #4
	str r3, [sp, #0]
	movs r1, #18
	movs r2, #20
	movs r3, #0
	movs r0, #9
	bl Func_02002a58
	ldr r2, .L_02009524
	movs r3, #128
	str r2, [sp, #0]
	lsls r3, r3, #8
	movs r1, #17
	movs r2, #19
	movs r0, #9
	bl Func_02002a58
	add sp, #4
	pop {pc}
	.2byte 0x0000
.L_02009520:
	.4byte Data_020044b8
.L_02009524:
	.4byte Data_020045b8
	.section .text.x02009528,"ax",%progbits
	.global Func_02001528
	.thumb_func
Func_02001528:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_0200953e
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009548
	b .L_02009588
.L_0200953e:
	movs r2, #128
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009588
.L_02009548:
	ldr r4, [r0, #12]
	ldr r3, [r1, #12]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_0200955c
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009566
	b .L_02009588
.L_0200955c:
	movs r2, #128
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009588
.L_02009566:
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_0200957a
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009584
	b .L_02009588
.L_0200957a:
	movs r2, #128
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009588
.L_02009584:
	movs r0, #1
	b .L_0200958a
.L_02009588:
	movs r0, #0
.L_0200958a:
	pop {pc}
	.section .text.x0200958c,"ax",%progbits
	.global Func_0200158c
	.thumb_func
Func_0200158c:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_020095a2
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_020095ac
	b .L_020095de
.L_020095a2:
	movs r2, #192
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_020095de
.L_020095ac:
	ldr r2, [r1, #12]
	ldr r3, [r0, #12]
	subs r3, r3, r2
	ldr r2, .L_020095e4
	adds r3, r3, r2
	ldr r2, .L_020095e8
	cmp r3, r2
	bhi .L_020095de
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_020095d0
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_020095da
	b .L_020095de
.L_020095d0:
	movs r2, #192
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_020095de
.L_020095da:
	movs r0, #1
	b .L_020095e0
.L_020095de:
	movs r0, #0
.L_020095e0:
	pop {pc}
	.2byte 0x0000
.L_020095e4:
	.4byte 0x0007ffff
.L_020095e8:
	.4byte 0x001ffffe
	.section .text.x020095ec,"ax",%progbits
	.global Func_020015ec
	.thumb_func
Func_020015ec:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009748
	ldr r2, .L_0200974c
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	str r4, [sp, #0]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_02009648
	adds r3, #15
.L_02009648:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_020096da
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001528
	cmp r0, #0
	beq .L_020096da
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r2, [r3]
	cmp r2, #0
	bne .L_020096da
	ldr r1, [r6, #76]
	cmp r1, #0
	beq .L_020096ae
	mov r3, r8
	adds r3, #104
	strh r2, [r3]
	mov r2, r8
	adds r2, #106
	cmp r1, #0
	ble .L_020096a6
	movs r3, #1
	b .L_020096ac
.L_020096a6:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_020096ac:
	strh r3, [r2]
.L_020096ae:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_020096da:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_0200973a
	mov r5, r8
	adds r5, #84
.L_020096ea:
	bl Object_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_0200972c
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_0200158c
	cmp r0, #0
	beq .L_0200972c
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_02009726
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_0200972c
.L_02009726:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_0200972c:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_0200973a
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_020096ea
.L_0200973a:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009748:
	.4byte gPartyState
.L_0200974c:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009750,"ax",%progbits
	.global Func_02001750
	.thumb_func
Func_02001750:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #4
	strb r6, [r3]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, .L_0200977c
	adds r0, r5, #0
	bl Func_020041dc
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200977c:
	.4byte Data_02004744
	.section .text.x02009780,"ax",%progbits
	.global Func_02001780
	.thumb_func
Func_02001780:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_020041e4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020097c0
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	str r6, [r5, #76]
	movs r1, #2
	bl Func_02001750
	ldr r3, .L_020097c4
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_020097b8
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_020097b8:
	adds r0, r5, #0
	movs r1, #2
	bl Func_020041cc
.L_020097c0:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020097c4:
	.4byte Func_020015ec
	.section .text.x020097c8,"ax",%progbits
	.global Func_020017c8
	.thumb_func
Func_020017c8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009920
	ldr r2, .L_02009924
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
	ldr r3, [r6, #16]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #16]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #8]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #8]
	str r4, [sp, #0]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_02009824
	adds r3, #15
.L_02009824:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_020098b4
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001528
	cmp r0, #0
	beq .L_020098b4
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_020098b4
	ldr r2, [r6, #76]
	cmp r2, #0
	beq .L_02009888
	mov r1, r8
	adds r1, #106
	strh r3, [r1]
	subs r1, #2
	cmp r2, #0
	ble .L_02009880
	movs r3, #1
	b .L_02009886
.L_02009880:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_02009886:
	strh r3, [r1]
.L_02009888:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r3, [r6, #16]
	ldr r2, [r6, #68]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_020098b4:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_02009914
	mov r5, r8
	adds r5, #84
.L_020098c4:
	bl Object_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009906
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_0200158c
	cmp r0, #0
	beq .L_02009906
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_02009900
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_02009906
.L_02009900:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_02009906:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_02009914
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_020098c4
.L_02009914:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009920:
	.4byte gPartyState
.L_02009924:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009928,"ax",%progbits
	.global Func_02001928
	.thumb_func
Func_02001928:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #4
	strb r6, [r3]
	movs r3, #3
	ldr r0, [r5, #80]
	ands r1, r3
	ldrb r2, [r0, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	lsls r1, r1, #2
	orrs r3, r1
	strb r3, [r0, #9]
	movs r1, #0
	adds r0, r5, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, .L_02009968
	adds r0, r5, #0
	bl Func_020041dc
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
.L_02009968:
	.4byte Data_02004744
	.section .text.x0200996c,"ax",%progbits
	.global Func_0200196c
	.thumb_func
Func_0200196c:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_020041e4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020099ae
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	negs r3, r6
	str r3, [r5, #76]
	movs r1, #3
	bl Func_02001928
	ldr r3, .L_020099b0
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_020099a6
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_020099a6:
	adds r0, r5, #0
	movs r1, #2
	bl Func_020041cc
.L_020099ae:
	pop {r5, r6, r7, pc}
.L_020099b0:
	.4byte Func_020015ec
	.section .text.x020099b4,"ax",%progbits
	.global Func_020019b4
	.thumb_func
Func_020019b4:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_020041e4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020099f4
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	str r6, [r5, #76]
	movs r1, #3
	bl Func_02001928
	ldr r3, .L_020099f8
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_020099ec
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_020099ec:
	adds r0, r5, #0
	movs r1, #2
	bl Func_020041cc
.L_020099f4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020099f8:
	.4byte Func_020017c8
	.section .text.x020099fc,"ax",%progbits
	.global Func_020019fc
	.thumb_func
Func_020019fc:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_020041e4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009a3e
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	negs r3, r6
	str r3, [r5, #76]
	movs r1, #3
	bl Func_02001928
	ldr r3, .L_02009a40
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_02009a36
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_02009a36:
	adds r0, r5, #0
	movs r1, #2
	bl Func_020041cc
.L_02009a3e:
	pop {r5, r6, r7, pc}
.L_02009a40:
	.4byte Func_020017c8
	.section .text.x02009a44,"ax",%progbits
	.global Func_02001a44
	.thumb_func
Func_02001a44:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_02009a5a
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009a64
	b .L_02009a94
.L_02009a5a:
	movs r2, #192
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009a94
.L_02009a64:
	ldr r2, [r1, #12]
	ldr r3, [r0, #12]
	subs r3, r3, r2
	ldr r2, .L_02009a98
	subs r3, #1
	cmp r3, r2
	bhi .L_02009a94
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_02009a86
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009a90
	b .L_02009a94
.L_02009a86:
	movs r2, #128
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009a94
.L_02009a90:
	movs r0, #1
	b .L_02009a96
.L_02009a94:
	movs r0, #0
.L_02009a96:
	pop {pc}
.L_02009a98:
	.4byte 0x000ffffe
	.section .text.x02009a9c,"ax",%progbits
	.global Func_02001a9c
	.thumb_func
Func_02001a9c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009b34
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	mov r8, r0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_02009ae0
	adds r3, #15
.L_02009ae0:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	mov r3, r8
	ldr r2, [r3, #12]
	ldr r3, [r3, #20]
	cmp r2, r3
	bne .L_02009b2e
	adds r0, r6, #0
	mov r1, r8
	bl Func_02001a44
	cmp r0, #0
	beq .L_02009b2e
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	movs r3, #0
	str r3, [r6, #76]
.L_02009b2e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009b34:
	.4byte gPartyState
	.section .text.x02009b38,"ax",%progbits
	.global Func_02001b38
	.thumb_func
Func_02001b38:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #4
	strb r6, [r3]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Func_020041cc
	adds r0, r5, #0
	ldr r1, .L_02009b74
	bl Func_020041dc
	adds r0, r5, #0
	movs r1, #10
	bl Object_SetPartAttribute
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009b74:
	.4byte Data_02004744
	.section .text.x02009b78,"ax",%progbits
	.global Func_02001b78
	.thumb_func
Func_02001b78:
	push {r5, r6, lr}
	ldr r3, [r0, #8]
	ldr r2, [r0, #4]
	adds r6, r1, #0
	ldr r1, [r0]
	ldr r0, .L_02009bec
	adds r3, r3, r0
	movs r0, #30
	adds r0, #255
	bl Func_020041e4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009bea
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #12
	movs r3, #192
	lsls r3, r3, #6
	lsrs r0, r0, #16
	adds r0, r0, r3
	bl Math_Cosine
	str r0, [r5, #68]
	bl Random16Far
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r0, [r5, #72]
	bl Random16Far
	lsls r0, r0, #17
	lsrs r0, r0, #16
	adds r0, r0, r6
	str r0, [r5, #76]
	bl Random16Far
	ldr r3, .L_02009bf0
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	movs r1, #2
	adds r0, r5, #0
	bl Func_02001b38
	ldr r3, .L_02009bf4
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #1
	bl Animation_SetStateFlags
.L_02009bea:
	pop {r5, r6, pc}
.L_02009bec:
	.4byte 0xfffe0000
.L_02009bf0:
	.4byte 0xffff8000
.L_02009bf4:
	.4byte Func_02001a9c
	.section .text.x02009bf8,"ax",%progbits
	.global Func_02001bf8
	.thumb_func
Func_02001bf8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_02009d74
	sub sp, #8
	mov r8, r0
	movs r0, #192
	lsls r0, r0, #18
	ldr r1, [r0, #32]
	mov r7, r8
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	ldr r2, [r2, #4]
	mov r9, r3
	ldr r3, .L_02009d78
	mov r4, r9
	ands r4, r3
	ands r2, r3
	ldr r3, [r1]
	mov r9, r4
	ldr r3, [r3, #4]
	adds r7, #20
	str r3, [sp, #4]
	mov r10, r2
	ldr r0, [r0, #108]
	str r0, [sp, #0]
	mov r0, r8
	movs r4, #6
	ldrsh r3, [r0, r4]
	cmp r3, #0
	beq .L_02009c52
	movs r1, #8
	ldrsh r3, [r0, r1]
	cmp r3, #0
	bne .L_02009c52
	ldr r3, [r0, #16]
	cmp r3, #0
	beq .L_02009c52
	mov lr, r3
	.2byte 0xf800
.L_02009c52:
	mov r2, r8
	ldrh r3, [r2, #6]
	mov r4, r8
	movs r2, #0
	mov r0, r8
	strh r3, [r4, #8]
	strh r2, [r0, #6]
	movs r1, #3
	mov r11, r1
.L_02009c64:
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #0
	beq .L_02009d54
	ldr r5, [r7, #8]
	cmp r5, #0
	beq .L_02009d54
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	ldr r4, [sp, #0]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r4, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02009c8e
	movs r3, #1
	orrs r0, r3
.L_02009c8e:
	adds r6, r5, #0
	adds r6, #91
	strb r0, [r6]
	mov r0, r8
	movs r4, #14
	ldrsh r3, [r0, r4]
	cmp r3, #0
	beq .L_02009ca8
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	strb r0, [r6]
.L_02009ca8:
	ldr r2, [r5, #8]
	mov r1, r9
	ldr r3, [r5, #16]
	subs r2, r2, r1
	ldr r1, [r5, #12]
	mov r4, r10
	subs r3, r3, r4
	subs r1, r3, r1
	movs r0, #6
	ldrsh r4, [r7, r0]
	asrs r3, r1, #16
	adds r1, r3, #0
	asrs r2, r2, #16
	subs r1, #8
	cmp r4, #0
	bne .L_02009cde
	adds r3, r2, #7
	movs r2, #167
	lsls r2, r2, #1
	cmp r3, r2
	bhi .L_02009d54
	movs r3, #48
	negs r3, r3
	cmp r1, r3
	ble .L_02009d54
	cmp r1, #239
	bgt .L_02009d54
.L_02009cde:
	movs r0, #2
	ldrsh r3, [r7, r0]
	ldrh r1, [r7, #2]
	cmp r3, #0
	bgt .L_02009d50
	ldrh r3, [r7, #4]
	movs r1, #240
	ands r1, r3
	cmp r1, #32
	beq .L_02009d24
	cmp r1, #32
	bgt .L_02009d00
	cmp r1, #0
	beq .L_02009d40
	cmp r1, #16
	beq .L_02009d32
	b .L_02009d4c
.L_02009d00:
	cmp r1, #48
	beq .L_02009d16
	cmp r1, #128
	bne .L_02009d4c
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001b78
	b .L_02009d4c
.L_02009d16:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_0200196c
	b .L_02009d4c
.L_02009d24:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_020019fc
	b .L_02009d4c
.L_02009d32:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_020019b4
	b .L_02009d4c
.L_02009d40:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001780
.L_02009d4c:
	movs r3, #8
	b .L_02009d52
.L_02009d50:
	subs r3, r1, #1
.L_02009d52:
	strh r3, [r7, #2]
.L_02009d54:
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	adds r7, #16
	cmp r2, #0
	blt .L_02009d64
	b .L_02009c64
.L_02009d64:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009d74:
	.4byte Data_020023c4 + 0x188
.L_02009d78:
	.4byte 0xffff0000
	.section .text.x02009d7c,"ax",%progbits
	.global Func_02001d7c
	.thumb_func
Func_02001d7c:
	push {r5, r6, lr}
	movs r0, #10
	adds r0, #255
	ldr r6, .L_02009dc8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009d96
	ldr r3, .L_02009dcc
	adds r0, r6, #0
	movs r1, #116
	mov lr, r3
	.2byte 0xf800
.L_02009d96:
	movs r0, #110
	movs r1, #1
	movs r2, #0
	movs r3, #0
	adds r0, #255
	bl Func_020041e4
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
	movs r1, #1
	bl Func_020041cc
	ldr r1, [r5, #80]
	movs r2, #1
	ldrb r3, [r1, #16]
	str r5, [r6, #112]
	strh r3, [r6, #12]
	ldrb r3, [r1, #17]
	orrs r3, r2
	strb r3, [r1, #17]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009dc8:
	.4byte Data_020023c4 + 0x188
.L_02009dcc:
	.4byte IwramClearWords
	.section .text.x02009dd0,"ax",%progbits
	.global Func_02001dd0
	.thumb_func
Func_02001dd0:
	push {r5, lr}
	ldr r5, .L_02009de0
	ldr r0, [r5, #112]
	bl Func_020041ec
	movs r3, #0
	str r3, [r5, #112]
	pop {r5, pc}
.L_02009de0:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009de4,"ax",%progbits
	.global Func_02001de4
	.thumb_func
Func_02001de4:
	ldr r3, .L_02009dec
	strh r0, [r3, #14]
	bx lr
	.2byte 0x0000
.L_02009dec:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009df0,"ax",%progbits
	.global Func_02001df0
	.thumb_func
Func_02001df0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r1
	ldr r1, .L_02009f1c
	sub sp, #16
	adds r6, r0, #0
	movs r0, #10
	str r1, [sp, #4]
	adds r0, #255
	adds r1, #20
	str r2, [sp, #12]
	str r3, [sp, #8]
	mov r9, r1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009ed4
	ldrh r3, [r6]
	movs r2, #0
	mov r11, r2
	mov r10, r3
	adds r6, #2
	cmp r3, #0
	ble .L_02009e92
.L_02009e2a:
	ldrh r7, [r6]
	movs r1, #15
	ands r1, r7
	movs r3, #240
	mov r0, r10
	str r1, [sp, #0]
	ands r7, r3
	bl Object_GetById
	adds r5, r0, #0
	adds r6, #2
	cmp r5, #0
	beq .L_02009e7e
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r5, #0
	movs r2, #128
	adds r3, #98
	movs r1, #1
	ands r2, r7
	strb r1, [r3]
	cmp r2, #0
	bne .L_02009e5e
	subs r3, #9
	strb r2, [r3]
.L_02009e5e:
	mov r2, r9
	mov r3, r9
	strh r1, [r2]
	mov r0, r10
	strh r7, [r3, #4]
	bl Object_GetById
	mov r1, r9
	str r0, [r1, #8]
	ldr r2, [sp, #0]
	lsls r3, r2, #16
	str r3, [r1, #12]
	mov r3, r11
	strh r3, [r1, #2]
	movs r2, #16
	add r9, r2
.L_02009e7e:
	movs r3, #1
	add r11, r3
	mov r1, r11
	cmp r1, #3
	bgt .L_02009e92
	ldrh r2, [r6]
	adds r6, #2
	mov r10, r2
	cmp r2, #0
	bgt .L_02009e2a
.L_02009e92:
	mov r3, r8
	cmp r3, #0
	beq .L_02009ed4
	movs r1, #0
	ldrh r2, [r3]
	mov r11, r1
	ldr r1, [sp, #4]
	movs r3, #2
	add r8, r3
	movs r3, #84
	strh r2, [r1, r3]
	cmp r2, #0
	ble .L_02009ed4
	adds r2, r1, #0
	adds r2, #84
.L_02009eb0:
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #1
	strh r3, [r2, #2]
	add r11, r1
	movs r3, #2
	add r8, r3
	mov r3, r11
	adds r2, #4
	cmp r3, #3
	bgt .L_02009ed4
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #2
	add r8, r1
	strh r3, [r2]
	cmp r3, #0
	bgt .L_02009eb0
.L_02009ed4:
	ldr r2, [sp, #12]
	ldr r3, [sp, #4]
	add r1, sp, #8
	str r2, [r3, #16]
	ldrh r1, [r1]
	ldr r2, [sp, #4]
	strh r1, [r2, #10]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #80
	ldrh r3, [r1]
	cmp r3, #0
	bne .L_02009f04
	movs r3, #192
	movs r2, #128
	lsls r3, r3, #4
	lsls r2, r2, #19
	adds r3, #8
	adds r2, #82
	strh r3, [r2]
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #16
	strh r3, [r1]
.L_02009f04:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009f20
	bl Scheduler_AddOrUpdateCallback
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009f1c:
	.4byte Data_020023c4 + 0x188
.L_02009f20:
	.4byte Func_02001bf8
	.section .text.x02009f24,"ax",%progbits
	.global Func_02001f24
	.thumb_func
Func_02001f24:
	ldr r3, .L_02009f30
	lsls r0, r0, #4
	adds r0, r0, r3
	movs r3, #20
	ldrsh r0, [r0, r3]
	bx lr
.L_02009f30:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009f34,"ax",%progbits
	.global Func_02001f34
	.thumb_func
Func_02001f34:
	ldr r3, .L_02009f40
	lsls r0, r0, #4
	adds r0, r0, r3
	strh r1, [r0, #20]
	bx lr
	.2byte 0x0000
.L_02009f40:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009f44,"ax",%progbits
	.global Func_02001f44
	.thumb_func
Func_02001f44:
	ldr r3, .L_02009f4c
	ldr r0, [r3]
	bx lr
	.2byte 0x0000
.L_02009f4c:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009f50,"ax",%progbits
	.global Func_02001f50
	.thumb_func
Func_02001f50:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #130
	lsls r0, r0, #1
	ldr r5, .L_02009f74
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009f6e
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	cmp r3, r6
	blt .L_02009f6e
	str r0, [r5]
.L_02009f6e:
	ldr r0, [r5]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009f74:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009f78,"ax",%progbits
	.global Func_02001f78
	.thumb_func
Func_02001f78:
	ldr r3, .L_02009f94
	lsls r0, r0, #4
	adds r0, r0, r3
	movs r3, #15
	ands r3, r1
	adds r0, #20
	lsls r3, r3, #16
	str r3, [r0, #12]
	ldr r3, .L_02009f90
	ands r1, r3
	strh r1, [r0, #4]
	bx lr
.L_02009f90:
	.4byte 0x000000f0
.L_02009f94:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009f98,"ax",%progbits
	.global Func_02001f98
	.thumb_func
Func_02001f98:
	ldr r3, .L_02009fa4
	lsls r0, r0, #4
	adds r0, r0, r3
	strh r1, [r0, #26]
	bx lr
	.2byte 0x0000
.L_02009fa4:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009fa8,"ax",%progbits
	.global Func_02001fa8
	.thumb_func
Func_02001fa8:
	ldr r3, .L_02009fb4
	lsls r0, r0, #4
	adds r0, r0, r3
	movs r3, #24
	ldrsh r0, [r0, r3]
	bx lr
.L_02009fb4:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009fb8,"ax",%progbits
	.global Func_02001fb8
	.thumb_func
Func_02001fb8:
	push {lr}
	ldr r2, .L_02009fc8
	cmp r0, #3
	bhi .L_02009fc6
	lsls r3, r0, #2
	adds r3, #84
	strh r1, [r2, r3]
.L_02009fc6:
	pop {pc}
.L_02009fc8:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009fcc,"ax",%progbits
	.global Func_02001fcc
	.thumb_func
Func_02001fcc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #16
	ldr r6, [r3, #108]
	bl Func_02004464
	mov r8, r0
	bl Object_GetById
	bl Party_CountActiveOwners
	movs r5, #0
	adds r7, r0, #0
	cmp r5, r7
	bge .L_0200a00e
.L_02009ff2:
	ldr r2, .L_0200a0b4
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r5, r1
	ldrb r0, [r2, r3]
	bl Owner_GetState
	ldrh r3, [r0, #56]
	lsls r2, r5, #1
	mov r1, sp
	adds r5, #1
	strh r3, [r1, r2]
	cmp r5, r7
	blt .L_02009ff2
.L_0200a00e:
	movs r0, #10
	negs r0, r0
	movs r1, #0
	bl Func_02004454
	movs r2, #182
	lsls r2, r2, #1
	movs r4, #183
	adds r3, r6, r2
	lsls r4, r4, #1
	movs r2, #0
	strh r2, [r3]
	movs r1, #129
	adds r3, r6, r4
	strh r2, [r3]
	mov r0, r8
	lsls r1, r1, #1
	movs r5, #0
	bl Func_020043bc
	cmp r5, r7
	bge .L_0200a0a8
.L_0200a03a:
	ldr r1, .L_0200a0b4
	movs r2, #134
	lsls r2, r2, #2
	adds r2, r2, r5
	ldrb r0, [r1, r2]
	mov r10, r1
	mov r8, r2
	bl Owner_GetState
	movs r4, #56
	ldrsh r3, [r0, r4]
	cmp r3, #0
	ble .L_0200a062
	movs r1, #183
	lsls r1, r1, #1
	adds r2, r6, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a0a2
.L_0200a062:
	mov r3, sp
	lsls r2, r5, #1
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a0a2
	movs r2, #182
	lsls r2, r2, #1
	adds r1, r6, r2
	ldrh r3, [r1]
	movs r4, #184
	adds r2, r3, #1
	lsls r3, r3, #16
	lsls r4, r4, #1
	asrs r3, r3, #15
	strh r2, [r1]
	adds r3, r3, r4
	mov r1, r10
	mov r4, r8
	ldrb r2, [r1, r4]
	movs r1, #181
	strh r2, [r6, r3]
	movs r3, #255
	lsls r1, r1, #1
	lsls r3, r3, #8
	adds r2, r6, r1
	adds r3, #255
	strh r3, [r2]
	movs r3, #50
	adds r3, #255
	adds r2, r0, r3
	movs r3, #0
	strb r3, [r2]
.L_0200a0a2:
	adds r5, #1
	cmp r5, r7
	blt .L_0200a03a
.L_0200a0a8:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a0b4:
	.4byte gPartyState
	.section .text.x0200a0b8,"ax",%progbits
	.global Func_020020b8
	.thumb_func
Func_020020b8:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200a11c
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200a0f6
	adds r3, #15
.L_0200a0f6:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	movs r1, #128
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	lsls r1, r1, #5
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r2, [r6, #80]
	ldrh r3, [r2, #18]
	adds r3, r3, r1
	strh r3, [r2, #18]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a11c:
	.4byte gPartyState
	.section .text.x0200a120,"ax",%progbits
	.global Func_02002120
	.thumb_func
Func_02002120:
	push {lr}
	ldr r3, .L_0200a144
	ldr r2, [r3]
	movs r3, #7
	ands r2, r3
	cmp r2, #0
	beq .L_0200a134
	cmp r2, #4
	beq .L_0200a13c
	b .L_0200a142
.L_0200a134:
	movs r1, #10
	bl Animation_ApplyChildValues
	b .L_0200a142
.L_0200a13c:
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200a142:
	pop {pc}
.L_0200a144:
	.4byte Data_0300122c
	.section .text.x0200a148,"ax",%progbits
	.global Func_02002148
	.thumb_func
Func_02002148:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200a33c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3]
	sub sp, #16
	cmp r3, #0
	beq .L_0200a16a
	b .L_0200a328
.L_0200a16a:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_02001fcc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r2, .L_0200a340
	adds r6, r0, #0
	str r2, [sp, #0]
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	ldr r3, .L_0200a344
	adds r0, r6, #0
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	movs r1, #49
	bl Func_020041cc
.L_0200a1a2:
	adds r3, r6, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Func_0200422c
	ldr r3, [sp, #0]
	ldr r1, [sp, #0]
	adds r3, #104
	adds r1, #106
	mov r9, r1
	mov r10, r3
	add r1, sp, #4
	cmp r0, #7
	bne .L_0200a21e
	movs r2, #0
	ldrsh r3, [r3, r2]
	mov r1, r9
	lsls r3, r3, #17
	str r3, [r6, #36]
	movs r5, #0
	movs r0, #0
	ldrsh r3, [r1, r0]
	lsls r3, r3, #17
	str r3, [r6, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r6, #52]
.L_0200a1dc:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	add r1, sp, #4
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_0200424c
	cmp r0, #0
	beq .L_0200a210
	movs r3, #0
	str r3, [r6, #36]
	str r3, [r6, #44]
	b .L_0200a31c
.L_0200a210:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #9
	ble .L_0200a1dc
	b .L_0200a31c
.L_0200a21e:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_0200424c
	cmp r0, #0
	bgt .L_0200a31c
	cmp r0, #0
	bge .L_0200a268
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	ldr r3, .L_0200a33c
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3]
	movs r1, #6
	negs r2, r2
	bl Func_0200436c
	b .L_0200a31c
.L_0200a268:
	ldrh r3, [r6, #32]
	movs r2, #0
	subs r3, #2
	mov r11, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	mov r8, r2
	adds r7, r5, #0
	adds r7, #89
.L_0200a27c:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200a2ac
	ldrb r2, [r7]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200a2ac
	cmp r5, r6
	beq .L_0200a2ac
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	mov r1, r11
	add r2, sp, #4
	bl Func_0200428c
	cmp r0, #0
	blt .L_0200a2ac
	movs r0, #1
	bl WaitFrames
	b .L_0200a31c
.L_0200a2ac:
	movs r3, #1
	add r8, r3
	mov r0, r8
	adds r7, #128
	adds r5, #128
	cmp r0, #63
	ble .L_0200a27c
	mov r2, r10
	movs r1, #0
	ldrsh r3, [r2, r1]
	ldr r2, [r6, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r6, #8]
	mov r2, r9
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldr r2, [r6, #16]
	ldr r7, .L_0200a348
	lsls r3, r3, #17
	ldr r0, .L_0200a34c
	adds r5, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r5, [r6, #16]
	cmp r2, r4
	ble .L_0200a2ea
	adds r2, r4, #0
.L_0200a2ea:
	ldr r0, .L_0200a350
	cmp r2, r0
	bge .L_0200a2f2
	adds r2, r0, #0
.L_0200a2f2:
	subs r3, r1, r2
	ldr r1, .L_0200a34c
	str r3, [r6, #8]
	adds r3, r5, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_0200a304
	adds r2, r4, #0
.L_0200a304:
	cmp r2, r0
	bge .L_0200a30a
	adds r2, r0, #0
.L_0200a30a:
	subs r3, r5, r2
	str r3, [r6, #16]
	ldr r2, [sp, #0]
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl WaitFrames
	b .L_0200a1a2
.L_0200a31c:
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200a328:
	ldr r0, [sp, #0]
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a33c:
	.4byte gPartyState
.L_0200a340:
	.4byte Data_020023c4 + 0x188
.L_0200a344:
	.4byte Func_02002120
.L_0200a348:
	.4byte 0x000fffff
.L_0200a34c:
	.4byte 0xfff80000
.L_0200a350:
	.4byte 0xffff0000
	.section .text.x0200a354,"ax",%progbits
	.global Func_02002354
	.thumb_func
Func_02002354:
	push {lr}
	ldr r3, .L_0200a368
	movs r2, #4
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a364
	bl Func_02002148
.L_0200a364:
	pop {pc}
	.2byte 0x0000
.L_0200a368:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a36c,"ax",%progbits
	.global Func_0200236c
	.thumb_func
Func_0200236c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r1, #217
	lsls r1, r1, #1
	adds r6, r5, r1
	ldrh r3, [r6]
	sub sp, #12
	cmp r3, #0
	bne .L_0200a398
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r5, r2
	adds r1, #2
	ldr r0, [r3]
	adds r3, r5, r1
	ldr r1, [r3]
	bl Func_020043fc
	movs r3, #1
	strh r3, [r6]
.L_0200a398:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r2, #179
	lsls r2, r2, #1
	movs r1, #173
	adds r3, r5, r2
	lsls r1, r1, #1
	movs r2, #0
	strh r2, [r3]
	adds r3, r5, r1
	adds r1, #4
	strh r2, [r3]
	adds r3, r5, r1
	subs r1, #2
	strh r2, [r3]
	adds r3, r5, r1
	adds r1, #8
	strh r2, [r3]
	movs r0, #10
	adds r3, r5, r1
	strh r2, [r3]
	movs r1, #0
	negs r0, r0
	bl Func_02001fcc
	movs r0, #224
	movs r1, #224
	lsls r1, r1, #8
	lsls r0, r0, #11
	bl Func_020043c4
	ldr r3, .L_0200a49c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #131
	lsls r0, r0, #1
	ldr r7, .L_0200a4a0
	bl GameFlag_SetBit
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	ldr r3, .L_0200a4a4
	movs r1, #49
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r0, r6, #0
	bl Func_020041cc
	ldr r3, [r7, #108]
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200a484
.L_0200a424:
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #0
	bl Func_0200422c
	ldr r1, [r7, #108]
	cmp r0, #7
	beq .L_0200a444
	ldr r2, [r1, #112]
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r3, [r6, #16]
	ldr r2, [r1, #120]
	adds r3, r3, r2
	b .L_0200a46c
.L_0200a444:
	ldr r3, [r1, #112]
	cmp r3, #0
	beq .L_0200a458
	ldr r3, [r6, #8]
	ldr r2, .L_0200a4a8
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #8]
.L_0200a458:
	ldr r3, [r7, #108]
	ldr r3, [r3, #120]
	cmp r3, #0
	beq .L_0200a46e
	ldr r3, [r6, #16]
	ldr r2, .L_0200a4a8
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #12
	adds r3, r3, r1
.L_0200a46c:
	str r3, [r6, #16]
.L_0200a46e:
	movs r3, #0
	strh r3, [r7, #4]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #108]
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a424
.L_0200a484:
	movs r5, #0
	movs r0, #30
	bl Battle_WaitMode0
	adds r0, r6, #0
	str r5, [r6, #108]
	movs r1, #0
	bl Animation_ApplyChildValues
	strh r5, [r7, #4]
	add sp, #12
	pop {r5, r6, r7, pc}
.L_0200a49c:
	.4byte gPartyState
.L_0200a4a0:
	.4byte Data_020023c4 + 0x188
.L_0200a4a4:
	.4byte Func_02002120
.L_0200a4a8:
	.4byte 0xfff00000
	.section .text.x0200a4ac,"ax",%progbits
	.global Func_020024ac
	.thumb_func
Func_020024ac:
	push {lr}
	ldr r3, .L_0200a4c4
	movs r2, #4
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a4c0
	bl Func_0200236c
	movs r0, #1
	b .L_0200a4c2
.L_0200a4c0:
	movs r0, #0
.L_0200a4c2:
	pop {pc}
.L_0200a4c4:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a4c8,"ax",%progbits
	.global Func_020024c8
	.thumb_func
Func_020024c8:
	ldr r3, .L_0200a4d0
	movs r2, #4
	ldrsh r0, [r3, r2]
	bx lr
.L_0200a4d0:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a4d4,"ax",%progbits
	.global Func_020024d4
	.thumb_func
Func_020024d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200a63c
	sub sp, #4
	ldr r3, [r1, #112]
	mov r11, r0
	cmp r3, #0
	bne .L_0200a4f0
	b .L_0200a648
.L_0200a4f0:
	movs r2, #0
	str r2, [sp, #0]
.L_0200a4f4:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r3, r11
	ldr r3, [r3, #8]
	lsls r5, r5, #4
	mov r8, r3
	add r8, r5
	lsls r0, r0, #4
	mov r1, r8
	subs r1, r1, r0
	mov r8, r1
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r11
	ldr r3, [r2, #16]
	ldr r2, [r2, #12]
	lsls r5, r5, #4
	lsls r6, r6, #3
	movs r1, #128
	lsls r0, r0, #4
	adds r6, r6, r2
	lsls r1, r1, #11
	adds r3, r3, r5
	subs r3, r3, r0
	adds r6, r6, r1
	movs r0, #234
	adds r0, #255
	mov r1, r8
	adds r2, r6, #0
	bl Func_020041e4
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200a628
	bl Random16Far
	mov r10, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r2, #128
	lsls r2, r2, #8
	adds r5, r5, r0
	ldr r1, .L_0200a640
	adds r0, r7, #0
	mov r9, r2
	bl Func_020041dc
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r1, r10
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r1, #2
	adds r3, r3, r2
	str r3, [r7, #40]
	mov r0, r10
	bl Math_Cosine
	ldr r3, .L_0200a644
	lsls r6, r6, #3
	mov r8, r3
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r10
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r2, .L_0200a63c
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	lsrs r5, r5, #2
	ldr r3, [r2, #112]
	add r5, r9
	movs r6, #0
	mov r1, r9
	str r5, [r7, #24]
	str r5, [r7, #28]
	str r1, [r7, #68]
	ldr r5, [r7, #80]
	str r0, [r7, #36]
	str r6, [r7, #52]
	ldr r3, [r3, #80]
	ldrb r0, [r5, #16]
	mov r8, r3
	bl Resource_ResetEntry
	ldrb r3, [r5, #17]
	ldr r1, .L_0200a63c
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #17]
	ldrh r3, [r1, #12]
	ldr r0, [r5, #40]
	strb r3, [r5, #16]
	bl ResourceMetadata_ClearRecord
	str r6, [r5, #40]
	strb r6, [r5, #27]
	mov r2, r8
	ldrb r3, [r2, #20]
	ldrb r0, [r5, #5]
	strb r3, [r5, #20]
	ldrb r3, [r2, #21]
	strb r3, [r5, #21]
	ldrb r1, [r2, #5]
	movs r2, #63
	adds r3, r2, #0
	lsrs r1, r1, #6
	lsls r1, r1, #6
	ands r3, r0
	orrs r3, r1
	strb r3, [r5, #5]
	mov r1, r8
	ldrb r3, [r1, #7]
	ldrb r1, [r5, #7]
	lsrs r3, r3, #6
	lsls r3, r3, #6
	ands r2, r1
	orrs r2, r3
	strb r2, [r5, #7]
	mov r3, r8
	ldrh r2, [r3, #8]
	ldr r1, .L_0200a638
	ldrh r3, [r5, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
.L_0200a628:
	ldr r1, [sp, #0]
	subs r1, #1
	str r1, [sp, #0]
	cmp r1, #0
	blt .L_0200a634
	b .L_0200a4f4
.L_0200a634:
	b .L_0200a648
	.2byte 0x0000
.L_0200a638:
	.4byte 0xfffffc00
.L_0200a63c:
	.4byte Data_020023c4 + 0x188
.L_0200a640:
	.4byte Data_02004774
.L_0200a644:
	.4byte IwramMulQ16
.L_0200a648:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a658,"ax",%progbits
	.global Func_02002658
	.thumb_func
Func_02002658:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r0, [sp, #12]
	str r1, [sp, #8]
	str r2, [sp, #4]
	str r3, [sp, #0]
	movs r3, #1
	mov r11, r3
.L_0200a674:
	movs r0, #70
	adds r0, #255
	ldr r1, [sp, #12]
	ldr r2, [sp, #8]
	ldr r3, [sp, #4]
	bl Func_020041e4
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200a716
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	ldr r3, [sp, #0]
	lsrs r5, r5, #4
	adds r5, r3, r5
	lsrs r0, r0, #4
	movs r3, #128
	subs r5, r5, r0
	lsls r3, r3, #7
	mov r10, r3
	adds r3, r5, #0
	add r3, r10
	mov r9, r3
	bl Random16Far
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #3
	mov r8, r3
	bl Random16Far
	ldr r1, .L_0200a730
	adds r6, r0, #0
	adds r0, r7, #0
	bl Func_020041dc
	movs r1, #0
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #160
	lsls r3, r3, #9
	adds r5, r5, r3
	str r5, [r7, #40]
	mov r0, r9
	bl Math_Cosine
	ldr r5, .L_0200a734
	adds r1, r0, #0
	mov r0, r8
	mov lr, r5
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r9
	bl Math_Sine
	adds r1, r0, #0
	mov r0, r8
	mov lr, r5
	.2byte 0xf800
	movs r3, #0
	str r3, [r7, #52]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	lsrs r6, r6, #1
	str r3, [r7, #72]
	movs r3, #128
	add r6, r10
	lsls r3, r3, #8
	str r0, [r7, #36]
	str r6, [r7, #24]
	str r6, [r7, #28]
	str r3, [r7, #68]
	adds r0, r7, #0
	movs r1, #1
	bl Animation_ApplyChildValues
.L_0200a716:
	movs r3, #1
	negs r3, r3
	add r11, r3
	mov r3, r11
	cmp r3, #0
	bge .L_0200a674
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a730:
	.4byte Data_020047b8
.L_0200a734:
	.4byte IwramMulQ16
	.section .text.x0200a738,"ax",%progbits
	.global Func_02002738
	.thumb_func
Func_02002738:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	adds r5, #91
	strb r0, [r5]
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200a74c,"ax",%progbits
	.global Func_0200274c
	.thumb_func
Func_0200274c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r3, #192
	movs r0, #100
	lsls r3, r3, #18
	adds r0, r0, r5
	ldr r6, [r3, #108]
	movs r1, #0
	ldrsh r3, [r0, r1]
	sub sp, #56
	mov r8, r0
	cmp r3, #0
	beq .L_0200a774
	b .L_0200aa00
.L_0200a774:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_0200a78e
	movs r3, #1
	orrs r0, r3
.L_0200a78e:
	adds r3, r5, #0
	adds r3, #91
	strb r0, [r3]
	add r7, sp, #44
	ldr r3, [r5, #8]
	movs r0, #128
	str r3, [r7]
	lsls r0, r0, #12
	ldr r3, [r5, #12]
	adds r2, r7, #0
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	str r3, [r7, #8]
	ldrh r1, [r5, #6]
	bl Vector_AddPolarOffsetFar
	ldr r1, [r7]
	ldr r2, [r7, #8]
	movs r0, #0
	bl Func_0200422c
	cmp r0, #7
	bne .L_0200a804
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #64]
	str r3, [r5, #60]
	str r3, [r5, #56]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	mov r0, r8
	movs r3, #1
	strh r3, [r0]
	movs r0, #145
	bl Func_0200446c
	movs r0, #160
	lsls r0, r0, #11
	movs r2, #128
	adds r1, r0, #0
	lsls r2, r2, #9
	bl Func_02004264
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004264
	ldr r3, [r5, #104]
	cmp r3, #0
	beq .L_0200a804
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
.L_0200a804:
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	ldr r3, [r5, #8]
	ldr r1, [r5, #16]
	asrs r3, r3, #20
	str r3, [sp, #32]
	movs r3, #184
	lsls r3, r3, #1
	ldr r4, [sp, #32]
	asrs r1, r1, #20
	adds r2, r0, r3
	ldr r2, [r2]
	lsls r3, r1, #7
	adds r3, r4, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	str r2, [sp, #28]
	movs r4, #212
	lsls r4, r4, #1
	adds r2, r0, r4
	ldr r2, [r2]
	subs r4, #92
	adds r2, r2, r3
	str r2, [sp, #24]
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r0, r2
	ldr r3, [r3]
	asrs r3, r3, #20
	str r3, [sp, #20]
	adds r3, r0, r4
	adds r4, #52
	ldr r2, [r3]
	adds r3, r0, r4
	ldr r3, [r3]
	adds r4, #4
	asrs r3, r3, #20
	str r3, [sp, #16]
	adds r3, r0, r4
	ldr r3, [r3]
	movs r0, #1
	asrs r3, r3, #20
	adds r3, r1, r3
	subs r3, #2
	asrs r2, r2, #20
	negs r0, r0
	str r3, [sp, #8]
	str r0, [sp, #40]
	subs r3, r1, #1
	adds r1, r1, r2
	subs r1, #1
	mov r8, r3
	mov r11, r1
.L_0200a870:
	ldr r0, [sp, #32]
	ldr r1, [sp, #16]
	ldr r2, [sp, #20]
	movs r4, #1
	adds r3, r0, r1
	subs r3, #1
	negs r4, r4
	mov r9, r3
	str r4, [sp, #36]
	adds r3, r0, r2
	adds r6, r0, #0
	subs r3, #1
	subs r6, #1
	mov r10, r3
.L_0200a88c:
	ldr r4, [sp, #40]
	ldr r0, [sp, #36]
	lsls r3, r4, #7
	adds r3, r3, r0
	lsls r3, r3, #2
	ldr r1, [sp, #28]
	str r3, [sp, #12]
	adds r2, r3, r1
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200a8ec
	movs r3, #0
	strb r3, [r2, #2]
	mov r2, r10
	mov r3, r11
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #1
	bl Func_02004244
	mov r4, r8
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r4, [sp, #4]
	str r6, [sp, #0]
	bl Func_0200423c
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_0200446c
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_02002658
.L_0200a8ec:
	ldr r4, [sp, #12]
	ldr r0, [sp, #24]
	adds r2, r4, r0
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200a940
	movs r3, #0
	strb r3, [r2, #2]
	ldr r2, [sp, #8]
	mov r1, r9
	str r1, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #2
	bl Func_02004244
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r6, [sp, #0]
	bl Func_0200423c
	movs r0, #143
	lsls r0, r0, #2
	bl Func_0200446c
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_02002658
.L_0200a940:
	ldr r0, [sp, #36]
	movs r4, #1
	adds r0, #1
	add r9, r4
	adds r6, #1
	add r10, r4
	str r0, [sp, #36]
	cmp r0, #1
	ble .L_0200a88c
	ldr r1, [sp, #8]
	ldr r2, [sp, #40]
	adds r1, #1
	adds r2, #1
	str r1, [sp, #8]
	add r8, r4
	add r11, r4
	str r2, [sp, #40]
	cmp r2, #1
	ble .L_0200a870
	ldr r3, [r5, #24]
	movs r4, #128
	lsls r4, r4, #9
	cmp r3, r4
	bge .L_0200a97e
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
.L_0200a97e:
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r5, #0
	bl Func_02004204
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	ldr r3, .L_0200aa44
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	ldr r6, .L_0200aa48
	bl Object_GetById
	ldr r1, [r5, #8]
	ldr r3, [r0, #8]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_0200a9b4
	movs r1, #160
	lsls r1, r1, #13
	cmp r2, r1
	blt .L_0200a9be
	b .L_0200aa34
.L_0200a9b4:
	movs r2, #160
	subs r3, r3, r1
	lsls r2, r2, #13
	cmp r3, r2
	bge .L_0200aa34
.L_0200a9be:
	ldr r3, [r5, #12]
	ldr r2, [r0, #12]
	ldr r4, .L_0200aa4c
	ldr r1, .L_0200aa50
	subs r3, r3, r2
	adds r3, r3, r4
	cmp r3, r1
	bhi .L_0200aa34
	ldr r3, [r5, #16]
	ldr r0, [r0, #16]
	subs r2, r3, r0
	cmp r2, #0
	blt .L_0200a9e2
	movs r3, #160
	lsls r3, r3, #13
	cmp r2, r3
	blt .L_0200a9ec
	b .L_0200aa34
.L_0200a9e2:
	movs r4, #160
	subs r3, r0, r3
	lsls r4, r4, #13
	cmp r3, r4
	bge .L_0200aa34
.L_0200a9ec:
	movs r3, #2
	strh r3, [r6, #4]
	ldrh r3, [r6, #10]
	movs r0, #170
	lsls r0, r0, #1
	adds r3, #2
	adds r2, r7, r0
	str r5, [r6, #108]
	strh r3, [r2]
	b .L_0200aa34
.L_0200aa00:
	cmp r3, #1
	bne .L_0200aa34
	adds r3, r5, #0
	adds r3, #91
	movs r2, #0
	strb r2, [r3]
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_0200aa26
	ldr r2, .L_0200aa54
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
	bl Func_020024d4
	b .L_0200aa34
.L_0200aa26:
	str r2, [r5, #16]
	str r2, [r5, #12]
	str r2, [r5, #8]
	str r2, [r5, #44]
	str r2, [r5, #40]
	str r2, [r5, #36]
	str r2, [r5, #108]
.L_0200aa34:
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200aa44:
	.4byte gPartyState
.L_0200aa48:
	.4byte Data_020023c4 + 0x188
.L_0200aa4c:
	.4byte 0x0007ffff
.L_0200aa50:
	.4byte 0x001ffffe
.L_0200aa54:
	.4byte 0xfffff000
	.section .text.x0200aa58,"ax",%progbits
	.global Func_02002a58
	.thumb_func
Func_02002a58:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	ldr r6, [sp, #24]
	adds r5, r1, #0
	mov r9, r2
	mov r10, r3
	bl Object_GetById
	mov r8, r0
	adds r0, r5, #0
	bl Object_GetById
	adds r5, r0, #0
	mov r0, r8
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_020041dc
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r5, #0
	adds r3, #85
	movs r6, #0
	strb r6, [r3]
	adds r0, r5, #0
	movs r1, #1
	bl Func_020041cc
	adds r3, r5, #0
	mov r2, r8
	adds r3, #100
	str r2, [r5, #104]
	mov r0, r10
	strh r6, [r3]
	adds r3, #2
	strh r0, [r3]
	mov r2, r9
	subs r3, #4
	strb r2, [r3]
	ldr r3, .L_0200aacc
	str r3, [r5, #108]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200aacc:
	.4byte Func_02002738
	.section .text.x0200aad0,"ax",%progbits
	.global Func_02002ad0
	.thumb_func
Func_02002ad0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	mov r3, r11
	adds r3, #98
	ldrb r0, [r3]
	sub sp, #12
	bl Object_GetById
	mov r3, r11
	adds r7, r0, #0
	adds r3, #102
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r3, [r7, #80]
	mov r2, r11
	mov r9, r3
	ldr r3, [r2, #8]
	mov r5, sp
	str r3, [r5]
	movs r0, #128
	ldr r3, [r2, #12]
	ldr r2, .L_0200ab60
	adds r1, r6, #0
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r2, r11
	ldr r3, [r2, #16]
	lsls r0, r0, #14
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	ldr r3, .L_0200ab5c
	movs r2, #0
	mov r10, r3
	adds r3, r7, #0
	mov r8, r2
	adds r3, #85
	mov r2, r10
	strh r6, [r7, #6]
	strb r2, [r3]
	adds r0, r7, #0
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_0200ab64
	mov r2, r10
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #90
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r7, #52]
	ldr r3, .L_0200ab68
	mov r2, r9
	adds r6, r6, r3
	b .L_0200ab6c
	.2byte 0x0000
.L_0200ab5c:
	.4byte 0x00000000
.L_0200ab60:
	.4byte 0xfff40000
.L_0200ab64:
	.4byte Func_0200274c
.L_0200ab68:
	.4byte 0xffffc000
.L_0200ab6c:
	mov r3, r8
	strh r6, [r2, #18]
	str r3, [r7, #24]
	str r3, [r7, #28]
	adds r3, r7, #0
	mov r2, r8
	adds r3, #100
	strh r2, [r3]
	mov r2, r11
	ldr r3, [r2, #104]
	movs r0, #104
	str r3, [r7, #104]
	bl Func_0200446c
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200ab98,"ax",%progbits
	.global Func_02002b98
	.thumb_func
Func_02002b98:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #100
	movs r3, #0
	ldrsh r6, [r7, r3]
	cmp r6, #0
	bne .L_0200abd0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #128
	lsls r3, r3, #13
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #9
	str r0, [r5, #12]
	movs r1, #1
	adds r0, r5, #0
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r3, [r5, #48]
	str r3, [r5, #52]
	bl Func_0200427c
	b .L_0200ac38
.L_0200abd0:
	cmp r6, #30
	bgt .L_0200abe8
	cmp r6, #30
	bne .L_0200ac38
	movs r0, #136
	bl Func_0200446c
	ldr r0, [r5, #104]
	movs r1, #2
	bl Func_020041cc
	b .L_0200ac38
.L_0200abe8:
	cmp r6, #60
	bgt .L_0200ac10
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldrh r3, [r7]
	ldr r0, [r5, #104]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	ldr r3, .L_0200ac34
	asrs r2, r2, #1
	ands r2, r3
	lsls r1, r2, #3
	subs r1, r1, r2
	bl Animation_ApplyChildValues
	b .L_0200ac38
.L_0200ac10:
	movs r1, #1
	ldr r0, [r5, #104]
	bl Func_020041cc
	movs r0, #184
	bl Func_0200446c
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #0
	str r0, [r5, #12]
	str r3, [r5, #104]
	movs r0, #0
	b .L_0200ac40
	.2byte 0x0000
.L_0200ac34:
	.4byte 0x00000001
.L_0200ac38:
	ldrh r3, [r7]
	movs r0, #1
	adds r3, #1
	strh r3, [r7]
.L_0200ac40:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200ac44,"ax",%progbits
	.global Func_02002c44
	.thumb_func
Func_02002c44:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #100
	movs r3, #0
	ldrsh r6, [r7, r3]
	cmp r6, #0
	bne .L_0200ac8e
	movs r0, #136
	bl Func_0200446c
	ldr r0, [r5, #104]
	movs r1, #2
	bl Func_020041cc
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #128
	lsls r3, r3, #13
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	movs r3, #128
	str r0, [r5, #12]
	lsls r3, r3, #8
	adds r0, r5, #0
	movs r1, #1
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r3, [r5, #52]
	bl Func_0200427c
	b .L_0200acdc
.L_0200ac8e:
	cmp r6, #32
	bgt .L_0200acb6
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldrh r3, [r7]
	ldr r0, [r5, #104]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	ldr r3, .L_0200acd8
	asrs r2, r2, #1
	ands r2, r3
	lsls r1, r2, #3
	subs r1, r1, r2
	bl Animation_ApplyChildValues
	b .L_0200acdc
.L_0200acb6:
	movs r1, #1
	ldr r0, [r5, #104]
	bl Func_020041cc
	movs r0, #184
	bl Func_0200446c
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #0
	str r0, [r5, #12]
	str r3, [r5, #104]
	movs r0, #0
	b .L_0200ace4
.L_0200acd8:
	.4byte 0x00000001
.L_0200acdc:
	ldrh r3, [r7]
	movs r0, #1
	adds r3, #1
	strh r3, [r7]
.L_0200ace4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200ace8,"ax",%progbits
	.global Func_02002ce8
	.thumb_func
Func_02002ce8:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02002ad0
	movs r3, #0
	str r3, [r5, #8]
	str r3, [r5, #12]
	str r3, [r5, #16]
	str r3, [r5, #36]
	str r3, [r5, #40]
	str r3, [r5, #44]
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200ad04,"ax",%progbits
	.global Func_02002d04
	.thumb_func
Func_02002d04:
	push {r5, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	movs r0, #0
	ldrsh r1, [r2, r0]
	ldrh r3, [r2]
	cmp r1, #0
	beq .L_0200ad1c
	subs r3, #1
	strh r3, [r2]
	b .L_0200ad82
.L_0200ad1c:
	adds r3, r5, #0
	adds r3, #90
	movs r0, #131
	strb r1, [r3]
	lsls r0, r0, #1
	bl GameFlag_Test
	movs r3, #1
	negs r3, r3
	cmp r0, #0
	bne .L_0200ad42
	ldr r3, .L_0200ad84
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200ad88
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
.L_0200ad42:
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_0200ad54
	adds r0, r5, #0
	movs r1, #9
	bl Func_020041cc
	b .L_0200ad82
.L_0200ad54:
	ldrh r1, [r5, #6]
	movs r2, #128
	subs r3, r3, r1
	lsls r3, r3, #16
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_0200ad66
	adds r3, r2, #0
.L_0200ad66:
	ldr r2, .L_0200ad8c
	cmp r3, r2
	bge .L_0200ad6e
	adds r3, r2, #0
.L_0200ad6e:
	adds r3, r1, r3
	adds r0, r5, #0
	movs r1, #2
	strh r3, [r5, #6]
	bl Func_020041cc
	adds r0, r5, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
.L_0200ad82:
	pop {r5, pc}
.L_0200ad84:
	.4byte gInput
.L_0200ad88:
	.4byte Data_020047fc
.L_0200ad8c:
	.4byte 0xfffff000
	.section .text.x0200ad90,"ax",%progbits
	.global Func_02002d90
	.thumb_func
Func_02002d90:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #162
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200adc0
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_020043f4
	bl Func_02004444
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
.L_0200adc0:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200adc4,"ax",%progbits
	.global Func_02002dc4
	.thumb_func
Func_02002dc4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200ae84
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	adds r7, r0, #0
.L_0200ade4:
	bl Func_02002d90
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_0200ae88
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_02004224
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_0200ae8c
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_0200ae9c
	ldr r3, .L_0200ae90
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200ae94
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_0200ae98
	cmp r3, r2
	bne .L_0200aec8
	b .L_0200b05e
.L_0200ae84:
	.4byte gPartyState
.L_0200ae88:
	.4byte 0xfff00000
.L_0200ae8c:
	.4byte IwramMulQ16
.L_0200ae90:
	.4byte gInput
.L_0200ae94:
	.4byte Data_0200483c
.L_0200ae98:
	.4byte 0xffff0000
.L_0200ae9c:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_0200aec4
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_0200aec8
.L_0200aec4:
	.4byte 0xffffc000
.L_0200aec8:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_02004224
	mov r11, r0
	cmp r0, #255
	beq .L_0200af4a
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200af4a
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	ldr r2, [r7, #12]
	bl Func_02004204
	adds r0, r7, #0
	movs r1, #2
	bl Func_020041cc
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	adds r0, r7, #0
	bl Func_0200420c
	ldr r3, .L_0200b06c
	str r3, [r7, #108]
	b .L_0200aff4
.L_0200af4a:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_0200b040
.L_0200af5e:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200b014
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_0200af8c:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200afb6
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200afb6
	cmp r5, r7
	beq .L_0200afb6
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_0200428c
	cmp r0, #0
	bge .L_0200b014
.L_0200afb6:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_0200af8c
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	ldr r3, [r6, #8]
	ldr r1, [r6]
	ldr r2, [r6, #4]
	bl Func_02004204
	adds r0, r7, #0
	bl Func_0200420c
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_0200b03a
.L_0200aff4:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_02004224
	mov r11, r0
	cmp r0, #255
	bne .L_0200af5e
.L_0200b014:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02004204
	adds r0, r7, #0
	bl Func_0200420c
	movs r0, #2
	bl WaitFrames
	b .L_0200ade4
.L_0200b03a:
	movs r0, #10
	bl WaitFrames
.L_0200b040:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_020041cc
.L_0200b05e:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b06c:
	.4byte Func_02002d04
	.section .text.x0200b070,"ax",%progbits
	.global Func_02003070
	.thumb_func
Func_02003070:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200b0d0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_0200b0d4
	ldr r3, .L_0200b0cc
	adds r7, r0, #0
	strh r3, [r2]
.L_0200b096:
	bl Func_02002d90
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_0200b0d8
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	b .L_0200b0dc
.L_0200b0cc:
	.4byte 0x00000000
.L_0200b0d0:
	.4byte gPartyState
.L_0200b0d4:
	.4byte Data_02004cf0
.L_0200b0d8:
	.4byte 0xfff00000
.L_0200b0dc:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_02004224
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_0200b148
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_0200b158
	ldr r3, .L_0200b14c
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200b150
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_0200b154
	cmp r3, r2
	bne .L_0200b184
	b .L_0200b34e
.L_0200b148:
	.4byte IwramMulQ16
.L_0200b14c:
	.4byte gInput
.L_0200b150:
	.4byte Data_0200483c
.L_0200b154:
	.4byte 0xffff0000
.L_0200b158:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_0200b180
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_0200b184
.L_0200b180:
	.4byte 0xffffc000
.L_0200b184:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_02004224
	mov r11, r0
	cmp r0, #255
	beq .L_0200b1fe
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200b1fe
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r2, [r7, #12]
	ldr r3, [sp, #8]
	bl Func_02004204
	adds r0, r7, #0
	movs r1, #2
	bl Func_020041cc
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	movs r5, #0
	b .L_0200b226
.L_0200b1fe:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_0200b34e
.L_0200b212:
	ldr r3, .L_0200b37c
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200b21e
	b .L_0200b34e
.L_0200b21e:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_0200b226:
	cmp r5, #179
	bgt .L_0200b234
	adds r0, r7, #0
	bl Func_02004284
	cmp r0, #0
	beq .L_0200b212
.L_0200b234:
	ldr r3, .L_0200b380
	str r3, [r7, #108]
	b .L_0200b302
.L_0200b23a:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200b322
	ldr r3, .L_0200b37c
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200b34e
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_0200b272:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200b29c
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200b29c
	cmp r5, r7
	beq .L_0200b29c
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_0200428c
	cmp r0, #0
	bge .L_0200b322
.L_0200b29c:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_0200b272
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_02004204
	b .L_0200b2da
.L_0200b2d2:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_0200b2da:
	cmp r5, #179
	bgt .L_0200b2f2
	adds r0, r7, #0
	bl Func_02004284
	cmp r0, #0
	bne .L_0200b2f2
	ldr r3, .L_0200b37c
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200b2d2
.L_0200b2f2:
	ldr r3, .L_0200b37c
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200b34e
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_0200b348
.L_0200b302:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_02004224
	mov r11, r0
	cmp r0, #255
	bne .L_0200b23a
.L_0200b322:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02004204
	adds r0, r7, #0
	bl Func_0200420c
	movs r0, #2
	bl WaitFrames
	b .L_0200b096
.L_0200b348:
	movs r0, #10
	bl WaitFrames
.L_0200b34e:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_020041cc
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b37c:
	.4byte Data_02004cf0
.L_0200b380:
	.4byte Func_02002d04
	.section .text.x0200b384,"ax",%progbits
	.global Func_02003384
	.thumb_func
Func_02003384:
	ldr r3, .L_0200b38c
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_0200b38c:
	.4byte Data_02004cf0
	.section .text.x0200b390,"ax",%progbits
	.global Func_02003390
	.thumb_func
Func_02003390:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200b3fc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #32
	bl ObjectTable_Get
	adds r5, r0, #0
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_0200b3f8
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r3, [sp, #16]
.L_0200b3c2:
	bl Func_02002d90
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #64]
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	ldr r2, .L_0200b400
	ldr r3, [r5, #8]
	movs r1, #128
	lsls r1, r1, #12
	ands r3, r2
	mov r9, r1
	add r6, sp, #20
	add r3, r9
	str r3, [r6]
	mov r8, r3
	b .L_0200b404
	.2byte 0x0000
.L_0200b3f8:
	.4byte 0xffffc000
.L_0200b3fc:
	.4byte gPartyState
.L_0200b400:
	.4byte 0xfff00000
.L_0200b404:
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r2
	adds r7, r3, r1
	mov r2, r8
	str r7, [r6, #8]
	str r2, [sp, #8]
	str r7, [sp, #4]
	movs r3, #34
	adds r3, r3, r5
	ldrb r0, [r3]
	adds r1, r2, #0
	adds r2, r7, #0
	mov r11, r3
	bl Func_02004224
	str r0, [sp, #12]
	movs r0, #128
	ldr r1, [sp, #16]
	lsls r0, r0, #13
	adds r2, r6, #0
	bl Vector_AddPolarOffsetFar
	mov r1, r11
	ldrb r0, [r1]
	ldr r2, [r6, #8]
	ldr r1, [r6]
	bl Func_02004224
	mov r10, r0
	cmp r0, #255
	beq .L_0200b498
	mov r2, r11
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	subs r0, r0, r3
	cmp r0, r9
	bgt .L_0200b498
	ldr r3, [sp, #8]
	ldr r2, .L_0200b490
	str r3, [r6]
	ldr r1, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r1, [r6, #8]
	str r3, [r5, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #52]
	adds r3, r5, #0
	adds r3, #100
	strh r2, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl Func_020041cc
	adds r0, r5, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	ldr r3, .L_0200b494
	str r3, [r5, #108]
	b .L_0200b542
	.2byte 0x0000
.L_0200b490:
	.4byte 0x00000000
.L_0200b494:
	.4byte Func_02002d04
.L_0200b498:
	add r1, sp, #16
	ldrh r1, [r1]
	movs r3, #0
	mov r2, r8
	strh r1, [r5, #6]
	str r3, [r5, #36]
	str r3, [r5, #44]
	str r2, [r5, #8]
	str r7, [r5, #16]
	b .L_0200b58e
.L_0200b4ac:
	mov r3, r11
	ldrb r0, [r3]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	movs r1, #128
	subs r0, r0, r3
	lsls r1, r1, #12
	cmp r0, r1
	bgt .L_0200b562
	ldrh r3, [r5, #32]
	movs r2, #0
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #20]
	movs r3, #89
	adds r3, r3, r6
	mov r9, r2
	mov r8, r3
.L_0200b4da:
	ldr r3, [r6]
	cmp r3, #0
	beq .L_0200b504
	mov r1, r8
	ldrb r2, [r1]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200b504
	cmp r6, r5
	beq .L_0200b504
	ldrh r3, [r6, #32]
	adds r0, r6, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #20
	bl Func_0200428c
	cmp r0, #0
	bge .L_0200b562
.L_0200b504:
	movs r2, #1
	add r9, r2
	movs r3, #128
	mov r1, r9
	add r8, r3
	adds r6, #128
	cmp r1, #63
	ble .L_0200b4da
	ldr r2, [r7]
	adds r0, r5, #0
	str r2, [sp, #8]
	ldr r3, [r7, #8]
	str r3, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl Func_02004204
	adds r0, r5, #0
	bl Func_0200420c
	ldr r1, [sp, #12]
	cmp r10, r1
	bne .L_0200b588
.L_0200b542:
	movs r0, #128
	ldr r1, [sp, #16]
	add r2, sp, #20
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
	mov r2, r11
	add r7, sp, #20
	ldrb r0, [r2]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Func_02004224
	mov r10, r0
	cmp r0, #255
	bne .L_0200b4ac
.L_0200b562:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #52]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	ldr r1, [sp, #8]
	ldr r3, [sp, #4]
	bl Func_02004204
	adds r0, r5, #0
	bl Func_0200420c
	movs r0, #2
	bl WaitFrames
	b .L_0200b3c2
.L_0200b588:
	movs r0, #10
	bl WaitFrames
.L_0200b58e:
	movs r3, #0
	str r3, [r5, #108]
	adds r1, r5, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	adds r0, r5, #0
	movs r1, #1
	bl Func_020041cc
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b602,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200b604,"ax",%progbits
	.global Func_02003604
	.thumb_func
Func_02003604:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	ldr r1, [r0, #80]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #72]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #76]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r2, [r0, #48]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [r0, #52]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	adds r0, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r0]
	adds r3, r3, r2
	strh r3, [r1, #18]
	bx lr
	.2byte 0x0000
	.section .text.x0200b63c,"ax",%progbits
	.global Func_0200363c
	.thumb_func
Func_0200363c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_0200b7f4
	sub sp, #4
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r8, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r8
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_0200b684
	cmp r7, #0
	beq .L_0200b684
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_0200b68c
.L_0200b684:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_0200b68c:
	mov r3, r10
	bl Func_020041e4
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200b69a
	b .L_0200b7e6
.L_0200b69a:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_020041cc
	ldr r2, .L_0200b7f8
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_020041dc
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_0200b7fc
	mov r1, r9
	str r3, [r6, #108]
	mov r3, r11
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	adds r0, r6, #0
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Object_SetSpritePriority
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_0200b800
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200b7e6
	cmp r7, #0
	beq .L_0200b7e6
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200b71c
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200b71c:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200b73c
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_0200b73c:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_0200b750
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200b750:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200b796
	ldr r3, .L_0200b7f8
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_0200b77e
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200b790
.L_0200b77e:
	ldr r2, .L_0200b800
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200b800
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200b790:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_0200b796:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200b7b2
	adds r0, r6, #0
	movs r1, #1
	bl Func_020041cc
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_020041dc
.L_0200b7b2:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200b7c4
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_0200b7c4:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200b7d6
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200b7d6:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200b7e6
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200b7e6:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b7f4:
	.4byte gPartyState
.L_0200b7f8:
	.4byte Data_02004ce0
.L_0200b7fc:
	.4byte Func_02003604
.L_0200b800:
	.4byte 0xffff0000
	.section .text.x0200b804,"ax",%progbits
	.global Func_02003804
	.thumb_func
Func_02003804:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_0200b91c
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_0200b910
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_0200b920
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	str r4, [sp, #0]
	bl Object_GetById
	mov r1, r8
	ldr r3, [r0, #8]
	movs r5, #0
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	ldr r4, [sp, #0]
	cmp r3, r2
	bne .L_0200b850
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_0200b858
.L_0200b850:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_0200b858:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_0200b924
	cmp r3, r2
	beq .L_0200b910
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_0200b910
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	str r4, [sp, #0]
	adds r3, r2, #0
	adds r3, #228
	ldr r0, [r3]
	ldr r5, [r3, #4]
	ldr r3, [r2]
	ands r0, r1
	ands r5, r1
	ldr r6, [r3, #4]
	movs r1, #16
	ldrsh r3, [r4, r1]
	ldr r2, .L_0200b928
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r3, r3, #5
	mov r10, r3
	mov r3, r8
	movs r2, #0
	ldrsh r1, [r3, r2]
	lsls r1, r1, #20
	subs r7, r1, r0
	movs r0, #2
	ldrsh r2, [r3, r0]
	movs r0, #0
	lsls r2, r2, #20
	bl Map_GetTerrainHeight
	mov r2, r8
	movs r1, #2
	ldrsh r3, [r2, r1]
	subs r0, r0, r6
	lsls r3, r3, #20
	subs r3, r3, r5
	subs r3, r3, r6
	subs r2, r3, r0
	asrs r7, r7, #16
	adds r0, r0, r3
	asrs r0, r0, #16
	adds r3, r7, #0
	movs r5, #167
	asrs r2, r2, #16
	adds r1, r0, #0
	adds r3, #15
	lsls r5, r5, #1
	adds r2, #14
	adds r1, #58
	ldr r4, [sp, #0]
	cmp r3, r5
	bhi .L_0200b910
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_0200b910
	cmp r2, #239
	bgt .L_0200b910
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r7, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r4, #20]
	lsls r3, r7, #16
	orrs r2, r3
	ldr r3, .L_0200b92c
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_0200418c
.L_0200b910:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b91c:
	.4byte Data_02004cf4
.L_0200b920:
	.4byte gPartyState
.L_0200b924:
	.4byte 0xffff0000
.L_0200b928:
	.4byte ResourceTableEntries
.L_0200b92c:
	.4byte 0x80008800
	.section .text.x0200b930,"ax",%progbits
	.global Func_02003930
	.thumb_func
Func_02003930:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200bb60
	str r1, [sp, #40]
	mov r8, r0
	movs r1, #32
	add r1, r8
	mov r9, r1
	mov r12, r9
	adds r5, r2, #0
	mov r2, r12
	adds r6, r3, #0
	str r2, [sp, #8]
	ldr r3, .L_0200bb64
	movs r1, #4
	ldr r7, [sp, #80]
	mov lr, r3
	.2byte 0xf800
	add r0, sp, #44
	ldrh r0, [r0]
	mov r1, r8
	strh r0, [r1, #4]
	add r1, sp, #40
	ldrh r1, [r1]
	mov r3, r8
	strh r1, [r3]
	strh r5, [r3, #2]
	movs r3, #255
	lsls r3, r3, #8
	mov r5, r8
	mov r0, r8
	adds r3, #255
	mov r1, r8
	strh r6, [r5, #6]
	movs r2, #0
	strh r7, [r0, #8]
	strh r3, [r1, #12]
	mov r3, r8
	strh r2, [r3, #10]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	mov r12, r3
	lsls r2, r2, #1
	mov r1, r12
	add r2, r12
	adds r1, #236
	ldr r0, [r1]
	ldr r3, [r2, #8]
	ldr r5, [r2, #48]
	adds r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #32]
	adds r1, #4
	ldr r3, [r2, #12]
	ldr r2, [r1]
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [sp, #28]
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #24]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r2
	asrs r2, r2, #20
	lsls r2, r2, #7
	adds r2, r2, r0
	lsls r2, r2, #2
	asrs r3, r3, #20
	adds r5, r5, r2
	movs r0, #0
	str r3, [sp, #20]
	str r5, [sp, #36]
	str r0, [sp, #12]
	cmp r0, r3
	bge .L_0200bab4
.L_0200b9e4:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_0200baa8
.L_0200b9f8:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_0200ba98
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_0200ba98
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_0200ba98
	ldr r2, [sp, #16]
	ldr r3, [sp, #32]
	mov r0, r9
	adds r7, r2, r3
	strh r7, [r0]
	ldr r1, [sp, #12]
	ldr r2, [sp, #28]
	add r0, sp, #40
	ldrh r0, [r0]
	adds r6, r1, r2
	mov r3, r9
	mov r1, r9
	strh r6, [r3, #2]
	strh r0, [r1, #4]
	ldr r1, [sp, #40]
	movs r0, #10
	adds r1, #1
	adds r0, #255
	str r1, [sp, #40]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ba4c
	cmp r5, r10
	bne .L_0200ba8a
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_0200ba8a
.L_0200ba4c:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ba8a
	mov r2, r8
	ldrh r4, [r2, #6]
	ldrh r5, [r2, #8]
	movs r3, #8
	ldrsh r1, [r2, r3]
	movs r3, #6
	ldrsh r0, [r2, r3]
	movs r2, #64
	adds r3, r2, #0
	ands r3, r4
	ands r2, r5
	lsls r3, r3, #16
	lsls r2, r2, #16
	asrs r3, r3, #16
	asrs r2, r2, #16
	orrs r7, r3
	orrs r6, r2
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004234
.L_0200ba8a:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_0200ba98:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_0200b9f8
.L_0200baa8:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_0200b9e4
.L_0200bab4:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bb0c
	ldr r3, .L_0200bb68
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #0
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	mov r0, r8
	asrs r1, r3, #20
	ldr r3, [sp, #8]
	mov r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	cmp r2, r3
	bge .L_0200bb0c
.L_0200bae6:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_0200bafc
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_0200bafc
	mov r0, r8
	strh r2, [r0, #12]
.L_0200bafc:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_0200bae6
.L_0200bb0c:
	movs r0, #128
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_0200bb1a:
	ldr r3, .L_0200bb6c
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_0200bb1a
	bl Resource_FindFreeEntry
	mov r1, r8
	strh r0, [r1, #16]
	lsls r0, r0, #16
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #1
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200bb70
	bl Scheduler_AddOrUpdateCallback
	mov r3, r8
	movs r2, #10
	ldrsh r0, [r3, r2]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bb60:
	.4byte Data_02004cf4
.L_0200bb64:
	.4byte IwramClearWords
.L_0200bb68:
	.4byte gPartyState
.L_0200bb6c:
	.4byte 0x11111111
.L_0200bb70:
	.4byte Func_02003804
	.section .text.x0200bb74,"ax",%progbits
	.global Func_02003b74
	.thumb_func
Func_02003b74:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, .L_0200bbf4
	movs r2, #133
	mov r8, r1
	lsls r2, r2, #2
	add r8, r2
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	mov r1, r8
	ldr r5, [r0, #8]
	ldr r6, [r0, #16]
	mov r10, r0
	movs r2, #128
	ldr r0, [r1]
	movs r1, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	asrs r5, r5, #20
	mov r2, r8
	asrs r6, r6, #20
	ldr r0, [r2]
	lsls r1, r5, #4
	lsls r2, r6, #4
	adds r1, #8
	adds r2, #8
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #12
	lsls r5, r5, #20
	lsls r6, r6, #20
	adds r5, r5, r3
	mov r1, r10
	adds r6, r6, r3
	ldr r2, [r1, #12]
	adds r3, r6, #0
	adds r1, r5, #0
	mov r0, r10
	bl Object_SetPositionAndResetMotion
	movs r0, #4
	bl Battle_WaitMode0
	bl Func_020043dc
	ldr r2, .L_0200bbf8
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200bbf4:
	.4byte gPartyState
.L_0200bbf8:
	.4byte 0xfff80000
	.section .text.x0200bbfc,"ax",%progbits
	.global Func_02003bfc
	.thumb_func
Func_02003bfc:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_0200bc68
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	mov r8, r0
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	mov r2, r8
	ldrh r1, [r2, #6]
	movs r2, #64
	ldr r6, [r0, #8]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	asrs r6, r6, #20
	orrs r6, r3
	ldrh r3, [r1, #8]
	ldr r5, [r0, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_02003b74
	movs r0, #161
	bl Func_0200446c
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02004234
	movs r0, #12
	bl Battle_WaitMode0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200bc68:
	.4byte gPartyState
	.section .text.x0200bc6c,"ax",%progbits
	.global Func_02003c6c
	.thumb_func
Func_02003c6c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200bd1c
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r8, r0
	ldr r0, [r1]
	sub sp, #8
	mov r10, r1
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r2, #64
	asrs r7, r3, #20
	mov r3, r8
	ldrh r1, [r3, #6]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	orrs r7, r3
	ldrh r3, [r1, #8]
	ldr r5, [r6, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_02003b74
	movs r0, #229
	bl Func_0200446c
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #2
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_02004234
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_0200bd14
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200bd18
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_0200bd20
	.2byte 0x0000
.L_0200bd14:
	.4byte 0x00000000
.L_0200bd18:
	.4byte 0x00008000
.L_0200bd1c:
	.4byte gPartyState
.L_0200bd20:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
.L_0200bd34:
	cmp r7, #5
	bne .L_0200bd3e
	movs r0, #204
	bl Func_0200446c
.L_0200bd3e:
	ldr r3, [r6, #24]
	ldr r1, .L_0200bd9c
	ldr r2, .L_0200bda0
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_0200bda4
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_0200bd34
	ldr r3, .L_0200bda8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	mov r1, r8
	strh r3, [r1, #14]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200bd9c:
	.4byte 0xfffffc00
.L_0200bda0:
	.4byte 0xfffffd00
.L_0200bda4:
	.4byte 0xffff6667
.L_0200bda8:
	.4byte gPartyState
	.section .text.x0200bdac,"ax",%progbits
	.global Func_02003dac
	.thumb_func
Func_02003dac:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200bddc
	adds r3, #15
.L_0200bddc:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200be04,"ax",%progbits
	.global Func_02003e04
	.thumb_func
Func_02003e04:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200bf88
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_020041fc
	movs r0, #1
	bl WaitFrames
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #8
	adds r5, r7, #0
	str r3, [r7, #72]
	adds r5, #85
	movs r3, #0
	str r3, [r7, #68]
	strb r3, [r5]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r4, #214
	lsls r4, r4, #1
	movs r2, #128
	adds r3, r3, r4
	lsls r2, r2, #1
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #204
	bl Func_0200446c
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200bf8c
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200be9e:
	mov r4, r10
	lsls r5, r4, #12
	adds r0, r5, #0
	bl Math_Cosine
	add r6, sp, #16
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl Math_Sine
	ldr r3, [r6]
	str r0, [r6, #8]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r6]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_0200bf90
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200bf94
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	ldr r4, [r6, #4]
	str r5, [r6, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r4, [sp, #0]
	ldr r4, .L_0200bf98
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_0200363c
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200be9e
	movs r0, #188
	bl Func_0200446c
	ldr r5, .L_0200bf88
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_020043bc
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004264
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02004264
	bl Func_0200426c
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_020043bc
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	bl Func_020042e4
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200bf88:
	.4byte gPartyState
.L_0200bf8c:
	.4byte Func_02003dac
.L_0200bf90:
	.4byte 0xffffa000
.L_0200bf94:
	.4byte 0xffffd000
.L_0200bf98:
	.4byte 0x01090001
	.section .text.x0200bf9c,"ax",%progbits
	.global Func_02003f9c
	.thumb_func
Func_02003f9c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200c044
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200c048
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	adds r5, r6, #0
	asrs r3, r3, #20
	mov r10, r3
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r7, #0
	adds r5, #32
	ldrh r2, [r6, #10]
	cmp r7, r3
	bge .L_0200c038
.L_0200bfd0:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200c02c
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200c02c
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c000
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02003bfc
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200c038
.L_0200c000:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200c038
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02003c6c
	movs r2, #2
	ldrsh r0, [r6, r2]
	mov r1, r8
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r1, r10
	adds r0, #8
	bl GameFlag_SetByte
	movs r0, #1
	b .L_0200c03a
.L_0200c02c:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200bfd0
.L_0200c038:
	movs r0, #0
.L_0200c03a:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c044:
	.4byte gPartyState
.L_0200c048:
	.4byte Data_02004cf4
	.section .text.x0200c04c,"ax",%progbits
	.global Func_0200404c
	.thumb_func
Func_0200404c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_0200c0fc
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_0200c100
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r6, r0, #0
	movs r2, #2
	ldrsh r0, [r5, r2]
	mov r10, r3
	bl GameFlag_GetByte
	adds r7, r0, #0
	movs r3, #2
	ldrsh r0, [r5, r3]
	adds r0, #8
	bl GameFlag_GetByte
	mov r8, r0
	cmp r7, #0
	bne .L_0200c09a
	cmp r0, #0
	beq .L_0200c0ee
.L_0200c09a:
	movs r2, #2
	ldrsh r0, [r5, r2]
	movs r1, #0
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r5, r3]
	movs r1, #0
	adds r0, #8
	bl GameFlag_SetByte
	mov r3, r9
	adds r2, r7, r3
	mov r3, r8
	movs r1, #128
	add r3, r11
	lsls r1, r1, #12
	lsls r3, r3, #20
	adds r3, r3, r1
	str r3, [r6, #16]
	movs r3, #230
	lsls r3, r3, #1
	add r3, r10
	lsls r2, r2, #20
	adds r2, r2, r1
	ldr r1, [r3]
	str r2, [r6, #8]
	str r2, [r1, #8]
	ldr r3, [r6, #16]
	str r3, [r1, #16]
	bl Func_020041fc
	bl Func_02003e04
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_0200c0ee:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c0fc:
	.4byte gPartyState
.L_0200c100:
	.4byte Data_02004cf4
	.section .rodata.x0200c474,"a",%progbits
	.global Data_02004474
Data_02004474:
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_020044b8
Data_020044b8:
	.4byte 0x0000002e
	.4byte Func_02002c44
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02380000
	.4byte 0x00200000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02380000
	.4byte 0x00200000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02400000
	.4byte 0x00200000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02400000
	.4byte 0x00160000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02500000
	.4byte 0x00160000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte Func_02000188
	.4byte 0x0000002e
	.4byte Func_02002ce8
	.4byte 0x00000011
	.global Data_020045b8
Data_020045b8:
	.4byte 0x0000002e
	.4byte Func_02002c44
	.4byte 0x00000004
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03280000
	.4byte 0x00180000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03180000
	.4byte 0x00180000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03180000
	.4byte 0x00200000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03180000
	.4byte 0x00200000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03100000
	.4byte 0x00200000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03100000
	.4byte 0x00160000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03000000
	.4byte 0x00160000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte Func_020001e8
	.4byte 0x0000002e
	.4byte Func_02002ce8
	.4byte 0x00000011
	.global Data_02004744
Data_02004744:
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02004774
Data_02004774:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_020047b8
Data_020047b8:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_020047fc
Data_020047fc:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xe000c000
	.4byte 0xc000a000
	.4byte 0x20004000
	.4byte 0x40006000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.global Data_0200483c
Data_0200483c:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000c000
	.4byte 0xc0008000
	.4byte 0x00004000
	.4byte 0x40008000
	.4byte 0x0000ffff
	.4byte 0xffff8000
.L_0200c85c:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200c898:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200c8d4:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200c910:
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000011
	.global Data_0200491c
Data_0200491c:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000400
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000400
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02004958
Data_02004958:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffff800
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020049a0
Data_020049a0:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020049dc
Data_020049dc:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000400
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x800000c0
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
	.4byte 0x0000011e
	.4byte 0x0010211e
	.4byte 0x0020111e
	.4byte 0x0030411e
	.4byte 0x0040311e
	.4byte 0x0050511d
	.4byte 0x0060611c
	.4byte 0x0070711d
	.4byte 0x0080811c
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff01a2
	.4byte .L_0200c910
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200c910
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0x0a3000b5
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0x0a3000b5
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0017
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff012f
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff012f
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
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
	.4byte 0x00000051
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000021
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000002
	.4byte 0x1a4b005a
	.4byte Func_0200236c
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte Func_02000060
	.4byte 0x60009a15
	.4byte 0xffff0009
	.4byte Func_02000074
	.4byte 0x20009a15
	.4byte 0xffff0009
	.4byte Data_0200007c + 0x1
	.4byte 0x00000006
	.4byte 0xffff0014
	.4byte Func_020005b0
	.4byte 0x50009705
	.4byte 0x12330014
	.4byte Func_0200024c
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_02000ba4
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002d82
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_02000bd8
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002d81
	.4byte 0x00000003
	.4byte 0xffff0015
	.4byte Func_02000e50
	.4byte 0x0000de04
	.4byte 0xffff0415
	.4byte Func_02000e80
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004ce0
Data_02004ce0:
	.4byte .L_0200c85c
	.4byte .L_0200c898
	.4byte .L_0200c8d4
	.section .bss,"aw",%nobits
	.global Data_02004cec
Data_02004cec:
	.space 0x00000004
	.global Data_02004cf0
Data_02004cf0:
	.space 0x00000004
	.global Data_02004cf4
Data_02004cf4:
