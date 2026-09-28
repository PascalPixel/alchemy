.syntax unified
	.thumb
	push	{lr}
	movs	r0, #8
	movs	r1, #11
	bl 0x0200a940
	pop	{pc}
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xab24
	.2byte 0x0200
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	movs	r0, #0
	bx	lr
	.global Func_02000050
	.thumb_func
Func_02000050:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xab54
	.2byte 0x0200
	.global Func_02000058
	.thumb_func
Func_02000058:
	push	{lr}
	ldr	r3, [pc, #96]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_02000070
	ldr	r0, [pc, #84]
	b.n	.L_020000b8
.L_02000070:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_0200007a
	ldr	r0, [pc, #84]
	b.n	.L_020000b8
.L_0200007a:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02000084
	ldr	r0, [pc, #80]
	b.n	.L_020000b8
.L_02000084:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_0200008e
	ldr	r0, [pc, #80]
	b.n	.L_020000b8
.L_0200008e:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02000098
	ldr	r0, [pc, #76]
	b.n	.L_020000b8
.L_02000098:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_020000a2
	ldr	r0, [pc, #76]
	b.n	.L_020000b8
.L_020000a2:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_020000ac
	ldr	r0, [pc, #72]
	b.n	.L_020000b8
.L_020000ac:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_020000b6
	ldr	r0, [pc, #72]
	b.n	.L_020000b8
.L_020000b6:
	ldr	r0, [pc, #72]
.L_020000b8:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000090
	.4byte 0x0200ac6c
	.4byte 0x00000092
	.4byte 0x0200ad2c
	.4byte 0x00000093
	.4byte 0x0200ad5c
	.4byte 0x00000095
	.4byte 0x0200adbc
	.4byte 0x00000096
	.4byte 0x0200ae04
	.4byte 0x00000097
	.4byte 0x0200af9c
	.4byte 0x00000098
	.4byte 0x0200ae64
	.4byte 0x00000099
	.4byte 0x0200afe4
	.2byte 0xac54
	.2byte 0x0200
	push	{lr}
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #216
	adds	r1, r1, r3
	adds	r0, r1, #0
	bl 0x0200a7d0
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #47
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #16
	movs	r2, #1
	movs	r3, #2
	movs	r0, #50
	bl 0x0200a840
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200a7d0
	add	sp, #8
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r1
	mov	r0, fp
	bl 0x0200a8a8
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #12
	beq.n	.L_0200015e
	b.n	.L_020002e4
.L_0200015e:
	bl 0x0200a898
	adds	r6, r7, #0
	movs	r0, #0
	bl 0x0200a968
	adds	r6, #85
	movs	r3, #3
	movs	r5, #0
	strb	r3, [r6, #0]
	movs	r0, #5
	bl 0x0200a798
	strb	r5, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #11
	ldr	r0, [r7, #80]
	str	r5, [r7, #40]
	str	r3, [r7, #12]
	mov	sl, r0
	mov	r9, r5
.L_02000188:
	mov	r2, r9
	mov	r3, sl
	lsls	r1, r2, #2
	ldrb	r2, [r3, #17]
	movs	r0, #3
	adds	r3, r0, #0
	ands	r3, r2
	orrs	r3, r1
	mov	r1, sl
	ldrb	r2, [r1, #26]
	strb	r3, [r1, #17]
	movs	r3, #8
	orrs	r3, r2
	mov	r8, r9
	movs	r2, #254
	ands	r3, r2
	mov	r2, r8
	strb	r3, [r1, #26]
	ands	r2, r0
	movs	r3, #1
	strb	r3, [r1, #25]
	mov	r8, r2
	cmp	r2, #0
	bne.n	.L_0200023e
	bl 0x0200a7b0
	adds	r5, r0, #0
	bl 0x0200a7b0
	ldr	r6, [r7, #8]
	lsls	r5, r5, #1
	adds	r6, r6, r5
	lsls	r0, r0, #1
	subs	r6, r6, r0
	bl 0x0200a7b0
	adds	r5, r0, #0
	bl 0x0200a7b0
	ldr	r3, [r7, #16]
	lsls	r5, r5, #1
	adds	r3, r3, r5
	lsls	r0, r0, #1
	subs	r3, r3, r0
	ldr	r2, [r7, #12]
	movs	r0, #14
	adds	r1, r6, #0
	bl 0x0200a808
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200023e
	ldr	r3, [r7, #20]
	ldr	r6, [r5, #80]
	str	r3, [r5, #20]
	ldr	r1, [pc, #248]
	bl 0x0200a800
	adds	r3, r5, #0
	adds	r3, #85
	mov	r0, r8
	strb	r0, [r3, #0]
	cmp	r6, #0
	beq.n	.L_0200023e
	movs	r1, #1
	adds	r0, r6, #0
	bl 0x0200a7f0
	mov	r1, r8
	strb	r1, [r6, #26]
	mov	r2, sl
	ldrb	r3, [r2, #9]
	movs	r0, #3
	lsls	r3, r3, #28
	ldrb	r2, [r6, #9]
	lsrs	r3, r3, #30
	ands	r3, r0
	subs	r0, #16
	adds	r1, r0, #0
	lsls	r3, r3, #2
	ands	r2, r1
	orrs	r2, r3
	adds	r0, r6, #0
	movs	r1, #13
	strb	r2, [r6, #9]
	bl 0x0200a878
	adds	r0, r6, #0
	movs	r1, #8
	bl 0x0200a888
.L_0200023e:
	movs	r0, #3
	bl 0x0200a798
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #24
	ble.n	.L_02000188
	movs	r3, #84
	adds	r3, r3, r7
	mov	sl, r3
	mov	r0, sl
	movs	r3, #0
	strb	r3, [r0, #0]
	str	r3, [r7, #12]
	mov	r9, r1
.L_0200025e:
	movs	r0, #14
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	adds	r0, #255
	bl 0x0200a808
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020002ae
	ldr	r1, [pc, #124]
	ldr	r6, [r5, #80]
	bl 0x0200a800
	movs	r1, #0
	adds	r3, r5, #0
	mov	r8, r1
	adds	r3, #85
	mov	r2, r8
	strb	r2, [r3, #0]
	adds	r2, r5, #0
	adds	r2, #34
	movs	r3, #1
	strb	r3, [r2, #0]
	cmp	r6, #0
	beq.n	.L_020002ae
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200a7f0
	mov	r3, r8
	strb	r3, [r6, #26]
	movs	r0, #13
	ldrb	r3, [r6, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r6, #9]
.L_020002ae:
	movs	r0, #15
	bl 0x0200a798
	movs	r1, #1
	negs	r1, r1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #0
	bge.n	.L_0200025e
	movs	r0, #30
	bl 0x0200a890
	movs	r3, #1
	mov	r0, sl
	strb	r3, [r0, #0]
	movs	r1, #0
	mov	r0, fp
	movs	r2, #0
	bl 0x0200a8d0
	bl 0x0200a8a0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #229
	bl 0x0200a7d0
.L_020002e4:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0xaae8
	.2byte 0x0200
	push	{lr}
	movs	r2, #192
	lsls	r2, r2, #2
	movs	r1, #65
	adds	r2, #1
	bl 0x0200a958
	pop	{pc}
	push	{lr}
	movs	r2, #192
	movs	r1, #64
	lsls	r2, r2, #2
	bl 0x0200a958
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r3, #14
	movs	r2, #37
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #76
	movs	r1, #70
	movs	r2, #14
	movs	r3, #8
	bl 0x0200a990
	add	sp, #12
	pop	{pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x0200a8a8
	movs	r3, #14
	movs	r2, #37
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	adds	r5, r0, #0
	movs	r1, #70
	movs	r0, #76
	movs	r2, #14
	movs	r3, #8
	bl 0x0200a990
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #153
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_020003bc
	movs	r1, #132
	movs	r2, #166
	movs	r0, #64
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200a8d0
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #16
	bne.n	.L_020003bc
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #41
	bne.n	.L_020003bc
	bl 0x0200a898
	movs	r0, #0
	bl 0x0200a968
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x0200a910
	movs	r2, #16
	ldr	r0, [r6, #0]
	movs	r1, #0
	negs	r2, r2
	bl 0x0200a8c0
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x0200a8c8
	bl 0x0200a8a0
.L_020003bc:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #14
	movs	r2, #37
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #76
	movs	r1, #70
	movs	r2, #14
	movs	r3, #8
	bl 0x0200a990
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #182
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_02000420
	movs	r1, #172
	movs	r2, #162
	movs	r0, #249
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200a9a8
	ldr	r2, [pc, #36]
	movs	r3, #149
	lsls	r3, r3, #2
	adds	r1, r2, r3
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #182
	strh	r3, [r1, #0]
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r2, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r0, #106
	movs	r1, #1
	bl 0x0200a938
.L_02000420:
	add	sp, #12
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #52]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	adds	r6, r0, #0
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200a900
	movs	r2, #128
	movs	r1, #6
	lsls	r2, r2, #5
	ldr	r0, [r5, #0]
	bl 0x0200a8e0
	movs	r0, #155
	lsls	r0, r0, #1
	bl 0x0200a9b0
	movs	r0, #145
	lsls	r0, r0, #1
	bl 0x0200a7d0
	adds	r0, r6, #0
	bl 0x0200a930
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	bl 0x0200a8a8
	cmp	r5, #9
	bne.n	.L_0200049a
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #17
	bne.n	.L_020004c4
	ldr	r3, [r0, #16]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	movs	r1, #128
	str	r3, [r0, #16]
	lsls	r1, r1, #12
	movs	r0, #2
	bl 0x0200a764
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #109
	bl 0x0200a7d0
	b.n	.L_020004c4
.L_0200049a:
	cmp	r5, #10
	bne.n	.L_020004c4
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #29
	bne.n	.L_020004c4
	ldr	r3, [r0, #16]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	movs	r1, #128
	str	r3, [r0, #16]
	lsls	r1, r1, #12
	movs	r0, #8
	bl 0x0200a764
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #110
	bl 0x0200a7d0
.L_020004c4:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #360]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #352]
	sub	sp, #12
	adds	r6, r0, #0
	cmp	r2, r3
	bne.n	.L_02000516
	ldr	r3, [pc, #344]
	adds	r2, r6, #0
	subs	r2, #10
	ldrb	r0, [r3, r2]
	bl 0x0200a708
	adds	r7, r0, #0
	cmp	r6, #12
	bne.n	.L_02000502
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #110
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02000502
	b.n	.L_0200062e
.L_02000502:
	cmp	r6, #18
	bne.n	.L_02000516
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #109
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02000516
	b.n	.L_0200062e
.L_02000516:
	ldr	r3, [pc, #284]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #284]
	cmp	r2, r3
	bne.n	.L_0200055a
	ldr	r3, [pc, #280]
	adds	r2, r6, #0
	subs	r2, #10
	ldrb	r0, [r3, r2]
	bl 0x0200a708
	adds	r7, r0, #0
	cmp	r6, #10
	bne.n	.L_02000548
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_0200062e
.L_02000548:
	cmp	r6, #12
	bne.n	.L_0200055a
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_0200062e
.L_0200055a:
	ldr	r3, [pc, #216]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #224]
	cmp	r2, r3
	bne.n	.L_02000588
	adds	r0, r6, #0
	subs	r0, #10
	bl 0x0200a708
	adds	r7, r0, #0
	cmp	r6, #10
	bne.n	.L_02000588
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_0200062e
.L_02000588:
	ldr	r5, [pc, #168]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #180]
	cmp	r2, r3
	bne.n	.L_020005a4
	adds	r0, r6, #0
	subs	r0, #10
	bl 0x0200a708
	adds	r7, r0, #0
.L_020005a4:
	cmp	r7, #0
	beq.n	.L_0200062e
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200a8a8
	adds	r5, r0, #0
	bl 0x0200a898
	movs	r0, #0
	bl 0x0200a968
	b.n	.L_02000610
.L_020005c2:
	cmp	r0, #0
	bge.n	.L_020005d4
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r5, #6]
	adds	r0, r6, #0
	bl 0x02008428
	b.n	.L_0200062e
.L_020005d4:
	ldr	r3, [r5, #16]
	movs	r1, #128
	lsls	r1, r1, #10
	adds	r3, r3, r1
	str	r3, [r5, #16]
	ldr	r2, [r5, #8]
	ldr	r3, [pc, #108]
	ldr	r1, [pc, #112]
	ands	r3, r2
	adds	r3, r3, r1
	movs	r1, #128
	lsls	r1, r1, #9
	cmp	r3, r1
	ble.n	.L_020005f4
	movs	r3, #128
	lsls	r3, r3, #9
.L_020005f4:
	ldr	r1, [pc, #96]
	cmp	r3, r1
	bge.n	.L_020005fc
	ldr	r3, [pc, #92]
.L_020005fc:
	subs	r3, r2, r3
	str	r3, [r5, #8]
	ldrh	r3, [r5, #6]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	strh	r3, [r5, #6]
	movs	r0, #1
	bl 0x0200a798
.L_02000610:
	ldr	r3, [r5, #8]
	mov	r1, sp
	str	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [r5, #12]
	lsls	r2, r2, #12
	str	r3, [r1, #4]
	adds	r0, r5, #0
	ldr	r3, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r1, #8]
	bl 0x0200a850
	cmp	r0, #0
	ble.n	.L_020005c2
.L_0200062e:
	add	sp, #12
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000093
	.4byte 0x0200a9b8
	.4byte 0x00000095
	.4byte 0x0200a9c1
	.4byte 0x00000096
	.4byte 0x00000097
	.4byte 0x000fffff
	.4byte 0xfff80000
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	movs	r0, #8
	bl 0x0200a8a8
	adds	r7, r0, #0
	bl 0x0200a898
	movs	r0, #0
	bl 0x0200a968
	ldr	r5, [pc, #152]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #8
	movs	r2, #0
	bl 0x0200a8e8
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #30
	bl 0x0200a908
	movs	r0, #204
	movs	r1, #1
	movs	r2, #156
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x0200a920
	bl 0x0200a928
	movs	r1, #2
	movs	r0, #8
	adds	r1, #255
	bl 0x0200a910
	movs	r5, #128
	lsls	r5, r5, #8
	movs	r6, #2
.L_020006b4:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #40]
	movs	r0, #152
	bl 0x0200a9b0
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	str	r5, [r7, #48]
	str	r3, [r7, #52]
	movs	r0, #8
	movs	r1, #0
	movs	r2, #8
	bl 0x0200a970
	movs	r3, #128
	lsls	r3, r3, #7
	subs	r6, #1
	adds	r5, r5, r3
	cmp	r6, #0
	bge.n	.L_020006b4
	movs	r0, #155
	lsls	r0, r0, #1
	bl 0x0200a9b0
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r7, #40]
	movs	r1, #0
	movs	r2, #8
	movs	r0, #8
	bl 0x0200a970
	movs	r0, #60
	bl 0x0200a890
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200a7d0
	bl 0x0200a8a0
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	bl 0x0200a8a8
	adds	r5, r0, #0
	adds	r7, r5, #0
	adds	r7, #85
	movs	r3, #3
	movs	r6, #60
	strb	r3, [r7, #0]
	b.n	.L_02000726
.L_02000724:
	subs	r6, #1
.L_02000726:
	cmp	r6, #0
	beq.n	.L_02000736
	movs	r0, #1
	bl 0x0200a798
	ldr	r3, [r5, #40]
	cmp	r3, #0
	bne.n	.L_02000724
.L_02000736:
	movs	r0, #10
	bl 0x0200a798
	movs	r3, #0
	strb	r3, [r7, #0]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	sub	sp, #8
	bl 0x0200a8a8
	cmp	r5, #9
	bne.n	.L_0200077e
	ldr	r3, [r0, #8]
	asrs	r5, r3, #20
	cmp	r5, #16
	bne.n	.L_0200077e
	movs	r0, #9
	bl 0x02008710
	movs	r3, #26
	str	r3, [sp, #4]
	movs	r0, #15
	movs	r1, #26
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200a840
	movs	r0, #151
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200a7d0
.L_0200077e:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x0200a900
	adds	r0, r5, #0
	bl 0x0200a8a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200a8d8
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200a7d0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200a7d0
	pop	{r5, pc}
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #26
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	adds	r5, r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #28
	movs	r1, #8
	bl 0x0200a840
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200a900
	adds	r0, r5, #0
	bl 0x0200a8a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r0, #0]
	adds	r1, r5, #0
	movs	r0, #0
	bl 0x0200a778
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7d0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200a7d8
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	movs	r1, #8
	sub	sp, #8
	bl 0x0200a8d8
	movs	r1, #3
	adds	r0, r5, #0
	bl 0x0200a900
	adds	r0, r5, #0
	bl 0x0200a8a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	adds	r0, r5, #0
	bl 0x0200a8a8
	movs	r6, #0
	movs	r1, #0
	str	r6, [r0, #108]
	movs	r0, #0
	bl 0x0200a778
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200a7d8
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7d8
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200a7d8
	ldr	r3, [pc, #112]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r0, [r7, #0]
	bl 0x0200a8a8
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200a8a8
	ldr	r3, [r6, #8]
	ldr	r2, [r0, #8]
	asrs	r3, r3, #20
	asrs	r2, r2, #20
	cmp	r2, r3
	bne.n	.L_020008bc
	ldr	r2, [r0, #16]
	ldr	r3, [r6, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_020008bc
	movs	r1, #129
	ldr	r0, [r7, #0]
	lsls	r1, r1, #1
	bl 0x0200a910
	movs	r1, #0
	movs	r2, #16
	ldr	r0, [r7, #0]
	bl 0x0200a970
	movs	r0, #2
	bl 0x0200a890
	movs	r1, #192
	ldr	r0, [r7, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8f8
	movs	r0, #10
	bl 0x0200a890
.L_020008bc:
	movs	r3, #26
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #22
	movs	r1, #8
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a840
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r1, #7
	bl 0x0200a8d8
	movs	r1, #3
	adds	r0, r5, #0
	bl 0x0200a900
	adds	r0, r5, #0
	bl 0x0200a8a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	adds	r0, r5, #0
	bl 0x0200a8a8
	movs	r6, #0
	str	r6, [r0, #108]
	movs	r1, #0
	movs	r0, #0
	bl 0x0200a778
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200a7d8
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200a7d8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r6, r0, #0
	movs	r1, #3
	adds	r0, r5, #0
	bl 0x0200a900
	adds	r0, r5, #0
	bl 0x0200a8a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200a8d8
	cmp	r6, #5
	beq.n	.L_02000956
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x0200a7e0
	cmp	r0, #3
	ble.n	.L_02000968
.L_02000956:
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200a7d0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200a7d0
.L_02000968:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #25
	movs	r2, #27
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	adds	r5, r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #23
	movs	r1, #27
	bl 0x0200a840
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200a900
	adds	r0, r5, #0
	bl 0x0200a8a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r0, #0]
	adds	r1, r5, #0
	movs	r0, #0
	bl 0x0200a778
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7d0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200a7d8
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	movs	r1, #8
	sub	sp, #8
	bl 0x0200a8d8
	movs	r1, #3
	adds	r0, r6, #0
	bl 0x0200a900
	adds	r0, r6, #0
	bl 0x0200a8a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	adds	r0, r6, #0
	bl 0x0200a8a8
	movs	r5, #0
	movs	r1, #0
	str	r5, [r0, #108]
	movs	r0, #0
	bl 0x0200a778
	movs	r0, #136
.L_020009f8:
	lsls	r0, r0, #2
	bl 0x0200a7d8
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7d8
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200a7d8
	movs	r0, #140
	movs	r1, #0
	lsls	r0, r0, #2
	bl 0x0200a7e8
	ldr	r3, [pc, #112]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r0, [r7, #0]
	bl 0x0200a8a8
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x0200a8a8
	ldr	r3, [r5, #8]
	ldr	r2, [r0, #8]
	asrs	r3, r3, #20
	asrs	r2, r2, #20
	cmp	r2, r3
	bne.n	.L_02000a76
	ldr	r2, [r0, #16]
	ldr	r3, [r5, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02000a76
	movs	r1, #129
	ldr	r0, [r7, #0]
	lsls	r1, r1, #1
	bl 0x0200a910
	movs	r1, #0
	movs	r2, #16
	ldr	r0, [r7, #0]
	bl 0x0200a970
	movs	r0, #2
	bl 0x0200a890
	movs	r1, #192
	ldr	r0, [r7, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8f8
	movs	r0, #10
	bl 0x0200a890
.L_02000a76:
	movs	r3, #25
	movs	r2, #27
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a840
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r1, #7
	bl 0x0200a8d8
	movs	r1, #3
	adds	r0, r5, #0
	bl 0x0200a900
	adds	r0, r5, #0
	bl 0x0200a8a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	adds	r0, r5, #0
	bl 0x0200a8a8
	movs	r6, #0
	str	r6, [r0, #108]
	movs	r1, #0
	movs	r0, #0
	bl 0x0200a778
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200a7d8
	movs	r0, #140
	lsls	r0, r0, #2
	movs	r1, #0
	bl 0x0200a7e8
	pop	{r5, r6, pc}
	push	{lr}
	adds	r0, r1, #0
	movs	r1, #2
	bl 0x0200a8d8
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #49
	bl 0x0200a7d0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #50
	bl 0x0200a7d8
	pop	{pc}
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r0, r6, #0
	sub	sp, #8
	bl 0x0200a8a8
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r7, r3, #20
	cmp	r7, #7
	bne.n	.L_02000b54
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x0200a8d8
	adds	r2, r5, #0
	adds	r2, #89
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	movs	r0, #192
	orrs	r3, r2
	lsls	r0, r0, #2
	strb	r3, [r1, #0]
	adds	r0, #50
	bl 0x0200a7d0
	movs	r0, #141
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200a7d0
	movs	r3, #15
	str	r3, [sp, #4]
	movs	r0, #7
	movs	r1, #17
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	bl 0x0200a840
.L_02000b54:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	push	{lr}
	adds	r0, r1, #0
	movs	r1, #2
	sub	sp, #8
	bl 0x0200a8d8
	movs	r0, #205
	lsls	r0, r0, #2
	bl 0x0200a7d0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #53
	bl 0x0200a7d8
	movs	r3, #27
	movs	r2, #13
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #27
	movs	r1, #14
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a840
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r0, r6, #0
	sub	sp, #8
	bl 0x0200a8a8
	adds	r5, r0, #0
	ldr	r3, [r5, #16]
	asrs	r7, r3, #20
	cmp	r7, #16
	bne.n	.L_02000c10
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x0200a8d8
	adds	r2, r5, #0
	adds	r2, #89
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	movs	r0, #192
	orrs	r3, r2
	lsls	r0, r0, #2
	strb	r3, [r1, #0]
	adds	r0, #53
	bl 0x0200a7d0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #54
	bl 0x0200a7d0
	movs	r1, #128
	movs	r2, #128
	adds	r0, r6, #0
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200a8b0
	movs	r1, #32
	movs	r2, #0
	negs	r1, r1
	adds	r0, r6, #0
	bl 0x0200a8c0
	adds	r0, r6, #0
	bl 0x0200a8c8
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x0200a8d8
	movs	r3, #25
	str	r3, [sp, #0]
	movs	r0, #25
	movs	r1, #18
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #4]
	bl 0x0200a840
.L_02000c10:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0x49026d02
	.4byte 0x185b8a53
	.4byte 0x47708253
	.2byte 0xf800
	.2byte 0xffff
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	ldr	r2, [pc, #104]
	ldr	r3, [r2, #0]
	mov	r8, r2
	subs	r3, #1
	str	r3, [r2, #0]
	bl 0x0200a7b0
	adds	r6, r0, #0
	bl 0x0200a7b0
	adds	r5, r0, #0
	lsls	r6, r6, #5
	lsls	r5, r5, #4
	movs	r3, #150
	lsls	r3, r3, #17
	adds	r5, r5, r6
	adds	r5, r5, r3
	bl 0x0200a7b0
	adds	r3, r0, #0
	lsls	r3, r3, #4
	movs	r2, #130
	lsls	r2, r2, #17
	adds	r3, r3, r6
	movs	r0, #30
	adds	r3, r3, r2
	adds	r1, r5, #0
	movs	r2, #0
	adds	r0, #255
	bl 0x0200a808
	ldr	r1, [pc, #44]
	adds	r5, r0, #0
	bl 0x0200a800
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a858
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200a860
	mov	r2, r8
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_02000c8e
	ldr	r0, [pc, #16]
	bl 0x0200a7a8
.L_02000c8e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x0200b458
	.4byte 0x0200a9c8
	.2byte 0x8c25
	.2byte 0x0200
	push	{r5, lr}
	ldr	r5, [pc, #64]
	sub	sp, #8
	ldr	r2, [r5, #0]
	subs	r1, r2, #1
	str	r1, [r5, #0]
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_02000cb4
	adds	r3, r2, #6
.L_02000cb4:
	movs	r2, #1
	ands	r1, r2
	lsls	r2, r1, #1
	movs	r0, #47
	asrs	r3, r3, #3
	subs	r0, r0, r2
	movs	r2, #15
	subs	r2, r2, r3
	movs	r1, #64
	subs	r1, r1, r3
	movs	r4, #29
	str	r2, [sp, #4]
	movs	r2, #2
	str	r4, [sp, #0]
	bl 0x0200a838
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_02000ce0
	ldr	r0, [pc, #12]
	bl 0x0200a7a8
.L_02000ce0:
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x0200b45c
	.2byte 0x8ca1
	.2byte 0x0200
	push	{r5, lr}
	ldr	r5, [pc, #52]
	movs	r3, #1
	ldr	r2, [r5, #0]
	sub	sp, #8
	subs	r2, #1
	ands	r3, r2
	lsls	r1, r3, #2
	str	r2, [r5, #0]
	adds	r1, r1, r3
	movs	r2, #15
	movs	r3, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #5
	adds	r1, #59
	movs	r0, #50
	movs	r2, #13
	bl 0x0200a838
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_02000d20
	ldr	r0, [pc, #12]
	bl 0x0200a7a8
.L_02000d20:
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x0200b460
	.2byte 0x8ced
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #19
	sub	sp, #8
	bl 0x0200a8a8
	mov	r9, r0
	bl 0x0200a898
	movs	r0, #0
	bl 0x0200a968
	movs	r0, #9
	mov	fp, r0
.L_02000d52:
	movs	r0, #1
	bl 0x0200a798
	mov	r1, r9
	ldr	r2, [r1, #80]
	ldr	r0, [pc, #544]
	ldrh	r3, [r2, #18]
	adds	r3, r3, r0
	strh	r3, [r2, #18]
	ldr	r3, [r1, #80]
	ldrh	r0, [r3, #18]
	bl 0x0200a7c0
	mov	r1, r9
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [r1, #8]
	asrs	r0, r0, #1
	subs	r3, r3, r0
	movs	r2, #1
	str	r3, [r1, #8]
	negs	r2, r2
	movs	r3, #128
	lsls	r3, r3, #24
	add	fp, r2
	str	r3, [r1, #56]
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_02000d52
	ldr	r3, [pc, #500]
	movs	r2, #128
	str	r3, [r1, #108]
	movs	r1, #128
	movs	r0, #19
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200a8b0
	movs	r1, #246
	lsls	r1, r1, #1
	movs	r0, #19
	movs	r2, #246
	bl 0x0200a8b8
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	mov	r0, r9
	mov	r2, r9
	str	r3, [r0, #72]
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	mov	r3, r9
	movs	r5, #0
	adds	r3, #34
	strb	r5, [r3, #0]
	movs	r0, #19
	bl 0x0200a8c8
	mov	r1, r9
	ldr	r3, [r1, #20]
	str	r5, [r1, #40]
	str	r3, [r1, #12]
	movs	r2, #248
	movs	r1, #240
	lsls	r1, r1, #17
	movs	r0, #19
	lsls	r2, r2, #16
	bl 0x0200a8d0
	mov	r2, r9
	ldr	r3, [r2, #80]
	movs	r0, #188
	strh	r5, [r3, #18]
	bl 0x0200a9b0
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x0200a868
	movs	r0, #141
	bl 0x0200a9b0
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200a868
	movs	r1, #240
	movs	r2, #248
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200a8d0
	movs	r1, #220
	movs	r0, #10
	lsls	r1, r1, #1
	movs	r2, #232
	bl 0x0200a8b8
	movs	r0, #10
	bl 0x0200a8a8
	movs	r3, #160
	lsls	r3, r3, #12
	str	r3, [r0, #40]
	mov	r3, r9
	str	r5, [r3, #108]
	movs	r0, #15
	mov	fp, r0
.L_02000e42:
	mov	r2, r9
	ldr	r1, [r2, #8]
	ldr	r2, [r2, #12]
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r0, r9
	adds	r2, r2, r3
	ldr	r3, [r0, #16]
	movs	r0, #255
	bl 0x0200a808
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02000ed0
	bl 0x0200a7b0
	mov	r8, r0
	bl 0x0200a7b0
	adds	r6, r0, #0
	bl 0x0200a7b0
	movs	r1, #128
.L_02000e70:
	adds	r5, r0, #0
	lsls	r1, r1, #5
	lsrs	r5, r5, #2
	adds	r5, r5, r1
	adds	r0, r7, #0
	ldr	r1, [pc, #268]
	bl 0x0200a800
	movs	r1, #0
	adds	r0, r7, #0
	bl 0x0200a858
	mov	r2, r8
	movs	r0, #128
	lsls	r3, r2, #2
	lsls	r0, r0, #10
	adds	r3, r3, r0
	str	r3, [r7, #40]
	mov	r0, r8
	bl 0x0200a7c0
	ldr	r2, [pc, #240]
	lsls	r6, r6, #3
	adds	r1, r0, #0
	mov	sl, r2
	adds	r0, r6, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x62f8
	mov	r0, r8
	bl 0x0200a7b8
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [r7, #52]
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r0, [r7, #36]
	str	r5, [r7, #24]
	str	r5, [r7, #28]
	str	r3, [r7, #68]
.L_02000ed0:
	movs	r3, #1
	negs	r3, r3
	add	fp, r3
	mov	r0, fp
	cmp	r0, #0
	bge.n	.L_02000e42
	bl 0x0200a870
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #168]
	bl 0x0200a7a0
	movs	r0, #40
	bl 0x0200a798
	movs	r0, #230
	movs	r1, #224
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #102
	adds	r1, #204
	bl 0x0200a918
	movs	r0, #168
	movs	r1, #1
	movs	r2, #140
	negs	r1, r1
	lsls	r0, r0, #17
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x0200a920
	movs	r1, #11
	mov	fp, r1
	movs	r5, #114
.L_02000f18:
	movs	r3, #18
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	adds	r1, r5, #0
	movs	r2, #13
	movs	r3, #5
	movs	r0, #50
	bl 0x0200a838
	movs	r0, #8
	bl 0x0200a798
	movs	r2, #1
	negs	r2, r2
	add	fp, r2
	mov	r3, fp
	subs	r5, #5
	cmp	r3, #0
	bgt.n	.L_02000f18
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #76]
	bl 0x0200a7a0
	movs	r0, #60
	bl 0x0200a798
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #64]
	bl 0x0200a7a0
	movs	r0, #24
	bl 0x0200a798
	movs	r0, #120
	bl 0x0200a890
	bl 0x0200a8a0
	movs	r0, #204
	lsls	r0, r0, #2
	bl 0x0200a7d0
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffffff00
	.4byte 0x02008c15
	.4byte 0x0200aa04
	.4byte 0x0300021c
	.4byte 0x02008ca1
	.4byte 0x02008c25
	.2byte 0x8ced
	.2byte 0x0200
	push	{lr}
	adds	r0, r1, #0
	movs	r1, #2
	bl 0x0200a8d8
	movs	r0, #206
	lsls	r0, r0, #2
	bl 0x0200a7d0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #57
	bl 0x0200a7d8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r0, r6, #0
	sub	sp, #8
	bl 0x0200a8a8
	adds	r5, r0, #0
	ldr	r3, [r5, #16]
	asrs	r7, r3, #20
	cmp	r7, #21
	bne.n	.L_02001014
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x0200a8d8
	adds	r2, r5, #0
	adds	r2, #89
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	movs	r0, #192
	orrs	r3, r2
	lsls	r0, r0, #2
	strb	r3, [r1, #0]
	adds	r0, #57
	bl 0x0200a7d0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #58
	bl 0x0200a7d0
	movs	r3, #11
	str	r3, [sp, #0]
	movs	r0, #11
	movs	r1, #19
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #4]
	bl 0x0200a840
.L_02001014:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	push	{lr}
	adds	r0, r1, #0
	movs	r1, #2
	bl 0x0200a8d8
	movs	r0, #143
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200a7d0
	movs	r0, #207
	lsls	r0, r0, #2
	bl 0x0200a7d8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r0, r6, #0
	sub	sp, #8
	bl 0x0200a8a8
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r7, r3, #20
	cmp	r7, #31
	bne.n	.L_020010b6
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x0200a8d8
	adds	r2, r5, #0
	adds	r2, #89
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	movs	r0, #207
	strb	r3, [r1, #0]
	lsls	r0, r0, #2
	bl 0x0200a7d0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #61
	bl 0x0200a7d0
	movs	r1, #128
	movs	r2, #128
	adds	r0, r6, #0
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200a8b0
	movs	r2, #32
	negs	r2, r2
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200a8c0
	adds	r0, r6, #0
	bl 0x0200a8c8
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x0200a8d8
	movs	r3, #11
	str	r3, [sp, #4]
	movs	r0, #30
	movs	r1, #13
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	bl 0x0200a840
.L_020010b6:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	bl 0x0200a948
	pop	{pc}
	push	{lr}
	adds	r0, r1, #0
	movs	r1, #2
	sub	sp, #8
	bl 0x0200a8d8
	movs	r0, #208
	lsls	r0, r0, #2
	bl 0x0200a7d0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #65
	bl 0x0200a7d8
	movs	r3, #7
	movs	r2, #38
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #39
	movs	r2, #1
	movs	r3, #1
	movs	r0, #7
	bl 0x0200a840
	movs	r0, #1
	bl 0x0200a084
	add	sp, #8
	pop	{pc}
	push	{lr}
	adds	r0, r1, #0
	movs	r1, #2
	bl 0x0200a8d8
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #66
	bl 0x0200a7d0
	movs	r0, #145
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200a7d8
	movs	r0, #1
	bl 0x0200a084
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r2, #210
	movs	r1, #64
	lsls	r2, r2, #2
	bl 0x0200a958
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200a9a0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x224a
	.2byte 0x0000
	.global Func_02001148
	.thumb_func
Func_02001148:
	push	{lr}
	ldr	r3, [pc, #124]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #116]
	cmp	r2, r3
	bne.n	.L_02001160
	ldr	r0, [pc, #112]
	b.n	.L_020011c6
.L_02001160:
	ldr	r3, [pc, #112]
	cmp	r2, r3
	bne.n	.L_0200116a
	ldr	r0, [pc, #112]
	b.n	.L_020011c6
.L_0200116a:
	ldr	r3, [pc, #112]
	cmp	r2, r3
	bne.n	.L_02001174
	ldr	r0, [pc, #108]
	b.n	.L_020011c6
.L_02001174:
	ldr	r3, [pc, #108]
	cmp	r2, r3
	bne.n	.L_0200117e
	ldr	r0, [pc, #108]
	b.n	.L_020011c6
.L_0200117e:
	ldr	r3, [pc, #108]
	cmp	r2, r3
	bne.n	.L_02001188
	ldr	r0, [pc, #104]
	b.n	.L_020011c6
.L_02001188:
	ldr	r3, [pc, #104]
	cmp	r2, r3
	bne.n	.L_02001192
	ldr	r0, [pc, #104]
	b.n	.L_020011c6
.L_02001192:
	ldr	r3, [pc, #104]
	cmp	r2, r3
	bne.n	.L_0200119c
	ldr	r0, [pc, #100]
	b.n	.L_020011c6
.L_0200119c:
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bne.n	.L_020011a6
	ldr	r0, [pc, #100]
	b.n	.L_020011c6
.L_020011a6:
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bne.n	.L_020011b0
	ldr	r0, [pc, #96]
	b.n	.L_020011c6
.L_020011b0:
	ldr	r3, [pc, #96]
	cmp	r2, r3
	bne.n	.L_020011ba
	ldr	r0, [pc, #96]
	b.n	.L_020011c6
.L_020011ba:
	ldr	r3, [pc, #96]
	cmp	r2, r3
	bne.n	.L_020011c4
	ldr	r0, [pc, #92]
	b.n	.L_020011c6
.L_020011c4:
	ldr	r0, [pc, #92]
.L_020011c6:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000008f
	.4byte 0x0200b038
	.4byte 0x00000090
	.4byte 0x0200b068
	.4byte 0x00000091
	.4byte 0x0200b128
	.4byte 0x00000092
	.4byte 0x0200b194
	.4byte 0x00000093
	.4byte 0x0200b1e8
	.4byte 0x00000094
	.4byte 0x0200b314
	.4byte 0x00000095
	.4byte 0x0200b338
	.4byte 0x00000096
	.4byte 0x0200b3e0
	.4byte 0x00000097
	.4byte 0x0200b4e8
	.4byte 0x00000098
	.4byte 0x0200b464
	.2byte 0x0099
	.2byte 0x0000
	push	{r5, r6, lr}
	lsls	r0, r0, #8
	add	sp, #176
	lsls	r0, r0, #8
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r4, [r6, #12]
	ldr	r3, [r6, #40]
	ldr	r2, [r6, #20]
	adds	r3, r4, r3
	cmp	r3, r2
	bge.n	.L_0200127e
	movs	r0, #14
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #8]
	adds	r0, #255
	adds	r2, r4, #0
	bl 0x0200a808
	movs	r3, #140
	lsls	r3, r3, #8
	adds	r5, r0, #0
	adds	r3, #204
	cmp	r5, #0
	beq.n	.L_0200126c
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldr	r1, [pc, #44]
	bl 0x0200a800
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a858
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200a7f8
.L_0200126c:
	movs	r0, #106
	bl 0x0200a9b0
	ldr	r3, [r6, #20]
	movs	r0, #0
	str	r3, [r6, #12]
	movs	r3, #0
	str	r3, [r6, #40]
	b.n	.L_02001280
.L_0200127e:
	movs	r0, #1
.L_02001280:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xaa48
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #112]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020012f8
	bl 0x0200a7b0
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bcs.n	.L_020012f8
	bl 0x0200a7b0
	movs	r3, #130
	adds	r5, r0, #0
	lsls	r3, r3, #18
	lsls	r5, r5, #5
	adds	r5, r5, r3
	bl 0x0200a7b0
	movs	r3, #176
	lsls	r0, r0, #3
	lsls	r3, r3, #16
	subs	r3, r3, r0
	movs	r2, #160
	movs	r0, #46
	adds	r1, r5, #0
	lsls	r2, r2, #16
	adds	r0, #255
	bl 0x0200a808
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r5, r0, #0
	adds	r3, #122
	cmp	r5, #0
	beq.n	.L_020012f8
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	movs	r3, #230
	lsls	r3, r3, #7
	adds	r3, #51
	str	r3, [r5, #72]
	ldr	r1, [pc, #28]
	bl 0x0200a800
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a858
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x0200a880
.L_020012f8:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.2byte 0xaa54
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200a730
	adds	r5, r0, #0
	cmp	r5, #180
	beq.n	.L_02001344
	cmp	r5, #180
	bgt.n	.L_0200131a
	cmp	r5, #0
	beq.n	.L_02001328
	b.n	.L_020013de
.L_0200131a:
	cmp	r5, #240
	beq.n	.L_0200136e
	movs	r1, #210
	lsls	r1, r1, #1
	cmp	r5, r1
	beq.n	.L_020013a6
	b.n	.L_020013de
.L_02001328:
	movs	r1, #1
	movs	r0, #0
	bl 0x0200a71c
	movs	r1, #1
	movs	r0, #1
	bl 0x0200a71c
	movs	r1, #1
	movs	r0, #4
	bl 0x0200a71c
	movs	r0, #5
	b.n	.L_02001398
.L_02001344:
	movs	r1, #0
	movs	r0, #0
	bl 0x0200a71c
	movs	r1, #0
	movs	r0, #1
	bl 0x0200a71c
	movs	r1, #0
	movs	r0, #4
	bl 0x0200a71c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a71c
	movs	r0, #1
	negs	r0, r0
	bl 0x0200a978
	b.n	.L_020013de
.L_0200136e:
	movs	r1, #1
	movs	r0, #2
	bl 0x0200a71c
	movs	r1, #1
	movs	r0, #3
	bl 0x0200a71c
	movs	r1, #1
	movs	r0, #6
	bl 0x0200a71c
	movs	r1, #1
	movs	r0, #7
	bl 0x0200a71c
	movs	r1, #1
	movs	r0, #8
	bl 0x0200a71c
	movs	r0, #9
.L_02001398:
	movs	r1, #1
	bl 0x0200a71c
	movs	r0, #170
	bl 0x0200a978
	b.n	.L_020013de
.L_020013a6:
	movs	r1, #0
	movs	r0, #2
	bl 0x0200a71c
	movs	r1, #0
	movs	r0, #3
	bl 0x0200a71c
	movs	r1, #0
	movs	r0, #6
	bl 0x0200a71c
	movs	r1, #0
	movs	r0, #7
	bl 0x0200a71c
	movs	r1, #0
	movs	r0, #8
	bl 0x0200a71c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a71c
	movs	r0, #1
	negs	r0, r0
	bl 0x0200a978
.L_020013de:
	adds	r3, r5, #0
	subs	r3, #251
	cmp	r3, #178
	bhi.n	.L_0200141a
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #110
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_0200141a
	movs	r0, #59
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_0200141a
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_0200141a
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #181
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #100
	strh	r3, [r2, #0]
.L_0200141a:
	movs	r0, #240
	lsls	r0, r0, #1
	bl 0x0200a73c
	pop	{r5, pc}
	push	{r5, lr}
	bl 0x0200a730
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001436
	cmp	r5, #120
	beq.n	.L_0200144e
	b.n	.L_02001466
.L_02001436:
	movs	r1, #1
	movs	r0, #0
	bl 0x0200a71c
	movs	r0, #1
	movs	r1, #1
	bl 0x0200a71c
	movs	r0, #170
	bl 0x0200a978
	b.n	.L_02001466
.L_0200144e:
	movs	r1, #0
	movs	r0, #0
	bl 0x0200a71c
	movs	r0, #1
	movs	r1, #0
	bl 0x0200a71c
	movs	r0, #1
	negs	r0, r0
	bl 0x0200a978
.L_02001466:
	adds	r3, r5, #0
	subs	r3, #11
	cmp	r3, #118
	bhi.n	.L_02001496
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001484
	movs	r0, #8
	bl 0x02008810
	b.n	.L_02001496
.L_02001484:
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001496
	movs	r0, #8
	bl 0x020088d8
.L_02001496:
	movs	r0, #240
	lsls	r0, r0, #1
	bl 0x0200a73c
	pop	{r5, pc}
	push	{r5, lr}
	bl 0x0200a730
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020014b2
	cmp	r5, #120
	beq.n	.L_020014ca
	b.n	.L_020014e2
.L_020014b2:
	movs	r1, #1
	movs	r0, #0
	bl 0x0200a71c
	movs	r0, #1
	movs	r1, #1
	bl 0x0200a71c
	movs	r0, #170
	bl 0x0200a978
	b.n	.L_020014e2
.L_020014ca:
	movs	r1, #0
	movs	r0, #0
	bl 0x0200a71c
	movs	r0, #1
	movs	r1, #0
	bl 0x0200a71c
	movs	r0, #1
	negs	r0, r0
	bl 0x0200a978
.L_020014e2:
	adds	r3, r5, #0
	subs	r3, #11
	cmp	r3, #118
	bhi.n	.L_02001512
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001500
	movs	r0, #10
	bl 0x020089c0
	b.n	.L_02001512
.L_02001500:
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001512
	movs	r0, #10
	bl 0x02008a94
.L_02001512:
	movs	r0, #240
	lsls	r0, r0, #1
	bl 0x0200a73c
	pop	{r5, pc}
	push	{lr}
	bl 0x0200a730
	cmp	r0, #0
	beq.n	.L_0200152c
	cmp	r0, #180
	beq.n	.L_02001544
	b.n	.L_0200155c
.L_0200152c:
	movs	r1, #1
	movs	r0, #0
	bl 0x0200a71c
	movs	r0, #1
	movs	r1, #1
	bl 0x0200a71c
	movs	r0, #170
	bl 0x0200a978
	b.n	.L_0200155c
.L_02001544:
	movs	r1, #0
	movs	r0, #0
	bl 0x0200a71c
	movs	r0, #1
	movs	r1, #0
	bl 0x0200a71c
	movs	r0, #1
	negs	r0, r0
	bl 0x0200a978
.L_0200155c:
	movs	r0, #240
	bl 0x0200a73c
	pop	{pc}
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r2, [r6, #40]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	ldr	r2, [r6, #20]
	cmp	r3, r2
	bge.n	.L_02001610
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7c8
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_0200158e
	movs	r0, #106
	bl 0x0200a9b0
.L_0200158a:
	movs	r0, #0
	b.n	.L_02001612
.L_0200158e:
	movs	r0, #14
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	adds	r0, #255
	bl 0x0200a808
	movs	r3, #140
	lsls	r3, r3, #8
	adds	r5, r0, #0
	adds	r3, #204
	cmp	r5, #0
	beq.n	.L_020015c2
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldr	r1, [pc, #100]
	bl 0x0200a800
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a858
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200a7f8
.L_020015c2:
	movs	r0, #106
	bl 0x0200a9b0
	ldr	r3, [r6, #20]
	movs	r0, #130
	str	r3, [r6, #12]
	str	r7, [r6, #40]
	lsls	r0, r0, #1
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_0200158a
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x0200a7e0
	adds	r5, r0, #0
	cmp	r5, #3
	bgt.n	.L_020015f4
	adds	r5, #1
	movs	r0, #140
	lsls	r0, r0, #2
	adds	r1, r5, #0
	bl 0x0200a7e8
.L_020015f4:
	cmp	r5, #0
	beq.n	.L_02001600
	movs	r0, #0
	movs	r1, #10
	bl 0x02008920
.L_02001600:
	movs	r0, #10
	bl 0x0200a8a8
	lsls	r3, r5, #14
	adds	r6, r0, #0
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	b.n	.L_0200158a
.L_02001610:
	movs	r0, #1
.L_02001612:
	pop	{r5, r6, r7, pc}
	.2byte 0xaa48
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #72]
	movs	r2, #63
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02001660
	movs	r1, #204
	movs	r3, #220
	movs	r2, #160
	movs	r0, #46
	lsls	r3, r3, #17
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	adds	r0, #255
	bl 0x0200a808
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r5, r0, #0
	adds	r3, #122
	cmp	r5, #0
	beq.n	.L_02001660
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	movs	r3, #230
	lsls	r3, r3, #7
	adds	r3, #51
	ldr	r1, [pc, #20]
	str	r3, [r5, #72]
	bl 0x0200a800
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a858
.L_02001660:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.2byte 0xaa60
	.2byte 0x0200
	push	{lr}
	ldr	r2, [r0, #40]
	ldr	r3, [r0, #12]
	movs	r0, #0
	adds	r3, r3, r2
	ldr	r2, [pc, #8]
	cmp	r3, r2
	blt.n	.L_0200167e
	movs	r0, #1
.L_0200167e:
	pop	{pc}
	.2byte 0x0000
	.2byte 0xffd0
	.2byte 0xb520
	ldr	r3, [pc, #104]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020016ee
	bl 0x0200a7b0
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bcs.n	.L_020016ee
	bl 0x0200a7b0
	movs	r3, #192
	lsls	r0, r0, #5
	lsls	r3, r3, #17
	adds	r1, r0, r3
	movs	r3, #200
	lsls	r3, r3, #17
	subs	r3, r3, r0
	movs	r2, #160
	movs	r0, #46
	lsls	r2, r2, #16
	adds	r0, #255
	bl 0x0200a808
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r5, r0, #0
	adds	r3, #122
	cmp	r5, #0
	beq.n	.L_020016ee
	adds	r2, r5, #0
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r3, [pc, #28]
	ldr	r1, [pc, #32]
	str	r3, [r5, #20]
	movs	r3, #230
	lsls	r3, r3, #7
	adds	r3, #51
	str	r3, [r5, #72]
	bl 0x0200a800
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a858
.L_020016ee:
	pop	{r5, pc}
	.4byte 0x0300122c
	.4byte 0xffc00000
	.2byte 0xaa6c
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #104]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02001766
	bl 0x0200a7b0
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bcs.n	.L_02001766
	bl 0x0200a7b0
	movs	r3, #208
	lsls	r0, r0, #5
	lsls	r3, r3, #17
	adds	r1, r0, r3
	movs	r3, #176
	lsls	r3, r3, #17
	subs	r3, r3, r0
	movs	r2, #160
	movs	r0, #46
	lsls	r2, r2, #16
	adds	r0, #255
	bl 0x0200a808
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r5, r0, #0
	adds	r3, #122
	cmp	r5, #0
	beq.n	.L_02001766
	adds	r2, r5, #0
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r3, [pc, #28]
	ldr	r1, [pc, #32]
	str	r3, [r5, #20]
	movs	r3, #230
	lsls	r3, r3, #7
	adds	r3, #51
	str	r3, [r5, #72]
	bl 0x0200a800
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a858
.L_02001766:
	pop	{r5, pc}
	.4byte 0x0300122c
	.4byte 0xffc00000
	.2byte 0xaa6c
	.2byte 0x0200
	.global Func_02001774
	.thumb_func
Func_02001774:
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #844]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r5, r0
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #32]
	ldr	r3, [r3, #108]
	subs	r0, #52
	adds	r1, r3, r0
	ldr	r3, [pc, #824]
	movs	r7, #0
	sub	sp, #8
	str	r7, [r1, #0]
	cmp	r2, r3
	bne.n	.L_02001850
	ldr	r2, [pc, #816]
	movs	r1, #152
	lsls	r1, r1, #2
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #98
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	bl 0x0200a120
	movs	r5, #8
.L_020017b6:
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #216
	adds	r0, r5, r1
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_020017d2
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a8d0
	b.n	.L_020017de
.L_020017d2:
	adds	r0, r5, #0
	bl 0x0200a8a8
	movs	r3, #160
	lsls	r3, r3, #9
	str	r3, [r0, #28]
.L_020017de:
	adds	r5, #1
	cmp	r5, #12
	ble.n	.L_020017b6
	movs	r0, #13
	movs	r1, #3
	bl 0x0200a900
	movs	r0, #13
	bl 0x0200a8a8
	movs	r5, #0
	adds	r0, #89
	strb	r5, [r0, #0]
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001818
	movs	r3, #47
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #50
	movs	r1, #16
	movs	r2, #1
	movs	r3, #2
	bl 0x0200a840
.L_02001818:
	movs	r0, #14
	bl 0x0200a8a8
	cmp	r0, #0
	beq.n	.L_02001830
	movs	r3, #192
	lsls	r3, r3, #8
	str	r3, [r0, #24]
	str	r3, [r0, #28]
	adds	r3, r0, #0
	adds	r3, #85
	strb	r5, [r3, #0]
.L_02001830:
	movs	r1, #144
	ldr	r0, [pc, #668]
	lsls	r1, r1, #3
	bl 0x0200a7a0
	ldrb	r2, [r6, #23]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	ldr	r2, [pc, #652]
	strb	r3, [r6, #23]
	movs	r3, #93
	strb	r3, [r2, #0]
	b.n	.L_02001e96
.L_02001850:
	ldr	r3, [pc, #644]
	cmp	r2, r3
	bne.n	.L_0200188a
	movs	r3, #129
	lsls	r3, r3, #2
	movs	r0, #192
	str	r3, [r1, #0]
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_02001870
	movs	r0, #64
	movs	r1, #1
	bl 0x0200a950
.L_02001870:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001880
	b.n	.L_02001e96
.L_02001880:
	movs	r0, #65
	movs	r1, #1
	bl 0x0200a950
	b.n	.L_02001e96
.L_0200188a:
	ldr	r3, [pc, #592]
	cmp	r2, r3
	bne.n	.L_020018b6
	movs	r3, #129
	lsls	r3, r3, #2
	movs	r0, #0
	str	r3, [r1, #0]
	bl 0x0200a960
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_020018aa
	b.n	.L_02001e96
.L_020018aa:
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a8d0
	b.n	.L_02001e96
.L_020018b6:
	ldr	r3, [pc, #552]
	cmp	r2, r3
	bne.n	.L_02001922
	movs	r3, #129
	lsls	r3, r3, #2
	movs	r0, #0
	str	r3, [r1, #0]
	bl 0x0200a960
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_020018de
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a8d0
.L_020018de:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #109
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_020018fa
	movs	r1, #140
	movs	r2, #232
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200a8d0
.L_020018fa:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #110
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001916
	movs	r1, #236
	movs	r2, #248
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200a8d0
.L_02001916:
	ldr	r0, [pc, #460]
	ldr	r1, [pc, #460]
	ldr	r2, [pc, #464]
	bl 0x0200a614
	b.n	.L_02001e96
.L_02001922:
	ldr	r3, [pc, #460]
	cmp	r2, r3
	bne.n	.L_02001930
	movs	r3, #129
	lsls	r3, r3, #2
	str	r3, [r1, #0]
	b.n	.L_02001e96
.L_02001930:
	ldr	r3, [pc, #448]
	cmp	r2, r3
	bne.n	.L_02001a26
	movs	r3, #129
	lsls	r3, r3, #2
	str	r3, [r1, #0]
	movs	r0, #0
	bl 0x0200a960
	movs	r1, #144
	ldr	r0, [pc, #432]
	lsls	r1, r1, #3
	bl 0x0200a7a0
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	cmp	r2, #3
	beq.n	.L_0200195e
	cmp	r2, #5
	bne.n	.L_02001968
.L_0200195e:
	ldr	r0, [pc, #412]
	ldr	r2, [pc, #412]
	movs	r1, #0
	bl 0x0200a614
.L_02001968:
	movs	r0, #8
	bl 0x0200a8a8
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200a858
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_020019aa
	movs	r3, #26
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #28
	movs	r1, #8
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a840
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a7f8
	b.n	.L_020019e6
.L_020019aa:
	movs	r3, #26
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #1
	movs	r0, #22
	movs	r1, #8
	bl 0x0200a840
	movs	r0, #8
	movs	r1, #3
	bl 0x0200a900
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #2
	orrs	r3, r2
	movs	r0, #136
	strb	r3, [r1, #0]
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_020019e6
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200a7f8
.L_020019e6:
	movs	r0, #9
	bl 0x0200a8a8
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r0, #151
	adds	r3, #85
	movs	r6, #0
	lsls	r0, r0, #4
	strb	r6, [r3, #0]
	adds	r0, #255
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_02001a06
	b.n	.L_02001e96
.L_02001a06:
	movs	r1, #132
	movs	r2, #212
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200a8d0
	movs	r3, #16
	movs	r2, #26
	str	r6, [r5, #20]
	str	r6, [r5, #12]
	movs	r0, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #26
	b.n	.L_02001db6
.L_02001a26:
	ldr	r3, [pc, #220]
	cmp	r2, r3
	beq.n	.L_02001a2e
	b.n	.L_02001b56
.L_02001a2e:
	movs	r3, #129
	lsls	r3, r3, #2
	str	r3, [r1, #0]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	cmp	r2, #2
	beq.n	.L_02001a5e
	cmp	r2, #4
	beq.n	.L_02001a5e
	cmp	r2, #9
	beq.n	.L_02001a5e
	bl 0x0200a980
	movs	r1, #194
	movs	r0, #0
	lsls	r1, r1, #2
	movs	r2, #8
	movs	r3, #9
	bl 0x0200a988
	b.n	.L_02001e96
.L_02001a5e:
	ldr	r2, [pc, #168]
	movs	r1, #0
	ldr	r0, [pc, #168]
	bl 0x0200a614
	ldr	r3, [pc, #88]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200a8a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r1, #144
	strb	r3, [r0, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #136]
	bl 0x0200a7a0
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #132]
	bl 0x0200a7a0
	movs	r0, #10
	bl 0x0200a8a8
	adds	r6, r0, #0
	adds	r3, r6, #0
	adds	r3, #85
	movs	r5, #0
	strb	r5, [r3, #0]
	movs	r1, #0
	bl 0x0200a858
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001b18
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a7f8
	b.n	.L_02001b40
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000090
	.4byte 0x0000008f
	.4byte 0x02009289
	.4byte 0x03001174
	.4byte 0x00000091
	.4byte 0x00000092
	.4byte 0x00000093
	.4byte 0x0200aa78
	.4byte 0x0200aab6
	.4byte 0x02009305
	.4byte 0x00000094
	.4byte 0x00000095
	.4byte 0x02009685
	.4byte 0x0200aabc
	.4byte 0x02009425
	.4byte 0x00000096
	.4byte 0x020094a1
	.4byte 0x0200aaca
	.4byte 0x020096fd
	.2byte 0x9619
	.2byte 0x0200
.L_02001b18:
	movs	r0, #10
	movs	r1, #3
	bl 0x0200a900
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #2
	orrs	r3, r2
	movs	r0, #136
	strb	r3, [r1, #0]
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_02001b40
	adds	r0, r6, #0
	movs	r1, #7
	bl 0x0200a7f8
.L_02001b40:
	movs	r3, #60
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #50
	movs	r1, #36
	movs	r2, #13
	movs	r3, #13
	bl 0x0200a838
	b.n	.L_02001e96
.L_02001b56:
	ldr	r3, [pc, #836]
	cmp	r2, r3
	bne.n	.L_02001c46
	movs	r3, #129
	lsls	r3, r3, #2
	str	r3, [r1, #0]
	ldr	r0, [pc, #828]
	ldr	r2, [pc, #828]
	movs	r1, #0
	bl 0x0200a614
	movs	r0, #206
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_02001b84
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #57
	bl 0x0200a7d0
	b.n	.L_02001bde
.L_02001b84:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #58
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001bde
	movs	r0, #8
	movs	r1, #3
	bl 0x0200a8d8
	movs	r1, #184
	movs	r2, #172
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	movs	r0, #8
	bl 0x0200a8d0
	movs	r0, #8
	bl 0x0200a8a8
	adds	r0, #89
	strb	r7, [r0, #0]
	movs	r0, #8
	bl 0x0200a8a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #3
	movs	r0, #8
	bl 0x0200a900
	movs	r3, #11
	movs	r2, #21
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #11
	movs	r1, #19
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a840
.L_02001bde:
	movs	r0, #143
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_02001bf6
	movs	r0, #207
	lsls	r0, r0, #2
	bl 0x0200a7d0
	b.n	.L_02001e96
.L_02001bf6:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #61
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_02001c06
	b.n	.L_02001e96
.L_02001c06:
	movs	r0, #9
	movs	r1, #3
	bl 0x0200a8d8
	movs	r1, #252
	movs	r2, #184
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #9
	bl 0x0200a8d0
	movs	r0, #9
	bl 0x0200a8a8
	movs	r3, #0
	adds	r0, #89
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200a8a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #11
	movs	r3, #31
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #30
	movs	r1, #13
	b.n	.L_02001db6
.L_02001c46:
	ldr	r3, [pc, #608]
	cmp	r2, r3
	beq.n	.L_02001c4e
	b.n	.L_02001dca
.L_02001c4e:
	movs	r3, #129
	lsls	r3, r3, #2
	str	r3, [r1, #0]
	movs	r0, #19
	bl 0x0200a8a8
	adds	r5, r0, #0
	movs	r0, #204
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001cba
	movs	r1, #240
	movs	r2, #248
	movs	r0, #19
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200a8d0
	movs	r0, #10
	adds	r0, #255
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_02001c90
	movs	r1, #220
	movs	r2, #232
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200a8d0
.L_02001c90:
	movs	r3, #29
	movs	r2, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #47
	movs	r1, #50
	movs	r2, #2
	movs	r3, #14
	bl 0x0200a838
	movs	r3, #18
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #50
	movs	r1, #59
	movs	r2, #13
	movs	r3, #5
	bl 0x0200a838
	b.n	.L_02001cc6
.L_02001cba:
	adds	r3, r5, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r5, #12]
.L_02001cc6:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #49
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_02001ce0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #50
	bl 0x0200a7d0
	b.n	.L_02001d3e
.L_02001ce0:
	movs	r0, #141
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001d36
	movs	r0, #9
	movs	r1, #3
	bl 0x0200a8d8
	movs	r1, #240
	movs	r2, #248
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	movs	r0, #9
	bl 0x0200a8d0
	movs	r0, #9
	bl 0x0200a8a8
	movs	r3, #0
	adds	r0, #89
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200a8a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #15
	movs	r3, #7
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #7
	movs	r1, #17
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a840
	b.n	.L_02001d3e
.L_02001d36:
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a8d8
.L_02001d3e:
	movs	r0, #205
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_02001d56
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #53
	bl 0x0200a7d0
	b.n	.L_02001e96
.L_02001d56:
	movs	r3, #27
	movs	r2, #13
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #27
	movs	r1, #14
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a840
	movs	r0, #192
.L_02001d6c:
	lsls	r0, r0, #2
	adds	r0, #54
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001dc0
	movs	r0, #10
	movs	r1, #3
	bl 0x0200a8d8
	movs	r1, #204
	movs	r2, #132
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r0, #10
	bl 0x0200a8d0
	movs	r0, #10
	bl 0x0200a8a8
	movs	r3, #0
	adds	r0, #89
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200a8a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #16
	movs	r3, #25
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #25
	movs	r1, #18
.L_02001db6:
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a840
	b.n	.L_02001e96
.L_02001dc0:
	movs	r0, #10
	movs	r1, #0
	bl 0x0200a8d8
	b.n	.L_02001e96
.L_02001dca:
	ldr	r3, [pc, #224]
	cmp	r2, r3
	bne.n	.L_02001e96
	movs	r3, #129
	lsls	r3, r3, #2
	movs	r0, #208
	str	r3, [r1, #0]
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_02001dee
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #65
	bl 0x0200a7d0
	b.n	.L_02001e0a
.L_02001dee:
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a8d8
	movs	r3, #7
	movs	r2, #38
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #7
	movs	r1, #39
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a840
.L_02001e0a:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #66
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_02001e24
	movs	r0, #145
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200a7d0
	b.n	.L_02001e2c
.L_02001e24:
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a8d8
.L_02001e2c:
	movs	r0, #210
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_02001e40
	movs	r0, #64
	movs	r1, #0
	bl 0x0200a950
.L_02001e40:
	ldr	r3, [pc, #108]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #0
	ldrsh	r5, [r3, r0]
	subs	r3, r5, #2
	cmp	r3, #1
	bhi.n	.L_02001e6c
	movs	r0, #208
	lsls	r0, r0, #2
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001e66
	movs	r0, #0
	bl 0x0200a084
	b.n	.L_02001e6c
.L_02001e66:
	movs	r0, #0
	bl 0x0200a114
.L_02001e6c:
	cmp	r5, #6
	bne.n	.L_02001e8c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #66
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001e86
	movs	r0, #0
	bl 0x0200a084
	b.n	.L_02001e8c
.L_02001e86:
	movs	r0, #0
	bl 0x0200a114
.L_02001e8c:
	subs	r3, r5, #4
	cmp	r3, #1
	bhi.n	.L_02001e96
	bl 0x0200a828
.L_02001e96:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0x00000097
	.4byte 0x0200aad8
	.4byte 0x0200951d
	.4byte 0x00000098
	.4byte 0x00000099
	.2byte 0x0240
	.2byte 0x0200
	.global Func_02001eb4
	.thumb_func
Func_02001eb4:
	movs	r0, #0
	bx	lr
	push	{r5, lr}
	movs	r0, #210
.L_02001ebc:
	movs	r5, #1
	lsls	r0, r0, #2
	sub	sp, #8
	negs	r5, r5
	bl 0x0200a7c8
	cmp	r0, #0
	beq.n	.L_02001ed0
	movs	r5, #62
	b.n	.L_02001fc8
.L_02001ed0:
	ldr	r1, [pc, #276]
	ldr	r3, [r1, #0]
	cmp	r3, #50
	bhi.n	.L_02001fba
	ldr	r2, [pc, #272]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x02009fac
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fb0
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fb4
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fb8
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fb4
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fba
	.4byte 0x02009fb0
	.4byte 0xe004253b
	.4byte 0xe0022538
	.4byte 0xe0002535
	.2byte 0x2532
.L_02001fba:
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	cmp	r3, #59
	ble.n	.L_02001fc8
	movs	r3, #0
	str	r3, [r1, #0]
.L_02001fc8:
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	beq.n	.L_02001fe4
	movs	r3, #76
	movs	r2, #61
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #45
	adds	r1, r5, #0
	movs	r2, #3
	movs	r3, #3
	bl 0x0200a848
.L_02001fe4:
	add	sp, #8
	pop	{r5, pc}
	push	{r4, r5, r6, r7, lr}
	lsls	r0, r0, #8
	ldr	r6, [sp, #896]
	lsls	r0, r0, #8
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #32]
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r5, r7, r2
	lsls	r3, r0, #19
	str	r3, [r5, #8]
	lsls	r3, r1, #19
	str	r3, [r5, #12]
	movs	r3, #127
	strh	r3, [r5, #40]
	movs	r3, #254
	lsls	r3, r3, #6
	strh	r3, [r5, #42]
	lsls	r3, r1, #7
	strh	r3, [r5, #46]
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	lsrs	r3, r0, #31
	movs	r2, #0
	strh	r0, [r5, #44]
	asrs	r1, r1, #1
	adds	r0, r0, r3
	str	r2, [r5, #24]
	str	r2, [r5, #28]
	str	r2, [r5, #32]
	str	r2, [r5, #36]
	asrs	r0, r0, #1
	ldr	r2, [pc, #72]
	lsls	r1, r1, #7
	adds	r1, r1, r0
	lsls	r3, r1, #2
	adds	r3, r3, r2
	str	r3, [r5, #48]
	movs	r4, #128
	ldr	r3, [pc, #64]
	lsls	r4, r4, #9
	str	r4, [r5, #16]
	str	r4, [r5, #20]
	adds	r1, r1, r3
	adds	r3, r7, #0
	adds	r3, #228
	str	r1, [r5, #52]
	ldr	r6, [pc, #52]
	adds	r1, r4, #0
	ldr	r0, [r3, #0]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x68ab
	ldr	r1, [r5, #20]
	adds	r0, r0, r3
	str	r0, [r5, #0]
	adds	r3, r7, #0
	adds	r3, #232
	ldr	r0, [r3, #0]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x68eb
	adds	r0, r0, r3
	str	r0, [r5, #4]
	bl 0x0200a810
	movs	r0, #1
	bl 0x0200a798
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.4byte 0x02024000
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200a828
	cmp	r5, #0
	beq.n	.L_020020da
	movs	r3, #224
	movs	r5, #128
	lsls	r3, r3, #4
	lsls	r5, r5, #19
	adds	r3, #6
	adds	r5, #82
	strh	r3, [r5, #0]
	movs	r0, #10
	bl 0x0200a798
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #4
	strh	r3, [r5, #0]
	movs	r0, #10
	bl 0x0200a798
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #2
	strh	r3, [r5, #0]
	movs	r0, #10
	bl 0x0200a798
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #1
	strh	r3, [r5, #0]
	movs	r0, #10
	bl 0x0200a798
	movs	r3, #192
	lsls	r3, r3, #4
	strh	r3, [r5, #0]
	movs	r0, #10
	bl 0x0200a798
.L_020020da:
	movs	r0, #100
	movs	r1, #50
	bl 0x02009ff0
	movs	r2, #192
	movs	r3, #128
	lsls	r2, r2, #4
	lsls	r3, r3, #19
	adds	r2, #7
	adds	r3, #82
	strh	r2, [r3, #0]
	ldr	r3, [pc, #24]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #6
	bne.n	.L_0200210a
	movs	r1, #144
	ldr	r0, [pc, #12]
	lsls	r1, r1, #3
	bl 0x0200a7a0
.L_0200210a:
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0x9eb9
	.2byte 0x0200
	push	{lr}
	movs	r0, #100
	movs	r1, #100
	bl 0x02009ff0
	pop	{pc}
	push	{r5, lr}
	movs	r0, #10
	adds	r0, #255
	ldr	r5, [pc, #20]
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	0x0200a13a
	ldr	r3, [pc, #12]
	adds	r0, r5, #0
	movs	r1, #12
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd20
	.4byte 0x0200234c
	.2byte 0x0258
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #208]
	sub	sp, #4
	mov	r9, r0
	bl 0x0200a998
	bl 0x0200a8a8
	ldr	r7, [r0, #80]
	mov	r8, r0
	ldr	r2, [r7, #44]
	cmp	r2, #0
	beq.n	.L_0200216e
	movs	r3, #1
	strb	r3, [r2, #6]
.L_0200216e:
	ldrb	r2, [r7, #26]
	movs	r3, #8
	orrs	r3, r2
	movs	r2, #254
	ands	r3, r2
	strb	r3, [r7, #26]
	movs	r3, #1
	strb	r3, [r7, #25]
	ldr	r1, [pc, #168]
	movs	r2, #240
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020021b0
	ldr	r3, [r1, #0]
	movs	r2, #2
	movs	r0, #160
	ands	r3, r2
	lsls	r0, r0, #5
	cmp	r3, #0
	beq.n	.L_0200219e
	lsls	r3, r0, #1
	adds	r3, r3, r0
	asrs	r0, r3, #1
.L_0200219e:
	movs	r1, #96
	bl 0x0200a790
	mov	r1, r9
	ldrh	r3, [r1, #2]
	mov	r2, r9
	adds	r3, r3, r0
	adds	r3, #1
	strh	r3, [r2, #2]
.L_020021b0:
	mov	r1, r9
	movs	r3, #2
	ldrsh	r0, [r1, r3]
	adds	r2, r0, #0
	cmp	r0, #0
	bge.n	.L_020021be
	adds	r2, #255
.L_020021be:
	ldrb	r1, [r7, #17]
	asrs	r2, r2, #8
	movs	r3, #3
	lsls	r2, r2, #2
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r7, #17]
	mov	r2, r9
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #5
	cmp	r0, r3
	blt.n	.L_0200229c
	bl 0x0200a898
	movs	r0, #0
	bl 0x0200a968
	ldr	r3, [pc, #56]
	mov	r0, r8
	adds	r0, #84
	str	r0, [sp, #0]
	strb	r3, [r0, #0]
	movs	r1, #1
	mov	fp, r1
.L_020021f2:
	mov	r0, r8
	mov	r2, r8
	ldr	r3, [r0, #16]
	movs	r0, #14
	ldr	r1, [r2, #8]
	adds	r0, #255
	ldr	r2, [r2, #12]
	bl 0x0200a808
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200225a
	ldr	r1, [pc, #32]
	ldr	r6, [r5, #80]
	bl 0x0200a800
	movs	r1, #0
	adds	r3, r5, #0
	mov	sl, r1
	adds	r3, #85
	mov	r2, sl
	b.n	.L_02002230
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200234c
	.4byte 0x03001150
	.2byte 0xaae8
	.2byte 0x0200
.L_02002230:
	strb	r2, [r3, #0]
	adds	r2, r5, #0
	adds	r2, #34
	movs	r3, #1
	strb	r3, [r2, #0]
	cmp	r6, #0
	beq.n	.L_0200225a
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200a7f0
	mov	r3, sl
	strb	r3, [r6, #26]
	movs	r0, #13
	ldrb	r3, [r6, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r6, #9]
.L_0200225a:
	movs	r0, #15
	bl 0x0200a798
	movs	r1, #1
	negs	r1, r1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #0
	bge.n	.L_020021f2
	movs	r0, #30
	bl 0x0200a890
	ldr	r0, [sp, #0]
	ldrb	r2, [r7, #17]
	movs	r3, #1
	strb	r3, [r0, #0]
	movs	r3, #3
	ands	r3, r2
	movs	r5, #0
	strb	r3, [r7, #17]
	strb	r5, [r7, #26]
	bl 0x0200a8a0
	mov	r1, r9
	ldr	r3, [r1, #4]
	mov	r2, r8
	str	r3, [r2, #8]
	mov	r0, r9
	ldr	r3, [r1, #8]
	str	r3, [r2, #16]
	mov	r3, r9
	strh	r5, [r3, #0]
	strh	r5, [r0, #2]
.L_0200229c:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r7, [pc, #124]
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #2
	beq.n	.L_0200232a
	bl 0x0200a998
	bl 0x0200a8a8
	adds	r5, r0, #0
	ldr	r6, [r5, #80]
	bl 0x0200a898
	movs	r0, #0
	bl 0x0200a968
	ldr	r2, [pc, #96]
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	movs	r0, #128
	lsls	r0, r0, #12
	ands	r1, r2
	ands	r3, r2
	adds	r1, r1, r0
	adds	r3, r3, r0
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	bl 0x0200a818
	adds	r0, r5, #0
	bl 0x0200a820
	b.n	.L_02002308
.L_020022f0:
	lsrs	r2, r1, #2
	adds	r2, #255
	movs	r3, #3
	ands	r3, r1
	lsls	r2, r2, #2
	orrs	r3, r2
	strb	r3, [r6, #17]
	movs	r3, #1
	strb	r3, [r6, #25]
	movs	r0, #1
	bl 0x0200a798
.L_02002308:
	ldrb	r1, [r6, #17]
	movs	r3, #252
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_020022f0
	bl 0x0200a8a0
	ldrb	r2, [r6, #26]
	movs	r3, #8
	orrs	r3, r2
	movs	r2, #254
	ands	r3, r2
	movs	r1, #0
	strb	r3, [r6, #26]
	movs	r3, #2
	strh	r3, [r7, #0]
	strh	r1, [r7, #2]
.L_0200232a:
	pop	{r5, r6, r7, pc}
	.4byte 0x0200234c
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb520
	ldr	r5, [pc, #64]
	bl 0x0200a37c
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_02002376
	bl 0x0200a998
	bl 0x0200a8a8
	ldr	r0, [r0, #80]
	ldr	r2, [r0, #44]
	cmp	r2, #0
	beq.n	.L_02002358
	movs	r3, #9
	strb	r3, [r2, #6]
.L_02002358:
	ldrb	r2, [r0, #26]
	movs	r3, #247
	ands	r3, r2
	movs	r2, #1
	orrs	r3, r2
	ldrb	r2, [r0, #17]
	strb	r3, [r0, #26]
	movs	r3, #3
	ands	r3, r2
	movs	r1, #0
	strb	r3, [r0, #17]
	movs	r3, #1
	strb	r3, [r0, #25]
	strh	r1, [r5, #0]
	strh	r1, [r5, #2]
.L_02002376:
	pop	{r5, pc}
	.2byte 0x234c
	.2byte 0x0200
	push	{r5, lr}
	ldr	r5, [pc, #32]
	bl 0x0200a998
	bl 0x0200a8a8
	ldr	r1, [pc, #24]
	ldr	r3, [r0, #8]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [r5, #8]
	pop	{r5, pc}
	.4byte 0x0200234c
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb500
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_020023be
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_020023c8
	b.n	.L_020023f8
.L_020023be:
	movs	r2, #192
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_020023f8
.L_020023c8:
	ldr	r2, [r1, #12]
	ldr	r3, [r0, #12]
	subs	r3, r3, r2
	ldr	r2, [pc, #44]
	subs	r3, #1
	cmp	r3, r2
	bhi.n	.L_020023f8
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_020023ea
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_020023f4
	b.n	.L_020023f8
.L_020023ea:
	movs	r2, #128
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_020023f8
.L_020023f4:
	movs	r0, #1
	b.n	.L_020023fa
.L_020023f8:
	movs	r0, #0
.L_020023fa:
	pop	{pc}
	.2byte 0xfffe
	.2byte 0x000f
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #232]
	ldr	r3, [pc, #232]
	mov	r8, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #4
	bl 0x0200a8a8
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r4, r0, #0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #16]
	str	r4, [sp, #0]
	bl 0x0200a790
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	ldr	r4, [sp, #0]
	cmp	r7, #0
	bge.n	.L_0200244e
	adds	r3, #15
.L_0200244e:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_0200249a
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x0200a3a8
	cmp	r0, #0
	beq.n	.L_0200249a
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	movs	r3, #0
	str	r3, [r6, #76]
.L_0200249a:
	movs	r3, #164
	lsls	r3, r3, #1
	mov	r2, r8
	ldrh	r0, [r2, r3]
	movs	r7, #0
	cmp	r0, #0
	beq.n	.L_020024e6
	adds	r5, r2, r3
.L_020024aa:
	bl 0x0200a8a8
	adds	r4, r0, #0
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_020024d8
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x0200a3a8
	cmp	r0, #0
	beq.n	.L_020024d8
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	movs	r3, #0
	str	r3, [r6, #76]
.L_020024d8:
	adds	r7, #1
	cmp	r7, #3
	bgt.n	.L_020024e6
	adds	r5, #2
	ldrh	r0, [r5, #0]
	cmp	r0, #0
	bne.n	.L_020024aa
.L_020024e6:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r2, r5, #0
	adds	r3, #4
	strb	r6, [r3, #0]
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #3
	ldr	r0, [r5, #80]
	ands	r1, r3
	ldrb	r2, [r0, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	lsls	r1, r1, #2
	orrs	r3, r1
	strb	r3, [r0, #9]
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200a858
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200a7f8
	adds	r0, r5, #0
	ldr	r1, [pc, #24]
	bl 0x0200a800
	adds	r0, r5, #0
	movs	r1, #10
	bl 0x0200a8f0
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0xaaf4
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #4]
	adds	r6, r1, #0
	ldr	r1, [r0, #0]
	ldr	r0, [pc, #104]
	adds	r3, r3, r0
	movs	r0, #30
	adds	r0, #255
	bl 0x0200a808
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020025c6
	bl 0x0200a7b0
	adds	r3, r0, #0
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #12
	movs	r3, #192
	lsls	r3, r3, #6
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	bl 0x0200a7c0
	str	r0, [r5, #68]
	bl 0x0200a7b0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	str	r0, [r5, #72]
	bl 0x0200a7b0
	lsls	r0, r0, #17
	lsrs	r0, r0, #16
	adds	r0, r0, r6
	str	r0, [r5, #76]
	bl 0x0200a7b0
	ldr	r3, [pc, #36]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r5, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	movs	r1, #3
	adds	r0, r5, #0
	bl 0x0200a4f8
	ldr	r3, [pc, #20]
	adds	r0, r5, #0
	str	r3, [r5, #108]
	movs	r1, #1
	bl 0x0200a860
.L_020025c6:
	pop	{r5, r6, pc}
	.4byte 0xfffe0000
	.4byte 0xffff8000
	.2byte 0xa401
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #56]
	movs	r6, #15
	ldr	r0, [r3, #4]
	adds	r5, r3, #0
	mov	lr, r0
	.2byte 0xf800
	.2byte 0x3508
.L_020025e4:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L_02002606
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #2]
	cmp	r3, #0
	bgt.n	.L_02002602
	adds	r0, r5, #4
	ldr	r1, [r5, #16]
	bl 0x0200a554
	movs	r3, #9
	b.n	.L_02002604
.L_02002602:
	subs	r3, r2, #1
.L_02002604:
	strh	r3, [r5, #2]
.L_02002606:
	subs	r6, #1
	adds	r5, #20
	cmp	r6, #0
	bge.n	.L_020025e4
	pop	{r5, r6, pc}
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r0, #0
	ldr	r0, [pc, #216]
	mov	r9, r2
	mov	sl, r0
	movs	r2, #8
	movs	r0, #10
	add	r2, sl
	adds	r0, #255
	sub	sp, #4
	adds	r7, r1, #0
	mov	r8, r2
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_020026bc
	movs	r1, #168
	lsls	r1, r1, #1
	ldr	r3, [pc, #188]
	mov	r0, sl
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x8831
	movs	r4, #0
	adds	r6, #2
	cmp	r1, #0
	ble.n	.L_02002694
.L_02002652:
	ldrh	r2, [r6, #0]
	mov	r0, r8
	movs	r3, #0
	ldrh	r5, [r6, #2]
	strh	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #13
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	str	r2, [r0, #12]
	str	r1, [r0, #4]
	adds	r2, r2, r3
	movs	r0, #0
	str	r4, [sp, #0]
	bl 0x0200a830
	ldr	r4, [sp, #0]
	mov	r2, r8
	mov	r3, r8
	lsls	r5, r5, #16
	str	r0, [r2, #8]
	str	r5, [r2, #16]
	movs	r0, #20
	strh	r4, [r3, #2]
	adds	r4, #1
	adds	r6, #4
	add	r8, r0
	cmp	r4, #15
	bgt.n	.L_02002694
	ldrh	r1, [r6, #0]
	adds	r6, #2
	cmp	r1, #0
	bgt.n	.L_02002652
.L_02002694:
	cmp	r7, #0
	beq.n	.L_020026bc
	ldrh	r1, [r7, #0]
	movs	r4, #0
	adds	r7, #2
	cmp	r1, #0
	ble.n	.L_020026bc
.L_020026a2:
	movs	r2, #164
	lsls	r3, r4, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	r0, sl
	adds	r4, #1
	strh	r1, [r0, r3]
	cmp	r4, #3
	bgt.n	.L_020026bc
	ldrh	r1, [r7, #0]
	adds	r7, #2
	cmp	r1, #0
	bgt.n	.L_020026a2
.L_020026bc:
	movs	r1, #128
	lsls	r1, r1, #19
	mov	r3, sl
	mov	r2, r9
	adds	r1, #80
	str	r2, [r3, #4]
	ldrh	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_020026e4
	movs	r3, #192
	movs	r2, #128
	lsls	r3, r3, #4
	lsls	r2, r2, #19
	adds	r3, #8
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #16
	strh	r3, [r1, #0]
.L_020026e4:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #24]
	bl 0x0200a7a0
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.4byte 0x03000258
	.4byte 0x0200a5d5
	.4byte 0x00834a03
	.4byte 0x009b181b
	.4byte 0x2208189b
	.4byte 0x47705e98
	.4byte 0x0200254c
	.4byte 0x00834a03
	.4byte 0x009b181b
	.4byte 0x8119189b
	.4byte 0x00004770
	.4byte 0x0200254c
	.4byte 0x68184b01
	.4byte 0x00004770
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r0, #130
	lsls	r0, r0, #1
	ldr	r5, [pc, #24]
	bl 0x0200a7c8
	cmp	r0, #0
	bne.n	.L_0200275a
	ldr	r3, [r5, #0]
	adds	r3, #1
	str	r3, [r5, #0]
	cmp	r3, r6
	blt.n	.L_0200275a
	str	r0, [r5, #0]
.L_0200275a:
	ldr	r0, [r5, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.4byte 0x00834a03
	.4byte 0x009b181b
	.4byte 0x6199189b
	.4byte 0x00004770
	.2byte 0x254c
	.2byte 0x0200
	push	{lr}
	ldr	r2, [pc, #16]
	cmp	r0, #3
	bhi.n	.L_0200278a
	lsls	r3, r0, #1
	movs	r0, #164
	lsls	r0, r0, #1
	adds	r3, r3, r0
	strh	r1, [r2, r3]
.L_0200278a:
	pop	{pc}
	.4byte 0x0200254c
	.section .rodata,"a",%progbits
	.4byte 0x06090200
	.4byte 0x01050604
	.4byte 0x00010003
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000026
	.4byte 0x0000002e
	.4byte 0x02009229
	.4byte 0x00000026
	.4byte 0x0000002e
	.4byte 0x02009565
	.4byte 0x00000026
	.4byte 0x0000002e
	.4byte 0x0200966d
	.4byte 0x00000026
	.4byte 0x00c800c8
	.4byte 0x01080004
	.4byte 0x000400c8
	.4byte 0x00d800e8
	.4byte 0x01180004
	.4byte 0x000400d8
	.4byte 0x00c80238
	.4byte 0x02780004
	.4byte 0x000400c8
	.4byte 0x00d80218
	.4byte 0x02580004
	.4byte 0x000400d8
	.4byte 0x00e80198
	.4byte 0x01d80004
	.4byte 0x000400e8
	.4byte 0x00090000
	.4byte 0x0000000a
	.4byte 0x006801a8
	.4byte 0x02180004
	.4byte 0x00040088
	.4byte 0x01980000
	.4byte 0x00040198
	.4byte 0x016801d8
	.4byte 0x00000004
	.4byte 0x00b800d8
	.4byte 0x01380004
	.4byte 0x00040088
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000020
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x0000002c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000008f
	.4byte 0x00123002
	.4byte 0x00201090
	.4byte 0x00302090
	.4byte 0x00000090
	.4byte 0x0010208f
	.4byte 0x0020308f
	.4byte 0x00303091
	.4byte 0x00404094
	.4byte 0x00505091
	.4byte 0x00000091
	.4byte 0x00303090
	.4byte 0x00505090
	.4byte 0x00602093
	.4byte 0x00703093
	.4byte 0x00809091
	.4byte 0x00908091
	.4byte 0x00000092
	.4byte 0x00101093
	.4byte 0x00204093
	.4byte 0x00000093
	.4byte 0x00101092
	.4byte 0x00206091
	.4byte 0x00307091
	.4byte 0x00402092
	.4byte 0x00a0a092
	.4byte 0x00b0b092
	.4byte 0x00c0c092
	.4byte 0x00d0d092
	.4byte 0x00e0e092
	.4byte 0x00f0f092
	.4byte 0x01010092
	.4byte 0x00000094
	.4byte 0x00101095
	.4byte 0x00404090
	.4byte 0x00000095
	.4byte 0x00101094
	.4byte 0x00202096
	.4byte 0x00303096
	.4byte 0x00405095
	.4byte 0x00504095
	.4byte 0x00a09096
	.4byte 0x00c09096
	.4byte 0x00000096
	.4byte 0x00101098
	.4byte 0x00202095
	.4byte 0x00303095
	.4byte 0x00405096
	.4byte 0x00504096
	.4byte 0x00000098
	.4byte 0x00101096
	.4byte 0x00202099
	.4byte 0x00303097
	.4byte 0x00404097
	.4byte 0x00000097
	.4byte 0x00303098
	.4byte 0x00404098
	.4byte 0x00000099
	.4byte 0x00202098
	.4byte 0x00304099
	.4byte 0x00403099
	.4byte 0x00506099
	.4byte 0x00605099
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x0002c000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x0002c000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x0002c000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x0002c000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x0002c000
	.4byte 0xffff012b
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x0002c000
	.4byte 0x08e50111
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x003b00f3
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x003b00f3
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00004000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0002c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0102c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0102c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x0102c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0102c000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x0002c000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x0102c000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x0102c000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x0002c000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x0002c000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x0002c000
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
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000ce01
	.4byte 0x18e00003
	.4byte 0x00000003
	.4byte 0x0000ce01
	.4byte 0x18e30004
	.4byte 0x00000004
	.4byte 0x0000ce01
	.4byte 0x18e40005
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0xffff003c
	.4byte 0x0200a145
	.4byte 0x00000002
	.4byte 0xffff003d
	.4byte 0x0200a2ad
	.4byte 0x00000002
	.4byte 0xffff003e
	.4byte 0x0200a335
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte 0x02008105
	.4byte 0x00004e15
	.4byte 0xffff0009
	.4byte 0x02008105
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x02008105
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte 0x02008105
	.4byte 0x00004e15
	.4byte 0xffff000c
	.4byte 0x02008105
	.4byte 0x00000c15
	.4byte 0xffff000d
	.4byte 0x02008119
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x0200813d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x50008805
	.4byte 0x03010033
	.4byte 0x020082f5
	.4byte 0x50008805
	.4byte 0x03000032
	.4byte 0x02008305
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008039
	.4byte 0x50008905
	.4byte 0xffff001e
	.4byte 0x02008315
	.4byte 0x50008905
	.4byte 0xffff001f
	.4byte 0x02008335
	.4byte 0x50008905
	.4byte 0xffff0020
	.4byte 0x020083c5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte 0x02008429
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte 0x02008429
	.4byte 0x00004602
	.4byte 0xffff000c
	.4byte 0x02008429
	.4byte 0x00004602
	.4byte 0xffff000d
	.4byte 0x02008429
	.4byte 0x00004602
	.4byte 0xffff000e
	.4byte 0x02008429
	.4byte 0x00004602
	.4byte 0xffff000f
	.4byte 0x02008429
	.4byte 0x00004602
	.4byte 0xffff0010
	.4byte 0x02008429
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020084c9
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020084c9
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x020084c9
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x020084c9
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x020084c9
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x020084c9
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x020084c9
	.4byte 0x00000002
	.4byte 0xffff0011
	.4byte 0x020084c9
	.4byte 0x00000002
	.4byte 0xffff0012
	.4byte 0x020084c9
	.4byte 0x00008c15
	.4byte 0x0a6d0009
	.4byte 0x02008465
	.4byte 0x00008c15
	.4byte 0x0a6e000a
	.4byte 0x02008465
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008465
	.4byte 0x00000006
	.4byte 0x03100064
	.4byte 0x0200865d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte 0x02008429
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020084c9
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x020084c9
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020084c9
	.4byte 0x00008c15
	.4byte 0x0a6f0009
	.4byte 0x02008745
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008745
	.4byte 0x50002115
	.4byte 0x02200008
	.4byte 0x02008785
	.4byte 0x00001815
	.4byte 0x12220008
	.4byte 0x020087bd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020084c9
	.4byte 0x00008515
	.4byte 0x03080008
	.4byte 0x00000000
	.4byte 0x50002115
	.4byte 0x0220000a
	.4byte 0x02008921
	.4byte 0x00001815
	.4byte 0x1222000a
	.4byte 0x0200896d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000070
	.4byte 0x00000078
	.4byte 0x00000018
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00002115
	.4byte 0x03310009
	.4byte 0x02008add
	.4byte 0x00002115
	.4byte 0x0334000a
	.4byte 0x02008b59
	.4byte 0x00008c15
	.4byte 0x03320009
	.4byte 0x02008afd
	.4byte 0x00008c15
	.4byte 0x0335000a
	.4byte 0x02008b91
	.4byte 0x00000009
	.4byte 0x03350000
	.4byte 0x02008b91
	.4byte 0x00008715
	.4byte 0x0330000b
	.4byte 0x02008d2d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020084c9
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020084c9
	.4byte 0x00000602
	.4byte 0xffff0040
	.4byte 0x020090bd
	.4byte 0x00002115
	.4byte 0x03380008
	.4byte 0x02008f9d
	.4byte 0x00002115
	.4byte 0x033b0009
	.4byte 0x02009019
	.4byte 0x00008c15
	.4byte 0x03390008
	.4byte 0x02008fbd
	.4byte 0x00008c15
	.4byte 0x033c0009
	.4byte 0x02009039
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000053
	.4byte 0x03480032
	.4byte 0x02009139
	.4byte 0x00002115
	.4byte 0x03400008
	.4byte 0x020090c5
	.4byte 0x00002115
	.4byte 0x03420009
	.4byte 0x02009101
	.4byte 0x00008c15
	.4byte 0x03410008
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0x03430009
	.4byte 0x00000000
	.4byte 0x50008805
	.4byte 0x03480032
	.4byte 0x02009129
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
