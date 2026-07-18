@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a6614
	.thumb_func
Func_080a6614:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #328]
	movs	r1, #136
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldrh	r3, [r3, #0]
	sub	sp, #20
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r1, #2
	adds	r3, r2, r1
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #8
	ldr	r0, [pc, #300]
	negs	r3, r3
	adds	r1, r5, #0
	movs	r2, #0
	bl	Func_08015080
	b.n	.L1
.L0:
	movs	r3, #8
	ldr	r0, [pc, #288]
	negs	r3, r3
	adds	r1, r5, #0
	movs	r2, #0
	bl	Func_08015080
.L1:
	ldr	r3, [pc, #268]
	movs	r2, #136
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #272]
	ands	r0, r3
	ldr	r3, [pc, #272]
	adds	r0, r0, r3
	add	r3, sp, #16
	movs	r1, #12
	movs	r2, #8
	add	r1, sp
	add	r2, sp
	mov	sl, r3
	add	r3, sp, #4
	mov	r9, r1
	mov	fp, r2
	str	r3, [sp, #0]
	mov	r8, r3
	mov	r1, sl
	mov	r3, fp
	mov	r2, r9
	bl	Func_08015108
	ldr	r3, [sp, #8]
	movs	r6, #1
	cmp	r3, #10
	bhi.n	.L2
	movs	r6, #0
.L2:
	ldr	r3, [pc, #208]
	movs	r1, #136
	lsls	r1, r1, #2
	adds	r7, r3, r1
	ldrh	r2, [r7, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L3
	ldr	r0, [pc, #204]
	movs	r1, #4
	ands	r0, r2
	bl	Func_08015120
	ldr	r0, [pc, #204]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	cmp	r6, #0
	bne.n	.L4
	ldrh	r0, [r7, #0]
	lsrs	r0, r0, #10
	bl	Func_08077008
	adds	r1, r5, #0
	movs	r2, #80
	movs	r3, #0
	bl	Func_08015090
	b.n	.L4
.L3:
	ldr	r0, [pc, #172]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
.L4:
	ldr	r3, [pc, #136]
	ldr	r2, [pc, #160]
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #140]
	ands	r0, r3
	ldr	r3, [pc, #140]
	adds	r0, r0, r3
	mov	r3, r8
	str	r3, [sp, #0]
	mov	r1, sl
	mov	r3, fp
	mov	r2, r9
	bl	Func_08015108
	ldr	r3, [sp, #8]
	movs	r6, #1
	cmp	r3, #10
	bhi.n	.L5
	movs	r6, #0
.L5:
	ldr	r3, [pc, #96]
	ldr	r1, [pc, #120]
	adds	r7, r3, r1
	ldrh	r2, [r7, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L6
	ldr	r0, [pc, #92]
	movs	r1, #4
	ands	r0, r2
	bl	Func_08015120
	ldr	r0, [pc, #104]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #8
	bl	Func_08015080
	cmp	r6, #0
	bne.n	.L7
	ldrh	r0, [r7, #0]
	lsrs	r0, r0, #10
	bl	Func_08077008
	adds	r1, r5, #0
	movs	r2, #80
	movs	r3, #8
	bl	Func_08015090
.L7:
	movs	r0, #15
	bl	Func_080150b8
	b.n	.L8
.L6:
	ldr	r0, [pc, #64]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #8
	bl	Func_08015080
.L8:
	movs	r0, #1
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x02000240
	.4byte 0x00000ae4
	.4byte 0x00000ae0
	.4byte 0x000003ff
	.4byte 0x00000333
	.4byte 0x00000ae7
	.4byte 0x00000ae5
	.4byte 0x00000222
	.4byte 0x00000ae8
	.4byte 0x00000ae6
