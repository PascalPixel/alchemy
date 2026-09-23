.syntax unified
	.thumb
	.set sub_08002074, 0x08002074
	.set sub_08002080, 0x08002080
	.set sub_0801314c, 0x0801314c
	.set sub_08014cc0, 0x08014cc0
	.set sub_08016ca4, 0x08016ca4
	.set sub_0803c274, 0x0803c274
	.set sub_0803ca20, 0x0803ca20
	.set sub_0803cca8, 0x0803cca8
	.set sub_0803cd08, 0x0803cd08
	.set sub_0803d178, 0x0803d178
	.set sub_080c8648, 0x080c8648
	.global Func_0803acd4
	.thumb_func
Func_0803acd4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r4, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	adds	r7, r2, #0
	mov	r8, r3
	lsls	r3, r7, #16
	asrs	r3, r3, #16
	adds	r6, r0, #0
	sub	sp, #8
	mov	r9, r1
	mov	sl, r3
	cmp	r6, #0
	bne.n	.L_0803ad1e
	movs	r1, #152
	lsls	r1, r1, #5
	adds	r1, #66
	add	r1, r8
	ldrh	r3, [r1, #0]
	movs	r2, #244
	lsls	r2, r2, #4
	mov	r0, r8
	lsls	r3, r3, #1
	adds	r3, r3, r2
	adds	r6, r0, r2
	ldr	r2, [pc, #36]
	strh	r2, [r0, r3]
	ldr	r2, [pc, #36]
	ldrh	r3, [r1, #0]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r1, #0]
.L_0803ad1e:
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	adds	r6, #2
	lsls	r1, r3, #16
	cmp	r1, #0
	beq.n	.L_0803adfe
.L_0803ad2a:
	lsrs	r5, r1, #16
	cmp	r5, #30
	bls.n	.L_0803ad40
	cmp	r5, #176
	bne.n	.L_0803adc8
	b.n	.L_0803ad40
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0x01ff
	.2byte 0x0000
.L_0803ad40:
	subs	r1, r5, #3
	cmp	r1, #26
	bhi.n	.L_0803adf0
	ldr	r2, [pc, #200]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0803adbc
	.4byte 0x0803adf0
	.4byte 0x0803adf0
	.4byte 0x0803adf0
	.4byte 0x0803adc4
	.4byte 0x0803adc4
	.4byte 0x0803adc4
	.4byte 0x0803adc4
	.4byte 0x0803adc4
	.4byte 0x0803adc4
	.4byte 0x0803adf0
	.4byte 0x0803adc2
	.4byte 0x0803adc2
	.4byte 0x0803adf0
	.4byte 0x0803adc4
	.4byte 0x0803adf0
	.4byte 0x0803adf0
	.4byte 0x0803adf0
	.4byte 0x0803adf0
	.4byte 0x0803adf0
	.4byte 0x0803adf0
	.4byte 0x0803adf0
	.4byte 0x0803adf0
	.4byte 0x0803adf0
	.4byte 0x0803adf0
	.4byte 0x0803adc2
	.2byte 0xadc4
	.2byte 0x0803
	mov	r7, sl
	adds	r4, #1
	b.n	.L_0803adf0
	adds	r6, #2
	adds	r6, #2
	b.n	.L_0803adf0
.L_0803adc8:
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r2, r7, #0
	adds	r3, r4, #0
	mov	r0, r9
	adds	r1, r5, #0
	str	r4, [sp, #4]
	bl	sub_0803c274
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #34
	adds	r3, r5, r0
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	ldr	r4, [sp, #4]
	cmp	r3, r2
	bls.n	.L_0803adf0
	adds	r7, #1
.L_0803adf0:
	movs	r0, #0
	ldrsh	r3, [r6, r0]
	adds	r6, #2
	lsls	r3, r3, #16
	adds	r1, r3, #0
	cmp	r3, #0
	bne.n	.L_0803ad2a
.L_0803adfe:
	movs	r3, #1
	mov	r2, r8
	strb	r3, [r2, #3]
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0xad50
	.2byte 0x0803
.L_0803ae14:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #0
	adds	r6, r1, #0
	adds	r7, r0, #0
	mov	sl, r2
	mov	fp, r3
	cmp	r6, #0
	bge.n	.L_0803ae3a
	cmp	r2, #0
	bne.n	.L_0803ae38
	movs	r3, #1
	mov	fp, r3
.L_0803ae38:
	negs	r6, r6
.L_0803ae3a:
	movs	r3, #32
	strb	r3, [r7, #0]
	ldr	r3, [pc, #148]
	adds	r5, r7, #0
	mov	r8, r3
	adds	r5, #12
	mov	r9, r7
.L_0803ae48:
	movs	r2, #10
	adds	r0, r6, #0
	mov	r1, r8
	negs	r2, r2
	bl	sub_08002080
	adds	r0, #48
	strb	r0, [r5, #0]
	mov	r1, r8
	adds	r0, r6, #0
	bl	sub_08002074
	subs	r5, #1
	adds	r6, r0, #0
	cmp	r5, r9
	bne.n	.L_0803ae48
	movs	r0, #0
	strb	r0, [r7, #13]
	movs	r1, #32
	movs	r0, #1
	movs	r4, #45
	adds	r2, r7, #0
	b.n	.L_0803ae7a
.L_0803ae76:
	adds	r2, #1
	adds	r0, #1
.L_0803ae7a:
	cmp	r0, #13
	beq.n	.L_0803ae94
	ldrb	r3, [r2, #1]
	cmp	r3, #48
	bne.n	.L_0803ae8c
	cmp	r0, #12
	beq.n	.L_0803ae76
	strb	r1, [r2, #1]
	b.n	.L_0803ae76
.L_0803ae8c:
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_0803ae94
	strb	r4, [r2, #0]
.L_0803ae94:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_0803aeb6
	ldrb	r3, [r7, #0]
	movs	r0, #0
	cmp	r3, #32
	bne.n	.L_0803aeb2
	adds	r2, r7, #0
.L_0803aea4:
	adds	r0, #1
	cmp	r0, #12
	beq.n	.L_0803aeb2
	adds	r2, #1
	ldrb	r3, [r2, #0]
	cmp	r3, #32
	beq.n	.L_0803aea4
.L_0803aeb2:
	adds	r0, r7, r0
	b.n	.L_0803aec6
.L_0803aeb6:
	mov	r3, sl
	cmp	r3, #12
	bls.n	.L_0803aec0
	movs	r3, #12
	mov	sl, r3
.L_0803aec0:
	mov	r3, sl
	subs	r0, r7, r3
	adds	r0, #13
.L_0803aec6:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x999a
	.2byte 0x1999
.L_0803aed8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #32
	mov	sl, r0
	adds	r5, r1, #0
	adds	r0, r2, #0
	ldr	r1, [sp, #60]
	ldr	r2, [sp, #68]
	adds	r7, r3, #0
	mov	r3, sl
	mov	r8, r1
	mov	lr, r2
	cmp	r3, #0
	beq.n	.L_0803af28
	ldr	r3, [pc, #36]
	lsls	r2, r0, #1
	strh	r3, [r2, r7]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r0, #1
	ands	r0, r2
	ldr	r1, [pc, #24]
	lsls	r3, r0, #1
	adds	r0, #1
	ands	r0, r2
	strh	r1, [r3, r7]
	lsls	r3, r0, #1
	strh	r1, [r3, r7]
	adds	r0, #1
	ands	r0, r2
	b.n	.L_0803af28
	movs	r0, r0
	.4byte 0x00000020
	.2byte 0x000a
	.2byte 0x0000
.L_0803af28:
	mov	r4, r8
	cmp	r4, #1
	beq.n	.L_0803af38
	cmp	r4, #3
	bne.n	.L_0803afbe
	ldr	r6, [sp, #64]
	cmp	r6, #0
	bne.n	.L_0803afbe
.L_0803af38:
	ldr	r3, [pc, #192]
	mov	r9, sp
	movs	r1, #0
	mov	r2, r9
	mov	ip, r1
	ldmia	r3!, {r1, r4, r6}
	stmia	r2!, {r1, r4, r6}
	ldmia	r3!, {r1, r4, r6}
	stmia	r2!, {r1, r4, r6}
	ldmia	r3!, {r4, r6}
	stmia	r2!, {r4, r6}
	ldrh	r2, [r5, #0]
	cmp	r2, #29
	bne.n	.L_0803af5c
	ldrh	r3, [r5, #2]
	adds	r5, #4
	subs	r3, #1
	mov	ip, r3
.L_0803af5c:
	mov	r1, ip
	cmp	r1, #0
	bne.n	.L_0803af7e
	cmp	r2, #65
	beq.n	.L_0803af7a
	cmp	r2, #73
	beq.n	.L_0803af7a
	cmp	r2, #85
	beq.n	.L_0803af7a
	cmp	r2, #69
	beq.n	.L_0803af7a
	movs	r3, #1
	mov	ip, r3
	cmp	r2, #79
	bne.n	.L_0803af7e
.L_0803af7a:
	movs	r4, #2
	mov	ip, r4
.L_0803af7e:
	movs	r3, #7
	mov	r6, ip
	ands	r6, r3
	lsls	r3, r6, #2
	mov	r1, r9
	ldr	r4, [r1, r3]
	movs	r6, #0
	ldrb	r3, [r4, #0]
	adds	r4, #1
	lsls	r1, r3, #24
	cmp	r1, #0
	beq.n	.L_0803afc8
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	mov	ip, r2
.L_0803af9e:
	lsls	r2, r0, #1
	asrs	r3, r1, #24
	strh	r3, [r2, r7]
	adds	r0, #1
	mov	r3, ip
	adds	r6, #1
	ands	r0, r3
	cmp	r6, #7
	bgt.n	.L_0803afc8
	ldrb	r3, [r4, #0]
	adds	r4, #1
	lsls	r3, r3, #24
	adds	r1, r3, #0
	cmp	r3, #0
	bne.n	.L_0803af9e
	b.n	.L_0803afc8
.L_0803afbe:
	ldrh	r3, [r5, #0]
	ldrh	r2, [r5, #0]
	cmp	r3, #29
	bne.n	.L_0803afca
	adds	r5, #4
.L_0803afc8:
	ldrh	r2, [r5, #0]
.L_0803afca:
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0803b00c
	movs	r6, #128
	lsls	r6, r6, #1
	adds	r6, #255
	movs	r4, #1
	movs	r1, #0
.L_0803afda:
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	lsls	r3, r0, #1
	strh	r2, [r3, r7]
	lsls	r2, r2, #16
	adds	r0, #1
	lsrs	r2, r2, #16
	adds	r5, #2
	ands	r0, r6
	cmp	r2, #83
	beq.n	.L_0803aff4
	cmp	r2, #115
	bne.n	.L_0803b000
.L_0803aff4:
	mov	r2, lr
	str	r4, [r2, #0]
	b.n	.L_0803b004
	movs	r0, r0
	.2byte 0xc10c
	.2byte 0x0805
.L_0803b000:
	mov	r3, lr
	str	r1, [r3, #0]
.L_0803b004:
	ldrh	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_0803afda
.L_0803b00c:
	mov	r4, r8
	cmp	r4, #2
	beq.n	.L_0803b01c
	cmp	r4, #3
	bne.n	.L_0803b044
	ldr	r6, [sp, #64]
	cmp	r6, #0
	beq.n	.L_0803b044
.L_0803b01c:
	mov	r1, lr
	ldr	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_0803b034
	ldr	r2, [pc, #48]
	lsls	r3, r0, #1
	strh	r2, [r3, r7]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r0, #1
	adds	r3, #255
	ands	r0, r3
.L_0803b034:
	ldr	r2, [pc, #36]
	lsls	r3, r0, #1
	strh	r2, [r3, r7]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r0, #1
	adds	r3, #255
	ands	r0, r3
.L_0803b044:
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L_0803b088
	ldr	r3, [pc, #20]
	movs	r1, #128
	lsls	r2, r0, #1
	lsls	r1, r1, #1
	strh	r3, [r2, r7]
	b.n	.L_0803b064
	movs	r0, r0
	.4byte 0x00000065
	.4byte 0x00000073
	.2byte 0x000a
	.2byte 0x0000
.L_0803b064:
	adds	r1, #255
	ldr	r3, [pc, #24]
	adds	r0, #1
	ands	r0, r1
	lsls	r2, r0, #1
	strh	r3, [r2, r7]
	adds	r0, #1
	ldr	r3, [pc, #16]
	ands	r0, r1
	lsls	r2, r0, #1
	strh	r3, [r2, r7]
	adds	r0, #1
	ands	r0, r1
	b.n	.L_0803b088
	.4byte 0x00000008
	.2byte 0x0020
	.2byte 0x0000
.L_0803b088:
	add	sp, #32
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #132
	str	r0, [sp, #48]
	movs	r0, #192
	lsls	r0, r0, #18
	mov	fp, r1
	ldr	r1, [r0, #60]
	movs	r2, #1
	movs	r3, #0
	movs	r5, #152
	str	r1, [sp, #44]
	str	r2, [sp, #40]
	str	r3, [sp, #36]
	lsls	r5, r5, #5
	adds	r5, #66
	adds	r3, r1, r5
	ldrh	r3, [r3, #0]
	str	r2, [sp, #20]
	str	r3, [sp, #32]
	adds	r6, r3, #0
	movs	r2, #244
	ldr	r3, [sp, #48]
	lsls	r2, r2, #4
	movs	r5, #1
	mov	sl, r0
	movs	r7, #0
	movs	r0, #0
	adds	r1, r1, r2
	negs	r5, r5
	str	r0, [sp, #28]
	str	r0, [sp, #24]
	str	r7, [sp, #52]
	mov	r8, r1
	str	r0, [sp, #16]
	cmp	r3, r5
	bne.n	.L_0803b0f8
	ldr	r0, [sp, #44]
	movs	r1, #152
	lsls	r1, r1, #5
	adds	r1, #68
	adds	r3, r0, r1
	ldrh	r3, [r3, #0]
	str	r3, [sp, #32]
	b.n	.L_0803b778
.L_0803b0f8:
	ldr	r5, [pc, #228]
	movs	r0, #200
	adds	r1, r5, #0
	bl	sub_08014cc0
	movs	r2, #132
	movs	r3, #128
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r1, r0, #0
	adds	r3, #212
	ldr	r0, [pc, #208]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, sl
	adds	r3, #200
	movs	r2, #56
	ldr	r3, [r3, #0]
	add	r2, sp
	mov	sl, r2
	mov	r0, sl
	ldr	r1, [sp, #48]
	mov	r9, r3
	bl	sub_0803d178
	mov	r3, sp
	adds	r3, #84
	str	r3, [sp, #12]
.L_0803b134:
	mov	r0, sl
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x1c3d
	adds	r7, r0, #0
	cmp	r7, #255
	bls.n	.L_0803b144
	movs	r7, #64
.L_0803b144:
	ldr	r0, [sp, #16]
	cmp	r0, #0
	beq.n	.L_0803b1e8
	cmp	r7, #31
	bls.n	.L_0803b154
	cmp	r7, #176
	beq.n	.L_0803b154
	b.n	.L_0803b71c
.L_0803b154:
	cmp	r7, #18
	beq.n	.L_0803b1ce
	cmp	r7, #18
	bhi.n	.L_0803b17e
	cmp	r7, #9
	bhi.n	.L_0803b172
	cmp	r7, #8
	bcs.n	.L_0803b1ce
	cmp	r7, #1
	beq.n	.L_0803b1d6
	cmp	r7, #1
	bcc.n	.L_0803b1ac
	cmp	r7, #2
	beq.n	.L_0803b1ac
	b.n	.L_0803b71c
.L_0803b172:
	cmp	r7, #16
	bne.n	.L_0803b178
	b.n	.L_0803b71c
.L_0803b178:
	cmp	r7, #17
	beq.n	.L_0803b1ce
	b.n	.L_0803b71c
.L_0803b17e:
	cmp	r7, #22
	beq.n	.L_0803b1b2
	cmp	r7, #22
	bhi.n	.L_0803b198
	cmp	r7, #20
	beq.n	.L_0803b1b6
	cmp	r7, #20
	bhi.n	.L_0803b1c0
	mov	r0, sl
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x2003
	b.n	.L_0803b1c6
.L_0803b198:
	cmp	r7, #29
	beq.n	.L_0803b1ce
	cmp	r7, #29
	bhi.n	.L_0803b1a6
	cmp	r7, #23
	beq.n	.L_0803b1c4
	b.n	.L_0803b71c
.L_0803b1a6:
	cmp	r7, #30
	beq.n	.L_0803b1ac
	b.n	.L_0803b71c
.L_0803b1ac:
	movs	r1, #0
	str	r1, [sp, #20]
	b.n	.L_0803b71c
.L_0803b1b2:
	movs	r0, #5
	b.n	.L_0803b1c6
.L_0803b1b6:
	mov	r0, sl
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x2002
	b.n	.L_0803b1c6
.L_0803b1c0:
	movs	r0, #4
	b.n	.L_0803b1c6
.L_0803b1c4:
	movs	r0, #6
.L_0803b1c6:
	mov	r1, fp
	bl	sub_0803cd08
	b.n	.L_0803b71c
.L_0803b1ce:
	mov	r0, sl
	mov	lr, r9
	.2byte 0xf800
	.2byte 0xe2a2
.L_0803b1d6:
	movs	r2, #0
	str	r2, [sp, #20]
	movs	r7, #2
	b.n	.L_0803b71c
	movs	r0, r0
	.4byte 0x00000144
	.2byte 0x8438
	.2byte 0x0803
.L_0803b1e8:
	ldr	r0, [sp, #44]
	movs	r1, #152
	lsls	r1, r1, #5
	adds	r1, #138
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0803b220
	ldr	r2, [sp, #40]
	cmp	r2, #0
	bne.n	.L_0803b220
	cmp	r7, #222
	beq.n	.L_0803b220
	cmp	r7, #223
	beq.n	.L_0803b220
	ldr	r3, [pc, #20]
	lsls	r2, r6, #1
	mov	r0, r8
	movs	r1, #128
	lsls	r1, r1, #1
	strh	r3, [r2, r0]
	adds	r6, #1
	adds	r1, #255
	ands	r6, r1
	b.n	.L_0803b220
	movs	r0, r0
	.2byte 0x0005
	.2byte 0x0000
.L_0803b220:
	ldr	r2, [sp, #44]
	movs	r0, #152
	lsls	r0, r0, #5
	adds	r0, #139
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0803b27c
	ldr	r1, [sp, #40]
	cmp	r1, #0
	bne.n	.L_0803b27c
	cmp	r7, #222
	beq.n	.L_0803b27c
	cmp	r7, #223
	beq.n	.L_0803b27c
	movs	r2, #128
	lsls	r2, r2, #1
	cmp	r5, r2
	bhi.n	.L_0803b27c
	cmp	r5, #127
	bls.n	.L_0803b27c
	cmp	r5, #222
	beq.n	.L_0803b27c
	cmp	r5, #223
	beq.n	.L_0803b27c
	cmp	r5, #32
	beq.n	.L_0803b27c
	cmp	r5, #165
	beq.n	.L_0803b27c
	cmp	r5, #161
	beq.n	.L_0803b27c
	cmp	r5, #164
	beq.n	.L_0803b27c
	ldr	r3, [pc, #20]
	lsls	r2, r6, #1
	mov	r5, r8
	movs	r0, #128
	lsls	r0, r0, #1
	strh	r3, [r2, r5]
	adds	r6, #1
	adds	r0, #255
	ands	r6, r0
	b.n	.L_0803b27c
	movs	r0, r0
	.2byte 0x00de
	.2byte 0x0000
.L_0803b27c:
	cmp	r7, #31
	bls.n	.L_0803b2fc
	cmp	r7, #176
	beq.n	.L_0803b2fc
	ldr	r1, [sp, #44]
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #138
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0803b2d4
	cmp	r7, #32
	beq.n	.L_0803b29e
	ldr	r3, [sp, #28]
	cmp	r3, #10
	bls.n	.L_0803b2d4
.L_0803b29e:
	movs	r0, #128
	ldr	r2, [pc, #44]
	lsls	r0, r0, #1
	adds	r0, #255
	lsls	r3, r6, #1
	adds	r6, #1
	ands	r6, r0
	mov	r5, r8
	strh	r2, [r3, r5]
	lsls	r3, r6, #1
	adds	r6, #1
	ands	r6, r0
	mov	r1, r8
	strh	r2, [r3, r1]
	lsls	r3, r6, #1
	strh	r2, [r3, r5]
	ldr	r1, [sp, #28]
	adds	r6, #1
	ands	r6, r0
	movs	r0, #1
	str	r0, [sp, #16]
	cmp	r1, #10
	bls.n	.L_0803b2d4
	movs	r7, #32
	b.n	.L_0803b2d4
	.2byte 0x002e
	.2byte 0x0000
.L_0803b2d4:
	cmp	r7, #34
	bne.n	.L_0803b2e6
	ldr	r2, [sp, #36]
	movs	r3, #1
	eors	r2, r3
	str	r2, [sp, #36]
	cmp	r2, #0
	beq.n	.L_0803b2e6
	movs	r7, #142
.L_0803b2e6:
	movs	r0, #128
	lsls	r0, r0, #1
	lsls	r3, r6, #1
	mov	r5, r8
	adds	r6, #1
	adds	r0, #255
	movs	r1, #0
	strh	r7, [r3, r5]
	ands	r6, r0
	str	r1, [sp, #40]
	b.n	.L_0803b71c
.L_0803b2fc:
	cmp	r7, #20
	bne.n	.L_0803b302
	b.n	.L_0803b464
.L_0803b302:
	cmp	r7, #20
	bhi.n	.L_0803b342
	cmp	r7, #9
	bhi.n	.L_0803b324
	cmp	r7, #8
	bcs.n	.L_0803b39c
	cmp	r7, #1
	bne.n	.L_0803b314
	b.n	.L_0803b6f8
.L_0803b314:
	cmp	r7, #1
	bcc.n	.L_0803b396
	cmp	r7, #2
	beq.n	.L_0803b396
	cmp	r7, #3
	bne.n	.L_0803b322
	b.n	.L_0803b6f8
.L_0803b322:
	b.n	.L_0803b6fc
.L_0803b324:
	cmp	r7, #17
	bne.n	.L_0803b32a
	b.n	.L_0803b57c
.L_0803b32a:
	cmp	r7, #17
	bhi.n	.L_0803b336
	cmp	r7, #16
	bne.n	.L_0803b334
	b.n	.L_0803b51e
.L_0803b334:
	b.n	.L_0803b6fc
.L_0803b336:
	cmp	r7, #18
	bne.n	.L_0803b33c
	b.n	.L_0803b542
.L_0803b33c:
	cmp	r7, #19
	beq.n	.L_0803b432
	b.n	.L_0803b6fc
.L_0803b342:
	cmp	r7, #27
	bne.n	.L_0803b348
	b.n	.L_0803b6c8
.L_0803b348:
	cmp	r7, #27
	bhi.n	.L_0803b370
	cmp	r7, #23
	bne.n	.L_0803b352
	b.n	.L_0803b4dc
.L_0803b352:
	cmp	r7, #23
	bhi.n	.L_0803b362
	cmp	r7, #21
	bne.n	.L_0803b35c
	b.n	.L_0803b49e
.L_0803b35c:
	cmp	r7, #22
	beq.n	.L_0803b3cc
	b.n	.L_0803b6fc
.L_0803b362:
	cmp	r7, #25
	bne.n	.L_0803b368
	b.n	.L_0803b68c
.L_0803b368:
	cmp	r7, #26
	bne.n	.L_0803b36e
	b.n	.L_0803b640
.L_0803b36e:
	b.n	.L_0803b6fc
.L_0803b370:
	cmp	r7, #30
	beq.n	.L_0803b396
	cmp	r7, #30
	bhi.n	.L_0803b384
	cmp	r7, #28
	bne.n	.L_0803b37e
	b.n	.L_0803b5b6
.L_0803b37e:
	cmp	r7, #29
	beq.n	.L_0803b39c
	b.n	.L_0803b6fc
.L_0803b384:
	cmp	r7, #176
	bne.n	.L_0803b38a
	b.n	.L_0803b66c
.L_0803b38a:
	movs	r2, #1
	negs	r2, r2
	cmp	r7, r2
	bne.n	.L_0803b394
	b.n	.L_0803b71c
.L_0803b394:
	b.n	.L_0803b6fc
.L_0803b396:
	movs	r3, #0
	str	r3, [sp, #20]
	b.n	.L_0803b71c
.L_0803b39c:
	movs	r0, #128
	lsls	r0, r0, #1
	lsls	r3, r6, #1
	adds	r0, #255
	mov	r5, r8
	adds	r6, #1
	ands	r6, r0
	strh	r7, [r3, r5]
	mov	r0, sl
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x21ff
	lsls	r1, r1, #8
	adds	r1, #255
	lsls	r3, r6, #1
	adds	r0, r0, r1
	mov	r2, r8
	strh	r0, [r3, r2]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r6, #1
	adds	r3, #255
	ands	r6, r3
	b.n	.L_0803b71c
.L_0803b3cc:
	mov	r1, fp
	movs	r0, #5
	bl	sub_0803cd08
	adds	r1, r0, #0
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_0803b3de
	negs	r3, r1
.L_0803b3de:
	movs	r5, #1
	str	r5, [sp, #24]
	cmp	r3, #1
	bgt.n	.L_0803b3ea
	movs	r0, #0
	str	r0, [sp, #24]
.L_0803b3ea:
	add	r5, sp, #68
	adds	r0, r5, #0
	movs	r2, #0
	bl	.L_0803ae14
	subs	r4, r0, r5
	cmp	r4, #16
	bne.n	.L_0803b3fc
	b.n	.L_0803b71c
.L_0803b3fc:
	ldrb	r3, [r5, r4]
	cmp	r3, #0
	bne.n	.L_0803b404
	b.n	.L_0803b71c
.L_0803b404:
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r0, r4, r5
	mov	ip, r1
	adds	r1, r0, #0
.L_0803b410:
	ldrb	r3, [r1, #0]
	lsls	r2, r6, #1
	mov	r5, r8
	strh	r3, [r2, r5]
	adds	r6, #1
	mov	r2, ip
	adds	r4, #1
	adds	r1, #1
	ands	r6, r2
	cmp	r4, #16
	bne.n	.L_0803b428
	b.n	.L_0803b71c
.L_0803b428:
	adds	r0, #1
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_0803b410
	b.n	.L_0803b71c
.L_0803b432:
	mov	r0, sl
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x4659
	subs	r5, r0, #1
	movs	r0, #3
	bl	sub_0803cd08
	adds	r2, r0, #0
	ldr	r0, [pc, #484]
	ldr	r1, [sp, #12]
	adds	r0, r2, r0
	movs	r2, #24
	bl	sub_0803ca20
	ldr	r3, [sp, #24]
	adds	r2, r6, #0
	str	r3, [sp, #4]
	add	r3, sp, #52
	str	r3, [sp, #8]
	movs	r0, #0
	ldr	r1, [sp, #12]
	mov	r3, r8
	str	r5, [sp, #0]
	b.n	.L_0803b5ae
.L_0803b464:
	mov	r0, sl
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x4659
	subs	r5, r0, #1
	movs	r0, #2
	bl	sub_0803cd08
	adds	r2, r0, #0
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r2, r0
	ldr	r0, [pc, #432]
	ldr	r1, [sp, #12]
	adds	r0, r2, r0
	movs	r2, #24
	bl	sub_0803ca20
	ldr	r1, [sp, #24]
	add	r3, sp, #52
	str	r1, [sp, #4]
	str	r3, [sp, #8]
	adds	r2, r6, #0
	movs	r0, #0
	ldr	r1, [sp, #12]
	mov	r3, r8
	str	r5, [sp, #0]
	b.n	.L_0803b5ae
.L_0803b49e:
	mov	r1, fp
	movs	r0, #4
	bl	sub_0803cd08
	adds	r2, r0, #0
	ldr	r0, [pc, #392]
	ldr	r1, [sp, #12]
	adds	r0, r2, r0
	movs	r2, #24
	bl	sub_0803ca20
	ldr	r1, [sp, #12]
	adds	r0, r6, #0
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0803b5b2
	movs	r4, #128
	lsls	r4, r4, #1
	adds	r4, #255
.L_0803b4c6:
	lsls	r3, r0, #1
	mov	r5, r8
	strh	r2, [r3, r5]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	adds	r0, #1
	adds	r3, r2, #0
	ands	r0, r4
	cmp	r3, #0
	bne.n	.L_0803b4c6
	b.n	.L_0803b5b2
.L_0803b4dc:
	mov	r1, fp
	movs	r0, #6
	bl	sub_0803cd08
	movs	r1, #1
	bl	sub_080c8648
	ldr	r3, [pc, #332]
	ldr	r1, [sp, #12]
	adds	r0, r0, r3
	movs	r2, #24
	bl	sub_0803ca20
	ldr	r1, [sp, #12]
	adds	r0, r6, #0
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0803b5b2
	movs	r4, #128
	lsls	r4, r4, #1
	adds	r4, #255
.L_0803b508:
	lsls	r3, r0, #1
	mov	r5, r8
	strh	r2, [r3, r5]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	adds	r0, #1
	adds	r3, r2, #0
	ands	r0, r4
	cmp	r3, #0
	bne.n	.L_0803b508
	b.n	.L_0803b5b2
.L_0803b51e:
	ldr	r3, [pc, #284]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	sub_08016ca4
	add	r1, sp, #84
	adds	r2, r1, #0
	movs	r4, #0
.L_0803b532:
	ldrb	r3, [r0, #0]
	adds	r4, #1
	strh	r3, [r2, #0]
	adds	r0, #1
	adds	r2, #2
	cmp	r4, #14
	bls.n	.L_0803b532
	b.n	.L_0803b59e
.L_0803b542:
	mov	r0, sl
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x4659
	subs	r5, r0, #1
	movs	r0, #1
	bl	sub_0803cd08
	bl	sub_08016ca4
	add	r1, sp, #84
	adds	r2, r1, #0
	movs	r4, #0
.L_0803b55c:
	ldrb	r3, [r0, #0]
	adds	r4, #1
	strh	r3, [r2, #0]
	adds	r0, #1
	adds	r2, #2
	cmp	r4, #14
	bls.n	.L_0803b55c
	ldr	r2, [sp, #24]
	add	r3, sp, #52
	str	r2, [sp, #4]
	str	r3, [sp, #8]
	adds	r2, r6, #0
	movs	r0, #0
	mov	r3, r8
	str	r5, [sp, #0]
	b.n	.L_0803b5ae
.L_0803b57c:
	mov	r0, sl
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x1e42
	adds	r0, r2, #0
	bl	sub_08016ca4
	add	r1, sp, #84
	adds	r2, r1, #0
	movs	r4, #0
.L_0803b590:
	ldrb	r3, [r0, #0]
	adds	r4, #1
	strh	r3, [r2, #0]
	adds	r0, #1
	adds	r2, #2
	cmp	r4, #14
	bls.n	.L_0803b590
.L_0803b59e:
	movs	r3, #0
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	add	r3, sp, #52
	str	r3, [sp, #8]
	adds	r2, r6, #0
	movs	r0, #0
	mov	r3, r8
.L_0803b5ae:
	bl	.L_0803aed8
.L_0803b5b2:
	adds	r6, r0, #0
	b.n	.L_0803b71c
.L_0803b5b6:
	mov	r0, sl
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x1e42
	adds	r0, r2, #0
	bl	sub_08016ca4
	adds	r5, r0, #0
	mov	r0, sl
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x2400
	mov	ip, r0
	add	r1, sp, #84
	adds	r0, r5, #0
	adds	r2, r1, #0
	cmp	r4, ip
	bcs.n	.L_0803b5fe
.L_0803b5da:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	strh	r3, [r2, #0]
	movs	r5, #128
	ldrb	r3, [r0, #0]
	lsls	r5, r5, #17
	adds	r3, #34
	lsls	r3, r3, #24
	adds	r2, #2
	cmp	r3, r5
	bhi.n	.L_0803b5f8
	ldrb	r3, [r0, #0]
	adds	r0, #1
	strh	r3, [r2, #0]
	adds	r2, #2
.L_0803b5f8:
	adds	r4, #1
	cmp	r4, ip
	bcc.n	.L_0803b5da
.L_0803b5fe:
	ldr	r3, [pc, #40]
	adds	r0, r6, #0
	strh	r3, [r2, #0]
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0803b5b2
	movs	r4, #128
	lsls	r4, r4, #1
	adds	r4, #255
.L_0803b612:
	lsls	r3, r0, #1
	mov	r5, r8
	strh	r2, [r3, r5]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	adds	r0, #1
	adds	r3, r2, #0
	ands	r0, r4
	cmp	r3, #0
	bne.n	.L_0803b612
	b.n	.L_0803b5b2
	.4byte 0x00000000
	.4byte 0x00000b63
	.4byte 0x0000025f
	.4byte 0x000005a7
	.4byte 0x00000e58
	.2byte 0x0240
	.2byte 0x0200
.L_0803b640:
	mov	r0, sl
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x3801
	lsls	r0, r0, #1
	adds	r2, r0, #0
	lsls	r3, r6, #1
	adds	r2, #128
	mov	r1, r8
	strh	r2, [r3, r1]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r6, #1
	ands	r6, r2
	lsls	r3, r6, #1
	adds	r0, #129
	mov	r5, r8
	adds	r6, #1
	strh	r0, [r3, r5]
	ands	r6, r2
	b.n	.L_0803b71c
.L_0803b66c:
	ldr	r3, [pc, #20]
	movs	r1, #128
	lsls	r2, r6, #1
	mov	r0, r8
	lsls	r1, r1, #1
	strh	r3, [r2, r0]
	adds	r1, #255
	adds	r6, #1
	ldr	r3, [pc, #8]
	ands	r6, r1
	b.n	.L_0803b6e2
	movs	r0, r0
	.4byte 0x0000008f
	.2byte 0x002d
	.2byte 0x0000
.L_0803b68c:
	ldr	r0, [sp, #24]
	cmp	r0, #0
	beq.n	.L_0803b71c
	ldr	r3, [sp, #52]
	cmp	r3, #0
	beq.n	.L_0803b6aa
	ldr	r3, [pc, #36]
	lsls	r2, r6, #1
	mov	r1, r8
	strh	r3, [r2, r1]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r6, #1
	adds	r2, #255
	ands	r6, r2
.L_0803b6aa:
	ldr	r3, [pc, #24]
	movs	r0, #128
	lsls	r0, r0, #1
	lsls	r2, r6, #1
	mov	r5, r8
	adds	r6, #1
	adds	r0, #255
	strh	r3, [r2, r5]
	ands	r6, r0
	b.n	.L_0803b71c
	movs	r0, r0
	.4byte 0x00000065
	.2byte 0x0073
	.2byte 0x0000
.L_0803b6c8:
	ldr	r2, [pc, #36]
	lsls	r3, r6, #1
	mov	r1, r8
	strh	r2, [r3, r1]
	movs	r1, #128
	ldr	r3, [sp, #52]
	lsls	r1, r1, #1
	adds	r6, #1
	adds	r1, #255
	ands	r6, r1
	cmp	r3, #0
	bne.n	.L_0803b71c
	ldr	r3, [pc, #16]
.L_0803b6e2:
	lsls	r2, r6, #1
	mov	r5, r8
	adds	r6, #1
	strh	r3, [r2, r5]
	ands	r6, r1
	b.n	.L_0803b71c
	movs	r0, r0
	.4byte 0x00000027
	.2byte 0x0073
	.2byte 0x0000
.L_0803b6f8:
	movs	r0, #1
	str	r0, [sp, #40]
.L_0803b6fc:
	movs	r2, #128
	lsls	r2, r2, #1
	lsls	r3, r6, #1
	mov	r1, r8
	adds	r6, #1
	adds	r2, #255
	strh	r7, [r3, r1]
	ands	r6, r2
	cmp	r7, #115
	beq.n	.L_0803b714
	cmp	r7, #83
	bne.n	.L_0803b718
.L_0803b714:
	movs	r3, #1
	b.n	.L_0803b71a
.L_0803b718:
	movs	r3, #0
.L_0803b71a:
	str	r3, [sp, #52]
.L_0803b71c:
	ldr	r3, [sp, #28]
	ldr	r5, [sp, #20]
	adds	r3, #1
	str	r3, [sp, #28]
	cmp	r5, #0
	beq.n	.L_0803b734
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	cmp	r3, r0
	bhi.n	.L_0803b734
	b.n	.L_0803b134
.L_0803b734:
	lsls	r3, r6, #1
	mov	r1, r8
	strh	r7, [r3, r1]
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r1, #255
	ldr	r3, [pc, #48]
	adds	r6, #1
	ands	r6, r1
	lsls	r2, r6, #1
	mov	r5, r8
	strh	r3, [r2, r5]
	adds	r3, r6, #1
	ands	r3, r1
	ldr	r0, [sp, #44]
	movs	r1, #152
	lsls	r1, r1, #5
	adds	r1, #66
	adds	r2, r0, r1
	strh	r3, [r2, #0]
	movs	r0, #200
	bl	sub_0801314c
	movs	r5, #152
	ldr	r2, [sp, #44]
	add	r0, sp, #32
	lsls	r5, r5, #5
	ldrh	r0, [r0, #0]
	adds	r5, #68
	adds	r3, r2, r5
	strh	r0, [r3, #0]
	b.n	.L_0803b778
	.2byte 0x0000
	.2byte 0x0000
.L_0803b778:
	mov	r1, fp
	cmp	r1, #0
	beq.n	.L_0803b782
	bl	sub_0803cca8
.L_0803b782:
	ldr	r0, [sp, #32]
	add	sp, #132
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
