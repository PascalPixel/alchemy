@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080118d8
	.thumb_func
Func_080118d8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #140]
	ldr	r5, [r3, #0]
	sub	sp, #4
	adds	r4, r0, #0
	movs	r6, #0
	mov	r0, sp
	adds	r1, r5, #0
	str	r6, [r0, #0]
	ldr	r3, [pc, #128]
	adds	r1, #24
	ldr	r2, [pc, #128]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldrh	r1, [r4, #0]
	ldr	r2, [pc, #124]
	adds	r4, #2
	cmp	r1, r2
	beq.n	.L0
	movs	r3, #255
	movs	r2, #253
	lsls	r3, r3, #8
	lsls	r2, r2, #8
	mov	sl, r3
	mov	r8, r2
	movs	r3, #15
	movs	r2, #128
	mov	lr, r3
	mov	ip, r2
	movs	r7, #0
.L3:
	adds	r3, r1, #0
	mov	r2, sl
	ands	r3, r2
	cmp	r3, r8
	bne.n	.L1
	mov	r3, lr
	adds	r2, r1, #0
	ands	r2, r3
	mov	r3, ip
	ands	r3, r1
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L2
	movs	r0, #1
.L2:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r5, r3
	adds	r3, #24
	str	r4, [r3, #0]
	str	r4, [r3, #4]
	strh	r7, [r3, #8]
	strh	r0, [r3, #10]
	adds	r6, #1
.L1:
	ldrh	r1, [r4, #0]
	ldr	r2, [pc, #44]
	adds	r4, #2
	cmp	r1, r2
	bne.n	.L3
.L0:
	cmp	r6, #0
	beq.n	.L4
	movs	r1, #200
	ldr	r0, [pc, #36]
	lsls	r1, r1, #4
	bl	Func_080041d8
.L4:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0x040000d4
	.4byte 0x85000030
	.4byte 0x0000ffff
	.4byte 0x0801179d
