@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08005c68
	.thumb_func
Func_08005c68:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #120]
	movs	r2, #130
	ldr	r3, [r3, #0]
	lsls	r2, r2, #5
	sub	sp, #4
	adds	r5, r3, r2
	movs	r6, #0
	movs	r3, #0
	mov	r8, r3
	mov	r7, sp
	mov	sl, r6
.L3:
	mov	r2, sl
	str	r2, [r7, #0]
	ldr	r3, [pc, #100]
	adds	r0, r7, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #96]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	bl	Func_08005b24
	cmp	r0, #15
	bhi.n	.L0
	lsls	r0, r0, #16
	movs	r3, #64
	lsrs	r0, r0, #16
	movs	r1, #0
	adds	r2, r5, #0
	bl	Func_08006ba8
	movs	r3, #1
	add	r8, r3
.L0:
	adds	r0, r6, #3
	bl	Func_08005b24
	cmp	r0, #15
	bhi.n	.L1
	lsls	r0, r0, #16
	adds	r2, r5, #0
	movs	r1, #136
	lsrs	r0, r0, #16
	adds	r2, #56
	lsls	r1, r1, #1
	movs	r3, #4
	bl	Func_08006ba8
	b.n	.L2
.L1:
	mov	r2, sl
	str	r2, [r5, #56]
.L2:
	adds	r6, #1
	adds	r5, #64
	cmp	r6, #2
	bls.n	.L3
	mov	r0, r8
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f1c
	.4byte 0x040000d4
	.4byte 0x85000010
