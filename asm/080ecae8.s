@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_08009038, 0x08009038
	.set sub_080b5088, 0x080b5088
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080e396c, 0x080e396c
	.set sub_080f9010, 0x080f9010
	.global Region_080ecae8
Region_080ecae8:
	add r2, r11
	movs r3, #2
	str r3, [r2, #0]
	ldr r2, [pc, #208]
	movs r3, #75
	add r2, r11
	str r3, [r2, #0]
	movs r7, #0
	movs r2, #128
	ldr r3, [pc, #200]
	mov r8, r7
	movs r1, #0
	lsls r2, r2, #3
.L_080ecb02:
	movs r0, #1
	add r8, r0
	str r1, [r3, #0]
	adds r3, #28
	cmp r8, r2
	bne .L_080ecb02
	movs r5, #225
	movs r1, #0
	lsls r5, r5, #7
	mov r8, r1
	movs r6, #0
	add r5, r11
	movs r7, #31
.L_080ecb1c:
	bl sub_08004458
	movs r2, #1
	ands r0, r7
	add r8, r2
	adds r0, #32
	mov r3, r8
	str r0, [r5, #0]
	str r6, [r5, #4]
	str r6, [r5, #24]
	adds r5, #28
	cmp r3, #16
	bne .L_080ecb1c
	ldr r2, [pc, #144]
	add r2, r11
	ldr r3, [r2, #0]
	ldr r3, [r3, #20]
	movs r4, #0
	mov r8, r4
	cmp r3, #0
	beq .L_080ecb7a
	movs r5, #225
	lsls r5, r5, #7
	adds r7, r2, #0
	add r6, sp, #88
	add r5, r11
	movs r4, #36
.L_080ecb52:
	ldr r3, [r7, #0]
	ldrsh r0, [r3, r4]
	adds r1, r6, #0
	str r4, [sp, #8]
	bl sub_080e396c
	ldr r3, [r6, #0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #0]
	ldr r3, [r7, #0]
	ldr r4, [sp, #8]
	movs r2, #1
	ldr r3, [r3, #20]
	add r8, r2
	adds r5, #28
	adds r4, #2
	cmp r8, r3
	bne .L_080ecb52
.L_080ecb7a:
	movs r2, #1
	movs r3, #1
	ldr r0, [pc, #76]
	mov r1, r11
	bl sub_080e0524
	ldr r0, [pc, #72]
	bl sub_080f9010
	ldr r2, [pc, #68]
	ldr r3, [pc, #40]
	strh r3, [r2, #0]
	ldr r3, [pc, #40]
	adds r2, #50
	strh r3, [r2, #0]
	movs r3, #0
	mov r10, r3
.L_080ecb9c:
	mov r4, r10
	cmp r4, #96
	bne .L_080ecba8
	movs r0, #134
	bl sub_080b50e8
.L_080ecba8:
	ldr r7, [sp, #24]
	str r7, [sp, #20]
	movs r7, #225
	movs r6, #0
	lsls r7, r7, #7
	mov r8, r6
	add r7, r11
	b .L_080ecbd8
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00007784
	.4byte 0x02010018
	.4byte 0x00007828
	.4byte 0x0000006f
	.4byte 0x00000121
	.4byte 0x04000020
.L_080ecbd8:
	mov r0, r8
	lsls r5, r0, #4
	adds r3, r5, #7
	cmp r10, r3
	bne .L_080ecbe8
	movs r0, #154
	bl sub_080f9010
.L_080ecbe8:
	cmp r10, r5
	bge .L_080ecbee
	b .L_080ecd06
.L_080ecbee:
	ldr r3, [r7, #24]
	cmp r3, #31
	ble .L_080ecbf6
	b .L_080ecd06
.L_080ecbf6:
	mov r1, r10
	lsls r0, r1, #4
	movs r1, #104
	bl sub_080022fc
	movs r6, #104
	ldr r3, [r7, #4]
	movs r4, #17
	adds r5, r0, #0
	ldr r2, [r7, #0]
	ldr r0, [sp, #20]
	str r6, [sp, #4]
	str r4, [sp, #0]
	adds r3, r3, r5
	ldr r4, [r0, #4]
	subs r2, #8
	ldr r0, [sp, #68]
	subs r3, #216
	mov r1, r11
	bl sub_080072f4
	ldr r3, [r7, #4]
	movs r1, #17
	adds r3, r3, r5
	subs r5, r6, r5
	ldr r2, [r7, #0]
	ldr r6, [sp, #20]
	str r1, [sp, #0]
	str r5, [sp, #4]
	subs r2, #8
	subs r3, #112
	ldr r4, [r6, #4]
	ldr r0, [sp, #68]
	mov r1, r11
	bl sub_080072f4
	movs r1, #34
	ldr r2, [r7, #0]
	ldr r3, [r7, #4]
	str r1, [sp, #0]
	movs r1, #65
	str r1, [sp, #4]
	movs r1, #221
	lsls r1, r1, #3
	subs r3, #65
	subs r2, #17
	ldr r4, [r6, #4]
	ldr r0, [sp, #68]
	add r1, r11
	bl sub_080072f4
	ldr r3, [r7, #4]
	cmp r3, #111
	bgt .L_080ecc68
	adds r3, #16
	str r3, [r7, #4]
	b .L_080ecc6e
.L_080ecc68:
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_080ecc6e:
	ldr r3, [r7, #4]
	cmp r3, #111
	ble .L_080ecd06
	movs r0, #0
	str r0, [sp, #32]
	ldr r1, [r7, #0]
	str r1, [sp, #28]
	ldr r3, [r7, #24]
	cmp r3, #7
	bgt .L_080ecc8a
	ldr r2, [pc, #464]
	movs r3, #4
	add r2, r11
	str r3, [r2, #0]
.L_080ecc8a:
	movs r2, #0
	mov r9, r2
	ldr r2, [pc, #456]
.L_080ecc90:
	ldr r3, [r2, #24]
	cmp r3, #0
	bne .L_080eccf8
	str r2, [sp, #16]
	bl sub_08004458
	ldr r6, [pc, #444]
	ands r6, r0
	bl sub_08004458
	ldr r5, [pc, #440]
	ldr r4, [sp, #28]
	ldr r3, [pc, #440]
	ldr r2, [sp, #16]
	ands r5, r0
	adds r5, r5, r3
	movs r0, #112
	lsls r3, r4, #16
	str r3, [r2, #0]
	lsls r3, r0, #16
	str r3, [r2, #4]
	adds r0, r5, #0
	bl sub_08002322
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	ldr r2, [sp, #16]
	asrs r3, r3, #7
	str r3, [r2, #12]
	adds r0, r5, #0
	bl sub_0800231c
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	ldr r2, [sp, #16]
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r2, #16]
	bl sub_08004458
	movs r3, #7
	ands r3, r0
	ldr r2, [sp, #16]
	adds r3, #32
	str r3, [r2, #24]
	ldr r1, [sp, #32]
	adds r1, #1
	str r1, [sp, #32]
	cmp r1, #4
	beq .L_080ecd06
.L_080eccf8:
	movs r3, #1
	movs r4, #128
	add r9, r3
	lsls r4, r4, #3
	adds r2, #28
	cmp r9, r4
	bne .L_080ecc90
.L_080ecd06:
	movs r6, #1
	add r8, r6
	mov r0, r8
	adds r7, #28
	cmp r0, #5
	beq .L_080ecd14
	b .L_080ecbd8
.L_080ecd14:
	ldr r2, [pc, #336]
	mov r4, r11
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	movs r1, #0
	mov r8, r1
	cmp r3, #0
	beq .L_080ecd60
	movs r6, #36
	movs r7, #8
.L_080ecd28:
	cmp r10, r7
	bne .L_080ecd4c
	mov r0, r11
	adds r5, r0, r2
	ldr r3, [r5, #0]
	ldrsh r0, [r3, r6]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	mov r3, r8
	bl sub_080d6888
	ldr r3, [r5, #0]
	movs r1, #2
	ldrsh r0, [r3, r6]
	bl sub_080b5088
.L_080ecd4c:
	ldr r2, [pc, #280]
	movs r3, #1
	mov r4, r11
	add r8, r3
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	adds r6, #2
	adds r7, #16
	cmp r8, r3
	bne .L_080ecd28
.L_080ecd60:
	movs r6, #0
	mov r8, r6
	ldr r6, [pc, #240]
.L_080ecd66:
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_080ecdde
	subs r3, #1
	movs r2, #128
	str r3, [r6, #24]
	adds r0, r6, #0
	movs r1, #60
	lsls r2, r2, #7
	bl sub_080e3908
	ldr r3, [r6, #4]
	asrs r7, r3, #16
	mov r12, r7
	cmp r7, #120
	ble .L_080ecd94
	ldr r3, [r6, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_080ecdde
.L_080ecd94:
	ldr r2, [r6, #0]
	cmp r2, #0
	blt .L_080ecdde
	asrs r7, r2, #16
	cmp r7, #126
	bgt .L_080ecdde
	cmp r3, #0
	blt .L_080ecdde
	ldr r0, [r6, #24]
	cmp r0, #0
	bge .L_080ecdac
	adds r0, #7
.L_080ecdac:
	asrs r0, r0, #3
	adds r0, #3
	lsls r5, r0, #1
	ldr r2, [pc, #184]
	mov r1, r8
	subs r3, r5, #2
	movs r4, #1
	ands r4, r1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #64]
	adds r1, r2, r1
	lsrs r2, r0, #31
	adds r2, r0, r2
	asrs r2, r2, #1
	subs r2, r7, r2
	mov r7, r12
	str r0, [sp, #0]
	subs r3, r7, r0
	str r5, [sp, #4]
	ldr r0, [sp, #24]
	lsls r4, r4, #2
	ldr r4, [r4, r0]
	ldr r0, [sp, #68]
	bl sub_080072f4
.L_080ecdde:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #3
	adds r6, #28
	cmp r8, r2
	bne .L_080ecd66
	movs r0, #4
	movs r1, #8
	bl sub_080e155c
	bl sub_080cd52c
	ldr r2, [pc, #116]
	movs r3, #1
	add r2, r11
	str r3, [r2, #0]
	movs r0, #1
	bl sub_080030f8
	movs r3, #1
	add r10, r3
	mov r4, r10
	cmp r4, #144
	beq .L_080ece12
	b .L_080ecb9c
.L_080ece12:
	ldr r5, [pc, #96]
	movs r6, #0
	mov r8, r6
	add r5, r11
.L_080ece1a:
	movs r7, #1
	ldmia r5!, {r0}
	add r8, r7
	bl sub_08009038
	mov r0, r8
	cmp r0, #8
	bne .L_080ece1a
	ldr r0, [pc, #76]
	bl sub_08004278
	movs r0, #47
	bl sub_08002dd8
	movs r0, #46
	bl sub_08002dd8
	bl sub_080cdbc0
	add sp, #116
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
