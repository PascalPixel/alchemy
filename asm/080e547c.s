@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080e46f0, 0x080e46f0
	.set sub_080e551a, 0x080e551a
	.global Region_080e547c
Region_080e547c:
	ldr r0, [pc, #92]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #88]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #84]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #80]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #76]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #72]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #68]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #64]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #60]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #56]
	bl sub_080e46f0
	b sub_080e551a
