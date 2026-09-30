.syntax unified
	.thumb
	.global Func_0818eda4
	.thumb_func
Func_0818eda4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #92
	str r0, [sp, #48]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	str r0, [sp, #44]
	movs r0, #0
	ldr r1, [r3, #92]
	str r1, [sp, #40]
	ldr r3, [r3, #100]
	str r3, [sp, #32]
	bl Func_081435e0
	ldr r3, .L_0818ee0c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	mov r2, sp
	adds r2, #60
	adds r1, r2, #0
	movs r0, #0
	str r2, [sp, #28]
	bl Func_08144aac
	ldr r0, .L_0818ee10
	ldr r1, .L_0818ee14
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r3, [sp, #40]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r3, r2
	ldr r0, .L_0818ee18
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r3, [sp, #40]
	movs r2, #184
	lsls r2, r2, #5
	b .L_0818ee1c
	.2byte 0x0000
.L_0818ee0c:
	.4byte 0x00001010
.L_0818ee10:
	.4byte 0x0000013e
.L_0818ee14:
	.4byte Data_02014000
.L_0818ee18:
	.4byte 0x000000cb
.L_0818ee1c:
	adds r1, r3, r2
	ldr r0, .L_0818f098
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0818f09c
	ldr r1, .L_0818f0a0
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r2, #0
	ldr r1, [sp, #32]
	movs r3, #0
	ldr r0, .L_0818f0a4
	bl Func_08157cf4
	ldr r0, .L_0818f0a8
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0818f0ac
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r3, [sp, #40]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r3, r0
	movs r3, #2
	str r3, [r2]
	ldr r1, [sp, #40]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0818f0b0
	bl Func_080145a8
	ldr r1, [sp, #48]
	mov r2, sp
	adds r2, #80
	ldr r0, [r1, #8]
	adds r1, r2, #0
	str r2, [sp, #24]
	bl Func_0815e21c
	ldr r1, [sp, #48]
	mov r2, sp
	adds r2, #68
	movs r3, #36
	ldrsh r0, [r1, r3]
	adds r1, r2, #0
	str r2, [sp, #20]
	bl Func_0815e21c
	mov r0, sp
	movs r3, #0
	adds r0, #52
	str r3, [sp, #36]
	str r0, [sp, #8]
.L_0818eea4:
	ldr r1, [sp, #36]
	cmp r1, #0
	bne .L_0818ef28
	movs r2, #0
	ldr r3, .L_0818f0b4
	mov r8, r2
	movs r1, #1
	movs r2, #128
	negs r1, r1
	lsls r2, r2, #2
.L_0818eeb8:
	movs r0, #1
	add r8, r0
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_0818eeb8
	ldr r2, [sp, #40]
	movs r3, #224
	ldr r6, [sp, #20]
	movs r1, #0
	lsls r3, r3, #2
	mov r8, r1
	adds r5, r2, r3
.L_0818eed2:
	bl Random16
	ldr r3, [r6]
	movs r2, #127
	ands r2, r0
	adds r3, r3, r2
	subs r3, #64
	lsls r3, r3, #16
	str r3, [r5]
	ldr r3, [r6, #4]
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r3, #248
	lsls r3, r3, #5
	adds r3, #255
	ands r3, r0
	movs r1, #1
	movs r0, #224
	lsls r0, r0, #7
	add r8, r1
	adds r3, r3, r0
	mov r2, r8
	str r3, [r5, #8]
	adds r5, #28
	cmp r2, #16
	bne .L_0818eed2
	ldr r3, [sp, #36]
	cmp r3, #0
	bne .L_0818ef28
	ldr r0, [sp, #40]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	movs r3, #50
	str r3, [r2]
.L_0818ef28:
	ldr r0, [sp, #36]
	cmp r0, #12
	bne .L_0818ef46
	ldr r1, [sp, #40]
	movs r3, #239
	lsls r3, r3, #7
	adds r2, r1, r3
	movs r0, #238
	movs r3, #3
	str r3, [r2]
	lsls r0, r0, #7
	ldr r3, .L_0818f0b8
	adds r0, #132
	adds r2, r1, r0
	str r3, [r2]
.L_0818ef46:
	ldr r1, [sp, #36]
	cmp r1, #54
	bne .L_0818ef66
	ldr r3, [sp, #40]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r3, r0
	movs r3, #2
	str r3, [r2]
	ldr r1, [sp, #40]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	movs r3, #50
	str r3, [r2]
.L_0818ef66:
	ldr r0, [sp, #36]
	cmp r0, #0
	bne .L_0818ef72
	movs r0, #139
	bl Audio_PlayCue
.L_0818ef72:
	ldr r1, [sp, #36]
	cmp r1, #16
	bne .L_0818ef7e
	movs r0, #134
	bl Audio_PlayCue
.L_0818ef7e:
	ldr r2, [sp, #36]
	cmp r2, #15
	bgt .L_0818ef86
	b .L_0818f0c0
.L_0818ef86:
	cmp r2, #16
	bne .L_0818f00a
	ldr r0, [sp, #24]
	ldr r7, [sp, #40]
	movs r3, #0
	mov r8, r3
	mov r10, r0
.L_0818ef94:
	bl Random16
	movs r1, #190
	lsls r1, r1, #7
	adds r1, #255
	bl Math_ModU
	movs r1, #160
	lsls r1, r1, #7
	adds r6, r0, #0
	adds r6, r6, r1
	bl Random16
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	movs r2, #128
	lsls r2, r2, #2
	ands r5, r0
	adds r0, r6, #0
	adds r5, r5, r2
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #8
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #16]
	mov r0, r10
	ldr r3, [r0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	bl Random16
	mov r1, r10
	ldr r3, [r1, #4]
	movs r2, #15
	ands r2, r0
	subs r3, r3, r2
	subs r3, #8
	lsls r3, r3, #16
	movs r2, #1
	str r3, [r7, #4]
	add r8, r2
	movs r3, #0
	str r3, [r7, #24]
	mov r3, r8
	adds r7, #28
	cmp r3, #32
	bne .L_0818ef94
.L_0818f00a:
	ldr r1, [sp, #40]
	movs r0, #0
	mov r8, r0
	mov r10, r1
.L_0818f012:
	mov r6, r10
	ldr r3, [r6, #24]
	cmp r3, #0
	blt .L_0818f086
	mov r3, r8
	cmp r3, #0
	bge .L_0818f022
	adds r3, #3
.L_0818f022:
	ldr r2, [sp, #36]
	asrs r3, r3, #2
	cmp r2, r3
	blt .L_0818f086
	mov r3, r8
	movs r0, #1
	ands r3, r0
	adds r5, r3, #5
	lsls r3, r3, #2
	str r3, [sp, #16]
	movs r1, #0
	lsrs r2, r5, #1
	mov r9, r1
	lsls r7, r5, #1
	mov r11, r2
.L_0818f040:
	ldr r0, .L_0818f0bc
	subs r3, r7, #2
	ldrh r1, [r0, r3]
	ldr r2, [sp, #32]
	mov r0, r11
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r6, r3]
	subs r2, r2, r0
	movs r0, #6
	ldrsh r3, [r6, r0]
	ldr r0, [sp, #28]
	subs r3, r3, r5
	mov r12, r3
	str r5, [sp, #0]
	ldr r3, [sp, #16]
	str r7, [sp, #4]
	ldr r4, [r3, r0]
	ldr r0, [sp, #44]
	mov r3, r12
	mov lr, r4
	.2byte 0xf800
	movs r1, #64
	movs r2, #0
	adds r0, r6, #0
	bl BattleFxKernels_IntegrateVector2
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #2
	bne .L_0818f040
	ldr r3, [r6, #24]
	adds r3, #1
	str r3, [r6, #24]
.L_0818f086:
	movs r0, #1
	add r8, r0
	movs r3, #28
	mov r1, r8
	add r10, r3
	cmp r1, #32
	bne .L_0818f012
	b .L_0818f1c4
	.2byte 0x0000
.L_0818f098:
	.4byte 0x000000c9
.L_0818f09c:
	.4byte 0x000000c2
.L_0818f0a0:
	.4byte Data_02014000
.L_0818f0a4:
	.4byte 0x00000134
.L_0818f0a8:
	.4byte 0x00000148
.L_0818f0ac:
	.4byte IwramCopyWords
.L_0818f0b0:
	.4byte Func_08143000
.L_0818f0b4:
	.4byte Data_02010018
.L_0818f0b8:
	.4byte 0x04040404
.L_0818f0bc:
	.4byte Data_08197410
.L_0818f0c0:
	ldr r2, [sp, #36]
	cmp r2, #0
	bne .L_0818f150
	ldr r0, [sp, #24]
	ldr r7, [sp, #40]
	movs r3, #0
	mov r8, r3
	mov r10, r0
.L_0818f0d0:
	bl Random16
	movs r6, #254
	ldr r1, .L_0818f44c
	lsls r6, r6, #7
	adds r6, #255
	ands r6, r0
	adds r6, r6, r1
	bl Random16
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	movs r2, #128
	lsls r2, r2, #2
	ands r5, r0
	adds r0, r6, #0
	adds r5, r5, r2
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #10
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #9
	str r3, [r7, #16]
	mov r0, r10
	ldr r3, [r0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	bl Random16
	mov r2, r10
	ldr r1, [r2, #4]
	movs r3, #15
	ands r3, r0
	ldr r2, [r7, #12]
	subs r1, r1, r3
	ldr r3, [r7]
	lsls r2, r2, #6
	subs r3, r3, r2
	str r3, [r7]
	ldr r3, [r7, #16]
	subs r1, #8
	lsls r3, r3, #6
	lsls r1, r1, #16
	subs r1, r1, r3
	movs r3, #0
	str r3, [r7, #24]
	movs r3, #1
	add r8, r3
	mov r0, r8
	str r1, [r7, #4]
	adds r7, #28
	cmp r0, #32
	bne .L_0818f0d0
.L_0818f150:
	ldr r5, [sp, #40]
	movs r1, #0
	mov r8, r1
.L_0818f156:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_0818f1b8
	mov r3, r8
	movs r2, #1
	ands r3, r2
	adds r6, r3, #2
	movs r0, #0
	lsls r3, r3, #2
	lsrs r1, r6, #1
	mov r10, r0
	mov r11, r3
	lsls r7, r6, #1
	mov r9, r1
.L_0818f172:
	ldr r2, .L_0818f450
	subs r3, r7, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #32]
	movs r0, #2
	ldrsh r2, [r5, r0]
	adds r1, r3, r1
	mov r3, r9
	subs r2, r2, r3
	movs r0, #6
	ldrsh r3, [r5, r0]
	ldr r0, [sp, #28]
	str r6, [sp, #0]
	str r7, [sp, #4]
	subs r3, r3, r6
	mov r12, r3
	mov r3, r11
	ldr r4, [r3, r0]
	ldr r0, [sp, #44]
	mov r3, r12
	mov lr, r4
	.2byte 0xf800
	movs r1, #64
	movs r2, #0
	adds r0, r5, #0
	bl BattleFxKernels_IntegrateVector2
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #4
	bne .L_0818f172
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_0818f1b8:
	movs r3, #1
	add r8, r3
	mov r0, r8
	adds r5, #28
	cmp r0, #32
	bne .L_0818f156
.L_0818f1c4:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #12]
	movs r0, #1
	bl Func_081969f8
	movs r3, #6
	adds r7, r0, #0
	str r3, [r7]
	ldr r2, [sp, #8]
	mov r0, sp
	str r2, [r7, #16]
	ldr r1, [sp, #12]
	adds r0, #52
	strb r3, [r0]
	str r1, [r7, #12]
	strb r3, [r2, #1]
	ldr r1, [sp, #40]
	mov r9, r0
	movs r0, #224
	lsls r0, r0, #3
	adds r3, r1, r0
	str r3, [r2, #4]
	ldr r3, .L_0818f454
	movs r2, #128
	str r3, [r7, #8]
	ldr r6, [sp, #24]
	movs r1, #0
	lsls r2, r2, #8
	mov r8, r1
	mov r10, r2
.L_0818f204:
	ldr r3, .L_0818f458
	mov r0, r8
	ldrb r2, [r3, r0]
	ldr r1, [sp, #36]
	cmp r1, r2
	blt .L_0818f286
	adds r3, r2, #0
	adds r3, #16
	cmp r1, r3
	bge .L_0818f286
	movs r3, #0
	str r3, [r7, #20]
	subs r5, r1, r2
	bl Func_08014de4
	movs r1, #128
	mov r0, r10
	lsls r1, r1, #9
	mov r2, r10
	bl Func_080151e4
	ldr r0, [r6]
	ldr r1, [r6, #4]
	subs r0, #128
	subs r1, #74
	lsls r1, r1, #16
	lsls r0, r0, #16
	movs r2, #0
	bl Func_08015160
	ldr r3, .L_0818f45c
	mov r2, r8
	lsls r0, r2, #12
	adds r0, r0, r3
	lsls r5, r5, #11
	bl Func_080150e4
	adds r0, r5, #0
	bl Trig_Sin
	ldr r3, .L_0818f460
	mov r1, r8
	ldrb r3, [r3, r1]
	adds r1, r3, #0
	muls r1, r0
	cmp r1, #0
	bge .L_0818f264
	adds r1, #63
.L_0818f264:
	movs r0, #170
	asrs r1, r1, #6
	lsls r0, r0, #7
	movs r2, #128
	lsls r1, r1, #1
	adds r0, #85
	lsls r2, r2, #8
	bl Func_080151e4
	ldr r0, .L_0818f464
	ldr r1, [sp, #12]
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_0818f286:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #8
	bne .L_0818f204
	mov r0, r9
	movs r3, #5
	movs r1, #53
	strb r3, [r0]
	add r1, sp
	movs r3, #2
	strb r3, [r1]
	ldr r2, [sp, #40]
	movs r0, #184
	lsls r0, r0, #5
	adds r3, r2, r0
	str r3, [sp, #56]
	ldr r3, .L_0818f468
	movs r1, #0
	str r3, [r7, #8]
	movs r3, #7
	str r3, [r7]
	ldr r0, [sp, #36]
	mov r8, r1
	ldr r1, .L_0818f46c
	lsls r3, r0, #13
	adds r1, r1, r3
	movs r2, #128
	ldr r3, [sp, #40]
	lsls r2, r2, #8
	movs r0, #224
	mov r11, r2
	lsls r0, r0, #2
	movs r2, #54
	mov r10, r1
	mov r9, r2
	adds r6, r3, r0
.L_0818f2d0:
	ldr r2, [sp, #36]
	mov r1, r8
	lsls r3, r1, #1
	cmp r2, r9
	blt .L_0818f330
	adds r3, #70
	cmp r2, r3
	bge .L_0818f330
	movs r0, #0
	movs r5, #128
	mov r3, r10
	str r0, [r7, #20]
	lsls r5, r5, #10
	subs r5, r5, r3
	bl Func_08014de4
	movs r1, #128
	mov r0, r11
	lsls r1, r1, #9
	mov r2, r11
	bl Func_080151e4
	ldr r0, [r6]
	ldr r1, .L_0818f470
	ldr r2, .L_0818f474
	adds r0, r0, r1
	ldr r1, [r6, #4]
	adds r1, r1, r2
	movs r2, #0
	bl Func_08015160
	ldr r0, [r6, #8]
	bl Func_080150e4
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #9
	mov r2, r11
	bl Func_080151e4
	ldr r0, .L_0818f478
	ldr r1, [sp, #12]
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_0818f330:
	ldr r3, .L_0818f44c
	movs r1, #1
	add r8, r1
	movs r0, #2
	mov r2, r8
	add r10, r3
	add r9, r0
	adds r6, #28
	cmp r2, #16
	bne .L_0818f2d0
	movs r2, #7
	add r3, sp, #52
	strb r2, [r3]
	str r3, [sp, #8]
	strb r2, [r3, #1]
	ldr r1, [sp, #8]
	ldr r0, .L_0818f47c
	ldr r3, .L_0818f480
	str r0, [r1, #4]
	str r3, [r7, #8]
	str r2, [r7]
	ldr r0, [sp, #36]
	movs r2, #0
	ldr r1, .L_0818f484
	mov r8, r2
	ldr r2, .L_0818f488
	lsls r3, r0, #3
	adds r1, r1, r3
	lsls r3, r0, #14
	adds r5, r3, r2
	negs r3, r0
	adds r3, #54
	subs r0, #54
	mov r9, r3
	ldr r3, [sp, #40]
	mov r10, r0
	movs r0, #224
	lsls r0, r0, #2
	mov r11, r1
	adds r6, r3, r0
.L_0818f380:
	mov r1, r8
	lsls r3, r1, #1
	ldr r0, [sp, #36]
	adds r2, r3, #0
	adds r2, #54
	cmp r0, r2
	blt .L_0818f3ee
	adds r3, #70
	cmp r0, r3
	bge .L_0818f3ee
	mov r1, r9
	mov r2, r10
	lsls r3, r1, #2
	cmp r2, #7
	ble .L_0818f3a4
	mov r0, r11
	subs r3, r3, r0
	adds r3, #64
.L_0818f3a4:
	str r3, [r7, #20]
	bl Func_08014de4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Func_080151e4
	ldr r0, [r6]
	ldr r1, .L_0818f470
	ldr r2, .L_0818f474
	adds r0, r0, r1
	ldr r1, [r6, #4]
	adds r1, r1, r2
	movs r2, #0
	bl Func_08015160
	movs r0, #240
	lsls r0, r0, #6
	bl Func_08015024
	adds r0, r5, #0
	adds r1, r5, #0
	adds r2, r5, #0
	bl Func_080151e4
	ldr r0, .L_0818f48c
	ldr r1, [sp, #12]
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_0818f3ee:
	movs r3, #16
	negs r3, r3
	ldr r0, .L_0818f490
	add r11, r3
	movs r3, #1
	movs r2, #2
	add r8, r3
	adds r5, r5, r0
	movs r1, #2
	negs r2, r2
	mov r0, r8
	add r9, r1
	add r10, r2
	adds r6, #28
	cmp r0, #16
	bne .L_0818f380
	movs r1, #0
	ldr r3, [sp, #36]
	mov r8, r1
	ldr r0, [sp, #36]
	ldr r1, [sp, #24]
	movs r2, #128
	lsls r2, r2, #8
	lsls r3, r3, #13
	mov r11, r2
	mov r10, r3
	lsls r6, r0, #3
	mov r9, r1
.L_0818f426:
	ldr r0, [sp, #36]
	mov r2, r8
	lsls r3, r2, #2
	cmp r0, r3
	blt .L_0818f4e8
	adds r3, #12
	cmp r0, r3
	bge .L_0818f4e8
	adds r0, r6, #0
	movs r3, #192
	lsls r3, r3, #9
	mov r1, r10
	subs r0, #64
	subs r5, r3, r1
	cmp r0, #0
	ble .L_0818f494
	movs r0, #0
	b .L_0818f494
	.2byte 0x0000
.L_0818f44c:
	.4byte 0xffffc000
.L_0818f450:
	.4byte Data_08197410
.L_0818f454:
	.4byte Data_081992b0
.L_0818f458:
	.4byte Data_08199eb4
.L_0818f45c:
	.4byte 0xffffc800
.L_0818f460:
	.4byte Data_08199ebc
.L_0818f464:
	.4byte Data_08199ea4
.L_0818f468:
	.4byte Data_08199220
.L_0818f46c:
	.4byte 0xfff94000
.L_0818f470:
	.4byte 0xff800000
.L_0818f474:
	.4byte 0xffc00000
.L_0818f478:
	.4byte Data_081991c0
.L_0818f47c:
	.4byte Data_02014000
.L_0818f480:
	.4byte Data_08199364
.L_0818f484:
	.4byte 0xfffffe50
.L_0818f488:
	.4byte 0xfff28000
.L_0818f48c:
	.4byte Data_081991e0
.L_0818f490:
	.4byte 0xffff8000
.L_0818f494:
	str r0, [r7, #20]
	bl Func_08014de4
	movs r1, #128
	mov r0, r11
	lsls r1, r1, #9
	mov r2, r11
	bl Func_080151e4
	mov r2, r9
	ldr r0, [r2]
	ldr r1, [r2, #4]
	subs r0, #128
	subs r1, #80
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	lsls r1, r5, #1
	adds r0, r1, #0
	adds r2, r5, #0
	bl Func_080151e4
	mov r0, r8
	lsls r3, r0, #14
	movs r0, #192
	lsls r0, r0, #7
	subs r0, r0, r3
	bl Func_080150e4
	ldr r0, .L_0818f5bc
	bl Func_08015024
	ldr r0, .L_0818f5c0
	ldr r1, [sp, #12]
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_0818f4e8:
	ldr r1, .L_0818f5c4
	movs r2, #1
	add r8, r2
	mov r3, r8
	add r10, r1
	subs r6, #32
	cmp r3, #2
	bne .L_0818f426
	movs r0, #0
	mov r8, r0
.L_0818f4fc:
	mov r1, r8
	ldr r2, [sp, #36]
	lsls r3, r1, #2
	adds r3, #54
	cmp r2, r3
	bne .L_0818f554
	cmp r1, #7
	bne .L_0818f514
	movs r0, #144
	bl Func_08118088 + 0x60
	b .L_0818f51a
.L_0818f514:
	movs r0, #144
	bl Audio_PlayCue
.L_0818f51a:
	ldr r1, [sp, #48]
	movs r2, #0
	movs r3, #36
	ldrsh r0, [r1, r3]
	movs r3, #0
	str r3, [sp, #0]
	movs r3, #100
	str r3, [sp, #4]
	movs r1, #1
	movs r3, #0
	bl Func_0815f000
	ldr r3, [sp, #48]
	movs r1, #7
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #5
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	movs r1, #238
	ldr r0, [sp, #40]
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r0, r1
	movs r3, #4
	str r3, [r2]
.L_0818f554:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #8
	bne .L_0818f4fc
	adds r0, r7, #0
	bl Sys_Free
	ldr r0, [sp, #12]
	bl Sys_Free
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r1, #240
	ldr r0, [sp, #40]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #36]
	adds r2, #1
	str r2, [sp, #36]
	cmp r2, #111
	beq .L_0818f596
	b .L_0818eea4
.L_0818f596:
	ldr r0, .L_0818f5c8
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #92
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0818f5bc:
	.4byte 0xfffff000
.L_0818f5c0:
	.4byte Data_08199210
.L_0818f5c4:
	.4byte 0xffff8000
.L_0818f5c8:
	.4byte Func_08143000
