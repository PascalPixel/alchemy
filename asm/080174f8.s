@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080174f8
	.thumb_func
Func_080174f8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #148]
	ldr	r6, [r3, #0]
	ldr	r7, [r3, #88]
	ldr	r3, [pc, #144]
	adds	r5, r6, r3
	movs	r3, #2
	movs	r2, #0
	strb	r3, [r5, #0]
	movs	r1, #1
	mov	r8, r2
	sub	sp, #4
	bl	Func_08018038
	movs	r2, #1
	mov	r9, r2
	mov	r3, r9
	movs	r2, #235
	strb	r3, [r5, #0]
	lsls	r2, r2, #4
	lsls	r3, r0, #1
	adds	r3, r3, r2
	ldrh	r3, [r6, r3]
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [r7, #0]
	cmp	r0, #0
	bne.n	.L1
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #15
	movs	r2, #30
	movs	r3, #6
	movs	r0, #0
	bl	Func_080162d4
	mov	r3, r9
	adds	r5, r0, #0
	str	r5, [r7, #0]
	movs	r2, #30
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #15
	movs	r3, #6
	bl	Func_08017248
	mov	r2, r8
	str	r2, [r7, #8]
	b.n	.L2
.L1:
	adds	r5, r0, #0
.L2:
	cmp	r5, #0
	beq.n	.L0
	ldr	r2, [r7, #8]
	adds	r0, r5, #0
	mov	r1, sl
	bl	Func_08016670
	movs	r3, #0
	mov	r8, r0
	str	r0, [r7, #4]
	str	r3, [r7, #8]
	cmp	r0, #0
	bne.n	.L0
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08016418
.L0:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x00000ea5
