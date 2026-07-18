@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801e940
	.thumb_func
Func_0801e940:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r7, r2, #0
	mov	r8, r3
	mov	sl, r1
	bl	Func_08004970
	ldrb	r3, [r5, #0]
	adds	r6, r0, #0
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
	ldr	r3, [pc, #28]
	lsrs	r7, r7, #3
	strh	r3, [r2, #0]
	mov	r3, r8
	lsrs	r3, r3, #3
	adds	r0, r6, #0
	mov	r1, sl
	adds	r2, r7, #0
	mov	r8, r3
	bl	Func_08017c8c
	adds	r0, r6, #0
	bl	Func_08002df0
	b.n	.L2
	movs	r0, r0
	.4byte 0x00000000
.L2:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
