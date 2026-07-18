@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b8228
	.thumb_func
Func_080b8228:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	mov	r8, r1
	bl	Func_080b7dd0
	adds	r7, r0, #0
	adds	r0, r5, #0
	ldr	r6, [r7, #0]
	bl	Func_08077008
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r0, r0, r2
	ldrb	r3, [r0, #0]
	cmp	r3, #148
	beq.n	.L0
	ldr	r3, [pc, #96]
	mov	r2, r8
	lsls	r5, r2, #2
	ldr	r3, [r3, r5]
	str	r3, [r6, #52]
	ldr	r3, [pc, #92]
	ldr	r3, [r3, r5]
	str	r3, [r6, #48]
	ldr	r3, [r6, #12]
	cmp	r3, #0
	beq.n	.L1
	cmp	r2, #4
	ble.n	.L2
.L1:
	ldr	r3, [pc, #80]
	ldr	r3, [r3, r5]
	str	r3, [r6, #40]
.L2:
	ldr	r3, [pc, #76]
	adds	r2, r6, #0
	str	r3, [r6, #72]
	adds	r2, #90
	movs	r3, #0
	str	r3, [r6, #68]
	adds	r0, r6, #0
	strb	r3, [r2, #0]
	bl	Func_08009140
	ldr	r3, [pc, #60]
	ldr	r2, [r7, #12]
	ldr	r3, [r3, r5]
	movs	r1, #100
	adds	r0, r3, #0
	muls	r0, r2
	bl	Func_080022ec
	ldr	r3, [r7, #16]
	adds	r1, r0, #0
	movs	r2, #0
	adds	r0, r6, #0
	bl	Func_08009150
.L0:
	adds	r0, r6, #0
	movs	r1, #5
	bl	Func_08009080
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080c59a4
	.4byte 0x080c59c4
	.4byte 0x080c59e4
	.4byte 0x00009999
	.4byte 0x080c5a04
