.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014d00, 0x08014d00
	.set sub_08022bd8, 0x08022bd8
	.global Func_08022f24
	.thumb_func
Func_08022f24:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldrb	r3, [r6, #24]
	ldr	r5, [r6, #40]
	mov	r8, r3
	cmp	r6, #0
	beq.n	.L_08022f5e
	ldrb	r3, [r5, #20]
	ldr	r2, [r5, #16]
	movs	r7, #0
	ldrb	r3, [r2, r3]
	cmp	r3, #241
	beq.n	.L_08022f5e
.L_08022f42:
	movs	r0, #1
	adds	r7, #1
	bl	sub_08013560
	cmp	r7, #89
	bgt.n	.L_08022f5e
	ldrb	r3, [r5, #20]
	ldr	r2, [r5, #16]
	ldrb	r3, [r2, r3]
	cmp	r3, #241
	beq.n	.L_08022f5e
	ldrb	r3, [r6, #24]
	cmp	r3, r8
	beq.n	.L_08022f42
.L_08022f5e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_08022f64:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldrb	r3, [r0, #16]
	ldr	r2, [pc, #108]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	ldr	r2, [pc, #104]
	adds	r5, r3, r2
	ldrb	r2, [r0, #20]
	ldrb	r3, [r0, #21]
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_08022f84
	adds	r3, #63
.L_08022f84:
	asrs	r6, r3, #6
	movs	r4, #0
	cmp	r4, r6
	bcs.n	.L_08022fd4
	ldr	r3, [pc, #84]
	movs	r0, #255
	lsls	r0, r0, #8
	movs	r2, #63
	mov	r8, r3
	mov	lr, r0
	mov	ip, r2
	movs	r7, #62
.L_08022f9c:
	adds	r3, r1, #0
	subs	r3, #64
	cmp	r3, #63
	bhi.n	.L_08022fca
	lsls	r3, r4, #4
	mov	r0, ip
	adds	r3, r1, r3
	ands	r3, r0
	mov	r0, r8
	ldrb	r2, [r0, r3]
	adds	r3, r2, #0
	ands	r3, r7
	adds	r0, r5, r3
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08022fc2
	ldrb	r3, [r0, #0]
	b.n	.L_08022fc8
.L_08022fc2:
	ldrh	r2, [r0, #0]
	mov	r3, lr
	ands	r3, r2
.L_08022fc8:
	strh	r3, [r0, #0]
.L_08022fca:
	adds	r4, #1
	adds	r5, #64
	adds	r1, #1
	cmp	r4, r6
	bcc.n	.L_08022f9c
.L_08022fd4:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x020036e0
	.4byte 0x06010000
	.2byte 0xeb4c
	.2byte 0x0802
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r6, #0
.L_08022fee:
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	.L_08022f64
	adds	r1, r6, #1
	adds	r0, r5, #0
	bl	.L_08022f64
	adds	r1, r6, #2
	adds	r0, r5, #0
	bl	.L_08022f64
	adds	r1, r6, #3
	adds	r0, r5, #0
	bl	.L_08022f64
	adds	r6, #4
	movs	r0, #1
	bl	sub_08013560
	cmp	r6, #127
	bls.n	.L_08022fee
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	str	r0, [sp, #0]
	mov	fp, r1
	movs	r7, #0
.L_08023032:
	mov	r3, fp
	cmp	r3, #0
	ble.n	.L_0802306e
	adds	r3, r7, #1
	mov	r9, r3
	ldr	r5, [sp, #0]
	adds	r3, r7, #2
	mov	sl, r3
	adds	r3, r7, #3
	mov	r8, r3
	mov	r6, fp
.L_08023048:
	ldr	r0, [r5, #0]
	adds	r1, r7, #0
	bl	.L_08022f64
	ldr	r0, [r5, #0]
	mov	r1, r9
	bl	.L_08022f64
	ldr	r0, [r5, #0]
	mov	r1, sl
	bl	.L_08022f64
	subs	r6, #1
	ldmia	r5!, {r0}
	mov	r1, r8
	bl	.L_08022f64
	cmp	r6, #0
	bne.n	.L_08023048
.L_0802306e:
	movs	r0, #1
	adds	r7, #4
	bl	sub_08013560
	cmp	r7, #127
	bls.n	.L_08023032
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	ldr	r4, [pc, #64]
	adds	r3, #228
	adds	r5, r1, #0
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	ldr	r3, [r0, #8]
	ands	r1, r4
	subs	r1, r3, r1
	ldr	r3, [r0, #16]
	ldr	r0, [pc, #52]
	ands	r2, r4
	subs	r2, r3, r2
	adds	r3, r1, r0
	ldr	r0, [pc, #48]
	cmp	r3, r0
	bhi.n	.L_080230c8
	cmp	r2, #0
	ble.n	.L_080230c8
	movs	r3, #224
	lsls	r3, r3, #16
	cmp	r2, r3
	bge.n	.L_080230c8
	asrs	r3, r1, #16
	stmia	r5!, {r3}
	asrs	r3, r2, #16
	str	r3, [r5, #0]
	movs	r0, #0
	b.n	.L_080230d2
.L_080230c8:
	movs	r3, #0
	stmia	r5!, {r3}
	movs	r0, #1
	str	r3, [r5, #0]
	negs	r0, r0
.L_080230d2:
	pop	{r5, pc}
	.4byte 0xffff0000
	.4byte 0x001fffff
	.2byte 0xfffe
	.2byte 0x012f
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	movs	r1, #92
	movs	r0, #24
	sub	sp, #4
	bl	sub_08014d00
	movs	r1, #128
	lsls	r1, r1, #6
	mov	r8, r0
	movs	r0, #20
	bl	sub_08014d00
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_08022bd8
	movs	r3, #128
	mov	r4, sp
	movs	r5, #0
	lsls	r3, r3, #19
	str	r5, [r4, #0]
	adds	r3, #212
	adds	r0, r4, #0
	adds	r1, r6, #0
	ldr	r2, [pc, #108]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #133
	lsls	r2, r2, #24
	str	r5, [r4, #0]
	adds	r0, r4, #0
	mov	r1, r8
	adds	r2, #23
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	cmp	r7, #4
	bne.n	.L_0802313e
	movs	r1, #128
	lsls	r1, r1, #3
	ldr	r0, [pc, #80]
	adds	r1, #138
	bl	sub_080145a8
	b.n	.L_08023156
.L_0802313e:
	movs	r1, #227
	lsls	r1, r1, #2
	adds	r1, #255
	ldr	r0, [pc, #68]
	bl	sub_080145a8
	movs	r1, #128
	lsls	r1, r1, #3
	ldr	r0, [pc, #64]
	adds	r1, #138
	bl	sub_080145a8
.L_08023156:
	subs	r3, r7, #3
	cmp	r3, #1
	bhi.n	.L_08023168
	movs	r1, #144
	ldr	r0, [pc, #52]
	lsls	r1, r1, #3
	bl	sub_080145a8
	b.n	.L_08023172
.L_08023168:
	movs	r1, #144
	ldr	r0, [pc, #44]
	lsls	r1, r1, #3
	bl	sub_080145a8
.L_08023172:
	mov	r1, r8
	movs	r2, #0
	movs	r3, #15
	strb	r3, [r1, #6]
	strb	r2, [r1, #7]
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x85000800
	.4byte 0x0802493d
	.4byte 0x08023f9d
	.4byte 0x080246b9
	.4byte 0x08023e19
	.4byte 0x0802386d
