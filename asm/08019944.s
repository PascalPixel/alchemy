@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08019944
	.thumb_func
Func_08019944:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #72]
	ldr	r4, [pc, #72]
	ldr	r2, [r3, #0]
	ldrh	r3, [r4, r2]
	adds	r5, r0, #0
	adds	r7, r1, #0
	movs	r1, #0
	movs	r6, #0
	mov	ip, r1
	ldr	r0, [pc, #60]
	cmp	r3, r5
	bne.n	.L0
	ldr	r6, [r0, r2]
	cmp	r7, #0
	beq.n	.L1
	str	r1, [r0, r2]
	strh	r1, [r4, r2]
	b.n	.L1
.L0:
	adds	r1, #1
	adds	r0, #4
	adds	r4, #2
	cmp	r1, #7
	bhi.n	.L1
	ldrh	r3, [r4, r2]
	cmp	r3, r5
	bne.n	.L0
	ldr	r6, [r0, r2]
	cmp	r7, #0
	beq.n	.L1
	mov	r3, ip
	str	r3, [r0, r2]
	mov	r3, ip
	strh	r3, [r4, r2]
.L1:
	adds	r0, r6, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e8c
	.4byte 0x000012dc
	.4byte 0x000012bc
