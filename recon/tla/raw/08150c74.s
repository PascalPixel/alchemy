.syntax unified
	.thumb
	.global Func_08150c74
	.thumb_func
Func_08150c74:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	mov r8, r0
	ldr r0, [r3, #92]
	sub sp, #112
	str r0, [sp, #68]
	movs r0, #1
	ldr r1, [r3, #96]
	ldr r5, .L_08150ce4
	str r1, [sp, #64]
	movs r6, #0
	ldr r3, [r3, #48]
	movs r7, #0
	str r3, [sp, #48]
	bl Func_081435e0
	ldr r3, .L_08150cdc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_08150ce0
	adds r2, #48
	strh r3, [r2]
	ldr r2, [sp, #68]
	movs r3, #148
	lsls r3, r3, #6
	adds r1, r2, r3
	ldr r0, .L_08150ce8
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #128
	ldr r3, .L_08150cec
	ldr r0, .L_08150ce4
	ldr r1, .L_08150cf0
	lsls r2, r2, #8
	mov lr, r3
	.2byte 0xf800
	movs r4, #7
	mov r12, r4
	mov lr, r5
	b .L_08150cf4
	.2byte 0x0000
.L_08150cdc:
	.4byte 0x00000100
.L_08150ce0:
	.4byte 0x00000000
.L_08150ce4:
	.4byte gMapCellBuffer
.L_08150ce8:
	.4byte 0x0000014e
.L_08150cec:
	.4byte IwramCopyWords
.L_08150cf0:
	.4byte 0x06008000
.L_08150cf4:
	adds r4, r6, #0
	adds r4, #96
	mov r1, r12
	adds r3, r4, #0
	ldr r2, [sp, #68]
	ands r3, r1
	lsls r3, r3, #3
	movs r5, #224
	mov r10, r3
	lsls r5, r5, #3
	adds r3, r7, r2
	movs r0, #0
	adds r1, r3, r5
.L_08150d0e:
	adds r3, r0, #0
	adds r3, #32
	adds r2, r3, #0
	mov r5, r12
	ands r2, r5
	cmp r3, #0
	bge .L_08150d1e
	adds r3, #7
.L_08150d1e:
	asrs r3, r3, #3
	lsls r3, r3, #6
	adds r3, r2, r3
	mov r5, r10
	adds r2, r3, r5
	adds r3, r4, #0
	cmp r3, #0
	bge .L_08150d30
	adds r3, #7
.L_08150d30:
	asrs r3, r3, #3
	lsls r3, r3, #11
	adds r3, r2, r3
	mov r2, lr
	ldrb r3, [r3, r2]
	adds r0, #1
	strb r3, [r1]
	adds r1, #1
	cmp r0, #40
	bne .L_08150d0e
	adds r6, #1
	adds r7, #40
	cmp r6, #16
	bne .L_08150cf4
	mov r4, r8
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_08150d66
	movs r2, #128
	ldr r3, .L_08151030
	lsls r2, r2, #19
	movs r5, #112
	adds r2, #40
	negs r5, r5
	str r3, [r2]
	str r5, [sp, #44]
	b .L_08150d6a
.L_08150d66:
	movs r0, #0
	str r0, [sp, #44]
.L_08150d6a:
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	mov r1, r8
	str r3, [sp, #52]
	movs r7, #0
	ldr r3, [r1, #20]
	cmp r3, #0
	beq .L_08150d98
	ldr r2, [sp, #68]
	movs r1, #0
	adds r2, #24
.L_08150d8a:
	str r1, [r2]
	mov r4, r8
	ldr r3, [r4, #20]
	adds r7, #1
	adds r2, #28
	cmp r7, r3
	bne .L_08150d8a
.L_08150d98:
	ldr r5, [sp, #68]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r5, r0
	movs r3, #1
	adds r1, #132
	str r3, [r2]
	adds r2, r5, r1
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08151034
	bl Func_080145a8
	mov r2, sp
	adds r2, #96
	str r2, [sp, #40]
	ldr r3, .L_08151038
	ldmia r3!, {r0, r4, r5}
	stmia r2!, {r0, r4, r5}
	ldr r3, [r3]
	movs r0, #141
	str r3, [r2]
	ldr r1, [sp, #68]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
	movs r3, #128
	str r3, [r2]
	bl Audio_PlayCue
	movs r4, #0
	str r4, [sp, #60]
	mov r5, r8
	ldr r2, [r5, #24]
	ldr r1, .L_0815103c
	lsls r3, r2, #1
	adds r3, r3, r2
	ldrb r3, [r1, r3]
	cmp r3, #0
	bne .L_08150df4
	b .L_08151010
.L_08150df4:
	ldr r0, [sp, #48]
	adds r0, #12
	str r0, [sp, #28]
.L_08150dfa:
	bl Func_08014de4
	ldr r1, [sp, #28]
	ldr r0, [sp, #48]
	bl Func_080156e8
	mov r1, r8
	ldr r3, [r1, #24]
	ldr r4, .L_0815103c
	lsls r2, r3, #1
	adds r2, r2, r3
	ldrb r3, [r4, r2]
	ldr r5, [sp, #60]
	subs r3, #16
	cmp r5, r3
	bne .L_08150e20
	movs r0, #133
	bl Func_081180e8
.L_08150e20:
	movs r0, #0
	movs r1, #8
	movs r2, #16
	str r0, [sp, #56]
	str r0, [sp, #20]
	str r1, [sp, #24]
	str r2, [sp, #16]
	str r0, [sp, #12]
.L_08150e30:
	ldr r4, [sp, #60]
	movs r3, #31
	ldr r5, [sp, #16]
	ands r3, r4
	adds r3, #32
	cmp r3, r5
	bne .L_08150e48
	ldr r0, [sp, #40]
	ldr r1, [sp, #12]
	ldr r3, [r0, r1]
	adds r3, #32
	str r3, [r0, r1]
.L_08150e48:
	ldr r2, [sp, #60]
	ldr r3, [sp, #16]
	cmp r2, r3
	bge .L_08150e52
	b .L_08150f5a
.L_08150e52:
	mov r4, r8
	ldr r3, [r4, #24]
	ldr r5, .L_0815103c
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r2, #1
	ldrb r3, [r5, r2]
	ldr r0, [sp, #16]
	ldr r1, [sp, #60]
	adds r3, r0, r3
	cmp r1, r3
	bge .L_08150f5a
	subs r0, r1, r0
	lsls r0, r0, #10
	bl Trig_Sin
	ldr r2, [sp, #12]
	ldr r4, [sp, #40]
	ldr r3, [r2, r4]
	muls r3, r0
	asrs r5, r3, #16
	cmp r5, #0
	bge .L_08150e82
	negs r5, r5
.L_08150e82:
	movs r3, #112
	ldr r2, [sp, #68]
	subs r3, r3, r5
	mov r11, r3
	movs r3, #148
	movs r0, #40
	lsls r3, r3, #6
	adds r1, r2, r3
	str r0, [sp, #0]
	str r5, [sp, #4]
	ldr r2, [sp, #24]
	ldr r4, [sp, #52]
	ldr r0, [sp, #64]
	mov r3, r11
	mov lr, r4
	.2byte 0xf800
	movs r2, #16
	str r2, [sp, #4]
	ldr r2, [sp, #68]
	movs r3, #96
	movs r4, #224
	subs r3, r3, r5
	lsls r4, r4, #3
	movs r5, #40
	str r5, [sp, #0]
	adds r1, r2, r4
	ldr r0, [sp, #64]
	ldr r2, [sp, #24]
	ldr r5, [sp, #52]
	mov lr, r5
	.2byte 0xf800
	mov r0, r8
	ldr r3, [r0, #20]
	movs r7, #0
	cmp r3, #0
	beq .L_08150f5a
	ldr r3, [sp, #24]
	ldr r2, [sp, #20]
	movs r1, #72
	adds r3, #40
	add r1, sp
	str r2, [sp, #36]
	str r3, [sp, #32]
	mov r10, r1
	add r6, sp, #84
	mov r9, r10
	movs r4, #36
.L_08150ee0:
	mov r5, r8
	ldrsh r0, [r4, r5]
	str r4, [sp, #8]
	bl GetBattleObjectSlotFar
	ldr r5, [r0]
	mov r1, r9
	ldr r3, [r5, #8]
	adds r0, r6, #0
	str r3, [r6]
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl Func_0815e1ec
	mov r2, r9
	ldr r3, [r2]
	ldr r0, [sp, #44]
	mov r1, r9
	adds r2, r3, r0
	str r2, [r1]
	ldr r3, [sp, #36]
	ldr r4, [sp, #8]
	adds r3, #8
	cmp r2, r3
	blt .L_08150f32
	ldr r3, [sp, #32]
	cmp r2, r3
	bgt .L_08150f32
	mov r0, r10
	ldr r3, [r0, #4]
	cmp r3, r11
	blt .L_08150f32
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r5, #40]
	movs r3, #171
	lsls r3, r3, #8
	adds r3, #133
	str r3, [r5, #72]
.L_08150f32:
	ldr r3, [r5, #12]
	cmp r3, #0
	bge .L_08150f4e
	mov r1, r8
	movs r3, #0
	ldrsh r0, [r4, r1]
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #5
	subs r3, #1
	str r4, [sp, #8]
	bl Func_0814cd48
	ldr r4, [sp, #8]
.L_08150f4e:
	mov r5, r8
	ldr r3, [r5, #20]
	adds r7, #1
	adds r4, #2
	cmp r7, r3
	bne .L_08150ee0
.L_08150f5a:
	ldr r0, [sp, #20]
	ldr r1, [sp, #24]
	ldr r2, [sp, #16]
	ldr r3, [sp, #12]
	ldr r4, [sp, #56]
	adds r0, #40
	adds r1, #40
	adds r2, #4
	adds r3, #4
	adds r4, #1
	str r0, [sp, #20]
	str r1, [sp, #24]
	str r2, [sp, #16]
	str r3, [sp, #12]
	str r4, [sp, #56]
	cmp r4, #3
	beq .L_08150f7e
	b .L_08150e30
.L_08150f7e:
	mov r5, r8
	ldr r3, [r5, #20]
	movs r7, #0
	cmp r3, #0
	beq .L_08150fcc
	ldr r5, [sp, #68]
	movs r6, #36
.L_08150f8c:
	mov r1, r8
	ldrsh r0, [r6, r1]
	bl GetBattleObjectSlotFar
	ldr r3, [r5, #24]
	ldr r0, [r0]
	cmp r3, #0
	bne .L_08150fbe
	ldr r3, [r0, #12]
	cmp r3, #0
	bgt .L_08150fbe
	ldr r3, [r0, #40]
	cmp r3, #0
	bge .L_08150fbe
	movs r3, #1
	str r3, [r5, #24]
	mov r3, r8
	ldrsh r0, [r6, r3]
	movs r3, #5
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	adds r3, r7, #0
	bl Func_0814cd48
.L_08150fbe:
	mov r0, r8
	ldr r3, [r0, #20]
	adds r7, #1
	adds r5, #28
	adds r6, #2
	cmp r7, r3
	bne .L_08150f8c
.L_08150fcc:
	mov r1, r8
	ldr r2, [r1, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, .L_0815103c
	adds r3, #2
	ldrb r1, [r2, r3]
	adds r0, r1, #0
	bl Func_08158ce0
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #68]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r5, [sp, #60]
	mov r0, r8
	adds r5, #1
	str r5, [sp, #60]
	ldr r1, .L_0815103c
	ldr r2, [r0, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldrb r3, [r1, r3]
	cmp r5, r3
	beq .L_08151010
	b .L_08150dfa
.L_08151010:
	ldr r0, .L_08151034
	bl Func_08014644
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #112
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08151030:
	.4byte 0xffff9000
.L_08151034:
	.4byte Func_08143000
.L_08151038:
	.4byte Data_08196e1c
.L_0815103c:
	.4byte Data_081982f7
