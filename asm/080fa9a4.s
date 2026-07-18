@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fa9a4
	.thumb_func
Func_080fa9a4:
	push	{r4, lr}
	ldr	r0, [pc, #44]
	ldr	r2, [r0, #0]
	ldr	r3, [r2, #0]
	ldr	r0, [pc, #40]
	cmp	r3, r0
	beq.n	.L0
	ldr	r0, [pc, #40]
	movs	r4, #182
	lsls	r4, r4, #8
	adds	r1, r4, #0
	strh	r1, [r0, #0]
	adds	r0, #12
	strh	r1, [r0, #0]
	ldrb	r0, [r2, #4]
	movs	r0, #0
	strb	r0, [r2, #4]
	adds	r0, r3, #0
	subs	r0, #10
	str	r0, [r2, #0]
.L0:
	pop	{r4}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03007ff0
	.4byte 0x68736d53
	.4byte 0x040000c6
