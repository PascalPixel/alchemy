@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ba918
	.thumb_func
Func_080ba918:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #255
	mov	r8, r0
	adds	r7, r1, #0
	movs	r6, #0
	mov	sl, r3
	b.n	.L0
.L3:
	ldr	r2, [r0, #40]
	ldrb	r3, [r2, #22]
	mov	r4, sl
	orrs	r3, r4
	strb	r3, [r2, #22]
	adds	r3, r0, #0
	adds	r3, #39
	adds	r1, r0, #0
	ldrb	r0, [r3, #0]
	adds	r1, #44
	strb	r7, [r2, #5]
	cmp	r0, #1
	ble.n	.L1
	movs	r5, #0
	movs	r4, #255
	subs	r0, #1
.L2:
	ldmia	r1!, {r2}
	ldrb	r3, [r2, #22]
	subs	r0, #1
	orrs	r3, r4
	strb	r5, [r2, #5]
	strb	r3, [r2, #22]
	cmp	r0, #0
	bne.n	.L2
.L1:
	adds	r6, #1
.L0:
	mov	r0, r8
	adds	r1, r6, #0
	bl	Func_080b7f70
	cmp	r0, #0
	bne.n	.L3
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
