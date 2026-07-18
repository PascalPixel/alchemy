@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08097adc
	.thumb_func
Func_08097adc:
	push	{r5, r6, lr}
	ldr	r3, [pc, #76]
	ldr	r0, [pc, #76]
	ldr	r5, [r3, #0]
	bl	Func_08004278
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #48]
	ldr	r6, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #68]
	strh	r6, [r3, #0]
	ldr	r3, [pc, #44]
	adds	r2, #20
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r1, [pc, #56]
	ldr	r3, [pc, #60]
	adds	r2, r3, r1
	ldrb	r0, [r2, #0]
	ldr	r2, [pc, #56]
	adds	r3, r3, r2
	ldrb	r1, [r3, #0]
	bl	Func_08015360
	ldr	r3, [pc, #52]
	adds	r5, r5, r3
	strb	r6, [r5, #0]
	b.n	.L0
	movs	r0, r0
	.4byte 0x00007fff
	.4byte 0x00000000
	.4byte 0x0000294a
	.4byte 0x00005294
	.4byte 0x03001e8c
	.4byte 0x08097869
	.4byte 0x050001e2
	.4byte 0x050001e6
	.4byte 0x00000205
	.4byte 0x02000240
	.4byte 0x00000206
	.4byte 0x00000ea4
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
