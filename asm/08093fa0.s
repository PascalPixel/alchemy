@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08009080, 0x08009080
	.set sub_08009150, 0x08009150
	.set sub_080091e0, 0x080091e0
	.set sub_08009220, 0x08009220
	.set sub_0809163c, 0x0809163c
	.set sub_080916b0, 0x080916b0
	.set sub_08091750, 0x08091750
	.set sub_08092054, 0x08092054
	.set sub_08092158, 0x08092158
	.set sub_080923c4, 0x080923c4
	.global Region_08093fa0
Region_08093fa0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, [pc, #368]
	ldr r0, [pc, #368]
	mov r9, r0
	ldr r0, [r1, #0]
	sub sp, #24
	bl sub_08092054
	adds r7, r0, #0
	movs r3, #10
	ldrsh r5, [r7, r3]
	movs r1, #18
	ldrsh r6, [r7, r1]
	ldr r3, [pc, #352]
	movs r2, #1
	ands r5, r3
	ands r6, r3
	mov r11, r2
	movs r0, #8
	movs r2, #8
	adds r0, r0, r5
	adds r2, r2, r6
	mov r8, r0
	mov r10, r2
	bl sub_080916b0
	adds r3, r7, #0
	adds r3, #84
	ldrb r3, [r3, #0]
	cmp r3, #1
	bne .L_08093ff2
	ldr r3, [r7, #80]
	adds r3, #38
	ldrb r3, [r3, #0]
	mov r11, r3
.L_08093ff2:
	movs r3, #249
	lsls r3, r3, #1
	add r9, r3
	mov r0, r9
	ldrb r3, [r0, #0]
	cmp r3, #0
	bne .L_080940b8
	mov r3, r8
	cmp r3, #0
	bge .L_0809400a
	adds r3, r5, #0
	adds r3, #23
.L_0809400a:
	asrs r2, r3, #4
	mov r3, r10
	cmp r3, #0
	bge .L_08094016
	adds r3, r6, #0
	adds r3, #23
.L_08094016:
	asrs r3, r3, #4
	lsls r3, r3, #7
	adds r3, r2, r3
	ldr r1, [pc, #268]
	ldr r0, [pc, #272]
	lsls r3, r3, #2
	adds r2, r3, r1
	adds r3, r3, r0
	ldrb r2, [r2, #2]
	ldrb r3, [r3, #2]
	cmp r2, r3
	beq .L_08094030
	b .L_08094138
.L_08094030:
	ldr r3, [r7, #8]
	mov r0, sp
	str r3, [r0, #0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	bl sub_08009220
	adds r5, r0, #0
	cmp r5, #0
	bne .L_08094138
	adds r6, r7, #0
	adds r6, #90
	ldr r1, [pc, #208]
	strb r5, [r6, #0]
	mov r2, r10
	ldr r0, [r1, #0]
	mov r1, r8
	bl sub_08092158
	movs r1, #6
	adds r0, r7, #0
	bl sub_08009080
	movs r0, #4
	bl sub_080030f8
	movs r1, #7
	adds r0, r7, #0
	bl sub_08009080
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #40]
	movs r0, #4
	bl sub_080030f8
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3, #0]
	mov r2, r11
	movs r3, #254
	ands r2, r3
	mov r11, r2
	adds r0, r7, #0
	mov r1, r11
	bl sub_080091e0
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	adds r0, r7, #0
	movs r1, #12
	str r5, [r7, #40]
	bl sub_08009080
	movs r0, #4
	bl sub_080030f8
	movs r3, #1
	mov r0, r9
	strb r3, [r0, #0]
	strb r3, [r6, #0]
	movs r0, #8
	bl sub_080030f8
	b .L_08094112
.L_080940b8:
	adds r5, r7, #0
	adds r5, #85
	movs r6, #0
	strb r6, [r5, #0]
	adds r0, r7, #0
	movs r1, #11
	bl sub_08009080
	mov r2, r8
	lsls r1, r2, #16
	movs r3, #128
	ldr r2, [r7, #12]
	lsls r3, r3, #12
	mov r0, r10
	adds r2, r2, r3
	lsls r3, r0, #16
	ldr r0, [pc, #88]
	adds r3, r3, r0
	adds r0, r7, #0
	bl sub_08009150
	ldr r1, [pc, #60]
	ldr r0, [r1, #0]
	bl sub_080923c4
	movs r3, #3
	strb r3, [r5, #0]
	ldr r5, [pc, #44]
	mov r2, r11
	ldr r3, [r7, #12]
	orrs r2, r5
	mov r11, r2
	str r3, [r7, #20]
	adds r0, r7, #0
	mov r1, r11
	bl sub_080091e0
	movs r0, #4
	bl sub_0809163c
	mov r3, r9
	strb r6, [r3, #0]
	adds r3, r7, #0
	adds r3, #90
	strb r5, [r3, #0]
.L_08094112:
	bl sub_08091750
	movs r0, #0
	b .L_08094140
	.2byte 0x0000
	.4byte 0x00000001
	.4byte 0x02000434
	.4byte 0x02000240
	.4byte 0x0000fff0
	.4byte 0x02010000
	.4byte 0x0200fe00
	.4byte 0xfff00000
.L_08094138:
	bl sub_08091750
	movs r0, #1
	negs r0, r0
.L_08094140:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
