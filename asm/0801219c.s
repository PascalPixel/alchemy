@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801219c
	.thumb_func
Func_0801219c:
	push	{lr}
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bge.n	.L0
	ldr	r2, [pc, #84]
	adds	r3, r3, r2
.L0:
	asrs	r4, r3, #16
	ldr	r2, [r0, #8]
	ldr	r3, [r0, #4]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L1
	ldr	r3, [pc, #68]
	adds	r0, r0, r3
.L1:
	ldr	r3, [pc, #68]
	ldr	r3, [r3, #0]
	asrs	r0, r0, #16
	cmp	r3, #0
	bne.n	.L2
	movs	r0, #0
	b.n	.L3
.L2:
	movs	r2, #200
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	adds	r3, r4, #0
	cmp	r3, #0
	bge.n	.L4
	adds	r3, #15
.L4:
	asrs	r4, r3, #4
	cmp	r0, #0
	bge.n	.L5
	adds	r0, #15
.L5:
	asrs	r3, r0, #4
	lsls	r3, r3, #7
	adds	r3, r4, r3
	lsls	r3, r3, #2
	adds	r1, r1, r3
	ldrb	r3, [r1, #2]
	movs	r2, #255
	eors	r3, r2
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	subs	r0, #1
.L3:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000ffff
	.4byte 0x03001e70
