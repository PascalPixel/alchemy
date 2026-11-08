.syntax unified
	.thumb
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
