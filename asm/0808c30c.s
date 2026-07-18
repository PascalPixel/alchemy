@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808c30c
	.thumb_func
Func_0808c30c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	mov	r8, r1
	cmp	r5, #0
	bge.n	.L0
	ldr	r0, [pc, #128]
	movs	r1, #0
	bl	Func_08091220
	movs	r0, #4
	bl	Func_08091254
	movs	r2, #10
	negs	r2, r2
	cmp	r5, r2
	bge.n	.L1
	movs	r0, #134
	bl	Func_080f9010
	b.n	.L2
.L1:
	movs	r0, #133
	bl	Func_080f9010
	b.n	.L2
.L0:
	movs	r0, #126
	bl	Func_080f9010
.L2:
	bl	Func_08077148
	cmp	r0, #0
	ble.n	.L3
	ldr	r3, [pc, #80]
	movs	r2, #252
	lsls	r2, r2, #1
	adds	r7, r3, r2
	adds	r6, r0, #0
.L5:
	ldrb	r0, [r7, #0]
	bl	Func_08077008
	mov	r3, r8
	adds	r1, r5, #0
	cmp	r3, #0
	beq.n	.L4
	movs	r2, #52
	ldrsh	r3, [r0, r2]
	movs	r1, #100
	adds	r0, r5, #0
	muls	r0, r3
	bl	Func_080022ec
	adds	r1, r0, #0
	cmp	r1, #0
	bne.n	.L4
	adds	r1, r5, #0
	cmp	r1, #0
	bge.n	.L4
	negs	r1, r1
.L4:
	ldrb	r0, [r7, #0]
	subs	r6, #1
	bl	Func_08077118
	adds	r7, #1
	cmp	r6, #0
	bne.n	.L5
.L3:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x02000240
