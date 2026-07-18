@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08094ac8
	.thumb_func
Func_08094ac8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r1, #130
	lsls	r1, r1, #3
	movs	r0, #29
	sub	sp, #8
	bl	Func_080048f4
	ldr	r3, [pc, #180]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	movs	r0, #170
	mov	r8, r3
	bl	Func_08091ff0
	movs	r6, #0
	adds	r7, r5, #0
	add	r0, sp, #4
	adds	r7, #8
	str	r6, [r0, #0]
	ldr	r3, [pc, #156]
	adds	r1, r5, #0
	ldr	r2, [pc, #156]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #14
	bl	Func_080048f4
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [pc, #140]
	bl	Func_080053e8
	bl	Func_08004080
	movs	r1, #192
	str	r0, [r5, #0]
	lsls	r1, r1, #2
	adds	r2, r6, #0
	bl	Func_08003fa4
	str	r0, [r5, #4]
	movs	r0, #14
	bl	Func_08002dd8
	movs	r5, #0
.L0:
	movs	r4, #0
	adds	r6, r7, #0
	stmia	r6!, {r4}
	ldr	r3, [pc, #108]
	stmia	r6!, {r3}
	movs	r3, #212
	lsls	r3, r3, #8
	str	r3, [r6, #0]
	mov	r3, r8
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #8]
	movs	r0, #0
	str	r1, [r7, #12]
	str	r2, [r7, #20]
	asrs	r1, r1, #16
	asrs	r2, r2, #16
	str	r4, [sp, #0]
	bl	Func_080091a8
	ldr	r2, [pc, #56]
	adds	r3, r5, #0
	ands	r3, r2
	lsls	r0, r0, #16
	adds	r3, #1
	adds	r5, #1
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	ldr	r4, [sp, #0]
	adds	r7, #32
	cmp	r5, #31
	bls.n	.L0
	ldr	r3, [pc, #56]
	movs	r2, #252
	lsls	r2, r2, #6
	strh	r2, [r3, #0]
	ldr	r2, [pc, #52]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r4, [r3, #0]
	ldr	r0, [pc, #44]
	movs	r1, #200
	lsls	r1, r1, #4
	bl	Func_080041d8
	add	sp, #8
	b.n	.L1
	movs	r0, r0
	.4byte 0x0000000f
	.4byte 0x03001e70
	.4byte 0x040000d4
	.4byte 0x85000104
	.4byte 0x0809ff58
	.4byte 0x40000400
	.4byte 0x04000050
	.4byte 0x00001008
	.4byte 0x08094821
.L1:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
