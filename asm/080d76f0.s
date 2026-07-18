@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080b5040, 0x080b5040
	.set sub_080d6750, 0x080d6750
	.set sub_080d77b4, 0x080d77b4
	.set sub_080dbb24, 0x080dbb24
	.set sub_080e0524, 0x080e0524
	.set sub_080ed408, 0x080ed408
	.global Region_080d76f0
Region_080d76f0:
	ldr r1, [sp, #44]
	ldr r0, [pc, #168]
	movs r2, #1
	movs r3, #1
	bl sub_080e0524
	ldr r0, [pc, #160]
	ldr r1, [sp, #28]
	movs r2, #0
	movs r3, #0
	bl sub_080e0524
	movs r6, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r6, [sp, #0]
	bl sub_080ed408
	ldr r2, [r5, #24]
	ldr r3, [sp, #24]
	str r2, [sp, #36]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	bl sub_080ed408
	ldr r5, [r5, #28]
	ldr r2, [pc, #116]
	str r5, [sp, #32]
	movs r3, #240
	mov r4, r8
	str r3, [r2, #16]
	ldr r0, [r4, #0]
	bl sub_080d6750
	movs r0, #1
	bl sub_080030f8
	ldr r1, [pc, #96]
	movs r0, #1
	movs r2, #0
	bl sub_080b5040
	movs r1, #186
	lsls r1, r1, #1
	movs r0, #9
	movs r2, #1
	bl sub_080dbb24
	ldr r3, [pc, #48]
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2, #0]
	ldr r3, [pc, #44]
	adds r2, #32
	strh r3, [r2, #0]
	ldr r3, [pc, #40]
	adds r2, #50
	strh r3, [r2, #0]
	ldr r3, [pc, #40]
	subs r2, #2
	mov r5, r10
	strh r3, [r2, #0]
	str r6, [r5, #0]
	ldr r7, [sp, #44]
	ldr r0, [pc, #48]
	movs r3, #50
	adds r2, r7, r0
	movs r1, #0
	str r3, [r2, #0]
	ldr r5, [pc, #40]
	mov r8, r1
	b sub_080d77b4
