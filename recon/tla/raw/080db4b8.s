.syntax unified
	.thumb
	.global BattleFx_Run
	.thumb_func
BattleFx_Run:
	push {r5, r6, r7, lr}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #224
	ldr r6, [r3]
	ldr r3, [r2, #108]
	movs r1, #30
	ldrsh r2, [r6, r1]
	movs r1, #26
	ldrsh r5, [r6, r1]
	cmp r2, #1
	bne .L_080db4d8
	bl BattleFx_RunItemBreakSequence
	b .L_080db668
.L_080db4d8:
	cmp r2, #7
	bne .L_080db4e2
	bl Func_080ddea8
	b .L_080db668
.L_080db4e2:
	cmp r2, #11
	bne .L_080db4ec
	bl Func_080df6f4
	b .L_080db668
.L_080db4ec:
	cmp r2, #4
	bne .L_080db4f6
	bl Func_080de21c
	b .L_080db668
.L_080db4f6:
	cmp r2, #5
	bne .L_080db500
	bl Func_080dede8
	b .L_080db668
.L_080db500:
	cmp r2, #14
	bne .L_080db50a
	bl Func_080dfd00
	b .L_080db668
.L_080db50a:
	cmp r2, #6
	bne .L_080db514
	bl Func_080de688
	b .L_080db668
.L_080db514:
	cmp r2, #3
	bne .L_080db51e
	bl Func_080de9f8
	b .L_080db668
.L_080db51e:
	cmp r2, #12
	bne .L_080db528
	bl Func_080ddb3c
	b .L_080db668
.L_080db528:
	cmp r2, #13
	bne .L_080db532
	bl Func_080e03cc
	b .L_080db668
.L_080db532:
	cmp r2, #15
	bne .L_080db53c
	bl Func_080e0dd4
	b .L_080db668
.L_080db53c:
	cmp r2, #16
	bne .L_080db546
	bl Func_080e1214
	b .L_080db668
.L_080db546:
	cmp r2, #10
	bne .L_080db550
	bl Func_080decb8
	b .L_080db668
.L_080db550:
	cmp r2, #8
	bne .L_080db55a
	bl Func_080dd820
	b .L_080db668
.L_080db55a:
	cmp r2, #17
	bne .L_080db564
	bl Func_080e09c0
	b .L_080db668
.L_080db564:
	cmp r2, #29
	bne .L_080db56e
	bl Func_080e1f2c
	b .L_080db668
.L_080db56e:
	cmp r2, #25
	bne .L_080db578
	bl Func_080e3074
	b .L_080db668
.L_080db578:
	cmp r2, #26
	bne .L_080db582
	bl Func_080e3d04
	b .L_080db668
.L_080db582:
	cmp r2, #21
	bne .L_080db58c
	bl Func_080e572c
	b .L_080db668
.L_080db58c:
	cmp r2, #18
	bne .L_080db596
	bl Func_080e5d5c
	b .L_080db668
.L_080db596:
	cmp r2, #20
	bne .L_080db5a0
	bl Func_080e68d0
	b .L_080db668
.L_080db5a0:
	cmp r2, #19
	bne .L_080db5aa
	bl Func_080e9090
	b .L_080db668
.L_080db5aa:
	cmp r2, #27
	bne .L_080db5b4
	bl Func_080e97c8
	b .L_080db668
.L_080db5b4:
	cmp r2, #24
	bne .L_080db5be
	bl Func_080e9af4
	b .L_080db668
.L_080db5be:
	cmp r2, #23
	bne .L_080db5c8
	bl Func_080e4a34
	b .L_080db668
.L_080db5c8:
	cmp r2, #30
	bne .L_080db5d2
	bl Func_080e781c
	b .L_080db668
.L_080db5d2:
	cmp r2, #22
	bne .L_080db5dc
	bl Func_080e7844
	b .L_080db668
.L_080db5dc:
	cmp r2, #28
	bne .L_080db5e6
	bl Func_080e82cc
	b .L_080db668
.L_080db5e6:
	cmp r2, #9
	bne .L_080db644
	ldr r3, .L_080db66c
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #106
	adds r7, r3, r2
	movs r3, #0
	ldrsh r0, [r7, r3]
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_080db60c
	bl Func_080e035c
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r7]
.L_080db60c:
	movs r1, #9
	movs r2, #24
	ldrsh r0, [r6, r2]
	bl Func_080cdbf8
	bl Func_080e03ac
	adds r5, r0, #0
	bl Func_080cce94
	cmp r0, #0
	beq .L_080db63e
	adds r1, r5, #0
	movs r3, #24
	ldrsh r0, [r6, r3]
	bl Func_080dc62c
	adds r0, r5, #0
	bl Func_080e011c
	adds r0, r5, #0
	bl Func_080e0308
	strh r5, [r7]
	b .L_080db668
.L_080db63e:
	bl Func_080e0134
	b .L_080db668
.L_080db644:
	cmp r2, #2
	bne .L_080db668
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r3, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080db65e
	bl Func_080dd950
.L_080db65e:
	movs r2, #24
	ldrsh r0, [r6, r2]
	adds r1, r5, #0
	bl Func_080dc978
.L_080db668:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080db66c:
	.4byte gPartyState
