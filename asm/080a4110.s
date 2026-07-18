@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080a4110
	.thumb_func
Func_080a4110:
	push	{lr}
	cmp	r0, #2
	bgt.n	.L0
	cmp	r1, #2
	bgt.n	.L0
	cmp	r0, #0
	blt.n	.L0
	cmp	r1, #0
	bge.n	.L1
.L0:
	movs	r0, #0
	b.n	.L2
.L1:
	lsls	r2, r1, #1
	adds	r2, r2, r1
	adds	r2, r2, r0
	ldr	r3, [pc, #8]
	lsls	r2, r2, #2
	ldr	r0, [r3, r2]
.L2:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x080af2e4
