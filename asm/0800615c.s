@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800615c
	.thumb_func
Func_0800615c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #8
	movs	r3, #0
	adds	r4, r0, #0
	str	r3, [sp, #4]
	ldr	r2, [pc, #188]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #188]
	adds	r1, r3, #0
	adds	r1, #64
	movs	r7, #3
.L0:
	ldr	r2, [r1, #16]
	ldr	r3, [r1, #0]
	subs	r7, #1
	str	r3, [r1, #16]
	stmia	r1!, {r2}
	cmp	r7, #0
	bge.n	.L0
	ldr	r1, [pc, #168]
	ldr	r3, [r1, #0]
	movs	r0, #0
	mov	lr, sp
	str	r3, [sp, #0]
	str	r0, [r1, #0]
	movs	r2, #1
	ldr	r3, [pc, #148]
	strh	r2, [r3, #0]
	subs	r3, r1, #4
	mov	r9, r3
	subs	r2, #2
	adds	r6, r1, #0
	strb	r0, [r3, #3]
	movs	r7, #0
	mov	sl, r9
	mov	r8, r2
	adds	r6, #76
	mov	ip, r4
.L5:
	ldr	r2, [r6, #0]
	movs	r0, #0
	movs	r1, #0
.L1:
	ldrh	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #2
	adds	r0, r0, r3
	cmp	r1, #13
	bls.n	.L1
	mov	r3, lr
	ldrb	r4, [r3, r7]
	cmp	r4, #1
	bne.n	.L2
	lsls	r5, r0, #16
	asrs	r3, r5, #16
	cmp	r3, r8
	bne.n	.L3
	ldr	r0, [r6, #0]
	ldr	r3, [pc, #100]
	adds	r0, #4
	mov	r1, ip
	ldr	r2, [pc, #96]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r1, sl
	ldrb	r3, [r1, #3]
	lsls	r4, r7
	ldr	r2, [pc, #72]
	orrs	r4, r3
	strb	r4, [r1, #3]
	mov	r9, r2
	b.n	.L4
.L2:
	lsls	r5, r0, #16
.L4:
	asrs	r3, r5, #16
	cmp	r3, r8
	bne.n	.L3
	ldr	r2, [r6, #0]
	ldrh	r3, [r2, #2]
	mvns	r3, r3
	strh	r3, [r2, #2]
	ldr	r3, [pc, #48]
	mov	r9, r3
.L3:
	movs	r1, #24
	adds	r7, #1
	adds	r6, #4
	add	ip, r1
	cmp	r7, #1
	ble.n	.L5
	mov	r2, r9
	ldrb	r3, [r2, #2]
	ldrb	r2, [r2, #3]
	mov	r1, r9
	ldrb	r0, [r1, #3]
	orrs	r3, r2
	strb	r3, [r1, #2]
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x04000208
	.4byte 0x02002240
	.4byte 0x02002244
	.4byte 0x040000d4
	.4byte 0x84000006
