@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.pyでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_080f9a9a
	.thumb_func
Func_080f9a9a:
	push	{r0}
	lsrs	r0, r2, #25
	bne.n	.L0
	ldr	r0, [pc, #12]
	cmp	r2, r0
	bcc.n	.L1
	lsrs	r0, r2, #14
	beq.n	.L0
.L1:
	movs	r3, #0
.L0:
	pop	{r0}
	bx	lr
	.4byte 0x080fb7a0
