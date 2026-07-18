@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b0444
	.thumb_func
Func_080b0444:
	push	{r5, r6, lr}
	ldr	r3, [pc, #104]
	ldr	r2, [pc, #104]
	str	r2, [r3, #16]
	movs	r2, #142
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #28
	strb	r2, [r3, #0]
	ldr	r1, [pc, #96]
	movs	r0, #1
	bl	Func_08077028
	adds	r1, r0, #0
	movs	r0, #1
	bl	Func_08077050
	ldr	r1, [pc, #84]
	movs	r0, #0
	bl	Func_08077028
	adds	r1, r0, #0
	movs	r0, #0
	bl	Func_08077050
	movs	r1, #231
	movs	r0, #2
	bl	Func_08077028
	movs	r0, #3
	bl	Func_08077008
	ldr	r6, [pc, #56]
	movs	r5, #1
	strb	r5, [r0, r6]
	movs	r0, #5
	bl	Func_08077008
	strb	r5, [r0, r6]
	movs	r0, #2
	bl	Func_08077008
	movs	r3, #160
	lsls	r3, r3, #1
	adds	r0, r0, r3
	strb	r5, [r0, #0]
	movs	r1, #30
	movs	r0, #1
	bl	Func_080b0278
	movs	r0, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x02000240
	.4byte 0x00030d40
	.4byte 0x0000048d
	.4byte 0x0000040b
	.4byte 0x00000131
