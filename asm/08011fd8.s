@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08011fd8
	.thumb_func
Func_08011fd8:
	push	{r5, r6, lr}
	ldr	r3, [pc, #80]
	ldr	r5, [r3, #0]
	adds	r4, r2, #0
	adds	r6, r0, #0
	asrs	r1, r1, #16
	asrs	r4, r4, #16
	ldr	r0, [pc, #72]
	cmp	r5, #0
	beq.n	.L0
	movs	r2, #3
	ands	r2, r6
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r3, r3, #4
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r5, r3]
.L0:
	cmp	r1, #0
	bge.n	.L1
	adds	r1, #15
.L1:
	adds	r2, r4, #0
	asrs	r1, r1, #4
	cmp	r2, #0
	bge.n	.L2
	adds	r2, #15
.L2:
	asrs	r3, r2, #4
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r0, r0, r3
	ldrb	r3, [r0, #3]
	ldr	r2, [pc, #24]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	movs	r0, #15
	ands	r0, r3
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001e70
	.4byte 0x02010000
	.4byte 0x0202c000
