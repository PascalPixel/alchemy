@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08097540
	.thumb_func
Func_08097540:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	mov	r8, r1
	movs	r1, #166
	lsls	r1, r1, #2
	adds	r6, r0, #0
	movs	r0, #22
	sub	sp, #4
	bl	Func_080048f4
	adds	r5, r0, #0
	bl	Func_08097384
	movs	r3, #0
	mov	r0, sp
	str	r3, [r0, #0]
	adds	r1, r5, #0
	ldr	r3, [pc, #136]
	ldr	r2, [pc, #140]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #136]
	ldr	r0, [r3, #0]
	movs	r1, #180
	lsls	r1, r1, #1
	lsls	r0, r0, #1
	bl	Func_08002304
	ldr	r2, [pc, #128]
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	bl	Func_080978c4
	ldr	r2, [pc, #120]
	adds	r3, r5, r2
	subs	r2, #1
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	subs	r2, #1
	lsls	r3, r3, #5
	lsls	r0, r0, #10
	orrs	r0, r3
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	orrs	r0, r3
	movs	r3, #128
	lsls	r3, r3, #14
	orrs	r0, r3
	movs	r1, #1
	bl	Func_08091200
	movs	r0, #8
	bl	Func_08091254
	movs	r2, #164
	lsls	r2, r2, #2
	adds	r3, r5, r2
	adds	r2, #2
	strh	r6, [r3, #0]
	adds	r3, r5, r2
	mov	r2, r8
	strh	r2, [r3, #0]
	movs	r3, #165
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r3, #8
	strb	r3, [r5, #0]
	bl	Func_08097a7c
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #36]
	bl	Func_080041d8
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0x850000a6
	.4byte 0x03001e40
	.4byte 0x0000028e
	.4byte 0x0000028d
	.4byte 0x08097645
