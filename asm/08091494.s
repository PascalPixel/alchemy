@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08091494
	.thumb_func
Func_08091494:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	movs	r1, #28
	movs	r0, #36
	sub	sp, #4
	bl	Func_080048f4
	movs	r1, #128
	adds	r7, r0, #0
	lsls	r1, r1, #3
	movs	r0, #14
	bl	Func_080048b0
	ldr	r3, [pc, #112]
	adds	r4, r0, #0
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [pc, #104]
	adds	r1, r4, #0
	ldr	r2, [pc, #104]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	adds	r2, r4, #0
	lsls	r1, r1, #2
	movs	r0, #94
	bl	Func_08003fa4
	movs	r0, #14
	bl	Func_08002dd8
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #80]
	bl	Func_080041d8
	ldr	r2, [pc, #80]
	ldr	r3, [pc, #80]
	strh	r2, [r3, #0]
	movs	r2, #16
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r2, #31
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [pc, #44]
	adds	r1, r7, #0
	ldr	r2, [pc, #60]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	cmp	r6, #0
	bne.n	.L0
	ldr	r3, [pc, #52]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	adds	r6, r0, #0
.L0:
	str	r6, [r7, #24]
	add	sp, #4
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x11111111
	.4byte 0x040000d4
	.4byte 0x85000080
	.4byte 0x080912b9
	.4byte 0x00003f9e
	.4byte 0x04000050
	.4byte 0x85000007
	.4byte 0x02000240
