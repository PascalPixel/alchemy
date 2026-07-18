@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080ec474, 0x080ec474
	.set sub_080ec596, 0x080ec596
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Region_080ec264
Region_080ec264:
	ldrh r3, [r3, #4]
	str r3, [sp, #40]
	ldr r2, [pc, #124]
	ldr r7, [r6, #16]
	movs r5, #2
	mov r0, r10
	add r2, r11
	movs r3, #75
	str r7, [sp, #36]
	str r5, [r0, #0]
	str r3, [r2, #0]
	ldr r1, [sp, #60]
	movs r3, #3
	str r1, [r7, #16]
	movs r2, #7
	movs r1, #7
	movs r0, #46
	str r3, [sp, #0]
	bl sub_080ed408
	ldr r3, [r6, #24]
	movs r1, #7
	str r3, [sp, #80]
	movs r2, #7
	movs r3, #3
	movs r0, #47
	str r5, [sp, #0]
	bl sub_080ed408
	ldr r3, [r6, #28]
	mov r2, sp
	adds r2, #80
	str r2, [sp, #24]
	str r3, [r2, #4]
	ldr r2, [pc, #64]
	ldr r3, [pc, #56]
	strh r3, [r2, #0]
	movs r3, #0
	mov r8, r3
	movs r1, #1
	movs r2, #128
	ldr r3, [pc, #56]
	negs r1, r1
	lsls r2, r2, #3
.L_080ec2bc:
	movs r4, #1
	add r8, r4
	str r1, [r3, #0]
	adds r3, #28
	cmp r8, r2
	bne .L_080ec2bc
	movs r7, #225
	movs r6, #0
	lsls r7, r7, #7
	mov r8, r6
	add r7, r11
.L_080ec2d2:
	bl sub_08004458
	ldr r6, [pc, #28]
	ands r6, r0
	movs r0, #128
	lsls r0, r0, #8
	adds r6, r6, r0
	b .L_080ec2f8
	.2byte 0x0000
	.4byte 0x00000784
	.4byte 0x00007784
	.4byte 0x0400000c
	.4byte 0x02010018
	.4byte 0x00003fff
.L_080ec2f8:
	bl sub_08004458
	movs r5, #127
	ands r5, r0
	adds r0, r6, #0
	bl sub_08002322
	adds r5, #255
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #2
	str r3, [r7, #0]
	adds r0, r6, #0
	bl sub_0800231c
	adds r3, r5, #0
	muls r3, r0
	asrs r2, r3, #2
	ldr r3, [r7, #0]
	negs r3, r3
	str r2, [r7, #4]
	cmp r3, #0
	bge .L_080ec328
	adds r3, #31
.L_080ec328:
	asrs r3, r3, #5
	str r3, [r7, #12]
	negs r3, r2
	cmp r3, #0
	bge .L_080ec334
	adds r3, #31
.L_080ec334:
	movs r1, #1
	asrs r3, r3, #5
	add r8, r1
	str r3, [r7, #16]
	mov r2, r8
	movs r3, #0
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #32
	bne .L_080ec2d2
	ldr r7, [pc, #260]
	mov r8, r3
.L_080ec34c:
	bl sub_08004458
	movs r3, #255
	adds r6, r0, #0
	ands r6, r3
	bl sub_08004458
	ldr r3, [pc, #248]
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl sub_08002322
	adds r3, r6, #0
	muls r3, r0
	movs r4, #128
	lsls r4, r4, #16
	asrs r3, r3, #3
	adds r3, r3, r4
	str r3, [r7, #0]
	adds r0, r5, #0
	bl sub_0800231c
	adds r3, r6, #0
	muls r3, r0
	movs r6, #192
	asrs r3, r3, #4
	lsls r6, r6, #15
	adds r3, r3, r6
	str r3, [r7, #4]
	bl sub_08004458
	ldr r3, [pc, #200]
	ands r3, r0
	ldr r0, [pc, #200]
	negs r3, r3
	adds r3, r3, r0
	lsls r3, r3, #5
	str r3, [r7, #12]
	bl sub_08004458
	ldr r3, [pc, #192]
	ldr r1, [pc, #192]
	ands r3, r0
	negs r3, r3
	adds r3, r3, r1
	lsls r3, r3, #5
	str r3, [r7, #16]
	movs r3, #0
	str r3, [r7, #24]
	movs r2, #1
	movs r3, #178
	add r8, r2
	lsls r3, r3, #2
	adds r7, #28
	cmp r8, r3
	bne .L_080ec34c
	movs r4, #0
	mov r10, r4
	mov r6, r10
	cmp r6, #24
	bne .L_080ec3ce
	movs r0, #162
	bl sub_080f9010
.L_080ec3ce:
	mov r7, r10
	cmp r7, #76
	bne .L_080ec3da
	movs r0, #164
	bl sub_080f9010
.L_080ec3da:
	mov r0, r10
	cmp r0, #154
	bne .L_080ec3e6
	movs r0, #142
	bl sub_080f9010
.L_080ec3e6:
	mov r1, r10
	cmp r1, #222
	bne .L_080ec3f2
	movs r0, #145
	bl sub_080f9010
.L_080ec3f2:
	mov r2, r10
	cmp r2, #15
	bhi .L_080ec43e
	ldr r7, [pc, #84]
	cmp r2, #1
	bne .L_080ec426
	ldr r5, [pc, #104]
	adds r6, r5, #0
	movs r3, #63
	adds r6, #128
.L_080ec406:
	str r3, [sp, #12]
	bl sub_08004458
	ldr r3, [sp, #12]
	ands r0, r3
	strb r0, [r5, #0]
	adds r5, #1
	cmp r5, r6
	bne .L_080ec406
	ldr r3, [pc, #48]
	movs r1, #144
	strh r3, [r7, #0]
	ldr r0, [pc, #76]
	lsls r1, r1, #3
	bl sub_080041d8
.L_080ec426:
	ldr r4, [sp, #60]
	ldrh r3, [r7, #0]
	mov r6, r10
	adds r3, r3, r4
	adds r4, #3
	strh r3, [r7, #0]
	str r4, [sp, #60]
	cmp r6, #15
	bne .L_080ec43e
	ldr r0, [pc, #48]
	bl sub_08004278
.L_080ec43e:
	mov r7, r10
	cmp r7, #21
	bgt .L_080ec446
	b sub_080ec596
.L_080ec446:
	ldr r3, [pc, #40]
	b sub_080ec474
