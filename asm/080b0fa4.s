@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b0fa4
	.thumb_func
Func_080b0fa4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r1, [sp, #8]
	ldr	r3, [pc, #264]
	movs	r2, #155
	ldr	r5, [r3, #0]
	lsls	r2, r2, #2
	adds	r7, r5, r2
	ldr	r2, [pc, #256]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r9, r0
	adds	r0, r1, #0
	movs	r1, #7
	str	r3, [sp, #4]
	bl	Func_080022fc
	ldr	r3, [sp, #8]
	mov	r2, r9
	subs	r6, r3, r0
	cmp	r2, #0
	beq.n	.L0
	mov	r0, r9
	bl	Func_08015060
	cmp	r6, #0
	beq.n	.L1
	ldr	r2, [pc, #220]
	adds	r3, r5, r2
	ldrh	r0, [r3, #0]
	movs	r3, #16
	negs	r3, r3
	movs	r1, #128
	str	r3, [sp, #0]
	mov	r2, r9
	movs	r3, #216
	lsls	r1, r1, #23
	bl	Func_080150c8
	movs	r2, #0
	movs	r3, #17
	strb	r2, [r0, #4]
	strb	r3, [r0, #5]
	strh	r2, [r0, #12]
.L1:
	ldr	r2, [sp, #4]
	adds	r3, r6, #7
	cmp	r3, r2
	bge.n	.L2
	movs	r2, #229
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldrh	r0, [r3, #0]
	movs	r1, #128
	movs	r3, #24
	str	r3, [sp, #0]
	mov	r2, r9
	movs	r3, #216
	lsls	r1, r1, #23
	bl	Func_080150c8
	movs	r2, #0
	movs	r3, #15
	strb	r2, [r0, #4]
	strb	r3, [r0, #5]
	strh	r2, [r0, #12]
.L2:
	ldr	r2, [sp, #4]
	movs	r3, #0
	mov	sl, r3
	cmp	r6, r2
	bcs.n	.L0
	lsls	r3, r6, #1
	adds	r3, r3, r7
	mov	r8, r3
	movs	r3, #16
	mov	fp, r3
.L4:
	mov	r3, r8
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	adds	r0, r5, #0
	bl	Func_08077018
	mov	r2, sl
	lsls	r3, r2, #5
	movs	r2, #0
	str	r2, [sp, #0]
	adds	r7, r0, #0
	movs	r1, #1
	adds	r0, r5, #0
	mov	r2, r9
	bl	Func_080152d0
	movs	r3, #252
	strb	r3, [r0, #15]
	ldr	r3, [sp, #8]
	cmp	r6, r3
	bne.n	.L3
	movs	r3, #9
	strb	r3, [r0, #5]
	movs	r3, #10
	strh	r3, [r0, #12]
	movs	r3, #253
	strb	r3, [r0, #15]
.L3:
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	movs	r3, #0
	mov	r2, fp
	mov	r1, r9
	bl	Func_080b0744
	movs	r3, #251
	movs	r2, #1
	strb	r3, [r0, #15]
	add	sl, r2
	movs	r3, #32
	add	fp, r3
	mov	r2, sl
	movs	r3, #2
	add	r8, r3
	adds	r6, #1
	cmp	r2, #6
	bhi.n	.L0
	ldr	r3, [sp, #4]
	cmp	r6, r3
	bcc.n	.L4
.L0:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x000003a6
	.4byte 0x00000392
