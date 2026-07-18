@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08016230
	.thumb_func
Func_08016230:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #132]
	ldr	r3, [r3, #0]
	mov	sl, r3
	ldrh	r3, [r0, #10]
	ldrh	r2, [r0, #22]
	mov	r8, r3
	movs	r3, #0
	strh	r3, [r0, #26]
	movs	r3, #8
	ands	r3, r2
	sub	sp, #4
	ldrh	r5, [r0, #12]
	ldrh	r6, [r0, #14]
	ldrh	r7, [r0, #8]
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	mov	r3, r8
	bl	Func_080170f8
	movs	r1, #240
	ldr	r3, [pc, #84]
	ldr	r0, [pc, #84]
	lsls	r1, r1, #4
	ldr	r2, [pc, #84]
	bl	Func_080072f0
	b.n	.L2
.L1:
	movs	r1, #240
	ldr	r3, [pc, #68]
	ldr	r0, [pc, #68]
	lsls	r1, r1, #4
	movs	r2, #0
	bl	Func_080072f0
.L2:
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	mov	r3, r8
	bl	Func_08017248
	b.n	.L3
.L0:
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	mov	r3, r8
	bl	Func_080170f8
.L3:
	ldr	r2, [pc, #36]
	movs	r3, #1
	add	r2, sl
	strb	r3, [r2, #0]
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x03000168
	.4byte 0x06002500
	.4byte 0x44444444
	.4byte 0x00000ea3
