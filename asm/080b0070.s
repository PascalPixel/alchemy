@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b0070
	.thumb_func
Func_080b0070:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #104]
	ldr	r3, [r3, #0]
	movs	r7, #155
	mov	r8, r3
	movs	r1, #0
	lsls	r7, r7, #2
	mov	sl, r1
	movs	r6, #0
	add	r7, r8
.L1:
	ldr	r3, [pc, #92]
	adds	r0, r6, #0
	add	r3, r8
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	bl	Func_08077238
	cmp	r5, r0
	bne.n	.L0
	adds	r0, r6, #0
	movs	r1, #0
	bl	Func_08077240
	cmp	r0, #0
	beq.n	.L0
	movs	r2, #1
	strh	r6, [r7, #0]
	add	sl, r2
	adds	r7, #2
.L0:
	ldr	r3, [pc, #56]
	adds	r6, #1
	cmp	r6, r3
	ble.n	.L1
	mov	r1, sl
	movs	r2, #155
	lsls	r3, r1, #1
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [pc, #28]
	mov	r1, r8
	strh	r2, [r1, r3]
	ldr	r3, [pc, #36]
	mov	r2, sl
	add	r3, r8
	strb	r2, [r3, #0]
	mov	r0, sl
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001f2c
	.4byte 0x000003a9
	.4byte 0x000001ff
	.4byte 0x000003a6
