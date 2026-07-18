@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080071a8
	.thumb_func
Func_080071a8:
	push	{r4, r5, r6, lr}
	adds	r5, r1, #0
	lsls	r0, r0, #16
	ldr	r3, [pc, #92]
	lsrs	r0, r0, #16
	adds	r4, r0, #0
	ldrb	r1, [r3, #28]
	lsls	r4, r1
	movs	r0, #224
	lsls	r0, r0, #20
	adds	r4, r4, r0
	ldr	r1, [pc, #80]
	ldrh	r0, [r1, #0]
	adds	r6, r0, #0
	movs	r0, #0
	strh	r0, [r1, #0]
	ldr	r2, [pc, #72]
	movs	r0, #170
	strb	r0, [r2, #0]
	ldr	r1, [pc, #72]
	movs	r0, #85
	strb	r0, [r1, #0]
	movs	r0, #160
	strb	r0, [r2, #0]
	ldr	r1, [r3, #24]
	cmp	r1, #0
	beq.n	.L0
.L1:
	ldrb	r0, [r5, #0]
	strb	r0, [r4, #0]
	adds	r5, #1
	adds	r4, #1
	subs	r1, #1
	cmp	r1, #0
	bne.n	.L1
.L0:
	subs	r4, #1
	subs	r5, #1
	ldr	r0, [pc, #28]
	strh	r6, [r0, #0]
	ldr	r0, [pc, #36]
	ldrb	r2, [r5, #0]
	ldr	r3, [r0, #0]
	movs	r0, #1
	adds	r1, r4, #0
	bl	Func_080072f0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x08007c10
	.4byte 0x04000208
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.4byte 0x02004c00
