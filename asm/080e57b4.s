@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_080cef64, 0x080cef64
	.set sub_080e5afc, 0x080e5afc
	.set sub_080e640e, 0x080e640e
	.set sub_080ed408, 0x080ed408
	.global Region_080e57b4
Region_080e57b4:
	ldr r1, [sp, #96]
	cmp r1, #21
	bne .L_080e57be
	bl sub_080e640e
.L_080e57be:
	cmp r1, #6
	beq .L_080e57c8
	cmp r1, #27
	beq .L_080e57c8
	b .L_080e58f2
.L_080e57c8:
	ldr r3, [sp, #84]
	subs r3, #6
	cmp r3, #13
	bhi .L_080e5818
	movs r2, #0
	ldr r5, [sp, #84]
	mov r10, r2
.L_080e57d6:
	lsrs r3, r5, #31
	adds r3, r5, r3
	movs r2, #3
	asrs r3, r3, #1
	ands r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r0, [sp, #60]
	lsls r1, r2, #4
	subs r1, r1, r2
	ldr r3, [pc, #736]
	ldr r2, [r0, #0]
	lsls r1, r1, #6
	adds r1, r1, r3
	lsrs r3, r2, #31
	adds r2, r2, r3
	movs r3, #24
	str r3, [sp, #0]
	asrs r2, r2, #1
	movs r3, #104
	subs r2, #8
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r3, #0
	bl sub_080072f4
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r5, #3
	cmp r2, #2
	bne .L_080e57d6
.L_080e5818:
	ldr r3, [sp, #84]
	subs r3, #8
	cmp r3, #15
	bls .L_080e5824
	bl sub_080e640e
.L_080e5824:
	ldr r5, [pc, #680]
	ldr r0, [pc, #684]
	movs r3, #0
	mov r10, r3
	movs r7, #3
	mov r11, r5
	mov r9, r0
.L_080e5832:
	mov r8, r10
	mov r1, r8
	ands r1, r7
	mov r8, r1
	bl sub_08004458
	ldr r3, [pc, #664]
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl sub_08002322
	ldr r2, [sp, #60]
	ldr r3, [r2, #0]
	adds r6, r0, #0
	lsrs r2, r3, #31
	adds r3, r3, r2
	lsls r6, r6, #3
	mov r1, r8
	asrs r3, r3, #1
	mov r0, r11
	asrs r6, r6, #16
	adds r6, r6, r3
	ldrb r3, [r0, r1]
	adds r0, r5, #0
	lsrs r3, r3, #1
	subs r6, r6, r3
	bl sub_0800231c
	mov r2, r9
	adds r5, r0, #0
	mov r0, r8
	ldrb r3, [r2, r0]
	lsls r5, r5, #5
	lsrs r3, r3, #1
	movs r0, #47
	asrs r5, r5, #16
	subs r5, r5, r3
	bl sub_08002dd8
	movs r0, #46
	bl sub_08002dd8
	bl sub_08004458
	ldr r3, [pc, #588]
	ands r0, r7
	ldrb r2, [r3, r0]
	adds r3, r7, #0
	orrs r3, r2
	movs r2, #2
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #47
	bl sub_080ed408
	ldr r2, [pc, #568]
	mov r1, r8
	lsls r3, r1, #1
	ldrh r1, [r2, r3]
	ldr r2, [pc, #564]
	mov r0, r11
	adds r1, r1, r2
	mov r2, r8
	ldrb r3, [r0, r2]
	str r3, [sp, #0]
	mov r0, r9
	ldrb r3, [r0, r2]
	ldr r2, [pc, #552]
	str r3, [sp, #4]
	adds r5, #56
	ldr r4, [r2, #0]
	adds r3, r5, #0
	adds r2, r6, #0
	ldr r0, [sp, #88]
	bl sub_080072f4
	movs r0, #47
	bl sub_08002dd8
	ldr r0, [pc, #532]
	ldr r5, [sp, #92]
	adds r3, r5, r0
	ldr r3, [r3, #0]
	ldr r1, [sp, #52]
	ldr r0, [r3, #4]
	bl sub_080cef64
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #3
	bne .L_080e5832
	bl sub_080e640e
.L_080e58f2:
	ldr r3, [sp, #96]
	cmp r3, #14
	beq .L_080e58fa
	b .L_080e5a2e
.L_080e58fa:
	movs r0, #47
	bl sub_08002dd8
	movs r0, #46
	bl sub_08002dd8
	ldr r5, [sp, #84]
	cmp r5, #23
	bls .L_080e590e
	b .L_080e5ab6
.L_080e590e:
	ldr r0, [sp, #60]
	ldr r3, [r0, #0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #12]
	lsls r3, r5, #5
	subs r3, #232
	mov r11, r3
	lsls r3, r5, #4
	adds r7, r3, #0
	mov r1, r11
	subs r7, #48
	cmp r1, #0
	ble .L_080e5930
	movs r2, #0
	mov r11, r2
.L_080e5930:
	cmp r7, #104
	ble .L_080e593a
.L_080e5934:
	subs r7, #104
	cmp r7, #104
	bgt .L_080e5934
.L_080e593a:
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	bl sub_080ed408
	ldr r3, [pc, #420]
	ldr r0, [sp, #12]
	mov r8, r3
	movs r5, #188
	mov r1, r11
	add r8, r5
	adds r1, r1, r7
	movs r6, #17
	movs r5, #104
	str r5, [sp, #4]
	str r6, [sp, #0]
	mov r9, r1
	subs r0, #8
	mov r10, r0
	mov r2, r8
	mov r3, r9
	ldr r0, [sp, #88]
	ldr r4, [r2, #0]
	subs r3, #104
	ldr r1, [pc, #368]
	mov r2, r10
	bl sub_080072f4
	subs r5, r5, r7
	str r5, [sp, #4]
	str r6, [sp, #0]
	mov r3, r8
	ldr r4, [r3, #0]
	ldr r0, [sp, #88]
	ldr r1, [pc, #348]
	mov r2, r10
	mov r3, r9
	bl sub_080072f4
	movs r0, #34
	str r0, [sp, #0]
	movs r0, #65
	str r0, [sp, #4]
	ldr r2, [sp, #12]
	mov r3, r11
	mov r5, r8
	ldr r1, [pc, #340]
	subs r2, #17
	adds r3, #47
	ldr r4, [r5, #0]
	ldr r0, [sp, #88]
	bl sub_080072f4
	movs r0, #47
	bl sub_08002dd8
	ldr r0, [sp, #84]
	cmp r0, #8
	bne .L_080e59be
	ldr r1, [sp, #92]
	ldr r2, [pc, #316]
	adds r3, r1, r2
	str r0, [r3, #0]
.L_080e59be:
	ldr r3, [sp, #84]
	cmp r3, #1
	ble .L_080e5ab6
	ldr r0, [sp, #92]
	movs r1, #225
	movs r5, #0
	lsls r1, r1, #7
	mov r10, r5
	movs r7, #0
	movs r6, #255
	adds r5, r0, r1
.L_080e59d4:
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_080e5a20
	ldr r2, [sp, #28]
	ldr r3, [r2, #8]
	str r3, [r5, #0]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r5, #4]
	ldr r3, [r2, #16]
	str r3, [r5, #8]
	bl sub_08004458
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl sub_08004458
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl sub_08004458
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #20]
	mov r0, r10
	lsrs r3, r0, #31
	add r3, r10
	asrs r3, r3, #1
	adds r3, #32
	adds r7, #1
	str r3, [r5, #24]
	cmp r7, #4
	beq .L_080e5ab6
.L_080e5a20:
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r5, #28
	cmp r2, #64
	bne .L_080e59d4
	b .L_080e5ab6
.L_080e5a2e:
	ldr r1, [sp, #96]
	cmp r1, #31
	bne sub_080e5afc
	movs r0, #47
	bl sub_08002dd8
	movs r0, #46
	bl sub_08002dd8
	ldr r3, [sp, #84]
	subs r3, #4
	cmp r3, #19
	bhi .L_080e5ab6
	ldr r2, [sp, #60]
	ldr r5, [r2, #0]
	lsrs r3, r5, #31
	movs r0, #2
	adds r5, r5, r3
	movs r3, #48
	str r0, [sp, #0]
	mov r9, r3
	mov r10, r0
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	bl sub_080ed408
	mov r3, r9
	movs r1, #24
	ldr r6, [pc, #132]
	str r1, [sp, #0]
	str r3, [sp, #4]
	asrs r5, r5, #1
	adds r6, #188
	adds r2, r5, #0
	ldr r4, [r6, #0]
	ldr r0, [sp, #88]
	subs r2, #24
	mov r8, r1
	ldr r1, [pc, #100]
	bl sub_080072f4
	movs r0, #47
	bl sub_08002dd8
	mov r0, r10
	str r0, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #7
	movs r0, #47
	bl sub_080ed408
	mov r1, r8
	mov r2, r9
	str r1, [sp, #0]
	str r2, [sp, #4]
	ldr r0, [sp, #88]
	ldr r4, [r6, #0]
	ldr r1, [pc, #60]
	adds r2, r5, #0
	movs r3, #48
	bl sub_080072f4
	movs r0, #47
	bl sub_08002dd8
.L_080e5ab6:
	ldr r0, [pc, #52]
	ldr r5, [sp, #92]
	adds r3, r5, r0
	ldr r3, [r3, #0]
	ldr r1, [sp, #52]
	ldr r0, [r3, #4]
	bl sub_080cef64
	bl sub_080e640e
	lsls r0, r0, #0
