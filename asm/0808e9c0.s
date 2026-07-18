@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808e9c0
	.thumb_func
Func_0808e9c0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #548]
	sub	sp, #12
	movs	r1, #0
	ldr	r2, [r3, #0]
	ldr	r3, [r3, #76]
	str	r1, [sp, #0]
	ldr	r2, [r2, #16]
	mov	sl, r2
	movs	r2, #142
	lsls	r2, r2, #1
	adds	r2, r2, r3
	mov	r8, r2
	add	r0, sp, #8
	str	r1, [r0, #0]
	ldr	r3, [pc, #524]
	mov	r1, r8
	ldr	r2, [pc, #524]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L0
	b.n	.L1
.L0:
	movs	r4, #1
	add	sl, r4
	ldrb	r7, [r3, #0]
	mov	r1, sl
	ldrb	r5, [r1, #0]
	add	sl, r4
	cmp	r7, #255
	bne.n	.L2
	cmp	r5, #255
	bne.n	.L2
	b.n	.L1
.L2:
	mov	r2, sl
	ldrb	r2, [r2, #0]
	movs	r3, #1
	add	sl, r3
	adds	r3, r2, #0
	subs	r3, #100
	str	r2, [sp, #4]
	cmp	r3, #139
	bls.n	.L3
	b.n	.L4
.L3:
	ldr	r3, [pc, #472]
	ldr	r0, [r3, #36]
	bl	Func_080072e4
	adds	r6, r0, #0
	movs	r4, #1
	ldr	r2, [r6, #0]
	negs	r4, r4
	cmp	r2, r4
	bne.n	.L5
	b.n	.L4
.L5:
	movs	r1, #128
	lsls	r7, r7, #20
	lsls	r1, r1, #12
	mov	fp, r7
	mov	r9, r1
	lsls	r7, r5, #20
.L19:
	movs	r4, #4
	ldrsh	r3, [r6, r4]
	ldr	r1, [sp, #4]
	cmp	r3, r1
	beq.n	.L6
	b.n	.L7
.L6:
	ldr	r3, [pc, #428]
	ands	r3, r2
	cmp	r3, #19
	bne.n	.L8
	mov	r4, r9
	mov	r1, fp
	movs	r0, #20
	add	r1, r9
	movs	r2, #0
	adds	r3, r7, r4
	bl	Func_080090c8
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L9
	b.n	.L7
.L9:
	bl	Func_0808e9a8
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_080091e0
	movs	r1, #6
	ldrsh	r0, [r6, r1]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L10
	ldr	r3, [pc, #376]
	ldr	r2, [r6, #8]
	ands	r2, r3
	movs	r3, #160
	lsls	r3, r3, #15
	cmp	r2, r3
	bne.n	.L11
	adds	r0, r5, #0
	bl	Func_080090d0
	b.n	.L7
.L11:
	adds	r0, r5, #0
	movs	r1, #2
	bl	Func_08009080
.L10:
	adds	r0, r5, #0
	bl	Func_08009140
	ldr	r2, [r5, #8]
	cmp	r2, #0
	bge.n	.L12
	ldr	r4, [pc, #340]
	adds	r2, r2, r4
.L12:
	adds	r3, r5, #0
	adds	r3, #100
	asrs	r2, r2, #16
	strh	r2, [r3, #0]
	ldr	r3, [r5, #16]
	cmp	r3, #0
	bge.n	.L13
	ldr	r1, [pc, #320]
	adds	r3, r3, r1
.L13:
	adds	r2, r5, #0
	asrs	r3, r3, #16
	adds	r2, #102
	strh	r3, [r2, #0]
	movs	r3, #1
	subs	r2, #67
	strb	r3, [r2, #0]
	adds	r2, #54
	strb	r3, [r2, #0]
	ldrh	r3, [r6, #4]
	mov	r2, r8
	strb	r3, [r2, #4]
	ldr	r3, [r5, #8]
	str	r5, [r2, #0]
	cmp	r3, #0
	bge.n	.L14
	ldr	r4, [pc, #288]
	adds	r3, r3, r4
.L14:
	ldr	r0, [r5, #16]
	asrs	r3, r3, #20
	mov	r1, r8
	strb	r3, [r1, #6]
	cmp	r0, #0
	bge.n	.L15
	ldr	r2, [pc, #272]
	adds	r0, r0, r2
	b.n	.L15
.L8:
	cmp	r3, #3
	bne.n	.L7
	ldr	r3, [pc, #256]
	ldr	r2, [r6, #8]
	ands	r2, r3
	movs	r3, #192
	lsls	r3, r3, #14
	cmp	r2, r3
	bne.n	.L7
	movs	r4, #6
	ldrsh	r0, [r6, r4]
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L7
	mov	r4, r9
	mov	r1, fp
	movs	r0, #28
	add	r1, r9
	movs	r2, #0
	adds	r3, r7, r4
	bl	Func_080090c8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L7
	bl	Func_0808e9a8
	movs	r1, #0
	adds	r0, r5, #0
	bl	Func_080091e0
	adds	r0, r5, #0
	bl	Func_08009140
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08009080
	ldr	r2, [r5, #8]
	cmp	r2, #0
	bge.n	.L16
	ldr	r1, [pc, #180]
	adds	r2, r2, r1
.L16:
	adds	r3, r5, #0
	adds	r3, #100
	asrs	r2, r2, #16
	strh	r2, [r3, #0]
	ldr	r3, [r5, #16]
	cmp	r3, #0
	bge.n	.L17
	ldr	r2, [pc, #160]
	adds	r3, r3, r2
.L17:
	adds	r2, r5, #0
	asrs	r3, r3, #16
	adds	r2, #102
	strh	r3, [r2, #0]
	movs	r3, #1
	subs	r2, #13
	strb	r3, [r2, #0]
	subs	r2, #54
	strb	r3, [r2, #0]
	mov	r3, r8
	str	r5, [r3, #0]
	ldrh	r3, [r6, #4]
	mov	r4, r8
	strb	r3, [r4, #4]
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bge.n	.L18
	ldr	r1, [pc, #128]
	adds	r3, r3, r1
.L18:
	ldr	r0, [r5, #16]
	asrs	r3, r3, #20
	mov	r2, r8
	strb	r3, [r2, #6]
	cmp	r0, #0
	bge.n	.L15
	ldr	r3, [pc, #112]
	adds	r0, r0, r3
.L15:
	mov	r4, r8
	asrs	r3, r0, #20
	strb	r3, [r4, #7]
	ldr	r2, [sp, #0]
	movs	r1, #8
	adds	r2, #1
	add	r8, r1
	str	r2, [sp, #0]
	cmp	r2, #9
	ble.n	.L4
	b.n	.L1
.L7:
	adds	r6, #12
	ldr	r3, [r6, #0]
	movs	r4, #1
	negs	r4, r4
	adds	r2, r3, #0
	cmp	r3, r4
	beq.n	.L4
	b.n	.L19
.L4:
	mov	r1, sl
	movs	r2, #1
	add	sl, r2
	ldrb	r7, [r1, #0]
	mov	r3, sl
	ldrb	r5, [r3, #0]
	add	sl, r2
	cmp	r7, #255
	beq.n	.L20
	b.n	.L2
.L20:
	cmp	r5, #255
	beq.n	.L1
	b.n	.L2
.L1:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0x040000d4
	.4byte 0x85000014
	.4byte 0x02008000
	.4byte 0x000001ff
	.4byte 0xfff00000
	.4byte 0x0000ffff
	.4byte 0x000fffff
