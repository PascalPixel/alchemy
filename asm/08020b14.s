@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08020b14
	.thumb_func
Func_08020b14:
	push	{lr}
	ldr	r3, [pc, #68]
	ldr	r4, [r3, #0]
	ldrb	r3, [r0, #0]
	sub	sp, #8
	movs	r1, #0
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #235
	lsls	r3, r3, #4
	adds	r2, r4, r3
.L1:
	ldrb	r3, [r0, #0]
	strh	r3, [r2, #0]
	adds	r0, #1
	ldrb	r3, [r0, #0]
	adds	r2, #2
	adds	r1, #1
	cmp	r3, #0
	bne.n	.L1
.L0:
	movs	r2, #235
	lsls	r3, r1, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldr	r2, [pc, #20]
	add	r1, sp, #4
	strh	r2, [r4, r3]
	movs	r0, #0
	mov	r2, sp
	movs	r3, #0
	bl	Func_08018850
	ldr	r0, [sp, #4]
	add	sp, #8
	b.n	.L2
	.4byte 0x00000000
	.4byte 0x03001e8c
.L2:
	pop	{r1}
	bx	r1
