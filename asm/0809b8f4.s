@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809b8f4
	.thumb_func
Func_0809b8f4:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	movs	r2, #128
	ldr	r0, [r6, #12]
	lsls	r2, r2, #24
	sub	sp, #4
	cmp	r0, r2
	bne.n	.L0
	b.n	.L1
.L0:
	ldr	r3, [r6, #4]
	ldr	r2, [r6, #16]
	subs	r7, r0, r3
	ldr	r3, [r6, #8]
	subs	r5, r2, r3
	adds	r3, r6, #0
	adds	r3, #65
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L2
	adds	r0, r7, #0
	cmp	r7, #0
	bge.n	.L3
	ldr	r3, [pc, #256]
	adds	r0, r7, r3
.L3:
	asrs	r0, r0, #16
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L4
	ldr	r2, [pc, #244]
	adds	r3, r5, r2
.L4:
	adds	r2, r0, #0
	muls	r2, r0
	asrs	r3, r3, #16
	adds	r0, r2, #0
	adds	r2, r3, #0
	muls	r2, r3
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #228]
	bl	Func_080072f0
	movs	r3, #128
	lsls	r0, r0, #16
	lsls	r3, r3, #16
	cmp	r0, r3
	bge.n	.L5
	ldr	r4, [pc, #216]
	adds	r0, r7, #0
	adds	r1, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r4
	adds	r3, r0, #0
	adds	r1, r5, #0
	adds	r0, r5, #0
	movs	r0, r0
	mov	ip, pc
	bx	r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	Func_080045d4
.L5:
	movs	r2, #128
	lsls	r2, r2, #12
	cmp	r0, r2
	bgt.n	.L2
	ldr	r1, [r6, #12]
	ldr	r2, [r6, #16]
	adds	r0, r6, #0
	bl	Func_0809ba5c
	b.n	.L1
.L2:
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	Func_080044d0
	adds	r3, r6, #0
	adds	r3, #66
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	lsls	r0, r0, #16
	asrs	r4, r0, #16
	cmp	r3, #0
	beq.n	.L6
	ldrh	r5, [r6, #48]
	subs	r3, r4, r5
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L7
	negs	r3, r2
.L7:
	movs	r7, #50
	ldrsh	r1, [r6, r7]
	ldrh	r0, [r6, #50]
	cmp	r3, r1
	blt.n	.L6
	cmp	r2, #0
	bge.n	.L8
	negs	r3, r2
	cmp	r3, r1
	ble.n	.L9
	negs	r3, r0
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	b.n	.L9
.L8:
	cmp	r2, r1
	ble.n	.L9
	adds	r2, r1, #0
.L9:
	adds	r3, r2, r5
	lsls	r3, r3, #16
	asrs	r4, r3, #16
.L6:
	lsls	r3, r4, #16
	lsrs	r0, r3, #16
	ldr	r2, [r6, #28]
	ldr	r3, [r6, #36]
	adds	r7, r2, r3
	ldr	r3, [r6, #32]
	strh	r0, [r6, #48]
	cmp	r7, r3
	ble.n	.L10
	adds	r7, r3, #0
.L10:
	lsls	r3, r0, #16
	asrs	r4, r3, #16
	adds	r0, r4, #0
	str	r7, [r6, #28]
	str	r4, [sp, #0]
	bl	Func_0800231c
	ldr	r5, [pc, #48]
	adds	r1, r7, #0
	mov	ip, pc
	bx	r5
	ldr	r3, [r6, #4]
	ldr	r4, [sp, #0]
	adds	r3, r3, r0
	adds	r0, r4, #0
	str	r3, [r6, #4]
	bl	Func_08002322
	adds	r1, r7, #0
	mov	ip, pc
	bx	r5
	ldr	r3, [r6, #8]
	adds	r3, r3, r0
	str	r3, [r6, #8]
.L1:
	add	sp, #4
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000ffff
	.4byte 0x030001d8
	.4byte 0x03000118
