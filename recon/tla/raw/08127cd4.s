.syntax unified
	.thumb
	.global Func_08127cd4
	.thumb_func
Func_08127cd4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	sub sp, #124
	str r3, [sp, #24]
	movs r2, #0
	adds r5, r0, #0
	adds r3, #64
	mov r1, sp
	movs r0, #116
	str r2, [sp, #32]
	adds r1, #96
	strb r2, [r3]
	adds r0, #255
	str r1, [sp, #28]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08127d10
	add r0, sp, #32
	bl Func_0812764c
	adds r5, r0, #0
.L_08127d10:
	movs r2, #165
	lsls r2, r2, #2
	cmp r5, r2
	bcc .L_08127d1a
	movs r5, #1
.L_08127d1a:
	lsls r3, r5, #1
	ldr r2, .L_08127f68
	adds r3, r3, r5
	lsls r3, r3, #3
	adds r3, r3, r2
	mov r11, r3
	movs r3, #2
	add r3, r11
	mov r10, r3
	ldrb r3, [r3, #8]
	movs r1, #0
	cmp r3, #0
	bne .L_08127d46
	mov r2, r11
	adds r2, #10
.L_08127d38:
	adds r1, #1
	cmp r1, #4
	bgt .L_08127d46
	adds r2, #1
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08127d38
.L_08127d46:
	cmp r1, #5
	bne .L_08127d54
	ldr r4, .L_08127f6c
	movs r6, #2
	mov r11, r4
	add r6, r11
	mov r10, r6
.L_08127d54:
	movs r1, #6
	movs r2, #0
	str r1, [sp, #20]
	str r2, [sp, #16]
	mov r5, r10
	movs r7, #0
	mov r6, r11
	adds r5, #8
.L_08127d64:
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_08127d86
	ldrh r0, [r6]
	adds r0, #8
	bl Summon_IsEntryFlagged
	negs r3, r0
	orrs r3, r0
	movs r2, #2
	lsrs r3, r3, #31
	subs r3, r2, r3
	ldrb r2, [r5]
	ldr r4, [sp, #20]
	muls r3, r2
	subs r4, r4, r3
	str r4, [sp, #20]
.L_08127d86:
	adds r7, #1
	adds r6, #2
	adds r5, #1
	cmp r7, #4
	bls .L_08127d64
	mov r2, r11
	mov r6, sp
	movs r1, #76
	movs r3, #15
	str r2, [sp, #8]
	adds r6, #56
	add r1, sp
	add r3, r11
	movs r4, #8
	str r6, [sp, #12]
	movs r7, #0
	mov r8, r1
	mov r9, r3
	add r10, r4
	movs r6, #0
.L_08127dae:
	mov r1, r10
	ldrb r2, [r1]
	movs r3, #1
	mov r1, r9
	ldr r4, [sp, #12]
	add r10, r3
	ldrb r3, [r1]
	str r2, [r6, r4]
	subs r5, r3, r2
	movs r4, #1
	add r9, r4
	cmp r5, #0
	ble .L_08127df8
	ldr r1, [sp, #8]
	ldrh r0, [r1]
	adds r0, #8
	bl Summon_IsEntryFlagged
	negs r1, r0
	orrs r1, r0
	lsrs r1, r1, #31
	movs r3, #2
	subs r1, r3, r1
	ldr r0, [sp, #20]
	bl __divsi3
	cmp r0, r5
	bge .L_08127de8
	adds r5, r0, #0
.L_08127de8:
	bl Random16
	adds r3, r5, #1
	muls r3, r0
	mov r2, r8
	lsrs r3, r3, #16
	str r3, [r6, r2]
	b .L_08127dfe
.L_08127df8:
	movs r3, #0
	mov r4, r8
	str r3, [r6, r4]
.L_08127dfe:
	ldr r1, [sp, #8]
	adds r7, #1
	adds r1, #2
	str r1, [sp, #8]
	adds r6, #4
	cmp r7, #4
	bls .L_08127dae
	ldr r4, [sp, #12]
	mov r1, r8
.L_08127e10:
	movs r2, #0
	mov r10, r2
	movs r7, #0
	movs r5, #0
	mov r6, r11
.L_08127e1a:
	ldrh r3, [r6]
	mov r2, r8
	adds r0, r3, #0
	ldr r3, [r5, r2]
	adds r6, #2
	adds r0, #8
	cmp r3, #0
	beq .L_08127e66
	str r1, [sp, #4]
	str r4, [sp, #0]
	bl Summon_IsEntryFlagged
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	adds r2, r3, #0
	movs r3, #2
	subs r2, r3, r2
	ldr r3, [sp, #20]
	ldr r1, [sp, #4]
	ldr r4, [sp, #0]
	cmp r2, r3
	ble .L_08127e50
	movs r3, #0
	mov r2, r8
	str r3, [r5, r2]
	b .L_08127e66
.L_08127e50:
	ldr r3, [r5, r4]
	adds r3, #1
	str r3, [r5, r4]
	ldr r3, [r5, r1]
	subs r3, #1
	str r3, [r5, r1]
	ldr r3, [sp, #20]
	subs r3, r3, r2
	str r3, [sp, #20]
	movs r2, #1
	mov r10, r2
.L_08127e66:
	adds r7, #1
	adds r5, #4
	cmp r7, #4
	bls .L_08127e1a
	mov r3, r10
	cmp r3, #0
	bne .L_08127e10
	ldr r2, [sp, #24]
	mov r4, r11
	ldrb r3, [r4, #20]
	adds r2, #66
	strb r3, [r2]
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08127e8c
	cmp r3, #1
	beq .L_08127f0a
	movs r7, #0
	b .L_08127f9e
.L_08127e8c:
	add r6, sp, #36
	mov r8, r6
	movs r7, #0
	mov r3, r8
.L_08127e94:
	stmia r3!, {r7}
	adds r7, #1
	cmp r7, #4
	bls .L_08127e94
	movs r7, #0
	mov r6, r8
.L_08127ea0:
	bl Random16
	lsls r5, r0, #2
	adds r5, r5, r0
	bl Random16
	lsls r3, r0, #2
	adds r3, r3, r0
	lsrs r5, r5, #16
	lsrs r3, r3, #16
	lsls r5, r5, #2
	lsls r3, r3, #2
	ldr r1, [r6, r5]
	ldr r2, [r6, r3]
	adds r7, #1
	str r2, [r6, r5]
	str r1, [r6, r3]
	cmp r7, #9
	bls .L_08127ea0
	ldr r1, [sp, #12]
	ldr r2, [sp, #16]
	movs r7, #0
	mov r12, r1
	mov r5, r8
	lsls r0, r2, #1
.L_08127ed2:
	ldr r2, [r5]
	mov r4, r12
	lsls r1, r2, #2
	ldr r3, [r4, r1]
	cmp r3, #0
	ble .L_08127f00
	ldr r6, [sp, #12]
	ldr r3, [sp, #28]
	ldr r1, [r6, r1]
	lsls r4, r2, #1
	adds r2, r0, r3
.L_08127ee8:
	mov r6, r11
	ldrh r3, [r6, r4]
	subs r1, #1
	adds r3, #8
	strh r3, [r2]
	ldr r3, [sp, #16]
	adds r2, #2
	adds r3, #1
	adds r0, #2
	str r3, [sp, #16]
	cmp r1, #0
	bne .L_08127ee8
.L_08127f00:
	adds r7, #1
	adds r5, #4
	cmp r7, #4
	bls .L_08127ed2
	b .L_08127fa2
.L_08127f0a:
	ldr r1, [sp, #16]
	ldr r2, [sp, #28]
	movs r6, #36
	ldr r4, [sp, #12]
	add r6, sp
	lsls r3, r1, #1
	mov r8, r6
	adds r6, r3, r2
.L_08127f1a:
	movs r5, #0
	movs r7, #0
	adds r1, r4, #0
	add r2, sp, #36
.L_08127f22:
	ldmia r1!, {r3}
	cmp r3, #0
	beq .L_08127f2c
	stmia r2!, {r7}
	adds r5, #1
.L_08127f2c:
	adds r7, #1
	cmp r7, #4
	bls .L_08127f22
	cmp r5, #0
	beq .L_08127fa2
	str r4, [sp, #0]
	bl Random16
	adds r3, r5, #0
	muls r3, r0
	lsrs r3, r3, #16
	lsls r3, r3, #2
	mov r1, r8
	ldr r2, [r1, r3]
	mov r1, r11
	lsls r3, r2, #1
	ldrh r3, [r1, r3]
	ldr r4, [sp, #0]
	adds r3, #8
	strh r3, [r6]
	ldr r3, [sp, #16]
	lsls r2, r2, #2
	adds r3, #1
	str r3, [sp, #16]
	adds r6, #2
	ldr r3, [r4, r2]
	subs r3, #1
	str r3, [r4, r2]
	b .L_08127f1a
	.2byte 0x0000
.L_08127f68:
	.4byte Data_0812ce7c
.L_08127f6c:
	.4byte Data_0812ce94
.L_08127f70:
	ldr r4, [sp, #12]
	lsls r3, r7, #2
	ldr r3, [r4, r3]
	cmp r3, #0
	ble .L_08127f9c
	ldr r6, [sp, #16]
	ldr r4, [sp, #28]
	adds r1, r3, #0
	lsls r3, r6, #1
	lsls r0, r7, #1
	adds r2, r3, r4
.L_08127f86:
	mov r6, r11
	ldrh r3, [r6, r0]
	subs r1, #1
	adds r3, #8
	strh r3, [r2]
	ldr r3, [sp, #16]
	adds r2, #2
	adds r3, #1
	str r3, [sp, #16]
	cmp r1, #0
	bne .L_08127f86
.L_08127f9c:
	adds r7, #1
.L_08127f9e:
	cmp r7, #4
	bls .L_08127f70
.L_08127fa2:
	ldr r4, [sp, #16]
	ldr r6, [sp, #28]
	ldr r3, .L_08127fc0
	lsls r2, r4, #1
	strh r3, [r2, r6]
	ldr r1, [sp, #24]
	movs r3, #6
	strh r3, [r1, #60]
	ldr r3, [sp, #24]
	movs r2, #0
	strh r2, [r3, #62]
	ldr r5, .L_08127fc4
	movs r7, #128
	b .L_08127fc8
	.2byte 0x0000
.L_08127fc0:
	.4byte 0x00000000
.L_08127fc4:
	.4byte IwramClearWords
.L_08127fc8:
	adds r0, r7, #0
	bl Owner_GetState
	movs r1, #166
	lsls r1, r1, #1
	adds r7, #1
	mov lr, r5
	.2byte 0xf800
	cmp r7, #133
	bls .L_08127fc8
	ldr r4, [sp, #28]
	movs r7, #0
	ldrh r3, [r4]
	cmp r3, #0
	beq .L_08128090
	movs r2, #0
.L_08127fe8:
	ldr r6, [sp, #28]
	movs r1, #1
	adds r5, r2, r6
	ldrh r0, [r5]
	bl Summon_TakeCharge
	movs r3, #128
	adds r4, r0, #0
	lsls r3, r3, #8
	ands r3, r4
	cmp r3, #0
	beq .L_0812800a
	ldrh r0, [r5]
	str r4, [sp, #0]
	bl Summon_ResetCharge
	ldr r4, [sp, #0]
.L_0812800a:
	movs r2, #254
	adds r6, r7, #0
	lsls r2, r2, #7
	adds r6, #128
	adds r2, #255
	ldrh r1, [r5]
	ands r2, r4
	adds r0, r6, #0
	bl BattleUnit_AssignFar
	adds r0, r6, #0
	bl Owner_GetState
	ldr r1, [sp, #32]
	mov r8, r0
	cmp r1, #0
	beq .L_08128034
	adds r0, r6, #0
	bl Func_0812786c
	b .L_08128050
.L_08128034:
	movs r0, #116
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_08128050
	movs r0, #46
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08128050
	adds r0, r6, #0
	bl Func_08127a0c
.L_08128050:
	ldr r1, [sp, #24]
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #104
	movs r3, #0
	adds r5, r1, r2
	strb r3, [r5]
	movs r3, #165
	lsls r3, r3, #1
	add r3, r8
	ldrh r0, [r3]
	cmp r0, #103
	bgt .L_0812807e
	cmp r0, #101
	blt .L_0812807e
	movs r0, #116
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0812807e
	movs r3, #1
	strb r3, [r5]
.L_0812807e:
	adds r7, #1
	cmp r7, #5
	bhi .L_08128090
	ldr r4, [sp, #28]
	lsls r3, r7, #1
	adds r2, r3, #0
	ldrh r3, [r2, r4]
	cmp r3, #0
	bne .L_08127fe8
.L_08128090:
	adds r0, r7, #0
	add sp, #124
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
