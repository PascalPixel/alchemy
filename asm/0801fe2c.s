@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801fe2c
	.thumb_func
Func_0801fe2c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r2, [sp, #8]
	str	r1, [sp, #12]
	mov	sl, r3
	ldr	r3, [pc, #200]
	movs	r2, #0
	ldr	r1, [r3, #0]
	str	r2, [sp, #4]
	movs	r3, #44
	mov	r2, sl
	ldrsb	r3, [r2, r3]
	movs	r2, #1
	negs	r2, r2
	mov	r9, r0
	cmp	r3, r2
	beq.n	.L0
	mov	r3, sl
	adds	r3, #51
	movs	r2, #154
	lsls	r2, r2, #1
	str	r3, [sp, #0]
	movs	r3, #0
	adds	r7, r1, r2
	mov	fp, r3
	subs	r2, #32
	movs	r3, #44
	adds	r6, r1, r2
	mov	r8, r3
.L2:
	ldr	r3, [sp, #0]
	mov	r2, sl
	mov	r1, r8
	ldrsb	r0, [r1, r2]
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	bl	Func_0808a5f0
	bl	Func_08009030
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L1
	movs	r1, #1
	bl	Func_08009020
	adds	r2, r5, #0
	adds	r2, #38
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #13
	ldrb	r3, [r5, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
.L1:
	str	r5, [r6, #0]
	mov	r2, r9
	ldr	r1, [sp, #12]
	ldrh	r3, [r2, #12]
	adds	r3, r1, r3
	add	r3, fp
	lsls	r3, r3, #3
	adds	r3, #16
	strh	r3, [r7, #0]
	ldrh	r3, [r2, #14]
	ldr	r2, [sp, #8]
	adds	r3, r2, r3
	lsls	r3, r3, #3
	adds	r3, #16
	strh	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #64]
	ldr	r2, [sp, #4]
	movs	r3, #1
	movs	r1, #3
	adds	r2, #1
	add	r8, r3
	adds	r7, #2
	add	fp, r1
	adds	r6, #4
	str	r2, [sp, #4]
	cmp	r2, #3
	bgt.n	.L0
	mov	r1, r8
	mov	r2, sl
	ldrsb	r3, [r1, r2]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L2
.L0:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #28]
	bl	Func_080041d8
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x0801ff59
