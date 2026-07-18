@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08003adc
	.thumb_func
Func_08003adc:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #28]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [pc, #24]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L1
	ldr	r3, [pc, #20]
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #4]
	b.n	.L2
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x03001c98
	.4byte 0x03001cd4
	.4byte 0x03001cf8
.L1:
	ldr	r3, [pc, #64]
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #56]
.L2:
	orrs	r3, r2
	ldr	r2, [pc, #60]
	strh	r3, [r2, #0]
	ldr	r6, [pc, #60]
	ldrb	r3, [r6, #0]
	adds	r3, #255
	strb	r3, [r6, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #60]
	ldrb	r5, [r2, #0]
	ldrb	r3, [r3, #0]
	ldrb	r2, [r2, #0]
	ldr	r7, [pc, #56]
	subs	r3, r3, r2
	ldrb	r2, [r6, #0]
	ldrb	r1, [r7, #0]
	adds	r0, r2, #0
	muls	r0, r3
	bl	Func_080022ec
	ldr	r3, [pc, #44]
	adds	r5, r5, r0
	strh	r5, [r3, #0]
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L0
	strb	r3, [r7, #0]
	b.n	.L0
	movs	r0, r0
	.4byte 0x000000c0
	.4byte 0x03001cf8
	.4byte 0x04000050
	.4byte 0x03001ac0
	.4byte 0x03001ca8
	.4byte 0x03001aec
	.4byte 0x03001c98
	.4byte 0x04000054
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
