@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08010d48
	.thumb_func
Func_08010d48:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r0, r3, #0
	ldr	r3, [pc, #188]
	ldr	r3, [r3, #0]
	mov	r8, r3
	ldr	r3, [r3, #0]
	movs	r6, #0
	sub	sp, #4
	adds	r4, r2, #0
	mov	ip, r6
	cmp	r3, #0
	beq.n	.L0
	ldmia	r3!, {r6}
	ldr	r3, [r3, #4]
	mov	ip, r3
.L0:
	asrs	r1, r1, #4
	lsls	r3, r1, #4
	mov	r2, ip
	asrs	r5, r5, #4
	asrs	r0, r0, #3
	asrs	r4, r4, #3
	adds	r7, r3, r5
	asrs	r2, r2, #24
	asrs	r3, r0, #31
	mov	ip, r2
	asrs	r5, r4, #31
	lsrs	r2, r3, #31
	mov	lr, r3
	adds	r2, r0, r2
	lsrs	r3, r5, #31
	movs	r1, #15
	asrs	r2, r2, #1
	adds	r3, r4, r3
	ands	r2, r1
	asrs	r3, r3, #1
	ands	r3, r1
	lsls	r2, r2, #4
	adds	r2, r2, r3
	movs	r3, #156
	lsls	r3, r3, #1
	lsls	r2, r2, #1
	adds	r2, r2, r3
	asrs	r6, r6, #24
	mov	r3, r8
	strh	r7, [r3, r2]
	subs	r3, r6, r4
	cmp	r3, #0
	blt.n	.L1
	cmp	r3, #1
	ble.n	.L2
	b.n	.L3
.L1:
	subs	r3, r4, r6
	cmp	r3, #1
	bgt.n	.L3
.L2:
	mov	r2, ip
	subs	r3, r2, r0
	cmp	r3, #0
	blt.n	.L4
	cmp	r3, #1
	ble.n	.L5
	b.n	.L3
.L4:
	mov	r2, ip
	subs	r3, r0, r2
	cmp	r3, #1
	bgt.n	.L3
.L5:
	mov	r3, lr
	lsrs	r6, r5, #31
	lsrs	r5, r3, #31
	adds	r6, r4, r6
	adds	r5, r0, r5
	movs	r2, #1
	asrs	r6, r6, #1
	asrs	r5, r5, #1
	mov	r8, r2
	str	r2, [sp, #0]
	adds	r1, r6, #0
	adds	r2, r5, #0
	adds	r3, r7, #0
	movs	r0, #0
	bl	Func_080108e4
	movs	r2, #160
	lsls	r2, r2, #1
	adds	r3, r7, r2
	mov	r2, r8
	str	r2, [sp, #0]
	movs	r0, #1
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	Func_080108e4
.L3:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
