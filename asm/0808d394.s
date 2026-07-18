@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808d394
	.thumb_func
Func_0808d394:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #140]
	movs	r2, #1
	movs	r1, #0
	negs	r2, r2
	ldr	r7, [r3, #0]
	adds	r4, r0, #0
	movs	r6, #8
	mov	ip, r1
	mov	lr, r2
.L6:
	ldmia	r7!, {r0}
	cmp	r0, #0
	beq.n	.L0
	cmp	r4, #7
	bgt.n	.L1
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	ldrh	r2, [r0, #0]
	cmp	r3, lr
	beq.n	.L0
	movs	r1, #1
	negs	r1, r1
.L3:
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	cmp	r3, r4
	beq.n	.L2
	adds	r0, #24
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	ldrh	r2, [r0, #0]
	cmp	r3, r1
	bne.n	.L3
	b.n	.L0
.L1:
	ldrh	r2, [r0, #0]
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	adds	r1, r2, #0
	cmp	r3, lr
	beq.n	.L0
	movs	r5, #1
	negs	r5, r5
.L5:
	lsls	r3, r1, #16
	movs	r1, #224
	lsls	r1, r1, #11
	cmp	r3, r1
	ble.n	.L4
	cmp	r6, r4
	beq.n	.L2
	adds	r6, #1
.L4:
	adds	r0, #24
	ldrh	r2, [r0, #0]
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	adds	r1, r2, #0
	cmp	r3, r5
	bne.n	.L5
.L0:
	movs	r2, #1
	add	ip, r2
	mov	r3, ip
	cmp	r3, #3
	ble.n	.L6
	ldrh	r2, [r0, #0]
.L2:
	lsls	r3, r2, #16
	movs	r5, #1
	asrs	r3, r3, #16
	negs	r5, r5
	cmp	r3, r5
	bne.n	.L7
	movs	r0, #0
.L7:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001ebc
