.syntax unified
	.thumb
	.global Func_080cec34
	.thumb_func
Func_080cec34:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	bl Func_080ceba8
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	mov r8, r0
	mov r11, r3
	cmp r0, #0
	bne .L_080cec58
	b .L_080cee2c
.L_080cec58:
	movs r2, #0
	ldrsh r7, [r0, r2]
	movs r1, #0
	movs r3, #2
	mov r9, r1
	add r8, r3
	cmp r7, #0
	bne .L_080cec6a
	b .L_080cee2c
.L_080cec6a:
	movs r3, #128
	lsls r3, r3, #7
	ands r3, r7
	cmp r3, #0
	beq .L_080cec76
	b .L_080cee2c
.L_080cec76:
	mov r2, r8
	movs r1, #2
	ldrsh r3, [r2, r1]
	movs r1, #0
	ldrsh r6, [r2, r1]
	mov r10, r3
	movs r3, #4
	ldrsh r2, [r2, r3]
	movs r1, #6
	str r2, [sp, #0]
	add r8, r1
	cmp r7, #128
	beq .L_080ceca4
	movs r2, #1
	negs r2, r2
	cmp r10, r2
	beq .L_080ceca4
	mov r0, r10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ceca4
	b .L_080cee08
.L_080ceca4:
	add r5, sp, #4
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_080cec24
	cmp r0, #0
	beq .L_080cecb4
	b .L_080cee08
.L_080cecb4:
	adds r3, r7, #0
	subs r3, #128
	movs r1, #128
	movs r0, #232
	lsls r3, r3, #16
	lsls r1, r1, #9
	movs r6, #0
	adds r0, #255
	cmp r3, r1
	bls .L_080cece6
	movs r0, #10
	adds r0, #255
	cmp r7, #130
	beq .L_080cece6
	movs r0, #161
	lsls r0, r0, #1
	cmp r7, #132
	beq .L_080cece6
	movs r0, #234
	adds r0, #255
	cmp r7, #131
	beq .L_080cece6
	movs r0, #234
	movs r6, #1
	adds r0, #255
.L_080cece6:
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	bl Func_080200c0
	adds r5, r0, #0
	cmp r5, #0
	bne .L_080cecf8
	b .L_080cee08
.L_080cecf8:
	movs r3, #197
	lsls r3, r3, #1
	add r3, r11
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080ced18
	adds r1, r5, #0
	movs r2, #0
	adds r1, #85
	movs r3, #2
	strb r3, [r1]
	str r2, [r5, #20]
	str r2, [r5, #12]
	b .L_080ced70
.L_080ced18:
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Func_080201c0
	ldr r3, [r5, #12]
	str r0, [r5, #20]
	adds r3, r3, r0
	str r3, [r5, #12]
	cmp r6, #0
	beq .L_080ced70
	adds r2, r5, #0
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	ldr r2, .L_080cedc8
	ldr r3, [r5, #20]
	ldr r1, .L_080cedcc
	adds r3, r3, r2
	str r3, [r5, #20]
	str r3, [r5, #12]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	ldr r3, [r5, #8]
	asrs r4, r3, #20
	ldr r3, [r5, #16]
	asrs r0, r3, #20
	cmp r2, #0
	beq .L_080ced5c
	movs r1, #156
	lsls r1, r1, #1
	adds r3, r2, r1
	ldr r1, [r3]
.L_080ced5c:
	lsls r3, r0, #7
	adds r3, r4, r3
	lsls r3, r3, #2
	adds r1, r1, r3
	ldrb r3, [r1, #2]
	subs r3, #242
	cmp r3, #5
	bhi .L_080ced70
	movs r3, #0
	strb r3, [r1, #2]
.L_080ced70:
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetMode
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	mov r0, r10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ced92
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetMode
.L_080ced92:
	adds r0, r5, #0
	bl Object_ResetMotion
	ldr r2, [r5, #8]
	cmp r2, #0
	bge .L_080ceda6
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_080ceda6:
	adds r3, r5, #0
	adds r3, #100
	asrs r2, r2, #16
	strh r2, [r3]
	ldr r1, .L_080cedc4
	ldr r3, [r5, #16]
	mov r10, r1
	cmp r3, #0
	bge .L_080cedd0
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	b .L_080cedd0
	.2byte 0x0000
.L_080cedc4:
	.4byte 0x00000000
.L_080cedc8:
	.4byte 0xfff00000
.L_080cedcc:
	.4byte gMapCellBuffer
.L_080cedd0:
	adds r2, r5, #0
	asrs r3, r3, #16
	adds r2, #102
	strh r3, [r2]
	adds r6, r5, #0
	movs r3, #1
	subs r2, #67
	strb r3, [r2]
	mov r1, r9
	adds r6, #89
	movs r2, #138
	strb r3, [r6]
	lsls r2, r2, #1
	lsls r3, r1, #2
	adds r3, r3, r2
	mov r1, r11
	str r5, [r1, r3]
	cmp r7, #131
	bne .L_080cee00
	mov r0, r9
	adds r0, #64
	ldr r1, [sp, #0]
	bl Func_080d3b28
.L_080cee00:
	cmp r7, #133
	bne .L_080cee08
	mov r2, r10
	strb r2, [r6]
.L_080cee08:
	movs r3, #1
	add r9, r3
	mov r1, r9
	cmp r1, #15
	bgt .L_080cee2c
	mov r3, r8
	movs r2, #0
	ldrsh r7, [r3, r2]
	movs r1, #2
	add r8, r1
	cmp r7, #0
	beq .L_080cee2c
	ldr r2, .L_080cee3c
	adds r3, r7, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080cee2c
	b .L_080cec76
.L_080cee2c:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080cee3c:
	.4byte 0x00004000
