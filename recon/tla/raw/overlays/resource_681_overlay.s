.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080ce,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815c,"ax",%progbits
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	str r3, [sp, #0]
	ldr r3, .L_0200832c
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r10, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r10
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_020081a4
	cmp r7, #0
	beq .L_020081a4
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_020081ac
.L_020081a4:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_020081ac:
	mov r3, r8
	bl Engine_ObjectCreate
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020081ba
	b .L_0200831e
.L_020081ba:
	ldr r3, [r6, #80]
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	mov r8, r3
	bl Func_02004d9c
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02004dac
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_02008334
	mov r1, r9
	str r3, [r6, #108]
	ldr r3, [sp, #0]
	adds r0, r6, #0
	str r3, [r6, #68]
	ldr r3, [sp, #36]
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
	ldr r3, .L_02008338
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200831e
	cmp r7, #0
	beq .L_0200831e
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200823c
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200823c:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02008274
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r3, #3
	ldrb r2, [r7]
	adds r0, r6, #0
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	lsls r2, r2, #2
	mov r1, r8
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r1, [r7]
	bl Object_SetSpritePriority
.L_02008274:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_02008288
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02008288:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020082ce
	ldr r3, .L_02008330
	mov r1, r11
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_020082b6
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020082c8
.L_020082b6:
	ldr r2, .L_02008338
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02008338
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020082c8:
	bl __divsi3
	str r0, [r6, #52]
.L_020082ce:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020082ea
	adds r0, r6, #0
	movs r1, #1
	bl Func_02004d9c
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02004dac
.L_020082ea:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_020082fc
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #18]
.L_020082fc:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200830e
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200830e:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200831e
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200831e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200832c:
	.4byte gPartyState
.L_02008330:
	.4byte Data_0200541c
.L_02008334:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_02008338:
	.4byte 0xffff0000
	.section .text.x0200842a,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.else
	.2byte 0x0000
	.endif
	.section .text.x0200842c,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.else
	.global Func_0200042c
	.thumb_func
Func_0200042c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_020085c0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02004dc4
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
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #204
	bl Func_02005014
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_020085c4
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_020084c6:
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
	ldr r3, .L_020085c8
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_020085cc
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
	ldr r4, .L_020085d0
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_0200015c
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_020084c6
	movs r0, #188
	bl Func_02005014
	ldr r5, .L_020085c0
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02004f34
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004e24
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02004e24
	bl Func_02004e2c
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02004f34
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
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_02004e64
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020085c0:
	.4byte gPartyState
.L_020085c4:
	.4byte Effect_AdvanceMotion
.L_020085c8:
	.4byte 0xffffa000
.L_020085cc:
	.4byte 0xffffd000
.L_020085d0:
	.4byte 0x01090001
	.endif
	.section .text.x02008774,"ax",%progbits
	.global Func_02000774
	.thumb_func
Func_02000774:
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
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_020087a4
	adds r3, #15
.L_020087a4:
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
	ldr r3, .L_020087f0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r4, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r4, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020087f0:
	.4byte gPartyState
	.section .text.x020087f4,"ax",%progbits
	.global Func_020007f4
	.thumb_func
Func_020007f4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #9
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #202
	bl Func_02005014
	movs r0, #3
	mov r8, r0
.L_0200880c:
	movs r0, #30
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	adds r0, #255
	bl Engine_ObjectCreate
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008894
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r3, #4
	strb r5, [r3]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r6, #0
	movs r1, #1
	bl Func_02004d9c
	adds r0, r6, #0
	ldr r1, .L_020088a8
	bl Func_02004dac
	ldr r1, [r6, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r0, r0, #14
	bl Math_Cosine
	asrs r0, r0, #1
	str r0, [r6, #68]
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r0, r0, #14
	bl Math_Sine
	asrs r0, r0, #1
	str r0, [r6, #76]
	str r5, [r6, #72]
	bl Random16Far
	ldr r3, .L_020088ac
	lsls r0, r0, #9
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r6, #0
	adds r3, #100
	strh r0, [r3]
	ldr r3, .L_020088b0
	str r5, [r6, #48]
	str r5, [r6, #52]
	str r3, [r6, #108]
.L_02008894:
	movs r0, #1
	negs r0, r0
	add r8, r0
	mov r3, r8
	cmp r3, #0
	bge .L_0200880c
	movs r0, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020088a8:
	.4byte Data_020050f4
.L_020088ac:
	.4byte 0xffffff00
.L_020088b0:
	.4byte Func_02000774
	.section .text.x02008900,"ax",%progbits
	.global Func_02000900
	.thumb_func
Func_02000900:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	movs r0, #9
	bl Object_GetById
	mov r7, r8
	adds r7, #96
	ldrb r1, [r7]
	adds r5, r0, #0
	cmp r1, #30
	beq .L_02008924
	adds r3, r5, #0
	adds r3, #96
	ldrb r3, [r3]
	cmp r3, #30
	bne .L_0200894a
.L_02008924:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #181
	lsls r0, r0, #1
	adds r2, r3, r0
	movs r3, #42
	strh r3, [r2]
	cmp r1, #30
	bne .L_0200893c
	movs r3, #31
	strb r3, [r7]
.L_0200893c:
	adds r2, r5, #0
	adds r2, #96
	ldrb r3, [r2]
	cmp r3, #30
	bne .L_0200894a
	movs r3, #31
	strb r3, [r2]
.L_0200894a:
	ldrb r3, [r7]
	cmp r3, #2
	bls .L_02008962
	adds r2, r5, #0
	movs r0, #128
	adds r2, #91
	movs r3, #1
	lsls r0, r0, #2
	strb r3, [r2]
	adds r0, #6
	bl GameFlag_SetBit
.L_02008962:
	adds r6, r5, #0
	adds r6, #96
	ldrb r3, [r6]
	cmp r3, #2
	bls .L_0200899a
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200899a
	ldr r2, [r5]
	ldr r3, .L_020089e0
	cmp r2, r3
	beq .L_0200899a
	ldr r3, .L_020089e4
	cmp r2, r3
	beq .L_0200899a
	mov r2, r8
	movs r0, #131
	adds r2, #91
	movs r3, #1
	lsls r0, r0, #1
	strb r3, [r2]
	adds r0, #255
	bl GameFlag_SetBit
.L_0200899a:
	ldrb r2, [r7]
	cmp r2, #0
	bne .L_020089b0
	adds r3, r5, #0
	movs r0, #128
	adds r3, #91
	lsls r0, r0, #2
	strb r2, [r3]
	adds r0, #6
	bl GameFlag_ClearBit
.L_020089b0:
	ldrb r2, [r6]
	cmp r2, #0
	bne .L_020089c6
	mov r3, r8
	movs r0, #131
	adds r3, #91
	lsls r0, r0, #1
	strb r2, [r3]
	adds r0, #255
	bl GameFlag_ClearBit
.L_020089c6:
	ldrb r3, [r7]
	cmp r3, #35
	bls .L_020089d0
	movs r3, #35
	strb r3, [r7]
.L_020089d0:
	ldrb r3, [r6]
	cmp r3, #35
	bls .L_020089da
	movs r3, #35
	strb r3, [r6]
.L_020089da:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020089e0:
	.4byte Data_020056c4
.L_020089e4:
	.4byte Data_02005728
	.section .text.x02008af8,"ax",%progbits
	.global Func_02000af8
	.thumb_func
Func_02000af8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r0, r1, #0
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02004904
	ldr r3, .L_02008b60
	movs r1, #192
	adds r5, r0, #0
	lsls r1, r1, #8
	adds r7, r6, #0
	ands r1, r5
	mov r8, r3
	adds r7, #89
	movs r3, #0
	lsrs r1, r1, #14
	strb r3, [r7]
	adds r1, #2
	adds r0, r6, #0
	bl Func_02004d9c
	movs r3, #128
	lsls r3, r3, #7
	ands r3, r5
	cmp r3, #0
	beq .L_02008b36
	ldr r3, .L_02008b64
	b .L_02008b38
.L_02008b36:
	ldr r3, .L_02008b68
.L_02008b38:
	str r3, [r6, #108]
	movs r3, #1
	strb r3, [r7]
	mov r3, r8
	ldr r0, [r3]
	movs r3, #165
	lsls r3, r3, #4
	adds r0, r0, r3
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #20
	bl WaitFrames
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008b60:
	.4byte Data_020023c4 + 0x288
.L_02008b64:
	.4byte Func_02000aac
.L_02008b68:
	.4byte Func_02000a60
	.section .text.x02008b6c,"ax",%progbits
	.global Func_02000b6c
	.thumb_func
Func_02000b6c:
	push {r5, r6, lr}
	ldr r3, .L_02008c34
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r2, #61
	movs r1, #0
	movs r3, #3
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r2, #0
	movs r1, #26
	adds r5, r0, #0
	movs r0, #37
	str r3, [sp, #0]
	bl Func_02004ff4
	movs r1, #152
	movs r2, #200
	movs r0, #2
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02004dec
	cmp r0, #0
	bne .L_02008c2e
	movs r3, #38
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #36
	movs r1, #23
	movs r2, #1
	bl Func_02004dfc
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #38
	bne .L_02008bf6
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #25
	bne .L_02008bf6
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02004f34
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
.L_02008bf6:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008c20
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #10
	bl GameFlag_SetBit
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02004fdc
	movs r0, #20
	bl Func_02004f6c
	b .L_02008c2e
.L_02008c20:
	movs r0, #140
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02004e64
.L_02008c2e:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008c34:
	.4byte gPartyState
	.section .text.x02008d18,"ax",%progbits
	.balign 4
	.global Func_02000d18
	.thumb_func
Func_02000d18:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #12
	movs r2, #117
	movs r3, #8
	movs r1, #20
	str r2, [sp, #4]
	movs r0, #104
	movs r2, #1
	str r3, [sp, #0]
	str r1, [sp, #8]
	bl Func_02004ff4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #196
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008d46
	b .L_02008ebc
.L_02008d46:
	movs r5, #128
	movs r0, #0
	lsls r5, r5, #9
	mov r8, r0
	movs r3, #0
	movs r7, #2
	adds r6, r5, #0
.L_02008d54:
	ldr r2, .L_02008ec8
	lsls r3, r3, #2
	ldrsh r1, [r2, r3]
	ldrsh r2, [r2, r7]
	lsls r1, r1, #20
	lsls r2, r2, #20
	movs r0, #2
	bl Func_02004dec
	cmp r0, #0
	bne .L_02008d76
	adds r3, r6, #0
	movs r0, #128
	lsls r0, r0, #9
	asrs r3, r3, #16
	adds r6, r6, r0
	mov r8, r3
.L_02008d76:
	adds r3, r5, #0
	movs r2, #128
	lsls r2, r2, #9
	asrs r3, r3, #16
	adds r5, r5, r2
	adds r7, #4
	cmp r3, #11
	ble .L_02008d54
	mov r3, r8
	cmp r3, #0
	bgt .L_02008d8e
	b .L_02008ebc
.L_02008d8e:
	movs r0, #15
	bl Object_GetById
	ldr r3, .L_02008ecc
	adds r7, r0, #0
	movs r0, #133
	lsls r0, r0, #2
	adds r6, r3, r0
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	ldr r3, [r5, #16]
	asrs r3, r3, #19
	cmp r3, #53
	ble .L_02008dde
	ldr r3, [r5, #8]
	asrs r3, r3, #19
	cmp r3, #83
	ble .L_02008dde
	cmp r3, #91
	bgt .L_02008dde
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r1, #128
	lsls r1, r1, #1
	ldr r0, [r6]
	movs r2, #0
	bl Func_02004f2c
	ldr r0, [r6]
	ldr r1, .L_02008ed0
	bl ObjectMotion_EnableActionAndSetCallback
.L_02008dde:
	ldr r1, .L_02008ed4
	movs r0, #15
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #60
	bl Battle_WaitMode0
	ldr r1, .L_02008ed8
	movs r0, #15
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r3, #104
	str r3, [sp, #0]
	movs r5, #20
	movs r0, #117
	movs r1, #20
	movs r2, #7
	movs r3, #8
	str r5, [sp, #4]
	bl Func_02004e04
	movs r3, #111
	str r3, [sp, #0]
	movs r0, #111
	movs r1, #12
	movs r2, #1
	movs r3, #6
	str r5, [sp, #4]
	bl Func_02004e04
	movs r3, #40
	movs r2, #84
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #84
	movs r2, #8
	movs r3, #8
	movs r0, #49
	bl Func_02004dfc
	movs r5, #175
	movs r0, #30
	movs r6, #143
	bl Battle_WaitMode0
	lsls r6, r6, #1
	lsls r5, r5, #18
	movs r2, #234
	lsls r2, r2, #17
	adds r0, r5, #0
	movs r1, #0
	adds r3, r6, #0
	mov r10, r2
	bl OverlayObject_PrepareObject
	movs r3, #245
	lsls r3, r3, #17
	mov r8, r3
	adds r0, r5, #0
	movs r5, #181
	movs r1, #0
	mov r2, r8
	adds r3, r6, #0
	lsls r5, r5, #18
	bl OverlayObject_PrepareObject
	movs r1, #0
	mov r2, r10
	adds r3, r6, #0
	adds r0, r5, #0
	bl OverlayObject_PrepareObject
	movs r1, #0
	mov r2, r8
	adds r3, r6, #0
	adds r0, r5, #0
	bl OverlayObject_PrepareObject
	movs r0, #204
	bl Func_02005014
	adds r2, r7, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	ldr r1, [r7, #80]
	movs r2, #12
	ldrb r3, [r1, #9]
	movs r0, #10
	orrs r3, r2
	strb r3, [r1, #9]
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	bl Func_02004fdc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #196
	bl GameFlag_SetBit
	movs r0, #9
	bl Func_02004f6c
.L_02008ebc:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008ec8:
	.4byte Data_020051d8
.L_02008ecc:
	.4byte gPartyState
.L_02008ed0:
	.4byte Data_020051b8
.L_02008ed4:
	.4byte Data_02005118
.L_02008ed8:
	.4byte Data_02005168
	.section .text.x02008f00,"ax",%progbits
	.global Func_02000f00
	.thumb_func
Func_02000f00:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #9
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #10
	bl Object_GetById
	adds r5, r0, #0
	ldr r6, [r5, #80]
	ldr r3, .L_02008f8c
	ldr r1, [r6, #40]
	movs r2, #128
	mov r8, r1
	ldr r0, .L_02008f90
	adds r1, r5, #0
	mov lr, r3
	.2byte 0xf800
	mov r1, r8
	ldrb r3, [r1, #21]
	ldr r2, .L_02008f94
	str r3, [r2]
	ldr r2, .L_02008f98
	ldrb r3, [r6, #24]
	str r3, [r2]
	ldr r3, .L_02008f9c
	ldr r1, [r5, #8]
	movs r2, #0
	str r1, [r3]
	ldr r3, .L_02008fa0
	ldr r0, [r5, #16]
	str r2, [r5, #108]
	str r0, [r3]
	str r2, [r5, #44]
	ldr r3, [r5, #12]
	str r2, [r5, #40]
	str r2, [r5, #36]
	str r3, [r5, #60]
	str r1, [r5, #56]
	str r0, [r5, #64]
	movs r0, #10
	bl ObjectMotion_ResetTargetsAndVelocity
	adds r0, r5, #0
	movs r1, #10
	bl ObjectDispatch_ApplyValueToChildren
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	ldr r2, [r7]
	ldr r3, .L_02008fa4
	cmp r2, r3
	beq .L_02008f76
	ldr r3, .L_02008fa8
	cmp r2, r3
	bne .L_02008f86
.L_02008f76:
	adds r2, r7, #0
	adds r2, #91
	movs r3, #1
	strb r3, [r2]
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
.L_02008f86:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008f8c:
	.4byte IwramCopyWords
.L_02008f90:
	.4byte Data_0200646c
.L_02008f94:
	.4byte Data_020064f8
.L_02008f98:
	.4byte Data_020064f4
.L_02008f9c:
	.4byte Data_020064ec
.L_02008fa0:
	.4byte Data_020064f0
.L_02008fa4:
	.4byte Data_020055c0
.L_02008fa8:
	.4byte Data_0200559c
	.section .text.x02008fac,"ax",%progbits
	.global Func_02000fac
	.thumb_func
Func_02000fac:
	push {r5, r6, lr}
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #10
	bl Object_GetById
	ldr r3, .L_02009028
	adds r6, r0, #0
	ldr r1, [r3]
	ldr r3, [r6, #8]
	cmp r1, r3
	bne .L_02008fd2
	ldr r3, .L_0200902c
	ldr r2, [r3]
	ldr r3, [r6, #16]
	cmp r2, r3
	beq .L_02008fe6
.L_02008fd2:
	ldr r3, .L_0200902c
	asrs r1, r1, #16
	movs r0, #2
	ldrsh r2, [r3, r0]
	movs r0, #10
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	bl ObjectMotion_CommitCurrentPositionAndActivate
.L_02008fe6:
	ldr r2, [r5]
	ldr r3, .L_02009030
	cmp r2, r3
	beq .L_02008ff4
	ldr r3, .L_02009034
	cmp r2, r3
	bne .L_02009004
.L_02008ff4:
	adds r2, r5, #0
	adds r2, #91
	movs r3, #0
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
.L_02009004:
	movs r2, #128
	ldr r1, .L_02009038
	ldr r3, .L_0200903c
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_02009040
	movs r0, #10
	ldr r1, [r3]
	bl Object_SetModeById
	ldr r3, .L_02009044
	adds r0, r6, #0
	ldr r1, [r3]
	bl ObjectDispatch_ApplyValueToChildren
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009028:
	.4byte Data_020064ec
.L_0200902c:
	.4byte Data_020064f0
.L_02009030:
	.4byte Data_020055c0
.L_02009034:
	.4byte Data_0200559c
.L_02009038:
	.4byte Data_0200646c
.L_0200903c:
	.4byte IwramCopyWords
.L_02009040:
	.4byte Data_020064f4
.L_02009044:
	.4byte Data_020064f8
	.section .text.x02009048,"ax",%progbits
	.global Func_02001048
	.thumb_func
Func_02001048:
	push {r5, lr}
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	ldr r0, .L_020091a4
	bl Func_02004f0c
	movs r0, #10
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	ldr r2, [r5]
	ldr r3, .L_020091a8
	cmp r2, r3
	beq .L_02009078
	ldr r3, .L_020091ac
	cmp r2, r3
	bne .L_0200907e
.L_02009078:
	movs r0, #9
	bl Object_RefreshSelectorById
.L_0200907e:
	bl Func_02000f00
	movs r0, #10
	movs r1, #1
	bl Object_SetModeById
	ldr r3, .L_020091b0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #10
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02004f2c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	bl Func_02004f14
	movs r1, #248
	movs r2, #228
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #10
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #5
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	bl Func_02004f14
	movs r1, #6
	movs r2, #0
	adds r1, #255
	movs r0, #10
	bl Func_02004f2c
	movs r0, #45
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	bl Func_02004f14
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #244
	movs r1, #1
	movs r2, #196
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02004f54
	movs r0, #10
	movs r1, #0
	bl Func_02004f14
	movs r0, #10
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02004f54
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #5
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	bl Func_02004f14
	movs r0, #10
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #222
	bl GameFlag_SetBit
	ldr r0, [r5]
	ldr r3, .L_020091b4
	cmp r0, r3
	beq .L_02009186
	ldr r3, .L_020091b8
	cmp r0, r3
	bne .L_02009198
.L_02009186:
	ldr r2, .L_020091bc
	ldr r1, .L_020091c0
	ldr r3, [r2, #56]
	str r3, [r1]
	str r3, [r2, #8]
	ldr r1, .L_020091c4
	ldr r3, [r2, #64]
	str r3, [r1]
	str r3, [r2, #16]
.L_02009198:
	bl Func_02000fac
	bl Func_02004e64
	pop {r5, pc}
	.2byte 0x0000
.L_020091a4:
	.4byte MsgScene681No02
.L_020091a8:
	.4byte Data_02005610
.L_020091ac:
	.4byte Data_0200559c
.L_020091b0:
	.4byte gPartyState
.L_020091b4:
	.4byte Data_020056c4
.L_020091b8:
	.4byte Data_02005728
.L_020091bc:
	.4byte Data_0200646c
.L_020091c0:
	.4byte Data_020064ec
.L_020091c4:
	.4byte Data_020064f0
	.section .text.x020091c8,"ax",%progbits
	.global Func_020011c8
	.thumb_func
Func_020011c8:
	push {lr}
	movs r0, #10
	bl Object_GetById
	adds r0, #96
	ldrb r3, [r0]
	cmp r3, #2
	bhi .L_02009294
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #172
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009240
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #188
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009228
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #189
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009228
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #190
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009228
	movs r0, #140
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009230
.L_02009228:
	ldr r0, .L_02009298
	bl Func_02004f0c
	b .L_02009236
.L_02009230:
	ldr r0, .L_0200929c
	bl Func_02004f0c
.L_02009236:
	movs r0, #10
	movs r1, #0
	bl Func_02004f14
	b .L_02009290
.L_02009240:
	movs r0, #9
	bl Object_GetById
	ldr r3, .L_020092a0
	ldr r2, [r0]
	cmp r2, r3
	bne .L_02009254
	movs r0, #9
	bl Object_RefreshSelectorById
.L_02009254:
	bl Func_02000f00
	ldr r0, .L_020092a4
	bl Func_02004f0c
	ldr r3, .L_020092a8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	bl Func_02004f14
	movs r0, #10
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02000fac
.L_02009290:
	bl Func_02004e64
.L_02009294:
	pop {pc}
	.2byte 0x0000
.L_02009298:
	.4byte MsgScene681No10
.L_0200929c:
	.4byte MsgScene681No06
.L_020092a0:
	.4byte Data_02005610
.L_020092a4:
	.4byte MsgScene681No03
.L_020092a8:
	.4byte gPartyState
	.section .text.x0200936c,"ax",%progbits
	.global Func_0200136c
	.thumb_func
Func_0200136c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	movs r2, #102
	adds r2, r2, r6
	adds r5, r6, #0
	mov r10, r2
	ldrh r2, [r2]
	adds r5, #100
	ldrh r3, [r5]
	lsls r2, r2, #16
	ldr r7, [r6, #104]
	asrs r2, r2, #17
	adds r3, r3, r2
	strh r3, [r5]
	ldr r3, [r7, #8]
	movs r2, #128
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	movs r2, #8
	str r3, [r6, #16]
	adds r2, r2, r6
	movs r3, #0
	ldrsh r1, [r5, r3]
	ldr r0, [r6, #76]
	mov r8, r2
	bl Vector_AddPolarOffsetFar
	adds r2, r6, #0
	adds r2, #98
	ldrb r3, [r2]
	movs r0, #0
	adds r3, #255
	strb r3, [r2]
	lsls r3, r3, #24
	cmp r3, #0
	beq .L_0200941c
	ldr r3, [r6, #76]
	movs r2, #128
	lsls r2, r2, #10
	movs r0, #1
	cmp r3, r2
	beq .L_0200941c
	adds r0, r6, #0
	bl Func_02001314
	ldr r2, .L_02009428
	ldr r3, [r6, #76]
	mov r9, r2
	add r3, r9
	str r3, [r6, #76]
	mov r3, r10
	ldrh r2, [r3]
	ldrh r3, [r5]
	lsls r2, r2, #16
	asrs r2, r2, #17
	adds r3, r3, r2
	strh r3, [r5]
	ldr r3, [r7, #8]
	mov r2, r8
	str r3, [r2]
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	ldr r0, [r6, #76]
	str r3, [r6, #16]
	mov r2, r8
	movs r3, #0
	ldrsh r1, [r5, r3]
	bl Vector_AddPolarOffsetFar
	adds r0, r6, #0
	bl Func_02001314
	ldr r3, [r6, #76]
	movs r0, #1
	add r3, r9
	str r3, [r6, #76]
.L_0200941c:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009428:
	.4byte 0xfffc0000
	.section .text.x0200942c,"ax",%progbits
	.global Func_0200142c
	.thumb_func
Func_0200142c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #102
	adds r0, r0, r7
	mov r8, r0
	adds r5, r7, #0
	adds r5, #100
	mov r1, r8
	ldrh r3, [r1]
	ldrh r0, [r5]
	adds r0, r0, r3
	strh r0, [r5]
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Math_Cosine
	ldr r1, [r7, #76]
	ldr r6, .L_020094c8
	mov lr, r6
	.2byte 0xf800
	str r0, [r7, #8]
	movs r2, #0
	ldrsh r0, [r5, r2]
	bl Math_Sine
	ldr r1, [r7, #76]
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r7, #8]
	ldr r2, [r7, #68]
	asrs r0, r0, #1
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r3, [r7, #72]
	subs r5, #1
	adds r0, r0, r3
	str r0, [r7, #16]
	ldrb r3, [r5]
	cmp r3, #141
	beq .L_020094a4
	ldr r3, .L_020094cc
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020094a4
	adds r3, r7, #0
	adds r3, #98
	ldrb r0, [r3]
	lsls r0, r0, #10
	bl Math_Sine
	ldrb r3, [r5]
	muls r3, r0
	str r3, [r7, #76]
	ldrb r3, [r5]
	adds r3, #10
	strb r3, [r5]
.L_020094a4:
	mov r3, r8
	ldrh r2, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r0, #0
	cmp r3, #0
	beq .L_020094c2
	ldrb r3, [r5]
	cmp r3, #141
	bne .L_020094c0
	adds r3, r2, #0
	subs r3, #128
	mov r1, r8
	strh r3, [r1]
.L_020094c0:
	movs r0, #1
.L_020094c2:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020094c8:
	.4byte IwramMulQ16
.L_020094cc:
	.4byte gFrameCount
	.section .text.x02009548,"ax",%progbits
	.global Func_02001548
	.thumb_func
Func_02001548:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_020095cc
	sub sp, #68
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_020095c2
	add r2, sp, #28
	str r3, [r2, #4]
	movs r3, #209
	lsls r3, r3, #1
	adds r3, #255
	strh r3, [r2, #24]
	movs r3, #1
	str r3, [r2]
	mov r8, r2
	bl Random16Far
	movs r6, #31
	mov r2, r10
	ldr r3, [r2, #8]
	ands r0, r6
	subs r0, #16
	lsls r0, r0, #16
	add r5, sp, #16
	adds r3, r3, r0
	str r3, [r5]
	bl Random16Far
	mov r3, r10
	ldr r1, [r3, #12]
	ands r0, r6
	lsls r0, r0, #16
	movs r2, #128
	adds r1, r1, r0
	lsls r2, r2, #12
	adds r1, r1, r2
	str r1, [r5, #4]
	ldr r0, [r5]
	ldr r2, [r3, #16]
	movs r3, #128
	lsls r3, r3, #11
	str r2, [r5, #8]
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #8
	str r3, [sp, #0]
	movs r3, #152
	lsls r3, r3, #13
	str r3, [sp, #8]
	mov r3, r8
	str r3, [sp, #12]
	movs r3, #0
	str r7, [sp, #4]
	bl Func_0200015c
.L_020095c2:
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020095cc:
	.4byte gFrameCount
	.section .text.x020095d0,"ax",%progbits
	.global Func_020015d0
	.thumb_func
Func_020015d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	mov r11, r1
	mov r9, r0
	bl Object_GetById
	adds r7, r0, #0
	mov r0, r11
	bl Object_GetById
	mov r10, r0
	movs r0, #78
	bl Func_02005014
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl Func_02004f44
	movs r2, #10
	ldrsh r0, [r7, r2]
	movs r3, #14
	ldrsh r1, [r7, r3]
	movs r3, #18
	ldrsh r2, [r7, r3]
	lsls r1, r1, #16
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl Motion_CamBounds
	movs r0, #141
	bl Func_02005014
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_02004e24
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02004f9c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_02004f94
	movs r0, #60
	bl Func_02004fa4
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	str r3, [r7, #52]
	str r3, [r7, #48]
	ldr r1, .L_020096a4
	mov r0, r9
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #194
	bl Func_02005014
	movs r0, #45
	bl Battle_WaitMode0
	movs r1, #128
	mov r0, r9
	lsls r1, r1, #1
	bl Func_02004efc
	ldr r3, .L_0200969c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_020096a0
	subs r2, #2
	strh r3, [r2]
	movs r0, #0
	mov r8, r0
	b .L_020096a8
	.2byte 0x0000
.L_0200969c:
	.4byte 0x00001008
.L_020096a0:
	.4byte 0x00003f10
.L_020096a4:
	.4byte Data_02005264
.L_020096a8:
	movs r0, #168
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	lsls r0, r0, #2
	bl Engine_ObjectCreate
	adds r5, r0, #0
	movs r0, #246
	bl Func_02005014
	bl Random16Far
	movs r3, #31
	ldr r2, [r5, #8]
	ands r3, r0
	subs r3, #16
	lsls r3, r3, #16
	adds r2, r2, r3
	str r2, [r5, #8]
	bl Random16Far
	movs r3, #15
	ldr r2, [r5, #12]
	ands r3, r0
	subs r3, #8
	lsls r3, r3, #16
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #48]
	movs r3, #204
	lsls r3, r3, #6
	str r2, [r5, #12]
	adds r3, #51
	adds r2, r5, #0
	adds r2, #85
	str r3, [r5, #52]
	movs r3, #0
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #7
	bl Func_02004d9c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	ldr r1, .L_02009768
	adds r0, r5, #0
	bl Func_02004dac
	movs r6, #128
	ldr r3, .L_0200975c
	ldr r5, .L_02009760
	lsls r6, r6, #19
	adds r6, #82
	strh r3, [r6]
	movs r0, #2
	bl WaitFrames
	movs r0, #2
	strh r5, [r6]
	bl WaitFrames
	ldr r3, .L_02009764
	movs r0, #2
	strh r3, [r6]
	bl WaitFrames
	strh r5, [r6]
	movs r0, #2
	bl WaitFrames
	movs r2, #1
	add r8, r2
	mov r3, r8
	b .L_0200976c
.L_0200975c:
	.4byte 0x00001004
.L_02009760:
	.4byte 0x0000100a
.L_02009764:
	.4byte 0x00001010
.L_02009768:
	.4byte Data_02005218
.L_0200976c:
	cmp r3, #16
	bne .L_020096a8
	ldr r3, .L_020097ac
	movs r0, #30
	strh r3, [r6]
	bl WaitFrames
	movs r0, #0
	mov r8, r0
.L_0200977e:
	mov r0, r10
	ldr r3, [r0, #16]
	mov r2, r10
	movs r0, #168
	ldr r1, [r2, #8]
	lsls r0, r0, #2
	ldr r2, [r2, #12]
	bl Engine_ObjectCreate
	adds r5, r0, #0
	movs r0, #195
	bl Func_02005014
	bl Random16Far
	ldr r3, .L_020097b0
	movs r2, #128
	ands r0, r3
	lsls r2, r2, #8
	adds r6, r5, #0
	adds r0, r0, r2
	adds r6, #100
	b .L_020097b4
.L_020097ac:
	.4byte 0x00001008
.L_020097b0:
	.4byte 0x00007fff
.L_020097b4:
	strh r0, [r6]
	bl Random16Far
	mov r3, r8
	movs r2, #1
	ands r2, r3
	movs r3, #3
	ands r3, r0
	lsls r2, r2, #1
	adds r3, #9
	subs r2, #1
	lsls r2, r3
	ldr r7, .L_02009808
	adds r3, r5, #0
	adds r3, #102
	strh r2, [r3]
	subs r3, #17
	strb r7, [r3]
	movs r3, #244
	lsls r3, r3, #15
	str r3, [r5, #76]
	mov r2, r8
	movs r3, #16
	subs r3, r3, r2
	lsls r3, r3, #3
	adds r2, r5, #0
	adds r3, #15
	adds r2, #98
	mov r0, r10
	str r0, [r5, #104]
	movs r1, #2
	strb r3, [r2]
	adds r0, r5, #0
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #7
	b .L_0200980c
.L_02009808:
	.4byte 0x00000000
.L_0200980c:
	bl Func_02004d9c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	adds r2, r5, #0
	movs r3, #0
	ldrsh r1, [r6, r3]
	ldr r0, [r5, #76]
	adds r2, #8
	bl Vector_AddPolarOffsetFar
	adds r0, r5, #0
	ldr r1, .L_02009904
	bl Func_02004dac
	mov r0, r8
	cmp r0, #3
	bne .L_02009860
	movs r1, #128
	mov r0, r11
	lsls r1, r1, #1
	bl Func_02004efc
	mov r3, r10
	adds r3, #85
	strb r7, [r3]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	mov r2, r10
	str r3, [r2, #52]
	str r3, [r2, #48]
	mov r0, r11
	ldr r1, .L_02009908
	bl ObjectMotion_EnableActionAndSetCallback
.L_02009860:
	movs r0, #8
	bl WaitFrames
	movs r3, #1
	add r8, r3
	mov r0, r8
	cmp r0, #16
	beq .L_02009872
	b .L_0200977e
.L_02009872:
	movs r0, #220
	bl Func_02005014
	movs r0, #16
	bl WaitFrames
	movs r2, #2
	mov r8, r2
.L_02009882:
	mov r3, r10
	ldr r1, [r3, #8]
	ldr r3, [r3, #12]
	mov r0, r8
	lsls r2, r0, #16
	adds r2, r2, r3
	ldr r3, .L_0200990c
	mov r0, r10
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #168
	lsls r0, r0, #2
	bl Engine_ObjectCreate
	adds r5, r0, #0
	bl Random16Far
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	adds r2, r5, #0
	movs r3, #128
	adds r2, #102
	lsls r3, r3, #4
	strh r3, [r2]
	adds r3, r5, #0
	mov r2, r8
	adds r3, #98
	strb r2, [r3]
	movs r2, #1
	adds r3, #1
	strb r2, [r3]
	ldr r1, .L_02009900
	ldr r3, [r5, #8]
	movs r6, #0
	str r3, [r5, #68]
	ldr r3, [r5, #16]
	str r6, [r5, #76]
	str r3, [r5, #72]
	mov r3, r10
	str r3, [r5, #104]
	adds r3, r5, #0
	adds r3, #85
	strb r1, [r3]
	subs r3, #50
	strb r2, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #7
	bl Func_02004d9c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	b .L_02009910
.L_02009900:
	.4byte 0x00000000
.L_02009904:
	.4byte Data_02005234
.L_02009908:
	.4byte Data_02005288
.L_0200990c:
	.4byte 0xfff80000
.L_02009910:
	adds r0, r5, #0
	ldr r1, .L_020099bc
	bl Func_02004dac
	movs r0, #2
	add r8, r0
	mov r2, r8
	cmp r2, #32
	bne .L_02009882
	movs r0, #220
	bl Func_02005014
	movs r0, #50
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02004f94
	movs r0, #8
	bl Func_02004fa4
	movs r0, #16
	bl WaitFrames
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_02004e24
	mov r0, r9
	movs r1, #0
	bl Func_02004efc
	mov r0, r11
	movs r1, #0
	bl Func_02004efc
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #62
	bl Func_02005014
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02004f94
	movs r0, #80
	bl Func_02004fa4
	mov r0, r9
	ldr r1, .L_020099c0
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_020099c4
	mov r0, r11
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r3, .L_020099c8
	mov r0, r10
	str r3, [r0, #108]
	movs r0, #120
	bl WaitFrames
	mov r2, r10
	movs r0, #195
	str r6, [r2, #108]
	lsls r0, r0, #1
	bl Func_02005014
	bl Func_02004fd4
	movs r0, #1
	bl WaitFrames
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020099bc:
	.4byte Data_02005244
.L_020099c0:
	.4byte Data_020052ac
.L_020099c4:
	.4byte Data_020052dc
.L_020099c8:
	.4byte Func_02001548
	.section .text.x020099cc,"ax",%progbits
	.global Func_020019cc
	.thumb_func
Func_020019cc:
	push {r5, r6, r7, lr}
	ldr r3, .L_02009b50
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #4
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	ldr r7, .L_02009b54
	movs r1, #1
	adds r0, r7, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #1
	movs r2, #0
	bl Func_02004f2c
	ldr r3, [r5, #16]
	asrs r3, r3, #19
	cmp r3, #39
	bgt .L_02009a1c
	movs r1, #220
	movs r2, #164
	ldr r0, [r6]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
.L_02009a1c:
	ldr r1, [r6]
	movs r0, #11
	bl Func_02004ec4
	movs r1, #235
	movs r2, #177
	ldr r0, [r6]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #223
	movs r2, #177
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #11
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #11
	bl ObjectMotion_ArmCallback
	adds r0, r7, #1
	bl Func_02004f0c
	movs r1, #0
	movs r0, #11
	bl Func_02004f14
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r6]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #233
	movs r2, #171
	lsls r2, r2, #1
	ldr r0, [r6]
	lsls r1, r1, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	adds r0, r7, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004e24
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02004e24
	bl Func_02004e2c
	bl Func_02004ddc
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	ldrh r2, [r3]
	mov r6, sp
	movs r3, #255
	adds r6, #2
	ands r3, r2
	adds r7, r6, #0
	strh r3, [r6]
	cmp r3, #0
	beq .L_02009b58
.L_02009b20:
	ldrh r3, [r7]
	ldr r2, .L_02009b4c
	movs r1, #128
	lsls r1, r1, #19
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	ldrh r3, [r7]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	strh r3, [r7]
	adds r5, r7, #0
	movs r0, #1
	bl WaitFrames
	ldrh r3, [r5]
	cmp r3, #0
	bne .L_02009b20
	b .L_02009b58
	.2byte 0x0000
.L_02009b4c:
	.4byte 0x00001000
.L_02009b50:
	.4byte gPartyState
.L_02009b54:
	.4byte MsgScene681No09
.L_02009b58:
	movs r5, #128
	lsls r5, r5, #19
	ldrh r2, [r5]
	movs r3, #253
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r5]
	movs r1, #4
	movs r0, #12
	bl Func_020015d0
	ldrh r3, [r5]
	ldr r2, .L_02009ba0
	movs r1, #128
	orrs r3, r2
	strh r3, [r5]
	ldr r2, .L_02009ba4
	ldrh r3, [r6]
	lsls r1, r1, #19
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	movs r2, #128
	ldr r3, .L_02009ba8
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldrh r3, [r6]
	cmp r3, #8
	beq .L_02009bc6
.L_02009b96:
	ldrh r3, [r6]
	ldr r2, .L_02009ba4
	movs r1, #128
	lsls r1, r1, #19
	b .L_02009bac
.L_02009ba0:
	.4byte 0x00000f00
.L_02009ba4:
	.4byte 0x00001000
.L_02009ba8:
	.4byte 0x00003f42
.L_02009bac:
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	adds r5, r6, #0
	ldrh r3, [r6]
	movs r0, #1
	adds r3, #1
	strh r3, [r6]
	bl WaitFrames
	ldrh r3, [r5]
	cmp r3, #8
	bne .L_02009b96
.L_02009bc6:
	bl Func_02004dd4
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl Func_02004ebc
	movs r1, #139
	movs r0, #4
	bl UiText_DrawQuantityPairWithCue
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #186
	bl GameFlag_SetBit
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #11
	ldr r1, .L_02009c3c
	bl ObjectMotion_SetSpeedParameters
	movs r0, #11
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_02009c40
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009c1c
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #11
	bl ObjectMotion_ResetAndSetPosition
.L_02009c1c:
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl Func_02004ebc
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02004e64
	add sp, #4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c3c:
	.4byte 0x00013333
.L_02009c40:
	.4byte gPartyState
	.section .text.x02009c9c,"ax",%progbits
	.balign 4
	.global Func_02001c9c
	.thumb_func
Func_02001c9c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009d0c
	sub sp, #68
	add r2, sp, #16
	str r3, [r2, #36]
	mov r10, r2
	movs r7, #0
.L_02009cb0:
	lsls r6, r7, #12
	adds r0, r6, #0
	bl Math_Cosine
	add r5, sp, #56
	movs r3, #0
	str r0, [r5]
	adds r0, r6, #0
	str r3, [r5, #4]
	bl Math_Sine
	ldr r6, [r5]
	mov r8, r0
	str r0, [r5, #8]
	movs r1, #3
	adds r0, r6, #0
	bl __divsi3
	ldr r3, [r5, #4]
	adds r6, r6, r0
	str r6, [r5]
	str r3, [sp, #0]
	mov r3, r8
	str r3, [sp, #4]
	movs r3, #128
	mov r2, r10
	lsls r3, r3, #17
	adds r3, #1
	str r2, [sp, #12]
	movs r0, #232
	movs r2, #164
	str r3, [sp, #8]
	lsls r0, r0, #17
	ldr r1, .L_02009d10
	lsls r2, r2, #17
	adds r3, r6, #0
	adds r7, #2
	bl Func_0200015c
	cmp r7, #16
	bls .L_02009cb0
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009d0c:
	.4byte Effect_AdvanceMotionLocal
.L_02009d10:
	.4byte 0xffe00000
	.section .text.x02009d14,"ax",%progbits
	.global Func_02001d14
	.thumb_func
Func_02001d14:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #238
	sub sp, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d2c
	b .L_02009eb2
.L_02009d2c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #186
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d3c
	b .L_02009eb2
.L_02009d3c:
	movs r0, #12
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	ldrh r3, [r3]
	mov r5, sp
	adds r5, #2
	strh r3, [r5]
	mov r8, r0
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r1, r1, #12
	lsls r2, r2, #9
	lsls r0, r0, #12
	bl Func_02004e24
	movs r0, #107
	bl Func_02005014
	bl Func_02004ddc
	ldrb r3, [r5]
	strh r3, [r5]
	cmp r3, #0
	beq .L_02009dac
.L_02009d7e:
	ldrh r3, [r5]
	ldr r2, .L_02009da8
	movs r1, #128
	lsls r1, r1, #19
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	ldrh r3, [r5]
	cmp r3, #0
	bne .L_02009d7e
	b .L_02009dac
	.2byte 0x0000
.L_02009da8:
	.4byte 0x00001000
.L_02009dac:
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #253
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	movs r0, #30
	movs r6, #232
	movs r5, #174
	bl Battle_WaitMode0
	lsls r5, r5, #17
	lsls r6, r6, #17
	movs r3, #68
	adds r3, #255
	adds r2, r5, #0
	ldr r1, .L_02009e88
	adds r0, r6, #0
	bl OverlayObject_PrepareObject
	adds r2, r5, #0
	adds r1, r6, #0
	adds r7, r0, #0
	movs r0, #12
	bl Func_02004ebc
	bl Func_02001c9c
	movs r5, #0
.L_02009dea:
	cmp r5, #10
	bne .L_02009df2
	bl Func_02001c9c
.L_02009df2:
	mov r2, r8
	ldr r3, [r2, #16]
	ldr r2, .L_02009e8c
	movs r0, #2
	adds r3, r3, r2
	mov r2, r8
	str r3, [r2, #16]
	adds r5, #1
	bl Battle_WaitMode0
	cmp r5, #19
	ble .L_02009dea
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #238
	bl GameFlag_SetBit
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004e24
	movs r3, #0
	str r3, [r7, #8]
	str r3, [r7, #16]
	movs r0, #1
	bl Battle_WaitMode0
	adds r0, r7, #0
	bl Func_02004dbc
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02005014
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_02009e7c
	mov r6, sp
	orrs r3, r2
	strh r3, [r1]
	adds r6, #2
	ldrh r3, [r6]
	ldr r2, .L_02009e80
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
	movs r2, #128
	ldr r3, .L_02009e84
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldrh r3, [r6]
	cmp r3, #8
	beq .L_02009eaa
.L_02009e70:
	ldrh r3, [r6]
	ldr r2, .L_02009e80
	movs r1, #128
	lsls r1, r1, #19
	b .L_02009e90
	.2byte 0x0000
.L_02009e7c:
	.4byte 0x00000f00
.L_02009e80:
	.4byte 0x00001000
.L_02009e84:
	.4byte 0x00003f42
.L_02009e88:
	.4byte 0xffe00000
.L_02009e8c:
	.4byte 0xffff0000
.L_02009e90:
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	adds r5, r6, #0
	ldrh r3, [r6]
	movs r0, #1
	adds r3, #1
	strh r3, [r6]
	bl WaitFrames
	ldrh r3, [r5]
	cmp r3, #8
	bne .L_02009e70
.L_02009eaa:
	bl Func_02004dd4
	bl Func_02004e64
.L_02009eb2:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009ebc,"ax",%progbits
	.global Func_02001ebc
	.thumb_func
Func_02001ebc:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009eda
	ldr r0, .L_02009f5c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	b .L_02009f54
.L_02009eda:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #188
	bl GameFlag_Test
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #189
	bl GameFlag_Test
	mov r8, r0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #190
	bl GameFlag_Test
	adds r6, r0, #0
	movs r0, #140
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	add r5, r8
	adds r5, r5, r6
	adds r5, r5, r0
	ldr r3, .L_02009f60
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	ldr r6, .L_02009f64
	movs r1, #10
	adds r0, r6, #0
	bl Party_SetFields1eeAnd1f0
	movs r0, #196
	adds r0, #255
	bl PartyInventory_FindOwner
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_02009f44
	cmp r5, #0
	bne .L_02009f4c
	adds r0, r6, #0
	movs r1, #12
	bl Party_SetFields1f2And1f4
	b .L_02009f4c
.L_02009f44:
	adds r0, r6, #0
	movs r1, #11
	bl Party_SetFields1f2And1f4
.L_02009f4c:
	movs r0, #12
	adds r1, r5, #0
	bl Func_02004f74
.L_02009f54:
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009f5c:
	.4byte MsgScene681No08
.L_02009f60:
	.4byte gPartyState
.L_02009f64:
	.4byte 0x000000b1
	.section .text.x0200a020,"ax",%progbits
	.global Func_02002020
	.thumb_func
Func_02002020:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #8
	sub sp, #8
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #9
	bl Object_GetById
	mov r8, r0
	movs r0, #13
	bl Object_GetById
	mov r10, r0
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	movs r1, #1
	ldr r0, .L_0200a254
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #193
	bl Func_02005014
	mov r0, r10
	movs r1, #1
	bl Func_02004e3c
	ldr r5, .L_0200a258
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r1, [r5]
	movs r0, #13
	bl Func_02004ec4
	movs r1, #248
	movs r2, #178
	lsls r1, r1, #1
	movs r0, #13
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	movs r3, #192
	lsls r3, r3, #11
	mov r2, r10
	str r3, [r2, #40]
	movs r0, #13
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #45
	bl Battle_WaitMode0
	ldr r1, [r5]
	movs r0, #8
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	ldr r1, [r5]
	movs r0, #9
	bl ObjectMotion_SetAngleToward
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005014
	movs r0, #8
	movs r1, #5
	bl Object_SetModeById
	movs r1, #5
	movs r0, #9
	bl Object_SetModeById
	movs r0, #30
	bl WaitFrames
	movs r0, #8
	movs r1, #2
	bl Object_SetModeById
	movs r1, #2
	movs r0, #9
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #191
	bl Func_02005014
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #98
	bl GameFlag_Test
	movs r5, #119
	cmp r0, #0
	bne .L_0200a1d8
	movs r5, #19
.L_0200a10a:
	bl Random16Far
	str r0, [r7, #40]
	bl Random16Far
	mov r3, r8
	str r0, [r3, #40]
	subs r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a10a
	movs r2, #192
	movs r1, #128
	lsls r2, r2, #4
	movs r0, #8
	lsls r1, r1, #7
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #192
	movs r1, #128
	lsls r2, r2, #4
	movs r0, #9
	lsls r1, r1, #7
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #24
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	adds r6, r7, #0
	movs r1, #24
	movs r0, #9
	negs r1, r1
	movs r2, #0
	adds r6, #86
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldrb r3, [r6]
	movs r5, #0
	cmp r3, #0
	beq .L_0200a18a
.L_0200a166:
	bl Random16Far
	str r0, [r7, #40]
	bl Random16Far
	mov r2, r8
	str r0, [r2, #40]
	movs r0, #1
	bl WaitFrames
	movs r3, #44
	adds r5, #1
	adds r3, #255
	cmp r5, r3
	bgt .L_0200a18a
	ldrb r3, [r6]
	cmp r3, #0
	bne .L_0200a166
.L_0200a18a:
	movs r5, #9
.L_0200a18c:
	bl Random16Far
	str r0, [r7, #40]
	bl Random16Far
	mov r2, r8
	str r0, [r2, #40]
	subs r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a18c
	movs r3, #89
	str r3, [sp, #0]
	movs r5, #40
	movs r0, #119
	movs r1, #31
	movs r2, #1
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02004df4
	movs r3, #100
	str r3, [sp, #0]
	movs r0, #119
	movs r1, #31
	movs r2, #1
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02004df4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #98
	bl GameFlag_SetBit
	b .L_0200a1f8
.L_0200a1d8:
	bl Random16Far
	str r0, [r7, #40]
	bl Random16Far
	mov r3, r8
	str r0, [r3, #40]
	subs r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a1d8
	movs r0, #30
	bl Battle_WaitMode0
.L_0200a1f8:
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r3, .L_0200a258
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a22e
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #13
	bl ObjectMotion_ResetAndSetPosition
.L_0200a22e:
	movs r3, #192
	lsls r3, r3, #11
	mov r2, r10
	str r3, [r2, #40]
	movs r0, #13
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	bl Func_02004e64
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a254:
	.4byte MsgScene681No01
.L_0200a258:
	.4byte gPartyState
	.section .text.x0200a25c,"ax",%progbits
	.global Func_0200225c
	.thumb_func
Func_0200225c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #8
	sub sp, #8
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #12
	bl Object_GetById
	mov r8, r0
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	movs r1, #1
	ldr r0, .L_0200a4b0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #193
	bl Func_02005014
	mov r0, r8
	movs r1, #1
	bl Func_02004e3c
	ldr r5, .L_0200a4b4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r1, [r5]
	movs r0, #12
	bl Func_02004ec4
	movs r1, #132
	movs r2, #186
	lsls r1, r1, #2
	movs r0, #12
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPosition
	movs r3, #192
	lsls r3, r3, #11
	mov r2, r8
	str r3, [r2, #40]
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #45
	bl Battle_WaitMode0
	movs r2, #0
	ldr r1, [r5]
	movs r0, #8
	bl ObjectMotion_SetAngleToward
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005014
	movs r1, #5
	movs r0, #8
	bl Object_SetModeById
	movs r0, #30
	bl WaitFrames
	movs r1, #2
	movs r0, #8
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #191
	bl Func_02005014
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #189
	bl GameFlag_Test
	movs r5, #119
	cmp r0, #0
	beq .L_0200a31a
	b .L_0200a444
.L_0200a31a:
	movs r5, #19
.L_0200a31c:
	bl Random16Far
	subs r5, #1
	str r0, [r7, #40]
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a31c
	movs r2, #192
	movs r1, #128
	lsls r2, r2, #4
	movs r0, #8
	lsls r1, r1, #7
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #24
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #28
	movs r5, #22
	str r3, [sp, #0]
	movs r0, #27
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02004dfc
	adds r6, r7, #0
	movs r3, #29
	str r3, [sp, #0]
	movs r0, #32
	movs r3, #1
	movs r1, #16
	movs r2, #2
	adds r6, #86
	str r5, [sp, #4]
	bl Func_02004dfc
	ldrb r3, [r6]
	movs r5, #0
	cmp r3, #0
	beq .L_0200a396
.L_0200a37a:
	bl Random16Far
	str r0, [r7, #40]
	movs r0, #1
	bl WaitFrames
	movs r3, #44
	adds r5, #1
	adds r3, #255
	cmp r5, r3
	bgt .L_0200a396
	ldrb r3, [r6]
	cmp r3, #0
	bne .L_0200a37a
.L_0200a396:
	movs r3, #28
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #2
	movs r1, #93
	movs r2, #1
	movs r3, #5
	bl Func_02004df4
	movs r5, #9
.L_0200a3ac:
	bl Random16Far
	subs r5, #1
	str r0, [r7, #40]
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a3ac
	movs r0, #141
	lsls r0, r0, #2
	bl Func_02005014
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r3, .L_0200a4b4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a3f4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #12
	bl ObjectMotion_ResetAndSetPosition
.L_0200a3f4:
	movs r3, #192
	lsls r3, r3, #11
	mov r2, r8
	str r3, [r2, #40]
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a434
	movs r0, #194
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02004fdc
	movs r0, #20
	bl Func_02004f6c
	b .L_0200a4a6
.L_0200a434:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #189
	bl GameFlag_SetBit
	bl Func_02004e64
	b .L_0200a4a6
.L_0200a444:
	bl Random16Far
	subs r5, #1
	str r0, [r7, #40]
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a444
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r3, .L_0200a4b4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a48a
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #12
	bl ObjectMotion_ResetAndSetPosition
.L_0200a48a:
	movs r3, #192
	lsls r3, r3, #11
	mov r2, r8
	str r3, [r2, #40]
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	bl Func_02004e64
.L_0200a4a6:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a4b0:
	.4byte MsgScene681No01
.L_0200a4b4:
	.4byte gPartyState
	.section .text.x0200a4b8,"ax",%progbits
	.global Func_020024b8
	.thumb_func
Func_020024b8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #9
	sub sp, #8
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #12
	bl Object_GetById
	mov r8, r0
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	movs r1, #1
	ldr r0, .L_0200a710
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #193
	bl Func_02005014
	mov r0, r8
	movs r1, #1
	bl Func_02004e3c
	ldr r5, .L_0200a714
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r1, [r5]
	movs r0, #12
	bl Func_02004ec4
	movs r1, #168
	movs r2, #186
	lsls r1, r1, #2
	movs r0, #12
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPosition
	movs r3, #192
	lsls r3, r3, #11
	mov r2, r8
	str r3, [r2, #40]
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #45
	bl Battle_WaitMode0
	movs r2, #0
	ldr r1, [r5]
	movs r0, #9
	bl ObjectMotion_SetAngleToward
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005014
	movs r1, #5
	movs r0, #9
	bl Object_SetModeById
	movs r0, #30
	bl WaitFrames
	movs r1, #2
	movs r0, #9
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #191
	bl Func_02005014
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #190
	bl GameFlag_Test
	movs r5, #119
	cmp r0, #0
	beq .L_0200a576
	b .L_0200a6a6
.L_0200a576:
	movs r5, #19
.L_0200a578:
	bl Random16Far
	subs r5, #1
	str r0, [r7, #40]
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a578
	movs r2, #192
	movs r1, #128
	lsls r2, r2, #4
	movs r0, #9
	lsls r1, r1, #7
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #152
	movs r2, #176
	movs r0, #9
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPosition
	movs r3, #36
	movs r5, #22
	str r3, [sp, #0]
	movs r0, #35
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02004dfc
	adds r6, r7, #0
	movs r3, #37
	str r3, [sp, #0]
	movs r0, #32
	movs r3, #1
	movs r1, #16
	movs r2, #2
	adds r6, #86
	str r5, [sp, #4]
	bl Func_02004dfc
	ldrb r3, [r6]
	movs r5, #0
	cmp r3, #0
	beq .L_0200a5f6
.L_0200a5da:
	bl Random16Far
	str r0, [r7, #40]
	movs r0, #1
	bl WaitFrames
	movs r3, #44
	adds r5, #1
	adds r3, #255
	cmp r5, r3
	bgt .L_0200a5f6
	ldrb r3, [r6]
	cmp r3, #0
	bne .L_0200a5da
.L_0200a5f6:
	movs r3, #36
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #2
	movs r1, #93
	movs r2, #1
	movs r3, #5
	bl Func_02004df4
	movs r5, #9
.L_0200a60c:
	bl Random16Far
	subs r5, #1
	str r0, [r7, #40]
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a60c
	movs r0, #141
	lsls r0, r0, #2
	bl Func_02005014
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r3, .L_0200a714
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a654
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #12
	bl ObjectMotion_ResetAndSetPosition
.L_0200a654:
	movs r3, #192
	lsls r3, r3, #11
	mov r2, r8
	str r3, [r2, #40]
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a696
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #9
	bl GameFlag_SetBit
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02004fdc
	movs r0, #20
	bl Func_02004f6c
	b .L_0200a708
.L_0200a696:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #190
	bl GameFlag_SetBit
	bl Func_02004e64
	b .L_0200a708
.L_0200a6a6:
	bl Random16Far
	subs r5, #1
	str r0, [r7, #40]
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a6a6
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r3, .L_0200a714
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a6ec
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #12
	bl ObjectMotion_ResetAndSetPosition
.L_0200a6ec:
	movs r3, #192
	lsls r3, r3, #11
	mov r2, r8
	str r3, [r2, #40]
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	bl Func_02004e64
.L_0200a708:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200a710:
	.4byte MsgScene681No01
.L_0200a714:
	.4byte gPartyState
	.section .text.x0200a718,"ax",%progbits
	.global Func_02002718
	.thumb_func
Func_02002718:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #8
	sub sp, #8
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #14
	bl Object_GetById
	mov r8, r0
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	movs r1, #1
	ldr r0, .L_0200a96c
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #193
	bl Func_02005014
	mov r0, r8
	movs r1, #1
	bl Func_02004e3c
	ldr r5, .L_0200a970
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r1, [r5]
	movs r0, #14
	bl Func_02004ec4
	movs r1, #168
	movs r2, #198
	lsls r1, r1, #1
	movs r0, #14
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPosition
	movs r3, #192
	lsls r3, r3, #11
	mov r2, r8
	str r3, [r2, #40]
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #45
	bl Battle_WaitMode0
	movs r2, #0
	ldr r1, [r5]
	movs r0, #8
	bl ObjectMotion_SetAngleToward
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005014
	movs r1, #5
	movs r0, #8
	bl Object_SetModeById
	movs r0, #30
	bl WaitFrames
	movs r1, #2
	movs r0, #8
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #191
	bl Func_02005014
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #188
	bl GameFlag_Test
	movs r5, #119
	cmp r0, #0
	beq .L_0200a7d6
	b .L_0200a902
.L_0200a7d6:
	movs r5, #19
.L_0200a7d8:
	bl Random16Far
	subs r5, #1
	str r0, [r7, #40]
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a7d8
	movs r2, #192
	movs r1, #128
	lsls r2, r2, #4
	movs r0, #8
	lsls r1, r1, #7
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #16
	movs r0, #8
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r5, #23
	movs r0, #27
	movs r1, #15
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02004dfc
	adds r6, r7, #0
	movs r3, #25
	str r3, [sp, #0]
	movs r0, #25
	movs r3, #1
	movs r1, #25
	movs r2, #1
	adds r6, #86
	str r5, [sp, #4]
	bl Func_02004dfc
	ldrb r3, [r6]
	movs r5, #0
	cmp r3, #0
	beq .L_0200a852
.L_0200a836:
	bl Random16Far
	str r0, [r7, #40]
	movs r0, #1
	bl WaitFrames
	movs r3, #44
	adds r5, #1
	adds r3, #255
	cmp r5, r3
	bgt .L_0200a852
	ldrb r3, [r6]
	cmp r3, #0
	bne .L_0200a836
.L_0200a852:
	movs r3, #25
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #1
	movs r1, #85
	movs r2, #1
	movs r3, #4
	bl Func_02004df4
	movs r5, #9
.L_0200a868:
	bl Random16Far
	subs r5, #1
	str r0, [r7, #40]
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a868
	movs r0, #141
	lsls r0, r0, #2
	bl Func_02005014
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r3, .L_0200a970
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a8b0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #14
	bl ObjectMotion_ResetAndSetPosition
.L_0200a8b0:
	movs r3, #192
	lsls r3, r3, #11
	mov r2, r8
	str r3, [r2, #40]
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a8f2
	movs r0, #130
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02004fdc
	movs r0, #20
	bl Func_02004f6c
	b .L_0200a964
.L_0200a8f2:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #188
	bl GameFlag_SetBit
	bl Func_02004e64
	b .L_0200a964
.L_0200a902:
	bl Random16Far
	subs r5, #1
	str r0, [r7, #40]
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a902
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r3, .L_0200a970
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a948
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #14
	bl ObjectMotion_ResetAndSetPosition
.L_0200a948:
	movs r3, #192
	lsls r3, r3, #11
	mov r2, r8
	str r3, [r2, #40]
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	bl Func_02004e64
.L_0200a964:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200a96c:
	.4byte MsgScene681No01
.L_0200a970:
	.4byte gPartyState
	.section .text.x0200a974,"ax",%progbits
	.global Func_02002974
	.thumb_func
Func_02002974:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r1, #0
	asrs r3, r3, #19
	mov r8, r3
	ldr r3, [r0, #16]
	movs r0, #11
	mov r9, r1
	mov r10, r1
	asrs r7, r3, #19
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r2, .L_0200aa10
	asrs r6, r3, #19
	ldr r3, [r0, #16]
	movs r4, #1
	asrs r3, r3, #19
	mov r11, r3
	lsls r1, r4, #24
	mov r12, r2
.L_0200a9b0:
	mov r0, r12
	asrs r2, r1, #22
	ldrsh r3, [r0, r2]
	mov lr, r3
	cmp r8, lr
	bne .L_0200a9c8
	adds r3, r2, #2
	ldrsh r3, [r0, r3]
	cmp r7, r3
	bne .L_0200a9c8
	lsrs r1, r1, #24
	mov r9, r1
.L_0200a9c8:
	lsls r2, r4, #24
	asrs r1, r2, #22
	ldrsh r3, [r0, r1]
	cmp r6, r3
	bne .L_0200a9de
	adds r3, r1, #2
	ldrsh r3, [r0, r3]
	cmp r11, r3
	bne .L_0200a9de
	lsrs r1, r2, #24
	mov r10, r1
.L_0200a9de:
	movs r4, #128
	lsls r4, r4, #17
	adds r3, r2, r4
	lsrs r4, r3, #24
	movs r5, #176
	lsls r1, r4, #24
	lsls r5, r5, #20
	cmp r1, r5
	ble .L_0200a9b0
	mov r1, r10
	mov r2, r9
	lsls r3, r1, #28
	lsls r1, r2, #24
	orrs r1, r3
	movs r0, #132
	asrs r1, r1, #24
	lsls r0, r0, #2
	bl GameFlag_SetByte
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200aa10:
	.4byte Data_0200530c
	.section .text.x0200ab8c,"ax",%progbits
	.balign 4
	.global Func_02002b8c
	.thumb_func
Func_02002b8c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200ac64
	movs r2, #2
	mov r8, r1
	movs r3, #192
	movs r0, #131
	add r2, r8
	lsls r3, r3, #18
	lsls r0, r0, #1
	mov r10, r2
	ldr r7, [r3, #108]
	bl GameFlag_Test
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r6, r0, #0
	cmp r3, #0
	beq .L_0200abc0
	movs r3, #1
	orrs r6, r3
.L_0200abc0:
	movs r5, #8
.L_0200abc2:
	adds r0, r5, #0
	bl Object_GetById
	adds r5, #1
	adds r0, #91
	strb r6, [r0]
	cmp r5, #16
	ble .L_0200abc2
	cmp r6, #0
	bne .L_0200acac
	mov r1, r10
	ldrh r3, [r1]
	movs r1, #171
	adds r2, r3, #1
	mov r3, r10
	strh r2, [r3]
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200acac
	lsls r3, r2, #16
	cmp r3, #0
	beq .L_0200acac
.L_0200abf4:
	mov r2, r8
	ldrh r3, [r2]
	cmp r3, #60
	beq .L_0200ac02
	cmp r3, #180
	beq .L_0200ac68
	b .L_0200ac7a
.L_0200ac02:
	movs r0, #8
	movs r1, #0
	movs r2, #3
	bl Func_02002b58
	movs r0, #9
	movs r1, #3
	movs r2, #0
	bl Func_02002b58
	movs r5, #3
	movs r0, #10
	movs r1, #0
	movs r2, #3
	bl Func_02002b58
	negs r5, r5
	movs r0, #11
	movs r1, #0
	movs r2, #3
	bl Func_02002b58
	movs r0, #12
	movs r1, #0
	adds r2, r5, #0
	bl Func_02002b58
	movs r0, #13
	movs r1, #0
	adds r2, r5, #0
	bl Func_02002b58
	movs r0, #14
	movs r1, #0
	movs r2, #3
	bl Func_02002b58
	movs r0, #15
	movs r1, #0
	adds r2, r5, #0
	bl Func_02002b58
	movs r0, #16
	movs r1, #3
	movs r2, #0
	bl Func_02002b58
	b .L_0200ac7a
	.2byte 0x0000
.L_0200ac64:
	.4byte gSceneState
.L_0200ac68:
	movs r5, #8
.L_0200ac6a:
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	adds r5, #1
	bl Func_02002b58
	cmp r5, #16
	ble .L_0200ac6a
.L_0200ac7a:
	mov r1, r8
	ldrh r3, [r1]
	mov r2, r8
	adds r3, #1
	movs r1, #240
	strh r3, [r2]
	lsls r1, r1, #16
	lsls r3, r3, #16
	cmp r3, r1
	bne .L_0200ac92
	ldr r3, .L_0200aca8
	strh r3, [r2]
.L_0200ac92:
	mov r1, r10
	ldrh r3, [r1]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200abf4
	b .L_0200acac
.L_0200aca8:
	.4byte 0x00000000
.L_0200acac:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.section .text.x0200acb4,"ax",%progbits
	.global Func_02002cb4
	.thumb_func
Func_02002cb4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsls r0, r0, #24
	lsrs r0, r0, #24
	ldr r5, .L_0200ad24
	mov r8, r0
	movs r0, #10
	adds r0, #255
	adds r6, r5, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ad14
	strh r0, [r5]
	strh r0, [r6]
	bl Func_02004fe4
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r7, #0
	movs r5, #8
	movs r6, #2
.L_0200acec:
	ldr r2, .L_0200ad28
	lsls r3, r7, #2
	ldrsh r1, [r2, r3]
	ldrsh r2, [r2, r6]
	adds r0, r5, #0
	lsls r1, r1, #19
	lsls r2, r2, #19
	bl Func_02004ebc
	mov r0, r8
	lsls r1, r0, #24
	asrs r1, r1, #24
	adds r0, r5, #0
	adds r5, #1
	bl Func_02002b04
	adds r6, #4
	adds r7, #1
	cmp r5, #16
	ble .L_0200acec
.L_0200ad14:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200ad2c
	bl Scheduler_AddOrUpdateCallback
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200ad24:
	.4byte gSceneState
.L_0200ad28:
	.4byte Data_0200533c
.L_0200ad2c:
	.4byte Func_02002b8c
	.section .text.x0200ad30,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02002d30
	.thumb_func
Func_02002d30:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #188
	sub sp, #8
	bl GameFlag_Test
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #190
	bl GameFlag_Test
	mov r8, r0
	mov r2, r8
	movs r0, #144
	lsls r2, r2, #24
	lsls r0, r0, #4
	lsrs r2, r2, #24
	adds r0, #189
	mov r8, r2
	bl GameFlag_Test
	adds r6, r0, #0
	movs r0, #140
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	lsls r5, r5, #24
	lsrs r5, r5, #24
	lsls r5, r5, #24
	asrs r5, r5, #24
	mov r10, r5
	mov r3, r8
	lsls r3, r3, #24
	add r6, r10
	asrs r7, r3, #24
	lsls r6, r6, #24
	lsrs r6, r6, #24
	adds r0, r7, r0
	lsls r0, r0, #24
	lsls r6, r6, #24
	lsrs r0, r0, #24
	mov r9, r3
	asrs r3, r6, #24
	mov r8, r0
	lsls r5, r0, #24
	cmp r3, #0
	beq .L_ja_0200ac8e
	cmp r3, #2
	bne .L_ja_0200abe8
	movs r3, #24
	movs r2, #76
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #12
	movs r0, #74
	movs r1, #115
	movs r2, #7
	bl Func_02004e04
	asrs r3, r5, #24
	cmp r3, #0
	bne .L_ja_0200abbc
	b .L_ja_0200ad22
.L_ja_0200abbc:
	cmp r3, #2
	bne .L_ja_0200abcc
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #71
	b .L_ja_0200ac6e
.L_ja_0200abcc:
	cmp r7, #0
	beq .L_ja_0200abdc
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #67
	b .L_ja_0200ac6e
.L_ja_0200abdc:
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #79
	b .L_ja_0200ac6e
.L_ja_0200abe8:
	mov r2, r10
	cmp r2, #0
	beq .L_ja_0200ac36
	movs r3, #24
	movs r2, #76
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #12
	movs r0, #66
	movs r1, #95
	movs r2, #7
	bl Func_02004e04
	asrs r3, r5, #24
	cmp r3, #0
	bne .L_ja_0200ac0a
	b .L_ja_0200ad22
.L_ja_0200ac0a:
	cmp r3, #2
	bne .L_ja_0200ac1a
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #81
	b .L_ja_0200ac6e
.L_ja_0200ac1a:
	cmp r7, #0
	beq .L_ja_0200ac2a
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #75
	b .L_ja_0200ac6e
.L_ja_0200ac2a:
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #65
	b .L_ja_0200ac6e
.L_ja_0200ac36:
	movs r3, #26
	movs r2, #76
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #12
	movs r0, #69
	movs r1, #82
	movs r2, #5
	bl Func_02004e04
	asrs r3, r5, #24
	cmp r3, #0
	beq .L_ja_0200ad22
	cmp r3, #2
	bne .L_ja_0200ac60
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #69
	b .L_ja_0200ac6e
.L_ja_0200ac60:
	cmp r7, #0
	beq .L_ja_0200ac7a
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #73
.L_ja_0200ac6e:
	movs r1, #110
	movs r2, #1
	movs r3, #3
	bl Func_02004e04
	b .L_ja_0200ac8e
.L_ja_0200ac7a:
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #77
	movs r1, #110
	movs r2, #1
	movs r3, #3
	bl Func_02004e04
.L_ja_0200ac8e:
	mov r2, r8
	lsls r3, r2, #24
	asrs r0, r3, #24
	cmp r0, #0
	beq .L_ja_0200ad22
	cmp r0, #2
	bne .L_ja_0200acc2
	movs r3, #31
	movs r2, #76
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #83
	movs r1, #115
	movs r2, #6
	movs r3, #12
	bl Func_02004e04
	cmp r6, #0
	bne .L_ja_0200ad22
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #82
	movs r1, #124
	b .L_ja_0200acec
.L_ja_0200acc2:
	mov r3, r9
	cmp r3, #0
	beq .L_ja_0200acf6
	movs r3, #31
	movs r2, #76
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #76
	movs r1, #82
	movs r2, #4
	movs r3, #12
	bl Func_02004e04
	cmp r6, #0
	bne .L_ja_0200ad22
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #75
	movs r1, #91
.L_ja_0200acec:
	movs r2, #1
	movs r3, #3
	bl Func_02004e04
	b .L_ja_0200ad22
.L_ja_0200acf6:
	movs r3, #31
	movs r2, #76
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #82
	movs r1, #82
	movs r2, #6
	movs r3, #12
	bl Func_02004e04
	cmp r6, #0
	bne .L_ja_0200ad22
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #81
	movs r1, #91
	movs r2, #1
	movs r3, #3
	bl Func_02004e04
.L_ja_0200ad22:
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.global Func_02002f30
	.thumb_func
Func_02002f30:
	.4byte 0x465fb5e0
	.4byte 0x464d4656
	.4byte 0x4647b4e0
	.4byte 0x23c0b480
	.4byte 0x6edb049b
	.4byte 0x004020d6
	.4byte 0x181b2281
	.4byte 0x4faa0092
	.4byte 0x21f0601a
	.4byte 0x187b0049
	.4byte 0x5e1a2000
	.4byte 0xb0874ba7
	.4byte 0xd001429a
	.else
	.global Func_02002d30
	.thumb_func
Func_02002d30:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #188
	sub sp, #8
	bl GameFlag_Test
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #190
	bl GameFlag_Test
	mov r8, r0
	mov r2, r8
	movs r0, #144
	lsls r2, r2, #24
	lsls r0, r0, #4
	lsrs r2, r2, #24
	adds r0, #189
	mov r8, r2
	bl GameFlag_Test
	adds r6, r0, #0
	movs r0, #140
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	lsls r5, r5, #24
	lsrs r5, r5, #24
	lsls r5, r5, #24
	asrs r5, r5, #24
	mov r10, r5
	mov r3, r8
	lsls r3, r3, #24
	add r6, r10
	asrs r7, r3, #24
	lsls r6, r6, #24
	lsrs r6, r6, #24
	adds r0, r7, r0
	lsls r0, r0, #24
	lsls r6, r6, #24
	lsrs r0, r0, #24
	mov r9, r3
	asrs r3, r6, #24
	mov r8, r0
	lsls r5, r0, #24
	cmp r3, #0
	beq .L_0200ae8e
	cmp r3, #2
	bne .L_0200ade8
	movs r3, #24
	movs r2, #76
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #12
	movs r0, #74
	movs r1, #115
	movs r2, #7
	bl Func_02004e04
	asrs r3, r5, #24
	cmp r3, #0
	bne .L_0200adbc
	b .L_0200af22
.L_0200adbc:
	cmp r3, #2
	bne .L_0200adcc
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #71
	b .L_0200ae6e
.L_0200adcc:
	cmp r7, #0
	beq .L_0200addc
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #67
	b .L_0200ae6e
.L_0200addc:
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #79
	b .L_0200ae6e
.L_0200ade8:
	mov r2, r10
	cmp r2, #0
	beq .L_0200ae36
	movs r3, #24
	movs r2, #76
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #12
	movs r0, #66
	movs r1, #95
	movs r2, #7
	bl Func_02004e04
	asrs r3, r5, #24
	cmp r3, #0
	bne .L_0200ae0a
	b .L_0200af22
.L_0200ae0a:
	cmp r3, #2
	bne .L_0200ae1a
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #81
	b .L_0200ae6e
.L_0200ae1a:
	cmp r7, #0
	beq .L_0200ae2a
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #75
	b .L_0200ae6e
.L_0200ae2a:
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #65
	b .L_0200ae6e
.L_0200ae36:
	movs r3, #26
	movs r2, #76
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #12
	movs r0, #69
	movs r1, #82
	movs r2, #5
	bl Func_02004e04
	asrs r3, r5, #24
	cmp r3, #0
	beq .L_0200af22
	cmp r3, #2
	bne .L_0200ae60
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #69
	b .L_0200ae6e
.L_0200ae60:
	cmp r7, #0
	beq .L_0200ae7a
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #73
.L_0200ae6e:
	movs r1, #110
	movs r2, #1
	movs r3, #3
	bl Func_02004e04
	b .L_0200ae8e
.L_0200ae7a:
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #77
	movs r1, #110
	movs r2, #1
	movs r3, #3
	bl Func_02004e04
.L_0200ae8e:
	mov r2, r8
	lsls r3, r2, #24
	asrs r0, r3, #24
	cmp r0, #0
	beq .L_0200af22
	cmp r0, #2
	bne .L_0200aec2
	movs r3, #31
	movs r2, #76
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #83
	movs r1, #115
	movs r2, #6
	movs r3, #12
	bl Func_02004e04
	cmp r6, #0
	bne .L_0200af22
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #82
	movs r1, #124
	b .L_0200aeec
.L_0200aec2:
	mov r3, r9
	cmp r3, #0
	beq .L_0200aef6
	movs r3, #31
	movs r2, #76
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #76
	movs r1, #82
	movs r2, #4
	movs r3, #12
	bl Func_02004e04
	cmp r6, #0
	bne .L_0200af22
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #75
	movs r1, #91
.L_0200aeec:
	movs r2, #1
	movs r3, #3
	bl Func_02004e04
	b .L_0200af22
.L_0200aef6:
	movs r3, #31
	movs r2, #76
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #82
	movs r1, #82
	movs r2, #6
	movs r3, #12
	bl Func_02004e04
	cmp r6, #0
	bne .L_0200af22
	movs r3, #30
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #81
	movs r1, #91
	movs r2, #1
	movs r3, #3
	bl Func_02004e04
.L_0200af22:
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.global Func_02002f30
	.thumb_func
Func_02002f30:
	.4byte 0x465fb5e0
	.4byte 0x464d4656
	.4byte 0x4647b4e0
	.4byte 0x23c0b480
	.4byte 0x6edb049b
	.4byte 0x004020d6
	.4byte 0x181b2281
	.4byte 0x4fab0092
	.4byte 0x21f0601a
	.4byte 0x187b0049
	.4byte 0x5e1a2000
	.4byte 0xb0874ba8
	.4byte 0xd001429a
	.endif
	.section .text.x0200af64,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02002f64
	.thumb_func
Func_02002f64:
	.2byte 0xf000, 0xff17 @ bl 0x0200bb96
	.4byte 0x187d3102
	.4byte 0x5eab2200
	.4byte 0xdd072b08
	.4byte 0xf0012008
	.4byte 0x2100ff6f
	.4byte 0xff3cf001
	.4byte 0xfed6f7ff
	.4byte 0x5e2b2000
	.4byte 0xd0002b14
	.4byte 0xf001e0a7
	.4byte 0x2000ff57
	.4byte 0xf80cf002
	.4byte 0x21012001
	.4byte 0x42402201
	.4byte 0x42524249
	.4byte 0xf0012300
	.4byte 0x2185ffc3
	.4byte 0x187b0089
	.4byte 0x21006818
	.4byte 0xf0012200
	.4byte 0x2009ff73
	.4byte 0x22002100
	.4byte 0xff6ef001
	.4byte 0x22002100
	.4byte 0xf001200a
	.4byte 0x2009ff69
	.4byte 0xff52f001
	.4byte 0xf001200a
	.4byte 0xf001ff4f
	.4byte 0xf001ffd9
	.4byte 0x203cffdf
	.4byte 0xff28f001
	.4byte 0x00802082
	.4byte 0x251430ff
	.4byte 0xfeaef001
	.4byte 0xd00a2800
	.4byte 0x00802082
	.4byte 0xf00130ff
	.4byte 0x2090feaf
	.4byte 0x30bc0100
	.4byte 0xfea6f001
	.4byte 0x20c22515
	.4byte 0xf0010080
	.4byte 0x2800fe9d
	.4byte 0x20c2d009
	.4byte 0xf0010080
	.4byte 0x2090fe9f
	.4byte 0x30bd0100
	.4byte 0xfe96f001
	.4byte 0x20c02516
	.4byte 0x30090080
	.4byte 0xfe8cf001
	.4byte 0xd00a2800
	.4byte 0x008020c0
	.4byte 0xf0013009
	.4byte 0x2090fe8d
	.4byte 0x30be0100
	.4byte 0xfe84f001
	.4byte 0x20c02517
	.4byte 0x300a0080
	.4byte 0xfe7af001
	.4byte 0xd00a2800
	.4byte 0x008020c0
	.4byte 0xf001300a
	.4byte 0x208cfe7b
	.4byte 0x30ff0100
	.4byte 0xfe72f001
	.4byte 0x208d2518
	.4byte 0xf0010080
	.4byte 0xf7ffffb9
	.4byte 0x200ffe57
	.4byte 0xfed8f001
	.4byte 0x20082103
	.4byte 0xff10f001
	.4byte 0x00802080
	.4byte 0xf0013012
	.4byte 0x2080ffab
	.4byte 0x22802180
	.4byte 0x02890280
	.4byte 0xf0010252
	.4byte 0x22e6feaf
	.4byte 0x21012001
	.4byte 0x32660212
	.4byte 0x42404249
	.4byte 0xfea6f001
	.4byte 0xf0012078
	.4byte 0x2101febb
	.4byte 0xf0012008
	.4byte 0x203cfef3
	.4byte 0xfeb4f001
	.4byte 0x30ff200a
	.4byte 0xfe40f001
	.4byte 0xf0011c28
	.4byte 0x4b46ff39
	.4byte 0x005222f1
	.4byte 0x881b189b
	.4byte 0x3b092080
	.4byte 0x0240041b
	.4byte 0xd8374283
	.4byte 0x004020a2
	.4byte 0xfe2ef001
	.4byte 0x01002090
	.4byte 0xf0013027
	.4byte 0x2800fe25
	.4byte 0x2008d017
	.4byte 0xf0012104
	.4byte 0x231efecf
	.4byte 0x93002213
	.4byte 0x20559201
	.4byte 0x22052148
	.4byte 0xf0012302
	.4byte 0x2009fe61
	.4byte 0x22002100
	.4byte 0xfeb8f001
	.4byte 0x2100200a
	.4byte 0xf0012200
	.4byte 0x2090feb3
	.4byte 0x30ee0100
	.4byte 0xfe06f001
	.4byte 0xd00d2800
	.4byte 0x01002090
	.4byte 0xf00130ba
	.4byte 0x2800fdff
	.4byte 0x21e8d106
	.4byte 0x200c22a4
	.4byte 0x04520449
	.4byte 0xfe9ef001
	ldr r7, .L_ja_0200aff8
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r7, r1
	.2byte 0x2200
	ldrsh r3, [r3, r2]
	.4byte 0xd0002b09
	.4byte 0x2090e2be
	.4byte 0x30270100
	.4byte 0xfde8f001
	.4byte 0xd0012800
	.4byte 0xf857f001
	.4byte 0xf0012035
	.4byte 0x20c4ff31
	.4byte 0xf00130ff
	.4byte 0x2501fe4d
	.4byte 0x42a8426d
	.4byte 0xe28ad100
	.4byte 0x01002090
	.4byte 0xf00130ac
	.4byte 0x4682fdd3
	.4byte 0xd0272800
	.4byte 0x21022008
	.4byte 0xfe7cf001
	.4byte 0x21002009
	.4byte 0xf0012200
	.4byte 0x21d8fe6f
	.4byte 0x045222a0
	.4byte 0x0449200a
	.4byte 0xfe68f001
	.4byte 0x2103200a
	.4byte 0xfe98f001
	.4byte 0x2213231b
	.4byte 0x92019300
	.4byte 0x2113201e
	.4byte 0x23012201
	.4byte 0xfdfaf001
	.4byte 0x2105200a
	.4byte 0xfe5ef001
	.4byte 0xf81ff001
.L_ja_0200aff8:
	.4byte gPartyState
	.4byte 0x000000b1
	.4byte 0xf001200a
	.4byte 0x4683fe29
	.4byte 0xf0012008
	.4byte 0x4680fe25
	.4byte 0x004020c3
	.4byte 0xfeecf001
	.4byte 0x01002090
	.4byte 0xf00130ac
	.4byte 0x2090fd9b
	.4byte 0x30de0100
	.4byte 0xfd96f001
	.4byte 0xfe08f001
	.4byte 0xf0012000
	.4byte 0x2200febd
	.4byte 0x21002009
	.4byte 0xfe30f001
	.4byte 0x21012009
	.4byte 0xfe10f001
	.4byte 0x22b421e4
	.4byte 0x200a0452
	.4byte 0xf0010449
	.4byte 0x200afe25
	.4byte 0xf0012101
	.4byte 0x21e0fe05
	.4byte 0x200a2200
	.4byte 0xf0010209
	.4byte 0x2008fe4b
	.4byte 0xf0012102
	.4byte 0xf001fe1f
	.4byte 0xf001fe8d
	.4byte 0x20f4fe93
	.4byte 0x230122c0
	.4byte 0x04401c29
	.4byte 0xf0010452
	.4byte 0xf001fe53
	.4byte 0x2184fe55
	.4byte 0x00492200
	.4byte 0xf001200a
	.4byte 0x2028fe3b
	.4byte 0xfdccf001
	.4byte 0xf00148b2
	.4byte 0x200afe25
	.4byte 0xf0012100
	.4byte 0x200afe25
	.4byte 0xf0012103
	.4byte 0x200afe01
	.4byte 0xf0012100
	.4byte 0x200afe1d
	.4byte 0xf0012108
	.4byte 0x20c0fdf5
	.4byte 0x300a01c0
	.4byte 0xf0012100
	.4byte 0x23c0fe13
	.4byte 0x4658025b
	.4byte 0x22a421f4
	.4byte 0x00526303
	.4byte 0x200a0049
	.4byte 0xfdccf001
	.4byte 0xf0012005
	.4byte 0x2106fda5
	.4byte 0xf001200a
	.4byte 0x200afddd
	.4byte 0xfd9ef001
	.4byte 0x465a21c0
	.4byte 0x629102c9
	.4byte 0xf001200a
	.4byte 0x235bfd97
	.4byte 0x4699445b
	.4byte 0x23014647
	.4byte 0x4648375b
	.4byte 0x21007003
	.4byte 0x4658703b
	.4byte 0xfd32f001
	.4byte 0x21004640
	.4byte 0xfd2ef001
	.4byte 0x21064658
	.4byte 0xfd72f001
	.4byte 0x21064640
	.4byte 0xfd6ef001
	.4byte 0x2205251e
	.4byte 0x26132302
	.4byte 0x2055214c
	.4byte 0x96019500
	.4byte 0xfd4cf001
	.4byte 0x0080208f
	.4byte 0xfe4cf001
	.4byte 0x01c020fe
	.4byte 0x30ff2100
	.4byte 0xfe0af001
	.4byte 0xf001200a
	.4byte 0x200afe0f
	.4byte 0xfd64f001
	.4byte 0x21002080
	.4byte 0xf0010240
	.4byte 0x200afdff
	.4byte 0xfe04f001
	.4byte 0xf001200a
	.4byte 0x203cfd59
	.4byte 0xfd56f001
	.4byte 0x21004658
	.4byte 0xfd42f001
	.4byte 0x21004640
	.4byte 0xfd3ef001
	.4byte 0x20552302
	.4byte 0x2205214a
	.4byte 0x96019500
	.4byte 0xfd1ef001
	.4byte 0x4651464a
	.4byte 0x46587011
	.4byte 0x21107039
	.4byte 0xfce6f001
	.4byte 0x46402110
	.4byte 0xfce2f001
	.4byte 0xf001200a
	.4byte 0x305afd47
	.4byte 0x25fe7802
	.4byte 0x40131c2b
	.4byte 0x22b421e4
	.4byte 0x70030052
	.4byte 0x200a0049
	.4byte 0xf0012680
	.4byte 0x02f6fd51
	.4byte 0x2181465b
	.4byte 0x0049629e
	.4byte 0xf001200a
	.4byte 0x200afd91
	.4byte 0xfd4ef001
	.4byte 0x22142102
	.4byte 0x200a31ff
	.4byte 0xfd84f001
	.4byte 0xf001201e
	.4byte 0x2108fd15
	.4byte 0xf001200a
	.4byte 0x200afd4d
	.4byte 0xfd0ef001
	.4byte 0x024020c0
	.4byte 0x63084659
	.4byte 0x21f422a4
	.4byte 0x00520049
	.4byte 0xf001200a
	.4byte 0x200efd27
	.4byte 0xfd00f001
	.4byte 0x02d222c0
	.4byte 0x629a465b
	.4byte 0x200a2106
	.4byte 0xfd34f001
	.4byte 0xf001200a
	.4byte 0x208ffcf5
	.4byte 0xf0010080
	.4byte 0x200afdcd
	.4byte 0xfcfef001
	.4byte 0x7803305a
	.4byte 0x401d21e4
	.4byte 0x005222b4
	.4byte 0x00497005
	.4byte 0xf001200a
	.4byte 0x4658fd0b
	.4byte 0x00492181
	.4byte 0x200a6286
	.4byte 0xfd4cf001
	.4byte 0xf001200a
	.4byte 0x2100fd09
	.4byte 0xf001200a
	.4byte 0x200ffd35
	.4byte 0xfcd2f001
	.4byte 0x46402100
	.4byte 0xfc76f001
	.4byte 0xf001200f
	.4byte 0x20d7fccb
	.4byte 0xfda4f001
	.4byte 0x21f4201e
	.4byte 0x4a314b30
	.4byte 0x30ff0449
	.4byte 0xfc70f001
	.4byte 0x1c052100
	.4byte 0xfc9cf001
	.4byte 0x492d1c28
	.4byte 0xfc64f001
	.4byte 0x21011c28
	.4byte 0xfc58f001
	.4byte 0x21061c28
	.4byte 0xfca0f001
	.4byte 0x22002102
	.4byte 0xf001200a
	.4byte 0x2005fcef
	.4byte 0xfca8f001
	.4byte 0x00492181
	.4byte 0x200a2200
	.4byte 0xfd0ef001
	.4byte 0xf0012037
	.4byte 0x2005fc9f
	.4byte 0xfc9cf001
	.4byte 0xf0012088
	.4byte 0x20cefd75
	.4byte 0x4a1a21f4
	.4byte 0x04494b18
	.4byte 0xf0010040
	.4byte 0x2100fc41
	.4byte 0xf0011c05
	.4byte 0x4917fc6d
	.4byte 0xf0011c28
	.4byte 0x6d2afc35
	.4byte 0x490f2380
	.4byte 0x8253019b
	.4byte 0x1c2b22c0
	.4byte 0x33550252
	.4byte 0x7019632a
	.4byte 0x045b23e4
	.4byte 0x4b0f63ab
	.4byte 0x63eb200a
	.4byte 0x045b23b3
	.4byte 0x23c0642b
	.4byte 0x61ab01db
	.4byte 0xf00161eb
	.4byte 0x2110fc6f
	.4byte 0xf0014640
	.4byte 0x200afc13
	.4byte 0x0000e00e
	.4byte 0x00000000
	.4byte 0x00002430
	.4byte 0x01590000
	.4byte 0xfff00000
	.4byte Data_0200533c + 0x60
	.4byte Data_0200533c + 0x24
	.4byte 0xffe00000
	.4byte 0xfc58f001
	.4byte 0xf0012085
	.4byte 0x4653fd31
	.4byte 0x60ab2280
	.4byte 0x1c30612b
	.4byte 0x02521c31
	.4byte 0xfc34f001
	.4byte 0x21082700
	.4byte 0x2f014249
	.4byte 0x2108dd00
	.4byte 0x22082301
	.4byte 0x4252403b
	.4byte 0xd1002b00
	.4byte 0x20e42208
	.4byte 0x04130440
	.4byte 0x22b30409
	.4byte 0x04521809
	.4byte 0x189b2094
	.4byte 0x4a4430ff
	.4byte 0xfbe2f001
	.4byte 0xae0300bd
	.4byte 0x21025170
	.4byte 0xfbd0f001
	.4byte 0x21005970
	.4byte 0xfc08f001
	.4byte 0x59703701
	.4byte 0xf001493d
	.4byte 0x2f03fbcf
	.4byte 0x23a0ddd5
	.4byte 0x4658031b
	.4byte 0x200a6283
	.4byte 0xfc2af001
	.4byte 0x7802305a
	.4byte 0x401323fe
	.4byte 0x22a021d8
	.4byte 0x00497003
	.4byte 0x200a0052
	.4byte 0xfc36f001
	.4byte 0xf001200a
	.4byte 0x22e6fc0b
	.4byte 0x21012001
	.4byte 0x32660212
	.4byte 0x42494240
	.4byte 0xfbeaf001
	.4byte 0x200a2105
	.4byte 0xfc3af001
	.4byte 0xf001201e
	.4byte 0x200afbfb
	.4byte 0xf0012101
	.4byte 0xf001fc7b
	.4byte 0x4d26fc75
	.4byte 0x1c29200a
	.4byte 0xfc08f001
	.4byte 0xf001200a
	.4byte 0x2014fc09
	.4byte 0xfbeaf001
	.4byte 0x200a1c29
	.4byte 0xfbfef001
	.4byte 0xf0012014
	.4byte 0x20c0fbe3
	.4byte 0x300a01c0
	.4byte 0xf0012100
	.4byte 0x4b1bfc3d
	.4byte 0x00892185
	.4byte 0x6818185b
	.4byte 0xf0012101
	.4byte 0xf001fc49
	.4byte 0x200afc53
	.4byte 0xf0012103
	.4byte 0x231bfc37
	.4byte 0x93002213
	.4byte 0x201e9201
	.4byte 0x22012113
	.4byte 0xf0012301
	.4byte 0xf001fb99
	.4byte 0xf000fbcb
	.4byte 0x200afdc0
	.4byte 0xf00130ff
	.4byte 0x2800fb49
	.4byte 0xf000d001
	.4byte 0x21ecfdb8
	.4byte 0x200a22e4
	.4byte 0x04520449
	.4byte 0xfbe6f001
	.4byte 0x200a4906
	.4byte 0xfbc6f001
	.4byte 0xfdabf000
	.4byte 0xffe00000
	.4byte Data_0200533c + 0x60
	.4byte Data_02005e28
	.4byte gPartyState
	.4byte Data_02005728 + 0x64
	.4byte 0xd0002b0a
	.4byte 0x2007e2d0
	.4byte 0xfc78f001
	.4byte 0x01002090
	.4byte 0xf0013027
	.2byte 0xfb23
	mov r8, r0
	.4byte 0xd0012800
	.4byte 0xfd91f000
	.4byte 0xfb94f001
	.4byte 0xf0012000
	.4byte 0x2090fc49
	.4byte 0x30270100
	.4byte 0xfb18f001
	.4byte 0xf00148e3
	.4byte 0x2200fbe1
	.4byte 0x21002009
	.4byte 0xfbb4f001
	.4byte 0x20082103
	.4byte 0xfbb8f001
	.4byte 0xf001200b
	.4byte 0x2280fb89
	.4byte 0x63020252
	.4byte 0x4692200a
	.4byte 0xfb82f001
	.4byte 0x1c052680
	.4byte 0x21dc0236
	.2byte 0x229c
	.2byte 0x200b
	.4byte 0x04520449
	.2byte 0x632e
	.2byte 0xf001
	.2byte 0xfb9b
	movs r3, #133
	lsls r3, r3, #2
	adds r7, r7, r3
	.4byte 0x22a421e4
	.4byte 0x04496838
	.4byte 0xf0010452
	.4byte 0x21f4fb91
	.4byte 0x200a22d4
	.4byte 0x04520449
	.4byte 0xfb8af001
	.4byte 0x21006838
	.4byte 0xf0012200
	.4byte 0x200bfbb5
	.4byte 0x22002100
	.4byte 0xfbb0f001
	.4byte 0x200a21c0
	.4byte 0x22000209
	.4byte 0xfbaaf001
	.4byte 0xfbf0f001
	.4byte 0xfbf6f001
	.4byte 0x221e2181
	.4byte 0x200b0049
	.4byte 0xfba8f001
	.4byte 0x00802080
	.4byte 0xf0013012
	.4byte 0x2100fc13
	.4byte 0xf001200b
	.4byte 0x2080fb93
	.4byte 0x30120080
	.4byte 0xfc0af001
	.4byte 0x2100200a
	.4byte 0xfb8af001
	.4byte 0x00492180
	.4byte 0x68382200
	.4byte 0xfb90f001
	.4byte 0x00492180
	.4byte 0x200b2200
	.4byte 0xfb8af001
	.4byte 0xf001200f
	.4byte 0x2180fb1b
	.4byte 0x01c96838
	.4byte 0xf0012200
	.4byte 0x2180fb79
	.4byte 0x01c92200
	.4byte 0xf001200b
	.4byte 0x201efb73
	.4byte 0xfb0cf001
	.4byte 0x2101200a
	.4byte 0xfb8cf001
	.4byte 0xfb86f001
	.4byte 0x2102200a
	.4byte 0xfb4af001
	.4byte 0x2101200a
	.4byte 0xfb72f001
	.4byte 0x22bc21f4
	.4byte 0x00490052
	.4byte 0xf001200a
	.4byte 0x200afb1f
	.4byte 0xfb24f001
	.4byte 0x200a2102
	.4byte 0xfb38f001
	.4byte 0xf0012014
	.4byte 0x21ecfaed
	.4byte 0x004922b4
	.4byte 0x200a0052
	.4byte 0xfb0ef001
	.4byte 0xf001200a
	.4byte 0x6838fb13
	.4byte 0x2200210a
	.4byte 0xfb2ef001
	.4byte 0x210a200b
	.4byte 0xf0012200
	.4byte 0x200afb29
	.4byte 0x22002108
	.4byte 0xfb24f001
	.4byte 0x21012001
	.4byte 0x42522201
	.4byte 0x42402300
	.4byte 0xf0014249
	.4byte 0x200afb47
	.4byte 0xf0012108
	.4byte 0x200afb03
	.4byte 0xf0012100
	.4byte 0x23c0fb23
	.4byte 0x21f4025b
	.4byte 0x005222a4
	.4byte 0x0049632b
	.4byte 0xf001200a
	.4byte 0x2005fadd
	.4byte 0xfab6f001
	.4byte 0x200a2106
	.4byte 0xfaeef001
	.4byte 0xf001200a
	.4byte 0x23c0faaf
	.4byte 0x62ab02db
	.4byte 0xf001200a
	.4byte 0x208ffaa9
	.4byte 0xf0010080
	.4byte 0x1c2afb81
	.4byte 0x325b20fe
	.4byte 0x01c02301
	.4byte 0x21007013
	.4byte 0xf00130ff
	.4byte 0x200afb3b
	.4byte 0xfb40f001
	.4byte 0xf001200a
	.4byte 0x2080fa95
	.4byte 0x30120080
	.4byte 0xfb6cf001
	.4byte 0x46502100
	.4byte 0xfb2cf001
	.4byte 0xf001200a
	.4byte 0x200afb31
	.4byte 0xfa86f001
	.4byte 0x01c020fe
	.4byte 0x30ff2100
	.4byte 0xfb20f001
	.4byte 0xf001200a
	.4byte 0x200afb25
	.4byte 0xfa7af001
	.4byte 0x46502100
	.4byte 0xfb16f001
	.4byte 0xf001200a
	.4byte 0x200afb1b
	.4byte 0xfa70f001
	.4byte 0x01c020fe
	.4byte 0x30ff2100
	.4byte 0xfb0af001
	.4byte 0xf001200a
	.4byte 0x203cfb0f
	.4byte 0xfa64f001
	.4byte 0x21042008
	.4byte 0xfa9cf001
	.4byte 0x2213231e
	.4byte 0x92019300
	.4byte 0x21482055
	.4byte 0x23022205
	.4byte 0xfa2ef001
	.2byte 0x4b4b
	mov r0, r8
	.2byte 0x21e4
	.2byte 0x22b4
	str r3, [r5, #12]
	.2byte 0x0452
	str r0, [r5, #40]
	.2byte 0x0449
	movs r0, #10
	.2byte 0x632e
	.4byte 0xfa7ef001
	.4byte 0x2101200a
	.4byte 0xfa82f001
	.4byte 0x46502100
	.4byte 0xfae2f001
	.4byte 0xf001200a
	.4byte 0x200afae7
	.4byte 0xfa3cf001
	.4byte 0x68392200
	.4byte 0xf001200a
	.4byte 0x200afa87
	.4byte 0xf0012100
	.4byte 0x200afa93
	.4byte 0xf0012103
	.4byte 0x200afa6f
	.4byte 0xf0012100
	.4byte 0x2180fa8b
	.4byte 0x200a2200
	.4byte 0xf00101c9
	.4byte 0x200afa89
	.4byte 0xf0012102
	.4byte 0x21f4fa5d
	.4byte 0x005222c4
	.4byte 0x0049200a
	.4byte 0xfa42f001
	.4byte 0x200b2101
	.4byte 0xfa5ef001
	.4byte 0xf0012005
	.4byte 0x2100fa13
	.4byte 0xf001200b
	.4byte 0x200afa6f
	.4byte 0xfa3cf001
	.4byte 0x2100200a
	.4byte 0xfa44f001
	.4byte 0x210b2200
	.4byte 0xf001200a
	.4byte 0x200ffa53
	.4byte 0xfa00f001
	.4byte 0x2100200a
	.4byte 0xfa5cf001
	.4byte 0x2103200a
	.4byte 0xfa38f001
	.4byte 0x22002180
	.4byte 0x01c9200a
	.4byte 0xfa56f001
	.4byte 0x2102200a
	.4byte 0xfa2af001
	.4byte 0x228621f4
	.4byte 0x200a0092
	.4byte 0xf0010049
	.4byte 0x6838fa0f
	.4byte 0xf0012101
	.4byte 0x6838fa57
	.4byte 0x2200210b
	.4byte 0xfa2ef001
	.4byte 0x68392200
	.4byte 0xf001200b
	.4byte 0x201efa29
	.4byte 0xf9d6f001
	.4byte 0x2103200b
	.4byte 0xfa12f001
	.4byte 0x2102200b
	.4byte 0xfa0af001
	.4byte 0xf0016838
	.4byte 0x2800f9db
	.4byte 0x220ad00c
	.4byte 0x23125e81
	.4byte 0x200b5ec2
	.4byte 0xf9e6f001
	.4byte 0x0000e004
	.4byte 0x00002439
	.4byte 0xffe00000
	.4byte 0xf001200b
	.4byte 0x2100f9e9
	.4byte 0x200b2200
	.4byte 0xf9e8f001
	.4byte 0xf001200b
	.4byte 0x200af9e1
	.4byte 0x22002100
	.4byte 0xf9e0f001
	.4byte 0x22bc21f4
	.4byte 0x00520049
	.4byte 0xf0016838
	.4byte 0x6838f9cd
	.4byte 0xf9d2f001
	.4byte 0x21a020a0
	.4byte 0x465202c9
	.4byte 0xf00102c0
	.4byte 0x206bf983
	.4byte 0xfa74f001
	.4byte 0xf001201e
	.4byte 0x2180f995
	.4byte 0x22000049
	.4byte 0xf0016838
	.4byte 0x200ff9fb
	.4byte 0xf98cf001
	.4byte 0x020921c0
	.4byte 0x68382200
	.4byte 0xf9eaf001
	.4byte 0xf001200f
	.4byte 0x200cf983
	.4byte 0xf990f001
	.4byte 0x04db2380
	.4byte 0x881b3352
	.4byte 0x350a466d
	.4byte 0x21a04680
	.4byte 0x802b20a0
	.4byte 0x46520309
	.4byte 0xf0010300
	.4byte 0x206bf959
	.4byte 0xfa4af001
	.4byte 0xf930f001
	.4byte 0x802b782b
	.4byte 0xd0152b00
	.4byte 0x4a09882b
	.4byte 0x04c92180
	.4byte 0x31524313
	.4byte 0x20ff800b
	.4byte 0x0200882b
	.4byte 0x181b30ff
	.4byte 0x2001802b
	.4byte 0xf8c0f001
	.4byte 0x2b00882b
	.4byte 0xe001d1ec
	.4byte 0x00001000
	.4byte 0x04c92180
	.4byte 0x23fd880a
	.4byte 0x33ff021b
	.4byte 0x800b4013
	.4byte 0x26e8201e
	.4byte 0xf00125ae
	.4byte 0x046df945
	.4byte 0x23440476
	.4byte 0x492e1c2a
	.4byte 0x1c3033ff
	.4byte 0xfb60f7fc
	.4byte 0x1c071c2a
	.4byte 0x200c1c31
	.4byte 0xf96af001
	.4byte 0x1c282500
	.4byte 0xf0012105
	.4byte 0x2800f895
	.4byte 0xf7fed101
	.4byte 0x4641f85f
	.4byte 0x4a24690b
	.4byte 0x189b2002
	.4byte 0x3501610b
	.4byte 0xf924f001
	.4byte 0xddec2d13
	.4byte 0x01002090
	.4byte 0xf00130ee
	.4byte 0x22e6f8ad
	.4byte 0x21012001
	.4byte 0x42490212
	.4byte 0x42403266
	.4byte 0xf8fcf001
	.4byte 0x60bb2300
	.4byte 0x2001613b
	.4byte 0xf90ef001
	.4byte 0xf0011c38
	.4byte 0x20c3f8bf
	.4byte 0xf0010040
	.4byte 0x201ef9e3
	.4byte 0xf904f001
	.4byte 0x04c92180
	.4byte 0x4a0b880b
	.4byte 0x4313466e
	.4byte 0x360a800b
	.4byte 0x4a098833
	.4byte 0x43133152
	.4byte 0x2280800b
	.4byte 0x04d24b07
	.4byte 0x80133250
	.4byte 0x2b088833
	.4byte 0x8833d01b
	.4byte 0x21804a02
	.4byte 0xe00904c9
	.4byte 0x00000f00
	.4byte 0x00001000
	.4byte 0x00003f42
	.4byte 0xffe00000
	.4byte 0xffff0000
	.4byte 0x31524313
	.4byte 0x1c35800b
	.4byte 0x20018833
	.4byte 0x80333301
	.4byte 0xf83ef001
	.4byte 0x2b08882b
	.4byte 0xf001d1e3
	.4byte 0xf001f891
	.4byte 0xe2ccf8d7
	.4byte 0xd1342b0b
	.4byte 0xf8cef001
	.4byte 0xf0012000
	.2byte 0xf983
	.2byte 0x2009
	.4byte 0x22002100
	.4byte 0xf8f6f001
	.4byte 0x22ac21e0
	.4byte 0x0449200a
	.2byte 0x0452
	.2byte 0xf001
	.2byte 0xf8ef
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r7, r3
	.4byte 0x68292200
	.4byte 0xf001200a
	.4byte 0x2113f903
	.4byte 0xf0016828
	.4byte 0xf001f8eb
	.4byte 0xf001f959
	.4byte 0x48e9f95f
	bl Func_02004f0c
	movs r1, #0
	movs r0, #10
	bl Func_02004f14
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #242
	lsls r0, r0, #1
	movs r2, #243
	lsls r2, r2, #1
	adds r3, r7, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	adds r3, r7, r2
	.4byte 0x2b0ce03e
	.4byte 0xe292d000
	.4byte 0xf896f001
	.4byte 0xf0012000
	.4byte 0x2009f94b
	.4byte 0x22002100
	.4byte 0xf8bef001
	.4byte 0x22a021d7
	.4byte 0x200a0452
	.4byte 0xf0010449
	.4byte 0x200af8b7
	.4byte 0xf0012105
	.2byte 0xf8bb
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r7, r0
	.4byte 0x21136818
	.4byte 0xf8b4f001
	.4byte 0xf922f001
	.4byte 0xf928f001
	.4byte 0xf001201e
	.4byte 0x2102f871
	.4byte 0xf001200a
	.4byte 0x201ef8b5
	.4byte 0xf86af001
	.4byte 0xf00148c9
	.2byte 0xf8c3
	.2byte 0x200a
	movs r1, #0
	.2byte 0xf001
	.4byte 0xf001f8c3
	.4byte 0xf001f911
	.2byte 0xf913
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r7, r1
	adds r1, #2
	movs r2, #0
	ldrsh r0, [r3, r2]
	adds r3, r7, r1
	.4byte 0x5e992200
	.4byte 0xf8dcf001
	.4byte 0xf85af001
	.2byte 0xe24f
	ldr r3, .L_ja_0200be90
	cmp r2, r3
	bne .L_ja_0200bc80
	movs r0, #8
	bl Object_GetById
	adds r2, r0, #0
	adds r2, #35
	ldrb r3, [r2]
	movs r5, #32
	orrs r3, r5
	strb r3, [r2]
	movs r1, #3
	bl Func_02004e3c
	movs r0, #9
	bl Object_GetById
	adds r2, r0, #0
	adds r2, #35
	ldrb r3, [r2]
	movs r1, #3
	orrs r5, r3
	strb r5, [r2]
	bl Func_02004e3c
	movs r0, #8
	movs r1, #12
	bl Func_02002ac8
	movs r0, #9
	movs r1, #12
	bl Func_02002ac8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #98
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bc06
	movs r1, #220
	movs r2, #172
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02004ebc
	movs r1, #138
	movs r2, #172
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02004ebc
	b .L_ja_0200bc2c
.L_ja_0200bc06:
	movs r3, #89
	movs r5, #40
	str r3, [sp, #0]
	movs r0, #88
	movs r1, #40
	movs r2, #1
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02004df4
	movs r3, #100
	str r3, [sp, #0]
	movs r0, #99
	movs r1, #40
	movs r2, #1
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02004df4
.L_ja_0200bc2c:
	movs r0, #132
	lsls r0, r0, #2
	bl GameFlag_GetByte
	adds r5, r0, #0
	movs r0, #132
	movs r3, #15
	lsls r0, r0, #2
	ands r5, r3
	bl GameFlag_GetByte
	asrs r0, r0, #4
	lsls r0, r0, #24
	lsrs r6, r0, #24
	cmp r5, #0
	beq .L_ja_0200bc60
	ldr r2, .L_ja_0200be94
	lsls r3, r5, #2
	ldrsh r1, [r2, r3]
	adds r3, #2
	ldrsh r2, [r2, r3]
	lsls r1, r1, #19
	lsls r2, r2, #19
	movs r0, #10
	bl Func_02004ebc
.L_ja_0200bc60:
	lsls r3, r6, #24
	asrs r0, r3, #24
	cmp r0, #0
	bne .L_ja_0200bc6a
	b .L_ja_0200c036
.L_ja_0200bc6a:
	ldr r2, .L_ja_0200be94
	lsls r3, r0, #2
	ldrsh r1, [r2, r3]
	adds r3, #2
	ldrsh r2, [r2, r3]
	lsls r1, r1, #19
	lsls r2, r2, #19
	movs r0, #11
	bl Func_02004ebc
	b .L_ja_0200c036
.L_ja_0200bc80:
	ldr r3, .L_ja_0200be98
	cmp r2, r3
	bne .L_ja_0200bc9e
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #99
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200bc96
	b .L_ja_0200c036
.L_ja_0200bc96:
	movs r0, #1
	bl Func_02002cb4
	b .L_ja_0200c036
.L_ja_0200bc9e:
	ldr r3, .L_ja_0200be9c
	cmp r2, r3
	beq .L_ja_0200bca6
	b .L_ja_0200befa
.L_ja_0200bca6:
	movs r0, #10
	bl Object_GetById
	movs r1, #3
	adds r6, r0, #0
	bl Func_02004e3c
	movs r0, #8
	movs r1, #10
	bl Func_02002ac8
	movs r0, #9
	movs r1, #11
	bl Func_02002ac8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #188
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200bce6
	movs r3, #26
	movs r2, #77
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #77
	movs r2, #1
	movs r3, #15
	bl Func_02004df4
.L_ja_0200bce6:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #189
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bd3a
	movs r1, #240
	movs r2, #176
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004ebc
	movs r3, #85
	str r3, [sp, #4]
	movs r5, #28
	movs r0, #2
	movs r1, #93
	movs r2, #1
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02004df4
	movs r6, #22
	movs r0, #27
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004dfc
	movs r3, #29
	str r3, [sp, #0]
	movs r0, #32
	movs r1, #16
	movs r2, #2
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02004dfc
.L_ja_0200bd3a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #190
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bd8e
	movs r1, #152
	movs r2, #176
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02004ebc
	movs r3, #85
	str r3, [sp, #4]
	movs r5, #36
	movs r0, #2
	movs r1, #93
	movs r2, #1
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02004df4
	movs r6, #22
	movs r0, #35
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004dfc
	movs r3, #37
	str r3, [sp, #0]
	movs r0, #32
	movs r1, #16
	movs r2, #2
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02004dfc
.L_ja_0200bd8e:
	movs r0, #140
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bdc4
	movs r3, #37
	movs r2, #26
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #61
	movs r1, #0
	movs r2, #3
	movs r3, #3
	bl Func_02004e04
	movs r3, #38
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02004dfc
.L_ja_0200bdc4:
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #6
	beq .L_ja_0200bdd4
	b .L_ja_0200bede
.L_ja_0200bdd4:
	movs r3, #60
	mov r8, r3
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r7, r0
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	bl Event_StartScreenTransition
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #168
	movs r2, #192
	lsls r2, r2, #17
	lsls r1, r1, #18
	movs r0, #11
	bl Func_02004ebc
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #11
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #162
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	lsls r0, r0, #1
	bl Engine_ObjectCreate
	adds r5, r0, #0
	cmp r5, #0
	beq .L_ja_0200be7c
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r1, .L_ja_0200bea0
	adds r0, r5, #0
	bl Func_02004dac
	adds r0, r5, #0
	movs r1, #1
	bl Func_02004d9c
	movs r1, #14
	adds r0, r5, #0
	bl Animation_ApplyChildValues
	ldr r1, [r5, #80]
	movs r2, #12
	ldrb r3, [r1, #9]
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r3, .L_ja_0200bea4
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_ja_0200be7c:
	ldr r3, [r6, #12]
	movs r1, #128
	lsls r1, r1, #16
	adds r3, r3, r1
	str r3, [r6, #12]
	b .L_ja_0200beae
	.4byte 0x0000242f
	.4byte 0x00002438
.L_ja_0200be90:
	.4byte 0x000000b2
.L_ja_0200be94:
	.4byte Data_0200530c
.L_ja_0200be98:
	.4byte 0x000000b4
.L_ja_0200be9c:
	.4byte 0x000000b3
.L_ja_0200bea0:
	.4byte Data_020053d8
.L_ja_0200bea4:
	.4byte 0x00013333
.L_ja_0200bea8:
	movs r2, #1
	negs r2, r2
	add r8, r2
.L_ja_0200beae:
	mov r3, r8
	cmp r3, #0
	beq .L_ja_0200bec4
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #40]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	ble .L_ja_0200bea8
.L_ja_0200bec4:
	movs r0, #188
	bl Func_02005014
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #3
	bl Func_02004f6c
.L_ja_0200bede:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #196
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200beee
	b .L_ja_0200c036
.L_ja_0200beee:
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	b .L_ja_0200c036
.L_ja_0200befa:
	ldr r3, .L_ja_0200c04c
	cmp r2, r3
	beq .L_ja_0200bf02
	b .L_ja_0200c036
.L_ja_0200bf02:
	movs r0, #10
	movs r1, #1
	bl Func_02004fec
	movs r1, #9
	movs r0, #8
	bl Func_02002ac8
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	beq .L_ja_0200bf28
	cmp r3, #3
	beq .L_ja_0200bf28
	cmp r3, #5
	bne .L_ja_0200bf2c
.L_ja_0200bf28:
	bl Func_02004d00
.L_ja_0200bf2c:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bf4c
	movs r1, #192
	movs r2, #150
	movs r3, #170
	lsls r1, r1, #17
	lsls r2, r2, #15
	lsls r3, r3, #18
	movs r0, #11
	bl Func_02004a5c
.L_ja_0200bf4c:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bf68
	movs r2, #204
	movs r3, #170
	ldr r1, .L_ja_0200c050
	lsls r2, r2, #14
	lsls r3, r3, #18
	movs r0, #12
	bl Func_02004a5c
.L_ja_0200bf68:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bf86
	movs r2, #220
	movs r3, #170
	ldr r1, .L_ja_0200c054
	lsls r2, r2, #14
	lsls r3, r3, #18
	movs r0, #13
	bl Func_02004a5c
.L_ja_0200bf86:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #188
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200bfd8
	movs r1, #192
	movs r2, #184
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004ebc
	movs r3, #85
	str r3, [sp, #4]
	movs r5, #25
	movs r0, #1
	movs r1, #85
	movs r2, #1
	movs r3, #4
	str r5, [sp, #0]
	bl Func_02004df4
	movs r6, #23
	movs r0, #27
	movs r1, #15
	movs r2, #2
	movs r3, #1
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004dfc
	movs r0, #25
	movs r1, #25
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004dfc
.L_ja_0200bfd8:
	movs r0, #15
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #196
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200c036
	movs r3, #104
	str r3, [sp, #0]
	movs r5, #20
	movs r0, #117
	movs r1, #20
	movs r2, #7
	movs r3, #8
	str r5, [sp, #4]
	bl Func_02004e04
	movs r3, #111
	str r3, [sp, #0]
	movs r0, #111
	movs r1, #12
	movs r2, #1
	movs r3, #6
	str r5, [sp, #4]
	bl Func_02004e04
	movs r3, #40
	movs r2, #84
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #84
	movs r2, #8
	movs r3, #8
	bl Func_02004dfc
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
.L_ja_0200c036:
	movs r0, #0
	bl Func_02004fc4
	movs r0, #0
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_ja_0200c04c:
	.4byte 0x000000b5
.L_ja_0200c050:
	.4byte 0x01cf0000
.L_ja_0200c054:
	.4byte 0x02350000
	.else
	.global Func_02002f64
	.thumb_func
Func_02002f64:
	bl .L_0200bd8a
	.2byte 0x3102
	.2byte 0x187d
	.2byte 0x2200
	.2byte 0x5eab
	.2byte 0x2b08
	.2byte 0xdd07
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xff7d
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xff4a
	.2byte 0xf7ff
	.2byte 0xfed6
	.2byte 0x2000
	.2byte 0x5e2b
	.2byte 0x2b14
	.2byte 0xd000
	.2byte 0xe0a7
	.2byte 0xf001
	.2byte 0xff65
	.2byte 0x2000
	.2byte 0xf002
	.2byte 0xf81a
	.2byte 0x2001
	.2byte 0x2101
	.2byte 0x2201
	.2byte 0x4240
	.2byte 0x4249
	.2byte 0x4252
	.2byte 0x2300
	.2byte 0xf001
	.2byte 0xffd1
	.2byte 0x2185
	.2byte 0x0089
	.2byte 0x187b
	.2byte 0x6818
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xff81
	.2byte 0x2009
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xff7c
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xff77
	.2byte 0x2009
	.2byte 0xf001
	.2byte 0xff60
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xff5d
	.2byte 0xf001
	.2byte 0xffe7
	.2byte 0xf001
	.2byte 0xffed
	.2byte 0x203c
	.2byte 0xf001
	.2byte 0xff36
	.2byte 0x2082
	.2byte 0x0080
	.2byte 0x30ff
	.2byte 0x2514
	.2byte 0xf001
	.2byte 0xfebc
	.2byte 0x2800
	.2byte 0xd00a
	.2byte 0x2082
	.2byte 0x0080
	.2byte 0x30ff
	.2byte 0xf001
	.2byte 0xfebd
	.2byte 0x2090
	.2byte 0x0100
	.2byte 0x30bc
	.2byte 0xf001
	.2byte 0xfeb4
	.2byte 0x2515
	.2byte 0x20c2
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfeab
	.2byte 0x2800
	.2byte 0xd009
	.2byte 0x20c2
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfead
	.2byte 0x2090
	.2byte 0x0100
	.2byte 0x30bd
	.2byte 0xf001
	.2byte 0xfea4
	.2byte 0x2516
	.2byte 0x20c0
	.2byte 0x0080
	.2byte 0x3009
	.2byte 0xf001
	.2byte 0xfe9a
	.2byte 0x2800
	.2byte 0xd00a
	.2byte 0x20c0
	.2byte 0x0080
	.2byte 0x3009
	.2byte 0xf001
	.2byte 0xfe9b
	.2byte 0x2090
	.2byte 0x0100
	.2byte 0x30be
	.2byte 0xf001
	.2byte 0xfe92
	.2byte 0x2517
	.2byte 0x20c0
	.2byte 0x0080
	.2byte 0x300a
	.2byte 0xf001
	.2byte 0xfe88
	.2byte 0x2800
	.2byte 0xd00a
	.2byte 0x20c0
	.2byte 0x0080
	.2byte 0x300a
	.2byte 0xf001
	.2byte 0xfe89
	.2byte 0x208c
	.2byte 0x0100
	.2byte 0x30ff
	.2byte 0xf001
	.2byte 0xfe80
	.2byte 0x2518
	.2byte 0x208d
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xffcb
	.2byte 0xf7ff
	.2byte 0xfe57
	.2byte 0x200f
	.2byte 0xf001
	.2byte 0xfee6
	.2byte 0x2103
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xff1e
	.2byte 0x2080
	.2byte 0x0080
	.2byte 0x3012
	.2byte 0xf001
	.2byte 0xffbd
	.2byte 0x2080
	.2byte 0x2180
	.2byte 0x2280
	.2byte 0x0280
	.2byte 0x0289
	.2byte 0x0252
	.2byte 0xf001
	.2byte 0xfebd
	.2byte 0x22e6
	.2byte 0x2001
	.2byte 0x2101
	.2byte 0x0212
	.2byte 0x3266
	.2byte 0x4249
	.2byte 0x4240
	.2byte 0xf001
	.2byte 0xfeb4
	.2byte 0x2078
	.2byte 0xf001
	.2byte 0xfec9
	.2byte 0x2101
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xff01
	.2byte 0x203c
	.2byte 0xf001
	.2byte 0xfec2
	.2byte 0x200a
	.2byte 0x30ff
	.2byte 0xf001
	.2byte 0xfe4e
	.2byte 0x1c28
	.2byte 0xf001
	.2byte 0xff47
	.2byte 0x4b47
	.2byte 0x22f1
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x881b
	.2byte 0x2080
	.2byte 0x3b09
	.2byte 0x041b
	.2byte 0x0240
	.2byte 0x4283
	.2byte 0xd837
	.2byte 0x20a2
	.2byte 0x0040
	.2byte 0xf001
	.2byte 0xfe3c
	.2byte 0x2090
	.2byte 0x0100
	.2byte 0x3027
	.2byte 0xf001
	.2byte 0xfe33
	.2byte 0x2800
	.2byte 0xd017
	.2byte 0x2008
	.2byte 0x2104
	.2byte 0xf001
	.2byte 0xfedd
	.2byte 0x231e
	.2byte 0x2213
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2055
	.2byte 0x2148
	.2byte 0x2205
	.2byte 0x2302
	.2byte 0xf001
	.2byte 0xfe6f
	.2byte 0x2009
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfec6
	.2byte 0x200a
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfec1
	.2byte 0x2090
	.2byte 0x0100
	.2byte 0x30ee
	.2byte 0xf001
	.2byte 0xfe14
	.2byte 0x2800
	.2byte 0xd00d
	.2byte 0x2090
	.2byte 0x0100
	.2byte 0x30ba
	.2byte 0xf001
	.2byte 0xfe0d
	.2byte 0x2800
	.2byte 0xd106
	.2byte 0x21e8
	.2byte 0x22a4
	.2byte 0x200c
	.2byte 0x0449
	.2byte 0x0452
	.2byte 0xf001
	.2byte 0xfeac
	.2byte 0x4925
	.2byte 0x26f1
	.2byte 0x4688
	.2byte 0x0076
	.2byte 0x4446
	.2byte 0x2200
	.2byte 0x5eb3
	.2byte 0x2b09
	.2byte 0xd000
	.2byte 0xe2bf
	.2byte 0x2090
	.2byte 0x0100
	.2byte 0x3027
	.2byte 0xf001
	.2byte 0xfdf5
	.2byte 0x2800
	.2byte 0xd001
	.2byte 0xf001
	.2byte 0xf861
	.2byte 0x2035
	.2byte 0xf001
	.2byte 0xff42
	.2byte 0x20c4
	.2byte 0x30ff
	.2byte 0xf001
	.2byte 0xfe5a
	.2byte 0x2501
	.2byte 0x426d
	.2byte 0x42a8
	.2byte 0xd100
	.2byte 0xe28b
	.2byte 0x2090
	.2byte 0x0100
	.2byte 0x30ac
	.2byte 0xf001
	.2byte 0xfde0
	.2byte 0x4682
	.2byte 0x2800
	.2byte 0xd028
	.2byte 0x2008
	.2byte 0x2102
	.2byte 0xf001
	.2byte 0xfe89
	.2byte 0x2009
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfe7c
	.2byte 0x21d8
	.2byte 0x22a0
	.2byte 0x0452
	.2byte 0x200a
	.2byte 0x0449
	.2byte 0xf001
	.2byte 0xfe75
	.2byte 0x200a
	.2byte 0x2103
	.2byte 0xf001
	.2byte 0xfea5
	.2byte 0x231b
	.2byte 0x2213
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x201e
	.2byte 0x2113
	.2byte 0x2201
	.2byte 0x2301
	.2byte 0xf001
	.2byte 0xfe07
	.2byte 0x200a
	.2byte 0x2105
	.2byte 0xf001
	.2byte 0xfe6b
	.2byte 0xf001
	.2byte 0xf829
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x00b1
	.2byte 0x0000
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfe35
	.2byte 0x4683
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfe31
	.2byte 0x4680
	.2byte 0x20c3
	.2byte 0x0040
	.2byte 0xf001
	.2byte 0xfefc
	.2byte 0x2090
	.2byte 0x0100
	.2byte 0x30ac
	.2byte 0xf001
	.2byte 0xfda7
	.2byte 0x2090
	.2byte 0x0100
	.2byte 0x30de
	.2byte 0xf001
	.2byte 0xfda2
	.2byte 0xf001
	.2byte 0xfe14
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xfec9
	.2byte 0x2200
	.2byte 0x2009
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfe3c
	.2byte 0x2009
	.2byte 0x2101
	.2byte 0xf001
	.2byte 0xfe1c
	.2byte 0x21e4
	.2byte 0x22b4
	.2byte 0x0452
	.2byte 0x200a
	.2byte 0x0449
	.2byte 0xf001
	.2byte 0xfe31
	.2byte 0x200a
	.2byte 0x2101
	.2byte 0xf001
	.2byte 0xfe11
	.2byte 0x21e0
	.2byte 0x2200
	.2byte 0x200a
	.2byte 0x0209
	.2byte 0xf001
	.2byte 0xfe57
	.2byte 0x2008
	.2byte 0x2102
	.2byte 0xf001
	.2byte 0xfe2b
	.2byte 0xf001
	.2byte 0xfe99
	.2byte 0xf001
	.2byte 0xfe9f
	.2byte 0x20f4
	.2byte 0x22c0
	.2byte 0x2301
	.2byte 0x1c29
	.2byte 0x0440
	.2byte 0x0452
	.2byte 0xf001
	.2byte 0xfe5f
	.2byte 0xf001
	.2byte 0xfe61
	.2byte 0x2184
	.2byte 0x2200
	.2byte 0x0049
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfe47
	.2byte 0x2028
	.2byte 0xf001
	.2byte 0xfdd8
	.2byte 0x48b2
	.2byte 0xf001
	.2byte 0xfe31
	.2byte 0x200a
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfe31
	.2byte 0x200a
	.2byte 0x2103
	.2byte 0xf001
	.2byte 0xfe0d
	.2byte 0x200a
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfe29
	.2byte 0x200a
	.2byte 0x2108
	.2byte 0xf001
	.2byte 0xfe01
	.2byte 0x20c0
	.2byte 0x01c0
	.2byte 0x300a
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfe1f
	.2byte 0x23c0
	.2byte 0x025b
	.2byte 0x4658
	.2byte 0x21f4
	.2byte 0x22a4
	.2byte 0x6303
	.2byte 0x0052
	.2byte 0x0049
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfdd8
	.2byte 0x2005
	.2byte 0xf001
	.2byte 0xfdb1
	.2byte 0x2106
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfde9
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfdaa
	.2byte 0x21c0
	.2byte 0x465a
	.2byte 0x02c9
	.2byte 0x6291
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfda3
	.2byte 0x235b
	.2byte 0x445b
	.2byte 0x4699
	.2byte 0x4647
	.2byte 0x2301
	.2byte 0x375b
	.2byte 0x4648
	.2byte 0x7003
	.2byte 0x2100
	.2byte 0x703b
	.2byte 0x4658
	.2byte 0xf001
	.2byte 0xfd3e
	.2byte 0x4640
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfd3a
	.2byte 0x4658
	.2byte 0x2106
	.2byte 0xf001
	.2byte 0xfd7e
	.2byte 0x4640
	.2byte 0x2106
	.2byte 0xf001
	.2byte 0xfd7a
	.2byte 0x251e
	.2byte 0x2205
	.2byte 0x2302
	.2byte 0x2613
	.2byte 0x214c
	.2byte 0x2055
	.2byte 0x9500
	.2byte 0x9601
	.2byte 0xf001
	.2byte 0xfd58
	.2byte 0x208f
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfe5c
	.2byte 0x20fe
	.2byte 0x01c0
	.2byte 0x2100
	.2byte 0x30ff
	.2byte 0xf001
	.2byte 0xfe16
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfe1b
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd70
	.2byte 0x2080
	.2byte 0x2100
	.2byte 0x0240
	.2byte 0xf001
	.2byte 0xfe0b
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfe10
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd65
	.2byte 0x203c
	.2byte 0xf001
	.2byte 0xfd62
	.2byte 0x4658
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfd4e
	.2byte 0x4640
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfd4a
	.2byte 0x2302
	.2byte 0x2055
	.2byte 0x214a
	.2byte 0x2205
	.2byte 0x9500
	.2byte 0x9601
	.2byte 0xf001
	.2byte 0xfd2a
	.2byte 0x464a
	.2byte 0x4651
	.2byte 0x7011
	.2byte 0x4658
	.2byte 0x7039
	.2byte 0x2110
	.2byte 0xf001
	.2byte 0xfcf2
	.2byte 0x2110
	.2byte 0x4640
	.2byte 0xf001
	.2byte 0xfcee
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd53
	.2byte 0x305a
	.2byte 0x7802
	.2byte 0x25fe
	.2byte 0x1c2b
	.2byte 0x4013
	.2byte 0x21e4
	.2byte 0x22b4
	.2byte 0x0052
	.2byte 0x7003
	.2byte 0x0049
	.2byte 0x200a
	.2byte 0x2680
	.2byte 0xf001
	.2byte 0xfd5d
	.2byte 0x02f6
	.2byte 0x465b
	.2byte 0x2181
	.2byte 0x629e
	.2byte 0x0049
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd9d
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd5a
	.2byte 0x2102
	.2byte 0x2214
	.2byte 0x31ff
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd90
	.2byte 0x201e
	.2byte 0xf001
	.2byte 0xfd21
	.2byte 0x2108
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd59
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd1a
	.2byte 0x20c0
	.2byte 0x0240
	.2byte 0x4659
	.2byte 0x6308
	.2byte 0x22a4
	.2byte 0x21f4
	.2byte 0x0049
	.2byte 0x0052
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd33
	.2byte 0x200e
	.2byte 0xf001
	.2byte 0xfd0c
	.2byte 0x22c0
	.2byte 0x02d2
	.2byte 0x465b
	.2byte 0x629a
	.2byte 0x2106
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd40
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd01
	.2byte 0x208f
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfddd
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd0a
	.2byte 0x305a
	.2byte 0x7803
	.2byte 0x21e4
	.2byte 0x401d
	.2byte 0x22b4
	.2byte 0x0052
	.2byte 0x7005
	.2byte 0x0049
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd17
	.2byte 0x4658
	.2byte 0x2181
	.2byte 0x0049
	.2byte 0x6286
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd58
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd15
	.2byte 0x2100
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd41
	.2byte 0x200f
	.2byte 0xf001
	.2byte 0xfcde
	.2byte 0x2100
	.2byte 0x4640
	.2byte 0xf001
	.2byte 0xfc82
	.2byte 0x200f
	.2byte 0xf001
	.2byte 0xfcd7
	.2byte 0x20d7
	.2byte 0xf001
	.2byte 0xfdb4
	.2byte 0x201e
	.2byte 0x21f4
	.2byte 0x4b30
	.2byte 0x4a31
	.2byte 0x0449
	.2byte 0x30ff
	.2byte 0xf001
	.2byte 0xfc7c
	.2byte 0x2100
	.2byte 0x1c05
	.2byte 0xf001
	.2byte 0xfca8
	.2byte 0x1c28
	.2byte 0x492d
	.2byte 0xf001
	.2byte 0xfc70
	.2byte 0x1c28
	.2byte 0x2101
	.2byte 0xf001
	.2byte 0xfc64
	.2byte 0x1c28
	.2byte 0x2106
	.2byte 0xf001
	.2byte 0xfcac
	.2byte 0x2102
	.2byte 0x2200
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfcfb
	.2byte 0x2005
	.2byte 0xf001
	.2byte 0xfcb4
	.2byte 0x2181
	.2byte 0x0049
	.2byte 0x2200
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfd1a
	.2byte 0x2037
	.2byte 0xf001
	.2byte 0xfcab
	.2byte 0x2005
	.2byte 0xf001
	.2byte 0xfca8
	.2byte 0x2088
	.2byte 0xf001
	.2byte 0xfd85
	.2byte 0x20ce
	.2byte 0x21f4
	.2byte 0x4a1a
	.2byte 0x4b18
	.2byte 0x0449
	.2byte 0x0040
	.2byte 0xf001
	.2byte 0xfc4d
	.2byte 0x2100
	.2byte 0x1c05
	.2byte 0xf001
	.2byte 0xfc79
	.2byte 0x4917
	.2byte 0x1c28
	.2byte 0xf001
	.2byte 0xfc41
	.2byte 0x6d2a
	.2byte 0x2380
	.2byte 0x490f
	.2byte 0x019b
	.2byte 0x8253
	.2byte 0x22c0
	.2byte 0x1c2b
	.2byte 0x0252
	.2byte 0x3355
	.2byte 0x632a
	.2byte 0x7019
	.2byte 0x23e4
	.2byte 0x045b
	.2byte 0x63ab
	.2byte 0x4b0f
	.2byte 0x200a
	.2byte 0x63eb
	.2byte 0x23b3
	.2byte 0x045b
	.2byte 0x642b
	.2byte 0x23c0
	.2byte 0x01db
	.2byte 0x61ab
	.2byte 0x61eb
	.2byte 0xf001
	.2byte 0xfc7b
	.2byte 0x2110
	.2byte 0x4640
	.2byte 0xf001
	.2byte 0xfc1f
	.2byte 0x200a
	.2byte 0xe00e
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte MsgFieldAtLastTheSerpentHas
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0159
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xd39c
	.2byte 0x0200
	.2byte 0xd360
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xf001
	.2byte 0xfc64
	.2byte 0x2085
	.2byte 0xf001
	.2byte 0xfd41
	.2byte 0x4653
	.2byte 0x2280
	.2byte 0x60ab
	.2byte 0x612b
	.2byte 0x1c30
	.2byte 0x1c31
	.2byte 0x0252
	.2byte 0xf001
	.2byte 0xfc40
	.2byte 0x2700
	.2byte 0x2108
	.2byte 0x4249
	.2byte 0x2f01
	.2byte 0xdd00
	.2byte 0x2108
	.2byte 0x2301
	.2byte 0x2208
	.2byte 0x403b
	.2byte 0x4252
	.2byte 0x2b00
	.2byte 0xd100
	.2byte 0x2208
	.2byte 0x20e4
	.2byte 0x0440
	.2byte 0x0413
	.2byte 0x0409
	.2byte 0x22b3
	.2byte 0x1809
	.2byte 0x0452
	.2byte 0x2094
	.2byte 0x189b
	.2byte 0x30ff
	.2byte 0x4a44
	.2byte 0xf001
	.2byte 0xfbee
	.2byte 0x00bd
	.2byte 0xae03
	.2byte 0x5170
	.2byte 0x2102
	.2byte 0xf001
	.2byte 0xfbdc
	.2byte 0x5970
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfc14
	.2byte 0x3701
	.2byte 0x5970
	.2byte 0x493d
	.2byte 0xf001
	.2byte 0xfbdb
	.2byte 0x2f03
	.2byte 0xddd5
	.2byte 0x23a0
	.2byte 0x031b
	.2byte 0x4658
	.2byte 0x6283
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfc36
	.2byte 0x305a
	.2byte 0x7802
	.2byte 0x23fe
	.2byte 0x4013
	.2byte 0x21d8
	.2byte 0x22a0
	.2byte 0x7003
	.2byte 0x0049
	.2byte 0x0052
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfc42
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfc17
	.2byte 0x22e6
	.2byte 0x2001
	.2byte 0x2101
	.2byte 0x0212
	.2byte 0x3266
	.2byte 0x4240
	.2byte 0x4249
	.2byte 0xf001
	.2byte 0xfbf6
	.2byte 0x2105
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfc46
	.2byte 0x201e
	.2byte 0xf001
	.2byte 0xfc07
	.2byte 0x200a
	.2byte 0x2101
	.2byte 0xf001
	.2byte 0xfc87
	.2byte 0xf001
	.2byte 0xfc81
	.2byte 0x4d26
	.2byte 0x200a
	.2byte 0x1c29
	.2byte 0xf001
	.2byte 0xfc14
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfc15
	.2byte 0x2014
	.2byte 0xf001
	.2byte 0xfbf6
	.2byte 0x1c29
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfc0a
	.2byte 0x2014
	.2byte 0xf001
	.2byte 0xfbef
	.2byte 0x20c0
	.2byte 0x01c0
	.2byte 0x300a
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfc49
	.2byte 0x4b1b
	.2byte 0x2185
	.2byte 0x0089
	.2byte 0x185b
	.2byte 0x6818
	.2byte 0x2101
	.2byte 0xf001
	.2byte 0xfc55
	.2byte 0xf001
	.2byte 0xfc5f
	.2byte 0x200a
	.2byte 0x2103
	.2byte 0xf001
	.2byte 0xfc43
	.2byte 0x231b
	.2byte 0x2213
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x201e
	.2byte 0x2113
	.2byte 0x2201
	.2byte 0x2301
	.2byte 0xf001
	.2byte 0xfba5
	.2byte 0xf001
	.2byte 0xfbd7
	.2byte 0xf000
	.2byte 0xfdc9
	.2byte 0x200a
	.2byte 0x30ff
	.2byte 0xf001
	.2byte 0xfb55
	.2byte 0x2800
	.2byte 0xd001
	.2byte 0xf000
	.2byte 0xfdc1
	.2byte 0x21ec
	.2byte 0x22e4
	.2byte 0x200a
	.2byte 0x0449
	.2byte 0x0452
	.2byte 0xf001
	.2byte 0xfbf2
	.2byte 0x4906
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfbd2
	.2byte 0xf000
	.2byte 0xfdb4
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xd39c
	.2byte 0x0200
	.2byte 0xde28
	.2byte 0x0200
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0xd78c
	.2byte 0x0200
	.2byte 0x2b0a
	.2byte 0xd000
	.2byte 0xe2d0
	.2byte 0x2007
	.2byte 0xf001
	.2byte 0xfc88
	.2byte 0x2090
	.2byte 0x0100
	.2byte 0x3027
	.2byte 0xf001
	.2byte 0xfb2f
	.2byte 0x4681
	.2byte 0x2800
	.2byte 0xd001
	.2byte 0xf000
	.2byte 0xfd9a
	.2byte 0xf001
	.2byte 0xfba0
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xfc55
	.2byte 0x2090
	.2byte 0x0100
	.2byte 0x3027
	.2byte 0xf001
	.2byte 0xfb24
	.2byte 0x48e3
	.2byte 0xf001
	.2byte 0xfbed
	.2byte 0x2200
	.2byte 0x2009
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfbc0
	.2byte 0x2103
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfbc4
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfb95
	.2byte 0x2280
	.2byte 0x0252
	.2byte 0x6302
	.2byte 0x200a
	.2byte 0x4692
	.2byte 0xf001
	.2byte 0xfb8e
	.2byte 0x2680
	.2byte 0x1c05
	.2byte 0x0236
	.2byte 0x21dc
	.2byte 0x229c
	.2byte 0x2785
	.2byte 0x200b
	.2byte 0x0449
	.2byte 0x0452
	.2byte 0x632e
	.2byte 0x00bf
	.2byte 0x4447
	.2byte 0xf001
	.2byte 0xfba4
	.2byte 0x21e4
	.2byte 0x22a4
	.2byte 0x6838
	.2byte 0x0449
	.2byte 0x0452
	.2byte 0xf001
	.2byte 0xfb9d
	.2byte 0x21f4
	.2byte 0x22d4
	.2byte 0x200a
	.2byte 0x0449
	.2byte 0x0452
	.2byte 0xf001
	.2byte 0xfb96
	.2byte 0x6838
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfbc1
	.2byte 0x200b
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfbbc
	.2byte 0x21c0
	.2byte 0x200a
	.2byte 0x0209
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfbb6
	.2byte 0xf001
	.2byte 0xfbfc
	.2byte 0xf001
	.2byte 0xfc02
	.2byte 0x2181
	.2byte 0x221e
	.2byte 0x0049
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfbb4
	.2byte 0x2080
	.2byte 0x0080
	.2byte 0x3012
	.2byte 0xf001
	.2byte 0xfc23
	.2byte 0x2100
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfb9f
	.2byte 0x2080
	.2byte 0x0080
	.2byte 0x3012
	.2byte 0xf001
	.2byte 0xfc1a
	.2byte 0x200a
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfb96
	.2byte 0x2180
	.2byte 0x0049
	.2byte 0x2200
	.2byte 0x6838
	.2byte 0xf001
	.2byte 0xfb9c
	.2byte 0x2180
	.2byte 0x0049
	.2byte 0x2200
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfb96
	.2byte 0x200f
	.2byte 0xf001
	.2byte 0xfb27
	.2byte 0x2180
	.2byte 0x6838
	.2byte 0x01c9
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfb85
	.2byte 0x2180
	.2byte 0x2200
	.2byte 0x01c9
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfb7f
	.2byte 0x201e
	.2byte 0xf001
	.2byte 0xfb18
	.2byte 0x200a
	.2byte 0x2101
	.2byte 0xf001
	.2byte 0xfb98
	.2byte 0xf001
	.2byte 0xfb92
	.2byte 0x200a
	.2byte 0x2102
	.2byte 0xf001
	.2byte 0xfb56
	.2byte 0x200a
	.2byte 0x2101
	.2byte 0xf001
	.2byte 0xfb7e
	.2byte 0x21f4
	.2byte 0x22bc
	.2byte 0x0052
	.2byte 0x0049
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfb2b
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfb30
	.2byte 0x2102
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfb44
	.2byte 0x2014
	.2byte 0xf001
	.2byte 0xfaf9
	.2byte 0x21ec
	.2byte 0x22b4
	.2byte 0x0049
	.2byte 0x0052
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfb1a
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfb1f
	.2byte 0x6838
	.2byte 0x210a
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfb3a
	.2byte 0x200b
	.2byte 0x210a
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfb35
	.2byte 0x200a
	.2byte 0x2108
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfb30
	.2byte 0x2001
	.2byte 0x2101
	.2byte 0x2201
	.2byte 0x4252
	.2byte 0x2300
	.2byte 0x4240
	.2byte 0x4249
	.2byte 0xf001
	.2byte 0xfb53
	.2byte 0x200a
	.2byte 0x2108
	.2byte 0xf001
	.2byte 0xfb0f
	.2byte 0x200a
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfb2f
	.2byte 0x23c0
	.2byte 0x025b
	.2byte 0x21f4
	.2byte 0x22a4
	.2byte 0x0052
	.2byte 0x632b
	.2byte 0x0049
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfae9
	.2byte 0x2005
	.2byte 0xf001
	.2byte 0xfac2
	.2byte 0x2106
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfafa
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfabb
	.2byte 0x23c0
	.2byte 0x02db
	.2byte 0x62ab
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfab5
	.2byte 0x208f
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfb91
	.2byte 0x1c2a
	.2byte 0x20fe
	.2byte 0x325b
	.2byte 0x2301
	.2byte 0x01c0
	.2byte 0x7013
	.2byte 0x2100
	.2byte 0x30ff
	.2byte 0xf001
	.2byte 0xfb47
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfb4c
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfaa1
	.2byte 0x2080
	.2byte 0x0080
	.2byte 0x3012
	.2byte 0xf001
	.2byte 0xfb7c
	.2byte 0x2100
	.2byte 0x4650
	.2byte 0xf001
	.2byte 0xfb38
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfb3d
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfa92
	.2byte 0x20fe
	.2byte 0x01c0
	.2byte 0x2100
	.2byte 0x30ff
	.2byte 0xf001
	.2byte 0xfb2c
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfb31
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfa86
	.2byte 0x2100
	.2byte 0x4650
	.2byte 0xf001
	.2byte 0xfb22
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfb27
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfa7c
	.2byte 0x20fe
	.2byte 0x01c0
	.2byte 0x2100
	.2byte 0x30ff
	.2byte 0xf001
	.2byte 0xfb16
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfb1b
	.2byte 0x203c
	.2byte 0xf001
	.2byte 0xfa70
	.2byte 0x2008
	.2byte 0x2104
	.2byte 0xf001
	.2byte 0xfaa8
	.2byte 0x231e
	.2byte 0x2213
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2055
	.2byte 0x2148
	.2byte 0x2205
	.2byte 0x2302
	.2byte 0xf001
	.2byte 0xfa3a
	.2byte 0x4b4b
	.2byte 0x21e4
	.2byte 0x60eb
	.2byte 0x22b4
	.2byte 0x464b
	.2byte 0x62ab
	.2byte 0x0452
	.2byte 0x200a
	.2byte 0x0449
	.2byte 0x632e
	.2byte 0xf001
	.2byte 0xfa8a
	.2byte 0x200a
	.2byte 0x2101
	.2byte 0xf001
	.2byte 0xfa8e
	.2byte 0x2100
	.2byte 0x4650
	.2byte 0xf001
	.2byte 0xfaee
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfaf3
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfa48
	.2byte 0x2200
	.2byte 0x6839
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfa93
	.2byte 0x200a
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfa9f
	.2byte 0x200a
	.2byte 0x2103
	.2byte 0xf001
	.2byte 0xfa7b
	.2byte 0x200a
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfa97
	.2byte 0x2180
	.2byte 0x2200
	.2byte 0x200a
	.2byte 0x01c9
	.2byte 0xf001
	.2byte 0xfa95
	.2byte 0x200a
	.2byte 0x2102
	.2byte 0xf001
	.2byte 0xfa69
	.2byte 0x21f4
	.2byte 0x22c4
	.2byte 0x0052
	.2byte 0x200a
	.2byte 0x0049
	.2byte 0xf001
	.2byte 0xfa4e
	.2byte 0x2101
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfa6a
	.2byte 0x2005
	.2byte 0xf001
	.2byte 0xfa1f
	.2byte 0x2100
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfa7b
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfa48
	.2byte 0x200a
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfa50
	.2byte 0x2200
	.2byte 0x210b
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfa5f
	.2byte 0x200f
	.2byte 0xf001
	.2byte 0xfa0c
	.2byte 0x200a
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfa68
	.2byte 0x200a
	.2byte 0x2103
	.2byte 0xf001
	.2byte 0xfa44
	.2byte 0x2180
	.2byte 0x2200
	.2byte 0x200a
	.2byte 0x01c9
	.2byte 0xf001
	.2byte 0xfa62
	.2byte 0x200a
	.2byte 0x2102
	.2byte 0xf001
	.2byte 0xfa36
	.2byte 0x21f4
	.2byte 0x2286
	.2byte 0x0092
	.2byte 0x200a
	.2byte 0x0049
	.2byte 0xf001
	.2byte 0xfa1b
	.2byte 0x6838
	.2byte 0x2101
	.2byte 0xf001
	.2byte 0xfa63
	.2byte 0x6838
	.2byte 0x210b
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfa3a
	.2byte 0x2200
	.2byte 0x6839
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfa35
	.2byte 0x201e
	.2byte 0xf001
	.2byte 0xf9e2
	.2byte 0x200b
	.2byte 0x2103
	.2byte 0xf001
	.2byte 0xfa1e
	.2byte 0x200b
	.2byte 0x2102
	.2byte 0xf001
	.2byte 0xfa16
	.2byte 0x6838
	.2byte 0xf001
	.2byte 0xf9e7
	.2byte 0x2800
	.2byte 0xd00c
	.2byte 0x220a
	.2byte 0x5e81
	.2byte 0x2312
	.2byte 0x5ec2
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xf9f2
	.2byte 0xe004
	.2byte 0x0000
	.2byte MsgFieldItLooksLikeTheDragon
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xf9f5
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xf9f4
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xf9ed
	.2byte 0x200a
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xf9ec
	.2byte 0x21f4
	.2byte 0x22bc
	.2byte 0x0049
	.2byte 0x0052
	.2byte 0x6838
	.2byte 0xf001
	.2byte 0xf9d9
	.2byte 0x6838
	.2byte 0xf001
	.2byte 0xf9de
	.2byte 0x20a0
	.2byte 0x21a0
	.2byte 0x02c9
	.2byte 0x4652
	.2byte 0x02c0
	.2byte 0xf001
	.2byte 0xf98f
	.2byte 0x206b
	.2byte 0xf001
	.2byte 0xfa84
	.2byte 0x201e
	.2byte 0xf001
	.2byte 0xf9a1
	.2byte 0x2180
	.2byte 0x0049
	.2byte 0x2200
	.2byte 0x6838
	.2byte 0xf001
	.2byte 0xfa07
	.2byte 0x200f
	.2byte 0xf001
	.2byte 0xf998
	.2byte 0x21c0
	.2byte 0x0209
	.2byte 0x2200
	.2byte 0x6838
	.2byte 0xf001
	.2byte 0xf9f6
	.2byte 0x200f
	.2byte 0xf001
	.2byte 0xf98f
	.2byte 0x200c
	.2byte 0xf001
	.2byte 0xf99c
	.2byte 0x2380
	.2byte 0x04db
	.2byte 0x3352
	.2byte 0x881b
	.2byte 0x466d
	.2byte 0x350a
	.2byte 0x4680
	.2byte 0x21a0
	.2byte 0x20a0
	.2byte 0x802b
	.2byte 0x0309
	.2byte 0x4652
	.2byte 0x0300
	.2byte 0xf001
	.2byte 0xf965
	.2byte 0x206b
	.2byte 0xf001
	.2byte 0xfa5a
	.2byte 0xf001
	.2byte 0xf93c
	.2byte 0x782b
	.2byte 0x802b
	.2byte 0x2b00
	.2byte 0xd015
	.2byte 0x882b
	.2byte 0x4a09
	.2byte 0x2180
	.2byte 0x04c9
	.2byte 0x4313
	.2byte 0x3152
	.2byte 0x800b
	.2byte 0x20ff
	.2byte 0x882b
	.2byte 0x0200
	.2byte 0x30ff
	.2byte 0x181b
	.2byte 0x802b
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xf8cc
	.2byte 0x882b
	.2byte 0x2b00
	.2byte 0xd1ec
	.2byte 0xe001
	.2byte 0x1000
	.2byte 0x0000
	.2byte 0x2180
	.2byte 0x04c9
	.2byte 0x880a
	.2byte 0x23fd
	.2byte 0x021b
	.2byte 0x33ff
	.2byte 0x4013
	.2byte 0x800b
	.2byte 0x201e
	.2byte 0x26e8
	.2byte 0x25ae
	.2byte 0xf001
	.2byte 0xf951
	.2byte 0x046d
	.2byte 0x0476
	.2byte 0x2344
	.2byte 0x1c2a
	.2byte 0x492e
	.2byte 0x33ff
	.2byte 0x1c30
	.2byte 0xf7fc
	.2byte 0xfa5e
	.2byte 0x1c2a
	.2byte 0x1c07
	.2byte 0x1c31
	.2byte 0x200c
	.2byte 0xf001
	.2byte 0xf976
	.2byte 0x2500
	.2byte 0x1c28
	.2byte 0x2105
	.2byte 0xf001
	.2byte 0xf8a1
	.2byte 0x2800
	.2byte 0xd101
	.2byte 0xf7fe
	.2byte 0xf85d
	.2byte 0x4641
	.2byte 0x690b
	.2byte 0x4a24
	.2byte 0x2002
	.2byte 0x189b
	.2byte 0x610b
	.2byte 0x3501
	.2byte 0xf001
	.2byte 0xf930
	.2byte 0x2d13
	.2byte 0xddec
	.2byte 0x2090
	.2byte 0x0100
	.2byte 0x30ee
	.2byte 0xf001
	.2byte 0xf8b9
	.2byte 0x22e6
	.2byte 0x2001
	.2byte 0x2101
	.2byte 0x0212
	.2byte 0x4249
	.2byte 0x3266
	.2byte 0x4240
	.2byte 0xf001
	.2byte 0xf908
	.2byte 0x2300
	.2byte 0x60bb
	.2byte 0x613b
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xf91a
	.2byte 0x1c38
	.2byte 0xf001
	.2byte 0xf8cb
	.2byte 0x20c3
	.2byte 0x0040
	.2byte 0xf001
	.2byte 0xf9f3
	.2byte 0x201e
	.2byte 0xf001
	.2byte 0xf910
	.2byte 0x2180
	.2byte 0x04c9
	.2byte 0x880b
	.2byte 0x4a0b
	.2byte 0x466e
	.2byte 0x4313
	.2byte 0x800b
	.2byte 0x360a
	.2byte 0x8833
	.2byte 0x4a09
	.2byte 0x3152
	.2byte 0x4313
	.2byte 0x800b
	.2byte 0x2280
	.2byte 0x4b07
	.2byte 0x04d2
	.2byte 0x3250
	.2byte 0x8013
	.2byte 0x8833
	.2byte 0x2b08
	.2byte 0xd01b
	.2byte 0x8833
	.2byte 0x4a02
	.2byte 0x2180
	.2byte 0x04c9
	.2byte 0xe009
	.2byte 0x0f00
	.2byte 0x0000
	.2byte 0x1000
	.2byte 0x0000
	.2byte 0x3f42
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x4313
	.2byte 0x3152
	.2byte 0x800b
	.2byte 0x1c35
	.2byte 0x8833
	.2byte 0x2001
	.2byte 0x3301
	.2byte 0x8033
	.2byte 0xf001
	.2byte 0xf84a
	.2byte 0x882b
	.2byte 0x2b08
	.2byte 0xd1e3
	.2byte 0xf001
	.2byte 0xf89d
	.2byte 0xf001
	.2byte 0xf8e3
	.2byte 0xe2d5
	.2byte 0x2b0b
	.2byte 0xd122
	.2byte 0xf001
	.2byte 0xf8da
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xf98f
	.2byte 0x2585
	.2byte 0x2009
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xf901
	.2byte 0x00ad
	.2byte 0x21e0
	.2byte 0x22ac
	.2byte 0x200a
	.2byte 0x0449
	.2byte 0x0452
	.2byte 0x4445
	.2byte 0xf001
	.2byte 0xf8f8
	.2byte 0x2200
	.2byte 0x6829
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xf90f
	.2byte 0x2113
	.2byte 0x6828
	.2byte 0xf001
	.2byte 0xf8f7
	.2byte 0xf001
	.2byte 0xf965
	.2byte 0xf001
	.2byte 0xf96b
	.2byte 0x48e5
	.2byte 0xe02d
	.2byte 0x2b0c
	.2byte 0xd000
	.2byte 0xe2ad
	.2byte 0xf001
	.2byte 0xf8b4
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xf969
	.2byte 0x2009
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xf8dc
	.2byte 0x21d7
	.2byte 0x22a0
	.2byte 0x0452
	.2byte 0x200a
	.2byte 0x0449
	.2byte 0xf001
	.2byte 0xf8d5
	.2byte 0x200a
	.2byte 0x2105
	.2byte 0xf001
	.2byte 0xf8d9
	.2byte 0x2385
	.2byte 0x009b
	.2byte 0x4443
	.2byte 0x6818
	.2byte 0x2113
	.2byte 0xf001
	.2byte 0xf8d2
	.2byte 0xf001
	.2byte 0xf940
	.2byte 0xf001
	.2byte 0xf946
	.2byte 0x201e
	.2byte 0xf001
	.2byte 0xf88f
	.2byte 0x2102
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xf8d3
	.2byte 0x201e
	.2byte 0xf001
	.2byte 0xf888
	.2byte 0x48ce
	.2byte 0xf001
	.2byte 0xf8e1
	.2byte 0x2100
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xf8e1
	.2byte 0xf001
	.2byte 0xf92f
	.2byte 0xf001
	.2byte 0xf931
	.2byte 0x23f2
	.2byte 0x005b
	.2byte 0x4443
	.2byte 0x2100
	.2byte 0x5e58
	.2byte 0x23f3
	.2byte 0x005b
	.2byte 0x4443
	.2byte 0x2200
	.2byte 0x5e99
	.2byte 0xf001
	.2byte 0xf8f9
	.2byte 0x23f0
	.2byte 0x005b
	.2byte 0x4443
	.2byte 0x2100
	.2byte 0x5e58
	.2byte 0x2200
	.2byte 0x5eb1
	.2byte 0xf001
	.2byte 0xf940
	.2byte 0xf001
	.2byte 0xf86e
	.2byte 0xe260
.L_0200bd8a:
	ldr r3, .L_0200c084
	cmp r2, r3
	bne .L_0200be74
	movs r0, #8
	bl Object_GetById
	adds r2, r0, #0
	adds r2, #35
	ldrb r3, [r2]
	movs r5, #32
	orrs r3, r5
	strb r3, [r2]
	movs r1, #3
	bl Func_02004e3c
	movs r0, #9
	bl Object_GetById
	adds r2, r0, #0
	adds r2, #35
	ldrb r3, [r2]
	movs r1, #3
	orrs r5, r3
	strb r5, [r2]
	bl Func_02004e3c
	movs r0, #8
	movs r1, #12
	bl Func_02002ac8
	movs r0, #9
	movs r1, #12
	bl Func_02002ac8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #98
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bdfa
	movs r1, #220
	movs r2, #172
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02004ebc
	movs r1, #138
	movs r2, #172
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02004ebc
	b .L_0200be20
.L_0200bdfa:
	movs r3, #89
	movs r5, #40
	str r3, [sp, #0]
	movs r0, #88
	movs r1, #40
	movs r2, #1
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02004df4
	movs r3, #100
	str r3, [sp, #0]
	movs r0, #99
	movs r1, #40
	movs r2, #1
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02004df4
.L_0200be20:
	movs r0, #132
	lsls r0, r0, #2
	bl GameFlag_GetByte
	adds r5, r0, #0
	movs r0, #132
	movs r3, #15
	lsls r0, r0, #2
	ands r5, r3
	bl GameFlag_GetByte
	asrs r0, r0, #4
	lsls r0, r0, #24
	lsrs r6, r0, #24
	cmp r5, #0
	beq .L_0200be54
	ldr r2, .L_0200c088
	lsls r3, r5, #2
	ldrsh r1, [r2, r3]
	adds r3, #2
	ldrsh r2, [r2, r3]
	lsls r1, r1, #19
	lsls r2, r2, #19
	movs r0, #10
	bl Func_02004ebc
.L_0200be54:
	lsls r3, r6, #24
	asrs r0, r3, #24
	cmp r0, #0
	bne .L_0200be5e
	b .L_0200c24c
.L_0200be5e:
	ldr r2, .L_0200c088
	lsls r3, r0, #2
	ldrsh r1, [r2, r3]
	adds r3, #2
	ldrsh r2, [r2, r3]
	lsls r1, r1, #19
	lsls r2, r2, #19
	movs r0, #11
	bl Func_02004ebc
	b .L_0200c24c
.L_0200be74:
	ldr r3, .L_0200c08c
	cmp r2, r3
	bne .L_0200be92
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #99
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200be8a
	b .L_0200c24c
.L_0200be8a:
	movs r0, #1
	bl Func_02002cb4
	b .L_0200c24c
.L_0200be92:
	ldr r3, .L_0200c090
	cmp r2, r3
	beq .L_0200be9a
	b .L_0200c110
.L_0200be9a:
	movs r0, #10
	bl Object_GetById
	movs r1, #3
	adds r6, r0, #0
	bl Func_02004e3c
	movs r0, #8
	movs r1, #10
	bl Func_02002ac8
	movs r0, #9
	movs r1, #11
	bl Func_02002ac8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #188
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200beda
	movs r3, #26
	movs r2, #77
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #77
	movs r2, #1
	movs r3, #15
	bl Func_02004df4
.L_0200beda:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #189
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bf2e
	movs r1, #240
	movs r2, #176
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004ebc
	movs r3, #85
	str r3, [sp, #4]
	movs r5, #28
	movs r0, #2
	movs r1, #93
	movs r2, #1
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02004df4
	movs r6, #22
	movs r0, #27
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004dfc
	movs r3, #29
	str r3, [sp, #0]
	movs r0, #32
	movs r1, #16
	movs r2, #2
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02004dfc
.L_0200bf2e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #190
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bf82
	movs r1, #152
	movs r2, #176
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02004ebc
	movs r3, #85
	str r3, [sp, #4]
	movs r5, #36
	movs r0, #2
	movs r1, #93
	movs r2, #1
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02004df4
	movs r6, #22
	movs r0, #35
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004dfc
	movs r3, #37
	str r3, [sp, #0]
	movs r0, #32
	movs r1, #16
	movs r2, #2
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02004dfc
.L_0200bf82:
	movs r0, #140
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bfb8
	movs r3, #37
	movs r2, #26
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #61
	movs r1, #0
	movs r2, #3
	movs r3, #3
	bl Func_02004e04
	movs r3, #38
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02004dfc
.L_0200bfb8:
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #6
	beq .L_0200bfc8
	b .L_0200c0d2
.L_0200bfc8:
	movs r3, #60
	mov r8, r3
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r7, r0
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
	bl Event_StartScreenTransition
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #168
	movs r2, #192
	lsls r2, r2, #17
	lsls r1, r1, #18
	movs r0, #11
	bl Func_02004ebc
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #11
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #162
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	lsls r0, r0, #1
	bl Engine_ObjectCreate
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200c070
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r1, .L_0200c094
	adds r0, r5, #0
	bl Func_02004dac
	adds r0, r5, #0
	movs r1, #1
	bl Func_02004d9c
	movs r1, #14
	adds r0, r5, #0
	bl Animation_ApplyChildValues
	ldr r1, [r5, #80]
	movs r2, #12
	ldrb r3, [r1, #9]
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r3, .L_0200c098
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_0200c070:
	ldr r3, [r6, #12]
	movs r1, #128
	lsls r1, r1, #16
	adds r3, r3, r1
	str r3, [r6, #12]
	b .L_0200c0a2
	.2byte MsgFieldYouYouCannotFightThe
	.2byte 0x0000
	.2byte MsgFieldLikeISaidTheSerpent
	.2byte 0x0000
.L_0200c084:
	.4byte 0x000000b2
.L_0200c088:
	.4byte Data_0200530c
.L_0200c08c:
	.4byte 0x000000b4
.L_0200c090:
	.4byte 0x000000b3
.L_0200c094:
	.4byte Data_020053d8
.L_0200c098:
	.4byte 0x00013333
.L_0200c09c:
	movs r2, #1
	negs r2, r2
	add r8, r2
.L_0200c0a2:
	mov r3, r8
	cmp r3, #0
	beq .L_0200c0b8
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #40]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	ble .L_0200c09c
.L_0200c0b8:
	movs r0, #188
	bl Func_02005014
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #3
	bl Func_02004f6c
.L_0200c0d2:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #196
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c0ea
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
.L_0200c0ea:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c0f8
	b .L_0200c24c
.L_0200c0f8:
	ldr r3, .L_0200c264
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	beq .L_0200c10a
	b .L_0200c24c
.L_0200c10a:
	bl Func_0200042c
	b .L_0200c24c
.L_0200c110:
	ldr r3, .L_0200c268
	cmp r2, r3
	beq .L_0200c118
	b .L_0200c24c
.L_0200c118:
	movs r0, #10
	movs r1, #1
	bl Func_02004fec
	movs r0, #8
	movs r1, #9
	bl Func_02002ac8
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r7, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #1
	beq .L_0200c13e
	cmp r3, #3
	beq .L_0200c13e
	cmp r3, #5
	bne .L_0200c142
.L_0200c13e:
	bl Func_02004d00
.L_0200c142:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c162
	movs r1, #192
	movs r2, #150
	movs r3, #170
	lsls r1, r1, #17
	lsls r2, r2, #15
	lsls r3, r3, #18
	movs r0, #11
	bl Func_02004a5c
.L_0200c162:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c17e
	movs r2, #204
	movs r3, #170
	ldr r1, .L_0200c26c
	lsls r2, r2, #14
	lsls r3, r3, #18
	movs r0, #12
	bl Func_02004a5c
.L_0200c17e:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c19c
	movs r2, #220
	movs r3, #170
	ldr r1, .L_0200c270
	lsls r2, r2, #14
	lsls r3, r3, #18
	movs r0, #13
	bl Func_02004a5c
.L_0200c19c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #188
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c1ee
	movs r1, #192
	movs r2, #184
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004ebc
	movs r3, #85
	str r3, [sp, #4]
	movs r5, #25
	movs r0, #1
	movs r1, #85
	movs r2, #1
	movs r3, #4
	str r5, [sp, #0]
	bl Func_02004df4
	movs r6, #23
	movs r0, #27
	movs r1, #15
	movs r2, #2
	movs r3, #1
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004dfc
	movs r0, #25
	movs r1, #25
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004dfc
.L_0200c1ee:
	movs r0, #15
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #196
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c24c
	movs r3, #104
	str r3, [sp, #0]
	movs r5, #20
	movs r0, #117
	movs r1, #20
	movs r2, #7
	movs r3, #8
	str r5, [sp, #4]
	bl Func_02004e04
	movs r3, #111
	str r3, [sp, #0]
	movs r0, #111
	movs r1, #12
	movs r2, #1
	movs r3, #6
	str r5, [sp, #4]
	bl Func_02004e04
	movs r3, #40
	movs r2, #84
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #84
	movs r2, #8
	movs r3, #8
	bl Func_02004dfc
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004ebc
.L_0200c24c:
	movs r0, #0
	bl Func_02004fc4
	movs r0, #0
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c264:
	.4byte gPartyState
.L_0200c268:
	.4byte 0x000000b5
.L_0200c26c:
	.4byte 0x01cf0000
.L_0200c270:
	.4byte 0x02350000
	.endif
	.section .text.x0200c280,"ax",%progbits
	.global Func_02004280
	.thumb_func
Func_02004280:
	push {r5, r6, r7, lr}
	ldr r1, .L_0200c334
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200c338
	sub sp, #4
	cmp r2, r3
	beq .L_0200c298
	b .L_0200c41c
.L_0200c298:
	movs r3, #192
	movs r2, #241
	lsls r3, r3, #18
	lsls r2, r2, #1
	ldr r7, [r3, #32]
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #4
	ble .L_0200c2ae
	b .L_0200c3ac
.L_0200c2ae:
	adds r2, #50
	adds r3, r1, r2
	ldr r0, [r3]
	bl UnitGet
	adds r2, r0, #0
	movs r5, #0
	adds r2, #13
.L_0200c2be:
	ldrb r3, [r0]
	adds r0, #1
	adds r5, r5, r3
	cmp r0, r2
	ble .L_0200c2be
	adds r0, r5, #0
	bl Func_020044f8
	bl Func_02004fe4
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r3, #6
	adds r3, #255
	adds r2, r7, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #12
	ldrh r2, [r0]
	movs r3, #255
	lsls r3, r3, #8
	mov r1, sp
	adds r3, #252
	adds r1, #2
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200c330
	orrs r3, r2
	strh r3, [r0]
	bl Func_020048a0
	ldr r5, .L_0200c33c
	ldr r3, [r5]
	cmp r3, #3
	bne .L_0200c340
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #246
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c340
	movs r1, #208
	movs r2, #176
	movs r0, #64
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl Func_02004ebc
	b .L_0200c340
.L_0200c330:
	.4byte 0x00000001
.L_0200c334:
	.4byte gPartyState
.L_0200c338:
	.4byte 0x000000b1
.L_0200c33c:
	.4byte Data_020023c4 + 0x288
.L_0200c340:
	ldr r3, [r5]
	cmp r3, #9
	bne .L_0200c354
	movs r1, #208
	movs r2, #144
	movs r0, #65
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl Func_02004ebc
.L_0200c354:
	ldr r0, [r5]
	movs r2, #165
	lsls r2, r2, #4
	adds r0, r0, r2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c366
	b .L_0200c4c6
.L_0200c366:
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02004904
	movs r1, #192
	adds r5, r0, #0
	lsls r1, r1, #8
	adds r7, r6, #0
	ands r1, r5
	movs r3, #0
	adds r7, #89
	lsrs r1, r1, #14
	strb r3, [r7]
	adds r1, #2
	adds r0, r6, #0
	bl Func_02004d9c
	movs r3, #128
	lsls r3, r3, #7
	ands r3, r5
	cmp r3, #0
	beq .L_0200c39a
	ldr r3, .L_0200c4cc
	b .L_0200c39c
.L_0200c39a:
	ldr r3, .L_0200c4d0
.L_0200c39c:
	str r3, [r6, #108]
	movs r3, #1
	movs r0, #128
	strb r3, [r7]
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_0200c4c6
.L_0200c3ac:
	cmp r3, #8
	bgt .L_0200c3b2
	b .L_0200c4c6
.L_0200c3b2:
	ldr r2, .L_0200c4d4
	movs r3, #1
	str r3, [r2]
	movs r3, #188
	lsls r3, r3, #1
	adds r5, r7, r3
	movs r3, #127
	strh r3, [r5, #40]
	movs r1, #128
	movs r3, #254
	movs r2, #0
	lsls r1, r1, #9
	lsls r3, r3, #6
	str r2, [r5, #24]
	str r2, [r5, #28]
	str r2, [r5, #32]
	str r2, [r5, #36]
	str r1, [r5, #16]
	str r1, [r5, #20]
	strh r3, [r5, #42]
	adds r3, r7, #0
	adds r3, #228
	ldr r0, [r3]
	ldr r6, .L_0200c4d8
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #8]
	ldr r1, [r5, #20]
	adds r0, r0, r3
	str r0, [r5]
	adds r3, r7, #0
	adds r3, #232
	ldr r0, [r3]
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #12]
	adds r0, r0, r3
	str r0, [r5, #4]
	bl Func_02004dc4
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_0200c4dc
	ldr r3, .L_0200c4e0
	ldr r0, .L_0200c4e4
	movs r2, #24
	mov lr, r3
	.2byte 0xf800
	ldr r0, .L_0200c4e4
	bl Func_02004de4
	b .L_0200c4c6
.L_0200c41c:
	ldr r3, .L_0200c4e8
	cmp r2, r3
	bne .L_0200c474
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #9
	bhi .L_0200c4c6
	ldr r2, .L_0200c4ec
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0200c43c:
	.4byte .L_0200c4b0
	.4byte .L_0200c4b0
	.4byte .L_0200c46c
	.4byte .L_0200c46c
	.4byte .L_0200c4b0
	.4byte .L_0200c4b8
	.4byte .L_0200c4b8
	.4byte .L_0200c4b0
	.4byte .L_0200c464
	.4byte .L_0200c464
.L_0200c464:
	movs r0, #102
	bl Func_02004274
	b .L_0200c4c6
.L_0200c46c:
	movs r0, #103
	bl Func_02004274
	b .L_0200c4c6
.L_0200c474:
	ldr r3, .L_0200c4f0
	cmp r2, r3
	bne .L_0200c4c6
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #6
	bhi .L_0200c4c6
	ldr r2, .L_0200c4f4
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0200c494:
	.4byte .L_0200c4b0
	.4byte .L_0200c4b8
	.4byte .L_0200c4b0
	.4byte .L_0200c4b8
	.4byte .L_0200c4b0
	.4byte .L_0200c4c0
	.4byte .L_0200c4b8
.L_0200c4b0:
	movs r0, #100
	bl Func_02004274
	b .L_0200c4c6
.L_0200c4b8:
	movs r0, #101
	bl Func_02004274
	b .L_0200c4c6
.L_0200c4c0:
	movs r0, #102
	bl Func_02004274
.L_0200c4c6:
	movs r0, #0
	add sp, #4
	pop {r5, r6, r7, pc}
.L_0200c4cc:
	.4byte Func_02000aac
.L_0200c4d0:
	.4byte Func_02000a60
.L_0200c4d4:
	.4byte Data_020023c4 + 0x288
.L_0200c4d8:
	.4byte IwramMulQ16
.L_0200c4dc:
	.4byte Data_020053e4
.L_0200c4e0:
	.4byte IwramCopyWords
.L_0200c4e4:
	.4byte Data_0202de00
.L_0200c4e8:
	.4byte 0x000000b2
.L_0200c4ec:
	.4byte .L_0200c43c
.L_0200c4f0:
	.4byte 0x000000b5
.L_0200c4f4:
	.4byte .L_0200c494
	.section .text.x0200c4f8,"ax",%progbits
	.global Func_020044f8
	.thumb_func
Func_020044f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_0200c7d0
	adds r5, r0, #0
	ldr r0, [r6]
	ldr r1, .L_0200c7d4
	sub sp, #8
	str r0, [sp, #4]
	movs r0, #10
	adds r1, #4
	adds r0, #255
	mov r11, r1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c524
	b .L_0200c846
.L_0200c524:
	movs r0, #252
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c532
	b .L_0200c846
.L_0200c532:
	str r5, [r6]
.L_0200c534:
	movs r0, #196
	lsls r0, r0, #4
	bl Runtime_BumpAllocateAlternatePool
	movs r1, #196
	adds r5, r0, #0
	lsls r1, r1, #4
	ldr r3, .L_0200c7d8
	mov lr, r3
	.2byte 0xf800
	movs r2, #14
	adds r4, r5, #0
	mov r10, r2
	mov r8, r2
	movs r6, #0
	adds r4, #112
	movs r7, #28
	adds r0, r5, #0
	adds r1, r5, #0
.L_0200c55a:
	movs r3, #196
	lsls r3, r3, #4
	movs r2, #99
	adds r3, r3, r6
	subs r7, #1
	strb r2, [r0]
	adds r6, #4
	strb r2, [r4]
	adds r0, #112
	strb r2, [r1]
	adds r4, #112
	adds r1, #4
	strb r2, [r5, r3]
	cmp r7, #0
	bge .L_0200c55a
	movs r7, #1
.L_0200c57a:
	movs r0, #0
	mov r9, r0
.L_0200c57e:
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #16
	bgt .L_0200c534
	mov r1, r8
	str r1, [sp, #0]
	bl Random16Far
	lsls r0, r0, #2
	lsrs r0, r0, #16
	mov r6, r10
	ldr r1, [sp, #0]
	cmp r0, #0
	bne .L_0200c5a4
	cmp r7, #1
	beq .L_0200c57e
	cmp r7, #13
	beq .L_0200c57e
.L_0200c5a4:
	mov r2, r8
	lsls r3, r2, #3
	cmp r0, #1
	beq .L_0200c5f8
	cmp r0, #1
	bgt .L_0200c5b6
	cmp r0, #0
	beq .L_0200c5c0
	b .L_0200c6a4
.L_0200c5b6:
	cmp r0, #2
	beq .L_0200c62e
	cmp r0, #3
	beq .L_0200c668
	b .L_0200c6a4
.L_0200c5c0:
	mov r0, r8
	subs r3, r3, r0
	lsls r3, r3, #2
	add r3, r10
	lsls r2, r3, #2
	adds r3, r2, #0
	adds r3, #112
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	adds r3, #224
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	adds r3, #108
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	adds r3, #116
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	mov r1, r8
	adds r1, #1
	b .L_0200c6a4
.L_0200c5f8:
	mov r2, r8
	subs r3, r3, r2
	lsls r3, r3, #2
	add r3, r10
	lsls r2, r3, #2
	subs r3, r2, #4
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	subs r3, #8
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	subs r3, #116
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	adds r3, #108
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	mov r6, r10
	subs r6, #1
	b .L_0200c6a4
.L_0200c62e:
	mov r0, r8
	subs r3, r3, r0
	lsls r3, r3, #2
	add r3, r10
	lsls r2, r3, #2
	adds r3, r2, #4
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	adds r3, #8
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0200c57e
	adds r3, r2, #0
	subs r3, #108
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_0200c656
	b .L_0200c57e
.L_0200c656:
	adds r3, r2, #0
	adds r3, #116
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_0200c662
	b .L_0200c57e
.L_0200c662:
	mov r6, r10
	adds r6, #1
	b .L_0200c6a4
.L_0200c668:
	mov r1, r8
	subs r3, r3, r1
	lsls r3, r3, #2
	add r3, r10
	lsls r2, r3, #2
	adds r3, r2, #0
	subs r3, #112
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_0200c67e
	b .L_0200c57e
.L_0200c67e:
	adds r3, r2, #0
	subs r3, #224
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_0200c68a
	b .L_0200c57e
.L_0200c68a:
	adds r3, r2, #0
	subs r3, #116
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_0200c696
	b .L_0200c57e
.L_0200c696:
	adds r3, r2, #0
	subs r3, #108
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_0200c6a2
	b .L_0200c57e
.L_0200c6a2:
	subs r1, #1
.L_0200c6a4:
	mov r2, r8
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	add r3, r10
	lsls r3, r3, #2
	adds r3, r3, r5
	strb r7, [r3]
	adds r7, #1
	strb r6, [r3, #1]
	strb r1, [r3, #2]
	mov r10, r6
	mov r8, r1
	cmp r7, #14
	bgt .L_0200c6c4
	b .L_0200c57a
.L_0200c6c4:
	movs r3, #14
	mov r10, r3
	mov r8, r3
	movs r7, #1
.L_0200c6cc:
	mov r0, r8
	lsls r0, r0, #3
	mov r1, r8
	subs r3, r0, r1
	lsls r3, r3, #2
	add r3, r10
	lsls r3, r3, #2
	adds r2, r3, r5
	adds r3, #112
	ldrsb r1, [r5, r3]
	adds r4, r7, #1
	mov r12, r2
	mov lr, r0
	adds r2, r7, #0
	cmp r1, r4
	bne .L_0200c6f0
	adds r2, r4, #0
	b .L_0200c70c
.L_0200c6f0:
	subs r3, r7, #1
	mov r0, r12
	cmp r1, r3
	bne .L_0200c6fc
	adds r2, r1, #0
	b .L_0200c70c
.L_0200c6fc:
	subs r0, #112
	movs r1, #0
	ldrsb r1, [r0, r1]
	subs r3, r2, #1
	cmp r1, r3
	bne .L_0200c70c
	adds r2, r1, #0
	b .L_0200c6fc
.L_0200c70c:
	cmp r2, #0
	bne .L_0200c712
	movs r2, #1
.L_0200c712:
	lsls r3, r7, #3
	mov r0, lr
	mov r1, r8
	mov r9, r3
	add r3, r11
	strb r2, [r3, #2]
	subs r3, r0, r1
	lsls r3, r3, #2
	add r3, r10
	lsls r0, r3, #2
	adds r3, r0, #0
	subs r3, #112
	ldrsb r1, [r5, r3]
	adds r2, r7, #0
	cmp r1, r4
	bne .L_0200c736
	adds r2, r4, #0
	b .L_0200c752
.L_0200c736:
	subs r3, r7, #1
	adds r0, r0, r5
	cmp r1, r3
	bne .L_0200c742
	adds r2, r1, #0
	b .L_0200c752
.L_0200c742:
	adds r0, #112
	movs r1, #0
	ldrsb r1, [r0, r1]
	subs r3, r2, #1
	cmp r1, r3
	bne .L_0200c752
	adds r2, r1, #0
	b .L_0200c742
.L_0200c752:
	cmp r2, #0
	bne .L_0200c758
	movs r2, #1
.L_0200c758:
	mov r3, r9
	add r3, r11
	mov r1, r8
	mov r0, lr
	strb r2, [r3, #5]
	subs r3, r0, r1
	lsls r3, r3, #2
	add r3, r10
	lsls r3, r3, #2
	subs r3, #4
	ldrsb r1, [r5, r3]
	adds r2, r7, #0
	cmp r1, r4
	bne .L_0200c778
	adds r2, r4, #0
	b .L_0200c79e
.L_0200c778:
	subs r3, r7, #1
	mov r6, r10
	cmp r1, r3
	bne .L_0200c784
	adds r2, r1, #0
	b .L_0200c79e
.L_0200c784:
	mov r1, r8
	mov r0, lr
	subs r3, r0, r1
	lsls r3, r3, #2
	adds r6, #1
	adds r3, r3, r6
	lsls r3, r3, #2
	ldrsb r1, [r5, r3]
	subs r3, r2, #1
	cmp r1, r3
	bne .L_0200c79e
	adds r2, r1, #0
	b .L_0200c784
.L_0200c79e:
	cmp r2, #0
	bne .L_0200c7a4
	movs r2, #1
.L_0200c7a4:
	mov r3, r9
	add r3, r11
	strb r2, [r3, #3]
	mov r3, r8
	lsls r0, r3, #3
	subs r3, r0, r3
	lsls r3, r3, #2
	add r3, r10
	lsls r3, r3, #2
	adds r3, #4
	ldrsb r1, [r5, r3]
	adds r2, r7, #0
	cmp r1, r4
	bne .L_0200c7c4
	adds r2, r4, #0
	b .L_0200c7f4
.L_0200c7c4:
	subs r3, r7, #1
	mov r6, r10
	cmp r1, r3
	bne .L_0200c7dc
	adds r2, r1, #0
	b .L_0200c7f4
.L_0200c7d0:
	.4byte gRandomState
.L_0200c7d4:
	.4byte Data_020023c4 + 0x288
.L_0200c7d8:
	.4byte IwramClearWords
.L_0200c7dc:
	mov r1, r8
	subs r3, r0, r1
	lsls r3, r3, #2
	subs r6, #1
	adds r3, r3, r6
	lsls r3, r3, #2
	ldrsb r1, [r5, r3]
	subs r3, r2, #1
	cmp r1, r3
	bne .L_0200c7f4
	adds r2, r1, #0
	b .L_0200c7dc
.L_0200c7f4:
	cmp r2, #0
	bne .L_0200c7fa
	movs r2, #1
.L_0200c7fa:
	mov r3, r9
	add r3, r11
	strb r2, [r3, #4]
	strb r7, [r3]
	mov r2, r12
	mov r3, r12
	ldrb r2, [r2, #1]
	lsls r2, r2, #24
	asrs r2, r2, #24
	ldrb r3, [r3, #2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r7, r4, #0
	mov r10, r2
	mov r8, r3
	cmp r7, #14
	bgt .L_0200c81e
	b .L_0200c6cc
.L_0200c81e:
	mov r2, r11
	adds r2, #117
	ldr r1, .L_0200c854
	movs r3, #251
	strb r3, [r2]
	mov r0, r11
	movs r3, #250
	strb r3, [r0, #10]
	movs r3, #1
	str r3, [r1]
	adds r0, r5, #0
	bl MemFree
	movs r0, #252
	lsls r0, r0, #2
	bl GameFlag_SetBit
	ldr r3, .L_0200c858
	ldr r2, [sp, #4]
	str r2, [r3]
.L_0200c846:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c854:
	.4byte Data_020023c4 + 0x288
.L_0200c858:
	.4byte gRandomState
	.section .text.x0200c8a0,"ax",%progbits
	.global Func_020048a0
	.thumb_func
Func_020048a0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200c8fc
	sub sp, #8
	ldr r1, [r3]
	ldr r3, .L_0200c900
	lsls r2, r1, #1
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	cmp r1, #1
	ble .L_0200c8f4
	movs r1, #0
	ldrsb r1, [r2, r1]
	movs r7, #1
	ldrsb r7, [r2, r7]
	movs r2, #132
	lsls r2, r2, #1
	mov r8, r1
	adds r5, r3, r2
	movs r6, #1
.L_0200c8ce:
	ldr r2, [r5, #8]
	ldr r3, [r5, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	mov r1, r8
	adds r0, r1, r2
	adds r1, r7, r3
	adds r2, #1
	adds r3, #1
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #15
	movs r3, #10
	subs r6, #1
	bl Func_02004e0c
	adds r5, #56
	cmp r6, #0
	bge .L_0200c8ce
.L_0200c8f4:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200c8fc:
	.4byte Data_020023c4 + 0x288
.L_0200c900:
	.4byte Data_020053fc
	.section .text.x0200c904,"ax",%progbits
	.global Func_02004904
	.thumb_func
Func_02004904:
	push {lr}
	ldr r2, .L_0200c958
	ldr r1, [r2]
	lsls r3, r1, #3
	adds r3, r3, r2
	adds r3, #4
	ldrb r2, [r3, #2]
	adds r1, #1
	cmp r2, #251
	beq .L_0200c91c
	cmp r2, r1
	bne .L_0200c922
.L_0200c91c:
	movs r0, #128
	lsls r0, r0, #7
	b .L_0200c954
.L_0200c922:
	ldrb r2, [r3, #3]
	cmp r2, #251
	beq .L_0200c92c
	cmp r2, r1
	bne .L_0200c932
.L_0200c92c:
	movs r0, #128
	lsls r0, r0, #8
	b .L_0200c954
.L_0200c932:
	ldrb r2, [r3, #4]
	cmp r2, #251
	beq .L_0200c93c
	cmp r2, r1
	bne .L_0200c940
.L_0200c93c:
	movs r0, #0
	b .L_0200c954
.L_0200c940:
	ldrb r2, [r3, #5]
	cmp r2, #251
	beq .L_0200c94a
	cmp r2, r1
	bne .L_0200c950
.L_0200c94a:
	movs r0, #192
	lsls r0, r0, #8
	b .L_0200c954
.L_0200c950:
	movs r0, #1
	negs r0, r0
.L_0200c954:
	pop {pc}
	.2byte 0x0000
.L_0200c958:
	.4byte Data_020023c4 + 0x288
	.section .text.x0200c95c,"ax",%progbits
	.global Func_0200495c
	.thumb_func
Func_0200495c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	mov r12, r1
	ldr r1, [r3, #32]
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #3
	movs r0, #132
	adds r3, r1, r3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r3, #48]
	adds r2, r1, #0
	adds r3, r1, #0
	adds r2, #236
	adds r3, #244
	ldr r0, [r2]
	ldr r3, [r3]
	subs r3, r3, r0
	asrs r7, r3, #20
	adds r3, r1, #0
	adds r3, #248
	adds r1, #240
	ldr r2, [r3]
	ldr r3, [r1]
	asrs r0, r0, #20
	subs r2, r2, r3
	asrs r3, r3, #20
	lsls r3, r3, #7
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r4, r4, r3
	asrs r5, r2, #20
	movs r1, #0
	ldr r6, [r4]
	cmp r1, r5
	bge .L_0200c9e0
.L_0200c9a8:
	lsls r2, r1, #16
	lsrs r3, r2, #7
	movs r0, #0
	adds r1, r4, r3
	cmp r0, r7
	bge .L_0200c9d2
.L_0200c9b4:
	ldrb r3, [r1, #2]
	cmp r3, #0
	beq .L_0200c9c0
	cmp r3, r12
	beq .L_0200c9c0
	str r6, [r1]
.L_0200c9c0:
	lsls r3, r0, #16
	movs r0, #128
	lsls r0, r0, #9
	adds r3, r3, r0
	asrs r0, r3, #16
	lsrs r3, r3, #16
	adds r1, #4
	cmp r3, r7
	blt .L_0200c9b4
.L_0200c9d2:
	movs r1, #128
	lsls r1, r1, #9
	adds r3, r2, r1
	asrs r1, r3, #16
	lsrs r3, r3, #16
	cmp r3, r5
	blt .L_0200c9a8
.L_0200c9e0:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200c9e4,"ax",%progbits
	.global Func_020049e4
	.thumb_func
Func_020049e4:
	push {r5, r6, lr}
	ldr r2, .L_0200ca50
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r2, r0
	ldrb r3, [r3]
	cmp r3, #2
	bne .L_0200ca4e
	movs r3, #192
	movs r1, #133
	lsls r3, r3, #18
	lsls r1, r1, #2
	ldr r6, [r3, #108]
	ldr r5, [r3, #32]
	adds r3, r2, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	bge .L_0200ca14
	ldr r2, .L_0200ca54
	adds r3, r3, r2
.L_0200ca14:
	ldr r2, [r0, #16]
	asrs r1, r3, #20
	ldr r3, [r0, #12]
	subs r0, r2, r3
	movs r2, #254
	lsls r2, r2, #7
	adds r2, #255
	adds r3, r0, r2
	cmp r3, #0
	bge .L_0200ca2c
	ldr r2, .L_0200ca58
	adds r3, r0, r2
.L_0200ca2c:
	movs r0, #212
	lsls r0, r0, #1
	asrs r3, r3, #20
	adds r2, r5, r0
	ldr r2, [r2]
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r2, [r2, #2]
	subs r3, r2, #1
	cmp r3, #229
	bhi .L_0200ca4e
	movs r1, #170
	lsls r1, r1, #1
	adds r3, r6, r1
	strh r2, [r3]
.L_0200ca4e:
	pop {r5, r6, pc}
.L_0200ca50:
	.4byte gPartyState
.L_0200ca54:
	.4byte 0x000fffff
.L_0200ca58:
	.4byte 0x00107ffe
	.section .text.x0200ca5c,"ax",%progbits
	.global Func_02004a5c
	.thumb_func
Func_02004a5c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r1, #0
	mov r8, r2
	adds r6, r3, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #212
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r4, r0, #0
	ldr r0, [r3]
	adds r3, r5, #0
	cmp r5, #0
	bge .L_0200ca86
	ldr r2, .L_0200cae4
	adds r3, r5, r2
.L_0200ca86:
	asrs r7, r3, #20
	mov r3, r8
	subs r2, r6, r3
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	adds r1, r2, r3
	cmp r1, #0
	bge .L_0200ca9c
	ldr r3, .L_0200cae8
	adds r1, r2, r3
.L_0200ca9c:
	asrs r1, r1, #20
	lsls r3, r1, #7
	adds r3, r7, r3
	lsls r3, r3, #2
	adds r0, r0, r3
	movs r3, #255
	strb r3, [r0, #2]
	ldr r0, .L_0200caec
	movs r3, #128
	ands r5, r0
	ands r6, r0
	lsls r3, r3, #12
	adds r2, r5, r3
	lsls r1, r1, #20
	adds r3, r6, r3
	str r2, [r4, #8]
	str r3, [r4, #16]
	adds r2, r4, #0
	subs r3, r3, r1
	str r3, [r4, #12]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, r4, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r4, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200cae4:
	.4byte 0x000fffff
.L_0200cae8:
	.4byte 0x00107ffe
.L_0200caec:
	.4byte 0xfff00000
	.section .text.x0200caf0,"ax",%progbits
	.global Func_02004af0
	.thumb_func
Func_02004af0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	ldr r3, .L_0200ccf0
	str r1, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #72]
	str r3, [sp, #8]
	bl Func_02004e5c
	movs r0, #0
	bl Func_02004fcc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	adds r0, r6, #0
	bl Object_GetById
	ldr r2, [r5, #12]
	adds r7, r0, #0
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	adds r0, r6, #0
	bl Func_02004a5c
	ldr r1, .L_0200ccf4
	adds r0, r7, #0
	bl Func_02004dac
	movs r2, #15
	mov r11, r2
.L_0200cb54:
	ldr r2, [r5, #12]
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	movs r0, #255
	bl Engine_ObjectCreate
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200cbcc
	ldr r1, .L_0200ccf8
	bl Func_02004dac
	bl Random16Far
	mov r10, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #6
	lsrs r2, r0, #1
	adds r2, r2, r3
	str r0, [sp, #4]
	mov r0, r10
	mov r9, r2
	bl Math_Cosine
	ldr r2, .L_0200ccfc
	lsls r6, r6, #3
	adds r1, r0, #0
	mov r8, r2
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	str r0, [r7, #40]
	mov r0, r10
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	movs r3, #0
	str r3, [r7, #52]
	mov r3, r9
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #8
	str r0, [r7, #36]
	str r3, [r7, #68]
.L_0200cbcc:
	movs r2, #1
	negs r2, r2
	add r11, r2
	mov r3, r11
	cmp r3, #0
	bge .L_0200cb54
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #78
	bl Func_02005014
	movs r0, #217
	bl Func_02005014
	movs r2, #230
	movs r0, #128
	movs r1, #128
	lsls r2, r2, #8
	lsls r0, r0, #10
	lsls r1, r1, #10
	adds r2, #102
	bl Func_02004e24
	ldr r3, .L_0200ccf0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #2
	ldr r0, [r3]
	adds r1, #255
	bl Func_02004f34
	movs r1, #40
	adds r0, r5, #0
	bl Func_02004d9c
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #204
	bl Func_02005014
	adds r2, r5, #0
	movs r3, #3
	adds r2, #85
	strb r3, [r2]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	movs r6, #0
	str r3, [r5, #72]
	b .L_0200cc36
.L_0200cc34:
	adds r6, #1
.L_0200cc36:
	cmp r6, #179
	bgt .L_0200cc48
	movs r0, #1
	bl WaitFrames
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	cmp r2, r3
	bgt .L_0200cc34
.L_0200cc48:
	movs r0, #188
	bl Func_02005014
	ldr r3, [sp, #8]
	ldr r6, .L_0200ccf0
	str r3, [r5, #72]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	movs r1, #0
	bl Func_02004f34
	adds r0, r5, #0
	movs r1, #38
	bl Func_02004d9c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_02004e24
	movs r0, #10
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004e24
	movs r0, #20
	bl WaitFrames
	ldr r2, [r5, #16]
	movs r3, #1
	ldr r1, [r5, #12]
	ldr r0, [r5, #8]
	bl Motion_CamBounds
	bl Func_02004f54
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #1
	adds r0, r5, #0
	bl Func_02004d9c
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r0, #20
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r2, r6, r3
	movs r3, #0
	strb r3, [r2]
	bl Func_02004e64
	ldr r0, [sp, #12]
	bl GameFlag_SetBit
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ccf0:
	.4byte gPartyState
.L_0200ccf4:
	.4byte Data_020062d4
.L_0200ccf8:
	.4byte Data_02006290
.L_0200ccfc:
	.4byte IwramMulQ16
	.section .bss,"aw",%nobits
	.global Data_0200646c
Data_0200646c:
	.space 0x00000080
	.global Data_020064ec
Data_020064ec:
	.space 0x00000004
	.global Data_020064f0
Data_020064f0:
	.space 0x00000004
	.global Data_020064f4
Data_020064f4:
	.space 0x00000004
	.global Data_020064f8
Data_020064f8:
