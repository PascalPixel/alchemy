.syntax unified
	.thumb
	.global Func_0814ef44
	.thumb_func
Func_0814ef44:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #148
	str r0, [sp, #76]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	adds r7, r1, #0
	str r0, [sp, #72]
	movs r2, #0
	ldr r1, [r3, #96]
	movs r0, #0
	str r1, [sp, #68]
	ldr r3, [r3, #100]
	str r2, [sp, #48]
	str r3, [sp, #56]
	bl BattleFx_BeginCanvasLayer
	cmp r7, #13
	bne .L_0814ef84
	ldr r3, [sp, #72]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #180
	adds r2, r3, r5
	movs r3, #24
	str r3, [r2]
.L_0814ef84:
	ldr r0, [sp, #76]
	ldr r3, [r0, #28]
	cmp r3, #1
	bne .L_0814efee
	movs r1, #2
	cmp r7, #6
	beq .L_0814efaa
	cmp r7, #2
	beq .L_0814ef9a
	cmp r7, #7
	bne .L_0814ef9e
.L_0814ef9a:
	movs r1, #0
	b .L_0814efaa
.L_0814ef9e:
	adds r3, r7, #0
	subs r3, #9
	movs r1, #3
	cmp r3, #1
	bls .L_0814efaa
	movs r1, #1
.L_0814efaa:
	cmp r7, #6
	beq .L_0814efbe
	cmp r7, #0
	beq .L_0814efbe
	cmp r7, #8
	beq .L_0814efbe
	cmp r7, #9
	beq .L_0814efbe
	cmp r7, #10
	bne .L_0814efd2
.L_0814efbe:
	ldr r2, [sp, #76]
	ldr r0, [sp, #76]
	ldr r3, [r2, #4]
	add r2, sp, #136
	lsls r3, r3, #4
	orrs r1, r3
	add r3, sp, #124
	bl Func_0815585c
	b .L_0814efe8
.L_0814efd2:
	ldr r5, [sp, #76]
	add r2, sp, #136
	ldr r3, [r5, #4]
	adds r0, r5, #0
	lsls r3, r3, #4
	orrs r1, r3
	movs r3, #32
	orrs r1, r3
	add r3, sp, #124
	bl Func_0815585c
.L_0814efe8:
	ldr r0, [sp, #76]
	movs r3, #0
	str r3, [r0, #24]
.L_0814efee:
	cmp r7, #0
	beq .L_0814effa
	cmp r7, #8
	beq .L_0814effa
	cmp r7, #9
	bne .L_0814f02c
.L_0814effa:
	ldr r2, [sp, #76]
	add r5, sp, #112
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r5, #0
	bl Func_0815e20c
	ldr r2, [r5]
	movs r3, #64
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #19
	str r3, [sp, #48]
	adds r2, #40
	lsls r3, r3, #8
	str r3, [r2]
	ldr r3, .L_0814f028
	subs r2, #8
	strh r3, [r2]
	movs r3, #0
	str r3, [sp, #44]
	b .L_0814f030
	.2byte 0x0000
.L_0814f028:
	.4byte 0x00000100
.L_0814f02c:
	movs r5, #1
	str r5, [sp, #44]
.L_0814f030:
	ldr r1, [sp, #56]
	ldr r0, .L_0814f30c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r2, [sp, #72]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0814f310
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r1, [sp, #72]
	movs r2, #240
	lsls r2, r2, #4
	adds r5, r1, r2
	ldr r0, .L_0814f314
	adds r1, r5, #0
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r3, #0
	ldr r0, [sp, #72]
	mov r10, r3
	movs r3, #156
	movs r1, #144
	lsls r3, r3, #6
	lsls r1, r1, #1
	adds r2, r0, r3
.L_0814f072:
	ldrb r3, [r5]
	movs r0, #1
	add r10, r0
	strb r3, [r2]
	adds r5, #2
	adds r2, #1
	cmp r10, r1
	bne .L_0814f072
	cmp r7, #1
	bls .L_0814f09a
	cmp r7, #3
	beq .L_0814f09a
	cmp r7, #4
	beq .L_0814f09a
	cmp r7, #5
	beq .L_0814f09a
	cmp r7, #8
	beq .L_0814f09a
	cmp r7, #12
	bne .L_0814f0aa
.L_0814f09a:
	ldr r1, [sp, #76]
	ldr r3, [r1, #24]
	cmp r3, #0
	bne .L_0814f0a6
	ldr r0, .L_0814f318
	b .L_0814f0c8
.L_0814f0a6:
	ldr r0, .L_0814f31c
	b .L_0814f0c8
.L_0814f0aa:
	adds r3, r7, #0
	subs r3, #9
	cmp r3, #1
	bhi .L_0814f0b6
	ldr r0, .L_0814f320
	b .L_0814f0c8
.L_0814f0b6:
	cmp r7, #6
	beq .L_0814f0c2
	cmp r7, #11
	beq .L_0814f0c2
	cmp r7, #13
	bne .L_0814f0c6
.L_0814f0c2:
	ldr r0, .L_0814f324
	b .L_0814f0c8
.L_0814f0c6:
	ldr r0, .L_0814f328
.L_0814f0c8:
	bl Resource_GetTableEntry
	adds r2, r0, #0
	movs r0, #160
	adds r1, r2, #0
	ldr r3, .L_0814f32c
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r2, [sp, #44]
	cmp r2, #0
	bne .L_0814f106
	cmp r7, #6
	bne .L_0814f0ea
	ldr r0, .L_0814f330
	b .L_0814f130
.L_0814f0ea:
	cmp r7, #2
	beq .L_0814f0f2
	cmp r7, #7
	bne .L_0814f0f6
.L_0814f0f2:
	ldr r0, .L_0814f334
	b .L_0814f130
.L_0814f0f6:
	adds r3, r7, #0
	subs r3, #9
	cmp r3, #1
	bhi .L_0814f102
	ldr r0, .L_0814f320
	b .L_0814f130
.L_0814f102:
	ldr r0, .L_0814f338
	b .L_0814f130
.L_0814f106:
	cmp r7, #6
	beq .L_0814f112
	cmp r7, #11
	beq .L_0814f112
	cmp r7, #13
	bne .L_0814f116
.L_0814f112:
	ldr r0, .L_0814f33c
	b .L_0814f130
.L_0814f116:
	cmp r7, #2
	beq .L_0814f11e
	cmp r7, #7
	bne .L_0814f122
.L_0814f11e:
	ldr r0, .L_0814f340
	b .L_0814f130
.L_0814f122:
	adds r3, r7, #0
	subs r3, #9
	cmp r3, #1
	bhi .L_0814f12e
	ldr r0, .L_0814f344
	b .L_0814f130
.L_0814f12e:
	ldr r0, .L_0814f348
.L_0814f130:
	bl Resource_GetTableEntry
	adds r2, r0, #0
	ldr r3, [sp, #72]
	movs r5, #184
	adds r2, #128
	lsls r5, r5, #5
	adds r1, r3, r5
	adds r0, r2, #0
	bl Func_0801587c
	ldr r1, [sp, #76]
	mov r2, sp
	adds r2, #80
	ldr r0, [r1, #4]
	adds r1, r2, #0
	str r2, [sp, #40]
	bl Func_08144aac
	cmp r7, #0
	beq .L_0814f16a
	cmp r7, #6
	beq .L_0814f16a
	cmp r7, #8
	beq .L_0814f16a
	cmp r7, #9
	beq .L_0814f16a
	cmp r7, #10
	bne .L_0814f1c4
.L_0814f16a:
	ldr r5, .L_0814f34c
	movs r3, #0
	mov r10, r3
	movs r6, #255
.L_0814f172:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5]
	bl Random16
	movs r3, #127
	ands r3, r0
	adds r3, #64
	lsls r3, r3, #15
	str r3, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5, #8]
	movs r1, #128
	movs r0, #1
	movs r3, #0
	add r10, r0
	lsls r1, r1, #2
	str r3, [r5, #24]
	adds r5, #28
	cmp r10, r1
	bne .L_0814f172
	movs r2, #54
	str r2, [sp, #64]
	cmp r7, #9
	bne .L_0814f1b4
	b .L_0814f446
.L_0814f1b4:
	movs r3, #76
	str r3, [sp, #64]
	cmp r7, #8
	bne .L_0814f1be
	b .L_0814f446
.L_0814f1be:
	ldr r5, [sp, #76]
	ldr r3, [r5, #20]
	b .L_0814f440
.L_0814f1c4:
	cmp r7, #7
	bne .L_0814f220
	ldr r5, .L_0814f34c
	movs r0, #0
	movs r1, #255
	mov r10, r0
	movs r6, #0
	mov r8, r1
.L_0814f1d4:
	movs r3, #192
	lsls r3, r3, #14
	str r3, [r5, #4]
	str r6, [r5]
	str r6, [r5, #8]
	bl Random16
	mov r2, r8
	ands r0, r2
	subs r0, #127
	lsls r0, r0, #8
	str r0, [r5, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	adds r3, #64
	negs r3, r3
	lsls r3, r3, #9
	str r3, [r5, #16]
	bl Random16
	mov r3, r8
	ands r0, r3
	subs r0, #127
	lsls r0, r0, #8
	str r0, [r5, #20]
	movs r1, #128
	movs r0, #1
	add r10, r0
	lsls r1, r1, #2
	str r6, [r5, #24]
	adds r5, #28
	cmp r10, r1
	bne .L_0814f1d4
	ldr r2, [sp, #76]
	ldr r3, [r2, #20]
	b .L_0814f440
.L_0814f220:
	cmp r7, #12
	bne .L_0814f26a
	ldr r5, .L_0814f34c
	movs r3, #0
	mov r10, r3
	movs r6, #255
.L_0814f22c:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5]
	bl Random16
	movs r3, #127
	ands r3, r0
	adds r3, #64
	lsls r3, r3, #15
	str r3, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5, #8]
	movs r1, #128
	movs r0, #1
	movs r3, #0
	add r10, r0
	lsls r1, r1, #2
	str r3, [r5, #24]
	adds r5, #28
	cmp r10, r1
	bne .L_0814f22c
	ldr r2, [sp, #76]
	ldr r3, [r2, #20]
	b .L_0814f440
.L_0814f26a:
	cmp r7, #1
	beq .L_0814f272
	cmp r7, #13
	bne .L_0814f2b6
.L_0814f272:
	ldr r5, .L_0814f34c
	movs r3, #0
	mov r10, r3
	movs r6, #255
.L_0814f27a:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5, #8]
	movs r1, #128
	movs r0, #1
	movs r3, #0
	add r10, r0
	lsls r1, r1, #2
	str r3, [r5, #24]
	adds r5, #28
	cmp r10, r1
	bne .L_0814f27a
	ldr r2, [sp, #76]
	ldr r3, [r2, #20]
	b .L_0814f440
.L_0814f2b6:
	cmp r7, #11
	bne .L_0814f350
	ldr r5, .L_0814f34c
	movs r3, #0
	mov r10, r3
.L_0814f2c0:
	bl Random16
	movs r1, #200
	bl Math_ModU
	subs r0, #100
	lsls r0, r0, #15
	str r0, [r5]
	bl Random16
	movs r1, #200
	bl Math_ModU
	subs r0, #100
	lsls r0, r0, #15
	str r0, [r5, #4]
	bl Random16
	movs r1, #200
	bl Math_ModU
	subs r0, #100
	lsls r0, r0, #15
	str r0, [r5, #8]
	movs r1, #128
	movs r0, #1
	movs r3, #0
	add r10, r0
	lsls r1, r1, #2
	str r3, [r5, #24]
	adds r5, #28
	cmp r10, r1
	bne .L_0814f2c0
	ldr r2, [sp, #76]
	ldr r3, [r2, #20]
	lsls r3, r3, #3
	adds r3, #46
	b .L_0814f444
.L_0814f30c:
	.4byte 0x00000134
.L_0814f310:
	.4byte 0x0000017e
.L_0814f314:
	.4byte 0x00000190
.L_0814f318:
	.4byte 0x00000177
.L_0814f31c:
	.4byte 0x0000017d
.L_0814f320:
	.4byte 0x00000152
.L_0814f324:
	.4byte 0x00000148
.L_0814f328:
	.4byte 0x00000184
.L_0814f32c:
	.4byte IwramCopyWords
.L_0814f330:
	.4byte 0x00000150
.L_0814f334:
	.4byte 0x00000156
.L_0814f338:
	.4byte 0x00000154
.L_0814f33c:
	.4byte 0x00000151
.L_0814f340:
	.4byte 0x00000157
.L_0814f344:
	.4byte 0x00000153
.L_0814f348:
	.4byte 0x00000155
.L_0814f34c:
	.4byte gMapCellBuffer
.L_0814f350:
	cmp r7, #2
	bne .L_0814f3ba
	ldr r5, .L_0814f678
	movs r3, #0
	mov r10, r3
	mov r8, r5
.L_0814f35c:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	movs r5, #63
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	mov r0, r8
	str r3, [r0]
	ldr r3, .L_0814f67c
	str r3, [r0, #4]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	mov r1, r8
	str r3, [r1, #8]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #32
	mov r2, r8
	lsls r3, r3, #13
	str r3, [r2, #16]
	movs r5, #1
	movs r3, #0
	movs r0, #128
	str r3, [r2, #24]
	add r10, r5
	movs r3, #28
	lsls r0, r0, #2
	add r8, r3
	cmp r10, r0
	bne .L_0814f35c
	b .L_0814f43c
.L_0814f3ba:
	cmp r7, #3
	bne .L_0814f3fe
	ldr r5, .L_0814f678
	movs r2, #0
	mov r10, r2
	movs r6, #255
.L_0814f3c6:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #14
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	movs r3, #0
	str r0, [r5, #8]
	str r3, [r5, #24]
	movs r0, #128
	movs r3, #1
	add r10, r3
	lsls r0, r0, #2
	adds r5, #28
	cmp r10, r0
	bne .L_0814f3c6
	b .L_0814f43c
.L_0814f3fe:
	ldr r5, .L_0814f678
	movs r2, #0
	mov r10, r2
	movs r6, #255
.L_0814f406:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	movs r3, #0
	str r0, [r5, #8]
	str r3, [r5, #24]
	movs r0, #128
	movs r3, #1
	add r10, r3
	lsls r0, r0, #2
	adds r5, #28
	cmp r10, r0
	bne .L_0814f406
.L_0814f43c:
	ldr r1, [sp, #76]
	ldr r3, [r1, #20]
.L_0814f440:
	lsls r3, r3, #3
	adds r3, #58
.L_0814f444:
	str r3, [sp, #64]
.L_0814f446:
	movs r2, #64
	str r2, [sp, #52]
	ldr r5, [sp, #76]
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_0814f458
	movs r0, #32
	str r0, [sp, #52]
	b .L_0814f460
.L_0814f458:
	cmp r3, #2
	bne .L_0814f460
	movs r1, #128
	str r1, [sp, #52]
.L_0814f460:
	cmp r7, #8
	bne .L_0814f468
	movs r2, #128
	str r2, [sp, #52]
.L_0814f468:
	cmp r7, #12
	bne .L_0814f470
	movs r3, #16
	str r3, [sp, #52]
.L_0814f470:
	ldr r5, [sp, #72]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r5, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r5, r1
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_0814f680
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [sp, #64]
	movs r2, #0
	mov r11, r2
	cmp r3, #0
	bne .L_0814f49e
	b .L_0814f9c4
.L_0814f49e:
	mov r5, sp
	adds r5, #136
	str r5, [sp, #32]
	str r2, [sp, #8]
.L_0814f4a6:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	mov r0, r11
	str r3, [sp, #36]
	cmp r0, #40
	bne .L_0814f4ba
	movs r0, #0
	bl Func_081180e8
.L_0814f4ba:
	cmp r7, #7
	bne .L_0814f4ce
	movs r3, #3
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	bne .L_0814f4ce
	ldr r0, .L_0814f684
	bl Func_0815f0a0
.L_0814f4ce:
	ldr r2, [sp, #76]
	ldr r3, [r2, #28]
	cmp r3, #1
	bne .L_0814f5d0
	ldr r3, [sp, #44]
	cmp r3, #0
	bne .L_0814f552
	ldr r0, [sp, #8]
	bl Trig_Sin
	ldr r5, [sp, #32]
	lsls r3, r0, #2
	ldr r2, [r5]
	adds r3, r3, r0
	lsls r3, r3, #2
	ldr r0, [sp, #48]
	asrs r3, r3, #16
	adds r3, r3, r2
	adds r3, r3, r0
	subs r3, #20
	ldr r0, [sp, #8]
	mov r10, r3
	bl Trig_Cos
	ldr r3, [r5, #4]
	lsls r0, r0, #2
	asrs r0, r0, #16
	adds r0, r0, r3
	adds r5, r0, #0
	mov r1, r11
	subs r5, #24
	cmp r1, #32
	ble .L_0814f518
	lsls r3, r1, #1
	subs r3, r5, r3
	adds r5, r3, #0
	adds r5, #64
.L_0814f518:
	ldr r2, [sp, #72]
	movs r3, #184
	lsls r3, r3, #5
	adds r2, r2, r3
	mov r8, r2
	movs r6, #40
	str r6, [sp, #0]
	str r6, [sp, #4]
	ldr r4, [sp, #80]
	ldr r0, [sp, #68]
	mov r1, r8
	mov r2, r10
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
	mov r0, r11
	cmp r0, #3
	bgt .L_0814f5d0
	ldr r1, [sp, #40]
	str r6, [sp, #0]
	str r6, [sp, #4]
	ldr r0, [sp, #68]
	ldr r4, [r1, #4]
	mov r2, r10
	mov r1, r8
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
	b .L_0814f5d0
.L_0814f552:
	ldr r0, [sp, #8]
	bl Trig_Sin
	ldr r5, [sp, #32]
	lsls r2, r0, #2
	ldr r3, [r5]
	adds r2, r2, r0
	lsrs r1, r3, #31
	adds r3, r3, r1
	lsls r2, r2, #1
	asrs r3, r3, #1
	asrs r2, r2, #16
	adds r2, r2, r3
	subs r2, #10
	ldr r0, [sp, #8]
	mov r9, r2
	bl Trig_Cos
	ldr r3, [r5, #4]
	lsls r0, r0, #2
	asrs r0, r0, #16
	adds r0, r0, r3
	adds r5, r0, #0
	mov r0, r11
	subs r5, #24
	cmp r0, #32
	ble .L_0814f590
	lsls r3, r0, #1
	subs r3, r5, r3
	adds r5, r3, #0
	adds r5, #64
.L_0814f590:
	ldr r1, [sp, #72]
	movs r2, #184
	lsls r2, r2, #5
	movs r3, #20
	movs r0, #40
	adds r6, r1, r2
	str r3, [sp, #0]
	str r0, [sp, #4]
	adds r1, r6, #0
	mov r10, r3
	mov r8, r0
	ldr r4, [sp, #80]
	ldr r0, [sp, #68]
	mov r2, r9
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
	mov r1, r11
	cmp r1, #3
	bgt .L_0814f5d0
	mov r2, r10
	mov r3, r8
	str r2, [sp, #0]
	str r3, [sp, #4]
	ldr r0, [sp, #40]
	adds r1, r6, #0
	ldr r4, [r0, #4]
	mov r2, r9
	ldr r0, [sp, #68]
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
.L_0814f5d0:
	movs r1, #0
	str r1, [sp, #60]
	ldr r2, [sp, #76]
	ldr r3, [r2, #20]
	cmp r3, #0
	bne .L_0814f5de
	b .L_0814f996
.L_0814f5de:
	ldr r3, [sp, #36]
	mov r5, sp
	mov r0, r11
	adds r3, #12
	adds r5, #88
	lsls r0, r0, #9
	movs r1, #36
	str r3, [sp, #20]
	str r5, [sp, #28]
	str r0, [sp, #16]
	str r1, [sp, #12]
.L_0814f5f4:
	ldr r5, [sp, #76]
	ldr r2, [sp, #12]
	ldrsh r0, [r2, r5]
	bl GetBattleObjectSlotFar
	ldr r5, [r0]
	ldr r0, [sp, #60]
	lsls r0, r0, #3
	mov r9, r0
	bl Func_08014de4
	ldr r0, [sp, #36]
	ldr r1, [sp, #20]
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [r5, #8]
	ldr r1, [sp, #28]
	str r3, [r1]
	movs r3, #160
	lsls r3, r3, #14
	str r3, [r1, #4]
	ldr r3, [r5, #16]
	str r3, [r1, #8]
	ldr r0, [sp, #28]
	bl SceneTransform_ApplyPosition
	mov r3, r9
	adds r3, #20
	cmp r11, r3
	bne .L_0814f636
	movs r0, #126
	bl Audio_PlayCue
.L_0814f636:
	cmp r7, #8
	bne .L_0814f652
	mov r3, r9
	adds r3, #41
	cmp r11, r3
	bne .L_0814f6a6
	ldr r2, [sp, #12]
	ldr r5, [sp, #76]
	movs r1, #7
	ldrsh r0, [r2, r5]
	movs r3, #32
	movs r2, #1
	str r3, [sp, #0]
	b .L_0814f66c
.L_0814f652:
	cmp r7, #11
	bne .L_0814f688
	mov r3, r9
	adds r3, #24
	cmp r11, r3
	bne .L_0814f6a6
	ldr r1, [sp, #12]
	ldr r3, [sp, #76]
	ldrsh r0, [r1, r3]
	movs r3, #28
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #7
.L_0814f66c:
	negs r2, r2
	ldr r3, [sp, #60]
	bl Func_0814cd48
	b .L_0814f6a6
	.2byte 0x0000
.L_0814f678:
	.4byte gMapCellBuffer
.L_0814f67c:
	.4byte 0xffce0000
.L_0814f680:
	.4byte Func_08143000
.L_0814f684:
	.4byte 0x00000166
.L_0814f688:
	mov r3, r9
	adds r3, #36
	cmp r11, r3
	bne .L_0814f6a6
	ldr r2, [sp, #76]
	ldr r5, [sp, #12]
	movs r3, #28
	ldrsh r0, [r5, r2]
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #7
	negs r2, r2
	ldr r3, [sp, #60]
	bl Func_0814cd48
.L_0814f6a6:
	cmp r11, r9
	bgt .L_0814f6ac
	b .L_0814f978
.L_0814f6ac:
	cmp r7, #0
	beq .L_0814f71e
	cmp r7, #6
	beq .L_0814f71e
	cmp r7, #7
	beq .L_0814f71e
	cmp r7, #8
	beq .L_0814f71e
	cmp r7, #10
	beq .L_0814f71e
	cmp r7, #12
	beq .L_0814f71e
	cmp r7, #9
	bne .L_0814f6d6
	mov r3, r11
	mov r5, r9
	subs r0, r3, r5
	lsls r0, r0, #11
	bl Func_08015068
	b .L_0814f732
.L_0814f6d6:
	cmp r7, #11
	beq .L_0814f732
	cmp r7, #13
	bne .L_0814f6e8
	mov r1, r11
	lsls r0, r1, #9
	bl Func_080150e4
	b .L_0814f732
.L_0814f6e8:
	cmp r7, #1
	bne .L_0814f6fe
	mov r2, r11
	lsls r5, r2, #9
	adds r0, r5, #0
	bl SceneTransform_ApplyPitch
	adds r0, r5, #0
	bl Func_080150e4
	b .L_0814f732
.L_0814f6fe:
	cmp r7, #2
	beq .L_0814f706
	cmp r7, #12
	bne .L_0814f71a
.L_0814f706:
	ldr r3, [sp, #60]
	mov r5, r11
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #3
	subs r0, r5, r0
	lsls r0, r0, #9
	bl Func_08015068
	b .L_0814f732
.L_0814f71a:
	cmp r7, #3
	bne .L_0814f726
.L_0814f71e:
	ldr r0, [sp, #16]
	bl Func_08015068
	b .L_0814f732
.L_0814f726:
	ldr r0, [sp, #16]
	bl Func_08015068
	ldr r0, [sp, #16]
	bl SceneTransform_ApplyPitch
.L_0814f732:
	ldr r1, [sp, #52]
	movs r0, #0
	mov r10, r0
	cmp r1, #0
	bne .L_0814f73e
	b .L_0814f978
.L_0814f73e:
	ldr r3, [sp, #60]
	subs r5, r7, #3
	lsls r2, r3, #6
	ldr r0, .L_0814f9e8
	lsls r3, r3, #9
	subs r3, r3, r2
	str r5, [sp, #24]
	lsls r3, r3, #2
	adds r6, r3, r0
.L_0814f750:
	ldr r1, [sp, #24]
	cmp r1, #2
	bhi .L_0814f766
	mov r2, r10
	lsrs r3, r2, #31
	add r3, r10
	asrs r3, r3, #1
	add r3, r9
	adds r2, r3, #0
	adds r2, #32
	b .L_0814f77e
.L_0814f766:
	cmp r7, #7
	bne .L_0814f77a
	mov r5, r10
	lsrs r3, r5, #31
	add r3, r10
	asrs r3, r3, #1
	add r3, r9
	adds r2, r3, #0
	adds r2, #64
	b .L_0814f77e
.L_0814f77a:
	movs r2, #128
	lsls r2, r2, #9
.L_0814f77e:
	mov r3, r10
	cmp r3, #0
	bge .L_0814f786
	adds r3, #3
.L_0814f786:
	asrs r3, r3, #2
	add r3, r9
	cmp r11, r3
	bgt .L_0814f790
	b .L_0814f96a
.L_0814f790:
	cmp r11, r2
	blt .L_0814f796
	b .L_0814f96a
.L_0814f796:
	ldr r3, [r6]
	asrs r3, r3, #8
	adds r0, r3, #0
	muls r0, r3
	ldr r3, [r6, #4]
	asrs r3, r3, #8
	adds r2, r3, #0
	muls r2, r3
	ldr r3, [r6, #8]
	adds r0, r0, r2
	asrs r3, r3, #8
	adds r1, r3, #0
	muls r1, r3
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, .L_0814f9ec
	mov lr, r3
	.2byte 0xf800
	asrs r0, r0, #9
	mov r8, r0
	lsls r3, r0, #1
	add r3, r8
	lsls r0, r3, #4
	cmp r0, #0
	bge .L_0814f7ca
	adds r0, #63
.L_0814f7ca:
	asrs r0, r0, #6
	mov r8, r0
	cmp r0, #0
	bne .L_0814f7d4
	b .L_0814f96a
.L_0814f7d4:
	add r5, sp, #100
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0815e1ec
	cmp r7, #0
	beq .L_0814f7ea
	cmp r7, #8
	beq .L_0814f7ea
	cmp r7, #9
	bne .L_0814f7f2
.L_0814f7ea:
	ldr r3, [r5]
	ldr r2, [sp, #48]
	adds r3, r3, r2
	b .L_0814f7f6
.L_0814f7f2:
	ldr r3, [r5]
	asrs r3, r3, #1
.L_0814f7f6:
	str r3, [r5]
	ldr r3, [r5, #4]
	movs r0, #58
	adds r3, #16
	str r3, [r5, #4]
	ldr r3, [r5, #8]
	adds r0, #255
	cmp r3, r0
	bgt .L_0814f80e
	movs r3, #157
	lsls r3, r3, #1
	str r3, [r5, #8]
.L_0814f80e:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #122
	cmp r3, r2
	ble .L_0814f81c
	str r2, [r5, #8]
	adds r3, r2, #0
.L_0814f81c:
	ldr r2, .L_0814f9f0
	adds r1, r3, r2
	adds r2, r1, #0
	cmp r1, #0
	bge .L_0814f82a
	adds r2, r3, #0
	subs r2, #187
.L_0814f82a:
	asrs r2, r2, #7
	movs r3, #3
	subs r4, r3, r2
	cmp r7, #0
	beq .L_0814f83c
	cmp r7, #8
	beq .L_0814f83c
	cmp r7, #9
	bne .L_0814f842
.L_0814f83c:
	mov r3, r10
	lsls r0, r3, #2
	b .L_0814f858
.L_0814f842:
	cmp r7, #11
	beq .L_0814f84a
	cmp r7, #13
	bne .L_0814f850
.L_0814f84a:
	movs r3, #4
	subs r4, r3, r2
	b .L_0814f862
.L_0814f850:
	cmp r7, #7
	bne .L_0814f862
	mov r1, r10
	lsls r0, r1, #2
.L_0814f858:
	add r0, r11
	movs r1, #9
	bl __modsi3
	adds r4, r0, #0
.L_0814f862:
	cmp r7, #0
	beq .L_0814f886
	cmp r7, #3
	beq .L_0814f886
	cmp r7, #4
	beq .L_0814f886
	cmp r7, #5
	beq .L_0814f886
	cmp r7, #7
	beq .L_0814f886
	cmp r7, #8
	beq .L_0814f886
	cmp r7, #9
	beq .L_0814f886
	cmp r7, #11
	beq .L_0814f886
	cmp r7, #13
	bne .L_0814f8b4
.L_0814f886:
	ldr r2, .L_0814f9f4
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #72]
	movs r3, #224
	adds r1, r2, r1
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, .L_0814f9f8
	ldr r2, [r5]
	ldrb r0, [r3, r4]
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r5, [sp, #40]
	lsrs r4, r0, #1
	subs r2, r2, r4
	subs r3, r3, r4
	ldr r0, [sp, #68]
	ldr r4, [r5, #4]
	mov lr, r4
	.2byte 0xf800
	b .L_0814f904
.L_0814f8b4:
	cmp r7, #12
	bne .L_0814f8dc
	ldr r3, [r5, #4]
	ldr r2, [r5]
	subs r3, #12
	mov r12, r3
	ldr r3, [sp, #72]
	movs r5, #156
	movs r1, #24
	lsls r5, r5, #6
	str r1, [sp, #4]
	subs r2, #6
	adds r1, r3, r5
	str r7, [sp, #0]
	ldr r4, [sp, #80]
	ldr r0, [sp, #68]
	mov r3, r12
	mov lr, r4
	.2byte 0xf800
	b .L_0814f904
.L_0814f8dc:
	ldr r2, .L_0814f9fc
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #56]
	lsrs r3, r4, #31
	adds r1, r2, r1
	ldr r2, [r5]
	adds r3, r4, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #4]
	str r4, [sp, #0]
	ldr r5, [sp, #40]
	subs r3, r3, r4
	ldr r0, [sp, #68]
	ldr r4, [r5, #4]
	mov lr, r4
	.2byte 0xf800
.L_0814f904:
	cmp r7, #2
	bls .L_0814f924
	cmp r7, #6
	beq .L_0814f924
	cmp r7, #8
	beq .L_0814f924
	cmp r7, #9
	beq .L_0814f924
	cmp r7, #10
	beq .L_0814f924
	cmp r7, #11
	beq .L_0814f924
	cmp r7, #13
	beq .L_0814f924
	cmp r7, #12
	bne .L_0814f94e
.L_0814f924:
	ldr r5, [r6]
	mov r1, r8
	adds r0, r5, #0
	bl Math_Div
	subs r5, r5, r0
	str r5, [r6]
	ldr r5, [r6, #4]
	mov r1, r8
	adds r0, r5, #0
	bl Math_Div
	subs r5, r5, r0
	str r5, [r6, #4]
	ldr r5, [r6, #8]
	mov r1, r8
	adds r0, r5, #0
	bl Math_Div
	subs r5, r5, r0
	str r5, [r6, #8]
.L_0814f94e:
	cmp r7, #7
	bne .L_0814f96a
	ldr r2, [r6]
	ldr r3, [r6, #12]
	adds r2, r2, r3
	str r2, [r6]
	ldr r3, [r6, #16]
	ldr r2, [r6, #4]
	adds r2, r2, r3
	str r2, [r6, #4]
	ldr r3, [r6, #20]
	ldr r2, [r6, #8]
	adds r2, r2, r3
	str r2, [r6, #8]
.L_0814f96a:
	ldr r1, [sp, #52]
	movs r0, #1
	add r10, r0
	adds r6, #28
	cmp r10, r1
	beq .L_0814f978
	b .L_0814f750
.L_0814f978:
	ldr r2, [sp, #16]
	ldr r3, .L_0814fa00
	ldr r5, [sp, #12]
	ldr r0, [sp, #60]
	adds r2, r2, r3
	adds r5, #2
	adds r0, #1
	str r2, [sp, #16]
	str r5, [sp, #12]
	str r0, [sp, #60]
	ldr r1, [sp, #76]
	ldr r3, [r1, #20]
	cmp r0, r3
	beq .L_0814f996
	b .L_0814f5f4
.L_0814f996:
	bl Func_081434f8
	movs r5, #240
	ldr r3, [sp, #72]
	lsls r5, r5, #7
	adds r5, #232
	adds r2, r3, r5
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #8]
	movs r1, #128
	ldr r3, [sp, #64]
	lsls r1, r1, #4
	movs r2, #1
	adds r0, r0, r1
	add r11, r2
	str r0, [sp, #8]
	cmp r11, r3
	beq .L_0814f9c4
	b .L_0814f4a6
.L_0814f9c4:
	ldr r0, .L_0814fa04
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #148
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0814f9e8:
	.4byte gMapCellBuffer
.L_0814f9ec:
	.4byte IwramFillWords + 0x74
.L_0814f9f0:
	.4byte 0xfffffec6
.L_0814f9f4:
	.4byte Data_0819744c
.L_0814f9f8:
	.4byte Data_0819745e
.L_0814f9fc:
	.4byte Data_08197410
.L_0814fa00:
	.4byte 0xfffff000
.L_0814fa04:
	.4byte Func_08143000
