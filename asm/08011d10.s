@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_08011d10
	.thumb_func
Func_08011d10:
	push	{lr}
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	lsls	r1, r3, #19
	movs	r3, #1
	ldrsb	r3, [r0, r3]
	lsls	r3, r3, #19
	subs	r3, r3, r1
	adds	r0, r2, #0
	muls	r0, r3
	cmp	r0, #0
	bge.n	.L0
	adds	r0, #15
.L0:
	asrs	r0, r0, #4
	adds	r0, r1, r0
	pop	{r1}
	bx	r1
