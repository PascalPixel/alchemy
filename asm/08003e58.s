@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08003e58
	.thumb_func
Func_08003e58:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	lsrs	r5, r1, #6
	cmp	r6, #95
	bls.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	ldr	r1, [pc, #96]
	ldr	r2, [pc, #100]
	mov	ip, r1
	movs	r4, #0
	mov	lr, r2
	mov	r7, ip
.L7:
	movs	r3, #128
	movs	r0, #1
	lsls	r3, r3, #2
	negs	r0, r0
	cmp	r4, r3
	bge.n	.L1
	ldrb	r3, [r7, r4]
	cmp	r3, #255
	bne.n	.L2
	adds	r0, r4, #0
	adds	r1, r5, r0
	cmp	r0, r1
	bcs.n	.L3
	adds	r2, r0, r7
.L4:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	cmp	r3, #255
	bne.n	.L2
	adds	r4, #1
	cmp	r4, r1
	bcc.n	.L4
.L3:
	movs	r2, #0
	cmp	r2, r5
	bcs.n	.L5
.L6:
	adds	r3, r0, r2
	mov	r1, ip
	adds	r2, #1
	strb	r6, [r1, r3]
	cmp	r2, r5
	bcc.n	.L6
	b.n	.L5
.L2:
	mov	r2, ip
	ldrb	r3, [r2, r4]
	mov	r1, lr
	lsls	r3, r3, #2
	ldrh	r3, [r1, r3]
	lsrs	r3, r3, #6
	adds	r4, r4, r3
	b.n	.L7
.L5:
	lsls	r0, r0, #6
.L1:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001810
	.4byte 0x03001b10
