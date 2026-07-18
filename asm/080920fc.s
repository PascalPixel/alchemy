@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080920fc
	.thumb_func
Func_080920fc:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	bl	Func_0808ba1c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	adds	r1, r5, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r1, r6, #0
	bl	Func_08093a6c
	adds	r0, r5, #0
	bl	Func_08009148
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
