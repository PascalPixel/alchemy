@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ae7fc
	.thumb_func
Func_080ae7fc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #120]
	ldr	r3, [r3, #0]
	mov	sl, r3
	ldr	r3, [pc, #116]
	add	r3, sl
	movs	r2, #0
	ldrb	r3, [r3, #0]
	mov	r8, r2
	mov	fp, r0
	cmp	r8, r3
	bge.n	.L0
	movs	r7, #130
	movs	r3, #1
	lsls	r7, r7, #2
	mov	r9, r3
	add	r7, sl
.L5:
	ldrh	r0, [r7, #0]
	bl	Func_08077008
	movs	r6, #0
	movs	r4, #0
	adds	r0, #248
.L4:
	ldr	r5, [r0, #16]
	movs	r1, #0
.L3:
	mov	r2, r9
	lsls	r2, r1
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L1
	ldr	r3, [r0, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
.L1:
	adds	r4, #1
.L2:
	adds	r1, #1
	cmp	r1, #19
	ble.n	.L3
	adds	r6, #1
	adds	r0, #4
	cmp	r6, #3
	ble.n	.L4
	mov	r3, r8
	mov	r2, fp
	strb	r4, [r2, r3]
	ldr	r3, [pc, #32]
	add	r3, sl
	movs	r2, #1
	ldrb	r3, [r3, #0]
	add	r8, r2
	adds	r7, #2
	cmp	r8, r3
	blt.n	.L5
.L0:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x00000219
