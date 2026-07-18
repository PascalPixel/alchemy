@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080041d8, 0x080041d8
	.set sub_080b5040, 0x080b5040
	.set sub_080cd104, 0x080cd104
	.set sub_080e17c4, 0x080e17c4
	.set sub_080ed408, 0x080ed408
	.global Region_080e1724
Region_080e1724:
	mov r10, r1
	ldr r0, [pc, #132]
	adds r1, r5, #0
	bl sub_080041d8
	adds r1, r5, #0
	ldr r0, [pc, #124]
	bl sub_080041d8
	movs r2, #239
	ldr r1, [sp, #128]
	lsls r2, r2, #7
	adds r1, r1, r2
	str r6, [r1, #0]
	mov r8, r1
	movs r0, #0
	movs r1, #0
	bl sub_080cd104
	ldr r1, [pc, #104]
	movs r0, #1
	movs r2, #0
	bl sub_080b5040
	ldr r2, [pc, #96]
	movs r3, #240
	str r3, [r2, #16]
	movs r0, #0
	movs r1, #1
	bl sub_080cd104
	ldr r6, [pc, #88]
	ldr r5, [pc, #52]
	movs r3, #3
	strh r5, [r6, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #46
	str r3, [sp, #0]
	bl sub_080ed408
	ldr r3, [pc, #72]
	adds r3, #184
	ldr r3, [r3, #0]
	movs r2, #128
	str r3, [sp, #108]
	ldr r3, [pc, #28]
	lsls r2, r2, #19
	strh r3, [r2, #0]
	ldr r3, [pc, #28]
	adds r2, #82
	strh r5, [r6, #0]
	strh r3, [r2, #0]
	ldr r3, [pc, #24]
	subs r2, #2
	strh r3, [r2, #0]
	mov r4, r8
	mov r3, r10
	str r3, [r4, #0]
	b sub_080e17c4
