@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809ad90
	.thumb_func
Func_0809ad90:
	push	{lr}
	bl	Func_0808ba1c
	cmp	r0, #0
	beq.n	.L0
	ldr	r1, [pc, #64]
	movs	r3, #148
	lsls	r3, r3, #2
	adds	r2, r1, r3
	ldr	r3, [r0, #108]
	str	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r1, r1, r3
	movs	r3, #0
	strb	r3, [r1, #0]
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L1
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	cmp	r3, #0
	beq.n	.L1
	ldrb	r3, [r3, #5]
	strb	r3, [r1, #0]
.L1:
	ldr	r3, [pc, #28]
	adds	r2, r0, #0
	str	r3, [r0, #108]
	adds	r2, #91
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r1, #0
	bl	Func_08009088
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000249
	.4byte 0x0809ad71
