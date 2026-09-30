.syntax unified
	.thumb
	.global Func_081b34a8
	.thumb_func
Func_081b34a8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #192
	lsls r1, r1, #3
	adds r1, #14
	movs r0, #100
	sub sp, #120
	bl Runtime_AllocateHeapBlock
	movs r1, #128
	str r0, [sp, #48]
	lsls r1, r1, #8
	movs r0, #96
	bl Runtime_AllocateHeapBlock
	movs r1, #246
	lsls r1, r1, #7
	str r0, [sp, #44]
	adds r1, #124
	movs r0, #92
	bl Runtime_AllocateBlock
	movs r1, #192
	lsls r1, r1, #3
	str r0, [sp, #40]
	adds r1, #28
	movs r0, #180
	bl Runtime_AllocateBlock
	str r0, [sp, #36]
	ldr r0, .L_081b353c
	ldr r5, .L_081b3540
	bl Func_080132fc
	movs r0, #160
	lsls r0, r0, #1
	adds r5, r5, r0
	movs r3, #255
	strb r3, [r5]
	ldr r2, [sp, #36]
	movs r3, #0
	adds r2, #162
	strh r3, [r2]
	ldr r1, [sp, #36]
	movs r3, #1
	adds r1, #152
	str r1, [sp, #32]
	str r3, [r1]
	bl Func_080144c0
	ldr r5, .L_081b3530
	ldr r3, .L_081b3544
	ldr r1, .L_081b3548
	strb r5, [r3]
	ldr r7, .L_081b3534
	ldr r4, .L_081b3538
	movs r0, #0
	mov r8, r0
	movs r6, #0
.L_081b3528:
	movs r2, #0
	mov r10, r2
	b .L_081b354c
	.2byte 0x0000
.L_081b3530:
	.4byte 0x00000000
.L_081b3534:
	.4byte 0x0000a1a6
.L_081b3538:
	.4byte 0x0000a1a8
.L_081b353c:
	.4byte 0x0000000c
.L_081b3540:
	.4byte Data_0200024c
.L_081b3544:
	.4byte Data_0300120c
.L_081b3548:
	.4byte 0x06002800
.L_081b354c:
	mov r3, r10
	subs r3, #5
	cmp r3, #19
	bhi .L_081b3564
	cmp r6, #2
	ble .L_081b3564
	cmp r6, #13
	bgt .L_081b3564
	mov r2, r8
	adds r3, r2, r1
	strh r7, [r3]
	b .L_081b357c
.L_081b3564:
	mov r3, r10
	cmp r3, #29
	ble .L_081b3572
	mov r2, r8
	adds r3, r2, r1
	strh r5, [r3]
	b .L_081b357c
.L_081b3572:
	mov r3, r8
	adds r2, r3, r1
	adds r3, r0, r4
	strh r3, [r2]
	adds r0, #1
.L_081b357c:
	movs r2, #1
	add r10, r2
	movs r3, #2
	mov r2, r10
	add r8, r3
	cmp r2, #32
	bne .L_081b354c
	adds r6, #1
	cmp r6, #20
	bne .L_081b3528
	ldr r0, .L_081b3650
	bl Resource_GetTableEntry
	ldr r1, [sp, #48]
	bl Func_0801587c
	ldr r0, .L_081b3654
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r4, r0, #0
	adds r3, #212
	ldr r1, .L_081b3658
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r4, #32
	adds r0, r4, #0
	ldr r1, .L_081b365c
	bl Func_0801587c
	ldr r0, .L_081b365c
	ldr r7, .L_081b3660
	movs r3, #0
	mov r8, r3
	movs r6, #0
	movs r5, #0
	mov r12, r0
.L_081b35ce:
	movs r1, #0
	lsls r3, r5, #6
	mov r2, r12
	mov r10, r1
	adds r4, r3, r2
.L_081b35d8:
	mov r3, r10
	subs r3, #5
	cmp r3, #19
	bhi .L_081b35e8
	cmp r6, #2
	ble .L_081b35e8
	cmp r6, #13
	ble .L_081b3602
.L_081b35e8:
	mov r2, r8
	adds r1, r2, r7
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #32
	add r8, r3
.L_081b3602:
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r4, #32
	cmp r1, #30
	bne .L_081b35d8
	adds r6, #1
	adds r5, #15
	cmp r6, #20
	bne .L_081b35ce
	movs r1, #192
	ldr r3, .L_081b3664
	ldr r0, .L_081b3668
	lsls r1, r1, #2
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_081b364c
	ldr r5, .L_081b366c
	movs r2, #0
	movs r4, #2
	mov r8, r2
	movs r6, #0
	mov r12, r3
	negs r4, r4
	movs r0, #0
.L_081b3634:
	movs r7, #0
	adds r2, r0, #0
	mov r10, r7
	adds r1, r4, #0
	adds r2, #148
.L_081b363e:
	cmp r1, #14
	bls .L_081b3670
	mov r7, r8
	adds r3, r7, r5
	mov r7, r12
	strh r7, [r3]
	b .L_081b3676
.L_081b364c:
	.4byte 0x000000bf
.L_081b3650:
	.4byte 0x00000137
.L_081b3654:
	.4byte 0x00000081
.L_081b3658:
	.4byte 0x05000140
.L_081b365c:
	.4byte gMapCellBuffer
.L_081b3660:
	.4byte 0x0600b500
.L_081b3664:
	.4byte IwramClearWords
.L_081b3668:
	.4byte 0x06002d00
.L_081b366c:
	.4byte 0x06003000
.L_081b3670:
	mov r7, r8
	adds r3, r7, r5
	strh r2, [r3]
.L_081b3676:
	movs r3, #1
	add r10, r3
	movs r7, #2
	mov r3, r10
	adds r2, #1
	add r8, r7
	cmp r3, #32
	bne .L_081b363e
	adds r6, #1
	adds r4, #1
	adds r0, #32
	cmp r6, #20
	bne .L_081b3634
	ldr r3, .L_081b36c4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	strh r3, [r2]
	ldr r3, .L_081b36c8
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_081b36d8
	movs r1, #0
	strh r1, [r3]
	strh r1, [r3, #2]
	strh r1, [r3, #4]
	strh r1, [r3, #6]
	strh r1, [r3, #8]
	strh r1, [r3, #10]
	ldr r3, .L_081b36cc
	adds r2, #60
	strh r3, [r2]
	ldr r3, .L_081b36d0
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_081b36d4
	adds r2, #6
	b .L_081b36dc
	.2byte 0x0000
.L_081b36c4:
	.4byte 0x00000509
.L_081b36c8:
	.4byte 0x00000680
.L_081b36cc:
	.4byte 0x00003737
.L_081b36d0:
	.4byte 0x00002727
.L_081b36d4:
	.4byte 0x00003f44
.L_081b36d8:
	.4byte Data_03001120
.L_081b36dc:
	strh r3, [r2]
	ldr r3, .L_081b3714
	adds r2, #2
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	ldr r2, .L_081b3718
	adds r3, #20
	strh r1, [r3]
	adds r3, #4
	strh r1, [r3]
	subs r3, #2
	strh r2, [r3]
	adds r3, #4
	strh r2, [r3]
	movs r0, #128
	ldr r3, .L_081b371c
	lsls r0, r0, #19
	adds r0, #64
	strh r3, [r0]
	ldr r2, .L_081b3720
	ldr r3, .L_081b3724
	movs r4, #128
	lsls r4, r4, #19
	adds r4, #68
	strh r3, [r4]
	strh r2, [r0]
	b .L_081b3728
.L_081b3714:
	.4byte 0x00001010
.L_081b3718:
	.4byte 0x0000ff60
.L_081b371c:
	.4byte 0x000028c8
.L_081b3720:
	.4byte 0x000000f0
.L_081b3724:
	.4byte 0x00001878
.L_081b3728:
	movs r3, #128
	ldr r0, .L_081b3768
	lsls r3, r3, #19
	adds r3, #66
	strh r0, [r4]
	strh r2, [r3]
	adds r3, #4
	strh r0, [r3]
	ldr r7, [sp, #40]
	ldr r3, [sp, #36]
	movs r0, #224
	movs r2, #144
	lsls r0, r0, #3
	lsls r2, r2, #4
	adds r0, r7, r0
	adds r2, r7, r2
	adds r3, #140
	str r0, [sp, #28]
	str r2, [sp, #24]
	str r3, [sp, #20]
	str r1, [r3]
	ldr r3, [sp, #36]
	movs r0, #238
	adds r3, #144
	str r1, [r3]
	ldr r3, [sp, #36]
	lsls r0, r0, #7
	adds r3, #148
	adds r0, #140
	str r1, [r3]
	adds r3, r7, r0
	b .L_081b376c
.L_081b3768:
	.4byte 0x000000a0
.L_081b376c:
	str r1, [r3]
	ldr r2, [sp, #36]
	ldr r0, .L_081b37d0
	adds r2, #164
	str r2, [sp, #16]
	str r1, [r2]
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_081b37c8
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #128
	strh r3, [r2]
	ldr r3, .L_081b37cc
	adds r2, #2
	strh r3, [r2]
	ldr r0, .L_081b37d4
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r4, r0, #0
	adds r3, #212
	ldr r1, .L_081b37d8
	adds r2, #120
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #240
	lsls r3, r3, #1
	adds r4, r4, r3
	adds r0, r4, #0
	ldr r1, .L_081b37dc
	b .L_081b37e0
	.2byte 0x0000
.L_081b37c8:
	.4byte 0x00002f8b
.L_081b37cc:
	.4byte 0x00005bf6
.L_081b37d0:
	.4byte 0x00000152
.L_081b37d4:
	.4byte 0x00000082
.L_081b37d8:
	.4byte 0x05000200
.L_081b37dc:
	.4byte gMapCellBuffer
.L_081b37e0:
	bl Func_0801587c
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081b38a0
	ldr r1, .L_081b38a4
	ldr r2, .L_081b38a8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_081b38ac
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r4, r0, #0
	adds r3, #212
	ldr r1, .L_081b38b0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r4, #32
	adds r0, r4, #0
	ldr r1, .L_081b38a0
	bl Func_0801587c
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081b38a0
	ldr r1, .L_081b38b4
	ldr r2, .L_081b38b8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Resource_FarCall004
	bl Func_081b336c
	movs r7, #0
	mov r10, r7
	ldr r7, [sp, #36]
.L_081b3836:
	movs r3, #8
	str r3, [r7]
	movs r3, #0
	strb r3, [r7, #25]
	movs r3, #255
	strb r3, [r7, #26]
	movs r6, #0
	adds r5, r7, #4
.L_081b3846:
	bl Random16
	movs r1, #5
	bl Math_ModU
	adds r6, #1
	strb r0, [r5]
	adds r5, #1
	cmp r6, #21
	bne .L_081b3846
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r7, #28
	cmp r1, #5
	bne .L_081b3836
	ldr r3, [sp, #36]
	movs r2, #0
	add r7, sp, #88
	mov r10, r2
	adds r4, r7, #0
	mov r9, r3
.L_081b3872:
	movs r0, #0
	mov r8, r0
	movs r5, #0
.L_081b3878:
	str r4, [sp, #8]
	bl Random16
	movs r1, #21
	bl Math_ModU
	mov r1, r8
	str r0, [r5, r7]
	movs r6, #0
	ldr r4, [sp, #8]
	cmp r1, #0
	beq .L_081b38d4
	ldr r3, [r7]
	cmp r0, r3
	bne .L_081b38bc
	movs r2, #1
	negs r2, r2
	subs r5, #4
	add r8, r2
	b .L_081b38d4
.L_081b38a0:
	.4byte gMapCellBuffer
.L_081b38a4:
	.4byte 0x06010000
.L_081b38a8:
	.4byte 0x84001b30
.L_081b38ac:
	.4byte 0x00000083
.L_081b38b0:
	.4byte 0x050003e0
.L_081b38b4:
	.4byte 0x06016e00
.L_081b38b8:
	.4byte 0x84000480
.L_081b38bc:
	adds r6, #1
	cmp r6, r8
	beq .L_081b38d4
	lsls r3, r6, #2
	ldr r2, [r5, r4]
	ldr r3, [r4, r3]
	cmp r2, r3
	bne .L_081b38bc
	movs r3, #1
	negs r3, r3
	subs r5, #4
	add r8, r3
.L_081b38d4:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #4
	cmp r1, #8
	bne .L_081b3878
	movs r6, #0
	adds r1, r7, #0
.L_081b38e4:
	adds r2, r6, #0
	cmp r6, #5
	ble .L_081b38ec
	movs r2, #5
.L_081b38ec:
	ldmia r1!, {r3}
	mov r0, r9
	adds r3, #4
	adds r6, #1
	strb r2, [r0, r3]
	cmp r6, #8
	bne .L_081b38e4
	movs r2, #1
	add r10, r2
	movs r1, #28
	mov r3, r10
	add r9, r1
	cmp r3, #5
	bne .L_081b3872
	movs r3, #1
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #7
	movs r3, #3
	movs r0, #104
	bl Func_08138000
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #104]
	movs r1, #8
	str r3, [sp, #52]
	movs r3, #0
	str r3, [sp, #0]
	movs r2, #7
	movs r3, #3
	movs r0, #188
	bl Func_08138000
	adds r5, #188
	ldr r3, [r5]
	mov r7, sp
	adds r7, #52
	str r7, [sp, #12]
	movs r1, #128
	str r3, [r7, #4]
	ldr r0, [sp, #44]
	ldr r3, .L_081b39d4
	lsls r1, r1, #8
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, [sp, #44]
	ldr r1, .L_081b39d8
	ldr r2, .L_081b39dc
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	movs r0, #160
	lsls r2, r2, #24
	lsls r0, r0, #19
	ldr r1, [sp, #28]
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, .L_081b39e0
	ldr r1, [sp, #24]
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r5, #128
	lsls r5, r5, #1
	ldr r1, .L_081b39e0
	ldr r0, [sp, #24]
	movs r2, #0
	adds r3, r5, #0
	bl Graphics_ScaleRgb555
	movs r1, #160
	movs r2, #0
	adds r3, r5, #0
	lsls r1, r1, #19
	ldr r0, [sp, #28]
	bl Graphics_ScaleRgb555
	ldr r3, .L_081b39d0
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	movs r0, #228
	bl PartyInventory_CountItemFar
	cmp r0, #1
	bne .L_081b39ea
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #26
	movs r3, #4
	movs r0, #2
	bl UiWindow_CreateFar
	adds r1, r0, #0
	ldr r0, [sp, #36]
	movs r2, #153
	lsls r2, r2, #3
	adds r3, r0, r2
	str r1, [r3]
	ldr r0, .L_081b39e4
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_081b39e8
.L_081b39d0:
	.4byte 0x00003740
.L_081b39d4:
	.4byte IwramFillWords
.L_081b39d8:
	.4byte 0x06003500
.L_081b39dc:
	.4byte 0x84002000
.L_081b39e0:
	.4byte 0x05000200
.L_081b39e4:
	.4byte 0x00000d6c
.L_081b39e8:
	b .L_081b3a20
.L_081b39ea:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #26
	movs r3, #4
	movs r0, #2
	bl UiWindow_CreateFar
	ldr r3, [sp, #36]
	ldr r5, .L_081b3c68
	movs r7, #153
	lsls r7, r7, #3
	adds r1, r0, #0
	adds r6, r3, r7
	adds r0, r5, #0
	str r1, [r6]
	movs r2, #0
	movs r3, #0
	adds r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	ldr r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
.L_081b3a20:
	ldr r0, [sp, #40]
	movs r1, #240
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #0
	movs r1, #144
	movs r5, #200
	str r3, [r2]
	lsls r1, r1, #3
	lsls r5, r5, #4
	ldr r0, .L_081b3c6c
	bl Scheduler_AddOrUpdateCallback
	adds r1, r5, #0
	ldr r0, .L_081b3c70
	bl Scheduler_AddOrUpdateCallback
	ldr r0, .L_081b3c74
	adds r1, r5, #0
	bl Scheduler_AddOrUpdateCallback
	ldr r7, [sp, #20]
	movs r2, #0
	ldr r3, [r7]
	mov r11, r2
	cmp r3, #10
	bne .L_081b3a5a
	b .L_081b3da2
.L_081b3a5a:
	mov r0, r11
	cmp r0, #16
	bgt .L_081b3a80
	movs r6, #128
	lsls r5, r0, #12
	lsls r6, r6, #1
	ldr r1, .L_081b3c78
	adds r2, r5, #0
	adds r3, r6, #0
	ldr r0, [sp, #24]
	bl Graphics_ScaleRgb555
	movs r1, #160
	ldr r0, [sp, #28]
	lsls r1, r1, #19
	adds r2, r5, #0
	adds r3, r6, #0
	bl Graphics_ScaleRgb555
.L_081b3a80:
	ldr r1, [sp, #20]
	ldr r3, [r1]
	cmp r3, #3
	beq .L_081b3a8a
	b .L_081b3c12
.L_081b3a8a:
	mov r0, r11
	movs r1, #80
	bl __modsi3
	cmp r0, #15
	bgt .L_081b3a9e
	ldr r0, .L_081b3c7c
	bl Func_081b21ec
	b .L_081b3ac8
.L_081b3a9e:
	cmp r0, #31
	bgt .L_081b3aaa
	ldr r0, .L_081b3c80
	bl Func_081b21ec
	b .L_081b3ac8
.L_081b3aaa:
	cmp r0, #47
	bgt .L_081b3ab6
	ldr r0, .L_081b3c84
	bl Func_081b21ec
	b .L_081b3ac8
.L_081b3ab6:
	cmp r0, #63
	bgt .L_081b3ac2
	ldr r0, .L_081b3c88
	bl Func_081b21ec
	b .L_081b3ac8
.L_081b3ac2:
	ldr r0, .L_081b3c8c
	bl Func_081b21ec
.L_081b3ac8:
	ldr r2, [sp, #16]
	ldr r3, [r2]
	cmp r3, #15
	bgt .L_081b3ad8
	bl Func_081b2150
	ldr r7, [sp, #16]
	ldr r3, [r7]
.L_081b3ad8:
	cmp r3, #16
	ble .L_081b3b7a
	movs r3, #7
	mov r0, r11
	ands r3, r0
	cmp r3, #0
	bne .L_081b3b7a
	bl Random16
	movs r3, #127
	ands r3, r0
	adds r5, r3, #0
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r1, r3, #0
	mov r2, r11
	adds r5, #56
	adds r1, #48
	cmp r2, #0
	bge .L_081b3b06
	adds r2, #7
.L_081b3b06:
	movs r3, #3
	asrs r2, r2, #3
	ands r2, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r2, .L_081b3c90
	lsls r3, r3, #10
	adds r7, r3, r2
	lsls r5, r5, #16
	movs r3, #0
	lsls r1, r1, #16
	mov r8, r3
	mov r9, r5
	mov r10, r1
.L_081b3b22:
	bl Random16
	movs r5, #255
	ands r5, r0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r6, r0, #0
	ands r6, r3
	mov r1, r10
	mov r0, r9
	str r1, [r7, #4]
	str r0, [r7]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #64
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #16
	str r3, [r7, #24]
	movs r2, #1
	movs r3, #128
	add r8, r2
	lsls r3, r3, #1
	adds r7, #28
	cmp r8, r3
	bne .L_081b3b22
.L_081b3b7a:
	movs r7, #0
	mov r8, r7
	ldr r7, .L_081b3c90
.L_081b3b80:
	ldr r0, [r7, #24]
	cmp r0, #0
	ble .L_081b3c04
	ldr r3, [r7]
	ldr r1, .L_081b3c94
	subs r0, #1
	str r0, [r7, #24]
	cmp r3, r1
	bhi .L_081b3bd6
	ldr r6, [r7, #4]
	ldr r2, .L_081b3c98
	cmp r6, r2
	bgt .L_081b3bd6
	cmp r6, #0
	blt .L_081b3bd6
	movs r1, #12
	asrs r5, r3, #16
	bl Math_Div
	adds r0, #1
	ldr r1, .L_081b3c9c
	lsls r4, r0, #1
	mov r3, r8
	movs r2, #1
	ands r2, r3
	asrs r6, r6, #16
	subs r3, r4, #2
	ldrh r1, [r1, r3]
	subs r5, r5, r0
	ldr r3, [sp, #48]
	subs r6, r6, r0
	str r4, [sp, #0]
	ldr r0, [sp, #12]
	str r4, [sp, #4]
	lsls r2, r2, #2
	ldr r4, [r2, r0]
	adds r1, r3, r1
	ldr r0, [sp, #44]
	adds r3, r6, #0
	adds r2, r5, #0
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r7]
.L_081b3bd6:
	ldr r2, [r7, #12]
	ldr r1, [r7, #16]
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [r7, #4]
	adds r3, r3, r1
	str r3, [r7, #4]
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_081b3bf0
	adds r3, #63
.L_081b3bf0:
	asrs r3, r3, #6
	str r3, [r7, #12]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_081b3c00
	adds r3, #63
.L_081b3c00:
	asrs r3, r3, #6
	str r3, [r7, #16]
.L_081b3c04:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #3
	adds r7, #28
	cmp r8, r2
	bne .L_081b3b80
.L_081b3c12:
	ldr r7, [sp, #20]
	ldr r3, [r7]
	cmp r3, #0
	beq .L_081b3c20
	cmp r3, #2
	beq .L_081b3c20
	b .L_081b3d80
.L_081b3c20:
	movs r0, #0
	add r5, sp, #60
	mov r8, r0
	movs r2, #0
	adds r3, r5, #0
.L_081b3c2a:
	movs r1, #1
	add r8, r1
	mov r7, r8
	stmia r3!, {r2}
	cmp r7, #7
	bne .L_081b3c2a
	ldr r0, [sp, #20]
	ldr r3, [r0]
	cmp r3, #0
	bne .L_081b3ca0
	movs r2, #1
	str r2, [r5, #12]
	ldr r1, [sp, #32]
	ldr r3, [r1]
	cmp r3, #1
	ble .L_081b3c50
	str r2, [r5, #16]
	str r2, [r5, #8]
	ldr r3, [r1]
.L_081b3c50:
	cmp r3, #2
	ble .L_081b3c5c
	str r2, [r5, #20]
	str r2, [r5, #4]
	ldr r7, [sp, #32]
	ldr r3, [r7]
.L_081b3c5c:
	cmp r3, #3
	ble .L_081b3cc4
	str r2, [r5, #24]
	str r2, [r5]
	b .L_081b3cc4
	.2byte 0x0000
.L_081b3c68:
	.4byte 0x00000d6b
.L_081b3c6c:
	.4byte Func_081b243c
.L_081b3c70:
	.4byte Func_081b3294
.L_081b3c74:
	.4byte Func_081b20a0
.L_081b3c78:
	.4byte 0x05000200
.L_081b3c7c:
	.4byte 0x00000154
.L_081b3c80:
	.4byte 0x00000156
.L_081b3c84:
	.4byte 0x00000178
.L_081b3c88:
	.4byte 0x00000163
.L_081b3c8c:
	.4byte 0x00000152
.L_081b3c90:
	.4byte gMapCellBuffer
.L_081b3c94:
	.4byte 0x00ffffff
.L_081b3c98:
	.4byte 0x007fffff
.L_081b3c9c:
	.4byte Data_081b4888
.L_081b3ca0:
	mov r3, r11
	mov r0, r8
	ands r3, r0
	cmp r3, #3
	bgt .L_081b3cc4
	ldr r2, [sp, #36]
	movs r1, #0
	mov r8, r1
	adds r0, r5, #0
	adds r2, #172
.L_081b3cb4:
	ldmia r2!, {r3}
	str r3, [r1, r0]
	movs r3, #1
	add r8, r3
	mov r7, r8
	adds r1, #4
	cmp r7, #7
	bne .L_081b3cb4
.L_081b3cc4:
	movs r0, #0
	mov r8, r0
.L_081b3cc8:
	movs r2, #1
	mov r1, r8
	eors r2, r1
	negs r3, r2
	orrs r3, r2
	lsrs r6, r3, #31
	movs r3, #65
	subs r6, r3, r6
	ldr r3, [r5, #4]
	cmp r3, #0
	beq .L_081b3cee
	mov r3, r8
	adds r3, #19
	movs r0, #20
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl Func_081b22b8
.L_081b3cee:
	ldr r3, [r5, #8]
	cmp r3, #0
	beq .L_081b3d04
	mov r3, r8
	adds r3, #35
	movs r0, #28
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl Func_081b22b8
.L_081b3d04:
	ldr r3, [r5, #12]
	cmp r3, #0
	beq .L_081b3d1a
	mov r3, r8
	adds r3, #51
	movs r0, #20
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl Func_081b22b8
.L_081b3d1a:
	ldr r3, [r5, #16]
	cmp r3, #0
	beq .L_081b3d30
	mov r3, r8
	adds r3, #67
	movs r0, #28
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl Func_081b22b8
.L_081b3d30:
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L_081b3d46
	mov r3, r8
	adds r3, #83
	movs r0, #20
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl Func_081b22b8
.L_081b3d46:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_081b3d5e
	mov r1, r8
	mov r3, r8
	adds r1, #5
	adds r3, #91
	movs r0, #28
	movs r2, #200
	str r6, [sp, #0]
	bl Func_081b22b8
.L_081b3d5e:
	ldr r3, [r5, #24]
	cmp r3, #0
	beq .L_081b3d76
	mov r1, r8
	mov r3, r8
	adds r1, #97
	adds r3, #11
	movs r0, #28
	movs r2, #200
	str r6, [sp, #0]
	bl Func_081b22b8
.L_081b3d76:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #3
	bne .L_081b3cc8
.L_081b3d80:
	ldr r7, [sp, #40]
	movs r0, #240
	lsls r0, r0, #7
	adds r0, #232
	adds r3, r7, r0
	movs r2, #1
	str r2, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #20]
	movs r1, #1
	ldr r3, [r2]
	add r11, r1
	cmp r3, #10
	beq .L_081b3da2
	b .L_081b3a5a
.L_081b3da2:
	movs r3, #0
	movs r6, #128
	mov r8, r3
	lsls r6, r6, #1
.L_081b3daa:
	mov r7, r8
	movs r5, #128
	lsls r3, r7, #12
	lsls r5, r5, #9
	subs r5, r5, r3
	ldr r0, [sp, #24]
	ldr r1, .L_081b3e24
	adds r2, r5, #0
	adds r3, r6, #0
	bl Graphics_ScaleRgb555
	movs r1, #160
	lsls r1, r1, #19
	ldr r0, [sp, #28]
	adds r2, r5, #0
	adds r3, r6, #0
	bl Graphics_ScaleRgb555
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #17
	bne .L_081b3daa
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_081b3e28
	bl Scheduler_RemoveCallback
	ldr r0, .L_081b3e2c
	bl Scheduler_RemoveCallback
	ldr r0, .L_081b3e30
	bl Scheduler_RemoveCallback
	movs r0, #180
	bl Runtime_ReleaseHeapBlock
	movs r0, #96
	bl Runtime_ReleaseHeapBlock
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	movs r0, #100
	bl Runtime_ReleaseHeapBlock
	add sp, #120
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081b3e24:
	.4byte 0x05000200
.L_081b3e28:
	.4byte Func_081b20a0
.L_081b3e2c:
	.4byte Func_081b243c
.L_081b3e30:
	.4byte Func_081b3294
