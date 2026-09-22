.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0811a038, 0x0811a038
	.global Overlay_08119374
Overlay_08119374:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #36]
	sub	sp, #20
	adds	r3, r1, #0
	adds	r3, #68
	ldrb	r3, [r3, #0]
	movs	r7, #0
	cmp	r3, #0
	bne.n	.L_0811938c
	b.n	.L_0811959a
.L_0811938c:
	adds	r3, r1, #0
	adds	r3, #80
	ldrb	r2, [r3, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #60]
	lsls	r2, r2, #3
	adds	r5, r2, r3
	adds	r3, r1, #0
	adds	r3, #82
	ldrb	r3, [r3, #0]
	ldr	r6, [pc, #52]
	cmp	r3, #0
	beq.n	.L_081193ae
	b.n	.L_08119578
.L_081193ae:
	ldr	r3, [pc, #24]
	movs	r0, #1
	strh	r3, [r6, #0]
	ldr	r3, [pc, #20]
	strh	r3, [r6, #2]
	ldr	r3, [pc, #20]
	strh	r3, [r6, #8]
	ldr	r3, [pc, #20]
	strh	r3, [r6, #10]
	bl	sub_08013560
	b.n	.L_081193e6
	movs	r0, r0
	.4byte 0x00000065
	.4byte 0x00000078
	.4byte 0x00000054
	.4byte 0x00000055
	.4byte 0x02003874
	.2byte 0x3a74
	.2byte 0x0200
.L_081193e0:
	movs	r0, #1
	bl	sub_08013560
.L_081193e6:
	ldr	r3, [pc, #116]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_081193fa
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L_081193e0
	b.n	.L_08119578
.L_081193fa:
	ldrh	r2, [r6, #4]
	ldrh	r3, [r5, #4]
	movs	r7, #0
	cmp	r2, r3
	beq.n	.L_08119406
	b.n	.L_08119578
.L_08119406:
	ldrh	r2, [r6, #6]
	ldrh	r3, [r5, #6]
	cmp	r2, r3
	beq.n	.L_08119410
	b.n	.L_08119578
.L_08119410:
	ldrh	r2, [r6, #0]
	ldrh	r3, [r5, #0]
	cmp	r2, r3
	bne.n	.L_081193e0
	ldrh	r2, [r6, #2]
	ldrh	r3, [r5, #2]
	cmp	r2, r3
	bne.n	.L_081193e0
	ldrh	r2, [r6, #8]
	ldrh	r3, [r5, #8]
	cmp	r2, r3
	bne.n	.L_081193e0
	ldrh	r2, [r6, #10]
	ldrh	r3, [r5, #10]
	cmp	r2, r3
	bne.n	.L_081193e0
	ldr	r3, [pc, #32]
	strh	r3, [r6, #12]
	ldr	r3, [pc, #32]
	strh	r3, [r6, #14]
	b.n	.L_08119440
.L_0811943a:
	movs	r0, #1
	bl	sub_08013560
.L_08119440:
	ldr	r3, [pc, #24]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_08119460
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L_0811943a
	b.n	.L_08119578
	.4byte 0x00000072
	.4byte 0x0000006e
	.2byte 0x124c
	.2byte 0x0300
.L_08119460:
	ldrh	r2, [r6, #8]
	ldrh	r3, [r5, #8]
	movs	r7, #0
	cmp	r2, r3
	beq.n	.L_0811946c
	b.n	.L_08119578
.L_0811946c:
	ldrh	r2, [r6, #10]
	ldrh	r3, [r5, #10]
	cmp	r2, r3
	beq.n	.L_08119476
	b.n	.L_08119578
.L_08119476:
	ldrh	r2, [r6, #12]
	ldrh	r3, [r5, #12]
	cmp	r2, r3
	bne.n	.L_0811943a
	ldrh	r2, [r6, #14]
	ldrh	r3, [r5, #14]
	cmp	r2, r3
	bne.n	.L_0811943a
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #40]
	strh	r3, [r6, #0]
	strh	r3, [r6, #4]
	ldr	r3, [pc, #40]
	strh	r2, [r6, #2]
	strh	r3, [r6, #6]
	b.n	.L_0811949c
.L_08119496:
	movs	r0, #1
	bl	sub_08013560
.L_0811949c:
	ldr	r3, [pc, #28]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_081194c0
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L_08119496
	b.n	.L_08119578
	.4byte 0x00000045
	.4byte 0x00000058
	.4byte 0x00000043
	.2byte 0x124c
	.2byte 0x0300
.L_081194c0:
	ldrh	r2, [r6, #12]
	ldrh	r3, [r5, #12]
	movs	r7, #0
	cmp	r2, r3
	bne.n	.L_08119578
	ldrh	r2, [r6, #14]
	ldrh	r3, [r5, #14]
	cmp	r2, r3
	bne.n	.L_08119578
	ldrh	r2, [r6, #0]
	ldrh	r3, [r5, #0]
	cmp	r2, r3
	bne.n	.L_08119496
	ldrh	r2, [r6, #2]
	ldrh	r3, [r5, #2]
	cmp	r2, r3
	bne.n	.L_08119496
	ldrh	r2, [r6, #4]
	ldrh	r3, [r5, #4]
	cmp	r2, r3
	bne.n	.L_08119496
	ldrh	r2, [r6, #6]
	ldrh	r3, [r5, #6]
	cmp	r2, r3
	bne.n	.L_08119496
	ldr	r3, [pc, #36]
	strh	r3, [r6, #8]
	ldr	r3, [pc, #36]
	strh	r3, [r6, #10]
	b.n	.L_08119502
.L_081194fc:
	movs	r0, #1
	bl	sub_08013560
.L_08119502:
	ldr	r3, [pc, #28]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_08119524
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L_081194fc
	b.n	.L_08119578
	movs	r0, r0
	.4byte 0x00000074
	.4byte 0x00000075
	.2byte 0x124c
	.2byte 0x0300
.L_08119524:
	ldrh	r2, [r6, #0]
	ldrh	r3, [r5, #0]
	movs	r7, #0
	cmp	r2, r3
	bne.n	.L_08119578
	ldrh	r2, [r6, #2]
	ldrh	r3, [r5, #2]
	cmp	r2, r3
	bne.n	.L_08119578
	ldrh	r2, [r6, #4]
	ldrh	r3, [r5, #4]
	cmp	r2, r3
	bne.n	.L_08119578
	ldrh	r2, [r6, #6]
	ldrh	r3, [r5, #6]
	cmp	r2, r3
	bne.n	.L_08119578
	ldrh	r2, [r6, #8]
	ldrh	r3, [r5, #8]
	cmp	r2, r3
	bne.n	.L_081194fc
	ldrh	r2, [r6, #10]
	ldrh	r3, [r5, #10]
	cmp	r2, r3
	bne.n	.L_081194fc
	ldr	r3, [pc, #40]
	strh	r3, [r6, #12]
	ldr	r3, [pc, #40]
	strh	r3, [r6, #14]
	b.n	.L_08119566
.L_08119560:
	movs	r0, #1
	bl	sub_08013560
.L_08119566:
	ldr	r3, [pc, #32]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_0811958c
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L_08119560
.L_08119578:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811959c
	movs	r0, r0
	.4byte 0x00000052
	.4byte 0x0000004e
	.2byte 0x124c
	.2byte 0x0300
.L_0811958c:
	ldrh	r3, [r5, #12]
	movs	r7, #0
	cmp	r3, #114
	bne.n	.L_0811959a
	ldrh	r3, [r5, #14]
	cmp	r3, #110
	beq.n	.L_08119560
.L_0811959a:
	movs	r0, #0
.L_0811959c:
	add	sp, #20
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	sub	sp, #16
	mov	r5, sp
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r0, r5, #0
	ldr	r6, [r3, #36]
	bl	sub_0811a038
	movs	r1, #0
	adds	r4, r0, #0
	cmp	r1, r4
	bge.n	.L_081195ce
	adds	r0, r5, #0
.L_081195bc:
	ldrh	r2, [r0, #0]
	adds	r3, r1, #0
	adds	r2, #72
	subs	r3, #128
	adds	r1, #1
	adds	r0, #2
	strb	r3, [r6, r2]
	cmp	r1, r4
	blt.n	.L_081195bc
.L_081195ce:
	add	sp, #16
	pop	{r5, r6, pc}
