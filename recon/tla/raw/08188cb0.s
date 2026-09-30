.syntax unified
	.thumb
	.global Func_08188cb0
	.thumb_func
Func_08188cb0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	ldr r3, [r3, #96]
	sub sp, #40
	mov r10, r0
	movs r0, #1
	str r3, [sp, #16]
	mov r11, r1
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_08188d08
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	mov r2, r10
	ldr r3, [r2, #24]
	cmp r3, #0
	bne .L_08188cf0
	movs r2, #128
	ldr r3, .L_08188d0c
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
.L_08188cf0:
	movs r5, #224
	lsls r5, r5, #3
	add r5, r11
	movs r3, #1
	ldr r0, .L_08188d10
	adds r1, r5, #0
	movs r2, #1
	bl Func_08157cf4
	mov r4, r10
	ldr r3, [r4, #24]
	b .L_08188d14
.L_08188d08:
	.4byte 0x00001010
.L_08188d0c:
	.4byte 0x00000100
.L_08188d10:
	.4byte 0x00000123
.L_08188d14:
	cmp r3, #0
	bne .L_08188d34
	movs r1, #139
	lsls r1, r1, #7
	movs r2, #1
	movs r3, #0
	ldr r0, .L_0818908c
	add r1, r11
	bl Func_08157cf4
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #50
	b .L_08188d90
.L_08188d34:
	movs r6, #0
	movs r2, #250
	mov r8, r6
	movs r0, #63
	lsls r2, r2, #6
	adds r1, r5, #0
.L_08188d40:
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_08188d58
	lsls r3, r3, #1
	adds r3, #32
	movs r4, #252
	strb r3, [r1]
	lsls r4, r4, #22
	lsls r3, r3, #24
	cmp r3, r4
	bls .L_08188d58
	strb r0, [r1]
.L_08188d58:
	movs r6, #1
	add r8, r6
	adds r1, #1
	cmp r8, r2
	bne .L_08188d40
	movs r1, #139
	lsls r1, r1, #7
	add r1, r11
	movs r2, #1
	movs r3, #1
	ldr r0, .L_08189090
	bl Func_08157cf4
	ldr r0, .L_08189094
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08189098
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #75
.L_08188d90:
	str r3, [r2]
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0818909c
	bl Scheduler_AddOrUpdateCallback
	mov r1, r10
	ldr r0, [r1, #4]
	add r1, sp, #20
	bl Func_08144aac
	movs r2, #0
	mov r3, r11
	mov r8, r2
	adds r3, #24
.L_08188db8:
	movs r4, #1
	add r8, r4
	mov r6, r8
	str r2, [r3]
	adds r3, #28
	cmp r6, #64
	bne .L_08188db8
	mov r1, r10
	movs r3, #224
	ldr r0, [r1, #8]
	lsls r3, r3, #11
	movs r2, #36
	ldrsh r1, [r1, r2]
	movs r2, #16
	bl BattleMotion_ApproachTargetFar
	movs r6, #8
	movs r3, #0
	negs r6, r6
	movs r4, #28
	str r3, [sp, #12]
	str r6, [sp, #8]
	add r4, sp
	mov r9, r4
.L_08188de8:
	mov r1, r10
	ldr r3, [r1, #24]
	cmp r3, #0
	bne .L_08188e94
	ldr r0, [r1, #8]
	mov r1, r9
	bl Func_0815e21c
	mov r2, r10
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_08188e0a
	mov r3, r9
	ldr r2, [r3]
	movs r1, #128
	movs r3, #80
	b .L_08188e12
.L_08188e0a:
	mov r4, r9
	ldr r2, [r4]
	movs r1, #128
	movs r3, #32
.L_08188e12:
	lsls r1, r1, #19
	subs r3, r3, r2
	adds r1, #40
	lsls r3, r3, #8
	str r3, [r1]
	ldr r6, [sp, #8]
	cmp r6, #15
	bls .L_08188e24
	b .L_08188f3e
.L_08188e24:
	lsrs r3, r6, #31
	adds r3, r6, r3
	asrs r5, r3, #1
	cmp r5, #6
	ble .L_08188e30
	movs r5, #6
.L_08188e30:
	mov r1, r10
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_08188e60
	ldr r2, .L_081890a0
	lsls r3, r5, #1
	ldrh r1, [r2, r3]
	movs r3, #224
	lsls r3, r3, #3
	add r1, r11
	adds r1, r1, r3
	ldr r3, .L_081890a4
	mov r4, r9
	ldrsb r2, [r3, r5]
	ldr r3, .L_081890a8
	ldr r0, [r4, #4]
	ldrsb r3, [r3, r5]
	adds r2, #30
	adds r3, r3, r0
	ldr r0, .L_081890ac
	subs r3, #60
	ldrb r0, [r0, r5]
	ldr r4, [sp, #20]
	b .L_08188eec
.L_08188e60:
	ldr r6, .L_081890a0
	lsls r3, r5, #1
	ldrh r1, [r6, r3]
	ldr r3, .L_081890a4
	movs r2, #224
	lsls r2, r2, #3
	add r1, r11
	adds r1, r1, r2
	ldrsb r2, [r3, r5]
	ldr r3, .L_081890ac
	mov r6, r9
	ldrb r4, [r3, r5]
	ldr r3, .L_081890a8
	ldr r0, [r6, #4]
	ldrsb r3, [r3, r5]
	str r4, [sp, #0]
	adds r3, r3, r0
	ldr r0, .L_081890b0
	negs r2, r2
	ldrb r0, [r0, r5]
	subs r2, r2, r4
	str r0, [sp, #4]
	adds r2, #98
	subs r3, #60
	ldr r4, [sp, #20]
	b .L_08188ef4
.L_08188e94:
	mov r2, r10
	movs r1, #36
	ldrsh r0, [r2, r1]
	mov r1, r9
	bl Func_0815e21c
	ldr r3, [sp, #8]
	cmp r3, #15
	bhi .L_08188f3e
	ldr r4, [sp, #8]
	lsrs r3, r3, #31
	adds r3, r4, r3
	asrs r5, r3, #1
	cmp r5, #6
	ble .L_08188eb4
	movs r5, #6
.L_08188eb4:
	mov r6, r10
	ldr r3, [r6, #4]
	cmp r3, #0
	bne .L_08188efc
	ldr r2, .L_081890a0
	lsls r3, r5, #1
	ldrh r1, [r2, r3]
	mov r4, r9
	ldr r2, [r4]
	movs r3, #224
	lsls r3, r3, #3
	add r1, r11
	adds r1, r1, r3
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, .L_081890a4
	asrs r2, r2, #1
	ldrsb r3, [r3, r5]
	ldr r0, [r4, #4]
	adds r2, r2, r3
	ldr r3, .L_081890a8
	subs r2, #16
	ldrsb r3, [r3, r5]
	ldr r4, [sp, #20]
	adds r3, r3, r0
	ldr r0, .L_081890ac
	subs r3, #92
	ldrb r0, [r0, r5]
.L_08188eec:
	str r0, [sp, #0]
	ldr r0, .L_081890b0
	ldrb r0, [r0, r5]
	str r0, [sp, #4]
.L_08188ef4:
	ldr r0, [sp, #16]
	mov lr, r4
	.2byte 0xf800
	b .L_08188f3e
.L_08188efc:
	ldr r6, .L_081890a0
	lsls r3, r5, #1
	ldrh r1, [r6, r3]
	movs r2, #224
	lsls r2, r2, #3
	mov r3, r9
	add r1, r11
	adds r1, r1, r2
	ldr r2, [r3]
	mov r6, r9
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, .L_081890a4
	asrs r2, r2, #1
	ldrsb r3, [r3, r5]
	ldr r0, [r6, #4]
	subs r2, r2, r3
	ldr r3, .L_081890ac
	ldrb r4, [r3, r5]
	ldr r3, .L_081890a8
	subs r2, r2, r4
	ldrsb r3, [r3, r5]
	str r4, [sp, #0]
	adds r3, r3, r0
	ldr r0, .L_081890b0
	adds r2, #16
	ldrb r0, [r0, r5]
	subs r3, #92
	str r0, [sp, #4]
	ldr r4, [sp, #20]
	ldr r0, [sp, #16]
	mov lr, r4
	.2byte 0xf800
.L_08188f3e:
	ldr r1, [sp, #12]
	cmp r1, #18
	bne .L_0818900c
	mov r3, r10
	movs r2, #36
	ldrsh r0, [r3, r2]
	mov r1, r9
	bl Func_0815e21c
	movs r0, #134
	bl Func_081180e8
	mov r6, r10
	movs r3, #8
	movs r4, #36
	ldrsh r0, [r6, r4]
	movs r1, #7
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	movs r1, #36
	ldrsh r0, [r6, r1]
	movs r1, #6
	bl Func_08118088
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	movs r3, #4
	str r3, [r2]
	movs r4, #16
	ldr r3, [r6, #24]
	movs r2, #0
	lsls r3, r3, #4
	negs r4, r4
	mov r8, r2
	cmp r3, r4
	beq .L_0818900c
	mov r5, r11
.L_08188f92:
	bl Random16
	movs r6, #128
	movs r3, #255
	ands r3, r0
	lsls r6, r6, #1
	adds r7, r3, r6
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r6, r0, #0
	mov r1, r10
	ands r6, r3
	ldr r3, [r1, #24]
	cmp r3, #0
	bne .L_08188fbc
	movs r3, #128
	lsls r3, r3, #15
	b .L_08188fc8
.L_08188fbc:
	mov r2, r9
	ldr r3, [r2]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
.L_08188fc8:
	str r3, [r5]
	movs r3, #160
	lsls r3, r3, #15
	str r3, [r5, #4]
	adds r0, r6, #0
	bl Trig_Sin
	adds r3, r7, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r5, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r5, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #16
	str r3, [r5, #24]
	mov r4, r10
	movs r3, #1
	add r8, r3
	ldr r3, [r4, #24]
	adds r5, #28
	lsls r3, r3, #4
	adds r3, #16
	cmp r8, r3
	bne .L_08188f92
.L_0818900c:
	movs r6, #0
	mov r8, r6
	mov r5, r11
.L_08189012:
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_081890e4
	subs r3, #1
	str r3, [r5, #24]
	movs r1, #60
	adds r0, r5, #0
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #4]
	movs r1, #208
	lsls r1, r1, #15
	cmp r3, r1
	ble .L_0818903e
	ldr r3, [r5, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
	b .L_081890e4
.L_0818903e:
	ldr r2, [r5]
	ldr r4, .L_081890b4
	cmp r2, r4
	bhi .L_081890e4
	cmp r3, #0
	blt .L_081890e4
	mov r1, r10
	asrs r7, r3, #16
	ldr r3, [r1, #24]
	asrs r6, r2, #16
	cmp r3, #0
	bne .L_081890b8
	ldr r0, [sp, #12]
	add r0, r8
	cmp r0, #0
	bge .L_08189060
	adds r0, #3
.L_08189060:
	movs r1, #6
	asrs r0, r0, #2
	bl Math_Mod
	adds r1, r0, #0
	lsls r1, r1, #8
	movs r2, #139
	lsls r2, r2, #7
	add r1, r11
	movs r0, #16
	adds r1, r1, r2
	adds r3, r7, #0
	adds r2, r6, #0
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, #8
	subs r3, #8
	ldr r4, [sp, #20]
	ldr r0, [sp, #16]
	mov lr, r4
	.2byte 0xf800
	b .L_081890e4
.L_0818908c:
	.4byte 0x0000012e
.L_08189090:
	.4byte 0x0000016d
.L_08189094:
	.4byte 0x00000130
.L_08189098:
	.4byte IwramCopyWords
.L_0818909c:
	.4byte Func_08143000
.L_081890a0:
	.4byte Data_08199d40
.L_081890a4:
	.4byte Data_08199d4e
.L_081890a8:
	.4byte Data_08199d55
.L_081890ac:
	.4byte Data_08199d31
.L_081890b0:
	.4byte Data_08199d38
.L_081890b4:
	.4byte 0x007effff
.L_081890b8:
	movs r1, #3
	mov r0, r8
	bl Math_Mod
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #6
	movs r3, #139
	lsls r3, r3, #7
	add r1, r11
	movs r0, #24
	adds r1, r1, r3
	adds r2, r6, #0
	adds r3, r7, #0
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, #12
	subs r3, #12
	ldr r4, [sp, #20]
	ldr r0, [sp, #16]
	mov lr, r4
	.2byte 0xf800
.L_081890e4:
	movs r4, #1
	add r8, r4
	mov r6, r8
	adds r5, #28
	cmp r6, #64
	bne .L_08189012
	movs r1, #8
	movs r0, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #8]
	ldr r2, [sp, #12]
	adds r1, #1
	adds r2, #1
	str r1, [sp, #8]
	str r2, [sp, #12]
	cmp r2, #70
	beq .L_08189120
	b .L_08188de8
.L_08189120:
	ldr r0, .L_08189144
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08189144:
	.4byte Func_08143000
