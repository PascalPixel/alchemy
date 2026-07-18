@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080ab1f4
	.thumb_func
Func_080ab1f4:
	push	{r5, r6, lr}
	adds	r4, r0, #0
	ldrh	r0, [r4, #12]
	adds	r0, r0, r1
	ldrh	r1, [r4, #14]
	sub	sp, #4
	ldr	r5, [sp, #20]
	adds	r6, r3, #0
	adds	r1, r1, r2
	adds	r0, #1
	ldr	r3, [sp, #16]
	adds	r1, #1
	adds	r2, r6, #0
	str	r5, [sp, #0]
	bl	Func_080153c8
	add	sp, #4
	pop	{r5, r6}
	pop	{r1}
	bx	r1
