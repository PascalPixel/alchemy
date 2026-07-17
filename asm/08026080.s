@ Reconstructed thumb disassembly of a call-graph-reached code region
@ (code interleaved with its data), verified byte-identical by build_asm.
.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002322, 0x08002322
	.set sub_08003d28, 0x08003d28
	.set sub_08003dec, 0x08003dec
	.set sub_080040b4, 0x080040b4
	.set sub_080040d0, 0x080040d0
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_0801e71c, 0x0801e71c
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_0801e8b0, 0x0801e8b0
	.set sub_0801ea08, 0x0801ea08
	.set sub_08026a5a, 0x08026a5a
	.set sub_08026a64, 0x08026a64
	.set sub_08026a6a, 0x08026a6a
	.set sub_08026a72, 0x08026a72
	.set sub_08026a84, 0x08026a84
	.set sub_08026b8c, 0x08026b8c
	.set sub_08026b96, 0x08026b96
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_080b50b8, 0x080b50b8
	.set sub_080b50e0, 0x080b50e0
	.global Region_08026080
Region_08026080:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #324
	str r2, [sp, #84]
	str r3, [sp, #80]
	ldr r5, [pc, #272]
	mov r10, r0
	ldr r0, [r5, #0]
	ldr r2, [pc, #272]
	str r0, [sp, #76]
	movs r0, #128
	mov r8, r1
	lsls r0, r0, #1
	movs r1, #0
	str r1, [sp, #64]
	str r2, [sp, #56]
	bl sub_080040b4
	ldr r4, [sp, #84]
	movs r3, #0
	str r0, [sp, #52]
	mov r9, r3
	cmp r4, #0
	bne .L_080260be
	movs r6, #1
	str r6, [sp, #84]
.L_080260be:
	mov r7, r8
	cmp r7, #2
	beq .L_080260c8
	cmp r7, #4
	bne .L_080260d4
.L_080260c8:
	adds r3, r5, #0
	adds r3, #192
	ldr r2, [r3, #0]
	movs r3, #2
	negs r3, r3
	b .L_080260dc
.L_080260d4:
	adds r3, r5, #0
	adds r3, #192
	ldr r2, [r3, #0]
	movs r3, #16
.L_080260dc:
	str r3, [r2, #40]
	mov r0, sp
	adds r0, #212
	mov r3, sp
	str r0, [sp, #36]
	movs r2, #0
	movs r7, #5
	adds r3, #234
.L_080260ec:
	subs r7, #1
	strb r2, [r3, #0]
	subs r3, #4
	cmp r7, #0
	bge .L_080260ec
	movs r1, #1
	negs r1, r1
	mov r2, r8
	str r1, [sp, #68]
	cmp r2, #2
	bne .L_0802613e
	ldr r4, [sp, #76]
	movs r3, #88
	ldrsh r3, [r4, r3]
	movs r7, #0
	cmp r3, #255
	beq .L_08026194
	movs r6, #154
	lsls r6, r6, #1
	ldr r0, [sp, #64]
	add r6, sp
	adds r2, r4, #0
	lsls r3, r0, #1
	str r6, [sp, #28]
	adds r2, #88
	adds r1, r3, r6
.L_08026120:
	ldrh r3, [r2, #0]
	strh r3, [r1, #0]
	ldr r3, [sp, #64]
	adds r7, #1
	adds r3, #1
	adds r1, #2
	str r3, [sp, #64]
	adds r2, #2
	cmp r7, #5
	bgt .L_080261b8
	movs r4, #0
	ldrsh r3, [r2, r4]
	cmp r3, #255
	bne .L_08026120
	b .L_080261b8
.L_0802613e:
	mov r5, r8
	cmp r5, #4
	bne .L_08026158
	movs r6, #154
	lsls r6, r6, #1
	add r6, sp
	mov r7, r10
	adds r0, r6, #0
	movs r1, #1
	str r6, [sp, #28]
	strh r7, [r0, #0]
	str r1, [sp, #64]
	b .L_080261b8
.L_08026158:
	ldr r1, [sp, #76]
	movs r3, #100
	adds r1, #2
	ldrsh r3, [r1, r3]
	movs r7, #0
	cmp r3, #255
	beq .L_080261b0
	movs r3, #154
	lsls r3, r3, #1
	add r3, sp
	str r3, [sp, #28]
	ldr r4, [sp, #64]
	ldr r5, [sp, #28]
	lsls r3, r4, #1
	movs r0, #100
	adds r2, r3, r5
.L_08026178:
	ldrh r3, [r1, r0]
	strh r3, [r2, #0]
	ldr r6, [sp, #64]
	adds r7, #1
	adds r6, #1
	adds r2, #2
	str r6, [sp, #64]
	adds r0, #2
	cmp r7, #5
	bgt .L_080261b8
	ldrsh r3, [r1, r0]
	cmp r3, #255
	bne .L_08026178
	b .L_080261b8
.L_08026194:
	movs r5, #154
	lsls r5, r5, #1
	add r5, sp
	str r5, [sp, #28]
	b .L_080261b8
.L_0802619e:
	ldr r6, [sp, #28]
	mov r7, r11
	ldrh r6, [r6, r7]
	mov r10, r6
	b .L_080262b6
	.4byte 0x03001e74
	.4byte 0x0000ffff
.L_080261b0:
	movs r0, #154
	lsls r0, r0, #1
	add r0, sp
	str r0, [sp, #28]
.L_080261b8:
	ldr r1, [sp, #64]
	ldr r3, [pc, #44]
	ldr r4, [sp, #28]
	lsls r2, r1, #1
	mov r5, r8
	strh r3, [r4, r2]
	str r1, [sp, #60]
	cmp r5, #2
	beq .L_080261cc
	b .L_080262e0
.L_080261cc:
	ldr r6, [sp, #84]
	cmp r6, #255
	beq .L_080262b6
	ldr r7, [sp, #80]
	cmp r7, #0
	beq .L_080262b6
	movs r5, #0
	movs r7, #0
	cmp r5, r1
	bge .L_080262b6
	ldr r4, [pc, #8]
	movs r6, #0
	b .L_080261f0
	.2byte 0x0000
	.4byte 0x000000ff
	.4byte 0x0000ffff
.L_080261f0:
	ldr r0, [sp, #28]
	ldrh r3, [r6, r0]
	mov r11, r6
	cmp r3, #254
	beq .L_080262ac
	adds r0, r3, #0
	str r4, [sp, #8]
	bl sub_08077008
	ldr r2, [sp, #80]
	adds r1, r0, #0
	ldr r4, [sp, #8]
	cmp r2, #4
	beq .L_08026240
	cmp r2, #4
	bhi .L_08026216
	cmp r2, #3
	beq .L_08026234
	b .L_080262a6
.L_08026216:
	ldr r3, [sp, #80]
	cmp r3, #5
	beq .L_08026222
	cmp r3, #6
	beq .L_08026264
	b .L_080262a6
.L_08026222:
	movs r0, #56
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_080262a6
	ldr r1, [sp, #28]
	ldrh r1, [r6, r1]
	movs r7, #1
	mov r10, r1
	b .L_080262a6
.L_08026234:
	ldr r2, [pc, #324]
	adds r3, r1, r2
	ldrb r3, [r3, #0]
	lsls r3, r3, #24
	asrs r3, r3, #24
	b .L_080262a0
.L_08026240:
	movs r0, #156
	lsls r0, r0, #1
	adds r3, r1, r0
	ldr r3, [r3, #0]
	ldr r2, [pc, #308]
	ands r3, r2
	cmp r3, #0
	bne .L_080262a4
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3, #0]
	ands r3, r4
	cmp r3, #0
	bne .L_080262a4
	adds r0, #9
	adds r3, r1, r0
	b .L_0802629e
.L_08026264:
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r1, r2
	ldr r3, [r3, #0]
	ldr r2, [pc, #272]
	ands r3, r2
	cmp r3, #0
	bne .L_080262a4
	movs r0, #158
	lsls r0, r0, #1
	adds r3, r1, r0
	ldrh r3, [r3, #0]
	ands r3, r4
	cmp r3, #0
	bne .L_080262a4
	ldr r2, [pc, #256]
	adds r3, r1, r2
	ldrb r3, [r3, #0]
	cmp r3, #0
	bne .L_080262a4
	subs r0, #11
	adds r3, r1, r0
	ldrb r3, [r3, #0]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080262a4
	subs r2, #1
	adds r3, r1, r2
.L_0802629e:
	ldrb r3, [r3, #0]
.L_080262a0:
	cmp r3, #0
	beq .L_080262a6
.L_080262a4:
	movs r7, #1
.L_080262a6:
	cmp r7, #0
	beq .L_080262ac
	b .L_0802619e
.L_080262ac:
	ldr r3, [sp, #64]
	adds r5, #1
	adds r6, #2
	cmp r5, r3
	blt .L_080261f0
.L_080262b6:
	ldr r4, [sp, #64]
	movs r5, #0
	cmp r5, r4
	bge .L_080262d8
	ldr r6, [sp, #28]
	ldrh r3, [r6, #0]
	cmp r3, r10
	beq .L_080262d8
	adds r2, r6, #0
.L_080262c8:
	ldr r7, [sp, #64]
	adds r5, #1
	cmp r5, r7
	bge .L_080262d8
	adds r2, #2
	ldrh r3, [r2, #0]
	cmp r3, r10
	bne .L_080262c8
.L_080262d8:
	ldr r0, [sp, #64]
	cmp r5, r0
	beq .L_080262e0
	str r5, [sp, #68]
.L_080262e0:
	ldr r1, [sp, #68]
	cmp r1, #0
	bge .L_08026308
	ldr r3, [sp, #64]
	subs r3, #1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #68]
	b .L_08026308
.L_080262f4:
	ldr r2, [sp, #68]
	ldr r4, [sp, #64]
	adds r3, r2, r4
	subs r3, #1
	adds r0, r3, #0
	adds r1, r4, #0
	str r3, [sp, #68]
	bl sub_080022fc
	str r0, [sp, #68]
.L_08026308:
	ldr r5, [sp, #68]
	lsls r5, r5, #1
	str r5, [sp, #24]
	ldr r6, [sp, #28]
	ldrh r3, [r6, r5]
	cmp r3, #254
	beq .L_080262f4
	movs r0, #182
	lsls r0, r0, #1
	bl sub_080770c0
	cmp r0, #0
	beq .L_08026336
	mov r7, r8
	cmp r7, #1
	bne .L_08026336
	ldrh r0, [r6, r5]
	bl sub_08077008
	movs r1, #56
	ldrsh r3, [r0, r1]
	cmp r3, #0
	beq .L_080262f4
.L_08026336:
	mov r2, r8
	cmp r2, #2
	beq .L_08026354
	add r5, sp, #200
	mov r0, r10
	adds r1, r5, #0
	bl sub_080b50b8
	ldr r4, [sp, #36]
	movs r3, #8
	strb r3, [r4, #2]
	ldr r3, [r5, #0]
	strb r3, [r4, #0]
	movs r3, #128
	strb r3, [r4, #1]
.L_08026354:
	movs r3, #74
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #30
	movs r3, #4
	bl sub_080162d4
	movs r6, #152
	mov r5, sp
	mov r7, sp
	adds r5, #236
	add r6, sp
	adds r7, #88
	str r0, [sp, #72]
	str r5, [sp, #32]
	mov r11, r6
	str r7, [sp, #20]
	b .L_0802638e
	.2byte 0x0000
	.4byte 0x00000131
	.4byte 0xff0000ff
	.4byte 0x00000141
	ldr r0, [sp, #68]
	lsls r0, r0, #1
	str r0, [sp, #24]
.L_0802638e:
	movs r1, #0
	str r1, [sp, #48]
	ldr r3, [sp, #24]
	ldr r2, [sp, #28]
	mov r1, r11
	ldrh r0, [r2, r3]
	bl sub_080b50b8
	ldr r3, [pc, #72]
	ldr r4, [sp, #32]
	str r3, [r4, #4]
	ldr r5, [sp, #48]
	str r5, [r4, #8]
	ldr r5, [pc, #64]
	ldr r1, [r5, #0]
	movs r3, #31
	lsrs r1, r1, #2
	ands r1, r3
	ldr r3, [pc, #60]
	lsls r1, r1, #8
	adds r1, r1, r3
	ldr r0, [sp, #52]
	bl sub_080040d0
	ldr r3, [pc, #36]
	ldr r6, [sp, #32]
	ands r0, r3
	ldrh r2, [r6, #8]
	ldr r3, [pc, #44]
	ands r3, r2
	orrs r3, r0
	ldr r0, [r5, #0]
	adds r7, r6, #0
	strh r3, [r7, #8]
	lsls r0, r0, #12
	bl sub_08002322
	cmp r0, #0
	bge .L_080263fc
	ldr r1, [pc, #24]
	adds r0, r0, r1
	b .L_080263fc
	.2byte 0x0000
	.4byte 0x000003ff
	.4byte 0x40002000
	.4byte 0x03001e40
	.4byte 0x080346f8
	.4byte 0xfffffc00
	.4byte 0x00007fff
.L_080263fc:
	mov r4, r11
	ldr r3, [r4, #4]
	asrs r2, r0, #15
	adds r0, r3, r2
	str r0, [r4, #4]
	ldr r5, [sp, #36]
	movs r1, #1
	ldrb r2, [r5, #2]
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0802644e
	ldr r4, [r4, #0]
	ldrb r3, [r5, #0]
	adds r3, r4, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r1, r3, #1
	ldrb r3, [r5, #1]
	adds r3, r0, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r0, r3, #1
	subs r3, r4, r1
	cmp r3, #0
	blt .L_08026436
	cmp r3, #7
	ble .L_0802643c
	b .L_08026440
.L_08026436:
	subs r3, r1, r4
	cmp r3, #7
	bgt .L_08026440
.L_0802643c:
	movs r6, #1
	str r6, [sp, #48]
.L_08026440:
	mov r7, r11
	str r1, [r7, #0]
	ldr r2, [sp, #36]
	str r0, [r7, #4]
	strb r1, [r2, #0]
	strb r0, [r2, #1]
	b .L_08026486
.L_0802644e:
	movs r4, #192
	lsls r3, r2, #24
	lsls r4, r4, #18
	cmp r3, r4
	bhi .L_08026468
	mov r5, r11
	ldr r6, [sp, #36]
	ldr r3, [r5, #0]
	str r0, [r5, #4]
	strb r3, [r6, #0]
	strb r0, [r6, #1]
	strb r1, [r6, #2]
	b .L_08026486
.L_08026468:
	ldr r7, [sp, #36]
	ldrb r3, [r7, #0]
	mov r0, r11
	str r3, [r0, #0]
	ldrb r3, [r7, #1]
	str r3, [r0, #4]
	adds r3, r2, #0
	adds r3, #252
	movs r2, #192
	strb r3, [r7, #2]
	lsls r2, r2, #18
	lsls r3, r3, #24
	cmp r3, r2
	bhi .L_08026486
	strb r1, [r7, #2]
.L_08026486:
	mov r3, r11
	ldr r2, [r3, #0]
	ldr r4, [sp, #32]
	ldr r3, [pc, #48]
	subs r2, #8
	ldrh r1, [r4, #6]
	ands r2, r3
	ldr r3, [pc, #44]
	ands r3, r1
	orrs r3, r2
	adds r5, r4, #0
	mov r6, r11
	strh r3, [r5, #6]
	ldr r3, [r6, #4]
	subs r3, #16
	strb r3, [r5, #4]
	ldr r0, [sp, #32]
	movs r1, #240
	bl sub_08003dec
	ldr r7, [sp, #84]
	cmp r7, #255
	bne .L_080264d8
	ldr r2, [pc, #16]
	ldr r3, [sp, #88]
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #1
	b .L_080264cc
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0xffff0000
.L_080264cc:
	orrs r3, r2
	ldr r2, [pc, #284]
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #17
	b .L_080264ea
.L_080264d8:
	ldr r2, [pc, #276]
	ldr r3, [sp, #88]
	ands r3, r2
	movs r2, #176
	orrs r3, r2
	ldr r2, [pc, #264]
	ands r3, r2
	movs r2, #176
	lsls r2, r2, #16
.L_080264ea:
	orrs r3, r2
	str r3, [sp, #88]
	ldr r0, [sp, #20]
	ldr r3, [pc, #252]
	ldr r2, [r0, #4]
	ands r2, r3
	str r2, [r0, #4]
	ldr r0, [sp, #20]
	bl sub_08003d28
	ldr r1, [sp, #56]
	movs r3, #1
	ands r3, r1
	str r0, [sp, #44]
	cmp r3, #0
	bne .L_0802650c
	b sub_08026b96
.L_0802650c:
	movs r2, #0
	str r2, [sp, #64]
	ldr r1, [sp, #36]
	movs r0, #253
	movs r7, #5
.L_08026516:
	ldrb r2, [r1, #2]
	adds r3, r0, #0
	ands r3, r2
	subs r7, #1
	strb r3, [r1, #2]
	adds r1, #4
	cmp r7, #0
	bge .L_08026516
	ldr r3, [sp, #84]
	movs r7, #0
	cmp r7, r3
	bcs .L_080265f4
	ldr r6, [sp, #64]
	add r4, sp, #172
	add r0, sp, #324
	ldr r1, [sp, #64]
	adds r3, r6, r0
	mov r10, r4
	ldr r5, [sp, #28]
	ldr r4, [sp, #36]
	adds r6, r3, #0
	mov r2, r10
	lsls r3, r1, #1
	mov lr, r5
	adds r0, r4, #0
	adds r5, r3, r2
	movs r3, #254
	subs r6, #160
	mov r8, r3
	adds r0, #24
.L_08026552:
	ldr r1, [sp, #68]
	ldr r2, [sp, #60]
	adds r3, r1, r7
	cmp r3, r2
	bge .L_08026592
	lsls r3, r3, #1
	mov r1, lr
	ldrh r2, [r1, r3]
	adds r3, r2, #0
	cmp r3, #254
	beq .L_08026592
	ldrb r3, [r4, #2]
	strh r2, [r5, #0]
	movs r2, #2
	orrs r2, r3
	movs r3, #0
	orrs r2, r3
	movs r3, #3
	ldrsb r3, [r4, r3]
	strb r2, [r4, #2]
	cmp r3, r7
	beq .L_08026586
	mov r1, r8
	ands r2, r1
	strb r2, [r4, #2]
	strb r7, [r4, #3]
.L_08026586:
	strb r7, [r6, #0]
	ldr r2, [sp, #64]
	adds r2, #1
	str r2, [sp, #64]
	adds r6, #1
	adds r5, #2
.L_08026592:
	cmp r7, #0
	beq .L_080265de
	ldr r1, [sp, #68]
	subs r3, r1, r7
	cmp r3, #0
	blt .L_080265de
	lsls r3, r3, #1
	mov r1, lr
	ldrh r2, [r1, r3]
	adds r3, r2, #0
	cmp r3, #254
	beq .L_080265de
	strh r2, [r5, #0]
	movs r2, #6
	ldrb r3, [r0, #2]
	subs r2, r2, r7
	mov r12, r2
	movs r2, #2
	orrs r2, r3
	movs r3, #0
	orrs r2, r3
	movs r3, #3
	ldrsb r3, [r0, r3]
	negs r1, r7
	strb r2, [r0, #2]
	cmp r3, r1
	beq .L_080265d0
	mov r3, r8
	ands r2, r3
	strb r2, [r0, #2]
	strb r1, [r0, #3]
.L_080265d0:
	mov r1, r12
	strb r1, [r6, #0]
	ldr r2, [sp, #64]
	adds r2, #1
	str r2, [sp, #64]
	adds r6, #1
	adds r5, #2
.L_080265de:
	ldr r3, [sp, #84]
	adds r7, #1
	adds r4, #4
	subs r0, #4
	cmp r7, r3
	bcc .L_08026552
	b .L_080265f8
	.4byte 0x0000ffff
	.4byte 0xffff0000
.L_080265f4:
	add r4, sp, #172
	mov r10, r4
.L_080265f8:
	ldr r1, [sp, #36]
	movs r4, #2
	movs r0, #6
	movs r7, #5
.L_08026600:
	ldrb r2, [r1, #2]
	adds r3, r4, #0
	ands r3, r2
	cmp r3, #0
	bne .L_0802660c
	strb r0, [r1, #3]
.L_0802660c:
	subs r7, #1
	adds r1, #4
	cmp r7, #0
	bge .L_08026600
	ldr r5, [sp, #64]
	ldr r2, [pc, #44]
	lsls r3, r5, #1
	mov r6, r10
	strh r2, [r6, r3]
	mov r0, r10
	movs r1, #1
	bl sub_080b50e0
	ldr r7, [sp, #28]
	ldr r0, [sp, #24]
	ldrh r3, [r7, r0]
	cmp r3, #7
	bls .L_08026632
	b sub_08026a84
.L_08026632:
	ldr r1, [sp, #84]
	cmp r1, #255
	bne .L_0802663a
	b sub_08026b8c
.L_0802663a:
	ldr r2, [sp, #80]
	cmp r2, #0
	bne .L_08026642
	b sub_08026b8c
.L_08026642:
	b .L_08026648
	.4byte 0x000000ff
.L_08026648:
	adds r0, r3, #0
	bl sub_08077008
	ldr r3, [sp, #24]
	adds r6, r0, #0
	mov r1, r11
	ldrh r0, [r7, r3]
	bl sub_080b50b8
	mov r4, r9
	cmp r4, #0
	beq .L_08026668
	mov r0, r9
	movs r1, #1
	bl sub_08016418
.L_08026668:
	ldr r3, [sp, #80]
	subs r3, #1
	cmp r3, #6
	bls .L_08026672
	b sub_08026b8c
.L_08026672:
	ldr r2, [pc, #616]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
	.4byte 0x080266c8
	.4byte 0x0802671e
	.4byte 0x080267b4
	.4byte 0x080267f8
	.4byte 0x08026780
	.4byte 0x0802691c
	.4byte 0x08026698
	mov r5, r11
	ldr r3, [r5, #0]
	cmp r3, #0
	bge .L_080266a2
	adds r3, #7
.L_080266a2:
	asrs r3, r3, #3
	subs r0, r3, #4
	adds r3, #4
	cmp r3, #29
	ble .L_080266ae
	movs r0, #22
.L_080266ae:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #9
	movs r3, #3
	bl sub_080162d4
	mov r9, r0
	movs r0, #2
	bl sub_0801e71c
	ldr r0, [pc, #536]
	b sub_08026a72
	mov r7, r11
	ldr r3, [r7, #0]
	cmp r3, #0
	bge .L_080266d2
	adds r3, #7
.L_080266d2:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #6
	cmp r3, #29
	ble .L_080266de
	movs r0, #17
.L_080266de:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #13
	movs r3, #3
	bl sub_080162d4
	mov r9, r0
	mov r1, r9
	ldr r0, [pc, #496]
	movs r2, #0
	movs r3, #0
	bl sub_0801e8b0
	movs r5, #0
	movs r1, #56
	ldrsh r0, [r6, r1]
	mov r2, r9
	movs r1, #4
	movs r3, #16
	str r5, [sp, #0]
	bl sub_0801ea08
	movs r2, #48
	ldr r0, [pc, #472]
	mov r1, r9
	movs r3, #0
	bl sub_0801e8b0
	movs r2, #52
	ldrsh r0, [r6, r2]
	b .L_08026772
	mov r4, r11
	ldr r3, [r4, #0]
	cmp r3, #0
	bge .L_08026728
	adds r3, #7
.L_08026728:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #6
	cmp r3, #29
	ble .L_08026734
	movs r0, #17
.L_08026734:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #13
	movs r3, #3
	bl sub_080162d4
	mov r9, r0
	mov r1, r9
	ldr r0, [pc, #420]
	movs r2, #0
	movs r3, #0
	bl sub_0801e8b0
	movs r5, #58
	ldrsh r0, [r6, r5]
	movs r1, #4
	movs r5, #0
	mov r2, r9
	movs r3, #16
	str r5, [sp, #0]
	bl sub_0801ea08
	ldr r0, [pc, #388]
	mov r1, r9
	movs r2, #48
	movs r3, #0
	bl sub_0801e8b0
	movs r7, #54
	ldrsh r0, [r6, r7]
.L_08026772:
	movs r1, #4
	mov r2, r9
	movs r3, #56
	str r5, [sp, #0]
	bl sub_0801ea08
	b sub_08026b8c
	mov r0, r11
	ldr r3, [r0, #0]
	cmp r3, #0
	bge .L_0802678a
	adds r3, #7
.L_0802678a:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #5
	cmp r3, #29
	ble .L_08026796
	movs r0, #18
.L_08026796:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r3, #3
	movs r2, #12
	bl sub_080162d4
	movs r1, #56
	ldrsh r3, [r6, r1]
	mov r9, r0
	cmp r3, #0
	beq .L_080267b0
	b sub_08026a6a
.L_080267b0:
	ldr r0, [pc, #316]
	b .L_080267ec
	mov r2, r11
	ldr r3, [r2, #0]
	cmp r3, #0
	bge .L_080267be
	adds r3, #7
.L_080267be:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #5
	cmp r3, #29
	ble .L_080267ca
	movs r0, #18
.L_080267ca:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r3, #3
	movs r2, #12
	bl sub_080162d4
	ldr r4, [pc, #280]
	adds r3, r6, r4
	ldrb r3, [r3, #0]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r9, r0
	cmp r3, #0
	bne .L_080267ea
	b sub_08026a6a
.L_080267ea:
	ldr r0, [pc, #268]
.L_080267ec:
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl sub_0801e7c0
	b sub_08026b8c
	movs r0, #156
	lsls r0, r0, #1
	adds r7, r6, r0
	ldrb r3, [r7, #0]
	movs r5, #0
	cmp r3, #0
	beq .L_08026808
	movs r5, #1
.L_08026808:
	ldr r1, [pc, #240]
	adds r1, r1, r6
	ldrb r3, [r1, #0]
	mov r8, r1
	cmp r3, #0
	beq .L_08026816
	adds r5, #1
.L_08026816:
	movs r2, #158
	lsls r2, r2, #1
	adds r2, r2, r6
	ldrb r3, [r2, #0]
	mov r10, r2
	cmp r3, #0
	beq .L_08026826
	adds r5, #1
.L_08026826:
	ldr r3, [pc, #216]
	adds r3, r6, r3
	str r3, [sp, #40]
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_08026834
	adds r5, #1
.L_08026834:
	ldr r4, [pc, #204]
	adds r6, r6, r4
	ldrb r3, [r6, #0]
	cmp r3, #0
	beq .L_08026840
	adds r5, #1
.L_08026840:
	cmp r5, #0
	bne .L_08026846
	movs r5, #1
.L_08026846:
	movs r3, #9
	subs r1, r3, r5
	cmp r1, #3
	bgt .L_08026850
	movs r1, #4
.L_08026850:
	mov r0, r11
	ldr r3, [r0, #0]
	cmp r3, #0
	bge .L_0802685a
	adds r3, #7
.L_0802685a:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #9
	cmp r3, #29
	ble .L_08026866
	movs r0, #14
.L_08026866:
	movs r2, #6
	adds r3, r5, #2
	str r2, [sp, #0]
	movs r2, #16
	bl sub_080162d4
	ldrb r3, [r7, #0]
	mov r9, r0
	movs r5, #0
	cmp r3, #0
	beq .L_0802688a
	ldr r0, [pc, #136]
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl sub_0801e7c0
	movs r5, #1
.L_0802688a:
	mov r1, r8
	ldrb r3, [r1, #0]
	cmp r3, #0
	beq .L_080268a0
	lsls r3, r5, #3
	ldr r0, [pc, #116]
	mov r1, r9
	movs r2, #0
	bl sub_0801e7c0
	adds r5, #1
.L_080268a0:
	mov r2, r10
	ldrb r3, [r2, #0]
	cmp r3, #0
	beq .L_080268b6
	lsls r3, r5, #3
	ldr r0, [pc, #100]
	mov r1, r9
	movs r2, #0
	bl sub_0801e7c0
	adds r5, #1
.L_080268b6:
	ldr r4, [sp, #40]
	ldrb r3, [r4, #0]
	cmp r3, #0
	beq .L_080268cc
	lsls r3, r5, #3
	ldr r0, [pc, #80]
	mov r1, r9
	movs r2, #0
	bl sub_0801e7c0
	adds r5, #1
.L_080268cc:
	ldrb r3, [r6, #0]
	cmp r3, #0
	bne .L_080268d4
	b sub_08026a64
.L_080268d4:
	lsls r3, r5, #3
	ldr r0, [pc, #64]
	b sub_08026a5a
