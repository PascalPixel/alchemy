@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0807905c
	.thumb_func
Func_0807905c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	mov	fp, r0
	bl	Func_08077394
	mov	sl, r0
	movs	r0, #44
	bl	Func_08004970
	ldr	r3, [pc, #576]
	add	r3, sl
	ldrb	r3, [r3, #0]
	mov	r9, r0
	str	r3, [r0, #0]
	mov	r0, sl
	ldrb	r1, [r0, #15]
	mov	r2, r9
	ldr	r3, [pc, #564]
	str	r1, [r2, #4]
	movs	r2, #0
	strh	r1, [r6, #0]
	strh	r3, [r6, #2]
	strh	r2, [r6, #4]
	strh	r2, [r6, #6]
	strh	r2, [r6, #8]
	strh	r2, [r6, #10]
	strh	r2, [r6, #12]
	strh	r2, [r6, #14]
	cmp	r1, #98
	ble.n	.L0
	b.n	.L1
.L0:
	ldrb	r3, [r0, #15]
	adds	r3, #1
	strb	r3, [r0, #15]
	adds	r3, r1, #1
	strh	r3, [r6, #0]
	ldrb	r1, [r0, #15]
	mov	r0, fp
	bl	Func_08079008
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L2
	movs	r2, #146
	lsls	r2, r2, #1
	add	r2, sl
	ldr	r3, [r2, #0]
	cmp	r3, r0
	bcs.n	.L2
	str	r0, [r2, #0]
.L2:
	mov	r0, fp
	bl	Func_08078ed8
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	adds	r1, r0, #0
	mov	r0, r9
	str	r1, [r0, #8]
	ldrh	r0, [r6, #0]
	cmp	r3, #1
	bne.n	.L3
	adds	r2, r1, #0
	adds	r2, #80
	ldrh	r3, [r6, #4]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	adds	r2, r1, #0
	adds	r2, #92
	ldrh	r2, [r2, #0]
	strh	r3, [r6, #4]
	ldrh	r3, [r6, #6]
	adds	r3, r3, r2
	adds	r2, r1, #0
	adds	r2, #104
	ldrh	r2, [r2, #0]
	strh	r3, [r6, #6]
	ldrh	r3, [r6, #8]
	adds	r3, r3, r2
	adds	r2, r1, #0
	adds	r2, #116
	ldrh	r2, [r2, #0]
	strh	r3, [r6, #8]
	ldrh	r3, [r6, #10]
	adds	r3, r3, r2
	adds	r2, r1, #0
	adds	r2, #128
	ldrh	r2, [r2, #0]
	strh	r3, [r6, #10]
	ldrh	r3, [r6, #12]
	adds	r3, r3, r2
	strh	r3, [r6, #12]
	adds	r3, r1, #0
	adds	r3, #140
	ldrb	r2, [r3, #0]
	ldrh	r3, [r6, #14]
	adds	r3, r3, r2
	strh	r3, [r6, #14]
.L3:
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r1, #20
	bl	Func_080022ec
	lsls	r0, r0, #16
	asrs	r5, r0, #16
	cmp	r5, #0
	bge.n	.L4
	movs	r5, #0
.L4:
	cmp	r5, #4
	ble.n	.L5
	movs	r5, #4
.L5:
	lsls	r0, r5, #1
	mov	r8, r0
	mov	r3, r9
	ldr	r1, [r3, #8]
	mov	r3, r8
	adds	r3, #82
	ldrsh	r2, [r1, r3]
	subs	r3, #2
	ldrsh	r3, [r1, r3]
	subs	r7, r2, r3
	bl	Func_08004458
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r0, r0, r7
	movs	r1, #20
	bl	Func_080022f4
	ldrh	r3, [r6, #4]
	adds	r3, r3, r0
	strh	r3, [r6, #4]
	mov	r3, r9
	ldr	r2, [r3, #8]
	mov	r3, r8
	adds	r3, #94
	ldrsh	r1, [r2, r3]
	subs	r3, #2
	ldrsh	r3, [r2, r3]
	subs	r7, r1, r3
	bl	Func_08004458
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r0, r0, r7
	movs	r1, #20
	bl	Func_080022f4
	ldrh	r3, [r6, #6]
	adds	r3, r3, r0
	strh	r3, [r6, #6]
	mov	r3, r9
	ldr	r2, [r3, #8]
	mov	r3, r8
	adds	r3, #106
	ldrh	r1, [r2, r3]
	subs	r3, #2
	ldrh	r3, [r2, r3]
	subs	r7, r1, r3
	bl	Func_08004458
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r0, r0, r7
	movs	r1, #20
	bl	Func_080022f4
	ldrh	r3, [r6, #8]
	adds	r3, r3, r0
	mov	r0, r9
	ldr	r2, [r0, #8]
	strh	r3, [r6, #8]
	mov	r3, r8
	adds	r3, #118
	ldrh	r1, [r2, r3]
	subs	r3, #2
	ldrh	r3, [r2, r3]
	subs	r7, r1, r3
	bl	Func_08004458
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r0, r0, r7
	movs	r1, #20
	bl	Func_080022f4
	ldrh	r3, [r6, #10]
	adds	r3, r3, r0
	strh	r3, [r6, #10]
	mov	r3, r9
	ldr	r2, [r3, #8]
	mov	r3, r8
	adds	r3, #130
	ldrh	r1, [r2, r3]
	subs	r3, #2
	ldrh	r3, [r2, r3]
	subs	r7, r1, r3
	bl	Func_08004458
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r0, r0, r7
	movs	r1, #20
	bl	Func_080022f4
	ldrh	r3, [r6, #12]
	adds	r3, r3, r0
	mov	r0, r9
	ldr	r2, [r0, #8]
	strh	r3, [r6, #12]
	adds	r3, r5, #0
	adds	r3, #141
	ldrb	r1, [r2, r3]
	subs	r3, #1
	ldrb	r3, [r2, r3]
	subs	r7, r1, r3
	bl	Func_08004458
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	movs	r1, #20
	adds	r0, r0, r7
	bl	Func_080022f4
	mov	r2, sl
	ldrh	r3, [r2, #16]
	ldrh	r1, [r6, #14]
	ldrh	r2, [r6, #4]
	adds	r1, r1, r0
	adds	r3, r3, r2
	mov	r0, sl
	strh	r3, [r0, #16]
	ldrh	r2, [r6, #6]
	ldrh	r3, [r0, #18]
	adds	r3, r3, r2
	mov	r2, sl
	strh	r3, [r2, #18]
	ldrh	r3, [r2, #24]
	ldrh	r2, [r6, #8]
	adds	r3, r3, r2
	strh	r3, [r0, #24]
	ldrh	r2, [r6, #10]
	ldrh	r3, [r0, #26]
	adds	r3, r3, r2
	mov	r2, sl
	strh	r3, [r2, #26]
	ldrh	r3, [r2, #28]
	ldrh	r2, [r6, #12]
	adds	r3, r3, r2
	strh	r3, [r0, #28]
	ldrb	r3, [r0, #30]
	adds	r3, r3, r1
	strb	r3, [r0, #30]
	movs	r3, #1
	strb	r3, [r0, #31]
	mov	r3, sl
	movs	r2, #0
	adds	r3, #32
	strb	r2, [r3, #0]
	adds	r3, #1
	strh	r1, [r6, #14]
	strb	r2, [r3, #0]
	mov	r0, fp
	bl	Func_08078bf0
	mov	r0, fp
	bl	Func_08077428
.L1:
	mov	r0, r9
	bl	Func_08002df0
	adds	r0, r6, #0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000129
	.4byte 0x0000ffff
