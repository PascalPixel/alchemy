.syntax unified
	.thumb
	.global Func_081504d8
	.thumb_func
Func_081504d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #108
	str r1, [sp, #56]
	str r0, [sp, #60]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #52]
	movs r0, #0
	ldr r3, [r3, #96]
	str r3, [sp, #48]
	bl Func_081435e0
	movs r2, #128
	ldr r3, .L_08150528
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r1, [sp, #56]
	cmp r1, #0
	beq .L_08150512
	cmp r1, #3
	bne .L_08150530
.L_08150512:
	ldr r2, [sp, #52]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0815052c
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	b .L_08150542
	.2byte 0x0000
.L_08150528:
	.4byte 0x00000100
.L_0815052c:
	.4byte 0x0000015f
.L_08150530:
	ldr r5, [sp, #52]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r5, r2
	ldr r0, .L_081507f0
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
.L_08150542:
	ldr r3, [sp, #56]
	cmp r3, #0
	beq .L_0815055c
	ldr r5, [sp, #56]
	cmp r5, #3
	bne .L_08150552
	ldr r0, .L_081507f4
	b .L_0815055e
.L_08150552:
	ldr r0, [sp, #56]
	cmp r0, #1
	bne .L_0815055c
	ldr r0, .L_081507f4
	b .L_0815055e
.L_0815055c:
	ldr r0, .L_081507f8
.L_0815055e:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_081507fc
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r2, [sp, #52]
	movs r3, #178
	lsls r3, r3, #6
	adds r1, r2, r3
	ldr r0, .L_08150800
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r0, #174
	movs r5, #1
	lsls r0, r0, #2
	mov r9, r5
	mov lr, r0
	movs r4, #57
.L_0815058e:
	ldr r3, [sp, #52]
	movs r2, #178
	movs r1, #0
	lsls r2, r2, #6
	str r1, [sp, #40]
	adds r1, r3, r2
	adds r3, r0, r3
	mov r12, r4
	adds r3, r3, r2
.L_081505a0:
	ldrb r2, [r1]
	adds r1, #1
	cmp r2, r12
	ble .L_081505aa
	mov r2, r12
.L_081505aa:
	cmp r2, #0
	bge .L_081505b0
	movs r2, #0
.L_081505b0:
	strb r2, [r3]
	ldr r5, [sp, #40]
	adds r3, #1
	adds r5, #1
	str r5, [sp, #40]
	cmp r5, lr
	bne .L_081505a0
	movs r2, #1
	movs r1, #174
	add r9, r2
	lsls r1, r1, #2
	mov r3, r9
	adds r0, r0, r1
	subs r4, #7
	cmp r3, #8
	bne .L_0815058e
	ldr r5, [sp, #60]
	ldr r3, [r5, #4]
	cmp r3, #1
	bne .L_081505ea
	movs r2, #128
	ldr r3, .L_08150804
	lsls r2, r2, #19
	movs r0, #112
	adds r2, #40
	negs r0, r0
	str r3, [r2]
	str r0, [sp, #36]
	b .L_081505f6
.L_081505ea:
	movs r2, #128
	lsls r2, r2, #19
	movs r3, #0
	adds r2, #40
	str r3, [r2]
	str r3, [sp, #36]
.L_081505f6:
	ldr r5, .L_08150808
	movs r1, #0
	mov r9, r1
	movs r7, #192
.L_081505fe:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r6, r0, #0
	ands r6, r3
	movs r3, #0
	str r3, [r5]
	ldr r2, [sp, #56]
	cmp r2, #0
	beq .L_0815061a
	cmp r2, #3
	bne .L_0815064a
.L_0815061a:
	movs r2, #31
	mov r3, r9
	ands r2, r3
	cmp r2, #0
	bge .L_08150626
	adds r2, #3
.L_08150626:
	asrs r2, r2, #2
	lsls r3, r2, #1
	ldr r0, .L_0815080c
	adds r3, r3, r2
	lsls r3, r3, #17
	adds r3, r3, r0
	str r3, [r5, #4]
	mov r3, r9
	cmp r3, #0
	bge .L_0815063c
	adds r3, #3
.L_0815063c:
	asrs r3, r3, #2
	lsls r3, r3, #2
	mov r1, r9
	ldr r2, .L_08150810
	subs r3, r1, r3
	lsls r3, r3, #17
	b .L_08150678
.L_0815064a:
	movs r2, #31
	mov r3, r9
	ands r2, r3
	cmp r2, #0
	bge .L_08150656
	adds r2, #3
.L_08150656:
	asrs r2, r2, #2
	lsls r3, r2, #1
	ldr r0, .L_0815080c
	adds r3, r3, r2
	lsls r3, r3, #17
	adds r3, r3, r0
	str r3, [r5, #4]
	mov r3, r9
	cmp r3, #0
	bge .L_0815066c
	adds r3, #3
.L_0815066c:
	asrs r3, r3, #2
	lsls r3, r3, #2
	mov r1, r9
	ldr r2, .L_08150814
	subs r3, r1, r3
	lsls r3, r3, #19
.L_08150678:
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r0, [sp, #60]
	ldr r3, [r0, #4]
	cmp r3, #1
	bne .L_0815068a
	movs r3, #128
	lsls r3, r3, #10
	b .L_0815068c
.L_0815068a:
	ldr r3, .L_08150810
.L_0815068c:
	str r3, [r5, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	movs r1, #128
	lsls r1, r1, #9
	asrs r3, r3, #6
	adds r3, r3, r1
	str r3, [r5, #16]
	adds r0, r6, #0
	bl Trig_Sin
	adds r3, r7, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r5, #20]
	bl Random16
	movs r3, #255
	ands r3, r0
	str r3, [r5, #24]
	movs r2, #1
	movs r3, #128
	add r9, r2
	lsls r3, r3, #2
	adds r5, #28
	cmp r9, r3
	bne .L_081505fe
	ldr r5, [sp, #60]
	mov r1, sp
	ldr r0, [r5, #4]
	adds r1, #64
	str r1, [sp, #32]
	bl Func_08144aac
	ldr r3, [sp, #52]
	movs r5, #239
	lsls r5, r5, #7
	adds r2, r3, r5
	movs r3, #2
	str r3, [r2]
	ldr r0, [sp, #52]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #132
	adds r2, r0, r1
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_08150818
	lsls r1, r1, #4
	bl Func_080145a8
	movs r2, #0
	ldr r3, [sp, #60]
	str r2, [sp, #44]
	movs r5, #64
	ldr r2, [r3, #20]
	negs r5, r5
	lsls r3, r2, #2
	cmp r3, r5
	bne .L_0815070e
	b .L_0815097a
.L_0815070e:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	ldr r0, [sp, #44]
	str r3, [sp, #28]
	cmp r0, #72
	bne .L_08150726
	movs r0, #0
	bl Func_081180e8
	ldr r1, [sp, #60]
	ldr r2, [r1, #20]
.L_08150726:
	movs r3, #0
	str r3, [sp, #40]
	cmp r2, #0
	bne .L_08150730
	b .L_08150950
.L_08150730:
	ldr r5, [sp, #44]
	ldr r2, [sp, #28]
	subs r5, #24
	ldr r0, [sp, #44]
	movs r1, #36
	adds r2, #12
	str r5, [sp, #16]
	str r1, [sp, #12]
	str r2, [sp, #24]
	str r3, [sp, #8]
	mov r8, r0
.L_08150746:
	ldr r3, [sp, #12]
	ldr r1, [sp, #60]
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlotFar
	mov r2, r8
	ldr r6, [r0]
	cmp r2, #0
	bgt .L_0815075a
	b .L_08150922
.L_0815075a:
	bl Func_08014de4
	ldr r0, [sp, #28]
	ldr r1, [sp, #24]
	bl Func_080156e8
	ldr r3, [r6, #8]
	add r5, sp, #72
	str r3, [r5]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_08014de4
	ldr r0, [sp, #28]
	ldr r1, [sp, #24]
	bl Func_080156e8
	adds r0, r5, #0
	bl Func_08015128
	movs r3, #0
	add r0, sp, #96
	add r5, sp, #84
	str r3, [r0]
	str r3, [r0, #4]
	str r3, [r0, #8]
	adds r1, r5, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	ldr r0, [sp, #36]
	ldr r1, [r5, #4]
	ldr r2, [sp, #56]
	adds r6, r3, r0
	mov r10, r1
	cmp r2, #0
	beq .L_081507ae
	cmp r2, #3
	bne .L_0815081c
.L_081507ae:
	mov r3, r8
	cmp r3, #26
	bgt .L_0815085e
	mov r0, r8
	cmp r3, #0
	bge .L_081507bc
	adds r0, #3
.L_081507bc:
	movs r1, #7
	asrs r0, r0, #2
	bl __modsi3
	lsls r1, r0, #4
	subs r1, r1, r0
	ldr r0, [sp, #52]
	lsls r1, r1, #6
	movs r2, #224
	adds r1, r0, r1
	lsls r2, r2, #3
	movs r0, #24
	adds r1, r1, r2
	mov r3, r10
	adds r2, r6, #0
	str r0, [sp, #0]
	movs r0, #40
	str r0, [sp, #4]
	subs r2, #12
	subs r3, #20
	ldr r4, [sp, #64]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	b .L_0815085e
	.2byte 0x0000
.L_081507f0:
	.4byte 0x0000015e
.L_081507f4:
	.4byte 0x0000017b
.L_081507f8:
	.4byte 0x0000017f
.L_081507fc:
	.4byte IwramCopyWords
.L_08150800:
	.4byte 0x00000160
.L_08150804:
	.4byte 0xffff9000
.L_08150808:
	.4byte gMapCellBuffer
.L_0815080c:
	.4byte 0xfff60000
.L_08150810:
	.4byte 0xfffe0000
.L_08150814:
	.4byte 0xfff00000
.L_08150818:
	.4byte Func_08143000
.L_0815081c:
	mov r3, r8
	cmp r3, #23
	bgt .L_0815085e
	mov r0, r8
	cmp r3, #0
	bge .L_0815082a
	adds r0, #3
.L_0815082a:
	movs r1, #6
	asrs r0, r0, #2
	bl __modsi3
	lsls r1, r0, #1
	adds r1, r1, r0
	lsls r1, r1, #3
	adds r1, r1, r0
	ldr r0, [sp, #52]
	lsls r1, r1, #6
	adds r1, r0, r1
	movs r0, #40
	str r0, [sp, #0]
	str r0, [sp, #4]
	movs r2, #224
	ldr r0, [sp, #32]
	lsls r2, r2, #3
	adds r1, r1, r2
	mov r3, r10
	adds r2, r6, #0
	ldr r4, [r0, #4]
	subs r2, #20
	subs r3, #20
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
.L_0815085e:
	mov r1, r8
	cmp r1, #24
	bne .L_0815086a
	movs r0, #143
	bl Audio_PlayCue
.L_0815086a:
	ldr r2, [sp, #16]
	cmp r2, #36
	bhi .L_08150922
	mov r3, r8
	movs r1, #0
	cmp r3, #28
	ble .L_0815088a
	adds r3, r2, #0
	cmp r2, #0
	bge .L_08150882
	mov r3, r8
	subs r3, #21
.L_08150882:
	asrs r1, r3, #2
	cmp r1, #7
	ble .L_0815088a
	movs r1, #7
.L_0815088a:
	movs r3, #174
	lsls r3, r3, #2
	adds r2, r1, #0
	muls r2, r3
	mov r11, r5
	ldr r3, [sp, #8]
	ldr r5, .L_081509a0
	str r2, [sp, #20]
	movs r0, #0
	mov r9, r0
	adds r7, r3, r5
.L_081508a0:
	mov r3, r9
	cmp r3, #0
	bge .L_081508a8
	adds r3, #3
.L_081508a8:
	asrs r3, r3, #2
	mov r0, r9
	lsls r3, r3, #2
	subs r3, r0, r3
	lsls r2, r3, #1
	adds r6, r2, r3
	ldr r3, [r7, #24]
	mov r1, r8
	adds r0, r3, r1
	cmp r0, #0
	bge .L_081508c0
	adds r0, #7
.L_081508c0:
	movs r1, #3
	asrs r0, r0, #3
	bl __modsi3
	mov r1, r11
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0815e1ec
	mov r2, r11
	ldr r3, [r2]
	ldr r0, [sp, #36]
	ldr r1, [r2, #4]
	ldr r2, .L_081509a4
	adds r5, r6, r5
	adds r6, r3, r0
	lsls r3, r5, #1
	mov r10, r1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #20]
	ldr r3, [sp, #52]
	adds r1, r2, r1
	adds r1, r3, r1
	ldr r3, .L_081509a8
	movs r0, #178
	ldrb r3, [r3, r5]
	lsls r0, r0, #6
	str r3, [sp, #0]
	ldr r3, .L_081509ac
	adds r1, r1, r0
	ldrb r3, [r3, r5]
	ldr r4, [sp, #64]
	str r3, [sp, #4]
	ldr r0, [sp, #48]
	adds r2, r6, #0
	mov r3, r10
	mov lr, r4
	.2byte 0xf800
	adds r0, r7, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r7, #28
	cmp r2, #24
	bne .L_081508a0
.L_08150922:
	ldr r3, [sp, #16]
	ldr r0, [sp, #12]
	subs r3, #4
	str r3, [sp, #16]
	ldr r1, [sp, #8]
	ldr r3, [sp, #40]
	movs r2, #224
	movs r5, #4
	lsls r2, r2, #2
	negs r5, r5
	adds r0, #2
	adds r3, #1
	adds r1, r1, r2
	str r0, [sp, #12]
	str r3, [sp, #40]
	add r8, r5
	str r1, [sp, #8]
	ldr r5, [sp, #60]
	ldr r0, [sp, #40]
	ldr r3, [r5, #20]
	cmp r0, r3
	beq .L_08150950
	b .L_08150746
.L_08150950:
	ldr r1, [sp, #52]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r5, [sp, #44]
	ldr r0, [sp, #60]
	adds r5, #1
	str r5, [sp, #44]
	ldr r3, [r0, #20]
	adds r2, r3, #0
	lsls r3, r2, #2
	adds r3, #64
	cmp r5, r3
	beq .L_0815097a
	b .L_0815070e
.L_0815097a:
	ldr r0, .L_081509b0
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #108
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081509a0:
	.4byte gMapCellBuffer
.L_081509a4:
	.4byte Data_081974dc
.L_081509a8:
	.4byte Data_081974f4
.L_081509ac:
	.4byte Data_08197500
.L_081509b0:
	.4byte Func_08143000
