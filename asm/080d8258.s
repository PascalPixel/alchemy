@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_08004278, 0x08004278
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d7f08, 0x080d7f08
	.set sub_080e155c, 0x080e155c
	.global Region_080d8258
Region_080d8258:
	movs r0, #2
	movs r1, #2
	bl sub_080e155c
	ldr r3, [sp, #44]
	ldr r4, [pc, #68]
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2, #0]
	movs r0, #1
	bl sub_080030f8
	ldr r7, [sp, #40]
	movs r5, #14
	adds r7, #1
	add r9, r5
	str r7, [sp, #40]
	cmp r7, #146
	beq .L_080d8280
	b sub_080d7f08
.L_080d8280:
	movs r0, #47
	bl sub_08002dd8
	movs r0, #46
	bl sub_08002dd8
	ldr r0, [pc, #28]
	bl sub_08004278
	bl sub_080cdbc0
	add sp, #332
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
