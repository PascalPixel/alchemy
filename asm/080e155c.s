@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e155c
	.thumb_func
Func_080e155c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #112]
	adds	r6, r0, #0
	adds	r7, r1, #0
	ldr	r0, [pc, #108]
	ldr	r1, [r3, #0]
	adds	r0, r0, r1
	ldr	r3, [r0, #0]
	mov	r8, r0
	cmp	r3, #0
	ble.n	.L0
	bl	Func_08004458
	subs	r6, #1
	ands	r6, r0
	bl	Func_08004458
	lsrs	r5, r7, #31
	adds	r5, r7, r5
	subs	r2, r7, #1
	asrs	r5, r5, #1
	ands	r2, r0
	subs	r2, r2, r5
	ldr	r1, [pc, #76]
	adds	r3, r2, #0
	subs	r6, r6, r5
	adds	r3, #32
	strh	r6, [r1, #4]
	strh	r3, [r1, #6]
	ldr	r1, [pc, #68]
	movs	r3, #120
	subs	r6, r3, r6
	subs	r3, r3, r2
	str	r6, [r1, #12]
	str	r3, [r1, #16]
	mov	r2, r8
	ldr	r3, [r2, #0]
	subs	r3, #1
	str	r3, [r2, #0]
	b.n	.L1
.L0:
	ldr	r0, [pc, #48]
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #36]
	adds	r0, #4
	strh	r3, [r2, #4]
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	strh	r3, [r2, #6]
	ldr	r2, [pc, #28]
	movs	r3, #120
	str	r3, [r2, #12]
	str	r3, [r2, #16]
.L1:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x000077a8
	.4byte 0x03001ad0
	.4byte 0x03001ce0
	.4byte 0x000077a0
