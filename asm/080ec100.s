@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_0800307c, 0x0800307c
	.set sub_080041d8, 0x080041d8
	.set sub_080c9048, 0x080c9048
	.set sub_080cd594, 0x080cd594
	.set sub_080ec190, 0x080ec190
	.global Region_080ec100
Region_080ec100:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, [pc, #104]
	ldr r1, [r6, #0]
	sub sp, #116
	str r1, [sp, #68]
	subs r3, r6, #4
	ldr r3, [r3, #0]
	mov r11, r3
	ldr r3, [pc, #92]
	ldr r2, [r6, #4]
	add r3, r11
	str r2, [sp, #64]
	str r0, [r3, #0]
	movs r0, #0
	mov r8, r3
	bl sub_080cd594
	bl sub_080c9048
	ldr r2, [pc, #60]
	movs r3, #160
	lsls r3, r3, #19
	movs r5, #144
	strh r2, [r3, #0]
	lsls r5, r5, #3
	adds r3, #2
	strh r2, [r3, #0]
	adds r1, r5, #0
	ldr r0, [pc, #56]
	bl sub_080041d8
	ldr r2, [pc, #56]
	movs r0, #2
	movs r1, #0
	bl sub_0800307c
	movs r4, #239
	lsls r4, r4, #7
	add r4, r11
	movs r3, #0
	str r3, [r4, #0]
	adds r1, r5, #0
	ldr r0, [pc, #36]
	mov r10, r4
	bl sub_080041d8
	ldr r2, [pc, #32]
	ldr r3, [pc, #8]
	b sub_080ec190
