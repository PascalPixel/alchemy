@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08098848
	.thumb_func
Func_08098848:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #244]
	ldr	r6, [r3, #0]
	ldr	r7, [r6, #20]
	sub	sp, #20
	ldr	r5, [r6, #16]
	cmp	r7, #0
	beq.n	.L0
	bl	Func_08097384
	adds	r0, r5, #0
	str	r7, [r5, #104]
	ldr	r1, [pc, #224]
	bl	Func_08009098
	ldr	r0, [r6, #4]
	add	r5, sp, #8
	str	r0, [r5, #0]
	movs	r2, #128
	ldr	r1, [r6, #8]
	lsls	r2, r2, #13
	adds	r1, r1, r2
	str	r1, [r5, #4]
	movs	r3, #128
	ldr	r2, [r6, #12]
	lsls	r3, r3, #14
	adds	r0, r0, r3
	movs	r3, #128
	str	r2, [r5, #8]
	lsls	r3, r3, #8
	bl	Func_08098a84
	ldr	r2, [pc, #188]
	str	r0, [sp, #0]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r0, r0, r2
	movs	r3, #0
	ldr	r2, [r5, #8]
	bl	Func_08098a84
	str	r0, [sp, #4]
	movs	r0, #15
	mov	r8, sp
	bl	Func_080030f8
	mov	r6, r8
	movs	r5, #1
.L2:
	ldmia	r6!, {r0}
	cmp	r0, #0
	beq.n	.L1
	movs	r1, #224
	ldrh	r2, [r0, #6]
	lsls	r1, r1, #12
	bl	Func_08096bec
.L1:
	subs	r5, #1
	cmp	r5, #0
	bge.n	.L2
	ldr	r0, [sp, #0]
	bl	Func_08009158
	ldr	r3, [pc, #132]
	movs	r0, #130
	str	r3, [r7, #108]
	bl	Func_080f9010
	adds	r2, r7, #0
	adds	r2, #85
	movs	r3, #4
	ldr	r0, [sp, #0]
	strb	r3, [r2, #0]
	ldr	r5, [r7, #12]
	cmp	r0, #0
	beq.n	.L3
	mov	r2, r8
	ldr	r3, [r2, #4]
	cmp	r3, #0
	beq.n	.L3
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r3, r5, r2
	cmp	r5, r3
	bgt.n	.L3
	b.n	.L4
.L5:
	ldr	r0, [sp, #0]
.L4:
	ldr	r3, [r0, #12]
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r3, r3, r1
	str	r3, [r0, #12]
	mov	r3, r8
	ldr	r2, [r3, #4]
	ldr	r3, [r2, #12]
	adds	r3, r3, r1
	str	r3, [r2, #12]
	ldr	r3, [r7, #12]
	adds	r3, r3, r1
	str	r3, [r7, #12]
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r2, r5, r3
	ldr	r3, [r7, #12]
	cmp	r3, r2
	ble.n	.L5
	ldr	r0, [sp, #0]
.L3:
	bl	Func_080981b0
	mov	r2, r8
	ldr	r0, [r2, #4]
	bl	Func_080981b0
	bl	Func_0809748c
.L0:
	add	sp, #20
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
	.4byte 0x0809f0bc
	.4byte 0xffe00000
	.4byte 0x08096b89
