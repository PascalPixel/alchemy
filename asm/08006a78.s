@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Region_08006a78
Region_08006a78:
	ldr r1, [pc, #44]
	ldr r0, [r1, #0]
	movs r2, #0
	strh r2, [r0, #0]
	adds r0, #2
	str r0, [r1, #0]
	strh r2, [r0, #0]
	subs r0, #2
	str r0, [r1, #0]
	ldr r3, [pc, #32]
	strh r2, [r3, #0]
	ldr r2, [pc, #32]
	ldr r0, [pc, #32]
	ldrb r0, [r0, #0]
	movs r1, #8
	lsls r1, r0
	ldrh r0, [r2, #0]
	bics r0, r1
	strh r0, [r2, #0]
	ldr r0, [pc, #24]
	ldrh r0, [r0, #0]
	strh r0, [r3, #0]
	bx lr
	.2byte 0x0000
	.4byte 0x02004c28
	.4byte 0x04000208
	.4byte 0x04000200
	.4byte 0x02004c20
	.4byte 0x02004c2c
	.4byte 0x47707800
	adds r2, r0, #0
	ldr r1, [pc, #20]
	adds r0, r2, #1
	str r0, [r1, #0]
	ldr r3, [pc, #16]
	movs r0, #1
	eors r3, r0
	ldr r0, [pc, #16]
	ldr r1, [pc, #8]
	subs r0, r0, r1
	lsls r0, r0, #15
	b .L_08006af0
	.4byte 0x02004c1c
	.4byte 0x08006abd
	.4byte 0x08006ac1
.L_08006ae4:
	ldrh r0, [r3, #0]
	strh r0, [r2, #0]
	adds r3, #2
	adds r2, #2
	subs r0, r1, #1
	lsls r0, r0, #16
.L_08006af0:
	lsrs r1, r0, #16
	cmp r1, #0
	bne .L_08006ae4
	bx lr
