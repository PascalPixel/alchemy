@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_08021bc8
	.thumb_func
Func_08021bc8:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	movs	r0, #0
.L0:
	ldr	r3, [pc, #8]
	lsls	r2, r0, #2
	ldr	r0, [r3, r2]
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x08073968
