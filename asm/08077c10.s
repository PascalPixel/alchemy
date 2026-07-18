@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08077c10
	.thumb_func
Func_08077c10:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #144]
	sub	sp, #8
	bl	Func_08079374
	bl	Func_080795fc
	mov	sl, r0
	movs	r0, #0
	mov	r8, r0
	cmp	r8, sl
	bge.n	.L0
	ldr	r3, [pc, #124]
	movs	r2, #252
	lsls	r2, r2, #1
	adds	r2, r2, r3
	movs	r3, #128
	lsls	r3, r3, #2
	mov	r9, r2
	mov	fp, r3
.L5:
	mov	r2, r9
	ldrb	r0, [r2, #0]
	bl	Func_08077394
	movs	r2, #216
	adds	r7, r0, #0
	movs	r1, #14
.L4:
	ldrh	r3, [r2, r7]
	mov	r0, fp
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L1
	ldrh	r0, [r2, r7]
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	bl	Func_08078414
	adds	r5, r0, #0
	ldr	r2, [sp, #0]
	ldr	r1, [sp, #4]
	adds	r5, #24
	movs	r6, #3
.L3:
	ldrb	r3, [r5, #0]
	adds	r5, #4
	cmp	r3, #27
	bne.n	.L2
	ldr	r0, [pc, #52]
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	bl	Func_08079358
	ldr	r2, [sp, #0]
	ldr	r1, [sp, #4]
.L2:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L3
.L1:
	subs	r1, #1
	adds	r2, #2
	cmp	r1, #0
	bge.n	.L4
	movs	r2, #1
	add	r8, r2
	add	r9, r2
	cmp	r8, sl
	blt.n	.L5
.L0:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000167
	.4byte 0x02000240
