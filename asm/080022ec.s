@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Region_080022ec
Region_080022ec:
	ldr r3, [pc, #0]
	bx r3
	.4byte 0x03000380
	ldr r3, [pc, #0]
	bx r3
	.4byte 0x030003f0
	ldr r3, [pc, #0]
	bx r3
	.4byte 0x030003ac
	ldr r3, [pc, #0]
	bx r3
	.4byte 0x030003e0
.L_0800230c:
	lsrs r2, r0, #14
	movs r0, #1
	ands r0, r2
	lsls r0, r0, #16
	lsrs r2, r2, #2
	bcc .L_0800231a
	negs r0, r0
.L_0800231a:
	bx lr
	movs r1, #64
	lsls r1, r1, #8
	adds r0, r0, r1
	adds r0, #32
	lsls r1, r0, #18
	lsrs r1, r1, #24
	beq .L_0800230c
	lsrs r2, r0, #15
	bcc .L_08002334
	movs r3, #128
	lsls r3, r3, #1
	subs r1, r3, r1
.L_08002334:
	lsls r1, r1, #1
	add r3, pc, #12
	ldrh r0, [r3, r1]
	lsrs r2, r2, #1
	bcc .L_08002342
	negs r0, r0
	bx lr
.L_08002342:
	bx lr
