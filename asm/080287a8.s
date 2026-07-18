@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080287a8
	.thumb_func
Func_080287a8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #80]
	ldr	r3, [r3, #0]
	mov	r8, r3
	mov	r2, r8
	adds	r2, #142
	movs	r1, #0
	ldrsh	r7, [r2, r1]
	mov	sl, r0
	ldrh	r3, [r2, #0]
	cmp	r7, #5
	bgt.n	.L0
	adds	r3, #1
	strh	r3, [r2, #0]
	bl	Func_08004080
	mov	r1, sl
	adds	r6, r0, #0
	lsls	r5, r7, #2
	bl	Func_0802875c
	lsls	r3, r7, #1
	adds	r5, r5, r7
	adds	r3, r3, r7
	lsls	r5, r5, #2
	lsls	r3, r3, #3
	add	r5, r8
	adds	r3, #32
	strh	r3, [r5, #12]
	movs	r3, #136
	strh	r3, [r5, #14]
	adds	r3, r7, #0
	adds	r3, #132
	mov	r1, sl
	mov	r2, r8
	strh	r6, [r5, #18]
	strb	r1, [r2, r3]
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f38
