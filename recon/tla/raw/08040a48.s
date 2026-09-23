.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_08013b30, 0x08013b30
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014d00, 0x08014d00
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_08039418, 0x08039418
	.set sub_080409a4, 0x080409a4
	.set sub_080409dc, 0x080409dc
	.set sub_080409f0, 0x080409f0
	.set sub_08041c54, 0x08041c54
	.set sub_08041fa4, 0x08041fa4
	.set sub_08042188, 0x08042188
	.set sub_08042244, 0x08042244
	.set sub_08042314, 0x08042314
	.set sub_08044460, 0x08044460
	.set sub_080ad020, 0x080ad020
	.set sub_080c8268, 0x080c8268
	.set sub_08108030, 0x08108030
	.set sub_08108040, 0x08108040
	.set sub_08108048, 0x08108048
	.set sub_081c0010, 0x081c0010
	.global Func_08040a48
	.thumb_func
Func_08040a48:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #496]
	sub	sp, #24
	movs	r2, #8
	movs	r1, #0
	add	r2, sp
	movs	r0, #1
	mov	r8, r1
	mov	r9, r0
	mov	fp, r2
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	ldr	r3, [r3, #0]
	mov	sl, fp
	str	r3, [r2, #0]
	bl	sub_080409a4
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r3, [r3, #0]
	str	r3, [sp, #4]
	bl	sub_080409f0
	adds	r7, r0, #0
	movs	r0, #1
	bl	sub_08013560
.L_08040a8c:
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L_08040b42
	mov	r0, r8
	movs	r3, #0
	movs	r1, #4
	adds	r0, #4
	mov	r9, r3
	bl	sub_08002064
	mov	r4, sl
	mov	r8, r0
	ldr	r0, [r4, #12]
	movs	r1, #5
	adds	r0, #5
	bl	sub_08002064
	mov	r1, sl
	str	r0, [r1, #12]
	adds	r0, r7, #0
	bl	sub_08039418
	movs	r6, #2
	movs	r5, #2
.L_08040abc:
	adds	r2, r6, #0
	adds	r0, r7, #0
	movs	r1, #0
	movs	r3, #14
	subs	r5, #1
	str	r6, [sp, #0]
	bl	sub_08041c54
	adds	r6, #2
	cmp	r5, #0
	bge.n	.L_08040abc
	movs	r5, #0
	mov	r6, fp
.L_08040ad6:
	lsls	r3, r5, #4
	ldmia	r6!, {r0}
	movs	r1, #0
	str	r3, [sp, #0]
	adds	r2, r7, #0
	movs	r3, #72
	adds	r5, #1
	bl	sub_08042244
	cmp	r5, #3
	ble.n	.L_08040ad6
	ldr	r0, [pc, #348]
	adds	r1, r7, #0
	movs	r2, #8
	movs	r3, #0
	bl	sub_08042188
	ldr	r0, [pc, #340]
	adds	r1, r7, #0
	movs	r2, #8
	movs	r3, #16
	bl	sub_08042188
	ldr	r0, [pc, #332]
	adds	r1, r7, #0
	movs	r2, #8
	movs	r3, #32
	bl	sub_08042188
	movs	r2, #8
	movs	r3, #48
	ldr	r0, [pc, #320]
	adds	r1, r7, #0
	bl	sub_08042188
	movs	r2, #12
	ldrsh	r1, [r7, r2]
	movs	r3, #14
	ldrsh	r2, [r7, r3]
	mov	r4, r8
	lsls	r3, r4, #4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	movs	r4, #160
	ldr	r3, [sp, #4]
	lsls	r4, r4, #3
	lsls	r1, r1, #3
	adds	r4, #164
	adds	r0, r3, r4
	subs	r1, #4
	adds	r2, #12
	movs	r3, #3
	bl	sub_08108040
.L_08040b42:
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [pc, #272]
	movs	r2, #2
	ldr	r3, [r5, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08040c26
	ldr	r3, [r5, #4]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08040b7a
	mov	r0, r8
	cmp	r0, #3
	bne.n	.L_08040b6e
	mov	r1, fp
	ldr	r0, [r1, #12]
	bl	sub_08013b30
	b.n	.L_08040b7a
.L_08040b6e:
	mov	r2, r8
	lsls	r3, r2, #2
	mov	r4, fp
	ldr	r0, [r4, r3]
	bl	sub_081c0010
.L_08040b7a:
	ldr	r3, [r5, #4]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08040b92
	movs	r0, #195
	lsls	r0, r0, #1
	bl	sub_081c0010
	movs	r0, #0
	bl	sub_081c0010
.L_08040b92:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08040bae
	mov	r0, r8
	lsls	r2, r0, #2
	mov	r1, sl
	ldr	r3, [r1, r2]
	adds	r3, #10
	str	r3, [r1, r2]
	movs	r2, #1
	mov	r9, r2
.L_08040bae:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08040bca
	mov	r3, r8
	lsls	r2, r3, #2
	mov	r4, sl
	ldr	r3, [r4, r2]
	movs	r0, #1
	subs	r3, #10
	str	r3, [r4, r2]
	mov	r9, r0
.L_08040bca:
	ldr	r3, [r5, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08040be4
	mov	r1, r8
	lsls	r2, r1, #2
	mov	r4, sl
	ldr	r3, [r4, r2]
	movs	r0, #1
	adds	r3, #1
	str	r3, [r4, r2]
	mov	r9, r0
.L_08040be4:
	ldr	r3, [r5, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08040bfe
	mov	r1, r8
	lsls	r2, r1, #2
	mov	r4, sl
	ldr	r3, [r4, r2]
	movs	r0, #1
	subs	r3, #1
	str	r3, [r4, r2]
	mov	r9, r0
.L_08040bfe:
	ldr	r3, [r5, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08040c12
	movs	r1, #1
	negs	r1, r1
	movs	r2, #1
	add	r8, r1
	mov	r9, r2
.L_08040c12:
	ldr	r3, [r5, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08040c1e
	b.n	.L_08040a8c
.L_08040c1e:
	movs	r3, #1
	add	r8, r3
	mov	r9, r3
	b.n	.L_08040a8c
.L_08040c26:
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_0803939c
	bl	sub_080409dc
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #0
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0805ea98
	.4byte 0x0805eaa8
	.4byte 0x0805eab0
	.4byte 0x0805eab8
	.4byte 0x0805eabc
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r0, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #164
	adds	r0, r0, r3
	bl	sub_08108030
	pop	{pc}
.L_08040c78:
	push	{lr}
	movs	r1, #197
	lsls	r1, r1, #3
	movs	r0, #208
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #0
	adds	r1, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r2, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl	sub_080145a8
	add	sp, #4
	pop	{pc}
	.4byte 0x8500018a
	.2byte 0x0c61
	.2byte 0x0804
.L_08040cb0:
	push	{lr}
	ldr	r0, [pc, #12]
	bl	sub_08014644
	movs	r0, #208
	bl	sub_0801314c
	pop	{pc}
	.2byte 0x0c61
	.2byte 0x0804
.L_08040cc4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r3, [r3, #0]
	sub	sp, #4
	mov	r8, r3
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #6
	movs	r1, #2
	movs	r2, #18
	movs	r3, #16
	bl	sub_08039260
	movs	r5, #3
	adds	r7, r0, #0
	movs	r6, #3
.L_08040cec:
	adds	r2, r5, #0
	adds	r0, r7, #0
	movs	r1, #0
	movs	r3, #17
	subs	r6, #1
	str	r5, [sp, #0]
	bl	sub_08041c54
	adds	r5, #3
	cmp	r6, #0
	bge.n	.L_08040cec
	ldr	r5, [pc, #104]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #4
	bl	sub_08041fa4
	adds	r0, r5, #1
	adds	r1, r7, #0
	movs	r2, #8
	movs	r3, #28
	bl	sub_08041fa4
	adds	r0, r5, #2
	adds	r1, r7, #0
	movs	r2, #8
	movs	r3, #52
	bl	sub_08041fa4
	adds	r0, r5, #3
	adds	r1, r7, #0
	movs	r2, #8
	movs	r3, #76
	adds	r5, #4
	bl	sub_08041fa4
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #100
	bl	sub_08041fa4
	bl	sub_08044460
	movs	r1, #128
	movs	r6, #0
	lsls	r1, r1, #23
	adds	r2, r7, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl	sub_08042314
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #164
	add	r3, r8
	str	r0, [r3, #0]
	add	sp, #4
	adds	r0, r7, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x115b
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	bl	.L_08040c78
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r3, [r3, #0]
	movs	r0, #1
	mov	sl, r3
	bl	sub_08013560
	bl	.L_08040cc4
	movs	r7, #1
	negs	r7, r7
	mov	r8, r0
.L_08040d98:
	cmp	r7, #0
	beq.n	.L_08040de8
	adds	r0, r5, #5
	movs	r1, #5
	bl	sub_08002064
	mov	r1, r8
	adds	r5, r0, #0
	movs	r0, #12
	ldrsh	r3, [r1, r0]
	mov	r0, r8
	lsls	r3, r3, #3
	subs	r1, r3, #4
	movs	r3, #14
	ldrsh	r2, [r0, r3]
	lsls	r3, r5, #1
	adds	r3, r3, r5
	adds	r3, r3, r2
	lsls	r3, r3, #3
	adds	r2, r3, #0
	movs	r3, #1
	negs	r3, r3
	adds	r2, #16
	cmp	r7, r3
	bne.n	.L_08040dd8
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #164
	add	r0, sl
	bl	sub_08108048
	b.n	.L_08040de6
.L_08040dd8:
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #164
	add	r0, sl
	movs	r3, #3
	bl	sub_08108040
.L_08040de6:
	movs	r7, #0
.L_08040de8:
	movs	r0, #1
	bl	sub_08013560
	ldr	r6, [pc, #112]
	movs	r2, #2
	ldr	r3, [r6, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08040e06
	movs	r0, #113
	movs	r5, #1
	bl	sub_081c0010
	negs	r5, r5
	b.n	.L_08040e42
.L_08040e06:
	ldr	r3, [r6, #4]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08040e18
	movs	r0, #112
	bl	sub_081c0010
	b.n	.L_08040e42
.L_08040e18:
	ldr	r3, [r6, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08040e2c
	movs	r0, #111
	subs	r5, #1
	movs	r7, #1
	bl	sub_081c0010
.L_08040e2c:
	ldr	r3, [r6, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08040d98
	movs	r0, #111
	adds	r5, #1
	movs	r7, #1
	bl	sub_081c0010
	b.n	.L_08040d98
.L_08040e42:
	mov	r0, r8
	movs	r1, #2
	bl	sub_0803939c
	bl	.L_08040cb0
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r5, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	movs	r1, #243
	movs	r0, #4
	bl	sub_080ad020
	movs	r1, #244
	movs	r0, #4
	bl	sub_080ad020
	ldr	r0, [pc, #8]
	movs	r1, #4
	bl	sub_080c8268
	pop	{pc}
	.2byte 0x0101
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl	sub_08016cfc
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl	sub_08016d18
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #170
	bl	sub_08016cfc
	movs	r1, #202
	adds	r1, #255
	movs	r0, #4
	bl	sub_080ad020
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #66
	adds	r3, r3, r2
	movs	r2, #141
	strh	r2, [r3, #0]
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl	sub_080c8268
	pop	{pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x005f
	.2byte 0x0000
	bx	lr
	.2byte 0x0000
