@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801ef08
	.thumb_func
Func_0801ef08:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	r8, r0
	movs	r1, #16
	movs	r0, #16
	sub	sp, #4
	bl	Func_080048f4
	ldr	r3, [pc, #64]
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #64]
	adds	r6, r6, r3
	movs	r3, #0
	mov	sl, r3
	movs	r3, #1
	adds	r5, r0, #0
	strb	r3, [r6, #0]
	mov	r0, r8
	bl	Func_0801eea0
	ldrh	r3, [r5, #10]
	ldrh	r1, [r5, #6]
	ldrh	r2, [r5, #8]
	movs	r4, #6
	ldrh	r0, [r5, #4]
	str	r4, [sp, #0]
	bl	Func_080162d4
	str	r0, [r5, #0]
	mov	r0, r8
	bl	Func_0801f200
	mov	r3, sl
	strb	r3, [r6, #0]
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000ea6
