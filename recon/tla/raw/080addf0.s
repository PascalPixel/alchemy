.syntax unified
	.thumb
	.global Func_080addf0
	.thumb_func
Func_080addf0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	movs r3, #128
	movs r2, #133
	mov r5, sp
	movs r4, #0
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r4, [r5]
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_080adeb8
	adds r2, #184
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	str r4, [r5]
	adds r0, r5, #0
	ldr r1, .L_080adebc
	ldr r2, .L_080adec0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #19
	movs r3, #128
	adds r2, #212
	lsls r3, r3, #24
.L_080ade30:
	ldr r1, [r2, #8]
	ands r1, r3
	mov r11, r1
	cmp r1, #0
	bne .L_080ade30
	movs r3, #128
	movs r2, #133
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r1, [r5]
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_080adec4
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_080adebc
	movs r3, #255
	strb r3, [r2, #4]
	mov r3, r11
	str r3, [r5]
	movs r3, #128
	lsls r3, r3, #19
	mov r9, r2
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_080adec8
	ldr r2, .L_080adecc
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Func_080af7ac
	ldr r7, .L_080adeb8
	ldr r3, .L_080adeb0
	movs r1, #140
	lsls r1, r1, #2
	mov r8, r3
	adds r2, r7, r1
	movs r3, #1
	strh r3, [r2]
	adds r1, #2
	movs r2, #141
	adds r3, r7, r1
	lsls r2, r2, #2
	movs r1, #2
	strh r1, [r3]
	movs r0, #4
	adds r3, r7, r2
	strh r0, [r3]
	ldr r2, .L_080adeb4
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #54
	mov r10, r2
	adds r2, r7, r3
	movs r3, #8
	strh r3, [r2]
	movs r3, #142
	lsls r3, r3, #2
	adds r2, r7, r3
	subs r3, #56
	strh r3, [r2]
	b .L_080aded0
	.2byte 0x0000
.L_080adeb0:
	.4byte 0x00000000
.L_080adeb4:
	.4byte 0x00000004
.L_080adeb8:
	.4byte gPartyState
.L_080adebc:
	.4byte Data_02001000
.L_080adec0:
	.4byte 0x850007c8
.L_080adec4:
	.4byte GameFlagBytes
.L_080adec8:
	.4byte Data_02000520
.L_080adecc:
	.4byte 0x85000298
.L_080aded0:
	adds r3, #58
	adds r2, r7, r3
	movs r3, #128
	lsls r3, r3, #1
	strh r3, [r2]
	movs r2, #143
	lsls r2, r2, #2
	adds r3, r7, r2
	strh r1, [r3]
	movs r1, #144
	lsls r1, r1, #2
	mov r2, r11
	adds r3, r7, r1
	adds r1, #2
	strh r2, [r3]
	adds r3, r7, r1
	subs r1, #46
	strh r2, [r3]
	adds r3, r7, r1
	str r0, [r3]
	bl Func_080afdd8
	movs r0, #5
	bl Func_080afdd8
	movs r1, #149
	movs r0, #4
	bl OwnerAction_Add
	movs r1, #140
	movs r0, #4
	bl OwnerAction_Add
	movs r1, #141
	movs r0, #6
	bl OwnerAction_Add
	movs r1, #33
	movs r0, #7
	bl OwnerAction_Add
	movs r1, #149
	movs r0, #0
	bl OwnerAction_Add
	movs r1, #140
	movs r0, #0
	bl OwnerAction_Add
	movs r1, #140
	movs r0, #1
	bl OwnerAction_Add
	movs r1, #141
	movs r0, #2
	bl OwnerAction_Add
	movs r1, #144
	movs r0, #2
	bl OwnerAction_Add
	movs r2, #163
	movs r3, #200
	lsls r2, r2, #2
	str r3, [r7, #16]
	mov r1, r11
	adds r3, r7, r2
	adds r2, #48
	str r1, [r3]
	adds r3, r7, r2
	mov r1, r11
	subs r2, #72
	strh r1, [r3]
	adds r3, r7, r2
	mov r1, r8
	strb r1, [r3]
	ldr r5, .L_080adfa0
	movs r1, #128
	subs r2, #72
	lsls r1, r1, #2
	adds r3, r7, r2
	adds r1, #42
	strb r5, [r3]
	subs r2, #1
	adds r3, r7, r1
	strb r5, [r3]
	ldr r6, .L_080adfa4
	adds r3, r7, r2
	subs r1, #5
	strb r5, [r3]
	mov r2, r8
	adds r3, r7, r1
	adds r1, #1
	strb r2, [r3]
	adds r3, r7, r1
	mov r2, r11
	strb r6, [r3]
	str r2, [r7]
	bl Func_080add74
	movs r1, #182
	lsls r1, r1, #2
	adds r3, r7, r1
	b .L_080adfa8
.L_080adfa0:
	.4byte 0x00000001
.L_080adfa4:
	.4byte 0x00000008
.L_080adfa8:
	str r0, [r3]
	ldr r3, .L_080ae00c
	mov r2, r11
	str r2, [r3]
	ldr r3, .L_080ae010
	mov r1, r8
	strb r1, [r3]
	movs r1, #128
	str r2, [r7, #4]
	lsls r1, r1, #2
	ldrb r2, [r3]
	adds r1, #74
	adds r3, r7, r1
	strb r2, [r3]
	ldr r3, .L_080ae014
	mov r2, r11
	strh r2, [r3]
	ldr r2, .L_080ae018
	ldr r3, .L_080ae008
	adds r1, #60
	strh r3, [r2]
	ldr r3, .L_080ae01c
	mov r8, r9
	ldrh r2, [r3]
	adds r3, r7, r1
	strb r2, [r3]
	movs r2, #62
	adds r2, #255
	adds r3, r7, r2
	mov r1, r10
	adds r2, #1
	strb r1, [r3]
	adds r3, r7, r2
	strb r1, [r3]
	movs r1, #64
	adds r1, #255
	adds r3, r7, r1
	mov r2, r10
	strb r2, [r3]
	adds r1, #1
	movs r2, #66
	adds r3, r7, r1
	adds r2, #255
	strb r6, [r3]
	adds r1, #2
	adds r3, r7, r2
	strb r6, [r3]
	b .L_080ae020
.L_080ae008:
	.4byte 0xffffffff
.L_080ae00c:
	.4byte gFrameWaitCount
.L_080ae010:
	.4byte gAutoSleepEnabled
.L_080ae014:
	.4byte gIdleFrameCount
.L_080ae018:
	.4byte Data_020036d0
.L_080ae01c:
	.4byte Data_02003860
.L_080ae020:
	adds r2, #2
	adds r3, r7, r1
	strb r6, [r3]
	adds r1, #2
	adds r3, r7, r2
	movs r2, #16
	strb r2, [r3]
	adds r3, r7, r1
	adds r1, #1
	strb r2, [r3]
	adds r3, r7, r1
	strb r2, [r3]
	movs r2, #163
	lsls r2, r2, #1
	adds r3, r7, r2
	adds r1, #2
	movs r2, #32
	strb r2, [r3]
	adds r3, r7, r1
	adds r1, #1
	strb r2, [r3]
	adds r3, r7, r1
	strb r2, [r3]
	movs r3, #74
	adds r3, #255
	adds r2, r7, r3
	adds r1, #2
	movs r3, #64
	strb r3, [r2]
	adds r2, r7, r1
	adds r1, #1
	strb r3, [r2]
	adds r2, r7, r1
	strb r3, [r2]
	movs r7, #0
.L_080ae066:
	lsls r3, r7, #1
	adds r3, r3, r7
	movs r2, #136
	lsls r3, r3, #2
	lsls r2, r2, #5
	add r3, r8
	adds r2, #184
	adds r5, r3, r2
	movs r6, #7
.L_080ae078:
	bl Random16
	lsls r3, r0, #2
	adds r3, r3, r0
	lsrs r3, r3, #16
	subs r6, #1
	strb r3, [r5]
	adds r5, #1
	cmp r6, #0
	bge .L_080ae078
	adds r7, #1
	cmp r7, #31
	ble .L_080ae066
	ldr r3, .L_080ae0d4
	movs r4, #152
	ldr r5, .L_080ae0d8
	lsls r4, r4, #5
	movs r7, #0
	mov r12, r3
	movs r0, #0
	adds r4, #56
.L_080ae0a2:
	adds r2, r0, r7
	mov r1, r12
	adds r3, r2, r1
	adds r1, r3, r4
	adds r2, r2, r5
	movs r6, #4
.L_080ae0ae:
	ldrb r3, [r2]
	subs r6, #1
	strb r3, [r1]
	adds r2, #1
	adds r1, #1
	cmp r6, #0
	bge .L_080ae0ae
	adds r7, #1
	adds r0, #4
	cmp r7, #3
	ble .L_080ae0a2
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ae0d4:
	.4byte Data_02001000
.L_080ae0d8:
	.4byte Data_080b1f2c
