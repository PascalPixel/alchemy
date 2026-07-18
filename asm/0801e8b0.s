@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801e8b0
	.thumb_func
Func_0801e8b0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #2
	mov	sl, r2
	mov	r8, r3
	adds	r7, r1, #0
	bl	Func_08004970
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	ldrb	r3, [r5, #0]
	adds	r2, r6, #0
	cmp	r3, #0
	beq.n	.L0
.L1:
	ldrb	r3, [r5, #0]
	strh	r3, [r2, #0]
	adds	r5, #1
	ldrb	r3, [r5, #0]
	adds	r2, #2
	cmp	r3, #0
	bne.n	.L1
.L0:
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	mov	r1, r8
	ldrh	r3, [r7, #14]
	lsrs	r2, r1, #3
	adds	r3, r3, r2
	mov	r4, sl
	ldrh	r2, [r7, #12]
	lsrs	r1, r4, #3
	adds	r3, #1
	adds	r2, r2, r1
	lsls	r3, r3, #5
	adds	r3, r3, r2
	adds	r1, r3, #1
	movs	r3, #160
	lsls	r3, r3, #2
	cmp	r1, r3
	bcs.n	.L2
	ldr	r4, [pc, #24]
	lsls	r1, r1, #1
	adds	r2, r1, r4
	movs	r3, #7
	adds	r1, r0, r1
	mov	r0, sl
	ands	r3, r0
	adds	r0, r6, #0
	b.n	.L3
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001e8c
	.4byte 0x06002000
.L3:
	bl	Func_0801de5c
	adds	r0, r6, #0
	bl	Func_08002df0
.L2:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
