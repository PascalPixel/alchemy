@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b8c1c
	.thumb_func
Func_080b8c1c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #388]
	adds	r5, r0, #0
	ldr	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	sub	sp, #92
	ldr	r1, [pc, #380]
	cmp	r3, #4
	bgt.n	.L0
	movs	r1, #128
	lsls	r1, r1, #6
.L0:
	ldr	r3, [r2, #0]
	cmp	r3, r1
	bne.n	.L1
	movs	r3, #40
	str	r3, [r2, #4]
	movs	r0, #40
	bl	Func_080030f8
	b.n	.L2
.L1:
	movs	r3, #40
	str	r1, [r2, #0]
	str	r3, [r2, #4]
	movs	r0, #40
	bl	Func_080030f8
.L2:
	movs	r2, #8
	ldrsh	r3, [r5, r2]
	add	r6, sp, #8
	str	r3, [r6, #0]
	movs	r1, #12
	ldrsh	r3, [r5, r1]
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	str	r3, [r6, #16]
	movs	r1, #10
	ldrsh	r3, [r5, r1]
	str	r0, [r6, #8]
	str	r3, [r6, #12]
	bl	Func_080b8808
	cmp	r0, #0
	bge.n	.L3
	movs	r0, #1
	negs	r0, r0
	b.n	.L4
.L3:
	ldr	r3, [r6, #12]
	cmp	r3, #127
	ble.n	.L5
	add	r7, sp, #44
	movs	r0, #2
	b.n	.L6
.L5:
	add	r7, sp, #44
	movs	r0, #1
.L6:
	adds	r1, r7, #0
	bl	Func_080b6b40
	str	r0, [r6, #20]
	ldr	r3, [pc, #280]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	ands	r0, r3
	bl	Func_08015130
	ldr	r0, [r6, #8]
	bl	Func_080b7dd0
	ldr	r0, [r0, #0]
	movs	r1, #3
	mov	sl, r0
	bl	Func_08009080
	mov	r0, sl
	movs	r1, #16
	bl	Func_08009088
	ldrh	r3, [r5, #10]
	cmp	r3, #7
	bhi.n	.L7
	movs	r2, #1
	mov	r8, r2
	str	r2, [r6, #4]
	movs	r0, #1
	adds	r1, r7, #0
	bl	Func_080b6b40
	mov	r3, r8
	b.n	.L8
.L7:
	movs	r3, #0
	str	r3, [r6, #4]
	movs	r0, #2
	adds	r1, r7, #0
	bl	Func_080b6b40
	movs	r3, #1
.L8:
	str	r3, [r6, #20]
	ldr	r3, [r6, #20]
	movs	r7, #0
	adds	r2, r6, #0
	cmp	r3, #0
	beq.n	.L9
	movs	r5, #0
.L12:
	lsls	r3, r7, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	Func_080b7dd0
	ldr	r3, [r0, #0]
	ldr	r1, [r3, #80]
	adds	r3, r1, #0
	adds	r3, #39
	ldrb	r3, [r3, #0]
	subs	r3, #1
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L10
	mov	ip, r3
	adds	r3, r5, r6
	adds	r2, r3, #0
	adds	r2, #52
	adds	r1, #40
.L11:
	ldmia	r1!, {r3}
	ldrb	r3, [r3, #5]
	adds	r0, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r0, ip
	bne.n	.L11
.L10:
	ldr	r3, [r6, #20]
	adds	r7, #1
	adds	r5, #4
	adds	r2, r6, #0
	cmp	r7, r3
	bne.n	.L12
.L9:
	movs	r7, #0
	adds	r0, r6, #0
	str	r7, [r6, #0]
	str	r7, [r6, #24]
	bl	Func_080c9020
	movs	r3, #1
	str	r3, [r6, #0]
	adds	r0, r6, #0
	bl	Func_080c9020
	movs	r3, #2
	str	r3, [r6, #0]
	adds	r0, r6, #0
	bl	Func_080c9020
	movs	r3, #3
	str	r3, [r6, #0]
	adds	r0, r6, #0
	bl	Func_080c9020
	adds	r0, r6, #0
	str	r7, [r6, #0]
	bl	Func_080c9018
	mov	r0, sl
	movs	r1, #1
	bl	Func_08009080
	add	r5, sp, #8
	ldr	r3, [r5, #20]
	adds	r2, r5, #0
	cmp	r3, #0
	beq.n	.L13
	movs	r6, #36
.L14:
	str	r2, [sp, #4]
	ldrsh	r0, [r2, r6]
	str	r2, [sp, #0]
	bl	Func_080b8000
	ldr	r1, [sp, #4]
	ldr	r3, [r1, #20]
	adds	r7, #1
	adds	r6, #2
	ldr	r2, [sp, #0]
	cmp	r7, r3
	bne.n	.L14
.L13:
	ldr	r0, [r5, #8]
	bl	Func_080b8000
	movs	r0, #0
.L4:
	add	sp, #92
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f00
	.4byte 0xffffe000
	.4byte 0x03001e74
