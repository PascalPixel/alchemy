.syntax unified
	.thumb
	.global Func_080dcb44
	.thumb_func
Func_080dcb44:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #88]
	movs r0, #165
	lsls r0, r0, #2
	adds r1, r7, r0
	ldrb r2, [r1]
	sub sp, #24
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080dcb6e
	adds r3, #255
	strb r3, [r1]
	b .L_080dcdba
.L_080dcb6e:
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #138
	adds r3, r7, r1
	ldrb r3, [r3]
	movs r2, #1
	eors r2, r3
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #4
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r6, r7, r3
	movs r5, #0
.L_080dcb8a:
	movs r2, #162
	lsls r2, r2, #2
	adds r3, r7, r2
	ldrh r0, [r3]
	lsls r3, r5, #3
	adds r0, r0, r3
	movs r1, #160
	lsls r0, r0, #16
	bl __divsi3
	bl Trig_Sin
	adds r5, #1
	asrs r0, r0, #14
	strh r0, [r6]
	adds r6, #2
	cmp r5, #159
	ble .L_080dcb8a
	movs r3, #162
	lsls r3, r3, #2
	adds r2, r7, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #4
	strh r3, [r2]
	lsls r0, r0, #2
	adds r0, #138
	adds r1, r7, r0
	ldrb r3, [r1]
	movs r2, #1
	eors r3, r2
	strb r3, [r1]
	cmp r3, #0
	beq .L_080dcc0e
	bl Func_080dce60
	movs r1, #199
	lsls r1, r1, #1
	adds r1, #255
	movs r2, #163
	adds r3, r7, r1
	lsls r2, r2, #2
	movs r0, #0
	ldrsb r0, [r3, r0]
	adds r3, r7, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	subs r1, #2
	lsls r3, r3, #5
	lsls r0, r0, #10
	orrs r0, r3
	adds r3, r7, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r1, #1
	orrs r0, r3
	movs r3, #128
	lsls r3, r3, #14
	orrs r0, r3
	bl Func_080d170c
	movs r0, #1
	bl BattleFx_StartBufferInterpolation
.L_080dcc0e:
	movs r2, #164
	lsls r2, r2, #2
	adds r3, r7, r2
	ldrh r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl Func_080dcf2c
	movs r0, #203
	lsls r0, r0, #1
	adds r0, #255
	adds r3, r7, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080dcc38
	cmp r3, #8
	beq .L_080dcc38
	cmp r3, #16
	beq .L_080dcc38
	b .L_080dcd8c
.L_080dcc38:
	movs r1, #164
	lsls r1, r1, #2
	adds r5, r7, r1
	ldrh r0, [r5]
	bl Object_GetById
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #146
	adds r2, r2, r7
	adds r6, r0, #0
	ldrh r0, [r2]
	mov r11, r2
	bl Object_GetById
	mov r8, r0
	cmp r6, #0
	bne .L_080dcc5e
	b .L_080dcd8c
.L_080dcc5e:
	cmp r0, #0
	bne .L_080dcc64
	b .L_080dcd8c
.L_080dcc64:
	ldrh r0, [r5]
	bl BattleAction_FindDescriptor
	movs r3, #0
	ldrsh r0, [r0, r3]
	ldr r3, [r6, #8]
	add r1, sp, #12
	str r3, [r1]
	mov r10, r1
	bl Resource_GetMetadataRecordFarFar
	movs r2, #8
	ldrsb r2, [r0, r2]
	ldr r3, [r6, #12]
	ldr r5, .L_080dccec
	lsls r2, r2, #16
	adds r3, r3, r2
	adds r3, r3, r5
	mov r2, r10
	str r3, [r2, #4]
	mov r1, r11
	ldr r3, [r6, #16]
	ldrh r0, [r1]
	str r3, [r2, #8]
	mov r3, r8
	ldr r3, [r3, #80]
	mov r9, r3
	bl BattleAction_FindDescriptor
	mov r1, r8
	ldr r3, [r1, #8]
	mov r6, sp
	movs r2, #0
	ldrsh r0, [r0, r2]
	str r3, [r6]
	bl Resource_GetMetadataRecordFarFar
	movs r3, #8
	ldrsb r3, [r0, r3]
	mov r0, r8
	ldr r2, [r0, #12]
	lsls r3, r3, #16
	adds r2, r2, r3
	adds r5, r2, r5
	str r5, [r6, #4]
	mov r1, r9
	ldr r3, [r0, #16]
	movs r0, #128
	str r3, [r6, #8]
	lsls r0, r0, #8
	ldrh r3, [r1, #18]
	cmp r3, r0
	bne .L_080dccd2
	ldr r1, .L_080dccf0
	b .L_080dcd0a
.L_080dccd2:
	movs r0, #128
	lsls r0, r0, #7
	cmp r3, r0
	bne .L_080dccf8
	ldr r3, [r6]
	movs r1, #192
	ldr r0, .L_080dccf4
	lsls r1, r1, #11
	adds r3, r3, r1
	str r3, [r6]
	adds r3, r2, r0
	b .L_080dcd0c
	.2byte 0x0000
.L_080dccec:
	.4byte 0xfffe0000
.L_080dccf0:
	.4byte 0xffe60000
.L_080dccf4:
	.4byte 0xfff20000
.L_080dccf8:
	movs r1, #192
	lsls r1, r1, #8
	cmp r3, r1
	bne .L_080dcd0e
	ldr r3, [r6]
	ldr r0, .L_080dcd80
	ldr r1, .L_080dcd84
	adds r3, r3, r0
	str r3, [r6]
.L_080dcd0a:
	adds r3, r2, r1
.L_080dcd0c:
	str r3, [r6, #4]
.L_080dcd0e:
	movs r0, #211
	lsls r0, r0, #1
	adds r0, #255
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_080200c0
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080dcd8c
	ldr r2, [r5, #80]
	movs r3, #0
	mov r9, r2
	adds r2, r5, #0
	adds r2, #85
	strb r3, [r2]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r5, #48]
	str r3, [r5, #52]
	mov r3, r10
	ldr r0, [r3, #8]
	ldr r3, [r6, #8]
	mov r2, r10
	ldr r1, [r2]
	subs r0, r0, r3
	ldr r3, [r6]
	subs r1, r1, r3
	bl ArcTan2
	ldr r3, .L_080dcd88
	ldr r2, .L_080dcd7c
	str r3, [r5, #108]
	mov r3, r9
	strb r2, [r3, #26]
	ldrb r2, [r3, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	strh r0, [r5, #6]
	orrs r3, r2
	mov r0, r9
	strb r3, [r0, #9]
	mov r2, r10
	mov r0, r10
	ldr r1, [r2]
	ldr r3, [r0, #8]
	ldr r2, [r2, #4]
	adds r0, r5, #0
	bl Object_SetPosition
	b .L_080dcd8c
.L_080dcd7c:
	.4byte 0x00000000
.L_080dcd80:
	.4byte 0xfffa0000
.L_080dcd84:
	.4byte 0xfff20000
.L_080dcd88:
	.4byte Func_080dcf08
.L_080dcd8c:
	movs r1, #203
	lsls r1, r1, #1
	adds r1, #255
	adds r5, r7, r1
	ldrb r3, [r5]
	adds r2, r3, #0
	cmp r2, #99
	beq .L_080dcdba
	cmp r2, #0
	bne .L_080dcda8
	movs r0, #130
	bl Audio_PlayCue
	ldrb r3, [r5]
.L_080dcda8:
	adds r3, #1
	movs r2, #240
	strb r3, [r5]
	lsls r2, r2, #22
	lsls r3, r3, #24
	cmp r3, r2
	bls .L_080dcdba
	movs r3, #0
	strb r3, [r5]
.L_080dcdba:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
