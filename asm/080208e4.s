@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080208e4
	.thumb_func
Func_080208e4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #0
	adds	r5, r0, #0
	mov	r8, r1
	bl	Func_080056cc
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L0
	ldr	r0, [pc, #140]
	movs	r1, #1
	bl	Func_0801776c
	movs	r2, #9
	negs	r2, r2
	mov	r8, r2
	b.n	.L1
.L0:
	bl	Func_08005c68
	ldr	r3, [pc, #124]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	adds	r1, r5, #0
	mov	sl, r3
	bl	Func_08020244
	movs	r2, #1
	adds	r7, r0, #0
	negs	r2, r2
	cmp	r7, r2
	bne.n	.L2
	mov	r8, r7
	b.n	.L1
.L2:
	ldr	r5, [pc, #100]
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	Func_08005a78
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r5, r5, r3
	adds	r6, r0, #0
	adds	r1, r5, #0
	adds	r0, r7, #3
	bl	Func_08005a78
	orrs	r6, r0
	cmp	r6, #0
	beq.n	.L3
	movs	r1, #1
	ldr	r0, [pc, #72]
	bl	Func_0801776c
	movs	r1, #2
	negs	r1, r1
	mov	r8, r1
	b.n	.L1
.L3:
	ldr	r3, [pc, #60]
	ldr	r1, [pc, #64]
	ldr	r2, [r3, #4]
	str	r2, [r1, #0]
	ldr	r1, [pc, #60]
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	ldr	r2, [pc, #60]
	strb	r3, [r2, #0]
	ldr	r3, [pc, #60]
	mov	r2, r8
	strh	r2, [r3, #0]
	mov	r3, sl
	strh	r7, [r3, #0]
.L1:
	bl	Func_08005cf8
	mov	r0, r8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000000a
	.4byte 0x02002004
	.4byte 0x02000000
	.4byte 0x0000000c
	.4byte 0x02000240
	.4byte 0x03001c9c
	.4byte 0x0000022a
	.4byte 0x03001d08
	.4byte 0x03001d24
