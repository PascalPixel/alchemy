@ 再構築サム逆アセンブル。この関数は
@ インラインアセンブリによるレジスタ固定でしか一致せず、C内アセンブリは禁止のため、
@ アセンブリなしのC一致が得られるまでここに置く。build_asm.pyで
@ 非公開ROMとの一致を再確認する。
.syntax unified
	.thumb
	.global Func_080a5fe0
	.thumb_func
Func_080a5fe0:
	push	{r5, lr}
	ldr	r3, [pc, #64]
	movs	r2, #188
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #56]
	ands	r0, r3
	bl	Func_08077080
	adds	r5, r0, #0
	ldrb	r0, [r5, #12]
	bl	Func_0808a488
	cmp	r0, #0
	beq.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	ldrb	r3, [r5, #8]
	movs	r0, #2
	cmp	r3, #255
	beq.n	.L1
	ldrb	r3, [r5, #0]
	movs	r2, #2
	eors	r3, r2
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	movs	r3, #1
	subs	r0, r3, r0
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x00003fff
