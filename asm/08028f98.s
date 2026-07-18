@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08028f98
	.thumb_func
Func_08028f98:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #24
	add	r1, sp, #8
	mov	r9, r1
	movs	r7, #0
	mov	r2, r9
	strh	r7, [r2, #0]
	movs	r1, #228
	ldr	r2, [pc, #216]
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r6, [r3, r1]
	movs	r1, #229
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #10
	add	r1, sp
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	mov	sl, r1
	mov	r2, sl
	strh	r3, [r2, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #5
	movs	r2, #30
	movs	r0, #0
	bl	Func_080162d4
	mov	r2, sl
	adds	r1, r6, #0
	adds	r7, r0, #0
	bl	Func_08028ef0
	add	r3, sp, #12
	mov	r8, r3
	add	r1, sp, #4
	mov	r0, r8
	bl	Func_0801c0dc
	ldr	r2, [pc, #152]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L0
	adds	r5, r2, #0
.L1:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L1
.L0:
	adds	r1, r6, #0
	adds	r0, r7, #0
	mov	r2, sl
	mov	r3, r9
	bl	Func_08029094
	movs	r1, #1
	lsls	r0, r0, #16
	asrs	r5, r0, #16
	negs	r1, r1
	cmp	r5, r1
	bne.n	.L2
	ldr	r0, [sp, #4]
	bl	Func_0801c17c
	adds	r0, r7, #0
	movs	r1, #2
	bl	Func_08016418
	mov	r3, sl
	adds	r0, r6, #0
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl	Func_0808a238
	adds	r0, r5, #0
	b.n	.L3
.L2:
	movs	r1, #2
	negs	r1, r1
	cmp	r5, r1
	bne.n	.L4
	ldr	r0, [sp, #4]
	bl	Func_0801c17c
	adds	r0, r7, #0
	movs	r1, #2
	bl	Func_08016418
	adds	r0, r5, #0
	b.n	.L3
.L4:
	mov	r1, r9
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	lsls	r2, r3, #3
	subs	r2, r2, r3
	lsls	r2, r2, #1
	mov	r0, r8
	adds	r2, #60
	movs	r1, #74
	bl	Func_0801c154
	movs	r0, #1
	adds	r6, r5, #0
	bl	Func_080030f8
	b.n	.L0
.L3:
	add	sp, #24
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x03001ae8
