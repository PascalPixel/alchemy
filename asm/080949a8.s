@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080949a8
	.thumb_func
Func_080949a8:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #252]
	movs	r1, #252
	ldr	r6, [r3, #0]
	lsls	r1, r1, #5
	adds	r5, r6, r1
	ldr	r7, [r3, #8]
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	blt.n	.L0
	movs	r0, #179
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L1
	movs	r3, #128
	strh	r3, [r5, #0]
.L1:
	ldrh	r3, [r5, #0]
	subs	r2, r3, #1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	strh	r2, [r5, #0]
	cmp	r3, #11
	bhi.n	.L0
	ldr	r2, [pc, #204]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08094a14
	.4byte 0x08094a86
	.4byte 0x08094aa2
	.4byte 0x08094aa2
	.4byte 0x08094aa2
	.4byte 0x08094a62
	.4byte 0x08094a86
	.4byte 0x08094aa2
	.4byte 0x08094aa2
	.4byte 0x08094aa2
	.4byte 0x08094a62
	.4byte 0x08094a86
	ldr	r1, [pc, #152]
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L2
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	movs	r2, #100
	muls	r2, r0
	lsls	r3, r5, #1
	adds	r3, r3, r5
	lsls	r3, r3, #3
	adds	r3, r3, r5
	lsls	r3, r3, #4
	lsrs	r2, r2, #16
	lsrs	r3, r3, #16
	movs	r1, #252
	subs	r3, r3, r2
	lsls	r1, r1, #5
	adds	r2, r6, r1
	adds	r3, #150
	strh	r3, [r2, #0]
	ldr	r2, [pc, #104]
	adds	r3, r6, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L3
	movs	r0, #172
	bl	Func_080f9010
	b.n	.L2
.L3:
	movs	r0, #171
	bl	Func_080f9010
.L2:
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08091200
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r0, r6, r2
	movs	r2, #196
	lsls	r2, r2, #5
	adds	r1, r7, r2
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #64]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #64]
	adds	r2, r7, r3
	movs	r3, #12
	b.n	.L4
	movs	r3, #168
	lsls	r3, r3, #4
	adds	r0, r6, r3
	movs	r1, #1
	bl	Func_08091200
	ldr	r1, [pc, #44]
	movs	r3, #1
	adds	r2, r7, r1
.L4:
	strb	r3, [r2, #0]
	ldr	r2, [pc, #40]
	movs	r1, #0
	adds	r3, r7, r2
	strb	r1, [r3, #0]
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ec8
	.4byte 0x080949e4
	.4byte 0x00001f82
	.4byte 0x00001f84
	.4byte 0x040000d4
	.4byte 0x840002a0
	.4byte 0x00002a01
	.4byte 0x00002a02
