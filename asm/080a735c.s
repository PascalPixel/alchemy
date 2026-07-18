@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080a735c
	.thumb_func
Func_080a735c:
	push	{lr}
	lsls	r0, r0, #18
	lsrs	r0, r0, #18
	bl	Func_08077080
	ldrb	r3, [r0, #12]
	cmp	r3, #0
	bne.n	.L0
	ldrb	r2, [r0, #1]
	movs	r3, #192
	ands	r3, r2
	movs	r0, #1
	cmp	r3, #192
	bne.n	.L1
.L0:
	movs	r0, #0
.L1:
	pop	{r1}
	bx	r1
