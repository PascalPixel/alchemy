@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fae58
	.thumb_func
Func_080fae58:
	push	{r4, r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #28
	ldr	r0, [pc, #16]
	ldr	r0, [r0, #0]
	str	r0, [sp, #4]
	ldrb	r0, [r0, #10]
	cmp	r0, #0
	beq.n	.L0
	subs	r0, #1
	ldr	r1, [sp, #4]
	strb	r0, [r1, #10]
	b.n	.L1
	.4byte 0x03007ff0
.L0:
	movs	r0, #14
	ldr	r2, [sp, #4]
	strb	r0, [r2, #10]
.L1:
	movs	r6, #1
	ldr	r0, [sp, #4]
	ldr	r4, [r0, #28]
.L50:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	ands	r0, r1
	adds	r2, r6, #1
	mov	sl, r2
	movs	r2, #64
	adds	r2, r2, r4
	mov	r9, r2
	cmp	r0, #0
	bne.n	.L2
	b.n	.L3
.L2:
	cmp	r6, #2
	beq.n	.L4
	cmp	r6, #2
	bgt.n	.L5
	cmp	r6, #1
	beq.n	.L6
	b.n	.L7
.L5:
	cmp	r6, #3
	beq.n	.L8
	b.n	.L7
.L6:
	ldr	r0, [pc, #16]
	str	r0, [sp, #8]
	ldr	r7, [pc, #16]
	ldr	r2, [pc, #16]
	str	r2, [sp, #12]
	adds	r0, #4
	str	r0, [sp, #16]
	adds	r2, #2
	b.n	.L9
	.4byte 0x04000060
	.4byte 0x04000062
	.4byte 0x04000063
.L4:
	ldr	r0, [pc, #8]
	str	r0, [sp, #8]
	ldr	r7, [pc, #8]
	ldr	r2, [pc, #12]
	b.n	.L10
	movs	r0, r0
	.4byte 0x04000061
	.4byte 0x04000068
	.4byte 0x04000069
.L8:
	ldr	r0, [pc, #16]
	str	r0, [sp, #8]
	ldr	r7, [pc, #16]
	ldr	r2, [pc, #20]
	str	r2, [sp, #12]
	adds	r0, #4
	str	r0, [sp, #16]
	adds	r2, #2
	b.n	.L9
	movs	r0, r0
	.4byte 0x04000070
	.4byte 0x04000072
	.4byte 0x04000073
.L7:
	ldr	r0, [pc, #92]
	str	r0, [sp, #8]
	ldr	r7, [pc, #92]
	ldr	r2, [pc, #96]
.L10:
	str	r2, [sp, #12]
	adds	r0, #11
	str	r0, [sp, #16]
	adds	r2, #4
.L9:
	str	r2, [sp, #20]
	ldr	r0, [sp, #4]
	ldrb	r0, [r0, #10]
	str	r0, [sp, #0]
	ldr	r2, [sp, #12]
	ldrb	r0, [r2, #0]
	mov	r8, r0
	adds	r2, r1, #0
	movs	r0, #128
	ands	r0, r2
	cmp	r0, #0
	beq.n	.L11
	movs	r3, #64
	adds	r0, r3, #0
	ands	r0, r2
	lsls	r0, r0, #24
	lsrs	r5, r0, #24
	adds	r0, r6, #1
	mov	sl, r0
	movs	r1, #64
	adds	r1, r1, r4
	mov	r9, r1
	cmp	r5, #0
	bne.n	.L12
	movs	r0, #3
	strb	r0, [r4, #0]
	strb	r0, [r4, #29]
	adds	r0, r4, #0
	str	r3, [sp, #24]
	bl	Func_080fadf0
	ldr	r3, [sp, #24]
	cmp	r6, #2
	beq.n	.L13
	cmp	r6, #2
	bgt.n	.L14
	cmp	r6, #1
	beq.n	.L15
	b.n	.L16
	movs	r0, r0
	.4byte 0x04000071
	.4byte 0x04000078
	.4byte 0x04000079
.L14:
	cmp	r6, #3
	beq.n	.L17
	b.n	.L16
.L15:
	ldrb	r0, [r4, #31]
	ldr	r2, [sp, #8]
	strb	r0, [r2, #0]
.L13:
	ldr	r0, [r4, #36]
	lsls	r0, r0, #6
	ldrb	r1, [r4, #30]
	adds	r0, r1, r0
	strb	r0, [r7, #0]
	b.n	.L18
.L17:
	ldr	r1, [r4, #36]
	ldr	r0, [r4, #40]
	cmp	r1, r0
	beq.n	.L19
	ldr	r2, [sp, #8]
	strb	r3, [r2, #0]
	ldr	r1, [pc, #44]
	ldr	r2, [r4, #36]
	ldr	r0, [r2, #0]
	str	r0, [r1, #0]
	adds	r1, #4
	ldr	r0, [r2, #4]
	str	r0, [r1, #0]
	adds	r1, #4
	ldr	r0, [r2, #8]
	str	r0, [r1, #0]
	adds	r1, #4
	ldr	r0, [r2, #12]
	str	r0, [r1, #0]
	str	r2, [r4, #40]
.L19:
	ldr	r0, [sp, #8]
	strb	r5, [r0, #0]
	ldrb	r0, [r4, #30]
	strb	r0, [r7, #0]
	ldrb	r0, [r4, #30]
	cmp	r0, #0
	beq.n	.L20
	movs	r0, #192
	b.n	.L21
	movs	r0, r0
	.4byte 0x04000090
.L20:
	movs	r1, #128
	negs	r1, r1
	strb	r1, [r4, #26]
	b.n	.L22
.L16:
	ldrb	r0, [r4, #30]
	strb	r0, [r7, #0]
	ldr	r0, [r4, #36]
	lsls	r0, r0, #3
	ldr	r2, [sp, #16]
	strb	r0, [r2, #0]
.L18:
	ldrb	r0, [r4, #4]
	adds	r0, #8
	mov	r8, r0
	ldrb	r0, [r4, #30]
	cmp	r0, #0
	beq.n	.L21
	movs	r0, #64
.L21:
	strb	r0, [r4, #26]
.L22:
	ldrb	r1, [r4, #4]
	movs	r2, #0
	strb	r1, [r4, #11]
	movs	r0, #255
	ands	r0, r1
	adds	r1, r6, #1
	mov	sl, r1
	movs	r1, #64
	adds	r1, r1, r4
	mov	r9, r1
	cmp	r0, #0
	bne.n	.L23
	b.n	.L24
.L23:
	strb	r2, [r4, #9]
	b.n	.L25
.L11:
	movs	r0, #4
	ands	r0, r2
	cmp	r0, #0
	beq.n	.L26
	ldrb	r0, [r4, #13]
	subs	r0, #1
	strb	r0, [r4, #13]
	movs	r2, #255
	ands	r0, r2
	lsls	r0, r0, #24
	adds	r1, r6, #1
	mov	sl, r1
	movs	r2, #64
	adds	r2, r2, r4
	mov	r9, r2
	cmp	r0, #0
	ble.n	.L12
	b.n	.L27
.L12:
	lsls	r0, r6, #24
	lsrs	r0, r0, #24
	bl	Func_080fada0
	movs	r0, #0
	strb	r0, [r4, #0]
	b.n	.L28
.L26:
	movs	r0, #64
	ands	r0, r1
	adds	r2, r6, #1
	mov	sl, r2
	movs	r2, #64
	adds	r2, r2, r4
	mov	r9, r2
	cmp	r0, #0
	beq.n	.L29
	movs	r0, #3
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L29
	movs	r0, #252
	ands	r0, r1
	movs	r2, #0
	strb	r0, [r4, #0]
	ldrb	r1, [r4, #7]
	strb	r1, [r4, #11]
	movs	r0, #255
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L30
	movs	r0, #1
	ldrb	r1, [r4, #29]
	orrs	r0, r1
	strb	r0, [r4, #29]
	cmp	r6, #3
	beq.n	.L25
	ldrb	r2, [r4, #7]
	mov	r8, r2
	b.n	.L25
.L29:
	ldrb	r0, [r4, #11]
	cmp	r0, #0
	bne.n	.L25
	cmp	r6, #3
	bne.n	.L31
	movs	r0, #1
	ldrb	r1, [r4, #29]
	orrs	r0, r1
	strb	r0, [r4, #29]
.L31:
	adds	r0, r4, #0
	bl	Func_080fadf0
	movs	r0, #3
	ldrb	r2, [r4, #0]
	ands	r0, r2
	cmp	r0, #0
	bne.n	.L32
	ldrb	r0, [r4, #9]
	subs	r0, #1
	strb	r0, [r4, #9]
	movs	r1, #255
	ands	r0, r1
	lsls	r0, r0, #24
	cmp	r0, #0
	bgt.n	.L33
.L30:
	ldrb	r2, [r4, #12]
	ldrb	r1, [r4, #10]
	adds	r0, r2, #0
	muls	r0, r1
	adds	r0, #255
	asrs	r0, r0, #8
	movs	r1, #0
	strb	r0, [r4, #9]
	lsls	r0, r0, #24
	cmp	r0, #0
	beq.n	.L12
	movs	r0, #4
	ldrb	r2, [r4, #0]
	orrs	r0, r2
	strb	r0, [r4, #0]
	movs	r0, #1
	ldrb	r1, [r4, #29]
	orrs	r0, r1
	strb	r0, [r4, #29]
	cmp	r6, #3
	beq.n	.L27
	movs	r2, #8
	mov	r8, r2
	b.n	.L27
.L33:
	ldrb	r0, [r4, #7]
	b.n	.L34
.L32:
	cmp	r0, #1
	bne.n	.L35
.L39:
	ldrb	r0, [r4, #25]
	strb	r0, [r4, #9]
	movs	r0, #7
	b.n	.L34
.L35:
	cmp	r0, #2
	bne.n	.L36
	ldrb	r0, [r4, #9]
	subs	r0, #1
	strb	r0, [r4, #9]
	movs	r1, #255
	ands	r0, r1
	lsls	r0, r0, #24
	ldrb	r2, [r4, #25]
	lsls	r1, r2, #24
	cmp	r0, r1
	bgt.n	.L37
.L41:
	ldrb	r0, [r4, #6]
	cmp	r0, #0
	bne.n	.L38
	movs	r0, #252
	ldrb	r1, [r4, #0]
	ands	r0, r1
	strb	r0, [r4, #0]
	b.n	.L30
.L38:
	ldrb	r0, [r4, #0]
	subs	r0, #1
	strb	r0, [r4, #0]
	movs	r0, #1
	ldrb	r2, [r4, #29]
	orrs	r0, r2
	strb	r0, [r4, #29]
	cmp	r6, #3
	beq.n	.L39
	movs	r0, #8
	mov	r8, r0
	b.n	.L39
.L37:
	ldrb	r0, [r4, #5]
	b.n	.L34
.L36:
	ldrb	r0, [r4, #9]
	adds	r0, #1
	strb	r0, [r4, #9]
	movs	r1, #255
	ands	r0, r1
	ldrb	r2, [r4, #10]
	cmp	r0, r2
	bcc.n	.L40
.L24:
	ldrb	r0, [r4, #0]
	subs	r0, #1
	movs	r2, #0
	strb	r0, [r4, #0]
	ldrb	r1, [r4, #5]
	strb	r1, [r4, #11]
	movs	r0, #255
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L41
	movs	r0, #1
	ldrb	r1, [r4, #29]
	orrs	r0, r1
	strb	r0, [r4, #29]
	ldrb	r0, [r4, #10]
	strb	r0, [r4, #9]
	cmp	r6, #3
	beq.n	.L25
	ldrb	r2, [r4, #5]
	mov	r8, r2
	b.n	.L25
.L40:
	ldrb	r0, [r4, #4]
.L34:
	strb	r0, [r4, #11]
.L25:
	ldrb	r0, [r4, #11]
	subs	r0, #1
	strb	r0, [r4, #11]
	ldr	r0, [sp, #0]
	cmp	r0, #0
	bne.n	.L27
	subs	r0, #1
	str	r0, [sp, #0]
	b.n	.L29
.L27:
	movs	r0, #2
	ldrb	r1, [r4, #29]
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L42
	cmp	r6, #3
	bgt.n	.L43
	movs	r0, #8
	ldrb	r2, [r4, #1]
	ands	r0, r2
	cmp	r0, #0
	beq.n	.L43
	ldr	r0, [pc, #16]
	ldrb	r0, [r0, #0]
	cmp	r0, #63
	bgt.n	.L44
	ldr	r0, [r4, #32]
	adds	r0, #2
	ldr	r1, [pc, #8]
	b.n	.L45
	movs	r0, r0
	.4byte 0x04000089
	.4byte 0x000007fc
.L44:
	cmp	r0, #127
	bgt.n	.L43
	ldr	r0, [r4, #32]
	adds	r0, #1
	ldr	r1, [pc, #16]
.L45:
	ands	r0, r1
	str	r0, [r4, #32]
.L43:
	cmp	r6, #4
	beq.n	.L46
	ldr	r0, [r4, #32]
	ldr	r1, [sp, #16]
	strb	r0, [r1, #0]
	b.n	.L47
	movs	r0, r0
	.4byte 0x000007fe
.L46:
	ldr	r2, [sp, #16]
	ldrb	r0, [r2, #0]
	movs	r1, #8
	ands	r1, r0
	ldr	r0, [r4, #32]
	orrs	r0, r1
	strb	r0, [r2, #0]
.L47:
	movs	r0, #192
	ldrb	r1, [r4, #26]
	ands	r0, r1
	adds	r1, r4, #0
	adds	r1, #33
	ldrb	r1, [r1, #0]
	adds	r0, r1, r0
	strb	r0, [r4, #26]
	movs	r2, #255
	ands	r0, r2
	ldr	r1, [sp, #20]
	strb	r0, [r1, #0]
.L42:
	movs	r0, #1
	ldrb	r2, [r4, #29]
	ands	r0, r2
	cmp	r0, #0
	beq.n	.L28
	ldr	r1, [pc, #60]
	ldrb	r0, [r1, #0]
	ldrb	r2, [r4, #28]
	bics	r0, r2
	ldrb	r2, [r4, #27]
	orrs	r0, r2
	strb	r0, [r1, #0]
	cmp	r6, #3
	bne.n	.L48
	ldr	r0, [pc, #48]
	ldrb	r1, [r4, #9]
	adds	r0, r1, r0
	ldrb	r0, [r0, #0]
	ldr	r2, [sp, #12]
	strb	r0, [r2, #0]
	movs	r1, #128
	adds	r0, r1, #0
	ldrb	r2, [r4, #26]
	ands	r0, r2
	cmp	r0, #0
	beq.n	.L28
	ldr	r0, [sp, #8]
	strb	r1, [r0, #0]
	ldrb	r0, [r4, #26]
	ldr	r1, [sp, #20]
	strb	r0, [r1, #0]
	movs	r0, #127
	ldrb	r2, [r4, #26]
	ands	r0, r2
	strb	r0, [r4, #26]
	b.n	.L28
	movs	r0, r0
	.4byte 0x04000081
	.4byte 0x080fba04
.L48:
	movs	r0, #15
	mov	r1, r8
	ands	r1, r0
	mov	r8, r1
	ldrb	r2, [r4, #9]
	lsls	r0, r2, #4
	add	r0, r8
	ldr	r1, [sp, #12]
	strb	r0, [r1, #0]
	movs	r2, #128
	ldrb	r0, [r4, #26]
	orrs	r0, r2
	ldr	r1, [sp, #20]
	strb	r0, [r1, #0]
	cmp	r6, #1
	bne.n	.L28
	ldr	r0, [sp, #8]
	ldrb	r1, [r0, #0]
	movs	r0, #8
	ands	r0, r1
	cmp	r0, #0
	bne.n	.L28
	ldrb	r0, [r4, #26]
	orrs	r0, r2
	ldr	r1, [sp, #20]
	strb	r0, [r1, #0]
.L28:
	movs	r0, #0
	strb	r0, [r4, #29]
.L3:
	mov	r6, sl
	mov	r4, r9
	cmp	r6, #4
	bgt.n	.L49
	b.n	.L50
.L49:
	add	sp, #28
	pop	{r3, r4, r5}
	mov	r8, r3
	mov	r9, r4
	mov	sl, r5
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
