@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808b25c
	.thumb_func
Func_0808b25c:
	push	{r5, r6, lr}
	ldr	r2, [pc, #68]
	movs	r3, #224
	mov	ip, r2
	lsls	r3, r3, #1
	ldr	r4, [pc, #64]
	add	r3, ip
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldmia	r4!, {r2}
	cmp	r2, #0
	beq.n	.L0
	cmp	r2, r0
	beq.n	.L0
	movs	r6, #128
	ldr	r5, [pc, #48]
	lsls	r6, r6, #24
.L2:
	adds	r3, r2, #0
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L1
	adds	r1, r5, #0
	ands	r1, r2
.L1:
	ldmia	r4!, {r2}
	cmp	r2, #0
	beq.n	.L0
	cmp	r2, r0
	bne.n	.L2
.L0:
	movs	r3, #235
	lsls	r3, r3, #1
	add	r3, ip
	strh	r1, [r3, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x0809e270
	.4byte 0x0000ffff
