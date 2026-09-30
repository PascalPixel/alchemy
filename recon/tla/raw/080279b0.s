.syntax unified
	.thumb
	.global Func_080279b0
	.thumb_func
Func_080279b0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #100
	movs r1, #0
	adds r7, r0, #0
	ldr r3, .L_08027d14
	movs r0, #2
	str r1, [sp, #16]
	str r0, [sp, #4]
	ldr r1, .L_08027d1c
	movs r2, #143
	lsls r2, r2, #2
	adds r3, r3, r2
	ldrh r2, [r3]
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	beq .L_080279f0
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #5
	str r3, [sp, #4]
	b .L_080279fc
.L_080279f0:
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
.L_080279fc:
	ldr r5, .L_08027d1c
	movs r2, #15
	ldr r3, [r5]
	ldr r1, .L_08027d20
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r0, [r1, r3]
	lsls r3, r0, #16
	lsrs r3, r3, #16
	mov r8, r3
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	str r0, [sp, #12]
	cmp r8, r3
	bne .L_08027a2c
	ldr r0, [sp, #16]
	movs r3, #4
	b .L_08027bae
.L_08027a24:
	mov r1, r9
	asrs r1, r1, #16
	str r1, [sp, #12]
	b .L_08027bb2
.L_08027a2c:
	mov r3, sp
	movs r2, #0
	adds r3, #88
	str r3, [sp, #0]
	str r2, [sp, #16]
	ldr r3, [r7, #8]
	ldr r0, [sp, #0]
	mov r1, r8
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	movs r0, #128
	lsls r0, r0, #12
	ldr r2, [sp, #0]
	bl Func_0801489c
	ldr r3, .L_08027d24
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08027a74
	ldr r3, [r5]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08027a66
	b .L_08027bb2
.L_08027a66:
	movs r0, #100
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08027a74
	b .L_08027bb2
.L_08027a74:
	ldr r3, [r7, #8]
	add r1, sp, #76
	str r3, [r1]
	ldr r3, [r7, #12]
	mov r11, r1
	str r3, [r1, #4]
	ldr r3, [r7, #16]
	movs r0, #128
	str r3, [r1, #8]
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #85
	add r1, r8
	lsls r0, r0, #12
	mov r2, r11
	bl Func_0801489c
	ldr r3, [r7, #8]
	add r2, sp, #64
	str r3, [r2]
	ldr r3, [r7, #12]
	ldr r1, .L_08027d28
	str r3, [r2, #4]
	ldr r3, [r7, #16]
	movs r0, #128
	str r3, [r2, #8]
	add r1, r8
	lsls r0, r0, #12
	mov r9, r2
	bl Func_0801489c
	adds r0, r7, #0
	ldr r1, [sp, #0]
	bl Func_0802db64
	mov r1, r11
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0802db64
	mov r1, r9
	adds r6, r0, #0
	adds r0, r7, #0
	bl Func_0802db64
	orrs r5, r6
	orrs r5, r0
	cmp r5, #0
	beq .L_08027bb2
	add r3, sp, #20
	ldr r1, [sp, #16]
	mov r10, r3
	movs r3, #128
	lsls r3, r3, #5
	add r3, r8
	mov r0, r10
	strh r3, [r0, r1]
	ldr r3, .L_08027d2c
	mov r2, r10
	add r3, r8
	strh r3, [r2, #2]
	movs r3, #128
	lsls r3, r3, #6
	add r3, r8
	strh r3, [r0, #4]
	ldr r3, .L_08027d30
	mov r1, r10
	add r3, r8
	strh r3, [r1, #6]
	movs r3, #192
	lsls r3, r3, #6
	add r3, r8
	strh r3, [r2, #8]
	ldr r3, .L_08027d34
	movs r1, #0
	add r3, r8
	strh r3, [r0, #10]
	str r1, [sp, #8]
	mov r8, r9
.L_08027b12:
	ldr r2, [sp, #8]
	mov r0, r10
	lsls r3, r2, #1
	ldrsh r2, [r0, r3]
	ldr r3, [r7, #8]
	lsls r2, r2, #16
	str r3, [sp, #88]
	ldr r3, [r7, #12]
	lsrs r5, r2, #16
	str r3, [sp, #92]
	ldr r3, [r7, #16]
	movs r0, #128
	str r3, [sp, #96]
	add r3, sp, #88
	adds r1, r5, #0
	lsls r0, r0, #12
	mov r9, r2
	adds r2, r3, #0
	bl Func_0801489c
	ldr r3, [r7, #8]
	mov r0, r11
	str r3, [r0]
	ldr r3, [r7, #12]
	movs r2, #168
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	lsls r2, r2, #5
	str r3, [r0, #8]
	adds r2, #85
	movs r0, #128
	adds r1, r5, r2
	lsls r0, r0, #12
	mov r2, r11
	bl Func_0801489c
	ldr r3, [r7, #8]
	mov r0, r8
	str r3, [r0]
	ldr r3, [r7, #12]
	ldr r1, .L_08027d28
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	adds r5, r5, r1
	str r3, [r0, #8]
	movs r0, #128
	adds r1, r5, #0
	lsls r0, r0, #12
	mov r2, r8
	bl Func_0801489c
	add r2, sp, #88
	adds r1, r2, #0
	adds r0, r7, #0
	bl Func_0802db64
	mov r1, r11
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0802db64
	mov r1, r8
	adds r6, r0, #0
	adds r0, r7, #0
	bl Func_0802db64
	orrs r5, r6
	orrs r5, r0
	cmp r5, #0
	bne .L_08027ba0
	b .L_08027a24
.L_08027ba0:
	ldr r3, [sp, #8]
	adds r3, #1
	str r3, [sp, #8]
	cmp r3, #6
	blt .L_08027b12
	ldr r0, [sp, #16]
	movs r3, #1
.L_08027bae:
	orrs r0, r3
	str r0, [sp, #16]
.L_08027bb2:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	cmp r3, #0
	beq .L_08027bdc
	ldr r1, [sp, #16]
	movs r2, #3
	ands r2, r1
	cmp r2, #0
	beq .L_08027bd4
	movs r0, #194
	lsls r0, r0, #1
	adds r2, r3, r0
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_08027bdc
.L_08027bd4:
	movs r1, #194
	lsls r1, r1, #1
	adds r3, r3, r1
	strh r2, [r3]
.L_08027bdc:
	ldr r2, [sp, #16]
	cmp r2, #0
	beq .L_08027bec
	adds r0, r7, #0
	movs r1, #9
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_08027bf4
.L_08027bec:
	adds r0, r7, #0
	ldr r1, [sp, #4]
	bl ObjectDispatch_ApplyArgumentToChildren
.L_08027bf4:
	ldr r3, [sp, #16]
	cmp r3, #0
	beq .L_08027c4e
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #60]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r0, [sp, #16]
	movs r3, #3
	ands r3, r0
	cmp r3, #0
	beq .L_08027c38
	ldr r1, [sp, #12]
	ldrh r2, [r7, #6]
	lsls r3, r1, #16
	lsrs r3, r3, #16
	subs r3, r3, r2
	lsls r3, r3, #16
	asrs r1, r3, #16
	movs r3, #128
	lsls r3, r3, #5
	cmp r1, r3
	ble .L_08027c2c
	adds r1, r3, #0
.L_08027c2c:
	ldr r3, .L_08027d2c
	cmp r1, r3
	bge .L_08027c34
	adds r1, r3, #0
.L_08027c34:
	adds r3, r2, r1
	strh r3, [r7, #6]
.L_08027c38:
	movs r2, #100
	adds r2, r2, r7
	mov r8, r2
	movs r3, #0
	mov r0, r8
	adds r2, r7, #0
	strh r3, [r0]
	adds r2, #102
	movs r3, #2
	strh r3, [r2]
	b .L_08027ca2
.L_08027c4e:
	add r3, sp, #88
	ldr r2, [r3, #4]
	ldr r1, [r3]
	adds r0, r7, #0
	ldr r3, [r3, #8]
	bl Object_SetMoveTarget
	ldr r1, [r7, #36]
	ldr r6, .L_08027d38
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	ldr r1, [r7, #44]
	adds r5, r0, #0
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	adds r5, r5, r0
	adds r0, r5, #0
	bl Func_080149e0
	ldr r1, [sp, #16]
	str r1, [r7, #36]
	str r1, [r7, #44]
	ldr r2, [sp, #12]
	lsls r1, r2, #16
	adds r2, r7, #0
	adds r2, #36
	lsrs r1, r1, #16
	bl Func_0801489c
	movs r3, #100
	adds r3, r3, r7
	mov r8, r3
	ldrh r2, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_08027ca2
	subs r3, r2, #1
	mov r1, r8
	strh r3, [r1]
.L_08027ca2:
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	ldr r3, .L_08027d1c
	ldr r1, .L_08027d3c
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	movs r2, #143
	lsls r3, r3, #2
	lsls r2, r2, #1
	ldr r4, [r1, r3]
	adds r1, r0, r2
	ldrh r0, [r1]
	subs r3, r4, r0
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, #0
	bge .L_08027ccc
	adds r3, #7
.L_08027ccc:
	asrs r2, r3, #3
	movs r3, #128
	lsls r3, r3, #2
	cmp r2, r3
	ble .L_08027cd8
	adds r2, r3, #0
.L_08027cd8:
	ldr r3, .L_08027d40
	cmp r2, r3
	bge .L_08027ce0
	adds r2, r3, #0
.L_08027ce0:
	adds r3, r2, #0
	adds r3, #15
	cmp r3, #30
	bhi .L_08027cec
	ldrh r3, [r1]
	subs r2, r4, r3
.L_08027cec:
	adds r3, r0, r2
	strh r3, [r1]
	adds r3, r7, #0
	adds r3, #84
	ldrb r6, [r3]
	cmp r6, #1
	bne .L_08027dc0
	adds r0, r7, #0
	adds r0, #8
	ldr r5, [r7, #80]
	bl Func_0802dac0
	cmp r0, #9
	bne .L_08027d44
	ldr r3, [r5, #44]
	strb r6, [r3, #6]
	ldr r3, .L_08027d18
	strb r3, [r5, #26]
	b .L_08027d4c
	.2byte 0x0000
.L_08027d14:
	.4byte gPartyState
.L_08027d18:
	.4byte 0x00000000
.L_08027d1c:
	.4byte gInput
.L_08027d20:
	.4byte Data_0802ec5c
.L_08027d24:
	.4byte Data_03001238
.L_08027d28:
	.4byte 0xffffeaab
.L_08027d2c:
	.4byte 0xfffff000
.L_08027d30:
	.4byte 0xffffe000
.L_08027d34:
	.4byte 0xffffd000
.L_08027d38:
	.4byte IwramMulQ16
.L_08027d3c:
	.4byte Data_0802eca0
.L_08027d40:
	.4byte 0xfffffe00
.L_08027d44:
	ldr r2, [r5, #44]
	movs r3, #9
	strb r3, [r2, #6]
	strb r6, [r5, #26]
.L_08027d4c:
	cmp r0, #6
	bne .L_08027dc0
	mov r1, r8
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_08027dc0
	ldr r2, [sp, #16]
	cmp r2, #0
	bne .L_08027dc0
	movs r0, #14
	adds r0, #255
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	bl Func_08023220
	adds r5, r0, #0
	cmp r5, #0
	beq .L_08027dc0
	ldr r1, .L_08027e00
	ldr r6, [r5, #80]
	bl ObjectDispatch_Initialize
	add r0, sp, #16
	ldrb r0, [r0]
	adds r3, r5, #0
	adds r3, #85
	adds r2, r5, #0
	strb r0, [r3]
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	cmp r6, #0
	beq .L_08027dba
	movs r1, #1
	adds r0, r6, #0
	bl Animation_ApplyChildArgument
	add r1, sp, #16
	ldrb r1, [r1]
	movs r2, #13
	strb r1, [r6, #26]
	ldrb r1, [r6, #5]
	negs r2, r2
	adds r3, r2, #0
	ands r3, r1
	movs r1, #4
	orrs r3, r1
	strb r3, [r6, #5]
	ldrb r3, [r6, #9]
	ands r2, r3
	movs r3, #8
	orrs r2, r3
	strb r2, [r6, #9]
.L_08027dba:
	movs r3, #10
	mov r2, r8
	strh r3, [r2]
.L_08027dc0:
	bl Func_08026e60
	ldr r3, .L_08027e04
	ldr r1, .L_08027e08
	movs r0, #140
	lsls r0, r0, #2
	adds r3, r3, r0
	ldrh r2, [r3]
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_08027de8
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #251
	strh r3, [r2]
.L_08027de8:
	ldrh r3, [r7, #4]
	movs r0, #1
	adds r3, #1
	strh r3, [r7, #4]
	add sp, #100
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08027e00:
	.4byte Data_0802ec94
.L_08027e04:
	.4byte gPartyState
.L_08027e08:
	.4byte gInput
