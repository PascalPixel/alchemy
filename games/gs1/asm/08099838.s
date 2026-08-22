@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08099838
	.thumb_func
Func_08099838:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #200]
	ldr	r3, [r3, #0]
	sub	sp, #8
	str	r3, [sp, #4]
	movs	r2, #250
	ldr	r3, [pc, #192]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	adds	r5, r0, #0
	ldr	r6, [r5, #80]
	ldr	r3, [r6, #40]
	movs	r0, #130
	mov	r9, r3
	bl	Func_080f9010
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_08009080
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #37
	adds	r6, #38
	str	r3, [r5, #108]
	mov	fp, r6
	mov	r8, r3
	str	r2, [sp, #0]
	movs	r3, #1
	adds	r6, r2, #0
	mov	sl, r3
	mov	r5, fp
.L0:
	mov	r3, r9
	movs	r2, #7
	strb	r2, [r3, #5]
	movs	r7, #2
	mov	r2, sl
	strb	r2, [r6, #0]
	movs	r0, #2
	strb	r7, [r5, #0]
	bl	Func_080030f8
	mov	r3, sl
	movs	r2, #0
	strb	r3, [r6, #0]
	movs	r0, #2
	strb	r2, [r5, #0]
	bl	Func_080030f8
	movs	r3, #1
	add	r8, r3
	mov	r2, r8
	cmp	r2, #9
	bls.n	.L0
	movs	r3, #0
	mov	r8, r3
	mov	r2, r8
	mov	r3, r9
	strb	r2, [r3, #5]
	mov	r2, fp
	strb	r7, [r2, #0]
	ldr	r3, [sp, #0]
	ldr	r5, [pc, #80]
	movs	r6, #1
	movs	r1, #200
	strb	r6, [r3, #0]
	lsls	r1, r1, #4
	adds	r0, r5, #0
	bl	Func_080041d8
	ldr	r3, [pc, #60]
	movs	r2, #147
	lsls	r2, r2, #2
	adds	r3, r3, r2
	strh	r6, [r3, #0]
	bl	Func_080072f8
	movs	r2, #191
	ldr	r3, [sp, #4]
	lsls	r2, r2, #1
	adds	r5, r3, r2
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	ldr	r2, [pc, #40]
	cmp	r3, r2
	bne.n	.L1
	bl	Func_08099738
	mov	r3, r8
	strh	r3, [r5, #0]
.L1:
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
	.4byte 0x08099679
	.4byte 0x00002092
