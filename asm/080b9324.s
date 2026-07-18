@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b9324
	.thumb_func
Func_080b9324:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	movs	r2, #0
	str	r0, [sp, #16]
	str	r2, [sp, #12]
	ldr	r3, [pc, #240]
	ldr	r3, [r3, #0]
	adds	r3, #69
	str	r3, [sp, #8]
	ldrb	r3, [r3, #0]
	movs	r0, #0
	cmp	r3, #1
	bne.n	.L0
	b.n	.L1
.L0:
	mov	r3, sp
	adds	r3, #20
	movs	r0, #2
	adds	r1, r3, #0
	str	r3, [sp, #4]
	bl	Func_080b6b40
	movs	r4, #31
	mov	r8, r0
	ldr	r6, [sp, #4]
	cmp	r0, #0
	bne.n	.L2
	movs	r0, #0
	b.n	.L1
.L2:
	str	r4, [sp, #0]
	bl	Func_08004458
	mov	r5, r8
	muls	r5, r0
	bl	Func_08004458
	mov	r2, r8
	muls	r2, r0
	lsrs	r5, r5, #16
	lsrs	r2, r2, #16
	lsls	r5, r5, #1
	lsls	r2, r2, #1
	ldr	r4, [sp, #0]
	ldrh	r1, [r6, r5]
	ldrh	r3, [r6, r2]
	subs	r4, #1
	strh	r3, [r6, r5]
	strh	r1, [r6, r2]
	cmp	r4, #0
	bge.n	.L2
	ldr	r2, [sp, #8]
	ldrb	r3, [r2, #0]
	cmp	r3, #2
	bne.n	.L3
	bl	Func_08004458
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	adds	r3, #1
	cmp	r3, #1
	bgt.n	.L4
	movs	r3, #2
.L4:
	cmp	r3, r8
	bge.n	.L3
	mov	r8, r3
.L3:
	movs	r4, #0
	cmp	r4, r8
	bge.n	.L5
.L12:
	ldr	r2, [sp, #4]
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	mov	r9, r3
	mov	r0, r9
	str	r4, [sp, #0]
	bl	Func_08077008
	movs	r3, #67
	adds	r6, r0, #0
	adds	r3, r3, r6
	mov	sl, r3
	ldrb	r3, [r3, #0]
	movs	r7, #0
	ldr	r4, [sp, #0]
	cmp	r7, r3
	bge.n	.L6
	movs	r2, #64
	adds	r2, r2, r6
	mov	fp, r2
	ldr	r2, [sp, #12]
	lsls	r3, r2, #4
	ldr	r2, [sp, #16]
	adds	r5, r3, r2
.L11:
	mov	r3, r9
	strh	r3, [r5, #0]
	mov	r3, fp
	ldrh	r2, [r3, #0]
	strh	r2, [r5, #4]
	cmp	r7, #0
	beq.n	.L7
	lsls	r2, r2, #16
	asrs	r3, r2, #16
	lsrs	r2, r2, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	strh	r3, [r5, #4]
.L7:
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L8
	subs	r2, #1
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L9
.L8:
	movs	r3, #8
	strh	r3, [r5, #6]
	ldr	r3, [pc, #8]
	strh	r3, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #1
	strh	r3, [r5, #10]
	b.n	.L10
	.4byte 0x00000000
	.4byte 0x03001e74
.L9:
	adds	r0, r5, #0
	movs	r1, #0
	str	r4, [sp, #0]
	bl	Func_080bd424
	ldr	r4, [sp, #0]
.L10:
	ldr	r2, [sp, #12]
	adds	r2, #1
	str	r2, [sp, #12]
	ldr	r2, [sp, #8]
	ldrb	r3, [r2, #0]
	adds	r5, #16
	cmp	r3, #2
	beq.n	.L6
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r7, #1
	cmp	r7, r3
	blt.n	.L11
.L6:
	adds	r4, #1
	cmp	r4, r8
	blt.n	.L12
.L5:
	ldr	r0, [sp, #12]
.L1:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
