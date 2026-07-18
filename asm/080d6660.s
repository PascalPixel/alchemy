@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d6660
	.thumb_func
Func_080d6660:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r1, [pc, #80]
	movs	r0, #41
	bl	Func_080048b0
	ldr	r1, [pc, #76]
	movs	r0, #39
	bl	Func_080048b0
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #40
	bl	Func_080048b0
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #64]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	ldr	r2, [r5, #0]
	str	r5, [r3, #0]
	cmp	r2, #0
	bne.n	.L0
	str	r2, [r5, #24]
	b.n	.L1
.L0:
	ldr	r3, [pc, #52]
	lsls	r2, r2, #2
	subs	r2, #4
	ldr	r3, [r3, r2]
	adds	r0, r5, #0
	bl	Func_080072f0
.L1:
	movs	r0, #40
	bl	Func_08002dd8
	movs	r0, #39
	bl	Func_08002dd8
	movs	r0, #41
	bl	Func_08002dd8
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x00000302
	.4byte 0x0000782c
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x080ee2b4
