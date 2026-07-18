@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a9a5c
	.thumb_func
Func_080a9a5c:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r3, [pc, #124]
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	adds	r0, r1, #0
	mov	sl, r2
	mov	r8, r3
	bl	Func_08077008
	bl	Func_080a9cbc
	bl	Func_080a345c
	ldr	r5, [pc, #104]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	adds	r0, r5, #1
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #32
	bl	Func_08015080
	adds	r0, r5, #2
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #16
	adds	r5, #3
	bl	Func_08015080
	adds	r0, r5, #0
	movs	r5, #228
	lsls	r5, r5, #1
	movs	r3, #48
	adds	r1, r6, #0
	movs	r2, #0
	add	r5, r8
	bl	Func_08015080
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080a9aec
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L0
	movs	r0, #1
	bl	Func_080030f8
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_080a3e28
	adds	r0, r5, #0
	bl	Func_080a9c18
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x00000b24
