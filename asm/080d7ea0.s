@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_08007300, 0x08007300
	.set sub_080b50e8, 0x080b50e8
	.set sub_080d6888, 0x080d6888
	.set sub_080d7e38, 0x080d7e38
	.set sub_080d8258, 0x080d8258
	.set sub_080d8260, 0x080d8260
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080f9010, 0x080f9010
	.global Region_080d7ea0
Region_080d7ea0:
	ands r0, r6
	negs r0, r0
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl sub_08004458
	movs r2, #1
	ands r0, r6
	movs r3, #0
	adds r0, #127
	add r8, r2
	str r3, [r5, #24]
	lsls r0, r0, #12
	mov r3, r8
	str r0, [r5, #16]
	adds r5, #28
	cmp r3, #64
	bne sub_080d7e38
	movs r4, #0
	ldr r5, [pc, #848]
	mov r8, r4
.L_080d7ecc:
	bl sub_08004458
	movs r3, #127
	ands r3, r0
	str r3, [r5, #0]
	bl sub_08004458
	mov r7, r8
	lsrs r3, r7, #31
	add r3, r8
	movs r2, #63
	ands r2, r0
	asrs r3, r3, #1
	adds r2, r2, r3
	negs r3, r7
	str r2, [r5, #4]
	movs r0, #1
	lsrs r2, r3, #31
	adds r3, r3, r2
	add r8, r0
	asrs r3, r3, #1
	mov r1, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #64
	bne .L_080d7ecc
	movs r2, #0
	ldr r3, [pc, #796]
	str r2, [sp, #40]
	mov r9, r3
	ldr r5, [sp, #40]
	movs r4, #0
	mov r10, r4
	cmp r5, #96
	bne .L_080d7f18
	movs r0, #134
	bl sub_080b50e8
.L_080d7f18:
	ldr r7, [sp, #40]
	cmp r7, #16
	bne .L_080d7f28
	ldr r0, [sp, #44]
	ldr r1, [pc, #768]
	movs r3, #32
	adds r2, r0, r1
	str r3, [r2, #0]
.L_080d7f28:
	ldr r2, [sp, #40]
	cmp r2, #16
	ble .L_080d7f42
	adds r3, r2, #0
	subs r3, #16
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r10, r3
	cmp r3, #16
	ble .L_080d7f42
	movs r3, #16
	mov r10, r3
.L_080d7f42:
	ldr r3, [sp, #40]
	subs r3, #9
	cmp r3, #62
	bhi .L_080d7f5a
	ldr r4, [sp, #40]
	movs r3, #3
	ands r3, r4
	cmp r3, #0
	bne .L_080d7f5a
	movs r0, #132
	bl sub_080f9010
.L_080d7f5a:
	ldr r5, [sp, #40]
	cmp r5, #72
	bne .L_080d7f66
	movs r0, #145
	bl sub_080f9010
.L_080d7f66:
	ldr r7, [sp, #40]
	cmp r7, #64
	ble .L_080d7f8e
	movs r3, #64
	subs r3, r3, r7
	lsls r2, r3, #3
	subs r2, r2, r3
	movs r3, #40
	str r3, [sp, #0]
	movs r3, #80
	str r3, [sp, #4]
	ldr r4, [pc, #680]
	ldr r3, [sp, #44]
	adds r2, #88
	adds r1, r3, r4
	ldr r0, [sp, #48]
	mov r3, r9
	ldr r5, [sp, #36]
	bl sub_080072f8
.L_080d7f8e:
	ldr r7, [sp, #40]
	cmp r7, #71
	bgt .L_080d7fd8
	movs r0, #0
	mov r1, r10
	mov r8, r0
	cmp r1, #0
	beq .L_080d7fd8
	ldr r7, [pc, #652]
	ldr r6, [pc, #652]
.L_080d7fa2:
	mov r0, r8
	movs r1, #3
	bl sub_080022fc
	ldr r4, [pc, #648]
	lsls r3, r0, #1
	ldrb r5, [r4, r0]
	ldrh r1, [r7, r3]
	ldr r2, [sp, #44]
	ldr r4, [pc, #640]
	ldrb r3, [r6, #1]
	ldrb r0, [r4, r0]
	adds r1, r2, r1
	ldrb r2, [r6, #0]
	subs r3, r3, r5
	str r5, [sp, #4]
	movs r5, #1
	str r0, [sp, #0]
	subs r2, #56
	ldr r0, [sp, #48]
	ldr r4, [sp, #36]
	add r8, r5
	bl sub_080072f4
	adds r6, #2
	cmp r8, r10
	bne .L_080d7fa2
.L_080d7fd8:
	ldr r7, [sp, #40]
	cmp r7, #72
	bne .L_080d803e
	ldr r1, [sp, #44]
	movs r2, #225
	movs r0, #0
	lsls r2, r2, #7
	ldr r6, [pc, #584]
	mov r8, r0
	adds r5, r1, r2
.L_080d7fec:
	mov r3, r8
	movs r2, #15
	ands r2, r3
	lsls r2, r2, #1
	ldrb r3, [r6, r2]
	subs r3, #56
	adds r2, #1
	lsls r3, r3, #16
	str r3, [r5, #0]
	ldrb r3, [r6, r2]
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl sub_08004458
	movs r3, #127
	ands r3, r0
	subs r3, #63
	lsls r3, r3, #13
	str r3, [r5, #12]
	bl sub_08004458
	movs r3, #31
	ands r3, r0
	negs r3, r3
	movs r4, #1
	subs r3, #16
	add r8, r4
	lsls r3, r3, #14
	mov r7, r8
	str r3, [r5, #16]
	adds r5, #28
	cmp r7, #64
	bne .L_080d7fec
	ldr r0, [sp, #40]
	cmp r0, #72
	bne .L_080d803e
	ldr r1, [sp, #44]
	ldr r3, [pc, #492]
	adds r2, r1, r3
	movs r3, #4
	str r3, [r2, #0]
.L_080d803e:
	ldr r4, [sp, #40]
	cmp r4, #71
	ble .L_080d80e0
	movs r5, #32
	mov r10, r5
	cmp r4, #72
	beq .L_080d8050
	movs r7, #64
	mov r10, r7
.L_080d8050:
	movs r0, #0
	mov r1, r10
	mov r8, r0
	cmp r1, #0
	beq .L_080d80da
	ldr r2, [sp, #44]
	movs r3, #225
	lsls r3, r3, #7
	adds r6, r2, r3
.L_080d8062:
	movs r4, #6
	ldrsh r7, [r6, r4]
	cmp r7, #135
	bgt .L_080d80d0
	movs r1, #3
	mov r0, r8
	bl sub_080022fc
	ldr r2, [pc, #456]
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r5, [sp, #44]
	movs r3, #2
	ldrsh r2, [r6, r3]
	ldr r4, [pc, #448]
	ldr r3, [pc, #448]
	adds r1, r5, r1
	ldrb r5, [r3, r0]
	ldrb r0, [r4, r0]
	subs r3, r7, r5
	str r0, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [sp, #36]
	bl sub_080072f4
	movs r2, #128
	adds r0, r6, #0
	movs r1, #64
	lsls r2, r2, #9
	bl sub_080e3908
	movs r5, #6
	ldrsh r3, [r6, r5]
	cmp r3, #120
	ble .L_080d80d0
	movs r7, #128
	ldr r3, [r6, #16]
	lsls r7, r7, #12
	cmp r3, r7
	ble .L_080d80d0
	negs r3, r3
	cmp r3, #0
	bge .L_080d80bc
	adds r3, #3
.L_080d80bc:
	asrs r3, r3, #2
	str r3, [r6, #16]
	movs r3, #240
	lsls r3, r3, #15
	str r3, [r6, #4]
	ldr r0, [sp, #44]
	ldr r1, [pc, #344]
	movs r3, #1
	adds r2, r0, r1
	str r3, [r2, #0]
.L_080d80d0:
	movs r2, #1
	add r8, r2
	adds r6, #28
	cmp r8, r10
	bne .L_080d8062
.L_080d80da:
	ldr r3, [sp, #40]
	cmp r3, #71
	bgt .L_080d8144
.L_080d80e0:
	ldr r7, [sp, #44]
	movs r0, #225
	movs r4, #0
	lsls r0, r0, #7
	mov r8, r4
	adds r5, r7, r0
.L_080d80ec:
	movs r1, #2
	ldrsh r2, [r5, r1]
	movs r4, #6
	ldrsh r3, [r5, r4]
	movs r1, #24
	str r1, [sp, #0]
	movs r1, #48
	subs r2, #12
	subs r3, #24
	str r1, [sp, #4]
	ldr r0, [sp, #48]
	ldr r1, [sp, #44]
	ldr r7, [sp, #36]
	bl sub_08007300
	ldr r3, [r5, #0]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #0]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	asrs r3, r3, #16
	cmp r3, #120
	ble .L_080d8138
	ldr r0, [sp, #40]
	cmp r0, #47
	bgt .L_080d8138
	bl sub_08004458
	movs r3, #127
	ands r3, r0
	adds r3, #64
	lsls r3, r3, #16
	str r3, [r5, #0]
	ldr r3, [pc, #272]
	str r3, [r5, #4]
.L_080d8138:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #64
	bne .L_080d80ec
.L_080d8144:
	ldr r2, [pc, #260]
	ldr r4, [sp, #44]
	movs r3, #0
	mov r8, r3
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080d8186
	movs r6, #36
	movs r5, #32
.L_080d8158:
	ldr r7, [sp, #40]
	cmp r7, r5
	bne .L_080d8172
	ldr r0, [sp, #44]
	ldr r3, [r0, r2]
	ldrsh r0, [r3, r6]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #9
	movs r2, #5
	subs r3, #1
	bl sub_080d6888
.L_080d8172:
	movs r2, #1
	add r8, r2
	ldr r4, [sp, #44]
	ldr r2, [pc, #208]
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	adds r6, #2
	adds r5, #8
	cmp r8, r3
	bne .L_080d8158
.L_080d8186:
	ldr r5, [sp, #40]
	cmp r5, #72
	ble .L_080d8208
	adds r6, r5, #0
	movs r7, #0
	ldr r5, [pc, #136]
	mov r8, r7
	subs r6, #54
.L_080d8196:
	ldr r3, [r5, #24]
	cmp r3, #17
	bhi .L_080d81ce
	lsrs r0, r3, #31
	adds r0, r3, r0
	movs r1, #9
	asrs r0, r0, #1
	bl sub_080022fc
	ldr r3, [pc, #164]
	ldrb r4, [r3, r0]
	ldr r3, [pc, #164]
	lsls r0, r0, #1
	ldrh r1, [r3, r0]
	ldr r0, [sp, #28]
	ldr r2, [r5, #0]
	ldr r3, [r5, #4]
	adds r1, r0, r1
	lsrs r0, r4, #1
	subs r3, r3, r0
	subs r2, r2, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [sp, #36]
	bl sub_080072f4
	ldr r3, [r5, #24]
.L_080d81ce:
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #18
	bne .L_080d81fc
	ldr r7, [sp, #40]
	cmp r7, #127
	bgt .L_080d81fc
	bl sub_08004458
	movs r3, #127
	ands r3, r0
	str r3, [r5, #0]
	bl sub_08004458
	lsrs r3, r6, #31
	movs r2, #63
	adds r3, r6, r3
	asrs r3, r3, #1
	ands r2, r0
	adds r2, r2, r3
	movs r3, #0
	str r2, [r5, #4]
	str r3, [r5, #24]
.L_080d81fc:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #64
	bne .L_080d8196
.L_080d8208:
	ldr r3, [sp, #40]
	subs r3, #72
	cmp r3, #7
	bhi sub_080d8258
	movs r0, #8
	movs r1, #8
	bl sub_080e155c
	b sub_080d8260
