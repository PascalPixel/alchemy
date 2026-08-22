@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808a5f8
	.thumb_func
Func_0808a5f8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #200]
	mov	r8, r0
	movs	r0, #224
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r1, #0
	ldrsh	r7, [r3, r1]
	ldr	r3, [pc, #188]
	ldr	r0, [r3, #20]
	bl	Func_080072e4
	ldr	r1, [pc, #184]
	movs	r2, #0
	adds	r6, r0, #0
	mov	r9, r2
	cmp	r8, r1
	beq.n	.L0
	ldmia	r6!, {r5}
	ldr	r3, [pc, #176]
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L1
	ldr	r3, [pc, #172]
	ands	r5, r3
	ldr	r3, [pc, #172]
	cmp	r5, r3
	beq.n	.L2
.L9:
	cmp	r5, r7
	bne.n	.L1
	mov	sl, r6
	b.n	.L3
.L8:
	cmp	r2, #255
	beq.n	.L4
	cmp	r2, r8
	bne.n	.L3
.L4:
	cmp	r0, #0
	beq.n	.L5
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L3
.L5:
	adds	r1, r5, #0
	mov	r9, r7
	b.n	.L2
.L3:
	ldmia	r6!, {r3}
	movs	r2, #255
	lsls	r2, r2, #12
	ands	r2, r3
	lsrs	r7, r2, #12
	movs	r0, #128
	movs	r2, #255
	ldr	r5, [pc, #112]
	lsls	r2, r2, #20
	lsls	r0, r0, #21
	ands	r2, r3
	ands	r0, r3
	ands	r5, r3
	lsrs	r2, r2, #20
	cmp	r0, #0
	beq.n	.L6
	ldmia	r6!, {r0}
.L6:
	ldr	r3, [pc, #96]
	cmp	r5, r3
	beq.n	.L7
	cmp	r2, #0
	bne.n	.L8
.L7:
	mov	r0, sl
	ldr	r1, [r0, #0]
	ands	r1, r3
	b.n	.L2
.L1:
	ldmia	r6!, {r5}
	ldr	r3, [pc, #68]
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L1
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #68]
	ands	r5, r3
	cmp	r5, r2
	bne.n	.L9
.L2:
	ldr	r3, [pc, #48]
	cmp	r1, r3
	beq.n	.L0
	ldr	r2, [pc, #32]
	movs	r0, #224
	lsls	r0, r0, #1
	adds	r3, r2, r0
	strh	r1, [r3, #0]
	movs	r1, #225
	lsls	r1, r1, #1
	adds	r3, r2, r1
	mov	r2, r9
	strh	r2, [r3, #0]
.L0:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x02008000
	.4byte 0x000003e7
	.4byte 0xfffff000
	.4byte 0x00000fff
	.4byte 0x000001ff
