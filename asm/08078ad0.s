@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_08078ad0
	.thumb_func
Func_08078ad0:
	push	{lr}
	ldr	r3, [pc, #28]
	ldr	r2, [pc, #28]
	ands	r3, r0
	ldrb	r0, [r2, r3]
	movs	r4, #0
	cmp	r0, #0
	beq.n	.L0
	subs	r0, #1
	bl	Func_08078aa0
	adds	r4, r0, #0
.L0:
	adds	r0, r4, #0
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x0807b490
