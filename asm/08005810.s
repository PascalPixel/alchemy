@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08005810
	.thumb_func
Func_08005810:
	push	{r5, r6, lr}
	ldr	r3, [pc, #80]
	sub	sp, #64
	ldr	r2, [r3, #0]
	movs	r6, #0
	movs	r5, #0
	mov	r1, sp
.L1:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	cmp	r3, #0
	bne.n	.L0
	stmia	r1!, {r5}
	adds	r6, #1
.L0:
	adds	r5, #1
	cmp	r5, #15
	bls.n	.L1
	movs	r5, #16
	cmp	r6, #0
	beq.n	.L2
	cmp	r6, #1
	bne.n	.L3
	ldr	r5, [sp, #0]
	bl	Func_08005b24
	cmp	r0, #16
	bne.n	.L2
	movs	r5, #16
	b.n	.L2
.L3:
	bl	Func_08004458
	adds	r1, r6, #0
	bl	Func_08002304
	adds	r5, r0, #0
	mov	r2, sp
	lsls	r3, r5, #2
	ldr	r5, [r2, r3]
.L2:
	adds	r0, r5, #0
	add	sp, #64
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001f1c
