@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_08017394
	.thumb_func
Func_08017394:
	push	{lr}
	ldrh	r3, [r0, #22]
	cmp	r3, #0
	bne.n	.L0
	movs	r2, #26
	ldrsh	r3, [r0, r2]
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L1
.L0:
	movs	r0, #0
.L1:
	pop	{r1}
	bx	r1
