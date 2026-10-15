.syntax unified
	.thumb
	.section .text.x02008054,"ax",%progbits
	.balign 4
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	movs r0, #9
	bl Object_GetById
	mov r10, r0
	movs r0, #10
	bl Object_GetById
	mov r9, r0
	movs r0, #23
	bl Object_GetById
	adds r6, r0, #0
	ldr r2, [r6, #12]
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	ldr r5, [r6, #80]
	bl Object_SetPositionAndResetMotion
	mov r2, r8
	cmp r2, #0
	beq .L_0200809a
	cmp r2, #2
	beq .L_0200809a
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02002b68
	movs r7, #0
	b .L_02008118
.L_0200809a:
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02002b68
	movs r7, #0
.L_020080a4:
	mov r2, r10
	ldr r3, [r2, #12]
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #153
	adds r3, r3, r2
	mov r2, r10
	str r3, [r2, #12]
	movs r2, #224
	ldr r3, [r6, #12]
	lsls r2, r2, #3
	adds r2, #174
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_020081b0
	ldr r3, [r6, #8]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #8]
	adds r7, #1
	ldrh r3, [r5, #18]
	adds r3, #32
	strh r3, [r5, #18]
	bl WaitFrames
	cmp r7, #63
	ble .L_020080a4
	b .L_0200811c
.L_020080dc:
	mov r2, r9
	ldr r3, [r2, #12]
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #153
	adds r3, r3, r2
	mov r2, r9
	str r3, [r2, #12]
	movs r2, #224
	ldr r3, [r6, #12]
	lsls r2, r2, #3
	adds r2, #174
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r2, #160
	ldr r3, [r6, #8]
	lsls r2, r2, #4
	adds r2, #61
	adds r3, r3, r2
	str r3, [r6, #8]
	movs r2, #255
	ldrh r3, [r5, #18]
	lsls r2, r2, #8
	adds r2, #224
	adds r3, r3, r2
	strh r3, [r5, #18]
	movs r0, #1
	bl WaitFrames
	adds r7, #1
.L_02008118:
	cmp r7, #63
	ble .L_020080dc
.L_0200811c:
	mov r3, r8
	subs r3, #2
	cmp r3, #1
	bhi .L_020081a4
	movs r0, #30
	bl Battle_WaitMode0
	mov r0, r10
	bl Func_0200105c
	mov r0, r9
	bl Func_0200105c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #10
	bl Func_02002b68
	movs r7, #255
.L_02008142:
	movs r0, #9
	bl Object_GetById
	movs r5, #200
	ldr r3, [r0, #12]
	lsls r5, r5, #5
	adds r5, #153
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #12]
	subs r7, #1
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r0, #1
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	bl WaitFrames
	cmp r7, #0
	bge .L_02008142
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02002b68
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #80
	bl Func_02002b68
	movs r0, #10
	bl Battle_WaitMode0
	bl AudioCommand_WaitForStateByteClear
	mov r3, r10
	movs r2, #4
	adds r3, #85
	strb r2, [r3]
	mov r3, r9
	adds r3, #85
	strb r2, [r3]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
.L_020081a4:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020081b0:
	.4byte 0xfffff5c3
	.section .text.x0200832c,"ax",%progbits
	.balign 4
	.global Func_0200032c
	.thumb_func
Func_0200032c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r1
	mov r0, r8
	sub sp, #8
	bl Object_GetById
	mov r2, r8
	adds r7, r0, #0
	cmp r2, #13
	bne .L_0200841e
	ldr r0, [r7, #8]
	asrs r3, r0, #20
	cmp r3, #6
	bne .L_02008414
	ldr r3, [r7, #16]
	asrs r3, r3, #20
	cmp r3, #12
	bne .L_02008414
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	adds r6, r7, #0
	bl GameFlag_SetBit
	adds r6, #85
	movs r3, #3
	strb r3, [r6]
	movs r0, #2
	bl WaitFrames
	ldr r2, [r7, #12]
	ldr r3, [r7, #20]
	movs r5, #0
	b .L_02008384
.L_02008374:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #29
	bgt .L_0200838e
	ldr r2, [r7, #12]
	ldr r3, [r7, #20]
.L_02008384:
	cmp r2, r3
	bgt .L_02008374
	ldr r3, [r7, #40]
	cmp r3, #0
	bne .L_02008374
.L_0200838e:
	movs r3, #0
	strb r3, [r6]
	ldr r3, .L_0200852c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #0
	movs r2, #0
	mov r0, r8
	bl Func_02002ab8
	movs r0, #134
	bl Func_02002b68
	movs r3, #70
	str r3, [sp, #0]
	movs r6, #10
	movs r0, #66
	movs r1, #39
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02002a50
	movs r3, #75
	str r3, [sp, #4]
	movs r5, #6
	movs r0, #5
	movs r1, #75
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002a50
	movs r3, #12
	str r3, [sp, #4]
	movs r1, #12
	movs r2, #1
	movs r3, #1
	movs r0, #5
	str r5, [sp, #0]
	bl Func_02002a48
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #167
	bl Func_02002b68
	movs r3, #71
	str r3, [sp, #0]
	movs r0, #70
	movs r1, #39
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02002a50
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_020001b4
	b .L_0200841e
.L_02008414:
	ldr r1, [r7, #16]
	movs r2, #0
	movs r3, #6
	bl Func_02002a78
.L_0200841e:
	mov r3, r8
	cmp r3, #14
	bne .L_020084fe
	ldr r0, [r7, #8]
	asrs r3, r0, #20
	cmp r3, #57
	bne .L_020084f4
	ldr r3, [r7, #16]
	asrs r3, r3, #20
	cmp r3, #12
	bne .L_020084f4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	adds r6, r7, #0
	bl GameFlag_SetBit
	adds r6, #85
	movs r3, #3
	strb r3, [r6]
	movs r0, #2
	bl WaitFrames
	ldr r2, [r7, #12]
	ldr r3, [r7, #20]
	movs r5, #0
	b .L_02008464
.L_02008454:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #29
	bgt .L_0200846e
	ldr r2, [r7, #12]
	ldr r3, [r7, #20]
.L_02008464:
	cmp r2, r3
	bgt .L_02008454
	ldr r3, [r7, #40]
	cmp r3, #0
	bne .L_02008454
.L_0200846e:
	movs r3, #0
	strb r3, [r6]
	ldr r3, .L_0200852c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #0
	movs r2, #0
	mov r0, r8
	bl Func_02002ab8
	movs r0, #134
	bl Func_02002b68
	movs r3, #121
	str r3, [sp, #0]
	movs r6, #10
	movs r0, #68
	movs r1, #39
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02002a50
	movs r3, #75
	str r3, [sp, #4]
	movs r5, #57
	movs r0, #58
	movs r1, #75
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002a50
	movs r3, #12
	str r3, [sp, #4]
	movs r1, #12
	movs r2, #1
	movs r3, #1
	movs r0, #58
	str r5, [sp, #0]
	bl Func_02002a48
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #167
	bl Func_02002b68
	movs r3, #120
	str r3, [sp, #0]
	movs r0, #72
	movs r1, #39
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02002a50
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02000264
	b .L_020084fe
.L_020084f4:
	ldr r1, [r7, #16]
	movs r2, #0
	movs r3, #6
	bl Func_02002a78
.L_020084fe:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008524
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008524
	movs r0, #146
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
.L_02008524:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200852c:
	.4byte gPartyState
	.section .text.x0200879c,"ax",%progbits
	.balign 4
	.global Func_0200079c
	.thumb_func
Func_0200079c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008848
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_020087ae
	adds r0, #3
.L_020087ae:
	asrs r0, r0, #2
	movs r1, #5
	bl __modsi3
	ldr r3, .L_0200884c
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008802
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	ldrh r1, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_020087e2
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_0200883e
.L_020087e2:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0200883e
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200883e
.L_02008802:
	movs r5, #0
	movs r6, #4
.L_02008806:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl __modsi3
	ldr r3, .L_02008850
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_02008806
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_02008854
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02008848
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200883e:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008848:
	.4byte Data_02003314
.L_0200884c:
	.4byte Data_02003318
.L_02008850:
	.4byte Data_02003340
.L_02008854:
	.4byte 0x05000184
	.section .text.x02008930,"ax",%progbits
	.global Func_02000930
	.thumb_func
Func_02000930:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_02008a3c
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_02002a88
	movs r0, #0
	bl Func_02002b18
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	movs r5, #0
	strh r5, [r3]
	movs r3, #85
	adds r3, r3, r6
	mov r9, r3
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #99
	adds r3, r3, r7
	mov r8, r3
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_020089d0
.L_0200898a:
	ldr r3, [r7, #8]
	ldr r2, .L_02008a40
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_020089a6
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_020089a6:
	ldr r3, .L_02008a44
	adds r1, r6, #0
	ldr r2, [r3]
	ldrb r3, [r3]
	adds r1, #35
	lsls r3, r3, #12
	strh r3, [r6, #6]
	movs r3, #1
	ands r2, r3
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r1]
	movs r0, #1
	eors r3, r2
	strb r3, [r1]
	bl WaitFrames
	mov r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_0200898a
.L_020089d0:
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #68
	movs r2, #1
	add r3, r10
	strh r2, [r3]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	strh r2, [r3]
	ldr r3, [r7, #8]
	ldrh r1, [r7, #6]
	subs r2, #3
	asrs r3, r3, #19
	ands r3, r2
	asrs r1, r1, #13
	adds r3, r3, r1
	subs r3, #1
	lsls r3, r3, #19
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	ldr r0, .L_02008a38
	asrs r3, r3, #19
	ands r3, r2
	movs r2, #2
	ands r1, r2
	subs r3, r3, r1
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r6, #16]
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r6, #40]
	adds r3, r6, #0
	adds r3, #35
	strb r0, [r3]
	mov r2, r9
	movs r3, #3
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	b .L_02008a48
.L_02008a38:
	.4byte 0x00000001
.L_02008a3c:
	.4byte gPartyState
.L_02008a40:
	.4byte 0x0003ffff
.L_02008a44:
	.4byte gFrameCount
.L_02008a48:
	bl Motion_CamBounds
	bl Func_02002af8
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_02008a90
	lsls r3, r3, #9
	str r3, [r7, #52]
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	movs r0, #0
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r6, #16]
	adds r2, r0, #0
	ldr r1, [r6, #8]
	adds r0, r7, #0
	bl Func_02002a28
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_02008aae
	b .L_02008a94
	.2byte 0x0000
.L_02008a90:
	.4byte 0x00000000
.L_02008a94:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_02008aae
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_02008a94
.L_02008aae:
	movs r0, #127
	bl Func_02002b68
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_02008ace
.L_02008abc:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_02008ace
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_02008abc
.L_02008ace:
	adds r0, r7, #0
	bl Func_02002a30
	ldr r5, .L_02008b18
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r5]
	bl Object_AttachWorkTargetToObject
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #70
	add r2, r10
	movs r3, #1
	strh r3, [r2]
	movs r3, #170
	lsls r3, r3, #1
	movs r6, #0
	add r3, r10
	strh r6, [r3]
	bl Func_02002a90
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008b18:
	.4byte gPartyState
	.section .text.x02008b48,"ax",%progbits
	.global Func_02000b48
	.thumb_func
Func_02000b48:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_02008bb0
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	movs r3, #179
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #0
	bne .L_02008ba6
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008ba6
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008ba6
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02008bb4
.L_02008ba6:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
	b .L_02008cf2
.L_02008bb0:
	.4byte gPartyState
.L_02008bb4:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
	adds r3, r6, #0
	adds r3, #100
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #31
	ands r3, r2
	cmp r3, #31
	bne .L_02008bd4
	movs r0, #231
	bl Func_02002b68
.L_02008bd4:
	ldr r3, [r7, #80]
	ldr r0, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r0, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #9]
	movs r2, #2
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	bl Func_02002b50
	cmp r0, #255
	beq .L_02008cd6
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_02002b20
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_02008cd6
	ldr r2, .L_02008cc4
	cmp r5, r2
	blt .L_02008cd6
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02008c9c
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_02008c34
	subs r5, r3, r2
.L_02008c34:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_02000b1c
	cmp r0, #12
	bgt .L_02008c54
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_02008c54
	movs r2, #1
	mov r8, r2
.L_02008c54:
	mov r3, r8
	cmp r3, #0
	beq .L_02008c9c
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008c9c
	ldrh r3, [r6, #6]
	str r6, [r7, #104]
	strh r3, [r7, #6]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #181
	lsls r2, r2, #1
	strb r3, [r1]
	add r2, r10
	movs r3, #200
	strh r3, [r2]
	ldr r3, .L_02008cc8
	movs r2, #128
	ldr r0, .L_02008cc0
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	strb r0, [r3]
	mov r2, r9
	movs r3, #1
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #99
	strb r3, [r2]
.L_02008c9c:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_02008ccc
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	ldr r1, [r6, #48]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #16]
	b .L_02008cd0
.L_02008cc0:
	.4byte 0x00000000
.L_02008cc4:
	.4byte 0xffe00000
.L_02008cc8:
	.4byte gPartyState
.L_02008ccc:
	.4byte IwramMulQ16
.L_02008cd0:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_02008cf2
.L_02008cd6:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_02008d00
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_02002a00
	movs r0, #228
	bl Func_02002b68
	ldr r3, .L_02008d04
	str r5, [r3]
.L_02008cf2:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d00:
	.4byte Data_0200331c
.L_02008d04:
	.4byte Data_0200333c
	.section .text.x02008d08,"ax",%progbits
	.global Func_02000d08
	.thumb_func
Func_02000d08:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_02002b68
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_02008dbc
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #8]
	add r2, sp, #56
	adds r3, r3, r0
	str r3, [r2]
	mov r8, r2
	ldrh r0, [r5, #6]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #16]
	mov r2, r8
	adds r3, r3, r0
	str r3, [r2, #8]
	movs r0, #140
	ldr r1, [r2]
	lsls r0, r0, #1
	ldr r2, [r5, #12]
	bl Func_02002a08
	movs r1, #2
	adds r7, r0, #0
	bl Func_020029f0
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r3, r7, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r2, .L_02008db8
	ldrh r3, [r5, #6]
	add r4, sp, #16
	strh r3, [r7, #6]
	adds r3, r7, #0
	adds r3, #100
	strh r6, [r3]
	subs r3, #2
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	ldr r3, .L_02008dc0
	str r3, [r7, #108]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #1
	str r3, [r4]
	movs r3, #7
	str r3, [r4, #4]
	mov r3, r8
	ldr r0, [r3]
	ldr r2, [r3, #8]
	ldr r3, .L_02008dc4
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_02008dc8
.L_02008db8:
	.4byte 0x00000000
.L_02008dbc:
	.4byte IwramMulQ16
.L_02008dc0:
	.4byte Func_02000b48
.L_02008dc4:
	.4byte 0xfffa0000
.L_02008dc8:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_02001e78
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008de0,"ax",%progbits
	.global Func_02000de0
	.thumb_func
Func_02000de0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02008e7c
	sub sp, #56
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_02008e6e
	add r6, sp, #16
	movs r3, #3
	str r3, [r6]
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #8]
	str r3, [r6, #12]
	movs r3, #14
	str r3, [r6, #4]
	bl Random16Far
	mov r2, r10
	lsls r3, r0, #3
	ldr r2, [r2, #8]
	adds r3, r3, r0
	lsrs r3, r3, #16
	subs r3, #4
	lsls r3, r3, #16
	mov r8, r2
	add r8, r3
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	movs r2, #32
	subs r2, r2, r3
	mov r3, r10
	ldr r5, [r3, #12]
	lsls r2, r2, #16
	adds r5, r5, r2
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsrs r0, r0, #16
	movs r2, #160
	lsls r2, r2, #11
	lsls r0, r0, #16
	adds r0, r0, r2
	movs r1, #10
	bl __divsi3
	mov r3, r10
	ldr r2, [r3, #16]
	movs r3, #176
	lsls r3, r3, #12
	str r0, [sp, #0]
	str r3, [sp, #8]
	mov r0, r8
	adds r1, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl Func_02001e78
.L_02008e6e:
	movs r0, #0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008e7c:
	.4byte gFrameCount
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
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #23
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #80]
	movs r1, #128
	mov r8, r2
	movs r2, #248
	movs r0, #24
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02002ab8
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02002ab8
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02002ab8
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_02002ab8
	movs r0, #24
	bl Object_GetById
	movs r3, #85
	movs r2, #0
	adds r3, r3, r5
	str r2, [r0, #24]
	strb r2, [r3]
	mov r11, r3
	ldr r3, [r5, #20]
	movs r0, #160
	str r3, [r5, #12]
	movs r3, #85
	adds r3, r3, r6
	strb r2, [r3]
	mov r9, r3
	ldr r3, [r6, #20]
	lsls r0, r0, #4
	str r3, [r6, #12]
	movs r3, #85
	adds r3, r3, r7
	strb r2, [r3]
	mov r10, r3
	ldr r3, [r7, #20]
	adds r0, #10
	str r3, [r7, #12]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f5c
	ldr r3, [r6, #12]
	ldr r2, .L_02008ff4
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_02008ff8
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02008ffc
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #11
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_02008f5c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008fa6
	ldr r3, [r7, #12]
	ldr r2, .L_02008ff4
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02008ff8
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02009000
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_02009004
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_02008fa6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008fe6
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008fe6
	ldr r3, [r6, #12]
	ldr r2, .L_02009008
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_0200900c
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	mov r2, r10
	strb r3, [r2]
	mov r2, r11
	strb r3, [r2]
.L_02008fe6:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008ff4:
	.4byte 0x00066640
.L_02008ff8:
	.4byte 0x0001eb80
.L_02008ffc:
	.4byte 0xfffd70c0
.L_02009000:
	.4byte 0x00028f40
.L_02009004:
	.4byte 0xfffff800
.L_02009008:
	.4byte 0x00199900
.L_0200900c:
	.4byte 0x001b8480
	.section .text.x02009080,"ax",%progbits
	.balign 4
	.global Func_02001080
	.thumb_func
Func_02001080:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #220
	ldr r7, [r3]
	ldr r3, [r2, #108]
	movs r0, #230
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r3, [r3]
	movs r1, #176
	lsls r1, r1, #4
	adds r1, #2
	mov r9, r3
	adds r3, r7, r1
	movs r4, #0
	ldrsh r2, [r3, r4]
	sub sp, #44
	str r2, [sp, #4]
	movs r0, #176
	lsls r0, r0, #4
	adds r3, r7, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #5
	bls .L_020090c4
	b .L_020095e8
.L_020090c4:
	ldr r2, .L_02009410
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_020090cc:
	.4byte .L_020090e4
	.4byte .L_02009264
	.4byte .L_020093a6
	.4byte .L_02009424
	.4byte .L_02009566
	.4byte .L_020095bc
.L_020090e4:
	movs r3, #176
	lsls r3, r3, #4
	adds r3, #8
	adds r2, r7, r3
	movs r4, #176
	movs r3, #30
	strb r3, [r2]
	lsls r4, r4, #4
	adds r4, #11
	adds r3, r7, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r5, #160
	lsls r5, r5, #12
	cmp r3, #0
	beq .L_02009108
	ldr r5, .L_02009414
.L_02009108:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #196
	adds r2, r7, r0
	ldr r3, [r2]
	add r6, sp, #20
	subs r3, r3, r5
	movs r1, #160
	str r3, [r6]
	lsls r1, r1, #4
	adds r1, #200
	adds r1, r1, r7
	ldr r3, [r1]
	movs r4, #192
	lsls r4, r4, #13
	adds r3, r3, r4
	str r3, [r6, #4]
	adds r0, #8
	mov r11, r1
	adds r1, r7, r0
	ldr r3, [r1]
	mov r4, r11
	str r3, [r6, #8]
	movs r3, #8
	ldr r2, [r2]
	add r3, sp
	str r2, [r3]
	mov r8, r3
	ldr r3, [r4]
	mov r0, r8
	str r3, [r0, #4]
	movs r4, #176
	ldr r3, [r1]
	movs r1, #160
	str r3, [r0, #8]
	ldr r3, [sp, #4]
	ldr r5, [r6]
	lsls r1, r1, #4
	lsls r4, r4, #4
	adds r1, #232
	adds r4, #8
	subs r2, r2, r5
	adds r4, r4, r7
	adds r1, r1, r7
	mov r9, r1
	adds r0, r3, #0
	muls r0, r2
	movs r1, #0
	ldrsb r1, [r4, r1]
	mov r10, r4
	bl __divsi3
	adds r5, r5, r0
	mov r0, r9
	str r5, [r0]
	mov r2, r8
	ldr r3, [r2, #4]
	ldr r5, [r6, #4]
	movs r1, #160
	ldr r4, [sp, #4]
	lsls r1, r1, #4
	adds r1, #236
	subs r3, r3, r5
	mov r2, r10
	adds r1, r1, r7
	mov r9, r1
	adds r0, r4, #0
	muls r0, r3
	movs r1, #0
	ldrsb r1, [r2, r1]
	bl __divsi3
	mov r3, r9
	adds r5, r5, r0
	str r5, [r3]
	mov r0, r8
	ldr r3, [r0, #8]
	ldr r5, [r6, #8]
	ldr r1, [sp, #4]
	movs r4, #175
	subs r3, r3, r5
	mov r2, r10
	lsls r4, r4, #4
	adds r4, r4, r7
	adds r0, r1, #0
	muls r0, r3
	movs r1, #0
	ldrsb r1, [r2, r1]
	mov r9, r4
	bl __divsi3
	mov r3, r9
	adds r5, r5, r0
	str r5, [r3]
	mov r4, r11
	ldr r3, [r4]
	ldr r0, .L_02009418
	mov r2, r8
	adds r3, r3, r0
	str r3, [r6, #4]
	ldr r5, [r6]
	ldr r3, [r2]
	movs r1, #160
	ldr r4, [sp, #4]
	lsls r1, r1, #4
	adds r1, #244
	subs r3, r3, r5
	mov r2, r10
	adds r1, r1, r7
	mov r9, r1
	adds r0, r4, #0
	muls r0, r3
	movs r1, #0
	ldrsb r1, [r2, r1]
	bl __divsi3
	mov r3, r9
	adds r5, r5, r0
	str r5, [r3]
	mov r0, r8
	ldr r3, [r0, #4]
	ldr r5, [r6, #4]
	ldr r1, [sp, #4]
	movs r4, #160
	lsls r4, r4, #4
	subs r3, r3, r5
	mov r2, r10
	adds r4, #248
	adds r4, r4, r7
	adds r0, r1, #0
	muls r0, r3
	movs r1, #0
	ldrsb r1, [r2, r1]
	mov r9, r4
	bl __divsi3
	mov r3, r9
	adds r5, r5, r0
	str r5, [r3]
	mov r0, r8
	ldr r3, [r0, #8]
	ldr r5, [r6, #8]
	ldr r1, [sp, #4]
	movs r4, #160
	lsls r4, r4, #4
	subs r3, r3, r5
	mov r2, r10
	adds r4, #252
	adds r4, r4, r7
	adds r0, r1, #0
	muls r0, r3
	movs r1, #0
	ldrsb r1, [r2, r1]
	mov r9, r4
	bl __divsi3
	mov r3, r9
	adds r5, r5, r0
	movs r4, #176
	str r5, [r3]
	lsls r4, r4, #4
	adds r4, #2
	adds r1, r7, r4
	mov r4, r10
	movs r0, #0
	ldrsh r2, [r1, r0]
	movs r3, #0
	ldrsb r3, [r4, r3]
	cmp r2, r3
	beq .L_0200925e
	b .L_020095e8
.L_0200925e:
	movs r0, #176
	lsls r0, r0, #4
	b .L_02009554
.L_02009264:
	movs r1, #176
	lsls r1, r1, #4
	adds r1, #8
	adds r2, r7, r1
	movs r3, #60
	strb r3, [r2]
	movs r2, #176
	lsls r2, r2, #4
	adds r2, #11
	adds r3, r7, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r5, #240
	lsls r5, r5, #13
	cmp r3, #0
	beq .L_02009288
	ldr r5, .L_0200941c
.L_02009288:
	movs r4, #160
	lsls r4, r4, #4
	adds r4, #196
	add r3, sp, #20
	adds r2, r7, r4
	mov r8, r3
	ldr r3, [r2]
	mov r0, r8
	movs r1, #160
	str r3, [r0]
	lsls r1, r1, #4
	adds r1, #200
	adds r0, r7, r1
	ldr r3, [r0]
	mov r4, r8
	str r3, [r4, #4]
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #204
	adds r1, r7, r3
	ldr r3, [r1]
	add r6, sp, #8
	str r3, [r4, #8]
	movs r4, #160
	ldr r2, [r2]
	lsls r4, r4, #4
	subs r2, r2, r5
	str r2, [r6]
	adds r4, #232
	ldr r3, [r0]
	adds r4, r7, r4
	str r3, [r6, #4]
	mov r0, r8
	ldr r3, [r1]
	str r3, [r6, #8]
	str r4, [sp, #0]
	ldr r1, [sp, #4]
	ldr r5, [r0]
	subs r2, r2, r5
	adds r0, r1, #0
	muls r0, r2
	movs r2, #176
	lsls r2, r2, #4
	adds r2, #8
	adds r2, r2, r7
	movs r1, #0
	ldrsb r1, [r2, r1]
	mov r10, r2
	bl __divsi3
	ldr r3, [sp, #0]
	adds r5, r5, r0
	str r5, [r3]
	mov r0, r8
	ldr r5, [r0, #4]
	ldr r3, [r6, #4]
	ldr r1, [sp, #4]
	movs r4, #160
	lsls r4, r4, #4
	subs r3, r3, r5
	mov r2, r10
	adds r4, #236
	adds r4, r4, r7
	adds r0, r1, #0
	muls r0, r3
	movs r1, #0
	ldrsb r1, [r2, r1]
	mov r11, r4
	bl __divsi3
	mov r3, r11
	adds r5, r5, r0
	str r5, [r3]
	mov r0, r8
	ldr r5, [r0, #8]
	ldr r3, [r6, #8]
	ldr r1, [sp, #4]
	movs r4, #175
	subs r3, r3, r5
	mov r2, r10
	lsls r4, r4, #4
	adds r4, r4, r7
	adds r0, r1, #0
	muls r0, r3
	movs r1, #0
	ldrsb r1, [r2, r1]
	mov r9, r4
	bl __divsi3
	mov r3, r9
	adds r5, r5, r0
	str r5, [r3]
	ldr r0, [sp, #0]
	movs r4, #160
	ldr r3, [r0]
	lsls r4, r4, #4
	adds r4, #244
	adds r2, r7, r4
	str r3, [r2]
	mov r4, r11
	movs r1, #160
	ldr r3, [r4]
	lsls r1, r1, #4
	adds r1, #248
	adds r2, r7, r1
	str r3, [r2]
	mov r1, r9
	movs r0, #160
	ldr r3, [r1]
	lsls r0, r0, #4
	adds r0, #252
	adds r2, r7, r0
	str r3, [r2]
	movs r3, #176
	lsls r3, r3, #4
	adds r3, #2
	adds r6, r7, r3
	movs r4, #0
	ldrsh r0, [r6, r4]
	movs r2, #168
	lsls r2, r2, #4
	movs r1, #60
	lsls r0, r0, #16
	adds r5, r7, r2
	bl __divsi3
	str r0, [r5, #20]
	str r0, [r5, #24]
	mov r1, r10
	movs r0, #0
	ldrsh r2, [r6, r0]
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r2, r3
	beq .L_02009398
	b .L_020095e8
.L_02009398:
	movs r2, #176
	lsls r2, r2, #4
	adds r3, r7, r2
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	b .L_020095b2
.L_020093a6:
	movs r4, #176
	lsls r4, r4, #4
	adds r4, #2
	adds r3, r7, r4
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	bne .L_020093c2
	movs r1, #176
	lsls r1, r1, #4
	adds r1, #10
	adds r2, r7, r1
	movs r3, #1
	strb r3, [r2]
.L_020093c2:
	movs r2, #176
	lsls r2, r2, #4
	adds r2, #11
	adds r3, r7, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r5, #128
	lsls r5, r5, #8
	cmp r3, #0
	beq .L_020093da
	ldr r5, .L_02009420
.L_020093da:
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #232
	adds r2, r7, r3
	ldr r3, [r2]
	movs r4, #160
	adds r3, r3, r5
	str r3, [r2]
	lsls r4, r4, #4
	adds r4, #244
	adds r2, r7, r4
	ldr r3, [r2]
	movs r0, #176
	adds r3, r3, r5
	str r3, [r2]
	lsls r0, r0, #4
	adds r0, #2
	adds r1, r7, r0
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, #14
	beq .L_02009408
	b .L_020095e8
.L_02009408:
	adds r4, #12
	adds r3, r7, r4
	b .L_02009556
	.2byte 0x0000
.L_02009410:
	.4byte .L_020090cc
.L_02009414:
	.4byte 0xfff60000
.L_02009418:
	.4byte 0xffe80000
.L_0200941c:
	.4byte 0xffe20000
.L_02009420:
	.4byte 0xffff8000
.L_02009424:
	ldr r0, [sp, #4]
	cmp r0, #0
	bne .L_02009430
	movs r0, #131
	bl Func_02002b68
.L_02009430:
	movs r2, #160
	lsls r2, r2, #4
	adds r2, #196
	movs r4, #173
	adds r3, r7, r2
	lsls r4, r4, #4
	movs r1, #160
	ldr r5, [r3]
	lsls r1, r1, #4
	adds r3, r7, r4
	ldr r3, [r3]
	adds r1, #220
	adds r1, r1, r7
	mov r10, r1
	ldr r1, [sp, #4]
	asrs r5, r5, #2
	asrs r3, r3, #2
	subs r3, r3, r5
	adds r0, r1, #0
	muls r0, r3
	movs r1, #96
	bl __divsi3
	adds r5, r5, r0
	lsls r5, r5, #2
	mov r2, r10
	movs r3, #174
	movs r4, #160
	movs r0, #160
	str r5, [r2]
	lsls r3, r3, #4
	lsls r4, r4, #4
	lsls r0, r0, #4
	adds r4, #200
	adds r3, r3, r7
	adds r0, #212
	adds r2, r7, r4
	mov r8, r3
	adds r3, r7, r0
	ldr r5, [r2]
	ldr r3, [r3]
	ldr r1, [sp, #4]
	subs r3, r3, r5
	adds r0, r1, #0
	muls r0, r3
	movs r1, #96
	bl __divsi3
	mov r2, r8
	adds r5, r5, r0
	movs r3, #160
	movs r4, #160
	movs r0, #160
	str r5, [r2]
	lsls r3, r3, #4
	lsls r4, r4, #4
	lsls r0, r0, #4
	adds r4, #204
	adds r3, #228
	adds r0, #216
	adds r2, r7, r4
	adds r6, r7, r3
	adds r3, r7, r0
	ldr r5, [r2]
	ldr r3, [r3]
	ldr r1, [sp, #4]
	subs r3, r3, r5
	adds r0, r1, #0
	muls r0, r3
	movs r1, #96
	bl __divsi3
	adds r5, r5, r0
	str r5, [r6]
	mov r2, r10
	mov r3, r8
	ldr r1, [r2]
	mov r0, r9
	ldr r2, [r3]
	adds r3, r5, #0
	bl Object_SetPositionAndResetMotion
	movs r4, #176
	lsls r4, r4, #4
	adds r4, #11
	adds r3, r7, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r5, #128
	lsls r5, r5, #13
	cmp r3, #0
	beq .L_020094ec
	ldr r5, .L_020096e0
.L_020094ec:
	movs r1, #160
	lsls r1, r1, #4
	adds r1, #220
	adds r3, r7, r1
	ldr r3, [r3]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #232
	adds r4, r7, r0
	subs r3, r3, r5
	str r3, [r4]
	adds r1, #4
	adds r3, r7, r1
	movs r2, #160
	ldr r3, [r3]
	lsls r2, r2, #4
	adds r2, #236
	adds r0, r7, r2
	str r3, [r0]
	adds r2, #4
	adds r1, r7, r2
	subs r2, #12
	adds r3, r7, r2
	ldr r3, [r3]
	str r3, [r1]
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #244
	adds r2, r7, r3
	ldr r3, [r4]
	movs r4, #160
	str r3, [r2]
	lsls r4, r4, #4
	ldr r3, [r0]
	adds r4, #248
	adds r2, r7, r4
	str r3, [r2]
	movs r0, #160
	ldr r3, [r1]
	lsls r0, r0, #4
	adds r0, #252
	adds r2, r7, r0
	str r3, [r2]
	movs r2, #176
	lsls r2, r2, #4
	adds r2, #2
	adds r1, r7, r2
	movs r4, #0
	ldrsh r3, [r1, r4]
	cmp r3, #96
	bne .L_020095e8
	adds r0, #4
.L_02009554:
	adds r3, r7, r0
.L_02009556:
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r1]
	b .L_020095e8
.L_02009566:
	movs r1, #176
	lsls r1, r1, #4
	adds r1, #2
	adds r6, r7, r1
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	bne .L_02009588
	movs r0, #220
	bl Func_02002b68
	movs r4, #177
	lsls r4, r4, #4
	adds r3, r7, r4
	ldr r0, [r3]
	bl Func_0200105c
.L_02009588:
	movs r0, #168
	lsls r0, r0, #4
	adds r5, r7, r0
	ldr r3, [r5, #20]
	ldr r1, .L_020096e4
	movs r2, #204
	lsls r2, r2, #6
	adds r3, r3, r1
	adds r2, #50
	str r3, [r5, #20]
	str r3, [r5, #24]
	cmp r3, r2
	bgt .L_020095e8
	movs r3, #0
	str r3, [r5, #20]
	movs r3, #176
	lsls r3, r3, #4
	adds r2, r7, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020095b2:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r6]
	b .L_020095e8
.L_020095bc:
	movs r4, #176
	lsls r4, r4, #4
	adds r4, #2
	adds r1, r7, r4
	movs r0, #0
	ldrsh r2, [r1, r0]
	cmp r2, #0
	bne .L_020095d2
	adds r4, #7
	adds r3, r7, r4
	strb r2, [r3]
.L_020095d2:
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #16
	bne .L_020095e8
	movs r1, #176
	movs r3, #186
	lsls r1, r1, #4
	lsls r3, r3, #2
	adds r2, r7, r1
	adds r3, #255
	strh r3, [r2]
.L_020095e8:
	movs r4, #176
	lsls r4, r4, #4
	adds r4, #11
	adds r3, r7, r4
	movs r2, #168
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	lsls r2, r2, #4
	adds r5, r7, r2
	ldr r2, [r5, #20]
	add r6, sp, #32
	cmp r3, #0
	bne .L_0200960e
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02009618
	negs r3, r2
	b .L_02009618
.L_0200960e:
	adds r3, r2, #0
	cmp r3, #0
	bge .L_02009616
	negs r3, r3
.L_02009616:
	negs r3, r3
.L_02009618:
	str r3, [r5, #20]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #220
	adds r3, r7, r0
	ldr r3, [r3]
	movs r1, #174
	str r3, [r6]
	lsls r1, r1, #4
	adds r3, r7, r1
	ldr r3, [r3]
	movs r2, #160
	str r3, [r6, #4]
	lsls r2, r2, #4
	adds r2, #228
	adds r3, r7, r2
	ldr r3, [r3]
	adds r0, r6, #0
	str r3, [r6, #8]
	bl Func_02002b20
	ldr r3, [r6]
	adds r0, r5, #0
	str r3, [r5, #12]
	ldr r3, [r6, #8]
	str r3, [r5, #16]
	bl Func_02002b38
	movs r4, #224
	movs r0, #128
	movs r3, #0
	lsls r4, r4, #3
	lsls r0, r0, #2
	mov r8, r3
	adds r5, r7, r4
	adds r6, r7, r0
.L_02009660:
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_02009718
	movs r3, #1
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009690
	movs r2, #160
	lsls r2, r2, #4
	adds r2, #232
	adds r3, r7, r2
	ldr r3, [r3]
	movs r4, #160
	str r3, [r5]
	lsls r4, r4, #4
	adds r4, #236
	adds r3, r7, r4
	ldr r3, [r3]
	movs r0, #175
	str r3, [r5, #4]
	lsls r0, r0, #4
	adds r3, r7, r0
	b .L_020096b0
.L_02009690:
	movs r1, #160
	lsls r1, r1, #4
	adds r1, #244
	adds r3, r7, r1
	ldr r3, [r3]
	movs r2, #160
	str r3, [r5]
	lsls r2, r2, #4
	adds r2, #248
	adds r3, r7, r2
	ldr r3, [r3]
	movs r4, #160
	str r3, [r5, #4]
	lsls r4, r4, #4
	adds r4, #252
	adds r3, r7, r4
.L_020096b0:
	ldr r3, [r3]
	str r3, [r5, #8]
	movs r0, #176
	lsls r0, r0, #4
	adds r0, #10
	adds r3, r7, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_020096e8
	bl Random16Far
	bl Math_Sine
	ldr r3, [r5, #4]
	lsls r0, r0, #3
	adds r3, r3, r0
	str r3, [r5, #4]
	movs r3, #152
	lsls r3, r3, #7
	adds r3, #204
	str r3, [r5, #16]
	b .L_02009718
.L_020096e0:
	.4byte 0xfff00000
.L_020096e4:
	.4byte 0xfffff800
.L_020096e8:
	bl Random16Far
	bl Math_Sine
	ldr r3, [r5, #4]
	lsls r0, r0, #1
	adds r3, r3, r0
	str r3, [r5, #4]
	bl Random16Far
	bl Math_Cosine
	ldr r3, [r5]
	lsls r2, r0, #2
	adds r2, r2, r0
	lsls r2, r2, #1
	adds r3, r3, r2
	str r3, [r5]
	bl Random16Far
	ldr r1, .L_02009758
	adds r0, r0, r1
	lsls r0, r0, #1
	str r0, [r5, #16]
.L_02009718:
	ldr r1, [r5, #24]
	cmp r1, #15
	bhi .L_0200975c
	movs r3, #176
	lsls r3, r3, #4
	adds r3, #6
	adds r2, r7, r3
	movs r3, #3
	ands r1, r3
	lsls r3, r1, #1
	ldrh r1, [r2]
	ldr r2, .L_02009750
	adds r1, r1, r3
	ldr r3, .L_02009754
	adds r0, r6, #0
	ands r1, r3
	ldrh r3, [r6, #8]
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #8]
	adds r1, r5, #0
	bl Func_02002b40
	ldr r3, [r5, #4]
	ldr r2, [r5, #16]
	subs r3, r3, r2
	str r3, [r5, #4]
	b .L_0200975c
.L_02009750:
	.4byte 0xfffffc00
.L_02009754:
	.4byte 0x000003ff
.L_02009758:
	.4byte 0xffff8000
.L_0200975c:
	ldr r3, [r5, #24]
	movs r4, #176
	adds r2, r3, #1
	str r2, [r5, #24]
	lsls r4, r4, #4
	adds r4, #9
	adds r3, r7, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_0200977c
	cmp r2, #16
	bne .L_0200977c
	movs r3, #0
	str r3, [r5, #24]
.L_0200977c:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #40
	adds r5, #28
	cmp r1, #7
	bgt .L_0200978c
	b .L_02009660
.L_0200978c:
	movs r3, #176
	lsls r3, r3, #4
	adds r3, #2
	adds r2, r7, r3
	ldrh r3, [r2]
	add sp, #44
	adds r3, #1
	strh r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x020097a8,"ax",%progbits
	.global Func_020017a8
	.thumb_func
Func_020017a8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	movs r1, #176
	lsls r1, r1, #4
	sub sp, #12
	adds r5, r0, #0
	adds r1, #20
	movs r0, #220
	str r2, [sp, #8]
	bl Runtime_AllocateHeapBlockFar
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r7, r0, #0
	movs r0, #230
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r3, [r3]
	adds r0, r5, #0
	mov r11, r3
	bl Object_GetById
	movs r1, #176
	lsls r1, r1, #4
	adds r1, #12
	adds r3, r7, r1
	str r0, [r3]
	adds r0, r6, #0
	bl Object_GetById
	movs r2, #177
	lsls r2, r2, #4
	adds r3, r7, r2
	str r0, [r3]
	ldr r0, .L_02009898
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_020029a0
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #1
	adds r2, r7, #0
	str r0, [sp, #4]
	bl VramBlock_LoadCached
	mov r9, r0
	movs r0, #176
	lsls r0, r0, #4
	adds r0, #6
	adds r3, r7, r0
	mov r1, r9
	movs r2, #224
	movs r0, #128
	strh r1, [r3]
	lsls r2, r2, #3
	movs r3, #0
	lsls r0, r0, #2
	movs r1, #31
	adds r6, r7, r2
	mov r10, r3
	adds r5, r7, r0
	mov r8, r1
.L_02009838:
	mov r2, r9
	adds r0, r5, #0
	str r2, [sp, #0]
	movs r1, #4
	movs r2, #4
	movs r3, #0
	bl Func_02002b30
	ldrb r3, [r5, #5]
	ldrb r2, [r5, #9]
	movs r0, #32
	orrs r3, r0
	strb r3, [r5, #5]
	movs r1, #13
	movs r3, #15
	ands r3, r2
	negs r1, r1
	ldr r2, .L_02009894
	ands r3, r1
	adds r1, #12
	add r8, r1
	strb r3, [r5, #9]
	strh r2, [r5, #30]
	mov r3, r10
	subs r0, #34
	mov r2, r8
	str r3, [r6, #24]
	add r10, r0
	adds r5, #40
	adds r6, #28
	cmp r2, #0
	bge .L_02009838
	ldr r0, .L_0200989c
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_020029a0
	bl Resource_FindFreeEntry
	movs r5, #128
	lsls r5, r5, #2
	adds r1, r5, #0
	adds r2, r7, #0
	mov r10, r0
	b .L_020098a0
.L_02009894:
	.4byte 0x000000f0
.L_02009898:
	.4byte 0x000001eb
.L_0200989c:
	.4byte 0x000001f6
.L_020098a0:
	bl VramBlock_LoadCached
	movs r1, #176
	lsls r1, r1, #4
	movs r2, #168
	adds r1, #4
	lsls r2, r2, #4
	adds r5, r7, r2
	adds r3, r7, r1
	strh r0, [r3]
	movs r1, #30
	str r0, [sp, #0]
	movs r2, #7
	adds r0, r5, #0
	ldr r3, .L_020098f4
	bl Func_02002b30
	ldr r3, .L_020098f0
	movs r2, #13
	strh r3, [r5, #30]
	ldrb r3, [r5, #9]
	negs r2, r2
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	ldrb r3, [r5, #5]
	movs r0, #32
	movs r1, #15
	movs r6, #0
	ands r2, r1
	orrs r3, r0
	strb r3, [r5, #5]
	strb r2, [r5, #9]
	str r6, [r5, #20]
	str r6, [r5, #24]
	ldr r2, [sp, #8]
	cmp r2, #0
	bne .L_0200992e
	b .L_020098f8
	.2byte 0x0000
.L_020098f0:
	.4byte 0x000000f0
.L_020098f4:
	.4byte 0x80004000
.L_020098f8:
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #196
	movs r0, #160
	adds r2, r7, r3
	lsls r0, r0, #4
	movs r3, #152
	lsls r3, r3, #16
	adds r0, #200
	movs r1, #160
	str r3, [r2]
	lsls r1, r1, #4
	adds r3, r7, r0
	movs r0, #128
	lsls r0, r0, #14
	adds r1, #204
	str r0, [r3]
	adds r3, r7, r1
	movs r1, #164
	lsls r1, r1, #16
	str r1, [r3]
	movs r3, #173
	lsls r3, r3, #4
	adds r2, r7, r3
	movs r3, #235
	lsls r3, r3, #17
	b .L_0200995c
.L_0200992e:
	movs r1, #160
	lsls r1, r1, #4
	adds r1, #196
	movs r3, #218
	adds r2, r7, r1
	lsls r3, r3, #18
	str r3, [r2]
	movs r2, #160
	lsls r2, r2, #4
	adds r2, #200
	movs r0, #128
	adds r3, r7, r2
	lsls r0, r0, #14
	adds r1, #8
	str r0, [r3]
	adds r3, r7, r1
	movs r1, #164
	lsls r1, r1, #16
	str r1, [r3]
	movs r3, #173
	lsls r3, r3, #4
	adds r2, r7, r3
	ldr r3, .L_02009a2c
.L_0200995c:
	str r3, [r2]
	movs r2, #160
	lsls r2, r2, #4
	adds r2, #212
	adds r3, r7, r2
	str r0, [r3]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #216
	adds r3, r7, r0
	str r1, [r3]
	movs r2, #160
	lsls r2, r2, #4
	adds r2, #196
	adds r3, r7, r2
	movs r1, #160
	ldr r3, [r3]
	lsls r1, r1, #4
	adds r1, #220
	adds r1, r1, r7
	movs r0, #160
	str r3, [r1]
	lsls r0, r0, #4
	movs r3, #174
	lsls r3, r3, #4
	adds r0, #200
	adds r6, r7, r3
	adds r3, r7, r0
	ldr r3, [r3]
	adds r2, #8
	str r3, [r6]
	mov r8, r1
	adds r3, r7, r2
	movs r1, #160
	ldr r3, [r3]
	lsls r1, r1, #4
	adds r1, #228
	adds r5, r7, r1
	str r3, [r5]
	movs r3, #128
	lsls r3, r3, #8
	mov r0, r11
	str r3, [r0, #48]
	str r3, [r0, #52]
	bl Func_02002a20
	mov r2, r8
	ldr r1, [r2]
	ldr r3, [r5]
	ldr r2, [r6]
	mov r0, r11
	bl Func_02002a28
	movs r0, #142
	bl Func_02002b68
	movs r0, #176
	lsls r0, r0, #4
	adds r0, #12
	adds r3, r7, r0
	ldr r0, [r3]
	bl Func_0200105c
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #176
	add r0, sp, #8
	ldrb r0, [r0]
	lsls r1, r1, #4
	adds r1, #11
	adds r3, r7, r1
	strb r0, [r3]
	movs r0, #176
	lsls r0, r0, #4
	subs r1, #11
	adds r0, #2
	movs r2, #0
	adds r5, r7, r1
	adds r3, r7, r0
	strh r2, [r5]
	strh r2, [r3]
	movs r3, #176
	lsls r3, r3, #4
	ldr r1, .L_02009a28
	adds r3, #9
	adds r2, r7, r3
	adds r0, #8
	movs r3, #1
	strb r3, [r2]
	adds r3, r7, r0
	strb r1, [r3]
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009a30
	bl Scheduler_AddOrUpdateCallback
	movs r2, #186
	movs r1, #0
	ldrsh r3, [r5, r1]
	b .L_02009a46
	.2byte 0x0000
.L_02009a28:
	.4byte 0x00000000
.L_02009a2c:
	.4byte 0x02260000
.L_02009a30:
	.4byte Func_02001080
.L_02009a34:
	movs r0, #1
	bl WaitFrames
	movs r0, #176
	lsls r0, r0, #4
	adds r3, r7, r0
	movs r2, #186
	movs r1, #0
	ldrsh r3, [r3, r1]
.L_02009a46:
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	bne .L_02009a34
	ldr r0, .L_02009a74
	bl Scheduler_RemoveCallbackFar
	mov r0, r10
	bl Resource_ResetEntry
	ldr r0, [sp, #4]
	bl Resource_ResetEntry
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009a74:
	.4byte Func_02001080
	.section .text.x02009a78,"ax",%progbits
	.global Func_02001a78
	.thumb_func
Func_02001a78:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_02009c0c
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02009c10
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_02009c14
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_02009c18
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02009c1c
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_02009aca
	b .L_02009bfe
.L_02009aca:
	ldr r2, .L_02009c20
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_02009ad8
	b .L_02009bee
.L_02009ad8:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_02009ae0
	b .L_02009bee
.L_02009ae0:
	mov r1, r10
	subs r0, r3, r1
	ldr r2, [sp, #0]
	ldr r3, [r5, #12]
	movs r1, #128
	subs r3, r3, r2
	ldr r2, [r5, #16]
	lsls r1, r1, #12
	adds r3, r3, r1
	mov r1, r8
	subs r2, r2, r1
	ldr r1, [sp, #0]
	subs r2, r2, r1
	subs r4, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r3, #58
	mov r11, r3
	ldr r3, .L_02009c24
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_02009b56
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #7
	movs r1, #167
	adds r4, r2, #0
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #16
	cmp r3, r1
	bhi .L_02009bee
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_02009bee
	cmp r4, #239
	bgt .L_02009bee
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	mov r3, r12
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02009c28
	b .L_02009b92
.L_02009b56:
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #0
	movs r1, #175
	adds r4, r2, #0
	adds r3, #23
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #64
	cmp r3, r1
	bhi .L_02009bee
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_02009bee
	cmp r4, #175
	bgt .L_02009bee
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	movs r3, #0
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02009c2c
.L_02009b92:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02009c30
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_02009bd0
	adds r0, r5, #0
	bl Func_02002b48
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	strb r3, [r6, #9]
	b .L_02009be4
.L_02009bd0:
	movs r3, #3
	ands r3, r2
	movs r0, #13
	ldrb r2, [r6, #9]
	negs r0, r0
	adds r1, r0, #0
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	strb r2, [r6, #9]
.L_02009be4:
	adds r0, r6, #0
	mov r1, r11
	bl Func_020029c0
	adds r6, #12
.L_02009bee:
	ldr r3, .L_02009c1c
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02009bfe
	b .L_02009aca
.L_02009bfe:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009c0c:
	.4byte 0xffff0000
.L_02009c10:
	.4byte Data_02003380
.L_02009c14:
	.4byte ResourceTableEntries
.L_02009c18:
	.4byte Data_020033c4
.L_02009c1c:
	.4byte Data_02003382
.L_02009c20:
	.4byte Data_02003384
.L_02009c24:
	.4byte Data_02003484
.L_02009c28:
	.4byte 0x40002000
.L_02009c2c:
	.4byte 0xc000a000
.L_02009c30:
	.4byte Data_02003486
	.section .text.x02009e78,"ax",%progbits
	.balign 4
	.global Func_02001e78
	.thumb_func
Func_02001e78:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_0200a030
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
	beq .L_02009ec0
	cmp r7, #0
	beq .L_02009ec0
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02009ec8
.L_02009ec0:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02009ec8:
	mov r3, r10
	bl Func_02002a08
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02009ed6
	b .L_0200a022
.L_02009ed6:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_020029f0
	ldr r2, .L_0200a034
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02002a00
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_0200a038
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
	ldr r3, .L_0200a03c
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200a022
	cmp r7, #0
	beq .L_0200a022
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02009f58
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_02009f58:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009f78
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_02009f78:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_02009f8c
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02009f8c:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009fd2
	ldr r3, .L_0200a034
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_02009fba
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02009fcc
.L_02009fba:
	ldr r2, .L_0200a03c
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200a03c
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02009fcc:
	bl __divsi3
	str r0, [r6, #52]
.L_02009fd2:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009fee
	adds r0, r6, #0
	movs r1, #1
	bl Func_020029f0
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02002a00
.L_02009fee:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200a000
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_0200a000:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200a012
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200a012:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200a022
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200a022:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a030:
	.4byte gPartyState
.L_0200a034:
	.4byte Data_02003330
.L_0200a038:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_0200a03c:
	.4byte 0xffff0000
	.section .text.x0200a040,"ax",%progbits
	.global Func_02002040
	.thumb_func
Func_02002040:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_0200a158
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_0200a14c
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_0200a15c
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
	bne .L_0200a08c
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_0200a094
.L_0200a08c:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_0200a094:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_0200a160
	cmp r3, r2
	beq .L_0200a14c
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_0200a14c
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
	ldr r2, .L_0200a164
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
	bhi .L_0200a14c
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_0200a14c
	cmp r2, #239
	bgt .L_0200a14c
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
	ldr r3, .L_0200a168
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_020029c0
.L_0200a14c:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a158:
	.4byte Data_02003488
.L_0200a15c:
	.4byte gPartyState
.L_0200a160:
	.4byte 0xffff0000
.L_0200a164:
	.4byte ResourceTableEntries
.L_0200a168:
	.4byte 0x80008800
	.section .text.x0200a16c,"ax",%progbits
	.global Func_0200216c
	.thumb_func
Func_0200216c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200a39c
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
	ldr r3, .L_0200a3a0
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
	bge .L_0200a2f0
.L_0200a220:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_0200a2e4
.L_0200a234:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_0200a2d4
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_0200a2d4
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_0200a2d4
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
	bne .L_0200a288
	cmp r5, r10
	bne .L_0200a2c6
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_0200a2c6
.L_0200a288:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2c6
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
	bl Func_02002a40
.L_0200a2c6:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_0200a2d4:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_0200a234
.L_0200a2e4:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_0200a220
.L_0200a2f0:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a348
	ldr r3, .L_0200a3a4
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
	bge .L_0200a348
.L_0200a322:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_0200a338
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_0200a338
	mov r0, r8
	strh r2, [r0, #12]
.L_0200a338:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_0200a322
.L_0200a348:
	movs r0, #128
	lsls r0, r0, #1
	bl MemAlloc
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_0200a356:
	ldr r3, .L_0200a3a8
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_0200a356
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
	bl MemFree
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a3ac
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
.L_0200a39c:
	.4byte Data_02003488
.L_0200a3a0:
	.4byte IwramClearWords
.L_0200a3a4:
	.4byte gPartyState
.L_0200a3a8:
	.4byte 0x11111111
.L_0200a3ac:
	.4byte Func_02002040
	.section .text.x0200a438,"ax",%progbits
	.balign 4
	.global Func_02002438
	.thumb_func
Func_02002438:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_0200a4a4
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
	bl Func_020023b0
	movs r0, #161
	bl Func_02002b68
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
	bl Func_02002a40
	movs r0, #12
	bl Battle_WaitMode0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200a4a4:
	.4byte gPartyState
	.section .text.x0200a4a8,"ax",%progbits
	.global Func_020024a8
	.thumb_func
Func_020024a8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200a558
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
	bl Func_020023b0
	movs r0, #229
	bl Func_02002b68
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
	bl Func_02002a40
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_0200a550
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
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
	ldr r2, .L_0200a554
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_0200a55c
	.2byte 0x0000
.L_0200a550:
	.4byte 0x00000000
.L_0200a554:
	.4byte 0x00008000
.L_0200a558:
	.4byte gPartyState
.L_0200a55c:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
.L_0200a570:
	cmp r7, #5
	bne .L_0200a57a
	movs r0, #204
	bl Func_02002b68
.L_0200a57a:
	ldr r3, [r6, #24]
	ldr r1, .L_0200a5d8
	ldr r2, .L_0200a5dc
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_0200a5e0
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_0200a570
	ldr r3, .L_0200a5e4
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
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a5d8:
	.4byte 0xfffffc00
.L_0200a5dc:
	.4byte 0xfffffd00
.L_0200a5e0:
	.4byte 0xffff6667
.L_0200a5e4:
	.4byte gPartyState
	.section .text.x0200a640,"ax",%progbits
	.balign 4
	.global Func_02002640
	.thumb_func
Func_02002640:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a7c4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02002a88
	movs r0, #0
	bl Func_02002b18
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02002a18
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
	bl Func_02002b68
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200a7c8
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200a6da:
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
	ldr r3, .L_0200a7cc
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200a7d0
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
	ldr r4, .L_0200a7d4
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02001e78
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200a6da
	movs r0, #188
	bl Func_02002b68
	ldr r5, .L_0200a7c4
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02002ad8
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02002a60
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02002a60
	bl Func_02002a68
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02002ad8
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
	bl Func_02002a90
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a7c4:
	.4byte gPartyState
.L_0200a7c8:
	.4byte Effect_AdvanceMotion
.L_0200a7cc:
	.4byte 0xffffa000
.L_0200a7d0:
	.4byte 0xffffd000
.L_0200a7d4:
	.4byte 0x01090001
	.section .text.x0200a7d8,"ax",%progbits
	.global Func_020027d8
	.thumb_func
Func_020027d8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a880
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200a884
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
	bge .L_0200a874
.L_0200a80c:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200a868
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200a868
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a83c
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02002438
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200a874
.L_0200a83c:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200a874
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_020024a8
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
	b .L_0200a876
.L_0200a868:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200a80c
.L_0200a874:
	movs r0, #0
.L_0200a876:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a880:
	.4byte gPartyState
.L_0200a884:
	.4byte Data_02003488
	.section .text.x0200a888,"ax",%progbits
	.global Func_02002888
	.thumb_func
Func_02002888:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_0200a938
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_0200a93c
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
	bne .L_0200a8d6
	cmp r0, #0
	beq .L_0200a92a
.L_0200a8d6:
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
	bl Func_02002a18
	bl Func_02002640
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_0200a92a:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a938:
	.4byte gPartyState
.L_0200a93c:
	.4byte Data_02003488
	.section .bss,"aw",%nobits
	.global Data_0200333c
Data_0200333c:
	.space 0x00000004
	.global Data_02003340
Data_02003340:
	.space 0x00000020
	.global Data_02003360
Data_02003360:
	.space 0x00000004
	.global Data_02003364
Data_02003364:
	.space 0x0000001c
	.global Data_02003380
Data_02003380:
	.space 0x00000002
	.global Data_02003382
Data_02003382:
	.space 0x00000002
	.global Data_02003384
Data_02003384:
	.space 0x00000040
	.global Data_020033c4
Data_020033c4:
	.space 0x000000c0
	.global Data_02003484
Data_02003484:
	.space 0x00000002
	.global Data_02003486
Data_02003486:
	.space 0x00000002
	.global Data_02003488
Data_02003488:
