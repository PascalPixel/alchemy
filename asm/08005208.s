@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08005208
	.thumb_func
Func_08005208:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	adds	r5, r0, #0
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	asrs	r5, r5, #1
	adds	r0, r5, #0
	mov	sl, r2
	mov	r8, r1
	bl	Func_08002322
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	Func_0800231c
	lsls	r1, r0, #2
	adds	r1, r1, r0
	lsls	r1, r1, #4
	ldr	r3, [pc, #28]
	adds	r0, r6, #0
	bl	Func_080072f0
	ldr	r3, [pc, #24]
	mov	r2, r8
	str	r2, [r3, #4]
	mov	r2, sl
	str	r0, [r3, #0]
	str	r2, [r3, #8]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x0300013c
	.4byte 0x03001ce0
