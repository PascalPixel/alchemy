.syntax unified
	.thumb
	.set sub_08016ce4, 0x08016ce4
	.set sub_080ad0f0, 0x080ad0f0
	.set sub_080c9920, 0x080c9920
	.set sub_080c9f2c, 0x080c9f2c
	.set sub_080c9fd8, 0x080c9fd8
	.global Func_080ca17c
	.thumb_func
Func_080ca17c:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_080c9f2c
	adds	r1, r5, #0
	bl	sub_080c9fd8
	pop	{r5, pc}
.L_080ca18c:
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #1
	ldr	r2, [pc, #12]
	adds	r3, r3, r1
	lsls	r3, r3, #1
	adds	r3, #4
	ldrh	r0, [r2, r3]
	bx	lr
	movs	r0, r0
	.2byte 0xdacc
	.2byte 0x080e
	push	{lr}
	ldr	r3, [pc, #16]
	lsls	r0, r0, #2
	adds	r0, r0, r3
	ldrh	r3, [r0, #0]
	ldrh	r1, [r0, #2]
	adds	r0, r3, #0
	bl	.L_080ca18c
	pop	{pc}
	.2byte 0xef54
	.2byte 0x080e
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r4, [pc, #172]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r4, r2
	movs	r1, #79
	mov	sl, r1
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r5, [pc, #160]
	mov	fp, r1
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r4, r1
	adds	r1, #10
	movs	r2, #0
	ldrsh	r7, [r3, r2]
	ldrh	r0, [r5, #0]
	adds	r3, r4, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	adds	r1, r0, #0
	mov	r9, r2
	lsls	r3, r1, #16
	movs	r2, #1
	asrs	r3, r3, #16
	negs	r2, r2
	cmp	r3, r2
	beq.n	.L_080ca262
	movs	r6, #254
	lsls	r6, r6, #7
	adds	r6, #255
	mov	r8, r2
.L_080ca20a:
	ldrb	r2, [r5, #3]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ca21e
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	cmp	r3, fp
	bne.n	.L_080ca252
	b.n	.L_080ca226
.L_080ca21e:
	lsls	r3, r0, #16
	asrs	r3, r3, #16
	cmp	r3, r9
	bne.n	.L_080ca252
.L_080ca226:
	ldrh	r2, [r5, #2]
	adds	r3, r6, #0
	ands	r3, r2
	cmp	r3, r6
	beq.n	.L_080ca238
	lsls	r3, r2, #17
	asrs	r3, r3, #17
	cmp	r3, r7
	bne.n	.L_080ca252
.L_080ca238:
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	cmp	r0, r8
	beq.n	.L_080ca248
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ca252
.L_080ca248:
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	ldr	r4, [pc, #40]
	mov	sl, r3
	b.n	.L_080ca262
.L_080ca252:
	adds	r5, #8
	ldrh	r1, [r5, #0]
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	adds	r0, r1, #0
	cmp	r3, r8
	bne.n	.L_080ca20a
	ldr	r4, [pc, #20]
.L_080ca262:
	movs	r2, #132
	lsls	r2, r2, #2
	adds	r3, r4, r2
	mov	r1, sl
	strh	r1, [r3, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0xf094
	.2byte 0x080e
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #0
	adds	r6, r1, #0
	ldr	r5, [pc, #108]
	mov	r8, r3
	adds	r7, r0, #0
	bl	sub_080c9920
	ldrh	r4, [r5, #0]
	movs	r2, #1
	adds	r1, r4, #0
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	negs	r2, r2
	mov	ip, r0
	cmp	r3, r2
	beq.n	.L_080ca2f0
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	mov	lr, r2
.L_080ca2ae:
	ldrb	r2, [r5, #3]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ca2c2
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	cmp	r3, r7
	bne.n	.L_080ca2e2
	b.n	.L_080ca2ca
.L_080ca2c2:
	lsls	r3, r4, #16
	asrs	r3, r3, #16
	cmp	r3, ip
	bne.n	.L_080ca2e2
.L_080ca2ca:
	ldrh	r2, [r5, #2]
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, r0
	beq.n	.L_080ca2dc
	lsls	r3, r2, #17
	asrs	r3, r3, #17
	cmp	r3, r6
	bne.n	.L_080ca2e2
.L_080ca2dc:
	ldr	r5, [r5, #4]
	mov	r8, r5
	b.n	.L_080ca2f0
.L_080ca2e2:
	adds	r5, #8
	ldrh	r1, [r5, #0]
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	adds	r4, r1, #0
	cmp	r3, lr
	bne.n	.L_080ca2ae
.L_080ca2f0:
	mov	r0, r8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0xf4a4
	.2byte 0x080e
	push	{r5, r6, r7, lr}
	ldr	r0, [pc, #96]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r0, r1
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	movs	r4, #241
	ldr	r2, [pc, #84]
	lsls	r4, r4, #1
	adds	r3, r0, r4
	movs	r7, #0
	ldrsh	r5, [r3, r7]
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	movs	r4, #1
	negs	r4, r4
	ldrh	r1, [r2, #0]
	cmp	r3, r4
	beq.n	.L_080ca35c
	movs	r7, #242
	movs	r3, #243
	lsls	r7, r7, #1
	lsls	r3, r3, #1
	mov	ip, r4
	adds	r4, r0, r7
	adds	r0, r0, r3
.L_080ca332:
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	cmp	r3, r6
	bne.n	.L_080ca350
	movs	r7, #2
	ldrsh	r3, [r2, r7]
	cmp	r3, ip
	beq.n	.L_080ca346
	cmp	r3, r5
	bne.n	.L_080ca350
.L_080ca346:
	ldrh	r3, [r2, #4]
	strh	r3, [r4, #0]
	ldrh	r3, [r2, #6]
	strh	r3, [r0, #0]
	b.n	.L_080ca35c
.L_080ca350:
	adds	r2, #16
	movs	r7, #0
	ldrsh	r3, [r2, r7]
	ldrh	r1, [r2, #0]
	cmp	r3, ip
	bne.n	.L_080ca332
.L_080ca35c:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0xf824
	.2byte 0x080e
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #222
	adds	r6, r1, #0
	ldr	r5, [pc, #108]
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ca3e2
	ldr	r1, [pc, #104]
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	asrs	r0, r1, #16
	ldrh	r2, [r5, #0]
	cmp	r3, r0
	beq.n	.L_080ca3e2
	adds	r4, r1, #0
	ldr	r1, [pc, #92]
	mov	ip, r0
	adds	r3, r1, #4
	mov	lr, r3
	movs	r3, #8
	adds	r3, r3, r1
	mov	r8, r3
.L_080ca3a0:
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	cmp	r3, r7
	bne.n	.L_080ca3d6
	movs	r0, #2
	ldrsh	r3, [r5, r0]
	asrs	r2, r4, #16
	cmp	r3, r2
	beq.n	.L_080ca3ba
	cmp	r6, r2
	beq.n	.L_080ca3ba
	cmp	r3, r6
	bne.n	.L_080ca3d6
.L_080ca3ba:
	movs	r2, #8
	ldrsh	r3, [r5, r2]
	lsls	r3, r3, #16
	str	r3, [r1, #0]
	movs	r0, #10
	ldrsh	r3, [r5, r0]
	mov	r1, lr
	lsls	r3, r3, #16
	str	r3, [r1, #0]
	movs	r2, #12
	ldrsh	r3, [r5, r2]
	mov	r0, r8
	str	r3, [r0, #0]
	b.n	.L_080ca3e2
.L_080ca3d6:
	adds	r5, #16
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	ldrh	r2, [r5, #0]
	cmp	r3, ip
	bne.n	.L_080ca3a0
.L_080ca3e2:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x080ef824
	.4byte 0xffff0000
	.2byte 0x04b8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #222
	adds	r6, r1, #0
	ldr	r5, [pc, #128]
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ca482
	ldr	r1, [pc, #124]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r1, r2
	movs	r2, #0
	strh	r2, [r3, #0]
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	ldr	r2, [pc, #108]
	asrs	r0, r2, #16
	cmp	r3, r0
	beq.n	.L_080ca482
	movs	r3, #158
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r4, r2, #0
	movs	r2, #159
	mov	lr, r3
	lsls	r2, r2, #2
	movs	r3, #160
	adds	r2, r2, r1
	lsls	r3, r3, #2
	mov	r8, r2
	adds	r1, r1, r3
	mov	ip, r0
.L_080ca442:
	movs	r2, #4
	ldrsh	r3, [r5, r2]
	cmp	r3, r7
	bne.n	.L_080ca478
	movs	r2, #6
	ldrsh	r3, [r5, r2]
	asrs	r2, r4, #16
	cmp	r3, r2
	beq.n	.L_080ca45c
	cmp	r6, r2
	beq.n	.L_080ca45c
	cmp	r3, r6
	bne.n	.L_080ca478
.L_080ca45c:
	movs	r2, #8
	ldrsh	r3, [r5, r2]
	mov	r2, lr
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	movs	r2, #10
	ldrsh	r3, [r5, r2]
	mov	r2, r8
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	movs	r2, #12
	ldrsh	r3, [r5, r2]
	str	r3, [r1, #0]
	b.n	.L_080ca482
.L_080ca478:
	adds	r5, #16
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, ip
	bne.n	.L_080ca442
.L_080ca482:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x080ef824
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x4b03
	movs	r2, #251
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bx	lr
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r7, [pc, #88]
	movs	r2, #240
	ldr	r0, [pc, #88]
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldmia	r0!, {r2}
	cmp	r2, #0
	beq.n	.L_080ca4e2
	cmp	r2, r1
	beq.n	.L_080ca4e2
	movs	r4, #255
	movs	r6, #128
	lsls	r4, r4, #8
	lsls	r6, r6, #24
	adds	r4, #255
.L_080ca4cc:
	adds	r3, r2, #0
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_080ca4d8
	adds	r5, r4, #0
	ands	r5, r2
.L_080ca4d8:
	ldmia	r0!, {r2}
	cmp	r2, #0
	beq.n	.L_080ca4e2
	cmp	r2, r1
	bne.n	.L_080ca4cc
.L_080ca4e2:
	ldr	r3, [pc, #40]
	cmp	r5, r3
	bne.n	.L_080ca4f8
	movs	r0, #163
	lsls	r0, r0, #4
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ca4f6
	ldr	r5, [pc, #24]
.L_080ca4f6:
	ldr	r7, [pc, #12]
.L_080ca4f8:
	movs	r2, #251
	lsls	r2, r2, #1
	adds	r3, r7, r2
	strh	r5, [r3, #0]
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x080ef984
	.4byte 0x0000006a
	.2byte 0x0069
	.2byte 0x0000
	push	{lr}
	subs	r0, #1
	cmp	r0, #11
	bhi.n	.L_080ca56c
	ldr	r2, [pc, #92]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080ca554
	.4byte 0x080ca558
	.4byte 0x080ca55c
	.4byte 0x080ca560
	.4byte 0x080ca564
	.4byte 0x080ca564
	.4byte 0x080ca560
	.4byte 0x080ca56c
	.4byte 0x080ca56c
	.4byte 0x080ca56c
	.4byte 0x080ca568
	.2byte 0xa568
	.2byte 0x080c
	ldr	r2, [pc, #40]
	b.n	.L_080ca56e
	ldr	r2, [pc, #40]
	b.n	.L_080ca56e
	ldr	r2, [pc, #40]
	b.n	.L_080ca56e
	ldr	r2, [pc, #40]
	b.n	.L_080ca56e
	ldr	r2, [pc, #40]
	b.n	.L_080ca56e
	ldr	r2, [pc, #40]
	b.n	.L_080ca56e
.L_080ca56c:
	ldr	r2, [pc, #40]
.L_080ca56e:
	ldr	r3, [pc, #44]
	movs	r1, #251
	lsls	r1, r1, #1
	adds	r3, r3, r1
	strh	r2, [r3, #0]
	pop	{pc}
	movs	r0, r0
	.4byte 0x080ca524
	.4byte 0x00000046
	.4byte 0x00000048
	.4byte 0x00000073
	.4byte 0x00000047
	.4byte 0x0000006d
	.4byte 0x0000006e
	.4byte 0x00000053
	.2byte 0x0240
	.2byte 0x0200
.L_080ca5a0:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_080ad0f0
	cmp	r0, #4
	ble.n	.L_080ca5ae
	movs	r0, #4
.L_080ca5ae:
	movs	r2, #0
	cmp	r2, r0
	bge.n	.L_080ca5ce
	ldr	r3, [pc, #28]
	movs	r4, #134
	lsls	r4, r4, #2
	adds	r1, r3, r4
.L_080ca5bc:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, r5
	bne.n	.L_080ca5c8
	movs	r0, #1
	b.n	.L_080ca5d0
.L_080ca5c8:
	adds	r2, #1
	cmp	r2, r0
	blt.n	.L_080ca5bc
.L_080ca5ce:
	movs	r0, #0
.L_080ca5d0:
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	lsls	r0, r0, #4
	adds	r0, r0, r1
	lsls	r0, r0, #16
	asrs	r7, r0, #16
	movs	r0, #182
	lsls	r0, r0, #1
	ldr	r5, [pc, #180]
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ca5f4
	movs	r6, #79
	b.n	.L_080ca636
.L_080ca5f4:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	adds	r5, #2
	lsls	r2, r3, #16
	lsrs	r1, r2, #16
	cmp	r1, #0
	beq.n	.L_080ca636
	lsls	r3, r7, #16
	lsrs	r0, r3, #16
	cmp	r1, r0
	beq.n	.L_080ca636
	movs	r1, #240
	movs	r4, #128
	lsls	r1, r1, #4
	lsls	r4, r4, #8
	adds	r1, #255
	mov	ip, r0
.L_080ca616:
	lsrs	r2, r2, #16
	adds	r3, r2, #0
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080ca624
	adds	r6, r2, #0
	ands	r6, r1
.L_080ca624:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	adds	r5, #2
	lsls	r2, r3, #16
	lsrs	r3, r2, #16
	cmp	r3, #0
	beq.n	.L_080ca636
	cmp	r3, ip
	bne.n	.L_080ca616
.L_080ca636:
	lsls	r3, r6, #16
	cmp	r3, #0
	bne.n	.L_080ca68c
	ldr	r3, [pc, #96]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_080ca65e
	movs	r0, #252
	lsls	r0, r0, #3
	adds	r0, #255
	bl	sub_08016ce4
	movs	r6, #56
	cmp	r0, #0
	beq.n	.L_080ca68c
.L_080ca65e:
	movs	r0, #4
	bl	.L_080ca5a0
	movs	r6, #50
	cmp	r0, #0
	bne.n	.L_080ca68c
	movs	r0, #0
	bl	.L_080ca5a0
	movs	r6, #128
	lsls	r6, r6, #2
	adds	r6, #238
	cmp	r0, #0
	bne.n	.L_080ca68c
	movs	r0, #5
	bl	.L_080ca5a0
	negs	r3, r0
	orrs	r3, r0
	lsrs	r3, r3, #31
	adds	r6, r3, #0
	movs	r3, #50
	subs	r6, r3, r6
.L_080ca68c:
	ldr	r3, [pc, #16]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #14
	adds	r3, r3, r2
	strh	r6, [r3, #0]
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080efd1c
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	cmp	r5, #8
	bgt.n	.L_080ca6d6
	cmp	r5, #8
	bne.n	.L_080ca6d6
	ldr	r3, [pc, #40]
	movs	r1, #253
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #32]
	movs	r5, #9
	cmp	r2, r3
	beq.n	.L_080ca6c6
	movs	r5, #11
.L_080ca6c6:
	movs	r0, #252
	lsls	r0, r0, #3
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ca6d6
	adds	r5, #1
.L_080ca6d6:
	adds	r0, r5, #0
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000001
