@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08098070
	.thumb_func
Func_08098070:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldrh	r3, [r0, #6]
	mov	r8, r0
	movs	r0, #128
	lsls	r0, r0, #6
	mov	r2, r8
	adds	r5, r3, r0
	ldr	r1, [r2, #8]
	movs	r3, #192
	ldr	r2, [r2, #12]
	movs	r6, #128
	lsls	r3, r3, #8
	mov	r0, r8
	lsls	r6, r6, #13
	ands	r5, r3
	adds	r2, r2, r6
	ldr	r3, [r0, #16]
	movs	r0, #215
	bl	Func_08096c80
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	movs	r3, #128
	mov	r2, sl
	lsls	r3, r3, #7
	str	r3, [r2, #28]
	str	r3, [r2, #24]
	ldr	r3, [pc, #188]
	str	r3, [r2, #108]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #48]
	str	r3, [r2, #52]
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	mov	r0, sl
	movs	r1, #3
	bl	Func_08009080
	mov	r0, sl
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	Func_08096bec
	movs	r3, #7
	mov	r9, r3
.L3:
	mov	r0, r8
	ldr	r2, [r0, #12]
	movs	r3, #128
	lsls	r3, r3, #13
	ldr	r1, [r0, #8]
	adds	r2, r2, r3
	ldr	r3, [r0, #16]
	ldr	r0, [pc, #140]
	bl	Func_08096c80
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L2
	ldr	r1, [pc, #132]
	bl	Func_08009098
	bl	Func_08004458
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r2, r7, #0
	adds	r2, #85
	adds	r0, r0, r3
	str	r3, [r7, #52]
	movs	r3, #2
	str	r0, [r7, #48]
	strb	r3, [r2, #0]
	ldr	r3, [pc, #108]
	str	r3, [r7, #72]
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	subs	r5, r5, r0
	str	r5, [r7, #40]
	bl	Func_08004458
	lsls	r6, r0, #1
	adds	r6, r6, r0
	movs	r0, #128
	lsls	r0, r0, #12
	lsls	r6, r6, #3
	adds	r6, r6, r0
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	mov	r2, r8
	subs	r5, r5, r0
	ldrh	r3, [r2, #6]
	lsrs	r5, r5, #3
	adds	r5, r5, r3
	adds	r0, r7, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	Func_08096bec
.L2:
	movs	r3, #1
	negs	r3, r3
	add	r9, r3
	mov	r0, r9
	cmp	r0, #0
	bge.n	.L3
	movs	r0, #138
	bl	Func_080f9010
	mov	r0, sl
.L1:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x08097b71
	.4byte 0x0000011d
	.4byte 0x0809f0d4
	.4byte 0x0000051e
