@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808fe38
	.thumb_func
Func_0808fe38:
	push	{r5, r6, lr}
	movs	r1, #168
	adds	r6, r0, #0
	lsls	r1, r1, #3
	movs	r0, #31
	sub	sp, #4
	bl	Func_080048f4
	movs	r5, #0
	adds	r4, r0, #0
	mov	r0, sp
	str	r5, [r0, #0]
	ldr	r3, [pc, #64]
	adds	r1, r4, #0
	ldr	r2, [pc, #64]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #165
	lsls	r2, r2, #3
	adds	r3, r4, r2
	adds	r2, #2
	strh	r6, [r3, #0]
	adds	r3, r4, r2
	strh	r5, [r3, #0]
	ldr	r3, [pc, #48]
	adds	r2, r4, r3
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #48]
	movs	r3, #1
	adds	r4, r4, r2
	movs	r1, #200
	strh	r3, [r4, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #40]
	bl	Func_080041d8
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #36]
	bl	Func_080041d8
	add	sp, #4
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x040000d4
	.4byte 0x85000150
	.4byte 0x00000534
	.4byte 0x00003f3f
	.4byte 0x00000536
	.4byte 0x0808f52d
	.4byte 0x0808f499
