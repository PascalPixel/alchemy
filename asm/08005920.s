@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08005920
	.thumb_func
Func_08005920:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #300]
	ldr	r3, [r3, #0]
	sub	sp, #20
	mov	sl, r3
	adds	r5, r1, #0
	movs	r1, #64
	mov	r8, r0
	movs	r3, #0
	mov	r0, sp
	add	r1, sl
	str	r3, [r0, #0]
	mov	r9, r1
	ldr	r3, [pc, #280]
	ldr	r2, [pc, #280]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	ldr	r1, [pc, #268]
	lsls	r2, r2, #24
.L0:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L0
	mov	r0, r8
	bl	Func_08005b24
	adds	r7, r0, #0
	mov	r0, r8
	bl	Func_08005810
	adds	r6, r0, #0
	movs	r0, #1
	cmp	r6, #15
	bhi.n	.L1
	mov	r1, sl
	ldr	r3, [pc, #232]
	adds	r0, r5, #0
	adds	r1, #80
	ldr	r2, [pc, #236]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	ldr	r1, [pc, #220]
	lsls	r2, r2, #24
.L2:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L2
	ldr	r3, [pc, #208]
	ldr	r0, [pc, #216]
	add	r1, sp, #4
	ldr	r2, [pc, #216]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	ldr	r1, [pc, #192]
	lsls	r2, r2, #24
.L3:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L3
	add	r5, sp, #4
	mov	r2, r8
	strb	r2, [r5, #7]
	bl	Func_08005ae0
	strh	r0, [r5, #8]
	mov	r0, r8
	bl	Func_08005c2c
	adds	r0, #1
	strh	r0, [r5, #10]
	ldr	r3, [pc, #160]
	adds	r0, r5, #0
	mov	r1, r9
	ldr	r2, [pc, #172]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	ldr	r1, [pc, #144]
	lsls	r2, r2, #24
.L4:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L4
	adds	r0, r6, #0
	bl	Func_08005868
	cmp	r0, #0
	bne.n	.L5
	cmp	r7, #15
	bhi.n	.L6
	adds	r0, r7, #0
	bl	Func_08005b64
	cmp	r0, #0
	bne.n	.L5
.L6:
	ldrh	r3, [r5, #10]
	ldr	r1, [pc, #132]
	cmp	r3, r1
	bls.n	.L7
	movs	r3, #1
	strh	r3, [r5, #10]
	adds	r0, r5, #0
	ldr	r3, [pc, #96]
	mov	r1, r9
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	ldr	r1, [pc, #84]
	lsls	r2, r2, #24
.L8:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L8
	adds	r0, r7, #0
	bl	Func_08005868
	cmp	r0, #0
	bne.n	.L5
	adds	r0, r6, #0
	bl	Func_08005b64
	cmp	r0, #0
	beq.n	.L9
.L5:
	movs	r0, #1
	b.n	.L1
.L9:
	adds	r6, r7, #0
.L7:
	mov	r2, sl
	movs	r3, #1
	strb	r3, [r2, r6]
	adds	r3, r6, #0
	adds	r3, #16
	mov	r1, r8
	strb	r1, [r2, r3]
	lsls	r3, r6, #1
	ldrh	r2, [r5, #10]
	adds	r3, #32
	mov	r1, sl
	strh	r2, [r1, r3]
	movs	r0, #0
.L1:
	add	sp, #20
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f1c
	.4byte 0x040000d4
	.4byte 0x85000400
	.4byte 0x840003fc
	.4byte 0x080079b8
	.4byte 0x84000002
	.4byte 0x84000004
	.4byte 0x0000fde8
