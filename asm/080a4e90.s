@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080a4e90
	.thumb_func
Func_080a4e90:
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #134
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #4
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #13
	movs	r2, #0
	movs	r3, #17
	bl	Func_080a23f4
	add	sp, #4
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
