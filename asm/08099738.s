@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08099738
	.thumb_func
Func_08099738:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #192]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	adds	r5, r0, #0
	ldr	r6, [r5, #80]
	ldr	r2, [r6, #40]
	movs	r0, #154
	mov	sl, r2
	bl	Func_080f9010
	ldr	r0, [pc, #168]
	bl	Func_08004278
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_08009080
	movs	r3, #0
	str	r3, [r5, #108]
	movs	r3, #37
	adds	r3, r3, r6
	adds	r6, #38
	mov	r9, r6
	mov	fp, r3
	movs	r1, #1
	movs	r7, #0
	mov	r6, fp
	mov	r8, r1
	mov	r5, r9
.L0:
	movs	r2, #7
	mov	r3, sl
	strb	r2, [r3, #5]
	mov	r1, r8
	movs	r3, #2
	strb	r1, [r6, #0]
	movs	r0, #2
	strb	r3, [r5, #0]
	bl	Func_080030f8
	mov	r2, r8
	movs	r3, #0
	strb	r2, [r6, #0]
	movs	r0, #2
	strb	r3, [r5, #0]
	adds	r7, #1
	bl	Func_080030f8
	cmp	r7, #4
	bls.n	.L0
	movs	r1, #0
	movs	r2, #7
	mov	r5, fp
	movs	r7, #0
	mov	r8, r1
	movs	r6, #1
	mov	fp, r2
.L1:
	mov	r1, sl
	mov	r3, fp
	strb	r3, [r1, #5]
	mov	r2, r8
	mov	r3, r9
	strb	r6, [r5, #0]
	movs	r0, #2
	strb	r2, [r3, #0]
	bl	Func_080030f8
	mov	r1, r8
	mov	r2, sl
	strb	r1, [r2, #5]
	strb	r6, [r5, #0]
	movs	r0, #2
	adds	r7, #1
	bl	Func_080030f8
	cmp	r7, #4
	bls.n	.L1
	mov	r1, r9
	movs	r3, #1
	strb	r3, [r1, #0]
	ldr	r3, [pc, #24]
	movs	r1, #147
	lsls	r1, r1, #2
	movs	r2, #0
	adds	r3, r3, r1
	strh	r2, [r3, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x08099679
