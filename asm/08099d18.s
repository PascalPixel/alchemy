@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08099d18
	.thumb_func
Func_08099d18:
	push	{r5, r6, lr}
	ldr	r3, [pc, #120]
	ldr	r3, [r3, #0]
	ldr	r5, [r3, #20]
	sub	sp, #12
	ldr	r3, [r5, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	bl	Func_08004458
	ldr	r3, [r5, #12]
	lsls	r0, r0, #4
	movs	r2, #192
	lsls	r2, r2, #13
	subs	r3, r3, r0
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl	Func_08004458
	lsls	r5, r0, #1
	adds	r5, r5, r0
	bl	Func_08004458
	lsls	r5, r5, #4
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	Func_0800447c
	ldr	r0, [pc, #64]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl	Func_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r3, [pc, #40]
	movs	r1, #0
	str	r3, [r5, #72]
	bl	Func_08009080
	adds	r2, r5, #0
	adds	r2, #94
	movs	r3, #12
	strh	r3, [r2, #0]
	ldr	r1, [pc, #28]
	adds	r0, r5, #0
	bl	Func_08009098
.L0:
	add	sp, #12
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
	.4byte 0x0000011d
	.4byte 0x00001999
	.4byte 0x0809f0b0
