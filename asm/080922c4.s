@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080922c4
	.thumb_func
Func_080922c4:
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
	movs	r1, #2
	bl	Func_08009080
	ldr	r1, [r5, #8]
	lsls	r3, r6, #16
	adds	r1, r1, r3
	ldr	r3, [r5, #16]
	lsls	r0, r7, #16
	adds	r3, r3, r0
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	bl	Func_08009150
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
