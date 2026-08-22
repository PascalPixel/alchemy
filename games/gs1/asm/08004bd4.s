@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08004bd4
	.thumb_func
Func_08004bd4:
	push	{r5, r6, lr}
	sub	sp, #48
	adds	r5, r0, #0
	bl	Func_08002322
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	Func_0800231c
	mov	ip, r0
	mov	r5, sp
	adds	r0, r5, #0
	movs	r1, #128
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	lsls	r1, r1, #9
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	mov	r3, ip
	str	r6, [r5, #20]
	negs	r6, r6
	str	r3, [r5, #16]
	str	r3, [r5, #32]
	str	r6, [r5, #28]
	ldr	r3, [pc, #12]
	adds	r0, r5, #0
	bl	Func_080072f0
	add	sp, #48
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x030002c0
