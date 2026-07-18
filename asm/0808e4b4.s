@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808e4b4
	.thumb_func
Func_0808e4b4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r2, #0
	ldr	r3, [pc, #136]
	ldr	r5, [pc, #140]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r5, r5, r2
	ldr	r3, [r3, #0]
	mov	r9, r0
	ldr	r0, [r5, #0]
	sub	sp, #12
	ldr	r7, [r3, #16]
	mov	sl, r1
	bl	Func_0808ba1c
	ldrh	r0, [r0, #6]
	str	r0, [sp, #8]
	mov	r1, sl
	ldr	r0, [r5, #0]
	bl	Func_0808df1c
	movs	r3, #0
	str	r0, [r6, #0]
	mov	r8, r0
	mov	fp, r3
	bl	Func_0808bd24
	ldr	r2, [pc, #96]
	str	r0, [sp, #4]
	cmp	r9, r2
	bne.n	.L0
	movs	r3, #1
	mov	fp, r3
.L0:
	movs	r2, #1
	ldr	r1, [r7, #0]
	negs	r2, r2
	cmp	r1, r2
	beq.n	.L1
.L9:
	movs	r3, #4
	ldrsh	r5, [r7, r3]
	movs	r3, #240
	lsls	r3, r3, #8
	ldrh	r2, [r7, #4]
	ands	r5, r3
	ldr	r3, [pc, #48]
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r6, r3, #16
	movs	r3, #15
	movs	r4, #255
	ands	r3, r1
	ands	r4, r2
	cmp	r3, #5
	bne.n	.L2
	movs	r2, #6
	ldrsh	r0, [r7, r2]
	str	r4, [sp, #0]
	bl	Func_0808d428
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L2
	cmp	r6, #0
	beq.n	.L3
	ldr	r2, [sp, #8]
	subs	r3, r5, r2
	ldr	r2, [pc, #20]
	adds	r3, r3, r2
	b.n	.L4
	movs	r0, r0
	.4byte 0x00000800
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x70000005
	.4byte 0x000017ff
.L4:
	lsls	r3, r3, #16
	ldr	r2, [pc, #108]
	lsrs	r3, r3, #16
	cmp	r3, r2
	bhi.n	.L2
.L3:
	ldrb	r0, [r7, #1]
	str	r4, [sp, #0]
	bl	Func_08077080
	ldrb	r3, [r0, #12]
	ldr	r4, [sp, #0]
	cmp	r3, sl
	bne.n	.L2
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L5
	ldr	r1, [r7, #0]
	ldr	r3, [pc, #80]
	ands	r3, r1
	cmp	r3, r9
	bne.n	.L2
	b.n	.L6
.L5:
	ldr	r1, [r7, #0]
.L6:
	movs	r3, #128
	ands	r3, r1
	adds	r0, r7, #0
	cmp	r3, #0
	bne.n	.L7
	movs	r3, #16
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L8
	cmp	r4, r8
	bne.n	.L2
	b.n	.L7
.L8:
	ldr	r2, [sp, #4]
	adds	r0, r7, #0
	cmp	r4, r2
	beq.n	.L7
.L2:
	adds	r7, #12
	ldr	r3, [r7, #0]
	movs	r2, #1
	negs	r2, r2
	adds	r1, r3, #0
	cmp	r3, r2
	bne.n	.L9
.L1:
	movs	r0, #0
.L7:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00002ffe
	.4byte 0x7000000f
