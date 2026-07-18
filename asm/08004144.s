@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08004144
	.thumb_func
Func_08004144:
	push	{r5, r6, lr}
	sub	sp, #8
	ldr	r2, [pc, #72]
	movs	r4, #19
	b.n	.L0
.L5:
	ldr	r2, [pc, #68]
.L0:
	adds	r1, r2, #0
	cmp	r4, #0
	ble.n	.L1
	adds	r0, r4, #0
.L4:
	movs	r3, #12
	ldrsh	r2, [r1, r3]
	movs	r5, #4
	ldrsh	r3, [r1, r5]
	cmp	r2, r3
	ble.n	.L2
	adds	r3, r1, #0
	mov	r2, sp
	ldmia	r3!, {r5, r6}
	stmia	r2!, {r5, r6}
	adds	r2, r1, #0
	adds	r1, r3, #0
	ldmia	r3!, {r5, r6}
	stmia	r2!, {r5, r6}
	mov	r3, sp
	ldmia	r3!, {r5, r6}
	stmia	r2!, {r5, r6}
	b.n	.L3
.L2:
	adds	r1, #8
.L3:
	subs	r0, #1
	cmp	r0, #0
	bne.n	.L4
.L1:
	subs	r4, #1
	cmp	r4, #1
	bgt.n	.L5
	add	sp, #8
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001a20
