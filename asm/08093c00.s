@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08093c00
	.thumb_func
Func_08093c00:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #516]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #20
	bl	Func_08092054
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #4]
	adds	r6, r0, #0
	ldrh	r3, [r6, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #8
	ands	r2, r3
	movs	r3, #85
	adds	r3, r3, r6
	mov	r9, r2
	ldrb	r2, [r3, #0]
	mov	r8, r3
	str	r2, [sp, #0]
	ldr	r3, [pc, #468]
	ldr	r3, [r3, #0]
	mov	sl, r3
	movs	r3, #1
	mov	fp, r3
	add	r7, sp, #8
.L10:
	ldr	r3, [r6, #8]
	ldr	r5, [pc, #460]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r5
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r3, [r6, #12]
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	ands	r3, r5
	adds	r3, r3, r2
	movs	r0, #128
	lsls	r0, r0, #13
	mov	r1, r9
	str	r3, [r7, #8]
	adds	r2, r7, #0
	bl	Func_0800447c
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	Func_080091d8
	cmp	r0, #1
	bne.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	ldr	r3, [r6, #8]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r5
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r3, [r6, #12]
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	ands	r3, r5
	adds	r3, r3, r2
	movs	r0, #128
	lsls	r0, r0, #14
	mov	r1, r9
	str	r3, [r7, #8]
	adds	r2, r7, #0
	bl	Func_0800447c
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	Func_080091d8
	cmp	r0, #0
	beq.n	.L2
	b.n	.L3
.L2:
	adds	r3, r6, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L4
	ldr	r3, [r6, #80]
	adds	r3, #38
	ldrb	r3, [r3, #0]
	mov	fp, r3
.L4:
	bl	Func_080916b0
	movs	r1, #6
	adds	r0, r6, #0
	bl	Func_08009080
	movs	r0, #6
	bl	Func_080030f8
	movs	r0, #152
	bl	Func_080f9010
	adds	r0, r6, #0
	movs	r1, #7
	bl	Func_08009080
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r6, #48]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r6, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #126
	ands	r3, r2
	mov	r2, r8
	strb	r3, [r2, #0]
	movs	r1, #254
	mov	r3, fp
	ands	r1, r3
	adds	r0, r6, #0
	bl	Func_080091e0
	ldr	r3, [pc, #256]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r3, #10
	ldrsh	r2, [r7, r3]
	movs	r3, #2
	ldrsh	r1, [r7, r3]
	bl	Func_08092158
	adds	r0, r6, #0
	movs	r1, #6
	bl	Func_08009080
	adds	r0, r6, #0
	mov	r1, fp
	bl	Func_080091e0
	adds	r0, r6, #0
	movs	r1, #207
	bl	Func_08093af8
	cmp	r0, #0
	bne.n	.L5
	adds	r0, r6, #0
	movs	r1, #205
	bl	Func_08093af8
	cmp	r0, #0
	beq.n	.L6
.L5:
	movs	r1, #7
	bl	Func_08009080
	ldr	r5, [pc, #200]
	ldr	r3, [r6, #12]
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r6, #20]
	adds	r3, r3, r5
	str	r3, [r6, #20]
	movs	r0, #2
	bl	Func_080030f8
	ldr	r3, [r6, #12]
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r6, #20]
	adds	r3, r3, r5
	str	r3, [r6, #20]
	movs	r0, #10
	bl	Func_080030f8
	movs	r5, #128
	ldr	r3, [r6, #12]
	lsls	r5, r5, #9
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r6, #20]
	adds	r3, r3, r5
	str	r3, [r6, #20]
	movs	r0, #4
	bl	Func_080030f8
	ldr	r3, [r6, #12]
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r6, #20]
	adds	r3, r3, r5
	str	r3, [r6, #20]
	b.n	.L7
.L6:
	movs	r0, #6
	bl	Func_080030f8
.L7:
	mov	r2, sp
	ldrb	r2, [r2, #0]
	mov	r3, r8
	strb	r2, [r3, #0]
	bl	Func_08091750
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L8
	movs	r3, #216
	lsls	r3, r3, #1
	add	r3, sl
	movs	r1, #128
	ldr	r4, [pc, #96]
	ldr	r0, [r3, #0]
	lsls	r1, r1, #14
	movs	r0, r0
	mov	ip, pc
	bx	r4
	movs	r2, #218
	lsls	r2, r2, #1
	add	r2, sl
	ldr	r3, [r2, #0]
	adds	r3, r3, r0
	str	r3, [r2, #0]
.L8:
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl	Func_080091b0
	cmp	r0, #249
	bne.n	.L9
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08009080
	movs	r0, #6
	bl	Func_080030f8
	b.n	.L10
.L9:
	movs	r2, #0
	str	r2, [sp, #4]
.L3:
	ldr	r0, [sp, #4]
.L1:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x03001ebc
	.4byte 0xfff00000
	.4byte 0xffff0000
	.4byte 0x03000118
