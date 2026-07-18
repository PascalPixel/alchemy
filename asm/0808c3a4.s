@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808c3a4
	.thumb_func
Func_0808c3a4:
	push	{r5, r6, r7, lr}
	movs	r7, #0
	bl	Func_08077148
	cmp	r7, r0
	bge.n	.L0
	ldr	r3, [pc, #140]
	movs	r2, #252
	lsls	r2, r2, #1
	adds	r6, r3, r2
	adds	r5, r0, #0
.L7:
	ldrb	r0, [r6, #0]
	bl	Func_08077008
	ldr	r2, [pc, #128]
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	beq.n	.L1
	cmp	r3, #2
	beq.n	.L2
	b.n	.L3
.L1:
	movs	r3, #52
	ldrsh	r0, [r0, r3]
	movs	r1, #20
	adds	r0, #10
	bl	Func_080022ec
	negs	r1, r0
	cmp	r1, #0
	bne.n	.L4
	movs	r1, #1
	negs	r1, r1
.L4:
	cmp	r7, #0
	bgt.n	.L5
	movs	r7, #1
	b.n	.L5
.L2:
	movs	r2, #52
	ldrsh	r0, [r0, r2]
	movs	r1, #10
	adds	r0, #5
	bl	Func_080022ec
	negs	r1, r0
	cmp	r1, #0
	bne.n	.L6
	movs	r1, #1
	negs	r1, r1
.L6:
	cmp	r7, #1
	bgt.n	.L5
	movs	r7, #2
	b.n	.L5
.L3:
	movs	r1, #0
.L5:
	ldrb	r0, [r6, #0]
	subs	r5, #1
	bl	Func_08077118
	adds	r6, #1
	cmp	r5, #0
	bne.n	.L7
.L0:
	cmp	r7, #0
	beq.n	.L8
	ldr	r0, [pc, #32]
	movs	r1, #0
	bl	Func_08091220
	movs	r0, #4
	bl	Func_08091254
	movs	r0, #133
	bl	Func_080f9010
.L8:
	adds	r0, r7, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x02000240
	.4byte 0x00000131
	.4byte 0x000001ff
