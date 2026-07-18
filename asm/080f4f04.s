@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_08004458, 0x08004458
	.set sub_080072f0, 0x080072f0
	.set sub_080150b0, 0x080150b0
	.set sub_080f4100, 0x080f4100
	.set sub_080f44b8, 0x080f44b8
	.set sub_080f4d84, 0x080f4d84
	.set sub_080f9010, 0x080f9010
	.global Region_080f4f04
Region_080f4f04:
	negs r2, r2
	movs r3, #0
	str r3, [r7, #4]
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r2, r3, #4
	cmp r2, #0
	bge .L_080f4f16
	adds r2, #63
.L_080f4f16:
	asrs r3, r2, #6
	str r3, [r7, #16]
	bl sub_08004458
	movs r3, #3
	ands r0, r3
	cmp r0, #0
	bne .L_080f4f74
	ldr r3, [r7, #20]
	ldr r2, [r7, #12]
	asrs r3, r3, #8
	asrs r2, r2, #8
	adds r4, r3, #0
	muls r4, r3
	adds r0, r2, #0
	muls r0, r2
	adds r3, r4, #0
	adds r0, r0, r3
	ldr r3, [pc, #524]
	bl sub_080072f0
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_080f4f4c
	adds r3, #63
.L_080f4f4c:
	asrs r6, r3, #6
	bl sub_08004458
	ldr r3, [pc, #504]
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl sub_08002322
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #8
	str r3, [r7, #12]
	adds r0, r5, #0
	bl sub_0800231c
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #8
	b .L_080f4f96
.L_080f4f74:
	ldr r2, [r7, #12]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_080f4f82
	adds r3, #63
.L_080f4f82:
	ldr r2, [r7, #20]
	asrs r3, r3, #6
	str r3, [r7, #12]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_080f4f94
	adds r3, #63
.L_080f4f94:
	asrs r3, r3, #6
.L_080f4f96:
	str r3, [r7, #20]
	ldr r5, [sp, #32]
	ldr r3, [r5, #0]
	ldr r6, [sp, #28]
	cmp r3, #19
	ble .L_080f4fa6
	subs r3, #20
	str r3, [r5, #0]
.L_080f4fa6:
	ldr r3, [r7, #16]
	ldr r0, [pc, #420]
	cmp r3, r0
	bgt .L_080f4fee
	movs r1, #0
	str r1, [r7, #12]
	str r1, [r7, #16]
	str r1, [r7, #20]
	ldr r2, [sp, #32]
	str r1, [r2, #0]
	ldr r5, [r7, #24]
	adds r0, r5, #0
	cmp r5, #0
	bge .L_080f4fc6
	ldr r3, [pc, #400]
	adds r0, r5, r3
.L_080f4fc6:
	asrs r0, r0, #10
	movs r1, #3
	bl sub_080022fc
	cmp r0, #1
	bne .L_080f4fdc
	movs r4, #128
	lsls r4, r4, #4
	adds r3, r5, r4
	str r3, [r7, #24]
	b .L_080f4fee
.L_080f4fdc:
	cmp r0, #2
	bne .L_080f4fee
	movs r0, #128
	lsls r0, r0, #3
	adds r3, r5, r0
	str r3, [r7, #24]
	b .L_080f4fee
	mov r1, r11
	lsls r6, r1, #2
.L_080f4fee:
	ldr r3, [r7, #12]
	lsls r2, r3, #6
	subs r2, r2, r3
	cmp r2, #0
	bge .L_080f4ffa
	adds r2, #63
.L_080f4ffa:
	asrs r3, r2, #6
	str r3, [r7, #12]
	ldr r3, [r7, #16]
	lsls r2, r3, #6
	subs r2, r2, r3
	cmp r2, #0
	bge .L_080f500a
	adds r2, #63
.L_080f500a:
	asrs r3, r2, #6
	str r3, [r7, #16]
	ldr r3, [r7, #20]
	lsls r2, r3, #6
	subs r2, r2, r3
	cmp r2, #0
	bge .L_080f501a
	adds r2, #63
.L_080f501a:
	asrs r3, r2, #6
	str r3, [r7, #20]
	ldr r4, [sp, #28]
	ldr r5, [sp, #36]
	ldr r2, [r7, #24]
	ldr r3, [r4, r5]
	ldr r0, [pc, #304]
	adds r2, r2, r3
	str r2, [r7, #24]
	cmp r2, r0
	ble .L_080f5036
	ldr r1, [pc, #296]
	adds r3, r2, r1
	str r3, [r7, #24]
.L_080f5036:
	ldr r2, [sp, #32]
	ldr r3, [r2, #0]
	cmp r3, #0
	ble .L_080f5042
	subs r3, #1
	str r3, [r2, #0]
.L_080f5042:
	ldr r3, [r7, #12]
	cmp r3, #0
	bne .L_080f50a6
	ldr r3, [r7, #16]
	cmp r3, #0
	bne .L_080f50a6
	ldr r3, [r7, #20]
	cmp r3, #0
	bne .L_080f50a6
	ldr r3, [r7, #4]
	cmp r3, #0
	bne .L_080f50a6
	movs r1, #192
	ldr r0, [r7, #24]
	lsls r1, r1, #4
	add r5, sp, #224
	bl sub_080022ec
	ldr r1, [pc, #248]
	movs r3, #0
	str r0, [r5, r6]
	mov r10, r3
	ldrsh r3, [r1, r3]
	ldr r2, [r7, #8]
	lsls r3, r3, #16
	cmp r2, r3
	ble .L_080f5082
	adds r3, r6, #0
	adds r3, #8
	mov r6, r10
	str r6, [r5, r3]
	b .L_080f50a0
.L_080f5082:
	movs r0, #1
	add r10, r0
	mov r3, r10
	cmp r3, #6
	beq .L_080f50a0
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
	lsls r3, r3, #16
	cmp r2, r3
	ble .L_080f5082
	ldr r3, [sp, #28]
	add r2, sp, #224
	adds r3, #8
	mov r5, r10
	str r5, [r2, r3]
.L_080f50a0:
	ldr r6, [sp, #56]
	adds r6, #1
	str r6, [sp, #56]
.L_080f50a6:
	ldr r0, [sp, #32]
	ldr r1, [sp, #28]
	movs r2, #1
	add r11, r2
	adds r0, #4
	adds r1, #4
	mov r3, r11
	str r0, [sp, #32]
	str r1, [sp, #28]
	adds r7, #28
	cmp r3, #2
	beq .L_080f50c0
	b sub_080f4d84
.L_080f50c0:
	ldr r4, [sp, #56]
	cmp r4, #2
	beq .L_080f50c8
	b .L_080f5260
.L_080f50c8:
	movs r7, #1
	movs r6, #0
	movs r5, #2
	negs r7, r7
	movs r0, #60
	str r5, [sp, #96]
	str r6, [sp, #88]
	str r7, [sp, #84]
	str r6, [sp, #80]
	str r0, [sp, #76]
	add r1, sp, #224
	ldr r2, [r1, #0]
	ldr r3, [r1, #4]
	cmp r2, r3
	bne .L_080f5168
	ldr r3, [r1, #8]
	cmp r2, r3
	bne .L_080f5168
	ldr r3, [r1, #12]
	cmp r2, r3
	bne .L_080f5168
	ldr r2, [sp, #52]
	movs r3, #120
	str r3, [r2, #4]
	str r3, [sp, #144]
	str r6, [sp, #84]
	ldr r3, [sp, #136]
	ldr r2, [r1, #0]
	cmp r2, r3
	bne .L_080f510c
	ldr r4, [sp, #92]
	lsls r3, r4, #4
	subs r3, r3, r4
	b .L_080f5126
.L_080f510c:
	ldr r5, [sp, #40]
	ldr r3, [r5, #4]
	cmp r2, r3
	bne .L_080f5120
	ldr r6, [sp, #92]
	lsls r3, r6, #1
	adds r3, r3, r6
	lsls r3, r3, #3
	adds r3, r3, r6
	b .L_080f5126
.L_080f5120:
	ldr r7, [sp, #92]
	lsls r3, r7, #2
	adds r3, r3, r7
.L_080f5126:
	str r3, [sp, #80]
	ldr r3, [pc, #56]
	ldr r1, [sp, #108]
	movs r2, #153
	lsls r2, r2, #3
	ldr r0, [r3, #16]
	adds r3, r1, r2
	ldr r2, [r3, #0]
	movs r1, #6
	movs r3, #0
	str r3, [sp, #0]
	bl sub_080150b0
	movs r0, #93
	bl sub_080f9010
	b .L_080f5260
	.4byte 0x030001d8
	.4byte 0x0000ffff
	.4byte 0x0002ffff
	.4byte 0x000003ff
	.4byte 0x000047ff
	.4byte 0xffffb800
	.4byte 0x080f541a
	.4byte 0x02000240
.L_080f5168:
	ldr r5, [pc, #392]
	movs r4, #0
	add r7, sp, #768
	mov r8, r4
	mov r11, r4
	mov r12, r5
	adds r6, r7, #0
.L_080f5176:
	mov r5, r11
	adds r5, #1
	mov r10, r5
	cmp r5, #4
	beq .L_080f51b0
	add r1, sp, #768
	lsls r3, r5, #2
	mov r0, r11
	adds r3, r3, r1
	ldr r1, [pc, #364]
	add r7, sp, #768
	lsls r2, r0, #2
	mov r0, r12
	adds r4, r3, r0
	add r7, r12
	adds r0, r6, r1
.L_080f5196:
	ldr r1, [r7, r2]
	ldmia r4!, {r3}
	cmp r1, r3
	bne .L_080f51a6
	movs r3, #1
	stmia r0!, {r1}
	adds r6, #4
	add r8, r3
.L_080f51a6:
	movs r1, #1
	add r10, r1
	mov r3, r10
	cmp r3, #4
	bne .L_080f5196
.L_080f51b0:
	mov r11, r5
	cmp r5, #3
	bne .L_080f5176
	mov r4, r8
	cmp r4, #1
	bne .L_080f51de
	ldr r5, [sp, #92]
	ldr r3, [pc, #316]
	str r5, [sp, #80]
	ldr r6, [sp, #108]
	movs r7, #153
	lsls r7, r7, #3
	ldr r0, [r3, #16]
	movs r1, #0
	adds r3, r6, r7
	ldr r2, [r3, #0]
	str r1, [sp, #0]
	movs r3, #0
	movs r1, #6
	bl sub_080150b0
	movs r2, #3
	str r2, [sp, #84]
.L_080f51de:
	mov r3, r8
	cmp r3, #2
	bne .L_080f521e
	ldr r4, [sp, #92]
	lsls r4, r4, #1
	str r4, [sp, #80]
	ldr r3, [pc, #272]
	ldr r5, [sp, #108]
	movs r6, #153
	lsls r6, r6, #3
	ldr r0, [r3, #16]
	adds r3, r5, r6
	ldr r2, [r3, #0]
	movs r1, #6
	movs r3, #0
	movs r7, #0
	str r7, [sp, #0]
	bl sub_080150b0
	bl sub_08004458
	movs r3, #1
	ands r3, r0
	ldr r0, [sp, #52]
	lsls r3, r3, #2
	movs r2, #60
	str r2, [r3, r0]
	movs r1, #2
	movs r0, #91
	str r1, [sp, #84]
	bl sub_080f9010
.L_080f521e:
	mov r2, r8
	cmp r2, #3
	bne .L_080f5260
	ldr r4, [sp, #92]
	lsls r3, r4, #1
	adds r3, r3, r4
	str r3, [sp, #80]
	ldr r5, [sp, #108]
	ldr r3, [pc, #204]
	movs r6, #153
	lsls r6, r6, #3
	ldr r0, [r3, #16]
	adds r3, r5, r6
	ldr r2, [r3, #0]
	movs r1, #6
	movs r3, #0
	movs r7, #0
	str r7, [sp, #0]
	bl sub_080150b0
	bl sub_08004458
	movs r3, #1
	ands r3, r0
	ldr r0, [sp, #52]
	lsls r3, r3, #2
	movs r2, #60
	str r2, [r3, r0]
	movs r1, #1
	movs r0, #92
	str r1, [sp, #84]
	bl sub_080f9010
.L_080f5260:
	ldr r2, [sp, #68]
	cmp r2, #128
	beq .L_080f5286
	lsls r3, r2, #3
	adds r2, r3, #0
	ldr r0, [pc, #148]
	movs r1, #0
	adds r2, #204
	adds r3, #200
.L_080f5272:
	ldr r4, [sp, #108]
	str r0, [r4, r3]
	str r1, [r4, r2]
	ldr r5, [sp, #68]
	adds r5, #1
	adds r2, #8
	adds r3, #8
	str r5, [sp, #68]
	cmp r5, #128
	bne .L_080f5272
.L_080f5286:
	movs r1, #228
	mov r8, r1
	mov r1, sp
	adds r1, #240
	movs r6, #0
	mov r7, sp
	mov r4, sp
	movs r2, #220
	movs r3, #212
	str r1, [sp, #4]
	mov r11, r6
	adds r7, #166
	add r5, sp, #164
	adds r4, #162
	add r0, sp, #160
	mov lr, r2
	mov r12, r3
	movs r6, #204
.L_080f52aa:
	ldr r1, [sp, #4]
	ldmia r1!, {r3}
	adds r2, r1, #0
	str r2, [sp, #4]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r3, #128
	ldr r2, [pc, #52]
	ldr r1, [pc, #48]
	lsls r3, r3, #16
	asrs r3, r3, #16
	strh r3, [r7, #0]
	strh r2, [r5, #0]
	strh r1, [r4, #0]
	strh r3, [r0, #0]
	ldrh r2, [r7, #0]
	lsls r2, r2, #16
	mov r10, r2
	ldr r2, [sp, #108]
	ldr r3, [r2, r6]
	mov r1, r10
	orrs r3, r1
	str r3, [r2, r6]
	ldrh r2, [r5, #0]
	lsls r2, r2, #16
	mov r10, r2
	ldr r2, [sp, #108]
	mov r1, r12
	ldr r3, [r2, r1]
	mov r2, r10
	orrs r3, r2
	ldr r2, [sp, #108]
	str r3, [r2, r1]
	b .L_080f5304
	.4byte 0x00000000
	.4byte 0xfffffde0
	.4byte 0xfffffd78
	.4byte 0x02000240
	.4byte 0x40f02000
.L_080f5304:
	ldrh r2, [r4, #0]
	ldr r1, [sp, #108]
	lsls r2, r2, #16
	mov r10, r2
	mov r2, lr
	ldr r3, [r1, r2]
	mov r1, r10
	orrs r3, r1
	ldr r1, [sp, #108]
	str r3, [r1, r2]
	ldrh r2, [r0, #0]
	lsls r2, r2, #16
	mov r10, r2
	mov r2, r8
	ldr r3, [r1, r2]
	mov r1, r10
	orrs r3, r1
	ldr r1, [sp, #108]
	str r3, [r1, r2]
	movs r3, #1
	add r11, r3
	movs r2, #32
	mov r1, r11
	add r8, r2
	add lr, r2
	add r12, r2
	adds r6, #32
	cmp r1, #4
	bne .L_080f52aa
	ldr r0, [sp, #108]
	movs r1, #224
	ldr r3, [pc, #140]
	adds r0, #200
	lsls r1, r1, #19
	ldr r2, [pc, #136]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	bl sub_080030f8
	ldr r2, [sp, #100]
	adds r2, #1
	str r2, [sp, #100]
	bl sub_080f44b8
	movs r3, #0
	movs r6, #128
	mov r11, r3
	lsls r6, r6, #1
.L_080f5366:
	mov r4, r11
	movs r5, #128
	lsls r3, r4, #12
	lsls r5, r5, #9
	subs r5, r5, r3
	ldr r0, [sp, #116]
	adds r2, r5, #0
	ldr r1, [pc, #96]
	adds r3, r6, #0
	bl sub_080f4100
	adds r2, r5, #0
	movs r1, #160
	movs r5, #1
	add r0, sp, #256
	lsls r1, r1, #19
	adds r3, r6, #0
	add r11, r5
	bl sub_080f4100
	mov r7, r11
	movs r0, #1
	bl sub_080030f8
	cmp r7, #17
	bne .L_080f5366
	movs r0, #12
	bl sub_08002dd8
	movs r0, #45
	bl sub_08002dd8
	movs r0, #40
	bl sub_08002dd8
	movs r0, #39
	bl sub_08002dd8
	movs r0, #41
	bl sub_08002dd8
	movs r3, #192
	lsls r3, r3, #2
	add sp, r3
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
