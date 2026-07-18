@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08003d28
	.thumb_func
Func_08003d28:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #172]
	movs	r2, #0
	ldrsh	r1, [r0, r2]
	ldrb	r3, [r3, #0]
	mov	r8, r1
	mov	r9, r3
	movs	r1, #2
	ldrsh	r3, [r0, r1]
	mov	r2, r9
	ldrh	r6, [r0, #4]
	mov	sl, r3
	movs	r0, #0
	cmp	r2, #31
	bhi.n	.L0
	mov	r3, r9
	lsls	r2, r3, #3
	ldr	r3, [pc, #144]
	adds	r7, r2, r3
	cmp	r8, sl
	beq.n	.L1
	mov	r1, r8
	negs	r3, r1
	cmp	r3, sl
	bne.n	.L2
.L1:
	cmp	r6, #0
	bne.n	.L2
	movs	r0, #128
	ldr	r3, [pc, #124]
	mov	r1, sl
	lsls	r0, r0, #9
	bl	Func_080072f0
	mov	r1, r8
	negs	r3, r1
	adds	r2, r0, #0
	cmp	r3, sl
	bne.n	.L3
	negs	r2, r0
.L3:
	lsls	r3, r2, #16
	lsrs	r3, r3, #16
	str	r3, [r7, #0]
	lsls	r3, r0, #16
	str	r3, [r7, #4]
	b.n	.L4
.L2:
	adds	r0, r6, #0
	bl	Func_08002322
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	Func_0800231c
	mov	r1, r8
	adds	r6, r0, #0
	bl	Func_080022ec
	mov	r1, r8
	strh	r0, [r7, #0]
	adds	r0, r5, #0
	bl	Func_080022ec
	adds	r7, #2
	negs	r5, r5
	strh	r0, [r7, #0]
	mov	r1, sl
	adds	r0, r5, #0
	bl	Func_080022ec
	adds	r7, #2
	strh	r0, [r7, #0]
	mov	r1, sl
	adds	r0, r6, #0
	bl	Func_080022ec
	adds	r7, #2
	strh	r0, [r7, #0]
.L4:
	ldr	r2, [pc, #20]
	mov	r3, r9
	adds	r3, #1
	strb	r3, [r2, #0]
	mov	r0, r9
.L0:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001d00
	.4byte 0x03001d40
	.4byte 0x03000380
