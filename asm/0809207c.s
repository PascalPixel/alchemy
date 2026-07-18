@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_0809207c
	.thumb_func
Func_0809207c:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	Func_0808ba1c
	cmp	r0, #0
	beq.n	.L0
	adds	r1, r0, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r1, r5, #0
	bl	Func_08093a6c
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
