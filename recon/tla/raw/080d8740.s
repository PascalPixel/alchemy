.syntax unified
	.thumb
	.set sub_08002096, 0x08002096
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_0801401c, 0x0801401c
	.set sub_080140d8, 0x080140d8
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_080148e8, 0x080148e8
	.set sub_08014d00, 0x08014d00
	.set sub_08014d78, 0x08014d78
	.set sub_0801587c, 0x0801587c
	.set sub_08016ce4, 0x08016ce4
	.set sub_08020090, 0x08020090
	.set sub_080200c0, 0x080200c0
	.set sub_080200c8, 0x080200c8
	.set sub_08020218, 0x08020218
	.set sub_08020278, 0x08020278
	.set sub_08020280, 0x08020280
	.set sub_080cd91c, 0x080cd91c
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d32c8, 0x080d32c8
	.set sub_080d332c, 0x080d332c
	.set sub_080d3838, 0x080d3838
	.set sub_080d7a78, 0x080d7a78
	.set sub_080d7ab4, 0x080d7ab4
	.set sub_080d8fa8, 0x080d8fa8
	.set sub_080d99b8, 0x080d99b8
	.set sub_080dc0d8, 0x080dc0d8
	.set sub_080dc390, 0x080dc390
	.set sub_080ebea8, 0x080ebea8
	.set sub_080ebeb4, 0x080ebeb4
	.set sub_080ebec8, 0x080ebec8
	.set sub_08108058, 0x08108058
	.set sub_08108060, 0x08108060
	.set sub_081c0010, 0x081c0010
	.global Func_080d8740
	.thumb_func
Func_080d8740:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #52
	adds	r5, r0, #0
	bl	sub_080d2d84
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_080d875e
	b.n	.L_080d8960
.L_080d875e:
	bl	sub_080d7a78
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r0, #129
	lsls	r0, r0, #14
	adds	r0, #132
	str	r3, [sp, #4]
	bl	sub_08108058
	movs	r0, #30
	bl	sub_08013560
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #91
	strb	r3, [r2, #0]
	movs	r0, #173
	bl	sub_081c0010
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_080d332c
	movs	r0, #175
	bl	sub_081c0010
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_080d332c
	movs	r0, #20
	bl	sub_08013560
	movs	r0, #152
	bl	sub_081c0010
	movs	r1, #3
	movs	r2, #14
	adds	r0, r5, #0
	bl	sub_080d32c8
	movs	r0, #152
	bl	sub_081c0010
	movs	r1, #5
	movs	r2, #16
	adds	r0, r5, #0
	bl	sub_080d32c8
	movs	r0, #152
	bl	sub_081c0010
	movs	r1, #7
	movs	r2, #18
	adds	r0, r5, #0
	bl	sub_080d32c8
	movs	r0, #20
	bl	sub_08013560
	ldr	r3, [r6, #80]
	mov	r8, r6
	ldr	r3, [r3, #40]
	movs	r7, #7
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	movs	r3, #0
	mov	r9, r1
	add	r1, sp, #20
	str	r1, [sp, #0]
	mov	sl, r3
	mov	fp, r1
.L_080d87f4:
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #8]
	mov	r0, r9
	bl	sub_080200c0
	ldr	r3, [sp, #0]
	adds	r5, r0, #0
	stmia	r3!, {r5}
	adds	r2, r3, #0
	str	r2, [sp, #0]
	cmp	r5, #0
	beq.n	.L_080d885e
	movs	r3, #240
	lsls	r3, r3, #8
	adds	r2, r5, #0
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	subs	r2, #50
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #90
	ldrb	r3, [r1, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	ldr	r3, [pc, #316]
	movs	r1, #9
	str	r3, [r5, #108]
	ldrh	r3, [r6, #6]
	strh	r3, [r5, #6]
	bl	sub_08020278
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08020090
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08020218
	mov	r1, sl
	ldr	r0, [r5, #80]
	bl	sub_080dc0d8
	mov	r1, r8
	str	r1, [r5, #104]
	mov	sl, r0
	mov	r8, r5
.L_080d885e:
	subs	r7, #1
	cmp	r7, #0
	bge.n	.L_080d87f4
	mov	r2, sl
	ldrb	r2, [r2, #16]
	movs	r0, #153
	mov	r8, r2
	bl	sub_081c0010
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r7, #14
.L_080d887a:
	ldr	r3, [r6, #12]
	movs	r1, #128
	lsls	r1, r1, #12
	adds	r3, r3, r1
	str	r3, [r6, #12]
	movs	r0, #1
	subs	r7, #1
	bl	sub_08013560
	cmp	r7, #0
	bge.n	.L_080d887a
	adds	r0, r6, #0
	bl	sub_080200c8
	mov	r5, fp
	movs	r7, #7
.L_080d889a:
	ldmia	r5!, {r0}
	subs	r7, #1
	bl	sub_080200c8
	cmp	r7, #0
	bge.n	.L_080d889a
	mov	r2, r8
	cmp	r2, #96
	beq.n	.L_080d88b2
	mov	r0, r8
	bl	sub_08014274
.L_080d88b2:
	movs	r0, #10
	bl	sub_08013560
	ldr	r5, [pc, #184]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	ldr	r0, [r5, #0]
	bl	sub_080d3838
	movs	r0, #20
	bl	sub_08013560
	ldr	r0, [r5, #0]
	bl	sub_080d2d84
	movs	r1, #28
	bl	sub_08020090
	movs	r0, #20
	bl	sub_08013560
	ldr	r3, [r6, #8]
	add	r5, sp, #8
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r6, #12]
	movs	r7, #23
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_080dc390
	ldr	r6, [sp, #4]
	adds	r6, #80
.L_080d88fe:
	movs	r1, #46
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #8]
	adds	r0, r6, #0
	adds	r1, #255
	bl	sub_080ebec8
	adds	r0, r6, #0
	ldr	r1, [pc, #104]
	bl	sub_080ebeb4
	adds	r0, r6, #0
	movs	r1, #7
	bl	sub_080ebea8
	ldr	r0, [r6, #0]
	movs	r1, #9
	bl	sub_08020280
	subs	r7, #1
	movs	r0, #1
	bl	sub_08013560
	adds	r6, #72
	cmp	r7, #0
	bge.n	.L_080d88fe
	movs	r0, #120
	bl	sub_08013560
	ldr	r2, [sp, #4]
	movs	r1, #2
	adds	r2, #144
	movs	r7, #23
.L_080d8940:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_080d894a
	strb	r1, [r2, #0]
.L_080d894a:
	subs	r7, #1
	adds	r2, #72
	cmp	r7, #0
	bge.n	.L_080d8940
	movs	r0, #50
	bl	sub_08013560
	bl	sub_08108060
	bl	sub_080d7ab4
.L_080d8960:
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080d870d
	.4byte 0x02000240
	.2byte 0x85b9
	.2byte 0x080d
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r1, #136
	lsls	r1, r1, #5
	adds	r1, #12
	movs	r0, #156
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r8, r3
	adds	r5, r0, #0
	mov	r0, r8
	bl	sub_08014d78
	movs	r3, #0
	adds	r6, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r5, #0
	ldr	r2, [pc, #64]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_080143ac
	adds	r1, r6, #0
	str	r0, [r5, #8]
	ldr	r0, [pc, #52]
	bl	sub_0801587c
	adds	r2, r6, #0
	mov	r1, r8
	ldr	r0, [r5, #8]
	bl	sub_080142d4
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #36]
	bl	sub_080145a8
	movs	r1, #228
	lsls	r1, r1, #2
	adds	r1, #255
	ldr	r0, [pc, #28]
	bl	sub_080145a8
	adds	r0, r6, #0
	bl	sub_08013164
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x85000443
	.4byte 0x080f0c04
	.4byte 0x080d8a01
	.2byte 0x8c11
	.2byte 0x080d
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r3, r1, #0
	adds	r3, #156
	ldr	r3, [r3, #0]
	sub	sp, #44
	adds	r0, r3, #0
	movs	r2, #0
	adds	r0, #12
	str	r0, [sp, #32]
	str	r2, [sp, #24]
	mov	r8, r2
	ldr	r3, [r3, #8]
	ldr	r2, [pc, #456]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	movs	r4, #0
	lsrs	r3, r3, #5
	str	r3, [sp, #20]
	movs	r0, #0
	ldr	r1, [r1, #32]
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r3, [r2, #0]
	str	r3, [sp, #16]
	ldr	r5, [sp, #16]
	ldr	r3, [pc, #432]
	ands	r5, r3
	str	r5, [sp, #16]
	ldr	r2, [r2, #4]
	ands	r2, r3
	str	r2, [sp, #12]
	ldr	r3, [r1, #0]
	movs	r1, #7
	ldr	r3, [r3, #4]
	str	r1, [sp, #28]
	str	r3, [sp, #8]
.L_080d8a5a:
	ldr	r2, [sp, #32]
	ldr	r3, [r2, #28]
	adds	r5, r3, #0
	cmp	r3, #0
	bne.n	.L_080d8a66
	b.n	.L_080d8bc2
.L_080d8a66:
	mov	r3, sp
	adds	r3, #36
	str	r3, [sp, #4]
.L_080d8a6c:
	ldr	r1, [sp, #32]
	ldr	r6, [r5, #8]
	movs	r2, #6
	ldrsh	r1, [r1, r2]
	ldr	r3, [r5, #12]
	mov	sl, r1
	ldr	r2, [r5, #4]
	ldr	r1, [pc, #384]
	mov	r9, r2
	mov	fp, r3
	adds	r6, r6, r1
	cmp	r0, #0
	bne.n	.L_080d8a88
	b.n	.L_080d8bac
.L_080d8a88:
	ldr	r3, [sp, #24]
	str	r4, [sp, #0]
	subs	r0, r3, r2
	mov	r2, fp
	subs	r1, r2, r4
	mov	r2, r8
	subs	r3, r2, r6
	adds	r1, r1, r3
	bl	sub_080148e8
	movs	r3, #128
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	lsls	r3, r3, #7
	adds	r7, r0, r3
	ldr	r3, [sp, #24]
	ldr	r1, [sp, #16]
	add	r3, r9
	lsrs	r2, r3, #31
	movs	r0, #255
	adds	r3, r3, r2
	lsls	r0, r0, #8
	adds	r0, #255
	mov	r2, r8
	asrs	r3, r3, #1
	mov	ip, r0
	ands	r7, r0
	subs	r0, r3, r1
	adds	r3, r2, r6
	lsrs	r2, r3, #31
	ldr	r4, [sp, #0]
	ldr	r1, [sp, #8]
	adds	r3, r3, r2
	asrs	r3, r3, #1
	mov	r2, fp
	subs	r1, r3, r1
	adds	r3, r4, r2
	mov	r8, r1
	lsrs	r2, r3, #31
	ldr	r1, [sp, #12]
	adds	r3, r3, r2
	asrs	r3, r3, #1
	ldr	r2, [sp, #8]
	subs	r3, r3, r1
	ldr	r1, [pc, #284]
	subs	r4, r3, r2
	mov	r3, r8
	subs	r2, r4, r3
	adds	r3, r0, r1
	ldr	r1, [pc, #280]
	cmp	r3, r1
	bhi.n	.L_080d8bac
	ldr	r3, [pc, #276]
	cmp	r2, r3
	ble.n	.L_080d8bac
	ldr	r1, [pc, #276]
	cmp	r2, r1
	bgt.n	.L_080d8bac
	movs	r3, #128
	asrs	r1, r0, #16
	lsls	r3, r3, #1
	adds	r3, #255
	subs	r1, #4
	asrs	r2, r2, #16
	ands	r1, r3
	mov	r0, r8
	movs	r3, #255
	subs	r2, #4
	ands	r2, r3
	adds	r3, r0, r4
	asrs	r3, r3, #16
	adds	r3, #58
	adds	r4, r5, #0
	mov	r8, r3
	adds	r4, #20
	movs	r3, #0
	str	r3, [r4, #0]
	lsls	r1, r1, #16
	movs	r3, #128
	orrs	r2, r1
	lsls	r3, r3, #6
	orrs	r2, r3
	str	r2, [r5, #24]
	ldr	r1, [sp, #20]
	movs	r3, #128
	lsls	r3, r3, #4
	orrs	r3, r1
	str	r3, [r5, #28]
	cmp	r7, #0
	beq.n	.L_080d8b8c
	ldr	r0, [sp, #4]
	ldr	r2, [pc, #184]
	ldr	r3, [r0, #4]
	mov	r1, ip
	ands	r3, r2
	orrs	r3, r7
	str	r3, [r0, #4]
	ldr	r3, [sp, #36]
	movs	r0, #4
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #1
	orrs	r3, r2
	movs	r2, #128
	ands	r3, r1
	lsls	r2, r2, #17
	orrs	r3, r2
	str	r3, [sp, #36]
	ldrb	r3, [r5, #25]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r5, #25]
	ldr	r0, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_0801401c
	movs	r3, #31
	ands	r0, r3
	movs	r1, #63
	ldrb	r3, [r5, #27]
	negs	r1, r1
	adds	r2, r1, #0
	lsls	r0, r0, #1
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r5, #27]
	ldr	r4, [sp, #0]
.L_080d8b8c:
	movs	r3, #3
	mov	r2, sl
	ands	r2, r3
	movs	r0, #13
	ldrb	r3, [r5, #29]
	negs	r0, r0
	lsls	r1, r2, #2
	adds	r2, r0, #0
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r5, #29]
	strh	r7, [r5, #16]
	adds	r0, r4, #0
	mov	r1, r8
	bl	sub_080140d8
.L_080d8bac:
	mov	r1, r9
	str	r1, [sp, #24]
	mov	r8, r6
	ldr	r5, [r5, #0]
	mov	r4, fp
	movs	r0, #1
	cmp	r5, #0
	beq.n	.L_080d8bbe
	b.n	.L_080d8a6c
.L_080d8bbe:
	ldr	r2, [sp, #32]
	ldr	r3, [r2, #28]
.L_080d8bc2:
	movs	r5, #0
	str	r5, [sp, #24]
	mov	r8, r5
	movs	r4, #0
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080d8bd2
	strh	r7, [r3, #16]
.L_080d8bd2:
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #32]
	subs	r1, #1
	adds	r2, #32
	str	r1, [sp, #28]
	str	r2, [sp, #32]
	cmp	r1, #0
	blt.n	.L_080d8be4
	b.n	.L_080d8a5a
.L_080d8be4:
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x020036e0
	.4byte 0xffff0000
	.4byte 0xfffc0000
	.4byte 0x001fffff
	.4byte 0x012ffffe
	.4byte 0xffe00000
	.2byte 0xffff
	.2byte 0x00df
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r0, #130
	adds	r3, #156
	lsls	r0, r0, #1
	mov	r8, r1
	ldr	r6, [r3, #0]
	movs	r7, #0
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080d8cd2
	movs	r3, #194
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #12
	ble.n	.L_080d8cd2
	ldr	r2, [pc, #248]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #18
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080d8cbc
	adds	r1, #2
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
	bl	sub_080cd91c
	movs	r2, #1
	adds	r5, r6, #0
	negs	r2, r2
	adds	r5, #12
	cmp	r0, r2
	beq.n	.L_080d8cd6
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	cmp	r3, r0
	beq.n	.L_080d8c90
	movs	r2, #2
	ldrsh	r3, [r5, r2]
	cmp	r3, r0
	beq.n	.L_080d8c94
	adds	r2, r5, #0
.L_080d8c76:
	adds	r7, #1
	adds	r2, #32
	cmp	r7, #7
	bgt.n	.L_080d8cd6
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, r0
	beq.n	.L_080d8c94
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	cmp	r3, r0
	bne.n	.L_080d8c76
	b.n	.L_080d8c94
.L_080d8c90:
	adds	r5, r6, #0
	adds	r5, #12
.L_080d8c94:
	cmp	r7, #7
	bgt.n	.L_080d8cd6
	lsls	r3, r7, #5
	adds	r3, #12
	ldrsh	r0, [r6, r3]
	adds	r3, r6, r3
	movs	r2, #2
	ldrsh	r1, [r3, r2]
	bl	sub_080d99b8
	cmp	r0, #0
	beq.n	.L_080d8cd6
	movs	r2, #181
	movs	r3, #252
	lsls	r2, r2, #1
	lsls	r3, r3, #8
	add	r2, r8
	adds	r3, #133
	strh	r3, [r2, #0]
	b.n	.L_080d8cd6
.L_080d8cbc:
	cmp	r3, #3
	bne.n	.L_080d8cd2
	movs	r2, #181
	movs	r3, #252
	lsls	r2, r2, #1
	lsls	r3, r3, #8
	add	r2, r8
	adds	r3, #134
	adds	r5, r6, #0
	strh	r3, [r2, #0]
	b.n	.L_080d8cd4
.L_080d8cd2:
	adds	r5, r6, #0
.L_080d8cd4:
	adds	r5, #12
.L_080d8cd6:
	ldr	r3, [pc, #100]
	movs	r7, #0
	mov	r8, r3
.L_080d8cdc:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq.n	.L_080d8d28
	ldr	r0, [r5, #20]
	bl	sub_08002096
	ldr	r1, [r5, #12]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x696b
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r3, r3, r1
	movs	r1, #243
	lsls	r1, r1, #8
	adds	r6, r0, #0
	str	r3, [r5, #20]
	ldr	r0, [r5, #12]
	adds	r1, #51
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x22c8
	lsls	r2, r2, #5
	adds	r2, #152
	str	r0, [r5, #12]
	cmp	r0, r2
	bgt.n	.L_080d8d18
	movs	r3, #0
	str	r3, [r5, #12]
	str	r3, [r5, #24]
.L_080d8d18:
	movs	r3, #0
	ldrsh	r1, [r5, r3]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	adds	r0, r7, #0
	adds	r3, r6, #0
	bl	sub_080d8fa8
.L_080d8d28:
	adds	r7, #1
	adds	r5, #32
	cmp	r7, #7
	ble.n	.L_080d8cdc
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r3, [r3, #0]
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r0, r3, r2
	movs	r2, #0
.L_080d8d52:
	movs	r3, #18
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L_080d8d64
	adds	r2, #1
	adds	r0, #32
	cmp	r2, #127
	ble.n	.L_080d8d52
	movs	r0, #0
.L_080d8d64:
	pop	{pc}
	.align 2, 0
