@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08015fb8
	.thumb_func
Func_08015fb8:
	push	{lr}
	mov	ip, r3
	mov	r3, r9
	push	{r3}
	mov	r3, ip
	adds	r3, r0, #0
	ldr	r0, [pc, #56]
	mov	r2, r9
	sub	sp, #4
	adds	r4, r1, #0
	str	r2, [sp, #0]
	ands	r4, r0
	ldr	r2, [pc, #48]
	ands	r0, r3
	lsls	r0, r0, #5
	lsls	r4, r4, #5
	adds	r0, r0, r2
	subs	r2, #16
	adds	r1, r4, r2
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #40]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #40]
	adds	r4, r4, r3
	adds	r0, r4, #0
	ldr	r3, [pc, #36]
	movs	r1, #20
	bl	Func_080072f0
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x06000010
	.4byte 0x040000d4
	.4byte 0x80000008
	.4byte 0x0600000c
	.4byte 0x03000164
