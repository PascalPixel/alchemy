@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_08004278, 0x08004278
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_080d6750, 0x080d6750
	.set sub_080d67dc, 0x080d67dc
	.set sub_080dbb24, 0x080dbb24
	.set sub_080e0524, 0x080e0524
	.set sub_080e1848, 0x080e1848
	.set sub_080e1df8, 0x080e1df8
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Region_080e1acc
Region_080e1acc:
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #3
	bl sub_080ed408
	ldr r4, [pc, #720]
	ldr r4, [r4, #0]
	str r4, [sp, #88]
	ldr r0, [sp, #100]
	mov r10, r0
	cmp r0, #128
	ble .L_080e1aee
	movs r1, #128
	mov r10, r1
.L_080e1aee:
	movs r2, #0
	mov r3, r10
	mov r9, r2
	cmp r3, #0
	beq .L_080e1b88
	lsls r3, r5, #1
	adds r3, r3, r5
	ldr r7, [sp, #104]
	lsls r3, r3, #7
	movs r4, #146
	adds r3, r3, r5
	adds r7, #1
	lsls r4, r4, #1
	lsls r3, r3, #1
	add r4, sp
	lsls r0, r7, #1
	str r3, [sp, #84]
	mov r8, r4
	add r6, sp, #160
	mov r11, r0
.L_080e1b16:
	mov r1, r9
	lsls r5, r1, #9
	adds r0, r5, #0
	bl sub_08002322
	lsls r3, r0, #1
	adds r3, r3, r0
	mov r2, r8
	lsls r3, r3, #5
	str r3, [r2, #0]
	adds r0, r5, #0
	bl sub_0800231c
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #5
	negs r3, r3
	mov r4, r8
	str r3, [r4, #8]
	adds r1, r6, #0
	mov r0, r8
	bl sub_080e3944
	ldr r2, [r6, #0]
	movs r5, #6
	ldrsh r3, [r6, r5]
	asrs r2, r2, #17
	ldr r0, [pc, #612]
	adds r2, #64
	adds r3, #60
	mov r1, r11
	str r2, [r6, #0]
	str r3, [r6, #4]
	subs r1, #2
	ldrh r1, [r0, r1]
	ldr r0, [sp, #84]
	ldr r4, [sp, #128]
	adds r1, r0, r1
	lsrs r0, r7, #31
	ldr r5, [pc, #592]
	adds r0, r7, r0
	adds r1, r4, r1
	asrs r0, r0, #1
	adds r1, r1, r5
	subs r2, r2, r0
	movs r5, #1
	mov r0, r11
	str r0, [sp, #4]
	subs r3, r3, r7
	str r7, [sp, #0]
	ldr r0, [sp, #124]
	ldr r4, [sp, #88]
	add r9, r5
	bl sub_080072f4
	cmp r9, r10
	bne .L_080e1b16
.L_080e1b88:
	movs r0, #47
	bl sub_08002dd8
	ldr r3, [sp, #116]
	subs r3, #151
	cmp r3, #16
	bhi .L_080e1c90
	ldr r0, [sp, #28]
	ldr r1, [sp, #116]
	movs r4, #0
	mov r8, r0
	cmp r1, #151
	ble .L_080e1ba6
	adds r4, r1, #0
	subs r4, #152
.L_080e1ba6:
	cmp r4, #15
	ble .L_080e1bac
	movs r4, #15
.L_080e1bac:
	lsls r3, r4, #1
	ldr r5, [sp, #48]
	negs r3, r3
	adds r7, r3, #0
	adds r3, r4, r5
	movs r2, #1
	adds r6, r4, #0
	adds r3, #1
	mov r9, r2
	adds r7, #30
	adds r6, #49
	mov r10, r3
.L_080e1bc4:
	mov r0, r9
	adds r3, r4, r0
	cmp r3, #15
	bgt .L_080e1be6
	movs r2, #1
	movs r3, #16
	subs r3, r3, r0
	str r2, [sp, #4]
	str r4, [sp, #8]
	str r7, [sp, #0]
	ldr r0, [sp, #124]
	mov r1, r10
	adds r2, r6, #0
	ldr r5, [sp, #108]
	bl sub_080072f8
	ldr r4, [sp, #8]
.L_080e1be6:
	movs r0, #1
	add r9, r0
	mov r1, r9
	subs r7, #2
	adds r6, #1
	add r10, r0
	cmp r1, #10
	bne .L_080e1bc4
	movs r2, #0
	mov r3, r8
	mov r9, r2
	cmp r3, #0
	beq .L_080e1c32
	lsls r2, r4, #1
	movs r3, #32
	adds r6, r4, #0
	subs r3, r3, r2
	adds r6, #48
	mov r10, r3
	movs r7, #1
.L_080e1c0e:
	ldr r2, [sp, #48]
	mov r3, r9
	mov r5, r10
	adds r1, r2, r4
	str r5, [sp, #0]
	str r4, [sp, #8]
	adds r3, #16
	str r7, [sp, #4]
	ldr r0, [sp, #124]
	adds r2, r6, #0
	ldr r5, [sp, #108]
	bl sub_080072f8
	movs r0, #1
	add r9, r0
	ldr r4, [sp, #8]
	cmp r9, r8
	bne .L_080e1c0e
.L_080e1c32:
	movs r1, #2
	str r1, [sp, #0]
	movs r2, #7
	movs r1, #7
	movs r3, #3
	movs r0, #47
	bl sub_080ed408
	ldr r2, [sp, #28]
	movs r5, #96
	movs r6, #32
	str r5, [sp, #4]
	str r6, [sp, #0]
	ldr r3, [pc, #352]
	subs r2, #56
	mov r8, r2
	ldr r1, [sp, #128]
	ldr r4, [r3, #0]
	ldr r0, [sp, #124]
	movs r2, #32
	mov r3, r8
	bl sub_080072f4
	movs r0, #47
	bl sub_08002dd8
	movs r4, #2
	movs r1, #7
	movs r2, #7
	movs r3, #7
	movs r0, #47
	str r4, [sp, #0]
	bl sub_080ed408
	str r5, [sp, #4]
	str r6, [sp, #0]
	ldr r5, [pc, #308]
	ldr r0, [sp, #124]
	ldr r4, [r5, #0]
	ldr r1, [sp, #128]
	movs r2, #64
	mov r3, r8
	bl sub_080072f4
	movs r0, #47
	bl sub_08002dd8
.L_080e1c90:
	ldr r0, [sp, #128]
	ldr r1, [pc, #296]
	movs r3, #1
	adds r2, r0, r1
	str r3, [r2, #0]
	movs r0, #1
	bl sub_080030f8
	movs r0, #192
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	ldr r4, [sp, #20]
	ldr r5, [sp, #16]
	ldr r1, [sp, #116]
	lsls r0, r0, #5
	adds r2, #20
	adds r3, #4
	adds r4, #2
	adds r5, r5, r0
	adds r1, #1
	str r2, [sp, #28]
	str r3, [sp, #24]
	str r4, [sp, #20]
	str r5, [sp, #16]
	str r1, [sp, #116]
	ldr r2, [sp, #116]
	cmp r2, #170
	beq .L_080e1d00
	ldr r4, [sp, #20]
	movs r3, #2
	str r3, [sp, #104]
	str r4, [sp, #100]
	cmp r2, #16
	bne .L_080e1cda
	movs r0, #140
	bl sub_080f9010
.L_080e1cda:
	ldr r5, [sp, #116]
	cmp r5, #132
	bne .L_080e1ce6
	movs r0, #131
	bl sub_080f9010
.L_080e1ce6:
	ldr r0, [sp, #116]
	cmp r0, #151
	bne .L_080e1cf2
	movs r0, #145
	bl sub_080f9010
.L_080e1cf2:
	ldr r3, [pc, #204]
	ldr r3, [r3, #0]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	bne .L_080e1d00
	b sub_080e1848
.L_080e1d00:
	ldr r1, [sp, #132]
	movs r3, #0
	str r3, [r1, #16]
	ldr r0, [pc, #188]
	bl sub_08004278
	bl sub_080d67dc
	ldr r4, [pc, #180]
	ldr r2, [sp, #128]
	adds r3, r2, r4
	ldr r0, [r3, #0]
	bl sub_080d6750
	ldr r1, [pc, #172]
	movs r0, #9
	movs r2, #1
	bl sub_080dbb24
	ldr r0, [pc, #168]
	ldr r1, [pc, #168]
	movs r2, #1
	movs r3, #0
	bl sub_080e0524
	ldr r0, [pc, #164]
	ldr r1, [sp, #128]
	movs r2, #1
	movs r3, #1
	bl sub_080e0524
	movs r2, #192
	ldr r5, [sp, #128]
	lsls r2, r2, #7
	adds r1, r5, r2
	movs r3, #0
	movs r2, #1
	ldr r0, [pc, #144]
	bl sub_080e0524
	ldr r0, [pc, #140]
	bl sub_08002f40
	adds r1, r0, #0
	movs r0, #160
	ldr r3, [pc, #136]
	movs r2, #128
	lsls r0, r0, #19
	bl sub_080072f0
	movs r2, #0
	ldr r1, [sp, #120]
	movs r3, #0
	ldr r0, [pc, #124]
	bl sub_080e0524
	ldr r2, [pc, #120]
	ldr r3, [pc, #56]
	movs r4, #128
	strh r3, [r2, #0]
	movs r3, #160
	movs r5, #0
	lsls r3, r3, #15
	lsls r4, r4, #15
	str r3, [sp, #80]
	str r4, [sp, #76]
	str r5, [sp, #72]
	str r5, [sp, #68]
	ldr r2, [pc, #100]
	ldr r3, [pc, #104]
	mov r9, r5
	movs r1, #0
.L_080e1d90:
	movs r0, #1
	add r9, r0
	str r1, [r3, #0]
	adds r3, #28
	cmp r9, r2
	bne .L_080e1d90
	ldr r2, [sp, #128]
	movs r3, #225
	movs r1, #0
	lsls r3, r3, #7
	mov r9, r1
	adds r5, r2, r3
	b sub_080e1df8
