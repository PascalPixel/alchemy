.syntax unified
	.thumb
	.section .text.x02008148,"ax",%progbits
	.p2align 2
	.global LinkLobby_PollPeerReady
	.thumb_func
LinkLobby_PollPeerReady:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #352]
	ldr r3, [r3]
	movs r2, #1
	movs r0, #0
	mov r8, r3
	mov r10, r2
	bl 0x0200985c
	movs r2, #224
	ldr r3, [r0, #16]
	lsls r2, r2, #16
	cmp r3, r2
	ble .L_02000148_0
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200981c
.L_02000148_0:
	movs r3, #193
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	beq .L_02000148_1
	movs r0, #0
	bl 0x0200808c
	ldr r0, [pc, #304]
	bl 0x0200980c
	cmp r0, #0
	bne .L_02000148_2
	ldr r2, [pc, #296]
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	cmp r3, #25
	ble .L_02000148_3
	ldr r7, [pc, #288]
	ldr r6, [pc, #292]
	movs r5, #3
.L_02000148_4:
	adds r0, r6, #0
	movs r1, #20
	subs r5, #1
	bl 0x02009908
	adds r6, #24
	cmp r5, #0
	bge .L_02000148_4
	ldr r2, [pc, #264]
	movs r3, #0
	str r3, [r2]
	movs r0, #4
	bl 0x02008128
	b .L_02000148_3
.L_02000148_2:
	ldr r2, [pc, #248]
	movs r3, #0
	str r3, [r2]
.L_02000148_3:
	ldr r3, [pc, #244]
	ldr r3, [r3]
	cmp r3, #0
	bne .L_02000148_5
	movs r0, #0
	bl 0x0200808c
	cmp r0, #0
	beq .L_02000148_7
	movs r0, #1
	bl 0x0200808c
	cmp r0, #0
	bne .L_02000148_9
	movs r0, #2
	bl 0x0200808c
	cmp r0, #0
	beq .L_02000148_7
.L_02000148_9:
	ldr r0, [pc, #216]
	bl 0x02009814
	ldr r0, [pc, #216]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000148_11
	movs r2, #193
	lsls r2, r2, #1
	add r2, r8
	movs r3, #1
	strh r3, [r2]
.L_02000148_11:
	movs r3, #1
	mov r10, r3
	b .L_02000148_5
.L_02000148_7:
	ldr r0, [pc, #184]
	bl 0x0200981c
	movs r2, #0
	mov r10, r2
.L_02000148_5:
	ldr r0, [pc, #176]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000148_1
	ldr r0, [pc, #168]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000148_1
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200980c
	cmp r0, #0
	bne .L_02000148_1
	movs r2, #193
	lsls r2, r2, #1
	add r2, r8
	movs r3, #1
	strh r3, [r2]
.L_02000148_1:
	ldr r0, [pc, #132]
	bl 0x0200980c
	cmp r0, #0
	bne .L_02000148_12
	ldr r0, [pc, #128]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000148_13
.L_02000148_12:
	ldr r0, [pc, #120]
	bl 0x0200980c
	cmp r0, #0
	bne .L_02000148_13
.L_02000148_6:
	movs r0, #0
	bl 0x0200808c
	cmp r0, #0
	bne .L_02000148_13
.L_02000148_8:
	ldr r3, [pc, #80]
	ldr r3, [r3]
	cmp r3, #24
	ble .L_02000148_13
	movs r2, #193
.L_02000148_10:
	lsls r2, r2, #1
	add r2, r8
	movs r3, #2
	strh r3, [r2]
	ldr r0, [pc, #88]
	bl 0x02009814
	ldr r0, [pc, #68]
	bl 0x0200981c
	ldr r0, [pc, #68]
	bl 0x0200981c
	movs r0, #4
	bl 0x02008128
.L_02000148_13:
	ldr r0, [pc, #64]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000148_14
	movs r2, #193
	lsls r2, r2, #1
	add r2, r8
	movs r3, #2
	strh r3, [r2]
.L_02000148_14:
	mov r0, r10
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x00000303
	.4byte 0x02009f4c
	.4byte 0x03000164
	.4byte 0x02002024
	.4byte 0x00000201
	.4byte 0x00000202
	.4byte 0x00000173
	.4byte 0x00000205
	.global LinkLobby_CallIntoCircle
	.thumb_func
LinkLobby_CallIntoCircle:
	push {lr}
	ldr r0, [pc, #88]
	bl 0x0200980c
	cmp r0, #0
	bne 0x0200832e
.L_020002e4:
	ldr r2, [pc, #80]
	ldr r3, [r2]
	movs r1, #150
	adds r3, #1
	lsls r1, r1, #1
.L_020002ee:
	str r3, [r2]
	cmp r3, r1
	bne .L_020002ee_0
	str r0, [r2]
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200981c
.L_020002ee_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200980c
	cmp r0, #0
	bne .L_020002ee_1
	bl 0x02009844
	ldr r0, [pc, #44]
	bl 0x02009884
	movs r1, #0
	movs r0, #8
	bl 0x0200988c
	movs r0, #5
	bl 0x02009714
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02009814
	bl 0x0200984c
.L_020002ee_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0x0203
	.2byte 0x0000
	.2byte 0x9f50
	.2byte 0x0200
	.4byte 0x0000292e
	.section .text.x02008580,"ax",%progbits
	.p2align 2
	.global LinkLobby_SendPartyRecords
	.thumb_func
LinkLobby_SendPartyRecords:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #170
	lsls	r5, r5, #1
	adds	r0, r5, #0
	sub	sp, #32
	bl 0x0200973c
	add	r5, sp, #16
	movs	r1, #0
	mov	r8, r0
	adds	r0, r5, #0
	str	r1, [sp, #4]
	bl 0x0200853c
	mov	r2, sp
	adds	r2, #8
	str	r2, [sp, #0]
	ldr	r1, [sp, #0]
	movs	r6, #150
	mov	r3, sp
	lsls	r6, r6, #2
	mov	fp, r0
	movs	r2, #0
	adds	r3, #15
	mov	ip, r1
.L_020005be:
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r3, ip
	bge.n	.L_020005be
	movs	r7, #0
	cmp	r7, fp
	bge.n	.L_0200068a
	movs	r2, #0
	mov	r9, r5
	mov	sl, r2
.L_020005d2:
	mov	r3, sl
	mov	r1, r9
	ldrh	r0, [r3, r1]
	bl 0x02009804
	movs	r2, #170
	adds	r1, r0, #0
	lsls	r2, r2, #1
	ldr	r3, [pc, #452]
	mov	r0, r8
	bl 0x020098f8
	movs	r2, #149
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #2
	strb	r3, [r2, #0]
	mov	r1, sl
	mov	r3, r9
	ldrh	r2, [r1, r3]
	ldr	r1, [sp, #0]
	adds	r3, r7, #0
	subs	r3, #128
	strb	r3, [r1, r2]
	movs	r1, #170
	mov	r0, r8
	lsls	r1, r1, #1
	bl 0x02009764
	movs	r2, #1
	negs	r2, r2
	movs	r5, #0
	cmp	r0, r2
	bne.n	.L_0200063a
	str	r0, [sp, #4]
	b.n	.L_0200078e
.L_0200061a:
	movs	r0, #1
	subs	r6, #1
	bl 0x02009714
	cmp	r6, #0
	blt.n	.L_02000632
	ldr	r3, [pc, #388]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_0200063a
.L_02000632:
	adds	r5, #1
	cmp	r5, #24
	ble.n	.L_0200063a
	b.n	.L_02000772
.L_0200063a:
	bl 0x02009774
	cmp	r0, #0
	bne.n	.L_0200061a
	movs	r0, #2
	bl 0x02009714
	adds	r7, #1
	movs	r1, #2
	add	sl, r1
	cmp	r7, fp
	blt.n	.L_020005d2
	b.n	.L_0200068a
.L_02000654:
	movs	r0, #1
	subs	r6, #1
	bl 0x02009714
	cmp	r6, #0
	blt.n	.L_0200066c
	ldr	r3, [pc, #328]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_0200067a
.L_0200066c:
	adds	r5, #1
	cmp	r5, #24
	ble.n	.L_0200067a
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #4]
	b.n	.L_0200078e
.L_0200067a:
	bl 0x02009774
	cmp	r0, #0
	bne.n	.L_02000654
	movs	r0, #2
	bl 0x02009714
	adds	r7, #1
.L_0200068a:
	cmp	r7, #2
	bgt.n	.L_020006ae
	movs	r3, #149
	lsls	r3, r3, #1
	add	r3, r8
	movs	r5, #0
	movs	r1, #170
	strb	r5, [r3, #0]
	mov	r0, r8
	lsls	r1, r1, #1
	bl 0x02009764
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_0200067a
	str	r0, [sp, #4]
	b.n	.L_0200078e
.L_020006ae:
	movs	r5, #160
	mov	r0, r8
	lsls	r5, r5, #1
	bl 0x02009744
	adds	r0, r5, #0
	bl 0x0200973c
	mov	r8, r0
	movs	r0, #0
	bl 0x020097fc
	ldr	r3, [pc, #224]
	adds	r1, r0, #0
	adds	r2, r5, #0
	mov	r0, r8
	bl 0x020098f8
	mov	r5, r8
	movs	r2, #132
	lsls	r2, r2, #1
	add	r2, r8
	movs	r1, #0
	ldr	r3, [r2, #0]
	mov	sl, r1
	movs	r7, #150
	movs	r1, #128
	adds	r5, #8
	lsls	r7, r7, #2
	movs	r4, #0
	lsls	r1, r1, #1
	cmp	sl, r3
	bge.n	.L_0200073a
	ldr	r3, [sp, #0]
	adds	r6, r2, #0
	mov	ip, r3
	adds	r0, r5, #0
.L_020006f8:
	ldrb	r3, [r0, #2]
	mov	r2, ip
	ldrb	r3, [r2, r3]
	strb	r3, [r0, #2]
	lsls	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0200072c
	ldr	r3, [r6, #0]
	subs	r3, #1
	cmp	r4, r3
	bge.n	.L_02000722
	ldr	r2, [r5, r1]
	lsls	r3, r4, #2
	subs	r2, #1
	adds	r1, r3, r5
	subs	r2, r2, r4
.L_02000718:
	ldr	r3, [r1, #4]
	subs	r2, #1
	stmia	r1!, {r3}
	cmp	r2, #0
	bne.n	.L_02000718
.L_02000722:
	ldr	r3, [r6, #0]
	subs	r3, #1
	str	r3, [r6, #0]
	subs	r0, #4
	subs	r4, #1
.L_0200072c:
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r3, [r5, r1]
	adds	r4, #1
	adds	r0, #4
	cmp	r4, r3
	blt.n	.L_020006f8
.L_0200073a:
	movs	r1, #160
	mov	r0, r8
	lsls	r1, r1, #1
	bl 0x02009764
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_0200077a
	str	r0, [sp, #4]
	b.n	.L_0200078e
.L_02000750:
	movs	r0, #1
	subs	r7, #1
	bl 0x02009714
	cmp	r7, #0
	blt.n	.L_02000768
	ldr	r3, [pc, #76]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_0200077a
.L_02000768:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #24
	ble.n	.L_0200077a
.L_02000772:
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #4]
	b.n	.L_0200078e
.L_0200077a:
	bl 0x02009774
	cmp	r0, #0
	bne.n	.L_02000750
	movs	r0, #1
	bl 0x02009714
	movs	r0, #2
	bl 0x02009714
.L_0200078e:
	mov	r0, r8
	bl 0x02009744
	ldr	r0, [sp, #4]
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001388
	.2byte 0x1f64
	.2byte 0x0300
	.global LinkLobby_ExchangePartyRecords
	.thumb_func
LinkLobby_ExchangePartyRecords:
	push {r5, r6, lr}
	movs r6, #0
	ldr r0, [pc, #136]
	bl 0x0200980c
	ldr r3, [pc, #136]
	strb r6, [r3]
	cmp r0, #0
	bne .L_020007b0_0
	movs r0, #5
	bl 0x02009714
	bl 0x02008580
	adds r6, r0, #0
	cmp r6, #0
	blt .L_020007b0_1
	movs r0, #5
	bl 0x02009714
	bl 0x02008398
	adds r6, r0, #0
	adds r5, r6, #0
	cmp r6, #0
	bge .L_020007b0_2
	b .L_020007b0_3
.L_020007b0_0:
	bl 0x02008398
	adds r6, r0, #0
	adds r5, r6, #0
	cmp r6, #0
	blt .L_020007b0_1
	movs r0, #10
	bl 0x02009714
	bl 0x02008580
	adds r6, r0, #0
	cmp r6, #0
	blt .L_020007b0_1
.L_020007b0_2:
	movs r0, #252
	lsls r0, r0, #2
	adds r1, r5, #0
	bl 0x0200982c
	adds r6, r5, #0
.L_020007b0_3:
	cmp r5, #0
	bge .L_020007b0_4
.L_020007b0_1:
	ldr r1, [pc, #52]
	ldr r0, [pc, #52]
	ldrh r4, [r0]
	strh r0, [r0]
	movs r2, #0
	movs r3, #128
	strb r3, [r1, #1]
	ldr r3, [pc, #44]
	str r2, [r3]
	ldr r3, [pc, #44]
	strh r2, [r3]
	ldr r3, [pc, #44]
	str r2, [r3]
	ldr r3, [pc, #44]
	strb r2, [r1, #3]
	strb r2, [r1, #2]
	strh r2, [r3]
	strh r4, [r0]
.L_020007b0_4:
	adds r0, r6, #0
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000302
	.4byte 0x020023a0
	.4byte 0x02002220
	.4byte 0x04000208
	.4byte 0x02002080
	.4byte 0x02002008
	.4byte 0x020023ac
	.4byte 0x02002238
	.global LinkLobby_RunConnectionSequence
	.thumb_func
LinkLobby_RunConnectionSequence:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #616]
	ldr r3, [r3]
	movs r2, #0
	ldr r0, [pc, #616]
	mov r8, r2
	movs r7, #0
	mov r10, r3
	movs r6, #0
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000860_0
	bl 0x02009844
	b .L_02000860_1
.L_02000860_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200980c
	cmp r0, #0
	bne .L_02000860_2
	b .L_02000860_3
.L_02000860_2:
	ldr r0, [pc, #580]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000860_4
	b .L_02000860_3
.L_02000860_16:
	movs r2, #193
	lsls r2, r2, #1
	movs r3, #2
	add r2, r10
	strh r3, [r2]
	ldr r0, [pc, #560]
	bl 0x02009814
	ldr r0, [pc, #556]
	bl 0x0200981c
	ldr r0, [pc, #556]
	bl 0x0200981c
	movs r0, #4
	bl 0x02008128
	movs r0, #128
	movs r3, #1
	lsls r0, r0, #2
	mov r8, r3
	bl 0x0200981c
	b .L_02000860_6
.L_02000860_4:
	bl 0x02009844
	ldr r0, [pc, #528]
	bl 0x02009814
	movs r0, #2
	bl 0x02008128
	movs r0, #2
	bl 0x0200808c
	cmp r0, #0
	bne .L_02000860_9
	ldr r0, [pc, #512]
	movs r1, #5
	movs r2, #4
	movs r3, #1
	bl 0x020097c4
	adds r7, r0, #0
	b .L_02000860_9
.L_02000860_17:
	movs r0, #1
	bl 0x02009714
	ldr r0, [pc, #476]
	movs r5, #0
	bl 0x0200980c
	cmp r0, #0
	bne .L_02000860_10
	movs r5, #1
.L_02000860_10:
	ldr r0, [pc, #460]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000860_11
	movs r5, #1
.L_02000860_11:
	movs r0, #2
	bl 0x0200808c
	cmp r0, #0
	bne .L_02000860_13
	movs r0, #1
	bl 0x0200808c
	cmp r0, #0
	bne .L_02000860_13
	adds r6, #1
	cmp r6, #25
	ble .L_02000860_15
	movs r5, #1
	b .L_02000860_15
.L_02000860_13:
	movs r6, #0
.L_02000860_15:
	cmp r5, #0
	bne .L_02000860_16
.L_02000860_9:
	movs r0, #2
	bl 0x0200808c
	cmp r0, #0
	beq .L_02000860_17
.L_02000860_6:
	cmp r7, #0
	beq .L_02000860_18
	adds r0, r7, #0
	movs r1, #1
	bl 0x020097b4
.L_02000860_18:
	movs r0, #5
	bl 0x02009714
.L_02000860_1:
	mov r2, r8
	cmp r2, #0
	beq .L_02000860_19
	b .L_02000860_20
.L_02000860_19:
	movs r1, #249
	lsls r1, r1, #3
	movs r0, #54
	bl 0x0200972c
	ldr r5, [pc, #384]
	adds r6, r0, #0
.L_02000860_8:
	adds r0, r5, #0
	bl 0x02009724
	movs r0, #5
	bl 0x02009794
	movs r0, #8
	bl 0x02009714
	movs r0, #5
	bl 0x0200979c
	ldr r0, [pc, #332]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000860_5
	ldr r3, [pc, #352]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #8
	bl 0x0200987c
	ldr r0, [pc, #336]
	bl 0x02009884
.L_02000860_12:
	movs r1, #0
	movs r0, #8
	bl 0x0200988c
	movs r0, #45
.L_02000860_14:
	bl 0x02009714
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009864
	movs r1, #216
	movs r2, #184
	movs r0, #0
	bl 0x0200986c
	movs r0, #0
	bl 0x02009874
	movs r0, #0
	movs r1, #216
	movs r2, #168
	bl 0x0200986c
	movs r0, #0
	bl 0x02009874
	b .L_02000860_21
.L_02000860_5:
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009864
	movs r1, #216
	movs r2, #200
	movs r0, #0
	bl 0x0200986c
	movs r0, #0
	bl 0x02009874
.L_02000860_7:
	movs r0, #0
	ldr r1, [pc, #240]
	ldr r2, [pc, #244]
	bl 0x02009864
	movs r0, #0
	movs r1, #216
	movs r2, #168
	bl 0x0200986c
	bl 0x020087b0
	cmp r0, #0
	bge .L_02000860_22
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009864
	movs r2, #200
	movs r1, #216
	movs r0, #0
	bl 0x0200986c
	movs r0, #5
	bl 0x02009794
	movs r0, #8
	bl 0x02009714
	movs r0, #5
	bl 0x0200979c
	movs r0, #0
	bl 0x02009874
	movs r0, #54
	bl 0x02009734
	movs r0, #0
	bl 0x02008128
	movs r0, #4
	bl 0x02008128
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x0200971c
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200977c
	ldr r0, [pc, #104]
	bl 0x0200981c
	ldr r0, [pc, #100]
	bl 0x0200981c
	ldr r0, [pc, #128]
	bl 0x0200981c
	ldr r0, [pc, #92]
	bl 0x0200981c
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200981c
	movs r2, #193
	lsls r2, r2, #1
	add r2, r10
	movs r3, #2
	strh r3, [r2]
	b .L_02000860_20
.L_02000860_22:
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x02009864
	movs r0, #0
	bl 0x02009874
.L_02000860_21:
	ldr r0, [pc, #32]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000860_25
	ldr r5, [pc, #68]
	movs r1, #8
	adds r0, r5, #0
	bl 0x020098ac
	adds r0, r5, #0
	movs r1, #9
	bl 0x020098b4
	b .L_02000860_26
	.4byte 0x03001ebc
	.4byte 0x00000173
	.4byte 0x00000205
	.4byte 0x00000201
	.4byte 0x00000202
	.4byte 0x00000203
	.4byte 0x00002928
	.4byte 0x02008149
	.4byte 0x02000240
	.4byte 0x0000293b
	.4byte 0x00001999
	.4byte 0x00000ccc
	.4byte 0x00000303
	.4byte 0x000000be
.L_02000860_25:
	ldr r5, [pc, #96]
	movs r1, #10
	adds r0, r5, #0
	bl 0x020098ac
	adds r0, r5, #0
	movs r1, #11
	bl 0x020098b4
.L_02000860_26:
	ldr r3, [pc, #84]
	ldr r2, [pc, #84]
	adds r3, r3, r2
	movs r2, #4
	strb r2, [r3]
	movs r0, #1
	movs r1, #1
	bl 0x020098a4
	ldr r2, [pc, #72]
	ldr r3, [pc, #48]
	ldr r1, [pc, #48]
	strh r3, [r2, #2]
	ldr r3, [pc, #48]
	strh r1, [r2]
	strh r1, [r2, #4]
	strh r3, [r2, #6]
	ldr r4, [pc, #60]
	ldr r2, [pc, #64]
	movs r1, #0
	adds r0, r6, #0
.L_02000860_27:
	ldrb r3, [r0]
	adds r1, #1
	strb r3, [r2]
	adds r0, #1
	adds r2, #1
	cmp r1, r4
	bls .L_02000860_27
	movs r0, #54
	bl 0x02009734
.L_02000860_20:
	bl 0x0200984c
	b .L_02000860_3
	.2byte 0x0000
	.4byte 0x00000058
	.4byte 0x00000045
	.4byte 0x00000043
	.4byte 0x000000be
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x02002224
	.4byte 0x000007c7
.L_02000860_23:
	.4byte 0x02018000
.L_02000860_3:
	pop {r3, r5}
.L_02000860_24:
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.global LinkLobby_RunBattleApplication
	.thumb_func
LinkLobby_RunBattleApplication:
	push {r5, r6, r7, lr}
	ldr r6, [pc, #432]
	bl 0x02009844
	ldr r7, [pc, #428]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r0, #8
	ldr r1, [r3]
	movs r2, #0
	bl 0x0200987c
	movs r0, #0
	bl 0x0200808c
	cmp r0, #0
	bne .L_02000b94_1
	movs r0, #1
	bl 0x02009714
.L_02000b94_1:
	movs r0, #0
	bl 0x0200808c
	cmp r0, #0
	bne .L_02000b94_2
	movs r0, #5
	bl 0x02008128
	bl 0x0200803c
	ldr r0, [pc, #380]
	bl 0x0200980c
	cmp r0, #0
	bne .L_02000b94_5
	adds r0, r6, #5
	bl 0x02009884
	movs r1, #0
	movs r0, #8
	bl 0x0200988c
	movs r0, #0
	movs r1, #0
	bl 0x02009854
	adds r5, r0, #0
	cmp r5, #0
	bne .L_02000b94_6
	movs r0, #250
	movs r1, #0
	lsls r0, r0, #2
	bl 0x0200982c
	ldr r0, [pc, #332]
	bl 0x02009814
	movs r0, #185
	lsls r0, r0, #1
.L_02000b94_4:
	bl 0x0200981c
	movs r0, #182
	lsls r0, r0, #1
	bl 0x0200981c
	ldr r0, [pc, #312]
	bl 0x02009814
	ldr r2, [pc, #312]
	adds r3, r7, r2
	adds r0, r6, #7
	strh r5, [r3]
	b .L_02000b94_7
.L_02000b94_6:
	ldr r0, [pc, #292]
	bl 0x0200981c
	movs r0, #182
	lsls r0, r0, #1
	bl 0x02009814
	movs r0, #0
	bl 0x02008128
	adds r0, r6, #6
.L_02000b94_0:
	b .L_02000b94_7
.L_02000b94_2:
	ldr r0, [pc, #268]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000b94_8
	movs r0, #0
	bl 0x02008128
	ldr r0, [pc, #264]
	bl 0x02009884
	movs r1, #0
	movs r0, #8
	bl 0x0200988c
	ldr r0, [pc, #244]
	bl 0x0200981c
	ldr r0, [pc, #232]
	bl 0x0200981c
.L_02000b94_8:
	ldr r0, [pc, #232]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000b94_9
.L_02000b94_5:
	adds r0, r6, #3
.L_02000b94_7:
	bl 0x02009884
.L_02000b94_18:
	movs r0, #8
	movs r1, #0
	bl 0x0200988c
	b .L_02000b94_10
.L_02000b94_9:
	ldr r0, [pc, #216]
	bl 0x0200980c
	cmp r0, #0
	bne .L_02000b94_11
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200980c
	cmp r0, #0
	bne .L_02000b94_11
	adds r0, r6, #0
	bl 0x02009884
	movs r0, #8
	movs r1, #0
	bl 0x0200988c
	movs r0, #192
	lsls r0, r0, #2
	bl 0x02009814
	b .L_02000b94_10
.L_02000b94_11:
	movs r0, #192
	lsls r0, r0, #2
	bl 0x02009814
	ldr r0, [pc, #164]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000b94_12
	adds r0, r6, #2
	bl 0x02009884
	b .L_02000b94_13
.L_02000b94_12:
	adds r0, r6, #1
	bl 0x02009884
.L_02000b94_13:
	movs r1, #0
	movs r0, #8
	bl 0x0200988c
	movs r0, #0
	movs r1, #0
	bl 0x02009854
	cmp r0, #0
	bne .L_02000b94_14
	movs r0, #0
	bl 0x0200808c
	cmp r0, #0
	beq .L_02000b94_15
	movs r0, #182
	lsls r0, r0, #1
.L_02000b94_3:
	bl 0x02009814
	movs r0, #185
	lsls r0, r0, #1
	bl 0x02009814
	ldr r0, [pc, #92]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000b94_16
	adds r0, r6, #3
	bl 0x02009884
	b .L_02000b94_17
.L_02000b94_16:
	adds r0, r6, #4
	bl 0x02009884
.L_02000b94_17:
	movs r0, #1
	bl 0x02008128
	ldr r0, [pc, #52]
	bl 0x02009814
	b .L_02000b94_18
.L_02000b94_15:
	ldr r0, [pc, #60]
	bl 0x02009814
	b .L_02000b94_10
.L_02000b94_14:
	adds r0, r6, #0
	bl 0x02009884
	movs r0, #8
	movs r1, #0
	bl 0x0200988c
.L_02000b94_10:
	bl 0x0200984c
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00002930
	.4byte 0x02000240
	.4byte 0x00000173
	.4byte 0x00000202
	.4byte 0x000002aa
	.4byte 0x0000293d
	.4byte 0x00000201
	.4byte 0x00000205
	.section .text.x02008f8c,"ax",%progbits
	.p2align 2
	.global LinkLobby_TalkByProgress
	.thumb_func
LinkLobby_TalkByProgress:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r0, #0
	ldr r5, [pc, #148]
	bl 0x02008f30
	mov r8, r0
	adds r0, r6, #0
	bl 0x02008f30
	adds r7, r0, #0
	bl 0x02009844
	ldr r3, [pc, #132]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r0, r6, #0
	ldr r1, [r3]
	movs r2, #0
	bl 0x0200987c
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000f8c_0
	movs r0, #188
	lsls r0, r0, #2
	bl 0x0200980c
	movs r3, #188
	lsls r3, r3, #2
	adds r0, r6, r3
	bl 0x0200980c
	adds r5, r0, #0
	ldr r0, [pc, #84]
	bl 0x0200980c
	cmp r0, #0
	beq .L_02000f8c_1
	cmp r5, #0
	beq .L_02000f8c_2
	ldr r5, [pc, #76]
	b .L_02000f8c_3
.L_02000f8c_2:
	ldr r5, [pc, #76]
	b .L_02000f8c_3
.L_02000f8c_1:
	cmp r5, #0
	beq .L_02000f8c_4
	ldr r5, [pc, #72]
	b .L_02000f8c_3
.L_02000f8c_4:
	ldr r5, [pc, #72]
	b .L_02000f8c_3
.L_02000f8c_0:
	mov r2, r8
	cmp r2, #0
	beq .L_02000f8c_5
	cmp r7, #0
	bne .L_02000f8c_3
	ldr r5, [pc, #60]
	b .L_02000f8c_3
.L_02000f8c_5:
	ldr r5, [pc, #60]
.L_02000f8c_3:
	adds r0, r5, r6
	subs r0, #1
	bl 0x02009884
	adds r0, r6, #0
	movs r1, #0
	bl 0x02009894
	bl 0x0200984c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x0000294e
	.4byte 0x02000240
	.4byte 0x00000305
	.4byte 0x00002967
	.4byte 0x0000296c
	.4byte 0x00002971
	.4byte 0x00002976
	.4byte 0x00002953
	.4byte 0x00002958
	.global LinkLobby_StartExchange
	.thumb_func
LinkLobby_StartExchange:
	push {lr}
	bl 0x02009754
	movs r0, #2
	bl 0x0200975c
	ldr r0, [pc, #8]
	movs r1, #1
	bl 0x0200989c
	pop {r1}
	bx r1
	.4byte 0x00000001
	.global LinkLobby_TalkToAttendant
	.thumb_func
LinkLobby_TalkToAttendant:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	movs r6, #0
	bl 0x02009844
	cmp r5, #13
	beq .L_0200106c_0
	cmp r5, #13
	bgt .L_0200106c_1
	cmp r5, #12
	bne .L_0200106c_1
	ldr r7, [pc, #80]
	b .L_0200106c_2
.L_0200106c_0:
	ldr r7, [pc, #80]
	b .L_0200106c_2
.L_0200106c_1:
	ldr r7, [pc, #80]
.L_0200106c_2:
	ldr r3, [pc, #80]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r0, r5, #0
	ldr r1, [r3]
	movs r2, #0
	bl 0x0200987c
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200980c
	cmp r0, #0
	beq .L_0200106c_3
	ldr r0, [pc, #56]
	bl 0x0200980c
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	adds r6, r3, #0
	movs r3, #2
	subs r6, r3, r6
.L_0200106c_3:
	adds r0, r7, r6
	bl 0x02009884
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200988c
	bl 0x0200984c
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x00002985
	.4byte 0x0000297f
	.4byte 0x00002982
	.4byte 0x02000240
	.4byte 0x00000305
	.global LinkLobby_TalkAlternating
	.thumb_func
LinkLobby_TalkAlternating:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl 0x02009844
	ldr r3, [pc, #84]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r0, r6, #0
	ldr r1, [r3]
	movs r2, #0
	bl 0x0200987c
	movs r0, #129
	lsls r0, r0, #2
	bl 0x0200980c
	cmp r0, #0
	bne .L_020010e8_0
	bl 0x02009834
	cmp r0, #3
	bgt .L_020010e8_1
	ldr r5, [pc, #52]
	b .L_020010e8_2
.L_020010e8_1:
	ldr r5, [pc, #52]
.L_020010e8_2:
	movs r0, #129
	lsls r0, r0, #2
	bl 0x02009814
	b .L_020010e8_3
.L_020010e8_0:
	movs r0, #129
	lsls r0, r0, #2
	ldr r5, [pc, #40]
	bl 0x0200981c
.L_020010e8_3:
	adds r0, r5, #0
	bl 0x02009884
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200988c
	bl 0x0200984c
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000298d
	.4byte 0x0000298c
	.4byte 0x0000298e
	.section .text.x020091bc,"ax",%progbits
	.p2align 2
	.global Scene_ShowDialoguePair292a
	.thumb_func
Scene_ShowDialoguePair292a:
	push {r5, lr}
	movs r0, #85
	bl 0x020098e4
	ldr r0, [pc, #88]
	movs r1, #5
	movs r2, #4
	movs r3, #1
.L_020011cc:
	bl 0x020097c4
	adds r5, r0, #0
	b .L_020011cc_0
.L_020011cc_1:
	movs r0, #1
	bl 0x02009714
.L_020011cc_0:
	bl 0x020097cc
	cmp r0, #0
	beq .L_020011cc_1
	bl 0x020097f4
	adds r0, r5, #0
	movs r1, #1
	bl 0x020097b4
	movs r0, #1
	bl 0x02009714
	ldr r0, [pc, #44]
	movs r1, #5
	movs r2, #4
	movs r3, #1
	bl 0x020097c4
	adds r5, r0, #0
	b .L_020011cc_2
.L_020011cc_3:
	movs r0, #1
	bl 0x02009714
.L_020011cc_2:
	bl 0x020097cc
	cmp r0, #0
	beq .L_020011cc_3
	adds r0, r5, #0
	movs r1, #1
	bl 0x020097b4
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x292a
	.2byte 0x0000
	.4byte 0x0000292b
	.global Scene_ShowDialoguePair292c
	.thumb_func
Scene_ShowDialoguePair292c:
	push {r5, lr}
	movs r0, #85
	bl 0x020098e4
	ldr r0, [pc, #88]
	movs r1, #5
	movs r2, #4
	movs r3, #1
	bl 0x020097c4
	adds r5, r0, #0
	b .L_02001228_0
.L_02001228_1:
	movs r0, #1
	bl 0x02009714
.L_02001228_0:
	bl 0x020097cc
	cmp r0, #0
	beq .L_02001228_1
	bl 0x020097f4
	adds r0, r5, #0
	movs r1, #1
	bl 0x020097b4
	movs r0, #1
	bl 0x02009714
	ldr r0, [pc, #44]
	movs r1, #5
	movs r2, #4
	movs r3, #1
	bl 0x020097c4
	adds r5, r0, #0
	b .L_02001228_2
.L_02001228_3:
	movs r0, #1
	bl 0x02009714
.L_02001228_2:
	bl 0x020097cc
	cmp r0, #0
	beq .L_02001228_3
	adds r0, r5, #0
	movs r1, #1
	bl 0x020097b4
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x0000292c
	.4byte 0x0000292d
	.global Scene_DrawThreeDigitValue
	.thumb_func
Scene_DrawThreeDigitValue:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #68]
	adds r5, r0, #0
	sub sp, #8
	cmp r5, r3
	ble .L_02001294_0
	adds r5, r3, #0
.L_02001294_0:
	movs r6, #0
	movs r7, #1
.L_02001294_1:
	adds r0, r5, #0
	movs r1, #10
	bl 0x0200970c
	movs r2, #16
	adds r1, r0, #0
	subs r2, r2, r6
	movs r0, #27
	movs r3, #8
	str r7, [sp, #0]
	str r7, [sp, #4]
	bl 0x0200978c
	adds r0, r5, #0
	movs r1, #10
	bl 0x02009704
	adds r6, #1
	adds r5, r0, #0
	cmp r6, #2
	ble .L_02001294_1
	bl 0x02009784
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x000003e7
	.global LinkLobby_RunRoundResult
	.thumb_func
LinkLobby_RunRoundResult:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #148]
	movs r2, #0
	str r2, [r3]
	ldr r3, [pc, #144]
	str r2, [r3]
	ldr r3, [pc, #144]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #65
	str r2, [r3]
	movs r0, #2
	sub sp, #8
	bl 0x0200975c
	ldr r3, [pc, #128]
	movs r2, #172
	lsls r2, r2, #2
	adds r3, r3, r2
	ldrh r0, [r3]
	bl 0x02009294
	movs r3, #13
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r1, #11
	movs r0, #11
	bl 0x020097a4
	movs r0, #4
	bl 0x02008128
	movs r0, #1
	bl 0x02009714
	movs r0, #5
	bl 0x0200979c
	ldr r2, [pc, #80]
	ldr r3, [pc, #44]
	strh r3, [r2, #8]
	ldr r3, [pc, #44]
	strh r3, [r2, #10]
	ldr r3, [pc, #44]
	strh r3, [r2, #12]
	ldr r3, [pc, #44]
	strh r3, [r2, #14]
	movs r6, #0
.L_020012e0_2:
	movs r3, #188
	lsls r3, r3, #2
	adds r5, r6, r3
	adds r0, r5, #0
	bl 0x0200981c
	adds r0, r6, #0
	bl 0x02008f30
	cmp r0, #0
	beq .L_020012e0_1
	adds r0, r5, #0
	bl 0x02009814
	b .L_020012e0_1
	.4byte 0x00000054
	.4byte 0x00000041
	.4byte 0x0000004c
	.4byte 0x0000004b
	.4byte 0x02009f50
	.4byte 0x02009f4c
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x02002224
.L_020012e0_1:
	adds r6, #1
	cmp r6, #7
	ble .L_020012e0_2
	ldr r6, [pc, #816]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #8
	beq .L_020012e0_3
	b .L_020012e0_4
.L_020012e0_3:
	bl 0x02009844
	bl 0x020098bc
	bl 0x020098c4
	movs r0, #5
	bl 0x02008128
	movs r3, #169
	lsls r3, r3, #2
	adds r2, r6, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	ldr r3, [pc, #772]
	adds r2, r6, r3
	ldrh r3, [r2]
	movs r0, #254
	adds r3, #1
	strh r3, [r2]
	lsls r0, r0, #2
	bl 0x02009824
	lsls r0, r0, #24
	asrs r6, r0, #24
	lsls r3, r6, #1
	adds r5, r3, #2
	cmp r5, #14
	ble .L_020012e0_6
	movs r5, #14
.L_020012e0_6:
	movs r0, #250
	lsls r0, r0, #2
	bl 0x02009824
	cmp r0, #2
	bne .L_020012e0_7
	movs r0, #250
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200982c
	adds r6, #1
	adds r5, #1
	b .L_020012e0_8
.L_020012e0_7:
	adds r1, r0, #1
	movs r0, #250
	lsls r0, r0, #2
	bl 0x0200982c
.L_020012e0_8:
	ldr r7, [pc, #696]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #8
	bl 0x0200987c
	ldr r0, [pc, #688]
	adds r0, r5, r0
	bl 0x02009884
	movs r1, #0
	movs r0, #8
	bl 0x0200988c
	movs r0, #0
	movs r1, #0
	bl 0x02009854
	cmp r0, #0
	bne .L_020012e0_9
	cmp r6, #90
	ble .L_020012e0_10
	movs r6, #90
.L_020012e0_10:
	movs r0, #254
	lsls r0, r0, #2
	adds r1, r6, #0
	bl 0x0200982c
	b .L_020012e0_11
.L_020012e0_9:
	ldr r0, [pc, #644]
	bl 0x0200981c
	movs r0, #254
.L_020012e0_0:
	movs r1, #1
	lsls r0, r0, #2
	negs r1, r1
	bl 0x0200982c
	ldr r3, [pc, #620]
	adds r5, r7, r3
	ldrh r0, [r5]
	movs r1, #5
	bl 0x020097ec
	movs r3, #170
	lsls r3, r3, #2
	adds r2, r7, r3
	ldrh r5, [r5]
	ldrh r3, [r2]
	cmp r3, r5
	bcs .L_020012e0_12
	strh r5, [r2]
	ldr r0, [pc, #604]
	bl 0x02009884
	movs r1, #0
	movs r0, #8
	bl 0x0200988c
	bl 0x02009228
	b .L_020012e0_13
.L_020012e0_12:
	ldr r0, [pc, #588]
	bl 0x02009884
	movs r0, #8
	movs r1, #0
	bl 0x0200988c
.L_020012e0_13:
	movs r0, #0
	bl 0x02008128
	b .L_020012e0_11
.L_020012e0_4:
	cmp r3, #9
	bne .L_020012e0_15
	ldr r3, [pc, #564]
	adds r2, r6, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	bl 0x02009844
	bl 0x020098bc
	bl 0x020098c4
	movs r0, #5
	bl 0x02008128
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r6, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #8
	bl 0x0200987c
	ldr r3, [pc, #500]
	adds r5, r6, r3
	ldrh r0, [r5]
	movs r1, #5
	bl 0x020097ec
.L_020012e0_5:
	movs r3, #170
	lsls r3, r3, #2
	adds r2, r6, r3
	ldrh r5, [r5]
	ldrh r3, [r2]
	cmp r3, r5
	bcs .L_020012e0_16
	strh r5, [r2]
	ldr r0, [pc, #484]
	bl 0x02009884
	movs r1, #0
	movs r0, #8
	bl 0x0200988c
	bl 0x02009228
	b .L_020012e0_17
.L_020012e0_16:
	ldr r0, [pc, #476]
	bl 0x02009884
	movs r0, #8
	movs r1, #0
	bl 0x0200988c
.L_020012e0_17:
	ldr r3, [pc, #436]
	ldr r2, [pc, #436]
	adds r3, r3, r2
	movs r2, #0
	strh r2, [r3]
	ldr r0, [pc, #436]
	bl 0x0200981c
	movs r0, #254
	movs r1, #1
	lsls r0, r0, #2
	negs r1, r1
	bl 0x0200982c
	movs r0, #0
.L_020012e0_23:
	bl 0x02008128
.L_020012e0_11:
	bl 0x0200984c
	b .L_020012e0_19
.L_020012e0_15:
	cmp r3, #10
	bne .L_020012e0_20
	bl 0x02009844
	bl 0x020098bc
	bl 0x020098c4
	movs r0, #0
	bl 0x02008128
	movs r0, #4
	bl 0x02008128
	movs r0, #250
	lsls r0, r0, #2
	bl 0x0200980c
	cmp r0, #0
	beq .L_020012e0_22
	ldr r3, [pc, #388]
	movs r0, #250
	lsls r0, r0, #2
	ldr r5, [r3]
	bl 0x0200981c
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r0, #193
	movs r3, #2
	strh r3, [r2]
	lsls r0, r0, #2
	bl 0x0200981c
	movs r0, #20
	bl 0x02009714
	bl 0x0200803c
	movs r0, #0
	bl 0x02008128
	movs r0, #4
	b .L_020012e0_23
.L_020012e0_22:
	movs r3, #171
	lsls r3, r3, #2
	adds r2, r6, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	ldr r3, [pc, #328]
	adds r2, r6, r3
	ldrh r3, [r2]
	adds r1, r3, #1
	strh r1, [r2]
	movs r2, #172
	lsls r2, r2, #2
	adds r5, r6, r2
	lsls r3, r1, #16
	ldrh r2, [r5]
	lsrs r3, r3, #16
	cmp r2, r3
	bcs .L_020012e0_24
	strh r1, [r5]
.L_020012e0_24:
	ldrh r0, [r5]
	bl 0x02009294
	bl 0x020091bc
	movs r0, #193
.L_020012e0_14:
	lsls r0, r0, #2
	bl 0x02009814
	ldr r0, [pc, #288]
	bl 0x02009814
	b .L_020012e0_11
.L_020012e0_20:
	cmp r3, #11
	bne .L_020012e0_25
	bl 0x02009844
	bl 0x020098bc
	bl 0x020098c4
	movs r0, #0
	bl 0x02008128
	movs r0, #4
	bl 0x02008128
	ldr r0, [pc, #224]
	bl 0x0200980c
	cmp r0, #0
	bne .L_020012e0_26
	ldr r2, [pc, #244]
	adds r3, r6, r2
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	ldr r2, [pc, #228]
	adds r3, r6, r2
	strh r0, [r3]
	bl 0x020091bc
.L_020012e0_26:
	movs r0, #193
	lsls r0, r0, #2
	bl 0x02009814
	ldr r0, [pc, #212]
	bl 0x0200981c
	b .L_020012e0_11
.L_020012e0_25:
	bl 0x0200974c
	movs r0, #185
	lsls r0, r0, #1
	bl 0x0200981c
	movs r0, #254
	movs r1, #1
	lsls r0, r0, #2
	negs r1, r1
	bl 0x0200982c
	ldr r3, [pc, #188]
	adds r5, r6, r3
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_020012e0_27
	bl 0x02009844
	bl 0x020098bc
	bl 0x020098c4
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r6, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #8
.L_020012e0_18:
	bl 0x0200987c
	ldr r0, [pc, #156]
	bl 0x02009884
	movs r0, #8
	movs r1, #0
	bl 0x02009894
	bl 0x0200984c
.L_020012e0_27:
	ldr r3, [pc, #140]
	movs r2, #0
	strb r2, [r5]
.L_020012e0_21:
	movs r0, #0
	strb r2, [r3]
	bl 0x02008128
	movs r0, #4
	bl 0x02008128
.L_020012e0_19:
	ldr r5, [pc, #124]
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x0200971c
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200977c
	ldr r3, [pc, #44]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #8
	bne .L_020012e0_28
	ldr r0, [pc, #40]
	bl 0x0200980c
	cmp r0, #0
	bne .L_020012e0_29
.L_020012e0_28:
	movs r0, #1
	bl 0x0200983c
	bl 0x020098d4
.L_020012e0_29:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000002aa
	.4byte 0x0000293e
	.4byte 0x00000173
	.4byte 0x0000293c
	.4byte 0x00002939
	.4byte 0x000002a6
	.4byte 0x0000293a
	.4byte 0x03001ebc
	.4byte 0x000002b2
	.4byte 0x00000305
	.4byte 0x000002ae
	.4byte 0x0000022a
	.4byte 0x00002929
	.4byte 0x03001d08
	.4byte 0x02008149
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
	.global LinkLobby_SlotValues
LinkLobby_SlotValues:
	.4byte 0x434d4753
	.4byte 0x33323130
	.4byte 0x31434241
	.4byte 0x32454443
	.4byte 0x33474645
	.4byte 0x434d4753
	.global LinkLobby_SlotColumns
LinkLobby_SlotColumns:
	.4byte 0x01010100
	.4byte 0x00000001
	.global gLinkLobbyEntrances
gLinkLobbyEntrances:
	.4byte 0xffff000b
	.4byte 0x00000080
	.4byte 0xc00000a4
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff000a
	.4byte 0x00000160
	.4byte 0xc0000094
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0008
	.4byte 0x000000d8
	.4byte 0x800000c8
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0009
	.4byte 0x000000d8
	.4byte 0x800000c8
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0001
	.4byte 0x000000d8
	.4byte 0xc0000120
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0000
	.4byte 0x000000d8
	.4byte 0xc00000d0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gLinkLobbyExits
gLinkLobbyExits:
	.4byte 0x000001ff
	.global gLinkLobbyPlacements
gLinkLobbyPlacements:
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x01004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00014000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00010000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00010000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00014000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x005c0000
	.4byte 0x00014000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00010000
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00010000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00018000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00a40000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001c000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00014000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00010000
	.4byte 0x10010001
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00014000
	.4byte 0x10020002
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0x10030003
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00018000
	.4byte 0x10050005
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gLinkLobbyBattlePlacements
gLinkLobbyBattlePlacements:
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x01004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00014000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00010000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00010000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x005c0000
	.4byte 0x00014000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00010000
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00014000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x0001c000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00018000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001c000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00014000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00010000
	.4byte 0x10010001
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00010000
	.4byte 0x10020002
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00018000
	.4byte 0x10030003
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00014000
	.4byte 0x10050005
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gLinkLobbyEvents
gLinkLobbyEvents:
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x02008f8d
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x02008f8d
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x02008f8d
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte 0x02008f8d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008b95
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008d69
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008f19
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000292f
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0200906d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200906d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0200906d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008e11
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020090e9
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0000298f
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002991
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002992
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002993
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002990
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02009159
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002994
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00002995
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008861
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02009051
	.4byte 0x00000006
	.4byte 0xffff0001
	.4byte 0x020082d9
	.4byte 0x00000006
	.4byte 0xffff0002
	.4byte 0x02008341
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
