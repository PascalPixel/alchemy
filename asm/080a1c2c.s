@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.tsで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080a1c2c
	.thumb_func
Func_080a1c2c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	mov	r8, r2
	adds	r7, r3, #0
	cmp	r6, #31
	ble.n	.L0
	movs	r6, #0
.L0:
	ldr	r1, [sp, #20]
	ldr	r5, [r0, #0]
	adds	r0, r6, #0
	bl	Func_080022ec
	lsls	r0, r0, #4
	adds	r0, r0, r7
	strh	r0, [r5, #8]
	ldr	r1, [sp, #20]
	adds	r0, r6, #0
	bl	Func_080022fc
	lsls	r0, r0, #4
	add	r0, r8
	strh	r0, [r5, #6]
	adds	r0, r5, #0
	bl	Func_080a17c4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
