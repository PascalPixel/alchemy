@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080b8808
	.thumb_func
Func_080b8808:
	push	{lr}
	cmp	r0, #7
	bhi.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	adds	r3, r0, #0
	subs	r3, #128
	movs	r0, #0
	cmp	r3, #5
	bls.n	.L1
	movs	r0, #1
	negs	r0, r0
.L1:
	pop	{r1}
	bx	r1
