.syntax unified
	.thumb
	.global LuckyDice_Run
	.thumb_func
LuckyDice_Run:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_081ac2e0
	movs r1, #192
	lsls r1, r1, #3
	adds r1, #14
	movs r0, #100
	add sp, r5
	bl Runtime_AllocateHeapBlock
	movs r1, #128
	lsls r1, r1, #2
	movs r0, #96
	bl Runtime_AllocateHeapBlock
	movs r1, #246
	lsls r1, r1, #7
	str r0, [sp, #116]
	adds r1, #124
	movs r0, #92
	bl Runtime_AllocateBlock
	movs r1, #192
	lsls r1, r1, #3
	str r0, [sp, #112]
	adds r1, #20
	movs r0, #180
	bl Runtime_AllocateBlock
	movs r1, #76
	str r0, [sp, #108]
	movs r0, #48
	bl Runtime_AllocateBlock
	str r0, [sp, #104]
	ldr r0, .L_081ac2e4
	bl Func_080132fc
	bl Func_081ac028
	bl Func_080144c0
	ldr r2, .L_081ac2e8
	movs r3, #0
	strb r3, [r2]
	ldr r6, .L_081ac2ec
	movs r0, #0
	mov r11, r0
	movs r5, #0
	movs r4, #0
.L_081ac1ca:
	movs r1, #0
	adds r0, r4, #0
.L_081ac1ce:
	mov r3, r11
	adds r2, r3, r6
	movs r7, #2
	adds r3, r1, r0
	adds r1, #1
	strh r3, [r2]
	add r11, r7
	cmp r1, #32
	bne .L_081ac1ce
	adds r5, #1
	adds r4, #30
	cmp r5, #20
	bne .L_081ac1ca
	ldr r0, .L_081ac2f0
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r4, r0, #0
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	movs r0, #160
	lsls r2, r2, #24
	lsls r0, r0, #19
	add r1, sp, #256
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #224
	lsls r0, r0, #1
	adds r4, r4, r0
	adds r0, r4, #0
	ldr r1, .L_081ac2f4
	bl Func_0801587c
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081ac2f4
	ldr r1, .L_081ac2f8
	ldr r2, .L_081ac2fc
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081ac300
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r4, r0, #0
	adds r3, #212
	ldr r1, [sp, #116]
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	lsls r2, r2, #24
	adds r0, r4, #0
	ldr r1, .L_081ac304
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #2
	adds r4, r4, r1
	adds r0, r4, #0
	ldr r1, .L_081ac2f4
	bl Func_0801587c
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081ac2f4
	ldr r1, .L_081ac308
	ldr r2, .L_081ac30c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r5, #128
	lsls r5, r5, #1
	adds r3, r5, #0
	ldr r1, .L_081ac304
	ldr r0, [sp, #116]
	movs r2, #0
	bl Graphics_ScaleRgb555Buffer
	movs r1, #160
	adds r3, r5, #0
	lsls r1, r1, #19
	movs r2, #0
	add r0, sp, #256
	bl Graphics_ScaleRgb555Buffer
	ldr r3, .L_081ac2c8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	strh r3, [r2]
	ldr r3, .L_081ac2cc
	adds r2, #62
	strh r3, [r2]
	ldr r3, .L_081ac2d0
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_081ac2d4
	subs r2, #74
	strh r3, [r2]
	ldr r3, .L_081ac2d8
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_081ac2dc
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_081ac310
	movs r2, #0
	strh r2, [r3, #4]
	strh r2, [r3, #6]
	b .L_081ac314
.L_081ac2c8:
	.4byte 0x00000686
.L_081ac2cc:
	.4byte 0x00003737
.L_081ac2d0:
	.4byte 0x00002723
.L_081ac2d4:
	.4byte 0x00003340
.L_081ac2d8:
	.4byte 0x00003f44
.L_081ac2dc:
	.4byte 0x00000810
.L_081ac2e0:
	.4byte 0xfffffd00
.L_081ac2e4:
	.4byte 0x0000000c
.L_081ac2e8:
	.4byte Data_0300120c
.L_081ac2ec:
	.4byte 0x06003000
.L_081ac2f0:
	.4byte 0x00000084
.L_081ac2f4:
	.4byte gMapCellBuffer
.L_081ac2f8:
	.4byte 0x06004000
.L_081ac2fc:
	.4byte 0x84002580
.L_081ac300:
	.4byte 0x00000086
.L_081ac304:
	.4byte 0x05000200
.L_081ac308:
	.4byte 0x06010000
.L_081ac30c:
	.4byte 0x84001f00
.L_081ac310:
	.4byte Data_03001120
.L_081ac314:
	movs r3, #128
	ldr r1, .L_081ac350
	lsls r3, r3, #19
	adds r3, #20
	strh r2, [r3]
	adds r3, #2
	strh r1, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #2
	strh r1, [r3]
	ldr r1, .L_081ac354
	ldr r2, .L_081ac358
	adds r3, #38
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	subs r3, #2
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	mov r4, sp
	movs r3, #1
	movs r2, #0
	adds r4, #152
	negs r3, r3
	str r2, [sp, #96]
	str r2, [sp, #88]
	str r3, [sp, #84]
	b .L_081ac35c
.L_081ac350:
	.4byte 0x0000ff60
.L_081ac354:
	.4byte 0x000000f0
.L_081ac358:
	.4byte 0x000000a0
.L_081ac35c:
	str r2, [sp, #80]
	str r2, [sp, #76]
	str r4, [sp, #44]
	str r4, [sp, #20]
	ldr r5, [sp, #112]
	mov r11, r2
	movs r6, #127
	movs r7, #31
.L_081ac36c:
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	ands r0, r7
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #16
	str r0, [r5, #8]
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #14
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	adds r0, #32
	lsls r0, r0, #12
	str r0, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #14
	movs r2, #0
	str r0, [r5, #20]
	str r2, [r5, #24]
	ldr r1, [sp, #20]
	movs r3, #150
	lsls r3, r3, #1
	stmia r1!, {r3}
	movs r3, #1
	add r11, r3
	adds r0, r1, #0
	mov r4, r11
	str r0, [sp, #20]
	adds r5, #28
	cmp r4, #2
	bne .L_081ac36c
	mov r5, sp
	add r3, sp, #224
	adds r5, #144
	str r2, [r3, #12]
	str r2, [r3, #8]
	str r2, [r3, #4]
	str r2, [r3]
	str r5, [sp, #56]
	str r2, [r5, #4]
	str r2, [sp, #144]
	bl Random16
	movs r1, #6
	bl Math_ModU
	mov r6, sp
	adds r6, #136
	str r0, [sp, #136]
	str r6, [sp, #48]
.L_081ac3f4:
	bl Random16
	movs r1, #6
	bl Math_ModU
	ldr r7, [sp, #48]
	str r0, [r7, #4]
	ldr r3, [sp, #136]
	cmp r3, r0
	beq .L_081ac3f4
	ldr r0, [sp, #112]
	movs r3, #160
	lsls r3, r3, #14
	ldr r2, .L_081ac714
	str r3, [r0]
	ldr r3, .L_081ac718
	movs r1, #192
	lsls r1, r1, #15
	str r1, [r0, #4]
	str r2, [r0, #8]
	str r3, [r0, #28]
	str r1, [r0, #32]
	str r2, [r0, #36]
	bl Resource_FarCall004
	movs r1, #6
	str r1, [sp, #0]
	mov r8, r1
	movs r2, #12
	movs r1, #0
	movs r3, #3
	movs r0, #18
	bl UiWindow_CreateFar
	movs r3, #128
	ldr r2, [sp, #108]
	ldr r5, .L_081ac71c
	lsls r3, r3, #3
	adds r3, #196
	adds r6, r2, r3
	adds r1, r0, #0
	str r1, [r6]
	adds r0, r5, #0
	movs r2, #48
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r3, .L_081ac720
	ldr r2, [r6]
	ldr r0, [r3, #16]
	movs r1, #6
	movs r3, #0
	str r3, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	mov r4, r8
	movs r1, #16
	movs r2, #8
	movs r3, #4
	movs r0, #22
	str r4, [sp, #0]
	bl UiWindow_CreateFar
	adds r1, r0, #0
	ldr r7, [sp, #108]
	movs r0, #128
	lsls r0, r0, #3
	adds r0, #204
	adds r6, r7, r0
	adds r0, r5, #0
	str r1, [r6]
	adds r0, #10
	movs r2, #0
	movs r3, #0
	adds r5, #11
	bl UiText_DrawCharacterAtOffsetFar
	ldr r1, [r6]
	movs r2, #0
	adds r0, r5, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	ldr r2, [sp, #104]
	movs r1, #0
	adds r2, #12
	str r1, [sp, #100]
	str r2, [sp, #40]
.L_081ac4a4:
	ldr r4, [sp, #100]
	movs r3, #0
	mov r8, r3
	cmp r4, #16
	bgt .L_081ac4ce
	movs r6, #128
	lsls r5, r4, #12
	lsls r6, r6, #1
	ldr r1, .L_081ac724
	adds r2, r5, #0
	adds r3, r6, #0
	ldr r0, [sp, #116]
	bl Graphics_ScaleRgb555Buffer
	movs r1, #160
	add r0, sp, #256
	lsls r1, r1, #19
	adds r2, r5, #0
	adds r3, r6, #0
	bl Graphics_ScaleRgb555Buffer
.L_081ac4ce:
	bl Func_08014de4
	ldr r0, [sp, #104]
	ldr r1, [sp, #40]
	bl Func_080156e8
	ldr r3, .L_081ac728
	ldr r5, [sp, #108]
	movs r2, #196
	str r3, [r5, r2]
	movs r3, #214
	movs r2, #200
	lsls r3, r3, #2
	str r3, [r5, r2]
	ldr r3, .L_081ac72c
	movs r2, #204
	str r3, [r5, r2]
	movs r3, #230
	movs r2, #208
	lsls r3, r3, #2
	str r3, [r5, r2]
	ldr r3, .L_081ac730
	movs r2, #212
	str r3, [r5, r2]
	movs r3, #232
	movs r2, #216
	lsls r3, r3, #2
	str r3, [r5, r2]
	ldr r3, .L_081ac734
	movs r2, #220
	movs r1, #234
	str r3, [r5, r2]
	lsls r1, r1, #2
	movs r3, #224
	str r1, [r5, r3]
	ldr r3, .L_081ac738
	movs r2, #228
	str r3, [r5, r2]
	movs r3, #232
	str r1, [r5, r3]
	movs r6, #0
	mov r11, r6
	ldr r2, [sp, #48]
	ldr r6, .L_081ac73c
	movs r5, #236
	lsls r5, r5, #2
	movs r4, #32
	movs r0, #240
	movs r1, #236
.L_081ac530:
	ldr r7, [sp, #108]
	adds r3, r4, #0
	orrs r3, r6
	str r3, [r7, r1]
	adds r4, #16
	ldmia r2!, {r3}
	adds r1, #8
	lsls r3, r3, #3
	adds r3, r3, r5
	str r3, [r7, r0]
	movs r3, #1
	add r11, r3
	mov r7, r11
	adds r0, #8
	cmp r7, #2
	bne .L_081ac530
	ldr r3, [sp, #96]
	movs r0, #7
	subs r3, #2
	str r0, [sp, #68]
	cmp r3, #1
	bhi .L_081ac62a
	movs r1, #0
	mov r11, r1
	movs r2, #128
	movs r1, #224
	lsls r1, r1, #14
	movs r6, #0
	lsls r2, r2, #1
	movs r7, #252
.L_081ac56c:
	ldr r4, [sp, #88]
	mov r3, r11
	lsls r5, r3, #2
	cmp r4, r6
	bne .L_081ac586
	movs r0, #48
	adds r0, #255
	str r1, [sp, #16]
	str r2, [sp, #12]
	bl Audio_PlayCue
	ldr r2, [sp, #12]
	ldr r1, [sp, #16]
.L_081ac586:
	ldr r0, [sp, #88]
	cmp r0, r6
	blt .L_081ac5a8
	ldr r3, .L_081ac740
	ldr r4, [sp, #108]
	orrs r3, r1
	str r3, [r4, r7]
	add r3, sp, #224
	ldr r3, [r3, r5]
	adds r7, #8
	lsls r3, r3, #5
	adds r3, #152
	str r3, [r4, r2]
	ldr r5, [sp, #68]
	adds r2, #8
	adds r5, #1
	str r5, [sp, #68]
.L_081ac5a8:
	movs r3, #1
	movs r0, #128
	add r11, r3
	lsls r0, r0, #14
	mov r4, r11
	adds r1, r1, r0
	adds r6, #5
	cmp r4, #4
	bne .L_081ac56c
	ldr r5, [sp, #88]
	cmp r5, #19
	ble .L_081ac62a
	ldr r6, [sp, #84]
	cmp r6, #0
	blt .L_081ac62a
	ldr r3, .L_081ac744
	movs r7, #0
	ldrb r2, [r3, r6]
	mov r11, r7
	cmp r2, #0
	beq .L_081ac62a
	ldr r3, .L_081ac748
	ldr r1, [sp, #68]
	ldrb r3, [r3, r6]
	movs r0, #96
	lsls r3, r3, #3
	adds r4, r3, #0
	lsls r3, r1, #3
	adds r1, r3, #0
	mov r12, r0
	adds r0, r3, #0
	ldr r3, .L_081ac74c
	adds r5, r2, #0
	movs r7, #128
	mov lr, r3
	movs r3, #158
	lsls r2, r5, #4
	lsls r7, r7, #1
	lsls r3, r3, #2
	adds r4, #152
	adds r0, #200
	adds r1, #196
	adds r7, #255
	subs r2, r3, r2
.L_081ac600:
	adds r3, r2, #0
	ands r3, r7
	mov r6, r12
	lsls r3, r3, #16
	orrs r3, r6
	mov r6, lr
	orrs r3, r6
	ldr r6, [sp, #108]
	adds r2, #32
	str r3, [r6, r1]
	str r4, [r6, r0]
	ldr r3, [sp, #68]
	movs r6, #1
	adds r3, #1
	add r11, r6
	adds r0, #8
	adds r1, #8
	str r3, [sp, #68]
	adds r4, #32
	cmp r11, r5
	bne .L_081ac600
.L_081ac62a:
	ldr r7, [sp, #96]
	cmp r7, #2
	beq .L_081ac632
	b .L_081ac758
.L_081ac632:
	ldr r0, [sp, #88]
	cmp r0, #20
	bne .L_081ac6d0
	ldr r1, [sp, #80]
	cmp r1, #0
	ble .L_081ac6d0
	adds r0, r1, #0
	bl Func_080ad1d8
	ldr r3, [sp, #80]
	movs r2, #1
	str r2, [sp, #72]
	cmp r3, #9
	ble .L_081ac67c
	ldr r5, [sp, #80]
	movs r4, #2
	str r4, [sp, #72]
	cmp r5, #99
	ble .L_081ac67c
	movs r0, #186
	ldr r7, [sp, #80]
	lsls r0, r0, #2
	movs r6, #3
	adds r0, #255
	str r6, [sp, #72]
	cmp r7, r0
	ble .L_081ac67c
	movs r3, #156
	ldr r2, [sp, #80]
	lsls r3, r3, #6
	movs r1, #4
	adds r3, #15
	str r1, [sp, #72]
	cmp r2, r3
	ble .L_081ac67c
	movs r4, #5
	str r4, [sp, #72]
.L_081ac67c:
	ldr r5, [sp, #72]
	movs r0, #8
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	subs r0, r0, r3
	adds r2, r5, #0
	movs r3, #6
	str r3, [sp, #0]
	adds r2, #13
	movs r1, #16
	movs r3, #3
	bl UiWindow_CreateFar
	ldr r6, [sp, #108]
	movs r7, #153
	lsls r7, r7, #3
	adds r5, r6, r7
	ldr r6, .L_081ac750
	adds r1, r0, #0
	str r1, [r5]
	adds r0, r6, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, [sp, #72]
	ldr r1, [r5]
	lsls r2, r0, #3
	adds r2, #56
	subs r0, r6, #1
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r1, #0
	ldr r2, [r5]
	ldr r0, [sp, #80]
	str r1, [sp, #0]
	movs r3, #56
	ldr r1, [sp, #72]
	bl UiText_DrawNumberInWindowFar
.L_081ac6d0:
	ldr r2, [sp, #76]
	cmp r2, #0
	ble .L_081ac6da
	subs r2, #1
	str r2, [sp, #76]
.L_081ac6da:
	ldr r3, [sp, #88]
	cmp r3, #19
	ble .L_081ac70a
	ldr r3, .L_081ac754
	movs r2, #1
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_081ac70a
	movs r0, #112
	bl Audio_PlayCue
	ldr r4, [sp, #80]
	cmp r4, #0
	ble .L_081ac702
	movs r5, #3
	movs r6, #0
	str r5, [sp, #96]
	str r6, [sp, #76]
	b .L_081ac70a
.L_081ac702:
	movs r0, #0
	str r0, [sp, #96]
	movs r7, #1
	mov r8, r7
.L_081ac70a:
	ldr r1, [sp, #88]
	adds r1, #1
	str r1, [sp, #88]
	b .L_081ac980
	.2byte 0x0000
.L_081ac714:
	.4byte 0xff600000
.L_081ac718:
	.4byte 0xffd80000
.L_081ac71c:
	.4byte 0x00000d69
.L_081ac720:
	.4byte gPartyState
.L_081ac724:
	.4byte 0x05000200
.L_081ac728:
	.4byte 0xc0006000
.L_081ac72c:
	.4byte 0x40102020
.L_081ac730:
	.4byte 0x40102030
.L_081ac734:
	.4byte 0x40042020
.L_081ac738:
	.4byte 0x40042030
.L_081ac73c:
	.4byte 0x40202000
.L_081ac740:
	.4byte 0x80002040
.L_081ac744:
	.4byte Data_081ad368
.L_081ac748:
	.4byte Data_081ad36c
.L_081ac74c:
	.4byte 0x80002000
.L_081ac750:
	.4byte 0x00000d6a
.L_081ac754:
	.4byte gInput
.L_081ac758:
	ldr r2, [sp, #96]
	cmp r2, #3
	bne .L_081ac7f4
	ldr r3, [sp, #76]
	cmp r3, #0
	bne .L_081ac798
	movs r4, #0
	ldr r5, [sp, #108]
	str r4, [sp, #80]
	movs r6, #153
	lsls r6, r6, #3
	adds r3, r5, r6
	ldr r2, [r3]
	movs r0, #0
	ldr r1, [sp, #72]
	movs r3, #56
	str r4, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	movs r7, #128
	ldr r3, .L_081aca44
	ldr r1, [sp, #80]
	lsls r7, r7, #3
	adds r7, #196
	ldr r0, [r3, #16]
	adds r3, r5, r7
	ldr r2, [r3]
	str r1, [sp, #0]
	movs r3, #0
	movs r1, #6
	bl UiText_DrawNumberInWindowFar
.L_081ac798:
	ldr r2, [sp, #76]
	cmp r2, #15
	bne .L_081ac7ec
	ldr r4, [sp, #108]
	movs r3, #0
	str r3, [sp, #96]
	movs r5, #153
	lsls r5, r5, #3
	adds r3, r4, r5
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #8
	movs r3, #4
	movs r0, #22
	bl UiWindow_CreateFar
	adds r1, r0, #0
	ldr r7, [sp, #108]
	movs r0, #128
	ldr r5, .L_081aca48
	lsls r0, r0, #3
	adds r0, #204
	adds r6, r7, r0
	str r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r1, [r6]
	adds r0, r5, #1
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	movs r1, #1
	mov r8, r1
.L_081ac7ec:
	ldr r2, [sp, #76]
	adds r2, #1
	str r2, [sp, #76]
	b .L_081ac980
.L_081ac7f4:
	ldr r3, [sp, #96]
	cmp r3, #0
	beq .L_081ac7fc
	b .L_081ac980
.L_081ac7fc:
	ldr r4, [sp, #100]
	cmp r4, #17
	bgt .L_081ac804
	b .L_081ac980
.L_081ac804:
	ldr r6, .L_081aca4c
	movs r2, #64
	ldr r3, [r6, #12]
	ldr r3, [r6, #12]
	ldr r3, [r6]
	ands r3, r2
	cmp r3, #0
	beq .L_081ac826
	ldr r5, [sp, #112]
	movs r2, #128
	ldr r3, [r5, #4]
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r5, #32]
	adds r3, r3, r2
	str r3, [r5, #32]
.L_081ac826:
	ldr r3, [r6]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_081ac840
	ldr r7, [sp, #112]
	ldr r2, .L_081aca50
	ldr r3, [r7, #4]
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r3, [r7, #32]
	adds r3, r3, r2
	str r3, [r7, #32]
.L_081ac840:
	ldr r0, [sp, #112]
	ldr r1, .L_081aca54
	ldr r3, [r0, #4]
	cmp r3, r1
	bgt .L_081ac850
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r0, #4]
.L_081ac850:
	movs r2, #160
	lsls r2, r2, #16
	cmp r3, r2
	ble .L_081ac85c
	ldr r3, [sp, #112]
	str r2, [r3, #4]
.L_081ac85c:
	ldr r4, [sp, #112]
	ldr r3, [r4, #32]
	cmp r3, r1
	bgt .L_081ac86a
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r4, #32]
.L_081ac86a:
	cmp r3, r2
	ble .L_081ac872
	ldr r5, [sp, #112]
	str r2, [r5, #32]
.L_081ac872:
	ldr r7, .L_081aca44
	bl Func_080ad290
	ldr r2, [r7, #16]
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	cmp r2, r3
	bcs .L_081ac890
	ldr r6, [sp, #108]
	movs r7, #128
	lsls r7, r7, #3
	adds r7, #204
	adds r3, r6, r7
	b .L_081ac8aa
.L_081ac890:
	ldr r5, [r6, #12]
	movs r3, #2
	ands r5, r3
	cmp r5, #0
	beq .L_081ac8b6
	movs r0, #113
	bl Audio_PlayCue
	movs r1, #128
	ldr r0, [sp, #108]
	lsls r1, r1, #3
	adds r1, #204
	adds r3, r0, r1
.L_081ac8aa:
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
	bl .L_081ad2d2
.L_081ac8b6:
	ldr r3, [r6, #12]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_081ac980
	movs r0, #151
	lsls r0, r0, #1
	bl Audio_PlayCue
	movs r4, #128
	ldr r2, [sp, #108]
	lsls r4, r4, #3
	adds r4, #204
	adds r3, r2, r4
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r6, #1
	str r6, [sp, #96]
	bl Func_080ad290
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	str r3, [sp, #92]
	ldr r3, [r7, #16]
	ldr r0, [sp, #92]
	cmp r0, r3
	bls .L_081ac8f4
	str r3, [sp, #92]
.L_081ac8f4:
	ldr r1, [sp, #92]
	negs r0, r1
	bl Func_080ad1d8
	movs r4, #128
	ldr r2, [sp, #108]
	lsls r4, r4, #3
	adds r4, #196
	adds r3, r2, r4
	ldr r0, [r7, #16]
	ldr r2, [r3]
	movs r1, #6
	movs r3, #0
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	ldr r7, [sp, #44]
	ldr r6, [sp, #112]
	movs r5, #0
	mov r11, r5
.L_081ac91c:
	mov r0, r11
	cmp r0, #1
	ble .L_081ac92c
	ldr r3, .L_081aca58
	str r3, [r6, #8]
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r6, #4]
.L_081ac92c:
	bl Random16
	movs r3, #127
	ands r3, r0
	subs r3, #64
	lsls r3, r3, #12
	str r3, [r6, #12]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #48
	lsls r3, r3, #13
	str r3, [r6, #16]
	bl Random16
	movs r5, #63
	movs r1, #6
	ands r5, r0
	ldr r0, [r6, #4]
	bl Math_Div
	adds r5, #140
	lsls r5, r5, #12
	adds r5, r5, r0
	str r5, [r6, #20]
	bl Random16
	movs r1, #144
	lsls r1, r1, #7
	bl Math_ModU
	movs r1, #1
	movs r3, #150
	add r11, r1
	lsls r3, r3, #1
	mov r2, r11
	str r0, [r6, #24]
	stmia r7!, {r3}
	adds r6, #28
	cmp r2, #2
	bne .L_081ac91c
.L_081ac980:
	mov r3, r8
	cmp r3, #1
	bne .L_081ac9c0
	ldr r4, [sp, #112]
	movs r3, #160
	lsls r3, r3, #14
	ldr r2, .L_081aca5c
	str r3, [r4]
	ldr r3, .L_081aca60
	movs r1, #192
	lsls r1, r1, #15
	str r1, [r4, #4]
	str r1, [r4, #32]
	str r2, [r4, #8]
	str r3, [r4, #28]
	str r2, [r4, #36]
	bl Random16
	movs r1, #6
	bl Math_ModU
	str r0, [sp, #136]
.L_081ac9ac:
	bl Random16
	movs r1, #6
	bl Math_ModU
	ldr r5, [sp, #48]
	str r0, [r5, #4]
	ldr r3, [sp, #136]
	cmp r3, r0
	beq .L_081ac9ac
.L_081ac9c0:
	mov r7, sp
	mov r1, sp
	adds r7, #128
	movs r0, #1
	movs r6, #0
	adds r1, #132
	str r7, [sp, #52]
	str r6, [r7]
	str r0, [sp, #132]
	str r1, [sp, #24]
	mov r11, r0
	mov r8, r7
	mov lr, r7
.L_081ac9da:
	ldr r3, [sp, #24]
	mov r6, r11
	ldmia r3!, {r5}
	subs r6, #1
	adds r2, r3, #0
	str r2, [sp, #24]
	cmp r6, #0
	blt .L_081aca64
	ldr r4, [sp, #52]
	lsls r0, r6, #2
	ldr r3, [r0, r4]
	ldr r7, [sp, #112]
	lsls r2, r3, #3
	subs r2, r2, r3
	lsls r3, r5, #3
	subs r3, r3, r5
	lsls r3, r3, #2
	adds r1, r3, #0
	lsls r2, r2, #2
	adds r2, #8
	adds r1, #8
	ldr r2, [r7, r2]
	ldr r3, [r7, r1]
	cmp r2, r3
	ble .L_081aca66
	mov r10, r1
	adds r1, r0, #0
	adds r4, r1, #4
	mov r12, lr
.L_081aca14:
	mov r0, r12
	ldr r3, [r1, r0]
	subs r6, #1
	str r3, [r4, r0]
	subs r1, #4
	subs r4, #4
	cmp r6, #0
	blt .L_081aca64
	mov r3, r12
	adds r0, r1, #0
	ldr r2, [r0, r3]
	ldr r7, [sp, #112]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, #8
	ldr r2, [r7, r3]
	mov r3, r10
	ldr r3, [r7, r3]
	mov r9, r3
	cmp r2, r9
	bgt .L_081aca14
	b .L_081aca66
	.2byte 0x0000
.L_081aca44:
	.4byte gPartyState
.L_081aca48:
	.4byte 0x00000d73
.L_081aca4c:
	.4byte gInput
.L_081aca50:
	.4byte 0xfffc0000
.L_081aca54:
	.4byte 0x001fffff
.L_081aca58:
	.4byte 0xff4c0000
.L_081aca5c:
	.4byte 0xff600000
.L_081aca60:
	.4byte 0xffd80000
.L_081aca64:
	lsls r0, r6, #2
.L_081aca66:
	adds r3, r0, #4
	mov r4, r8
	str r5, [r3, r4]
	movs r5, #1
	add r11, r5
	mov r6, r11
	cmp r6, #2
	bne .L_081ac9da
	ldr r1, [sp, #68]
	movs r7, #0
	lsls r3, r1, #3
	mov r11, r7
	movs r0, #240
	movs r2, #200
	ldr r7, [sp, #52]
	adds r2, r2, r3
	add r0, sp
	adds r3, #196
	add r6, sp, #200
	mov r9, r0
	mov r10, r2
	mov r8, r3
.L_081aca92:
	ldr r2, [r7]
	ldr r5, [sp, #112]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r4, r5, r3
	adds r0, r4, #0
	adds r1, r6, #0
	str r4, [sp, #8]
	bl Func_08015778
	ldr r2, [r6, #8]
	movs r0, #94
	adds r0, #255
	ldr r4, [sp, #8]
	cmp r2, r0
	bgt .L_081acabc
	movs r3, #175
	lsls r3, r3, #1
	str r3, [r6, #8]
	adds r2, r3, #0
.L_081acabc:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #138
	cmp r2, r3
	ble .L_081acaca
	str r3, [r6, #8]
	adds r2, r3, #0
.L_081acaca:
	ldr r3, [r7]
	ldr r1, .L_081ace5c
	lsls r3, r3, #2
	adds r2, r2, r1
	mov r5, r9
	str r2, [r5, r3]
	ldr r5, [r7]
	mov r0, r9
	lsls r3, r5, #2
	ldr r1, [r0, r3]
	movs r0, #128
	lsrs r3, r1, #31
	adds r1, r1, r3
	asrs r1, r1, #1
	adds r1, #128
	lsls r0, r0, #4
	str r4, [sp, #8]
	bl Math_Div
	ldr r3, [r6]
	movs r1, #16
	subs r2, r3, r0
	ldr r3, [r6, #4]
	negs r1, r1
	subs r0, r3, r0
	adds r3, r0, #0
	adds r3, #96
	ldr r4, [sp, #8]
	cmp r3, r1
	ble .L_081acb5c
	lsls r1, r5, #25
	movs r5, #158
	lsls r5, r5, #2
	adds r3, r2, r5
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	ands r3, r2
	lsls r3, r3, #16
	subs r2, #159
	orrs r1, r3
	adds r3, r0, r2
	movs r2, #255
	ands r3, r2
	orrs r1, r3
	ldr r3, .L_081ace60
	mov r5, r8
	orrs r1, r3
	ldr r3, [sp, #108]
	ldr r2, .L_081ace64
	str r1, [r3, r5]
	ldr r0, [r4, #24]
	cmp r0, #0
	bge .L_081acb3e
	movs r1, #192
	lsls r1, r1, #2
	adds r1, #255
	adds r0, r0, r1
.L_081acb3e:
	asrs r3, r0, #10
	ldrb r3, [r2, r3]
	movs r2, #128
	lsls r2, r2, #3
	lsls r3, r3, #3
	orrs r3, r2
	ldr r2, [sp, #108]
	mov r4, r10
	str r3, [r2, r4]
	ldr r0, [sp, #68]
	movs r5, #8
	adds r0, #1
	str r0, [sp, #68]
	add r10, r5
	add r8, r5
.L_081acb5c:
	movs r1, #1
	add r11, r1
	mov r2, r11
	adds r7, #4
	cmp r2, #2
	bne .L_081aca92
	ldr r3, [sp, #96]
	cmp r3, #0
	bne .L_081acc2c
	ldr r1, [sp, #68]
	ldr r7, [sp, #52]
	mov r6, sp
	adds r6, #240
	ldr r0, [sp, #112]
	lsls r3, r1, #3
	movs r5, #200
	movs r2, #200
	str r6, [sp, #64]
	movs r4, #0
	add r5, sp
	str r7, [sp, #28]
	adds r2, r2, r3
	adds r7, r3, #0
	mov r11, r4
	mov r9, r5
	mov r10, r0
	mov r8, r2
	adds r7, #196
.L_081acb94:
	mov r1, r9
	mov r0, r10
	bl Func_08015778
	ldr r5, [sp, #28]
	ldr r0, [sp, #64]
	ldmia r5!, {r3}
	adds r4, r5, #0
	str r4, [sp, #28]
	lsls r3, r3, #2
	ldr r6, [r0, r3]
	movs r0, #128
	lsrs r3, r6, #31
	adds r6, r6, r3
	asrs r6, r6, #1
	adds r6, #128
	adds r1, r6, #0
	lsls r0, r0, #4
	bl Math_Div
	mov r1, r9
	ldr r5, [r1]
	adds r1, r6, #0
	subs r5, r5, r0
	movs r0, #192
	lsls r0, r0, #2
	bl Math_Div
	mov r3, r9
	ldr r2, [r3, #4]
	movs r4, #158
	movs r3, #128
	lsls r4, r4, #2
	lsls r3, r3, #1
	movs r6, #176
	adds r2, r2, r0
	adds r3, #255
	adds r5, r5, r4
	lsls r6, r6, #1
	ands r5, r3
	adds r2, r2, r6
	movs r3, #255
	ands r2, r3
	ldr r3, .L_081ace68
	lsls r5, r5, #16
	ldr r0, [sp, #108]
	orrs r5, r2
	orrs r5, r3
	mov r1, r11
	str r5, [r0, r7]
	cmp r1, #1
	bne .L_081acc06
	ldr r3, [r0, r7]
	movs r2, #128
	lsls r2, r2, #21
	orrs r3, r2
	str r3, [r0, r7]
.L_081acc06:
	ldr r2, [sp, #108]
	movs r3, #144
	lsls r3, r3, #4
	mov r4, r8
	adds r3, #88
	str r3, [r2, r4]
	ldr r6, [sp, #68]
	movs r1, #1
	add r11, r1
	movs r5, #8
	adds r6, #1
	movs r0, #28
	mov r2, r11
	add r8, r5
	adds r7, #8
	str r6, [sp, #68]
	add r10, r0
	cmp r2, #2
	bne .L_081acb94
.L_081acc2c:
	ldr r0, [sp, #68]
	movs r6, #128
	movs r3, #0
	lsls r6, r6, #19
	mov r11, r3
	mov r9, r6
	lsls r3, r0, #3
	movs r1, #200
	ldr r6, [sp, #112]
	adds r1, r1, r3
	adds r3, #196
	add r4, sp, #212
	add r5, sp, #200
	add r7, sp, #248
	mov r10, r1
	mov r8, r3
.L_081acc4c:
	ldr r3, [r6]
	movs r2, #0
	str r3, [r4]
	str r2, [r4, #4]
	adds r0, r4, #0
	ldr r3, [r6, #8]
	adds r1, r5, #0
	str r3, [r4, #8]
	str r4, [sp, #8]
	bl Func_08015778
	ldr r3, [r5, #8]
	movs r0, #94
	adds r0, #255
	ldr r4, [sp, #8]
	cmp r3, r0
	bgt .L_081acc74
	movs r3, #175
	lsls r3, r3, #1
	str r3, [r5, #8]
.L_081acc74:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #138
	cmp r3, r2
	ble .L_081acc82
	str r2, [r5, #8]
	adds r3, r2, #0
.L_081acc82:
	ldr r1, .L_081ace5c
	adds r3, r3, r1
	str r3, [r7]
	movs r0, #6
	ldrsh r2, [r6, r0]
	movs r0, #16
	adds r3, r3, r2
	str r3, [r7]
	negs r0, r0
	ldr r1, [r5, #4]
	ldr r2, [r5]
	adds r3, r1, #0
	adds r3, #88
	cmp r3, r0
	ble .L_081accde
	movs r0, #156
	lsls r0, r0, #2
	adds r3, r2, r0
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	ands r3, r2
	movs r0, #172
	mov r2, r9
	lsls r0, r0, #1
	lsls r3, r3, #16
	orrs r3, r2
	adds r2, r1, r0
	movs r1, #255
	ands r2, r1
	orrs r3, r2
	ldr r2, .L_081ace60
	ldr r1, [sp, #108]
	orrs r3, r2
	mov r2, r8
	str r3, [r1, r2]
	movs r3, #137
	mov r0, r10
	lsls r3, r3, #4
	str r3, [r1, r0]
	ldr r2, [sp, #68]
	movs r1, #8
	adds r2, #1
	str r2, [sp, #68]
	add r10, r1
	add r8, r1
.L_081accde:
	movs r0, #1
	movs r3, #128
	add r11, r0
	lsls r3, r3, #18
	mov r1, r11
	add r9, r3
	adds r7, #4
	adds r6, #28
	cmp r1, #2
	bne .L_081acc4c
	ldr r2, [sp, #96]
	cmp r2, #1
	beq .L_081accfa
	b .L_081ad1ce
.L_081accfa:
	ldr r4, [sp, #44]
	movs r3, #0
	str r3, [sp, #60]
	str r4, [sp, #36]
	str r3, [sp, #32]
	ldr r7, [sp, #112]
	mov r11, r3
.L_081acd08:
	ldr r1, [r7]
	ldr r3, [r7, #12]
	ldr r2, [r7, #16]
	adds r1, r1, r3
	ldr r3, [r7, #4]
	str r1, [r7]
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r2, [r7, #20]
	ldr r3, [r7, #8]
	movs r6, #0
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r5, [sp, #112]
	mov r9, r5
.L_081acd26:
	cmp r11, r6
	beq .L_081acda0
	mov r0, r9
	ldr r3, [r0]
	ldr r2, [r7, #4]
	subs r3, r1, r3
	asrs r4, r3, #16
	ldr r3, [r0, #4]
	subs r2, r2, r3
	asrs r2, r2, #16
	mov r8, r2
	ldr r3, [r0, #8]
	ldr r2, [r7, #8]
	mov r5, r8
	subs r2, r2, r3
	asrs r2, r2, #16
	mov r10, r2
	mov r3, r8
	muls r3, r5
	adds r2, r4, #0
	muls r2, r4
	mov r0, r10
	adds r2, r2, r3
	mov r3, r10
	muls r3, r0
	adds r0, r2, r3
	movs r2, #144
	lsls r2, r2, #4
	adds r2, #195
	cmp r0, r2
	bgt .L_081acda0
	str r4, [sp, #8]
	ldr r3, .L_081ace6c
	mov lr, r3
	.2byte 0xf800
	ldr r4, [sp, #8]
	adds r5, r0, #0
	adds r1, r5, #0
	lsls r0, r4, #15
	bl Math_Div
	ldr r3, [r7, #12]
	adds r1, r5, #0
	adds r3, r3, r0
	str r3, [r7, #12]
	mov r3, r8
	lsls r0, r3, #15
	bl Math_Div
	ldr r3, [r7, #16]
	mov r4, r10
	adds r3, r3, r0
	str r3, [r7, #16]
	adds r1, r5, #0
	lsls r0, r4, #15
	bl Math_Div
	ldr r3, [r7, #20]
	ldr r1, [r7]
	adds r3, r3, r0
	str r3, [r7, #20]
.L_081acda0:
	movs r5, #28
	adds r6, #1
	add r9, r5
	cmp r6, #2
	bne .L_081acd26
	ldr r0, .L_081ace70
	cmp r1, r0
	bge .L_081acdc8
	ldr r2, [r7, #12]
	str r0, [r7]
	negs r2, r2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_081acdc2
	adds r3, #63
.L_081acdc2:
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r1, r0, #0
.L_081acdc8:
	movs r3, #200
	lsls r3, r3, #16
	cmp r1, r3
	ble .L_081acde6
	ldr r2, [r7, #12]
	str r3, [r7]
	negs r2, r2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_081acde2
	adds r3, #63
.L_081acde2:
	asrs r3, r3, #6
	str r3, [r7, #12]
.L_081acde6:
	ldr r2, [r7, #8]
	ldr r1, .L_081ace74
	cmp r2, r1
	bge .L_081ace06
	ldr r2, [r7, #20]
	str r1, [r7, #8]
	negs r2, r2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_081ace00
	adds r3, #63
.L_081ace00:
	asrs r3, r3, #6
	str r3, [r7, #20]
	adds r2, r1, #0
.L_081ace06:
	ldr r3, [r7, #4]
	movs r6, #180
	lsls r6, r6, #16
	adds r3, r3, r6
	cmp r2, r3
	ble .L_081ace38
	ldr r1, [r7, #20]
	str r3, [r7, #8]
	cmp r1, #0
	ble .L_081ace38
	ldr r2, [r7, #16]
	lsrs r3, r1, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	adds r2, r2, r3
	str r2, [r7, #16]
	negs r2, r1
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r2, r3, #4
	cmp r2, #0
	bge .L_081ace34
	adds r2, #63
.L_081ace34:
	asrs r3, r2, #6
	str r3, [r7, #20]
.L_081ace38:
	ldr r3, [r7, #16]
	ldr r0, .L_081ace78
	adds r2, r3, r0
	ldr r3, [r7, #4]
	str r2, [r7, #16]
	cmp r3, #0
	ble .L_081ace48
	b .L_081acf5c
.L_081ace48:
	negs r2, r2
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r1, #0
	lsls r2, r3, #4
	str r1, [r7, #4]
	cmp r2, #0
	bge .L_081ace7c
	adds r2, #63
	b .L_081ace7c
.L_081ace5c:
	.4byte 0xfffffea2
.L_081ace60:
	.4byte 0x40002300
.L_081ace64:
	.4byte Data_081ad374
.L_081ace68:
	.4byte 0x80002000
.L_081ace6c:
	.4byte IwramFillWords + 0x74
.L_081ace70:
	.4byte 0xff380000
.L_081ace74:
	.4byte 0xff6a0000
.L_081ace78:
	.4byte 0xffff8000
.L_081ace7c:
	asrs r3, r2, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #3
	ands r0, r3
	cmp r0, #0
	bne .L_081acede
	ldr r2, [r7, #12]
	ldr r3, [r7, #20]
	asrs r2, r2, #8
	asrs r3, r3, #8
	adds r0, r2, #0
	muls r0, r2
	adds r2, r3, #0
	muls r2, r3
	adds r3, r2, #0
	adds r0, r0, r3
	ldr r3, .L_081ad0bc
	mov lr, r3
	.2byte 0xf800
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_081aceb2
	adds r3, #63
.L_081aceb2:
	asrs r6, r3, #6
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #8
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #8
	b .L_081acf00
.L_081acede:
	ldr r2, [r7, #12]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_081aceec
	adds r3, #63
.L_081aceec:
	ldr r2, [r7, #20]
	asrs r3, r3, #6
	str r3, [r7, #12]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_081acefe
	adds r3, #63
.L_081acefe:
	asrs r3, r3, #6
.L_081acf00:
	str r3, [r7, #20]
	ldr r5, [sp, #36]
	ldr r4, [sp, #32]
	ldr r3, [r5]
	cmp r3, #19
	ble .L_081acf10
	subs r3, #20
	str r3, [r5]
.L_081acf10:
	ldr r3, [r7, #16]
	ldr r6, .L_081ad0c0
	cmp r3, r6
	bgt .L_081acf60
	movs r0, #0
	str r0, [r7, #12]
	str r0, [r7, #16]
	str r0, [r7, #20]
	ldr r1, [sp, #36]
	str r0, [r1]
	ldr r5, [r7, #24]
	adds r0, r5, #0
	cmp r5, #0
	bge .L_081acf34
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #255
	adds r0, r5, r2
.L_081acf34:
	asrs r0, r0, #10
	movs r1, #3
	str r4, [sp, #8]
	bl __modsi3
	ldr r4, [sp, #8]
	cmp r0, #1
	bne .L_081acf4e
	movs r6, #128
	lsls r6, r6, #4
	adds r3, r5, r6
	str r3, [r7, #24]
	b .L_081acf60
.L_081acf4e:
	cmp r0, #2
	bne .L_081acf60
	movs r0, #128
	lsls r0, r0, #3
	adds r3, r5, r0
	str r3, [r7, #24]
	b .L_081acf60
.L_081acf5c:
	mov r1, r11
	lsls r4, r1, #2
.L_081acf60:
	ldr r3, [r7, #12]
	lsls r2, r3, #6
	subs r2, r2, r3
	cmp r2, #0
	bge .L_081acf6c
	adds r2, #63
.L_081acf6c:
	asrs r3, r2, #6
	str r3, [r7, #12]
	ldr r3, [r7, #16]
	lsls r2, r3, #6
	subs r2, r2, r3
	cmp r2, #0
	bge .L_081acf7c
	adds r2, #63
.L_081acf7c:
	asrs r3, r2, #6
	str r3, [r7, #16]
	ldr r3, [r7, #20]
	lsls r2, r3, #6
	subs r2, r2, r3
	cmp r2, #0
	bge .L_081acf8c
	adds r2, #63
.L_081acf8c:
	asrs r3, r2, #6
	str r3, [r7, #20]
	ldr r5, [sp, #32]
	ldr r6, [sp, #44]
	ldr r2, [r7, #24]
	ldr r3, [r5, r6]
	movs r0, #142
	lsls r0, r0, #7
	adds r2, r2, r3
	adds r0, #255
	str r2, [r7, #24]
	cmp r2, r0
	ble .L_081acfac
	ldr r1, .L_081ad0c4
	adds r3, r2, r1
	str r3, [r7, #24]
.L_081acfac:
	ldr r2, [sp, #36]
	ldr r3, [r2]
	cmp r3, #0
	ble .L_081acfb8
	subs r3, #1
	str r3, [r2]
.L_081acfb8:
	ldr r3, [r7, #12]
	cmp r3, #0
	bne .L_081ad016
	ldr r3, [r7, #16]
	cmp r3, #0
	bne .L_081ad016
	ldr r3, [r7, #20]
	cmp r3, #0
	bne .L_081ad016
	ldr r3, [r7, #4]
	cmp r3, #0
	bne .L_081ad016
	movs r1, #192
	ldr r0, [r7, #24]
	lsls r1, r1, #4
	add r5, sp, #224
	str r4, [sp, #8]
	bl Math_Div
	ldr r1, .L_081ad0c8
	ldr r4, [sp, #8]
	movs r6, #0
	str r0, [r5, r4]
	ldrsh r3, [r1, r6]
	ldr r2, [r7, #8]
	lsls r3, r3, #16
	cmp r2, r3
	ble .L_081acff8
	adds r3, r4, #0
	adds r3, #8
	str r6, [r5, r3]
	b .L_081ad010
.L_081acff8:
	adds r6, #1
	cmp r6, #6
	beq .L_081ad010
	lsls r3, r6, #1
	ldrsh r3, [r1, r3]
	lsls r3, r3, #16
	cmp r2, r3
	ble .L_081acff8
	ldr r3, [sp, #32]
	add r2, sp, #224
	adds r3, #8
	str r6, [r2, r3]
.L_081ad010:
	ldr r5, [sp, #60]
	adds r5, #1
	str r5, [sp, #60]
.L_081ad016:
	ldr r6, [sp, #36]
	ldr r0, [sp, #32]
	movs r1, #1
	add r11, r1
	adds r6, #4
	adds r0, #4
	mov r2, r11
	str r6, [sp, #36]
	str r0, [sp, #32]
	adds r7, #28
	cmp r2, #2
	beq .L_081ad030
	b .L_081acd08
.L_081ad030:
	ldr r3, [sp, #60]
	cmp r3, #2
	beq .L_081ad038
	b .L_081ad1ce
.L_081ad038:
	movs r6, #1
	movs r5, #0
	movs r4, #2
	negs r6, r6
	movs r7, #60
	str r4, [sp, #96]
	str r5, [sp, #88]
	str r6, [sp, #84]
	str r5, [sp, #80]
	str r7, [sp, #76]
	add r1, sp, #224
	ldr r2, [r1]
	ldr r3, [r1, #4]
	cmp r2, r3
	bne .L_081ad0d0
	ldr r3, [r1, #8]
	cmp r2, r3
	bne .L_081ad0d0
	ldr r3, [r1, #12]
	cmp r2, r3
	bne .L_081ad0d0
	ldr r0, [sp, #56]
	movs r3, #120
	str r3, [r0, #4]
	str r3, [sp, #144]
	str r5, [sp, #84]
	ldr r3, [sp, #136]
	ldr r2, [r1]
	cmp r2, r3
	bne .L_081ad07c
	ldr r1, [sp, #92]
	lsls r3, r1, #4
	subs r3, r3, r1
	b .L_081ad096
.L_081ad07c:
	ldr r4, [sp, #48]
	ldr r3, [r4, #4]
	cmp r2, r3
	bne .L_081ad090
	ldr r5, [sp, #92]
	lsls r3, r5, #1
	adds r3, r3, r5
	lsls r3, r3, #3
	adds r3, r3, r5
	b .L_081ad096
.L_081ad090:
	ldr r6, [sp, #92]
	lsls r3, r6, #2
	adds r3, r3, r6
.L_081ad096:
	str r3, [sp, #80]
	ldr r3, .L_081ad0cc
	ldr r7, [sp, #108]
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #196
	ldr r0, [r3, #16]
	adds r3, r7, r1
	ldr r2, [r3]
	movs r1, #6
	movs r3, #0
	str r3, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	movs r0, #93
	bl Audio_PlayCue
	b .L_081ad1ce
	.2byte 0x0000
.L_081ad0bc:
	.4byte IwramFillWords + 0x74
.L_081ad0c0:
	.4byte 0x0002ffff
.L_081ad0c4:
	.4byte 0xffffb800
.L_081ad0c8:
	.4byte Data_081ad386
.L_081ad0cc:
	.4byte gPartyState
.L_081ad0d0:
	ldr r5, .L_081ad264
	movs r4, #0
	add r6, sp, #768
	mov r8, r4
	mov r11, r4
	mov lr, r5
	adds r7, r6, #0
.L_081ad0de:
	mov r5, r11
	adds r5, #1
	adds r6, r5, #0
	cmp r5, #4
	beq .L_081ad118
	mov r1, r11
	add r0, sp, #768
	lsls r2, r1, #2
	ldr r1, .L_081ad268
	add r0, lr
	add r4, sp, #768
	lsls r3, r5, #2
	mov r12, r0
	adds r3, r3, r4
	mov r0, lr
	adds r4, r3, r0
	adds r0, r7, r1
.L_081ad100:
	mov r3, r12
	ldr r1, [r3, r2]
	ldmia r4!, {r3}
	cmp r1, r3
	bne .L_081ad112
	stmia r0!, {r1}
	movs r1, #1
	adds r7, #4
	add r8, r1
.L_081ad112:
	adds r6, #1
	cmp r6, #4
	bne .L_081ad100
.L_081ad118:
	mov r11, r5
	cmp r5, #3
	bne .L_081ad0de
	mov r2, r8
	cmp r2, #1
	bne .L_081ad148
	ldr r3, [sp, #92]
	ldr r4, [sp, #108]
	str r3, [sp, #80]
	movs r5, #128
	ldr r3, .L_081ad26c
	lsls r5, r5, #3
	adds r5, #196
	ldr r0, [r3, #16]
	adds r3, r4, r5
	ldr r2, [r3]
	movs r6, #0
	movs r1, #6
	movs r3, #0
	str r6, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	movs r7, #3
	str r7, [sp, #84]
.L_081ad148:
	mov r0, r8
	cmp r0, #2
	bne .L_081ad18a
	ldr r1, [sp, #92]
	ldr r2, [sp, #108]
	lsls r1, r1, #1
	str r1, [sp, #80]
	ldr r3, .L_081ad26c
	movs r4, #128
	lsls r4, r4, #3
	adds r4, #196
	ldr r0, [r3, #16]
	adds r3, r2, r4
	ldr r2, [r3]
	movs r1, #6
	movs r3, #0
	movs r5, #0
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	bl Random16
	movs r3, #1
	ldr r6, [sp, #56]
	ands r3, r0
	lsls r3, r3, #2
	movs r2, #60
	str r2, [r3, r6]
	movs r7, #2
	movs r0, #91
	str r7, [sp, #84]
	bl Audio_PlayCue
.L_081ad18a:
	mov r0, r8
	cmp r0, #3
	bne .L_081ad1ce
	ldr r1, [sp, #92]
	ldr r2, [sp, #108]
	lsls r3, r1, #1
	adds r3, r3, r1
	str r3, [sp, #80]
	movs r4, #128
	ldr r3, .L_081ad26c
	lsls r4, r4, #3
	adds r4, #196
	ldr r0, [r3, #16]
	adds r3, r2, r4
	ldr r2, [r3]
	movs r1, #6
	movs r3, #0
	movs r5, #0
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	bl Random16
	movs r3, #1
	ldr r6, [sp, #56]
	ands r3, r0
	lsls r3, r3, #2
	movs r2, #60
	str r2, [r3, r6]
	movs r7, #1
	movs r0, #92
	str r7, [sp, #84]
	bl Audio_PlayCue
.L_081ad1ce:
	ldr r0, [sp, #68]
	cmp r0, #128
	beq .L_081ad1f6
	ldr r2, [sp, #68]
	ldr r0, .L_081ad270
	lsls r3, r2, #3
	adds r2, r3, #0
	movs r1, #0
	adds r2, #200
	adds r3, #196
.L_081ad1e2:
	ldr r4, [sp, #108]
	str r0, [r4, r3]
	str r1, [r4, r2]
	ldr r5, [sp, #68]
	adds r2, #8
	adds r5, #1
	adds r3, #8
	str r5, [sp, #68]
	cmp r5, #128
	bne .L_081ad1e2
.L_081ad1f6:
	movs r1, #224
	mov r8, r1
	mov r1, sp
	adds r1, #240
	str r1, [sp, #4]
	movs r6, #0
	mov r7, sp
	mov r4, sp
	movs r2, #216
	movs r3, #208
	mov r11, r6
	adds r7, #166
	add r5, sp, #164
	adds r4, #162
	add r0, sp, #160
	mov lr, r2
	mov r12, r3
	movs r6, #200
.L_081ad21a:
	ldr r1, [sp, #4]
	ldmia r1!, {r3}
	adds r2, r1, #0
	str r2, [sp, #4]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	ldr r2, .L_081ad260
	ldr r1, .L_081ad260
	adds r3, #128
	lsls r3, r3, #16
	asrs r3, r3, #16
	strh r3, [r7]
	strh r2, [r5]
	strh r1, [r4]
	strh r3, [r0]
	ldrh r2, [r7]
	lsls r2, r2, #16
	mov r10, r2
	ldr r2, [sp, #108]
	mov r1, r10
	ldr r3, [r2, r6]
	orrs r3, r1
	str r3, [r2, r6]
	ldrh r2, [r5]
	mov r1, r12
	lsls r2, r2, #16
	mov r10, r2
	ldr r2, [sp, #108]
	adds r6, #32
	ldr r3, [r2, r1]
	mov r2, r10
	orrs r3, r2
	ldr r2, [sp, #108]
	b .L_081ad274
.L_081ad260:
	.4byte 0x00000000
.L_081ad264:
	.4byte 0xfffffde0
.L_081ad268:
	.4byte 0xfffffd78
.L_081ad26c:
	.4byte gPartyState
.L_081ad270:
	.4byte 0x40f02000
.L_081ad274:
	str r3, [r2, r1]
	ldrh r2, [r4]
	ldr r1, [sp, #108]
	lsls r2, r2, #16
	mov r10, r2
	mov r2, lr
	ldr r3, [r1, r2]
	mov r1, r10
	orrs r3, r1
	ldr r1, [sp, #108]
	str r3, [r1, r2]
	ldrh r2, [r0]
	lsls r2, r2, #16
	mov r10, r2
	mov r2, r8
	ldr r3, [r1, r2]
	mov r1, r10
	orrs r3, r1
	ldr r1, [sp, #108]
	str r3, [r1, r2]
	movs r3, #1
	add r11, r3
	movs r2, #32
	mov r1, r11
	add r8, r2
	add lr, r2
	add r12, r2
	cmp r1, #4
	bne .L_081ad21a
	ldr r0, [sp, #108]
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #224
	adds r3, #212
	adds r0, #196
	lsls r1, r1, #19
	ldr r2, .L_081ad340
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #100]
	adds r2, #1
	str r2, [sp, #100]
	bl .L_081ac4a4
.L_081ad2d2:
	movs r3, #0
	movs r6, #128
	mov r11, r3
	lsls r6, r6, #1
.L_081ad2da:
	mov r4, r11
	movs r5, #128
	lsls r3, r4, #12
	lsls r5, r5, #9
	subs r5, r5, r3
	ldr r0, [sp, #116]
	adds r2, r5, #0
	ldr r1, .L_081ad344
	adds r3, r6, #0
	bl Graphics_ScaleRgb555Buffer
	adds r2, r5, #0
	movs r1, #160
	movs r5, #1
	add r0, sp, #256
	lsls r1, r1, #19
	adds r3, r6, #0
	add r11, r5
	bl Graphics_ScaleRgb555Buffer
	mov r7, r11
	movs r0, #1
	bl WaitFrames
	cmp r7, #17
	bne .L_081ad2da
	movs r0, #48
	bl Runtime_ReleaseHeapBlock
	movs r0, #180
	bl Runtime_ReleaseHeapBlock
	movs r0, #96
	bl Runtime_ReleaseHeapBlock
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	movs r0, #100
	bl Runtime_ReleaseHeapBlock
	movs r3, #192
	lsls r3, r3, #2
	add sp, r3
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081ad340:
	.4byte 0x84000100
.L_081ad344:
	.4byte 0x05000200
