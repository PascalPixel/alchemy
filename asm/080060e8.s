@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080060e8
	.thumb_func
Func_080060e8:
	push	{r5, lr}
	ldr	r4, [pc, #96]
	ldr	r1, [r4, #40]
	ldrb	r3, [r4, #11]
	strb	r3, [r1, #0]
	ldrb	r2, [r4, #3]
	ldrb	r3, [r4, #2]
	movs	r5, #0
	eors	r3, r2
	strb	r3, [r1, #1]
	strh	r5, [r1, #2]
	ldr	r3, [pc, #80]
	adds	r1, #4
	ldr	r2, [pc, #80]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [r4, #40]
	movs	r1, #0
.L0:
	ldrh	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #2
	adds	r5, r5, r3
	cmp	r1, #13
	bls.n	.L0
	ldr	r3, [r4, #40]
	mvns	r2, r5
	strh	r2, [r3, #2]
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L1
	ldr	r2, [pc, #48]
	movs	r3, #0
	strh	r3, [r2, #0]
.L1:
	movs	r3, #1
	negs	r3, r3
	str	r3, [r4, #20]
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L2
	ldrb	r3, [r4, #8]
	cmp	r3, #0
	beq.n	.L2
	ldr	r2, [pc, #24]
	ldr	r3, [pc, #8]
	strh	r3, [r2, #0]
.L2:
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x000000c0
	.4byte 0x02002240
	.4byte 0x040000d4
	.4byte 0x84000006
	.4byte 0x0400010e
