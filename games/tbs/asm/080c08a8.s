@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c08a8
	.thumb_func
Func_080c08a8:
	push	{r5, lr}
	movs	r1, #168
	lsls	r1, r1, #2
	movs	r0, #10
	sub	sp, #4
	bl	Func_080048f4
	ldr	r3, [pc, #28]
	adds	r1, r0, #0
	movs	r4, #0
	mov	r0, sp
	ldr	r5, [r3, #0]
	str	r4, [r0, #0]
	ldr	r3, [pc, #20]
	ldr	r2, [pc, #20]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	str	r4, [r5, #8]
	add	sp, #4
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001f00
	.4byte 0x040000d4
	.4byte 0x850000a8
