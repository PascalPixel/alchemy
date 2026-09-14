@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080160fc
	.thumb_func
Func_080160fc:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #96]
	ldr	r2, [pc, #96]
	ldr	r7, [r3, #0]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L0
	subs	r2, #3
	adds	r3, r7, r2
	ldrb	r4, [r3, #0]
	cmp	r4, #0
	beq.n	.L0
	ldr	r3, [pc, #80]
	mov	ip, r3
	movs	r3, #1
	ands	r3, r4
	adds	r5, r7, #0
	cmp	r3, #0
	beq.n	.L1
	movs	r4, #63
.L1:
	movs	r3, #63
	ands	r4, r3
	movs	r2, #1
	movs	r6, #128
	lsrs	r4, r4, #1
	mov	lr, r2
	lsls	r6, r6, #1
.L3:
	adds	r3, r4, #0
	mov	r2, lr
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	ldr	r3, [pc, #44]
	adds	r0, r5, #0
	mov	r1, ip
	ldr	r2, [pc, #40]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L2:
	lsrs	r4, r4, #1
	adds	r5, r5, r6
	add	ip, r6
	cmp	r4, #0
	bne.n	.L3
	ldr	r2, [pc, #28]
	adds	r3, r7, r2
	strb	r4, [r3, #0]
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x00000ea6
	.4byte 0x06002000
	.4byte 0x040000d4
	.4byte 0x84000040
	.4byte 0x00000ea3
