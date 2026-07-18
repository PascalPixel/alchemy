@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080a1804
	.thumb_func
Func_080a1804:
	push	{lr}
	movs	r0, #0
	movs	r2, #0
	bl	Func_080150e0
	pop	{r0}
	bx	r0
