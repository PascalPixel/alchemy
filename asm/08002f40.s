@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_08002f40
	.thumb_func
Func_08002f40:
	ldr	r3, [pc, #4]
	lsls	r0, r0, #2
	ldr	r0, [r3, r0]
	bx	lr
	.4byte 0x08320000
