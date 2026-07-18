@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08092f84
	.thumb_func
Func_08092f84:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #164]
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	mov	r9, r3
	bl	Func_08092c40
	mov	sl, r0
	movs	r0, #1
	bl	Func_080030f8
	adds	r0, r7, #0
	bl	Func_08092ba8
	movs	r5, #0
	mov	r8, r0
	cmp	r7, #7
	bgt.n	.L0
	ldr	r6, [pc, #132]
	ands	r6, r7
	adds	r0, r6, #0
	bl	Func_0808d394
	cmp	r0, #0
	bne.n	.L0
	mov	r8, r6
.L0:
	mov	r0, r8
	bl	Func_08015100
	movs	r3, #230
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L1
	b.n	.L2
.L4:
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #150
	adds	r5, #1
	lsls	r3, r3, #2
	cmp	r5, r3
	bhi.n	.L3
	ldr	r1, [pc, #84]
	ldr	r2, [r1, #0]
	movs	r3, #4
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L2
	ldr	r2, [r1, #0]
	adds	r3, #252
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L2
	ldr	r2, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L2
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
.L3:
	bl	Func_08015140
.L2:
	mov	r0, sl
	bl	Func_08015050
	cmp	r0, #0
	beq.n	.L4
.L1:
	movs	r0, #1
	bl	Func_080030f8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x00000fff
	.4byte 0x03001ae8
