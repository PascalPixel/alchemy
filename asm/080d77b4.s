@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002304, 0x08002304
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080072f4, 0x080072f4
	.set sub_08007300, 0x08007300
	.set sub_08009008, 0x08009008
	.set sub_08009020, 0x08009020
	.set sub_080d7c08, 0x080d7c08
	.set sub_080d7c0e, 0x080d7c0e
	.set sub_080d7c4c, 0x080d7c4c
	.set sub_080d7c58, 0x080d7c58
	.set sub_080f9010, 0x080f9010
	.global Region_080d77b4
Region_080d77b4:
.L_080d77b4:
	bl sub_08004458
	movs r3, #63
	ands r3, r0
	adds r3, #32
	str r3, [r5, #0]
	bl sub_08004458
	movs r3, #31
	ands r3, r0
	adds r3, #120
	str r3, [r5, #4]
	movs r2, #1
	movs r3, #1
	negs r3, r3
	add r8, r2
	str r3, [r5, #24]
	mov r3, r8
	adds r5, #28
	cmp r3, #64
	bne .L_080d77b4
	movs r4, #0
	ldr r1, [pc, #132]
	mov r8, r4
	add r2, sp, #76
.L_080d77e6:
	ldrb r3, [r1, #0]
	movs r5, #1
	str r3, [r2, #0]
	add r8, r5
	ldrb r3, [r1, #1]
	mov r7, r8
	str r3, [r2, #4]
	adds r1, #2
	adds r2, #8
	cmp r7, #32
	bne .L_080d77e6
	movs r0, #141
	bl sub_080f9010
	movs r0, #0
	str r0, [sp, #40]
	ldr r3, [pc, #100]
	mov r2, sp
	adds r2, #52
	movs r1, #16
	ldr r3, [r3, #0]
	str r2, [sp, #16]
	mov r10, r1
	ldr r3, [sp, #40]
	cmp r3, #15
	bhi .L_080d787c
	ldr r7, [pc, #84]
	cmp r3, #1
	bne .L_080d7848
	ldr r5, [pc, #80]
	adds r6, r5, #0
	movs r3, #63
	adds r6, #128
.L_080d7828:
	str r3, [sp, #12]
	bl sub_08004458
	ldr r3, [sp, #12]
	ands r0, r3
	strb r0, [r5, #0]
	adds r5, #1
	cmp r5, r6
	bne .L_080d7828
	ldr r3, [pc, #40]
	movs r1, #144
	strh r3, [r7, #0]
	ldr r0, [pc, #52]
	lsls r1, r1, #3
	bl sub_080041d8
.L_080d7848:
	ldr r4, [sp, #24]
	ldrh r3, [r7, #0]
	adds r3, r3, r4
	strh r3, [r7, #0]
	ldr r5, [sp, #40]
	adds r4, #3
	str r4, [sp, #24]
	cmp r5, #15
	bne .L_080d787c
	ldr r0, [pc, #28]
	bl sub_08004278
	b .L_080d787c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x080ee974
	.4byte 0x03001b04
	.4byte 0x02010000
	.4byte 0x02010002
	.4byte 0x080d66cd
.L_080d787c:
	ldr r7, [sp, #40]
	cmp r7, #103
	ble .L_080d7888
	movs r0, #0
	mov r10, r0
	b .L_080d789e
.L_080d7888:
	ldr r1, [sp, #40]
	cmp r1, #63
	ble .L_080d7894
	movs r2, #6
	mov r10, r2
	b .L_080d789e
.L_080d7894:
	ldr r3, [sp, #40]
	cmp r3, #31
	ble .L_080d789e
	movs r4, #10
	mov r10, r4
.L_080d789e:
	ldr r5, [sp, #40]
	cmp r5, #167
	bgt .L_080d78c4
	bl sub_08004458
	movs r5, #3
	ands r0, r5
	subs r0, #1
	mov r9, r0
	bl sub_08004458
	ldr r3, [pc, #796]
	ands r0, r5
	subs r7, r0, #1
	mov r1, r9
	adds r0, #31
	strh r1, [r3, #4]
	strh r0, [r3, #6]
	b .L_080d78d2
.L_080d78c4:
	movs r2, #0
	mov r9, r2
	ldr r2, [pc, #776]
	movs r7, #0
	movs r3, #32
	strh r7, [r2, #4]
	strh r3, [r2, #6]
.L_080d78d2:
	ldr r2, [sp, #40]
	subs r2, #176
	cmp r2, #3
	bhi .L_080d78ee
	ldr r3, [pc, #764]
	ldrsb r3, [r3, r2]
	negs r4, r3
	ldr r2, [pc, #752]
	mov r9, r4
	adds r7, r3, #0
	mov r5, r9
	adds r3, #32
	strh r5, [r2, #4]
	strh r3, [r2, #6]
.L_080d78ee:
	movs r0, #0
	mov r1, r10
	mov r8, r0
	cmp r1, #0
	beq .L_080d793a
	ldr r2, [pc, #736]
	ldr r6, [pc, #740]
	mov r11, r2
.L_080d78fe:
	mov r0, r8
	movs r1, #3
	bl sub_080022fc
	mov r4, r11
	lsls r3, r0, #1
	ldrh r1, [r4, r3]
	ldr r5, [sp, #44]
	ldr r4, [pc, #724]
	ldrb r2, [r6, #0]
	adds r1, r5, r1
	mov r3, r9
	ldrb r5, [r4, r0]
	ldr r4, [pc, #716]
	subs r2, r2, r3
	ldrb r3, [r6, #1]
	ldrb r0, [r4, r0]
	subs r3, r3, r5
	str r5, [sp, #4]
	movs r5, #1
	str r0, [sp, #0]
	subs r3, r3, r7
	ldr r0, [sp, #48]
	ldr r4, [sp, #36]
	add r8, r5
	bl sub_080072f4
	adds r6, #2
	cmp r8, r10
	bne .L_080d78fe
.L_080d793a:
	ldr r3, [pc, #688]
	ldr r0, [sp, #40]
	ldr r4, [r3, #4]
	ldr r3, [r3, #0]
	str r3, [sp, #52]
	str r4, [sp, #56]
	cmp r0, #174
	bne .L_080d796c
	ldr r2, [sp, #44]
	ldr r3, [pc, #672]
	movs r1, #0
	movs r4, #13
	mov r8, r1
	negs r4, r4
	adds r0, r2, r3
.L_080d7958:
	ldmia r0!, {r1}
	ldrb r2, [r1, #9]
	adds r3, r4, #0
	movs r5, #1
	ands r3, r2
	add r8, r5
	strb r3, [r1, #9]
	mov r1, r8
	cmp r1, #9
	bne .L_080d7958
.L_080d796c:
	ldr r2, [sp, #40]
	cmp r2, #208
	ble .L_080d7990
	ldr r4, [sp, #44]
	ldr r5, [pc, #636]
	adds r3, r4, r5
	ldr r0, [r3, #0]
	ldr r1, [pc, #636]
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080d7984
	adds r3, #3
.L_080d7984:
	asrs r3, r3, #2
	movs r2, #3
	ands r3, r2
	ldrb r1, [r1, r3]
	bl sub_08009020
.L_080d7990:
	ldr r0, [sp, #16]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [sp, #52]
	add r2, sp, #60
	str r3, [r0, #4]
	movs r3, #0
	str r3, [r2, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r2, #4]
	mov r10, r2
	lsls r3, r7, #16
	movs r2, #152
	negs r3, r3
	mov r5, r9
	lsls r2, r2, #15
	movs r1, #0
	adds r4, r3, r2
	lsls r3, r5, #16
	negs r3, r3
	str r1, [sp, #20]
	mov r11, r1
	mov r9, r3
.L_080d79c0:
	mov r0, r11
	ldr r1, [sp, #44]
	movs r5, #144
	lsls r3, r0, #2
	ldr r2, [pc, #548]
	lsls r5, r5, #16
	adds r3, r3, r1
	movs r7, #0
	mov r8, r4
	add r5, r9
	adds r6, r3, r2
.L_080d79d6:
	mov r3, r10
	mov r0, r8
	str r5, [r3, #0]
	str r0, [r3, #8]
	mov r1, r10
	ldmia r6!, {r0}
	ldr r2, [sp, #16]
	movs r3, #0
	str r4, [sp, #8]
	bl sub_08009008
	movs r1, #128
	lsls r1, r1, #14
	adds r7, #1
	adds r5, r5, r1
	ldr r4, [sp, #8]
	cmp r7, #3
	bne .L_080d79d6
	ldr r3, [sp, #20]
	movs r2, #3
	adds r3, #1
	add r11, r2
	adds r4, r4, r1
	str r3, [sp, #20]
	cmp r3, #3
	bne .L_080d79c0
	ldr r1, [sp, #40]
	subs r1, #160
	cmp r1, #157
	bhi .L_080d7a58
	ldr r3, [sp, #40]
	ldr r5, [sp, #40]
	subs r3, #208
	movs r2, #80
	movs r4, #8
	cmp r5, #175
	bgt .L_080d7a2c
	movs r3, #96
	subs r2, r3, r1
	lsls r3, r1, #2
	adds r4, r3, #0
	subs r4, #56
	b .L_080d7a40
.L_080d7a2c:
	ldr r7, [sp, #40]
	cmp r7, #208
	ble .L_080d7a40
	cmp r3, #0
	bge .L_080d7a3a
	adds r3, r7, #0
	subs r3, #205
.L_080d7a3a:
	asrs r3, r3, #2
	adds r4, r3, #0
	adds r4, #8
.L_080d7a40:
	movs r3, #24
	str r3, [sp, #0]
	movs r3, #48
	str r3, [sp, #4]
	ldr r5, [pc, #432]
	ldr r3, [sp, #44]
	ldr r0, [sp, #48]
	adds r1, r3, r5
	ldr r7, [sp, #32]
	adds r3, r4, #0
	bl sub_08007300
.L_080d7a58:
	ldr r0, [sp, #40]
	cmp r0, #32
	bne .L_080d7a64
	movs r0, #134
	bl sub_080f9010
.L_080d7a64:
	ldr r1, [sp, #40]
	cmp r1, #64
	bne .L_080d7a70
	movs r0, #134
	bl sub_080f9010
.L_080d7a70:
	ldr r2, [sp, #40]
	cmp r2, #104
	bne .L_080d7a7c
	movs r0, #134
	bl sub_080f9010
.L_080d7a7c:
	ldr r3, [sp, #40]
	cmp r3, #176
	bne .L_080d7a88
	movs r0, #134
	bl sub_080f9010
.L_080d7a88:
	ldr r4, [sp, #40]
	cmp r4, #226
	bne .L_080d7a94
	movs r0, #145
	bl sub_080f9010
.L_080d7a94:
	bl sub_080049ac
	ldr r5, [sp, #40]
	cmp r5, #32
	bne .L_080d7af2
	movs r7, #0
	ldr r5, [pc, #348]
	mov r8, r7
	movs r6, #127
	movs r7, #31
.L_080d7aa8:
	bl sub_08004458
	ands r0, r7
	adds r0, #68
	lsls r0, r0, #16
	str r0, [r5, #0]
	bl sub_08004458
	ands r0, r7
	adds r0, #8
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl sub_08004458
	ands r0, r6
	subs r0, #63
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl sub_08004458
	negs r0, r0
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl sub_08004458
	movs r3, #15
	ands r3, r0
	movs r0, #1
	add r8, r0
	adds r3, #32
	mov r1, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #32
	bne .L_080d7aa8
.L_080d7af2:
	ldr r2, [sp, #40]
	cmp r2, #64
	bne .L_080d7b50
	movs r3, #0
	ldr r5, [pc, #264]
	mov r8, r3
	movs r6, #31
.L_080d7b00:
	bl sub_08004458
	movs r1, #48
	bl sub_08002304
	adds r0, #60
	lsls r0, r0, #16
	str r0, [r5, #0]
	bl sub_08004458
	ands r0, r6
	adds r0, #52
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl sub_08004458
	movs r3, #127
	ands r3, r0
	subs r3, #63
	lsls r3, r3, #12
	str r3, [r5, #12]
	bl sub_08004458
	negs r0, r0
	ands r0, r6
	subs r0, #32
	lsls r0, r0, #13
	str r0, [r5, #16]
	bl sub_08004458
	movs r3, #15
	movs r4, #1
	ands r3, r0
	add r8, r4
	adds r3, #32
	mov r7, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #32
	bne .L_080d7b00
.L_080d7b50:
	ldr r0, [sp, #40]
	cmp r0, #104
	bne .L_080d7bac
	movs r1, #0
	ldr r5, [pc, #164]
	mov r8, r1
	movs r6, #31
.L_080d7b5e:
	bl sub_08004458
	movs r3, #63
	ands r3, r0
	adds r3, #52
	lsls r3, r3, #16
	str r3, [r5, #0]
	bl sub_08004458
	ands r0, r6
	adds r0, #72
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl sub_08004458
	movs r3, #127
	ands r3, r0
	subs r3, #63
	lsls r3, r3, #11
	str r3, [r5, #12]
	bl sub_08004458
	negs r0, r0
	ands r0, r6
	subs r0, #32
	lsls r0, r0, #13
	str r0, [r5, #16]
	bl sub_08004458
	movs r3, #15
	ands r3, r0
	movs r2, #1
	adds r3, #32
	add r8, r2
	str r3, [r5, #24]
	mov r3, r8
	adds r5, #28
	cmp r3, #32
	bne .L_080d7b5e
.L_080d7bac:
	ldr r3, [sp, #40]
	subs r3, #32
	cmp r3, #175
	bhi sub_080d7c58
	movs r4, #0
	ldr r5, [pc, #72]
	mov r8, r4
	ldr r3, [r5, #24]
	cmp r3, #0
	blt sub_080d7c4c
	ldr r7, [sp, #40]
	cmp r7, #191
	ble sub_080d7c08
	mov r0, r8
	movs r1, #7
	bl sub_080022fc
	adds r4, r0, #4
	b sub_080d7c0e
