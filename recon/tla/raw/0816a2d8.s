.syntax unified
	.thumb
	.global Func_0816a2d8
	.thumb_func
Func_0816a2d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	str r0, [sp, #16]
	str r1, [sp, #12]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	movs r0, #0
	str r1, [sp, #8]
	ldr r3, [r3, #96]
	mov r9, r3
	bl Func_081435e0
	ldr r3, [sp, #12]
	subs r3, #3
	cmp r3, #2
	bls .L_0816a310
	movs r2, #128
	ldr r3, .L_0816a338
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
.L_0816a310:
	movs r2, #128
	ldr r3, .L_0816a33c
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r2, [sp, #12]
	cmp r2, #0
	bne .L_0816a348
	ldr r3, [sp, #8]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r3, r2
	ldr r0, .L_0816a340
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0816a344
	b .L_0816a3f8
	.2byte 0x0000
.L_0816a338:
	.4byte 0x00000000
.L_0816a33c:
	.4byte 0x00001010
.L_0816a340:
	.4byte 0x00000111
.L_0816a344:
	.4byte 0x00000112
.L_0816a348:
	ldr r3, [sp, #12]
	cmp r3, #1
	bne .L_0816a364
	ldr r2, [sp, #8]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0816a3c0
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0816a3c4
	b .L_0816a3f8
.L_0816a364:
	ldr r1, [sp, #12]
	cmp r1, #4
	bne .L_0816a3d4
	ldr r2, [sp, #8]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0816a3c8
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r1, .L_0816a3cc
	movs r2, #1
	ldr r0, .L_0816a3d0
	movs r3, #1
	bl Func_08157cf4
	ldr r1, .L_0816a3bc
	movs r4, #160
	movs r2, #31
	movs r5, #0
	mov r12, r1
	lsls r4, r4, #19
	mov lr, r2
.L_0816a396:
	ldrh r0, [r4]
	mov r3, r12
	lsls r1, r0, #16
	lsrs r2, r1, #26
	lsrs r1, r1, #21
	ands r2, r3
	ands r1, r3
	mov r3, lr
	ands r3, r0
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r1
	adds r5, #1
	strh r3, [r4]
	adds r4, #2
	cmp r5, #64
	bne .L_0816a396
	b .L_0816a422
.L_0816a3bc:
	.4byte 0x0000001f
.L_0816a3c0:
	.4byte 0x0000010f
.L_0816a3c4:
	.4byte 0x00000110
.L_0816a3c8:
	.4byte 0x0000010d
.L_0816a3cc:
	.4byte gMapCellBuffer
.L_0816a3d0:
	.4byte 0x0000010e
.L_0816a3d4:
	ldr r1, [sp, #12]
	cmp r1, #3
	bne .L_0816a3f0
	ldr r2, [sp, #8]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0816a6f4
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0816a6f8
	b .L_0816a3f8
.L_0816a3f0:
	ldr r1, [sp, #12]
	cmp r1, #5
	bne .L_0816a404
	ldr r0, .L_0816a6fc
.L_0816a3f8:
	ldr r1, .L_0816a700
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	b .L_0816a422
.L_0816a404:
	ldr r2, [sp, #8]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0816a704
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0816a708
	ldr r1, .L_0816a700
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
.L_0816a422:
	ldr r1, [sp, #8]
	movs r3, #239
	lsls r3, r3, #7
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	movs r3, #0
	movs r1, #200
	lsls r1, r1, #4
	str r3, [r2]
	ldr r0, .L_0816a70c
	bl Func_080145a8
	ldr r1, [sp, #12]
	cmp r1, #1
	beq .L_0816a44e
	cmp r1, #5
	bne .L_0816a45c
.L_0816a44e:
	ldr r2, [sp, #16]
	movs r3, #36
	ldrsh r1, [r2, r3]
	movs r3, #128
	ldr r0, [r2, #8]
	lsls r3, r3, #12
	b .L_0816a46c
.L_0816a45c:
	ldr r1, [sp, #12]
	cmp r1, #3
	bne .L_0816a474
	ldr r2, [sp, #16]
	movs r3, #160
	ldr r1, [r2, #8]
	lsls r3, r3, #12
	adds r0, r1, #0
.L_0816a46c:
	movs r2, #16
	bl Func_08118078
	b .L_0816a4a6
.L_0816a474:
	ldr r3, [sp, #12]
	cmp r3, #4
	bne .L_0816a496
	ldr r1, [sp, #16]
	ldr r0, [r1, #8]
	bl GetBattleObjectSlotFar
	ldr r5, [r0]
	movs r1, #2
	adds r0, r5, #0
	bl Object_SetMode
	adds r0, r5, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildrenFar
	b .L_0816a4a6
.L_0816a496:
	ldr r2, [sp, #16]
	movs r3, #36
	ldrsh r1, [r2, r3]
	ldr r0, [r2, #8]
	movs r3, #0
	movs r2, #16
	bl Func_08118078
.L_0816a4a6:
	ldr r1, [sp, #12]
	cmp r1, #4
	bne .L_0816a4b4
	movs r0, #8
	bl WaitFrames
	b .L_0816a4ba
.L_0816a4b4:
	movs r0, #16
	bl WaitFrames
.L_0816a4ba:
	ldr r2, [sp, #16]
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_0816a4cc
	movs r0, #104
	movs r1, #7
	bl Func_081963ec
	b .L_0816a4d4
.L_0816a4cc:
	movs r0, #104
	movs r1, #3
	bl Func_081963ec
.L_0816a4d4:
	ldr r1, [sp, #16]
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #104]
	ldr r3, [r1, #4]
	cmp r3, #1
	bne .L_0816a4ec
	movs r0, #188
	movs r1, #15
	bl Func_081963ec
	b .L_0816a4f4
.L_0816a4ec:
	movs r0, #188
	movs r1, #11
	bl Func_081963ec
.L_0816a4f4:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	movs r0, #212
	ldr r7, [r3]
	bl Audio_PlayCue
	movs r2, #0
	movs r3, #120
	movs r1, #60
	mov r10, r2
	mov r8, r3
	mov r11, r1
.L_0816a50e:
	ldr r2, [sp, #12]
	cmp r2, #5
	bne .L_0816a5be
	mov r3, r10
	cmp r3, #3
	bgt .L_0816a53c
	mov r1, r8
	mov r2, r11
	str r1, [sp, #0]
	str r2, [sp, #4]
	ldr r1, .L_0816a700
	movs r2, #0
	movs r3, #0
	mov r0, r9
	mov lr, r6
	.2byte 0xf800
	mov r3, r8
	mov r1, r11
	str r3, [sp, #0]
	str r1, [sp, #4]
	mov r0, r9
	ldr r1, .L_0816a700
	b .L_0816a5b4
.L_0816a53c:
	mov r2, r10
	cmp r2, #7
	bgt .L_0816a564
	mov r3, r8
	mov r1, r11
	str r3, [sp, #0]
	str r1, [sp, #4]
	movs r2, #0
	ldr r1, .L_0816a710
	movs r3, #0
	mov r0, r9
	mov lr, r6
	.2byte 0xf800
	mov r2, r8
	mov r3, r11
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r0, r9
	ldr r1, .L_0816a710
	b .L_0816a5b4
.L_0816a564:
	mov r1, r10
	cmp r1, #11
	bgt .L_0816a58c
	mov r2, r8
	mov r3, r11
	str r2, [sp, #0]
	str r3, [sp, #4]
	ldr r1, .L_0816a714
	movs r2, #0
	movs r3, #0
	mov r0, r9
	mov lr, r6
	.2byte 0xf800
	mov r1, r8
	mov r2, r11
	str r1, [sp, #0]
	str r2, [sp, #4]
	mov r0, r9
	ldr r1, .L_0816a714
	b .L_0816a5b4
.L_0816a58c:
	mov r3, r10
	cmp r3, #15
	bgt .L_0816a620
	ldr r5, .L_0816a718
	mov r1, r8
	mov r2, r11
	str r1, [sp, #0]
	str r2, [sp, #4]
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	mov r0, r9
	mov lr, r6
	.2byte 0xf800
	mov r3, r8
	mov r1, r11
	str r3, [sp, #0]
	str r1, [sp, #4]
	mov r0, r9
	adds r1, r5, #0
.L_0816a5b4:
	movs r2, #0
	movs r3, #60
	mov lr, r7
	.2byte 0xf800
	b .L_0816a620
.L_0816a5be:
	mov r2, r10
	cmp r2, #3
	bgt .L_0816a5d6
	mov r3, r8
	ldr r2, [sp, #8]
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	mov r0, r9
	b .L_0816a5fe
.L_0816a5d6:
	mov r1, r10
	cmp r1, #7
	bgt .L_0816a5ee
	mov r2, r8
	ldr r3, [sp, #8]
	str r2, [sp, #0]
	str r2, [sp, #4]
	movs r2, #253
	lsls r2, r2, #6
	adds r1, r3, r2
	mov r0, r9
	b .L_0816a5fe
.L_0816a5ee:
	mov r3, r10
	cmp r3, #11
	bgt .L_0816a608
	mov r1, r8
	str r1, [sp, #0]
	str r1, [sp, #4]
	mov r0, r9
	ldr r1, .L_0816a700
.L_0816a5fe:
	movs r2, #0
	movs r3, #0
	mov lr, r6
	.2byte 0xf800
	b .L_0816a620
.L_0816a608:
	mov r2, r10
	cmp r2, #15
	bgt .L_0816a620
	mov r3, r8
	str r3, [sp, #0]
	str r3, [sp, #4]
	mov r0, r9
	ldr r1, .L_0816a714
	movs r2, #0
	movs r3, #0
	mov lr, r6
	.2byte 0xf800
.L_0816a620:
	mov r3, r10
	subs r3, #16
	cmp r3, #3
	bhi .L_0816a636
	movs r1, #128
	ldr r3, .L_0816a71c
	mov r0, r9
	lsls r1, r1, #7
	ldr r2, .L_0816a720
	mov lr, r3
	.2byte 0xf800
.L_0816a636:
	mov r1, r10
	cmp r1, #18
	bne .L_0816a642
	movs r0, #134
	bl Func_081180e8
.L_0816a642:
	mov r2, r10
	cmp r2, #20
	bne .L_0816a662
	ldr r1, [sp, #8]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	adds r3, r1, r2
	movs r2, #8
	str r2, [r3]
	ldr r1, [sp, #16]
	movs r3, #36
	ldrsh r0, [r1, r3]
	movs r1, #4
	bl Func_08118088
.L_0816a662:
	movs r0, #16
	movs r1, #16
	bl Func_08158ce0
	bl Func_081434f8
	movs r1, #240
	ldr r3, [sp, #8]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r3, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #21
	beq .L_0816a68e
	b .L_0816a50e
.L_0816a68e:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0816a70c
	bl Func_08014644
	ldr r1, [sp, #12]
	cmp r1, #3
	bne .L_0816a6ca
	movs r1, #240
	ldr r5, .L_0816a724
	lsls r1, r1, #6
	ldr r0, .L_0816a728
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	lsls r1, r1, #6
	mov r0, r9
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_0816a72c
	bl Func_08014644
	ldr r0, [sp, #16]
	bl Func_081504cc
	b .L_0816a6e4
.L_0816a6ca:
	ldr r2, [sp, #12]
	cmp r2, #4
	bne .L_0816a6e0
	ldr r3, [sp, #16]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	movs r1, #16
	ldr r0, [r0]
	bl ObjectDispatch_ApplyValueToChildrenFar
.L_0816a6e0:
	bl Func_08143bb8
.L_0816a6e4:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0816a6f4:
	.4byte 0x0000010f
.L_0816a6f8:
	.4byte 0x00000110
.L_0816a6fc:
	.4byte 0x000000fe
.L_0816a700:
	.4byte gMapCellBuffer
.L_0816a704:
	.4byte 0x0000010d
.L_0816a708:
	.4byte 0x0000010e
.L_0816a70c:
	.4byte Func_08143000
.L_0816a710:
	.4byte Data_02011c20
.L_0816a714:
	.4byte Data_02013840
.L_0816a718:
	.4byte Data_02015460
.L_0816a71c:
	.4byte IwramFillWords
.L_0816a720:
	.4byte 0x3f3f3f3f
.L_0816a724:
	.4byte IwramClearWords
.L_0816a728:
	.4byte 0x06004000
.L_0816a72c:
	.4byte Func_08143488
