@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080c23c0
	.thumb_func
Func_080c23c0:
	push	{lr}
	cmp	r0, #171
	bls.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	ldr	r3, [pc, #24]
	lsls	r2, r0, #3
	adds	r2, r2, r3
	ldrb	r3, [r2, #2]
	lsls	r3, r3, #31
	movs	r1, #0
	cmp	r3, #0
	beq.n	.L2
	movs	r1, #1
.L2:
	adds	r0, r1, #0
.L1:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x080c7420
