@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808b090
	.thumb_func
Func_0808b090:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r4, [pc, #172]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r3, r4, r2
	movs	r1, #18
	mov	sl, r1
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	mov	fp, r1
	movs	r1, #225
	lsls	r1, r1, #1
	ldr	r5, [pc, #152]
	adds	r3, r4, r1
	adds	r1, #10
	movs	r2, #0
	ldrsh	r7, [r3, r2]
	ldrh	r0, [r5, #0]
	adds	r3, r4, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	adds	r1, r0, #0
	mov	r9, r2
	lsls	r3, r1, #16
	movs	r2, #1
	asrs	r3, r3, #16
	negs	r2, r2
	cmp	r3, r2
	beq.n	.L0
	ldr	r6, [pc, #124]
	mov	r8, r2
.L6:
	ldrb	r2, [r5, #3]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	cmp	r3, fp
	bne.n	.L2
	b.n	.L3
.L1:
	lsls	r3, r0, #16
	asrs	r3, r3, #16
	cmp	r3, r9
	bne.n	.L2
.L3:
	ldrh	r2, [r5, #2]
	adds	r3, r6, #0
	ands	r3, r2
	cmp	r3, r6
	beq.n	.L4
	lsls	r3, r2, #17
	asrs	r3, r3, #17
	cmp	r3, r7
	bne.n	.L2
.L4:
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	cmp	r0, r8
	beq.n	.L5
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L2
.L5:
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	ldr	r4, [pc, #44]
	mov	sl, r3
	b.n	.L0
.L2:
	adds	r5, #8
	ldrh	r1, [r5, #0]
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	adds	r0, r1, #0
	cmp	r3, r8
	bne.n	.L6
	ldr	r4, [pc, #24]
.L0:
	movs	r2, #248
	lsls	r2, r2, #1
	adds	r3, r4, r2
	mov	r1, sl
	strh	r1, [r3, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x0809d9f0
	.4byte 0x00007fff
