@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080cd104
	.thumb_func
Func_080cd104:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #136
	str	r1, [sp, #4]
	ldr	r3, [pc, #320]
	ldr	r1, [r3, #0]
	subs	r3, #4
	ldr	r3, [r3, #0]
	add	r2, sp, #8
	str	r3, [sp, #0]
	mov	fp, r2
	add	r3, sp, #136
	adds	r7, r0, #0
	mov	r9, r1
	movs	r6, #63
	mov	r5, fp
	mov	r8, r3
.L0:
	bl	Func_08004458
	ands	r0, r6
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r8
	bne.n	.L0
	cmp	r7, #1
	bne.n	.L1
	movs	r1, #0
	movs	r2, #1
	mov	r8, r1
	mov	sl, r2
	movs	r5, #0
.L8:
	add	r8, sl
	movs	r3, #1
	add	sl, r3
	cmp	r5, r8
	beq.n	.L2
	ldr	r1, [sp, #4]
	mov	ip, fp
	movs	r4, #7
	subs	r7, r3, r1
.L7:
	movs	r6, #0
	mov	r0, ip
.L6:
	ldrb	r3, [r0, #0]
	subs	r1, r5, r3
	adds	r0, #1
	cmp	r1, #0
	blt.n	.L3
	cmp	r1, #127
	bgt.n	.L3
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L4
	adds	r2, r6, #7
.L4:
	asrs	r2, r2, #3
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L5
	adds	r3, r1, #7
.L5:
	asrs	r3, r3, #3
	lsls	r2, r2, #4
	adds	r2, r2, r3
	adds	r3, r6, #0
	ands	r3, r4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ands	r1, r4
	lsls	r2, r2, #3
	adds	r2, r2, r1
	mov	r3, r9
	strb	r7, [r3, r2]
.L3:
	adds	r6, #1
	cmp	r6, #128
	bne.n	.L6
	adds	r5, #1
	cmp	r5, r8
	bne.n	.L7
.L2:
	ldr	r1, [sp, #0]
	ldr	r3, [pc, #176]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #128
	lsls	r1, r1, #1
	cmp	r8, r1
	ble.n	.L8
	b.n	.L9
.L1:
	movs	r2, #0
	movs	r3, #1
	mov	r8, r2
	mov	sl, r3
	movs	r6, #0
.L16:
	mov	r1, sl
	lsrs	r3, r1, #31
	add	r3, sl
	asrs	r3, r3, #1
	movs	r2, #4
	add	r8, r3
	add	sl, r2
	cmp	r6, r8
	beq.n	.L10
	ldr	r1, [sp, #4]
	movs	r3, #1
	mov	ip, fp
	movs	r4, #7
	subs	r7, r3, r1
.L15:
	movs	r5, #0
	mov	r0, ip
.L14:
	ldrb	r3, [r0, #0]
	subs	r1, r6, r3
	adds	r0, #1
	cmp	r1, #0
	blt.n	.L11
	cmp	r1, #127
	bgt.n	.L11
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L12
	adds	r2, r1, #7
.L12:
	asrs	r2, r2, #3
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L13
	adds	r3, r5, #7
.L13:
	asrs	r3, r3, #3
	lsls	r2, r2, #4
	adds	r2, r2, r3
	ands	r1, r4
	lsls	r2, r2, #3
	adds	r2, r2, r1
	adds	r3, r5, #0
	ands	r3, r4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	mov	r3, r9
	strb	r7, [r3, r2]
.L11:
	adds	r5, #1
	cmp	r5, #128
	bne.n	.L14
	adds	r6, #1
	cmp	r6, r8
	bne.n	.L15
.L10:
	ldr	r1, [sp, #0]
	ldr	r3, [pc, #40]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	mov	r1, r8
	cmp	r1, #191
	ble.n	.L16
.L9:
	add	sp, #136
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ef0
	.4byte 0x00007824
