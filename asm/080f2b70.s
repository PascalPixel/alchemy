@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f2b70
	.thumb_func
Func_080f2b70:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	movs	r0, #110
	bl	Func_080f9010
	ldr	r2, [pc, #88]
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r5, [pc, #84]
	bl	Func_080040e8
	movs	r0, #1
	bl	Func_08003b70
	bl	Func_08004760
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #60]
	movs	r6, #0
	strh	r6, [r3, #10]
	adds	r0, r5, #0
	bl	Func_08002f40
	movs	r1, #160
	ldr	r7, [pc, #48]
	adds	r4, r0, #0
	ldr	r3, [pc, #48]
	lsls	r1, r1, #19
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #224
	lsls	r3, r3, #1
	ldr	r5, [pc, #44]
	adds	r4, r4, r3
	adds	r1, r5, #0
	adds	r0, r4, #0
	b.n	.L0
	.4byte 0x00000681
	.4byte 0x00001440
	.4byte 0x03001d18
	.4byte 0x00000018
	.4byte 0x0400000c
	.4byte 0x03001ad0
	.4byte 0x000001ff
	.4byte 0x040000d4
	.4byte 0x84000070
	.4byte 0x02010000
.L0:
	bl	Func_08005340
	ldr	r3, [pc, #132]
	adds	r0, r5, #0
	ldr	r1, [pc, #132]
	ldr	r2, [pc, #136]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #128
	ldr	r1, [pc, #132]
	lsls	r3, r3, #1
	movs	r5, #0
.L2:
	movs	r0, #0
.L1:
	adds	r2, r3, #0
	movs	r4, #128
	lsls	r3, r2, #16
	lsls	r4, r4, #9
	adds	r3, r3, r4
	adds	r0, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r0, #29
	bls.n	.L1
	strh	r7, [r1, #0]
	adds	r5, #1
	adds	r1, #2
	strh	r7, [r1, #0]
	adds	r1, #2
	cmp	r5, #19
	bls.n	.L2
	ldr	r3, [pc, #92]
	movs	r5, #0
	movs	r2, #0
.L3:
	adds	r5, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r5, #3
	bls.n	.L3
	ldr	r3, [pc, #56]
	ldr	r0, [pc, #72]
	ldr	r1, [pc, #72]
	ldr	r2, [pc, #76]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	Func_0800479c
	bl	Func_08004760
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L4
	movs	r0, #1
	bl	Func_08003bb4
	bl	Func_08003ce0
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	movs	r2, #9
	ands	r3, r2
	movs	r5, #0
	b.n	.L5
	.4byte 0x00001540
	.4byte 0x040000d4
	.4byte 0x06004000
	.4byte 0x84002580
	.4byte 0x06003000
	.4byte 0x03001ad0
	.4byte 0x04000010
	.4byte 0x84000004
	.4byte 0x03001c94
.L7:
	movs	r0, #1
	adds	r5, #1
	bl	Func_080030f8
	cmp	r5, #119
	bhi.n	.L6
	ldr	r3, [pc, #156]
	ldr	r3, [r3, #0]
	movs	r2, #9
	ands	r3, r2
.L5:
	cmp	r3, #0
	beq.n	.L7
	movs	r6, #1
	negs	r6, r6
	b.n	.L6
.L4:
	ldr	r3, [pc, #140]
	ldr	r3, [r3, #0]
	movs	r2, #9
	ands	r3, r2
	movs	r5, #0
	b.n	.L8
.L10:
	movs	r0, #1
	adds	r5, #1
	bl	Func_080030f8
	cmp	r5, #59
	bhi.n	.L9
	ldr	r3, [pc, #116]
	ldr	r3, [r3, #0]
	movs	r2, #9
	ands	r3, r2
.L8:
	cmp	r3, #0
	beq.n	.L10
	movs	r6, #1
	negs	r6, r6
.L9:
	cmp	r6, #0
	beq.n	.L11
	movs	r0, #8
	bl	Func_08003bb4
	b.n	.L12
.L11:
	movs	r0, #60
	bl	Func_08003bb4
.L12:
	bl	Func_08003ce0
	cmp	r6, #0
	bne.n	.L13
	ldr	r3, [pc, #72]
	ldr	r3, [r3, #0]
	movs	r2, #9
	ands	r3, r2
	movs	r5, #0
	b.n	.L14
.L16:
	movs	r0, #1
	adds	r5, #1
	bl	Func_080030f8
	cmp	r5, #179
	bhi.n	.L15
	ldr	r3, [pc, #48]
	ldr	r3, [r3, #0]
	movs	r2, #9
	ands	r3, r2
.L14:
	cmp	r3, #0
	beq.n	.L16
	movs	r6, #1
	negs	r6, r6
.L15:
	cmp	r6, #0
	beq.n	.L17
.L13:
	movs	r0, #8
	bl	Func_08003b70
	b.n	.L18
.L17:
	movs	r0, #60
	bl	Func_08003b70
.L18:
	bl	Func_08003ce0
.L6:
	adds	r0, r6, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001c94
