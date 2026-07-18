@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08021c64
	.thumb_func
Func_08021c64:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r5, r1, #0
	movs	r1, #193
	mov	r8, r0
	lsls	r1, r1, #3
	movs	r0, #17
	bl	Func_080048b0
	adds	r6, r0, #0
	ldr	r0, [pc, #52]
	bl	Func_08002f40
	ldr	r3, [pc, #48]
	lsls	r5, r5, #1
	adds	r2, r6, r3
	ldrh	r3, [r5, r0]
	adds	r0, r0, r3
	str	r0, [r2, #0]
	adds	r1, r6, #0
	bl	Func_080053e8
	adds	r1, r6, #0
	mov	r0, r8
	bl	Func_080040d0
	adds	r5, r0, #0
	movs	r0, #17
	bl	Func_08002dd8
	adds	r0, r5, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000000f1
	.4byte 0x00000604
