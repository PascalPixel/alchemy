@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_0808b9f8
	.thumb_func
Func_0808b9f8:
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r0, #140
	ldr	r3, [r3, #0]
	lsls	r0, r0, #1
	movs	r1, #0
	movs	r2, #65
	adds	r3, r3, r0
.L0:
	subs	r2, #1
	str	r1, [r3, #0]
	subs	r3, #4
	cmp	r2, #0
	bge.n	.L0
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
