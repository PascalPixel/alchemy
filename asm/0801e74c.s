@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801e74c
	.thumb_func
Func_0801e74c:
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	mov	fp, r3
	ldr	r3, [pc, #68]
	mov	r9, r2
	ldr	r5, [r3, #0]
	ldr	r2, [pc, #68]
	movs	r3, #0
	mov	r8, r3
	adds	r6, r5, r2
	mov	r2, r8
	mov	sl, r1
	strh	r2, [r6, #0]
	movs	r1, #1
	bl	Func_08018038
	ldrh	r3, [r6, #0]
	movs	r1, #235
	lsls	r1, r1, #4
	lsls	r3, r3, #1
	adds	r3, r3, r1
	mov	r2, r8
	strh	r2, [r5, r3]
	ldrh	r3, [r6, #0]
	ldr	r2, [pc, #24]
	adds	r3, #1
	ands	r3, r2
	adds	r5, r5, r1
	strh	r3, [r6, #0]
	adds	r0, r5, #0
	mov	r1, sl
	mov	r2, r9
	mov	r3, fp
	bl	Func_08017aa4
	b.n	.L0
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0x000012b2
.L0:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
