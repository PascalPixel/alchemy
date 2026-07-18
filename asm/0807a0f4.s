@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0807a0f4
	.thumb_func
Func_0807a0f4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	lsls	r3, r7, #2
	adds	r3, r3, r7
	mov	sl, r1
	lsls	r3, r3, #2
	add	r3, sl
	adds	r3, #48
	mov	fp, r3
	ldr	r3, [pc, #152]
	movs	r2, #0
	mov	r0, fp
	mov	r9, r2
	mov	r8, r3
	bl	Func_08079338
	cmp	r0, #0
	beq.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	bl	Func_080795fc
	cmp	r9, r0
	bge.n	.L2
	ldr	r3, [pc, #124]
	movs	r2, #252
	lsls	r2, r2, #1
	adds	r6, r3, r2
	adds	r5, r0, #0
.L5:
	ldrb	r0, [r6, #0]
	bl	Func_08077394
	movs	r2, #140
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrb	r3, [r0, r3]
	cmp	r3, #9
	bhi.n	.L3
	adds	r0, r0, r2
	movs	r1, #0
	movs	r2, #3
.L4:
	ldrb	r3, [r0, #0]
	subs	r2, #1
	adds	r0, #1
	adds	r1, r1, r3
	cmp	r2, #0
	bge.n	.L4
	cmp	r8, r1
	ble.n	.L3
	ldrb	r3, [r6, #0]
	mov	r8, r1
	mov	r9, r3
.L3:
	subs	r5, #1
	adds	r6, #1
	cmp	r5, #0
	bne.n	.L5
.L2:
	ldr	r2, [pc, #56]
	cmp	r8, r2
	bne.n	.L6
	movs	r0, #2
	negs	r0, r0
	b.n	.L1
.L6:
	adds	r1, r7, #0
	mov	r2, sl
	mov	r0, r9
	bl	Func_0807a1b4
	adds	r1, r7, #0
	mov	r2, sl
	mov	r0, r9
	bl	Func_0807a458
	mov	r0, fp
	bl	Func_08079358
	mov	r0, r9
.L1:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000003e7
	.4byte 0x02000240
