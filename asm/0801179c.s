@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801179c
	.thumb_func
Func_0801179c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #228]
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #228]
	mov	r8, r3
	ldr	r3, [pc, #228]
	movs	r1, #0
	movs	r6, #132
	mov	r5, r8
	ldr	r7, [pc, #224]
	mov	sl, r1
	lsls	r6, r6, #24
	mov	ip, r2
	adds	r5, #24
	mov	lr, r3
.L10:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L0
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	bne.n	.L0
.L3:
	ldrh	r2, [r5, #8]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L1
	ldr	r4, [r5, #4]
	ldrh	r0, [r4, #0]
	adds	r4, #2
	cmp	r0, lr
	bne.n	.L2
	ldr	r3, [r5, #0]
	str	r3, [r5, #4]
	b.n	.L3
.L2:
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r1, #254
	ands	r3, r0
	lsls	r1, r1, #8
	cmp	r3, r1
	bne.n	.L4
	movs	r2, #255
	ands	r2, r0
	cmp	r2, #255
	beq.n	.L0
	ldr	r3, [r5, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r5, #4]
	b.n	.L3
.L4:
	ldrh	r2, [r4, #0]
	adds	r4, #2
	ldrh	r3, [r4, #2]
	ldrh	r1, [r4, #0]
	strh	r3, [r5, #8]
	mov	r4, r8
	ldrb	r3, [r4, #22]
	cmp	r3, #0
	bne.n	.L5
	movs	r3, #192
	lsls	r3, r3, #3
	cmp	r0, r3
	bcc.n	.L6
	ldr	r4, [pc, #124]
	lsls	r2, r2, #3
	lsls	r0, r0, #5
	lsls	r1, r1, #5
	ldr	r3, [pc, #120]
	adds	r0, r0, r4
	add	r1, ip
	b.n	.L7
.L6:
	lsls	r2, r2, #3
	lsls	r0, r0, #5
	lsls	r1, r1, #5
	ldr	r3, [pc, #104]
	add	r0, ip
	add	r1, ip
	b.n	.L7
.L5:
	movs	r3, #128
	lsls	r3, r3, #2
	cmp	r0, r3
	bcc.n	.L8
	ldr	r4, [pc, #92]
	lsls	r2, r2, #4
	lsls	r0, r0, #6
	lsls	r1, r1, #6
	ldr	r3, [pc, #80]
	adds	r0, r0, r4
	b.n	.L9
.L8:
	lsls	r2, r2, #4
	lsls	r0, r0, #6
	lsls	r1, r1, #6
	ldr	r3, [pc, #68]
	adds	r0, r0, r7
.L9:
	adds	r1, r1, r7
.L7:
	orrs	r2, r6
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r5, #4]
	adds	r3, #8
	str	r3, [r5, #4]
	b.n	.L3
.L1:
	mov	r1, lr
	adds	r3, r2, r1
	strh	r3, [r5, #8]
.L0:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r5, #12
	cmp	r3, #15
	bls.n	.L10
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0x06004000
	.4byte 0x0000ffff
	.4byte 0x06008000
	.4byte 0x0201c000
	.4byte 0x040000d4
	.4byte 0x02020000
