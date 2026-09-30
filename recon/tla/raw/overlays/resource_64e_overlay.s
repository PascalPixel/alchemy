.syntax unified
	.thumb
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	cmp r5, #0
	beq .L_02000038_0
	adds r3, r5, #0
	adds r3, #84
	ldrb r2, [r3]
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	beq .L_02000038_0
	ldr r1, [r5, #80]
	movs r2, #13
	ldrb r0, [r1, #9]
	movs r3, #3
	negs r2, r2
	ands r4, r3
	adds r3, r2, #0
	lsls r4, r4, #2
	ands r3, r0
	orrs r3, r4
	strb r3, [r1, #9]
	adds r1, #37
	ldrb r3, [r1]
	ands r2, r3
	orrs r2, r4
	strb r2, [r1]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
.L_02000038_0:
	pop {r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	adds	r6, r1, #0
	strb	r3, [r2, #0]
	movs	r1, #0
	bl 0x02009b6c
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl 0x02009c14
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl 0x02009b3c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020000d8
	movs	r1, #0
	bl 0x02008038
	adds	r0, r5, #0
	movs	r1, #14
	bl 0x02008080
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02009b74
	adds	r0, r5, #0
	b.n	.L_020000da
.L_020000d8:
	movs	r0, #0
.L_020000da:
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl 0x02009b3c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000114
	movs	r1, #1
	bl 0x02008038
	adds	r0, r5, #0
	movs	r1, #15
	bl 0x02008080
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	b.n	.L_02000116
.L_02000114:
	movs	r0, #0
.L_02000116:
	pop	{r5, r6, pc}
	.4byte 0x6c426883
	.4byte 0x189b6d01
	.4byte 0x6c826083
	.4byte 0x189b68c3
	.4byte 0x6cc260c3
	.4byte 0x189b6903
	.4byte 0x6b026103
	.4byte 0x189b6983
	.4byte 0x6b426183
	.4byte 0x189b69c3
	.4byte 0x306461c3
	.4byte 0x88028a4b
	.4byte 0x824b189b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r3
	ldr	r3, [pc, #416]
	sub	sp, #4
	mov	sl, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	ldr	r7, [sp, #48]
	bl 0x02009ba4
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
.L_02000182:
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02000198
	cmp	r7, #0
	beq.n	.L_02000198
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_020001a0
.L_02000198:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_020001a0:
	mov	r3, sl
	bl 0x02009b3c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020001ae
	b.n	.L_020002f6
.L_020001ae:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x02009b2c
	ldr	r2, [pc, #324]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x02009b34
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	ldr	r3, [sp, #0]
	mov	r1, fp
	strb	r5, [r3, #26]
	ldr	r3, [pc, #296]
	str	r1, [r6, #68]
	str	r3, [r6, #108]
	ldr	r3, [sp, #36]
	mov	r2, r9
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	adds	r0, r6, #0
	str	r3, [r6, #76]
	ldr	r3, [r2, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x02008038
	movs	r3, #100
	adds	r3, r3, r6
	mov	r9, r3
	ldr	r3, [pc, #264]
	mov	r2, r8
	mov	r1, r9
	ands	r3, r2
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	strh	r5, [r1, #0]
	cmp	r3, #0
	beq.n	.L_020002f6
	cmp	r7, #0
	beq.n	.L_020002f6
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200022c
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x02009c14
.L_0200022c:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r1, r8
	ands	r3, r1
.L_02000234:
	cmp	r3, #0
	beq.n	.L_0200024c
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x02008038
.L_0200024c:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02000260
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02000260:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020002a6
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_0200028e
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x02009afc
.L_02000282:
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_020002a0
.L_0200028e:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x02009afc
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020002a0:
	bl 0x02009afc
	str	r0, [r6, #52]
.L_020002a6:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020002c2
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x02009b2c
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x02009b34
.L_020002c2:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002d4
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_020002d4:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002e6
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_020002e6:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002f6
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_020002f6:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x02009dcc
	.4byte 0x02008119
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb520
	ldmia	r0!, {r5}
	ldmia	r1!, {r3}
	ldmia	r0!, {r4}
	subs	r5, r5, r3
	ldmia	r1!, {r3}
	asrs	r5, r5, #16
	ldr	r2, [r1, #0]
	subs	r4, r4, r3
	ldr	r3, [r0, #0]
	asrs	r4, r4, #16
	subs	r3, r3, r2
	asrs	r3, r3, #16
	adds	r0, r5, #0
	muls	r0, r5
	adds	r2, r4, #0
	muls	r2, r4
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #4]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd20
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r1, r0, #0
	adds	r5, r3, #0
	movs	r4, #8
	adds	r5, #52
.L_0200035c:
	ldmia	r5!, {r0}
	ldr	r2, [r1, #0]
	ldr	r3, [r0, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02000382
	ldr	r2, [r1, #4]
	ldr	r3, [r0, #12]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02000382
	ldr	r2, [r1, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	beq.n	.L_0200038a
.L_02000382:
	adds	r4, #1
	cmp	r4, #63
	bls.n	.L_0200035c
	movs	r0, #0
.L_0200038a:
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #364]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl 0x02009ba4
	ldrh	r3, [r0, #6]
	ldr	r1, [pc, #348]
	lsrs	r3, r3, #12
	lsls	r5, r3, #2
	ldr	r2, [pc, #348]
	mov	r9, r1
	ldr	r1, [r1, r5]
	mov	sl, r2
	mov	r3, sl
	adds	r2, r1, #0
	ands	r2, r3
	ldr	r3, [r0, #8]
	mov	r7, sp
	adds	r3, r3, r2
	str	r3, [r7, #0]
	lsls	r1, r1, #16
	ldr	r3, [r0, #12]
	mov	r8, r0
	str	r3, [r7, #4]
	ldr	r3, [r0, #16]
	adds	r0, r7, #0
	adds	r3, r3, r1
	str	r3, [r7, #8]
	mov	r1, r8
	bl 0x0200834c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020003e2
	b.n	.L_020004f8
.L_020003e2:
	mov	r0, r9
	ldr	r1, [r0, r5]
	mov	r3, sl
	adds	r2, r1, #0
	ands	r2, r3
	ldr	r3, [r6, #8]
	lsls	r1, r1, #16
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r0, r7, #0
	ldr	r3, [r6, #12]
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	adds	r3, r3, r1
	str	r3, [r7, #8]
	adds	r1, r6, #0
	bl 0x0200834c
	cmp	r0, #0
	beq.n	.L_02000418
	adds	r3, r0, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020004f8
.L_02000418:
	ldr	r3, [r6, #8]
	movs	r0, #128
	str	r3, [r7, #0]
	lsls	r0, r0, #13
	ldr	r3, [r6, #12]
	adds	r1, r6, #0
	adds	r3, r3, r0
	str	r3, [r7, #4]
	adds	r0, r7, #0
	ldr	r3, [r6, #16]
	str	r3, [r7, #8]
	bl 0x0200834c
	cmp	r0, #0
	beq.n	.L_02000444
	adds	r3, r0, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020004f8
.L_02000444:
	adds	r2, r6, #0
	adds	r2, #34
	movs	r3, #2
	strb	r3, [r2, #0]
	mov	r2, r9
	ldr	r1, [r2, r5]
	mov	r3, sl
	adds	r2, r1, #0
	ands	r2, r3
	ldr	r3, [r6, #8]
	lsls	r1, r1, #16
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r0, r6, #0
	ldr	r3, [r6, #12]
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	adds	r3, r3, r1
	str	r3, [r7, #8]
	adds	r1, r7, #0
	bl 0x02009b64
	cmp	r0, #0
	bgt.n	.L_020004f8
	adds	r3, r6, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	mov	sl, r3
	cmp	r3, #0
	bne.n	.L_020004f8
	movs	r1, #8
	mov	r0, r8
	movs	r5, #204
	bl 0x02009b2c
	lsls	r5, r5, #6
	movs	r0, #15
	bl 0x02009b04
	adds	r5, #51
	movs	r0, #185
	bl 0x02009cdc
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	adds	r0, r6, #0
	bl 0x02009b44
	mov	r0, r8
	str	r5, [r0, #48]
	str	r5, [r0, #52]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	bl 0x02009b44
	adds	r0, r6, #0
	bl 0x02009b4c
	bl 0x02009cbc
	ldr	r3, [r7, #0]
	mov	r1, sl
	str	r3, [r6, #8]
	ldr	r3, [r7, #8]
	str	r1, [r6, #36]
	str	r3, [r6, #16]
	str	r1, [r6, #44]
	movs	r3, #128
	mov	r2, r8
	lsls	r3, r3, #24
	str	r3, [r2, #56]
	str	r3, [r2, #64]
	movs	r0, #10
	ldrsh	r3, [r2, r0]
	str	r1, [r2, #36]
	lsls	r3, r3, #16
	str	r1, [r2, #44]
	str	r3, [r2, #8]
	movs	r1, #18
	ldrsh	r3, [r2, r1]
	mov	r0, r8
	lsls	r3, r3, #16
	str	r3, [r2, #16]
	movs	r1, #1
	bl 0x02009b2c
.L_020004f8:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x02009d8c
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb560
	adds	r5, r3, #0
	ldr	r3, [sp, #12]
	ldr	r6, [sp, #16]
	mov	ip, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	beq.n	.L_02000562
	cmp	r0, #2
	bhi.n	.L_02000538
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r0, r0, #1
	lsls	r3, r3, #3
	adds	r3, r3, r0
	ldr	r0, [r4, r3]
	b.n	.L_0200053a
.L_02000538:
	ldr	r0, [pc, #44]
.L_0200053a:
	lsls	r3, r2, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	movs	r1, #0
	adds	r0, r0, r3
	cmp	r1, ip
	bcs.n	.L_02000562
.L_02000548:
	lsls	r3, r1, #9
	movs	r2, #0
	adds	r3, r0, r3
	cmp	r2, r5
	bcs.n	.L_0200055c
.L_02000552:
	adds	r2, #1
	strb	r6, [r3, #2]
	adds	r3, #4
	cmp	r2, r5
	bcc.n	.L_02000552
.L_0200055c:
	adds	r1, #1
	cmp	r1, ip
	bcc.n	.L_02000548
.L_02000562:
	movs	r0, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02010000
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #48]
	movs	r2, #3
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000586
	movs	r1, #7
	bl 0x02009c14
	b.n	.L_0200058c
.L_02000586:
	movs	r1, #0
	bl 0x02009c14
.L_0200058c:
	ldr	r3, [pc, #20]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0200059e
	movs	r0, #138
	bl 0x02009cdc
.L_0200059e:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #152]
	sub	sp, #56
	ldr	r7, [r3, #0]
	movs	r3, #7
	ands	r7, r3
	mov	sl, r0
	cmp	r7, #0
	bne.n	.L_02000640
	bl 0x02009b0c
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	movs	r2, #16
	movs	r3, #3
	add	r2, sp
	subs	r3, r3, r0
	str	r3, [r2, #0]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #14
	str	r3, [r2, #4]
	mov	r8, r2
	bl 0x02009b0c
	lsls	r3, r0, #3
	mov	r2, sl
	adds	r3, r3, r0
	ldr	r6, [r2, #8]
	lsrs	r3, r3, #16
	subs	r3, #4
	lsls	r3, r3, #16
	adds	r6, r6, r3
	bl 0x02009b0c
	mov	r2, sl
	lsls	r0, r0, #5
	ldr	r5, [r2, #12]
	lsrs	r0, r0, #16
	movs	r3, #32
	subs	r3, r3, r0
	lsls	r3, r3, #16
	adds	r5, r5, r3
	bl 0x02009b0c
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsrs	r0, r0, #16
	movs	r3, #160
	lsls	r3, r3, #11
	lsls	r0, r0, #16
	adds	r0, r0, r3
	movs	r1, #10
	bl 0x02009afc
	mov	r3, sl
	ldr	r2, [r3, #16]
	movs	r3, #176
	lsls	r3, r3, #12
	str	r3, [sp, #8]
	mov	r3, r8
	str	r0, [sp, #0]
	str	r3, [sp, #12]
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r3, #0
	str	r7, [sp, #4]
	bl 0x02008150
.L_02000640:
	movs	r0, #0
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x122c
	.2byte 0x0300
	.global Func_02000650
	.thumb_func
Func_02000650:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009e58
	.global Func_02000658
	.thumb_func
Func_02000658:
	movs r0, #0
	bx lr
	.global Func_0200065c
	.thumb_func
Func_0200065c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200a0f8
	.global Func_02000664
	.thumb_func
Func_02000664:
	push {lr}
	adds r3, r0, #0
	adds r3, #100
	ldrh r3, [r3]
	movs r1, #15
	ands r1, r3
	bl 0x02009c14
	movs r0, #0
	pop {pc}
	.global Func_02000678
	.thumb_func
Func_02000678:
	push {lr}
	movs r1, #0
	bl 0x02009b6c
	movs r0, #0
	pop {pc}
	.global Func_02000684
	.thumb_func
Func_02000684:
	push {lr}
	ldr r3, [pc, #24]
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_02000684_0
	ldr r0, [pc, #12]
	b .L_02000684_1
.L_02000684_0:
	ldr r0, [pc, #12]
.L_02000684_1:
	pop {pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200a378
	.4byte 0x0200a1b0
	push	{r5, r6, lr}
	bl 0x02009cc4
	adds	r5, r0, #0
	bl 0x02009ba4
	adds	r6, r0, #0
	bl 0x02009b94
	movs	r0, #0
	bl 0x02009ca4
	ldr	r1, [pc, #188]
	adds	r0, r5, #0
	bl 0x02009bb4
	adds	r0, r5, #0
	bl 0x02009bbc
	adds	r0, r5, #0
	bl 0x02009ba4
	movs	r1, #6
	bl 0x02009c14
	movs	r1, #128
	lsls	r1, r1, #11
	movs	r2, #128
	str	r1, [r6, #40]
	adds	r0, r5, #0
	lsls	r2, r2, #10
	bl 0x02009bac
	ldr	r3, [r6, #16]
	asrs	r3, r3, #20
	cmp	r3, #54
	bgt.n	.L_0200070a
	adds	r0, r5, #0
	bl 0x02009ba4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #210
	b.n	.L_0200071c
.L_0200070a:
	adds	r0, r5, #0
	bl 0x02009ba4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #238
.L_0200071c:
	movs	r3, #10
	ldrsh	r1, [r6, r3]
	lsls	r2, r2, #2
	adds	r0, r5, #0
.L_02000724:
	bl 0x02009bd4
	movs	r0, #1
	bl 0x02009b8c
	adds	r0, r5, #0
	bl 0x02009ba4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x02009b8c
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009c14
	ldr	r3, [pc, #56]
	movs	r1, #129
.L_02000750:
	str	r3, [r6, #108]
	movs	r2, #60
	adds	r0, r5, #0
	lsls	r1, r1, #1
	bl 0x02009c34
	adds	r0, r5, #0
	movs	r1, #4
	bl 0x02009bf4
	adds	r0, r5, #0
	bl 0x02009ba4
	movs	r1, #0
	bl 0x02009c14
	adds	r0, r5, #0
	movs	r1, #4
	bl 0x02009bf4
	movs	r3, #0
	str	r3, [r6, #108]
.L_0200077c:
	bl 0x02009b9c
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02009dd8
	.2byte 0x85a9
	.2byte 0x0200
	push	{r5, lr}
.L_0200078e:
	sub	sp, #8
	bl 0x02009cc4
	bl 0x02009ba4
	adds	r5, r0, #0
	movs	r1, #10
	ldrsh	r3, [r5, r1]
	movs	r1, #18
	ldrsh	r2, [r5, r1]
	ldr	r1, [pc, #200]
	adds	r3, r3, r1
	cmp	r3, #7
	bhi.n	.L_020007ba
	movs	r3, #197
	lsls	r3, r3, #2
	cmp	r2, r3
	blt.n	.L_020007ba
	movs	r1, #199
	lsls	r1, r1, #2
	cmp	r2, r1
	blt.n	.L_020007fe
.L_020007ba:
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #53
	movs	r1, #50
	movs	r2, #42
	movs	r3, #49
	bl 0x02009b54
	movs	r3, #3
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #117
	movs	r2, #41
	movs	r3, #117
	movs	r0, #55
	bl 0x02009b54
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009b24
	adds	r0, r5, #0
	adds	r0, #85
	ldrb	r1, [r0, #0]
	movs	r3, #1
	movs	r2, #0
	orrs	r3, r1
	strb	r3, [r0, #0]
	str	r2, [r5, #20]
	str	r2, [r5, #12]
	b.n	.L_02000868
.L_020007fe:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009b14
	cmp	r0, #0
	bne.n	.L_02000868
	bl 0x02009b94
	movs	r0, #0
	bl 0x02009ca4
	movs	r0, #5
	bl 0x02009b8c
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #52
	movs	r1, #50
	movs	r2, #42
	movs	r3, #49
	bl 0x02009b54
	movs	r3, #3
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #117
	movs	r2, #41
	movs	r3, #117
	movs	r0, #52
	bl 0x02009b54
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009b1c
	movs	r0, #161
	bl 0x02009cdc
	adds	r1, r5, #0
	adds	r1, #85
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	ldr	r3, [pc, #16]
	str	r3, [r5, #20]
	str	r3, [r5, #12]
	bl 0x02009b9c
.L_02000868:
	add	sp, #8
	pop	{r5, pc}
	.4byte 0xfffffd5c
	.2byte 0x0000
	.2byte 0xfffe
	.section .text.x020088d4,"ax",%progbits
	.global Func_020008d4
	.thumb_func
Func_020008d4:
	push {lr}
	bl 0x02009b94
	movs r0, #0
	bl 0x02009ca4
	bl 0x02009cc4
	movs r1, #1
	bl 0x02009bec
	ldr r0, [pc, #12]
	movs r1, #1
	bl 0x02009b7c
	bl 0x02009b9c
	pop {pc}
	.4byte 0x00001617
	.global Func_020008fc
	.thumb_func
Func_020008fc:
	push {lr}
	bl 0x02009b94
	movs r0, #0
	bl 0x02009ca4
	bl 0x02009cc4
	movs r1, #1
	bl 0x02009bec
	ldr r0, [pc, #12]
	movs r1, #1
	bl 0x02009b7c
	bl 0x02009b9c
	pop {pc}
	.4byte 0x00001618
	.section .text.x02008a0c,"ax",%progbits
	.global Func_02000a0c
	.thumb_func
Func_02000a0c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200a408
	.global Func_02000a14
	.thumb_func
Func_02000a14:
	movs r0, #0
	bx lr
	push	{lr}
	movs	r0, #140
	movs	r1, #1
	bl 0x02009c7c
	movs	r1, #10
	movs	r0, #4
	bl 0x02009c84
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #28]
	str	r3, [r2, #36]
	bl 0x02009c9c
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #1
	bl 0x02009c74
	bl 0x02009c8c
	bl 0x02009c94
	pop	{pc}
	.2byte 0x8a15
	.2byte 0x0200
	.global Func_02000a54
	.thumb_func
Func_02000a54:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #214
	lsls	r0, r0, #1
	movs	r2, #129
	adds	r3, r3, r0
	lsls	r2, r2, #2
	ldr	r1, [pc, #184]
	str	r2, [r3, #0]
	subs	r2, #34
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #99
	bne.n	.L_02000aa2
	ldr	r3, [pc, #172]
	movs	r0, #242
	lsls	r0, r0, #1
	adds	r2, r1, r0
	strh	r3, [r2, #0]
	movs	r3, #243
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #22
	strh	r3, [r2, #0]
	bl 0x02009b94
	movs	r0, #0
	bl 0x02009ca4
	bl 0x02008b2c
	bl 0x02008d30
	bl 0x02009b9c
	b.n	.L_02000b1a
.L_02000aa2:
	movs	r0, #10
	bl 0x02009ba4
	movs	r1, #6
	bl 0x02009c14
	movs	r0, #17
	bl 0x02009ba4
	movs	r1, #6
	bl 0x02009c14
	movs	r0, #18
	bl 0x02009ba4
	movs	r1, #6
	bl 0x02009c14
	movs	r0, #19
	bl 0x02009ba4
	movs	r1, #6
	bl 0x02009c14
	movs	r0, #20
	bl 0x02009ba4
	movs	r1, #6
	bl 0x02009c14
	movs	r0, #21
	bl 0x02009ba4
	movs	r1, #6
	bl 0x02009c14
	movs	r0, #22
	bl 0x02009ba4
	movs	r1, #6
	bl 0x02009c14
	movs	r0, #23
	bl 0x02009ba4
	movs	r1, #6
	bl 0x02009c14
	movs	r0, #24
	bl 0x02009ba4
	movs	r1, #6
	bl 0x02009c14
	movs	r0, #25
	bl 0x02009ba4
	movs	r1, #6
	bl 0x02009c14
.L_02000b1a:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0005
	.2byte 0x0000
	.global Func_02000b28
	.thumb_func
Func_02000b28:
	movs r0, #0
	bx lr
	push	{lr}
	movs	r1, #178
	movs	r2, #204
	movs	r0, #4
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x02009be4
	movs	r1, #174
	movs	r2, #210
	movs	r0, #5
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x02009be4
	movs	r1, #182
	movs	r2, #210
	movs	r0, #8
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x02009be4
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c2c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c2c
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c2c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009bac
	movs	r1, #128
	movs	r2, #128
	movs	r0, #5
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009bac
	movs	r1, #128
	movs	r2, #128
	movs	r0, #8
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009bac
	movs	r1, #128
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009bac
	movs	r0, #166
	movs	r1, #1
	movs	r2, #206
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #18
	movs	r3, #0
	bl 0x02009c44
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	subs	r3, #172
	str	r3, [r2, #0]
	adds	r3, #180
	adds	r2, r1, r3
	movs	r3, #60
	str	r3, [r2, #0]
	bl 0x02009c5c
	bl 0x02009c6c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #4
	bl 0x02009bf4
	movs	r0, #20
	bl 0x02009b8c
	movs	r0, #5
	movs	r1, #3
	bl 0x02009bec
	movs	r1, #3
	movs	r0, #8
	bl 0x02009bf4
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #166
	movs	r2, #206
	movs	r0, #4
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	bl 0x02009bd4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x02009c2c
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #178
	movs	r2, #206
	movs	r0, #8
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	bl 0x02009bd4
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	bl 0x02008a18
	bl 0x02008874
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #182
	movs	r2, #210
	movs	r0, #8
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	bl 0x02009bd4
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c2c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c2c
	movs	r1, #178
	movs	r2, #204
	movs	r0, #4
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	bl 0x02009bd4
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #4
	bl 0x02009bf4
	movs	r0, #20
	bl 0x02009b8c
	movs	r0, #5
	movs	r1, #3
	bl 0x02009bec
	movs	r1, #3
	movs	r0, #8
	bl 0x02009bf4
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #5
	movs	r1, #4
	bl 0x02009cb4
	movs	r0, #8
	movs	r1, #4
	bl 0x02009cb4
	movs	r1, #186
	movs	r2, #204
	movs	r0, #4
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	bl 0x02009bd4
	movs	r1, #186
	movs	r2, #195
	lsls	r2, r2, #2
	lsls	r1, r1, #2
	movs	r0, #4
	bl 0x02009bd4
	movs	r0, #129
	bl 0x02009cdc
	movs	r1, #1
	negs	r1, r1
	movs	r0, #4
	bl 0x02009ccc
	movs	r0, #50
	bl 0x02009b8c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c2c
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c2c
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x02009be4
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	ldr	r0, [pc, #1020]
	sub	sp, #8
	bl 0x02009c1c
	movs	r1, #1
	movs	r0, #8
	bl 0x02009c3c
	bl 0x02009c4c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #166
	movs	r2, #210
	movs	r0, #5
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	bl 0x02009bd4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c2c
	movs	r1, #174
	movs	r2, #210
	lsls	r2, r2, #2
	lsls	r1, r1, #2
	movs	r0, #8
	bl 0x02009bd4
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #8
	bl 0x02009bf4
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #5
	bl 0x02009bf4
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #174
	movs	r2, #230
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	movs	r0, #8
	bl 0x02009bcc
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #166
	movs	r2, #222
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	movs	r0, #5
	bl 0x02009bd4
	movs	r0, #8
	bl 0x02009bdc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x02009c34
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #174
	movs	r2, #222
	movs	r0, #8
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	bl 0x02009bd4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c2c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r0, #174
	movs	r1, #1
	movs	r2, #230
	movs	r3, #1
	lsls	r2, r2, #18
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x02009c44
	bl 0x02009c4c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #5
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	movs	r2, #50
	adds	r1, #255
	movs	r0, #8
	bl 0x02009c34
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #8
	bl 0x02009bf4
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #5
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #8
	bl 0x02009c34
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #129
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x02009c34
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #5
	bl 0x02009bf4
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #30
	bl 0x02009b8c
	movs	r0, #198
	bl 0x02009cdc
	movs	r1, #170
	movs	r2, #230
	movs	r0, #9
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x02009be4
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #9
	bl 0x02009cd4
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c2c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x02009c2c
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #20
	bl 0x02009b8c
	movs	r0, #9
	movs	r1, #4
	bl 0x02009bf4
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #5
	movs	r1, #2
	bl 0x02009bfc
	movs	r1, #2
	movs	r0, #8
	bl 0x02009c04
	movs	r0, #20
	bl 0x02009b8c
	movs	r0, #5
	movs	r1, #8
	movs	r2, #40
	bl 0x02009c0c
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x02009c34
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c2c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #129
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x02009c34
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x02009c34
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #5
	bl 0x02009c04
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #7
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x02009bac
	movs	r0, #8
	movs	r1, #5
	bl 0x02009cb4
	movs	r2, #32
	negs	r2, r2
	movs	r1, #0
	movs	r0, #5
	bl 0x02009cac
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x02009c34
	movs	r1, #8
	movs	r2, #8
	negs	r2, r2
	movs	r0, #9
	negs	r1, r1
	bl 0x02009cac
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #8
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x02009c34
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #9
	bl 0x02009c34
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #8
	bl 0x02009c34
	movs	r1, #129
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #5
	b.n	.L_02001134
	.2byte 0x0000
	.2byte 0x1570
	.2byte 0x0000
.L_02001134:
	bl 0x02009c34
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x02009c34
	movs	r1, #240
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #131
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x02009c34
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #10
	movs	r2, #30
	adds	r1, #255
	movs	r0, #9
	bl 0x02009c34
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #9
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #9
	movs	r1, #4
	bl 0x02009bf4
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #4
	movs	r0, #5
	bl 0x02009bf4
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #5
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009bac
	movs	r0, #5
	movs	r1, #0
	movs	r2, #16
	bl 0x02009cac
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #8
	movs	r2, #30
	adds	r1, #255
	movs	r0, #5
	bl 0x02009c34
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #9
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #7
	movs	r0, #9
	bl 0x02009bf4
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #144
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #129
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x02009c34
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #5
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r0, #5
	movs	r1, #4
	bl 0x02009bf4
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #16
	bl 0x02009cac
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #9
	movs	r1, #4
	bl 0x02009bf4
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #6
	movs	r2, #50
	adds	r1, #255
	movs	r0, #5
	bl 0x02009c34
	movs	r1, #2
	movs	r0, #9
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #5
	bl 0x02009bf4
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #8
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #5
	bl 0x02009bf4
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #9
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #5
	bl 0x02009bf4
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #129
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x02009c34
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #10
	movs	r2, #30
	adds	r1, #255
	movs	r0, #8
	bl 0x02009c34
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #5
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #129
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x02009c34
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #9
	movs	r1, #4
	bl 0x02009bf4
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #6
	movs	r2, #30
	adds	r1, #255
	movs	r0, #8
	bl 0x02009c34
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #8
	movs	r2, #30
	adds	r1, #255
	movs	r0, #5
	bl 0x02009c34
	movs	r1, #0
.L_020014ca:
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #8
	bl 0x02009bf4
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #5
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r2, #16
	movs	r1, #0
	negs	r2, r2
	movs	r0, #5
	bl 0x02009cac
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #5
	bl 0x02009bf4
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #224
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c2c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x02009c2c
	movs	r0, #50
	bl 0x02009b8c
	movs	r1, #144
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c2c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #5
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r0, #9
	movs	r1, #4
	bl 0x02009bf4
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x02009c34
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #9
	movs	r1, #4
	bl 0x02009bf4
	movs	r0, #9
	movs	r1, #0
	bl 0x02009c24
	movs	r1, #2
	movs	r0, #5
	bl 0x02009c04
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #240
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #25
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #8
	bl 0x02009bf4
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #9
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #129
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x02009c34
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #9
	bl 0x02009bf4
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #5
	movs	r1, #8
	movs	r2, #50
	bl 0x02009c0c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x02009c34
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #6
	movs	r0, #9
	bl 0x02009bf4
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x02009c34
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x02009c34
	movs	r1, #240
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #5
	bl 0x02009c34
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #9
	bl 0x02009c34
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #9
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #240
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #8
	bl 0x02009bf4
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #5
	bl 0x02009bf4
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #20
	bl 0x02009b8c
	movs	r0, #9
	movs	r1, #4
	bl 0x02009bf4
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #9
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #0
	movs	r0, #9
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #8
	bl 0x02009bf4
	movs	r0, #20
	bl 0x02009b8c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x02009c2c
	movs	r0, #20
	bl 0x02009b8c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #2
	movs	r0, #5
	bl 0x02009c04
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #128
	lsls	r1, r1, #5
	movs	r2, #0
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #30
	bl 0x02009b8c
	movs	r1, #6
	movs	r2, #30
	adds	r1, #255
	movs	r0, #5
	bl 0x02009c34
	movs	r1, #0
	movs	r0, #5
	bl 0x02009c24
	movs	r0, #10
	bl 0x02009b8c
	movs	r1, #3
	movs	r0, #5
	bl 0x02009bf4
	movs	r0, #20
	bl 0x02009b8c
	movs	r0, #9
	movs	r1, #3
	bl 0x02009bec
	movs	r1, #3
	movs	r0, #8
	bl 0x02009bf4
	movs	r0, #20
	bl 0x02009b8c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #8
	ldr	r1, [pc, #396]
	adds	r2, #153
	bl 0x02009bac
	movs	r0, #8
	movs	r1, #2
	bl 0x02009bec
	movs	r0, #5
	bl 0x02009ba4
	cmp	r0, #0
	beq.n	.L_0200198e
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #8
	bl 0x02009bc4
.L_0200198e:
	movs	r0, #8
	bl 0x02009bdc
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x02009be4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #9
	ldr	r1, [pc, #332]
	adds	r2, #153
	bl 0x02009bac
	movs	r0, #9
	movs	r1, #2
	bl 0x02009bec
	movs	r0, #5
	bl 0x02009ba4
	cmp	r0, #0
	beq.n	.L_020019cc
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #9
	bl 0x02009bc4
.L_020019cc:
	movs	r0, #9
	bl 0x02009bdc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #9
	bl 0x02009be4
	movs	r0, #10
	bl 0x02009b8c
	movs	r0, #5
	movs	r1, #16
	movs	r2, #0
	bl 0x02009cac
	movs	r1, #170
	movs	r2, #236
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	movs	r0, #5
	bl 0x02009bd4
	movs	r0, #15
	bl 0x02009b8c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #50
	bl 0x02009b8c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #5
	bl 0x02009c2c
	movs	r0, #15
	bl 0x02009b8c
	movs	r1, #182
	movs	r2, #236
	movs	r0, #5
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	bl 0x02009bd4
	movs	r1, #182
	movs	r2, #232
	movs	r0, #5
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	bl 0x02009bd4
	movs	r1, #182
	movs	r2, #228
	lsls	r2, r2, #18
	lsls	r1, r1, #18
	movs	r0, #5
	bl 0x02009be4
	movs	r0, #129
	bl 0x02009cdc
	movs	r1, #1
	negs	r1, r1
	movs	r0, #5
	bl 0x02009ccc
	movs	r0, #40
	bl 0x02009b8c
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #53
	movs	r1, #50
	movs	r2, #42
	movs	r3, #49
	bl 0x02009b54
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r5, #5
	movs	r3, #117
	movs	r1, #117
	movs	r2, #41
	movs	r0, #55
	str	r5, [sp, #4]
	bl 0x02009b54
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009b24
	movs	r1, #166
	movs	r2, #198
	movs	r0, #10
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x02009be4
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r0, r0
	negs	r2, r2
	movs	r3, #0
	bl 0x02009c44
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r5, [r3, #0]
	movs	r0, #4
	bl 0x02009b84
	movs	r0, #50
	bl 0x02009b8c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r3, #218
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #60
	str	r3, [r2, #0]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	subs	r3, #172
	str	r3, [r2, #0]
	bl 0x02009c64
	bl 0x02009c6c
	movs	r0, #22
	bl 0x02009c54
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x00013333
	.4byte 0x02000240
	.section .rodata.x02009ce4,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.4byte 0x02009ce4
	.4byte 0x02009d1c
	.4byte 0x02009d54
	.4byte 0x00000027
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x02008571
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00011999
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00011999
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000e666
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000e666
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000011
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0xc00000c8
	.4byte 0x00000000
	.4byte 0x00f00038
	.4byte 0x000000d8
	.4byte 0xffff0002
	.4byte 0x00000078
	.4byte 0x40000068
	.4byte 0x00000000
	.4byte 0x00f00038
	.4byte 0x000000d8
	.4byte 0xffff0003
	.4byte 0x00000158
	.4byte 0xc00000e8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0004
	.4byte 0x000001d8
	.4byte 0x400000b8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0005
	.4byte 0x00000178
	.4byte 0x400000b8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0006
	.4byte 0x00000158
	.4byte 0x40000068
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0007
	.4byte 0x00000348
	.4byte 0xc00000f8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff0008
	.4byte 0x000003c8
	.4byte 0x400000a8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff0009
	.4byte 0x00000358
	.4byte 0x40000058
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000a
	.4byte 0x00000328
	.4byte 0x40000058
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000b
	.4byte 0x000002c8
	.4byte 0x400000a8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000c
	.4byte 0x000000c8
	.4byte 0xc0000198
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff000d
	.4byte 0x00000068
	.4byte 0xc00001f8
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff000e
	.4byte 0x00000068
	.4byte 0x400001d8
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff0014
	.4byte 0x00000208
	.4byte 0x400002b8
	.4byte 0x01300000
	.4byte 0x02400158
	.4byte 0x000002d8
	.4byte 0xffff0015
	.4byte 0x00000158
	.4byte 0x40000238
	.4byte 0x01300000
	.4byte 0x02400158
	.4byte 0x000002d8
	.4byte 0xffff0016
	.4byte 0x000002d8
	.4byte 0x400003a8
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0017
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0018
	.4byte 0x00000188
	.4byte 0x400003b8
	.4byte 0x01100000
	.4byte 0x02000328
	.4byte 0x000003d8
	.4byte 0xffff0019
	.4byte 0x00000188
	.4byte 0x40000358
	.4byte 0x01100000
	.4byte 0x02000328
	.4byte 0x000003d8
	.4byte 0xffff001a
	.4byte 0x000002f8
	.4byte 0x400003a8
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff001e
	.4byte 0x00000058
	.4byte 0x40000368
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff001f
	.4byte 0x000000b8
	.4byte 0x40000368
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff0020
	.4byte 0x000000b8
	.4byte 0x40000288
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff0062
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0063
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02400000
	.4byte 0xffff02e0
	.4byte 0x000003c8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00109004
	.4byte 0x00203005
	.4byte 0x00302005
	.4byte 0x0041f005
	.4byte 0x0051e005
	.4byte 0x00607005
	.4byte 0x00706005
	.4byte 0x00820005
	.4byte 0x00916005
	.4byte 0x00a0c005
	.4byte 0x00b0d005
	.4byte 0x00c0a005
	.4byte 0x00d0b005
	.4byte 0x00e18005
	.4byte 0x0141a005
	.4byte 0x01519005
	.4byte 0x01609005
	.4byte 0x0180e005
	.4byte 0x01915005
	.4byte 0x01a14005
	.4byte 0x01e05005
	.4byte 0x01f04005
	.4byte 0x02008005
	.4byte 0x000001ff
	.4byte 0x00000016
	.4byte 0x00000023
	.4byte 0x00002126
	.4byte 0x00000016
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x02008665
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x02008679
	.4byte 0x00000011
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0150
	.4byte 0x0200a15c
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x000c0000
	.4byte 0x00024000
	.4byte 0xffff00e6
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x000c0000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x0102c000
	.4byte 0xffff01c4
	.4byte 0x0200a180
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0xffff0150
	.4byte 0x0200a15c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff0150
	.4byte 0x0200a15c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte 0x0200a15c
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte 0x0200a15c
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte 0x0200a15c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0xffff0150
	.4byte 0x0200a15c
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte 0x0200a15c
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte 0x0200a15c
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte 0x0200a15c
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0004
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0150
	.4byte 0x0200a15c
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000021
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000031
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000031
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000031
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000031
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000031
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000021
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x00000021
	.4byte 0xffff001a
	.4byte 0x0000001a
	.4byte 0x00000021
	.4byte 0xffff001e
	.4byte 0x0000001e
	.4byte 0x00000021
	.4byte 0xffff001f
	.4byte 0x0000001f
	.4byte 0x00000021
	.4byte 0xffff0020
	.4byte 0x00000020
	.4byte 0x00000002
	.4byte 0x0200002d
	.4byte 0x020086ad
	.4byte 0x00000002
	.4byte 0xffff002e
	.4byte 0x0200878d
	.4byte 0x00000003
	.4byte 0xffff0023
	.4byte 0x020088d5
	.4byte 0x00000003
	.4byte 0xffff0024
	.4byte 0x020088d5
	.4byte 0x00000003
	.4byte 0xffff0025
	.4byte 0x020088d5
	.4byte 0x00000003
	.4byte 0xffff0026
	.4byte 0x020088d5
	.4byte 0x00000003
	.4byte 0xffff0027
	.4byte 0x020088d5
	.4byte 0x00000003
	.4byte 0xffff0028
	.4byte 0x020088fd
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008999
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008925
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
