@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080113e4
	.thumb_func
Func_080113e4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #164]
	ldr	r3, [r3, #0]
	sub	sp, #16
	movs	r1, #0
	str	r3, [sp, #12]
	str	r1, [sp, #8]
	str	r1, [sp, #4]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	ldmia	r3!, {r2}
	str	r2, [sp, #8]
	ldr	r3, [r3, #4]
	str	r3, [sp, #4]
.L0:
	ldr	r1, [sp, #8]
	movs	r2, #255
	lsls	r2, r2, #24
	adds	r3, r1, r2
	ldr	r1, [sp, #4]
	ldr	r2, [pc, #128]
	asrs	r3, r3, #25
	str	r3, [sp, #8]
	adds	r3, r1, r2
	asrs	r3, r3, #25
	str	r3, [sp, #4]
	movs	r3, #0
	mov	r9, r3
	mov	fp, r3
.L3:
	movs	r1, #0
	ldr	r6, [sp, #4]
	mov	sl, r1
.L2:
	adds	r3, r6, #0
	movs	r2, #15
	ands	r3, r2
	movs	r5, #0
	mov	r8, r6
	lsls	r7, r3, #4
.L1:
	ldr	r3, [sp, #8]
	adds	r1, r3, r5
	adds	r3, r1, #0
	movs	r2, #15
	ands	r3, r2
	adds	r4, r7, r3
	movs	r2, #156
	lsls	r3, r4, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	ldrh	r4, [r2, r3]
	movs	r3, #1
	add	r4, fp
	str	r3, [sp, #0]
	mov	r0, r9
	mov	r2, r8
	adds	r3, r4, #0
	adds	r5, #1
	bl	Func_080108e4
	cmp	r5, #1
	bls.n	.L1
	movs	r3, #1
	add	sl, r3
	mov	r1, sl
	adds	r6, #1
	cmp	r1, #1
	bls.n	.L2
	movs	r2, #160
	add	r9, r3
	lsls	r2, r2, #1
	mov	r3, r9
	add	fp, r2
	cmp	r3, #1
	bls.n	.L3
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
	.4byte 0x03001e70
	.4byte 0xfec00000
