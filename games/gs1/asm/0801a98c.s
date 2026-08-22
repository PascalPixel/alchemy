@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801a98c
	.thumb_func
Func_0801a98c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #132]
	ldr	r3, [r3, #0]
	movs	r0, #192
	mov	sl, r3
	lsls	r0, r0, #2
	movs	r3, #0
	sub	sp, #24
	add	r0, sl
	mov	fp, r3
	movs	r3, #210
	str	r0, [sp, #8]
	lsls	r3, r3, #2
	movs	r1, #182
	movs	r2, #195
	add	r3, sl
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	ldr	r7, [r3, #0]
	add	r1, sl
	add	r2, sl
	mov	r8, r1
	mov	r9, r2
	cmp	r7, #0
	bne.n	.L0
	b.n	.L1
.L0:
	adds	r6, r7, #0
	adds	r6, #40
	movs	r4, #4
	ldrb	r3, [r6, #5]
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	movs	r5, #63
	strb	r3, [r6, #5]
	negs	r5, r5
	ldrb	r3, [r6, #7]
	adds	r2, r5, #0
	ands	r3, r2
	strb	r3, [r6, #7]
	ldrh	r1, [r7, #16]
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #44]
	ands	r1, r3
	ldrh	r3, [r6, #6]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #6]
	ldrh	r3, [r7, #18]
	movs	r0, #240
	strb	r3, [r6, #4]
	adds	r1, r3, #0
	movs	r3, #232
	str	r0, [sp, #4]
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r2, [r3, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L2
	adds	r3, r2, r1
	strb	r3, [r6, #4]
	b.n	.L3
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x03001e98
.L2:
	movs	r1, #16
	ldrsh	r2, [r7, r1]
	movs	r3, #24
	ldrsh	r1, [r7, r3]
	ldrh	r5, [r7, #16]
	ldrh	r4, [r7, #24]
	cmp	r2, r1
	beq.n	.L4
	ldrh	r0, [r7, #20]
	mov	ip, r0
	movs	r0, #20
	ldrsh	r3, [r7, r0]
	cmp	r3, #0
	ble.n	.L5
	adds	r3, r2, r3
	cmp	r3, r1
	bgt.n	.L6
	mov	r1, ip
	adds	r3, r5, r1
	b.n	.L7
.L5:
	adds	r3, r2, r3
	cmp	r3, r1
	bge.n	.L8
.L6:
	strh	r4, [r7, #16]
	b.n	.L9
.L8:
	mov	r2, ip
	adds	r3, r5, r2
.L7:
	strh	r3, [r7, #16]
.L9:
	ldrh	r1, [r7, #16]
	ldr	r3, [pc, #16]
	ldr	r2, [pc, #16]
	ands	r1, r3
	ldrh	r3, [r6, #6]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #6]
	b.n	.L3
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
.L4:
	ldr	r3, [pc, #724]
	add	r3, sl
	ldrh	r3, [r3, #0]
	cmp	fp, r3
	bne.n	.L3
	movs	r3, #241
	str	r3, [sp, #4]
	mov	r4, r8
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	beq.n	.L3
	add	r5, sp, #12
	adds	r1, r5, #0
	ldrh	r0, [r7, #8]
	bl	Func_080b50b0
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L3
	ldr	r2, [r5, #0]
	mov	r3, r8
	strh	r2, [r3, #24]
	mov	r4, r8
	ldr	r1, [r5, #4]
	movs	r5, #34
	ldrsh	r3, [r4, r5]
	strh	r1, [r4, #26]
	cmp	r3, #0
	bne.n	.L3
	mov	r0, r8
	strh	r2, [r0, #16]
	movs	r3, #1
	mov	r2, r8
	strh	r1, [r2, #18]
	strh	r3, [r4, #34]
.L3:
	movs	r5, #34
	ldrsh	r3, [r7, r5]
	cmp	r3, #0
	beq.n	.L10
	ldr	r0, [pc, #648]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L11
	ldr	r3, [pc, #644]
	add	r3, sl
	ldrh	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L12
	ldrb	r3, [r6, #5]
	movs	r0, #13
	negs	r0, r0
	ands	r3, r0
	movs	r2, #4
	orrs	r3, r2
	b.n	.L13
.L12:
	ldrb	r3, [r6, #5]
	movs	r1, #13
	negs	r1, r1
	ands	r3, r1
.L13:
	strb	r3, [r6, #5]
	ldrh	r3, [r7, #10]
	cmp	r3, #1
	bne.n	.L11
	ldrb	r3, [r6, #5]
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #5]
.L11:
	adds	r0, r6, #0
	ldr	r1, [sp, #4]
	bl	Func_08003dec
.L10:
	ldr	r7, [r7, #4]
	movs	r3, #1
	add	fp, r3
	cmp	r7, #0
	beq.n	.L1
	b.n	.L0
.L1:
	mov	r4, r9
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L14
	b.n	.L15
.L14:
	mov	r0, sl
	bl	Func_0801b36c
	mov	r5, r9
	adds	r5, #40
	movs	r6, #13
	ldrb	r3, [r5, #5]
	negs	r6, r6
	adds	r2, r6, #0
	ands	r2, r3
	movs	r3, #4
	ldrb	r1, [r5, #7]
	negs	r3, r3
	ands	r2, r3
	subs	r3, #59
	ands	r3, r1
	movs	r1, #17
	negs	r1, r1
	ands	r2, r1
	movs	r1, #32
	adds	r7, r0, #0
	orrs	r2, r1
	movs	r0, #63
	ands	r2, r0
	ands	r3, r0
	movs	r1, #128
	strb	r2, [r5, #5]
	orrs	r3, r1
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #7]
	adds	r3, r6, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	mov	r0, r9
	ldrh	r3, [r0, #14]
	ldr	r2, [pc, #492]
	ldrh	r1, [r5, #8]
	ands	r2, r3
	ldr	r3, [pc, #492]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
	ldrh	r2, [r7, #16]
	ldr	r1, [pc, #484]
	subs	r2, #4
	ands	r2, r1
	ldr	r3, [pc, #484]
	ldrh	r1, [r5, #6]
	mov	fp, r3
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldr	r3, [pc, #476]
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #476]
	movs	r2, #15
	lsrs	r3, r3, #1
	ands	r3, r2
	ldrb	r3, [r0, r3]
	ldrb	r1, [r7, #18]
	lsls	r3, r3, #24
	asrs	r3, r3, #25
	adds	r1, r1, r3
	subs	r1, #4
	strb	r1, [r5, #4]
	mov	r4, r9
	movs	r0, #34
	ldrsh	r2, [r4, r0]
	movs	r0, #38
	ldrsh	r3, [r4, r0]
	ldrh	r1, [r4, #34]
	cmp	r2, r3
	beq.n	.L16
	movs	r0, #208
	lsls	r0, r0, #2
	add	r0, sl
	strh	r1, [r0, #0]
	ldr	r3, [pc, #432]
	ldrh	r2, [r4, #34]
	add	r3, sl
	strh	r2, [r3, #0]
	movs	r2, #209
	lsls	r2, r2, #2
	add	r2, sl
	movs	r3, #0
	strh	r3, [r2, #0]
	bl	Func_08003d28
	movs	r3, #31
	ldrb	r2, [r5, #7]
	ands	r0, r3
	movs	r3, #63
	negs	r3, r3
	lsls	r0, r0, #1
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r5, #7]
	ldrb	r3, [r5, #5]
	ldrh	r1, [r5, #6]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r5, #5]
	ldr	r2, [pc, #388]
	lsls	r3, r1, #23
	lsrs	r3, r3, #23
	ldr	r4, [pc, #360]
	adds	r3, r3, r2
	mov	r2, fp
	ands	r3, r4
	ands	r2, r1
	orrs	r2, r3
	ldrb	r3, [r5, #4]
	adds	r3, #240
	strh	r2, [r5, #6]
	strb	r3, [r5, #4]
	mov	r0, r9
	ldrh	r3, [r0, #34]
	ldrh	r2, [r0, #36]
	mov	r1, r9
	adds	r3, r3, r2
	strh	r3, [r1, #34]
.L16:
	ldr	r0, [pc, #312]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L17
	ldrb	r3, [r5, #5]
	adds	r2, r6, #0
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	strb	r2, [r5, #5]
.L17:
	adds	r0, r5, #0
	movs	r1, #248
	bl	Func_08003dec
.L15:
	mov	r0, sl
	movs	r1, #0
	bl	Func_0801aeec
	mov	r0, sl
	movs	r1, #1
	bl	Func_0801aeec
	movs	r3, #211
	lsls	r3, r3, #2
	add	r3, sl
	ldr	r7, [r3, #0]
	cmp	r7, #0
	bne.n	.L18
	b.n	.L19
.L18:
	movs	r2, #208
	lsls	r2, r2, #2
	movs	r3, #13
	add	r2, sl
	negs	r3, r3
	mov	r9, r2
	mov	fp, r3
.L27:
	movs	r4, #16
	ldrsh	r2, [r7, r4]
	movs	r5, #24
	ldrsh	r3, [r7, r5]
	adds	r6, r7, #0
	adds	r6, #40
	ldrh	r1, [r7, #16]
	cmp	r2, r3
	beq.n	.L20
	ldrh	r3, [r7, #20]
	adds	r3, r1, r3
	strh	r3, [r7, #16]
.L20:
	movs	r0, #18
	ldrsh	r2, [r7, r0]
	movs	r4, #26
	ldrsh	r3, [r7, r4]
	ldrh	r1, [r7, #18]
	cmp	r2, r3
	beq.n	.L21
	ldrh	r3, [r7, #22]
	adds	r3, r1, r3
	strh	r3, [r7, #18]
.L21:
	ldr	r4, [pc, #212]
	ldrh	r3, [r7, #16]
	ldr	r5, [pc, #212]
	ldrh	r1, [r6, #6]
	adds	r2, r4, #0
	ands	r2, r3
	adds	r3, r5, #0
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r6, #6]
	ldrh	r3, [r7, #18]
	strb	r3, [r6, #4]
	movs	r3, #34
	ldrsh	r2, [r7, r3]
	mov	lr, r2
	movs	r3, #38
	ldrsh	r2, [r7, r3]
	mov	ip, r2
	movs	r0, #0
	ldrh	r1, [r7, #34]
	cmp	lr, ip
	beq.n	.L22
	ldrh	r3, [r7, #36]
	adds	r3, r1, r3
	mov	r1, r9
	strh	r3, [r7, #34]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #172]
	ldrh	r2, [r7, #34]
	add	r3, sl
	strh	r2, [r3, #0]
	movs	r3, #209
	lsls	r3, r3, #2
	add	r3, sl
	strh	r0, [r3, #0]
	mov	r0, r9
	str	r4, [sp, #0]
	bl	Func_08003d28
	movs	r3, #31
	movs	r1, #63
	ands	r0, r3
	negs	r1, r1
	ldrb	r3, [r6, #7]
	adds	r2, r1, #0
	ands	r3, r2
	lsls	r0, r0, #1
	orrs	r3, r0
	strb	r3, [r6, #7]
	ldrb	r3, [r6, #5]
	ldrh	r1, [r6, #6]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r6, #5]
	lsls	r2, r1, #23
	ldr	r3, [pc, #124]
	lsrs	r2, r2, #23
	ldr	r4, [sp, #0]
	adds	r2, r2, r3
	adds	r3, r5, #0
	ands	r2, r4
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r6, #6]
	ldrb	r3, [r6, #4]
	adds	r3, #248
	strb	r3, [r6, #4]
	b.n	.L23
.L22:
	movs	r4, #4
	ldrb	r3, [r6, #5]
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	movs	r5, #63
	strb	r3, [r6, #5]
	negs	r5, r5
	ldrb	r3, [r6, #7]
	adds	r2, r5, #0
	ands	r3, r2
	strb	r3, [r6, #7]
.L23:
	ldr	r0, [pc, #36]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L24
	ldr	r3, [pc, #28]
	add	r3, sl
	ldrh	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L25
	ldrb	r3, [r6, #5]
	mov	r0, fp
	ands	r3, r0
	movs	r2, #4
	orrs	r3, r2
	b.n	.L26
	movs	r0, r0
	.4byte 0x0000039e
	.4byte 0x00000103
	.4byte 0x000002e2
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x03001800
	.4byte 0x08036740
	.4byte 0x00000342
	.4byte 0x0000fff0
	.4byte 0x0000fff8
.L25:
	ldrb	r3, [r6, #5]
	mov	r1, fp
	ands	r3, r1
.L26:
	strb	r3, [r6, #5]
	ldrh	r3, [r7, #10]
	cmp	r3, #1
	bne.n	.L24
	ldrb	r3, [r6, #5]
	mov	r2, fp
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #5]
.L24:
	adds	r0, r6, #0
	movs	r1, #240
	bl	Func_08003dec
	ldr	r7, [r7, #4]
	cmp	r7, #0
	beq.n	.L19
	b.n	.L27
.L19:
	mov	r4, r8
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L28
	b.n	.L29
.L28:
	ldr	r3, [pc, #76]
	ldr	r2, [r3, #0]
	ldr	r5, [pc, #76]
	movs	r3, #15
	lsrs	r2, r2, #2
	ands	r2, r3
	mov	fp, r5
	lsls	r2, r2, #8
	movs	r1, #128
	add	r2, fp
	ldrh	r0, [r4, #12]
	lsls	r1, r1, #1
	bl	Func_08003fa4
	ldr	r3, [pc, #44]
	ldr	r1, [sp, #8]
	ands	r0, r3
	ldrh	r2, [r1, #8]
	ldr	r3, [pc, #48]
	ands	r3, r2
	orrs	r3, r0
	adds	r2, r1, #0
	strh	r3, [r2, #8]
	mov	r3, r8
	ldrh	r0, [r3, #24]
	movs	r4, #24
	ldrsh	r2, [r3, r4]
	ldrh	r1, [r3, #16]
	movs	r5, #16
	ldrsh	r3, [r3, r5]
	cmp	r2, r3
	beq.n	.L30
	subs	r3, r2, r3
	asrs	r3, r3, #1
	cmp	r3, #0
	beq.n	.L31
	b.n	.L32
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x03001800
	.4byte 0x080346f8
	.4byte 0xfffffc00
.L32:
	adds	r3, r1, r3
	mov	r0, r8
	strh	r3, [r0, #16]
	b.n	.L30
.L31:
	mov	r1, r8
	strh	r0, [r1, #16]
.L30:
	mov	r2, r8
	mov	r5, r8
	ldrh	r1, [r2, #26]
	movs	r3, #26
	ldrsh	r2, [r2, r3]
	movs	r4, #18
	ldrsh	r3, [r5, r4]
	adds	r0, r3, #0
	cmp	r2, r3
	beq.n	.L33
	subs	r3, r2, r3
	asrs	r3, r3, #1
	cmp	r3, #0
	beq.n	.L34
	adds	r3, r0, r3
	mov	r0, r8
	strh	r3, [r0, #18]
	adds	r0, r3, #0
	b.n	.L33
.L34:
	mov	r2, r8
	strh	r1, [r2, #18]
	adds	r0, r1, #0
.L33:
	ldr	r3, [pc, #68]
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #68]
	movs	r2, #15
	lsrs	r3, r3, #2
	ands	r3, r2
	ldrb	r3, [r1, r3]
	ldr	r4, [sp, #8]
	adds	r3, r3, r0
	subs	r3, #32
	strb	r3, [r4, #4]
	mov	r5, r8
	ldrh	r2, [r5, #16]
	ldr	r3, [pc, #40]
	subs	r2, #4
	ands	r2, r3
	ldrh	r1, [r4, #6]
	ldr	r3, [pc, #44]
	ldr	r0, [sp, #8]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #6]
	ldr	r0, [pc, #36]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L35
	ldr	r3, [pc, #32]
	add	r3, sl
	ldrh	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L36
	b.n	.L37
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x03001800
	.4byte 0x08036740
	.4byte 0xfffffe00
	.4byte 0x00000103
	.4byte 0x000002e2
.L37:
	ldr	r1, [sp, #8]
	movs	r2, #13
	ldrb	r3, [r1, #5]
	negs	r2, r2
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	strb	r2, [r1, #5]
	b.n	.L35
.L36:
	ldr	r3, [sp, #8]
	ldrb	r2, [r3, #5]
	movs	r3, #13
	negs	r3, r3
	ldr	r4, [sp, #8]
	ands	r3, r2
	strb	r3, [r4, #5]
.L35:
	ldr	r0, [sp, #8]
	movs	r1, #248
	bl	Func_08003dec
.L29:
	ldr	r2, [pc, #24]
	add	r2, sl
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000003a2
