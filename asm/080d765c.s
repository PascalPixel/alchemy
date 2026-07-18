@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080041d8, 0x080041d8
	.set sub_080c9048, 0x080c9048
	.set sub_080cd104, 0x080cd104
	.set sub_080cd594, 0x080cd594
	.set sub_080d76f0, 0x080d76f0
	.global Region_080d765c
Region_080d765c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, [pc, #112]
	ldr r1, [r5, #0]
	sub sp, #332
	str r1, [sp, #48]
	subs r3, r5, #4
	ldr r3, [r3, #0]
	str r3, [sp, #44]
	ldr r4, [sp, #44]
	ldr r7, [pc, #100]
	ldr r2, [r5, #4]
	adds r4, r4, r7
	movs r3, #1
	str r2, [sp, #28]
	str r3, [sp, #24]
	str r0, [r4, #0]
	movs r0, #0
	mov r8, r4
	bl sub_080cd594
	bl sub_080c9048
	ldr r2, [pc, #56]
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3, #0]
	adds r3, #2
	strh r2, [r3, #0]
	ldr r0, [sp, #44]
	movs r1, #239
	lsls r1, r1, #7
	adds r0, r0, r1
	movs r3, #0
	movs r1, #144
	str r3, [r0, #0]
	mov r10, r0
	lsls r1, r1, #3
	ldr r0, [pc, #48]
	bl sub_080041d8
	ldr r2, [pc, #44]
	ldr r3, [pc, #24]
	movs r0, #1
	strh r3, [r2, #0]
	movs r1, #0
	bl sub_080cd104
	ldr r2, [pc, #36]
	ldr r3, [pc, #12]
	strh r3, [r2, #0]
	b sub_080d76f0
