@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809b86c
	.thumb_func
Func_0809b86c:
	push	{r5, r6, r7, lr}
	adds	r4, r0, #0
	adds	r3, r4, #0
	adds	r3, #71
	ldrb	r2, [r3, #0]
	movs	r3, #4
	ands	r3, r2
	sub	sp, #24
	ldr	r5, [r4, #0]
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L0
	ldr	r2, [r4, #8]
	movs	r3, #254
	lsls	r3, r3, #17
	subs	r6, r3, r2
.L0:
	ldr	r3, [pc, #84]
	ldr	r0, [r4, #40]
	ldr	r1, [r5, #24]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [sp, #0]
	ldr	r0, [r4, #44]
	ldr	r1, [r5, #24]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	mov	r7, sp
	str	r0, [r7, #4]
	ldr	r2, [r4, #4]
	add	r1, sp, #8
	str	r2, [r1, #0]
	str	r6, [r1, #4]
	ldr	r0, [r4, #8]
	adds	r3, r0, r6
	str	r3, [r1, #8]
	movs	r3, #0
	str	r3, [r1, #12]
	ldr	r3, [pc, #44]
	adds	r2, r2, r3
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bhi.n	.L1
	ldr	r3, [pc, #40]
	cmp	r0, r3
	ble.n	.L1
	movs	r3, #224
	lsls	r3, r3, #16
	cmp	r0, r3
	bge.n	.L1
	adds	r0, r5, #0
	adds	r2, r7, #0
	movs	r3, #0
	bl	Func_08009008
.L1:
	add	sp, #24
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03000118
	.4byte 0x001fffff
	.4byte 0x012ffffe
	.4byte 0xffe00000
