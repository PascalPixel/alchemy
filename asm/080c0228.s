@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c0228
	.thumb_func
Func_080c0228:
	push	{r5, lr}
	ldr	r3, [pc, #96]
	ldr	r3, [r3, #0]
	ldr	r0, [r3, #0]
	cmp	r0, #79
	bhi.n	.L0
	movs	r3, #7
	ldr	r2, [pc, #88]
	ands	r3, r0
	adds	r4, r3, r2
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L1
	adds	r3, r0, #7
.L1:
	asrs	r3, r3, #3
	movs	r2, #13
	subs	r2, r2, r3
	ldr	r5, [pc, #72]
	lsls	r3, r2, #6
	movs	r1, #0
	adds	r2, r3, r5
.L2:
	adds	r1, #1
	strh	r4, [r2, #0]
	adds	r2, #2
	cmp	r1, #32
	bne.n	.L2
	movs	r3, #128
	lsls	r3, r3, #4
	orrs	r4, r3
	adds	r3, r0, #0
	cmp	r3, #0
	bge.n	.L3
	adds	r3, #7
.L3:
	asrs	r3, r3, #3
	adds	r2, r3, #0
	adds	r2, #13
	cmp	r2, #20
	bhi.n	.L0
	ldr	r0, [pc, #28]
	lsls	r3, r2, #6
	movs	r1, #0
	adds	r2, r3, r0
.L4:
	adds	r1, #1
	strh	r4, [r2, #0]
	adds	r2, #2
	cmp	r1, #32
	bne.n	.L4
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001ef8
	.4byte 0x0000f081
	.4byte 0x06006000
	ldr	r3, [pc, #4]
	movs	r2, #0
	strh	r2, [r3, #0]
	bx	lr
	.4byte 0x04000012
