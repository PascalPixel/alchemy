@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080df90c
	.thumb_func
Func_080df90c:
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	adds	r6, r1, #0
	mov	fp, r2
	bl	Func_080b5098
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	Func_080b5098
	ldr	r6, [r5, #0]
	ldr	r0, [r0, #0]
	movs	r2, #80
	mov	sl, r2
	ldr	r3, [r0, #8]
	ldr	r2, [r6, #8]
	subs	r3, r3, r2
	mov	r8, r0
	movs	r1, #100
	mov	r0, sl
	muls	r0, r3
	mov	r9, r2
	bl	Func_080022ec
	mov	r2, r8
	ldr	r3, [r2, #16]
	ldr	r2, [r6, #16]
	subs	r3, r3, r2
	adds	r5, r0, #0
	movs	r1, #100
	mov	r0, sl
	muls	r0, r3
	mov	r8, r2
	bl	Func_080022ec
	add	r9, r5
	add	r8, r0
	asrs	r5, r5, #8
	asrs	r0, r0, #8
	adds	r2, r0, #0
	muls	r2, r0
	adds	r3, r5, #0
	muls	r3, r5
	adds	r3, r3, r2
	adds	r0, r3, #0
	ldr	r2, [pc, #84]
	bl	Func_080072ec
	mov	r1, fp
	lsls	r0, r0, #8
	bl	Func_080022ec
	adds	r3, r6, #0
	movs	r1, #1
	adds	r3, #88
	str	r0, [r6, #52]
	str	r0, [r6, #48]
	strb	r1, [r3, #0]
	ldr	r3, [pc, #64]
	str	r3, [r6, #72]
	adds	r3, r6, #0
	movs	r2, #0
	adds	r3, #90
	str	r2, [r6, #40]
	str	r2, [r6, #68]
	adds	r0, r6, #0
	strb	r1, [r3, #0]
	bl	Func_08009140
	adds	r0, r6, #0
	mov	r1, r9
	movs	r2, #0
	mov	r3, r8
	bl	Func_08009150
	adds	r0, r6, #0
	movs	r1, #2
	bl	Func_08009080
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x030001d8
	.4byte 0x0000ab85
