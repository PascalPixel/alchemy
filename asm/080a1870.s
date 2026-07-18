@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a1870
	.thumb_func
Func_080a1870:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #44
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	ldr	r3, [pc, #204]
	mov	r9, r0
	ldr	r3, [r3, #0]
	mov	r0, sp
	adds	r0, #16
	mov	sl, r3
	str	r0, [sp, #0]
	bl	Func_08077158
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	mov	r8, r0
	mov	r1, r8
	mov	r2, sl
	movs	r5, #0
	strb	r1, [r2, #30]
	cmp	r5, r8
	bge.n	.L0
	movs	r7, #154
	movs	r6, #138
	lsls	r7, r7, #1
	lsls	r6, r6, #1
	add	r7, sl
	add	r6, sl
	mov	fp, r5
.L2:
	ldr	r1, [sp, #0]
	mov	r3, fp
	ldrh	r0, [r3, r1]
	bl	Func_0808a288
	bl	Func_08009030
	cmp	r0, #0
	beq.n	.L1
	str	r0, [r6, #0]
	mov	r2, r9
	ldrh	r3, [r2, #12]
	ldr	r2, [sp, #4]
	adds	r2, #16
	muls	r2, r5
	ldr	r1, [sp, #12]
	adds	r3, r1, r3
	lsls	r3, r3, #3
	adds	r3, r3, r2
	strh	r3, [r7, #0]
	mov	r2, r9
	ldr	r1, [sp, #8]
	ldrh	r3, [r2, #14]
	adds	r3, r1, r3
	lsls	r3, r3, #3
	adds	r3, #16
	strh	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #64]
	movs	r1, #13
	ldrb	r3, [r0, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	adds	r2, r0, #0
	strb	r3, [r0, #9]
	adds	r2, #38
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #1
	bl	Func_08009020
.L1:
	movs	r2, #2
	adds	r5, #1
	adds	r7, #2
	adds	r6, #4
	add	fp, r2
	cmp	r5, r8
	blt.n	.L2
.L0:
	cmp	r5, #7
	bgt.n	.L3
	lsls	r3, r5, #2
	movs	r0, #138
	add	r3, sl
	lsls	r0, r0, #1
	adds	r2, r3, r0
	movs	r3, #8
	movs	r1, #0
	subs	r5, r3, r5
.L4:
	subs	r5, #1
	stmia	r2!, {r1}
	cmp	r5, #0
	bne.n	.L4
.L3:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #24]
	bl	Func_080041d8
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x080a19a1
