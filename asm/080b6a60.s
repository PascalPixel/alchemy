@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b6a60
	.thumb_func
Func_080b6a60:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #112]
	ldr	r3, [r3, #0]
	adds	r3, #68
	ldrb	r3, [r3, #0]
	sub	sp, #4
	adds	r5, r0, #0
	movs	r6, #4
	cmp	r3, #0
	beq.n	.L0
	movs	r6, #3
.L0:
	bl	Func_08077148
	adds	r7, r0, #0
	cmp	r7, r6
	ble.n	.L1
	adds	r7, r6, #0
.L1:
	cmp	r7, #0
	ble.n	.L2
	ldr	r3, [pc, #80]
	movs	r1, #252
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #2
	mov	r8, r3
	adds	r6, r7, #0
.L4:
	ldrb	r0, [r2, #0]
	adds	r2, #1
	cmp	r5, #0
	beq.n	.L3
	strh	r0, [r5, #0]
	adds	r5, #2
.L3:
	str	r2, [sp, #0]
	bl	Func_08077008
	movs	r1, #149
	lsls	r1, r1, #1
	adds	r3, r0, r1
	subs	r6, #1
	mov	r1, r8
	strb	r1, [r3, #0]
	ldr	r2, [sp, #0]
	cmp	r6, #0
	bne.n	.L4
.L2:
	cmp	r5, #0
	beq.n	.L5
	ldr	r3, [pc, #16]
	strh	r3, [r5, #0]
.L5:
	adds	r0, r7, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000000ff
	.4byte 0x03001e74
	.4byte 0x02000240
