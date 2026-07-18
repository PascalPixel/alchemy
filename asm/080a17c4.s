@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a17c4
	.thumb_func
Func_080a17c4:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #1
	strb	r3, [r0, #5]
	ldr	r2, [pc, #44]
	ldrh	r3, [r0, #6]
	ldrh	r1, [r0, #22]
	ands	r2, r3
	ldr	r3, [pc, #40]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #22]
	ldrh	r3, [r0, #8]
	ldrb	r2, [r0, #23]
	strb	r3, [r0, #20]
	movs	r3, #63
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r0, #23]
	ldrb	r2, [r0, #21]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r0, #21]
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
