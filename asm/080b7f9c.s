@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b7f9c
	.thumb_func
Func_080b7f9c:
	push	{r5, r6, lr}
	ldr	r3, [pc, #88]
	ldr	r5, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #6
	strh	r3, [r5, #54]
	movs	r3, #254
	lsls	r3, r3, #8
	strh	r3, [r5, #52]
	movs	r3, #255
	movs	r6, #0
	lsls	r3, r3, #17
	str	r3, [r5, #32]
	str	r6, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #20]
	str	r6, [r5, #28]
	str	r6, [r5, #24]
	sub	sp, #12
	bl	Func_080049ac
	adds	r0, r5, #0
	adds	r0, #12
	bl	Func_08004cb4
	movs	r3, #54
	ldrsh	r0, [r5, r3]
	bl	Func_08004c1c
	movs	r3, #52
	ldrsh	r0, [r5, r3]
	bl	Func_08004bd4
	mov	r0, sp
	str	r6, [r0, #0]
	str	r6, [r0, #4]
	ldr	r3, [r5, #32]
	adds	r1, r5, #0
	str	r3, [r0, #8]
	ldr	r3, [pc, #16]
	bl	Func_080072f0
	add	sp, #12
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001e80
	.4byte 0x03000250
