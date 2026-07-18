@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Region_080f9c44
Region_080f9c44:
	ldr r0, [pc, #680]
	ldr r0, [r0, #0]
	ldr r2, [pc, #680]
	ldr r3, [r0, #0]
	subs r3, r3, r2
	cmp r3, #1
	bhi .L_080f9c84
	ldrb r1, [r0, #4]
	subs r1, #1
	strb r1, [r0, #4]
	bgt .L_080f9c84
	ldrb r1, [r0, #11]
	strb r1, [r0, #4]
	ldr r2, [pc, #40]
	ldr r1, [r2, #8]
	lsls r1, r1, #7
	bcc .L_080f9c6a
	ldr r1, [pc, #36]
	str r1, [r2, #8]
.L_080f9c6a:
	ldr r1, [r2, #20]
	lsls r1, r1, #7
	bcc .L_080f9c74
	ldr r1, [pc, #24]
	str r1, [r2, #20]
.L_080f9c74:
	movs r1, #4
	lsls r1, r1, #8
	strh r1, [r2, #10]
	strh r1, [r2, #22]
	movs r1, #182
	lsls r1, r1, #8
	strh r1, [r2, #10]
	strh r1, [r2, #22]
.L_080f9c84:
	bx lr
