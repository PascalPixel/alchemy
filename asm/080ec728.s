@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_0800307c, 0x0800307c
	.set sub_080030f8, 0x080030f8
	.set sub_08004278, 0x08004278
	.set sub_080072f4, 0x080072f4
	.set sub_08007308, 0x08007308
	.set sub_080cde90, 0x080cde90
	.set sub_080d67dc, 0x080d67dc
	.set sub_080e3908, 0x080e3908
	.set sub_080ec3c2, 0x080ec3c2
	.set sub_080ec6c8, 0x080ec6c8
	.set sub_080ecae8, 0x080ecae8
	.global Region_080ec728
Region_080ec728:
	adds r3, #4
	str r3, [r5, #4]
	ldr r1, [r5, #0]
	ldr r2, [r5, #4]
	movs r3, #6
	movs r4, #1
	negs r3, r3
	add r8, r4
	adds r1, #100
	adds r2, #52
	str r3, [r5, #12]
	mov r7, r8
	movs r3, #0
	str r1, [r5, #0]
	str r2, [r5, #4]
	str r1, [r5, #8]
	str r2, [r5, #16]
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #64
	bne sub_080ec6c8
	mov r0, r10
	cmp r0, #221
	ble .L_080ec7b6
	movs r5, #225
	movs r1, #0
	lsls r5, r5, #7
	ldr r7, [pc, #860]
	mov r8, r1
	movs r6, #2
	add r5, r11
.L_080ec766:
	mov r2, r8
	lsrs r3, r2, #31
	add r3, r8
	asrs r3, r3, #1
	adds r3, #222
	cmp r10, r3
	blt .L_080ec7aa
	ldrh r1, [r7, #0]
	ldr r3, [pc, #840]
	add r1, r11
	ldr r2, [r5, #0]
	adds r1, r1, r3
	ldr r3, [r5, #4]
	subs r2, #1
	subs r3, #1
	str r6, [sp, #0]
	str r6, [sp, #4]
	ldr r4, [sp, #80]
	ldr r0, [sp, #68]
	bl sub_080072f4
	ldr r2, [r5, #0]
	ldr r3, [r5, #12]
	adds r2, r2, r3
	str r2, [r5, #0]
	ldr r3, [r5, #4]
	ldr r2, [r5, #16]
	adds r3, r3, r2
	str r3, [r5, #4]
	cmp r3, #0
	bge .L_080ec7aa
	ldr r3, [r5, #8]
	str r2, [r5, #4]
	str r3, [r5, #0]
.L_080ec7aa:
	movs r4, #1
	add r8, r4
	mov r0, r8
	adds r5, #28
	cmp r0, #64
	bne .L_080ec766
.L_080ec7b6:
	mov r1, r10
	cmp r1, #175
	ble .L_080ec82a
	movs r6, #5
	cmp r1, #221
	ble .L_080ec7ce
	mov r3, r10
	subs r3, #223
	movs r6, #60
	cmp r3, #1
	bls .L_080ec7ce
	movs r6, #40
.L_080ec7ce:
	cmp r6, #60
	ble .L_080ec7d4
	movs r6, #60
.L_080ec7d4:
	movs r2, #0
	mov r8, r2
	cmp r6, #0
	beq .L_080ec82a
	lsls r7, r6, #1
.L_080ec7de:
	mov r5, r8
	adds r5, #1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r1, r3, #1
	lsls r3, r1, #2
	subs r4, r7, r3
	cmp r4, #11
	bgt .L_080ec7f2
	movs r4, #12
.L_080ec7f2:
	cmp r4, #63
	ble .L_080ec7f8
	movs r4, #63
.L_080ec7f8:
	movs r3, #1
	mov r0, r8
	ands r3, r0
	cmp r3, #0
	beq .L_080ec804
	negs r1, r1
.L_080ec804:
	lsrs r2, r1, #31
	adds r0, r1, #0
	adds r2, r1, r2
	adds r1, #1
	lsrs r3, r1, #31
	adds r1, r1, r3
	asrs r1, r1, #1
	asrs r2, r2, #1
	movs r3, #63
	subs r3, r3, r1
	adds r0, #48
	adds r2, #111
	movs r1, #0
	mov r8, r5
	str r4, [sp, #0]
	bl sub_080cde90
	cmp r8, r6
	bne .L_080ec7de
.L_080ec82a:
	mov r1, r9
	cmp r1, #39
	bhi .L_080ec862
	lsrs r3, r1, #31
	add r3, r9
	asrs r3, r3, #1
	adds r4, r3, #1
	cmp r4, #4
	ble .L_080ec83e
	movs r4, #4
.L_080ec83e:
	lsls r0, r4, #1
	ldr r2, [pc, #632]
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [pc, #632]
	str r0, [sp, #0]
	str r0, [sp, #4]
	add r1, r11
	ldr r6, [sp, #24]
	adds r1, r1, r2
	movs r3, #60
	movs r2, #108
	subs r2, r2, r4
	subs r3, r3, r4
	ldr r0, [sp, #68]
	ldr r4, [r6, #4]
	bl sub_080072f4
.L_080ec862:
	mov r2, r10
	subs r2, #192
	cmp r2, #7
	bhi .L_080ec89c
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	adds r4, r3, #3
	cmp r4, #6
	ble .L_080ec878
	movs r4, #6
.L_080ec878:
	lsls r0, r4, #1
	ldr r2, [pc, #576]
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r7, [pc, #572]
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r0, [sp, #24]
	add r1, r11
	movs r2, #108
	movs r3, #60
	subs r2, r2, r4
	subs r3, r3, r4
	adds r1, r1, r7
	ldr r4, [r0, #4]
	ldr r0, [sp, #68]
	bl sub_080072f4
.L_080ec89c:
	mov r1, r10
	cmp r1, #199
	ble .L_080ec90e
	mov r5, r10
	subs r5, #200
	adds r3, r5, #0
	cmp r5, #0
	bge .L_080ec8b0
	mov r3, r10
	subs r3, #197
.L_080ec8b0:
	asrs r6, r3, #2
	adds r4, r6, #5
	cmp r4, #8
	ble .L_080ec8ba
	movs r4, #8
.L_080ec8ba:
	lsls r0, r4, #1
	ldr r7, [pc, #508]
	subs r3, r0, #2
	ldrh r1, [r7, r3]
	ldr r2, [pc, #508]
	str r0, [sp, #0]
	str r0, [sp, #4]
	add r1, r11
	ldr r0, [sp, #24]
	adds r1, r1, r2
	movs r3, #60
	movs r2, #108
	subs r2, r2, r4
	subs r3, r3, r4
	ldr r4, [r0, #4]
	ldr r0, [sp, #68]
	bl sub_080072f4
	mov r1, r10
	cmp r1, #213
	bgt .L_080ec90e
	adds r4, r6, #1
	cmp r4, #4
	ble .L_080ec8ec
	movs r4, #4
.L_080ec8ec:
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r7, r3]
	ldr r2, [pc, #460]
	str r0, [sp, #0]
	str r0, [sp, #4]
	add r1, r11
	ldr r6, [sp, #24]
	adds r1, r1, r2
	movs r3, #52
	movs r2, #100
	subs r2, r2, r4
	subs r3, r3, r4
	ldr r0, [sp, #68]
	ldr r4, [r6, #4]
	bl sub_080072f4
.L_080ec90e:
	mov r3, r10
	subs r3, #214
	cmp r3, #7
	bhi .L_080ec94a
	adds r1, r3, #0
	cmp r1, #0
	bge .L_080ec920
	mov r1, r10
	subs r1, #211
.L_080ec920:
	asrs r3, r1, #2
	cmp r3, #2
	ble .L_080ec928
	movs r3, #2
.L_080ec928:
	lsls r1, r3, #3
	adds r1, r1, r3
	movs r3, #48
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r0, [sp, #24]
	lsls r1, r1, #8
	movs r7, #128
	add r1, r11
	lsls r7, r7, #3
	ldr r4, [r0, #4]
	adds r1, r1, r7
	ldr r0, [sp, #68]
	movs r2, #63
	movs r3, #18
	bl sub_080072f4
.L_080ec94a:
	mov r1, r10
	cmp r1, #221
	ble .L_080ec9a4
	mov r3, r10
	subs r3, #222
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r5, r3, #0
	cmp r3, #0
	bge .L_080ec962
	adds r5, r3, #3
.L_080ec962:
	asrs r5, r5, #2
	lsls r5, r5, #2
	subs r5, r3, r5
	adds r5, #3
	lsls r1, r5, #3
	adds r1, r1, r5
	lsls r1, r1, #8
	movs r2, #128
	lsls r2, r2, #3
	movs r6, #48
	add r1, r11
	adds r1, r1, r2
	str r6, [sp, #0]
	str r6, [sp, #4]
	ldr r4, [sp, #80]
	ldr r0, [sp, #68]
	movs r2, #72
	movs r3, #30
	bl sub_080072f4
	cmp r5, #5
	bne .L_080ec9a4
	movs r1, #176
	lsls r1, r1, #5
	str r6, [sp, #0]
	str r6, [sp, #4]
	ldr r4, [sp, #80]
	ldr r0, [sp, #68]
	add r1, r11
	movs r2, #66
	movs r3, #22
	bl sub_080072f4
.L_080ec9a4:
	mov r3, r10
	cmp r3, #27
	ble .L_080eca2a
	ldr r4, [sp, #24]
	ldr r4, [r4, #4]
	movs r6, #0
	ldr r5, [pc, #276]
	mov r9, r4
	mov r8, r6
	movs r7, #3
.L_080ec9b8:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_080eca1c
	movs r1, #3
	mov r0, r8
	bl sub_080022fc
	adds r0, #2
	lsls r4, r0, #1
	ldr r2, [pc, #256]
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #64]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #68]
	bl sub_08007308
	mov r3, r8
	ldr r2, [pc, #220]
	ands r3, r7
	lsls r3, r3, #2
	ldr r2, [r2, r3]
	adds r0, r5, #0
	movs r1, #62
	bl sub_080e3908
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	ldr r3, [r5, #16]
	cmp r3, #0
	ble .L_080eca1c
	movs r0, #6
	ldrsh r3, [r5, r0]
	cmp r3, #104
	ble .L_080eca1c
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_080eca1c:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #3
	adds r5, #28
	cmp r8, r2
	bne .L_080ec9b8
.L_080eca2a:
	movs r3, #0
	mov r8, r3
	movs r5, #32
.L_080eca30:
	mov r4, r8
	mov r2, r10
	lsls r1, r4, #5
	cmp r2, #0
	bge .L_080eca3c
	adds r2, #3
.L_080eca3c:
	movs r3, #31
	asrs r2, r2, #2
	ands r2, r3
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r6, [sp, #24]
	adds r2, r1, r2
	movs r7, #1
	subs r2, #32
	ldr r4, [r6, #4]
	ldr r0, [sp, #68]
	mov r1, r11
	movs r3, #88
	add r8, r7
	bl sub_080072f4
	mov r0, r8
	cmp r0, #5
	bne .L_080eca30
	ldr r2, [pc, #112]
	movs r3, #1
	add r2, r11
	str r3, [r2, #0]
	movs r0, #1
	bl sub_080030f8
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #244
	beq .L_080eca8e
	cmp r2, #16
	bgt .L_080eca80
	b sub_080ec3c2
.L_080eca80:
	ldr r3, [pc, #84]
	ldr r3, [r3, #0]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	bne .L_080eca8e
	b sub_080ec3c2
.L_080eca8e:
	add r4, sp, #40
	ldr r3, [pc, #72]
	ldrh r4, [r4, #0]
	strh r4, [r3, #4]
	ldr r6, [sp, #36]
	movs r2, #0
	str r2, [r6, #16]
	movs r1, #0
	movs r0, #2
	bl sub_0800307c
	bl sub_080d67dc
	ldr r0, [pc, #52]
	bl sub_08004278
	ldr r2, [pc, #52]
	ldr r3, [pc, #16]
	strh r3, [r2, #0]
	movs r2, #239
	lsls r2, r2, #7
	b sub_080ecae8
