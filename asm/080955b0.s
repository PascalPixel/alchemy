@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080955b0
	.thumb_func
Func_080955b0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	ldr	r3, [pc, #192]
	movs	r0, #183
	lsls	r0, r0, #1
	adds	r5, r1, #0
	mov	r8, r2
	ldr	r7, [r3, #0]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L0
	lsls	r0, r5, #2
	adds	r0, r0, r5
	lsls	r0, r0, #2
	add	r0, r8
	movs	r2, #0
	adds	r0, #48
	mov	sl, r2
	bl	Func_080770c8
	b.n	.L1
.L0:
	adds	r0, r5, #0
	mov	r1, r8
	bl	Func_08077288
	mov	sl, r0
.L1:
	mov	r3, sl
	cmp	r3, #0
	blt.n	.L2
	bl	Func_080916b0
	bl	Func_0808c44c
	movs	r2, #1
	negs	r2, r2
	cmp	r6, r2
	beq.n	.L3
	movs	r2, #207
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L4
	bl	Func_0808b8e8
.L4:
	cmp	r5, #0
	bne.n	.L5
	adds	r0, r6, #0
	bl	Func_08096140
	b.n	.L6
.L5:
	cmp	r5, #1
	bne.n	.L7
	adds	r0, r6, #0
	bl	Func_080965a8
	b.n	.L6
.L7:
	cmp	r5, #2
	bne.n	.L8
	adds	r0, r6, #0
	bl	Func_08095dd0
	b.n	.L6
.L8:
	cmp	r5, #3
	bne.n	.L6
	adds	r0, r6, #0
	bl	Func_08095a44
.L6:
	lsls	r3, r6, #2
	adds	r3, #20
	movs	r2, #0
	str	r2, [r7, r3]
	movs	r2, #207
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L3
	bl	Func_0808b98c
.L3:
	mov	r0, sl
	adds	r1, r5, #0
	mov	r2, r8
	bl	Func_08015358
	bl	Func_0808c4c0
	bl	Func_08091750
.L2:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
