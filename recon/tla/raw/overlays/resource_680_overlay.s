.syntax unified
	.thumb
	.section .text.x02008528,"ax",%progbits
	.global Func_02000528
	.thumb_func
Func_02000528:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r1, #0
	movs r2, #20
	sub sp, #12
	adds r7, r0, #0
	mov r8, r1
	mov r10, r2
.L_0200853e:
	ldr r3, [r7, #8]
	mov r5, sp
	str r3, [r5]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	mov r2, r8
	muls r2, r3
	ldr r3, [r7, #12]
	movs r1, #128
	adds r3, r3, r2
	lsls r1, r1, #11
	adds r3, r3, r1
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	movs r2, #192
	adds r1, r0, #0
	lsls r2, r2, #10
	lsls r0, r6, #2
	adds r0, r0, r6
	mov r9, r2
	add r0, r9
	adds r2, r5, #0
	bl Vector_AddPolarOffsetFar
	movs r0, #154
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	lsls r0, r0, #1
	bl Actor_Spawn
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020085d0
	ldr r3, .L_020085cc
	mov r1, r10
	str r3, [r6, #108]
	adds r3, r6, #0
	adds r3, #100
	strh r1, [r3]
	movs r2, #0
	adds r3, #2
	strh r2, [r3]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	mov r2, r9
	str r3, [r6, #72]
	str r2, [r6, #40]
	bl Random16Far
	ldr r5, .L_020085c8
	adds r3, r6, #0
	adds r3, #35
	strh r0, [r6, #6]
	movs r1, #2
	strb r5, [r3]
	adds r0, r6, #0
	bl Func_02000e5c
	b .L_020085d0
	.2byte 0x0000
.L_020085c8:
	.4byte 0x00000000
.L_020085cc:
	.4byte Func_02000474
.L_020085d0:
	movs r1, #1
	movs r3, #2
	add r8, r1
	negs r3, r3
	mov r2, r8
	add r10, r3
	cmp r2, #15
	ble .L_0200853e
	movs r0, #0
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020085f0,"ax",%progbits
	.global Func_020005f0
	.thumb_func
Func_020005f0:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #164
	ldr r2, [r3]
	movs r4, #0
	movs r1, #4
	ldrsh r3, [r2, r1]
	adds r0, r4, #0
	cmp r3, #8
	bne .L_0200860a
	adds r4, r2, #4
	b .L_02008622
.L_0200860a:
	adds r0, #1
	cmp r0, #7
	bgt .L_02008622
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r1, r3, #2
	adds r3, r1, #4
	ldrsh r3, [r2, r3]
	cmp r3, #8
	bne .L_0200860a
	adds r3, r2, r1
	adds r4, r3, #4
.L_02008622:
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r4, #16]
	movs r3, #64
	str r3, [r4, #4]
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008630,"ax",%progbits
	.global Func_02000630
	.thumb_func
Func_02000630:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #164
	ldr r2, [r3]
	movs r4, #0
	movs r1, #4
	ldrsh r3, [r2, r1]
	adds r0, r4, #0
	cmp r3, #8
	bne .L_0200864a
	adds r4, r2, #4
	b .L_02008662
.L_0200864a:
	adds r0, #1
	cmp r0, #7
	bgt .L_02008662
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r1, r3, #2
	adds r3, r1, #4
	ldrsh r3, [r2, r3]
	cmp r3, #8
	bne .L_0200864a
	adds r3, r2, r1
	adds r4, r3, #4
.L_02008662:
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r4, #16]
	ldr r3, [r4, #4]
	adds r3, #128
	str r3, [r4, #4]
	pop {r5, pc}
	.section .text.x020086ec,"ax",%progbits
	.balign 4
	.global Func_020006ec
	.thumb_func
Func_020006ec:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #192
	lsls r1, r1, #18
	ldr r3, [r1, #108]
	ldr r6, .L_0200877c
	movs r2, #214
	lsls r2, r2, #1
	mov r8, r1
	movs r1, #152
	adds r3, r3, r2
	ldr r5, .L_02008780
	.ifdef TLA_EDITION_JA
	adds r2, #85
	.else
	adds r2, #88
	.endif
	lsls r1, r1, #2
	str r2, [r3]
	adds r3, r6, r1
	movs r1, #5
	mov r10, r1
	.ifdef TLA_EDITION_JA
	adds r2, #97
	.else
	adds r2, #94
	.endif
	strh r5, [r3]
	adds r3, r6, r2
	mov r2, r10
	strh r2, [r3]
	sub sp, #8
	bl Func_02000f0c
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	movs r1, #240
	orrs r3, r2
	lsls r1, r1, #1
	strb r3, [r0]
	adds r3, r6, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r7, .L_02008778
	cmp r2, r5
	beq .L_02008744
	b .L_0200893c
.L_02008744:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008784
	bl Task_Add
	movs r0, #8
	movs r1, #1
	bl Func_02000f14
	movs r0, #9
	movs r1, #1
	bl Func_02000f14
	movs r0, #10
	movs r1, #1
	bl Func_02000f14
	movs r0, #11
	movs r1, #1
	bl Func_02000f14
	movs r0, #12
	movs r1, #1
	bl Func_02000f14
	b .L_02008788
.L_02008778:
	.4byte 0x00000000
.L_0200877c:
	.4byte gSequenceParentWork
.L_02008780:
	.4byte 0x000000ae
.L_02008784:
	.4byte Func_02000670
.L_02008788:
	movs r0, #13
	movs r1, #1
	bl Func_02000f14
	movs r0, #14
	movs r1, #1
	bl Func_02000f14
	movs r0, #15
	movs r1, #1
	bl Func_02000f14
	movs r0, #146
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020087e0
	movs r0, #16
	bl Actor_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #85
	strb r7, [r3]
	movs r3, #176
	lsls r3, r3, #15
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #16
	str r3, [r5, #12]
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r3, #27
	mov r2, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #6
	movs r1, #27
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
.L_020087e0:
	movs r0, #19
	bl Actor_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	movs r0, #144
	adds r3, #35
	lsls r0, r0, #4
	strb r7, [r3]
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008840
	adds r3, r5, #0
	adds r3, #85
	strb r7, [r3]
	movs r3, #162
	lsls r3, r3, #18
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r5, #12]
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r3, #37
	movs r2, #102
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #102
	movs r2, #1
	movs r3, #2
	bl Func_02000e44
	movs r3, #40
	movs r2, #46
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #27
	movs r1, #44
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
	b .L_0200888a
.L_02008840:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200888a
	ldr r3, [r5, #8]
	asrs r3, r3, #19
	cmp r3, #75
	beq .L_02008868
	movs r3, #37
	movs r2, #102
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #102
	movs r2, #1
	movs r3, #2
	bl Func_02000e44
.L_02008868:
	ldr r3, [r5, #8]
	asrs r3, r3, #19
	cmp r3, #79
	bne .L_0200888a
	adds r3, r5, #0
	adds r3, #85
	strb r7, [r3]
	movs r2, #46
	movs r3, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #46
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
.L_0200888a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200889a
	b .L_02008a36
.L_0200889a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020088aa
	b .L_02008a36
.L_020088aa:
	movs r2, #200
	movs r0, #20
	ldr r1, .L_02008a44
	lsls r2, r2, #18
	bl Func_02000e8c
	movs r5, #49
	movs r0, #53
	movs r1, #52
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02000e44
	movs r3, #50
	str r3, [sp, #0]
	movs r0, #53
	movs r1, #52
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02000e44
	movs r0, #20
	movs r1, #5
	bl Actor_SetModeById
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #97
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020088f2
	b .L_02008a36
.L_020088f2:
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #20
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #1
	movs r0, #20
	bl Func_02000ed4
	movs r0, #30
	bl FieldEvent_Wait
	ldr r0, .L_02008a48
	bl Func_02000ea4
	movs r0, #20
	movs r1, #0
	bl Func_02000eac
	movs r1, #2
	movs r0, #20
	bl Actor_StartRepeatedMotion
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #97
	bl GameFlag_SetBit
	bl Func_02000e7c
	b .L_02008a36
.L_0200893c:
	ldr r3, .L_02008a4c
	cmp r2, r3
	bne .L_020089e8
	movs r0, #8
	movs r1, #1
	bl Func_02000f14
	movs r0, #9
	movs r1, #1
	bl Func_02000f14
	movs r0, #10
	movs r1, #1
	bl Func_02000f14
	movs r0, #11
	movs r1, #1
	bl Func_02000f14
	movs r0, #12
	movs r1, #1
	bl Func_02000f14
	movs r0, #13
	movs r1, #1
	bl Func_02000f14
	movs r1, #1
	movs r0, #14
	bl Func_02000f14
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_02008992
	bl Func_02000dd0
.L_02008992:
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020089ae
	movs r1, #191
	movs r3, #212
	lsls r1, r1, #18
	ldr r2, .L_02008a50
	lsls r3, r3, #17
	movs r0, #17
	bl Func_02000b2c
.L_020089ae:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020089ca
	movs r3, #212
	ldr r1, .L_02008a54
	ldr r2, .L_02008a58
	lsls r3, r3, #17
	movs r0, #18
	bl Func_02000b2c
.L_020089ca:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a36
	movs r3, #212
	ldr r1, .L_02008a5c
	ldr r2, .L_02008a60
	lsls r3, r3, #17
	movs r0, #19
	bl Func_02000b2c
	b .L_02008a36
.L_020089e8:
	ldr r3, .L_02008a64
	cmp r2, r3
	bne .L_02008a36
	movs r0, #64
	bl Actor_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008a04
	movs r0, #64
	movs r1, #0
	movs r2, #0
	bl Func_02000e8c
.L_02008a04:
	mov r1, r8
	ldr r3, [r1, #108]
	movs r2, #138
	ldr r5, [r3, #84]
	lsls r2, r2, #1
	adds r3, r3, r2
	str r5, [r3]
	movs r0, #8
	bl Actor_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #85
	strb r7, [r3]
	movs r2, #13
	ldr r3, [r5, #16]
	movs r0, #11
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r1, #14
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
.L_02008a36:
	movs r0, #0
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a44:
	.4byte 0x031e0000
.L_02008a48:
	.4byte MsgIMNot
.L_02008a4c:
	.4byte 0x000000af
.L_02008a50:
	.4byte 0xfe790000
.L_02008a54:
	.4byte 0x03150000
.L_02008a58:
	.4byte 0xff080000
.L_02008a5c:
	.4byte 0x030a0000
.L_02008a60:
	.4byte 0xff890000
.L_02008a64:
	.4byte 0x000000b0
	.section .text.x02008ab4,"ax",%progbits
	.balign 4
	.global Func_02000ab4
	.thumb_func
Func_02000ab4:
	push {r5, r6, lr}
	ldr r2, .L_02008b20
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r2, r0
	ldrb r3, [r3]
	cmp r3, #2
	bne .L_02008b1e
	movs r3, #192
	movs r1, #133
	lsls r3, r3, #18
	lsls r1, r1, #2
	ldr r6, [r3, #108]
	ldr r5, [r3, #32]
	adds r3, r2, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	bge .L_02008ae4
	ldr r2, .L_02008b24
	adds r3, r3, r2
.L_02008ae4:
	ldr r2, [r0, #16]
	asrs r1, r3, #20
	ldr r3, [r0, #12]
	subs r0, r2, r3
	movs r2, #254
	lsls r2, r2, #7
	adds r2, #255
	adds r3, r0, r2
	cmp r3, #0
	bge .L_02008afc
	ldr r2, .L_02008b28
	adds r3, r0, r2
.L_02008afc:
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
	bhi .L_02008b1e
	movs r1, #170
	lsls r1, r1, #1
	adds r3, r6, r1
	strh r2, [r3]
.L_02008b1e:
	pop {r5, r6, pc}
.L_02008b20:
	.4byte gSequenceParentWork
.L_02008b24:
	.4byte 0x000fffff
.L_02008b28:
	.4byte 0x00107ffe
	.section .text.x02008b2c,"ax",%progbits
	.global Func_02000b2c
	.thumb_func
Func_02000b2c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r1, #0
	mov r8, r2
	adds r6, r3, #0
	bl Actor_GetById
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
	bge .L_02008b56
	ldr r2, .L_02008bb4
	adds r3, r5, r2
.L_02008b56:
	asrs r7, r3, #20
	mov r3, r8
	subs r2, r6, r3
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	adds r1, r2, r3
	cmp r1, #0
	bge .L_02008b6c
	ldr r3, .L_02008bb8
	adds r1, r2, r3
.L_02008b6c:
	asrs r1, r1, #20
	lsls r3, r1, #7
	adds r3, r7, r3
	lsls r3, r3, #2
	adds r0, r0, r3
	movs r3, #255
	strb r3, [r0, #2]
	ldr r0, .L_02008bbc
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
	bl Actor_SetSingleChildFlags
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008bb4:
	.4byte 0x000fffff
.L_02008bb8:
	.4byte 0x00107ffe
.L_02008bbc:
	.4byte 0xfff00000
	.section .text.x02008bc0,"ax",%progbits
	.global Func_02000bc0
	.thumb_func
Func_02000bc0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	ldr r3, .L_02008dc0
	str r1, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	bl Actor_GetById
	adds r5, r0, #0
	ldr r3, [r5, #72]
	str r3, [sp, #8]
	bl Func_02000e74
	movs r0, #0
	bl Func_02000ef4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	adds r0, r6, #0
	bl Actor_GetById
	ldr r2, [r5, #12]
	adds r7, r0, #0
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	adds r0, r6, #0
	bl Func_02000b2c
	ldr r1, .L_02008dc4
	adds r0, r7, #0
	bl Func_02000e2c
	movs r2, #15
	mov r11, r2
.L_02008c24:
	ldr r2, [r5, #12]
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	movs r0, #255
	bl Func_02000e34
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02008c9c
	ldr r1, .L_02008dc8
	bl Func_02000e2c
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
	ldr r2, .L_02008dcc
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
.L_02008c9c:
	movs r2, #1
	negs r2, r2
	add r11, r2
	mov r3, r11
	cmp r3, #0
	bge .L_02008c24
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #78
	bl Func_02000f24
	movs r0, #217
	bl Func_02000f24
	movs r2, #230
	movs r0, #128
	movs r1, #128
	lsls r2, r2, #8
	lsls r0, r0, #10
	lsls r1, r1, #10
	adds r2, #102
	bl Func_02000e54
	ldr r3, .L_02008dc0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #2
	ldr r0, [r3]
	adds r1, #255
	bl Func_02000ebc
	movs r1, #40
	adds r0, r5, #0
	bl Func_02000e24
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #204
	bl Func_02000f24
	adds r2, r5, #0
	movs r3, #3
	adds r2, #85
	strb r3, [r2]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	movs r6, #0
	str r3, [r5, #72]
	b .L_02008d06
.L_02008d04:
	adds r6, #1
.L_02008d06:
	cmp r6, #179
	bgt .L_02008d18
	movs r0, #1
	bl wait
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	cmp r2, r3
	bgt .L_02008d04
.L_02008d18:
	movs r0, #188
	bl Func_02000f24
	ldr r3, [sp, #8]
	ldr r6, .L_02008dc0
	str r3, [r5, #72]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	movs r1, #0
	bl Func_02000ebc
	adds r0, r5, #0
	movs r1, #38
	bl Func_02000e24
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_02000e54
	movs r0, #10
	bl wait
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02000e54
	movs r0, #20
	bl wait
	ldr r2, [r5, #16]
	movs r3, #1
	ldr r1, [r5, #12]
	ldr r0, [r5, #8]
	bl Camera_MoveWithinMapBounds
	bl Func_02000ecc
	movs r0, #20
	bl FieldEvent_Wait
	adds r0, r5, #0
	movs r1, #1
	bl Actor_SetSingleChildFlags
	movs r1, #1
	adds r0, r5, #0
	bl Func_02000e24
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r0, #20
	bl wait
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r2, r6, r3
	movs r3, #0
	strb r3, [r2]
	bl Func_02000e7c
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
.L_02008dc0:
	.4byte gSequenceParentWork
.L_02008dc4:
	.4byte gData_020016cc
.L_02008dc8:
	.4byte gData_02001688
.L_02008dcc:
	.4byte IwramMulQ16
