@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080fb75c
	.thumb_func
Func_080fb75c:
	ldr	r0, [r1, #64]
	ldrb	r2, [r0, #0]
	strb	r2, [r1, #31]
	adds	r0, #1
	str	r0, [r1, #64]
	bx	lr
