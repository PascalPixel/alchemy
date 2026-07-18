@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ae778
	.thumb_func
Func_080ae778:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #112]
	sub	sp, #16
	mov	r7, sp
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	adds	r0, r7, #0
	adds	r6, r1, #0
	mov	r8, r3
	bl	Func_080ae7fc
	ldrb	r3, [r7, r5]
	subs	r3, #1
	strb	r3, [r7, r5]
	ldrb	r3, [r7, r6]
	ldr	r2, [pc, #88]
	adds	r3, #1
	strb	r3, [r7, r6]
	add	r2, r8
	movs	r3, #1
	mov	sl, r3
	ldrb	r3, [r2, #0]
	movs	r5, #0
	cmp	r5, r3
	bcs.n	.L0
	mov	ip, r3
	adds	r0, r2, #0
	mov	lr, ip
.L3:
	adds	r1, r5, #0
	cmp	r5, ip
	bcs.n	.L1
	ldrb	r6, [r0, #0]
	adds	r4, r7, #0
.L2:
	adds	r3, r1, #1
	lsls	r3, r3, #24
	lsrs	r1, r3, #24
	cmp	r1, r6
	bcs.n	.L1
	ldrsb	r3, [r4, r5]
	ldrsb	r2, [r4, r1]
	subs	r3, r3, r2
	adds	r3, #1
	cmp	r3, #2
	bls.n	.L2
	movs	r3, #0
	mov	sl, r3
.L1:
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	cmp	r5, lr
	bcc.n	.L3
.L0:
	mov	r0, sl
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x00000219
