@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08017c1c
	.thumb_func
Func_08017c1c:
	push	{r5, r6, r7, lr}
	adds	r6, r3, #0
	ldr	r3, [pc, #80]
	adds	r5, r1, #0
	adds	r7, r2, #0
	ldr	r4, [r3, #0]
	cmp	r0, #0
	bne.n	.L0
	ldr	r3, [pc, #72]
	adds	r1, r4, r3
	ldrh	r3, [r1, #0]
	movs	r2, #235
	lsls	r2, r2, #4
	lsls	r3, r3, #1
	adds	r3, r3, r2
	adds	r0, r4, r2
	ldr	r2, [pc, #44]
	strh	r2, [r4, r3]
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #44]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r1, #0]
.L0:
	ldrh	r3, [r5, #14]
	ldrh	r2, [r5, #12]
	adds	r3, r3, r6
	adds	r3, #1
	lsls	r3, r3, #5
	adds	r2, r2, r7
	adds	r3, r3, r2
	adds	r1, r3, #1
	movs	r3, #160
	lsls	r3, r3, #2
	cmp	r1, r3
	bcs.n	.L1
	ldr	r3, [pc, #24]
	lsls	r1, r1, #1
	adds	r2, r1, r3
	adds	r1, r4, r1
	b.n	.L2
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0x000012b2
	.4byte 0x06002000
.L2:
	bl	Func_0801de5c
.L1:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
