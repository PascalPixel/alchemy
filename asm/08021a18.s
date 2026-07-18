@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08021a18
	.thumb_func
Func_08021a18:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	mov	r9, r0
	mov	sl, r2
	mov	r8, r2
	mov	fp, r2
.L3:
	movs	r3, #0
	mov	lr, r3
	mov	r3, fp
	add	r3, sl
	lsls	r7, r3, #6
.L2:
	mov	r2, r9
	adds	r6, r2, r7
	mov	r2, lr
	lsls	r3, r2, #5
	ldr	r2, [pc, #100]
	adds	r5, r3, r2
	movs	r3, #0
	mov	ip, r3
.L1:
	ldrh	r1, [r5, #0]
	movs	r0, #0
	adds	r5, #2
	movs	r4, #0
.L0:
	adds	r3, r1, #0
	movs	r2, #15
	ands	r3, r2
	add	r3, r8
	lsls	r2, r3, #1
	ldr	r3, [pc, #76]
	ldrh	r2, [r3, r2]
	lsls	r3, r4, #2
	lsls	r2, r3
	adds	r4, #1
	lsrs	r1, r1, #4
	orrs	r0, r2
	cmp	r4, #3
	ble.n	.L0
	movs	r2, #1
	add	ip, r2
	mov	r3, ip
	strh	r0, [r6, #0]
	adds	r6, #2
	cmp	r3, #15
	ble.n	.L1
	add	lr, r2
	mov	r2, lr
	adds	r7, #32
	cmp	r2, #9
	ble.n	.L2
	movs	r3, #16
	add	r8, r3
	movs	r3, #1
	movs	r2, #4
	add	sl, r3
	add	fp, r2
	mov	r2, sl
	cmp	r2, #1
	ble.n	.L3
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x06000600
	.4byte 0x080372c0
