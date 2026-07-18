@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080c2384
	.thumb_func
Func_080c2384:
	push	{lr}
	cmp	r0, #171
	bls.n	.L0
	ldr	r3, [pc, #16]
	ldrh	r0, [r3, #0]
	b.n	.L1
.L0:
	ldr	r3, [pc, #8]
	lsls	r2, r0, #3
	ldrh	r0, [r3, r2]
.L1:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x080c7420
