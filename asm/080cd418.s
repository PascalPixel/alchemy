@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
@ 080cd358領域から関数境界で分割。
.syntax unified
	.thumb
	.global Func_080cd418
	.thumb_func
Func_080cd418:
	ldr	r3, [pc, #92]
	ldr	r0, [pc, #96]
	ldr	r2, [r3, #0]
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	ldr	r1, [pc, #92]
	adds	r0, #2
	strh	r3, [r1, #0]
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #4
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	subs	r1, #2
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #4
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #2
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #2
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	subs	r1, #74
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #80
	strh	r3, [r1, #0]
	ldr	r3, [pc, #20]
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r1, #2
	strh	r3, [r1, #0]
	bx	lr
	.4byte 0x03001eec
	.4byte 0x000077bc
	.4byte 0x04000040
	.4byte 0x000077cc
