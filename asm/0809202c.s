@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_0809202c
	.thumb_func
Func_0809202c:
	push	{lr}
	ldr	r3, [pc, #28]
	ldr	r2, [pc, #28]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L0
	bl	Func_080f9010
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x00000cc8
