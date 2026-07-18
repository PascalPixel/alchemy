@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080040d0
	.thumb_func
Func_080040d0:
	push	{lr}
	adds	r2, r1, #0
	ldr	r1, [pc, #12]
	lsls	r3, r0, #2
	ldrh	r1, [r1, r3]
	bl	Func_08003fa4
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001b10
