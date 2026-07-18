@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b3444
	.thumb_func
Func_080b3444:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r0, [sp, #12]
	movs	r2, #0
	movs	r0, #1
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	mov	r9, r0
	mov	fp, r2
	bl	Func_080b010c
	ldr	r3, [pc, #436]
	ldr	r3, [r3, #0]
	movs	r5, #2
	mov	r8, r3
	movs	r1, #12
	movs	r2, #14
	movs	r3, #8
	movs	r0, #16
	str	r5, [sp, #0]
	bl	Func_08015010
	mov	r3, r8
	str	r0, [r3, #32]
	movs	r1, #14
	movs	r2, #13
	movs	r3, #3
	movs	r0, #0
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r3, #228
	lsls	r3, r3, #2
	ldr	r4, [sp, #4]
	mov	sl, r0
	add	r3, r8
	movs	r1, #128
	ldrh	r0, [r3, #0]
	lsls	r1, r1, #23
	mov	r2, sl
	movs	r3, #0
	str	r4, [sp, #0]
	bl	Func_080150c8
	movs	r6, #224
	adds	r5, r0, #0
	add	r0, sp, #4
	ldrb	r0, [r0, #0]
	lsls	r6, r6, #2
	movs	r3, #4
	add	r6, r8
	movs	r1, #32
	strb	r3, [r5, #5]
	strb	r0, [r5, #4]
	negs	r1, r1
	adds	r0, r6, #0
	movs	r2, #112
	bl	Func_080b0a20
	movs	r2, #234
	lsls	r2, r2, #2
	add	r2, r8
	movs	r3, #12
	str	r5, [r6, #0]
	strb	r3, [r2, #0]
	ldr	r2, [sp, #4]
	mov	r0, sl
	str	r2, [sp, #0]
	movs	r1, #2
	movs	r2, #0
	movs	r3, #8
	bl	Func_080a1028
	movs	r6, #234
	lsls	r6, r6, #2
	movs	r7, #0
	add	r6, r8
.L3:
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [pc, #300]
	add	r3, r8
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	movs	r4, #0
	adds	r0, r7, r1
	mov	r9, r4
	bl	Func_080022fc
	adds	r7, r0, #0
	movs	r0, #219
	lsls	r0, r0, #2
	lsls	r1, r7, #1
	mov	r3, r8
	adds	r2, r1, r0
	adds	r3, #2
	adds	r1, r1, r7
	ldrsh	r4, [r3, r2]
	lsls	r1, r1, #3
	subs	r1, #12
	mov	r0, sl
	movs	r2, #0
	mov	fp, r4
	bl	Func_080b0a6c
	movs	r3, #3
	mov	r0, sl
	adds	r1, r7, #0
	movs	r2, #0
	strb	r3, [r6, #0]
	bl	Func_080b11c4
	mov	r2, r8
	ldr	r0, [r2, #32]
	mov	r1, fp
	bl	Func_080b1dec
.L0:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #224]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L1
	mov	r0, fp
	bl	Func_08077248
	cmp	r0, #0
	bne.n	.L2
	movs	r0, #113
	bl	Func_080f9010
	b.n	.L3
.L2:
	movs	r0, #112
	bl	Func_080f9010
	mov	r0, fp
	bl	Func_080b362c
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L4
	movs	r3, #224
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #4
	strb	r3, [r2, #5]
	movs	r4, #1
	movs	r3, #12
	strb	r3, [r6, #0]
	mov	r9, r4
	b.n	.L3
.L4:
	ldr	r3, [sp, #12]
	mov	r2, fp
	str	r2, [r3, #0]
	ldr	r4, [sp, #8]
	str	r0, [r4, #0]
	movs	r0, #0
	str	r0, [sp, #4]
	b.n	.L5
.L1:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L6
	movs	r0, #113
	bl	Func_080f9010
	movs	r3, #1
	ldr	r2, [sp, #12]
	negs	r3, r3
	str	r3, [r2, #0]
	ldr	r4, [sp, #8]
	str	r3, [r4, #0]
	str	r3, [sp, #4]
	b.n	.L5
.L6:
	ldr	r5, [pc, #108]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	movs	r0, #111
	bl	Func_080f9010
	movs	r0, #1
	subs	r7, #1
	mov	r9, r0
.L7:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	adds	r7, #1
	mov	r9, r2
	b.n	.L3
.L5:
	bl	Func_080a1030
	mov	r0, sl
	movs	r1, #2
	bl	Func_08015018
	mov	r3, r8
	ldr	r0, [r3, #32]
	movs	r1, #2
	bl	Func_08015018
	movs	r0, #1
	bl	Func_080030f8
	bl	Func_080b0204
	ldr	r0, [sp, #4]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x000003a7
	.4byte 0x03001c94
	.4byte 0x03001b04
