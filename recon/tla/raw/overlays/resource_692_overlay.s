.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080ce,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x020086dc,"ax",%progbits
	.balign 4
	.global Func_020006dc
	.thumb_func
Func_020006dc:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	b .L_0200874c
.L_020086e2:
	ldrh r0, [r6]
	bl Actor_GetById
	adds r5, r0, #0
	adds r7, r5, #0
	movs r3, #0
	adds r7, #99
	strb r3, [r7]
	movs r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	bl Func_02000658
	movs r3, #64
	ands r0, r3
	cmp r0, #0
	beq .L_0200874a
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	cmp r0, #0
	beq .L_02008758
	movs r3, #212
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r5, #16]
	ldr r3, [r5, #12]
	ldr r1, [r5, #8]
	subs r2, r2, r3
	asrs r2, r2, #20
	adds r3, r2, #1
	asrs r1, r1, #20
	ldr r4, [r0]
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	subs r2, #1
	movs r3, #255
	strb r3, [r4, #2]
	lsls r2, r2, #7
	ldr r4, [r0]
	adds r1, r1, r2
	lsls r1, r1, #2
	movs r3, #1
	negs r3, r3
	adds r4, r4, r1
	strb r3, [r4, #2]
	movs r3, #1
	strb r3, [r7]
.L_0200874a:
	adds r6, #2
.L_0200874c:
	movs r2, #255
	ldrh r3, [r6]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_020086e2
.L_02008758:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200875c,"ax",%progbits
	.global Func_0200075c
	.thumb_func
Func_0200075c:
	push {r5, lr}
	adds r5, r0, #0
	movs r2, #99
	adds r2, r2, r5
	ldrb r3, [r2]
	mov r12, r2
	cmp r3, #0
	beq .L_020087aa
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	cmp r0, #0
	beq .L_020087aa
	movs r3, #212
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r5, #16]
	ldr r3, [r5, #12]
	ldr r1, [r5, #8]
	subs r2, r2, r3
	asrs r2, r2, #20
	adds r3, r2, #1
	asrs r1, r1, #20
	ldr r4, [r0]
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	subs r2, #1
	movs r3, #0
	strb r3, [r4, #2]
	lsls r2, r2, #7
	ldr r4, [r0]
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r4, r4, r1
	mov r2, r12
	strb r3, [r4, #2]
	strb r3, [r2]
.L_020087aa:
	pop {r5, pc}
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
	.section .text.x02008a30,"ax",%progbits
	.global Func_02000a30
	.thumb_func
Func_02000a30:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #188
	adds r3, r3, r1
	ldr r7, [r3]
	sub sp, #8
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	bl Func_02004ee8
	ldr r3, .L_02008b0c
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008b10
	cmp r2, r3
	bne .L_02008b3a
	ldr r0, .L_02008b14
	bl Func_020006dc
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #2
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	cmp r0, r3
	bne .L_02008a84
	b .L_02008bf4
.L_02008a84:
	movs r2, #34
	adds r2, r2, r7
	movs r3, #2
	strb r3, [r2]
	movs r3, #85
	adds r3, r3, r7
	mov r8, r3
	mov r1, r8
	movs r3, #3
	movs r0, #1
	strb r3, [r1]
	mov r10, r2
	bl wait
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008b22
	movs r1, #128
	lsls r1, r1, #5
	str r1, [r7, #72]
	adds r0, r7, #0
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #2
	orrs r3, r2
	strb r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r1, [r3]
	movs r6, #128
	ldr r3, .L_02008b08
	lsls r6, r6, #19
	adds r6, #80
	ldr r2, [r7, #16]
	strh r3, [r6]
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r3
	movs r3, #156
	lsls r3, r3, #1
	ldr r1, [r7, #20]
	ldr r0, [r7, #8]
	bl OverlayObject_PrepareObject
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0200063c
	movs r0, #213
	bl Func_02004f30
	adds r0, r5, #0
	bl Func_02004cb0
	adds r0, r7, #0
	movs r1, #4
	bl Func_02004c98
	movs r0, #1
	b .L_02008b18
	.2byte 0x0000
.L_02008b08:
	.4byte 0x00003f10
.L_02008b0c:
	.4byte gMainRoutineWork
.L_02008b10:
	.4byte 0x000000e7
.L_02008b14:
	.4byte gData_02005528
.L_02008b18:
	bl wait
	movs r3, #0
	strh r3, [r6]
	b .L_02008b2e
.L_02008b22:
	adds r0, r7, #0
	bl Func_0200063c
	movs r0, #188
	bl Func_02004f30
.L_02008b2e:
	movs r3, #0
	mov r1, r8
	mov r2, r10
	strb r3, [r1]
	strb r3, [r2]
	b .L_02008bf4
.L_02008b3a:
	ldr r3, .L_02008bd4
	cmp r2, r3
	bne .L_02008bf4
	ldr r0, .L_02008bd8
	bl Func_020006dc
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #2
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	cmp r0, r3
	beq .L_02008bf4
	movs r3, #34
	adds r3, r3, r7
	mov r9, r3
	mov r2, r9
	movs r3, #2
	strb r3, [r2]
	movs r3, #85
	adds r3, r3, r7
	mov r10, r3
	movs r1, #0
	movs r3, #3
	mov r8, r1
	mov r1, r10
	strb r3, [r1]
	movs r0, #1
	bl wait
	movs r2, #128
	lsls r2, r2, #5
	str r2, [r7, #72]
	adds r1, r7, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r5, #2
	orrs r5, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strb r5, [r1]
	movs r6, #128
	strh r2, [r3]
	ldr r3, .L_02008bd0
	lsls r6, r6, #19
	adds r6, #80
	ldr r2, [r7, #16]
	strh r3, [r6]
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r3
	movs r3, #156
	lsls r3, r3, #1
	ldr r1, [r7, #20]
	ldr r0, [r7, #8]
	bl OverlayObject_PrepareObject
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0200063c
	movs r0, #213
	bl Func_02004f30
	adds r0, r5, #0
	bl Func_02004cb0
	movs r1, #4
	adds r0, r7, #0
	bl Func_02004c98
	movs r0, #1
	b .L_02008bdc
.L_02008bd0:
	.4byte 0x00003f10
.L_02008bd4:
	.4byte 0x000000e8
.L_02008bd8:
	.4byte gData_0200552e
.L_02008bdc:
	bl wait
	ldr r5, .L_02008c14
	mov r1, r8
	strh r1, [r6]
	adds r0, r7, #0
	bl Func_0200063c
	mov r2, r10
	mov r3, r9
	strb r5, [r2]
	strb r5, [r3]
.L_02008bf4:
	ldr r3, [r7, #8]
	movs r1, #240
	asrs r6, r3, #20
	ldr r3, [r7, #16]
	lsls r1, r1, #1
	asrs r5, r3, #20
	ldr r3, .L_02008c18
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008c1c
	cmp r2, r3
	bne .L_02008c2a
	cmp r5, #16
	bne .L_02008cc2
	b .L_02008c20
.L_02008c14:
	.4byte 0x00000000
.L_02008c18:
	.4byte gMainRoutineWork
.L_02008c1c:
	.4byte 0x000000e3
.L_02008c20:
	movs r0, #159
	lsls r0, r0, #4
	bl GameFlag_SetBit
	b .L_02008cc2
.L_02008c2a:
	ldr r3, .L_02008cd4
	cmp r2, r3
	bne .L_02008c40
	cmp r5, #41
	bne .L_02008cc2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #241
	bl GameFlag_SetBit
	b .L_02008cc2
.L_02008c40:
	ldr r3, .L_02008cd8
	cmp r2, r3
	bne .L_02008c76
	cmp r6, #21
	bne .L_02008cc2
	cmp r5, #43
	bne .L_02008c58
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #242
	bl GameFlag_SetBit
.L_02008c58:
	cmp r5, #45
	bne .L_02008c66
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #243
	bl GameFlag_SetBit
.L_02008c66:
	cmp r5, #47
	bne .L_02008cc2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #244
	bl GameFlag_SetBit
	b .L_02008cc2
.L_02008c76:
	ldr r3, .L_02008cdc
	cmp r2, r3
	bne .L_02008cae
	cmp r6, #47
	bne .L_02008c9e
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #245
	bl GameFlag_SetBit
	movs r3, #46
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #74
	movs r2, #1
	movs r3, #1
	bl Func_02004ce8
.L_02008c9e:
	cmp r6, #57
	bne .L_02008cc2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #246
	bl GameFlag_SetBit
	b .L_02008cc2
.L_02008cae:
	ldr r3, .L_02008ce0
	cmp r2, r3
	bne .L_02008cc2
	cmp r6, #43
	bne .L_02008cc2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #247
	bl GameFlag_SetBit
.L_02008cc2:
	bl Func_02004d48
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008cd4:
	.4byte 0x000000e4
.L_02008cd8:
	.4byte 0x000000e5
.L_02008cdc:
	.4byte 0x000000e7
.L_02008ce0:
	.4byte 0x000000e8
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
	.section .text.x020091fc,"ax",%progbits
	.balign 4
	.global Func_020011fc
	.thumb_func
Func_020011fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009344
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #12
	bl Actor_GetById
	adds r5, r0, #0
	ldrh r1, [r5, #6]
	movs r3, #128
	lsls r3, r3, #6
	adds r1, r1, r3
	movs r3, #192
	lsls r3, r3, #8
	ldr r2, .L_02009348
	ands r1, r3
	ldr r3, [r5, #8]
	movs r0, #128
	lsls r0, r0, #12
	ands r3, r2
	mov r6, sp
	adds r3, r3, r0
	str r3, [r6]
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r2
	adds r3, r3, r0
	movs r0, #128
	lsls r0, r0, #14
	adds r2, r6, #0
	str r3, [r6, #8]
	bl Vector_AddPolarOffsetFar
	adds r0, r6, #0
	adds r1, r5, #0
	bl Actor_FindInCell
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02009336
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	movs r1, #6
	adds r0, r5, #0
	bl Func_02004c98
	movs r0, #6
	bl wait
	movs r0, #152
	bl Func_02004f30
	adds r0, r5, #0
	movs r1, #7
	bl Func_02004c98
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	ldr r3, .L_0200934c
	movs r2, #85
	str r3, [r5, #40]
	adds r2, r2, r5
	mov r10, r2
	ldrb r2, [r2]
	movs r3, #126
	ands r3, r2
	mov r2, r10
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r3, [r5, #8]
	ldr r0, [r6]
	movs r1, #12
	subs r0, r0, r3
	bl __divsi3
	ldr r3, [r5, #16]
	mov r11, r0
	ldr r0, [r6, #8]
	movs r1, #12
	subs r0, r0, r3
	bl __divsi3
	movs r3, #0
	mov r9, r0
	mov r8, r3
.L_020092ce:
	ldr r2, [r7, #8]
	ldr r3, [r6]
	movs r0, #1
	subs r2, r2, r3
	ldr r3, [r5, #8]
	add r2, r11
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6, #8]
	subs r2, r2, r3
	ldr r3, [r5, #16]
	add r2, r9
	adds r3, r3, r2
	str r3, [r5, #16]
	ldr r3, [r7, #8]
	str r3, [r6]
	ldr r3, [r7, #16]
	str r3, [r6, #8]
	bl wait
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #12
	bne .L_020092ce
	ldr r3, [r7, #8]
	movs r2, #128
	str r3, [r5, #8]
	ldr r3, [r7, #12]
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r5, #20]
	str r3, [r5, #12]
	ldr r3, [r7, #16]
	adds r0, r5, #0
	str r3, [r5, #16]
	movs r1, #6
	bl Func_02004c98
	adds r0, r5, #0
	movs r1, #1
	bl Actor_SetSingleChildFlags
	mov r3, r10
	ldrb r2, [r3]
	movs r3, #3
	orrs r3, r2
	mov r2, r10
	strb r3, [r2]
	bl Func_02004d48
.L_02009336:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009344:
	.4byte gMainRoutineWork
.L_02009348:
	.4byte 0xfff00000
.L_0200934c:
	.4byte 0x0004cccc
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
	.section .text.x02009d64,"ax",%progbits
	.global Func_02001d64
	.thumb_func
Func_02001d64:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #68
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	movs r0, #107
	bl Func_02004f30
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02004d10
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #194
	movs r1, #1
	movs r2, #136
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	bl Camera_MoveWithinMapBounds
	bl Func_02004e20
	movs r3, #148
	add r6, sp, #16
	adds r3, #255
	strh r3, [r6, #24]
	movs r3, #2
	str r3, [r6]
	movs r3, #10
	str r3, [r6, #4]
	ldr r3, .L_0200a11c
	movs r2, #0
	str r3, [r6, #28]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r6, #12]
	str r3, [r6, #8]
	add r3, sp, #56
	str r2, [r3, #4]
	mov r8, r3
	movs r7, #0
.L_02009dce:
	bl Random16Far
	movs r5, #31
	ands r0, r5
	movs r2, #190
	lsls r2, r2, #18
	lsls r0, r0, #16
	adds r0, r0, r2
	mov r3, r8
	str r0, [r3]
	bl Random16Far
	adds r2, r0, #0
	ands r2, r5
	movs r3, #128
	lsls r3, r3, #17
	lsls r2, r2, #16
	adds r2, r2, r3
	mov r3, r8
	str r2, [r3, #8]
	ldr r0, [r3]
	ldr r1, [r3, #4]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [sp, #0]
	movs r3, #236
	lsls r3, r3, #14
	str r3, [sp, #8]
	movs r5, #0
	movs r3, #0
	str r5, [sp, #4]
	str r6, [sp, #12]
	bl Func_0200015c
	movs r3, #7
	ands r3, r7
	cmp r3, #0
	bne .L_02009e24
	movs r0, #156
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004f30
.L_02009e24:
	cmp r7, #32
	bne .L_02009e68
	movs r1, #194
	movs r2, #248
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #186
	lsls r1, r1, #2
	movs r2, #216
	movs r0, #13
	bl Actor_ResetAndSetPosition
	movs r0, #13
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #13
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #13
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #13
	b .L_0200a04c
.L_02009e68:
	cmp r7, #40
	bne .L_02009eae
	movs r1, #194
	movs r2, #248
	movs r0, #20
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #194
	movs r2, #164
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #20
	bl Actor_ResetAndSetPosition
	movs r0, #20
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #20
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #20
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #20
	b .L_0200a04c
.L_02009eae:
	cmp r7, #48
	bne .L_02009ef2
	movs r1, #194
	movs r2, #248
	movs r0, #14
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #214
	lsls r1, r1, #2
	movs r2, #232
	movs r0, #14
	bl Actor_ResetAndSetPosition
	movs r0, #14
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #14
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #14
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #14
	b .L_0200a04c
.L_02009ef2:
	cmp r7, #56
	bne .L_02009f38
	movs r1, #194
	movs r2, #248
	movs r0, #19
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #174
	movs r2, #156
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #19
	bl Actor_ResetAndSetPosition
	movs r0, #19
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #19
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #19
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #19
	b .L_0200a04c
.L_02009f38:
	cmp r7, #64
	bne .L_02009f7e
	movs r1, #194
	movs r2, #248
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #182
	movs r2, #140
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #15
	bl Actor_ResetAndSetPosition
	movs r0, #15
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #15
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #15
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #15
	b .L_0200a04c
.L_02009f7e:
	cmp r7, #72
	bne .L_02009fc4
	movs r1, #194
	movs r2, #248
	movs r0, #18
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #206
	movs r2, #140
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #18
	bl Actor_ResetAndSetPosition
	movs r0, #18
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #18
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #18
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #18
	b .L_0200a04c
.L_02009fc4:
	cmp r7, #80
	bne .L_0200a00a
	movs r1, #194
	movs r2, #248
	movs r0, #16
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #206
	movs r2, #156
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #16
	bl Actor_ResetAndSetPosition
	movs r0, #16
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #16
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #16
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #16
	b .L_0200a04c
.L_0200a00a:
	cmp r7, #88
	bne .L_0200a056
	movs r1, #194
	movs r2, #248
	movs r0, #17
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #194
	lsls r1, r1, #2
	movs r2, #232
	movs r0, #17
	bl Actor_ResetAndSetPosition
	movs r0, #17
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #17
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #17
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #17
.L_0200a04c:
	bl Actor_GetById
	ldr r3, .L_0200a120
	str r3, [r0, #108]
	b .L_0200a0c4
.L_0200a056:
	cmp r7, #112
	bne .L_0200a0c4
	movs r1, #194
	movs r2, #248
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #174
	movs r0, #12
	lsls r1, r1, #2
	movs r2, #232
	bl Actor_ResetAndSetPosition
	movs r0, #174
	movs r1, #1
	movs r2, #232
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	movs r0, #12
	bl Actor_GetById
	movs r3, #208
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #12
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #12
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #12
	bl Actor_GetById
	ldr r3, .L_0200a120
	movs r2, #230
	str r3, [r0, #108]
	movs r1, #1
	movs r0, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004d10
.L_0200a0c4:
	movs r0, #3
	adds r7, #1
	bl wait
	cmp r7, #128
	beq .L_0200a0d2
	b .L_02009dce
.L_0200a0d2:
	movs r0, #12
	bl Actor_WaitForMoveById
	bl Func_02004d18
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02004f30
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #248
	bl GameFlag_SetBit
	ldr r3, .L_0200a124
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
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a11c:
	.4byte gData_02005050
.L_0200a120:
	.4byte Func_02001d04
.L_0200a124:
	.4byte gMainRoutineWork
	.section .text.x0200b678,"ax",%progbits
	.global Func_02003678
	.thumb_func
Func_02003678:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	ldr r5, .L_0200b700
	str r2, [r3]
	subs r2, #36
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b704
	sub sp, #8
	cmp r2, r3
	bne .L_0200b70c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	movs r5, #192
	orrs r3, r2
	lsls r5, r5, #2
	strb r3, [r0]
	ldr r1, .L_0200b708
	movs r0, #8
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b6d4
	bl .L_0200bec6
.L_0200b6d4:
	ldr r3, .L_0200b6f8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200b6fc
	subs r2, #2
	strh r3, [r2]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #71
	movs r1, #64
	movs r2, #47
	movs r3, #98
	bl Func_02004cd8
	b .L_0200bec6
.L_0200b6f8:
	.4byte 0x00001000
.L_0200b6fc:
	.4byte 0x00003f44
.L_0200b700:
	.4byte gMainRoutineWork
.L_0200b704:
	.4byte 0x000000e2
.L_0200b708:
	.4byte gData_02005074
.L_0200b70c:
	ldr r3, .L_0200b8dc
	cmp r2, r3
	bne .L_0200b7ae
	movs r0, #0
	bl Func_02004e60
	movs r0, #15
	bl Actor_GetById
	ldr r3, .L_0200b8e0
	movs r5, #0
	str r3, [r0, #108]
.L_0200b724:
	adds r0, r5, #0
	adds r0, #9
	bl Actor_GetById
	adds r2, r0, #0
	movs r3, #0
	adds r2, #85
	adds r5, #1
	strb r3, [r2]
	str r3, [r0, #20]
	str r3, [r0, #12]
	cmp r5, #6
	bne .L_0200b724
	movs r0, #159
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b756
	movs r0, #15
	bl Actor_GetById
	movs r3, #132
	lsls r3, r3, #17
	str r3, [r0, #16]
.L_0200b756:
	ldr r0, .L_0200b8e4
	bl Func_02004ed8
	ldr r0, .L_0200b8e8
	bl Func_02000880
	ldr r3, .L_0200b8ec
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #16
	bl Actor_GetById
	adds r2, r0, #0
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r0, #20]
	str r3, [r0, #12]
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b7a8
	b .L_0200bb24
.L_0200b7a8:
	bl Func_020034bc
	b .L_0200bec6
.L_0200b7ae:
	ldr r3, .L_0200b8f0
	cmp r2, r3
	bne .L_0200b84c
	movs r0, #0
	bl Func_02004e60
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #241
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b7d4
	movs r0, #11
	bl Actor_GetById
	movs r3, #166
	lsls r3, r3, #18
	str r3, [r0, #16]
.L_0200b7d4:
	ldr r0, .L_0200b8f4
	bl Func_02004ed8
	bl Func_02004ec0
	movs r1, #13
	movs r2, #14
	movs r0, #0
	bl Func_02004ec8
	ldr r0, .L_0200b8e8
	bl Func_02000880
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Actor_GetById
	adds r2, r0, #0
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r0, #20]
	str r3, [r0, #12]
	adds r0, #35
	ldrb r2, [r0]
	movs r5, #192
	movs r3, #128
	lsls r5, r5, #2
	orrs r3, r2
	adds r5, #1
	strb r3, [r0]
	ldr r1, .L_0200b8f8
	movs r0, #10
	adds r2, r5, #0
	bl Func_02000800
	movs r0, #12
	ldr r1, .L_0200b8fc
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b84a
	b .L_0200bb24
.L_0200b84a:
	b .L_0200b8aa
.L_0200b84c:
	ldr r3, .L_0200b900
	cmp r2, r3
	beq .L_0200b854
	b .L_0200ba84
.L_0200b854:
	movs r0, #0
	bl Func_02004e60
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	ldr r0, .L_0200b904
	bl Func_02000880
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	movs r6, #0
	cmp r3, #7
	bgt .L_0200b910
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #1
	ldr r1, .L_0200b908
	movs r0, #10
	adds r2, r5, #0
	bl Func_02000800
	movs r0, #12
	ldr r1, .L_0200b90c
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b8b0
.L_0200b8aa:
	bl Func_020034f0
	b .L_0200bec6
.L_0200b8b0:
	ldr r3, .L_0200b8d4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200b8d8
	subs r2, #2
	strh r3, [r2]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #71
	movs r1, #64
	movs r2, #35
	movs r3, #82
	bl Func_02004cd8
	b .L_0200bec6
.L_0200b8d4:
	.4byte 0x00001000
.L_0200b8d8:
	.4byte 0x00003f44
.L_0200b8dc:
	.4byte 0x000000e3
.L_0200b8e0:
	.4byte Func_02003630
.L_0200b8e4:
	.4byte gData_020054fc
.L_0200b8e8:
	.4byte gData_02005500
.L_0200b8ec:
	.4byte gMainRoutineWork
.L_0200b8f0:
	.4byte 0x000000e4
.L_0200b8f4:
	.4byte gData_02005508
.L_0200b8f8:
	.4byte gData_020050d4
.L_0200b8fc:
	.4byte gData_02005134
.L_0200b900:
	.4byte 0x000000e5
.L_0200b904:
	.4byte gData_0200550c
.L_0200b908:
	.4byte gData_0200519c
.L_0200b90c:
	.4byte gData_020051fc
.L_0200b910:
	movs r0, #23
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	movs r0, #24
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #25
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #3
	orrs r3, r5
	strb r3, [r0]
	movs r0, #23
	bl Actor_SetSpritePriority
	movs r0, #24
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r0, #25
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #242
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b96e
	movs r0, #23
	bl Actor_GetById
	movs r3, #172
	lsls r3, r3, #17
	str r3, [r0, #8]
.L_0200b96e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #243
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b988
	movs r0, #24
	bl Actor_GetById
	movs r3, #172
	lsls r3, r3, #17
	str r3, [r0, #8]
.L_0200b988:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #244
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b9a2
	movs r0, #25
	bl Actor_GetById
	movs r3, #172
	lsls r3, r3, #17
	str r3, [r0, #8]
.L_0200b9a2:
	ldr r0, .L_0200bb40
	bl Func_02004ed8
	movs r0, #26
	bl Actor_GetById
	adds r0, #85
	strb r6, [r0]
	movs r0, #26
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #26
	bl Actor_GetById
	str r6, [r0, #20]
	str r6, [r5, #12]
	movs r0, #15
	bl Actor_GetById
	movs r5, #3
	adds r0, #92
	strb r5, [r0]
	movs r0, #16
	bl Actor_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #17
	bl Actor_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #18
	bl Actor_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #19
	bl Actor_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #20
	bl Actor_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #21
	bl Actor_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #22
	bl Actor_GetById
	adds r0, #92
	ldr r6, .L_0200bb44
	strb r5, [r0]
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #2
	adds r1, r6, #0
	movs r0, #15
	adds r2, r5, #0
	bl Func_02000800
	adds r1, r6, #0
	movs r0, #16
	adds r2, r5, #0
	bl Func_02000800
	ldr r6, .L_0200bb48
	movs r0, #17
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	adds r1, r6, #0
	movs r0, #18
	adds r2, r5, #0
	bl Func_02000800
	ldr r6, .L_0200bb4c
	movs r0, #19
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	adds r1, r6, #0
	movs r0, #20
	adds r2, r5, #0
	bl Func_02000800
	ldr r6, .L_0200bb50
	movs r0, #21
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	movs r0, #22
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bb24
	bl Func_02003548
	b .L_0200bec6
.L_0200ba84:
	ldr r3, .L_0200bb54
	cmp r2, r3
	bne .L_0200bb58
	movs r0, #0
	bl Func_02004e60
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	movs r0, #10
	bl Func_02004f08
	movs r1, #1
	movs r0, #11
	bl Func_02004f08
	movs r0, #8
	bl Actor_GetById
	movs r1, #2
	mov r8, r1
	adds r3, r0, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	movs r6, #160
	lsls r6, r6, #13
	str r6, [r0, #20]
	str r6, [r0, #12]
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #128
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl Actor_GetById
	adds r3, r0, #0
	adds r3, #85
	mov r1, r8
	strb r1, [r3]
	str r6, [r0, #20]
	str r6, [r0, #12]
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bb24
	movs r1, #22
	movs r2, #26
	movs r3, #0
	movs r0, #0
	bl Func_02000694
	movs r1, #25
	movs r0, #0
	movs r2, #26
	movs r3, #0
	bl Func_02000694
	movs r0, #64
	movs r1, #3
	bl Actor_SetSpritePriority
	b .L_0200bec6
.L_0200bb24:
	ldr r3, .L_0200bb38
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200bb3c
	subs r2, #2
	strh r3, [r2]
	b .L_0200bec6
	.2byte 0x0000
.L_0200bb38:
	.4byte 0x00001000
.L_0200bb3c:
	.4byte 0x00003f44
.L_0200bb40:
	.4byte gData_02005520
.L_0200bb44:
	.4byte gData_0200525c
.L_0200bb48:
	.4byte gData_020052bc
.L_0200bb4c:
	.4byte gData_0200531c
.L_0200bb50:
	.4byte gData_0200537c
.L_0200bb54:
	.4byte 0x000000e6
.L_0200bb58:
	ldr r3, .L_0200bc7c
	cmp r2, r3
	beq .L_0200bb60
	b .L_0200bc90
.L_0200bb60:
	movs r0, #0
	bl Func_02004e60
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #245
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bbaa
	movs r3, #46
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #46
	movs r1, #74
	movs r2, #1
	bl Func_02004ce8
	movs r0, #9
	bl Actor_GetById
	movs r3, #190
	lsls r3, r3, #18
	str r3, [r0, #8]
.L_0200bbaa:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #246
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bbc6
	movs r1, #230
	movs r2, #154
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02004da0
.L_0200bbc6:
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #2
	ldr r0, .L_0200bc80
	bl Func_02004ed8
	ldr r1, .L_0200bc84
	movs r0, #8
	adds r2, r5, #0
	bl Func_02000800
	ldr r1, .L_0200bc88
	movs r0, #10
	adds r2, r5, #0
	bl Func_02000800
	movs r0, #11
	ldr r1, .L_0200bc8c
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bc4e
	movs r0, #9
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #246
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bc18
	movs r0, #12
	movs r1, #4
	bl Actor_SetModeById
.L_0200bc18:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #245
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bc34
	movs r3, #255
	lsls r3, r3, #8
	movs r0, #2
	movs r1, #46
	movs r2, #8
	bl Func_02000694
.L_0200bc34:
	movs r1, #10
	movs r2, #45
	movs r3, #0
	movs r0, #0
	bl Func_02000694
	movs r0, #0
	movs r1, #53
	movs r2, #40
	movs r3, #0
	bl Func_02000694
	b .L_0200bec6
.L_0200bc4e:
	ldr r3, .L_0200bc74
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200bc78
	subs r2, #2
	strh r3, [r2]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #71
	movs r1, #64
	movs r2, #50
	movs r3, #102
	bl Func_02004cd8
	b .L_0200bec6
	.2byte 0x0000
.L_0200bc74:
	.4byte 0x00001000
.L_0200bc78:
	.4byte 0x00003f44
.L_0200bc7c:
	.4byte 0x000000e7
.L_0200bc80:
	.4byte gData_02005528
.L_0200bc84:
	.4byte gData_020053dc
.L_0200bc88:
	.4byte gData_0200543c
.L_0200bc8c:
	.4byte gData_0200549c
.L_0200bc90:
	ldr r3, .L_0200bed0
	cmp r2, r3
	beq .L_0200bc98
	b .L_0200bdd4
.L_0200bc98:
	movs r6, #0
.L_0200bc9a:
	adds r0, r6, #0
	adds r0, #8
	bl Actor_GetById
	movs r1, #4
	adds r5, r0, #0
	ldr r7, .L_0200bed4
	bl Func_02004c98
	movs r1, #0
	adds r3, r5, #0
	mov r8, r1
	adds r3, #85
	mov r2, r8
	adds r6, #1
	strb r2, [r3]
	str r7, [r5, #20]
	str r7, [r5, #12]
	cmp r6, #3
	bne .L_0200bc9a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #247
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bce6
	movs r0, #23
	movs r1, #4
	bl Actor_SetModeById
	movs r1, #174
	movs r2, #162
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02004da0
.L_0200bce6:
	ldr r0, .L_0200bed8
	bl Func_02004ed8
	movs r0, #0
	bl Func_02004e60
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bd0a
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
.L_0200bd0a:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bd20
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
.L_0200bd20:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bd62
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bd62
	movs r0, #11
	bl Actor_GetById
	mov r3, r8
	adds r0, #85
	strb r3, [r0]
	movs r0, #11
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #11
	bl Actor_GetById
	movs r1, #4
	str r7, [r0, #20]
	str r7, [r5, #12]
	movs r0, #11
	bl Actor_SetModeById
	b .L_0200bd70
.L_0200bd62:
	movs r3, #255
	lsls r3, r3, #8
	movs r0, #0
	movs r1, #38
	movs r2, #18
	bl Func_02000694
.L_0200bd70:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #249
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bd86
	movs r0, #12
	movs r1, #0
	bl Actor_SetModeById
.L_0200bd86:
	movs r6, #0
.L_0200bd88:
	adds r0, r6, #0
	adds r0, #13
	movs r1, #2
	adds r6, #1
	bl Actor_SetModeById
	cmp r6, #4
	bne .L_0200bd88
	movs r6, #0
.L_0200bd9a:
	adds r0, r6, #0
	adds r0, #12
	bl Actor_GetById
	adds r6, #1
	movs r1, #0
	bl Actor_SetSingleChildFlags
	cmp r6, #11
	bne .L_0200bd9a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #248
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bdbe
	b .L_0200bec6
.L_0200bdbe:
	movs r6, #0
.L_0200bdc0:
	adds r0, r6, #0
	adds r0, #12
	movs r1, #0
	movs r2, #0
	adds r6, #1
	bl Func_02004da0
	cmp r6, #9
	bne .L_0200bdc0
	b .L_0200bec6
.L_0200bdd4:
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200bedc
	cmp r2, r3
	bne .L_0200bec6
	bl Func_0200255c
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r6, #0
.L_0200bdf2:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #2
	adds r0, r6, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200be2a
	lsls r3, r6, #1
	movs r5, #24
	subs r5, r5, r3
	movs r2, #2
	movs r3, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #65
	movs r1, #1
	movs r2, #80
	adds r3, r5, #0
	bl Func_02004cd8
	movs r3, #255
	movs r0, #0
	movs r1, #16
	adds r2, r5, #0
	lsls r3, r3, #8
	bl Func_02000694
.L_0200be2a:
	adds r6, #1
	cmp r6, #6
	bne .L_0200bdf2
	movs r0, #10
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #11
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200be80
	movs r0, #9
	bl Actor_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #9
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #9
	bl Actor_GetById
	ldr r3, .L_0200bed4
	movs r1, #4
	str r3, [r0, #20]
	str r3, [r5, #12]
	movs r0, #9
	bl Actor_SetModeById
	b .L_0200be98
.L_0200be80:
	movs r3, #255
	movs r1, #7
	movs r2, #21
	lsls r3, r3, #8
	movs r0, #0
	bl Func_02000694
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
.L_0200be98:
	movs r0, #8
	bl Actor_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #8
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #8
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r0, #20]
	str r3, [r5, #12]
	movs r0, #8
	bl Actor_GetById
	movs r1, #9
	bl Actor_SetActionCallback
.L_0200bec6:
	movs r0, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200bed0:
	.4byte 0x000000e8
.L_0200bed4:
	.4byte 0xffe00000
.L_0200bed8:
	.4byte gData_0200552e
.L_0200bedc:
	.4byte 0x000000e9
	.section .text.x0200bf44,"ax",%progbits
	.balign 4
	.global Func_02003f44
	.thumb_func
Func_02003f44:
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
	sub sp, #20
	str r3, [sp, #16]
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	mov r11, r0
	cmp r3, r2
	beq .L_0200c02a
.L_0200bf6a:
	mov r3, r11
	ldrh r3, [r3]
	adds r0, r3, #0
	str r3, [sp, #12]
	bl Actor_GetById
	mov r2, r11
	ldrh r2, [r2, #2]
	adds r7, r0, #0
	str r2, [sp, #8]
	movs r3, #34
	adds r3, r3, r7
	adds r0, r2, #0
	ldrb r2, [r3]
	mov r9, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #16]
	adds r0, #1
	ldr r5, [r2, r3]
	ldr r2, .L_0200c060
	adds r3, r5, r2
	ldr r2, .L_0200c064
	asrs r3, r3, #2
	adds r6, r3, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bfb8
	ldr r0, [sp, #12]
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
	b .L_0200c018
.L_0200bfb8:
	adds r0, r7, #0
	bl Func_02004ed0
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r5, r5, r3
	str r5, [sp, #4]
	mov r2, r9
	ldrb r0, [r2]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Func_02004d20
	mov r3, r9
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r10, r0
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	ldr r0, [sp, #12]
	bl Func_02004df8
	ldr r2, [sp, #4]
	movs r3, #128
	asrs r5, r5, #19
	strb r3, [r2, #3]
	adds r5, #4
	mov r3, r9
	adds r2, r5, #0
	ldrb r0, [r3]
	mov r1, r10
	bl Func_02004ef0
	add r8, r6
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c018
	adds r0, r7, #0
	movs r1, #0
	bl Func_02004c98
.L_0200c018:
	movs r3, #4
	add r11, r3
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200bf6a
.L_0200c02a:
	ldr r3, .L_0200c068
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	cmp r3, r0
	bge .L_0200c052
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_0200c052:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c060:
	.4byte 0xfdff0000
.L_0200c064:
	.4byte gMapShapeGrid
.L_0200c068:
	.4byte gMainRoutineWork
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
