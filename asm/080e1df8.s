@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_080072fc, 0x080072fc
	.set sub_080b5088, 0x080b5088
	.set sub_080b50e8, 0x080b50e8
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e21e8, 0x080e21e8
	.set sub_080e22e0, 0x080e22e0
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Region_080e1df8
Region_080e1df8:
.L_080e1df8:
	bl sub_08004458
	movs r3, #31
	ands r3, r0
	subs r3, #16
	str r3, [r5, #0]
	bl sub_08004458
	movs r3, #63
	ands r3, r0
	str r3, [r5, #4]
	bl sub_08004458
	movs r3, #15
	movs r4, #1
	ands r3, r0
	add r9, r4
	negs r3, r3
	mov r0, r9
	str r3, [r5, #24]
	adds r5, #28
	cmp r0, #16
	bne .L_080e1df8
	ldr r1, [sp, #128]
	movs r3, #239
	lsls r3, r3, #7
	ldr r4, [pc, #896]
	adds r2, r1, r3
	movs r3, #2
	str r3, [r2, #0]
	adds r2, r1, r4
	movs r3, #50
	movs r0, #145
	str r3, [r2, #0]
	bl sub_080f9010
	movs r1, #192
	ldr r0, [sp, #128]
	lsls r1, r1, #7
	movs r5, #0
	adds r1, r0, r1
	str r5, [sp, #116]
	str r1, [sp, #56]
	ldr r2, [sp, #116]
	cmp r2, #20
	bne .L_080e1e60
	ldr r0, [pc, #860]
	ldr r1, [sp, #128]
	movs r2, #1
	movs r3, #0
	bl sub_080e0524
.L_080e1e60:
	ldr r3, [sp, #116]
	subs r3, #80
	str r3, [sp, #64]
	cmp r3, #59
	bhi .L_080e1e7a
	ldr r4, [sp, #116]
	movs r3, #7
	ands r3, r4
	cmp r3, #0
	bne .L_080e1e7a
	movs r0, #134
	bl sub_080f9010
.L_080e1e7a:
	ldr r5, [sp, #116]
	cmp r5, #140
	bne .L_080e1e86
	movs r0, #134
	bl sub_080b50e8
.L_080e1e86:
	movs r0, #0
	movs r1, #8
	mov r8, r0
	mov r11, r1
.L_080e1e8e:
	ldr r4, [sp, #116]
	mov r3, r8
	lsls r2, r3, #7
	cmp r4, r11
	blt .L_080e1f78
	adds r3, r2, #0
	adds r3, #17
	cmp r4, r3
	bge .L_080e1f78
	adds r5, r2, #0
	subs r3, #8
	adds r5, #12
	cmp r4, r3
	blt .L_080e1ec4
	cmp r4, r5
	bge .L_080e1eca
	movs r3, #48
	movs r0, #112
	str r3, [sp, #0]
	str r0, [sp, #4]
	ldr r1, [sp, #128]
	ldr r0, [sp, #124]
	movs r2, #36
	movs r3, #0
	ldr r4, [sp, #108]
	bl sub_080072f4
.L_080e1ec4:
	ldr r0, [sp, #116]
	cmp r0, r5
	blt .L_080e1ef0
.L_080e1eca:
	mov r3, r11
	ldr r1, [sp, #116]
	adds r3, #8
	cmp r1, r3
	bge .L_080e1ef0
	movs r3, #48
	str r3, [sp, #0]
	movs r3, #112
	ldr r2, [sp, #128]
	str r3, [sp, #4]
	movs r3, #168
	lsls r3, r3, #5
	adds r1, r2, r3
	ldr r0, [sp, #124]
	movs r2, #36
	movs r3, #0
	ldr r4, [sp, #108]
	bl sub_080072f4
.L_080e1ef0:
	mov r3, r11
	ldr r5, [sp, #116]
	adds r3, #2
	cmp r5, r3
	bne .L_080e1f78
	movs r0, #0
	ldr r7, [pc, #696]
	mov r10, r0
	mov r9, r0
.L_080e1f02:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_080e1f62
	bl sub_08004458
	ldr r6, [pc, #684]
	ands r6, r0
	bl sub_08004458
	ldr r5, [pc, #680]
	ldr r1, [pc, #684]
	movs r2, #60
	lsls r3, r2, #16
	movs r4, #112
	ands r5, r0
	adds r5, r5, r1
	str r3, [r7, #0]
	lsls r3, r4, #16
	str r3, [r7, #4]
	adds r0, r5, #0
	bl sub_08002322
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r5, #0
	bl sub_0800231c
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl sub_08004458
	movs r3, #7
	ands r3, r0
	movs r5, #1
	movs r0, #128
	adds r3, #32
	add r10, r5
	lsls r0, r0, #2
	str r3, [r7, #24]
	cmp r10, r0
	beq .L_080e1f6e
.L_080e1f62:
	movs r1, #1
	ldr r2, [pc, #608]
	add r9, r1
	adds r7, #28
	cmp r9, r2
	bne .L_080e1f02
.L_080e1f6e:
	ldr r3, [sp, #128]
	ldr r4, [pc, #600]
	adds r2, r3, r4
	movs r3, #8
	str r3, [r2, #0]
.L_080e1f78:
	movs r0, #1
	add r8, r0
	movs r5, #128
	mov r1, r8
	add r11, r5
	cmp r1, #1
	bne .L_080e1e8e
	ldr r2, [sp, #116]
	cmp r2, #10
	bne .L_080e1fcc
	movs r3, #0
	mov r9, r3
	ldr r4, [sp, #128]
	ldr r3, [pc, #572]
	ldr r3, [r4, r3]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080e1fcc
	ldr r0, [pc, #560]
	movs r6, #36
	adds r5, r4, r0
.L_080e1fa2:
	ldr r3, [r5, #0]
	ldrsh r0, [r3, r6]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r9
	movs r1, #7
	bl sub_080d6888
	ldr r3, [r5, #0]
	movs r1, #4
	ldrsh r0, [r3, r6]
	bl sub_080b5088
	movs r3, #1
	add r9, r3
	ldr r3, [r5, #0]
	ldr r3, [r3, #20]
	adds r6, #2
	cmp r9, r3
	bne .L_080e1fa2
.L_080e1fcc:
	ldr r4, [sp, #64]
	cmp r4, #63
	bls .L_080e1fd4
	b sub_080e22e0
.L_080e1fd4:
	movs r5, #0
	ldr r6, [pc, #508]
	mov r9, r5
	movs r5, #80
.L_080e1fdc:
	ldr r0, [sp, #116]
	cmp r0, r5
	bne .L_080e1fee
	movs r1, #128
	ldr r0, [sp, #124]
	lsls r1, r1, #7
	ldr r2, [pc, #492]
	bl sub_080072fc
.L_080e1fee:
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #8
	cmp r2, #6
	bne .L_080e1fdc
	movs r3, #0
	movs r4, #80
	mov r9, r3
	mov r11, r4
.L_080e2002:
	ldr r0, [sp, #116]
	mov r5, r9
	lsls r3, r5, #1
	cmp r0, r11
	bge .L_080e200e
	b .L_080e2134
.L_080e200e:
	adds r3, #82
	cmp r0, r3
	bge .L_080e206c
	bl sub_08004458
	movs r1, #3
	adds r6, r0, #0
	ands r6, r1
	bl sub_08004458
	movs r2, #3
	adds r5, r0, #0
	movs r3, #2
	str r3, [sp, #0]
	ands r5, r2
	movs r3, #3
	movs r2, #7
	movs r1, #7
	movs r0, #47
	bl sub_080ed408
	movs r1, #3
	mov r0, r9
	bl sub_080022fc
	lsls r1, r0, #2
	adds r1, r1, r0
	movs r3, #72
	lsls r1, r1, #4
	ldr r4, [sp, #128]
	str r3, [sp, #0]
	str r3, [sp, #4]
	adds r1, r1, r0
	ldr r0, [pc, #392]
	subs r6, #3
	adds r5, #32
	lsls r1, r1, #6
	adds r1, r4, r1
	adds r2, r6, #0
	ldr r4, [r0, #0]
	adds r3, r5, #0
	ldr r0, [sp, #124]
	bl sub_080072f4
	movs r0, #47
	bl sub_08002dd8
.L_080e206c:
	ldr r1, [sp, #116]
	cmp r1, r11
	bne .L_080e2134
	movs r2, #0
	ldr r7, [pc, #320]
	mov r10, r2
	mov r8, r2
.L_080e207a:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_080e20d6
	bl sub_08004458
	ldr r6, [pc, #308]
	ands r6, r0
	bl sub_08004458
	ldr r3, [pc, #336]
	adds r5, r0, #0
	ands r5, r3
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r7, #0]
	movs r3, #160
	lsls r3, r3, #15
	str r3, [r7, #4]
	adds r0, r5, #0
	bl sub_08002322
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r5, #0
	bl sub_0800231c
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl sub_08004458
	movs r3, #7
	ands r3, r0
	adds r3, #32
	str r3, [r7, #24]
	movs r3, #1
	add r10, r3
	mov r4, r10
	cmp r4, #16
	beq .L_080e20e2
.L_080e20d6:
	movs r5, #1
	ldr r0, [pc, #236]
	add r8, r5
	adds r7, #28
	cmp r8, r0
	bne .L_080e207a
.L_080e20e2:
	ldr r1, [sp, #128]
	ldr r3, [pc, #228]
	ldr r5, [pc, #232]
	adds r2, r1, r3
	movs r3, #8
	str r3, [r2, #0]
	adds r2, r1, r5
	ldr r3, [r2, #0]
	ldr r3, [r3, #20]
	movs r4, #0
	mov r8, r4
	cmp r3, #0
	beq .L_080e2134
	ldr r7, [pc, #228]
	adds r5, r2, #0
	movs r6, #36
.L_080e2102:
	bl sub_08004458
	ldr r3, [r5, #0]
	movs r1, #3
	ands r0, r1
	ldrb r1, [r7, r0]
	ldrsh r0, [r3, r6]
	movs r3, #4
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r8
	bl sub_080d6888
	ldr r3, [r5, #0]
	movs r1, #4
	ldrsh r0, [r3, r6]
	bl sub_080b5088
	ldr r3, [r5, #0]
	movs r0, #1
	ldr r3, [r3, #20]
	add r8, r0
	adds r6, #2
	cmp r8, r3
	bne .L_080e2102
.L_080e2134:
	movs r2, #1
	add r9, r2
	movs r1, #2
	mov r3, r9
	add r11, r1
	cmp r3, #30
	beq .L_080e2144
	b .L_080e2002
.L_080e2144:
	bl sub_08004458
	movs r6, #7
	ands r6, r0
	bl sub_08004458
	ldr r3, [pc, #140]
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl sub_08002322
	adds r6, #8
	adds r4, r6, #0
	muls r4, r0
	asrs r4, r4, #16
	mov r8, r4
	adds r0, r5, #0
	adds r4, #72
	str r4, [sp, #60]
	bl sub_0800231c
	adds r3, r6, #0
	muls r3, r0
	movs r2, #32
	asrs r3, r3, #16
	subs r2, r2, r3
	mov r11, r2
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	str r5, [sp, #0]
	bl sub_080ed408
	movs r1, #24
	movs r5, #24
	movs r6, #12
	negs r1, r1
	str r5, [sp, #4]
	str r6, [sp, #0]
	ldr r2, [pc, #64]
	movs r0, #60
	add r1, r11
	add r8, r0
	mov r10, r1
	ldr r0, [sp, #124]
	ldr r1, [sp, #56]
	ldr r4, [r2, #0]
	mov r3, r10
	mov r2, r8
	b sub_080e21e8
