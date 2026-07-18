@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a1ac0
	.thumb_func
Func_080a1ac0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #28]
	mov	sl, r3
	add	r2, sl
	ldrh	r3, [r2, #0]
	mov	r8, r1
	movs	r1, #2
	sub	sp, #4
	mov	r9, r1
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #0
	strh	r3, [r2, #0]
	b.n	.L1
	.4byte 0x03001f2c
	.4byte 0x00000222
.L0:
	mov	r2, sl
	ldr	r7, [r2, #20]
	ldrh	r3, [r7, #22]
	lsls	r3, r3, #23
	lsrs	r3, r3, #23
	adds	r3, #64
	ldrb	r2, [r7, #20]
	strh	r3, [r7, #6]
	adds	r2, #64
	strh	r2, [r7, #8]
	ldrh	r2, [r7, #6]
	movs	r3, #64
	add	r8, r3
	adds	r3, r2, #0
	subs	r3, #8
	adds	r0, #64
	cmp	r3, #0
	ble.n	.L2
	ldr	r1, [pc, #120]
	adds	r3, r2, r1
	strh	r3, [r7, #6]
.L2:
	ldrh	r6, [r7, #8]
	adds	r3, r6, #0
	subs	r3, #8
	cmp	r3, #0
	ble.n	.L3
	ldr	r2, [pc, #104]
	adds	r3, r6, r2
	strh	r3, [r7, #8]
	ldrh	r6, [r7, #8]
.L3:
	ldrh	r5, [r7, #6]
	lsls	r0, r0, #4
	lsls	r5, r5, #4
	subs	r0, r0, r5
	movs	r1, #2
	adds	r0, #1
	bl	Func_080022ec
	mov	r3, r8
	mov	fp, r0
	lsls	r6, r6, #4
	lsls	r0, r3, #4
	subs	r0, r0, r6
	adds	r0, #1
	movs	r1, #2
	bl	Func_080022ec
	ldr	r4, [pc, #52]
	mov	r8, r0
.L5:
	mov	r2, sl
	ldr	r0, [r2, #16]
	ldrh	r3, [r0, #12]
	add	r5, fp
	lsls	r3, r3, #3
	asrs	r1, r5, #4
	adds	r1, r1, r3
	subs	r1, #56
	ldr	r3, [pc, #36]
	strh	r1, [r7, #6]
	ands	r1, r4
	ands	r1, r3
	ldr	r2, [pc, #32]
	ldrh	r3, [r7, #22]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #22]
	ldrh	r3, [r0, #14]
	add	r6, r8
	lsls	r3, r3, #3
	asrs	r2, r6, #4
	adds	r2, r2, r3
	movs	r3, #1
	b.n	.L4
	movs	r0, r0
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x0000fff8
.L4:
	negs	r3, r3
	subs	r2, #56
	add	r9, r3
	strh	r2, [r7, #8]
	mov	r1, r9
	ands	r2, r4
	strb	r2, [r7, #20]
	cmp	r1, #0
	beq.n	.L1
	movs	r0, #1
	str	r4, [sp, #0]
	bl	Func_080030f8
	ldr	r4, [sp, #0]
	b.n	.L5
.L1:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	bx	lr
