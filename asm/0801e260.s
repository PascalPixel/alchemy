@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801e260
	.thumb_func
Func_0801e260:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r3
	ldr	r3, [pc, #144]
	adds	r7, r2, #0
	ldr	r5, [r3, #0]
	ldr	r2, [pc, #144]
	lsls	r1, r1, #5
	adds	r3, r5, r2
	adds	r1, r1, r0
	ldrb	r3, [r3, #0]
	lsls	r1, r1, #1
	movs	r6, #0
	sub	sp, #4
	adds	r0, r1, r5
	mov	r8, r3
	cmp	r6, fp
	bcs.n	.L0
	movs	r3, #32
	subs	r3, r3, r7
	lsls	r3, r3, #1
	str	r3, [sp, #0]
.L5:
	movs	r4, #0
	cmp	r4, r7
	bcs.n	.L1
	ldr	r3, [pc, #108]
	ldr	r2, [pc, #112]
	mov	r9, r3
	ldr	r3, [pc, #112]
	mov	sl, r2
	movs	r2, #255
	mov	lr, r3
	mov	ip, r2
.L4:
	ldrh	r3, [r0, #0]
	mov	r2, r9
	ands	r2, r3
	adds	r3, r2, #0
	subs	r3, #128
	adds	r0, #2
	cmp	r3, #127
	bls.n	.L2
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L3
	cmp	r2, sl
	bls.n	.L3
	cmp	r2, lr
	bhi.n	.L3
.L2:
	mov	r3, ip
	ands	r2, r3
	movs	r3, #128
	eors	r2, r3
	movs	r3, #218
	lsls	r3, r3, #4
	adds	r2, r2, r3
	ldrb	r1, [r5, r2]
	movs	r3, #252
	ands	r3, r1
	strb	r3, [r5, r2]
.L3:
	adds	r4, #1
	cmp	r4, r7
	bcc.n	.L4
.L1:
	ldr	r2, [sp, #0]
	adds	r6, #1
	adds	r0, r0, r2
	cmp	r6, fp
	bcc.n	.L5
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
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000ea2
	.4byte 0x000003ff
	.4byte 0x000001ff
	.4byte 0x0000027f
