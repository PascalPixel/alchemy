@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809b3d8
	.thumb_func
Func_0809b3d8:
	push	{r5, r6, lr}
	ldr	r3, [pc, #104]
	movs	r5, #237
	lsls	r5, r5, #1
	ldr	r1, [r0, #20]
	movs	r2, #160
	lsls	r2, r2, #12
	adds	r3, r3, r5
	adds	r4, r1, r2
	movs	r5, #0
	ldrsh	r2, [r3, r5]
	ldr	r3, [pc, #88]
	ldr	r6, [r0, #104]
	cmp	r2, r3
	bne.n	.L0
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r4, r1, r2
.L0:
	ldr	r5, [r0, #12]
	cmp	r5, r4
	bgt.n	.L1
	bl	Func_080090d0
	b.n	.L2
.L1:
	ldr	r3, [r0, #24]
	movs	r4, #192
	lsls	r4, r4, #4
	movs	r1, #128
	adds	r2, r3, r4
	lsls	r1, r1, #9
	cmp	r2, r1
	ble.n	.L3
	adds	r2, r1, #0
.L3:
	negs	r3, r2
	str	r2, [r0, #24]
	str	r3, [r0, #28]
	ldr	r4, [pc, #40]
	ldr	r3, [r6, #8]
	str	r3, [r0, #8]
	adds	r3, r5, r4
	str	r3, [r0, #12]
	subs	r3, r1, r2
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [r6, #16]
	movs	r5, #128
	subs	r3, r3, r2
	lsls	r5, r5, #13
	adds	r3, r3, r5
	str	r3, [r0, #16]
.L2:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000001
	.4byte 0xfffe0000
