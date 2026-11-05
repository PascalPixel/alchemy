.syntax unified
	.thumb
	.section .text.x02008668,"ax",%progbits
	.balign 4
	.global Func_02000668
	.thumb_func
Func_02000668:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	b .L_020086b8
.L_02008672:
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
	bl Func_020005cc
.L_020086b8:
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_02008672
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008ff4,"ax",%progbits
	.balign 4
	.global Func_02000ff4
	.thumb_func
Func_02000ff4:
	push {r5, r6, lr}
	movs r3, #192
	adds r6, r0, #0
	lsls r3, r3, #18
	adds r6, #100
	ldr r5, [r3, #108]
	movs r1, #0
	ldrsh r3, [r6, r1]
	movs r1, #132
	lsls r1, r1, #2
	ldrh r2, [r6]
	cmp r3, r1
	ble .L_02009012
	movs r3, #2
	b .L_0200901c
.L_02009012:
	movs r1, #200
	lsls r1, r1, #1
	cmp r3, r1
	ble .L_02009032
	movs r3, #16
.L_0200901c:
	ands r3, r2
	cmp r3, #0
	beq .L_0200902a
	movs r1, #10
	bl Actor_SetPartAttribute
	b .L_0200903a
.L_0200902a:
	movs r1, #0
	bl Actor_SetPartAttribute
	b .L_0200903a
.L_02009032:
	ldr r3, .L_02009080
	str r3, [r0, #108]
	movs r3, #1
	strh r3, [r6]
.L_0200903a:
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200907c
	subs r2, #12
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200907c
	adds r2, #4
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200907c
	adds r2, #10
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200907c
	adds r2, #74
	adds r3, r5, r2
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_0200907c
	ldrh r3, [r6]
	subs r3, #1
	strh r3, [r6]
.L_0200907c:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009080:
	.4byte Func_02001738
	.section .text.x020091e4,"ax",%progbits
	.balign 4
	.global Func_020011e4
	.thumb_func
Func_020011e4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
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
	bl Func_02003a54
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	bl Func_02003aec
	movs r0, #0
	bl Func_02003bcc
	ldr r3, [r6, #8]
	add r2, sp, #56
	str r3, [r2]
	ldr r3, [r6, #12]
	str r3, [r2, #4]
	ldr r3, [r6, #16]
	str r3, [r2, #8]
.L_0200923e:
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
	bl MapCell_GetAttributes
	asrs r0, r0, #8
	cmp r0, #212
	beq .L_020092d6
	cmp r0, #222
	beq .L_020092d6
	movs r2, #8
	mov r9, r2
.L_02009264:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	mov r0, r9
	lsls r3, r0, #2
	adds r3, #20
	ldr r5, [r2, r3]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_020092ca
	mov r1, r8
	ldr r2, [r1, #8]
	ldr r3, [r5, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200929e
	ldr r2, [r1, #12]
	ldr r3, [r5, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200929e
	ldr r2, [r1, #16]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_020092ca
.L_0200929e:
	adds r0, r5, #0
	adds r0, #8
	adds r1, r7, #0
	bl Func_020005a0
	cmp r0, #8
	bgt .L_020092ca
	ldr r2, [r5, #12]
	ldr r3, [r7, #4]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020092ca
	ldr r3, [r5, #80]
	movs r0, #136
	ldr r3, [r3, #40]
	lsls r0, r0, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, r0
	bne .L_020093ae
	b .L_020092d6
.L_020092ca:
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #64
	bne .L_02009264
	b .L_0200923e
.L_020092d6:
	ldr r3, .L_02009430
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
	bl Func_02003a84
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	bl Func_02003b5c
	ldr r1, [r7, #4]
	ldr r2, [r7, #8]
	movs r3, #1
	ldr r0, [r7]
	bl Camera_MoveWithinMapBounds
	adds r0, r6, #0
	bl Func_02003a8c
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	bl Func_02003a6c
	movs r3, #0
	mov r9, r3
.L_0200931a:
	ldr r3, .L_02009434
	movs r2, #1
	ldr r3, [r3]
	mov r10, r3
	mov r0, r10
	ands r0, r2
	mov r10, r0
	cmp r0, #0
	bne .L_02009378
	add r1, sp, #16
	mov r8, r1
	movs r3, #148
	mov r0, r8
	adds r3, #255
	strh r3, [r0, #24]
	ldr r3, .L_02009438
	str r2, [r0]
	str r3, [r0, #36]
	ldr r3, .L_0200943c
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
.L_02009378:
	movs r0, #1
	bl wait
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #16
	bne .L_0200931a
	movs r0, #24
	bl wait
	ldr r3, .L_02009440
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	movs r1, #1
	bl Camera_FollowActor
	bl Func_02003b6c
	movs r0, #10
	bl wait
	bl Func_02003af4
	b .L_02009424
.L_020093ae:
	ldr r3, .L_02009430
	movs r5, #128
	lsls r5, r5, #10
	str r5, [r6, #52]
	str r5, [r6, #48]
	ldr r2, [r7, #4]
	str r3, [r6, #108]
	ldr r1, [r7]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02003a84
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	bl Func_02003b5c
	ldr r2, [r7, #8]
	ldr r1, [r7, #4]
	movs r3, #1
	ldr r0, [r7]
	bl Camera_MoveWithinMapBounds
	adds r0, r6, #0
	bl Func_02003a8c
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	bl Func_02003a6c
	movs r0, #188
	lsls r0, r0, #2
	bl GameFlag_SetBit
	mov r1, r9
	lsls r0, r1, #16
	lsrs r0, r0, #16
	bl Func_02000c08
	movs r0, #188
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	ldr r3, .L_02009440
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Camera_FollowActor
	bl Func_02003b6c
	movs r0, #10
	bl wait
	bl Func_02003af4
.L_02009424:
	add sp, #68
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009430:
	.4byte Func_02000d40
.L_02009434:
	.4byte gFrameCount
.L_02009438:
	.4byte Func_020011b8
.L_0200943c:
	.4byte gData_02003d50
.L_02009440:
	.4byte gMainRoutineWork
	.section .text.x02009738,"ax",%progbits
	.global Func_02001738
	.thumb_func
Func_02001738:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, .L_020099b0
	mov r9, r3
	ldr r3, [r5]
	mov r8, r0
	movs r0, #15
	ands r3, r0
	sub sp, #72
	mov r10, r0
	cmp r3, #0
	bne .L_02009768
	mov r0, r8
	movs r1, #136
	bl Func_02003c4c
.L_02009768:
	movs r1, #10
	mov r0, r8
	bl Actor_SetPartAttribute
	movs r1, #100
	add r1, r8
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r11, r1
	cmp r3, #31
	bgt .L_020097e6
	ldr r7, [r5]
	movs r3, #3
	ands r7, r3
	cmp r7, #0
	beq .L_0200978a
	b .L_02009954
.L_0200978a:
	movs r3, #148
	add r6, sp, #32
	adds r3, #255
	strh r3, [r6, #24]
	movs r3, #10
	str r3, [r6, #4]
	movs r3, #2
	str r3, [r6]
	ldr r3, .L_020099b4
	str r3, [r6, #36]
	ldr r3, .L_020099b8
	str r3, [r6, #28]
	bl Random16Far
	mov r3, r10
	adds r5, r0, #0
	ands r5, r3
	bl Random16Far
	mov r4, r10
	mov r1, r8
	ands r0, r4
	ldr r4, [r1, #8]
	ldr r1, [r1, #12]
	movs r3, #192
	lsls r3, r3, #13
	movs r2, #128
	lsls r2, r2, #9
	adds r1, r1, r3
	mov r3, r8
	adds r4, r4, r2
	ldr r2, [r3, #16]
	ldr r3, .L_020099bc
	subs r5, #8
	subs r0, #8
	lsls r0, r0, #14
	lsls r5, r5, #14
	str r0, [sp, #4]
	str r3, [sp, #8]
	adds r0, r4, #0
	adds r3, r5, #0
	str r7, [sp, #0]
	str r6, [sp, #12]
	bl Func_0200015c
	b .L_02009954
.L_020097e6:
	cmp r3, #33
	beq .L_020097ec
	b .L_0200993e
.L_020097ec:
	mov r0, r8
	movs r1, #144
	bl Func_02003c4c
	mov r4, r8
	ldr r1, [r4, #8]
	ldr r2, [r4, #12]
	movs r0, #128
	lsls r0, r0, #9
	movs r3, #128
	adds r1, r1, r0
	lsls r3, r3, #14
	movs r0, #96
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r4, #16]
	bl Import_ActorCreate
	add r7, sp, #16
	str r0, [r7]
	movs r4, #1
	mov r10, r4
.L_02009818:
	ldr r3, [r7]
	movs r0, #96
	ldr r6, [r3, #80]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	adds r0, #255
	bl Import_ActorCreate
	mov r1, r10
	ldrb r3, [r6, #16]
	lsls r5, r1, #2
	ldr r1, [r0, #80]
	movs r2, #1
	strb r3, [r1, #16]
	ldrb r3, [r1, #17]
	str r0, [r7, r5]
	orrs r3, r2
	strb r3, [r1, #17]
	ldr r2, [r7]
	movs r6, #0
	ldr r3, [r2, #8]
	movs r1, #0
	str r3, [r0, #8]
	ldr r3, [r2, #12]
	str r3, [r0, #12]
	ldr r3, [r2, #16]
	str r3, [r0, #16]
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
	bl Actor_SetSingleChildFlags
	ldr r0, [r7, r5]
	movs r1, #3
	bl Func_02003a54
	ldr r0, [r7, r5]
	mov r2, r10
	movs r3, #3
	subs r3, r3, r2
	ldr r5, .L_020099c0
	adds r2, r0, #0
	adds r2, #98
	strb r3, [r2]
	mov r3, r8
	str r3, [r0, #104]
	str r5, [r0, #108]
	movs r1, #3
	bl Actor_SetPartAttribute
	movs r4, #1
	add r10, r4
	mov r0, r10
	cmp r0, #4
	bne .L_02009818
	ldr r0, [r7]
	movs r1, #0
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
	bl Actor_SetSingleChildFlags
	ldr r0, [r7]
	movs r1, #3
	bl Func_02003a54
	ldr r0, [r7]
	movs r1, #3
	bl Scene_SetActorSpritePriority
	ldr r0, [r7]
	movs r3, #3
	adds r2, r0, #0
	adds r2, #98
	mov r1, r8
	strb r3, [r2]
	str r1, [r0, #104]
	str r5, [r0, #108]
	movs r1, #3
	bl Actor_SetPartAttribute
	ldr r1, .L_020099c4
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldr r3, .L_020099c8
	movs r0, #0
	cmp r2, r3
	bne .L_020098de
	mov r1, r8
	ldr r0, [r1, #8]
	ldr r2, .L_020099cc
	ldr r1, [r1, #16]
	bl Func_020010cc
	b .L_0200991e
.L_020098de:
	ldr r3, .L_020099d0
	cmp r2, r3
	bne .L_0200991e
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	mov r1, r8
	ldr r0, [r1, #8]
	ldr r1, [r1, #16]
	cmp r3, #3
	bgt .L_02009900
	ldr r2, .L_020099d4
	bl Func_020010cc
	b .L_0200991e
.L_02009900:
	cmp r3, #8
	bgt .L_0200990c
	ldr r2, .L_020099d8
	bl Func_020010cc
	b .L_0200991e
.L_0200990c:
	cmp r3, #12
	bgt .L_02009918
	ldr r2, .L_020099dc
	bl Func_020010cc
	b .L_0200991e
.L_02009918:
	ldr r2, .L_020099e0
	bl Func_020010cc
.L_0200991e:
	bl Actor_GetById
	adds r2, r0, #0
	adds r2, #98
	movs r3, #4
	strb r3, [r2]
	ldr r3, .L_020099e4
	mov r2, r8
	str r2, [r0, #104]
	str r3, [r0, #108]
	mov r4, r11
	ldrh r3, [r4]
	mov r0, r11
	adds r3, #1
	strh r3, [r0]
	b .L_02009954
.L_0200993e:
	movs r1, #182
	lsls r1, r1, #1
	cmp r3, r1
	ble .L_02009954
	mov r0, r8
	movs r1, #0
	bl Actor_SetPartAttribute
	movs r3, #0
	mov r2, r8
	str r3, [r2, #108]
.L_02009954:
	movs r3, #179
	lsls r3, r3, #1
	add r3, r9
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	bne .L_020099a2
	movs r3, #173
	lsls r3, r3, #1
	add r3, r9
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	bne .L_020099a2
	movs r3, #175
	lsls r3, r3, #1
	add r3, r9
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_020099a2
	movs r3, #180
	lsls r3, r3, #1
	add r3, r9
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020099a2
	movs r3, #217
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_020099a2
	mov r2, r8
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020099a2:
	add sp, #72
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020099b0:
	.4byte gFrameCount
.L_020099b4:
	.4byte Func_020011b8
.L_020099b8:
	.4byte gData_02003d50
.L_020099bc:
	.4byte 0x01330000
.L_020099c0:
	.4byte Func_02001528
.L_020099c4:
	.4byte gMainRoutineWork
.L_020099c8:
	.4byte 0x000000df
.L_020099cc:
	.4byte gData_02003d9e
.L_020099d0:
	.4byte 0x000000e0
.L_020099d4:
	.4byte gData_02003db2
.L_020099d8:
	.4byte gData_02003dc4
.L_020099dc:
	.4byte gData_02003dd6
.L_020099e0:
	.4byte gData_02003dee
.L_020099e4:
	.4byte Func_020015e4
	.section .text.x02009e08,"ax",%progbits
	.global Func_02001e08
	.thumb_func
Func_02001e08:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009ed4
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #20
	bl Actor_GetById
	movs r2, #8
	adds r1, r0, #0
	add r2, sp
	ldr r3, [r1, #8]
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	mov r2, r10
	str r3, [r2]
	mov r0, r10
	ldr r3, [r1, #12]
	movs r6, #79
	str r3, [r2, #4]
	ldr r3, [r1, #16]
	str r3, [r2, #8]
	bl Actor_FindInCell
	adds r5, r0, #0
	bl Func_0200074c
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldr r3, [r5, #20]
	adds r7, r5, #0
	adds r7, #34
	asrs r1, r1, #20
	asrs r2, r2, #20
	asrs r3, r3, #19
	ldrb r0, [r7]
	bl Func_020005cc
	movs r3, #36
	str r3, [sp, #4]
	movs r0, #79
	movs r1, #46
	movs r2, #2
	mov r8, r3
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02003aa4
	bl Func_020003b4
	movs r3, #0
	mov r1, r8
	strb r3, [r7]
	movs r0, #79
	str r1, [sp, #4]
	movs r2, #2
	movs r1, #37
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02003aa4
	ldr r3, [r5, #20]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r3, r3, #19
	asrs r1, r1, #20
	asrs r2, r2, #20
	adds r3, #4
	ldrb r0, [r7]
	bl Func_020005cc
	ldr r0, .L_02009ed8
	bl Func_020006cc
	ldr r2, [r5, #8]
	mov r3, r10
	asrs r2, r2, #20
	str r2, [r3]
	mov r1, r10
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	str r3, [r1, #8]
	cmp r2, #16
	bne .L_02009eca
	cmp r3, #36
	bne .L_02009eca
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #217
	bl GameFlag_SetBit
.L_02009eca:
	add sp, #20
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009ed4:
	.4byte gMainRoutineWork
.L_02009ed8:
	.4byte gData_02003de4
	.section .text.x02009edc,"ax",%progbits
	.global Func_02001edc
	.thumb_func
Func_02001edc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009fa4
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #20
	bl Actor_GetById
	add r2, sp, #8
	adds r1, r0, #0
	ldr r3, [r1, #8]
	mov r10, r2
	ldr r2, .L_02009fa8
	mov r0, r10
	adds r3, r3, r2
	mov r2, r10
	str r3, [r2]
	movs r6, #99
	ldr r3, [r1, #12]
	str r3, [r2, #4]
	ldr r3, [r1, #16]
	str r3, [r2, #8]
	bl Actor_FindInCell
	adds r5, r0, #0
	bl Func_0200074c
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldr r3, [r5, #20]
	adds r7, r5, #0
	adds r7, #34
	asrs r1, r1, #20
	asrs r2, r2, #20
	asrs r3, r3, #19
	ldrb r0, [r7]
	bl Func_020005cc
	movs r3, #38
	str r3, [sp, #4]
	movs r0, #94
	movs r1, #44
	movs r2, #5
	mov r8, r3
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02003aa4
	bl Func_020003b4
	movs r3, #0
	mov r1, r8
	strb r3, [r7]
	movs r0, #99
	str r1, [sp, #4]
	movs r2, #5
	movs r1, #42
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02003aa4
	ldr r3, [r5, #20]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r3, r3, #19
	asrs r1, r1, #20
	asrs r2, r2, #20
	adds r3, #4
	ldrb r0, [r7]
	bl Func_020005cc
	ldr r0, .L_02009fac
	bl Func_020006cc
	ldr r2, [r5, #8]
	mov r3, r10
	asrs r2, r2, #20
	str r2, [r3]
	mov r1, r10
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	str r3, [r1, #8]
	cmp r2, #35
	bne .L_02009f9a
	cmp r3, #38
	bne .L_02009f9a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #219
	bl GameFlag_SetBit
.L_02009f9a:
	add sp, #20
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009fa4:
	.4byte gMainRoutineWork
.L_02009fa8:
	.4byte 0xfff00000
.L_02009fac:
	.4byte gData_02003de8
	.section .text.x0200a024,"ax",%progbits
	.balign 4
	.global Func_02002024
	.thumb_func
Func_02002024:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	movs r3, #128
	add r7, sp, #28
	lsls r3, r3, #10
	str r3, [r7, #8]
	str r3, [r7, #12]
	movs r3, #148
	adds r3, #255
	strh r3, [r7, #24]
	ldr r3, .L_0200a0c8
	movs r2, #1
	str r2, [r7]
	str r3, [r7, #28]
	movs r3, #0
	mov r11, r0
	mov r9, r1
	mov r10, r3
.L_0200a054:
	bl Random16Far
	movs r3, #63
	ands r0, r3
	ldr r6, .L_0200a0cc
	lsls r0, r0, #16
	add r0, r11
	add r5, sp, #16
	adds r0, r0, r6
	mov r8, r3
	str r0, [r5]
	bl Random16Far
	movs r3, #15
	ands r3, r0
	lsls r3, r3, #12
	str r3, [r5, #4]
	bl Random16Far
	mov r3, r8
	adds r2, r0, #0
	ands r2, r3
	lsls r2, r2, #16
	ldr r3, [r5, #4]
	add r2, r9
	adds r2, r2, r6
	str r2, [r5, #8]
	ldr r0, [r5]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #232
	lsls r3, r3, #14
	adds r3, #1
	str r3, [sp, #8]
	movs r1, #0
	movs r3, #0
	str r7, [sp, #12]
	bl Func_0200015c
	movs r3, #1
	add r10, r3
	mov r3, r10
	cmp r3, #4
	bne .L_0200a054
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #220
	bl GameFlag_SetBit
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a0c8:
	.4byte gData_02003d50
.L_0200a0cc:
	.4byte 0xffe00000
	.section .text.x0200a518,"ax",%progbits
	.global Func_02002518
	.thumb_func
Func_02002518:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	lsls r0, r0, #16
	lsrs r5, r0, #16
	adds r0, r5, #0
	sub sp, #16
	bl Actor_GetById
	ldr r3, .L_0200a610
	ldr r2, [r0, #108]
	cmp r2, r3
	bne .L_0200a604
	adds r0, r5, #0
	bl Actor_GetById
	ldr r0, [r0, #104]
	ldr r1, [r0, #8]
	mov r8, r0
	mov r3, r8
	movs r0, #128
	ldr r2, [r3, #12]
	lsls r0, r0, #9
	adds r1, r1, r0
	movs r0, #128
	lsls r0, r0, #14
	adds r2, r2, r0
	movs r0, #96
	ldr r3, [r3, #16]
	adds r0, #255
	bl Import_ActorCreate
	mov r7, sp
	str r0, [r7]
	movs r3, #1
	mov r10, r3
.L_0200a562:
	ldr r3, [r7]
	movs r0, #96
	ldr r6, [r3, #80]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	adds r0, #255
	bl Import_ActorCreate
	mov r3, r10
	ldr r1, [r0, #80]
	lsls r5, r3, #2
	ldrb r3, [r6, #16]
	movs r2, #1
	strb r3, [r1, #16]
	ldrb r3, [r1, #17]
	str r0, [r7, r5]
	orrs r3, r2
	strb r3, [r1, #17]
	ldr r2, [r7]
	movs r6, #0
	ldr r3, [r2, #8]
	movs r1, #0
	str r3, [r0, #8]
	ldr r3, [r2, #12]
	str r3, [r0, #12]
	ldr r3, [r2, #16]
	str r3, [r0, #16]
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
	bl Actor_SetSingleChildFlags
	ldr r0, [r7, r5]
	movs r1, #3
	bl Func_02003a54
	ldr r0, [r7, r5]
	mov r3, r10
	adds r2, r0, #0
	ldr r5, .L_0200a614
	adds r3, #255
	adds r2, #98
	strb r3, [r2]
	mov r3, r8
	str r3, [r0, #104]
	str r5, [r0, #108]
	movs r1, #3
	bl Actor_SetPartAttribute
	movs r0, #1
	add r10, r0
	mov r3, r10
	cmp r3, #4
	bne .L_0200a562
	ldr r0, [r7]
	movs r1, #0
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
	bl Actor_SetSingleChildFlags
	ldr r0, [r7]
	movs r1, #3
	bl Func_02003a54
	ldr r0, [r7]
	movs r1, #3
	bl Scene_SetActorSpritePriority
	ldr r0, [r7]
	movs r3, #3
	adds r2, r0, #0
	adds r2, #98
	strb r3, [r2]
	mov r3, r8
	str r3, [r0, #104]
	str r5, [r0, #108]
	movs r1, #3
	bl Actor_SetPartAttribute
.L_0200a604:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a610:
	.4byte Func_020015e4
.L_0200a614:
	.4byte Func_02001528
	.section .text.x0200a618,"ax",%progbits
	.global Func_02002618
	.thumb_func
Func_02002618:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r1, .L_0200a664
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	mov r8, r1
	movs r3, #152
	ldr r1, .L_0200a668
	lsls r3, r3, #2
	add r3, r8
	adds r2, #94
	strh r1, [r3]
	add r2, r8
	movs r3, #1
	strh r3, [r2]
	movs r3, #240
	lsls r3, r3, #1
	add r3, r8
	movs r0, #0
	ldrsh r2, [r3, r0]
	sub sp, #8
	ldr r7, .L_0200a660
	cmp r2, r1
	beq .L_0200a65e
	b .L_0200a780
.L_0200a65e:
	b .L_0200a66c
.L_0200a660:
	.4byte 0x00000000
.L_0200a664:
	.4byte gMainRoutineWork
.L_0200a668:
	.4byte 0x000000df
.L_0200a66c:
	movs r3, #133
	lsls r3, r3, #2
	add r3, r8
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	movs r0, #8
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #10
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	ldr r0, .L_0200a9fc
	bl Func_02000668
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a6ba
	movs r0, #21
	movs r1, #13
	movs r2, #0
	bl Func_02000ee4
.L_0200a6ba:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #213
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a6d6
	movs r1, #132
	movs r2, #180
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02003b0c
.L_0200a6d6:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #214
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a6f2
	movs r1, #248
	movs r2, #204
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02003b0c
.L_0200a6f2:
	ldr r6, .L_0200aa00
	movs r5, #78
	adds r0, r6, #0
	bl Func_02003c0c
	movs r3, #21
	str r3, [sp, #4]
	movs r0, #98
	movs r1, #21
	movs r2, #4
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02003aa4
	movs r3, #24
	str r3, [sp, #4]
	movs r0, #98
	movs r1, #27
	movs r2, #4
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02003aa4
	adds r0, r6, #0
	bl Func_020006cc
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a75e
	movs r0, #20
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r1, #128
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r0, #8]
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, #4
	strb r7, [r3]
	movs r2, #192
	ldr r3, [r0, #12]
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r0, #12]
	str r3, [r0, #20]
	movs r1, #0
	bl Actor_SetSingleChildFlags
.L_0200a75e:
	movs r0, #20
	bl Func_02002518
	movs r3, #241
	lsls r3, r3, #1
	add r3, r8
	movs r0, #0
	ldrsh r2, [r3, r0]
	cmp r2, #4
	ble .L_0200a774
	b .L_0200ac20
.L_0200a774:
	cmp r2, #3
	bge .L_0200a77a
	b .L_0200ac20
.L_0200a77a:
	movs r0, #128
	lsls r0, r0, #13
	b .L_0200ac1c
.L_0200a780:
	ldr r3, .L_0200aa04
	cmp r2, r3
	beq .L_0200a788
	b .L_0200ac26
.L_0200a788:
	movs r1, #133
	lsls r1, r1, #2
	add r1, r8
	ldr r0, [r1]
	mov r10, r1
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r2, #241
	lsls r2, r2, #1
	add r2, r8
	strb r3, [r0]
	movs r0, #0
	ldrsh r3, [r2, r0]
	mov r9, r2
	cmp r3, #3
	bgt .L_0200a848
	ldr r0, .L_0200aa08
	bl Func_02002d08
	ldr r0, .L_0200aa0c
	bl Func_02000668
	bl Func_02003bec
	movs r0, #0
	movs r1, #11
	movs r2, #12
	bl Func_02003bf4
	movs r0, #251
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a7e6
	movs r1, #220
	movs r2, #134
	movs r0, #15
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003b0c
.L_0200a7e6:
	ldr r0, .L_0200aa10
	bl Func_02003c0c
	mov r2, r9
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, #1
	bne .L_0200a81c
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a814
	mov r3, r10
	ldr r0, [r3]
	bl Actor_GetById
	ldr r3, [r0, #16]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #16]
.L_0200a814:
	ldr r0, .L_0200aa14
	bl Func_0200244c
	b .L_0200acda
.L_0200a81c:
	cmp r3, #2
	beq .L_0200a822
	b .L_0200acda
.L_0200a822:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a840
	mov r2, r10
	ldr r0, [r2]
	bl Actor_GetById
	ldr r3, [r0, #16]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #16]
.L_0200a840:
	ldr r0, .L_0200aa18
	bl Func_0200244c
	b .L_0200acda
.L_0200a848:
	cmp r3, #8
	ble .L_0200a84e
	b .L_0200aa28
.L_0200a84e:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a908
	movs r0, #8
	bl Actor_GetById
	adds r0, #85
	strb r7, [r0]
	movs r0, #8
	bl Actor_GetById
	ldr r3, [r0, #12]
	movs r5, #160
	lsls r5, r5, #15
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r0, #9
	bl Actor_GetById
	adds r0, #85
	strb r7, [r0]
	movs r0, #9
	bl Actor_GetById
	ldr r3, [r0, #12]
	movs r1, #8
	adds r3, r3, r5
	movs r2, #0
	str r3, [r0, #12]
	movs r0, #19
	bl Func_02000ee4
	movs r0, #10
	bl Actor_GetById
	adds r0, #85
	strb r7, [r0]
	movs r0, #10
	bl Actor_GetById
	ldr r3, [r0, #12]
	movs r5, #160
	lsls r5, r5, #16
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r0, #11
	bl Actor_GetById
	adds r0, #85
	strb r7, [r0]
	movs r0, #11
	bl Actor_GetById
	ldr r3, [r0, #12]
	movs r1, #10
	adds r3, r3, r5
	movs r2, #1
	str r3, [r0, #12]
	movs r0, #20
	bl Func_02000ee4
	movs r0, #12
	bl Actor_GetById
	adds r0, #85
	strb r7, [r0]
	movs r0, #12
	bl Actor_GetById
	ldr r3, [r0, #12]
	movs r5, #240
	lsls r5, r5, #16
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r0, #13
	bl Actor_GetById
	adds r0, #85
	strb r7, [r0]
	movs r0, #13
	bl Actor_GetById
	ldr r3, [r0, #12]
	movs r1, #12
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r2, #0
	movs r0, #21
	bl Func_02000ee4
.L_0200a908:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #216
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a924
	movs r1, #222
	movs r2, #142
	movs r0, #16
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02003b0c
.L_0200a924:
	ldr r0, .L_0200aa1c
	bl Func_02003c0c
	ldr r0, .L_0200aa20
	bl Func_02000668
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a98c
	movs r0, #15
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r6, #128
	lsls r6, r6, #9
	adds r3, r3, r6
	str r3, [r0, #8]
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, #4
	strb r7, [r3]
	movs r5, #192
	ldr r3, [r0, #12]
	lsls r5, r5, #13
	adds r3, r3, r5
	str r3, [r0, #12]
	str r3, [r0, #20]
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #18
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r1, #0
	adds r3, r3, r6
	str r3, [r0, #8]
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, #4
	strb r7, [r3]
	ldr r3, [r0, #12]
	adds r3, r3, r5
	str r3, [r0, #12]
	str r3, [r0, #20]
	bl Actor_SetSingleChildFlags
.L_0200a98c:
	movs r0, #15
	bl Func_02002518
	movs r0, #18
	bl Func_02002518
	mov r0, r9
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #5
	bne .L_0200a9ac
	movs r0, #144
	lsls r0, r0, #17
	bl Func_0200244c
	b .L_0200ac20
.L_0200a9ac:
	cmp r3, #6
	bne .L_0200a9c4
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a9be
	b .L_0200ac20
.L_0200a9be:
	bl Func_020022ac
	b .L_0200ac20
.L_0200a9c4:
	cmp r3, #7
	bne .L_0200a9ee
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a9e6
	mov r1, r10
	ldr r0, [r1]
	bl Actor_GetById
	ldr r3, [r0, #16]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r0, #16]
.L_0200a9e6:
	ldr r0, .L_0200aa24
	bl Func_0200244c
	b .L_0200ac20
.L_0200a9ee:
	cmp r3, #8
	beq .L_0200a9f4
	b .L_0200ac20
.L_0200a9f4:
	movs r0, #208
	lsls r0, r0, #15
	b .L_0200ac1c
	.2byte 0x0000
.L_0200a9fc:
	.4byte gData_02003d9e
.L_0200aa00:
	.4byte gData_02003d98
.L_0200aa04:
	.4byte 0x000000e0
.L_0200aa08:
	.4byte gData_02003dba
.L_0200aa0c:
	.4byte gData_02003db2
.L_0200aa10:
	.4byte gData_02003dac
.L_0200aa14:
	.4byte 0xfff00000
.L_0200aa18:
	.4byte 0xffa00000
.L_0200aa1c:
	.4byte gData_02003dd2
.L_0200aa20:
	.4byte gData_02003dc4
.L_0200aa24:
	.4byte 0xffd00000
.L_0200aa28:
	cmp r3, #12
	bgt .L_0200aaee
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #217
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aa48
	movs r1, #132
	movs r2, #146
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003b0c
.L_0200aa48:
	ldr r5, .L_0200ace8
	adds r0, r5, #0
	bl Func_02003c0c
	adds r0, r5, #0
	bl Func_020006cc
	bl Func_02003be4
	movs r1, #148
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #14
	movs r3, #13
	bl Func_02003bfc
	ldr r0, .L_0200acec
	bl Func_02000668
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200aaca
	movs r0, #10
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r6, #128
	lsls r6, r6, #9
	adds r3, r3, r6
	str r3, [r0, #8]
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, #4
	strb r7, [r3]
	movs r5, #192
	ldr r3, [r0, #12]
	lsls r5, r5, #13
	adds r3, r3, r5
	str r3, [r0, #12]
	str r3, [r0, #20]
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #12
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r1, #0
	adds r3, r3, r6
	str r3, [r0, #8]
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, #4
	strb r7, [r3]
	ldr r3, [r0, #12]
	adds r3, r3, r5
	str r3, [r0, #12]
	str r3, [r0, #20]
	bl Actor_SetSingleChildFlags
.L_0200aaca:
	movs r0, #10
	bl Func_02002518
	movs r0, #12
	bl Func_02002518
	mov r0, r9
	ldrh r3, [r0]
	movs r1, #128
	subs r3, #9
	lsls r3, r3, #16
	lsls r1, r1, #9
	cmp r3, r1
	bls .L_0200aae8
	b .L_0200ac20
.L_0200aae8:
	movs r0, #192
	lsls r0, r0, #14
	b .L_0200ac1c
.L_0200aaee:
	ldr r0, .L_0200acf0
	bl Func_02000668
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ab2c
	movs r0, #13
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r0, #8]
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, #4
	strb r7, [r3]
	movs r1, #192
	ldr r3, [r0, #12]
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #12]
	str r3, [r0, #20]
	movs r1, #0
	bl Actor_SetSingleChildFlags
.L_0200ab2c:
	movs r0, #13
	bl Func_02002518
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #218
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab4e
	movs r1, #162
	movs r2, #150
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02003b0c
.L_0200ab4e:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #219
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab6a
	movs r1, #142
	movs r2, #154
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02003b0c
.L_0200ab6a:
	ldr r0, .L_0200acf4
	bl Func_0200079c
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab90
	movs r3, #123
	movs r2, #48
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #48
	movs r2, #1
	movs r3, #3
	bl Func_02003aa4
.L_0200ab90:
	movs r0, #169
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200abb2
	movs r3, #125
	movs r2, #48
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #48
	movs r2, #1
	movs r3, #3
	bl Func_02003aa4
.L_0200abb2:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #82
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200abd4
	movs r3, #109
	movs r2, #56
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #108
	movs r1, #56
	movs r2, #1
	movs r3, #3
	bl Func_02003aa4
.L_0200abd4:
	mov r3, r9
	ldrh r2, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #14
	bne .L_0200abea
	movs r0, #192
	lsls r0, r0, #14
	bl Func_0200244c
	b .L_0200ac20
.L_0200abea:
	adds r3, r2, #0
	subs r3, #16
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #9
	cmp r3, r1
	bhi .L_0200ac20
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ac1a
	movs r3, #133
	lsls r3, r3, #2
	add r3, r8
	ldr r0, [r3]
	bl Actor_GetById
	ldr r3, [r0, #16]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r0, #16]
.L_0200ac1a:
	ldr r0, .L_0200acf8
.L_0200ac1c:
	bl Func_0200244c
.L_0200ac20:
	bl Func_020024c8
	b .L_0200acda
.L_0200ac26:
	ldr r3, .L_0200acfc
	cmp r2, r3
	bne .L_0200acda
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #220
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ac8e
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003b0c
	movs r3, #17
	str r3, [sp, #4]
	movs r5, #42
	movs r0, #0
	movs r1, #32
	movs r2, #5
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02003aa4
	movs r3, #81
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #96
	movs r2, #5
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02003aa4
	movs r5, #5
	movs r6, #8
	movs r0, #0
	movs r1, #32
	movs r2, #42
	movs r3, #17
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003a94
	movs r0, #0
	movs r1, #96
	movs r2, #42
	movs r3, #81
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003a94
.L_0200ac8e:
	movs r5, #133
	lsls r5, r5, #2
	add r5, r8
	ldr r0, [r5]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	ldr r0, .L_0200ad00
	bl Func_02000668
	movs r3, #241
	lsls r3, r3, #1
	add r3, r8
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #2
	bgt .L_0200acda
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200acd4
	ldr r0, [r5]
	bl Actor_GetById
	ldr r3, [r0, #16]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #16]
.L_0200acd4:
	ldr r0, .L_0200acf8
	bl Func_0200244c
.L_0200acda:
	movs r0, #0
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200ace8:
	.4byte gData_02003de4
.L_0200acec:
	.4byte gData_02003dd6
.L_0200acf0:
	.4byte gData_02003dee
.L_0200acf4:
	.4byte gData_02003de8
.L_0200acf8:
	.4byte 0xffe00000
.L_0200acfc:
	.4byte 0x000000e1
.L_0200ad00:
	.4byte gData_02003e02
	.section .text.x0200b0bc,"ax",%progbits
	.balign 4
	.global Func_020030bc
	.thumb_func
Func_020030bc:
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
	ldr r5, .L_0200b218
	str r3, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r11, r0
	ldr r1, [r5]
	movs r0, #8
	bl Func_02003b14
	ldr r1, [r5]
	movs r0, #9
	bl Func_02003b14
	ldr r1, [r5]
	movs r0, #10
	bl Func_02003b14
	movs r0, #1
	bl wait
	movs r0, #8
	bl Func_020030a4
	movs r0, #9
	bl Func_020030a4
	movs r0, #10
	bl Func_020030a4
	movs r1, #0
	movs r0, #9
	bl Actor_SetModeById
	movs r0, #1
	bl wait
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003b0c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02003b0c
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_02003b0c
	movs r0, #1
	bl wait
	b .L_0200b1f6
.L_0200b142:
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
	ldr r2, .L_0200b21c
	adds r3, r6, r2
	ldr r2, .L_0200b220
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	adds r2, r5, #0
	adds r2, #92
	movs r3, #3
	strb r3, [r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b198
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Func_02003b0c
	b .L_0200b1f2
.L_0200b198:
	adds r0, r5, #0
	bl Func_02003c04
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r6, r6, r3
	str r6, [sp, #4]
	add r8, r10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r7]
	bl Func_02003adc
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	mov r10, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	mov r0, r9
	bl Func_02003b3c
	ldr r6, [sp, #4]
	asrs r5, r5, #19
	movs r3, #128
	adds r5, #4
	adds r2, r5, #0
	strb r3, [r6, #3]
	ldrb r0, [r7]
	mov r1, r10
	bl Func_02003c24
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b1f2
	mov r0, r9
	movs r1, #9
	bl ObjectVisual_CopyAttributes
.L_0200b1f2:
	movs r3, #4
	add r11, r3
.L_0200b1f6:
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200b142
	movs r0, #10
	bl wait
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b218:
	.4byte gMainRoutineWork
.L_0200b21c:
	.4byte 0xfdff0000
.L_0200b220:
	.4byte gMapShapeGrid
	.section .text.x0200b8dc,"ax",%progbits
	.balign 4
	.global Func_020038dc
	.thumb_func
Func_020038dc:
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
	ldr r0, .L_0200b990
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_02003a14
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
.L_0200b934:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_02003c34
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
	bge .L_0200b934
	movs r1, #176
	lsls r1, r1, #5
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200b994
	bl Task_Add
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b990:
	.4byte 0x000001f0
.L_0200b994:
	.4byte Func_0200379c
