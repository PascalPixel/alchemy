@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080e19b2, 0x080e19b2
	.set sub_080e1acc, 0x080e1acc
	.set sub_080e1ae2, 0x080e1ae2
	.set sub_080e1b8e, 0x080e1b8e
	.set sub_080ed408, 0x080ed408
	.global Region_080e1a48
Region_080e1a48:
	ldr r1, [sp, #40]
	ldr r2, [sp, #36]
	ldr r3, [sp, #32]
	movs r4, #1
	add r9, r4
	adds r1, #24
	adds r2, #24
	subs r3, #48
	mov r5, r9
	str r1, [sp, #40]
	str r2, [sp, #36]
	str r3, [sp, #32]
	cmp r5, #3
	bne sub_080e19b2
	ldr r0, [sp, #116]
	cmp r0, #179
	ble .L_080e1a6c
	b sub_080e1b8e
.L_080e1a6c:
	movs r5, #0
	cmp r0, #155
	ble .L_080e1a76
	adds r5, r0, #0
	subs r5, #156
.L_080e1a76:
	cmp r5, #7
	ble .L_080e1a7c
	movs r5, #7
.L_080e1a7c:
	ldr r1, [sp, #116]
	cmp r1, #139
	bgt sub_080e1acc
	movs r3, #3
	movs r2, #7
	movs r0, #47
	movs r1, #7
	str r3, [sp, #0]
	bl sub_080ed408
	ldr r2, [pc, #52]
	ldr r2, [r2, #0]
	str r2, [sp, #88]
	b sub_080e1ae2
