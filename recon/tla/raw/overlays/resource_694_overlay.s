.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.balign 4
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, r6, lr}
	adds r4, r1, #0
	movs r1, #192
	lsls r1, r1, #18
	adds r1, #128
	ldr r6, [r1]
	ldr r1, .L_020080dc
	adds r5, r0, #0
	str r5, [r1]
	ldr r1, .L_020080e0
	str r4, [r1]
	ldr r1, .L_020080e4
	str r2, [r1]
	ldr r2, .L_020080e8
	str r3, [r2]
	movs r2, #255
	ldrh r3, [r5]
	b .L_02008082
.L_0200805c:
	ldrh r0, [r4]
	adds r4, #2
	ldrh r2, [r4]
	adds r4, #2
	ldrh r1, [r5]
	ldrh r3, [r4]
	adds r5, #2
	adds r4, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	movs r2, #160
	lsls r2, r2, #19
	lsls r1, r1, #1
	orrs r3, r0
	adds r1, r1, r2
	strh r3, [r1]
	ldrh r3, [r5]
	movs r2, #255
.L_02008082:
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_02008090
	ldrh r3, [r4]
	cmp r3, r2
	bne .L_0200805c
.L_02008090:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r0, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r0, r0, #19
	adds r1, r6, #0
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #224
	lsls r2, r2, #1
	adds r1, r6, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, .L_020080ec
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_020041cc
	ldr r3, .L_020080f0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_020080da
	bl Func_02000328
.L_020080da:
	pop {r5, r6, pc}
.L_020080dc:
	.4byte gData_02006010
.L_020080e0:
	.4byte gData_02006014
.L_020080e4:
	.4byte Data_02006018
.L_020080e8:
	.4byte gData_02006004
.L_020080ec:
	.4byte 0x05000200
.L_020080f0:
	.4byte gAdventure
	.section .text.x020081d4,"ax",%progbits
	.balign 4
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	push {lr}
	ldr r1, .L_020081ec
	ldrh r3, [r0]
	movs r2, #0
	cmp r3, r1
	beq .L_020081f0
.L_020081e0:
	adds r0, #2
	ldrh r3, [r0]
	adds r2, #1
	cmp r3, r1
	bne .L_020081e0
	b .L_020081f0
.L_020081ec:
	.4byte 0x0000ffff
.L_020081f0:
	subs r2, #1
	adds r0, r2, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x020081f8,"ax",%progbits
	.global Func_020001f8
	.thumb_func
Func_020001f8:
	push {r5, r6, r7, lr}
	ldr r1, .L_020082a8
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_02008232
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02008232
	ldr r0, .L_020082ac
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_02008232
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_02008232:
	cmp r4, #0
	bne .L_02008238
	b .L_02008324
.L_02008238:
	ldr r3, .L_020082b0
	ldr r6, .L_020082b4
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_02008286
	ldr r3, .L_020082b8
	ldr r2, .L_020082bc
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_02008250:
	mov r3, lr
	ldrh r2, [r3]
	ldrh r3, [r6]
	movs r0, #160
	muls r3, r2
	adds r3, r3, r5
	lsls r3, r3, #1
	ldrh r3, [r3, r7]
	lsls r0, r0, #19
	lsls r3, r3, #1
	adds r4, r3, r0
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	adds r1, #2
	ldrh r3, [r1]
	adds r1, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	adds r5, #1
	mov r2, r12
	ldrh r3, [r2]
	cmp r5, r3
	bcc .L_02008250
.L_02008286:
	ldr r3, .L_020082b4
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_020082c0
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_020082c4
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_020082cc
	ldr r3, .L_020082c8
	b .L_020082ce
.L_020082a8:
	.4byte gData_02006008
.L_020082ac:
	.4byte gData_0200600c
.L_020082b0:
	.4byte Data_02006018
.L_020082b4:
	.4byte gData_0200601c
.L_020082b8:
	.4byte gData_02006004
.L_020082bc:
	.4byte gData_02006020
.L_020082c0:
	.4byte gData_02006010
.L_020082c4:
	.4byte gData_02006000
.L_020082c8:
	.4byte gData_02006014
.L_020082cc:
	ldr r3, .L_02008314
.L_020082ce:
	lsls r2, r2, #1
	ldr r3, [r3]
	adds r1, r3, r2
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	ldrh r3, [r1, #2]
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	ldr r1, .L_02008318
	ldr r2, .L_0200830c
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_0200831c
	ldr r2, .L_02008320
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_02008324
	ldr r3, .L_02008310
	strh r3, [r1]
	b .L_02008324
	.2byte 0x0000
.L_0200830c:
	.4byte 0x00000001
.L_02008310:
	.4byte 0x00000000
.L_02008314:
	.4byte Data_02006018
.L_02008318:
	.4byte gData_02006000
.L_0200831c:
	.4byte gData_02006020
.L_02008320:
	.4byte gData_0200601c
.L_02008324:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020083f6,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200842e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008658,"ax",%progbits
	.balign 4
	.global Func_02000658
	.thumb_func
Func_02000658:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	adds r6, r1, #0
	adds r7, r6, #0
	mov r5, r8
	adds r7, #8
	adds r5, #8
	mov r10, r2
	adds r0, r7, #0
	movs r2, #0
	adds r1, r5, #0
	mov r9, r2
	bl Func_02000620
	cmp r0, r10
	bge .L_020086c0
	mov r2, r8
	ldr r3, [r2, #16]
	ldr r0, [r6, #16]
	ldr r1, [r7]
	subs r0, r0, r3
	ldr r3, [r5]
	movs r5, #128
	subs r1, r1, r3
	bl ArcTan2
	lsls r0, r0, #16
	lsrs r0, r0, #16
	ldr r3, .L_020086cc
	lsls r5, r5, #5
	adds r1, r0, r5
	mov r5, r8
	ldrh r2, [r5, #6]
	adds r4, r0, r3
	movs r3, #240
	lsls r3, r3, #8
	ands r4, r3
	ands r1, r3
	ands r0, r3
	ands r3, r2
	cmp r0, r3
	beq .L_020086bc
	cmp r1, r3
	beq .L_020086bc
	cmp r4, r3
	bne .L_020086c0
.L_020086bc:
	movs r2, #1
	mov r9, r2
.L_020086c0:
	mov r0, r9
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_020086cc:
	.4byte 0xfffff000
	.section .text.x020095ac,"ax",%progbits
	.balign 4
	.global Func_020015ac
	.thumb_func
Func_020015ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #17
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r1, #224
	lsls r1, r1, #14
	ldr r2, .L_0200971c
	movs r0, #17
	bl Func_020040d4
	movs r0, #1
	bl WaitFrames
	add r2, sp, #28
	movs r3, #2
	str r3, [r2]
	movs r0, #188
	mov r9, r2
	bl Func_020042b4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_0200401c
	movs r3, #0
	mov r10, r3
.L_020095fe:
	mov r4, r10
	lsls r6, r4, #12
	adds r0, r6, #0
	bl Math_Cosine
	movs r3, #0
	add r5, sp, #16
	str r3, [r5, #4]
	str r0, [r5]
	adds r0, r6, #0
	bl Math_Sine
	ldr r6, [r5]
	mov r8, r0
	str r0, [r5, #8]
	movs r1, #3
	adds r0, r6, #0
	bl __divsi3
	ldr r3, [r5, #4]
	movs r4, #200
	lsls r4, r4, #5
	adds r6, r6, r0
	adds r4, #153
	str r6, [r5]
	adds r3, r3, r4
	ldr r2, [r7, #16]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r3, [sp, #0]
	mov r3, r8
	str r3, [sp, #4]
	movs r4, #128
	mov r3, r9
	lsls r4, r4, #10
	str r3, [sp, #12]
	adds r3, r6, #0
	mov r8, r4
	str r4, [sp, #8]
	bl Func_02000430
	movs r4, #1
	add r10, r4
	mov r2, r10
	cmp r2, #16
	bls .L_020095fe
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #17
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #192
	movs r2, #192
	movs r0, #17
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r3, #192
	lsls r3, r3, #12
	movs r2, #170
	lsls r2, r2, #2
	str r3, [r7, #40]
	movs r1, #40
	movs r0, #17
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #10
	bl FieldEvent_WaitFrames
	ldr r3, .L_02009720
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r3, r4
	ldr r0, [r5]
	bl Object_GetById
	ldr r2, [r7, #8]
	ldr r3, [r0, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020096e0
	ldr r2, [r7, #16]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020096e0
	ldr r0, [r5]
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #128
	ldr r0, [r5]
	mov r1, r8
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r2, #170
	ldr r0, [r5]
	movs r1, #68
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndCommit
.L_020096e0:
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_0200401c
	ldr r3, .L_02009720
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #17
	movs r2, #0
	bl Object_TurnToFaceEachOther
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #231
	bl GameFlag_SetBit
	bl Func_02004074
	add sp, #68
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200971c:
	.4byte 0x02ba0000
.L_02009720:
	.4byte gAdventure
	.section .text.x0200ba8c,"ax",%progbits
	.ifdef TLA_EDITION_ES
	.global Func_02003a8c
	.thumb_func
Func_02003a8c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	movs r0, #13
	bl Func_02001c44
	movs r0, #16
	bl Func_02001c44
	movs r0, #17
	bl Func_02001c44
	movs r0, #19
	bl Func_02001c44
	movs r0, #20
	bl Func_02001c44
	movs r0, #14
	bl Object_GetById
	movs r1, #1
	bl Object_SetPartAttribute
	movs r0, #15
	bl Object_GetById
	movs r1, #2
	bl Object_SetPartAttribute
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #9
	bl Object_GetById
	movs r1, #3
	bl Object_SetPartAttribute
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #229
	bl GameFlag_Test
	cmp r0, #0
	bne .L_es_0200baf8
	b .L_es_0200bcaa
.L_es_0200baf8:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #229
	bl GameFlag_ClearBit
	ldr r2, .L_es_0200bcac
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #18
	bne .L_es_0200bb46
	ldr r3, .L_es_0200bcb0
	ldr r2, [r2, #16]
	ldr r3, [r3]
	subs r5, r2, r3
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	cmp r5, #0
	ble .L_es_0200bbb4
	movs r2, #156
	lsls r2, r2, #7
	adds r2, #31
	cmp r5, r2
	bgt .L_es_0200bb72
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #135
	cmp r5, r3
	bgt .L_es_0200bb84
	b .L_es_0200bb8c
.L_es_0200bb46:
	cmp r3, #19
	bne .L_es_0200bbd4
	ldr r3, .L_es_0200bcb0
	ldr r2, [r2, #16]
	ldr r3, [r3]
	subs r5, r2, r3
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	cmp r5, #0
	ble .L_es_0200bbb4
	movs r1, #156
	lsls r1, r1, #7
	adds r1, #31
	cmp r5, r1
	ble .L_es_0200bb7a
.L_es_0200bb72:
	movs r0, #93
	bl Func_020042b4
	b .L_es_0200bb92
.L_es_0200bb7a:
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #135
	cmp r5, r2
	ble .L_es_0200bb8c
.L_es_0200bb84:
	movs r0, #92
	bl Func_020042b4
	b .L_es_0200bb92
.L_es_0200bb8c:
	movs r0, #91
	bl Func_020042b4
.L_es_0200bb92:
	movs r0, #20
	bl FieldEvent_WaitFrames
	ldr r0, .L_es_0200bcb4
	bl Func_0200412c
	adds r0, r5, #0
	movs r1, #5
	bl Func_0200403c
	movs r0, #14
	movs r1, #0
	bl Func_02004144
	bl AudioCommand_WaitForStateByteClear
	b .L_es_0200bbce
.L_es_0200bbb4:
	cmp r5, #0
	bge .L_es_0200bbce
	ldr r0, .L_es_0200bcb8
	bl Func_0200412c
	negs r0, r5
	movs r1, #5
	bl Func_0200403c
	movs r0, #14
	movs r1, #0
	bl Func_02004144
.L_es_0200bbce:
	bl Func_02004074
	b .L_es_0200bcaa
.L_es_0200bbd4:
	cmp r3, #20
	bne .L_es_0200bcaa
	movs r3, #166
	lsls r3, r3, #1
	adds r7, r2, r3
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r3, #0
	ldrsb r3, [r7, r3]
	movs r5, #1
	negs r5, r5
	cmp r3, r5
	bne .L_es_0200bc0a
	movs r0, #1
	bl Func_02001938
	b .L_es_0200bca6
.L_es_0200bc0a:
	movs r1, #2
	negs r1, r1
	cmp r3, r1
	bne .L_es_0200bc22
	ldr r0, .L_es_0200bcbc
	bl Func_0200412c
	movs r0, #18
	movs r1, #0
	bl Func_02004144
	b .L_es_0200bca6
.L_es_0200bc22:
	ldr r0, .L_es_0200bcc0
	bl Func_0200412c
	movs r0, #18
	movs r1, #0
	bl Func_02004144
	movs r3, #0
	ldrsb r3, [r7, r3]
	cmp r3, r5
	beq .L_es_0200bc8c
	adds r6, r7, #0
.L_es_0200bc3a:
	cmp r6, r7
	bne .L_es_0200bc46
	ldr r0, .L_es_0200bcc4
	bl Func_0200412c
	b .L_es_0200bc4c
.L_es_0200bc46:
	ldr r0, .L_es_0200bcc8
	bl Func_0200412c
.L_es_0200bc4c:
	movs r0, #0
	ldrsb r0, [r6, r0]
	bl Func_02001a60
	movs r1, #2
	adds r5, r0, #0
	bl Func_0200403c
	movs r0, #18
	movs r1, #0
	bl Func_02004144
	adds r0, r5, #0
	movs r1, #3
	bl Func_020041fc
	movs r1, #0
	adds r0, r5, #0
	bl PartyInventory_GiveItem
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	adds r6, #1
	bl Func_0200415c
	movs r3, #0
	ldrsb r3, [r6, r3]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_es_0200bc3a
.L_es_0200bc8c:
	ldr r3, .L_es_0200bcac
	movs r1, #166
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #254
	ldr r0, .L_es_0200bccc
	strb r2, [r3]
	bl Func_0200412c
	movs r0, #18
	movs r1, #0
	bl Func_02004144
.L_es_0200bca6:
	bl Func_02004074
.L_es_0200bcaa:
	pop {r5, r6, r7, pc}
.L_es_0200bcac:
	.4byte gAdventure
.L_es_0200bcb0:
	.4byte gSceneState
.L_es_0200bcb4:
	.4byte 0x0000155a
.L_es_0200bcb8:
	.4byte 0x0000155b
.L_es_0200bcbc:
	.4byte MsgContigo04
.L_es_0200bcc0:
	.4byte MsgContigo05
.L_es_0200bcc4:
	.4byte MsgContigo11
.L_es_0200bcc8:
	.4byte MsgContigo14
.L_es_0200bccc:
	.4byte 0x00001580
	.endif
	.section .text.x0200bcd8,"ax",%progbits
	.global Func_02003cd8
	.thumb_func
Func_02003cd8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	ldr r2, [r7, #104]
	adds r6, r7, #0
	adds r6, #99
	mov r8, r2
	ldrb r2, [r6]
	movs r3, #1
	ands r3, r2
	sub sp, #24
	cmp r3, #0
	beq .L_0200bd12
	ldrb r0, [r6]
	movs r1, #6
	lsrs r0, r0, #1
	bl __umodsi3
	adds r1, r0, #0
	lsls r1, r1, #24
	lsrs r1, r1, #24
	adds r0, r7, #0
	bl Animation_ApplyChildValues
.L_0200bd12:
	adds r3, r7, #0
	adds r3, #98
	ldrb r5, [r3]
	cmp r5, #0
	bne .L_0200bd50
	ldrb r2, [r6]
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	bne .L_0200bd86
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #86
	bl Func_020042b4
	mov r1, r8
	adds r1, #166
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r2, r8
	lsls r3, r3, #1
	adds r3, #160
	strh r5, [r2, r3]
	ldr r2, .L_0200bd4c
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	b .L_0200bd86
	.2byte 0x0000
.L_0200bd4c:
	.4byte 0x00000001
.L_0200bd50:
	cmp r5, #1
	bne .L_0200bd86
	mov r3, r8
	adds r3, #160
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #31
	ble .L_0200bd86
	mov r3, r8
	adds r3, #162
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #31
	ble .L_0200bd86
	adds r0, r7, #0
	movs r1, #0
	bl Animation_ApplyChildValues
	mov r3, r8
	adds r3, #164
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	movs r3, #0
	str r3, [r7, #108]
	b .L_0200be40
.L_0200bd86:
	ldrb r3, [r6]
	movs r2, #1
	adds r3, #1
	strb r3, [r6]
	movs r3, #0
	str r3, [sp, #0]
	mov r6, r8
	mov r11, r2
	adds r6, #160
.L_0200bd98:
	movs r3, #0
	ldrsh r0, [r6, r3]
	lsls r0, r0, #10
	bl Math_Sine
	str r0, [sp, #4]
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	blt .L_0200be2c
	cmp r3, #31
	bgt .L_0200be2c
	ldr r3, [r7, #8]
	add r5, sp, #12
	str r3, [r5]
	adds r0, r5, #0
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r7, #12]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	bl Func_0200421c
	ldr r2, [r5]
	movs r3, #0
	str r2, [sp, #8]
	mov r10, r3
	ldr r5, [r5, #8]
	mov r9, r5
	ldr r5, [sp, #0]
	add r5, r8
.L_0200bddc:
	ldr r2, [sp, #8]
	mov r3, r9
	str r3, [r5, #16]
	str r2, [r5, #12]
	ldr r2, [sp, #4]
	mov r3, r10
	str r2, [r5, #20]
	str r2, [r5, #24]
	cmp r3, #0
	bne .L_0200bdfc
	adds r0, r7, #0
	bl Func_0200425c
	subs r0, #1
	strh r0, [r5, #30]
	b .L_0200be14
.L_0200bdfc:
	adds r0, r7, #0
	bl Func_0200425c
	ldr r3, [r5, #16]
	ldr r2, .L_0200be50
	adds r0, #1
	adds r3, r3, r2
	str r3, [r5, #16]
	ldr r3, [r5, #24]
	strh r0, [r5, #30]
	negs r3, r3
	str r3, [r5, #24]
.L_0200be14:
	adds r0, r5, #0
	bl Func_0200424c
	movs r3, #1
	add r10, r3
	mov r2, r10
	adds r5, #40
	cmp r2, #1
	ble .L_0200bddc
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
.L_0200be2c:
	ldr r3, [sp, #0]
	movs r2, #1
	negs r2, r2
	adds r3, #80
	add r11, r2
	str r3, [sp, #0]
	mov r3, r11
	adds r6, #2
	cmp r3, #0
	bge .L_0200bd98
.L_0200be40:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200be50:
	.4byte 0xffff0000
	.section .text.x0200be54,"ax",%progbits
	.global Func_02003e54
	.thumb_func
Func_02003e54:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r7, r1, #0
	sub sp, #4
	bl Resource_FindFreeEntry
	movs r1, #164
	adds r1, r1, r7
	mov r8, r1
	ldr r2, .L_0200beb0
	mov r3, r8
	strh r0, [r3]
	movs r1, #128
	lsls r0, r0, #16
	mov r10, r2
	lsls r1, r1, #1
	ldr r2, .L_0200beb4
	asrs r0, r0, #16
	bl VramCache_Load
	adds r3, r7, #0
	movs r2, #186
	movs r5, #0
	adds r3, #166
	lsls r2, r2, #2
	strh r5, [r3]
	adds r2, #255
	subs r3, #6
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #6
	str r6, [r3]
	adds r3, r6, #0
	mov r0, r10
	adds r3, #98
	strb r0, [r3]
	adds r3, #1
	b .L_0200beb8
	.2byte 0x0000
.L_0200beb0:
	.4byte 0x00000000
.L_0200beb4:
	.4byte gData_020042bc
.L_0200beb8:
	strb r0, [r3]
	ldr r3, .L_0200bf48
	mov r0, r8
	str r3, [r6, #108]
	movs r1, #0
	ldrsh r3, [r0, r1]
	ldr r2, .L_0200bf4c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	str r7, [r6, #104]
	lsrs r3, r3, #5
	mov r11, r3
	mov r9, r5
	mov r10, r5
.L_0200bed6:
	movs r1, #1
	mov r2, r10
	mov r8, r1
	adds r5, r2, r7
.L_0200bede:
	mov r3, r11
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #16
	movs r2, #16
	ldr r3, .L_0200bf50
	bl Func_02004244
	ldrb r3, [r5, #5]
	movs r0, #33
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r5, #9]
	adds r0, r6, #0
	bl Func_02004254
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r5, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #1
	lsls r0, r0, #2
	negs r2, r2
	orrs r3, r0
	add r8, r2
	strb r3, [r5, #9]
	mov r3, r8
	adds r5, #40
	cmp r3, #0
	bge .L_0200bede
	movs r1, #1
	add r9, r1
	movs r0, #80
	mov r2, r9
	add r10, r0
	cmp r2, #1
	ble .L_0200bed6
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bf48:
	.4byte Func_02003cd8
.L_0200bf4c:
	.4byte gResourceTableEntries
.L_0200bf50:
	.4byte 0x80004000
	.section .bss,"aw",%nobits
	.balign 16
	.global gData_02006000
gData_02006000:
	.space 0x00000004
	.global gData_02006004
gData_02006004:
	.space 0x00000004
	.global gData_02006008
gData_02006008:
	.space 0x00000004
	.global gData_0200600c
gData_0200600c:
	.space 0x00000004
	.global gData_02006010
gData_02006010:
	.space 0x00000004
	.global gData_02006014
gData_02006014:
	.space 0x00000004
	.global Data_02006018
Data_02006018:
	.space 0x00000004
	.global gData_0200601c
gData_0200601c:
	.space 0x00000004
	.global gData_02006020
gData_02006020:
	.space 0x00000010
	.global gData_02006030
gData_02006030:
