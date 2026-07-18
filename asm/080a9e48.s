@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a9e48
	.thumb_func
Func_080a9e48:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	mov	fp, r0
	adds	r0, r7, #0
	sub	sp, #4
	mov	r9, r2
	bl	Func_08077008
	ldr	r3, [pc, #152]
	ldr	r3, [r3, #0]
	mov	r1, fp
	str	r3, [sp, #0]
	ldr	r2, [pc, #148]
	lsls	r3, r1, #1
	adds	r5, r0, #0
	adds	r3, #216
	mov	sl, r2
	mov	r8, r3
	ldrh	r3, [r5, r3]
	mov	r1, sl
	ands	r1, r3
	mov	sl, r1
	mov	r0, sl
	bl	Func_08077018
	adds	r6, r0, #0
	ldrh	r3, [r6, #40]
	ldr	r0, [pc, #124]
	mov	r2, r9
	ands	r0, r3
	adds	r1, r7, #0
	movs	r3, #1
	bl	Func_080a9f10
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L0
	mov	r3, r8
	ldrh	r0, [r5, r3]
	bl	Func_08077018
	adds	r6, r0, #0
	ldrb	r2, [r6, #12]
	adds	r3, r2, #0
	cmp	r3, #1
	bne.n	.L1
	mov	r1, fp
	adds	r0, r7, #0
	bl	Func_08077058
	movs	r3, #228
	ldr	r2, [sp, #0]
	lsls	r3, r3, #1
	adds	r1, r2, r3
	adds	r0, r5, #0
	movs	r2, #0
	bl	Func_080a3ddc
	movs	r2, #134
	ldr	r1, [sp, #0]
	lsls	r2, r2, #2
	adds	r3, r1, r2
	strb	r0, [r3, #0]
	ldrb	r2, [r6, #12]
.L1:
	adds	r3, r2, #0
	cmp	r3, #4
	bne.n	.L2
	mov	r3, sl
	cmp	r3, #184
	bne.n	.L3
	movs	r1, #185
	mov	sl, r1
.L3:
	mov	r3, sl
	mov	r2, r8
	strh	r3, [r5, r2]
.L2:
	movs	r0, #0
.L0:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x000001ff
	.4byte 0x00003fff
	movs	r0, #1
	bx	lr
