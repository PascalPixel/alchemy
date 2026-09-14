@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a1fd4
	.thumb_func
Func_080a1fd4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r1
	ldr	r1, [sp, #28]
	adds	r5, r0, #0
	adds	r7, r2, #0
	movs	r0, #1
	mov	r2, r8
	adds	r6, r3, #0
	mov	sl, r1
	negs	r0, r0
	cmp	r2, #0
	bne.n	.L0
	b.n	.L1
.L0:
	ldr	r0, [pc, #324]
	bl	Func_08015418
	adds	r1, r7, #0
	mov	r0, r8
	bl	Func_080022ec
	adds	r1, r7, #0
	mov	r9, r0
	mov	r0, r8
	bl	Func_080022fc
	cmp	r0, #0
	beq.n	.L2
	movs	r3, #1
	add	r9, r3
.L2:
	cmp	r5, #0
	beq.n	.L3
	ldr	r2, [pc, #292]
	ldr	r4, [r2, #0]
	movs	r3, #16
	ldr	r1, [r2, #0]
	ands	r4, r3
	ldr	r5, [r2, #0]
	movs	r3, #32
	ands	r1, r3
	ldr	r2, [r2, #0]
	movs	r3, #64
	ands	r5, r3
	movs	r3, #128
	b.n	.L4
.L3:
	ldr	r2, [pc, #264]
	ldr	r4, [r2, #0]
	movs	r3, #128
	ldr	r1, [r2, #0]
	ands	r4, r3
	ldr	r5, [r2, #0]
	movs	r3, #64
	ands	r1, r3
	ldr	r2, [r2, #0]
	movs	r3, #32
	ands	r5, r3
	movs	r3, #16
.L4:
	ands	r2, r3
	cmp	r5, #0
	beq.n	.L5
	movs	r0, #111
	bl	Func_080f9010
	mov	r1, sl
	ldr	r3, [r1, #0]
	subs	r3, #1
	str	r3, [r1, #0]
	cmp	r3, #0
	bge.n	.L6
	mov	r3, r9
	subs	r3, #1
	str	r3, [r1, #0]
.L6:
	mov	r2, sl
	ldr	r3, [r2, #0]
	adds	r0, r7, #0
	muls	r0, r3
	ldr	r3, [r6, #0]
	mov	r2, r8
	adds	r3, r3, r0
	subs	r2, #1
	cmp	r3, r2
	ble.n	.L7
	mov	r1, r8
	subs	r3, r1, r0
	b.n	.L8
.L5:
	cmp	r2, #0
	beq.n	.L9
	movs	r0, #111
	bl	Func_080f9010
	mov	r2, sl
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	mov	r2, r9
	subs	r2, #1
	cmp	r3, r2
	ble.n	.L10
	mov	r3, sl
	str	r5, [r3, #0]
.L10:
	mov	r1, sl
	ldr	r3, [r1, #0]
	adds	r0, r7, #0
	muls	r0, r3
	ldr	r3, [r6, #0]
	mov	r2, r8
	adds	r3, r3, r0
	subs	r2, #1
	cmp	r3, r2
	ble.n	.L7
	mov	r2, r8
	subs	r3, r2, r0
.L8:
	subs	r3, #1
	subs	r1, r7, #1
	str	r3, [r6, #0]
	cmp	r3, r1
	ble.n	.L7
	str	r1, [r6, #0]
.L7:
	bl	Func_0800352c
	movs	r0, #1
	b.n	.L1
.L9:
	cmp	r1, #0
	beq.n	.L11
	movs	r0, #111
	bl	Func_080f9010
	ldr	r3, [r6, #0]
	subs	r3, #1
	str	r3, [r6, #0]
	cmp	r3, #0
	bge.n	.L12
	subs	r2, r7, #1
	str	r2, [r6, #0]
	mov	r1, sl
	ldr	r3, [r1, #0]
	muls	r3, r7
	mov	r1, r8
	subs	r3, r1, r3
	subs	r3, #1
	str	r3, [r6, #0]
	cmp	r3, r2
	ble.n	.L12
	str	r2, [r6, #0]
	b.n	.L12
.L11:
	movs	r0, #1
	negs	r0, r0
	cmp	r4, #0
	beq.n	.L1
	movs	r0, #111
	bl	Func_080f9010
	ldr	r2, [r6, #0]
	adds	r2, #1
	str	r2, [r6, #0]
	mov	r1, sl
	ldr	r3, [r1, #0]
	muls	r3, r7
	mov	r1, r8
	subs	r3, r1, r3
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L13
	str	r0, [r6, #0]
.L13:
	ldr	r3, [r6, #0]
	subs	r2, r7, #1
	cmp	r3, r2
	ble.n	.L12
	str	r0, [r6, #0]
.L12:
	movs	r0, #0
.L1:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x06002500
	.4byte 0x03001b04
