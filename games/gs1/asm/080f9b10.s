@ 音響列の反復回数を更新し、次の列位置へ進む補助関数。
@ 四バイト読出し関数の内部継続点へ末尾分岐する。
.syntax unified
	.thumb
	.set .L_080f9ac2, 0x080f9ac2
	.global Func_080f9b10
	.thumb_func
Func_080f9b10:
	push	{lr}
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.Lrepeat
	adds	r2, #1
	str	r2, [r1, #64]
	b.n	.L_080f9ac2
.Lrepeat:
	ldrb	r3, [r1, #3]
	adds	r3, #1
	strb	r3, [r1, #3]
	mov	ip, r3
	bl	Func_080f9ab4
	cmp	ip, r3
	bcs.n	.Ladvance
	b.n	.L_080f9ac2
.Ladvance:
	movs	r3, #0
	strb	r3, [r1, #3]
	adds	r2, #5
	str	r2, [r1, #64]
	pop	{r0}
	bx	r0
	.2byte 0
