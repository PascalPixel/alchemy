@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080919d8
	.thumb_func
Func_080919d8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #0
	mov	r8, r0
	mov	sl, r2
	bl	Func_08077148
	adds	r6, r0, #0
	cmp	sl, r6
	bge.n	.L0
	ldr	r3, [pc, #92]
	movs	r2, #252
	lsls	r2, r2, #1
	adds	r7, r3, r2
	adds	r5, r6, #0
.L1:
	ldrb	r0, [r7, #0]
	mov	r1, r8
	bl	Func_080772d8
	subs	r5, #1
	adds	r7, #1
	add	sl, r0
	cmp	r5, #0
	bne.n	.L1
.L0:
	lsls	r3, r6, #4
	subs	r3, r3, r6
	lsls	r3, r3, #1
	cmp	sl, r3
	blt.n	.L2
	mov	r0, r8
	movs	r1, #2
	bl	Func_08015120
	ldr	r5, [pc, #52]
	movs	r1, #1
	adds	r0, r5, #0
	bl	Func_08015040
	adds	r5, #1
	mov	r0, r8
	movs	r1, #2
	bl	Func_08015120
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015040
	movs	r0, #1
	negs	r0, r0
	b.n	.L3
.L2:
	movs	r0, #0
.L3:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x0000097d
