.syntax unified
	.thumb
	.global SerialRuntime_StepBlockTransfer
	.thumb_func
SerialRuntime_StepBlockTransfer:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_080065a8
	ldr r3, [r3]
	lsls r3, r3, #26
	lsrs r3, r3, #30
	movs r2, #1
	bics r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, .L_080065ac
	lsls r3, r3, #3
	adds r3, r3, r2
	mov r12, r3
	ldr r3, .L_080065b0
	ldrh r2, [r3]
	ldr r1, .L_080065a4
	movs r3, #3
	ands r3, r2
	mov r8, r1
	ldr r6, .L_080065b4
	cmp r3, #3
	beq .L_0800658e
	b .L_0800676e
.L_0800658e:
	ldr r7, .L_080065b8
	ldr r5, [r7]
	cmp r5, #0
	bne .L_08006598
	b .L_0800669a
.L_08006598:
	ldrb r3, [r6, #2]
	cmp r3, #1
	beq .L_080065a0
	b .L_08006696
.L_080065a0:
	b .L_080065bc
	.2byte 0x0000
.L_080065a4:
	.4byte 0x00000001
.L_080065a8:
	.4byte 0x04000128
.L_080065ac:
	.4byte gSerialPeerPayloads
.L_080065b0:
	.4byte gLinkStatus
.L_080065b4:
	.4byte gSerialTransfer
.L_080065b8:
	.4byte gSerialReceiveDest
.L_080065bc:
	mov r2, r12
	ldrb r3, [r2, #3]
	movs r1, #128
	adds r3, #255
	lsls r3, r3, #24
	lsls r1, r1, #17
	cmp r3, r1
	bhi .L_08006696
	ldr r0, .L_08006778
	movs r2, #127
	mov lr, r2
	mov r3, r12
	ldrb r1, [r0]
	ldrb r2, [r3]
	mov r3, lr
	ands r3, r1
	cmp r2, r3
	bne .L_08006650
	movs r1, #0
	mov lr, r1
	mov r2, lr
	strb r2, [r6]
	mov r3, r12
	ldrb r4, [r3, #3]
	cmp r4, #1
	beq .L_080065f6
	cmp r4, #2
	beq .L_08006622
	b .L_08006642
.L_080065f6:
	mov r0, r12
	ldr r3, .L_0800677c
	adds r0, #4
	adds r1, r5, #0
	ldr r2, .L_08006780
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [r7]
	ldr r2, .L_08006784
	adds r3, #20
	str r3, [r7]
	ldrh r3, [r2]
	adds r3, #20
	strh r3, [r2]
	movs r1, #128
	ldrb r3, [r6, #1]
	negs r1, r1
	adds r3, #1
	adds r2, r1, #0
	orrs r3, r2
	strb r3, [r6, #1]
	b .L_08006642
.L_08006622:
	mov r0, r12
	ldr r3, .L_0800677c
	adds r0, #4
	adds r1, r5, #0
	ldr r2, .L_08006780
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_08006784
	ldrh r3, [r2]
	adds r3, #20
	strh r3, [r2]
	mov r2, lr
	mov r3, r8
	strb r4, [r6, #2]
	strb r2, [r6, #1]
	strb r3, [r6]
.L_08006642:
	ldr r3, .L_08006778
	ldrb r2, [r3]
	movs r1, #127
	adds r2, #1
	ands r2, r1
	strb r2, [r3]
	b .L_0800669a
.L_08006650:
	ldrb r2, [r0]
	movs r4, #128
	adds r3, r4, #0
	ands r3, r2
	movs r1, #128
	cmp r3, #0
	beq .L_08006688
	ldrb r1, [r6]
	adds r3, r4, #0
	ands r3, r1
	lsls r3, r3, #24
	lsrs r2, r3, #24
	cmp r2, #0
	beq .L_08006672
	mov r1, r8
	strb r1, [r6]
	b .L_0800669a
.L_08006672:
	lsls r3, r1, #24
	movs r1, #128
	lsls r1, r1, #17
	cmp r3, r1
	bne .L_0800669a
	strb r2, [r6]
	ldrb r2, [r0]
	mov r3, lr
	ands r3, r2
	strb r3, [r0]
	b .L_0800669a
.L_08006688:
	ldrb r3, [r0]
	orrs r3, r1
	strb r3, [r6]
	ldrb r3, [r0]
	orrs r3, r1
	strb r3, [r0]
	b .L_0800669a
.L_08006696:
	movs r3, #0
	strb r3, [r6]
.L_0800669a:
	ldr r7, .L_08006788
	ldr r0, [r7]
	cmp r0, #0
	beq .L_08006748
	mov r2, r12
	ldrb r2, [r2, #2]
	mov lr, r2
	cmp r2, #1
	bne .L_0800672e
	mov r3, r12
	ldrb r2, [r3]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080066e4
	ldr r5, .L_08006778
	mov r2, r12
	ldrb r1, [r5]
	ldrb r3, [r2]
	movs r4, #127
	subs r1, r1, r3
	ands r1, r4
	lsls r2, r1, #2
	adds r2, r2, r1
	lsls r2, r2, #2
	subs r3, r0, r2
	ldr r0, .L_0800678c
	str r3, [r7]
	ldrh r3, [r0]
	adds r3, r3, r2
	strh r3, [r0]
	ldrb r3, [r5]
	subs r3, r3, r1
	strb r3, [r5]
	ldrb r3, [r5]
	ands r4, r3
	strb r4, [r5]
.L_080066e4:
	ldr r4, .L_0800678c
	ldrh r3, [r4]
	cmp r3, #0
	beq .L_0800672e
	ldr r3, .L_0800677c
	ldr r0, [r7]
	adds r1, r6, #4
	ldr r2, .L_08006780
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldrh r3, [r4]
	ldr r1, .L_08006790
	adds r3, r3, r1
	strh r3, [r4]
	ldrh r3, [r4]
	cmp r3, #0
	beq .L_0800670e
	mov r2, lr
	ldr r3, .L_08006778
	strb r2, [r6, #3]
	b .L_08006714
.L_0800670e:
	movs r3, #2
	strb r3, [r6, #3]
	ldr r3, .L_08006778
.L_08006714:
	ldrb r2, [r3]
	movs r3, #127
	ands r3, r2
	strb r3, [r6]
	ldr r3, [r7]
	adds r3, #20
	str r3, [r7]
	ldr r3, .L_08006778
	ldrb r2, [r3]
	movs r1, #127
	adds r2, #1
	ands r2, r1
	strb r2, [r3]
.L_0800672e:
	ldrb r3, [r6, #3]
	cmp r3, #2
	bne .L_08006748
	mov r1, r12
	ldrb r3, [r1, #2]
	cmp r3, #2
	bne .L_08006748
	ldr r2, .L_08006788
	movs r3, #0
	str r3, [r2]
	strb r3, [r6, #3]
	movs r3, #1
	strb r3, [r6]
.L_08006748:
	ldrb r3, [r6, #2]
	cmp r3, #2
	bne .L_0800675e
	mov r2, r12
	ldrb r3, [r2, #3]
	cmp r3, #2
	beq .L_0800676e
	ldr r2, .L_08006794
	movs r3, #0
	str r3, [r2]
	b .L_0800676c
.L_0800675e:
	movs r3, #0
	strb r3, [r6, #2]
	ldr r3, .L_08006794
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0800676e
	movs r3, #1
.L_0800676c:
	strb r3, [r6, #2]
.L_0800676e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_08006778:
	.4byte gSerialBlockSequence
.L_0800677c:
	.4byte 0x040000d4
.L_08006780:
	.4byte 0x84000005
.L_08006784:
	.4byte gSerialReceivedSize
.L_08006788:
	.4byte gSerialSendSource
.L_0800678c:
	.4byte gSerialSendSize
.L_08006790:
	.4byte 0x0000ffec
.L_08006794:
	.4byte gSerialReceiveDest
