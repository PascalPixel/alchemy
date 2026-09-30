.syntax unified
	.thumb
	.global Scene_RunParticleSequence
Scene_RunParticleSequence:
	.global Unnamed_080f7460
	.thumb_func
Unnamed_080f7460:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_080f74e8
	movs r0, #41
	sub sp, #116
	bl Runtime_AllocateHeapBlock
	movs r1, #128
	str r0, [sp, #44]
	lsls r1, r1, #8
	movs r0, #40
	bl Runtime_AllocateHeapBlock
	ldr r1, .L_080f74ec
	str r0, [sp, #40]
	movs r0, #39
	bl Runtime_AllocateBlock
	ldr r1, .L_080f74f0
	str r0, [sp, #36]
	movs r0, #45
	bl Runtime_AllocateBlock
	str r0, [sp, #32]
	ldr r0, .L_080f74f4
	ldr r5, .L_080f74f8
	bl RuntimeDispatch_NoOpHook
	movs r0, #144
	lsls r0, r0, #1
	adds r5, r5, r0
	movs r3, #255
	strb r3, [r5]
	ldr r2, [sp, #32]
	movs r3, #0
	adds r2, #162
	strh r3, [r2]
	ldr r1, [sp, #32]
	movs r3, #1
	adds r1, #152
	str r1, [sp, #28]
	str r3, [r1]
	bl Scheduler_ResetTaskTable
	ldr r5, .L_080f74dc
	ldr r3, .L_080f74fc
	movs r0, #0
	strb r5, [r3]
	ldr r1, .L_080f7500
	ldr r7, .L_080f74e0
	ldr r4, .L_080f74e4
	mov r8, r0
	movs r6, #0
.L_080f74d4:
	movs r2, #0
	mov r10, r2
	b .L_080f7504
	.2byte 0x0000
.L_080f74dc:
	.4byte 0x00000000
.L_080f74e0:
	.4byte 0x0000a1a6
.L_080f74e4:
	.4byte 0x0000a1a8
.L_080f74e8:
	.4byte 0x0000060e
.L_080f74ec:
	.4byte 0x0000782c
.L_080f74f0:
	.4byte 0x0000061c
.L_080f74f4:
	.4byte 0x0000000c
.L_080f74f8:
	.4byte Data_0200024c
.L_080f74fc:
	.4byte gOamCopyEnabled
.L_080f7500:
	.4byte 0x06002800
.L_080f7504:
	mov r3, r10
	subs r3, #5
	cmp r3, #19
	bhi .L_080f751c
	cmp r6, #2
	ble .L_080f751c
	cmp r6, #13
	bgt .L_080f751c
	mov r2, r8
	adds r3, r2, r1
	strh r7, [r3]
	b .L_080f7534
.L_080f751c:
	mov r3, r10
	cmp r3, #29
	ble .L_080f752a
	mov r2, r8
	adds r3, r2, r1
	strh r5, [r3]
	b .L_080f7534
.L_080f752a:
	mov r3, r8
	adds r2, r3, r1
	adds r3, r0, r4
	strh r3, [r2]
	adds r0, #1
.L_080f7534:
	movs r2, #1
	add r10, r2
	movs r3, #2
	mov r2, r10
	add r8, r3
	cmp r2, #32
	bne .L_080f7504
	adds r6, #1
	cmp r6, #20
	bne .L_080f74d4
	ldr r0, .L_080f75f8
	bl Resource_GetTableEntry
	ldr r1, [sp, #44]
	bl Resource_DecodeType01
	ldr r0, .L_080f75fc
	bl Resource_GetTableEntry
	ldr r3, .L_080f7600
	adds r4, r0, #0
	ldr r1, .L_080f7604
	ldr r2, .L_080f7608
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r4, #32
	adds r0, r4, #0
	ldr r1, .L_080f760c
	bl Resource_DecodeType01
	ldr r0, .L_080f760c
	movs r3, #0
	ldr r7, .L_080f7610
	mov r8, r3
	movs r6, #0
	movs r5, #0
	mov r12, r0
.L_080f757e:
	movs r1, #0
	lsls r3, r5, #6
	mov r2, r12
	mov r10, r1
	adds r4, r3, r2
.L_080f7588:
	mov r3, r10
	subs r3, #5
	cmp r3, #19
	bhi .L_080f7598
	cmp r6, #2
	ble .L_080f7598
	cmp r6, #13
	ble .L_080f75aa
.L_080f7598:
	mov r2, r8
	adds r1, r2, r7
	ldr r3, .L_080f7600
	adds r0, r4, #0
	ldr r2, .L_080f7608
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #32
	add r8, r3
.L_080f75aa:
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r4, #32
	cmp r1, #30
	bne .L_080f7588
	adds r6, #1
	adds r5, #15
	cmp r6, #20
	bne .L_080f757e
	movs r1, #192
	ldr r3, .L_080f7614
	ldr r0, .L_080f7618
	lsls r1, r1, #2
	bl _call_via_r3
	ldr r3, .L_080f75f4
	movs r2, #0
	movs r4, #2
	ldr r5, .L_080f761c
	mov r8, r2
	movs r6, #0
	mov r12, r3
	negs r4, r4
	movs r0, #0
.L_080f75dc:
	movs r7, #0
	adds r2, r0, #0
	mov r10, r7
	adds r1, r4, #0
	adds r2, #148
.L_080f75e6:
	cmp r1, #14
	bls .L_080f7620
	mov r7, r8
	adds r3, r7, r5
	mov r7, r12
	strh r7, [r3]
	b .L_080f7626
.L_080f75f4:
	.4byte 0x000000bf
.L_080f75f8:
	.4byte 0x00000076
.L_080f75fc:
	.4byte 0x0000003f
.L_080f7600:
	.4byte 0x040000d4
.L_080f7604:
	.4byte 0x05000140
.L_080f7608:
	.4byte 0x84000008
.L_080f760c:
	.4byte gMapCellBuffer
.L_080f7610:
	.4byte 0x0600b500
.L_080f7614:
	.4byte IwramClearWords
.L_080f7618:
	.4byte 0x06002d00
.L_080f761c:
	.4byte 0x06003000
.L_080f7620:
	mov r7, r8
	adds r3, r7, r5
	strh r2, [r3]
.L_080f7626:
	movs r3, #1
	add r10, r3
	movs r7, #2
	mov r3, r10
	adds r2, #1
	add r8, r7
	cmp r3, #32
	bne .L_080f75e6
	adds r6, #1
	adds r4, #1
	adds r0, #32
	cmp r6, #20
	bne .L_080f75dc
	ldr r2, .L_080f768c
	ldr r3, .L_080f7674
	strh r3, [r2]
	ldr r3, .L_080f7678
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_080f7690
	movs r1, #0
	strh r1, [r3]
	strh r1, [r3, #2]
	strh r1, [r3, #4]
	strh r1, [r3, #6]
	strh r1, [r3, #8]
	strh r1, [r3, #10]
	ldr r3, .L_080f767c
	adds r2, #60
	strh r3, [r2]
	ldr r3, .L_080f7680
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_080f7684
	adds r2, #6
	strh r3, [r2]
	ldr r3, .L_080f7688
	adds r2, #2
	b .L_080f7694
.L_080f7674:
	.4byte 0x00000509
.L_080f7678:
	.4byte 0x00000680
.L_080f767c:
	.4byte 0x00003737
.L_080f7680:
	.4byte 0x00002727
.L_080f7684:
	.4byte 0x00003f44
.L_080f7688:
	.4byte 0x00001010
.L_080f768c:
	.4byte 0x0400000a
.L_080f7690:
	.4byte gBgScroll
.L_080f7694:
	strh r3, [r2]
	ldr r3, .L_080f76e4
	ldr r2, .L_080f76d0
	strh r1, [r3]
	adds r3, #4
	strh r1, [r3]
	subs r3, #2
	strh r2, [r3]
	adds r3, #4
	strh r2, [r3]
	ldr r0, .L_080f76e8
	ldr r3, .L_080f76d4
	ldr r4, .L_080f76ec
	strh r3, [r0]
	ldr r2, .L_080f76d8
	ldr r3, .L_080f76dc
	strh r3, [r4]
	strh r2, [r0]
	ldr r3, .L_080f76f0
	ldr r0, .L_080f76e0
	strh r0, [r4]
	strh r2, [r3]
	adds r3, #4
	strh r0, [r3]
	ldr r7, [sp, #36]
	ldr r2, [sp, #32]
	movs r0, #128
	lsls r0, r0, #2
	b .L_080f76f4
	.2byte 0x0000
.L_080f76d0:
	.4byte 0x0000ff60
.L_080f76d4:
	.4byte 0x000028c8
.L_080f76d8:
	.4byte 0x000000f0
.L_080f76dc:
	.4byte 0x00001878
.L_080f76e0:
	.4byte 0x000000a0
.L_080f76e4:
	.4byte 0x04000014
.L_080f76e8:
	.4byte 0x04000040
.L_080f76ec:
	.4byte 0x04000044
.L_080f76f0:
	.4byte 0x04000042
.L_080f76f4:
	adds r0, r7, r0
	adds r2, #140
	str r0, [sp, #24]
	str r2, [sp, #20]
	str r1, [r2]
	ldr r3, [sp, #32]
	adds r3, #144
	str r1, [r3]
	ldr r3, [sp, #32]
	ldr r0, .L_080f7768
	adds r3, #148
	str r1, [r3]
	adds r3, r7, r0
	str r1, [r3]
	ldr r2, [sp, #32]
	adds r2, #168
	str r2, [sp, #16]
	str r1, [r2]
	ldr r0, .L_080f776c
	bl Resource_GetTableEntry
	movs r1, #160
	ldr r3, .L_080f7770
	lsls r1, r1, #19
	ldr r2, .L_080f7774
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_080f7778
	ldr r3, .L_080f7760
	strh r3, [r2]
	ldr r3, .L_080f7764
	adds r2, #2
	strh r3, [r2]
	ldr r0, .L_080f777c
	bl Resource_GetTableEntry
	ldr r3, .L_080f7770
	adds r4, r0, #0
	ldr r1, .L_080f7780
	ldr r2, .L_080f7784
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #240
	lsls r3, r3, #1
	adds r4, r4, r3
	adds r0, r4, #0
	ldr r1, .L_080f7788
	bl Resource_DecodeType01
	ldr r3, .L_080f7770
	ldr r0, .L_080f7788
	ldr r1, .L_080f778c
	ldr r2, .L_080f7790
	b .L_080f7794
.L_080f7760:
	.4byte 0x00002f8b
.L_080f7764:
	.4byte 0x00005bf6
.L_080f7768:
	.4byte 0x0000778c
.L_080f776c:
	.4byte 0x0000008f
.L_080f7770:
	.4byte 0x040000d4
.L_080f7774:
	.4byte 0x84000020
.L_080f7778:
	.4byte 0x05000080
.L_080f777c:
	.4byte 0x00000040
.L_080f7780:
	.4byte 0x05000200
.L_080f7784:
	.4byte 0x84000078
.L_080f7788:
	.4byte gMapCellBuffer
.L_080f778c:
	.4byte 0x06010000
.L_080f7790:
	.4byte 0x84001b30
.L_080f7794:
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_080f7838
	bl Resource_GetTableEntry
	ldr r3, .L_080f783c
	adds r4, r0, #0
	ldr r1, .L_080f7840
	ldr r2, .L_080f7844
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r4, #32
	adds r0, r4, #0
	ldr r1, .L_080f7848
	bl Resource_DecodeType01
	ldr r3, .L_080f783c
	ldr r0, .L_080f7848
	ldr r1, .L_080f784c
	ldr r2, .L_080f7850
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Func_08015000
	bl ReelGame_InitTitle
	movs r7, #0
	mov r10, r7
	ldr r7, [sp, #32]
.L_080f77ce:
	movs r3, #8
	str r3, [r7]
	movs r3, #0
	strb r3, [r7, #25]
	movs r3, #255
	strb r3, [r7, #26]
	movs r6, #0
	adds r5, r7, #4
.L_080f77de:
	bl Random16
	movs r1, #5
	bl IwramUnsignedRemainderEntry
	adds r6, #1
	strb r0, [r5]
	adds r5, #1
	cmp r6, #21
	bne .L_080f77de
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r7, #28
	cmp r1, #5
	bne .L_080f77ce
	ldr r3, [sp, #32]
	movs r2, #0
	add r7, sp, #84
	mov r10, r2
	adds r4, r7, #0
	mov r9, r3
.L_080f780a:
	movs r0, #0
	mov r8, r0
	movs r5, #0
.L_080f7810:
	str r4, [sp, #8]
	bl Random16
	movs r1, #21
	bl IwramUnsignedRemainderEntry
	mov r1, r8
	str r0, [r5, r7]
	movs r6, #0
	ldr r4, [sp, #8]
	cmp r1, #0
	beq .L_080f786c
	ldr r3, [r7]
	cmp r0, r3
	bne .L_080f7854
	movs r2, #1
	negs r2, r2
	subs r5, #4
	add r8, r2
	b .L_080f786c
.L_080f7838:
	.4byte 0x00000041
.L_080f783c:
	.4byte 0x040000d4
.L_080f7840:
	.4byte 0x050003e0
.L_080f7844:
	.4byte 0x84000008
.L_080f7848:
	.4byte gMapCellBuffer
.L_080f784c:
	.4byte 0x06016e00
.L_080f7850:
	.4byte 0x84000480
.L_080f7854:
	adds r6, #1
	cmp r6, r8
	beq .L_080f786c
	lsls r3, r6, #2
	ldr r2, [r5, r4]
	ldr r3, [r4, r3]
	cmp r2, r3
	bne .L_080f7854
	movs r3, #1
	negs r3, r3
	subs r5, #4
	add r8, r3
.L_080f786c:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #4
	cmp r1, #8
	bne .L_080f7810
	movs r6, #0
	adds r1, r7, #0
.L_080f787c:
	adds r2, r6, #0
	cmp r6, #5
	ble .L_080f7884
	movs r2, #5
.L_080f7884:
	ldmia r1!, {r3}
	mov r0, r9
	adds r3, #4
	adds r6, #1
	strb r2, [r0, r3]
	cmp r6, #8
	bne .L_080f787c
	movs r2, #1
	add r10, r2
	movs r1, #28
	mov r3, r10
	add r9, r1
	cmp r3, #5
	bne .L_080f780a
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl FarCall_EffectTable
	ldr r5, .L_080f7960
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	movs r1, #8
	str r3, [sp, #48]
	movs r2, #7
	movs r3, #3
	movs r0, #47
	str r3, [sp, #0]
	bl FarCall_EffectTable
	adds r5, #188
	ldr r3, [r5]
	mov r7, sp
	adds r7, #48
	str r7, [sp, #12]
	movs r1, #128
	str r3, [r7, #4]
	ldr r0, [sp, #40]
	ldr r3, .L_080f7964
	lsls r1, r1, #8
	movs r2, #0
	bl _call_via_r3
	ldr r3, .L_080f7968
	ldr r0, [sp, #40]
	ldr r1, .L_080f796c
	ldr r2, .L_080f7970
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #160
	lsls r0, r0, #19
	ldr r1, [sp, #36]
	ldr r2, .L_080f7974
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, [sp, #24]
	ldr r0, .L_080f7978
	ldr r2, .L_080f7974
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r5, #128
	lsls r5, r5, #1
	ldr r1, .L_080f7978
	ldr r0, [sp, #24]
	movs r2, #0
	adds r3, r5, #0
	bl Graphics_ScaleRgb555
	movs r1, #160
	movs r2, #0
	adds r3, r5, #0
	lsls r1, r1, #19
	ldr r0, [sp, #36]
	bl Graphics_ScaleRgb555
	movs r2, #128
	ldr r3, .L_080f795c
	lsls r2, r2, #19
	strh r3, [r2]
	movs r0, #228
	bl PartyInventory_CountItemFar
	cmp r0, #1
	bne .L_080f7982
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #18
	movs r3, #3
	movs r0, #6
	bl UiWindow_CreateFar
	movs r2, #153
	adds r1, r0, #0
	ldr r0, [sp, #32]
	lsls r2, r2, #3
	adds r3, r0, r2
	str r1, [r3]
	ldr r0, .L_080f797c
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080f7980
.L_080f795c:
	.4byte 0x00003740
.L_080f7960:
	.4byte Data_03001e50
.L_080f7964:
	.4byte IwramFillWords
.L_080f7968:
	.4byte 0x040000d4
.L_080f796c:
	.4byte 0x06003500
.L_080f7970:
	.4byte 0x84002000
.L_080f7974:
	.4byte 0x84000080
.L_080f7978:
	.4byte 0x05000200
.L_080f797c:
	.4byte 0x00000909
.L_080f7980:
	b .L_080f79b8
.L_080f7982:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #26
	movs r3, #4
	movs r0, #2
	bl UiWindow_CreateFar
	movs r7, #153
	ldr r3, [sp, #32]
	ldr r5, .L_080f7bec
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
.L_080f79b8:
	ldr r0, [sp, #36]
	ldr r1, .L_080f7bf0
	movs r5, #144
	adds r2, r0, r1
	movs r3, #0
	lsls r5, r5, #3
	str r3, [r2]
	adds r1, r5, #0
	ldr r0, .L_080f7bf4
	bl Scheduler_AddOrUpdateCallback
	ldr r0, .L_080f7bf8
	adds r1, r5, #0
	bl Scheduler_AddOrUpdateCallback
	ldr r7, [sp, #20]
	ldr r3, [r7]
	movs r2, #0
	mov r11, r2
	cmp r3, #10
	bne .L_080f79e4
	b .L_080f7d26
.L_080f79e4:
	mov r0, r11
	cmp r0, #16
	bgt .L_080f7a0a
	movs r6, #128
	lsls r5, r0, #12
	lsls r6, r6, #1
	ldr r1, .L_080f7bfc
	adds r2, r5, #0
	adds r3, r6, #0
	ldr r0, [sp, #24]
	bl Graphics_ScaleRgb555
	movs r1, #160
	ldr r0, [sp, #36]
	lsls r1, r1, #19
	adds r2, r5, #0
	adds r3, r6, #0
	bl Graphics_ScaleRgb555
.L_080f7a0a:
	ldr r1, [sp, #20]
	ldr r3, [r1]
	cmp r3, #3
	beq .L_080f7a14
	b .L_080f7b98
.L_080f7a14:
	mov r0, r11
	movs r1, #80
	bl Func_080022fc
	cmp r0, #15
	bgt .L_080f7a28
	ldr r0, .L_080f7c00
	bl Palette_StepTowardResource
	b .L_080f7a52
.L_080f7a28:
	cmp r0, #31
	bgt .L_080f7a34
	ldr r0, .L_080f7c04
	bl Palette_StepTowardResource
	b .L_080f7a52
.L_080f7a34:
	cmp r0, #47
	bgt .L_080f7a40
	ldr r0, .L_080f7c08
	bl Palette_StepTowardResource
	b .L_080f7a52
.L_080f7a40:
	cmp r0, #63
	bgt .L_080f7a4c
	ldr r0, .L_080f7c0c
	bl Palette_StepTowardResource
	b .L_080f7a52
.L_080f7a4c:
	ldr r0, .L_080f7c10
	bl Palette_StepTowardResource
.L_080f7a52:
	ldr r2, [sp, #16]
	ldr r3, [r2]
	cmp r3, #15
	bgt .L_080f7a62
	bl Palette_DarkenSceneStep
	ldr r7, [sp, #16]
	ldr r3, [r7]
.L_080f7a62:
	cmp r3, #16
	ble .L_080f7b00
	movs r3, #7
	mov r0, r11
	ands r3, r0
	cmp r3, #0
	bne .L_080f7b00
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
	bge .L_080f7a90
	adds r2, #7
.L_080f7a90:
	movs r3, #3
	asrs r2, r2, #3
	ands r2, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r2, .L_080f7c14
	lsls r3, r3, #10
	adds r7, r3, r2
	lsls r5, r5, #16
	movs r3, #0
	lsls r1, r1, #16
	mov r8, r3
	mov r9, r5
	mov r10, r1
.L_080f7aac:
	bl Random16
	movs r5, #255
	ands r5, r0
	bl Random16
	ldr r3, .L_080f7c18
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
	bl Func_0800231c
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
	bne .L_080f7aac
.L_080f7b00:
	movs r7, #0
	mov r8, r7
	ldr r7, .L_080f7c14
.L_080f7b06:
	ldr r0, [r7, #24]
	cmp r0, #0
	ble .L_080f7b8a
	ldr r3, [r7]
	ldr r1, .L_080f7c1c
	subs r0, #1
	str r0, [r7, #24]
	cmp r3, r1
	bhi .L_080f7b5c
	ldr r6, [r7, #4]
	ldr r2, .L_080f7c20
	cmp r6, r2
	bgt .L_080f7b5c
	cmp r6, #0
	blt .L_080f7b5c
	movs r1, #12
	asrs r5, r3, #16
	bl FixedPoint_Ratio
	adds r0, #1
	lsls r4, r0, #1
	mov r3, r8
	ldr r1, .L_080f7c24
	movs r2, #1
	ands r2, r3
	asrs r6, r6, #16
	subs r3, r4, #2
	ldrh r1, [r1, r3]
	subs r5, r5, r0
	ldr r3, [sp, #44]
	subs r6, r6, r0
	str r4, [sp, #0]
	ldr r0, [sp, #12]
	str r4, [sp, #4]
	lsls r2, r2, #2
	ldr r4, [r2, r0]
	adds r1, r3, r1
	ldr r0, [sp, #40]
	adds r3, r6, #0
	adds r2, r5, #0
	bl _call_via_r4
	ldr r3, [r7]
.L_080f7b5c:
	ldr r2, [r7, #12]
	adds r3, r3, r2
	str r3, [r7]
	ldr r1, [r7, #16]
	ldr r3, [r7, #4]
	adds r3, r3, r1
	str r3, [r7, #4]
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_080f7b76
	adds r3, #63
.L_080f7b76:
	asrs r3, r3, #6
	str r3, [r7, #12]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_080f7b86
	adds r3, #63
.L_080f7b86:
	asrs r3, r3, #6
	str r3, [r7, #16]
.L_080f7b8a:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #3
	adds r7, #28
	cmp r8, r2
	bne .L_080f7b06
.L_080f7b98:
	ldr r7, [sp, #20]
	ldr r3, [r7]
	cmp r3, #0
	beq .L_080f7ba6
	cmp r3, #2
	beq .L_080f7ba6
	b .L_080f7d08
.L_080f7ba6:
	movs r0, #0
	add r5, sp, #56
	mov r8, r0
	movs r2, #0
	adds r3, r5, #0
.L_080f7bb0:
	movs r1, #1
	add r8, r1
	mov r7, r8
	stmia r3!, {r2}
	cmp r7, #7
	bne .L_080f7bb0
	ldr r0, [sp, #20]
	ldr r3, [r0]
	cmp r3, #0
	bne .L_080f7c28
	movs r2, #1
	str r2, [r5, #12]
	ldr r1, [sp, #28]
	ldr r3, [r1]
	cmp r3, #1
	ble .L_080f7bd6
	str r2, [r5, #16]
	str r2, [r5, #8]
	ldr r3, [r1]
.L_080f7bd6:
	cmp r3, #2
	ble .L_080f7be2
	str r2, [r5, #20]
	str r2, [r5, #4]
	ldr r7, [sp, #28]
	ldr r3, [r7]
.L_080f7be2:
	cmp r3, #3
	ble .L_080f7c4c
	str r2, [r5, #24]
	str r2, [r5]
	b .L_080f7c4c
.L_080f7bec:
	.4byte 0x00000908
.L_080f7bf0:
	.4byte 0x00007824
.L_080f7bf4:
	.4byte Unnamed_080f6440
.L_080f7bf8:
	.4byte BattlePres_ProcessPendingTileTransfer
.L_080f7bfc:
	.4byte 0x05000200
.L_080f7c00:
	.4byte 0x00000091
.L_080f7c04:
	.4byte 0x00000093
.L_080f7c08:
	.4byte 0x000000b4
.L_080f7c0c:
	.4byte 0x000000a0
.L_080f7c10:
	.4byte 0x0000008f
.L_080f7c14:
	.4byte gMapCellBuffer
.L_080f7c18:
	.4byte 0x0000ffff
.L_080f7c1c:
	.4byte 0x00ffffff
.L_080f7c20:
	.4byte 0x007fffff
.L_080f7c24:
	.4byte Data_080f86f8
.L_080f7c28:
	mov r3, r11
	mov r0, r8
	ands r3, r0
	cmp r3, #3
	bgt .L_080f7c4c
	ldr r2, [sp, #32]
	movs r1, #0
	mov r8, r1
	adds r0, r5, #0
	adds r2, #172
.L_080f7c3c:
	ldmia r2!, {r3}
	str r3, [r1, r0]
	movs r3, #1
	add r8, r3
	mov r7, r8
	adds r1, #4
	cmp r7, #7
	bne .L_080f7c3c
.L_080f7c4c:
	movs r0, #0
	mov r8, r0
.L_080f7c50:
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
	beq .L_080f7c76
	mov r3, r8
	adds r3, #19
	movs r0, #20
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl BattleFx_DrawCanvasLine
.L_080f7c76:
	ldr r3, [r5, #8]
	cmp r3, #0
	beq .L_080f7c8c
	mov r3, r8
	adds r3, #35
	movs r0, #28
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl BattleFx_DrawCanvasLine
.L_080f7c8c:
	ldr r3, [r5, #12]
	cmp r3, #0
	beq .L_080f7ca2
	mov r3, r8
	adds r3, #51
	movs r0, #20
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl BattleFx_DrawCanvasLine
.L_080f7ca2:
	ldr r3, [r5, #16]
	cmp r3, #0
	beq .L_080f7cb8
	mov r3, r8
	adds r3, #67
	movs r0, #28
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl BattleFx_DrawCanvasLine
.L_080f7cb8:
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L_080f7cce
	mov r3, r8
	adds r3, #83
	movs r0, #20
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl BattleFx_DrawCanvasLine
.L_080f7cce:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_080f7ce6
	mov r1, r8
	mov r3, r8
	adds r1, #5
	adds r3, #91
	movs r0, #28
	movs r2, #200
	str r6, [sp, #0]
	bl BattleFx_DrawCanvasLine
.L_080f7ce6:
	ldr r3, [r5, #24]
	cmp r3, #0
	beq .L_080f7cfe
	mov r1, r8
	mov r3, r8
	adds r1, #97
	adds r3, #11
	movs r0, #28
	movs r2, #200
	str r6, [sp, #0]
	bl BattleFx_DrawCanvasLine
.L_080f7cfe:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #3
	bne .L_080f7c50
.L_080f7d08:
	ldr r7, [sp, #36]
	ldr r0, .L_080f7da4
	movs r2, #1
	adds r3, r7, r0
	str r2, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #20]
	ldr r3, [r2]
	movs r1, #1
	add r11, r1
	cmp r3, #10
	beq .L_080f7d26
	b .L_080f79e4
.L_080f7d26:
	movs r3, #0
	movs r6, #128
	mov r8, r3
	lsls r6, r6, #1
.L_080f7d2e:
	mov r7, r8
	movs r5, #128
	lsls r3, r7, #12
	lsls r5, r5, #9
	subs r5, r5, r3
	ldr r0, [sp, #24]
	ldr r1, .L_080f7da8
	adds r2, r5, #0
	adds r3, r6, #0
	bl Graphics_ScaleRgb555
	movs r1, #160
	lsls r1, r1, #19
	ldr r0, [sp, #36]
	adds r2, r5, #0
	adds r3, r6, #0
	bl Graphics_ScaleRgb555
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #17
	bne .L_080f7d2e
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_080f7dac
	bl Scheduler_RemoveCallback
	ldr r0, .L_080f7db0
	bl Scheduler_RemoveCallback
	movs r0, #45
	bl Runtime_ReleaseHeapBlock
	movs r0, #40
	bl Runtime_ReleaseHeapBlock
	movs r0, #39
	bl Runtime_ReleaseHeapBlock
	movs r0, #41
	bl Runtime_ReleaseHeapBlock
	add sp, #116
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080f7da4:
	.4byte 0x00007824
.L_080f7da8:
	.4byte 0x05000200
.L_080f7dac:
	.4byte BattlePres_ProcessPendingTileTransfer
.L_080f7db0:
	.4byte Unnamed_080f6440
