@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080a23c0
	.thumb_func
Func_080a23c0:
	push	{r5, lr}
	ldr	r3, [pc, #40]
	sub	sp, #4
	adds	r5, r0, #0
	ldr	r0, [r3, #16]
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r2, r5, #0
	movs	r1, #7
	movs	r3, #8
	bl	Func_080150a8
	ldr	r0, [pc, #20]
	adds	r1, r5, #0
	movs	r2, #64
	movs	r3, #0
	bl	Func_08015080
	add	sp, #4
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x00000b0b
