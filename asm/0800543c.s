@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800543c
	.thumb_func
Func_0800543c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	ip, r2
	ldr	r2, [pc, #60]
	lsrs	r2, r2, #2
	adds	r6, r1, #0
	mov	lr, r3
	mov	r1, sp
	lsls	r3, r2, #2
	movs	r4, #132
	subs	r1, r1, r3
	lsls	r4, r4, #24
	mov	r7, sp
	adds	r5, r0, #0
	mov	r8, sp
	ldr	r3, [pc, #40]
	mov	sp, r1
	ldr	r0, [pc, #40]
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	adds	r1, r6, #0
	mov	r2, ip
	mov	r3, lr
	bl	Func_08007318
	mov	sp, r8
	mov	sp, r7
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000084
	.4byte 0x040000d4
	.4byte 0x08001fb8
