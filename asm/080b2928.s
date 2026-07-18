@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b2928
	.thumb_func
Func_080b2928:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #112]
	ldr	r3, [r3, #0]
	movs	r6, #224
	mov	r8, r3
	lsls	r6, r6, #2
	add	r6, r8
	ldr	r3, [r6, #0]
	ldrb	r3, [r3, #5]
	mov	sl, r3
	movs	r3, #233
	lsls	r3, r3, #2
	add	r3, r8
	adds	r7, r0, #0
	ldrh	r0, [r3, #0]
	bl	Func_0808a540
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	Func_080b2884
	ldr	r2, [r6, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	adds	r7, r0, #0
	lsls	r5, r5, #16
	bl	Func_08015140
	movs	r3, #34
	orrs	r5, r3
	adds	r0, r7, #0
	movs	r1, #5
	movs	r2, #0
	adds	r3, r5, #0
	bl	Func_08015038
	b.n	.L0
.L1:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L1
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #224
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r3, [r3, #0]
	mov	r2, sl
	strb	r2, [r3, #5]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
