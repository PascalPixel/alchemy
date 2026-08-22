@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08095938
	.thumb_func
Func_08095938:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #260]
	movs	r1, #250
	adds	r5, r0, #0
	lsls	r1, r1, #1
	adds	r3, r3, r1
	adds	r7, r5, #0
	ldr	r0, [r3, #0]
	adds	r7, #64
	sub	sp, #12
	bl	Func_08092054
	movs	r2, #0
	ldrsb	r2, [r7, r2]
	cmp	r2, #0
	bne.n	.L0
	ldrh	r3, [r5, #60]
	adds	r3, #1
	strh	r3, [r5, #60]
	ldrh	r3, [r5, #62]
	adds	r3, #1
	strh	r3, [r5, #62]
	movs	r1, #56
	ldrsh	r3, [r5, r1]
	cmp	r3, #60
	bne.n	.L1
	strh	r2, [r5, #56]
.L3:
	ldrb	r3, [r7, #0]
	adds	r3, #1
	strb	r3, [r7, #0]
	b.n	.L1
.L0:
	cmp	r2, #1
	bne.n	.L2
	ldrh	r3, [r5, #62]
	adds	r3, #1
	strh	r3, [r5, #62]
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #40
	bne.n	.L1
	movs	r3, #0
	strh	r3, [r5, #56]
	b.n	.L3
.L2:
	cmp	r2, #2
	bne.n	.L4
	ldrh	r3, [r5, #62]
	adds	r3, #1
	strh	r3, [r5, #62]
	ldr	r3, [r0, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	movs	r1, #160
	ldr	r3, [r0, #12]
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r6, #4]
	ldr	r3, [r0, #16]
	adds	r0, r6, #0
	str	r3, [r6, #8]
	bl	Func_080974d8
	ldr	r3, [r6, #0]
	ldr	r2, [r5, #20]
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L5
	adds	r3, #7
.L5:
	asrs	r3, r3, #3
	adds	r3, r2, r3
	str	r3, [r5, #20]
	ldr	r2, [r5, #24]
	ldr	r3, [r6, #8]
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L6
	adds	r3, #7
.L6:
	asrs	r3, r3, #3
	adds	r3, r2, r3
	str	r3, [r5, #24]
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #40
	bne.n	.L7
	movs	r3, #0
	strh	r3, [r5, #56]
	ldrb	r3, [r7, #0]
	adds	r3, #1
	strb	r3, [r7, #0]
	b.n	.L7
.L4:
	cmp	r2, #3
	bne.n	.L8
	ldrh	r3, [r5, #60]
	subs	r3, #1
	strh	r3, [r5, #60]
	ldrh	r3, [r5, #62]
	adds	r3, #1
	strh	r3, [r5, #62]
	movs	r1, #56
	ldrsh	r3, [r5, r1]
	cmp	r3, #60
	bne.n	.L1
	movs	r3, #0
	strh	r3, [r5, #56]
	b.n	.L3
.L8:
	cmp	r2, #4
	bne.n	.L1
	adds	r0, r5, #0
	bl	Func_0809bb34
.L1:
	mov	r6, sp
.L7:
	ldr	r3, [r5, #20]
	str	r3, [r6, #0]
	ldr	r3, [r5, #24]
	str	r3, [r6, #8]
	movs	r2, #60
	ldrsh	r0, [r5, r2]
	movs	r3, #62
	ldrsh	r1, [r5, r3]
	lsls	r0, r0, #16
	lsls	r1, r1, #11
	adds	r2, r6, #0
	bl	Func_0800447c
	ldr	r3, [r6, #0]
	str	r3, [r5, #4]
	ldr	r3, [r6, #8]
	add	sp, #12
	str	r3, [r5, #8]
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
