@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801d014
	.thumb_func
Func_0801d014:
	push	{lr}
	movs	r1, #197
	lsls	r1, r1, #3
	movs	r0, #20
	sub	sp, #4
	bl	Func_080048f4
	movs	r3, #0
	adds	r4, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	adds	r1, r4, #0
	ldr	r3, [pc, #124]
	ldr	r2, [pc, #128]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #124]
	ldr	r0, [pc, #128]
	adds	r3, r1, r0
	ldr	r0, [pc, #128]
	ldrb	r2, [r3, #0]
	adds	r3, r4, r0
	strb	r2, [r3, #0]
	ldr	r3, [pc, #124]
	ldr	r0, [pc, #124]
	adds	r2, r4, r3
	movs	r3, #24
	strb	r3, [r2, #0]
	adds	r3, r1, r0
	ldr	r0, [pc, #120]
	ldrb	r2, [r3, #0]
	adds	r3, r4, r0
	strb	r2, [r3, #0]
	ldr	r3, [pc, #116]
	movs	r0, #131
	adds	r2, r4, r3
	lsls	r0, r0, #2
	movs	r3, #15
	strb	r3, [r2, #0]
	adds	r3, r1, r0
	ldr	r0, [pc, #104]
	ldrb	r2, [r3, #0]
	adds	r3, r4, r0
	strb	r2, [r3, #0]
	ldr	r3, [pc, #100]
	ldr	r0, [pc, #104]
	adds	r2, r4, r3
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r3, r1, r0
	ldr	r0, [pc, #96]
	ldrb	r2, [r3, #0]
	adds	r3, r4, r0
	strb	r2, [r3, #0]
	ldr	r2, [pc, #92]
	movs	r0, #2
	adds	r3, r4, r2
	strb	r0, [r3, #0]
	ldr	r3, [pc, #88]
	adds	r1, r1, r3
	ldrb	r2, [r1, #0]
	movs	r1, #179
	lsls	r1, r1, #3
	adds	r3, r4, r1
	strb	r2, [r3, #0]
	ldr	r2, [pc, #80]
	movs	r1, #200
	adds	r4, r4, r2
	strb	r0, [r4, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #72]
	bl	Func_080041d8
	add	sp, #4
	pop	{r0}
	bx	r0
	.4byte 0x040000d4
	.4byte 0x8500018a
	.4byte 0x02000240
	.4byte 0x00000205
	.4byte 0x00000594
	.4byte 0x00000599
	.4byte 0x00000206
	.4byte 0x00000595
	.4byte 0x0000059a
	.4byte 0x00000596
	.4byte 0x0000059b
	.4byte 0x0000020a
	.4byte 0x00000597
	.4byte 0x0000059c
	.4byte 0x0000022a
	.4byte 0x0000059d
	.4byte 0x0801cf49
