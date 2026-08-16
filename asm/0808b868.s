@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808b868
	.thumb_func
Func_0808b868:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #116]
	adds	r5, r0, #0
	ldr	r7, [pc, #116]
	movs	r0, #178
	lsls	r0, r0, #1
	ldr	r6, [r3, #0]
	bl	Func_080770d0
	adds	r0, r7, #0
	bl	Func_080770c8
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	beq.n	.L0
	adds	r4, r6, #0
	adds	r0, r7, #0
	mov	ip, r2
	adds	r4, #236
.L3:
	movs	r2, #2
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L1
	ldr	r2, [r5, #8]
	ldr	r3, [r4, #0]
	ldr	r1, [r5, #16]
	cmp	r3, r2
	bgt.n	.L2
	adds	r3, r6, #0
	adds	r3, #244
	ldr	r3, [r3, #0]
	cmp	r2, r3
	bgt.n	.L2
	adds	r3, r6, #0
	adds	r3, #240
	ldr	r3, [r3, #0]
	cmp	r3, r1
	bgt.n	.L2
	adds	r3, r6, #0
	adds	r3, #248
	ldr	r3, [r3, #0]
	cmp	r1, r3
	bgt.n	.L2
	movs	r3, #178
	lsls	r3, r3, #1
	strh	r3, [r5, #2]
	b.n	.L1
.L2:
	strh	r0, [r5, #2]
.L1:
	adds	r5, #24
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, ip
	bne.n	.L3
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0x00000165
