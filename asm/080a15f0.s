@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a15f0
	.thumb_func
Func_080a15f0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r2, #0
	adds	r5, r1, #0
	mov	r8, r0
	adds	r1, r6, #0
	ldr	r0, [pc, #284]
	movs	r2, #0
	movs	r3, #32
	sub	sp, #4
	bl	Func_08015080
	movs	r7, #40
	ldrh	r0, [r5, #60]
	adds	r2, r6, #0
	movs	r3, #16
	movs	r1, #3
	str	r7, [sp, #0]
	bl	Func_080150a8
	mov	r3, r8
	ldrh	r2, [r3, #60]
	ldrh	r3, [r5, #60]
	cmp	r2, r3
	beq.n	.L0
	adds	r0, r2, #0
	movs	r3, #64
	adds	r2, r6, #0
	movs	r1, #3
	str	r7, [sp, #0]
	bl	Func_080150a8
	mov	r3, r8
	ldrh	r2, [r3, #60]
	ldrh	r3, [r5, #60]
	cmp	r2, r3
	bls.n	.L1
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #36
	movs	r3, #0
	bl	Func_080ae99c
	b.n	.L0
.L1:
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #36
	movs	r3, #1
	bl	Func_080ae99c
.L0:
	ldr	r0, [pc, #200]
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #48
	bl	Func_08015080
	movs	r7, #56
	ldrh	r0, [r5, #62]
	adds	r2, r6, #0
	movs	r3, #16
	movs	r1, #3
	str	r7, [sp, #0]
	bl	Func_080150a8
	mov	r3, r8
	ldrh	r2, [r3, #62]
	ldrh	r3, [r5, #62]
	cmp	r2, r3
	beq.n	.L2
	adds	r0, r2, #0
	movs	r3, #64
	adds	r2, r6, #0
	movs	r1, #3
	str	r7, [sp, #0]
	bl	Func_080150a8
	mov	r3, r8
	ldrh	r2, [r3, #62]
	ldrh	r3, [r5, #62]
	cmp	r2, r3
	bls.n	.L3
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #52
	movs	r3, #0
	bl	Func_080ae99c
	b.n	.L2
.L3:
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #52
	movs	r3, #1
	bl	Func_080ae99c
.L2:
	ldr	r0, [pc, #116]
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #64
	bl	Func_08015080
	adds	r7, r5, #0
	movs	r3, #72
	adds	r7, #64
	mov	r5, r8
	ldrh	r0, [r7, #0]
	adds	r2, r6, #0
	str	r3, [sp, #0]
	mov	sl, r3
	movs	r1, #3
	movs	r3, #16
	adds	r5, #64
	bl	Func_080150a8
	ldrh	r2, [r5, #0]
	ldrh	r3, [r7, #0]
	cmp	r2, r3
	beq.n	.L4
	mov	r3, sl
	adds	r0, r2, #0
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r3, #64
	movs	r1, #3
	bl	Func_080150a8
	ldrh	r2, [r5, #0]
	ldrh	r3, [r7, #0]
	cmp	r2, r3
	bls.n	.L5
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #68
	movs	r3, #0
	bl	Func_080ae99c
	b.n	.L4
.L5:
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #68
	movs	r3, #1
	bl	Func_080ae99c
.L4:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000b1c
	.4byte 0x00000b1d
	.4byte 0x00000b20
