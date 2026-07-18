@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080990cc
	.thumb_func
Func_080990cc:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	sub	sp, #12
	cmp	r6, #0
	beq.n	.L0
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	subs	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	cmp	r2, #0
	beq.n	.L1
	ldr	r3, [r6, #56]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r6, #60]
	str	r3, [r5, #4]
	ldr	r3, [r6, #64]
	str	r3, [r5, #8]
	adds	r3, r6, #0
	adds	r3, #102
	movs	r4, #0
	ldrsh	r1, [r3, r4]
	lsls	r3, r2, #11
	lsls	r0, r2, #17
	subs	r1, r1, r3
	adds	r2, r5, #0
	bl	Func_0800447c
	ldr	r3, [r5, #0]
	str	r3, [r6, #8]
	ldr	r3, [r5, #4]
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	b.n	.L0
.L1:
	ldr	r3, [pc, #8]
	str	r3, [r6, #108]
.L0:
	add	sp, #12
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x08099041
