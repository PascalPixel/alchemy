@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080985fc
	.thumb_func
Func_080985fc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #140]
	sub	sp, #12
	ldr	r7, [r3, #0]
	bl	Func_08098698
	movs	r0, #134
	bl	Func_080f9010
	movs	r3, #4
	mov	r6, sp
	mov	r8, r3
.L1:
	ldr	r3, [r7, #4]
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	str	r3, [r6, #8]
	bl	Func_08004458
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r3, #128
	lsls	r3, r3, #11
	lsls	r5, r5, #1
	adds	r5, r5, r3
	bl	Func_08004458
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_0800447c
	ldr	r2, [r7, #8]
	ldr	r1, [r6, #0]
	ldr	r3, [r6, #8]
	movs	r0, #217
	str	r2, [r6, #4]
	bl	Func_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	ldr	r1, [pc, #64]
	bl	Func_08009098
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
.L0:
	bl	Func_08004458
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	adds	r0, #2
	bl	Func_080030f8
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L1
	movs	r0, #30
	bl	Func_080030f8
	bl	Func_0809748c
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30
	.4byte 0x0809f11c
