@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080114a0
	.thumb_func
Func_080114a0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #176]
	ldr	r3, [r3, #0]
	sub	sp, #20
	movs	r1, #0
	str	r3, [sp, #16]
	str	r1, [sp, #12]
	str	r1, [sp, #8]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	ldmia	r3!, {r2}
	str	r2, [sp, #12]
	ldr	r3, [r3, #4]
	str	r3, [sp, #8]
.L0:
	ldr	r1, [sp, #12]
	movs	r2, #255
	lsls	r2, r2, #24
	adds	r3, r1, r2
	ldr	r1, [sp, #8]
	ldr	r2, [pc, #140]
	asrs	r3, r3, #25
	str	r3, [sp, #12]
	adds	r3, r1, r2
	asrs	r3, r3, #25
	str	r3, [sp, #8]
	movs	r3, #0
	str	r3, [sp, #4]
	mov	r9, r3
.L4:
	ldr	r2, [sp, #4]
	movs	r1, #0
	ldr	r6, [sp, #8]
	mov	sl, r1
	mov	fp, r2
.L3:
	adds	r3, r6, #0
	movs	r1, #15
	ands	r3, r1
	movs	r5, #0
	mov	r8, r6
	lsls	r7, r3, #4
.L2:
	ldr	r2, [sp, #12]
	adds	r1, r2, r5
	adds	r3, r1, #0
	movs	r2, #15
	ands	r3, r2
	adds	r4, r7, r3
	movs	r2, #156
	lsls	r3, r4, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #16]
	ldrh	r4, [r2, r3]
	movs	r3, #0
	add	r4, fp
	str	r3, [sp, #0]
	mov	r0, r9
	mov	r2, r8
	adds	r3, r4, #0
	bl	Func_080108e4
	cmp	r0, #0
	bne.n	.L1
	adds	r5, #1
	cmp	r5, #1
	bls.n	.L2
	movs	r3, #1
	add	sl, r3
	mov	r1, sl
	adds	r6, #1
	cmp	r1, #1
	bls.n	.L3
	ldr	r2, [sp, #4]
	movs	r3, #160
	lsls	r3, r3, #1
	movs	r1, #1
	adds	r2, r2, r3
	add	r9, r1
	str	r2, [sp, #4]
	mov	r2, r9
	cmp	r2, #1
	bls.n	.L4
.L1:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0xfec00000
