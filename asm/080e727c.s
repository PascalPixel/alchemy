@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e727c
	.thumb_func
Func_080e727c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #52]
	ldr	r7, [pc, #44]
	mov	r8, r0
	mov	lr, r1
	mov	ip, r2
	movs	r6, #0
.L3:
	ldrh	r2, [r5, #0]
	lsls	r3, r2, #16
	lsrs	r1, r3, #26
	ands	r1, r7
	lsrs	r4, r3, #21
	movs	r0, #31
	ands	r4, r7
	ands	r0, r2
	add	r1, r8
	add	r4, lr
	add	r0, ip
	cmp	r1, #31
	ble.n	.L0
	movs	r1, #31
.L0:
	cmp	r4, #31
	ble.n	.L1
	movs	r4, #31
	b.n	.L1
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x05000002
.L1:
	cmp	r0, #31
	ble.n	.L2
	movs	r0, #31
.L2:
	lsls	r3, r1, #10
	lsls	r2, r4, #5
	orrs	r3, r2
	orrs	r3, r0
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #63
	bne.n	.L3
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
