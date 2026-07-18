@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801e7c0
	.thumb_func
Func_0801e7c0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r3, #0
	ldr	r3, [pc, #96]
	mov	sl, r1
	ldr	r7, [r3, #0]
	mov	r9, r2
	ldr	r1, [pc, #92]
	movs	r2, #0
	mov	r8, r2
	adds	r5, r7, r1
	mov	r3, r8
	strh	r3, [r5, #0]
	movs	r1, #1
	bl	Func_08018038
	ldrh	r3, [r5, #0]
	movs	r4, #235
	lsls	r4, r4, #4
	lsls	r3, r3, #1
	adds	r3, r3, r4
	mov	r0, r8
	strh	r0, [r7, r3]
	ldrh	r3, [r5, #0]
	ldr	r2, [pc, #52]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r5, #0]
	mov	r1, sl
	ldrh	r3, [r1, #14]
	lsrs	r6, r6, #3
	ldrh	r2, [r1, #12]
	adds	r3, r3, r6
	mov	r4, r9
	lsrs	r1, r4, #3
	adds	r3, #1
	adds	r2, r2, r1
	lsls	r3, r3, #5
	adds	r3, r3, r2
	movs	r0, #160
	adds	r1, r3, #1
	lsls	r0, r0, #2
	cmp	r1, r0
	bcs.n	.L0
	ldr	r3, [pc, #24]
	movs	r4, #235
	lsls	r1, r1, #1
	lsls	r4, r4, #4
	adds	r2, r1, r3
	adds	r0, r7, r4
	b.n	.L1
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0x000012b2
	.4byte 0x06002000
.L1:
	movs	r3, #7
	mov	r4, r9
	adds	r1, r7, r1
	ands	r3, r4
	bl	Func_0801de5c
.L0:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
