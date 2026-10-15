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
	bl Func_020012ec
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
	.4byte gData_02001a24
.L_020080e0:
	.4byte gData_02001a28
.L_020080e4:
	.4byte Data_02001a2c
.L_020080e8:
	.4byte gData_02001a18
.L_020080ec:
	.4byte 0x05000200
.L_020080f0:
	.4byte gPartyState
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
	.4byte gData_02001a1c
.L_020082ac:
	.4byte gData_02001a20
.L_020082b0:
	.4byte Data_02001a2c
.L_020082b4:
	.4byte gData_02001a30
.L_020082b8:
	.4byte gData_02001a18
.L_020082bc:
	.4byte gData_02001a34
.L_020082c0:
	.4byte gData_02001a24
.L_020082c4:
	.4byte gData_02001a14
.L_020082c8:
	.4byte gData_02001a28
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
	.4byte Data_02001a2c
.L_02008318:
	.4byte gData_02001a14
.L_0200831c:
	.4byte gData_02001a34
.L_02008320:
	.4byte gData_02001a30
.L_02008324:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008580,"ax",%progbits
	.balign 4
	.global Func_02000580
	.thumb_func
Func_02000580:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	mov r12, r0
	ldr r0, [r3]
	movs r3, #224
	lsls r3, r3, #4
	movs r4, #224
	adds r1, r0, r3
	mov lr, r4
	movs r7, #0
.L_0200859c:
	ldrh r3, [r0]
	movs r5, #31
	lsrs r2, r3, #10
	ldr r6, .L_020085f8
	ands r2, r5
	movs r4, #248
	lsls r2, r2, #10
	lsls r4, r4, #7
	adds r0, #2
	mov r8, r6
	cmp r2, r4
	bls .L_020085b6
	adds r2, r4, #0
.L_020085b6:
	strh r2, [r1]
	lsrs r2, r3, #5
	ands r2, r5
	mov r6, r12
	muls r6, r2
	adds r2, r6, #0
	mov r6, r8
	ands r2, r6
	adds r1, #2
	cmp r2, r4
	bls .L_020085ce
	adds r2, r4, #0
.L_020085ce:
	strh r2, [r1]
	adds r2, r5, #0
	ands r2, r3
	mov r3, r12
	muls r3, r2
	mov r6, r8
	adds r2, r3, #0
	ands r2, r6
	adds r1, #2
	cmp r2, r4
	bls .L_020085e6
	adds r2, r4, #0
.L_020085e6:
	adds r7, #1
	strh r2, [r1]
	adds r1, #2
	cmp r7, lr
	bcc .L_0200859c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020085f8:
	.4byte 0xfffffc00
	.section .text.x020085fc,"ax",%progbits
	.global Func_020005fc
	.thumb_func
Func_020005fc:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_02001254
	movs r0, #0
	bl Func_02001324
	movs r5, #8
.L_02008610:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008622
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008622:
	adds r5, #1
	cmp r5, #63
	bls .L_02008610
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r5, [r6, r3]
	movs r0, #158
	subs r5, #1
	bl Func_02001344
	lsrs r5, r5, #1
	ldr r0, .L_02008694
	lsls r5, r5, #3
	adds r3, r5, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r5]
	bl Func_0200122c
	ldr r5, .L_02008698
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r2, #4
	movs r1, #0
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #6
	bl Battle_WaitMode0
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_020012cc
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_0200125c
	pop {r5, r6, pc}
.L_02008694:
	.4byte gData_020019ec
.L_02008698:
	.4byte gPartyState
	.section .text.x0200869c,"ax",%progbits
	.global Func_0200069c
	.thumb_func
Func_0200069c:
	push {r5, lr}
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
	movs r0, #0
	bl Func_0200131c
	ldr r5, .L_020087b4
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087b8
	cmp r2, r3
	bne .L_020086dc
	bl Func_020008c8
	ldr r0, .L_020087bc
	ldr r1, .L_020087c0
	ldr r2, .L_020087c4
	ldr r3, .L_020087c8
	bl Func_02000038
	b .L_02008754
.L_020086dc:
	ldr r3, .L_020087cc
	cmp r2, r3
	bne .L_02008712
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_02008754
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200129c
	movs r0, #141
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02008754
.L_02008712:
	ldr r3, .L_020087d0
	cmp r2, r3
	bne .L_02008766
	movs r0, #85
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200872c
	movs r0, #165
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008736
.L_0200872c:
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008754
.L_02008736:
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008754
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200129c
	ldr r1, .L_020087d4
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
.L_02008754:
	ldr r3, .L_020087b4
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087d0
	cmp r2, r3
	beq .L_0200877e
.L_02008766:
	ldr r3, .L_020087b4
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087cc
	cmp r2, r3
	bne .L_020087b0
	ldr r3, .L_020087d0
	cmp r2, r3
	bne .L_02008794
.L_0200877e:
	ldr r3, .L_020087b4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	subs r3, #3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	b .L_02008796
.L_02008794:
	movs r3, #3
.L_02008796:
	movs r0, #128
	lsls r3, r3, #7
	lsls r0, r0, #3
	subs r0, r0, r3
	bl Func_02000580
	bl Func_0200133c
	movs r0, #1
	bl Func_020012fc
	bl Event_WaitTransitionFrames
.L_020087b0:
	movs r0, #0
	pop {r5, pc}
.L_020087b4:
	.4byte gPartyState
.L_020087b8:
	.4byte 0x00000088
.L_020087bc:
	.4byte gData_0200134c
.L_020087c0:
	.4byte gData_0200135c
.L_020087c4:
	.4byte gData_02001388
.L_020087c8:
	.4byte gData_020013b4
.L_020087cc:
	.4byte 0x0000008a
.L_020087d0:
	.4byte 0x00000089
.L_020087d4:
	.4byte gData_02001454
	.section .text.x020089f8,"ax",%progbits
	.balign 4
	.global Func_020009f8
	.thumb_func
Func_020009f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	ldr r3, .L_02008ae4
	str r1, [sp, #4]
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	adds r0, r5, #0
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02001254
	movs r0, #0
	bl Func_02001324
	ldr r3, [sp, #4]
	ldr r2, .L_02008ae8
	lsls r3, r3, #16
	mov r11, r3
	ldr r3, [r6, #8]
	movs r5, #128
	add r3, r11
	lsls r5, r5, #12
	ands r3, r2
	adds r1, r3, r5
	ldr r3, [sp, #0]
	mov r10, r2
	lsls r3, r3, #16
	mov r9, r3
	ldr r3, [r6, #16]
	adds r0, r6, #0
	add r3, r9
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #9
	str r2, [r6, #48]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r5
	mov r8, r2
	str r2, [r6, #52]
	ldr r2, [r6, #12]
	bl Func_0200121c
	adds r0, r6, #0
	movs r1, #27
	bl Func_02001214
	ldr r3, [r7, #8]
	mov r2, r10
	add r3, r11
	ands r3, r2
	adds r1, r3, r5
	ldr r3, [r7, #16]
	adds r0, r7, #0
	add r3, r9
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #9
	str r2, [r7, #48]
	mov r2, r8
	adds r3, r3, r5
	str r2, [r7, #52]
	ldr r2, [r7, #12]
	bl Func_0200121c
	ldr r3, [sp, #4]
	cmp r3, #0
	blt .L_02008aa2
	ldr r2, [sp, #0]
	cmp r2, #0
	bge .L_02008aac
.L_02008aa2:
	adds r0, r7, #0
	movs r1, #4
	bl Func_02001214
	b .L_02008ab4
.L_02008aac:
	adds r0, r7, #0
	movs r1, #3
	bl Func_02001214
.L_02008ab4:
	movs r0, #226
	bl Func_02001344
	adds r0, r6, #0
	bl Func_02001224
	movs r1, #2
	adds r0, r7, #0
	bl Func_02001214
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02001344
	bl Func_0200125c
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008ae4:
	.4byte gPartyState
.L_02008ae8:
	.4byte 0xfff00000
	.section .bss,"aw",%nobits
	.global gData_02001a14
gData_02001a14:
	.space 0x00000004
	.global gData_02001a18
gData_02001a18:
	.space 0x00000004
	.global gData_02001a1c
gData_02001a1c:
	.space 0x00000004
	.global gData_02001a20
gData_02001a20:
	.space 0x00000004
	.global gData_02001a24
gData_02001a24:
	.space 0x00000004
	.global gData_02001a28
gData_02001a28:
	.space 0x00000004
	.global Data_02001a2c
Data_02001a2c:
	.space 0x00000004
	.global gData_02001a30
gData_02001a30:
	.space 0x00000004
	.global gData_02001a34
gData_02001a34:
