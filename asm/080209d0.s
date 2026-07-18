@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080209d0
	.thumb_func
Func_080209d0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	ldr	r3, [pc, #124]
	movs	r0, #192
	lsls	r0, r0, #2
	ldr	r6, [r3, #0]
	mov	r8, r1
	bl	Func_08004970
	mov	sl, r0
	mov	r1, sl
	mov	r0, r8
	bl	Func_08005340
	ldrh	r3, [r5, #14]
	ldrh	r2, [r5, #12]
	lsls	r3, r3, #5
	adds	r3, r3, r2
	ldr	r1, [pc, #96]
	movs	r2, #0
	ldrh	r4, [r5, #10]
	lsls	r3, r3, #1
	mov	ip, r2
	mov	r7, sl
	adds	r0, r3, r1
	adds	r6, r6, r3
	cmp	ip, r4
	bge.n	.L0
	movs	r3, #32
	ldrh	r2, [r5, #8]
	mov	lr, r3
.L3:
	movs	r1, #0
	cmp	r1, r2
	bge.n	.L1
.L2:
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	strh	r3, [r0, #0]
	strh	r3, [r6, #0]
	ldrh	r2, [r5, #8]
	adds	r1, #1
	adds	r7, #2
	adds	r0, #2
	adds	r6, #2
	cmp	r1, r2
	blt.n	.L2
	ldrh	r4, [r5, #10]
.L1:
	mov	r1, lr
	subs	r3, r1, r2
	lsls	r3, r3, #1
	adds	r0, r0, r3
	adds	r6, r6, r3
	movs	r3, #1
	add	ip, r3
	cmp	ip, r4
	blt.n	.L3
.L0:
	mov	r0, sl
	bl	Func_08002df0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x06002000
