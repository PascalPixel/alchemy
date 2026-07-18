@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bd850
	.thumb_func
Func_080bd850:
	push	{lr}
	mov	ip, r3
	mov	r3, r9
	push	{r3}
	mov	r3, ip
	sub	sp, #4
	mov	r3, r9
	str	r3, [sp, #0]
	ldrb	r3, [r0, #28]
	ldr	r2, [pc, #40]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r2, [r3, #2]
	ldr	r3, [pc, #36]
	adds	r2, r2, r3
	adds	r3, r0, #0
	adds	r3, #32
	adds	r0, #33
	ldrb	r1, [r3, #0]
	ldrb	r3, [r0, #0]
	adds	r0, r2, #0
	muls	r1, r3
	ldr	r3, [pc, #20]
	bl	Func_080072f0
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r0}
	bx	r0
	.4byte 0x03001b10
	.4byte 0x06010000
	.4byte 0x03000164
