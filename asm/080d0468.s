@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d0468
	.thumb_func
Func_080d0468:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r0, [pc, #140]
	movs	r5, #0
.L0:
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	lsls	r2, r5, #10
	lsls	r1, r3, #5
	orrs	r2, r1
	orrs	r2, r3
	adds	r5, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r5, #32
	bne.n	.L0
	ldr	r3, [pc, #116]
	movs	r2, #16
	ldr	r3, [r3, #0]
	movs	r1, #0
	mov	sl, r2
	movs	r2, #240
	mov	r9, r3
	mov	r8, r1
	ldr	r3, [pc, #104]
	ldr	r0, [pc, #104]
	ldr	r1, [pc, #108]
	lsls	r2, r2, #7
	bl	Func_080072f0
	movs	r6, #128
	lsls	r6, r6, #1
	movs	r7, #63
	mov	r5, r9
	add	r6, r9
.L1:
	bl	Func_08004458
	ands	r0, r7
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r6
	bne.n	.L1
	movs	r6, #0
	movs	r5, #0
.L12:
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L2
	adds	r3, #3
.L2:
	asrs	r3, r3, #2
	add	r8, r3
	movs	r3, #1
	add	sl, r3
	cmp	r6, r8
	beq.n	.L3
	ldr	r7, [pc, #32]
.L10:
	movs	r0, #0
.L9:
	mov	r1, r9
	ldrb	r3, [r1, r0]
	subs	r1, r6, r3
	cmp	r1, #0
	blt.n	.L4
	cmp	r1, #119
	bgt.n	.L4
	movs	r4, #7
	adds	r3, r0, #0
	ands	r3, r4
	adds	r2, r0, #0
	cmp	r0, #0
	bge.n	.L5
	adds	r2, r0, #7
	b.n	.L5
	.4byte 0x0000001f
	.4byte 0x05000040
	.4byte 0x03001ef4
	.4byte 0x03001388
	.4byte 0x02010000
	.4byte 0x06008000
.L5:
	asrs	r2, r2, #3
	lsls	r2, r2, #6
	adds	r2, r3, r2
	adds	r3, r1, #0
	ands	r3, r4
	lsls	r3, r3, #3
	adds	r2, r2, r3
	adds	r3, r1, #0
	cmp	r3, #0
	bge.n	.L6
	adds	r3, #7
.L6:
	asrs	r3, r3, #3
	lsls	r3, r3, #11
	adds	r3, r2, r3
	ldr	r2, [pc, #184]
	adds	r4, r3, r2
	ldrb	r3, [r4, #0]
	movs	r1, #160
	lsls	r1, r1, #19
	lsls	r3, r3, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	movs	r3, #248
	lsls	r2, r2, #16
	lsls	r3, r3, #13
	ands	r3, r2
	lsrs	r1, r3, #16
	lsrs	r3, r2, #21
	ands	r3, r7
	lsrs	r2, r2, #26
	ands	r2, r7
	cmp	r1, r3
	bge.n	.L7
	adds	r1, r3, #0
.L7:
	cmp	r1, r2
	bge.n	.L8
	adds	r1, r2, #0
.L8:
	movs	r3, #63
	subs	r3, r3, r1
	strb	r3, [r4, #0]
.L4:
	movs	r2, #128
	adds	r0, #1
	lsls	r2, r2, #1
	cmp	r0, r2
	bne.n	.L9
	adds	r6, #1
	cmp	r6, r8
	bne.n	.L10
.L3:
	movs	r2, #240
	ldr	r3, [pc, #116]
	ldr	r1, [pc, #112]
	lsls	r2, r2, #7
	ldr	r0, [pc, #116]
	bl	Func_080072f0
	movs	r0, #1
	bl	Func_080030f8
	mov	r3, r8
	cmp	r3, #248
	bgt.n	.L11
	adds	r5, #1
	cmp	r5, #27
	bne.n	.L12
.L11:
	ldr	r0, [pc, #96]
	movs	r5, #0
.L13:
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	lsls	r2, r5, #10
	lsls	r1, r3, #5
	orrs	r2, r1
	orrs	r2, r3
	adds	r5, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r5, #32
	bne.n	.L13
	movs	r1, #240
	ldr	r2, [pc, #52]
	movs	r5, #0
	lsls	r1, r1, #7
.L14:
	ldrb	r3, [r2, #0]
	adds	r5, #1
	adds	r3, #64
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r5, r1
	bne.n	.L14
	movs	r2, #240
	ldr	r3, [pc, #36]
	ldr	r1, [pc, #28]
	lsls	r2, r2, #7
	ldr	r0, [pc, #32]
	bl	Func_080072f0
	movs	r0, #1
	bl	Func_080030f8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x03001388
	.4byte 0x06008000
	.4byte 0x050000c0
