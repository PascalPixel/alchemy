@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08091174
	.thumb_func
Func_08091174:
	push	{lr}
	ldr	r1, [pc, #84]
	movs	r0, #32
	sub	sp, #4
	bl	Func_080048f4
	movs	r3, #0
	adds	r4, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	adds	r1, r4, #0
	ldr	r3, [pc, #68]
	ldr	r2, [pc, #68]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #160
	lsls	r0, r0, #19
	ldr	r2, [pc, #64]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r1, r4, r2
	ldr	r0, [pc, #56]
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #224
	lsls	r3, r3, #4
	movs	r0, #128
	adds	r2, r4, r3
	adds	r1, r4, #0
	movs	r3, #0
	lsls	r0, r0, #9
	bl	Func_08090a5c
	ldr	r1, [pc, #32]
	ldr	r0, [pc, #36]
	bl	Func_080041d8
	add	sp, #4
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00002a04
	.4byte 0x040000d4
	.4byte 0x85000a81
	.4byte 0x84000070
	.4byte 0x05000200
	.4byte 0x00000c8f
	.4byte 0x080908e1
