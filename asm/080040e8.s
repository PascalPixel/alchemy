@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080040e8
	.thumb_func
Func_080040e8:
	push	{lr}
	ldr	r3, [pc, #48]
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r3, [pc, #44]
	ldr	r1, [pc, #48]
	ldr	r4, [pc, #32]
	strb	r2, [r3, #0]
	movs	r0, #0
	movs	r2, #19
.L0:
	ldrh	r3, [r1, #4]
	subs	r2, #1
	orrs	r3, r4
	str	r0, [r1, #0]
	strh	r3, [r1, #4]
	strb	r0, [r1, #6]
	adds	r1, #8
	cmp	r2, #0
	bge.n	.L0
	ldr	r2, [pc, #12]
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L1
	movs	r0, r0
	.4byte 0x0000ffff
	.4byte 0x03001d34
	.4byte 0x03001a10
	.4byte 0x03001a20
.L1:
	pop	{r0}
	bx	r0
