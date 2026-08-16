@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bffb8
	.thumb_func
Func_080bffb8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #8
	ldr	r2, [pc, #68]
	movs	r0, #6
	add	r0, sp
	ldrh	r3, [r2, #0]
	mov	r9, r0
	mov	r1, r9
	strh	r3, [r1, #0]
	ldr	r1, [pc, #52]
	orrs	r3, r1
	strh	r3, [r2, #0]
	add	r3, sp, #4
	adds	r2, #2
	mov	sl, r3
	ldrh	r3, [r2, #0]
	mov	r0, sl
	strh	r3, [r0, #0]
	orrs	r3, r1
	strh	r3, [r2, #0]
	movs	r3, #2
	add	r3, sp
	adds	r2, #2
	mov	r8, r3
	ldrh	r3, [r2, #0]
	mov	r0, r8
	strh	r3, [r0, #0]
	orrs	r3, r1
	strh	r3, [r2, #0]
	adds	r2, #2
	ldrh	r3, [r2, #0]
	mov	r7, sp
	strh	r3, [r7, #0]
	orrs	r3, r1
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000040
	.4byte 0x04000008
.L0:
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #66
	strh	r3, [r2, #0]
	movs	r0, #16
	bl	Func_08003b70
	ldr	r6, [pc, #52]
	movs	r5, #0
.L1:
	bl	Func_08004458
	bl	Func_08004458
	bl	Func_08004458
	bl	Func_08004458
	lsls	r3, r5, #8
	orrs	r3, r5
	strh	r3, [r6, #0]
	movs	r0, #1
	adds	r5, #1
	bl	Func_080030f8
	cmp	r5, #15
	ble.n	.L1
	ldr	r3, [pc, #8]
	movs	r2, #128
	b.n	.L2
	movs	r0, r0
	.4byte 0x00003eee
	.4byte 0x00000001
	.4byte 0x0400004c
.L2:
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #4
	bl	Func_080030f8
	mov	r1, r9
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #44]
	strh	r3, [r2, #0]
	mov	r0, sl
	ldrh	r3, [r0, #0]
	adds	r2, #2
	strh	r3, [r2, #0]
	mov	r1, r8
	ldrh	r3, [r1, #0]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldrh	r3, [r7, #0]
	adds	r2, #2
	movs	r0, #0
	strh	r3, [r2, #0]
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x04000008
