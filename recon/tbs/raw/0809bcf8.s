.syntax unified
	.thumb
	.set sub_08003dec, 0x08003dec
	.set sub_0800447c, 0x0800447c
	.set sub_08015060, 0x08015060
	.set sub_08015078, 0x08015078
	.set sub_080153c0, 0x080153c0
	.set sub_080770c0, 0x080770c0
	.set sub_0808b158, 0x0808b158
	.set sub_0808ba1c, 0x0808ba1c
	.global Unnamed_0809bcf8
	.global Func_0809bcf8
	.thumb_func
Unnamed_0809bcf8:
Func_0809bcf8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #296]
	movs	r0, #250
	lsls	r0, r0, #1
	adds	r5, r3, r0
	ldr	r1, [r5, #0]
	ldr	r2, [pc, #288]
	sub	sp, #60
	str	r1, [sp, #36]
	str	r2, [sp, #32]
	ldr	r0, [pc, #284]
	ldrh	r3, [r0, #0]
	ldr	r2, [pc, #284]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	movs	r1, #1
	lsrs	r3, r3, #5
	negs	r1, r1
	movs	r2, #100
	str	r3, [sp, #28]
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	ldr	r3, [pc, #268]
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #268]
	lsrs	r3, r3, #1
	movs	r2, #31
	ands	r3, r2
	ldr	r7, [pc, #248]
	ldrb	r3, [r1, r3]
	movs	r0, #142
	lsls	r0, r0, #1
	adds	r7, #32
	str	r3, [sp, #24]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_0809bd9c
	ldr	r1, [pc, #244]
	movs	r2, #192
	ldr	r3, [r1, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0809bd9e
	ldr	r0, [r5, #0]
	bl	sub_0808ba1c
	cmp	r0, #0
	beq.n	.L_0809be2c
	ldr	r2, [r0, #8]
	movs	r3, #240
	lsls	r3, r3, #24
	adds	r2, r2, r3
	asrs	r2, r2, #16
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r2, r3, #4
	cmp	r2, #0
	bge.n	.L_0809bd82
	ldr	r1, [pc, #204]
	adds	r2, r2, r1
.L_0809bd82:
	asrs	r2, r2, #12
	mov	fp, r2
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r0, r3, #5
	cmp	r0, #0
	bge.n	.L_0809bd98
	ldr	r1, [pc, #180]
	adds	r0, r0, r1
.L_0809bd98:
	asrs	r0, r0, #12
	b.n	.L_0809be2a
.L_0809bd9c:
	ldr	r1, [pc, #168]
.L_0809bd9e:
	ldr	r2, [r1, #0]
	movs	r1, #15
	lsrs	r2, r2, #4
	ands	r2, r1
	ldr	r3, [pc, #168]
	lsls	r2, r2, #1
	ldrh	r1, [r3, r2]
	ldr	r2, [pc, #164]
	cmp	r1, r2
	beq.n	.L_0809be14
	ldr	r0, [pc, #132]
	ldr	r3, [r0, #4]
	add	r5, sp, #48
	movs	r6, #0
	str	r3, [r5, #0]
	str	r6, [r5, #4]
	ldr	r3, [r0, #8]
	str	r3, [r5, #8]
	adds	r2, r5, #0
	ldr	r0, [r0, #24]
	bl	sub_0800447c
	movs	r1, #128
	ldr	r3, [r5, #0]
	lsls	r1, r1, #13
	cmp	r3, r1
	bge.n	.L_0809bdda
	movs	r3, #128
	lsls	r3, r3, #13
	str	r3, [r5, #0]
.L_0809bdda:
	movs	r2, #239
	lsls	r2, r2, #16
	cmp	r3, r2
	ble.n	.L_0809bde4
	str	r2, [r5, #0]
.L_0809bde4:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bge.n	.L_0809bdee
	str	r6, [r5, #8]
	movs	r3, #0
.L_0809bdee:
	movs	r2, #141
	lsls	r2, r2, #16
	cmp	r3, r2
	ble.n	.L_0809bdf8
	str	r2, [r5, #8]
.L_0809bdf8:
	ldr	r3, [r5, #0]
	ldr	r2, [pc, #60]
	str	r3, [r2, #4]
	ldr	r3, [r5, #8]
	movs	r0, #192
	str	r3, [r2, #8]
	ldr	r3, [r2, #24]
	lsls	r0, r0, #11
	cmp	r3, r0
	bge.n	.L_0809be1c
	movs	r1, #128
	lsls	r1, r1, #6
	adds	r3, r3, r1
	b.n	.L_0809be1a
.L_0809be14:
	movs	r3, #128
	ldr	r2, [pc, #32]
	lsls	r3, r3, #9
.L_0809be1a:
	str	r3, [r2, #24]
.L_0809be1c:
	ldr	r3, [pc, #24]
	movs	r0, #6
	ldrsh	r3, [r3, r0]
	ldr	r0, [pc, #20]
	movs	r1, #10
	ldrsh	r0, [r0, r1]
	mov	fp, r3
.L_0809be2a:
	mov	r9, r0
.L_0809be2c:
	movs	r6, #0
	b.n	.L_0809bf30
	.4byte 0x02000240
	.4byte 0x080a0138
	.4byte 0x02010000
	.4byte 0x03001b10
	.4byte 0x03001e40
	.4byte 0x0809f168
	.4byte 0x03001ae8
	.4byte 0x00000fff
	.4byte 0x0809f188
	.2byte 0xffff
	.2byte 0x0000
.L_0809be58:
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0809bf2e
	adds	r0, r6, #0
	bl	sub_0808ba1c
	cmp	r0, #0
	beq.n	.L_0809bf2e
	ldr	r2, [r0, #8]
	movs	r1, #240
	lsls	r1, r1, #24
	adds	r2, r2, r1
	asrs	r2, r2, #16
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r1, r3, #4
	cmp	r1, #0
	bge.n	.L_0809be82
	ldr	r2, [pc, #112]
	adds	r1, r1, r2
.L_0809be82:
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r0, r3, #5
	asrs	r5, r1, #12
	cmp	r0, #0
	bge.n	.L_0809be96
	ldr	r1, [pc, #92]
	adds	r0, r0, r1
.L_0809be96:
	asrs	r4, r0, #12
	movs	r0, #13
	mov	r2, r8
	ldrb	r3, [r7, #5]
	negs	r0, r0
	lsls	r1, r2, #2
	adds	r2, r0, #0
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r7, #5]
	ldr	r1, [sp, #28]
	ldr	r3, [pc, #48]
	add	r1, sl
	ands	r1, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r7, #8]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #8]
	ldr	r3, [pc, #40]
	subs	r1, r5, #1
	ands	r1, r3
	ldr	r2, [pc, #40]
	ldrh	r3, [r7, #6]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #6]
	mov	r1, fp
	subs	r3, r4, #1
	mov	r0, r9
	subs	r2, r5, r1
	strb	r3, [r7, #4]
	subs	r3, r4, r0
	adds	r1, r2, #0
	muls	r1, r2
	b.n	.L_0809bef4
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0x0fff
	.2byte 0x0000
.L_0809bef4:
	adds	r0, r3, #0
	muls	r0, r3
	str	r2, [sp, #44]
	str	r3, [sp, #40]
	adds	r2, r1, #0
	adds	r3, r0, #0
	ldr	r1, [sp, #16]
	adds	r2, r2, r3
	cmp	r2, r1
	bge.n	.L_0809bf14
	ldr	r3, [sp, #4]
	str	r6, [sp, #20]
	str	r3, [sp, #0]
	str	r2, [sp, #16]
	str	r5, [sp, #12]
	str	r4, [sp, #8]
.L_0809bf14:
	cmp	r6, #0
	bne.n	.L_0809bf24
	ldr	r3, [pc, #60]
	ldr	r3, [r3, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #7
	bhi.n	.L_0809bf2e
.L_0809bf24:
	adds	r0, r7, #0
	adds	r1, r4, #0
	adds	r7, #12
	bl	sub_08003dec
.L_0809bf2e:
	adds	r6, #1
.L_0809bf30:
	cmp	r6, #65
	bgt.n	.L_0809bf82
	ldr	r0, [sp, #36]
	cmp	r6, r0
	beq.n	.L_0809bf3e
	cmp	r6, #10
	ble.n	.L_0809bf2e
.L_0809bf3e:
	cmp	r6, #0
	bne.n	.L_0809bf5c
	movs	r0, #142
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_0809bf2e
	movs	r1, #0
	mov	sl, r1
	ldr	r0, [sp, #36]
	mov	r8, r1
	b.n	.L_0809be58
	.2byte 0x1e40
	.2byte 0x0300
.L_0809bf5c:
	ldr	r3, [sp, #32]
	ldmia	r3!, {r0}
	adds	r2, r3, #0
	str	r2, [sp, #32]
	ldmia	r3!, {r1}
	str	r1, [sp, #4]
	adds	r1, r3, #0
	str	r1, [sp, #32]
	cmp	r0, #0
	beq.n	.L_0809bf2e
	movs	r1, #1
	asrs	r2, r0, #16
	movs	r3, #1
	negs	r1, r1
	mov	sl, r2
	mov	r8, r3
	cmp	r0, r1
	beq.n	.L_0809bf82
	b.n	.L_0809be58
.L_0809bf82:
	movs	r3, #1
	ldr	r2, [sp, #20]
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_0809bfd4
	ldr	r3, [pc, #84]
	ldr	r3, [r3, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #7
	bhi.n	.L_0809bfd4
	ldrb	r2, [r7, #5]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r7, #5]
	ldr	r2, [sp, #28]
	ldr	r3, [pc, #52]
	adds	r2, #3
	ands	r2, r3
	ldrh	r1, [r7, #8]
	ldr	r3, [pc, #56]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #8]
	ldr	r2, [sp, #12]
	ldr	r3, [pc, #40]
	subs	r2, #2
	ldrh	r1, [r7, #6]
	ands	r2, r3
	ldr	r3, [pc, #44]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	ldr	r3, [sp, #8]
	subs	r3, #2
	strb	r3, [r7, #4]
	adds	r0, r7, #0
	ldr	r1, [sp, #8]
	bl	sub_08003dec
.L_0809bfd4:
	ldr	r7, [pc, #24]
	movs	r3, #63
	ldrb	r2, [r7, #7]
	b.n	.L_0809bff4
	.4byte 0x000003ff
	.4byte 0x000001ff
	.4byte 0x03001e40
	.4byte 0xfffffc00
	.4byte 0xfffffe00
	.2byte 0x0338
	.2byte 0x0201
.L_0809bff4:
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r7, #7]
	mov	r2, fp
	ldr	r3, [pc, #52]
	subs	r2, #17
	ands	r2, r3
	ldrh	r1, [r7, #6]
	ldr	r3, [pc, #48]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	mov	r3, r9
	adds	r3, #1
	strb	r3, [r7, #4]
	adds	r0, r7, #0
	movs	r1, #246
	bl	sub_08003dec
	ldr	r1, [pc, #32]
	ldr	r2, [sp, #20]
	movs	r0, #18
	ldrsh	r3, [r1, r0]
	cmp	r3, r2
	beq.n	.L_0809c0a8
	add	r3, sp, #20
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #16]
	strh	r3, [r0, #18]
	ldr	r0, [r0, #28]
	b.n	.L_0809c040
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0x0000
	.2byte 0x0201
.L_0809c040:
	bl	sub_08015060
	movs	r1, #1
	ldr	r0, [sp, #20]
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_0809c0a8
	cmp	r0, #0
	bne.n	.L_0809c058
	ldr	r2, [pc, #200]
	str	r2, [sp, #0]
	b.n	.L_0809c066
.L_0809c058:
	ldr	r0, [sp, #0]
	movs	r1, #1
	bl	sub_0808b158
	ldr	r3, [pc, #188]
	adds	r0, r0, r3
	str	r0, [sp, #0]
.L_0809c066:
	add	r1, sp, #44
	ldr	r0, [sp, #0]
	add	r2, sp, #40
	bl	sub_080153c0
	ldr	r3, [sp, #12]
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #44]
	subs	r3, #1
	mov	fp, r3
	subs	r0, #11
	adds	r3, r3, r1
	mov	r9, r0
	cmp	r3, #240
	ble.n	.L_0809c090
	movs	r3, #232
	subs	r3, r3, r1
	ldr	r1, [sp, #8]
	subs	r1, #20
	mov	fp, r3
	mov	r9, r1
.L_0809c090:
	mov	r2, r9
	cmp	r2, #0
	bge.n	.L_0809c09a
	movs	r3, #0
	mov	r9, r3
.L_0809c09a:
	ldr	r0, [pc, #136]
	mov	r2, fp
	ldr	r1, [r0, #28]
	mov	r3, r9
	ldr	r0, [sp, #0]
	bl	sub_08015078
.L_0809c0a8:
	ldr	r0, [pc, #124]
	ldr	r1, [pc, #128]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_0809c0d6
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	strh	r2, [r0, #0]
	movs	r2, #252
	adds	r3, #4
	lsls	r2, r2, #6
	stmia	r3!, {r2}
	ldr	r2, [pc, #96]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0809c0d6:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r3, [r0, #0]
	cmp	r3, #31
	bgt.n	.L_0809c108
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r0, #0]
	lsls	r2, r2, #2
	adds	r2, r2, r0
	ldr	r0, [sp, #24]
	movs	r3, #16
	subs	r3, r3, r0
	lsls	r3, r3, #8
	adds	r2, #4
	orrs	r3, r0
	stmia	r2!, {r3}
	ldr	r3, [pc, #52]
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_0809c108:
	strh	r4, [r1, #0]
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000984
	.4byte 0x0000099b
	.4byte 0x02010000
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x04000050
	.4byte 0x04000052
