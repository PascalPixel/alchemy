@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08011a84
	.thumb_func
Func_08011a84:
	push	{r5, r6, lr}
	ldr	r3, [pc, #68]
	ldr	r3, [r3, #0]
	sub	sp, #4
	adds	r4, r3, #0
	adds	r5, r0, #0
	movs	r6, #0
	mov	r0, sp
	adds	r4, #216
	str	r6, [r0, #0]
	ldr	r3, [pc, #52]
	adds	r1, r4, #0
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldrh	r3, [r5, #0]
	ldr	r2, [pc, #48]
	cmp	r3, r2
	beq.n	.L0
	str	r5, [r4, #0]
	str	r5, [r4, #4]
	strh	r6, [r4, #8]
	strh	r6, [r4, #10]
	movs	r6, #1
.L0:
	cmp	r6, #0
	beq.n	.L1
	movs	r1, #200
	ldr	r0, [pc, #32]
	lsls	r1, r1, #4
	bl	Func_080041d8
.L1:
	add	sp, #4
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0x040000d4
	.4byte 0x85000003
	.4byte 0x0000ffff
	.4byte 0x080119cd
