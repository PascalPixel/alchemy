@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0802875c
	.thumb_func
Func_0802875c:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r3, #128
	lsls	r3, r3, #3
	mov	r8, r3
	mov	sl, r0
	mov	r0, r8
	adds	r5, r1, #0
	bl	Func_08004938
	adds	r6, r0, #0
	ldr	r0, [pc, #44]
	bl	Func_08002f40
	lsls	r5, r5, #1
	ldrh	r3, [r5, r0]
	adds	r1, r6, #0
	adds	r0, r0, r3
	bl	Func_080053e8
	mov	r0, sl
	mov	r1, r8
	adds	r2, r6, #0
	bl	Func_08003fa4
	adds	r0, r6, #0
	bl	Func_08002df0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x000000f1
