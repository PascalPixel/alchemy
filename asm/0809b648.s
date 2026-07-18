@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809b648
	.thumb_func
Func_0809b648:
	push	{lr}
	ldr	r1, [pc, #64]
	movs	r3, #145
	lsls	r3, r3, #2
	adds	r2, r1, r3
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #146
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L0
	movs	r0, #150
	movs	r1, #4
	bl	Func_08015120
	ldr	r0, [pc, #32]
	movs	r1, #1
	bl	Func_08015040
	b.n	.L1
.L0:
	movs	r0, #236
	movs	r1, #2
	bl	Func_08015120
	ldr	r0, [pc, #16]
	movs	r1, #1
	bl	Func_08015040
.L1:
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x00000923
	.4byte 0x00000925
