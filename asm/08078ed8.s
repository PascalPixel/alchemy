@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_08078ed8
	.thumb_func
Func_08078ed8:
	movs	r3, #180
	muls	r0, r3
	ldr	r2, [pc, #4]
	adds	r0, r0, r2
	bx	lr
	movs	r0, r0
	.4byte 0x080844ec
