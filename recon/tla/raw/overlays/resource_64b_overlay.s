.syntax unified
	.thumb
	.section .text.x02008574,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02000574
	.thumb_func
Func_02000574:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #222
	mov r9, r3
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200854e
	b .L_ja_02008672
.L_ja_0200854e:
	ldr r1, .L_ja_0200867c
	movs r3, #133
	mov r8, r1
	lsls r3, r3, #2
	add r3, r8
	ldr r0, [r3]
	mov r10, r3
	bl Func_02004d68
	mov r1, r10
	adds r5, r0, #0
	ldr r0, [r1]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #8
	bl Object_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	ldr r1, [r6, #8]
	ldr r2, [r7, #16]
	subs r1, r1, r3
	ldr r3, [r6, #16]
	asrs r1, r1, #16
	subs r3, r3, r2
	asrs r3, r3, #16
	adds r2, r1, #0
	muls r2, r1
	adds r1, r3, #0
	muls r1, r3
	adds r3, r1, #0
	adds r2, r2, r3
	cmp r5, #8
	beq .L_ja_02008598
	cmp r2, #255
	bgt .L_ja_02008672
.L_ja_02008598:
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	add r2, r8
	movs r3, #7
	strb r3, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	add r2, r8
	movs r3, #1
	movs r0, #187
	strh r3, [r2]
	lsls r0, r0, #1
	bl GameFlag_SetBit
	ldr r3, [r7, #8]
	movs r2, #158
	lsls r2, r2, #2
	add r2, r8
	str r3, [r2]
	ldr r3, [r7, #16]
	movs r2, #159
	lsls r2, r2, #2
	add r2, r8
	str r3, [r2]
	movs r2, #160
	ldrh r3, [r7, #6]
	lsls r2, r2, #2
	add r2, r8
	str r3, [r2]
	movs r0, #78
	bl Func_02004e30
	movs r1, #6
	adds r0, r6, #0
	bl Func_02004b18
	movs r0, #6
	bl WaitFrames
	movs r0, #152
	bl Func_02004e30
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #7
	adds r0, r6, #0
	bl Func_02004b18
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r6, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r6, #40]
	str r3, [r6, #20]
	adds r0, r6, #0
	bl Func_02004b48
	ldr r3, [r7, #16]
	movs r2, #0
	ldr r1, [r7, #8]
	adds r0, r6, #0
	bl Func_02004b50
	adds r0, r6, #0
	bl Func_02004b58
	movs r1, #6
	adds r0, r6, #0
	bl Func_02004b18
	movs r5, #0
	movs r0, #6
	bl WaitFrames
	str r5, [r6, #20]
	mov r3, r10
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	bl Func_02004dc8
	bl Func_02004dc0
	bl Func_02004c28
	movs r3, #174
	lsls r3, r3, #1
	add r3, r9
	movs r0, #128
	strh r5, [r3]
	lsls r0, r0, #9
	movs r1, #30
	bl Func_02004df8
.L_ja_02008672:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_ja_0200867c:
	.4byte gPartyState
	.else
	.global Func_02000574
	.thumb_func
Func_02000574:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #222
	mov r9, r3
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008596
	b .L_020086c2
.L_02008596:
	ldr r1, .L_020086cc
	movs r3, #133
	mov r8, r1
	lsls r3, r3, #2
	add r3, r8
	ldr r0, [r3]
	mov r10, r3
	bl Func_02004d68
	mov r1, r10
	adds r5, r0, #0
	ldr r0, [r1]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #8
	bl Object_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	ldr r1, [r6, #8]
	ldr r2, [r7, #16]
	subs r1, r1, r3
	ldr r3, [r6, #16]
	asrs r1, r1, #16
	subs r3, r3, r2
	asrs r3, r3, #16
	adds r2, r1, #0
	muls r2, r1
	adds r1, r3, #0
	muls r1, r3
	adds r3, r1, #0
	adds r2, r2, r3
	cmp r5, #8
	beq .L_020085e0
	cmp r2, #255
	bgt .L_020086c2
.L_020085e0:
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	add r2, r8
	movs r3, #7
	strb r3, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	add r2, r8
	movs r3, #1
	movs r0, #187
	strh r3, [r2]
	lsls r0, r0, #1
	bl GameFlag_SetBit
	ldr r3, [r7, #8]
	movs r2, #158
	lsls r2, r2, #2
	add r2, r8
	str r3, [r2]
	ldr r3, [r7, #16]
	movs r2, #159
	lsls r2, r2, #2
	add r2, r8
	str r3, [r2]
	movs r2, #160
	ldrh r3, [r7, #6]
	lsls r2, r2, #2
	add r2, r8
	str r3, [r2]
	movs r0, #78
	bl Func_02004e30
	movs r1, #6
	adds r0, r6, #0
	bl Func_02004b18
	movs r0, #6
	bl WaitFrames
	movs r0, #152
	bl Func_02004e30
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #7
	adds r0, r6, #0
	bl Func_02004b18
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r6, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r6, #40]
	str r3, [r6, #20]
	adds r0, r6, #0
	bl Func_02004b48
	ldr r3, [r7, #16]
	movs r2, #0
	ldr r1, [r7, #8]
	adds r0, r6, #0
	bl Func_02004b50
	adds r0, r6, #0
	bl Func_02004b58
	movs r1, #6
	adds r0, r6, #0
	bl Func_02004b18
	movs r5, #0
	movs r0, #6
	bl WaitFrames
	str r5, [r6, #20]
	mov r3, r10
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	bl Func_02004dc8
	bl Func_02004dc0
	bl Func_02004c28
	movs r3, #174
	lsls r3, r3, #1
	add r3, r9
	strh r5, [r3]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	movs r0, #128
	strh r5, [r3]
	lsls r0, r0, #9
	movs r1, #30
	bl Func_02004df8
.L_020086c2:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_020086cc:
	.4byte gPartyState
	.endif
	.section .text.x020086d0,"ax",%progbits
	.global Func_020006d0
	.thumb_func
Func_020006d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008784
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #16
	bl Object_GetById
	mov r10, r0
	movs r0, #8
	bl Object_GetById
	movs r3, #192
	adds r6, r0, #0
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r5, r6, #0
	adds r5, #8
	adds r0, r5, #0
	str r3, [sp, #0]
	bl GetWorldMapCollision
	ldr r3, [r5]
	add r7, sp, #4
	str r3, [r7]
	movs r1, #0
	ldr r3, [r6, #12]
	mov r8, r0
	str r3, [r7, #4]
	movs r0, #192
	ldr r3, [r6, #16]
	adds r2, r7, #0
	str r3, [r7, #8]
	mov r11, r1
	lsls r0, r0, #13
	ldrh r1, [r6, #6]
	bl Vector_AddPolarOffsetFar
	ldr r3, [r7]
	movs r2, #0
	mov r9, r2
	cmp r3, #0
	bge .L_02008738
	ldr r0, .L_02008788
	adds r3, r3, r0
.L_02008738:
	asrs r2, r3, #21
	ldr r3, [r7, #8]
	movs r1, #31
	ands r2, r1
	cmp r3, #0
	bge .L_02008748
	ldr r0, .L_02008788
	adds r3, r3, r0
.L_02008748:
	asrs r3, r3, #21
	ands r3, r1
	lsls r3, r3, #5
	ldr r1, .L_0200878c
	adds r3, r2, r3
	lsls r3, r3, #2
	adds r2, r3, r1
	mov r3, r8
	cmp r3, #12
	ble .L_02008762
	movs r0, #1
	mov r9, r0
	b .L_02008802
.L_02008762:
	mov r3, r8
	subs r3, #1
	cmp r3, #2
	bhi .L_0200876e
	movs r1, #1
	mov r11, r1
.L_0200876e:
	ldrb r2, [r2, #3]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	bne .L_02008790
	mov r2, r11
	cmp r2, #0
	beq .L_02008790
	movs r3, #1
	b .L_02008800
	.2byte 0x0000
.L_02008784:
	.4byte gPartyState
.L_02008788:
	.4byte 0x001fffff
.L_0200878c:
	.4byte gMapBlocks
.L_02008790:
	ldr r3, [r7]
	movs r5, #192
	lsls r5, r5, #11
	adds r3, r3, r5
	str r3, [r7]
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02004b88
	ldr r2, .L_02008874
	ldr r3, [r7]
	mov r8, r2
	mov r1, r9
	orrs r1, r0
	add r3, r8
	mov r9, r1
	str r3, [r7]
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02004b88
	mov r3, r9
	orrs r3, r0
	mov r9, r3
	ldr r3, [r7]
	adds r1, r7, #0
	adds r3, r3, r5
	str r3, [r7]
	ldr r3, [r7, #4]
	adds r0, r6, #0
	adds r3, r3, r5
	str r3, [r7, #4]
	bl Func_02004b88
	ldr r3, [r7, #4]
	mov r1, r9
	orrs r1, r0
	add r3, r8
	mov r9, r1
	str r3, [r7, #4]
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02004b88
	ldr r3, [r7, #4]
	mov r2, r9
	adds r3, r3, r5
	orrs r2, r0
	str r3, [r7, #4]
	adds r0, r6, #0
	adds r1, r7, #0
	mov r9, r2
	bl Func_02004b88
	mov r3, r9
	orrs r3, r0
.L_02008800:
	mov r9, r3
.L_02008802:
	mov r0, r9
	cmp r0, #0
	beq .L_0200880a
	b .L_0200890a
.L_0200880a:
	movs r0, #78
	bl Func_02004e30
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #10
	bl Func_02004df8
	ldr r5, .L_02008878
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #18
	adds r2, r5, r1
	movs r3, #1
	strb r3, [r2]
	ldr r0, .L_02008870
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r5, r2
	mov r1, r9
	mov r11, r0
	movs r0, #187
	strh r1, [r3]
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r3, #158
	lsls r3, r3, #2
	adds r2, r5, r3
	ldr r3, [r6, #8]
	movs r0, #159
	str r3, [r2]
	lsls r0, r0, #2
	ldr r3, [r6, #16]
	adds r2, r5, r0
	str r3, [r2]
	movs r1, #160
	ldrh r3, [r6, #6]
	lsls r1, r1, #2
	adds r5, r5, r1
	str r3, [r5]
	mov r7, r10
	ldr r3, [r6, #8]
	b .L_0200887c
	.2byte 0x0000
.L_02008870:
	.4byte 0x00000000
.L_02008874:
	.4byte 0xfff40000
.L_02008878:
	.4byte gPartyState
.L_0200887c:
	movs r5, #128
	mov r0, r10
	adds r7, #85
	lsls r5, r5, #11
	ldrb r2, [r7]
	str r3, [r0, #8]
	str r5, [r0, #12]
	mov r1, r11
	ldr r3, [r6, #16]
	mov r8, r2
	str r3, [r0, #16]
	strb r1, [r7]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #6
	mov r0, r10
	bl Func_02004b18
	movs r0, #6
	bl WaitFrames
	movs r0, #152
	bl Func_02004e30
	movs r1, #7
	mov r0, r10
	bl Func_02004b18
	movs r3, #192
	mov r2, r10
	lsls r3, r3, #10
	str r3, [r2, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2, #52]
	mov r3, r8
	str r5, [r2, #40]
	mov r0, r10
	strb r3, [r7]
	bl Func_02004b48
	add r3, sp, #4
	movs r2, #0
	ldr r1, [r3]
	mov r0, r10
	ldr r3, [r3, #8]
	bl Func_02004b50
	mov r0, r10
	bl Func_02004b58
	movs r1, #6
	mov r0, r10
	bl Func_02004b18
	movs r0, #6
	bl WaitFrames
	bl Func_02004c28
	bl Func_02004dc8
	bl Func_02004dc0
	ldr r0, [sp, #0]
	movs r1, #174
	lsls r1, r1, #1
	adds r3, r0, r1
	mov r2, r9
	strh r2, [r3]
.L_0200890a:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x02008be0,"ax",%progbits
	.balign 4
	.global Func_02000be0
	.thumb_func
Func_02000be0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #12
	bl Random16Far
	lsls r0, r0, #4
	lsrs r5, r0, #16
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	ldr r0, .L_02008d0c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	cmp r5, #0
	bne .L_02008cba
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #79
	mov r8, r3
	mov r10, r2
	bl Func_02004e10
	bl Object_GetById
	ldr r3, .L_02008d10
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	bl Unit_Get
	adds r5, r0, #0
	bl Random16Far
	ldrb r3, [r5, #15]
	movs r2, #128
	muls r3, r0
	lsls r2, r2, #8
	lsls r3, r3, #1
	lsrs r3, r3, #16
	adds r2, #1
	ldr r0, .L_02008d14
	movs r1, #1
	adds r7, r3, r2
	bl UiText_ShowPositionedMessageAndWait
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	movs r0, #128
	ldr r3, [r6, #12]
	lsls r0, r0, #13
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	ldrh r1, [r6, #6]
	bl Vector_AddPolarOffsetFar
	movs r0, #79
	bl Object_GetById
	adds r6, r0, #0
	ldr r1, [r5]
	cmp r6, #0
	bne .L_02008c90
	movs r0, #234
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	adds r0, #255
	bl Func_02004b30
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008d00
	mov r2, r10
	lsls r3, r2, #2
	adds r3, #20
	mov r2, r8
	str r6, [r2, r3]
	b .L_02008c9a
.L_02008c90:
	str r1, [r6, #8]
	ldr r3, [r5, #4]
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
.L_02008c9a:
	movs r0, #79
	adds r1, r7, #0
	bl Func_02004e18
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #52
	add r3, r8
	movs r0, #128
	strh r7, [r3]
	lsls r0, r0, #5
	mov r3, r10
	orrs r0, r3
	bl Func_02004d70
	b .L_02008cfc
.L_02008cba:
	cmp r5, #1
	bne .L_02008cf0
	movs r0, #192
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004b78
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	negs r0, r0
	adds r2, #102
	bl Func_02004b78
	movs r0, #30
	bl FieldEvent_WaitFrames
	ldr r0, .L_02008d18
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	b .L_02008cfc
.L_02008cf0:
	ldr r3, .L_02008d18
	lsrs r0, r5, #1
	adds r0, r0, r3
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
.L_02008cfc:
	bl Func_02004c28
.L_02008d00:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d0c:
	.4byte MsgGazedIntoThe
.L_02008d10:
	.4byte gPartyState
.L_02008d14:
	.4byte MsgThereAreSome
.L_02008d18:
	.4byte MsgTheBoatS
	.section .text.x02008dc4,"ax",%progbits
	.balign 4
	.global Func_02000dc4
	.thumb_func
Func_02000dc4:
	push {r5, r6, r7, lr}
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008e24
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #222
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e24
	ldr r5, .L_02009188
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r5, r2
	movs r2, #0
	ldrsh r6, [r3, r2]
	cmp r6, #0
	bne .L_02008e36
	movs r3, #158
	lsls r3, r3, #2
	adds r7, r5, r3
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02008e24
	movs r0, #8
	bl Object_GetById
	ldr r3, [r7]
	movs r2, #159
	str r3, [r0, #8]
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r3, [r3]
	adds r2, #4
	str r3, [r0, #16]
	adds r3, r5, r2
	ldr r3, [r3]
	strh r3, [r0, #6]
	adds r3, r0, #0
	adds r3, #100
	adds r0, #102
	strh r6, [r3]
	strh r6, [r0]
.L_02008e24:
	ldr r3, .L_02009188
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02008e40
.L_02008e36:
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl Func_02004df8
.L_02008e40:
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #128
	lsls r3, r3, #3
	str r3, [r2]
	movs r3, #218
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	movs r1, #128
	lsls r1, r1, #9
	lsls r0, r0, #12
	bl Func_02004d20
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_02003960
	movs r0, #129
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ea0
	movs r2, #168
	movs r3, #144
	movs r0, #0
	movs r1, #240
	lsls r2, r2, #1
	lsls r3, r3, #1
	bl Func_02004b60
.L_02008ea0:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008eca
	movs r3, #128
	movs r0, #0
	movs r1, #224
	movs r2, #208
	lsls r3, r3, #1
	bl Func_02004b60
	movs r3, #128
	movs r0, #16
	movs r1, #224
	movs r2, #224
	lsls r3, r3, #1
	bl Func_02004b60
.L_02008eca:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #119
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008edc
	bl Func_02000d4c
.L_02008edc:
	ldr r1, .L_02009188
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #2
	cmp r3, #97
	bls .L_02008ef0
	b .L_020091ee
.L_02008ef0:
	ldr r2, .L_0200918c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02008ef8:
	.4byte .L_0200910c
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_0200909c
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_02009080
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020090ca
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020090fa
	.4byte .L_02009100
	.4byte .L_020091ee
	.4byte .L_02009124
	.4byte .L_0200912a
	.4byte .L_02009130
	.4byte .L_02009106
	.4byte .L_020091ee
	.4byte .L_02009136
	.4byte .L_0200909c
	.4byte .L_0200909c
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020090be
	.4byte .L_020090c4
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091a4
	.4byte .L_020091de
.L_02009080:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009090
	b .L_020091ee
.L_02009090:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_SetBit
	b .L_020091ee
.L_0200909c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #118
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020090ac
	b .L_020091ee
.L_020090ac:
	ldr r3, .L_02009188
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_020012bc
	b .L_020091ee
.L_020090be:
	bl Func_020029a4
	b .L_020091ee
.L_020090c4:
	bl Func_02003490
	b .L_020091ee
.L_020090ca:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090da
	b .L_020091ee
.L_020090da:
	movs r0, #150
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020090ea
	b .L_020091ee
.L_020090ea:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_SetBit
	bl Func_02001c5c
	b .L_020091ee
.L_020090fa:
	bl Func_02002698
	b .L_020091ee
.L_02009100:
	bl Func_02002ab0
	b .L_020091ee
.L_02009106:
	bl Func_02004c00
	b .L_020091ee
.L_0200910c:
	movs r0, #55
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020091ee
	ldr r1, .L_02009190
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	bl Func_02001e34
	b .L_020091ee
.L_02009124:
	bl Func_02002e00
	b .L_020091ee
.L_0200912a:
	bl Func_02002e78
	b .L_020091ee
.L_02009130:
	bl Func_0200310c
	b .L_020091ee
.L_02009136:
	movs r0, #2
	bl Func_02003d44
	movs r0, #7
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	ldr r2, .L_02009194
	ldr r1, .L_02009198
	movs r0, #7
	bl Func_02004c88
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_0200919c
	bl Func_02004cd8
	movs r1, #0
	movs r0, #7
	bl Func_02004cf0
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #7
	movs r1, #0
	bl Func_02004cf0
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	ldr r0, .L_020091a0
	movs r1, #9
	bl Func_02004d38
	b .L_020091ee
	.2byte 0x0000
.L_02009188:
	.4byte gPartyState
.L_0200918c:
	.4byte .L_02008ef8
.L_02009190:
	.4byte gData_0200632c
.L_02009194:
	.4byte 0x1c840000
.L_02009198:
	.4byte 0x1c6d0000
.L_0200919c:
	.4byte MsgIVeHeard
.L_020091a0:
	.4byte 0x00000109
.L_020091a4:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	movs r0, #128
	adds r3, r1, r2
	lsls r0, r0, #4
	movs r2, #1
	strh r2, [r3]
	adds r0, #222
	bl GameFlag_SetBit
	movs r0, #252
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #100
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #30
	adds r0, #255
	bl GameFlag_SetBit
	ldr r0, .L_020091f4
	movs r1, #61
	bl Func_02004d38
	b .L_020091ee
.L_020091de:
	movs r0, #100
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #30
	adds r0, #255
	bl GameFlag_SetBit
.L_020091ee:
	movs r0, #0
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020091f4:
	.4byte 0x00000002
	.section .text.x020092bc,"ax",%progbits
	.global Func_020012bc
	.thumb_func
Func_020012bc:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	cmp r5, #9
	beq .L_020092d0
	b .L_020095c0
.L_020092d0:
	movs r6, #192
	lsls r6, r6, #8
	adds r3, r6, #0
	ldr r1, .L_020095ac
	ldr r2, .L_020095b0
	movs r0, #20
	bl Func_02004c90
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02004da8
	ldr r0, .L_020095b4
	bl Func_02004cd8
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	bl Func_02004db0
	movs r0, #198
	lsls r0, r0, #9
	adds r0, #204
	movs r1, #16
	bl Func_02004df8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_02004d08
	movs r0, #4
	movs r1, #0
	movs r2, #32
	bl ObjectMotion_CommitPositionAndActivate
	adds r1, r6, #0
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	bl Func_02004e00
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02004da8
	adds r3, r6, #0
	movs r2, #0
	movs r1, #16
	movs r0, #23
	bl Func_02004dd8
	movs r0, #23
	bl Object_RefreshSelectorById
	adds r1, r6, #0
	movs r0, #23
	bl Func_02004d00
	movs r5, #128
	movs r0, #10
	bl FieldEvent_WaitFrames
	lsls r5, r5, #7
	movs r2, #150
	adds r3, r5, #0
	movs r0, #20
	ldr r1, .L_020095ac
	lsls r2, r2, #22
	bl Func_02004c90
	movs r2, #150
	adds r3, r5, #0
	movs r0, #21
	ldr r1, .L_020095ac
	lsls r2, r2, #22
	bl Func_02004c90
	movs r2, #150
	adds r3, r5, #0
	movs r0, #22
	ldr r1, .L_020095ac
	lsls r2, r2, #22
	bl Func_02004c90
	movs r0, #20
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #16
	movs r0, #21
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #22
	movs r1, #16
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #22
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r0, #20
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #21
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #22
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02004da8
	movs r2, #5
	movs r0, #23
	movs r1, #0
	bl Func_02004ce8
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r2, #0
	movs r1, #23
	movs r0, #4
	bl Object_LinkPair
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #21
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	adds r1, r6, #0
	movs r0, #4
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r6, #0
	movs r2, #0
	movs r0, #23
	bl ObjectMotion_ArmCallback
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r0, #23
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	movs r0, #22
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r0, #23
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_02004d08
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r0, #23
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #23
	movs r2, #0
	movs r0, #4
	bl Object_LinkPair
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	adds r1, r6, #0
	movs r0, #4
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r6, #0
	movs r0, #23
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #22
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r2, #8
	movs r0, #4
	movs r1, #8
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #8
	negs r1, r1
	movs r2, #8
	movs r0, #23
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #23
	bl ObjectMotion_CommitCurrentPositionAndActivate
	adds r1, r6, #0
	movs r0, #4
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r6, #0
	movs r0, #23
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #4
	bl Func_02004d08
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	ldr r3, .L_020095b8
	movs r1, #166
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
	movs r2, #2
	strb r2, [r3]
	ldr r5, .L_020095bc
	movs r1, #73
	adds r0, r5, #0
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #74
	bl Party_SetFields1f2And1f4
	bl Func_02004db0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	movs r0, #102
	movs r1, #3
	bl Func_02004d48
	b .L_02009c54
	.2byte 0x0000
.L_020095ac:
	.4byte 0x27b80000
.L_020095b0:
	.4byte 0x23280000
.L_020095b4:
	.4byte MsgHeyWaitA
.L_020095b8:
	.4byte gPartyState
.L_020095bc:
	.4byte 0x00000002
.L_020095c0:
	cmp r5, #74
	beq .L_020095c6
	b .L_0200976c
.L_020095c6:
	movs r0, #160
	lsls r0, r0, #4
	movs r5, #128
	lsls r5, r5, #7
	adds r0, #118
	bl GameFlag_ClearBit
	movs r0, #20
	ldr r1, .L_020099c0
	ldr r2, .L_020099c4
	adds r3, r5, #0
	bl Func_02004c90
	movs r0, #21
	ldr r1, .L_020099c8
	ldr r2, .L_020099c4
	adds r3, r5, #0
	bl Func_02004c90
	movs r0, #22
	ldr r1, .L_020099cc
	ldr r2, .L_020099c4
	adds r3, r5, #0
	bl Func_02004c90
	movs r3, #224
	movs r2, #151
	lsls r3, r3, #8
	movs r0, #23
	ldr r1, .L_020099c8
	lsls r2, r2, #22
	bl Func_02004c90
	movs r3, #160
	lsls r3, r3, #8
	ldr r2, .L_020099d0
	movs r0, #4
	ldr r1, .L_020099c0
	bl Func_02004c90
	movs r0, #4
	movs r1, #0
	bl Camera_FollowActor
	movs r0, #198
	lsls r0, r0, #9
	adds r0, #204
	movs r1, #1
	bl Func_02004df8
	movs r1, #19
	movs r0, #4
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02004da8
	ldr r0, .L_020099d4
	bl Func_02004cd8
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #20
	bl Func_02004d08
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	adds r1, r5, #0
	movs r0, #21
	bl Func_02004d00
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #21
	movs r1, #0
	bl Func_02004ce8
	adds r1, r5, #0
	movs r0, #22
	bl Func_02004d00
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r0, #22
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #22
	movs r1, #0
	bl Func_02004ce8
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	adds r1, r5, #0
	movs r0, #20
	bl Func_02004d00
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	movs r2, #5
	movs r0, #22
	movs r1, #0
	bl Func_02004ce8
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #40
	bl FieldEvent_WaitFrames
	ldr r2, .L_020099d8
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #243
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl Func_02004d38
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	str r2, [r3]
	b .L_02009c54
.L_0200976c:
	movs r0, #160
	lsls r0, r0, #4
	movs r6, #128
	lsls r6, r6, #7
	adds r0, #118
	bl GameFlag_ClearBit
	movs r0, #20
	ldr r1, .L_020099c0
	ldr r2, .L_020099dc
	adds r3, r6, #0
	bl Func_02004c90
	movs r5, #192
	movs r0, #21
	ldr r1, .L_020099c8
	ldr r2, .L_020099dc
	adds r3, r6, #0
	bl Func_02004c90
	lsls r5, r5, #8
	movs r0, #22
	ldr r1, .L_020099cc
	ldr r2, .L_020099dc
	adds r3, r6, #0
	bl Func_02004c90
	movs r0, #23
	ldr r1, .L_020099e0
	ldr r2, .L_020099e4
	adds r3, r5, #0
	bl Func_02004c90
	movs r1, #159
	adds r3, r5, #0
	ldr r2, .L_020099e4
	movs r0, #4
	lsls r1, r1, #22
	bl Func_02004c90
	movs r0, #4
	movs r1, #0
	bl Camera_FollowActor
	movs r0, #192
	movs r1, #1
	lsls r0, r0, #9
	bl Func_02004df8
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02004da8
	ldr r0, .L_020099e8
	bl Func_02004cd8
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #20
	bl Func_02004d08
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #21
	bl Func_02004d08
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	movs r2, #5
	movs r0, #22
	movs r1, #0
	bl Func_02004ce8
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	movs r1, #0
	movs r0, #21
	bl Func_02004d00
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #21
	bl Func_02004d08
	movs r2, #5
	movs r0, #21
	movs r1, #0
	bl Func_02004ce8
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #8
	bl Func_02004d00
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #22
	bl Func_02004d00
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	movs r0, #22
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r0, #20
	adds r1, r6, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #21
	adds r1, r6, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #22
	adds r1, r6, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #5
	movs r0, #23
	movs r1, #0
	bl Func_02004ce8
	movs r0, #20
	movs r1, #3
	bl Object_SetModeById
	movs r0, #21
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r0, #23
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #20
	bl Func_02004d08
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #21
	bl Func_02004d08
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl Object_LinkPair
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #20
	bl Func_02004d08
	adds r1, r6, #0
	movs r0, #20
	bl Func_02004d00
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #20
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_020099ec
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_02004d08
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009a14
.L_020099c0:
	.4byte 0x27b80000
.L_020099c4:
	.4byte 0x25900000
.L_020099c8:
	.4byte 0x27a80000
.L_020099cc:
	.4byte 0x27c80000
.L_020099d0:
	.4byte 0x25b00000
.L_020099d4:
	.4byte MsgItLooksLike
.L_020099d8:
	.4byte gPartyState
.L_020099dc:
	.4byte 0x25a00000
.L_020099e0:
	.4byte 0x27b00000
.L_020099e4:
	.4byte 0x25d00000
.L_020099e8:
	.4byte MsgWeLostAgain
.L_020099ec:
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_02004d08
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r0, #20
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
.L_02009a14:
	movs r1, #23
	movs r2, #0
	movs r0, #4
	bl Object_LinkPair
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r2, #5
	movs r0, #23
	movs r1, #0
	bl Func_02004ce8
	movs r0, #21
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #21
	movs r1, #0
	bl Func_02004ce8
	movs r0, #22
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #22
	movs r1, #0
	bl Func_02004ce8
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	bl Func_02000d4c
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_02004d08
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r2, #0
	movs r1, #21
	movs r0, #20
	bl Object_LinkPair
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #21
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #0
	movs r1, #22
	movs r0, #20
	bl Object_LinkPair
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #22
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #20
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #22
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #20
	bl Func_02004d00
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #32
	movs r0, #20
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #32
	movs r0, #21
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #32
	movs r0, #22
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #64
	movs r2, #64
	movs r0, #20
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #64
	movs r2, #64
	movs r0, #21
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #64
	movs r2, #64
	movs r0, #22
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	movs r2, #0
	movs r1, #0
	movs r0, #22
	bl Func_02004c88
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #20
	bl Func_02004d00
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02004d00
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	bl Func_02004d00
	movs r1, #3
	movs r0, #23
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #23
	movs r1, #0
	bl Func_02004ce8
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #23
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #23
	ldr r1, .L_02009c58
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #23
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02009c32
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #23
	bl ObjectMotion_ResetAndSetPosition
.L_02009c32:
	movs r0, #23
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	bl Func_02004db0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #15
	bl Func_02004df8
	bl Func_02004c28
.L_02009c54:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009c58:
	.4byte 0x00013333
	.section .text.x02009e34,"ax",%progbits
	.balign 4
	.global Func_02001e34
	.thumb_func
Func_02001e34:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #9
	sub sp, #4
	bl Object_GetById
	ldr r3, .L_0200a08c
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r3, r1
	adds r6, r0, #0
	ldr r0, [r5]
	bl Object_GetById
	ldr r2, .L_0200a090
	ldr r3, [r0, #8]
	ldr r1, .L_0200a094
	adds r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	mov r9, r1
	asrs r3, r3, #1
	add r3, r9
	str r3, [sp, #0]
	ldr r2, .L_0200a098
	ldr r3, [r0, #16]
	ldr r1, .L_0200a09c
	adds r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r8, r1
	movs r0, #183
	add r3, r8
	lsls r0, r0, #1
	mov r11, r3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009e90
	b .L_0200a368
.L_02009e90:
	ldr r3, .L_0200a0a0
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009ea4
	movs r0, #118
	adds r0, #255
	bl GameFlag_SetBit
.L_02009ea4:
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #0
	bl Party_RemoveActiveOwner
	movs r0, #1
	bl Party_RemoveActiveOwner
	movs r0, #2
	bl Party_RemoveActiveOwner
	movs r0, #3
	bl Party_RemoveActiveOwner
	movs r0, #4
	bl Party_RemoveActiveOwner
	movs r0, #5
	bl Party_RemoveActiveOwner
	movs r0, #6
	bl Party_RemoveActiveOwner
	movs r0, #7
	bl Party_RemoveActiveOwner
	movs r0, #4
	bl Party_AddActiveOwner
	movs r0, #5
	bl Party_AddActiveOwner
	movs r0, #6
	bl Party_AddActiveOwner
	movs r0, #1
	ldr r7, [r5]
	bl Func_02004bf8
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r1, #0
	movs r2, #7
	adds r0, r7, #0
	bl Djinn_AddToOwner
	movs r1, #0
	movs r2, #7
	adds r0, r7, #0
	bl DjinnRecovery_AddStandby
	bl Func_02004da8
	movs r1, #9
	movs r2, #0
	adds r0, r7, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #2
	adds r1, #255
	movs r2, #60
	adds r0, r7, #0
	bl Func_02004d08
	adds r2, r6, #0
	movs r3, #1
	adds r2, #102
	strh r3, [r2]
	adds r1, r7, #0
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_SetAngleToward
	movs r0, #16
	bl WaitFrames
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a0a4
	ldr r3, .L_0200a0a8
	movs r5, #15
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a0ac
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	bl Func_02004db0
	ldr r0, .L_0200a0b0
	movs r1, #6
	bl Func_02004df8
	bl Func_02004e00
	bl Func_02004da8
	movs r2, #85
	adds r2, r2, r6
	movs r3, #2
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r6, #72]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #48]
	str r3, [r6, #52]
	movs r3, #0
	str r3, [r6, #40]
	str r3, [r6, #20]
	mov r10, r2
	adds r0, r6, #0
	mov r1, r9
	movs r2, #0
	mov r3, r8
	bl Func_02004b50
.L_02009fbe:
	ldr r3, [r6, #24]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #28]
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02009fbe
	movs r0, #9
	adds r1, r7, #0
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #9
	adds r0, r7, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #16
	bl WaitFrames
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	movs r1, #0
	bl Animation_ApplyChildValues
	movs r3, #128
	lsls r3, r3, #9
	movs r1, #0
	str r3, [r6, #72]
	movs r0, #9
	bl UiText_OpenMessageAtObject
	adds r0, r7, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_0200a0b8
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #2
	movs r2, #20
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a0a8
	ldr r3, .L_0200a0a4
	ldr r1, .L_0200a0b4
	subs r5, r2, r3
	muls r0, r5
	mov r8, r1
	add r0, r8
	bl Func_02004cd8
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	adds r0, r7, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_0200a06c
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	muls r0, r5
	mov r3, r8
	adds r3, #1
	b .L_0200a07a
.L_0200a06c:
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	muls r0, r5
	mov r3, r8
	adds r3, #2
.L_0200a07a:
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	b .L_0200a0d8
	.2byte 0x0000
.L_0200a08c:
	.4byte gPartyState
.L_0200a090:
	.4byte 0xd8780000
.L_0200a094:
	.4byte 0x27880000
.L_0200a098:
	.4byte 0xdf5c0000
.L_0200a09c:
	.4byte 0x20a40000
.L_0200a0a0:
	.4byte gInput
.L_0200a0a4:
	.4byte MsgDjinnGiveTutorialA
.L_0200a0a8:
	.4byte MsgDjinnGiveTutorialB
.L_0200a0ac:
	.4byte MsgHey2
.L_0200a0b0:
	.4byte 0x00013333
.L_0200a0b4:
	.4byte MsgWellYouWeren
.L_0200a0b8:
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a348
	ldr r2, .L_0200a34c
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a350
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
.L_0200a0d8:
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	adds r0, r7, #0
	bl Func_02004d08
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #2
	movs r2, #20
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a34c
	ldr r3, .L_0200a348
	movs r5, #15
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a354
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	adds r0, r7, #0
	bl Func_02004d08
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	adds r0, r7, #0
	bl Func_02004d08
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	adds r0, r7, #0
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r2, #30
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r3, #0
	mov r2, r10
	strb r3, [r2]
	movs r2, #128
	adds r0, r6, #0
	ldr r1, [sp, #0]
	lsls r2, r2, #13
	mov r3, r11
	bl Func_02004b50
.L_0200a178:
	ldrh r3, [r6, #6]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r6, #6]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a178
	adds r0, r7, #0
	movs r1, #1
	bl Object_SetModeById
	movs r1, #0
	movs r0, #9
	bl Func_02004cf0
	movs r2, #0
	movs r3, #2
	mov r1, r10
	strb r3, [r1]
	str r2, [r6, #40]
	str r2, [r6, #20]
	movs r5, #7
.L_0200a1ac:
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r6, #6]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a1ac
	adds r0, r7, #0
	movs r1, #22
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02004d08
	movs r2, #0
	movs r0, #9
	adds r1, r7, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #9
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r0, #9
	movs r1, #2
	movs r2, #30
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl UiText_OpenMessageAtObject
	movs r6, #0
.L_0200a20c:
	adds r0, r7, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	adds r5, r0, #0
	cmp r6, #7
	bne .L_0200a254
	cmp r5, #0
	bne .L_0200a292
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #2
	movs r2, #20
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a348
	ldr r2, .L_0200a34c
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a358
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	b .L_0200a2ce
.L_0200a254:
	cmp r5, #1
	bne .L_0200a292
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #2
	movs r2, #20
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a34c
	ldr r3, .L_0200a348
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a35c
	adds r0, r6, r0
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl UiText_OpenMessageAtObject
	adds r6, #1
	b .L_0200a20c
.L_0200a292:
	adds r0, r7, #0
	movs r1, #22
	bl Object_SetModeById
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #4
	movs r2, #20
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a348
	ldr r2, .L_0200a34c
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a360
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
.L_0200a2ce:
	movs r0, #52
	movs r1, #4
	adds r0, #255
	bl Func_02004ba8
	movs r0, #81
	bl Func_02004e30
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a34c
	ldr r6, .L_0200a348
	ldr r5, .L_0200a364
	subs r6, r6, r3
	muls r0, r6
	movs r1, #3
	adds r0, r0, r5
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	muls r0, r6
	adds r5, #1
	adds r0, r0, r5
	bl Func_02004cd8
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	adds r0, r7, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_0200a32a
	b .L_0200a4c8
.L_0200a32a:
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a33e
	movs r0, #17
	bl Func_02004e30
	b .L_0200a4be
.L_0200a33e:
	movs r0, #35
	bl Func_02004e30
	b .L_0200a4be
	.2byte 0x0000
.L_0200a348:
	.4byte MsgDjinnGiveTutorialB
.L_0200a34c:
	.4byte MsgDjinnGiveTutorialA
.L_0200a350:
	.4byte MsgSeeYouKnow
.L_0200a354:
	.4byte MsgEvenIKnow
.L_0200a358:
	.4byte MsgOhAllRight
.L_0200a35c:
	.4byte MsgOhComeOn
.L_0200a360:
	.4byte MsgIMSo
.L_0200a364:
	.4byte MsgTheVenusDjinni
.L_0200a368:
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #9
	adds r1, r7, #0
	bl Func_02004c98
	movs r3, #160
	lsls r3, r3, #12
	str r3, [r6, #40]
	ldr r1, [sp, #0]
	mov r3, r11
	movs r2, #0
	adds r0, r6, #0
	bl Func_02004b50
	movs r0, #30
	bl FieldEvent_WaitFrames
	bl Func_02004da8
	movs r0, #9
	adds r1, r7, #0
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	adds r0, r7, #0
	movs r1, #9
	bl ObjectMotion_SetAngleToward
	movs r1, #22
	adds r0, r7, #0
	bl Object_SetModeById
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a57c
	ldr r3, .L_0200a580
	subs r5, r2, r3
	muls r0, r5
	ldr r3, .L_0200a584
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r2, #20
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r0, #9
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #0
	movs r0, #9
	bl Func_02004cf0
	movs r0, #111
	bl Func_02004e30
	movs r1, #2
	movs r0, #0
	bl Menu_AnimateSelectionToEntry
	movs r0, #112
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #114
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_02004e28
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a588
	muls r0, r5
	adds r0, r0, r3
	bl Func_02004cd8
	mov r3, r8
	movs r2, #0
	mov r1, r9
	adds r0, r6, #0
	bl Func_02004b50
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r2, #0
	movs r0, #9
	adds r1, r7, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	adds r0, r7, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_0200a4c8
	adds r0, r7, #0
	movs r1, #22
	bl Object_SetModeById
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a58c
	muls r0, r5
	adds r0, r0, r3
	bl Func_02004cd8
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	adds r0, r7, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	beq .L_0200a4c8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	ldr r3, [sp, #0]
	movs r0, #9
	asrs r1, r3, #16
	mov r3, r11
	asrs r2, r3, #16
	bl ObjectMotion_SetPositionAndReset
.L_0200a4be:
	bl Func_020011fc
	bl Func_02004db0
	b .L_0200a56c
.L_0200a4c8:
	movs r1, #22
	adds r0, r7, #0
	bl Object_SetModeById
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a580
	ldr r3, .L_0200a57c
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a590
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r2, #20
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02004d08
	movs r1, #0
	movs r0, #9
	bl Func_02004cf0
	movs r0, #112
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #114
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02004e28
	movs r2, #20
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	bl Func_02004db0
	movs r1, #0
	movs r2, #7
	movs r0, #9
	bl Func_02004dd0
	bl Func_02004dc0
	bl Func_02004c28
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #112
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #114
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200a56c:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a57c:
	.4byte MsgDjinnGiveTutorialB
.L_0200a580:
	.4byte MsgDjinnGiveTutorialA
.L_0200a584:
	.4byte MsgSeeWeDjinn
.L_0200a588:
	.4byte MsgSoWeGo
.L_0200a58c:
	.4byte MsgHmmmmDoYou
.L_0200a590:
	.4byte MsgWellThatS
	.section .text.x0200a8cc,"ax",%progbits
	.balign 4
	.global Func_020028cc
	.thumb_func
Func_020028cc:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200a994
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200a990
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	ldr r1, [r6, #8]
	ldr r0, .L_0200a998
	ldr r3, [r6, #16]
	adds r1, r1, r0
	ldr r0, .L_0200a99c
	ldr r2, [r6, #12]
	adds r3, r3, r0
	movs r0, #30
	adds r0, #255
	bl Func_02004b30
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200a990
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldr r7, [r5, #80]
	bl Random16Far
	lsls r0, r0, #1
	lsrs r0, r0, #16
	cmp r0, #0
	beq .L_0200a930
	bl Random16Far
	lsls r2, r0, #1
	adds r2, r2, r0
	lsls r2, r2, #4
	ldr r3, [r5, #8]
	lsrs r2, r2, #16
	lsls r2, r2, #16
	asrs r1, r2, #1
	subs r3, r3, r1
	str r3, [r5, #8]
	ldr r3, [r5, #16]
	subs r3, r3, r2
	b .L_0200a946
.L_0200a930:
	bl Random16Far
	ldr r3, [r5, #8]
	lsls r0, r0, #5
	lsrs r0, r0, #16
	lsls r0, r0, #16
	adds r3, r3, r0
	str r3, [r5, #8]
	ldr r3, [r5, #16]
	asrs r0, r0, #1
	adds r3, r3, r0
.L_0200a946:
	str r3, [r5, #16]
	movs r3, #0
	strb r3, [r7, #26]
	ldrb r1, [r7, #9]
	ldr r3, [r6, #80]
	movs r2, #12
	ldrb r3, [r3, #9]
	adds r0, r5, #0
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	adds r1, r5, #0
	adds r1, #35
	orrs r3, r2
	ldrb r2, [r1]
	strb r3, [r7, #9]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	adds r3, r6, #0
	adds r3, #85
	ldrb r3, [r3]
	adds r2, r5, #0
	adds r2, #85
	strb r3, [r2]
	movs r1, #9
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #2
	bl Func_02004b18
	ldr r1, .L_0200a9a0
	adds r0, r5, #0
	bl Func_02004b20
.L_0200a990:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a994:
	.4byte gFrameCount
.L_0200a998:
	.4byte 0xffe00000
.L_0200a99c:
	.4byte 0xfff00000
.L_0200a9a0:
	.4byte gData_02004f10
	.section .text.x0200aab0,"ax",%progbits
	.balign 4
	.global Func_02002ab0
	.thumb_func
Func_02002ab0:
	push {r5, r6, r7, lr}
	movs r0, #10
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	adds r5, r0, #0
	ldr r7, [r3, #32]
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r2, #152
	movs r0, #64
	movs r1, #176
	lsls r2, r2, #1
	movs r3, #240
	bl Func_02004b68
	movs r2, #160
	movs r0, #80
	movs r1, #176
	lsls r2, r2, #1
	movs r3, #240
	bl Func_02004b68
	ldr r3, .L_0200adac
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	str r3, [r5, #24]
	str r3, [r5, #28]
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	movs r0, #1
	bl WaitFrames
	ldr r6, .L_0200adb0
	movs r1, #133
	lsls r1, r1, #2
	adds r6, r6, r1
	ldr r0, [r6]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	ldr r0, [r6]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #10
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	adds r2, r5, #0
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #6
	ldr r2, [r5, #12]
	adds r0, r5, #0
	ldr r1, .L_0200adb4
	str r3, [r5, #52]
	str r3, [r5, #48]
	ldr r3, .L_0200adb8
	bl Func_02004b50
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02004da8
	ldr r0, .L_0200adbc
	bl Func_02004cd8
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #8
	movs r1, #0
	bl Func_02004cf0
	ldr r2, [r5, #12]
	ldr r3, .L_0200adc0
	adds r0, r5, #0
	ldr r1, .L_0200adc4
	bl Func_02004b50
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r0, #8
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #5
	bl Func_02004cf0
	movs r0, #78
	bl Func_02004e30
	movs r0, #162
	bl Func_02004e30
	ldr r0, [r6]
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #8
	bl Func_02004cf0
	movs r0, #61
	bl Func_02004e30
	bl Func_02004db0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_02004d20
	movs r1, #1
	ldr r0, .L_0200adc8
	negs r1, r1
	ldr r2, .L_0200adcc
	movs r3, #1
	bl Camera_MoveWithinMapBounds
	movs r5, #31
.L_0200abfe:
	movs r3, #143
	lsls r3, r3, #1
	adds r2, r7, r3
	ldrh r3, [r2]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #128
	adds r3, r3, r1
	strh r3, [r2]
	movs r3, #142
	lsls r3, r3, #1
	adds r2, r7, r3
	ldrh r3, [r2]
	movs r0, #10
	adds r3, #128
	strh r3, [r2]
	bl Object_GetById
	ldr r1, .L_0200add0
	ldr r3, [r0, #28]
	subs r5, #1
	adds r3, r3, r1
	str r3, [r0, #28]
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200abfe
	bl Func_02004da8
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r0, #8
	movs r1, #0
	bl Func_02004cf0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #9
	lsls r1, r1, #10
	bl Func_02004b78
	bl Func_02004db0
	movs r0, #153
	lsls r0, r0, #8
	adds r0, #153
	movs r1, #1
	bl Func_02004df8
	movs r3, #143
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r1, #142
	movs r3, #0
	strh r3, [r2]
	lsls r1, r1, #1
	movs r3, #224
	adds r2, r7, r1
	lsls r3, r3, #8
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	movs r3, #0
	ldr r0, .L_0200add4
	negs r1, r1
	ldr r2, .L_0200add8
	bl Camera_MoveWithinMapBounds
	bl Func_02004b40
	movs r2, #238
	movs r0, #8
	ldr r1, .L_0200addc
	lsls r2, r2, #21
	bl Func_02004c88
	movs r0, #9
	ldr r1, .L_0200ade0
	ldr r2, .L_0200ade4
	bl Func_02004c88
	movs r0, #4
	ldr r1, .L_0200ade8
	ldr r2, .L_0200adec
	bl Func_02004c88
	movs r0, #5
	ldr r1, .L_0200adf0
	ldr r2, .L_0200adf4
	bl Func_02004c88
	ldr r1, .L_0200adf8
	movs r0, #6
	ldr r2, .L_0200adfc
	bl Func_02004c88
	ldr r5, .L_0200adb0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #1
	bl WaitFrames
	bl Func_02004da8
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02004d10
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #6
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r2, #0
	movs r0, #6
	movs r1, #4
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #2
	movs r0, #8
	adds r1, #255
	bl Func_02004d10
	movs r0, #8
	movs r1, #0
	bl Func_02004cf0
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #224
	movs r2, #240
	lsls r1, r1, #4
	lsls r2, r2, #5
	adds r1, #24
	adds r2, #15
	movs r0, #8
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #163
	bl Func_02004e30
	movs r0, #0
	bl Func_020042e0
	movs r0, #65
	bl Func_02004d40
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200adac:
	.4byte 0x00013333
.L_0200adb0:
	.4byte gPartyState
.L_0200adb4:
	.4byte 0x27ff0000
.L_0200adb8:
	.4byte 0x1ecc0000
.L_0200adbc:
	.4byte MsgWeReSaved
.L_0200adc0:
	.4byte 0x1ebc0000
.L_0200adc4:
	.4byte 0x280f0000
.L_0200adc8:
	.4byte 0x283f0000
.L_0200adcc:
	.4byte 0x1e8c0000
.L_0200add0:
	.4byte 0xfffffd00
.L_0200add4:
	.4byte 0x0e470000
.L_0200add8:
	.4byte 0x1dd00000
.L_0200addc:
	.4byte 0x0e6a0000
.L_0200ade0:
	.4byte 0x0e3e0000
.L_0200ade4:
	.4byte 0x1dff0000
.L_0200ade8:
	.4byte 0x0e550000
.L_0200adec:
	.4byte 0x1df00000
.L_0200adf0:
	.4byte 0x0e490000
.L_0200adf4:
	.4byte 0x1dc50000
.L_0200adf8:
	.4byte 0x0e770000
.L_0200adfc:
	.4byte 0x1de30000
	.section .text.x0200b144,"ax",%progbits
	.balign 4
	.global Func_02003144
	.thumb_func
Func_02003144:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #252
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b15a
	b .L_0200b40a
.L_0200b15a:
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #120
	bl GameFlag_SetBit
	movs r1, #216
	movs r2, #132
	lsls r1, r1, #5
	lsls r2, r2, #6
	adds r1, #246
	movs r0, #8
	adds r2, #84
	bl ObjectMotion_SetPositionAndCommit
	bl Func_0200050c
	movs r0, #8
	bl Object_GetById
	ldr r1, [r0, #8]
	cmp r1, #0
	bge .L_0200b198
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r1, r1, r3
.L_0200b198:
	asrs r1, r1, #16
	movs r0, #8
	mov r8, r1
	bl Object_GetById
	ldr r2, [r0, #16]
	cmp r2, #0
	bge .L_0200b1b0
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_0200b1b0:
	asrs r7, r2, #16
	movs r6, #0
.L_0200b1b4:
	adds r0, r6, #0
	bl Object_GetById
	adds r5, r6, #1
	ldr r1, .L_0200b410
	lsls r2, r6, #2
	cmp r0, #0
	beq .L_0200b1ea
	str r6, [r1, r2]
	adds r0, r6, #0
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #24]
	adds r0, r6, #0
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	mov r3, r8
	adds r0, r6, #0
	lsls r1, r3, #16
	lsls r2, r7, #16
	bl Func_02004c88
	b .L_0200b1f0
.L_0200b1ea:
	movs r3, #1
	negs r3, r3
	str r3, [r1, r2]
.L_0200b1f0:
	adds r6, r5, #0
	cmp r6, #7
	ble .L_0200b1b4
	movs r0, #23
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #24]
	movs r0, #23
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	mov r3, r8
	lsls r1, r3, #16
	lsls r2, r7, #16
	movs r0, #23
	bl Func_02004c88
	bl Func_02004da8
	ldr r0, .L_0200b414
	bl Func_02004cd8
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02004e30
	movs r0, #2
	movs r1, #0
	bl Func_02004cf0
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02004d08
	movs r0, #1
	movs r1, #0
	bl Func_02004cf0
	movs r0, #3
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #0
	bl Func_02004cf0
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #23
	bl Func_02004cf0
	movs r0, #78
	bl Func_02004e30
	movs r0, #50
	bl FieldEvent_WaitFrames
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #5
	bl Func_02004cf0
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #38
	bl Func_02004e30
	movs r1, #0
	movs r0, #6
	bl Func_02004cf0
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #0
	bl Func_02004cf0
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #7
	bl Func_02004cf0
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #3
	bl Func_02004cf0
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #2
	bl Func_02004cf0
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #1
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r0, #1
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r0, #1
	movs r1, #0
	bl Func_02004cf0
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #3
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #1
	bl Func_02004cf0
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #23
	bl Func_02004cf0
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #23
	bl Func_02004cf0
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #23
	bl Func_02004cf0
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #0
	bl Func_02004cf0
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #23
	bl Func_02004cf0
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #6
	bl Func_02004cf0
	movs r0, #78
	bl Func_02004e30
	movs r1, #0
	movs r0, #23
	bl Func_02004cf0
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02004cf0
	bl Func_02004dc0
	bl Func_02004db0
	movs r6, #0
.L_0200b3cc:
	ldr r7, .L_0200b410
	lsls r5, r6, #2
	ldr r0, [r7, r5]
	cmp r0, #0
	blt .L_0200b3f6
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	ldr r0, [r7, r5]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	ldr r0, [r7, r5]
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
.L_0200b3f6:
	adds r6, #1
	cmp r6, #7
	ble .L_0200b3cc
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	bl Func_02004c28
.L_0200b40a:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200b410:
	.4byte Data_02006be8
.L_0200b414:
	.4byte MsgWeDidIt
	.section .text.x0200b6a8,"ax",%progbits
	.global Func_020036a8
	.thumb_func
Func_020036a8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #24
	bl Func_02004e08
	mov r8, r0
	bl Func_02004e10
	bl Object_GetById
	ldr r3, [r0, #80]
	ldrh r2, [r0, #32]
	ldr r3, [r3, #12]
	mov r10, r0
	adds r0, r2, #0
	muls r0, r3
	str r0, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r2, #230
	mov r9, r1
	lsls r2, r2, #1
	add r2, r9
	ldr r3, [r3, #32]
	ldr r6, [r2]
	movs r2, #150
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r5, r8
	ldr r3, [r3]
	ldrb r2, [r5]
	movs r5, #128
	lsls r5, r5, #9
	ldr r1, [r6, #8]
	ldr r0, [r6, #16]
	ldr r4, .L_0200b8c4
	cmp r3, r5
	bgt .L_0200b71e
	ldr r3, .L_0200b8c8
	movs r5, #160
	lsls r5, r5, #16
	adds r3, r1, r3
	adds r5, r1, r5
	ldr r1, .L_0200b8cc
	str r3, [sp, #12]
	movs r3, #200
	lsls r3, r3, #16
	adds r1, r0, r1
	adds r3, r0, r3
	str r5, [sp, #8]
	str r1, [sp, #4]
	str r3, [sp, #0]
	b .L_0200b73a
.L_0200b71e:
	ldr r5, .L_0200b8d0
	movs r3, #240
	adds r5, r1, r5
	str r5, [sp, #12]
	lsls r3, r3, #16
	ldr r5, .L_0200b8d4
	adds r3, r1, r3
	movs r1, #150
	lsls r1, r1, #17
	adds r5, r0, r5
	adds r1, r0, r1
	str r3, [sp, #8]
	str r5, [sp, #4]
	str r1, [sp, #0]
.L_0200b73a:
	movs r3, #0
	str r3, [sp, #16]
	adds r3, r2, #0
	mov r11, r4
	cmp r3, #0
	bne .L_0200b748
	b .L_0200b8b6
.L_0200b748:
	mov r5, r8
	ldrb r3, [r5, #1]
	cmp r3, #15
	bne .L_0200b752
	b .L_0200b89a
.L_0200b752:
	movs r1, #4
	ldrsh r3, [r5, r1]
	mov r0, r11
	ldr r6, [r0]
	mov r0, r8
	lsls r5, r3, #16
	ldr r1, [sp, #12]
	movs r2, #6
	ldrsh r3, [r0, r2]
	lsls r7, r3, #16
	cmp r5, r1
	bgt .L_0200b76c
	b .L_0200b88a
.L_0200b76c:
	ldr r2, [sp, #8]
	cmp r5, r2
	blt .L_0200b774
	b .L_0200b88a
.L_0200b774:
	ldr r3, [sp, #4]
	cmp r7, r3
	bgt .L_0200b77c
	b .L_0200b88a
.L_0200b77c:
	ldr r0, [sp, #0]
	cmp r7, r0
	blt .L_0200b784
	b .L_0200b88a
.L_0200b784:
	mov r2, r8
	movs r1, #10
	ldrsh r0, [r2, r1]
	movs r3, #1
	negs r3, r3
	movs r2, #1
	cmp r0, r3
	beq .L_0200b7b4
	movs r3, #128
	lsls r3, r3, #5
	ands r3, r0
	cmp r3, #0
	beq .L_0200b7a6
	bl GameFlag_Test
	adds r2, r0, #0
	b .L_0200b7b4
.L_0200b7a6:
	bl GameFlag_Test
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	movs r2, #1
	subs r2, r2, r3
.L_0200b7b4:
	cmp r2, #0
	beq .L_0200b88a
	cmp r6, #0
	bne .L_0200b808
	mov r2, r8
	movs r1, #2
	ldrsh r0, [r2, r1]
	adds r3, r7, #0
	adds r1, r5, #0
	movs r2, #0
	bl Func_02004b30
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200b89a
	ldr r1, .L_0200b8d8
	bl Func_02004b20
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	mov r5, r8
	adds r2, #4
	movs r3, #1
	strb r3, [r2]
	ldrh r3, [r5, #8]
	adds r0, r6, #0
	strh r3, [r6, #6]
	movs r1, #1
	bl Func_02004b18
	ldr r5, [r6, #80]
	movs r1, #192
	ldr r0, [r5, #12]
	ldr r3, .L_0200b8dc
	lsls r1, r1, #8
	mov lr, r3
	.2byte 0xf800
	str r0, [r5, #12]
	mov r0, r11
	str r6, [r0]
.L_0200b808:
	ldr r3, .L_0200b8e0
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #118
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200b89a
	ldr r3, [r6, #80]
	mov r5, r10
	ldr r0, [r3, #12]
	ldr r2, [r6, #8]
	ldr r3, [r5, #8]
	subs r1, r2, r3
	cmp r1, #0
	bge .L_0200b82c
	subs r1, r3, r2
.L_0200b82c:
	ldrh r3, [r6, #32]
	muls r3, r0
	ldr r0, [sp, #20]
	adds r2, r0, r3
	mov r3, r10
	ldr r0, [r6, #16]
	ldr r4, [r3, #16]
	subs r3, r0, r4
	cmp r3, #0
	blt .L_0200b848
	adds r3, r1, r3
	cmp r3, r2
	blt .L_0200b850
	b .L_0200b89a
.L_0200b848:
	subs r3, r4, r0
	adds r3, r1, r3
	cmp r3, r2
	bge .L_0200b89a
.L_0200b850:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b89a
	ldr r3, .L_0200b8e0
	movs r5, #128
	lsls r5, r5, #2
	adds r5, #18
	adds r3, r3, r5
	ldrb r3, [r3]
	cmp r3, #9
	bne .L_0200b87c
	movs r2, #179
	movs r3, #128
	lsls r2, r2, #1
	lsls r3, r3, #6
	add r2, r9
	adds r3, #139
	strh r3, [r2]
	b .L_0200b89a
.L_0200b87c:
	ldr r2, [sp, #16]
	movs r3, #170
	lsls r3, r3, #1
	adds r2, #100
	add r3, r9
	strh r2, [r3]
	b .L_0200b89a
.L_0200b88a:
	cmp r6, #0
	beq .L_0200b89a
	adds r0, r6, #0
	bl Func_02004b38
	movs r3, #0
	mov r0, r11
	str r3, [r0]
.L_0200b89a:
	ldr r1, [sp, #16]
	movs r2, #4
	adds r1, #1
	movs r3, #20
	str r1, [sp, #16]
	add r11, r2
	add r8, r3
	cmp r1, #127
	bhi .L_0200b8b6
	mov r5, r8
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_0200b8b6
	b .L_0200b748
.L_0200b8b6:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b8c4:
	.4byte gData_02006c08
.L_0200b8c8:
	.4byte 0xff600000
.L_0200b8cc:
	.4byte 0xfed40000
.L_0200b8d0:
	.4byte 0xff100000
.L_0200b8d4:
	.4byte 0xfe3e0000
.L_0200b8d8:
	.4byte gData_02006bd4
.L_0200b8dc:
	.4byte IwramMulQ16
.L_0200b8e0:
	.4byte gPartyState
	.section .text.x0200bd44,"ax",%progbits
	.global Func_02003d44
	.thumb_func
Func_02003d44:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r0
	movs r0, #1
	bl WaitFrames
	movs r6, #192
	movs r0, #10
	adds r0, #255
	lsls r6, r6, #18
	bl GameFlag_ClearBit
	ldr r3, [r6, #108]
	movs r0, #214
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r5, #0
	str r5, [r3]
	ldr r3, .L_0200be88
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #84
	strb r5, [r0]
	movs r2, #218
	ldr r3, [r6, #108]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #16
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	mov r3, r10
	cmp r3, #2
	bne .L_0200bdb4
	movs r0, #128
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02004b78
	movs r0, #141
	bl Func_02004e30
.L_0200bdb4:
	bl Func_02004da8
	movs r0, #128
	lsls r0, r0, #7
	bl Runtime_BumpAllocateAlternatePool
	adds r7, r0, #0
	adds r1, r7, #0
	ldr r0, .L_0200be8c
	bl Func_02004aa8
	movs r2, #128
	lsls r2, r2, #5
	adds r1, r7, r2
	ldr r0, .L_0200be90
	bl Func_02004aa8
	ldr r6, .L_0200be94
	ldr r5, .L_0200be98
	ldrh r3, [r5]
	adds r0, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200be0a
	lsls r3, r2, #1
	adds r3, r3, r2
	mov r1, r10
	adds r2, #1
	strh r2, [r6]
	lsls r2, r1, #5
	ldr r1, .L_0200be9c
	lsls r3, r3, #2
	adds r3, r3, r6
	adds r3, #4
	adds r2, r2, r1
	stmia r3!, {r2}
	ldr r2, .L_0200bea0
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #24
	adds r2, #16
	str r2, [r3]
.L_0200be0a:
	strh r0, [r5]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200be30
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r6
	adds r3, #4
	adds r2, #1
	stmia r3!, {r7}
	strh r2, [r6]
	ldr r2, .L_0200bea4
	stmia r3!, {r2}
	ldr r2, .L_0200bea8
	str r2, [r3]
.L_0200be30:
	strh r1, [r5]
	ldr r0, .L_0200beac
	movs r1, #144
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r0, #246
	bl Func_02004e30
	ldr r2, .L_0200beb0
	ldr r3, .L_0200be84
	mov r8, r2
	mov r0, r8
	strh r3, [r0]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200bebc
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #210
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200beb4
	stmia r3!, {r2}
	ldr r2, .L_0200beb8
	str r2, [r3]
	b .L_0200bebc
	.2byte 0x0000
.L_0200be84:
	.4byte 0x00000e00
.L_0200be88:
	.4byte gPartyState
.L_0200be8c:
	.4byte gWorldMapPackedTiles
.L_0200be90:
	.4byte gWorldMapPackedFrames
.L_0200be94:
	.4byte gIoWriteQueue
.L_0200be98:
	.4byte 0x04000208
.L_0200be9c:
	.4byte gWorldMapPalettes
.L_0200bea0:
	.4byte 0x050001c0
.L_0200bea4:
	.4byte 0x06001000
.L_0200bea8:
	.4byte 0x84000400
.L_0200beac:
	.4byte Func_02003c50
.L_0200beb0:
	.4byte gData_02006e08
.L_0200beb4:
	.4byte 0x06002000
.L_0200beb8:
	.4byte 0x84000140
.L_0200bebc:
	strh r1, [r5]
	movs r0, #2
	bl FieldEvent_WaitFrames
	ldr r3, .L_0200bef8
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200bf04
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #186
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200befc
	stmia r3!, {r2}
	ldr r2, .L_0200bf00
	str r2, [r3]
	b .L_0200bf04
	.2byte 0x0000
.L_0200bef8:
	.4byte 0x00000d00
.L_0200befc:
	.4byte 0x06002000
.L_0200bf00:
	.4byte 0x84000140
.L_0200bf04:
	strh r1, [r5]
	movs r0, #2
	bl FieldEvent_WaitFrames
	ldr r3, .L_0200bf40
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200bf4c
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #162
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200bf44
	stmia r3!, {r2}
	ldr r2, .L_0200bf48
	str r2, [r3]
	b .L_0200bf4c
	.2byte 0x0000
.L_0200bf40:
	.4byte 0x00000c00
.L_0200bf44:
	.4byte 0x06002000
.L_0200bf48:
	.4byte 0x84000140
.L_0200bf4c:
	strh r1, [r5]
	movs r0, #2
	bl FieldEvent_WaitFrames
	movs r1, #176
	lsls r1, r1, #4
	mov r2, r8
	strh r1, [r2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200bf86
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #138
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200c040
	stmia r3!, {r2}
	ldr r2, .L_0200c044
	str r2, [r3]
.L_0200bf86:
	strh r1, [r5]
	movs r0, #2
	bl FieldEvent_WaitFrames
	movs r1, #160
	lsls r1, r1, #4
	mov r11, r1
	mov r2, r11
	mov r3, r8
	strh r2, [r3]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200bfc4
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #228
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200c040
	stmia r3!, {r2}
	ldr r2, .L_0200c044
	str r2, [r3]
.L_0200bfc4:
	strh r1, [r5]
	movs r0, #2
	bl FieldEvent_WaitFrames
	movs r1, #144
	lsls r1, r1, #4
	mov r9, r1
	mov r2, r9
	mov r3, r8
	strh r2, [r3]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c002
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #180
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200c040
	stmia r3!, {r2}
	ldr r2, .L_0200c044
	str r2, [r3]
.L_0200c002:
	strh r1, [r5]
	movs r0, #2
	bl FieldEvent_WaitFrames
	ldr r3, .L_0200c03c
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c048
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #132
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200c040
	stmia r3!, {r2}
	ldr r2, .L_0200c044
	str r2, [r3]
	b .L_0200c048
.L_0200c03c:
	.4byte 0x00000800
.L_0200c040:
	.4byte 0x06002000
.L_0200c044:
	.4byte 0x84000140
.L_0200c048:
	strh r1, [r5]
	movs r0, #140
	bl FieldEvent_WaitFrames
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c07a
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #180
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
.L_0200c07a:
	strh r1, [r5]
	movs r0, #4
	bl FieldEvent_WaitFrames
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c0ac
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #228
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
.L_0200c0ac:
	strh r1, [r5]
	movs r0, #4
	bl FieldEvent_WaitFrames
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c0de
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #138
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
.L_0200c0de:
	strh r1, [r5]
	movs r0, #4
	bl FieldEvent_WaitFrames
	mov r1, r9
	mov r2, r8
	strh r1, [r2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c116
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #162
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
.L_0200c116:
	strh r1, [r5]
	movs r0, #4
	bl FieldEvent_WaitFrames
	mov r1, r11
	mov r2, r8
	strh r1, [r2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c14e
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #186
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
.L_0200c14e:
	strh r1, [r5]
	movs r0, #4
	bl FieldEvent_WaitFrames
	movs r1, #176
	lsls r1, r1, #4
	mov r2, r8
	strh r1, [r2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c188
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #210
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
.L_0200c188:
	strh r1, [r5]
	movs r0, #4
	bl FieldEvent_WaitFrames
	ldr r3, .L_0200c1c4
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c1d0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #234
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
	b .L_0200c1d0
	.2byte 0x0000
.L_0200c1c4:
	.4byte 0x00000c00
.L_0200c1c8:
	.4byte 0x06002000
.L_0200c1cc:
	.4byte 0x84000140
.L_0200c1d0:
	strh r1, [r5]
	mov r1, r10
	cmp r1, #1
	bne .L_0200c25c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02004b78
	movs r0, #141
	bl Func_02004e30
	ldr r3, .L_0200c228
	mov r2, r8
	strh r3, [r2]
	movs r0, #4
	bl FieldEvent_WaitFrames
	ldr r3, .L_0200c22c
	mov r0, r8
	strh r3, [r0]
	movs r0, #4
	bl FieldEvent_WaitFrames
	ldr r3, .L_0200c230
	mov r1, r8
	strh r3, [r1]
	movs r0, #4
	bl FieldEvent_WaitFrames
	ldr r3, .L_0200c234
	mov r2, r8
	strh r3, [r2]
	movs r0, #45
	bl FieldEvent_WaitFrames
	ldr r0, .L_0200c238
	bl Scheduler_RemoveCallbackFar
	b .L_0200c23c
	.2byte 0x0000
.L_0200c228:
	.4byte 0x00000d00
.L_0200c22c:
	.4byte 0x00000e00
.L_0200c230:
	.4byte 0x00000f00
.L_0200c234:
	.4byte 0x00001000
.L_0200c238:
	.4byte Func_02003c50
.L_0200c23c:
	bl Func_02004ac8
	movs r0, #128
	mov r3, r8
	lsls r0, r0, #19
	ldrh r1, [r3]
	adds r0, #82
	bl QueueIoWriteDelay2
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #80
	movs r1, #0
	bl QueueIoWriteDelay2
	b .L_0200c2c6
.L_0200c25c:
	ldr r3, .L_0200c294
	mov r0, r8
	strh r3, [r0]
	movs r0, #4
	bl FieldEvent_WaitFrames
	ldr r3, .L_0200c298
	mov r1, r8
	strh r3, [r1]
	movs r0, #4
	bl FieldEvent_WaitFrames
	ldr r3, .L_0200c29c
	mov r2, r8
	strh r3, [r2]
	movs r0, #4
	bl FieldEvent_WaitFrames
	ldr r3, .L_0200c2a0
	mov r0, r8
	strh r3, [r0]
	movs r0, #45
	bl FieldEvent_WaitFrames
	ldr r0, .L_0200c2a4
	bl Scheduler_RemoveCallbackFar
	b .L_0200c2a8
.L_0200c294:
	.4byte 0x00000d00
.L_0200c298:
	.4byte 0x00000e00
.L_0200c29c:
	.4byte 0x00000f00
.L_0200c2a0:
	.4byte 0x00001000
.L_0200c2a4:
	.4byte Func_02003c50
.L_0200c2a8:
	bl Func_02004ac8
	movs r0, #128
	mov r2, r8
	lsls r0, r0, #19
	ldrh r1, [r2]
	adds r0, #82
	bl QueueIoWriteDelay2
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #80
	movs r1, #0
	bl QueueIoWriteDelay2
.L_0200c2c6:
	adds r0, r7, #0
	bl MemFree
	movs r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x0200c2e0,"ax",%progbits
	.global Func_020042e0
	.thumb_func
Func_020042e0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #128
	lsls r0, r0, #7
	bl Runtime_BumpAllocateAlternatePool
	adds r7, r0, #0
	adds r1, r7, #0
	ldr r0, .L_0200c4d4
	bl Func_02004aa8
	movs r1, #128
	lsls r1, r1, #6
	adds r1, r1, r7
	ldr r0, .L_0200c4d8
	mov r8, r1
	bl Func_02004aa8
	ldr r6, .L_0200c4dc
	ldr r5, .L_0200c4e0
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c334
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r6]
	ldr r2, .L_0200c4e4
	adds r3, r3, r6
	adds r3, #4
	stmia r3!, {r2}
	ldr r2, .L_0200c4e8
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #24
	adds r2, #16
	str r2, [r3]
.L_0200c334:
	strh r1, [r5]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c35c
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r6
	adds r3, #4
	adds r2, #1
	stmia r3!, {r7}
	strh r2, [r6]
	movs r2, #192
	lsls r2, r2, #19
	stmia r3!, {r2}
	ldr r2, .L_0200c4ec
	str r2, [r3]
.L_0200c35c:
	strh r1, [r5]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c384
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r6
	adds r3, #4
	strh r2, [r6]
	mov r2, r8
	stmia r3!, {r2}
	ldr r2, .L_0200c4f0
	stmia r3!, {r2}
	ldr r2, .L_0200c4f4
	str r2, [r3]
.L_0200c384:
	strh r1, [r5]
	movs r0, #128
	movs r1, #252
	lsls r0, r0, #19
	lsls r1, r1, #6
	adds r0, #80
	adds r1, #65
	bl QueueIoWriteDelay2
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #19
	lsls r1, r1, #5
	adds r0, #82
	adds r1, #16
	bl QueueIoWriteDelay2
	ldr r2, .L_0200c4f8
	movs r3, #0
	strh r3, [r2]
	movs r3, #176
	lsls r3, r3, #1
	strh r3, [r2, #2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c3e0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r6
	strh r2, [r6]
	movs r2, #194
	adds r3, #4
	lsls r2, r2, #8
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #8
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0200c3e0:
	strh r1, [r5]
	movs r7, #180
	movs r6, #153
	lsls r7, r7, #6
	lsls r6, r6, #1
	adds r7, #96
	adds r6, #255
	movs r5, #95
.L_0200c3f0:
	lsls r3, r6, #4
	cmp r3, #0
	bge .L_0200c3f8
	adds r3, #127
.L_0200c3f8:
	asrs r2, r3, #7
	negs r3, r7
	cmp r3, #0
	bge .L_0200c402
	adds r3, #127
.L_0200c402:
	movs r1, #176
	lsls r1, r1, #1
	asrs r3, r3, #7
	adds r3, r3, r1
	ldr r1, .L_0200c4f8
	movs r0, #1
	mov r8, r1
	strh r2, [r1]
	mov r2, r8
	strh r3, [r2, #2]
	bl WaitFrames
	movs r3, #176
	lsls r3, r3, #1
	subs r5, #1
	adds r7, r7, r3
	adds r6, #17
	cmp r5, #0
	bge .L_0200c3f0
	movs r0, #164
	movs r5, #192
	bl Func_02004e30
	lsls r5, r5, #18
	movs r0, #120
	bl WaitFrames
	ldr r3, [r5, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #128
	movs r0, #254
	adds r3, r3, r1
	lsls r2, r2, #1
	lsls r0, r0, #7
	str r2, [r3]
	movs r1, #0
	adds r0, #255
	bl Func_02004d78
	movs r0, #1
	bl Func_02004d88
	movs r0, #128
	lsls r0, r0, #19
	movs r1, #128
	lsls r1, r1, #5
	adds r0, #82
	bl QueueIoWriteDelay2
	movs r0, #1
	bl WaitFrames
	movs r1, #0
	movs r0, #0
	bl Func_02004d78
	movs r0, #60
	bl Func_02004d88
	movs r0, #78
	bl Func_02004e30
	movs r0, #60
	bl WaitFrames
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02004e30
	movs r0, #120
	bl WaitFrames
	ldr r2, [r5, #24]
	movs r3, #1
	strh r3, [r2, #4]
	movs r0, #1
	bl WaitFrames
	bl Func_02004ad8
	bl Func_02004ac8
	ldr r3, .L_0200c4fc
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3]
	bl Func_02004bb8
	movs r6, #0
	mov r3, r8
	mov r1, r8
	strh r6, [r3]
	strh r6, [r1, #2]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c4d4:
	.4byte gWorldMapPackedTilesB
.L_0200c4d8:
	.4byte gWorldMapPackedFramesB
.L_0200c4dc:
	.4byte gIoWriteQueue
.L_0200c4e0:
	.4byte 0x04000208
.L_0200c4e4:
	.4byte gWorldMapPalettesB
.L_0200c4e8:
	.4byte 0x050001c0
.L_0200c4ec:
	.4byte 0x84000400
.L_0200c4f0:
	.4byte 0x06001000
.L_0200c4f4:
	.4byte 0x84000800
.L_0200c4f8:
	.4byte gBgScroll
.L_0200c4fc:
	.4byte gPartyState
	.section .text.x0200c634,"ax",%progbits
	.balign 4
	.global Func_02004634
	.thumb_func
Func_02004634:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	sub sp, #8
	adds r6, r0, #0
	mov r9, r3
	movs r7, #0
.L_0200c64e:
	movs r0, #16
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	ldr r2, [r6, #12]
	adds r0, #255
	bl Func_02004b30
	mov r8, sp
	lsls r3, r7, #2
	mov r1, r8
	str r0, [r3, r1]
	cmp r0, #0
	beq .L_0200c6f8
	ldr r3, [r6, #20]
	ldr r5, [r0, #80]
	str r3, [r0, #20]
	ldr r1, .L_0200c688
	adds r3, r0, #0
	adds r3, #85
	movs r2, #0
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	mov r10, r1
	str r6, [r0, #104]
	cmp r5, #0
	beq .L_0200c6f8
	b .L_0200c68c
	.2byte 0x0000
.L_0200c688:
	.4byte 0x00000000
.L_0200c68c:
	movs r1, #0
	adds r0, r5, #0
	bl Animation_ApplyChildArgument
	mov r2, r10
	strb r2, [r5, #26]
	ldrb r0, [r5, #16]
	bl Resource_ResetEntry
	movs r3, #248
	lsls r3, r3, #3
	add r3, r9
	ldrh r3, [r3]
	movs r2, #1
	strb r3, [r5, #16]
	ldrb r3, [r5, #17]
	ldr r1, .L_0200c6e8
	orrs r3, r2
	strb r3, [r5, #17]
	ldrb r3, [r5, #16]
	ldr r2, .L_0200c6ec
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r2, [r3, #2]
	ldrh r3, [r5, #8]
	lsls r2, r2, #17
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
	movs r1, #33
	ldrb r3, [r5, #5]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #63
	ands r3, r2
	movs r1, #64
	orrs r3, r1
	strb r3, [r5, #5]
	ldrb r3, [r5, #7]
	ands r2, r3
	movs r3, #128
	orrs r2, r3
	b .L_0200c6f0
	.2byte 0x0000
.L_0200c6e8:
	.4byte 0xfffffc00
.L_0200c6ec:
	.4byte gResourceTableEntries
.L_0200c6f0:
	ldr r3, [r5, #40]
	strb r2, [r5, #7]
	mov r2, r10
	strb r2, [r3, #22]
.L_0200c6f8:
	adds r7, #1
	cmp r7, #1
	ble .L_0200c64e
	.global Data_020046fe
Data_020046fe:
	ldr r2, [sp, #0]
	ldr r3, .L_0200c73c
	ldr r0, [r2, #80]
	str r3, [r2, #108]
	ldrb r1, [r0, #9]
	movs r2, #13
	negs r2, r2
	adds r3, r2, #0
	movs r4, #4
	ands r3, r1
	orrs r3, r4
	strb r3, [r0, #9]
	mov r3, r8
	ldr r1, [r3, #4]
	add sp, #8
	ldr r0, [r1, #80]
	ldrb r3, [r0, #9]
	ands r2, r3
	ldr r3, .L_0200c740
	orrs r2, r4
	str r3, [r1, #108]
	adds r1, #35
	movs r3, #2
	strb r2, [r0, #9]
	strb r3, [r1]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c73c:
	.4byte Func_020045e4
.L_0200c740:
	.4byte Func_02004598
	.section .text.x0200c744,"ax",%progbits
	.global Func_02004744
	.thumb_func
Func_02004744:
	push {r5, r6, r7, lr}
	lsls r3, r0, #4
	subs r3, r3, r0
	movs r2, #128
	lsls r3, r3, #1
	lsls r2, r2, #8
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r6, r3, #1
	movs r7, #0
	cmp r0, #0
	beq .L_0200c75e
	movs r7, #100
.L_0200c75e:
	cmp r6, #15
	bgt .L_0200c770
	ldr r3, .L_0200c8b0
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #1
	bhi .L_0200c770
	movs r7, #200
.L_0200c770:
	cmp r6, #7
	bgt .L_0200c782
	ldr r3, .L_0200c8b0
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #1
	bhi .L_0200c782
	movs r7, #200
.L_0200c782:
	ldr r1, .L_0200c8b4
	adds r2, r7, r6
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r2, r3
	bne .L_0200c790
	b .L_0200c8ae
.L_0200c790:
	ldr r5, .L_0200c8b8
	strh r2, [r1]
	movs r1, #128
	ldr r3, .L_0200c8bc
	adds r0, r5, #0
	lsls r1, r1, #1
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_0200c8c0
	cmp r7, #0
	bne .L_0200c7b2
	ldr r1, .L_0200c8c4
	adds r0, r5, #0
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_0200c89e
.L_0200c7b2:
	cmp r7, #100
	bne .L_0200c7c2
	ldr r1, .L_0200c8c8
	adds r0, r5, #0
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_0200c7cc
.L_0200c7c2:
	ldr r1, .L_0200c8c4
	adds r0, r5, #0
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_0200c7cc:
	movs r2, #0
	adds r5, r6, #0
	mov r12, r2
	cmp r5, #7
	ble .L_0200c7e8
	ldr r3, .L_0200c8c0
	ldr r0, .L_0200c8b8
	ldr r1, .L_0200c8cc
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	movs r3, #8
	mov r12, r3
	subs r5, #8
.L_0200c7e8:
	cmp r6, #15
	ble .L_0200c800
	ldr r0, .L_0200c8d0
	ldr r1, .L_0200c8d4
	ldr r3, .L_0200c8c0
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	adds r5, r6, #0
	movs r0, #16
	mov r12, r0
	subs r5, #16
.L_0200c800:
	cmp r6, #23
	ble .L_0200c818
	movs r2, #32
	ldr r0, .L_0200c8d8
	ldr r1, .L_0200c8dc
	ldr r3, .L_0200c8c0
	mov lr, r3
	.2byte 0xf800
	adds r5, r6, #0
	movs r2, #24
	mov r12, r2
	subs r5, #24
.L_0200c818:
	cmp r6, #30
	ble .L_0200c82e
	ldr r3, .L_0200c8c0
	ldr r0, .L_0200c8e0
	ldr r1, .L_0200c8e4
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	movs r3, #0
	mov r12, r3
	movs r5, #0
.L_0200c82e:
	cmp r5, #0
	beq .L_0200c89e
	movs r7, #0
	cmp r7, r5
	bge .L_0200c89e
.L_0200c838:
	mov r3, r12
	cmp r3, #0
	bge .L_0200c840
	adds r3, #7
.L_0200c840:
	asrs r3, r3, #3
	lsls r1, r3, #5
	lsrs r3, r7, #31
	adds r3, r7, r3
	asrs r0, r3, #1
	ldr r2, .L_0200c8e8
	ldr r3, .L_0200c8ec
	adds r2, r1, r2
	adds r3, r1, r3
	adds r6, r2, r0
	adds r0, r3, r0
	movs r3, #1
	ands r3, r7
	movs r4, #2
	cmp r3, #0
	bne .L_0200c87e
	movs r4, #1
.L_0200c862:
	ldrb r3, [r0]
	ldrb r1, [r6]
	movs r2, #240
	ands r2, r3
	movs r3, #15
	ands r3, r1
	orrs r2, r3
	adds r4, #1
	strb r2, [r0]
	adds r6, #4
	adds r0, #4
	cmp r4, #3
	ble .L_0200c862
	b .L_0200c898
.L_0200c87e:
	ldrb r3, [r0]
	ldrb r1, [r6]
	movs r2, #15
	ands r2, r3
	movs r3, #240
	ands r3, r1
	orrs r2, r3
	subs r4, #1
	strb r2, [r0]
	adds r6, #4
	adds r0, #4
	cmp r4, #0
	bge .L_0200c87e
.L_0200c898:
	adds r7, #1
	cmp r7, r5
	blt .L_0200c838
.L_0200c89e:
	ldr r3, .L_0200c8f0
	movs r1, #128
	movs r2, #0
	ldrsh r0, [r3, r2]
	lsls r1, r1, #1
	ldr r2, .L_0200c8b8
	bl VramCache_Load
.L_0200c8ae:
	pop {r5, r6, r7, pc}
.L_0200c8b0:
	.4byte gFrameCount
.L_0200c8b4:
	.4byte gData_02006e0c
.L_0200c8b8:
	.4byte Data_02006e20
.L_0200c8bc:
	.4byte IwramClearWords
.L_0200c8c0:
	.4byte IwramCopyWords
.L_0200c8c4:
	.4byte Data_02007020
.L_0200c8c8:
	.4byte gData_02006fa0
.L_0200c8cc:
	.4byte gData_02006f20
.L_0200c8d0:
	.4byte Data_02006e40
.L_0200c8d4:
	.4byte Data_02006f40
.L_0200c8d8:
	.4byte Data_02006e60
.L_0200c8dc:
	.4byte Data_02006f60
.L_0200c8e0:
	.4byte Data_02006e80
.L_0200c8e4:
	.4byte Data_02006f80
.L_0200c8e8:
	.4byte Data_02006f24
.L_0200c8ec:
	.4byte Data_02006e24
.L_0200c8f0:
	.4byte gData_02006e0e
	.section .text.x0200c8f4,"ax",%progbits
	.global Func_020048f4
	.thumb_func
Func_020048f4:
	push {r5, r6, r7, lr}
	ldr r0, .L_0200c990
	sub sp, #16
	str r0, [sp, #0]
	ldr r3, .L_0200c994
	ldr r2, .L_0200c998
	movs r1, #0
	ldrsh r3, [r3, r1]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r6, r3, #5
	bl Func_02004e10
	bl Object_GetById
	ldr r3, .L_0200c99c
	movs r2, #3
	ldr r3, [r3]
	adds r7, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0200c950
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c950
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #11
	cmp r3, r2
	ble .L_0200c950
	ldr r5, .L_0200c9a0
	ldrh r3, [r5]
	subs r3, #1
	strh r3, [r5]
	lsls r3, r3, #16
	cmp r3, #0
	bgt .L_0200c950
	movs r0, #1
	bl Func_02004c08
	ldr r3, .L_0200c98c
	strh r3, [r5]
.L_0200c950:
	bl Func_02004c10
	bl Func_02004744
	adds r0, r7, #0
	add r1, sp, #4
	adds r0, #8
	bl Render_ProjectPoint
	add r3, sp, #4
	ldr r0, [sp, #0]
	ldr r1, [r3]
	ldr r3, [r3, #4]
	movs r2, #0
	stmia r0!, {r2}
	ldr r5, .L_0200c9a4
	subs r1, #16
	lsls r1, r1, #16
	subs r3, #32
	orrs r3, r1
	adds r4, r0, #0
	orrs r3, r5
	str r4, [sp, #0]
	stmia r0!, {r3}
	movs r3, #128
	lsls r3, r3, #3
	adds r1, r0, #0
	orrs r6, r3
	b .L_0200c9a8
	.2byte 0x0000
.L_0200c98c:
	.4byte 0x0000000a
.L_0200c990:
	.4byte Data_02006e14
.L_0200c994:
	.4byte gData_02006e0e
.L_0200c998:
	.4byte gResourceTableEntries
.L_0200c99c:
	.4byte gInput
.L_0200c9a0:
	.4byte gData_02006e10
.L_0200c9a4:
	.4byte 0x80004000
.L_0200c9a8:
	str r1, [sp, #0]
	str r6, [r0]
	movs r1, #255
	ldr r0, .L_0200c9b8
	bl Func_02004ad0
	add sp, #16
	pop {r5, r6, r7, pc}
.L_0200c9b8:
	.4byte Data_02006e14
	.section .bss,"aw",%nobits
	.global Data_02006be8
Data_02006be8:
	.space 0x00000020
	.global gData_02006c08
gData_02006c08:
	.space 0x00000200
	.global gData_02006e08
gData_02006e08:
	.space 0x00000004
	.global gData_02006e0c
gData_02006e0c:
	.space 0x00000002
	.global gData_02006e0e
gData_02006e0e:
	.space 0x00000002
	.global gData_02006e10
gData_02006e10:
	.space 0x00000004
	.global Data_02006e14
Data_02006e14:
	.space 0x0000000c
	.global Data_02006e20
Data_02006e20:
	.space 0x00000004
	.global Data_02006e24
Data_02006e24:
	.space 0x0000001c
	.global Data_02006e40
Data_02006e40:
	.space 0x00000020
	.global Data_02006e60
Data_02006e60:
	.space 0x00000020
	.global Data_02006e80
Data_02006e80:
	.space 0x000000a0
	.global gData_02006f20
gData_02006f20:
	.space 0x00000004
	.global Data_02006f24
Data_02006f24:
	.space 0x0000001c
	.global Data_02006f40
Data_02006f40:
	.space 0x00000020
	.global Data_02006f60
Data_02006f60:
	.space 0x00000020
	.global Data_02006f80
Data_02006f80:
	.space 0x00000020
	.global gData_02006fa0
gData_02006fa0:
	.space 0x00000080
	.global Data_02007020
Data_02007020:
