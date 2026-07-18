@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08094730
	.thumb_func
Func_08094730:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	movs	r1, #242
	adds	r5, r0, #0
	lsls	r1, r1, #4
	movs	r0, #34
	sub	sp, #4
	mov	r8, r2
	adds	r7, r3, #0
	bl	Func_080048f4
	movs	r3, #0
	adds	r4, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	adds	r1, r4, #0
	ldr	r3, [pc, #104]
	ldr	r2, [pc, #108]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	adds	r1, r3, #0
	lsls	r2, r2, #24
.L0:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L0
	ldr	r2, [pc, #92]
	adds	r3, r4, r2
	adds	r2, #7
	strb	r5, [r3, #0]
	adds	r3, r4, r2
	str	r6, [r3, #0]
	ldr	r3, [pc, #84]
	adds	r2, r4, r3
	ldr	r3, [sp, #24]
	str	r3, [r2, #0]
	ldr	r2, [pc, #80]
	adds	r3, r4, r2
	str	r7, [r3, #0]
	ldr	r3, [pc, #76]
	adds	r2, r4, r3
	ldr	r3, [sp, #32]
	str	r3, [r2, #0]
	movs	r2, #241
	lsls	r2, r2, #4
	adds	r3, r4, r2
	mov	r2, r8
	str	r2, [r3, #0]
	ldr	r3, [pc, #64]
	adds	r2, r4, r3
	ldr	r3, [sp, #28]
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #56]
	bl	Func_080041d8
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl	Func_080041d8
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0x850003c8
	.4byte 0x00000f01
	.4byte 0x00000f0c
	.4byte 0x00000f18
	.4byte 0x00000f1c
	.4byte 0x00000f14
	.4byte 0x08094545
	.4byte 0x080944ed
