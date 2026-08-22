@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808f32c
	.thumb_func
Func_0808f32c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #324]
	ldr	r3, [r3, #0]
	movs	r1, #250
	ldr	r7, [r3, #16]
	ldr	r3, [pc, #320]
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl	Func_0808ba1c
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	str	r3, [sp, #4]
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	cmp	r7, #0
	bne.n	.L0
	b.n	.L1
.L0:
	b.n	.L2
.L12:
	ldrb	r3, [r7, #0]
	mov	sl, r3
	mov	r0, sl
	adds	r7, #1
	bl	Func_0808ed4c
	cmp	r0, #0
	bne.n	.L2
	mov	r3, sl
	subs	r3, #100
	cmp	r3, #139
	bhi.n	.L2
	ldr	r1, [sp, #4]
	subs	r3, r1, r5
	cmp	r3, #0
	blt.n	.L3
	cmp	r3, #8
	ble.n	.L4
	b.n	.L2
.L3:
	ldr	r2, [sp, #4]
	subs	r3, r5, r2
	cmp	r3, #8
	bgt.n	.L2
.L4:
	ldr	r1, [sp, #0]
	mov	r2, r8
	subs	r3, r1, r2
	cmp	r3, #0
	blt.n	.L5
	cmp	r3, #5
	ble.n	.L6
	b.n	.L2
.L5:
	ldr	r2, [sp, #0]
	mov	r1, r8
	subs	r3, r1, r2
	cmp	r3, #5
	bgt.n	.L2
.L6:
	ldr	r3, [pc, #216]
	ldr	r0, [r3, #36]
	bl	Func_080072e4
	adds	r6, r0, #0
	movs	r3, #1
	ldr	r2, [r6, #0]
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L2
	movs	r3, #128
	lsls	r5, r5, #20
	lsls	r3, r3, #12
	mov	fp, r5
	mov	r9, r3
.L10:
	movs	r1, #4
	ldrsh	r3, [r6, r1]
	cmp	r3, sl
	bne.n	.L7
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #3
	bne.n	.L7
	ldr	r3, [pc, #172]
	ldr	r2, [r6, #8]
	ands	r2, r3
	movs	r3, #128
	lsls	r3, r3, #14
	cmp	r2, r3
	beq.n	.L8
	cmp	r2, r3
	bhi.n	.L9
	cmp	r2, #0
	beq.n	.L8
	movs	r3, #128
	lsls	r3, r3, #13
	cmp	r2, r3
	beq.n	.L8
	b.n	.L7
.L9:
	movs	r1, #192
	lsls	r1, r1, #14
	cmp	r2, r1
	beq.n	.L8
	movs	r3, #160
	lsls	r3, r3, #15
	cmp	r2, r3
	bne.n	.L7
.L8:
	movs	r2, #1
	movs	r1, #6
	ldrsh	r0, [r6, r1]
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L7
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L7
	mov	r1, r8
	lsls	r3, r1, #20
	mov	r1, fp
	movs	r0, #22
	add	r1, r9
	movs	r2, #0
	add	r3, r9
	bl	Func_080090c8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L7
	ldr	r1, [pc, #84]
	bl	Func_08009098
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_080091e0
	ldr	r3, [pc, #76]
	str	r3, [r5, #108]
.L7:
	adds	r6, #12
	ldr	r3, [r6, #0]
	movs	r1, #1
	negs	r1, r1
	adds	r2, r3, #0
	cmp	r3, r1
	bne.n	.L10
.L2:
	ldrb	r5, [r7, #0]
	adds	r7, #1
	ldrb	r2, [r7, #0]
	adds	r7, #1
	mov	r8, r2
	cmp	r5, #255
	beq.n	.L11
	b.n	.L12
.L11:
	cmp	r2, #255
	beq.n	.L1
	b.n	.L12
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
	.4byte 0x03001e70
	.4byte 0x02000240
	.4byte 0x02008000
	.4byte 0xfff00000
	.4byte 0x0809e8a0
	.4byte 0x0808f28d
