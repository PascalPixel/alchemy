@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080cd594, 0x080cd594
	.set sub_080dda3c, 0x080dda3c
	.set sub_080e0524, 0x080e0524
	.global Region_080dd9c0
Region_080dd9c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r7, [pc, #84]
	adds r3, r7, #0
	ldmia r3!, {r1}
	ldr r6, [pc, #80]
	mov r9, r1
	ldr r3, [r3, #0]
	sub sp, #52
	add r6, r9
	str r3, [sp, #48]
	str r0, [r6, #0]
	movs r0, #1
	bl sub_080cd594
	ldr r2, [pc, #64]
	ldr r3, [pc, #44]
	strh r3, [r2, #0]
	ldr r3, [pc, #44]
	adds r2, #48
	strh r3, [r2, #0]
	ldr r3, [pc, #40]
	adds r2, #2
	strh r3, [r2, #0]
	ldr r0, [pc, #52]
	movs r3, #1
	mov r1, r9
	movs r2, #1
	bl sub_080e0524
	ldr r3, [r6, #0]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne sub_080dda3c
	ldr r2, [pc, #36]
	ldr r3, [pc, #36]
	str r3, [r2, #0]
	b sub_080dda3c
