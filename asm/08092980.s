@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08092980
	.thumb_func
Func_08092980:
	push	{lr}
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L0
	ldr	r3, [pc, #60]
	ldr	r3, [r3, #0]
	ldr	r0, [r0, #80]
	ldr	r1, [pc, #60]
	lsrs	r3, r3, #1
	movs	r2, #3
	ands	r3, r2
	mov	ip, r0
	ldrb	r4, [r1, r3]
	mov	r3, ip
	adds	r3, #39
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L1
	adds	r0, #40
	adds	r1, r3, #0
.L3:
	ldmia	r0!, {r2}
	cmp	r2, #0
	beq.n	.L2
	ldr	r3, [r2, #16]
	cmp	r3, #0
	beq.n	.L2
	strb	r4, [r2, #5]
.L2:
	subs	r1, #1
	cmp	r1, #0
	bne.n	.L3
.L1:
	mov	r2, ip
	adds	r2, #37
	movs	r3, #1
	strb	r3, [r2, #0]
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x03001e40
	.4byte 0x0809ed80
