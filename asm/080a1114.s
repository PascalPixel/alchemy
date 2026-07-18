@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080a1114
	.thumb_func
Func_080a1114:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L0
	bl	Func_08015018
	movs	r3, #0
	str	r3, [r5, #0]
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
