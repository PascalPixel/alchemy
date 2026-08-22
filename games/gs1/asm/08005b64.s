@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08005b64
	.thumb_func
Func_08005b64:
	push	{r5, r6, lr}
	ldr	r3, [pc, #136]
	sub	sp, #20
	adds	r5, r0, #0
	ldr	r6, [r3, #0]
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	add	r1, sp, #4
	ldr	r3, [pc, #124]
	ldr	r2, [pc, #124]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	ldr	r1, [pc, #112]
	lsls	r2, r2, #24
.L0:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L0
	ldr	r3, [pc, #100]
	ldr	r0, [pc, #108]
	add	r1, sp, #4
	ldr	r2, [pc, #108]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #128
	ldr	r1, [pc, #88]
	lsls	r3, r3, #24
.L1:
	ldr	r2, [r1, #8]
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L1
	add	r0, sp, #4
	movs	r3, #16
	adds	r1, r6, #0
	strb	r3, [r0, #7]
	strh	r2, [r0, #10]
	ldr	r3, [pc, #64]
	adds	r1, #64
	ldr	r2, [pc, #76]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	ldr	r1, [pc, #52]
	lsls	r2, r2, #24
.L2:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L2
	adds	r0, r5, #0
	bl	Func_08005868
	cmp	r0, #0
	beq.n	.L3
	movs	r0, #1
	b.n	.L4
.L3:
	adds	r2, r5, #0
	movs	r3, #16
	adds	r2, #16
	strb	r0, [r6, r5]
	strb	r3, [r6, r2]
	lsls	r3, r5, #1
	adds	r3, #32
	strh	r0, [r6, r3]
	movs	r0, #0
.L4:
	add	sp, #20
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001f1c
	.4byte 0x040000d4
	.4byte 0x85000004
	.4byte 0x080079b8
	.4byte 0x84000002
	.4byte 0x84000004
