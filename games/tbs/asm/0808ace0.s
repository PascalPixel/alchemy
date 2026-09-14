@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808ace0
	.thumb_func
Func_0808ace0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #236]
	ldr	r2, [pc, #236]
	ldr	r3, [r3, #0]
	movs	r1, #224
	lsls	r1, r1, #1
	mov	sl, r3
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r4, [r3, r1]
	mov	fp, r4
	movs	r4, #225
	lsls	r4, r4, #1
	adds	r3, r2, r4
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	movs	r6, #208
	sub	sp, #8
	lsls	r6, r6, #1
	movs	r3, #0
	mov	r9, r1
	add	r6, sl
	ldr	r5, [pc, #200]
	movs	r7, #0
	str	r3, [sp, #4]
	cmp	r0, #0
	beq.n	.L0
	movs	r1, #1
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	negs	r1, r1
	ldrh	r2, [r5, #0]
	cmp	r3, r1
	beq.n	.L0
	mov	r8, r1
	ldr	r1, [pc, #180]
.L4:
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	cmp	r3, fp
	bne.n	.L1
	movs	r0, #2
	ldrsh	r3, [r5, r0]
	cmp	r3, r8
	beq.n	.L2
	cmp	r3, r9
	bne.n	.L1
.L2:
	ldrh	r0, [r5, #4]
	adds	r3, r1, #0
	ands	r3, r0
	cmp	r3, r1
	beq.n	.L3
	lsls	r0, r0, #17
	asrs	r0, r0, #17
	str	r1, [sp, #0]
	bl	Func_080770c0
	ldr	r1, [sp, #0]
	cmp	r0, #0
	bne.n	.L1
.L3:
	ldrb	r3, [r5, #5]
	lsls	r3, r3, #24
	asrs	r3, r3, #31
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	movs	r1, #6
	ldrsh	r7, [r5, r1]
	str	r3, [sp, #4]
	b.n	.L0
.L1:
	adds	r5, #8
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	ldrh	r2, [r5, #0]
	cmp	r3, r8
	bne.n	.L4
.L0:
	movs	r3, #0
	strb	r3, [r6, #0]
	adds	r6, #1
	movs	r3, #0
.L6:
	strb	r7, [r6, #0]
	adds	r6, #1
	cmp	r7, #0
	beq.n	.L5
	adds	r7, #1
.L5:
	adds	r3, #1
	cmp	r3, #6
	bls.n	.L6
	ldr	r0, [sp, #4]
	cmp	r0, #0
	beq.n	.L7
	ldr	r0, [pc, #76]
	movs	r2, #208
	add	r0, sl
	ldrb	r3, [r0, #0]
	lsls	r2, r2, #1
	add	r2, sl
	movs	r1, #0
	strb	r3, [r2, #0]
	strb	r1, [r0, #0]
.L7:
	movs	r2, #212
	lsls	r2, r2, #1
	add	r2, sl
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #214
	lsls	r2, r2, #1
	movs	r3, #128
	add	r2, sl
	lsls	r3, r3, #13
	str	r3, [r2, #0]
	bl	Func_0808b25c
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0809d170
	.4byte 0x00007fff
	.4byte 0x000001a1
