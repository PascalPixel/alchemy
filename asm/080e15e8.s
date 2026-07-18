@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080c9048, 0x080c9048
	.set sub_080cd594, 0x080cd594
	.set sub_080e0524, 0x080e0524
	.set sub_080e1724, 0x080e1724
	.global Region_080e15e8
Region_080e15e8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #108]
	ldr r1, [r3, #0]
	sub sp, #336
	str r1, [sp, #132]
	adds r2, r3, #0
	subs r2, #20
	ldr r2, [r2, #0]
	str r2, [sp, #128]
	adds r2, r3, #0
	subs r2, #16
	ldr r2, [r2, #0]
	str r2, [sp, #124]
	subs r3, #12
	ldr r2, [sp, #128]
	ldr r4, [pc, #84]
	ldr r3, [r3, #0]
	str r3, [sp, #120]
	adds r3, r2, r4
	str r0, [r3, #0]
	movs r0, #128
	lsls r0, r0, #6
	bl sub_080cd594
	ldr r2, [pc, #68]
	ldr r3, [pc, #56]
	ldr r5, [pc, #68]
	strh r3, [r2, #0]
	ldr r1, [sp, #128]
	adds r0, r5, #0
	movs r2, #1
	movs r3, #1
	bl sub_080e0524
	adds r0, r5, #0
	ldr r1, [sp, #128]
	movs r2, #1
	movs r3, #1
	bl sub_080e0524
	movs r2, #192
	ldr r5, [sp, #128]
	lsls r2, r2, #5
	adds r1, r5, r2
	ldr r0, [pc, #36]
	movs r2, #0
	movs r3, #0
	bl sub_080e0524
	ldr r0, [pc, #32]
	movs r3, #0
	ldr r1, [sp, #120]
	movs r2, #0
	b .L_080e167c
	.4byte 0x00000100
	.4byte 0x03001f00
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x000000bc
	.4byte 0x00000075
	.4byte 0x00000073
.L_080e167c:
	bl sub_080e0524
	ldr r5, [pc, #136]
	movs r3, #0
	mov r9, r3
	movs r4, #0
	movs r0, #64
.L_080e168a:
	ldr r2, [sp, #128]
	movs r1, #0
	adds r3, r4, r2
	ldr r2, [pc, #124]
	mov r8, r1
	ldr r1, [sp, #120]
	mov r12, r0
	adds r3, r3, r2
.L_080e169a:
	ldrb r2, [r1, #0]
	adds r1, #1
	cmp r2, r12
	ble .L_080e16a4
	mov r2, r12
.L_080e16a4:
	cmp r2, #0
	bge .L_080e16aa
	movs r2, #0
.L_080e16aa:
	strb r2, [r3, #0]
	movs r2, #1
	add r8, r2
	adds r3, #1
	cmp r8, r5
	bne .L_080e169a
	add r9, r2
	mov r3, r9
	add r4, r8
	subs r0, #7
	cmp r3, #8
	bne .L_080e168a
	bl sub_080c9048
	ldr r2, [pc, #60]
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3, #0]
	adds r3, #2
	strh r2, [r3, #0]
	ldr r2, [pc, #64]
	ldr r3, [pc, #48]
	strh r3, [r2, #0]
	ldr r4, [sp, #128]
	ldr r5, [pc, #60]
	ldr r0, [pc, #60]
	adds r3, r4, r5
	movs r6, #0
	ldr r2, [pc, #60]
	str r6, [r3, #0]
	movs r1, #2
	adds r3, r4, r0
	str r1, [r3, #0]
	adds r5, #12
	adds r3, r4, r2
	movs r2, #1
	str r2, [r3, #0]
	adds r3, r4, r5
	str r6, [r3, #0]
	ldr r0, [sp, #132]
	movs r5, #144
	lsls r5, r5, #3
	str r2, [r0, #16]
	b sub_080e1724
