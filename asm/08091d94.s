@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_08091d94
	.thumb_func
Func_08091d94:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r0, #0
	bl	Func_08077088
	movs	r0, #83
	bl	Func_080f9010
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	adds	r0, r6, #0
	movs	r1, #4
	bl	Func_08015120
	ldr	r0, [pc, #12]
	movs	r1, #3
	bl	Func_08015040
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000001e
