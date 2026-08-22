@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b0a20
	.thumb_func
Func_080b0a20:
	push	{r5, r6, lr}
	ldr	r5, [r0, #0]
	ldr	r6, [pc, #44]
	movs	r3, #1
	ldr	r4, [pc, #44]
	strb	r3, [r0, #13]
	ldr	r3, [pc, #44]
	strh	r1, [r5, #6]
	strh	r1, [r0, #8]
	strh	r1, [r0, #4]
	ands	r1, r6
	ands	r1, r3
	strb	r4, [r0, #12]
	ldr	r3, [pc, #36]
	ldrh	r4, [r5, #22]
	ands	r3, r4
	orrs	r3, r1
	strh	r3, [r5, #22]
	ldr	r3, [r0, #0]
	strh	r2, [r0, #10]
	strh	r2, [r0, #6]
	strh	r2, [r3, #8]
	ands	r2, r6
	strb	r2, [r3, #20]
	b.n	.L0
	movs	r0, r0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0xfffffe00
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
