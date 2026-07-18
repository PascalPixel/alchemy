@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801f730
	.thumb_func
Func_0801f730:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	Func_080056cc
	movs	r5, #9
	negs	r5, r5
	cmp	r0, #0
	bne.n	.L0
	bl	Func_08005c68
	adds	r5, r0, #0
	cmp	r6, #0
	beq.n	.L0
	ldr	r3, [pc, #40]
	ldr	r1, [pc, #40]
	ldr	r3, [r3, #0]
	adds	r2, r3, r1
	movs	r1, #2
.L2:
	ldrb	r3, [r2, #0]
	lsls	r3, r3, #24
	adds	r2, #64
	cmp	r3, #0
	beq.n	.L1
	subs	r5, #1
.L1:
	subs	r1, #1
	cmp	r1, #0
	bge.n	.L2
.L0:
	bl	Func_08005cf8
	adds	r0, r5, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f1c
	.4byte 0x00001071
