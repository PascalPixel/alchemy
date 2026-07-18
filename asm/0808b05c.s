@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_0808b05c
	.thumb_func
Func_0808b05c:
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldr	r2, [pc, #8]
	lsls	r3, r3, #1
	adds	r3, #4
	ldrh	r0, [r2, r3]
	bx	lr
	movs	r0, r0
	.4byte 0x0809c610
