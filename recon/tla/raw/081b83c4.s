.syntax unified
	.thumb
	.global Func_081b83c4
	.thumb_func
Func_081b83c4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_081b860c
	movs r1, #128
	lsls r1, r1, #2
	movs r0, #96
	add sp, r5
	bl Runtime_AllocateHeapBlock
	movs r1, #246
	lsls r1, r1, #7
	str r0, [sp, #128]
	adds r1, #124
	movs r0, #92
	bl Runtime_AllocateBlock
	movs r1, #192
	lsls r1, r1, #3
	str r0, [sp, #124]
	adds r1, #20
	movs r0, #180
	bl Runtime_AllocateBlock
	movs r1, #76
	str r0, [sp, #120]
	movs r0, #48
	bl Runtime_AllocateBlock
	str r0, [sp, #116]
	ldr r0, .L_081b8610
	bl Func_080132fc
	bl Func_081b8020
	bl Scheduler_ResetTaskTable
	ldr r2, .L_081b8614
	movs r3, #0
	strb r3, [r2]
	ldr r6, .L_081b8618
	movs r0, #0
	mov r11, r0
	movs r5, #0
	movs r4, #0
.L_081b8426:
	movs r1, #0
	adds r0, r4, #0
.L_081b842a:
	mov r3, r11
	adds r2, r3, r6
	adds r3, r1, r0
	strh r3, [r2]
	adds r1, #1
	movs r2, #2
	add r11, r2
	cmp r1, #32
	bne .L_081b842a
	adds r5, #1
	adds r4, #30
	cmp r5, #20
	bne .L_081b8426
	ldr r0, .L_081b861c
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	str r0, [sp, #112]
	lsls r3, r3, #19
	movs r0, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r0, r0, #19
	add r1, sp, #244
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, [sp, #112]
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [sp, #112]
	movs r4, #224
	lsls r4, r4, #1
	adds r3, r3, r4
	adds r0, r3, #0
	ldr r1, .L_081b8620
	str r3, [sp, #112]
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081b8620
	ldr r1, .L_081b8624
	ldr r2, .L_081b8628
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081b862c
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r0, [sp, #112]
	adds r3, #212
	ldr r1, [sp, #128]
	adds r2, #48
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, [sp, #112]
	ldr r1, .L_081b8620
	adds r5, #128
	adds r0, r5, #0
	str r5, [sp, #112]
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081b8620
	ldr r1, .L_081b8630
	ldr r2, .L_081b8634
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081b8638
	bl Resource_GetTableEntry
	ldr r1, [sp, #128]
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r0, [sp, #112]
	adds r3, #212
	adds r1, #224
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r6, [sp, #112]
	ldr r1, .L_081b8620
	adds r6, #32
	adds r0, r6, #0
	str r6, [sp, #112]
	bl Resource_DecodeType01
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_081b8620
	ldr r1, .L_081b863c
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081b8640
	bl Resource_GetTableEntry
	ldr r1, [sp, #128]
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r0, [sp, #112]
	adds r3, #212
	adds r1, #192
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, [sp, #112]
	ldr r1, .L_081b8620
	adds r0, #32
	str r0, [sp, #112]
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081b8620
	ldr r1, .L_081b8644
	ldr r2, .L_081b8648
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081b864c
	bl Resource_GetTableEntry
	ldr r2, [sp, #128]
	movs r4, #128
	lsls r4, r4, #1
	adds r1, r2, r4
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r0, [sp, #112]
	adds r3, #212
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, [sp, #112]
	ldr r1, .L_081b8620
	adds r5, #32
	adds r0, r5, #0
	str r5, [sp, #112]
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081b8620
	ldr r1, .L_081b8650
	ldr r2, .L_081b8654
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081b8658
	bl Resource_GetTableEntry
	ldr r6, [sp, #128]
	movs r2, #144
	lsls r2, r2, #1
	adds r1, r6, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r0, [sp, #112]
	adds r3, #212
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [sp, #112]
	ldr r1, .L_081b8620
	adds r3, #32
	adds r0, r3, #0
	str r3, [sp, #112]
	bl Resource_DecodeType01
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_081b8620
	ldr r1, .L_081b865c
	adds r2, #96
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081b8660
	bl Resource_GetTableEntry
	movs r3, #128
	movs r4, #160
	movs r2, #132
	lsls r4, r4, #1
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r0, [sp, #112]
	adds r3, #212
	adds r1, r6, r4
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r5, #176
	lsls r5, r5, #1
	adds r4, r6, r5
	movs r6, #0
	mov r11, r6
	ldr r0, [sp, #112]
	ldr r6, .L_081b8608
	movs r5, #31
.L_081b85ec:
	ldrh r3, [r0]
	lsrs r2, r3, #10
	ands r2, r6
	adds r1, r2, #4
	adds r2, r5, #0
	ands r2, r3
	adds r2, #4
	cmp r2, #31
	ble .L_081b8600
	movs r2, #31
.L_081b8600:
	cmp r1, #31
	ble .L_081b8664
	movs r1, #31
	b .L_081b8664
.L_081b8608:
	.4byte 0x0000001f
.L_081b860c:
	.4byte 0xfffffd0c
.L_081b8610:
	.4byte 0x0000000c
.L_081b8614:
	.4byte Data_0300120c
.L_081b8618:
	.4byte 0x06003000
.L_081b861c:
	.4byte 0x00000085
.L_081b8620:
	.4byte gMapCellBuffer
.L_081b8624:
	.4byte 0x06004000
.L_081b8628:
	.4byte 0x84002580
.L_081b862c:
	.4byte 0x00000087
.L_081b8630:
	.4byte 0x06010000
.L_081b8634:
	.4byte 0x840004c0
.L_081b8638:
	.4byte 0x00000092
.L_081b863c:
	.4byte 0x06011300
.L_081b8640:
	.4byte 0x00000097
.L_081b8644:
	.4byte 0x06011500
.L_081b8648:
	.4byte 0x84000100
.L_081b864c:
	.4byte 0x0000008f
.L_081b8650:
	.4byte 0x06011900
.L_081b8654:
	.4byte 0x84000280
.L_081b8658:
	.4byte 0x00000091
.L_081b865c:
	.4byte 0x06012300
.L_081b8660:
	.4byte 0x0000008e
.L_081b8664:
	lsls r3, r2, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r1
	movs r1, #1
	add r11, r1
	mov r2, r11
	strh r3, [r4]
	adds r0, #2
	adds r4, #2
	cmp r2, #16
	bne .L_081b85ec
	ldr r4, [sp, #128]
	movs r3, #128
	movs r5, #176
	movs r2, #132
	lsls r5, r5, #1
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, r5
	ldr r1, .L_081b871c
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r6, [sp, #112]
	ldr r1, .L_081b8720
	adds r6, #32
	adds r0, r6, #0
	str r6, [sp, #112]
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081b8720
	ldr r1, .L_081b8724
	ldr r2, .L_081b8728
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	subs r5, #96
	ldr r1, .L_081b872c
	ldr r0, [sp, #128]
	movs r2, #0
	adds r3, r5, #0
	bl Graphics_ScaleRgb555BufferB
	movs r1, #160
	lsls r1, r1, #19
	movs r2, #0
	adds r3, r5, #0
	add r0, sp, #244
	bl Graphics_ScaleRgb555BufferB
	ldr r3, .L_081b8704
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	strh r3, [r2]
	ldr r3, .L_081b8708
	adds r2, #62
	strh r3, [r2]
	ldr r3, .L_081b870c
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_081b8710
	subs r2, #74
	strh r3, [r2]
	ldr r3, .L_081b8714
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_081b8718
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_081b8730
	movs r2, #0
	strh r2, [r3, #4]
	strh r2, [r3, #6]
	b .L_081b8734
	.2byte 0x0000
.L_081b8704:
	.4byte 0x00000686
.L_081b8708:
	.4byte 0x00003737
.L_081b870c:
	.4byte 0x00002723
.L_081b8710:
	.4byte 0x00003340
.L_081b8714:
	.4byte 0x00003f44
.L_081b8718:
	.4byte 0x00000810
.L_081b871c:
	.4byte 0x05000360
.L_081b8720:
	.4byte gMapCellBuffer
.L_081b8724:
	.4byte 0x06016600
.L_081b8728:
	.4byte 0x84000140
.L_081b872c:
	.4byte 0x05000200
.L_081b8730:
	.4byte Data_03001120
.L_081b8734:
	movs r3, #128
	ldr r1, .L_081b8770
	lsls r3, r3, #19
	adds r3, #20
	strh r2, [r3]
	adds r3, #2
	strh r1, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #2
	strh r1, [r3]
	ldr r1, .L_081b8774
	ldr r2, .L_081b8778
	adds r3, #38
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	subs r3, #2
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	movs r3, #1
	movs r0, #0
	negs r3, r3
	movs r1, #14
	movs r2, #1
	str r0, [sp, #104]
	str r0, [sp, #100]
	str r0, [sp, #96]
	b .L_081b877c
.L_081b8770:
	.4byte 0x0000ff60
.L_081b8774:
	.4byte 0x000000f0
.L_081b8778:
	.4byte 0x000000a0
.L_081b877c:
	str r0, [sp, #88]
	str r0, [sp, #84]
	str r0, [sp, #80]
	str r0, [sp, #76]
	str r0, [sp, #72]
	str r0, [sp, #68]
	str r1, [sp, #64]
	str r2, [sp, #60]
	str r3, [sp, #56]
	str r3, [sp, #52]
	ldr r6, [sp, #124]
	movs r4, #100
	mov r11, r0
	movs r7, #127
	mov r8, r0
	mov r10, r4
.L_081b879c:
	bl Random16
	ands r0, r7
	subs r0, #64
	lsls r0, r0, #16
	mov r5, r8
	str r5, [r6, #4]
	str r0, [r6]
	bl Random16
	ands r0, r7
	subs r0, #64
	lsls r0, r0, #16
	str r0, [r6, #8]
	mov r0, r11
	lsls r5, r0, #14
	adds r0, r5, #0
	bl Trig_Sin
	mov r3, r10
	muls r3, r0
	mov r1, r8
	str r1, [r6, #4]
	str r3, [r6]
	adds r0, r5, #0
	bl Trig_Cos
	mov r3, r10
	muls r3, r0
	str r3, [r6, #8]
	bl Random16
	ands r0, r7
	subs r0, #64
	lsls r0, r0, #14
	str r0, [r6, #12]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #32
	lsls r3, r3, #12
	str r3, [r6, #16]
	bl Random16
	ands r0, r7
	subs r0, #64
	lsls r0, r0, #14
	str r0, [r6, #20]
	bl Random16
	movs r1, #6
	bl Math_ModU
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #10
	str r3, [r6, #24]
	mov r3, r11
	lsls r2, r3, #2
	movs r5, #1
	movs r3, #150
	lsls r3, r3, #1
	add r1, sp, #228
	add r11, r5
	str r3, [r1, r2]
	mov r4, r8
	add r3, sp, #180
	mov r0, r11
	str r4, [r3, r2]
	adds r6, #28
	cmp r0, #4
	bne .L_081b879c
	bl UiWork_InitializeWithResourceCountersFar
	movs r1, #6
	str r1, [sp, #0]
	mov r8, r1
	movs r2, #12
	movs r1, #0
	movs r3, #3
	movs r0, #18
	bl UiWindow_CreateFar
	movs r3, #128
	ldr r2, [sp, #120]
	ldr r5, .L_081b8bec
	lsls r3, r3, #3
	adds r3, #196
	adds r6, r2, r3
	adds r1, r0, #0
	str r1, [r6]
	adds r0, r5, #0
	movs r2, #48
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r4, .L_081b8bf0
	ldr r2, [r6]
	ldr r0, [r4, #16]
	movs r3, #0
	movs r1, #6
	str r3, [sp, #0]
	mov r10, r4
	bl UiText_DrawNumberInWindowFar
	mov r6, r8
	movs r1, #3
	movs r2, #12
	movs r3, #3
	movs r0, #18
	str r6, [sp, #0]
	bl UiWindow_CreateFar
	adds r1, r0, #0
	ldr r0, [sp, #120]
	movs r2, #153
	lsls r2, r2, #3
	adds r7, r0, r2
	subs r5, #1
	adds r0, r5, #0
	str r1, [r7]
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r1, #16
	movs r2, #8
	movs r3, #4
	movs r0, #22
	str r6, [sp, #0]
	bl UiWindow_CreateFar
	movs r4, #128
	ldr r3, [sp, #120]
	ldr r5, .L_081b8bf4
	lsls r4, r4, #3
	adds r4, #204
	adds r6, r3, r4
	adds r1, r0, #0
	str r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	adds r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	ldr r1, [r6]
	adds r0, r5, #0
	movs r3, #8
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	bl Func_080ad290
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	str r3, [sp, #92]
	mov r5, r10
	ldr r3, [r5, #16]
	ldr r6, [sp, #92]
	cmp r6, r3
	bls .L_081b88e6
	str r3, [sp, #92]
.L_081b88e6:
	movs r3, #5
	ldr r2, [r7]
	ldr r0, [sp, #92]
	str r3, [sp, #0]
	movs r1, #6
	movs r3, #24
	bl UiText_DrawNumberInWindowFar
	movs r0, #1
	bl Audio_PlayCue
	movs r0, #0
	str r0, [sp, #108]
	ldr r1, [sp, #116]
	adds r1, #12
	str r1, [sp, #36]
	bl .L_081ba210
.L_081b890a:
	ldr r1, .L_081b8bf8
	movs r2, #2
	ldr r3, [r1, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_081b8932
	movs r0, #113
	bl Audio_PlayCue
	movs r4, #128
	ldr r2, [sp, #120]
	lsls r4, r4, #3
	adds r4, #204
	adds r3, r2, r4
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
	bl .L_081ba282
.L_081b8932:
	ldr r3, [r1, #12]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_081b8940
	bl .L_081b9550
.L_081b8940:
	movs r5, #151
	lsls r5, r5, #1
	adds r0, r5, #0
	bl Audio_PlayCue
	movs r0, #128
	ldr r6, [sp, #120]
	lsls r0, r0, #3
	adds r0, #204
	adds r3, r6, r0
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
	adds r0, r5, #0
	bl Audio_PlayCue
	ldr r6, .L_081b8bfc
	ldr r5, [sp, #124]
	movs r1, #0
	mov r11, r1
	movs r7, #127
.L_081b896c:
	movs r3, #192
	movs r2, #0
	lsls r3, r3, #15
	str r2, [r5, #8]
	str r3, [r5, #4]
	str r6, [r5]
	mov r8, r2
	bl Random16
	ands r0, r7
	subs r0, #64
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #48
	lsls r3, r3, #13
	str r3, [r5, #16]
	bl Random16
	ands r0, r7
	adds r0, #64
	lsls r0, r0, #13
	str r0, [r5, #20]
	bl Random16
	movs r1, #144
	lsls r1, r1, #7
	bl Math_ModU
	mov r3, r11
	str r0, [r5, #24]
	lsls r2, r3, #2
	movs r0, #1
	movs r3, #150
	add r1, sp, #228
	lsls r3, r3, #1
	movs r4, #192
	add r11, r0
	str r3, [r1, r2]
	lsls r4, r4, #13
	mov r1, r11
	adds r6, r6, r4
	adds r5, #28
	cmp r1, #4
	bne .L_081b896c
	bl Func_080ad290
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r7, .L_081b8bf0
	lsls r3, r3, #1
	str r3, [sp, #92]
	ldr r3, [r7, #16]
	ldr r2, [sp, #92]
	cmp r2, r3
	bls .L_081b89e4
	str r3, [sp, #92]
.L_081b89e4:
	ldr r3, [sp, #120]
	movs r4, #153
	lsls r4, r4, #3
	adds r6, r3, r4
	ldr r0, [sp, #92]
	ldr r2, [r6]
	movs r3, #24
	movs r5, #5
	movs r1, #6
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	ldr r1, [sp, #92]
	negs r0, r1
	bl Party_AdjustSixDigitCounterAFar
	movs r4, #128
	ldr r2, [sp, #120]
	lsls r4, r4, #3
	adds r4, #196
	adds r3, r2, r4
	mov r1, r8
	ldr r2, [r3]
	ldr r0, [r7, #16]
	movs r3, #0
	str r1, [sp, #0]
	movs r1, #6
	bl UiText_DrawNumberInWindowFar
	movs r3, #0
	movs r2, #2
	str r2, [sp, #96]
	str r3, [sp, #84]
	str r3, [sp, #80]
	bl Func_080ad290
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	str r3, [sp, #92]
	adds r0, r3, #0
	ldr r2, [r6]
	movs r1, #6
	movs r3, #24
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	bl .L_081b9550
.L_081b8a46:
	ldr r4, [sp, #96]
	cmp r4, #1
	beq .L_081b8a4e
	b .L_081b8d0e
.L_081b8a4e:
	ldr r5, [sp, #80]
	cmp r5, #7
	bgt .L_081b8a5a
	adds r5, #1
	str r5, [sp, #80]
	b .L_081b8c70
.L_081b8a5a:
	ldr r6, [sp, #84]
	cmp r6, #0
	ble .L_081b8ae0
	adds r6, #1
	str r6, [sp, #84]
	cmp r6, #16
	beq .L_081b8a6a
	b .L_081b8c70
.L_081b8a6a:
	movs r0, #151
	lsls r0, r0, #1
	bl Audio_PlayCue
	ldr r6, .L_081b8bfc
	ldr r5, [sp, #124]
	movs r0, #0
	mov r11, r0
	movs r7, #127
.L_081b8a7c:
	movs r3, #0
	str r3, [r5, #8]
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r5, #4]
	str r6, [r5]
	bl Random16
	ands r0, r7
	subs r0, #64
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #48
	lsls r3, r3, #13
	str r3, [r5, #16]
	bl Random16
	ands r0, r7
	adds r0, #64
	lsls r0, r0, #13
	str r0, [r5, #20]
	bl Random16
	movs r1, #144
	lsls r1, r1, #7
	bl Math_ModU
	mov r3, r11
	str r0, [r5, #24]
	lsls r2, r3, #2
	movs r0, #1
	movs r3, #150
	add r1, sp, #228
	lsls r3, r3, #1
	movs r4, #192
	add r11, r0
	str r3, [r1, r2]
	lsls r4, r4, #13
	mov r1, r11
	adds r6, r6, r4
	adds r5, #28
	cmp r1, #4
	bne .L_081b8a7c
	movs r2, #2
	str r2, [sp, #96]
	b .L_081b8c70
.L_081b8ae0:
	ldr r1, .L_081b8bf8
	movs r2, #2
	ldr r3, [r1, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_081b8b46
	ldr r3, [sp, #88]
	cmp r3, #0
	ble .L_081b8afa
	movs r0, #114
	bl Audio_PlayCue
	b .L_081b8c70
.L_081b8afa:
	ldr r4, [sp, #120]
	movs r5, #128
	lsls r5, r5, #3
	adds r5, #204
	adds r6, r4, r5
	ldr r0, [r6]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #8
	movs r3, #4
	movs r0, #22
	bl UiWindow_CreateFar
	ldr r5, .L_081b8bf4
	adds r1, r0, #0
	str r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	adds r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	ldr r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #113
	bl Audio_PlayCue
	movs r6, #0
	str r6, [sp, #96]
	b .L_081b8c70
.L_081b8b46:
	ldr r3, [r1, #12]
	ldr r0, [sp, #96]
	ands r3, r0
	cmp r3, #0
	beq .L_081b8bc6
	movs r0, #112
	bl Audio_PlayCue
	movs r1, #1
	ldr r2, [sp, #120]
	movs r4, #128
	str r1, [sp, #84]
	lsls r4, r4, #3
	adds r4, #204
	adds r3, r2, r4
	ldr r0, [r3]
	bl UiWork_FinalizeFar
	ldr r5, [sp, #88]
	cmp r5, #0
	beq .L_081b8b72
	b .L_081b8c70
.L_081b8b72:
	bl Func_080ad290
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	str r3, [sp, #92]
	ldr r5, .L_081b8bf0
	ldr r6, [sp, #92]
	ldr r3, [r5, #16]
	cmp r6, r3
	bls .L_081b8b8a
	str r3, [sp, #92]
.L_081b8b8a:
	ldr r0, [sp, #120]
	movs r1, #153
	lsls r1, r1, #3
	adds r3, r0, r1
	ldr r2, [r3]
	movs r3, #5
	str r3, [sp, #0]
	ldr r0, [sp, #92]
	movs r1, #6
	movs r3, #24
	bl UiText_DrawNumberInWindowFar
	ldr r2, [sp, #92]
	negs r0, r2
	bl Party_AdjustSixDigitCounterAFar
	ldr r0, [r5, #16]
	ldr r4, [sp, #120]
	movs r5, #128
	lsls r5, r5, #3
	adds r5, #196
	ldr r6, [sp, #88]
	adds r3, r4, r5
	ldr r2, [r3]
	movs r1, #6
	movs r3, #0
	str r6, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	b .L_081b8c70
.L_081b8bc6:
	ldr r3, [r1, #12]
	movs r2, #64
	movs r6, #1
	ands r3, r2
	negs r6, r6
	cmp r3, #0
	beq .L_081b8c12
	ldr r0, [sp, #72]
	cmp r0, #1
	bne .L_081b8c12
	ldr r1, [sp, #64]
	cmp r1, #24
	beq .L_081b8c28
	ldr r2, [sp, #88]
	cmp r2, #0
	bne .L_081b8c04
	ldr r6, .L_081b8c00
	b .L_081b8c06
	.2byte 0x0000
.L_081b8bec:
	.4byte 0x00000d69
.L_081b8bf0:
	.4byte gPartyState
.L_081b8bf4:
	.4byte 0x00000d77
.L_081b8bf8:
	.4byte gInput
.L_081b8bfc:
	.4byte 0xff940000
.L_081b8c00:
	.4byte 0x00000d79
.L_081b8c04:
	ldr r6, .L_081b8f10
.L_081b8c06:
	movs r0, #111
	bl Audio_PlayCue
	movs r3, #0
	str r3, [sp, #72]
	b .L_081b8c46
.L_081b8c12:
	ldr r3, [r1, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_081b8c46
	ldr r4, [sp, #72]
	cmp r4, #0
	bne .L_081b8c46
	ldr r5, [sp, #64]
	cmp r5, #4
	bne .L_081b8c30
.L_081b8c28:
	movs r0, #113
	bl Audio_PlayCue
	b .L_081b8c46
.L_081b8c30:
	ldr r6, [sp, #88]
	cmp r6, #0
	bne .L_081b8c3a
	ldr r6, .L_081b8f14
	b .L_081b8c3c
.L_081b8c3a:
	ldr r6, .L_081b8f18
.L_081b8c3c:
	movs r0, #111
	bl Audio_PlayCue
	movs r0, #1
	str r0, [sp, #72]
.L_081b8c46:
	movs r1, #1
	negs r1, r1
	cmp r6, r1
	beq .L_081b8c70
	ldr r2, [sp, #120]
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #204
	adds r5, r2, r3
	ldr r1, [r5]
	adds r0, r6, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, .L_081b8f1c
	ldr r1, [r5]
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
.L_081b8c70:
	ldr r4, [sp, #72]
	cmp r4, #0
	bne .L_081b8cc2
	ldr r5, [sp, #128]
	ldr r2, [sp, #84]
	movs r6, #192
	lsls r6, r6, #1
	adds r0, r5, r6
	lsls r6, r2, #12
	ldr r1, .L_081b8f20
	movs r2, #16
	adds r3, r6, #0
	bl Func_081b810c
	movs r3, #208
	lsls r3, r3, #1
	adds r0, r5, r3
	movs r5, #128
	lsls r5, r5, #8
	ldr r1, .L_081b8f24
	adds r2, r5, #0
	movs r3, #16
	bl Graphics_ScaleRgb555BufferB
	ldr r4, [sp, #128]
	movs r1, #224
	lsls r1, r1, #1
	adds r0, r4, r1
	movs r2, #16
	ldr r1, .L_081b8f28
	adds r3, r6, #0
	bl Func_081b810c
	ldr r2, [sp, #128]
	movs r3, #240
	lsls r3, r3, #1
	adds r0, r2, r3
	ldr r1, .L_081b8f2c
	adds r2, r5, #0
	bl .L_081b94fa
.L_081b8cc2:
	ldr r2, [sp, #84]
	ldr r4, [sp, #128]
	movs r5, #208
	lsls r6, r2, #12
	lsls r5, r5, #1
	adds r0, r4, r5
	ldr r1, .L_081b8f24
	movs r2, #16
	adds r3, r6, #0
	bl Func_081b810c
	ldr r3, [sp, #128]
	movs r4, #192
	movs r5, #128
	lsls r4, r4, #1
	lsls r5, r5, #8
	adds r0, r3, r4
	ldr r1, .L_081b8f20
	adds r2, r5, #0
	movs r3, #16
	bl Graphics_ScaleRgb555BufferB
	ldr r1, [sp, #128]
	movs r2, #240
	lsls r2, r2, #1
	adds r0, r1, r2
	adds r3, r6, #0
	ldr r1, .L_081b8f2c
	movs r2, #16
	bl Func_081b810c
	ldr r3, [sp, #128]
	movs r4, #224
	lsls r4, r4, #1
	adds r0, r3, r4
	ldr r1, .L_081b8f28
	adds r2, r5, #0
	b .L_081b94fa
.L_081b8d0e:
	ldr r5, [sp, #96]
	cmp r5, #2
	beq .L_081b8d16
	b .L_081b9002
.L_081b8d16:
	movs r6, #0
	str r6, [sp, #44]
	str r6, [sp, #20]
	ldr r7, [sp, #124]
	mov r11, r6
.L_081b8d20:
	ldr r1, [r7]
	ldr r3, [r7, #12]
	ldr r2, [r7, #16]
	adds r1, r1, r3
	ldr r3, [r7, #4]
	str r1, [r7]
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r2, [r7, #8]
	ldr r3, [r7, #20]
	movs r0, #0
	adds r2, r2, r3
	str r2, [r7, #8]
	adds r5, r2, #0
	ldr r2, [sp, #124]
	mov r9, r0
	mov r10, r2
.L_081b8d42:
	cmp r11, r9
	beq .L_081b8db8
	mov r4, r10
	ldr r3, [r4]
	ldr r2, [r7, #4]
	subs r3, r1, r3
	asrs r6, r3, #16
	ldr r3, [r4, #4]
	mov r0, r10
	subs r2, r2, r3
	ldr r3, [r0, #8]
	asrs r4, r2, #16
	subs r3, r5, r3
	asrs r3, r3, #16
	mov r8, r3
	adds r2, r6, #0
	muls r2, r6
	adds r3, r4, #0
	muls r3, r4
	mov r0, r8
	adds r2, r2, r3
	mov r3, r8
	muls r3, r0
	adds r0, r2, r3
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #35
	cmp r0, r2
	bgt .L_081b8db8
	str r4, [sp, #12]
	ldr r3, .L_081b8f30
	mov lr, r3
	.2byte 0xf800
	adds r5, r0, #0
	adds r1, r5, #0
	lsls r0, r6, #15
	bl Math_Div
	ldr r3, [r7, #12]
	ldr r4, [sp, #12]
	adds r3, r3, r0
	adds r1, r5, #0
	str r3, [r7, #12]
	lsls r0, r4, #15
	bl Math_Div
	ldr r3, [r7, #16]
	adds r1, r5, #0
	adds r3, r3, r0
	str r3, [r7, #16]
	mov r3, r8
	lsls r0, r3, #15
	bl Math_Div
	ldr r3, [r7, #20]
	ldr r1, [r7]
	adds r3, r3, r0
	str r3, [r7, #20]
	ldr r5, [r7, #8]
.L_081b8db8:
	movs r6, #1
	add r9, r6
	movs r4, #28
	mov r0, r9
	add r10, r4
	cmp r0, #4
	bne .L_081b8d42
	asrs r3, r1, #16
	asrs r2, r5, #16
	adds r0, r3, #0
	muls r0, r3
	adds r3, r2, #0
	muls r3, r2
	adds r0, r0, r3
	ldr r3, .L_081b8f30
	mov lr, r3
	.2byte 0xf800
	cmp r0, #199
	ble .L_081b8e12
	ldr r3, [r7]
	cmp r3, #0
	ble .L_081b8dee
	ldr r3, [r7, #12]
	cmp r3, #0
	bge .L_081b8df4
	negs r3, r3
	b .L_081b8df4
.L_081b8dee:
	ldr r3, [r7, #12]
	cmp r3, #0
	bge .L_081b8df6
.L_081b8df4:
	negs r3, r3
.L_081b8df6:
	str r3, [r7, #12]
	ldr r3, [r7, #8]
	cmp r3, #0
	ble .L_081b8e08
	ldr r3, [r7, #20]
	cmp r3, #0
	bge .L_081b8e0e
	negs r3, r3
	b .L_081b8e0e
.L_081b8e08:
	ldr r3, [r7, #20]
	cmp r3, #0
	bge .L_081b8e10
.L_081b8e0e:
	negs r3, r3
.L_081b8e10:
	str r3, [r7, #20]
.L_081b8e12:
	ldr r3, [r7, #16]
	ldr r1, .L_081b8f34
	adds r2, r3, r1
	ldr r3, [r7, #4]
	str r2, [r7, #16]
	cmp r3, #0
	ble .L_081b8e22
	b .L_081b8f3c
.L_081b8e22:
	negs r2, r2
	movs r3, #0
	str r3, [r7, #4]
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r2, r3, #4
	cmp r2, #0
	bge .L_081b8e34
	adds r2, #63
.L_081b8e34:
	asrs r3, r2, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #3
	ands r0, r3
	cmp r0, #0
	bne .L_081b8e96
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
	ldr r3, .L_081b8f30
	mov lr, r3
	.2byte 0xf800
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_081b8e6a
	adds r3, #63
.L_081b8e6a:
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
	b .L_081b8eb8
.L_081b8e96:
	ldr r2, [r7, #12]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_081b8ea4
	adds r3, #63
.L_081b8ea4:
	ldr r2, [r7, #20]
	asrs r3, r3, #6
	str r3, [r7, #12]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_081b8eb6
	adds r3, #63
.L_081b8eb6:
	asrs r3, r3, #6
.L_081b8eb8:
	str r3, [r7, #20]
	ldr r6, [sp, #20]
	add r2, sp, #228
	ldr r3, [r2, r6]
	cmp r3, #19
	ble .L_081b8ec8
	subs r3, #20
	str r3, [r2, r6]
.L_081b8ec8:
	ldr r3, [r7, #16]
	ldr r4, .L_081b8f38
	cmp r3, r4
	bgt .L_081b8f40
	movs r3, #0
	str r3, [r7, #12]
	str r3, [r7, #16]
	str r3, [r7, #20]
	ldr r5, [sp, #20]
	str r3, [r2, r5]
	ldr r5, [r7, #24]
	adds r0, r5, #0
	cmp r5, #0
	bge .L_081b8eec
	movs r1, #192
	lsls r1, r1, #2
	adds r1, #255
	adds r0, r5, r1
.L_081b8eec:
	asrs r0, r0, #10
	movs r1, #3
	bl Math_Mod
	cmp r0, #1
	bne .L_081b8f02
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r5, r2
	str r3, [r7, #24]
	b .L_081b8f40
.L_081b8f02:
	cmp r0, #2
	bne .L_081b8f40
	movs r4, #128
	lsls r4, r4, #3
	adds r3, r5, r4
	str r3, [r7, #24]
	b .L_081b8f40
.L_081b8f10:
	.4byte 0x00000d7b
.L_081b8f14:
	.4byte 0x00000d7a
.L_081b8f18:
	.4byte 0x00000d7c
.L_081b8f1c:
	.4byte 0x00000d7d
.L_081b8f20:
	.4byte 0x05000380
.L_081b8f24:
	.4byte 0x050003a0
.L_081b8f28:
	.4byte 0x050003c0
.L_081b8f2c:
	.4byte 0x050003e0
.L_081b8f30:
	.4byte IwramFillWords + 0x74
.L_081b8f34:
	.4byte 0xffff8000
.L_081b8f38:
	.4byte 0x0002ffff
.L_081b8f3c:
	mov r5, r11
	lsls r6, r5, #2
.L_081b8f40:
	ldr r3, [r7, #12]
	lsls r2, r3, #6
	subs r2, r2, r3
	cmp r2, #0
	bge .L_081b8f4c
	adds r2, #63
.L_081b8f4c:
	asrs r3, r2, #6
	str r3, [r7, #12]
	ldr r3, [r7, #16]
	lsls r2, r3, #6
	subs r2, r2, r3
	cmp r2, #0
	bge .L_081b8f5c
	adds r2, #63
.L_081b8f5c:
	asrs r3, r2, #6
	str r3, [r7, #16]
	ldr r3, [r7, #20]
	lsls r2, r3, #6
	subs r2, r2, r3
	cmp r2, #0
	bge .L_081b8f6c
	adds r2, #63
.L_081b8f6c:
	asrs r3, r2, #6
	str r3, [r7, #20]
	ldr r0, [sp, #20]
	add r1, sp, #228
	ldr r3, [r1, r0]
	ldr r2, [r7, #24]
	adds r2, r2, r3
	movs r3, #142
	lsls r3, r3, #7
	adds r3, #255
	str r2, [r7, #24]
	cmp r2, r3
	ble .L_081b8f8c
	ldr r4, .L_081b9214
	adds r3, r2, r4
	str r3, [r7, #24]
.L_081b8f8c:
	ldr r3, [r1, r6]
	cmp r3, #0
	ble .L_081b8f96
	subs r3, #1
	str r3, [r1, r6]
.L_081b8f96:
	ldr r3, [r7, #12]
	cmp r3, #0
	bne .L_081b8fc2
	ldr r3, [r7, #16]
	cmp r3, #0
	bne .L_081b8fc2
	ldr r3, [r7, #20]
	cmp r3, #0
	bne .L_081b8fc2
	ldr r3, [r7, #4]
	cmp r3, #0
	bne .L_081b8fc2
	movs r1, #192
	ldr r0, [r7, #24]
	lsls r1, r1, #4
	add r5, sp, #180
	bl Math_Div
	str r0, [r5, r6]
	ldr r5, [sp, #44]
	adds r5, #1
	str r5, [sp, #44]
.L_081b8fc2:
	ldr r6, [sp, #20]
	movs r0, #1
	add r11, r0
	adds r6, #4
	mov r1, r11
	str r6, [sp, #20]
	adds r7, #28
	cmp r1, #4
	beq .L_081b8fd6
	b .L_081b8d20
.L_081b8fd6:
	ldr r2, [sp, #44]
	cmp r2, #4
	beq .L_081b8fde
	b .L_081b9550
.L_081b8fde:
	movs r4, #0
	movs r3, #3
	str r3, [sp, #96]
	str r4, [sp, #84]
	str r4, [sp, #104]
	mov r11, r4
	add r2, sp, #180
.L_081b8fec:
	ldmia r2!, {r3}
	ldr r5, [sp, #104]
	movs r6, #1
	adds r3, r5, r3
	add r11, r6
	adds r3, #1
	mov r0, r11
	str r3, [sp, #104]
	cmp r0, #4
	bne .L_081b8fec
	b .L_081b9550
.L_081b9002:
	ldr r1, [sp, #96]
	cmp r1, #3
	beq .L_081b900a
	b .L_081b9220
.L_081b900a:
	ldr r2, [sp, #84]
	adds r2, #1
	str r2, [sp, #84]
	cmp r2, #24
	bne .L_081b901a
	movs r0, #152
	bl Audio_PlayCue
.L_081b901a:
	ldr r3, [sp, #84]
	cmp r3, #70
	bne .L_081b9026
	movs r0, #103
	bl Audio_PlayCue
.L_081b9026:
	ldr r4, [sp, #84]
	cmp r4, #40
	bne .L_081b9050
	ldr r5, [sp, #100]
	cmp r5, #1
	bls .L_081b903a
	movs r0, #55
	bl Audio_PlayCue
	b .L_081b904a
.L_081b903a:
	ldr r6, [sp, #52]
	movs r0, #1
	negs r0, r0
	cmp r6, r0
	beq .L_081b904a
	adds r0, r6, #0
	bl Audio_PlayCue
.L_081b904a:
	movs r1, #1
	negs r1, r1
	str r1, [sp, #52]
.L_081b9050:
	ldr r2, [sp, #84]
	cmp r2, #1
	bne .L_081b90f6
	ldr r6, [sp, #88]
	movs r5, #1
	movs r3, #3
	movs r4, #0
	negs r5, r5
	str r3, [sp, #56]
	str r4, [sp, #100]
	str r5, [sp, #52]
	cmp r6, #0
	bne .L_081b90b2
	add r0, sp, #180
	bl Func_081b834c
	str r0, [sp, #100]
	cmp r0, #3
	beq .L_081b909e
	cmp r0, #3
	bgt .L_081b9080
	cmp r0, #2
	beq .L_081b9094
	b .L_081b90d2
.L_081b9080:
	ldr r0, [sp, #100]
	cmp r0, #4
	beq .L_081b90a8
	cmp r0, #5
	bne .L_081b90d2
	movs r1, #93
	movs r2, #8
	str r1, [sp, #52]
	str r2, [sp, #60]
	b .L_081b90d2
.L_081b9094:
	movs r3, #91
	movs r4, #1
	str r3, [sp, #52]
	str r4, [sp, #60]
	b .L_081b90d2
.L_081b909e:
	movs r5, #92
	movs r6, #3
	str r5, [sp, #52]
	str r6, [sp, #60]
	b .L_081b90d2
.L_081b90a8:
	movs r0, #91
	movs r1, #2
	str r0, [sp, #52]
	str r1, [sp, #60]
	b .L_081b90d2
.L_081b90b2:
	ldr r2, [sp, #72]
	cmp r2, #0
	bne .L_081b90c6
	ldr r3, [sp, #104]
	ldr r4, [sp, #64]
	cmp r3, r4
	ble .L_081b90d2
	movs r5, #1
	str r5, [sp, #100]
	b .L_081b90d2
.L_081b90c6:
	ldr r6, [sp, #104]
	ldr r0, [sp, #64]
	cmp r6, r0
	bge .L_081b90d2
	movs r1, #1
	str r1, [sp, #100]
.L_081b90d2:
	ldr r2, [sp, #100]
	cmp r2, #1
	bne .L_081b90f6
	ldr r3, [sp, #88]
	cmp r3, #3
	bgt .L_081b90e2
	movs r4, #91
	str r4, [sp, #52]
.L_081b90e2:
	ldr r5, [sp, #88]
	cmp r5, #4
	bne .L_081b90ec
	movs r6, #92
	str r6, [sp, #52]
.L_081b90ec:
	ldr r0, [sp, #88]
	cmp r0, #5
	bne .L_081b90f6
	movs r1, #93
	str r1, [sp, #52]
.L_081b90f6:
	ldr r2, [sp, #100]
	cmp r2, #0
	bne .L_081b9104
	ldr r3, [sp, #84]
	cmp r3, #90
	beq .L_081b910c
	b .L_081b9550
.L_081b9104:
	ldr r4, [sp, #84]
	cmp r4, #68
	beq .L_081b910c
	b .L_081b9550
.L_081b910c:
	ldr r5, [sp, #100]
	cmp r5, #0
	beq .L_081b919a
	ldr r6, [sp, #88]
	cmp r6, #0
	bne .L_081b9124
	ldr r2, [sp, #92]
	ldr r1, [sp, #60]
	adds r0, r1, #0
	muls r0, r2
	str r0, [sp, #92]
	b .L_081b912a
.L_081b9124:
	ldr r3, [sp, #92]
	lsls r3, r3, #1
	str r3, [sp, #92]
.L_081b912a:
	ldr r4, [sp, #120]
	movs r5, #153
	lsls r5, r5, #3
	adds r3, r4, r5
	ldr r2, [r3]
	movs r3, #5
	str r3, [sp, #0]
	ldr r0, [sp, #92]
	movs r1, #6
	movs r3, #24
	bl UiText_DrawNumberInWindowFar
	ldr r6, [sp, #60]
	ldr r0, [sp, #88]
	lsls r6, r6, #1
	str r6, [sp, #60]
	cmp r0, #5
	bne .L_081b9160
	ldr r3, [sp, #92]
	movs r1, #14
	movs r2, #0
	movs r4, #4
	str r1, [sp, #64]
	str r2, [sp, #88]
	str r3, [sp, #76]
	str r4, [sp, #96]
	b .L_081b9192
.L_081b9160:
	movs r3, #6
	movs r5, #5
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #20
	movs r3, #4
	movs r6, #0
	movs r0, #4
	str r5, [sp, #96]
	str r5, [sp, #56]
	str r6, [sp, #68]
	bl UiWindow_CreateFar
	movs r2, #128
	adds r1, r0, #0
	ldr r0, [sp, #120]
	lsls r2, r2, #3
	adds r2, #204
	adds r3, r0, r2
	str r1, [r3]
	ldr r0, .L_081b9218
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_081b9192:
	movs r3, #0
	str r3, [sp, #84]
	str r3, [sp, #80]
	b .L_081b9550
.L_081b919a:
	movs r4, #14
	movs r5, #0
	str r4, [sp, #64]
	str r5, [sp, #88]
	bl Func_080ad290
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r6, [sp, #120]
	lsls r3, r3, #1
	str r3, [sp, #92]
	movs r0, #153
	lsls r0, r0, #3
	adds r3, r6, r0
	ldr r2, [r3]
	movs r3, #5
	str r3, [sp, #0]
	ldr r0, [sp, #92]
	movs r1, #6
	movs r3, #24
	bl UiText_DrawNumberInWindowFar
	movs r0, #1
	str r5, [sp, #60]
	bl Audio_PlayCue
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #8
	movs r3, #4
	movs r0, #22
	str r5, [sp, #96]
	bl UiWindow_CreateFar
	movs r3, #128
	ldr r2, [sp, #120]
	ldr r5, .L_081b921c
	lsls r3, r3, #3
	adds r3, #204
	adds r1, r0, #0
	adds r6, r2, r3
	adds r0, r5, #0
	str r1, [r6]
	movs r2, #0
	movs r3, #0
	adds r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #0
	ldr r1, [r6]
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	movs r5, #1
	movs r4, #0
	str r4, [sp, #80]
	str r4, [sp, #84]
	str r5, [sp, #56]
	b .L_081b9550
.L_081b9214:
	.4byte 0xffffb800
.L_081b9218:
	.4byte 0x00000d7e
.L_081b921c:
	.4byte 0x00000d77
.L_081b9220:
	ldr r6, [sp, #96]
	cmp r6, #4
	beq .L_081b9228
	b .L_081b93c6
.L_081b9228:
	ldr r0, [sp, #84]
	adds r0, #1
	str r0, [sp, #84]
	cmp r0, #20
	bne .L_081b92ae
	ldr r1, [sp, #76]
	movs r7, #1
	cmp r1, #9
	ble .L_081b925e
	ldr r2, [sp, #76]
	movs r7, #2
	cmp r2, #99
	ble .L_081b925e
	ldr r3, [sp, #76]
	movs r4, #250
	lsls r4, r4, #2
	movs r7, #3
	cmp r3, r4
	blt .L_081b925e
	movs r6, #156
	ldr r5, [sp, #76]
	lsls r6, r6, #6
	adds r6, #15
	movs r7, #4
	cmp r5, r6
	ble .L_081b925e
	movs r7, #5
.L_081b925e:
	lsrs r3, r7, #1
	movs r0, #9
	subs r0, r0, r3
	adds r2, r7, #0
	movs r3, #6
	str r3, [sp, #0]
	adds r2, #12
	movs r1, #16
	movs r3, #3
	bl UiWindow_CreateFar
	movs r2, #128
	adds r1, r0, #0
	ldr r0, [sp, #120]
	ldr r5, .L_081b9504
	lsls r2, r2, #3
	adds r2, #204
	adds r6, r0, r2
	str r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	subs r5, #1
	lsls r2, r7, #3
	ldr r1, [r6]
	adds r2, #48
	adds r0, r5, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #0
	ldr r2, [r6]
	ldr r0, [sp, #76]
	str r3, [sp, #0]
	adds r1, r7, #0
	movs r3, #48
	bl UiText_DrawNumberInWindowFar
.L_081b92ae:
	ldr r3, [sp, #84]
	cmp r3, #19
	bgt .L_081b92b6
	b .L_081b9550
.L_081b92b6:
	ldr r3, .L_081b9508
	ldr r5, [r3, #12]
	movs r3, #1
	ands r5, r3
	cmp r5, #0
	beq .L_081b9372
	ldr r4, [sp, #120]
	movs r5, #128
	lsls r5, r5, #3
	adds r5, #204
	adds r6, r4, r5
	movs r1, #1
	ldr r0, [r6]
	bl UiWork_FinalizeFar
	movs r0, #1
	bl Audio_PlayCue
	movs r0, #112
	bl Audio_PlayCue
	movs r0, #0
	movs r3, #6
	str r0, [sp, #96]
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #8
	movs r3, #4
	movs r0, #22
	bl UiWindow_CreateFar
	ldr r5, .L_081b950c
	adds r1, r0, #0
	str r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	adds r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	ldr r1, [r6]
	adds r0, r5, #0
	movs r3, #8
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r1, #0
	movs r2, #1
	str r1, [sp, #80]
	str r1, [sp, #84]
	str r2, [sp, #56]
	bl Func_080ad290
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	ldr r4, [sp, #120]
	str r3, [sp, #92]
	movs r5, #153
	lsls r5, r5, #3
	adds r3, r4, r5
	ldr r2, [r3]
	movs r3, #5
	str r3, [sp, #0]
	ldr r0, [sp, #92]
	movs r1, #6
	movs r3, #24
	bl UiText_DrawNumberInWindowFar
	ldr r6, [sp, #76]
	cmp r6, #0
	bgt .L_081b9348
	b .L_081b9550
.L_081b9348:
	movs r0, #105
	bl Audio_PlayCue
	ldr r0, [sp, #76]
	bl Party_AdjustSixDigitCounterAFar
	ldr r3, .L_081b9510
	ldr r1, [sp, #120]
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #196
	ldr r0, [r3, #16]
	adds r3, r1, r2
	ldr r2, [r3]
	ldr r3, [sp, #96]
	movs r1, #6
	str r3, [sp, #0]
	movs r3, #0
	bl UiText_DrawNumberInWindowFar
	b .L_081b9550
.L_081b9372:
	ldr r4, [sp, #76]
	cmp r4, #0
	bgt .L_081b937a
	b .L_081b9550
.L_081b937a:
	subs r4, #1
	str r4, [sp, #76]
	cmp r4, #0
	bne .L_081b9388
	movs r0, #105
	bl Audio_PlayCue
.L_081b9388:
	movs r0, #1
	bl Party_AdjustSixDigitCounterAFar
	ldr r3, .L_081b9510
	ldr r6, [sp, #120]
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #196
	ldr r0, [r3, #16]
	adds r3, r6, r1
	ldr r2, [r3]
	movs r1, #6
	movs r3, #0
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	ldr r2, [sp, #92]
	movs r4, #153
	subs r2, #1
	str r2, [sp, #92]
	lsls r4, r4, #3
	adds r3, r6, r4
	ldr r2, [r3]
	movs r3, #5
	str r3, [sp, #0]
	ldr r0, [sp, #92]
	movs r1, #6
	movs r3, #24
	bl UiText_DrawNumberInWindowFar
	b .L_081b9550
.L_081b93c6:
	ldr r5, [sp, #96]
	cmp r5, #5
	beq .L_081b93ce
	b .L_081b9550
.L_081b93ce:
	ldr r6, [sp, #80]
	cmp r6, #4
	bne .L_081b93da
	movs r0, #175
	bl Audio_PlayCue
.L_081b93da:
	ldr r0, [sp, #80]
	cmp r0, #7
	bgt .L_081b93e6
	adds r0, #1
	str r0, [sp, #80]
	b .L_081b94d4
.L_081b93e6:
	ldr r1, [sp, #84]
	cmp r1, #0
	ble .L_081b9488
	adds r1, #1
	str r1, [sp, #84]
	cmp r1, #16
	bne .L_081b94d4
	ldr r2, [sp, #120]
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #204
	adds r5, r2, r3
	ldr r0, [r5]
	movs r1, #1
	bl UiWork_FinalizeFar
	ldr r4, [sp, #68]
	cmp r4, #0
	bne .L_081b9472
	ldr r0, [sp, #88]
	ldr r6, [sp, #104]
	adds r0, #1
	movs r1, #1
	movs r3, #6
	str r0, [sp, #88]
	str r1, [sp, #96]
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #21
	movs r0, #4
	movs r3, #4
	str r6, [sp, #64]
	bl UiWindow_CreateFar
	adds r1, r0, #0
	str r1, [r5]
	ldr r2, [sp, #104]
	cmp r2, #24
	bne .L_081b9444
	movs r3, #1
	str r3, [sp, #72]
	ldr r0, .L_081b9514
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_081b9452
.L_081b9444:
	movs r4, #0
	ldr r0, .L_081b9518
	movs r2, #0
	movs r3, #0
	str r4, [sp, #72]
	bl UiText_DrawCharacterAtOffsetFar
.L_081b9452:
	ldr r5, [sp, #120]
	movs r6, #128
	lsls r6, r6, #3
	adds r6, #204
	adds r3, r5, r6
	ldr r1, [r3]
	ldr r0, .L_081b951c
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #0
	movs r1, #1
	str r0, [sp, #80]
	str r1, [sp, #56]
	b .L_081b9482
.L_081b9472:
	ldr r4, [sp, #92]
	movs r2, #14
	movs r3, #0
	movs r5, #4
	str r2, [sp, #64]
	str r3, [sp, #88]
	str r4, [sp, #76]
	str r5, [sp, #96]
.L_081b9482:
	movs r6, #0
	str r6, [sp, #84]
	b .L_081b94d4
.L_081b9488:
	ldr r1, .L_081b9508
	movs r2, #1
	ldr r3, [r1, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_081b949e
	movs r0, #112
	str r2, [sp, #84]
	bl Audio_PlayCue
	b .L_081b94d4
.L_081b949e:
	ldr r3, [r1, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_081b94ba
	ldr r0, [sp, #68]
	cmp r0, #1
	bne .L_081b94ba
	movs r0, #111
	bl Audio_PlayCue
	movs r1, #0
	str r1, [sp, #68]
	b .L_081b94d4
.L_081b94ba:
	ldr r3, [r1, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_081b94d4
	ldr r2, [sp, #68]
	cmp r2, #0
	bne .L_081b9528
	movs r0, #111
	bl Audio_PlayCue
	movs r3, #1
	str r3, [sp, #68]
.L_081b94d4:
	ldr r4, [sp, #68]
	cmp r4, #0
	bne .L_081b9528
	ldr r5, [sp, #128]
	ldr r2, [sp, #84]
	movs r6, #224
	lsls r6, r6, #1
	lsls r3, r2, #12
	adds r0, r5, r6
	ldr r1, .L_081b9520
	movs r2, #16
	bl Func_081b810c
	movs r3, #240
	lsls r3, r3, #1
	movs r2, #128
	adds r0, r5, r3
	ldr r1, .L_081b9524
	lsls r2, r2, #8
.L_081b94fa:
	movs r3, #16
	bl Graphics_ScaleRgb555BufferB
	b .L_081b9550
	.2byte 0x0000
.L_081b9504:
	.4byte 0x00000d6a
.L_081b9508:
	.4byte gInput
.L_081b950c:
	.4byte 0x00000d77
.L_081b9510:
	.4byte gPartyState
.L_081b9514:
	.4byte 0x00000d7c
.L_081b9518:
	.4byte 0x00000d7b
.L_081b951c:
	.4byte 0x00000d7d
.L_081b9520:
	.4byte 0x050003c0
.L_081b9524:
	.4byte 0x050003e0
.L_081b9528:
	ldr r4, [sp, #128]
	ldr r6, [sp, #84]
	movs r5, #240
	lsls r5, r5, #1
	adds r0, r4, r5
	ldr r1, .L_081b962c
	lsls r3, r6, #12
	movs r2, #16
	bl Func_081b810c
	ldr r1, [sp, #128]
	movs r2, #224
	lsls r2, r2, #1
	adds r0, r1, r2
	movs r2, #128
	ldr r1, .L_081b9630
	lsls r2, r2, #8
	movs r3, #16
	bl Graphics_ScaleRgb555BufferB
.L_081b9550:
	ldr r3, [sp, #56]
	movs r4, #1
	negs r4, r4
	cmp r3, r4
	bne .L_081b955c
	b .L_081b9800
.L_081b955c:
	cmp r3, #1
	beq .L_081b9562
	b .L_081b96aa
.L_081b9562:
	ldr r0, .L_081b9634
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r0, [sp, #112]
	adds r3, #212
	ldr r1, .L_081b9638
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, [sp, #128]
	movs r6, #192
	movs r2, #132
	lsls r6, r6, #1
	lsls r2, r2, #24
	ldr r0, [sp, #112]
	adds r1, r5, r6
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, [sp, #112]
	ldr r1, .L_081b963c
	adds r0, #32
	str r0, [sp, #112]
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081b963c
	ldr r1, .L_081b9640
	ldr r2, .L_081b9644
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081b9648
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r0, [sp, #112]
	adds r3, #212
	ldr r1, .L_081b964c
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #208
	lsls r2, r2, #1
	adds r1, r5, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, [sp, #112]
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [sp, #112]
	ldr r1, .L_081b963c
	adds r3, #32
	adds r0, r3, #0
	str r3, [sp, #112]
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081b963c
	ldr r1, .L_081b9650
	ldr r2, .L_081b9644
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081b9654
	bl Resource_GetTableEntry
	adds r6, #64
	adds r4, r5, r6
	str r0, [sp, #112]
	ldr r6, .L_081b9628
	movs r1, #0
	mov r11, r1
	movs r5, #31
.L_081b960a:
	ldrh r3, [r0]
	lsrs r2, r3, #10
	ands r2, r6
	adds r1, r2, #4
	adds r2, r5, #0
	ands r2, r3
	adds r2, #4
	cmp r2, #31
	ble .L_081b961e
	movs r2, #31
.L_081b961e:
	cmp r1, #31
	ble .L_081b9658
	movs r1, #31
	b .L_081b9658
	.2byte 0x0000
.L_081b9628:
	.4byte 0x0000001f
.L_081b962c:
	.4byte 0x050003e0
.L_081b9630:
	.4byte 0x050003c0
.L_081b9634:
	.4byte 0x0000008a
.L_081b9638:
	.4byte 0x05000380
.L_081b963c:
	.4byte gMapCellBuffer
.L_081b9640:
	.4byte 0x06014000
.L_081b9644:
	.4byte 0x84000180
.L_081b9648:
	.4byte 0x0000008b
.L_081b964c:
	.4byte 0x050003a0
.L_081b9650:
	.4byte 0x06014600
.L_081b9654:
	.4byte 0x0000008e
.L_081b9658:
	lsls r2, r2, #5
	lsls r3, r1, #10
	orrs r3, r2
	movs r2, #1
	orrs r3, r1
	add r11, r2
	strh r3, [r4]
	mov r3, r11
	adds r0, #2
	adds r4, #2
	cmp r3, #16
	bne .L_081b960a
	ldr r5, [sp, #128]
	movs r6, #224
	lsls r6, r6, #1
	movs r3, #128
	movs r2, #132
	adds r4, r5, r6
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	ldr r1, .L_081b98bc
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	lsls r2, r2, #24
	adds r0, r4, #0
	adds r1, #32
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #240
	lsls r2, r2, #1
	adds r1, r5, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r0, r4, #0
	adds r2, #8
	b .L_081b97f6
.L_081b96aa:
	ldr r3, [sp, #56]
	cmp r3, #3
	bne .L_081b975c
	ldr r4, [sp, #100]
	cmp r4, #5
	bhi .L_081b96f4
	ldr r2, .L_081b98c0
	lsls r3, r4, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_081b96c0:
	.4byte .L_081b96d8
	.4byte .L_081b96dc
	.4byte .L_081b96e0
	.4byte .L_081b96e4
	.4byte .L_081b96e8
	.4byte .L_081b96ec
.L_081b96d8:
	ldr r0, .L_081b98c4
	b .L_081b96ee
.L_081b96dc:
	ldr r0, .L_081b98c8
	b .L_081b96ee
.L_081b96e0:
	ldr r0, .L_081b98cc
	b .L_081b96ee
.L_081b96e4:
	ldr r0, .L_081b98d0
	b .L_081b96ee
.L_081b96e8:
	ldr r0, .L_081b98d4
	b .L_081b96ee
.L_081b96ec:
	ldr r0, .L_081b98d8
.L_081b96ee:
	bl Resource_GetTableEntry
	str r0, [sp, #112]
.L_081b96f4:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, [sp, #112]
	ldr r1, .L_081b98dc
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, [sp, #112]
	ldr r1, .L_081b98e0
	adds r5, #32
	adds r0, r5, #0
	str r5, [sp, #112]
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081b98e0
	ldr r1, .L_081b98e4
	ldr r2, .L_081b98e8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081b98ec
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r0, [sp, #112]
	adds r3, #212
	ldr r1, .L_081b98f0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r6, [sp, #112]
	ldr r1, .L_081b98e0
	adds r6, #32
	adds r0, r6, #0
	str r6, [sp, #112]
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081b98e0
	ldr r1, .L_081b98f4
	ldr r2, .L_081b98f8
	b .L_081b97f6
.L_081b975c:
	ldr r0, [sp, #56]
	cmp r0, #5
	bne .L_081b97fa
	ldr r0, .L_081b98fc
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r0, [sp, #112]
	adds r3, #212
	ldr r1, .L_081b98bc
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, [sp, #128]
	movs r4, #224
	lsls r4, r4, #1
	adds r1, r2, r4
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, [sp, #112]
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, [sp, #112]
	ldr r1, .L_081b98e0
	adds r5, #32
	adds r0, r5, #0
	str r5, [sp, #112]
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081b98e0
	ldr r1, .L_081b9900
	ldr r2, .L_081b9904
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081b9908
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r0, [sp, #112]
	adds r3, #212
	ldr r1, .L_081b990c
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r6, [sp, #128]
	movs r2, #240
	lsls r2, r2, #1
	adds r1, r6, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, [sp, #112]
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [sp, #112]
	ldr r1, .L_081b98e0
	adds r3, #32
	adds r0, r3, #0
	str r3, [sp, #112]
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081b98e0
	ldr r1, .L_081b9910
	ldr r2, .L_081b9904
.L_081b97f6:
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_081b97fa:
	movs r4, #1
	negs r4, r4
	str r4, [sp, #56]
.L_081b9800:
	ldr r5, [sp, #88]
	cmp r5, #0
	bgt .L_081b9808
	b .L_081b9a72
.L_081b9808:
	ldr r1, [sp, #48]
	movs r6, #0
	movs r0, #224
	lsls r0, r0, #7
	lsls r3, r1, #3
	mov r11, r6
	movs r7, #128
	movs r6, #128
	lsls r7, r7, #1
	lsls r6, r6, #23
	mov r12, r0
	adds r1, r3, #0
	movs r0, #128
	adds r2, r3, #0
	adds r7, #255
	adds r6, #2
	movs r4, #152
	lsls r0, r0, #2
	adds r1, #200
	adds r2, #196
.L_081b9830:
	adds r3, r0, #0
	ldr r5, [sp, #120]
	ands r3, r7
	lsls r3, r3, #16
	orrs r3, r6
	str r3, [r5, r2]
	adds r3, r4, #0
	mov r5, r12
	orrs r3, r5
	ldr r5, [sp, #120]
	adds r2, #8
	str r3, [r5, r1]
	ldr r3, [sp, #48]
	movs r5, #1
	adds r3, #1
	add r11, r5
	str r3, [sp, #48]
	mov r3, r11
	adds r1, #8
	adds r4, #4
	adds r0, #16
	cmp r3, #4
	bne .L_081b9830
	ldr r4, [sp, #88]
	cmp r4, #5
	bne .L_081b9914
	ldr r1, [sp, #48]
	movs r0, #192
	lsls r0, r0, #7
	lsls r3, r1, #3
	movs r7, #128
	movs r6, #128
	movs r5, #0
	lsls r7, r7, #1
	lsls r6, r6, #23
	mov r12, r0
	adds r1, r3, #0
	movs r0, #146
	adds r2, r3, #0
	mov r11, r5
	adds r7, #255
	adds r6, #2
	movs r4, #168
	lsls r0, r0, #2
	adds r1, #200
	adds r2, #196
.L_081b988c:
	adds r3, r0, #0
	ldr r5, [sp, #120]
	ands r3, r7
	lsls r3, r3, #16
	orrs r3, r6
	str r3, [r5, r2]
	adds r3, r4, #0
	mov r5, r12
	orrs r3, r5
	ldr r5, [sp, #120]
	adds r2, #8
	str r3, [r5, r1]
	ldr r3, [sp, #48]
	movs r5, #1
	adds r3, #1
	add r11, r5
	str r3, [sp, #48]
	mov r3, r11
	adds r1, #8
	adds r4, #4
	adds r0, #16
	cmp r3, #5
	bne .L_081b988c
	b .L_081b998c
.L_081b98bc:
	.4byte 0x050003c0
.L_081b98c0:
	.4byte .L_081b96c0
.L_081b98c4:
	.4byte 0x00000089
.L_081b98c8:
	.4byte 0x00000088
.L_081b98cc:
	.4byte 0x00000093
.L_081b98d0:
	.4byte 0x00000094
.L_081b98d4:
	.4byte 0x00000095
.L_081b98d8:
	.4byte 0x00000096
.L_081b98dc:
	.4byte 0x050003a0
.L_081b98e0:
	.4byte gMapCellBuffer
.L_081b98e4:
	.4byte 0x06014000
.L_081b98e8:
	.4byte 0x84000400
.L_081b98ec:
	.4byte 0x00000090
.L_081b98f0:
	.4byte 0x05000380
.L_081b98f4:
	.4byte 0x06015000
.L_081b98f8:
	.4byte 0x84000500
.L_081b98fc:
	.4byte 0x0000008c
.L_081b9900:
	.4byte 0x06012480
.L_081b9904:
	.4byte 0x84000280
.L_081b9908:
	.4byte 0x0000008d
.L_081b990c:
	.4byte 0x050003e0
.L_081b9910:
	.4byte 0x06012e80
.L_081b9914:
	ldr r1, [sp, #48]
	movs r7, #128
	lsls r3, r1, #3
	movs r6, #128
	movs r5, #192
	movs r4, #0
	lsls r7, r7, #1
	lsls r6, r6, #23
	lsls r5, r5, #7
	movs r0, #152
	adds r1, r3, #0
	adds r2, r3, #0
	mov r11, r4
	adds r7, #255
	adds r6, #2
	mov r12, r5
	movs r4, #188
	lsls r0, r0, #2
	adds r1, #200
	adds r2, #196
.L_081b993c:
	adds r3, r0, #0
	ldr r5, [sp, #120]
	ands r3, r7
	lsls r3, r3, #16
	orrs r3, r6
	str r3, [r5, r2]
	adds r3, r4, #0
	mov r5, r12
	orrs r3, r5
	ldr r5, [sp, #120]
	adds r2, #8
	str r3, [r5, r1]
	ldr r3, [sp, #48]
	movs r5, #1
	adds r3, #1
	add r11, r5
	str r3, [sp, #48]
	mov r3, r11
	adds r1, #8
	adds r4, #4
	adds r0, #16
	cmp r3, #3
	bne .L_081b993c
	ldr r4, [sp, #48]
	ldr r3, .L_081b9b10
	lsls r1, r4, #3
	ldr r5, [sp, #120]
	adds r2, r1, #0
	adds r2, #196
	str r3, [r5, r2]
	ldr r6, [sp, #88]
	movs r2, #128
	lsls r3, r6, #3
	adds r3, #200
	lsls r2, r2, #8
	adds r1, #200
	orrs r3, r2
	adds r4, #1
	str r3, [r5, r1]
	str r4, [sp, #48]
.L_081b998c:
	ldr r1, [sp, #48]
	movs r0, #0
	lsls r3, r1, #3
	adds r1, r3, #0
	adds r2, r3, #0
	movs r7, #128
	movs r6, #128
	movs r3, #144
	mov r11, r0
	lsls r7, r7, #1
	lsls r6, r6, #23
	movs r4, #142
	movs r0, #144
	lsls r3, r3, #8
	adds r7, #255
	adds r6, #24
	lsls r4, r4, #1
	lsls r0, r0, #2
	adds r1, #200
	adds r2, #196
	mov r12, r3
.L_081b99b6:
	adds r3, r0, #0
	ldr r5, [sp, #120]
	ands r3, r7
	lsls r3, r3, #16
	orrs r3, r6
	str r3, [r5, r2]
	adds r3, r4, #0
	mov r5, r12
	orrs r3, r5
	ldr r5, [sp, #120]
	adds r2, #8
	str r3, [r5, r1]
	ldr r3, [sp, #48]
	movs r5, #1
	adds r3, #1
	add r11, r5
	str r3, [sp, #48]
	mov r3, r11
	adds r1, #8
	adds r4, #4
	adds r0, #16
	cmp r3, #2
	bne .L_081b99b6
	ldr r4, [sp, #60]
	movs r3, #1
	cmp r4, #9
	ble .L_081b9a10
	ldr r5, [sp, #60]
	movs r3, #2
	cmp r5, #99
	ble .L_081b9a10
	ldr r6, [sp, #60]
	movs r0, #250
	lsls r0, r0, #2
	movs r3, #3
	cmp r6, r0
	blt .L_081b9a10
	movs r2, #156
	ldr r1, [sp, #60]
	lsls r2, r2, #6
	adds r2, #15
	movs r3, #4
	cmp r1, r2
	ble .L_081b9a10
	movs r3, #5
.L_081b9a10:
	lsls r2, r3, #2
	adds r2, r2, r3
	movs r3, #140
	lsls r2, r2, #1
	lsls r3, r3, #2
	subs r3, r3, r2
	movs r2, #128
	ldr r4, [sp, #48]
	lsls r2, r2, #1
	adds r2, #255
	ands r3, r2
	movs r2, #128
	ldr r5, [sp, #120]
	lsls r1, r4, #3
	lsls r2, r2, #23
	adds r0, r1, #0
	adds r2, #24
	lsls r3, r3, #16
	orrs r3, r2
	adds r0, #196
	str r3, [r5, r0]
	movs r3, #145
	lsls r3, r3, #8
	adds r1, #200
	adds r3, #24
	str r3, [r5, r1]
	movs r3, #10
	adds r4, #1
	str r3, [sp, #0]
	movs r3, #5
	str r3, [sp, #4]
	adds r0, r4, #0
	ldr r3, [sp, #60]
	movs r1, #48
	movs r2, #20
	str r4, [sp, #48]
	bl Func_081b8180
	movs r3, #11
	str r3, [sp, #0]
	movs r3, #1
	str r3, [sp, #4]
	movs r1, #80
	movs r2, #20
	movs r3, #0
	str r0, [sp, #48]
	bl Func_081b8180
	str r0, [sp, #48]
.L_081b9a72:
	ldr r6, [sp, #96]
	cmp r6, #1
	beq .L_081b9a7a
	b .L_081b9bfa
.L_081b9a7a:
	ldr r0, [sp, #84]
	cmp r0, #0
	ble .L_081b9a92
	ldr r1, [sp, #72]
	cmp r1, #1
	bne .L_081b9a92
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r5, r3, #0
	adds r5, #64
	b .L_081b9a94
.L_081b9a92:
	movs r5, #64
.L_081b9a94:
	ldr r2, [sp, #72]
	cmp r2, #0
	bne .L_081b9aae
	ldr r3, [sp, #108]
	lsls r0, r3, #10
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	ands r0, r3
	bl Trig_Sin
	asrs r0, r0, #14
	b .L_081b9ab0
.L_081b9aae:
	movs r0, #0
.L_081b9ab0:
	ldr r4, [sp, #80]
	lsls r2, r4, #4
	adds r6, r2, #0
	subs r3, r6, r4
	adds r1, r5, r3
	ldr r5, [sp, #64]
	cmp r5, #23
	bgt .L_081b9ada
	adds r2, r0, #0
	movs r0, #14
	str r0, [sp, #0]
	adds r3, r5, #0
	movs r0, #2
	str r0, [sp, #4]
	subs r1, #110
	adds r2, #60
	adds r3, #1
	ldr r0, [sp, #48]
	bl Func_081b8180
	str r0, [sp, #48]
.L_081b9ada:
	ldr r0, [sp, #84]
	cmp r0, #0
	ble .L_081b9af2
	ldr r1, [sp, #72]
	cmp r1, #0
	bne .L_081b9af2
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r5, r3, #0
	adds r5, #64
	b .L_081b9af4
.L_081b9af2:
	movs r5, #64
.L_081b9af4:
	ldr r2, [sp, #72]
	cmp r2, #1
	bne .L_081b9b14
	ldr r3, [sp, #108]
	lsls r0, r3, #10
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	ands r0, r3
	bl Trig_Sin
	asrs r0, r0, #14
	b .L_081b9b16
	.2byte 0x0000
.L_081b9b10:
	.4byte 0x80508000
.L_081b9b14:
	movs r0, #0
.L_081b9b16:
	ldr r4, [sp, #80]
	subs r6, r6, r4
	adds r1, r5, r6
	ldr r5, [sp, #64]
	cmp r5, #4
	ble .L_081b9b3c
	movs r2, #94
	subs r2, r2, r0
	movs r0, #15
	str r0, [sp, #0]
	adds r3, r5, #0
	movs r0, #2
	str r0, [sp, #4]
	subs r1, #110
	subs r3, #1
	ldr r0, [sp, #48]
	bl Func_081b8180
	str r0, [sp, #48]
.L_081b9b3c:
	ldr r1, [sp, #84]
	adds r4, r6, #0
	lsls r3, r1, #1
	adds r3, r3, r1
	movs r6, #128
	movs r7, #128
	movs r0, #0
	lsls r3, r3, #2
	lsls r6, r6, #2
	lsls r7, r7, #1
	mov r11, r0
	mov r12, r3
	mov r8, r6
	mov lr, r0
	movs r5, #0
	adds r7, #255
.L_081b9b5c:
	ldr r2, [sp, #84]
	movs r3, #64
	cmp r2, #0
	ble .L_081b9b6e
	ldr r0, [sp, #72]
	cmp r0, #1
	bne .L_081b9b6e
	mov r3, r12
	adds r3, #64
.L_081b9b6e:
	ldr r2, [sp, #48]
	adds r3, r3, r4
	subs r3, #120
	adds r3, r3, r5
	lsls r1, r2, #3
	add r3, r8
	movs r2, #128
	ands r3, r7
	lsls r2, r2, #24
	adds r2, #52
	lsls r3, r3, #16
	orrs r3, r2
	ldr r2, [sp, #120]
	adds r0, r1, #0
	adds r0, #196
	str r3, [r2, r0]
	movs r3, #192
	lsls r3, r3, #8
	orrs r3, r6
	adds r1, #200
	str r3, [r2, r1]
	ldr r3, [sp, #48]
	ldr r0, [sp, #84]
	adds r3, #1
	str r3, [sp, #48]
	movs r3, #64
	cmp r0, #0
	ble .L_081b9bb0
	ldr r1, [sp, #72]
	cmp r1, #0
	bne .L_081b9bb0
	mov r3, r12
	adds r3, #64
.L_081b9bb0:
	ldr r2, [sp, #48]
	adds r3, r3, r4
	subs r3, #120
	adds r3, r3, r5
	lsls r1, r2, #3
	add r3, r8
	movs r2, #128
	ands r3, r7
	lsls r2, r2, #24
	adds r2, #92
	lsls r3, r3, #16
	orrs r3, r2
	ldr r2, [sp, #120]
	adds r0, r1, #0
	adds r0, #196
	str r3, [r2, r0]
	movs r3, #140
	ldr r0, [sp, #120]
	lsls r3, r3, #2
	movs r2, #208
	add r3, lr
	lsls r2, r2, #8
	orrs r3, r2
	adds r1, #200
	str r3, [r0, r1]
	ldr r1, [sp, #48]
	movs r3, #1
	add r11, r3
	adds r1, #1
	movs r2, #16
	mov r0, r11
	str r1, [sp, #48]
	adds r6, #16
	add lr, r2
	adds r5, #32
	cmp r0, #3
	bne .L_081b9b5c
.L_081b9bfa:
	ldr r1, [sp, #96]
	cmp r1, #5
	bne .L_081b9cca
	ldr r4, [sp, #84]
	ldr r3, [sp, #80]
	movs r2, #0
	ldr r5, [sp, #80]
	movs r6, #0
	mov r11, r2
	lsls r2, r3, #4
	lsls r3, r4, #1
	adds r3, r3, r4
	mov lr, r6
	movs r6, #128
	lsls r3, r3, #2
	movs r7, #128
	lsls r6, r6, #1
	subs r4, r2, r5
	mov r12, r3
	movs r5, #0
	lsls r7, r7, #2
	adds r6, #255
.L_081b9c26:
	ldr r0, [sp, #84]
	movs r3, #64
	cmp r0, #0
	ble .L_081b9c38
	ldr r1, [sp, #68]
	cmp r1, #1
	bne .L_081b9c38
	mov r3, r12
	adds r3, #64
.L_081b9c38:
	ldr r2, [sp, #48]
	adds r3, r3, r4
	subs r3, #120
	adds r3, r3, r5
	lsls r1, r2, #3
	adds r3, r3, r7
	movs r2, #128
	ands r3, r6
	lsls r2, r2, #24
	adds r2, #52
	lsls r3, r3, #16
	orrs r3, r2
	ldr r2, [sp, #120]
	adds r0, r1, #0
	adds r0, #196
	str r3, [r2, r0]
	movs r3, #146
	lsls r3, r3, #1
	movs r2, #224
	ldr r0, [sp, #120]
	lsls r2, r2, #8
	add r3, lr
	orrs r3, r2
	adds r1, #200
	str r3, [r0, r1]
	ldr r1, [sp, #48]
	ldr r2, [sp, #84]
	adds r1, #1
	str r1, [sp, #48]
	movs r3, #64
	cmp r2, #0
	ble .L_081b9c82
	ldr r0, [sp, #68]
	cmp r0, #0
	bne .L_081b9c82
	mov r3, r12
	adds r3, #64
.L_081b9c82:
	ldr r2, [sp, #48]
	adds r3, r3, r4
	subs r3, #120
	adds r3, r3, r5
	lsls r1, r2, #3
	adds r3, r3, r7
	movs r2, #128
	ands r3, r6
	lsls r2, r2, #24
	adds r2, #84
	lsls r3, r3, #16
	orrs r3, r2
	ldr r2, [sp, #120]
	adds r0, r1, #0
	adds r0, #196
	str r3, [r2, r0]
	movs r3, #186
	ldr r0, [sp, #120]
	lsls r3, r3, #1
	movs r2, #240
	add r3, lr
	lsls r2, r2, #8
	orrs r3, r2
	adds r1, #200
	str r3, [r0, r1]
	ldr r1, [sp, #48]
	movs r3, #1
	add r11, r3
	adds r1, #1
	movs r2, #16
	mov r0, r11
	str r1, [sp, #48]
	add lr, r2
	adds r5, #32
	cmp r0, #5
	bne .L_081b9c26
.L_081b9cca:
	ldr r1, [sp, #96]
	cmp r1, #3
	beq .L_081b9cde
	cmp r1, #5
	beq .L_081b9cd6
	b .L_081b9e70
.L_081b9cd6:
	ldr r2, [sp, #100]
	cmp r2, #0
	bne .L_081b9cde
	b .L_081b9e70
.L_081b9cde:
	ldr r3, [sp, #84]
	ldr r6, .L_081b9f04
	lsls r3, r3, #4
	mov r8, r3
	add r6, r8
	movs r7, #48
	cmp r6, #100
	ble .L_081b9cf0
	movs r6, #100
.L_081b9cf0:
	ldr r4, [sp, #96]
	cmp r4, #5
	bne .L_081b9d28
	ldr r6, [sp, #80]
	movs r0, #128
	lsls r5, r6, #12
	lsls r0, r0, #5
	adds r5, r5, r0
	adds r0, r5, #0
	bl Trig_Cos
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	mov r1, r8
	asrs r3, r3, #16
	subs r3, r3, r1
	adds r0, r5, #0
	adds r6, r3, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	asrs r3, r3, #16
	adds r6, #56
	subs r7, r7, r3
	b .L_081b9d90
.L_081b9d28:
	ldr r2, [sp, #100]
	cmp r2, #0
	beq .L_081b9d66
	ldr r3, [sp, #84]
	cmp r3, #67
	ble .L_081b9d90
	adds r2, r3, #0
	movs r3, #160
	lsls r3, r3, #3
	subs r2, #68
	adds r3, #209
	adds r5, r2, #0
	muls r5, r3
	adds r0, r5, #0
	bl Trig_Cos
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	asrs r3, r3, #16
	adds r0, r5, #0
	adds r6, r3, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	asrs r3, r3, #16
	adds r6, #52
	subs r7, r7, r3
	b .L_081b9d90
.L_081b9d66:
	ldr r4, [sp, #84]
	cmp r4, #67
	ble .L_081b9d90
	ldr r1, .L_081b9f08
	lsls r0, r4, #12
	adds r0, r0, r1
	lsls r5, r4, #1
	bl Trig_Sin
	adds r6, r5, #0
	subs r6, #128
	adds r3, r6, #0
	muls r3, r0
	ldr r2, [sp, #84]
	asrs r3, r3, #16
	adds r6, r3, #0
	ldr r3, .L_081b9f0c
	adds r5, r5, r2
	lsls r5, r5, #1
	adds r6, #100
	adds r7, r5, r3
.L_081b9d90:
	movs r4, #31
	negs r4, r4
	cmp r6, r4
	blt .L_081b9dae
	movs r3, #12
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	ldr r0, [sp, #48]
	adds r1, r6, #0
	adds r2, r7, #0
	ldr r3, [sp, #104]
	bl Func_081b8274
	str r0, [sp, #48]
.L_081b9dae:
	ldr r5, [sp, #84]
	cmp r5, #30
	bgt .L_081b9dc0
	ldr r6, [sp, #96]
	cmp r6, #5
	bne .L_081b9e70
	ldr r0, [sp, #100]
	cmp r0, #0
	beq .L_081b9e70
.L_081b9dc0:
	ldr r7, .L_081b9f10
	ldr r1, [sp, #84]
	movs r0, #64
	add r7, r8
	cmp r1, #37
	ble .L_081b9dce
	movs r7, #60
.L_081b9dce:
	ldr r2, [sp, #96]
	cmp r2, #5
	bne .L_081b9de4
	ldr r4, [sp, #80]
	movs r0, #64
	lsls r3, r4, #1
	adds r3, r3, r4
	lsls r3, r3, #3
	adds r7, r3, #0
	adds r7, #60
	b .L_081b9e10
.L_081b9de4:
	ldr r5, [sp, #84]
	cmp r5, #67
	ble .L_081b9e10
	ldr r1, [sp, #84]
	ldr r2, .L_081b9f14
	lsls r0, r1, #12
	adds r0, r0, r2
	bl Trig_Sin
	lsls r5, r5, #1
	adds r6, r5, #0
	subs r6, #136
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #16
	adds r7, r3, #0
	ldr r3, [sp, #84]
	ldr r4, .L_081b9f18
	adds r5, r5, r3
	lsls r5, r5, #1
	adds r7, #60
	adds r0, r5, r4
.L_081b9e10:
	movs r6, #128
	ldr r5, [sp, #48]
	lsls r6, r6, #2
	movs r2, #128
	adds r3, r7, r6
	lsls r2, r2, #1
	subs r6, #1
	ands r3, r6
	adds r0, r0, r2
	movs r2, #255
	ands r0, r2
	lsls r1, r5, #3
	ldr r2, [sp, #120]
	lsls r3, r3, #16
	movs r4, #192
	adds r5, r1, #0
	orrs r3, r0
	lsls r4, r4, #24
	adds r5, #196
	orrs r3, r4
	str r3, [r2, r5]
	movs r3, #210
	adds r1, #200
	lsls r3, r3, #8
	str r3, [r2, r1]
	ldr r3, [sp, #48]
	movs r5, #144
	adds r3, #1
	lsls r5, r5, #2
	str r3, [sp, #48]
	lsls r2, r3, #3
	adds r3, r7, r5
	ands r3, r6
	lsls r3, r3, #16
	ldr r6, [sp, #120]
	adds r1, r2, #0
	orrs r3, r0
	orrs r3, r4
	adds r1, #196
	str r3, [r6, r1]
	movs r3, #210
	lsls r3, r3, #8
	adds r2, #200
	adds r3, #64
	str r3, [r6, r2]
	ldr r0, [sp, #48]
	adds r0, #1
	str r0, [sp, #48]
.L_081b9e70:
	movs r1, #0
	mov r11, r1
	add r3, sp, #140
.L_081b9e76:
	movs r4, #1
	mov r2, r11
	add r11, r4
	mov r5, r11
	stmia r3!, {r2}
	cmp r5, #4
	bne .L_081b9e76
	mov r6, sp
	adds r6, #144
	str r6, [sp, #24]
	add r7, sp, #140
	mov r11, r4
	mov r12, r7
.L_081b9e90:
	ldr r1, [sp, #24]
	movs r2, #1
	ldmia r1!, {r5}
	negs r2, r2
	adds r0, r1, #0
	add r2, r11
	str r0, [sp, #24]
	mov r9, r2
	cmp r2, #0
	blt .L_081b9efc
	lsls r0, r2, #2
	ldr r3, [r7, r0]
	lsls r2, r3, #3
	subs r2, r2, r3
	lsls r3, r5, #3
	subs r3, r3, r5
	lsls r3, r3, #2
	adds r1, r3, #0
	ldr r3, [sp, #124]
	lsls r2, r2, #2
	adds r2, #8
	adds r1, #8
	ldr r2, [r3, r2]
	ldr r3, [r3, r1]
	cmp r2, r3
	ble .L_081b9f20
	mov r4, r12
	mov lr, r1
	adds r1, r0, r4
	adds r4, r0, #0
.L_081b9ecc:
	movs r6, #1
	ldr r3, [r1]
	negs r6, r6
	add r9, r6
	mov r0, r9
	str r3, [r1, #4]
	subs r4, #4
	subs r1, #4
	cmp r0, #0
	blt .L_081b9f1c
	ldr r2, [r1]
	ldr r6, [sp, #124]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, #8
	ldr r2, [r6, r3]
	mov r3, lr
	ldr r3, [r6, r3]
	adds r0, r4, #0
	mov r8, r3
	cmp r2, r8
	bgt .L_081b9ecc
	b .L_081b9f20
.L_081b9efc:
	mov r4, r9
	lsls r0, r4, #2
	b .L_081b9f20
	.2byte 0x0000
.L_081b9f04:
	.4byte 0xfffffe88
.L_081b9f08:
	.4byte 0xfffc0000
.L_081b9f0c:
	.4byte 0xfffffe98
.L_081b9f10:
	.4byte 0xfffffd94
.L_081b9f14:
	.4byte 0xfffbc000
.L_081b9f18:
	.4byte 0xfffffea8
.L_081b9f1c:
	mov r6, r9
	lsls r0, r6, #2
.L_081b9f20:
	adds r3, r0, #4
	movs r0, #1
	add r11, r0
	mov r1, r11
	str r5, [r7, r3]
	cmp r1, #4
	bne .L_081b9e90
	ldr r5, [sp, #48]
	add r3, sp, #140
	mov r8, r3
	lsls r3, r5, #3
	adds r6, r3, #0
	mov r0, sp
	adds r6, #200
	adds r0, #196
	str r6, [sp, #28]
	str r0, [sp, #40]
	movs r2, #0
	adds r3, #196
	mov r11, r2
	add r4, sp, #156
	mov r9, r3
.L_081b9f4c:
	mov r1, r8
	ldr r2, [r1]
	adds r1, r4, #0
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r2, [sp, #124]
	lsls r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	mov r0, r10
	str r4, [sp, #12]
	bl Render_ProjectPoint
	ldr r4, [sp, #12]
	movs r3, #94
	ldr r2, [r4, #8]
	adds r3, #255
	cmp r2, r3
	bgt .L_081b9f78
	adds r3, #1
	str r3, [r4, #8]
	adds r2, r3, #0
.L_081b9f78:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #138
	cmp r2, r3
	ble .L_081b9f86
	str r3, [r4, #8]
	adds r2, r3, #0
.L_081b9f86:
	mov r5, r8
	ldr r3, [r5]
	ldr r6, .L_081ba2f0
	ldr r0, [sp, #40]
	lsls r3, r3, #2
	adds r2, r2, r6
	str r2, [r0, r3]
	ldr r7, [r5]
	lsls r3, r7, #2
	ldr r5, [r0, r3]
	adds r3, r5, #0
	cmp r5, #0
	bge .L_081b9fa2
	adds r3, r5, #3
.L_081b9fa2:
	asrs r3, r3, #2
	adds r6, r3, #0
	adds r6, #64
	movs r0, #192
	adds r1, r6, #0
	lsls r0, r0, #4
	str r4, [sp, #12]
	bl Math_Div
	ldr r4, [sp, #12]
	adds r1, r6, #0
	ldr r3, [r4]
	str r4, [sp, #12]
	subs r2, r3, r0
	movs r0, #128
	lsls r0, r0, #4
	str r2, [sp, #16]
	bl Math_Div
	ldr r4, [sp, #12]
	movs r1, #16
	ldr r3, [r4, #4]
	negs r1, r1
	subs r0, r3, r0
	adds r3, r0, #0
	adds r3, #92
	ldr r2, [sp, #16]
	cmp r3, r1
	ble .L_081ba038
	movs r5, #158
	lsls r5, r5, #2
	adds r3, r2, r5
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	ands r3, r2
	movs r6, #174
	lsls r3, r3, #16
	lsls r1, r7, #25
	lsls r6, r6, #1
	orrs r1, r3
	movs r2, #255
	adds r3, r0, r6
	ands r3, r2
	orrs r1, r3
	ldr r3, .L_081ba2f4
	ldr r0, [sp, #120]
	orrs r1, r3
	mov r2, r9
	str r1, [r0, r2]
	mov r3, r10
	ldr r0, [r3, #24]
	ldr r2, .L_081ba2f8
	cmp r0, #0
	bge .L_081ba018
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #255
	adds r0, r0, r5
.L_081ba018:
	asrs r3, r0, #10
	ldrb r3, [r2, r3]
	ldr r0, [sp, #28]
	movs r2, #128
	ldr r6, [sp, #120]
	lsls r2, r2, #3
	lsls r3, r3, #3
	orrs r3, r2
	str r3, [r6, r0]
	ldr r2, [sp, #48]
	adds r0, #8
	adds r2, #1
	str r0, [sp, #28]
	str r2, [sp, #48]
	movs r1, #8
	add r9, r1
.L_081ba038:
	movs r5, #1
	add r11, r5
	movs r3, #4
	mov r6, r11
	add r8, r3
	cmp r6, #4
	bne .L_081b9f4c
	ldr r5, [sp, #48]
	movs r2, #128
	add r3, sp, #212
	lsls r2, r2, #20
	movs r1, #168
	mov r8, r3
	str r2, [sp, #32]
	lsls r3, r5, #3
	ldr r4, [sp, #124]
	movs r0, #0
	add r1, sp
	adds r3, #196
	mov r11, r0
	mov r10, r1
	add r6, sp, #156
	mov r9, r3
.L_081ba066:
	ldr r3, [r4]
	mov r0, r10
	str r3, [r0]
	movs r3, #0
	str r3, [r0, #4]
	adds r1, r6, #0
	ldr r3, [r4, #8]
	str r4, [sp, #12]
	str r3, [r0, #8]
	bl Render_ProjectPoint
	ldr r3, [r6, #8]
	movs r1, #94
	adds r1, #255
	ldr r4, [sp, #12]
	cmp r3, r1
	bgt .L_081ba08e
	movs r3, #175
	lsls r3, r3, #1
	str r3, [r6, #8]
.L_081ba08e:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #138
	cmp r3, r2
	ble .L_081ba09c
	str r2, [r6, #8]
	adds r3, r2, #0
.L_081ba09c:
	ldr r2, .L_081ba2f0
	mov r1, r8
	adds r5, r3, r2
	mov r3, r8
	str r5, [r3]
	str r4, [sp, #12]
	movs r0, #6
	ldrsh r3, [r4, r0]
	movs r0, #192
	adds r5, r5, r3
	lsrs r3, r5, #31
	str r5, [r1]
	adds r5, r5, r3
	asrs r5, r5, #1
	adds r5, #128
	adds r1, r5, #0
	lsls r0, r0, #5
	bl Math_Div
	ldr r3, [r6]
	adds r1, r5, #0
	subs r7, r3, r0
	movs r0, #192
	lsls r0, r0, #4
	bl Math_Div
	ldr r3, [r6, #4]
	movs r2, #16
	subs r1, r3, r0
	adds r3, r1, #0
	adds r3, #92
	negs r2, r2
	ldr r4, [sp, #12]
	cmp r3, r2
	ble .L_081ba122
	ldr r3, [sp, #48]
	movs r5, #158
	movs r2, #128
	lsls r5, r5, #2
	lsls r2, r2, #1
	lsls r0, r3, #3
	adds r2, #255
	adds r3, r7, r5
	ands r3, r2
	ldr r2, [sp, #32]
	movs r5, #174
	lsls r5, r5, #1
	lsls r3, r3, #16
	orrs r3, r2
	adds r2, r1, r5
	movs r1, #255
	ands r2, r1
	orrs r3, r2
	ldr r2, .L_081ba2f4
	ldr r1, [sp, #120]
	orrs r3, r2
	mov r2, r9
	str r3, [r1, r2]
	movs r3, #137
	lsls r3, r3, #4
	adds r0, #200
	str r3, [r1, r0]
	ldr r5, [sp, #48]
	movs r3, #8
	adds r5, #1
	str r5, [sp, #48]
	add r9, r3
.L_081ba122:
	ldr r0, [sp, #32]
	movs r1, #128
	movs r3, #1
	lsls r1, r1, #18
	add r11, r3
	adds r0, r0, r1
	movs r2, #4
	mov r5, r11
	str r0, [sp, #32]
	add r8, r2
	adds r4, #28
	cmp r5, #4
	bne .L_081ba066
	ldr r6, [sp, #48]
	cmp r6, #128
	beq .L_081ba162
	lsls r3, r6, #3
	ldr r0, .L_081ba2fc
	adds r2, r3, #0
	movs r1, #0
	adds r2, #200
	adds r3, #196
.L_081ba14e:
	ldr r4, [sp, #120]
	str r0, [r4, r3]
	str r1, [r4, r2]
	ldr r5, [sp, #48]
	adds r2, #8
	adds r5, #1
	adds r3, #8
	str r5, [sp, #48]
	cmp r5, #128
	bne .L_081ba14e
.L_081ba162:
	mov r3, sp
	adds r3, #196
	str r3, [sp, #8]
	movs r6, #0
	movs r0, #224
	movs r1, #216
	movs r2, #208
	mov r11, r6
	mov r10, r6
	mov r8, r0
	add r6, sp, #132
	mov lr, r1
	mov r12, r2
	movs r7, #200
.L_081ba17e:
	ldr r5, [sp, #8]
	ldmia r5!, {r3}
	adds r4, r5, #0
	adds r1, r3, #0
	str r4, [sp, #8]
	cmp r1, #0
	bge .L_081ba18e
	adds r3, r1, #3
.L_081ba18e:
	asrs r3, r3, #2
	adds r3, #128
	lsls r3, r3, #16
	mov r2, sp
	asrs r3, r3, #16
	add r0, sp, #136
	adds r2, #138
	mov r4, r10
	strh r3, [r2]
	strh r4, [r0]
	mov r4, sp
	adds r4, #134
	mov r5, r10
	strh r5, [r4]
	strh r3, [r6]
	ldr r1, [sp, #120]
	ldrh r2, [r2]
	ldr r3, [r1, r7]
	lsls r2, r2, #16
	orrs r3, r2
	str r3, [r1, r7]
	mov r5, r12
	ldrh r2, [r0]
	ldr r3, [r1, r5]
	lsls r2, r2, #16
	orrs r3, r2
	str r3, [r1, r5]
	mov r0, lr
	ldrh r2, [r4]
	ldr r3, [r1, r0]
	lsls r2, r2, #16
	orrs r3, r2
	str r3, [r1, r0]
	mov r4, r8
	ldrh r2, [r6]
	ldr r3, [r1, r4]
	lsls r2, r2, #16
	movs r0, #1
	orrs r3, r2
	add r11, r0
	movs r5, #32
	str r3, [r1, r4]
	mov r1, r11
	add r8, r5
	add lr, r5
	add r12, r5
	adds r7, #32
	cmp r1, #8
	bne .L_081ba17e
	ldr r0, [sp, #120]
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #224
	adds r3, #212
	adds r0, #196
	lsls r1, r1, #19
	ldr r2, .L_081ba300
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #108]
	adds r2, #1
	str r2, [sp, #108]
.L_081ba210:
	ldr r4, [sp, #108]
	movs r3, #0
	str r3, [sp, #48]
	cmp r4, #16
	bgt .L_081ba23a
	movs r6, #128
	lsls r5, r4, #12
	lsls r6, r6, #1
	ldr r1, .L_081ba304
	adds r2, r5, #0
	adds r3, r6, #0
	ldr r0, [sp, #128]
	bl Graphics_ScaleRgb555BufferB
	movs r1, #160
	add r0, sp, #244
	lsls r1, r1, #19
	adds r2, r5, #0
	adds r3, r6, #0
	bl Graphics_ScaleRgb555BufferB
.L_081ba23a:
	bl Func_08014de4
	ldr r0, [sp, #116]
	ldr r1, [sp, #36]
	bl Graphics_PrepareTransferInIwramWork
	ldr r5, [sp, #96]
	cmp r5, #0
	beq .L_081ba250
	bl .L_081b8a46
.L_081ba250:
	ldr r6, [sp, #108]
	cmp r6, #17
	bgt .L_081ba25a
	bl .L_081b9550
.L_081ba25a:
	ldr r5, .L_081ba308
	bl Func_080ad290
	ldr r2, [r5, #16]
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	cmp r2, r3
	bcc .L_081ba270
	bl .L_081b890a
.L_081ba270:
	ldr r0, [sp, #120]
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #204
	adds r3, r0, r1
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
.L_081ba282:
	movs r2, #0
	movs r6, #128
	mov r11, r2
	lsls r6, r6, #1
.L_081ba28a:
	mov r4, r11
	movs r5, #128
	lsls r3, r4, #12
	lsls r5, r5, #9
	subs r5, r5, r3
	ldr r0, [sp, #128]
	adds r2, r5, #0
	ldr r1, .L_081ba304
	adds r3, r6, #0
	bl Graphics_ScaleRgb555BufferB
	movs r1, #160
	adds r2, r5, #0
	add r0, sp, #244
	lsls r1, r1, #19
	adds r3, r6, #0
	movs r5, #1
	bl Graphics_ScaleRgb555BufferB
	add r11, r5
	movs r0, #1
	bl WaitFrames
	mov r0, r11
	cmp r0, #17
	bne .L_081ba28a
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
	movs r3, #189
	lsls r3, r3, #2
	add sp, r3
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081ba2f0:
	.4byte 0xfffffea2
.L_081ba2f4:
	.4byte 0x40002300
.L_081ba2f8:
	.4byte Data_081ba30c
.L_081ba2fc:
	.4byte 0x40f02000
.L_081ba300:
	.4byte 0x84000100
.L_081ba304:
	.4byte 0x05000200
.L_081ba308:
	.4byte gPartyState
