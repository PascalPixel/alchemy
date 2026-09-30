.syntax unified
	.thumb
	.global Func_0814627c
	.thumb_func
Func_0814627c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #132
	str r0, [sp, #80]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	movs r0, #0
	str r1, [sp, #76]
	ldr r2, [r3, #96]
	str r2, [sp, #72]
	ldr r5, [r3, #100]
	ldr r3, [r3, #48]
	str r3, [sp, #52]
	bl Func_081435e0
	ldr r3, .L_081462e4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, [sp, #76]
	movs r4, #224
	lsls r4, r4, #3
	adds r1, r3, r4
	ldr r0, .L_081462e8
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	adds r1, r5, #0
	ldr r0, .L_081462ec
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r5, [sp, #76]
	movs r2, #184
	lsls r2, r2, #5
	adds r1, r5, r2
	ldr r0, .L_081462f0
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r3, #239
	b .L_081462f4
.L_081462e4:
	.4byte 0x00000100
.L_081462e8:
	.4byte 0x0000013a
.L_081462ec:
	.4byte 0x00000134
.L_081462f0:
	.4byte 0x00000137
.L_081462f4:
	lsls r3, r3, #7
	adds r2, r5, r3
	movs r4, #238
	movs r3, #3
	str r3, [r2]
	lsls r4, r4, #7
	ldr r3, .L_08146620
	adds r4, #132
	adds r2, r5, r4
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08146624
	bl Func_080145a8
	ldr r1, [sp, #80]
	mov r2, sp
	adds r2, #120
	movs r5, #36
	ldrsh r0, [r1, r5]
	adds r1, r2, #0
	str r2, [sp, #44]
	bl Func_0815e20c
	ldr r3, [sp, #44]
	movs r0, #142
	ldr r2, [r3]
	movs r3, #64
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #19
	str r3, [sp, #48]
	adds r2, #40
	lsls r3, r3, #8
	str r3, [r2]
	bl Audio_PlayCue
	movs r4, #0
	str r4, [sp, #68]
	ldr r5, [sp, #80]
	movs r1, #72
	ldr r2, [r5, #20]
	negs r1, r1
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	cmp r3, r1
	bne .L_08146356
	b .L_08146608
.L_08146356:
	ldr r2, [sp, #68]
	cmp r2, #64
	bne .L_08146362
	movs r0, #0
	bl Func_081180e8
.L_08146362:
	ldr r3, [sp, #68]
	cmp r3, #46
	bne .L_08146378
	ldr r4, [sp, #80]
	movs r2, #16
	ldr r0, [r4, #8]
	movs r5, #36
	ldrsh r1, [r4, r5]
	movs r3, #0
	bl Func_08118078
.L_08146378:
	movs r1, #170
	movs r2, #170
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [sp, #68]
	adds r2, #85
	movs r3, #0
	adds r1, #171
	bl Func_081496c8
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r1, #23
	movs r0, #188
	str r3, [sp, #60]
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	ldr r1, [sp, #68]
	str r3, [sp, #64]
	cmp r1, #16
	ble .L_081463ce
	movs r3, #15
	ands r3, r1
	cmp r3, #0
	bne .L_081463ce
	ldr r3, [sp, #76]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #132
	adds r2, r3, r4
	ldr r3, [r2]
	ldr r5, .L_08146628
	adds r3, r3, r5
	str r3, [r2]
.L_081463ce:
	ldr r2, [sp, #76]
	ldr r4, [sp, #68]
	movs r3, #224
	lsls r3, r3, #3
	adds r3, r2, r3
	str r3, [sp, #28]
	lsls r3, r4, #1
	adds r3, r3, r4
	movs r1, #0
	lsls r3, r3, #9
	movs r5, #36
	str r1, [sp, #56]
	str r1, [sp, #20]
	str r3, [sp, #16]
	str r5, [sp, #12]
	mov r9, r4
.L_081463ee:
	ldr r1, [sp, #12]
	ldr r3, [sp, #80]
	ldrsh r0, [r1, r3]
	bl GetBattleObjectSlotFar
	mov r4, r9
	ldr r6, [r0]
	cmp r4, #95
	bls .L_08146402
	b .L_081465aa
.L_08146402:
	bl Func_08014de4
	ldr r0, [sp, #52]
	adds r1, r0, #0
	adds r1, #12
	bl Func_080156e8
	ldr r3, [r6, #8]
	add r5, sp, #108
	str r3, [r5]
	movs r1, #96
	ldr r3, [r6, #12]
	add r1, sp
	str r3, [r5, #4]
	adds r0, r5, #0
	ldr r3, [r6, #16]
	mov r10, r1
	str r3, [r5, #8]
	bl Func_0815e1ec
	ldr r2, [sp, #44]
	ldr r4, [sp, #48]
	ldr r3, [r2]
	mov r1, r10
	adds r3, r3, r4
	str r3, [r1]
	ldr r3, [r1, #4]
	mov r2, r9
	subs r3, #24
	str r3, [r1, #4]
	cmp r2, #67
	ble .L_08146444
	b .L_08146570
.L_08146444:
	ldr r5, [sp, #20]
	ldr r1, [sp, #76]
	lsls r2, r5, #3
	subs r2, r2, r5
	lsls r2, r2, #2
	adds r5, r2, r1
	ldr r2, [sp, #16]
	movs r3, #168
	lsls r3, r3, #10
	movs r4, #0
	subs r2, r3, r2
	str r4, [sp, #24]
	movs r3, #64
	mov r4, r9
	subs r3, r3, r4
	lsls r3, r3, #9
	movs r7, #0
	add r6, sp, #84
	mov r8, r2
	mov r11, r3
.L_0814646c:
	bl Func_08014de4
	mov r1, r9
	cmp r1, #63
	bgt .L_08146490
	mov r2, r8
	str r2, [r6]
	str r2, [r6, #4]
	str r2, [r6, #8]
	adds r0, r6, #0
	bl Func_080151ac
	mov r0, r11
	bl Func_080150e4
	mov r0, r11
	bl Func_08015068
.L_08146490:
	ldr r0, [sp, #24]
	bl Func_080150e4
	add r3, sp, #108
	adds r1, r3, #0
	ldr r0, .L_0814662c
	bl Func_0815e1ec
	mov r4, r10
	ldr r3, [r4]
	ldr r2, [sp, #108]
	adds r7, #1
	adds r2, r2, r3
	str r2, [r5, #12]
	ldr r3, [sp, #112]
	ldr r2, [r4, #4]
	adds r3, r3, r2
	adds r3, #16
	str r3, [r5, #16]
	ldr r1, [sp, #24]
	movs r2, #170
	lsls r2, r2, #7
	adds r2, #85
	adds r1, r1, r2
	str r1, [sp, #24]
	adds r5, #28
	cmp r7, #3
	bne .L_0814646c
	ldr r3, [sp, #20]
	movs r7, #0
	str r3, [sp, #36]
.L_081464ce:
	ldr r4, [sp, #36]
	ldr r5, [sp, #76]
	adds r2, r7, r4
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r7, #1
	lsls r3, r3, #2
	adds r3, r5, r3
	movs r1, #3
	adds r0, r7, #0
	str r3, [sp, #40]
	str r7, [sp, #32]
	bl __modsi3
	ldr r1, [sp, #36]
	mov r2, r9
	adds r0, r0, r1
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #2
	adds r5, r5, r3
	mov r11, r5
	cmp r2, #0
	bge .L_08146500
	adds r2, #15
.L_08146500:
	asrs r2, r2, #4
	movs r3, #5
	subs r4, r3, r2
	movs r2, #0
	mov r8, r2
	lsls r7, r4, #1
.L_0814650c:
	ldr r1, [sp, #40]
	mov r5, r11
	ldr r6, [r1, #12]
	ldr r3, [r5, #12]
	movs r1, #24
	subs r3, r3, r6
	mov r0, r8
	muls r0, r3
	str r4, [sp, #8]
	bl Math_Div
	ldr r2, [sp, #40]
	ldr r3, [r5, #16]
	ldr r5, [r2, #16]
	adds r6, r6, r0
	subs r3, r3, r5
	mov r0, r8
	muls r0, r3
	movs r1, #24
	bl Math_Div
	ldr r2, .L_08146630
	subs r3, r7, #2
	ldrh r1, [r2, r3]
	ldr r4, [sp, #8]
	ldr r3, [sp, #76]
	adds r5, r5, r0
	movs r2, #184
	subs r5, r5, r4
	subs r6, r6, r4
	adds r1, r3, r1
	lsls r2, r2, #5
	adds r1, r1, r2
	adds r3, r5, #0
	adds r2, r6, #0
	str r7, [sp, #0]
	str r7, [sp, #4]
	ldr r0, [sp, #72]
	ldr r5, [sp, #60]
	mov lr, r5
	.2byte 0xf800
	movs r1, #1
	add r8, r1
	mov r2, r8
	ldr r4, [sp, #8]
	cmp r2, #24
	bne .L_0814650c
	ldr r7, [sp, #32]
	cmp r7, #3
	bne .L_081464ce
.L_08146570:
	mov r3, r9
	cmp r3, #63
	ble .L_081465aa
	mov r4, r10
	ldr r2, [r4]
	ldr r3, [r4, #4]
	movs r5, #24
	str r5, [sp, #0]
	movs r5, #48
	subs r2, #24
	subs r3, #24
	str r5, [sp, #4]
	ldr r1, [sp, #28]
	ldr r4, [sp, #60]
	ldr r0, [sp, #72]
	mov lr, r4
	.2byte 0xf800
	mov r1, r10
	ldr r3, [r1, #4]
	movs r4, #24
	ldr r2, [r1]
	subs r3, #24
	str r5, [sp, #4]
	str r4, [sp, #0]
	ldr r0, [sp, #72]
	ldr r1, [sp, #28]
	ldr r5, [sp, #64]
	mov lr, r5
	.2byte 0xf800
.L_081465aa:
	ldr r1, [sp, #20]
	ldr r2, [sp, #16]
	adds r1, #32
	ldr r5, [sp, #12]
	str r1, [sp, #20]
	ldr r3, .L_08146634
	ldr r1, [sp, #56]
	movs r4, #8
	adds r2, r2, r3
	negs r4, r4
	adds r5, #2
	adds r1, #1
	str r2, [sp, #16]
	add r9, r4
	str r5, [sp, #12]
	str r1, [sp, #56]
	cmp r1, #1
	beq .L_081465d0
	b .L_081463ee
.L_081465d0:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r4, #240
	ldr r2, [sp, #76]
	ldr r5, [sp, #56]
	lsls r4, r4, #7
	adds r4, #232
	adds r3, r2, r4
	str r5, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #68]
	ldr r3, [sp, #80]
	adds r1, #1
	str r1, [sp, #68]
	ldr r2, [r3, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, #72
	cmp r1, r3
	beq .L_08146608
	b .L_08146356
.L_08146608:
	ldr r0, .L_08146624
	bl Func_08014644
	bl Func_08143bb8
	add sp, #132
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08146620:
	.4byte 0x04040404
.L_08146624:
	.4byte Func_08143000
.L_08146628:
	.4byte 0x01010101
.L_0814662c:
	.4byte Data_08197918
.L_08146630:
	.4byte Data_08197424
.L_08146634:
	.4byte 0xffffd000
