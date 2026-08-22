@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08017248
	.thumb_func
Func_08017248:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	lr, r3
	ldr	r3, [pc, #132]
	lsls	r1, r1, #5
	ldr	r3, [r3, #0]
	adds	r1, r1, r0
	sub	sp, #4
	lsls	r1, r1, #1
	adds	r6, r2, #0
	ldr	r4, [sp, #36]
	str	r3, [sp, #0]
	adds	r1, r1, r3
	cmp	r6, #1
	bls.n	.L0
	mov	r2, lr
	cmp	r2, #1
	bls.n	.L0
	cmp	r6, #30
	bhi.n	.L0
	cmp	r2, #30
	bhi.n	.L0
	adds	r1, #64
	cmp	r4, #0
	bne.n	.L1
	movs	r3, #1
	negs	r3, r3
	add	r3, lr
	movs	r5, #1
	mov	r8, r3
	cmp	r5, r8
	bcs.n	.L2
	movs	r3, #32
	subs	r3, r3, r6
	lsls	r3, r3, #1
	subs	r2, r6, #1
	mov	sl, r3
	ldr	r3, [pc, #68]
	mov	ip, r2
	mov	r9, ip
	mov	fp, r3
.L5:
	movs	r4, #1
	adds	r1, #2
	cmp	r4, r9
	bcs.n	.L3
	mov	r3, lr
	subs	r3, #2
	adds	r2, r3, #0
	ldr	r7, [pc, #36]
	mov	r3, fp
	ldr	r6, [pc, #36]
	adds	r0, r3, r5
.L4:
	adds	r3, r0, #0
	ands	r3, r7
	orrs	r3, r6
	adds	r4, #1
	strh	r3, [r1, #0]
	adds	r0, r0, r2
	adds	r1, #2
	cmp	r4, ip
	bcc.n	.L4
.L3:
	adds	r1, #2
	adds	r5, #1
	add	r1, sl
	cmp	r5, r8
	bcc.n	.L5
	b.n	.L2
	.4byte 0x00000fff
	.4byte 0x0000f000
	.4byte 0x03001e8c
	.4byte 0x00000127
.L1:
	mov	r0, lr
	movs	r5, #1
	subs	r0, #1
	cmp	r5, r0
	bcs.n	.L2
	movs	r3, #32
	subs	r3, r3, r6
	ldr	r2, [pc, #76]
	lsls	r3, r3, #1
	mov	sl, r3
	mov	r9, r2
	mov	r8, r0
.L8:
	movs	r4, #0
	cmp	r4, r6
	bcs.n	.L6
	movs	r3, #2
	negs	r3, r3
	add	r3, lr
	ldr	r2, [pc, #56]
	mov	fp, r3
	ldr	r7, [pc, #44]
	mov	r3, r9
	mov	ip, r2
	adds	r0, r5, r3
.L7:
	adds	r3, r0, #0
	mov	r2, ip
	ands	r3, r2
	orrs	r3, r7
	adds	r4, #1
	strh	r3, [r1, #0]
	add	r0, fp
	adds	r1, #2
	cmp	r4, r6
	bcc.n	.L7
.L6:
	adds	r5, #1
	add	r1, sl
	cmp	r5, r8
	bcc.n	.L8
.L2:
	ldr	r3, [sp, #0]
	ldr	r1, [pc, #20]
	adds	r2, r3, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L0
	.4byte 0x0000f000
	.4byte 0x00000127
	.4byte 0x00000fff
	.4byte 0x00000ea3
.L0:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
