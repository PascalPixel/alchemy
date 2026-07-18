@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08026e80
	.thumb_func
Func_08026e80:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #276]
	ldr	r7, [r3, #0]
	sub	sp, #8
	cmp	r7, #0
	bne.n	.L0
	b.n	.L1
.L0:
	ldr	r0, [r7, #40]
	ldr	r5, [r7, #44]
	cmp	r0, r5
	beq.n	.L2
	subs	r6, r0, r5
	adds	r0, r6, #0
	movs	r1, #3
	bl	Func_080022ec
	cmp	r0, #0
	bne.n	.L3
	subs	r0, #1
	cmp	r6, #0
	blt.n	.L3
	movs	r0, #1
.L3:
	adds	r0, r5, r0
	str	r0, [r7, #44]
	bl	Func_080b50d0
.L2:
	adds	r5, r7, #0
	adds	r6, r7, #0
	adds	r5, #36
	movs	r2, #2
.L5:
	ldrb	r3, [r5, #0]
	adds	r5, #1
	cmp	r3, #0
	beq.n	.L4
	adds	r0, r6, #0
	movs	r1, #240
	str	r2, [sp, #4]
	bl	Func_08003dec
	ldr	r2, [sp, #4]
.L4:
	subs	r2, #1
	adds	r6, #12
	cmp	r2, #0
	bge.n	.L5
	ldr	r0, [pc, #192]
	bl	Func_080219c8
	ldr	r3, [r7, #80]
	cmp	r3, #0
	beq.n	.L1
	ldr	r3, [pc, #184]
	ldr	r2, [r3, #0]
	adds	r3, r2, #0
	adds	r3, #82
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L6
	movs	r3, #0
	str	r3, [r7, #76]
	b.n	.L1
.L9:
	ldrh	r3, [r2, #12]
	cmp	r3, #86
	bne.n	.L7
	ldrh	r3, [r2, #14]
	cmp	r3, #83
	bne.n	.L7
	movs	r3, #225
	lsls	r3, r3, #2
	str	r3, [r7, #76]
	adds	r6, r3, #0
	b.n	.L7
.L6:
	ldr	r6, [r7, #76]
	cmp	r6, #0
	bge.n	.L8
	adds	r3, r2, #0
	adds	r3, #80
	ldrb	r2, [r3, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #128]
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldrh	r3, [r2, #8]
	cmp	r3, #69
	bne.n	.L7
	ldrh	r3, [r2, #10]
	cmp	r3, #68
	beq.n	.L9
.L7:
	cmp	r6, #0
	blt.n	.L1
.L8:
	ldr	r3, [r7, #68]
	cmp	r3, #0
	bne.n	.L10
	ldr	r3, [r7, #72]
	cmp	r3, #0
	bne.n	.L10
	bl	Func_08021c34
	ldr	r6, [r7, #76]
	str	r0, [r7, #68]
.L10:
	cmp	r6, #0
	ble.n	.L11
	subs	r3, r6, #1
	str	r3, [r7, #76]
	adds	r6, r3, #0
.L11:
	cmp	r6, #0
	blt.n	.L1
	adds	r0, r6, #0
	adds	r0, #59
	movs	r1, #60
	bl	Func_080022ec
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L12
	lsls	r3, r5, #4
	subs	r3, r3, r5
	lsls	r3, r3, #2
	cmp	r3, r6
	bne.n	.L12
	movs	r0, #108
	bl	Func_080f9010
.L12:
	ldr	r2, [r7, #68]
	cmp	r2, #0
	beq.n	.L1
	movs	r3, #8
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #2
	movs	r3, #16
	bl	Func_0801ea08
.L1:
	add	sp, #8
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f34
	.4byte 0x06006680
	.4byte 0x03001e74
	.4byte 0x02002024
