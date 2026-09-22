.syntax unified
	.thumb
	.set sub_08013438, 0x08013438
	.global Overlay_080164e8
Overlay_080164e8:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #80]
	ldr	r5, [pc, #80]
	ldr	r7, [r3, #0]
	ldrb	r3, [r5, #1]
	adds	r6, r0, #0
	adds	r0, r1, #0
	cmp	r3, #1
	bne.n	.L_0801650a
	bl	.L_080165b4
	adds	r0, r6, #0
	bl	.L_08016544
	ldrb	r3, [r5, #11]
	adds	r3, #1
	strb	r3, [r5, #11]
.L_0801650a:
	ldrb	r3, [r5, #2]
	ldrb	r2, [r5, #3]
	lsls	r3, r3, #8
	orrs	r2, r3
	ldrb	r3, [r5, #0]
	cmp	r3, #8
	bne.n	.L_0801651c
	movs	r3, #128
	orrs	r2, r3
.L_0801651c:
	ldrb	r3, [r5, #9]
	adds	r0, r2, #0
	cmp	r3, #0
	beq.n	.L_0801652a
	movs	r3, #128
	lsls	r3, r3, #5
	orrs	r0, r3
.L_0801652a:
	lsls	r3, r7, #26
	lsrs	r3, r3, #30
	cmp	r3, #1
	bls.n	.L_08016538
	movs	r3, #128
	lsls	r3, r3, #6
	orrs	r0, r3
.L_08016538:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x04000128
	.2byte 0x5360
	.2byte 0x0200
.L_08016544:
	push	{r5, lr}
	ldr	r4, [pc, #100]
	movs	r5, #0
	ldr	r1, [r4, #40]
	ldrb	r3, [r4, #11]
	strb	r3, [r1, #0]
	ldrb	r2, [r4, #3]
	ldrb	r3, [r4, #2]
	strh	r5, [r1, #2]
	eors	r3, r2
	strb	r3, [r1, #1]
	movs	r2, #132
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, #4
	adds	r2, #6
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [r4, #40]
	movs	r1, #0
.L_08016570:
	ldrh	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #2
	adds	r5, r5, r3
	cmp	r1, #13
	bls.n	.L_08016570
	ldr	r3, [r4, #40]
	mvns	r2, r5
	strh	r2, [r3, #2]
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_0801658e
	ldr	r2, [pc, #36]
	movs	r3, #0
	strh	r3, [r2, #0]
.L_0801658e:
	movs	r3, #1
	negs	r3, r3
	str	r3, [r4, #20]
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_080165a6
	ldrb	r3, [r4, #8]
	cmp	r3, #0
	beq.n	.L_080165a6
	ldr	r2, [pc, #12]
	ldr	r3, [pc, #4]
	strh	r3, [r2, #0]
.L_080165a6:
	pop	{r5, pc}
	.4byte 0x000000c0
	.4byte 0x02005360
	.2byte 0x010e
	.2byte 0x0400
.L_080165b4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #8
	movs	r3, #0
	adds	r4, r0, #0
	str	r3, [sp, #4]
	ldr	r2, [pc, #192]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #192]
	movs	r7, #3
	adds	r1, r3, #0
	adds	r1, #64
.L_080165d2:
	ldr	r2, [r1, #16]
	ldr	r3, [r1, #0]
	subs	r7, #1
	str	r3, [r1, #16]
	stmia	r1!, {r2}
	cmp	r7, #0
	bge.n	.L_080165d2
	ldr	r1, [pc, #172]
	movs	r0, #0
	ldr	r3, [r1, #0]
	mov	lr, sp
	str	r3, [sp, #0]
	str	r0, [r1, #0]
	movs	r2, #1
	ldr	r3, [pc, #152]
	strh	r2, [r3, #0]
	subs	r3, r1, #4
	strb	r0, [r3, #3]
	mov	r9, r3
	subs	r2, #2
	adds	r6, r1, #0
	movs	r7, #0
	mov	sl, r9
	mov	r8, r2
	adds	r6, #76
	mov	ip, r4
.L_08016606:
	ldr	r2, [r6, #0]
	movs	r0, #0
	movs	r1, #0
.L_0801660c:
	ldrh	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #2
	adds	r0, r0, r3
	cmp	r1, #13
	bls.n	.L_0801660c
	mov	r3, lr
	ldrb	r4, [r3, r7]
	cmp	r4, #1
	bne.n	.L_0801664e
	lsls	r5, r0, #16
	asrs	r3, r5, #16
	cmp	r3, r8
	bne.n	.L_08016662
	ldr	r0, [r6, #0]
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, #4
	mov	r1, ip
	adds	r2, #6
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r1, sl
	ldrb	r3, [r1, #3]
	ldr	r2, [pc, #72]
	lsls	r4, r7
	orrs	r4, r3
	strb	r4, [r1, #3]
	mov	r9, r2
	b.n	.L_08016650
.L_0801664e:
	lsls	r5, r0, #16
.L_08016650:
	asrs	r3, r5, #16
	cmp	r3, r8
	bne.n	.L_08016662
	ldr	r2, [r6, #0]
	ldrh	r3, [r2, #2]
	mvns	r3, r3
	strh	r3, [r2, #2]
	ldr	r3, [pc, #44]
	mov	r9, r3
.L_08016662:
	movs	r1, #24
	adds	r7, #1
	adds	r6, #4
	add	ip, r1
	cmp	r7, #1
	ble.n	.L_08016606
	mov	r2, r9
	ldrb	r3, [r2, #2]
	ldrb	r2, [r2, #3]
	mov	r1, r9
	ldrb	r0, [r1, #3]
	orrs	r3, r2
	strb	r3, [r1, #2]
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x04000208
	.4byte 0x02005360
	.2byte 0x5364
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #120]
	sub	sp, #8
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #116]
	mov	lr, sp
	mov	r2, lr
	str	r3, [r2, #0]
	str	r4, [r2, #4]
	ldr	r3, [r0, #0]
	ldr	r1, [pc, #108]
	lsls	r3, r3, #25
	lsrs	r3, r3, #31
	ldr	r2, [r1, #20]
	strb	r3, [r1, #9]
	movs	r3, #1
	negs	r3, r3
	cmp	r2, r3
	bne.n	.L_080166ce
	movs	r3, #254
	lsls	r3, r3, #8
	adds	r3, #254
	ldr	r2, [r1, #44]
	strh	r3, [r0, #2]
	ldr	r3, [r1, #40]
	str	r2, [r1, #40]
	str	r3, [r1, #44]
	b.n	.L_080166da
.L_080166ce:
	cmp	r2, #0
	blt.n	.L_080166da
	ldr	r3, [r1, #44]
	lsls	r2, r2, #1
	ldrh	r3, [r2, r3]
	strh	r3, [r0, #2]
.L_080166da:
	ldr	r7, [pc, #60]
	ldr	r3, [r7, #20]
	cmp	r3, #14
	bgt.n	.L_080166e6
	adds	r3, #1
	str	r3, [r7, #20]
.L_080166e6:
	movs	r6, #0
.L_080166e8:
	mov	r2, lr
	lsls	r5, r6, #1
	ldrh	r3, [r2, r5]
	movs	r2, #254
	lsls	r2, r2, #8
	adds	r2, #254
	cmp	r3, r2
	bne.n	.L_0801671c
	lsls	r3, r6, #2
	mov	ip, r3
	mov	r2, ip
	adds	r2, #24
	ldr	r3, [r7, r2]
	cmp	r3, #13
	ble.n	.L_08016720
	movs	r3, #1
	negs	r3, r3
	str	r3, [r7, r2]
	b.n	.L_0801674c
	movs	r0, r0
	.4byte 0x04000120
	.4byte 0x04000128
	.2byte 0x5360
	.2byte 0x0200
.L_0801671c:
	lsls	r2, r6, #2
	mov	ip, r2
.L_08016720:
	mov	r3, ip
	adds	r3, #24
	mov	r4, ip
	ldr	r1, [r7, r3]
	adds	r4, #48
	mov	r3, lr
	ldr	r0, [r7, r4]
	ldrh	r5, [r3, r5]
	lsls	r2, r1, #1
	strh	r5, [r2, r0]
	cmp	r1, #13
	bne.n	.L_0801674c
	mov	r3, ip
	adds	r3, #64
	ldr	r2, [r7, r3]
	adds	r1, r6, #4
	str	r0, [r7, r3]
	str	r2, [r7, r4]
	ldrb	r3, [r7, r1]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r7, r1]
.L_0801674c:
	ldr	r0, [pc, #76]
	ldrb	r3, [r0, #9]
	cmp	r3, #0
	beq.n	.L_0801675e
	adds	r3, r6, #4
	ldrb	r2, [r0, r3]
	movs	r1, #2
	orrs	r2, r1
	strb	r2, [r0, r3]
.L_0801675e:
	mov	r2, ip
	adds	r2, #24
	ldr	r3, [r0, r2]
	cmp	r3, #14
	bgt.n	.L_0801676c
	adds	r3, #1
	str	r3, [r0, r2]
.L_0801676c:
	adds	r6, #1
	adds	r7, r0, #0
	cmp	r6, #1
	ble.n	.L_080166e8
	ldrb	r3, [r0, #0]
	cmp	r3, #8
	bne.n	.L_080167a8
	ldr	r0, [pc, #36]
	ldr	r3, [pc, #16]
	ldr	r1, [pc, #36]
	strh	r3, [r0, #0]
	ldr	r2, [pc, #16]
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	ldr	r3, [pc, #12]
	strh	r3, [r0, #0]
	b.n	.L_080167a8
	.4byte 0x00000000
	.4byte 0x00000080
	.4byte 0x000000c0
	.4byte 0x02005360
	.4byte 0x0400010e
	.2byte 0x0128
	.2byte 0x0400
.L_080167a8:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	push	{lr}
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #24]
	movs	r0, #7
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	bl	sub_08013438
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl	sub_08013438
	b.n	.L_080167d4
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0x11b8
	.2byte 0x0300
.L_080167d4:
	pop	{pc}
