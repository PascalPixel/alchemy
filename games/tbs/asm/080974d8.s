@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080974d8
	.thumb_func
Func_080974d8:
	push	{r5, r6, lr}
	ldr	r2, [pc, #92]
	movs	r1, #207
	ldr	r3, [r2, #0]
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	sub	sp, #12
	adds	r6, r0, #0
	cmp	r3, #3
	bne.n	.L0
	mov	r5, sp
	adds	r1, r5, #0
	bl	Func_08005268
	ldr	r3, [r5, #0]
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [r5, #4]
	lsls	r3, r3, #16
	b.n	.L1
.L0:
	adds	r3, r2, #0
	subs	r3, #76
	ldr	r2, [r3, #0]
	adds	r3, r2, #0
	adds	r3, #228
	adds	r2, #232
	ldr	r1, [r3, #0]
	ldr	r0, [r2, #0]
	ldr	r3, [pc, #36]
	ands	r1, r3
	ands	r0, r3
	ldr	r3, [r6, #0]
	subs	r3, r3, r1
	ldr	r2, [r6, #4]
	str	r3, [r6, #0]
	ldr	r3, [r6, #8]
	subs	r3, r3, r2
	subs	r3, r3, r0
.L1:
	str	r3, [r6, #8]
	movs	r3, #0
	str	r3, [r6, #4]
	add	sp, #12
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0xffff0000
