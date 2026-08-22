@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b29a8
	.thumb_func
Func_080b29a8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #0
	sub	sp, #8
	mov	r8, r1
	adds	r5, r0, #0
	mov	sl, r1
	bl	Func_080b010c
	ldr	r3, [pc, #312]
	ldr	r2, [pc, #312]
	ldr	r7, [r3, #0]
	mov	r1, r8
	adds	r3, r7, r2
	strb	r1, [r3, #0]
	adds	r0, r5, #0
	bl	Func_0808a080
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	movs	r1, #233
	ldrh	r2, [r3, #0]
	lsls	r1, r1, #2
	adds	r3, r7, r1
	strh	r2, [r3, #0]
	movs	r1, #0
	ldrh	r0, [r3, #0]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080150f8
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L0
	movs	r0, #5
	negs	r0, r0
	movs	r5, #2
	movs	r1, #0
	movs	r2, #5
	movs	r3, #5
	str	r5, [sp, #0]
	bl	Func_08015010
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L0
	movs	r1, #0
	movs	r2, #5
	movs	r3, #5
	movs	r0, #0
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r3, #4
	negs	r3, r3
	mov	r8, r0
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	mov	r3, r8
	bl	Func_080150d8
.L0:
	movs	r2, #228
	lsls	r2, r2, #2
	adds	r3, r7, r2
	movs	r1, #128
	ldrh	r0, [r3, #0]
	movs	r6, #0
	lsls	r1, r1, #23
	mov	r2, r8
	movs	r3, #0
	str	r6, [sp, #0]
	bl	Func_080150c8
	movs	r3, #1
	adds	r5, r0, #0
	strb	r3, [r5, #5]
	movs	r3, #224
	lsls	r3, r3, #2
	strb	r6, [r5, #4]
	movs	r1, #32
	adds	r6, r7, r3
	negs	r1, r1
	adds	r0, r6, #0
	movs	r2, #112
	bl	Func_080b0a20
	str	r5, [r6, #0]
	ldr	r0, [pc, #156]
	bl	Func_080b28d4
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #11
	movs	r2, #12
	movs	r3, #4
	movs	r0, #16
	bl	Func_08015010
	str	r0, [r7, #12]
	bl	Func_080b10cc
	ldr	r1, [pc, #124]
	adds	r6, r7, r1
	b.n	.L1
.L4:
	ldr	r5, [pc, #128]
	adds	r0, r5, #0
	bl	Func_080b28d4
	bl	Func_080b280c
	cmp	r0, #0
	bne.n	.L2
	adds	r0, r5, #1
	bl	Func_080b28d4
	b.n	.L3
.L2:
	bl	Func_080b2b10
.L3:
	movs	r2, #224
	movs	r3, #0
	lsls	r2, r2, #2
	movs	r1, #32
	adds	r0, r7, r2
	strb	r3, [r6, #0]
	negs	r1, r1
	movs	r2, #112
	bl	Func_080b0a20
	ldr	r0, [pc, #84]
	bl	Func_080b28d4
.L1:
	mov	r0, sl
	bl	Func_08015388
	movs	r1, #1
	mov	sl, r0
	mov	r3, sl
	negs	r1, r1
	strb	r3, [r6, #0]
	cmp	sl, r1
	bne.n	.L4
	ldr	r0, [pc, #60]
	bl	Func_080b28d4
	ldr	r0, [r7, #12]
	movs	r1, #2
	bl	Func_08015018
	mov	r0, r8
	movs	r1, #2
	bl	Func_08015018
	bl	Func_080b0204
	movs	r0, #0
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x000003aa
	.4byte 0x00000d21
	.4byte 0x00000d24
	.4byte 0x00000d22
	.4byte 0x00000d23
