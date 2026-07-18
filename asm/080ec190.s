@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080b5040, 0x080b5040
	.set sub_080cd104, 0x080cd104
	.set sub_080d6750, 0x080d6750
	.set sub_080dbb24, 0x080dbb24
	.set sub_080e0524, 0x080e0524
	.set sub_080ec264, 0x080ec264
	.global Region_080ec190
Region_080ec190:
	movs r1, #0
	strh r3, [r2, #0]
	movs r0, #1
	bl sub_080cd104
	ldr r3, [pc, #56]
	ldr r2, [pc, #56]
	mov r7, r8
	strh r3, [r2, #0]
	ldr r0, [r7, #0]
	bl sub_080d6750
	movs r1, #189
	lsls r1, r1, #1
	movs r0, #8
	movs r2, #1
	bl sub_080dbb24
	ldr r2, [pc, #36]
	movs r3, #240
	str r3, [r2, #16]
	movs r0, #1
	bl sub_080030f8
	ldr r1, [pc, #28]
	movs r0, #1
	movs r2, #0
	bl sub_080b5040
	ldr r0, [pc, #24]
	ldr r1, [sp, #64]
	movs r2, #0
	movs r3, #0
	b .L_080ec1e8
	.4byte 0x0000f0f0
	.4byte 0x04000040
	.4byte 0x03001ce0
	.4byte 0x0000003d
	.4byte 0x00000073
.L_080ec1e8:
	bl sub_080e0524
	ldr r0, [pc, #92]
	mov r1, r11
	movs r2, #1
	movs r3, #1
	bl sub_080e0524
	ldr r1, [pc, #84]
	ldr r0, [pc, #88]
	add r1, r11
	movs r2, #0
	movs r3, #0
	bl sub_080e0524
	ldr r3, [pc, #52]
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2, #0]
	ldr r3, [pc, #48]
	adds r2, #32
	strh r3, [r2, #0]
	ldr r3, [pc, #44]
	adds r2, #50
	strh r3, [r2, #0]
	ldr r3, [pc, #44]
	subs r2, #2
	strh r3, [r2, #0]
	ldr r4, [pc, #52]
	ldr r3, [pc, #56]
	movs r1, #140
	movs r2, #200
	movs r0, #1
	lsls r1, r1, #17
	lsls r2, r2, #16
	str r0, [sp, #60]
	str r1, [sp, #56]
	str r2, [sp, #52]
	str r3, [sp, #48]
	str r4, [sp, #44]
	ldr r3, [pc, #36]
	b sub_080ec264
