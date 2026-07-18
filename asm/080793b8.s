@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080793b8
	.thumb_func
Func_080793b8:
	lsls	r3, r0, #20
	lsrs	r0, r3, #23
	ldr	r3, [pc, #4]
	ldrb	r0, [r3, r0]
	bx	lr
	movs	r0, r0
	.4byte 0x02000040
