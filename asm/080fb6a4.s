@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fb6a4
	.thumb_func
Func_080fb6a4:
	push	{r4, lr}
	ldr	r2, [r1, #64]
	ldr	r0, [pc, #48]
	ands	r4, r0
	ldrb	r0, [r2, #0]
	orrs	r4, r0
	ldrb	r0, [r2, #1]
	lsls	r3, r0, #8
	ldr	r0, [pc, #40]
	ands	r4, r0
	orrs	r4, r3
	ldrb	r0, [r2, #2]
	lsls	r3, r0, #16
	ldr	r0, [pc, #36]
	ands	r4, r0
	orrs	r4, r3
	ldrb	r0, [r2, #3]
	lsls	r3, r0, #24
	ldr	r0, [pc, #28]
	ands	r4, r0
	orrs	r4, r3
	str	r4, [r1, #40]
	adds	r2, #4
	str	r2, [r1, #64]
	pop	{r4}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0xff00ffff
	.4byte 0x00ffffff
