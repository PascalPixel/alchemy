@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f0678
	.thumb_func
Func_080f0678:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #128
	lsls	r0, r0, #3
	sub	sp, #4
	bl	Func_08004970
	ldr	r5, [pc, #252]
	mov	r4, sp
	movs	r3, #0
	str	r0, [r5, #0]
	str	r3, [r4, #0]
	adds	r0, r4, #0
	ldr	r3, [pc, #244]
	ldr	r1, [pc, #244]
	ldr	r2, [pc, #248]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #244]
	adds	r0, r4, #0
	str	r3, [r4, #0]
	ldr	r1, [pc, #244]
	ldr	r3, [pc, #224]
	ldr	r2, [pc, #244]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [r5, #0]
	movs	r1, #192
	ldr	r0, [pc, #236]
	movs	r6, #0
	lsls	r1, r1, #2
.L0:
	lsls	r2, r6, #21
	adds	r3, r5, #0
	orrs	r2, r0
	stmia	r3!, {r2}
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L0
	movs	r1, #192
	ldr	r0, [pc, #216]
	movs	r6, #0
	lsls	r1, r1, #2
.L1:
	lsls	r2, r6, #21
	adds	r3, r5, #0
	orrs	r2, r0
	stmia	r3!, {r2}
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L1
	movs	r1, #192
	ldr	r0, [pc, #192]
	movs	r6, #0
	lsls	r1, r1, #2
.L2:
	lsls	r2, r6, #21
	adds	r3, r5, #0
	orrs	r2, r0
	stmia	r3!, {r2}
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L2
	movs	r2, #16
	mov	lr, r2
	ldr	r3, [pc, #168]
	movs	r2, #128
	lsls	r2, r2, #14
	movs	r6, #0
	movs	r7, #0
	mov	sl, r3
	mov	r8, r2
.L4:
	movs	r0, #192
	adds	r3, r7, r6
	movs	r4, #0
	mov	ip, lr
	lsls	r0, r0, #13
	lsls	r1, r3, #3
.L3:
	mov	r3, ip
	orrs	r3, r0
	mov	r2, sl
	orrs	r3, r2
	adds	r2, r5, #0
	stmia	r2!, {r3}
	adds	r4, #1
	str	r1, [r2, #0]
	adds	r5, #8
	add	r0, r8
	adds	r1, #4
	cmp	r4, #5
	bls.n	.L3
	movs	r3, #8
	adds	r6, #1
	add	lr, r3
	adds	r7, #2
	cmp	r6, #15
	bls.n	.L4
	movs	r2, #192
	ldr	r1, [pc, #108]
	movs	r6, #0
	lsls	r2, r2, #2
.L5:
	adds	r3, r5, #0
	stmia	r3!, {r1}
	adds	r6, #1
	str	r2, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L5
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #88]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #88]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #88]
	movs	r1, #144
	strh	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #84]
	bl	Func_080041d8
	movs	r1, #200
	ldr	r0, [pc, #80]
	lsls	r1, r1, #4
	bl	Func_080041d8
	ldr	r7, [pc, #76]
	movs	r6, #0
	movs	r5, #0
	b.n	.L6
	.4byte 0x00000000
	.4byte 0x02004c0c
	.4byte 0x040000d4
	.4byte 0x06010000
	.4byte 0x85001800
	.4byte 0x11111111
	.4byte 0x06016000
	.4byte 0x85000040
	.4byte 0x80004000
	.4byte 0x80004088
	.4byte 0x40004098
	.4byte 0x40004000
	.4byte 0x00c000c0
	.4byte 0x02004c00
	.4byte 0x02004c08
	.4byte 0x02004c04
	.4byte 0x080f0539
	.4byte 0x080f0615
	.4byte 0x080f1220
.L6:
	adds	r1, r5, #0
	ldr	r0, [r7, #0]
	.4byte 0x36012201
	bl	Func_080f07f0
	adds	r5, #24
	cmp	r6, #31
	.4byte 0xb001d9f6
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
