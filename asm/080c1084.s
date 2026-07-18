@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c1084
	.thumb_func
Func_080c1084:
	push	{lr}
	ldr	r3, [pc, #72]
	ldr	r0, [r3, #0]
	cmp	r0, #0
	beq.n	.L0
	movs	r2, #202
	lsls	r2, r2, #3
	adds	r3, r0, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #40]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r0, r0, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r0, #0]
	ldr	r1, [pc, #48]
	ldrsb	r3, [r2, r3]
	strh	r3, [r1, #0]
	ldrh	r2, [r0, #0]
	movs	r3, #15
	adds	r1, r2, #1
	ands	r1, r3
	cmp	r2, #14
	bls.n	.L1
	movs	r3, #16
	orrs	r1, r3
.L1:
	strh	r1, [r0, #0]
	b.n	.L0
	.4byte 0x00003f90
	.4byte 0x00000010
	.4byte 0x03001e74
	.4byte 0x04000050
	.4byte 0x0000064e
	.4byte 0x080c5c10
	.4byte 0x04000054
.L0:
	pop	{r0}
	bx	r0
