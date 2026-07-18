@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800070c
	.thumb_func
Func_0800070c:
	push	{r5, r6, r7, lr}
	mov	r1, r8
	mov	r2, r9
	mov	r3, sl
	mov	r4, fp
	push	{r0, r1, r2, r3, r4}
	sub	sp, #24
	ldr	r3, [r0, #32]
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [r0, #36]
	bl	Func_0800070a
	ldr	r0, [sp, #24]
.L0:
	ldr	r3, [r0, #40]
	bl	Func_0800070a
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #16]
	str	r3, [sp, #0]
	ldr	r5, [pc, #40]
	adds	r5, r5, r0
	ldrb	r4, [r0, #4]
	subs	r7, r4, #1
	bls.n	.L1
	ldrb	r1, [r0, #11]
	subs	r1, r1, r7
	adds	r2, r3, #0
	muls	r2, r1
	adds	r5, r5, r2
.L1:
	str	r5, [sp, #8]
	ldr	r4, [sp, #24]
	ldr	r0, [r4, #20]
	mov	r9, r0
	ldr	r0, [r4, #24]
	mov	ip, r0
	ldrb	r0, [r4, #6]
	adds	r4, #80
	ldr	r3, [pc, #0]
	bx	r3
	.4byte 0x03000659
	.4byte 0x00000350
