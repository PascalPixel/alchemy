@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08021d88
	.thumb_func
Func_08021d88:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r3, r1, #0
	lsls	r5, r3, #3
	subs	r5, r5, r3
	adds	r4, r0, #0
	mov	r8, r2
	lsls	r5, r5, #2
	movs	r2, #130
	adds	r6, r4, r5
	lsls	r3, r3, #4
	lsls	r2, r2, #1
	sub	sp, #4
	adds	r6, r6, r2
	mov	r1, r8
	adds	r2, r3, #0
	str	r4, [sp, #0]
	bl	Func_08021cb8
	movs	r2, #142
	lsls	r2, r2, #1
	ldr	r4, [sp, #0]
	adds	r3, r5, r2
	mov	r2, r8
	str	r2, [r4, r3]
	ldr	r3, [pc, #40]
	str	r3, [r6, #4]
	movs	r3, #0
	str	r3, [r6, #8]
	movs	r3, #136
	lsls	r3, r3, #1
	adds	r5, r5, r3
	ldrh	r0, [r4, r5]
	mov	r1, r8
	bl	Func_08021c64
	ldr	r3, [pc, #16]
	ldrh	r2, [r6, #8]
	ands	r0, r3
	ldr	r3, [pc, #16]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r6, #8]
	add	sp, #4
	b.n	.L0
	.4byte 0x000003ff
	.4byte 0x80002000
	.4byte 0xfffffc00
.L0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
