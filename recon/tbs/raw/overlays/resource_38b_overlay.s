.syntax unified
	.thumb
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, lr}
	adds r5, r0, #0
	ldr r1, [r5, #104]
	cmp r1, #0
	beq .L_02000030_0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	ldr r0, [r1, #16]
	ldr r3, [r5, #16]
	ldr r1, [r1, #8]
	subs r0, r0, r3
	ldr r3, [r5, #8]
	subs r1, r1, r3
	bl 0x02009028
	ldrh r3, [r5, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	asrs r0, r0, #16
	cmp r0, #0
	beq .L_02000030_0
	movs r2, #128
	lsls r2, r2, #5
	cmp r0, r2
	ble .L_02000030_1
	adds r0, r2, #0
.L_02000030_1:
	ldr r2, [pc, #20]
	cmp r0, r2
	bge .L_02000030_2
	adds r0, r2, #0
.L_02000030_2:
	adds r3, r3, r0
	strh r3, [r5, #6]
.L_02000030_0:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xfffff000
	.global Func_02000088
	.thumb_func
Func_02000088:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000088_0
	ldr r0, [pc, #36]
	b .L_02000088_1
.L_02000088_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000088_2
	ldr r0, [pc, #36]
	b .L_02000088_1
.L_02000088_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000088_3
	ldr r0, [pc, #32]
	b .L_02000088_1
.L_02000088_3:
	ldr r0, [pc, #32]
.L_02000088_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000001e
	.4byte 0x020091ec
	.4byte 0x00000023
	.4byte 0x0200930c
	.4byte 0x00000020
	.4byte 0x0200936c
	.4byte 0x020091d4
	.global Func_020000dc
	.thumb_func
Func_020000dc:
	push {lr}
	ldr r3, [pc, #24]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #16]
	movs r0, #0
	cmp r2, r3
	bne .L_020000dc_0
	ldr r0, [pc, #12]
.L_020000dc_0:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000020
	.4byte 0x020093fc
	.global Func_02000104
	.thumb_func
Func_02000104:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200942c
	.global Func_0200010c
	.thumb_func
Func_0200010c:
	push {lr}
	movs r0, #9
	movs r1, #3
	movs r2, #0
	bl 0x020091b0
	pop {r0}
	bx r0
	.global Func_0200011c
	.thumb_func
Func_0200011c:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_0200011c_0
	ldr r0, [pc, #36]
	b .L_0200011c_1
.L_0200011c_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200011c_2
	ldr r0, [pc, #36]
	b .L_0200011c_1
.L_0200011c_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200011c_3
	ldr r0, [pc, #32]
	b .L_0200011c_1
.L_0200011c_3:
	ldr r0, [pc, #32]
.L_0200011c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000001e
	.4byte 0x02009498
	.4byte 0x00000023
	.4byte 0x02009600
	.4byte 0x00000020
	.4byte 0x020096f0
	.4byte 0x02009480
	.global Func_02000170
	.thumb_func
Func_02000170:
	push {lr}
	bl 0x020090b8
	ldr r0, [pc, #24]
	movs r1, #1
	bl 0x02009090
	ldr r0, [pc, #20]
	movs r1, #1
	bl 0x02009090
	bl 0x020090c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000947
	.4byte 0x000029dd
	.global Func_02000198
	.thumb_func
Func_02000198:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000198_0
	ldr r0, [pc, #36]
	b .L_02000198_1
.L_02000198_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000198_2
	ldr r0, [pc, #36]
	b .L_02000198_1
.L_02000198_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000198_3
	ldr r0, [pc, #32]
	b .L_02000198_1
.L_02000198_3:
	ldr r0, [pc, #32]
.L_02000198_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000001e
	.4byte 0x02009744
	.4byte 0x00000023
	.4byte 0x02009a2c
	.4byte 0x00000020
	.4byte 0x02009bc4
	.4byte 0x02009738
	.global Func_020001ec
	.thumb_func
Func_020001ec:
	push {lr}
	bl 0x020090b8
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x02009090
	bl 0x020090c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001472
	.global Func_02000208
	.thumb_func
Func_02000208:
	push {lr}
	bl 0x020090b8
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x02009090
	bl 0x020090c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000146e
	.global Func_02000224
	.thumb_func
Func_02000224:
	push {lr}
	bl 0x020090b8
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x02009090
	bl 0x020090c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001470
	.global Func_02000240
	.thumb_func
Func_02000240:
	push {lr}
	bl 0x020090b8
	ldr r0, [pc, #52]
	bl 0x02009130
	ldr r0, [pc, #48]
	bl 0x02009098
	cmp r0, #0
	beq .L_02000240_0
	ldr r3, [pc, #44]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000240_0:
	movs r1, #0
	movs r0, #9
	bl 0x02009138
	ldr r0, [pc, #16]
	bl 0x020090a0
	bl 0x020090c0
	pop {r0}
	bx r0
	.4byte 0x000013ae
	.4byte 0x00000301
	.4byte 0x03001ebc
	.global Func_02000288
	.thumb_func
Func_02000288:
	push {lr}
	bl 0x020090b8
	ldr r0, [pc, #20]
	bl 0x02009130
	movs r1, #0
	movs r0, #12
	bl 0x02009148
	bl 0x020090c0
	pop {r0}
	bx r0
	.4byte 0x000013b3
	.global Func_020002a8
	.thumb_func
Func_020002a8:
	push {lr}
	bl 0x020090b8
	ldr r0, [pc, #20]
	bl 0x02009130
	movs r1, #0
	movs r0, #14
	bl 0x02009148
	bl 0x020090c0
	pop {r0}
	bx r0
	.4byte 0x000013b7
	.global Func_020002c8
	.thumb_func
Func_020002c8:
	push {lr}
	bl 0x020090b8
	ldr r0, [pc, #20]
	bl 0x02009130
	movs r1, #0
	movs r0, #21
	bl 0x02009148
	bl 0x020090c0
	pop {r0}
	bx r0
	.4byte 0x000016bf
	.global Func_020002e8
	.thumb_func
Func_020002e8:
	push {lr}
	bl 0x020090b8
	ldr r0, [pc, #20]
	bl 0x02009130
	movs r1, #0
	movs r0, #16
	bl 0x02009148
	bl 0x020090c0
	pop {r0}
	bx r0
	.4byte 0x000016c8
	.global Func_02000308
	.thumb_func
Func_02000308:
	push {lr}
	bl 0x020090b8
	ldr r0, [pc, #20]
	bl 0x02009130
	movs r1, #0
	movs r0, #18
	bl 0x02009148
	bl 0x020090c0
	pop {r0}
	bx r0
	.4byte 0x000016cc
	.global Func_02000328
	.thumb_func
Func_02000328:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #204]
	ldr r7, [r3]
	bl 0x020090b8
	movs r5, #8
	movs r6, #0
.L_02000328_1:
	adds r0, r5, #0
	bl 0x020090d0
	cmp r0, #0
	beq .L_02000328_0
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
.L_02000328_0:
	adds r5, #1
	cmp r5, #65
	bls .L_02000328_1
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrh r3, [r3]
	subs r3, #3
	lsls r3, r3, #16
	asrs r5, r3, #16
	cmp r5, #6
	bne .L_02000328_2
	movs r0, #188
	bl 0x020091b8
	b .L_02000328_3
.L_02000328_2:
	movs r0, #158
	bl 0x020091b8
.L_02000328_3:
	ldr r2, [pc, #140]
	lsls r0, r5, #2
	ldrsh r1, [r2, r0]
	adds r3, r0, #2
	ldrsh r2, [r2, r3]
	ldr r3, [pc, #136]
	ldr r0, [r3, r0]
	bl 0x02009068
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #0
	lsls r1, r1, #8
	bl 0x020090d8
	movs r0, #0
	bl 0x020090d0
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r3, [pc, #92]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	cmp r5, #6
	bne .L_02000328_4
	movs r0, #0
	movs r1, #2
	bl 0x02009108
	movs r2, #4
	movs r0, #0
	movs r1, #0
	negs r2, r2
	bl 0x020090f8
	b .L_02000328_5
.L_02000328_4:
	movs r2, #16
	movs r0, #0
	movs r1, #3
	negs r2, r2
	bl 0x020090f0
.L_02000328_5:
	cmp r5, #4
	bne .L_02000328_6
	movs r0, #0
	movs r1, #3
	bl 0x02009158
	b .L_02000328_7
.L_02000328_6:
	movs r0, #0
	movs r1, #2
	bl 0x02009158
.L_02000328_7:
	movs r0, #16
	bl 0x020090b0
	adds r0, r5, #3
	bl 0x02009188
	bl 0x020090c0
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02009d0c
	.4byte 0x02009cf0
	.global Func_02000404
	.thumb_func
Func_02000404:
	push {r5, r6, r7, lr}
	movs r0, #0
	bl 0x020090d0
	adds r7, r0, #0
	movs r0, #11
	bl 0x020090d0
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #6
	beq .L_02000404_0
	b .L_02000404_1
.L_02000404_0:
	bl 0x020090b8
	movs r0, #11
	movs r1, #1
	bl 0x02009158
	movs r1, #2
	movs r0, #0
	bl 0x02009120
	movs r0, #20
	bl 0x020090b0
	movs r0, #0
	ldr r1, [pc, #284]
	ldr r2, [pc, #288]
	bl 0x020090d8
	ldr r1, [pc, #276]
	ldr r2, [pc, #280]
	movs r0, #11
	bl 0x020090d8
	movs r0, #0
	bl 0x020090d0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	ldr r6, [pc, #260]
	adds r3, r5, #0
	adds r3, #85
	movs r2, #0
	movs r1, #129
	strb r2, [r3]
	movs r0, #0
	lsls r1, r1, #1
	str r6, [r7, #24]
	bl 0x02009168
	movs r5, #128
	movs r0, #0
	movs r1, #16
	bl 0x02009108
	lsls r5, r5, #9
	movs r0, #11
	movs r1, #111
	movs r2, #196
	bl 0x020090e0
	movs r2, #185
	movs r1, #128
	movs r0, #0
	str r5, [r7, #24]
	bl 0x020090e8
	movs r0, #20
	bl 0x020090b0
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	str r6, [r7, #24]
	bl 0x02009168
	movs r0, #0
	movs r1, #16
	bl 0x02009108
	movs r0, #11
	movs r1, #121
	movs r2, #190
	bl 0x020090e0
	movs r2, #189
	movs r1, #141
	movs r0, #0
	str r5, [r7, #24]
	bl 0x020090e8
	movs r0, #20
	bl 0x020090b0
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	str r6, [r7, #24]
	bl 0x02009168
	movs r0, #0
	movs r1, #16
	bl 0x02009108
	movs r1, #132
	movs r2, #186
	movs r0, #11
	bl 0x020090e0
	str r5, [r7, #24]
	movs r0, #0
	bl 0x020090d0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	ldr r1, [pc, #104]
	movs r0, #0
	ldr r2, [pc, #104]
	bl 0x020090d8
	movs r0, #0
	movs r1, #166
	movs r2, #185
	bl 0x020090e8
	movs r1, #128
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x02009150
	movs r0, #11
	movs r1, #2
	bl 0x02009158
	movs r1, #11
	movs r0, #0
	bl 0x020091a0
	movs r0, #10
	bl 0x02009018
	ldr r0, [pc, #60]
	bl 0x02009130
	movs r1, #0
	movs r0, #11
	bl 0x02009138
	bl 0x020091a8
	movs r0, #10
	bl 0x02009018
	ldr r0, [pc, #40]
	bl 0x020090a0
	bl 0x020090c0
.L_02000404_1:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00003333
	.4byte 0x00001999
	.4byte 0xffff0000
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x00001774
	.4byte 0x00000848
	.global Func_02000578
	.thumb_func
Func_02000578:
	push {lr}
	bl 0x02009190
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000584
	.thumb_func
Func_02000584:
	push {r5, r6, lr}
	movs r0, #0
	bl 0x020090d0
	adds r5, r0, #0
	ldr r0, [pc, #724]
	bl 0x02009098
	cmp r0, #0
	bne .L_02000584_0
	b .L_02000584_1
.L_02000584_0:
	ldr r0, [pc, #716]
	bl 0x02009098
	cmp r0, #0
	bne .L_02000584_2
	b .L_02000584_1
.L_02000584_2:
	bl 0x020090b8
	ldr r0, [pc, #704]
	ldr r1, [pc, #704]
	bl 0x02009170
	movs r1, #1
	movs r2, #173
	lsls r2, r2, #16
	movs r3, #1
	ldr r0, [pc, #696]
	negs r1, r1
	bl 0x02009178
	bl 0x02009180
	movs r0, #12
	bl 0x020090d0
	ldr r3, [r5, #8]
	ldr r2, [r0, #8]
	cmp r2, r3
	ble .L_02000584_3
	movs r1, #160
	movs r0, #13
	lsls r1, r1, #7
	movs r2, #20
	bl 0x02009150
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #13
	bl 0x02009160
	ldr r0, [pc, #648]
	bl 0x02009130
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl 0x02009140
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009160
	b .L_02000584_4
.L_02000584_3:
	movs r1, #192
	movs r0, #12
	lsls r1, r1, #6
	movs r2, #20
	bl 0x02009150
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #12
	bl 0x02009160
	ldr r0, [pc, #596]
	bl 0x02009130
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl 0x02009140
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009160
.L_02000584_4:
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009160
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02009150
	movs r1, #160
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009150
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02009150
	movs r1, #134
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #184
	bl 0x020090e8
	movs r1, #192
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #8
	bl 0x02009150
	movs r0, #13
	movs r1, #2
	bl 0x02009120
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl 0x02009140
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x02009150
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #20
	bl 0x02009150
	movs r1, #128
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #8
	bl 0x02009150
	movs r0, #12
	movs r1, #3
	bl 0x02009110
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #14
	bl 0x02009168
	movs r0, #40
	bl 0x020090b0
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #10
	bl 0x02009150
	movs r1, #160
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009150
	movs r1, #192
	movs r2, #10
	movs r0, #13
	lsls r1, r1, #6
	bl 0x02009150
	movs r0, #14
	movs r1, #1
	bl 0x02009120
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl 0x02009140
	movs r0, #12
	movs r1, #3
	bl 0x02009108
	movs r1, #3
	movs r0, #13
	bl 0x02009110
	movs r0, #20
	bl 0x020090b0
	movs r0, #14
	movs r1, #0
	bl 0x02009138
	ldr r1, [pc, #340]
	ldr r2, [pc, #324]
	movs r0, #14
	bl 0x020090d8
	movs r0, #14
	bl 0x020090d0
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #133
	movs r2, #172
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #14
	bl 0x020090e8
	movs r0, #1
	bl 0x020090b0
	movs r0, #14
	bl 0x020090d0
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #10
	bl 0x020090b0
	movs r0, #14
	movs r1, #3
	bl 0x02009110
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl 0x02009140
	ldr r0, [pc, #256]
	movs r1, #1
	bl 0x02009090
	ldr r3, [pc, #252]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #194
	movs r1, #3
	bl 0x02009198
	movs r1, #0
	movs r0, #194
	bl 0x020090c8
	movs r0, #14
	movs r1, #3
	bl 0x02009110
	movs r0, #0
	movs r1, #1
	bl 0x02009108
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009150
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #14
	bl 0x020090d8
	movs r0, #14
	bl 0x020090d0
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #131
	ands r5, r3
	movs r2, #156
	lsls r1, r1, #1
	strb r5, [r0]
	movs r0, #14
	bl 0x020090e8
	movs r0, #1
	bl 0x020090b0
	movs r0, #14
	bl 0x020090d0
	adds r0, #90
	ldrb r3, [r0]
	orrs r6, r3
	strb r6, [r0]
	movs r0, #20
	bl 0x020090b0
	movs r0, #12
	movs r1, #2
	bl 0x02009120
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl 0x02009140
	movs r0, #12
	movs r1, #3
	bl 0x02009108
	movs r0, #13
	movs r1, #3
	bl 0x02009108
	movs r0, #14
	movs r1, #3
	bl 0x02009110
	ldr r5, [pc, #88]
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #9
	adds r2, r5, #0
	bl 0x02009128
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #9
	adds r2, r5, #0
	bl 0x02009128
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #9
	adds r2, r5, #0
	bl 0x02009128
	ldr r0, [pc, #56]
	bl 0x020090a0
	bl 0x020090c0
.L_02000584_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000845
	.4byte 0x00000848
	.4byte 0x00026666
	.4byte 0x00004ccc
	.4byte 0x01070000
	.4byte 0x00001775
	.4byte 0x00009999
	.4byte 0x0000177a
	.4byte 0x03001ebc
	.4byte 0x020091c0
	.4byte 0x00000849
	.global Func_02000890
	.thumb_func
Func_02000890:
	push {lr}
	ldr r3, [pc, #68]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #128
	lsls r2, r2, #1
	lsls r3, r3, #1
	str r3, [r1, r2]
	ldr r3, [pc, #56]
	ldrsh r2, [r3, r2]
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000890_0
	bl 0x020088f0
	b .L_02000890_1
.L_02000890_0:
	ldr r3, [pc, #48]
	cmp r2, r3
	bne .L_02000890_2
	bl 0x02008ae0
	movs r1, #200
	ldr r0, [pc, #40]
	lsls r1, r1, #4
	bl 0x02009020
	b .L_02000890_1
.L_02000890_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000890_1
	bl 0x02008d10
.L_02000890_1:
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000001e
	.4byte 0x00000023
	.4byte 0x02008ed9
	.4byte 0x00000020
	.global Func_020008f0
	.thumb_func
Func_020008f0:
	push {lr}
	ldr r0, [pc, #192]
	bl 0x02009098
	cmp r0, #0
	beq .L_020008f0_0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x02009100
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02009150
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009150
	b .L_020008f0_1
.L_020008f0_0:
	movs r0, #9
	bl 0x020090d0
	movs r1, #0
	bl 0x02009088
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl 0x02009100
.L_020008f0_1:
	movs r0, #8
	bl 0x020090d0
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r0, #28]
	movs r2, #225
	ldr r3, [pc, #112]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #10
	bne .L_020008f0_2
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x02009100
	b .L_020008f0_3
.L_020008f0_2:
	cmp r3, #9
	bne .L_020008f0_3
	ldr r0, [pc, #88]
	bl 0x020090a8
.L_020008f0_3:
	ldr r0, [pc, #84]
	bl 0x02009098
	cmp r0, #0
	bne .L_020008f0_4
	ldr r3, [pc, #68]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #11
	bne .L_020008f0_4
	movs r1, #248
	movs r2, #216
	movs r0, #20
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x02009100
.L_020008f0_4:
	bl 0x020089cc
	ldr r0, [pc, #44]
	bl 0x02009098
	cmp r0, #0
	beq .L_020008f0_5
	ldr r0, [pc, #40]
	bl 0x02009098
	cmp r0, #0
	bne .L_020008f0_5
	movs r0, #193
	lsls r0, r0, #2
	bl 0x020090a0
.L_020008f0_5:
	pop {r0}
	bx r0
	.4byte 0x00000845
	.4byte 0x02000240
	.4byte 0x0000012f
	.4byte 0x00000109
	.4byte 0x0000084a
	.4byte 0x0000084b
	.global Func_020009cc
	.thumb_func
Func_020009cc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #0
	sub sp, #8
	bl 0x020090d0
	adds r5, r0, #0
	movs r0, #20
	bl 0x020090d0
	ldr r3, [r0, #16]
	asrs r7, r3, #20
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	mov r9, r3
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	mov r10, r3
	movs r3, #12
	ldr r6, [r0, #8]
	movs r5, #15
	str r3, [sp, #4]
	movs r0, #15
	movs r1, #11
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009078
	movs r3, #13
	str r3, [sp, #4]
	movs r0, #15
	movs r1, #11
	movs r2, #3
	mov r8, r3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009078
	movs r3, #14
	str r3, [sp, #4]
	movs r0, #15
	movs r1, #11
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009078
	asrs r6, r6, #20
	movs r0, #1
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r7, [sp, #4]
	bl 0x02009078
	cmp r6, #16
	bne .L_020009cc_0
	cmp r7, #13
	beq .L_020009cc_1
.L_020009cc_0:
	movs r3, #16
	str r3, [sp, #0]
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x02009078
.L_020009cc_1:
	mov r3, r9
	cmp r3, #16
	bne .L_020009cc_2
	mov r3, r10
	cmp r3, #13
	bne .L_020009cc_2
	bl 0x020090b8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #20
	bl 0x02009160
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x020090d8
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x02009118
	cmp r7, #13
	bne .L_020009cc_3
	movs r1, #131
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #196
	bl 0x020090e0
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #20
	bl 0x02009150
	b .L_020009cc_4
.L_020009cc_3:
	movs r1, #143
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #218
	bl 0x020090e0
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009150
.L_020009cc_4:
	bl 0x020090c0
.L_020009cc_2:
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000ae0
	.thumb_func
Func_02000ae0:
	push {r5, r6, r7, lr}
	movs r0, #10
	sub sp, #8
	bl 0x020090d0
	adds r6, r0, #0
	movs r0, #11
	bl 0x020090d0
	adds r7, r0, #0
	movs r0, #8
	bl 0x020090d0
	movs r1, #0
	bl 0x02009088
	ldr r0, [pc, #412]
	bl 0x02009098
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000ae0_0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x02009100
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x02009100
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x02009100
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #56
	movs r1, #15
	movs r2, #40
	movs r3, #15
	bl 0x02009070
	movs r3, #10
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #26
	movs r1, #15
	movs r2, #1
	movs r3, #3
	bl 0x02009078
	ldr r0, [pc, #336]
	bl 0x02009098
	cmp r0, #0
	bne .L_02000ae0_1
	ldr r0, [pc, #328]
	bl 0x02009098
	cmp r0, #0
	beq .L_02000ae0_2
	b .L_02000ae0_3
.L_02000ae0_2:
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x02009100
.L_02000ae0_1:
	movs r1, #208
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009150
	movs r1, #176
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009150
	b .L_02000ae0_3
.L_02000ae0_0:
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x02009100
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x02009100
	movs r2, #0
	movs r1, #0
	movs r0, #14
	bl 0x02009100
	movs r0, #9
	bl 0x020090d0
	movs r1, #0
	bl 0x02009088
	movs r0, #10
	bl 0x020090d0
	movs r1, #0
	bl 0x02009088
	movs r0, #11
	bl 0x020090d0
	movs r1, #0
	bl 0x02009088
	adds r2, r6, #0
	adds r2, #85
	strb r5, [r2]
	ldr r0, [pc, #212]
	bl 0x02009098
	cmp r0, #0
	beq .L_02000ae0_4
	movs r0, #9
	bl 0x020090d0
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #16
	orrs r3, r5
	strb r3, [r0]
	movs r0, #16
	bl 0x020090d0
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #11
	bl 0x020090d0
	adds r0, #89
	ldrb r3, [r0]
	movs r1, #142
	orrs r3, r5
	movs r2, #156
	strb r3, [r0]
	lsls r2, r2, #16
	lsls r1, r1, #16
	movs r0, #16
	bl 0x02009100
	movs r0, #16
	bl 0x020090d0
	movs r1, #0
	bl 0x02009088
	movs r1, #142
	movs r2, #156
	movs r0, #10
	lsls r2, r2, #16
	lsls r1, r1, #16
	bl 0x02009100
	ldr r2, [r6, #80]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r2, #30]
	ldr r2, [pc, #116]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r0, [pc, #100]
	bl 0x02009098
	cmp r0, #0
	beq .L_02000ae0_5
	movs r1, #132
	movs r2, #186
	movs r0, #11
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x02009100
	b .L_02000ae0_3
.L_02000ae0_5:
	movs r1, #176
	movs r2, #196
	lsls r2, r2, #16
	movs r0, #11
	lsls r1, r1, #15
	bl 0x02009100
	movs r1, #3
	movs r0, #11
	bl 0x02009158
	adds r3, r7, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r1, #4
	orrs r2, r1
	strb r2, [r3]
	b .L_02000ae0_3
.L_02000ae0_4:
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r6, #12]
	adds r3, r7, #0
	adds r3, #85
	strb r0, [r3]
	movs r3, #192
	lsls r3, r3, #14
	str r3, [r7, #12]
.L_02000ae0_3:
	bl 0x02008cb4
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000845
	.4byte 0x00000849
	.4byte 0x00000848
	.4byte 0x00000881
	.4byte 0xfff80000
	.global Func_02000cb4
	.thumb_func
Func_02000cb4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #8
	sub sp, #8
	bl 0x020090d0
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	ldr r5, [pc, #64]
	asrs r7, r3, #20
	movs r6, #0
.L_02000cb4_0:
	ldrb r3, [r5]
	ldrb r2, [r5, #1]
	movs r0, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	adds r6, #2
	bl 0x02009078
	adds r5, #2
	cmp r6, #19
	bls .L_02000cb4_0
	mov r3, r8
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r7, [sp, #4]
	bl 0x02009078
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02009d28
	.global Func_02000d10
	.thumb_func
Func_02000d10:
	push {r5, lr}
	ldr r3, [pc, #96]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #68
	str r2, [r3]
	bl 0x02008fa0
	ldr r3, [pc, #80]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_02000d10_0
	ldr r0, [pc, #64]
	bl 0x02009098
	cmp r0, #0
	bne .L_02000d10_0
	movs r0, #0
	bl 0x020090d0
	adds r5, r0, #0
	bl 0x020090b8
	movs r1, #128
	lsls r1, r1, #13
	ldr r0, [r5, #8]
	str r1, [r5, #12]
	ldr r2, [r5, #16]
	movs r3, #0
	bl 0x02009178
	bl 0x02009050
	bl 0x020090c0
	movs r0, #1
	bl 0x02009018
.L_02000d10_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000109
	.global Func_02000d80
	.thumb_func
Func_02000d80:
	push {r5, lr}
	ldr r3, [pc, #44]
	ldr r3, [r3]
	adds r2, r3, #0
	adds r5, r0, #0
	movs r4, #8
	adds r2, #52
.L_02000d80_2:
	ldmia r2!, {r0}
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r5, r3
	bne .L_02000d80_0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r1, r3
	beq .L_02000d80_1
.L_02000d80_0:
	adds r4, #1
	cmp r4, #65
	bls .L_02000d80_2
	movs r0, #0
.L_02000d80_1:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_02000db4
	.thumb_func
Func_02000db4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #0
	sub sp, #12
	bl 0x020090d0
	ldrh r3, [r0, #6]
	ldr r2, [pc, #240]
	lsrs r3, r3, #12
	lsls r5, r3, #2
	ldr r3, [r2, r5]
	mov r8, r0
	movs r1, #10
	ldrsh r0, [r0, r1]
	mov r10, r2
	asrs r2, r3, #16
	adds r0, r0, r2
	mov r2, r8
	movs r4, #18
	ldrsh r1, [r2, r4]
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r1, r1, r3
	asrs r0, r0, #4
	asrs r1, r1, #4
	bl 0x02008d80
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02000db4_0
	movs r3, #0
	adds r2, r7, #0
	adds r2, #34
	mov r9, r3
	movs r3, #2
	strb r3, [r2]
	mov r4, r10
	ldr r1, [r4, r5]
	ldr r2, [pc, #184]
	ldr r3, [r7, #8]
	ands r2, r1
	mov r6, sp
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, [r7, #12]
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	adds r1, r6, #0
	str r3, [r6, #8]
	bl 0x02009080
	cmp r0, #0
	bgt .L_02000db4_0
	movs r1, #8
	mov r0, r8
	bl 0x02009038
	ldr r5, [pc, #144]
	movs r0, #15
	bl 0x02009018
	movs r0, #185
	bl 0x020091b8
	str r5, [r7, #48]
	str r5, [r7, #52]
	adds r0, r7, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl 0x02009058
	mov r1, r8
	str r5, [r1, #48]
	str r5, [r1, #52]
	mov r0, r8
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl 0x02009058
	adds r0, r7, #0
	bl 0x02009060
	ldr r3, [r6]
	str r3, [r7, #8]
	ldr r3, [r6, #8]
	mov r2, r9
	str r3, [r7, #16]
	str r2, [r7, #36]
	str r2, [r7, #44]
	movs r1, #1
	mov r0, r8
	bl 0x02009038
	ldr r3, [pc, #72]
	movs r4, #224
	lsls r4, r4, #1
	adds r3, r3, r4
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #64]
	cmp r2, r3
	bne .L_02000db4_1
	bl 0x02008cb4
	b .L_02000db4_0
.L_02000db4_1:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000db4_2
	bl 0x020089cc
	b .L_02000db4_0
.L_02000db4_2:
	ldr r3, [pc, #48]
	cmp r2, r3
	bne .L_02000db4_0
	bl 0x02008fa0
.L_02000db4_0:
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02009d3c
	.4byte 0xffff0000
	.4byte 0x00003333
	.4byte 0x02000240
	.4byte 0x00000023
	.4byte 0x0000001e
	.4byte 0x00000020
	.global Func_02000ed8
	.thumb_func
Func_02000ed8:
	push {r5, lr}
	ldr r3, [pc, #88]
	movs r1, #250
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r0, [r3]
	bl 0x020090d0
	movs r2, #142
	ldr r3, [r0, #8]
	lsls r2, r2, #16
	cmp r3, r2
	bge .L_02000ed8_0
	movs r1, #128
	ldr r3, [r0, #12]
	lsls r1, r1, #12
	cmp r3, r1
	bge .L_02000ed8_1
	ldr r5, [pc, #56]
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldrh r2, [r5]
	cmp r3, #0
	bne .L_02000ed8_2
	bl 0x02008f3c
	ldrh r2, [r5]
.L_02000ed8_2:
	adds r3, r2, #1
	movs r2, #240
	strh r3, [r5]
	lsls r2, r2, #13
	lsls r3, r3, #16
	cmp r3, r2
	bne .L_02000ed8_0
	ldr r3, [pc, #16]
	strh r3, [r5]
	b .L_02000ed8_0
.L_02000ed8_1:
	ldr r2, [pc, #20]
	ldr r3, [pc, #8]
	strh r3, [r2]
.L_02000ed8_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x02009d88
	.global Func_02000f3c
	.thumb_func
Func_02000f3c:
	push {r5, r6, r7, lr}
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	movs r0, #24
	bl 0x02009048
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000f3c_0
	ldr r1, [pc, #72]
	ldr r6, [r5, #80]
	bl 0x02009040
	adds r3, r5, #0
	adds r3, #85
	movs r7, #0
	adds r2, r5, #0
	strb r7, [r3]
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	adds r2, #1
	movs r3, #2
	strb r3, [r2]
	cmp r6, #0
	beq .L_02000f3c_0
	adds r0, r6, #0
	movs r1, #2
	bl 0x02009030
	adds r3, r6, #0
	adds r3, #38
	strb r7, [r3]
	movs r3, #13
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #5]
	ldrb r3, [r6, #9]
	movs r2, #12
	orrs r3, r2
	strb r3, [r6, #9]
.L_02000f3c_0:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02009d7c
	.global Func_02000fa0
	.thumb_func
Func_02000fa0:
	push {r5, lr}
	movs r0, #8
	sub sp, #8
	bl 0x020090d0
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000fa0_0
	ldr r3, [r5, #16]
	asrs r2, r3, #20
	cmp r2, #6
	bne .L_02000fa0_1
	movs r3, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #2
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x02009078
	b .L_02000fa0_2
.L_02000fa0_1:
	movs r3, #14
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x02009078
.L_02000fa0_2:
	ldr r3, [r5, #16]
	asrs r0, r3, #20
	cmp r0, #9
	bne .L_02000fa0_3
	movs r3, #14
	str r3, [sp, #0]
	str r0, [sp, #4]
	movs r1, #0
	movs r0, #2
	movs r2, #1
	movs r3, #1
	bl 0x02009078
	b .L_02000fa0_0
.L_02000fa0_3:
	movs r3, #14
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #1
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x02009078
.L_02000fa0_0:
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.section .rodata,"a",%progbits
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f0
	.4byte 0xc00001d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000000e0
	.4byte 0x40000058
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000178
	.4byte 0x40000168
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000000c8
	.4byte 0x40000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000068
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x000000a8
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000048
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000128
	.4byte 0x400000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00000190
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000028
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000108
	.4byte 0xc00000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000038
	.4byte 0x00000178
	.4byte 0x00100000
	.4byte 0x01900010
	.4byte 0x00000190
	.4byte 0xffff0002
	.4byte 0x00000178
	.4byte 0x800000b8
	.4byte 0x00100000
	.4byte 0x01900010
	.4byte 0x00000190
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000e8
	.4byte 0x40000090
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0xc0000198
	.4byte 0x00000000
	.4byte 0x00f00120
	.4byte 0x000001c0
	.4byte 0xffff0002
	.4byte 0x00000078
	.4byte 0x40000180
	.4byte 0x00000000
	.4byte 0x00f00120
	.4byte 0x000001c0
	.4byte 0xffff0003
	.4byte 0x00040048
	.4byte 0xc0000080
	.4byte 0x00200000
	.4byte 0x01800020
	.4byte 0x00000130
	.4byte 0xffff0004
	.4byte 0x00040128
	.4byte 0xc00000e0
	.4byte 0x00200000
	.4byte 0x01800020
	.4byte 0x00000130
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001a0042
	.4byte 0x004e0080
	.4byte 0x008c0026
	.4byte 0x0003ffff
	.4byte 0x001a0122
	.4byte 0x012e00e0
	.4byte 0x00ec0026
	.4byte 0x0004ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00105002
	.4byte 0x00201022
	.4byte 0x0030601f
	.4byte 0x0040101f
	.4byte 0x0050201f
	.4byte 0x0060301f
	.4byte 0x0070501f
	.4byte 0x0080401f
	.4byte 0x00923009
	.4byte 0x00a01020
	.4byte 0x00b04020
	.4byte 0x00000020
	.4byte 0x0010a01e
	.4byte 0x00203020
	.4byte 0x00302020
	.4byte 0x0040b01e
	.4byte 0x00000023
	.4byte 0x00106002
	.4byte 0x0022f002
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
	.4byte 0xffff0069
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00006000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x0001e000
	.4byte 0xffff0065
	.4byte 0x00000002
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00010000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00018000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00006000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00018000
	.4byte 0xffff00f3
	.4byte 0x00000007
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00024000
	.4byte 0xffff0069
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00023000
	.4byte 0xffff0069
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00023000
	.4byte 0xffff0069
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00023000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x011a0000
	.4byte 0x00000000
	.4byte 0x00aa0000
	.4byte 0x00009000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x00aa0000
	.4byte 0x00007000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01060000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x00009000
	.4byte 0xffff0017
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f3
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00640000
	.4byte 0x00024000
	.4byte 0x006c005d
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008329
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008241
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x020082c9
	.4byte 0x00000000
	.4byte 0x0845000a
	.4byte 0x000013b1
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000016c2
	.4byte 0x00000000
	.4byte 0x0845000b
	.4byte 0x000013b2
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000016c3
	.4byte 0x00000000
	.4byte 0x0845000c
	.4byte 0x02008289
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000016c4
	.4byte 0x00000000
	.4byte 0x0845000d
	.4byte 0x000013b6
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000016c5
	.4byte 0x00000000
	.4byte 0x0845000e
	.4byte 0x020082a9
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000016c6
	.4byte 0x00000000
	.4byte 0x0845000f
	.4byte 0x000013ba
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000016c7
	.4byte 0x00000000
	.4byte 0x08450010
	.4byte 0x000013bb
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020082e9
	.4byte 0x00000000
	.4byte 0x08450011
	.4byte 0x000013bc
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000016cb
	.4byte 0x00000000
	.4byte 0x08450012
	.4byte 0x000013bd
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008309
	.4byte 0x00000000
	.4byte 0x08450013
	.4byte 0x000013be
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000016cf
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000013b0
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000016d3
	.4byte 0x00008d15
	.4byte 0x0845000a
	.4byte 0x000013c6
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000016d4
	.4byte 0x00008d15
	.4byte 0x0845000b
	.4byte 0x000013c7
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000016d5
	.4byte 0x00008d15
	.4byte 0x0845000c
	.4byte 0x000013c8
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000016d6
	.4byte 0x00008d15
	.4byte 0x0845000d
	.4byte 0x000013c9
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000016d7
	.4byte 0x00008d15
	.4byte 0x0845000e
	.4byte 0x000013ca
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000016d8
	.4byte 0x00008d15
	.4byte 0x0845000f
	.4byte 0x000013cb
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000016d9
	.4byte 0x00008d15
	.4byte 0x08450010
	.4byte 0x000013cc
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000016da
	.4byte 0x00008d15
	.4byte 0x08450011
	.4byte 0x000013cd
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000016db
	.4byte 0x00008d15
	.4byte 0x08450012
	.4byte 0x000013ce
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000016dc
	.4byte 0x00008d15
	.4byte 0x08450013
	.4byte 0x000013cf
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000016dd
	.4byte 0x00008c15
	.4byte 0xffff0014
	.4byte 0x020089cd
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x02008db5
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x00000023
	.4byte 0x0f500064
	.4byte 0x001000b4
	.4byte 0x00000023
	.4byte 0x0f510065
	.4byte 0x00200005
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x02008171
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008225
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008225
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008225
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008225
	.4byte 0x00008d15
	.4byte 0x18810009
	.4byte 0x00001771
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001471
	.4byte 0x00008d15
	.4byte 0x08480010
	.4byte 0x00001772
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001770
	.4byte 0x00008d15
	.4byte 0x0848000a
	.4byte 0x00001772
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001770
	.4byte 0x00008d15
	.4byte 0x0848000b
	.4byte 0x00001773
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001774
	.4byte 0x00000000
	.4byte 0x0848000c
	.4byte 0x0000177f
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000177c
	.4byte 0x00000000
	.4byte 0x0848000d
	.4byte 0x00001780
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000177d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000177e
	.4byte 0x00008d15
	.4byte 0x0848000c
	.4byte 0x00001784
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001781
	.4byte 0x00008d15
	.4byte 0x0848000d
	.4byte 0x00001785
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001782
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001783
	.4byte 0x00000003
	.4byte 0xffff000b
	.4byte 0x020081ed
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte 0x00400953
	.4byte 0x00000002
	.4byte 0x0849000d
	.4byte 0x02008585
	.4byte 0x00008c15
	.4byte 0x0848000b
	.4byte 0x02008405
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008579
	.4byte 0x00008602
	.4byte 0xffff000a
	.4byte 0x02008579
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte 0x02008db5
	.4byte 0x00000202
	.4byte 0xffff000c
	.4byte 0x02008db5
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02008cb5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0xffff000a
	.4byte 0x02008209
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0200810d
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02008fa1
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte 0x02008db5
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x02008db5
	.4byte 0x00000202
	.4byte 0xffff000d
	.4byte 0x02008db5
	.4byte 0x00000602
	.4byte 0xffff000b
	.4byte 0x02008579
	.4byte 0x00008602
	.4byte 0xffff000b
	.4byte 0x02008579
	.4byte 0x00004602
	.4byte 0xffff000c
	.4byte 0x02008579
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00210025
	.4byte 0x00020004
	.4byte 0x00210005
	.4byte 0x00040021
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x0002002b
	.4byte 0x00050002
	.4byte 0x002b0021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00340023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020034
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x00020036
	.4byte 0x00050002
	.4byte 0x00360021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00300023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020030
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x0002002e
	.4byte 0x00050002
	.4byte 0x002e0021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00320023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020032
	.4byte 0x00050002
	.4byte 0x0000ffff
	.4byte 0x02009cac
	.4byte 0x02009c96
	.4byte 0x02009cc2
	.4byte 0x02009cd8
	.4byte 0x02009c6a
	.4byte 0x02009c80
	.4byte 0x02009c54
	.4byte 0x00140036
	.4byte 0x0012002b
	.4byte 0x00130025
	.4byte 0x000b0029
	.4byte 0x000b0023
	.4byte 0x00090031
	.4byte 0x00060037
	.4byte 0x12091208
	.4byte 0x1308120a
	.4byte 0x130a1309
	.4byte 0x1409130b
	.4byte 0x140b140a
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
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001b
