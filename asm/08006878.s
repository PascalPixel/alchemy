@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08006878
	.thumb_func
Func_08006878:
	push	{r4, r5, lr}
	sub	sp, #68
	mov	r0, sp
	bl	Func_08006ac0
	mov	r5, sp
	adds	r5, #1
	ldr	r2, [pc, #24]
	movs	r0, #170
	strb	r0, [r2, #0]
	ldr	r1, [pc, #20]
	movs	r0, #85
	strb	r0, [r1, #0]
	movs	r0, #144
	strb	r0, [r2, #0]
	add	r1, sp, #64
	ldr	r2, [pc, #12]
	adds	r0, r2, #0
	b.n	.L0
	movs	r0, r0
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.4byte 0x00004e20
.L1:
	ldrh	r0, [r1, #0]
	subs	r0, #1
.L0:
	strh	r0, [r1, #0]
	ldrh	r0, [r1, #0]
	cmp	r0, #0
	bne.n	.L1
	ldr	r0, [pc, #44]
	bl	Func_080072f8
	lsls	r0, r0, #24
	lsrs	r4, r0, #16
	movs	r0, #224
	lsls	r0, r0, #20
	bl	Func_080072f8
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	orrs	r4, r0
	ldr	r2, [pc, #24]
	movs	r0, #170
	strb	r0, [r2, #0]
	ldr	r1, [pc, #24]
	movs	r0, #85
	strb	r0, [r1, #0]
	movs	r0, #240
	strb	r0, [r2, #0]
	add	r1, sp, #64
	ldr	r2, [pc, #16]
	adds	r0, r2, #0
	b.n	.L2
	.4byte 0x0e000001
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.4byte 0x00004e20
.L3:
	ldrh	r0, [r1, #0]
	subs	r0, #1
.L2:
	strh	r0, [r1, #0]
	ldrh	r0, [r1, #0]
	cmp	r0, #0
	bne.n	.L3
	adds	r0, r4, #0
	add	sp, #68
	pop	{r4, r5}
	pop	{r1}
	bx	r1
