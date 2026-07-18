@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801dd28
	.thumb_func
Func_0801dd28:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #132
	str	r1, [sp, #0]
	adds	r6, r3, #0
	ldr	r3, [pc, #252]
	ldr	r3, [r3, #0]
	mov	fp, r0
	ldr	r0, [pc, #252]
	mov	r9, r2
	mov	r8, r3
	bl	Func_08002f40
	mov	sl, r0
	mov	r0, fp
	ldrb	r5, [r0, #0]
	add	r1, sp, #4
	movs	r3, #192
	mov	ip, r1
	lsls	r7, r5, #5
	lsls	r3, r3, #19
	mov	lr, ip
	adds	r2, r7, r3
	movs	r1, #0
.L0:
	ldrb	r4, [r2, #0]
	movs	r0, #15
	adds	r3, r4, #0
	ands	r3, r0
	mov	r0, ip
	strb	r3, [r0, #0]
	lsrs	r3, r4, #4
	strb	r3, [r0, #1]
	adds	r1, #1
	movs	r3, #2
	adds	r2, #1
	add	ip, r3
	cmp	r1, #31
	bls.n	.L0
	mov	r4, r9
	lsls	r3, r4, #5
	mov	r0, sl
	adds	r2, r0, r3
	movs	r3, #15
	mov	ip, lr
	movs	r1, #0
	mov	sl, r3
.L3:
	ldrb	r0, [r2, #0]
	mov	r4, sl
	adds	r3, r0, #0
	ands	r3, r4
	ldrb	r3, [r6, r3]
	adds	r2, #1
	cmp	r3, #0
	beq.n	.L1
	mov	r4, ip
	strb	r3, [r4, #0]
.L1:
	movs	r3, #1
	add	ip, r3
	lsrs	r3, r0, #4
	ldrb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L2
	mov	r4, ip
	strb	r3, [r4, #0]
.L2:
	movs	r0, #1
	adds	r1, #1
	add	ip, r0
	cmp	r1, #31
	bls.n	.L3
	mov	ip, lr
	movs	r1, #0
	mov	r0, ip
.L4:
	ldrb	r3, [r0, #1]
	ldrb	r2, [r0, #0]
	lsls	r3, r3, #4
	orrs	r2, r3
	movs	r4, #1
	mov	r3, ip
	adds	r1, #1
	adds	r0, #2
	strb	r2, [r3, #0]
	add	ip, r4
	cmp	r1, #31
	bls.n	.L4
	lsls	r3, r5, #24
	cmp	r3, #0
	blt.n	.L5
	movs	r1, #234
	lsls	r1, r1, #4
	movs	r0, #218
	movs	r4, #0
	add	r1, r8
	movs	r6, #127
	lsls	r0, r0, #4
.L7:
	ldrh	r3, [r1, #0]
	adds	r2, r3, #1
	ands	r2, r6
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	strh	r2, [r1, #0]
	mov	r7, r8
	adds	r2, r5, r0
	ldrb	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L6
	adds	r4, #1
	cmp	r4, #127
	bls.n	.L7
.L6:
	movs	r3, #1
	mov	r0, r8
	strb	r3, [r0, r2]
	movs	r3, #128
	orrs	r5, r3
	ldr	r2, [pc, #32]
	adds	r3, r5, #0
	orrs	r3, r2
	mov	r1, fp
	strh	r3, [r1, #0]
	ldr	r2, [sp, #0]
	strh	r3, [r2, #0]
	lsls	r7, r5, #5
.L5:
	movs	r4, #192
	lsls	r4, r4, #19
	ldr	r3, [pc, #24]
	add	r0, sp, #4
	adds	r1, r7, r4
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #132
	b.n	.L8
	.4byte 0x0000f000
	.4byte 0x03001e8c
	.4byte 0x00000013
	.4byte 0x040000d4
	.4byte 0x84000008
.L8:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
