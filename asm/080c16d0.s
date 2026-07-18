@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c16d0
	.thumb_func
Func_080c16d0:
	push	{lr}
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r3, [pc, #56]
	ldr	r0, [pc, #56]
	bl	Func_080072f0
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	movs	r0, #40
	bl	Func_08002dd8
	movs	r0, #39
	bl	Func_08002dd8
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r0, [pc, #24]
	bl	Func_08004278
	ldr	r0, [pc, #20]
	bl	Func_08004278
	b.n	.L0
	.4byte 0x00001341
	.4byte 0x03000164
	.4byte 0x06004000
	.4byte 0x080c1439
	.4byte 0x080c11ed
.L0:
	pop	{r1}
	bx	r1
