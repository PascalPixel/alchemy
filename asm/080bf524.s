@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080bf524
	.thumb_func
Func_080bf524:
	push	{lr}
	bl	Func_08077008
	movs	r3, #159
	lsls	r3, r3, #1
	adds	r1, r0, r3
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r3, #255
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L1
.L0:
	movs	r0, #0
.L1:
	pop	{r1}
	bx	r1
