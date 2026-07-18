@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08077080, 0x08077080
	.set sub_08077178, 0x08077178
	.set sub_080771a0, 0x080771a0
	.set sub_080772b8, 0x080772b8
	.set sub_080772f8, 0x080772f8
	.set sub_080bd3c8, 0x080bd3c8
	.global Region_080bef88
Region_080bef88:
	ldr r3, [sp, #12]
	ldr r0, [r3, #0]
	bl sub_080772f8
	movs r1, #200
	lsls r0, r0, #16
	bl sub_080022ec
	adds r5, r0, #0
	bl sub_080771a0
	ldr r3, [pc, #584]
	ands r0, r3
	cmp r5, r0
	bgt .L_080befa8
	b .L_080bf1a8
.L_080befa8:
	ldr r4, [sp, #8]
	ldr r3, [r4, #0]
	movs r2, #1
	adds r3, #44
	strb r2, [r3, #0]
	b .L_080bf1a8
	mov r0, r11
	bl sub_08077080
	adds r7, r0, #0
	ldr r0, [sp, #8]
	ldrb r2, [r7, #2]
	ldr r3, [r0, #0]
	mov r1, r11
	str r2, [r3, #80]
	movs r2, #0
	str r2, [r3, #88]
	str r1, [r3, #76]
	ldrb r3, [r7, #3]
	adds r2, r3, #0
	cmp r2, #65
	beq .L_080beff2
	cmp r2, #41
	beq .L_080befe8
	cmp r2, #42
	beq .L_080befe8
	cmp r2, #43
	beq .L_080befe8
	cmp r2, #44
	beq .L_080befe8
	cmp r2, #68
	bne .L_080bf044
.L_080befe8:
	adds r2, r3, #0
	cmp r2, #65
	beq .L_080beff2
	cmp r2, #68
	bne .L_080beff6
.L_080beff2:
	movs r5, #153
	b .L_080bf002
.L_080beff6:
	cmp r2, #41
	beq .L_080bf000
	movs r5, #64
	cmp r2, #43
	bne .L_080bf002
.L_080bf000:
	movs r5, #32
.L_080bf002:
	cmp r3, #65
	beq .L_080bf010
	cmp r3, #41
	beq .L_080bf010
	movs r6, #2
	cmp r3, #42
	bne .L_080bf012
.L_080bf010:
	movs r6, #1
.L_080bf012:
	bl sub_080771a0
	movs r3, #255
	ands r0, r3
	cmp r0, r5
	bge .L_080bf0f8
	ldr r3, [sp, #8]
	ldr r2, [r3, #0]
	movs r3, #1
	ldrsb r3, [r2, r3]
	movs r0, #0
	cmp r0, r3
	bge .L_080bf0f8
	adds r1, r2, #0
	adds r2, #30
.L_080bf030:
	ldrb r3, [r2, #0]
	adds r3, r3, r6
	strb r3, [r2, #0]
	movs r3, #1
	ldrsb r3, [r1, r3]
	adds r0, #1
	adds r2, #1
	cmp r0, r3
	blt .L_080bf030
	b .L_080bf0f8
.L_080bf044:
	adds r3, #220
	movs r4, #128
	lsls r3, r3, #24
	lsls r4, r4, #19
	cmp r3, r4
	bhi .L_080bf0b4
	ldrb r3, [r7, #3]
	subs r3, #36
	cmp r3, #4
	bhi .L_080bf084
	ldr r2, [pc, #400]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.4byte 0x080bf074
	.4byte 0x080bf078
	.4byte 0x080bf07c
	.4byte 0x080bf080
	.4byte 0x080bf084
	movs r5, #63
	b .L_080bf086
	movs r5, #31
	b .L_080bf086
	movs r5, #15
	b .L_080bf086
	movs r5, #7
	b .L_080bf086
.L_080bf084:
	movs r5, #3
.L_080bf086:
	bl sub_080771a0
	ands r0, r5
	cmp r0, #0
	bne .L_080bf0f8
	ldr r1, [sp, #8]
	ldr r2, [r1, #0]
	movs r3, #1
	ldrsb r3, [r2, r3]
	movs r0, #0
	cmp r0, r3
	bge .L_080bf0f8
	adds r1, r2, #0
	movs r4, #2
	adds r2, #44
.L_080bf0a4:
	strb r4, [r2, #0]
	movs r3, #1
	ldrsb r3, [r1, r3]
	adds r0, #1
	adds r2, #1
	cmp r0, r3
	blt .L_080bf0a4
	b .L_080bf0f8
.L_080bf0b4:
	mov r2, r11
	cmp r2, #178
	bne .L_080bf0f8
	ldr r5, [sp, #8]
	ldr r3, [r5, #0]
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r6, #0
	cmp r6, r3
	bge .L_080bf0f8
.L_080bf0ca:
	mov r4, r10
	ldr r3, [r4, #0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldr r3, [r5, #0]
	adds r3, #2
	ldrb r1, [r3, r6]
	ldrb r2, [r7, #2]
	ldrb r3, [r7, #3]
	movs r4, #100
	str r4, [sp, #0]
	bl sub_08077178
	ldr r1, [r5, #0]
	adds r2, r6, #0
	adds r3, r1, #2
	adds r2, #56
	strb r0, [r3, r2]
	movs r3, #1
	ldrsb r3, [r1, r3]
	adds r6, #1
	cmp r6, r3
	blt .L_080bf0ca
.L_080bf0f8:
	ldr r2, [pc, #244]
	cmp r11, r2
	bhi .L_080bf11e
	ldr r3, [sp, #8]
	ldr r2, [pc, #240]
	ldr r1, [r3, #0]
	mov r4, r11
	lsls r3, r4, #2
	ldr r2, [r2, r3]
	movs r3, #30
	ldrsb r3, [r1, r3]
	str r2, [r1, #88]
	cmp r3, #1
	ble .L_080bf11e
	lsls r3, r3, #12
	ldr r0, [pc, #224]
	adds r3, r2, r3
	adds r3, r3, r0
	str r3, [r1, #88]
.L_080bf11e:
	ldr r1, [pc, #220]
	cmp r11, r1
	bhi .L_080bf138
	ldr r1, [pc, #216]
	mov r2, r11
	ldrb r3, [r1, r2]
	cmp r3, #0
	beq .L_080bf138
	ldr r3, [sp, #8]
	mov r4, r11
	ldr r2, [r3, #0]
	ldrb r3, [r1, r4]
	b .L_080bf16c
.L_080bf138:
	mov r0, r11
	bl sub_080bd3c8
	cmp r0, #0
	beq .L_080bf14a
	ldr r0, [sp, #8]
	ldr r2, [r0, #0]
	movs r3, #3
	b .L_080bf16c
.L_080bf14a:
	ldr r1, [sp, #8]
	ldr r2, [r1, #0]
	ldr r3, [r2, #88]
	cmp r3, #0
	beq .L_080bf16a
	ldr r4, [sp, #12]
	ldr r0, [pc, #172]
	ldr r3, [r4, #0]
	adds r3, r3, r0
	ldrb r3, [r3, #0]
	cmp r3, #0
	bne .L_080bf166
	movs r3, #8
	b .L_080bf16c
.L_080bf166:
	movs r3, #3
	b .L_080bf16c
.L_080bf16a:
	movs r3, #1
.L_080bf16c:
	str r3, [r2, #84]
	ldrb r0, [r7, #3]
	bl sub_080772b8
	cmp r0, #0
	beq .L_080bf186
	ldr r1, [sp, #8]
	ldr r3, [r1, #0]
	movs r1, #128
	ldr r2, [r3, #88]
	lsls r1, r1, #9
	orrs r2, r1
	str r2, [r3, #88]
.L_080bf186:
	mov r2, r11
	cmp r2, #178
	bne .L_080bf1a8
	ldr r3, [sp, #8]
	ldr r1, [r3, #0]
	adds r3, r1, #0
	adds r3, #58
	ldrb r3, [r3, #0]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080bf1a8
	ldr r3, [r1, #88]
	movs r2, #128
	lsls r2, r2, #5
	orrs r3, r2
	str r3, [r1, #88]
.L_080bf1a8:
	mov r4, r10
	ldr r3, [r4, #0]
	movs r0, #6
	ldrsh r3, [r3, r0]
	cmp r3, #2
	bne .L_080bf1c6
	ldr r1, [sp, #8]
	ldr r2, [r1, #0]
	ldr r3, [r2, #84]
	cmp r3, #5
	beq .L_080bf1c6
	cmp r3, #9
	beq .L_080bf1c6
	movs r3, #4
	str r3, [r2, #84]
.L_080bf1c6:
	ldr r2, [sp, #8]
	mov r4, r10
	ldr r3, [r2, #0]
	ldr r2, [r4, #0]
	ldrh r2, [r2, #6]
	adds r3, #72
	strh r2, [r3, #0]
	movs r0, #0
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
