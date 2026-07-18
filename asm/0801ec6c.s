@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801ec6c
	.thumb_func
Func_0801ec6c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r9, r3
	ldr	r3, [pc, #176]
	adds	r5, r0, #0
	movs	r0, #32
	sub	sp, #16
	mov	sl, r1
	adds	r6, r2, #0
	ldr	r7, [r3, #0]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L0
	cmp	r5, #0
	bne.n	.L1
	movs	r5, #18
.L1:
	cmp	r5, #1
	bne.n	.L0
	movs	r5, #19
.L0:
	adds	r0, r5, #0
	bl	Func_08019d2c
	movs	r1, #1
	adds	r5, r0, #0
	negs	r1, r1
	movs	r0, #0
	cmp	r5, r1
	beq.n	.L2
	cmp	r6, #1
	bls.n	.L3
	ldr	r2, [pc, #124]
	adds	r3, r7, r2
	ldrh	r3, [r3, #0]
	ldr	r2, [pc, #124]
	movs	r6, #1
	cmp	r3, r2
	beq.n	.L3
	ldr	r1, [pc, #120]
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	movs	r6, #0
	cmp	r3, r2
	bne.n	.L2
.L3:
	movs	r2, #14
	adds	r2, r2, r6
	mov	r8, r2
	mov	r1, r8
	str	r1, [sp, #0]
	movs	r1, #0
	add	r2, sp, #12
	add	r3, sp, #8
	str	r1, [sp, #4]
	adds	r0, r5, #0
	mov	r1, sl
	bl	Func_0801a4fc
	ldr	r3, [sp, #48]
	movs	r1, #128
	str	r3, [sp, #0]
	ldr	r0, [sp, #12]
	lsls	r1, r1, #24
	mov	r2, r9
	ldr	r3, [sp, #44]
	bl	Func_0801eadc
	cmp	r0, #0
	beq.n	.L4
	mov	r2, r8
	lsls	r1, r2, #4
	ldrb	r2, [r0, #25]
	movs	r3, #15
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #25]
	movs	r3, #2
	strb	r3, [r0, #4]
.L4:
	ldr	r1, [pc, #40]
	lsls	r3, r6, #1
	adds	r2, r3, r1
	strh	r5, [r7, r2]
	ldr	r2, [pc, #36]
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	strh	r2, [r7, r3]
.L2:
	add	sp, #16
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e8c
	.4byte 0x000012ee
	.4byte 0x000003e7
	.4byte 0x000012ec
	.4byte 0x000012f0
