@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_08092158
	.thumb_func
Func_08092158:
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r7, r2, #0
	bl	Func_0808ba1c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	adds	r2, r5, #0
	adds	r2, #91
	movs	r3, #0
	strb	r3, [r2, #0]
	bl	Func_08009140
	adds	r0, r5, #0
	lsls	r1, r6, #16
	ldr	r2, [r5, #12]
	lsls	r3, r7, #16
	bl	Func_08009150
	adds	r0, r5, #0
	bl	Func_08009158
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
