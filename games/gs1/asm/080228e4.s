@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080228e4
	.thumb_func
Func_080228e4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	mov	r8, r1
	movs	r1, #0
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	str	r1, [sp, #4]
	str	r1, [sp, #0]
	mov	r2, r8
	ldrh	r3, [r2, #0]
	mov	sl, r0
	mov	r9, r1
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [pc, #104]
	ldr	r6, [sp, #12]
	mov	fp, r3
	mov	r5, r8
	subs	r6, #2
.L5:
	ldrh	r0, [r5, #0]
	bl	Func_08077080
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	ldrh	r2, [r5, #0]
	mov	r3, fp
	ands	r3, r2
	strh	r3, [r6, #2]
	movs	r1, #1
	add	r9, r1
	mov	r1, sl
	ldrh	r2, [r1, #0]
	ldrh	r3, [r5, #0]
	eors	r3, r2
	mov	r2, fp
	ands	r3, r2
	adds	r6, #2
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L2
	ldr	r7, [pc, #40]
	adds	r4, r5, #0
.L3:
	adds	r0, #1
	cmp	r0, #31
	bgt.n	.L2
	adds	r1, #4
	ldrh	r3, [r4, #0]
	ldrh	r2, [r1, #0]
	eors	r3, r2
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L3
.L2:
	cmp	r0, #32
	bne.n	.L1
	ldr	r3, [sp, #4]
	adds	r3, #1
	str	r3, [sp, #4]
	ldr	r2, [pc, #8]
	ldrh	r3, [r6, #0]
	orrs	r3, r2
	b.n	.L4
	.4byte 0x00003fff
	.4byte 0x00008000
	.4byte 0x00003fff
.L4:
	strh	r3, [r6, #0]
.L1:
	mov	r3, r8
	adds	r5, #4
	adds	r3, #124
	cmp	r5, r3
	bgt.n	.L0
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L5
.L0:
	mov	r1, sl
	ldrh	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L6
	mov	r2, r9
	lsls	r3, r2, #1
	ldr	r1, [sp, #12]
	ldr	r2, [pc, #88]
	mov	r5, sl
	adds	r7, r3, r1
	mov	fp, r2
.L11:
	ldrh	r0, [r5, #0]
	bl	Func_08077080
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	mov	r1, r8
	ldrh	r2, [r1, #0]
	ldrh	r3, [r5, #0]
	eors	r3, r2
	mov	r2, fp
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L8
	ldr	r6, [pc, #40]
	adds	r4, r5, #0
.L9:
	adds	r0, #1
	cmp	r0, #31
	bgt.n	.L8
	adds	r1, #4
	ldrh	r3, [r4, #0]
	ldrh	r2, [r1, #0]
	eors	r3, r2
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L9
.L8:
	cmp	r0, #32
	bne.n	.L7
	ldr	r3, [sp, #0]
	adds	r3, #1
	str	r3, [sp, #0]
	ldrh	r3, [r5, #0]
	ldr	r2, [pc, #8]
	mov	r1, fp
	b.n	.L10
	.4byte 0x00003fff
	.4byte 0x00004000
	.4byte 0x00003fff
.L10:
	ands	r3, r1
	orrs	r3, r2
	movs	r2, #1
	strh	r3, [r7, #0]
	add	r9, r2
	adds	r7, #2
.L7:
	mov	r3, sl
	adds	r5, #4
	adds	r3, #124
	cmp	r5, r3
	bgt.n	.L6
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L11
.L6:
	ldr	r3, [sp, #4]
	ldr	r1, [sp, #8]
	str	r3, [r1, #0]
	ldr	r2, [sp, #0]
	ldr	r3, [sp, #48]
	mov	r0, r9
	str	r2, [r3, #0]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
