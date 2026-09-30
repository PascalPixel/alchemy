.syntax unified
	.thumb
	.global Func_08171430
	.thumb_func
Func_08171430:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #116
	str r0, [sp, #68]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	movs r7, #0
	str r0, [sp, #64]
	movs r0, #1
	ldr r1, [r5, #96]
	str r1, [sp, #60]
	ldr r6, [r5, #100]
	bl Func_081435e0
	ldr r3, .L_08171494
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	ldr r2, [sp, #64]
	movs r3, #184
	lsls r3, r3, #5
	adds r1, r2, r3
	ldr r0, .L_08171498
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r4, [sp, #64]
	movs r2, #142
	lsls r2, r2, #7
	adds r1, r4, r2
	ldr r0, .L_0817149c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	adds r1, r6, #0
	movs r2, #0
	b .L_081714a0
.L_08171494:
	.4byte 0x00001010
.L_08171498:
	.4byte 0x000000c4
.L_0817149c:
	.4byte 0x000000c9
.L_081714a0:
	movs r3, #0
	ldr r0, .L_08171678
	bl Func_08157cf4
	ldr r0, .L_0817167c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08171680
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r3, [sp, #64]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r0, [sp, #64]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #132
	adds r2, r0, r1
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08171684
	bl Func_080145a8
	ldr r5, [r5, #48]
	ldr r1, [sp, #68]
	str r5, [sp, #48]
	movs r6, #2
	movs r3, #54
	ldrsh r2, [r5, r3]
	movs r5, #0
	str r2, [sp, #44]
	movs r4, #36
	ldrsh r0, [r1, r4]
	bl GetBattleObjectSlotFar
	ldr r2, [sp, #64]
	ldr r0, [r0]
	movs r3, #224
	lsls r3, r3, #3
	adds r3, r2, r3
	movs r4, #0
	str r0, [sp, #40]
	str r3, [sp, #36]
	str r4, [sp, #52]
.L_0817150a:
	ldr r1, .L_08171688
	movs r2, #128
	adds r0, r7, r1
	lsls r2, r2, #9
	adds r1, r6, #0
	bl Func_0815b434
	adds r3, r5, #3
	muls r3, r6
	ldr r2, [sp, #36]
	adds r6, #2
	strh r7, [r5, r2]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r7, r7, r3
	ldr r3, [sp, #52]
	adds r5, #2
	adds r3, #1
	str r3, [sp, #52]
	cmp r3, #32
	bne .L_0817150a
	ldr r0, [sp, #48]
	mov r1, sp
	mov r2, sp
	movs r4, #0
	adds r0, #12
	adds r1, #72
	adds r2, #104
	str r4, [sp, #56]
	str r0, [sp, #8]
	str r1, [sp, #16]
	str r2, [sp, #28]
.L_0817154c:
	ldr r3, [sp, #56]
	cmp r3, #64
	bne .L_08171558
	movs r0, #134
	bl Func_081180e8
.L_08171558:
	ldr r4, [sp, #56]
	cmp r4, #31
	ble .L_0817156c
	movs r3, #3
	ands r3, r4
	cmp r3, #0
	bne .L_0817156c
	ldr r0, .L_0817168c
	bl Func_0815f0a0
.L_0817156c:
	ldr r0, [sp, #56]
	cmp r0, #0
	bne .L_08171648
	add r1, sp, #44
	ldrh r1, [r1]
	ldr r2, [sp, #48]
	strh r1, [r2, #54]
	ldr r3, [sp, #64]
	movs r2, #0
	str r2, [sp, #52]
	adds r3, #24
	subs r2, #1
.L_08171584:
	str r2, [r3]
	ldr r4, [sp, #52]
	adds r3, #28
	adds r4, #1
	str r4, [sp, #52]
	cmp r4, #32
	bne .L_08171584
	movs r0, #0
	ldr r1, .L_08171690
	ldr r2, [sp, #64]
	movs r3, #224
	str r0, [sp, #52]
	lsls r3, r3, #2
	mov r8, r1
	adds r6, r2, r3
.L_081715a2:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r7, r3, #0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r7, #32
	adds r3, r7, #0
	muls r3, r0
	adds r0, r5, #0
	str r3, [r6]
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	str r3, [r6, #8]
	movs r3, #220
	lsls r3, r3, #15
	str r3, [r6, #4]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r0, r5, #0
	adds r7, r3, #4
	bl Trig_Sin
	ldr r4, [sp, #40]
	adds r2, r7, #0
	muls r2, r0
	ldr r3, [r4, #8]
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r6, #12]
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	ldr r0, [sp, #40]
	mov r1, r8
	ldr r5, [r0, #16]
	ldr r0, [r6, #12]
	adds r5, r5, r3
	ldr r3, [r6]
	str r5, [r6, #20]
	str r1, [r6, #16]
	subs r0, r0, r3
	movs r1, #6
	bl Math_Div
	str r0, [r6, #12]
	ldr r0, [r6, #4]
	mov r2, r8
	subs r0, r2, r0
	movs r1, #6
	bl Math_Div
	ldr r3, [r6, #8]
	str r0, [r6, #16]
	subs r5, r5, r3
	adds r0, r5, #0
	movs r1, #6
	bl Math_Div
	str r0, [r6, #20]
	ldr r4, [sp, #52]
	negs r3, r4
	lsls r3, r3, #2
	adds r4, #1
	str r3, [r6, #24]
	adds r6, #28
	str r4, [sp, #52]
	cmp r4, #32
	bne .L_081715a2
.L_08171648:
	ldr r3, [sp, #56]
	subs r3, #20
	cmp r3, #59
	bhi .L_0817169e
	ldr r0, [sp, #56]
	movs r2, #128
	lsls r2, r2, #1
	cmp r0, #71
	ble .L_08171664
	ldr r1, [sp, #56]
	movs r3, #160
	lsls r2, r1, #5
	lsls r3, r3, #4
	subs r2, r3, r2
.L_08171664:
	ldr r4, [sp, #68]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_08171694
	ldr r0, [sp, #48]
	ldrh r3, [r0, #54]
	adds r1, r0, #0
	adds r3, r3, r2
	strh r3, [r1, #54]
	b .L_0817169e
.L_08171678:
	.4byte 0x00000134
.L_0817167c:
	.4byte 0x00000149
.L_08171680:
	.4byte IwramCopyWords
.L_08171684:
	.4byte Func_08143000
.L_08171688:
	.4byte gMapCellBuffer
.L_0817168c:
	.4byte 0x00000154
.L_08171690:
	.4byte 0xfff80000
.L_08171694:
	ldr r4, [sp, #48]
	ldrh r3, [r4, #54]
	adds r0, r4, #0
	subs r3, r3, r2
	strh r3, [r0, #54]
.L_0817169e:
	bl Func_08014de4
	ldr r1, [sp, #8]
	ldr r0, [sp, #48]
	bl Func_080156e8
	movs r1, #0
	ldr r2, [sp, #64]
	str r1, [sp, #52]
	mov r8, r2
.L_081716b2:
	mov r3, r8
	ldr r0, [r3, #24]
	cmp r0, #15
	bls .L_081716bc
	b .L_081717c2
.L_081716bc:
	lsls r0, r0, #11
	bl Trig_Sin
	mov r4, r8
	ldr r3, [r4, #12]
	muls r3, r0
	asrs r3, r3, #16
	lsls r7, r3, #1
	ldr r3, [r4, #24]
	subs r5, r7, #2
	adds r3, #1
	str r3, [r4, #24]
	cmp r5, #61
	bhi .L_081717c2
	ldr r1, [sp, #28]
	mov r0, r8
	bl Func_0815e1ec
	ldr r0, [sp, #28]
	asrs r5, r5, #1
	ldr r3, [r0]
	ldr r1, [r0, #4]
	lsrs r2, r3, #31
	adds r3, r3, r2
	str r1, [sp, #32]
	asrs r3, r3, #1
	movs r1, #19
	movs r0, #188
	mov r11, r3
	bl Func_081963ec
	ldr r2, [sp, #36]
	lsls r5, r5, #1
	adds r5, r5, r2
	asrs r6, r7, #1
	ldr r2, [sp, #32]
	movs r3, #0
	ldrsh r1, [r5, r3]
	ldr r4, .L_08171ab4
	str r6, [sp, #0]
	str r7, [sp, #4]
	mov r0, r11
	movs r3, #192
	subs r0, r0, r6
	lsls r3, r3, #18
	mov r10, r5
	mov r9, r0
	subs r5, r2, r7
	adds r3, #188
	ldr r0, [sp, #60]
	adds r1, r1, r4
	mov r2, r9
	ldr r4, [r3]
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #23
	movs r0, #188
	bl Func_081963ec
	mov r0, r10
	movs r4, #0
	ldrsh r1, [r0, r4]
	ldr r2, .L_08171ab4
	str r6, [sp, #0]
	str r7, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r4, [r3]
	adds r1, r1, r2
	ldr r0, [sp, #60]
	mov r2, r11
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r4, [sp, #52]
	cmp r4, #15
	ble .L_081717c2
	movs r1, #27
	movs r0, #188
	bl Func_081963ec
	mov r2, r10
	movs r0, #0
	ldrsh r1, [r2, r0]
	ldr r3, .L_08171ab4
	str r6, [sp, #0]
	str r7, [sp, #4]
	movs r0, #192
	lsls r0, r0, #18
	adds r0, #188
	ldr r4, [r0]
	mov r2, r9
	adds r1, r1, r3
	ldr r0, [sp, #60]
	ldr r3, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #31
	movs r0, #188
	bl Func_081963ec
	mov r3, r10
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r4, .L_08171ab4
	str r6, [sp, #0]
	str r7, [sp, #4]
	movs r0, #192
	lsls r0, r0, #18
	adds r0, #188
	adds r1, r1, r4
	mov r2, r11
	ldr r4, [r0]
	ldr r3, [sp, #32]
	ldr r0, [sp, #60]
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_081717c2:
	ldr r2, [sp, #52]
	movs r1, #28
	adds r2, #1
	add r8, r1
	str r2, [sp, #52]
	cmp r2, #28
	beq .L_081717d2
	b .L_081716b2
.L_081717d2:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	movs r0, #1
	bl Func_081969f8
	movs r3, #0
	str r3, [sp, #24]
	ldr r2, .L_08171ab8
	ldr r3, [sp, #72]
	ldr r4, [sp, #16]
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_08171abc
	mov r1, r8
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #2
	orrs r3, r2
	str r3, [sp, #72]
	ldr r3, .L_08171ac0
	str r4, [r0, #16]
	str r3, [r0, #8]
	str r1, [r0, #12]
	ldr r2, [sp, #24]
	movs r3, #7
	str r2, [r0, #20]
	str r3, [r0]
	ldr r4, [sp, #64]
	mov r11, r0
	ldr r1, [sp, #16]
	movs r0, #142
	lsls r0, r0, #7
	adds r3, r4, r0
	str r3, [r1, #4]
	movs r3, #224
	lsls r3, r3, #1
	mov r9, r4
	adds r4, r4, r3
	movs r2, #0
	mov r10, r4
	ldr r4, [sp, #64]
	movs r0, #224
	str r2, [sp, #52]
	lsls r0, r0, #2
	adds r5, r4, r0
.L_08171832:
	ldr r2, [r5, #24]
	adds r3, r2, #1
	str r3, [r5, #24]
	cmp r3, #17
	bls .L_0817183e
	b .L_0817199c
.L_0817183e:
	cmp r3, #11
	ble .L_0817189c
	ldr r3, [r5, #12]
	subs r2, #11
	muls r2, r3
	ldr r3, [r5]
	add r6, sp, #92
	adds r1, r3, r2
	str r1, [r6]
	ldr r3, [r5, #24]
	ldr r2, [r5, #16]
	subs r3, #12
	muls r2, r3
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r5, #24]
	ldr r2, [r5, #20]
	subs r3, #12
	muls r2, r3
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r3, [r5, #24]
	cmp r3, #16
	bne .L_08171890
	mov r2, r10
	str r1, [r2]
	ldr r3, [r6, #4]
	str r3, [r2, #4]
	ldr r3, [r6, #8]
	str r3, [r2, #8]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #8
	mov r4, r10
	str r3, [r4, #12]
	movs r3, #0
	str r3, [r4, #24]
.L_08171890:
	ldr r7, [sp, #28]
	adds r0, r6, #0
	adds r1, r7, #0
	bl Func_0815e1ec
	b .L_081718a8
.L_0817189c:
	ldr r7, [sp, #28]
	adds r0, r5, #0
	adds r1, r7, #0
	bl Func_0815e1ec
	add r6, sp, #92
.L_081718a8:
	ldr r3, [r7]
	mov r0, r8
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	subs r2, r3, #2
	strh r2, [r0]
	ldr r2, [r7, #4]
	mov r1, r8
	adds r3, #2
	mov r4, r8
	strh r2, [r1, #2]
	strh r3, [r4, #8]
	strh r2, [r0, #10]
	ldr r2, [r5, #24]
	cmp r2, #5
	bgt .L_081718ee
	ldr r3, [r5, #12]
	muls r2, r3
	ldr r3, [r5]
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, [r5, #24]
	ldr r2, [r5, #16]
	muls r2, r3
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r5, #24]
	ldr r2, [r5, #20]
	muls r2, r3
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
	b .L_08171918
.L_081718ee:
	ldr r2, [r5, #12]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, [r5]
	lsls r3, r3, #1
	adds r2, r2, r3
	str r2, [r6]
	ldr r2, [r5, #16]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, [r5, #4]
	lsls r3, r3, #1
	adds r2, r2, r3
	str r2, [r6, #4]
	ldr r2, [r5, #20]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, [r5, #8]
	lsls r3, r3, #1
	adds r2, r2, r3
	str r2, [r6, #8]
.L_08171918:
	ldr r3, [r5, #24]
	cmp r3, #6
	bne .L_08171972
	ldr r3, [r6]
	mov r1, r9
	str r3, [r1]
	ldr r3, [r6, #4]
	str r3, [r1, #4]
	ldr r3, [r6, #8]
	str r3, [r1, #8]
	bl Random16
	movs r3, #7
	ands r3, r0
	mov r2, r9
	adds r3, #24
	str r3, [r2, #12]
	movs r3, #0
	str r3, [r2, #24]
	ldr r3, [sp, #64]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #4
	str r3, [r2]
	movs r0, #145
	bl Audio_PlayCue
	ldr r2, [sp, #68]
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #0
	bl Func_08118088
	ldr r4, [sp, #68]
	movs r1, #7
	movs r3, #36
	ldrsh r0, [r4, r3]
	movs r3, #2
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
.L_08171972:
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_0815e1ec
	ldr r3, [r7]
	mov r0, r8
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	subs r2, r3, #2
	strh r2, [r0, #16]
	ldr r2, [r7, #4]
	mov r1, r8
	adds r3, #2
	mov r4, r8
	strh r2, [r1, #18]
	strh r3, [r4, #24]
	strh r2, [r0, #26]
	mov r0, r11
	bl Func_08196a7c
.L_0817199c:
	ldr r2, [sp, #52]
	movs r1, #28
	adds r2, #1
	add r9, r1
	add r10, r1
	adds r5, #28
	str r2, [sp, #52]
	cmp r2, #14
	beq .L_081719b0
	b .L_08171832
.L_081719b0:
	ldr r3, [sp, #56]
	subs r3, #16
	cmp r3, #63
	bls .L_081719ba
	b .L_08171b16
.L_081719ba:
	ldr r4, [sp, #68]
	mov r1, sp
	movs r3, #36
	ldrsh r0, [r4, r3]
	adds r1, #80
	str r1, [sp, #20]
	bl Func_0815e21c
	ldr r3, [sp, #16]
	movs r2, #5
	strb r2, [r3]
	add r3, sp, #72
	str r3, [sp, #16]
	strb r2, [r3, #1]
	ldr r4, [sp, #64]
	ldr r1, [sp, #16]
	movs r0, #184
	lsls r0, r0, #5
	adds r3, r4, r0
	str r3, [r1, #4]
	mov r2, r11
	movs r3, #6
	str r3, [r2]
	ldr r3, .L_08171ac0
	ldr r0, .L_08171ac4
	str r3, [r2, #8]
	ldr r4, [sp, #56]
	movs r3, #0
	lsls r4, r4, #8
	str r3, [sp, #52]
	str r4, [sp, #12]
	ldr r6, .L_08171ac8
	mov r9, r0
.L_081719fc:
	ldr r1, [sp, #12]
	movs r2, #128
	lsls r2, r2, #5
	adds r0, r1, r2
	bl Trig_Sin
	ldr r4, [sp, #48]
	adds r5, r0, #0
	movs r3, #54
	ldrsh r0, [r4, r3]
	bl Trig_Sin
	adds r1, r0, #0
	ldr r0, [sp, #20]
	ldr r4, [sp, #52]
	ldr r3, [r0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	subs r3, #64
	mov r2, r9
	lsls r0, r3, #16
	ldrsb r3, [r2, r4]
	muls r3, r5
	cmp r3, #0
	bge .L_08171a38
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
.L_08171a38:
	asrs r3, r3, #16
	muls r3, r1
	ldr r4, [sp, #48]
	subs r0, r0, r3
	mov r10, r0
	movs r3, #54
	ldrsh r0, [r4, r3]
	bl Trig_Cos
	ldr r1, [sp, #52]
	adds r2, r0, #0
	mov r0, r9
	ldrsb r3, [r0, r1]
	adds r0, r5, #0
	muls r0, r3
	cmp r0, #0
	bge .L_08171a62
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_08171a62:
	asrs r3, r0, #16
	muls r3, r2
	ldr r1, [sp, #56]
	negs r7, r3
	cmp r1, #47
	bgt .L_08171a7a
	ldr r2, .L_08171acc
	lsls r0, r1, #9
	adds r0, r0, r2
	bl Trig_Sin
	b .L_08171a7e
.L_08171a7a:
	movs r0, #128
	lsls r0, r0, #9
.L_08171a7e:
	ldr r4, [sp, #52]
	ldr r2, .L_08171ad0
	lsls r3, r4, #2
	ldr r3, [r2, r3]
	muls r3, r0
	ldr r0, [sp, #56]
	lsrs r5, r3, #16
	cmp r0, #63
	ble .L_08171aa2
	movs r3, #0
	ldrsb r3, [r6, r3]
	lsls r2, r0, #3
	movs r1, #128
	subs r3, r3, r2
	lsls r1, r1, #2
	adds r1, r3, r1
	str r1, [sp, #24]
	b .L_08171ada
.L_08171aa2:
	ldr r2, [sp, #56]
	cmp r2, #23
	bgt .L_08171ad4
	movs r3, #0
	ldrsb r3, [r6, r3]
	lsls r2, r2, #3
	adds r3, r3, r2
	subs r3, #192
	b .L_08171ad8
.L_08171ab4:
	.4byte gMapCellBuffer
.L_08171ab8:
	.4byte 0xffffff00
.L_08171abc:
	.4byte 0xffff00ff
.L_08171ac0:
	.4byte Data_08199244
.L_08171ac4:
	.4byte Data_08198c10
.L_08171ac8:
	.4byte Data_08198c0c
.L_08171acc:
	.4byte 0xffffe000
.L_08171ad0:
	.4byte Data_08198bfc
.L_08171ad4:
	movs r3, #0
	ldrsb r3, [r6, r3]
.L_08171ad8:
	str r3, [sp, #24]
.L_08171ada:
	ldr r4, [sp, #24]
	mov r0, r11
	str r4, [r0, #20]
	bl Func_08014de4
	mov r0, r10
	adds r1, r7, #0
	movs r2, #0
	bl Func_08015160
	lsls r1, r5, #1
	adds r0, r5, #0
	adds r2, r5, #0
	bl Func_080151e4
	mov r1, r8
	ldr r0, .L_08171b6c
	movs r2, #4
	bl Func_08196958
	mov r0, r11
	bl Func_08196a7c
	ldr r1, [sp, #52]
	adds r6, #1
	adds r1, #1
	str r1, [sp, #52]
	cmp r1, #4
	beq .L_08171b16
	b .L_081719fc
.L_08171b16:
	mov r0, r11
	bl Sys_Free
	mov r0, r8
	bl Sys_Free
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #64]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #56]
	adds r0, #1
	str r0, [sp, #56]
	cmp r0, #74
	beq .L_08171b4e
	b .L_0817154c
.L_08171b4e:
	ldr r0, .L_08171b70
	bl Func_08014644
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #116
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08171b6c:
	.4byte Data_081991e0
.L_08171b70:
	.4byte Func_08143000
