@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a8b10
	.thumb_func
Func_080a8b10:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r0, r2, #0
	adds	r6, r1, #0
	bl	Func_08077008
	movs	r1, #0
	adds	r2, r0, #0
	adds	r3, r5, #4
	mov	ip, r5
.L0:
	strb	r1, [r3, #0]
	subs	r3, #1
	cmp	r3, ip
	bge.n	.L0
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L1
	cmp	r6, #1
	bne.n	.L1
	strb	r6, [r5, #0]
	movs	r0, #1
.L1:
	ldr	r1, [pc, #72]
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L2
	cmp	r3, #1
	bne.n	.L3
	strb	r3, [r5, #1]
	b.n	.L4
.L3:
	movs	r3, #1
	strb	r3, [r5, #2]
.L4:
	adds	r0, #1
.L2:
	movs	r1, #152
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L5
	movs	r3, #1
	strb	r3, [r5, #3]
	adds	r0, #1
.L5:
	movs	r1, #160
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L6
	movs	r3, #1
	strb	r3, [r5, #4]
	adds	r0, #1
.L6:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x00000131
