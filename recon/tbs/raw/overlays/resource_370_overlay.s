.syntax unified
	.thumb
	.section .text.x020083cc,"ax",%progbits
	.global Func_020003cc
	.thumb_func
Func_020003cc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #728]
	movs	r0, #0
	add	sp, r5
	str	r0, [sp, #20]
	bl 0x02008054
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #716]
	bl 0x0200932c
	ldr	r3, [pc, #712]
	movs	r1, #224
	ldr	r3, [r3, #0]
	lsls	r1, r1, #1
	ldr	r2, [sp, #20]
	adds	r3, r3, r1
	str	r2, [r3, #0]
	bl 0x020094ac
	bl 0x020094bc
	ldr	r3, [pc, #696]
	movs	r0, #225
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #2
	bne.n	.L_020004a0
.L_02000416:
	ldr	r5, [pc, #684]
	movs	r1, #5
	adds	r0, r5, #0
	bl 0x02009394
	movs	r0, #1
	bl 0x020094c4
	adds	r7, r0, #0
	bl 0x020093c4
	cmp	r7, #0
	bne.n	.L_02000450
	adds	r0, r5, #1
	movs	r1, #1
	bl 0x02009394
.L_02000438:
	ldr	r3, [pc, #644]
	ldr	r2, [pc, #652]
	adds	r3, r3, r2
	movs	r2, #1
	strb	r2, [r3, #0]
	bl 0x020093ec
	movs	r3, #1
	adds	r7, r0, #0
	negs	r3, r3
	cmp	r7, r3
	beq.n	.L_02000416
.L_02000450:
	movs	r0, #60
	bl 0x02009494
	bl 0x020094b4
	movs	r0, #17
	bl 0x020094cc
	movs	r0, #150
	lsls	r0, r0, #1
	bl 0x02009494
	ldr	r0, [pc, #608]
	movs	r1, #72
	bl 0x020094a4
	bl 0x02008cbe
.L_02000474:
	movs	r0, #112
	bl 0x020094cc
	mov	r0, sl
	bl 0x0200939c
	mov	r0, sl
	movs	r1, #2
	bl 0x0200938c
	movs	r1, #2
	ldr	r0, [sp, #16]
	bl 0x0200938c
	movs	r1, #2
	ldr	r0, [sp, #12]
	bl 0x0200938c
	movs	r0, #1
	bl 0x02009324
	b.n	.L_020004a8
.L_020004a0:
	add	r0, sp, #20
	ldr	r3, [pc, #556]
	ldrb	r0, [r0, #0]
	strb	r0, [r3, #0]
.L_020004a8:
	bl 0x0200940c
	adds	r6, r0, #0
	cmp	r6, #0
	bge.n	.L_020004d2
	ldr	r3, [pc, #544]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_020004d2
	ldr	r1, [pc, #540]
	ldr	r3, [pc, #512]
	movs	r2, #1
	adds	r3, r3, r1
	strb	r2, [r3, #0]
	ldr	r3, [pc, #532]
	ldr	r0, [pc, #536]
	strb	r2, [r3, #0]
	movs	r1, #1
	movs	r2, #8
	bl 0x020093dc
.L_020004d2:
	cmp	r6, #0
	bne.n	.L_020004f6
	ldr	r2, [sp, #20]
	cmp	r2, #0
	bne.n	.L_020004f6
	movs	r0, #30
	bl 0x02009324
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #508]
	bl 0x0200932c
	movs	r0, #1
	bl 0x02009324
	movs	r3, #1
	str	r3, [sp, #20]
.L_020004f6:
	cmp	r6, #0
	ble.n	.L_02000502
	bl 0x020093cc
	adds	r6, r0, #0
	b.n	.L_02000504
.L_02000502:
	movs	r6, #0
.L_02000504:
	cmp	r6, #0
	bne.n	.L_0200057a
	bl 0x0200944c
	ldr	r2, [pc, #432]
	ldr	r0, [pc, #472]
	ldr	r1, [pc, #472]
	adds	r3, r2, r0
	adds	r2, r2, r1
	ldrb	r0, [r3, #0]
	ldrb	r1, [r2, #0]
	bl 0x0200941c
	movs	r5, #1
	movs	r7, #0
.L_02000522:
	movs	r0, #6
	bl 0x02009324
	adds	r0, r7, #0
	bl 0x02009414
	movs	r2, #1
	adds	r6, r0, #0
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L_02000540
	cmp	r7, #0
	beq.n	.L_020004a8
	subs	r7, #1
	b.n	.L_02000522
.L_02000540:
	ldr	r3, [pc, #428]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_0200054c
	movs	r5, #4
.L_0200054c:
	ldr	r3, [pc, #420]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_02000558
	movs	r5, #7
.L_02000558:
	adds	r7, #1
	cmp	r7, r5
	blt.n	.L_02000522
	bl 0x0200947c
	ldr	r1, [pc, #348]
	movs	r0, #224
	ldr	r3, [pc, #400]
	lsls	r0, r0, #1
	adds	r2, r1, r0
	strh	r3, [r2, #0]
	movs	r3, #225
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #20
	strh	r3, [r2, #0]
	b.n	.L_02000c96
.L_0200057a:
	cmp	r6, #1
	beq.n	.L_02000580
	b.n	.L_020006a0
.L_02000580:
	movs	r0, #1
	bl 0x020093f4
	adds	r6, r0, #0
	movs	r0, #1
	negs	r0, r0
	cmp	r6, r0
	beq.n	.L_020004a8
	ldr	r0, [pc, #360]
	bl 0x0200945c
	ldr	r5, [pc, #296]
	ldr	r1, [pc, #332]
	ldr	r2, [pc, #336]
	adds	r3, r5, r1
	ldrb	r0, [r3, #0]
	adds	r3, r5, r2
	ldrb	r1, [r3, #0]
	bl 0x0200941c
	bl 0x02009484
	ldr	r3, [r5, #0]
	cmp	r3, r0
	beq.n	.L_020005d8
	movs	r0, #226
	lsls	r0, r0, #1
	adds	r3, r5, r0
	movs	r1, #224
	ldrh	r2, [r3, #0]
	lsls	r1, r1, #1
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r2, #227
	lsls	r2, r2, #1
	adds	r3, r5, r2
	subs	r0, #2
	ldrh	r3, [r3, #0]
	adds	r2, r5, r0
	strh	r3, [r2, #0]
	subs	r0, #185
	bl 0x02009464
	b.n	.L_02000696
.L_020005d8:
	ldr	r3, [pc, #292]
	movs	r2, #130
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, r2
	bne.n	.L_0200061e
	bl 0x02008384
	cmp	r0, #0
	beq.n	.L_020005f2
	ldr	r0, [pc, #276]
	b.n	.L_02000664
.L_020005f2:
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r0, #224
	ldrh	r2, [r3, #0]
	lsls	r0, r0, #1
	adds	r3, r5, r0
	strh	r2, [r3, #0]
	adds	r1, #2
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	adds	r0, #2
	adds	r2, r5, r0
	strh	r3, [r2, #0]
	subs	r0, #185
	bl 0x02009464
	movs	r0, #159
	lsls	r0, r0, #1
	bl 0x0200945c
	b.n	.L_02000696
.L_0200061e:
	ldr	r3, [pc, #232]
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldr	r2, [r5, #4]
	ldr	r3, [r3, #0]
	cmp	r2, r3
	beq.n	.L_02000696
	ldr	r6, [pc, #220]
	movs	r1, #9
	adds	r0, r6, #0
	bl 0x02009394
	adds	r0, r6, #1
	movs	r1, #13
	bl 0x02009394
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x02009424
	cmp	r0, #0
	beq.n	.L_02000656
	bl 0x020093c4
	b.n	.L_020004a8
.L_02000656:
	bl 0x020093c4
	bl 0x02008384
	cmp	r0, #0
	beq.n	.L_0200066c
	adds	r0, r6, #2
.L_02000664:
	movs	r1, #9
	bl 0x02009394
	b.n	.L_020004a8
.L_0200066c:
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r0, #224
	ldrh	r2, [r3, #0]
	lsls	r0, r0, #1
	adds	r3, r5, r0
	movs	r1, #227
	strh	r2, [r3, #0]
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	adds	r0, #2
	adds	r2, r5, r0
	strh	r3, [r2, #0]
	subs	r0, #185
	bl 0x02009464
	ldr	r0, [pc, #124]
	bl 0x0200945c
.L_02000696:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x02009464
	b.n	.L_02000c96
.L_020006a0:
	cmp	r6, #2
	bne.n	.L_020006aa
	bl 0x020093fc
	b.n	.L_020004a8
.L_020006aa:
	cmp	r6, #3
	bne.n	.L_02000714
	bl 0x02009404
	b.n	.L_020004a8
	.4byte 0xfffffddc
	.4byte 0x020081fd
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000007
	.4byte 0x0000020f
	.4byte 0x00000002
	.4byte 0x03001ca0
	.4byte 0x03001f54
	.4byte 0x0000022a
	.4byte 0x03001d08
	.4byte 0x0000000a
	.4byte 0x02008155
	.4byte 0x00000205
	.4byte 0x00000206
	.4byte 0x020096b2
	.4byte 0x020096b4
	.4byte 0x00000008
	.4byte 0x00000109
	.4byte 0x03001ae8
	.4byte 0x00000006
	.4byte 0x02001000
	.4byte 0x00000004
	.2byte 0x013f
	.2byte 0x0000
.L_02000714:
	cmp	r6, #4
	bne.n	.L_020007b2
	movs	r0, #4
	bl 0x020093f4
	movs	r1, #1
	adds	r6, r0, #0
	negs	r1, r1
	cmp	r6, r1
	bne.n	.L_0200072a
	b.n	.L_020004a8
.L_0200072a:
	ldr	r5, [pc, #312]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	str	r2, [r3, #0]
	ldr	r0, [pc, #304]
	bl 0x02009454
	cmp	r0, #0
	beq.n	.L_02000774
	bl 0x0200948c
	movs	r0, #0
	bl 0x02009474
	movs	r0, #1
	bl 0x02009474
	movs	r0, #2
	bl 0x02009474
	movs	r0, #3
	bl 0x02009474
	movs	r0, #0
	bl 0x0200946c
	movs	r0, #1
	bl 0x0200946c
	movs	r0, #2
	bl 0x0200946c
	movs	r0, #3
	bl 0x0200946c
.L_02000774:
	ldr	r0, [pc, #244]
	ldr	r1, [pc, #248]
	adds	r3, r5, r0
	ldrb	r0, [r3, #0]
	adds	r3, r5, r1
	ldrb	r1, [r3, #0]
	bl 0x0200941c
	ldr	r0, [pc, #236]
	bl 0x02009464
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x02009464
	movs	r0, #191
	lsls	r0, r0, #1
	bl 0x0200945c
	ldr	r2, [pc, #220]
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r0, [pc, #216]
	movs	r1, #1
	bl 0x020094a4
	b.n	.L_02000c96
.L_020007aa:
	movs	r7, #1
	b.n	.L_020008e0
.L_020007ae:
	movs	r4, #0
	b.n	.L_0200095e
.L_020007b2:
	cmp	r6, #5
	beq.n	.L_020007b8
	b.n	.L_020004a8
.L_020007b8:
	movs	r0, #5
	bl 0x020093f4
	movs	r2, #1
	adds	r6, r0, #0
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L_020007ca
	b.n	.L_020004a8
.L_020007ca:
	ldr	r2, [pc, #152]
	ldr	r0, [pc, #156]
	ldr	r1, [pc, #160]
	adds	r3, r2, r0
	adds	r2, r2, r1
	ldrb	r0, [r3, #0]
	ldrb	r1, [r2, #0]
	bl 0x0200941c
.L_020007dc:
	movs	r0, #0
	bl 0x020093d4
	movs	r2, #1
	adds	r6, r0, #0
	negs	r2, r2
	cmp	r6, r2
	beq.n	.L_020007b8
	cmp	r6, #1
	beq.n	.L_020007f2
	b.n	.L_020009cc
.L_020007f2:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #5
	movs	r2, #18
	movs	r3, #8
	movs	r0, #6
	bl 0x02009384
	ldr	r5, [pc, #124]
	adds	r6, r0, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #4
	bl 0x020093ac
	adds	r0, r5, #1
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #16
	adds	r5, #3
	bl 0x020093ac
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #36
	bl 0x020093ac
	bl 0x0200936c
	movs	r0, #10
	bl 0x02009324
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #36]
	strh	r3, [r2, #0]
	strh	r3, [r2, #2]
	strh	r3, [r2, #4]
	strh	r3, [r2, #6]
	ldr	r2, [pc, #28]
	ldr	r3, [pc, #64]
	movs	r7, #0
	movs	r5, #3
	movs	r1, #0
.L_0200084c:
	adds	r1, #1
	strh	r2, [r3, #0]
	strh	r2, [r3, #2]
	strh	r2, [r3, #4]
	strh	r2, [r3, #6]
	adds	r3, #24
	cmp	r1, #4
	bne.n	.L_0200084c
	b.n	.L_020008ce
	.2byte 0x0000
	.4byte 0x00000030
	.4byte 0x02000240
	.4byte 0x00000952
	.4byte 0x00000205
	.4byte 0x00000206
	.4byte 0x00000109
	.4byte 0x03001ca0
	.4byte 0x000000be
	.4byte 0x00000c83
	.4byte 0x02002224
	.2byte 0x2024
	.2byte 0x0200
.L_0200088c:
	ldr	r3, [pc, #620]
	ldrh	r2, [r3, #0]
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, r5
	bne.n	.L_020008c8
	ldr	r3, [pc, #612]
	ldr	r3, [r3, #0]
	lsls	r3, r3, #26
	movs	r2, #1
	lsrs	r3, r3, #30
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #600]
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	cmp	r3, #85
	bne.n	.L_020008c8
	ldrh	r3, [r2, #2]
	cmp	r3, #86
	bne.n	.L_020008c8
	ldrh	r3, [r2, #4]
	cmp	r3, #84
	bne.n	.L_020008c8
	ldrh	r3, [r2, #6]
	cmp	r3, #83
	bne.n	.L_020008c8
	b.n	.L_020007aa
.L_020008c8:
	movs	r0, #1
	bl 0x02009324
.L_020008ce:
	ldr	r3, [pc, #568]
	ldr	r2, [r3, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0200088c
	movs	r0, #113
	bl 0x020094cc
.L_020008e0:
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200938c
	cmp	r7, #0
	bne.n	.L_020008ee
	b.n	.L_020007dc
.L_020008ee:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #10
	movs	r2, #20
	movs	r3, #4
	movs	r0, #5
	bl 0x02009384
	adds	r6, r0, #0
	movs	r2, #0
	movs	r3, #4
	adds	r1, r6, #0
	ldr	r0, [pc, #516]
	bl 0x020093ac
	movs	r0, #10
	bl 0x02009324
	ldr	r1, [pc, #508]
	ldr	r0, [pc, #508]
	bl 0x0200937c
	movs	r0, #10
	bl 0x02009324
	movs	r5, #0
	movs	r1, #3
	movs	r4, #1
	movs	r7, #0
	b.n	.L_0200093a
.L_0200092a:
	movs	r0, #1
	str	r1, [sp, #8]
	str	r4, [sp, #4]
	bl 0x02009324
	ldr	r4, [sp, #4]
	ldr	r1, [sp, #8]
	adds	r7, #1
.L_0200093a:
	ldr	r3, [pc, #476]
	cmp	r7, r3
	bgt.n	.L_0200095e
	ldr	r3, [pc, #440]
	ldrh	r2, [r3, #0]
	adds	r3, r1, #0
	ands	r3, r2
	adds	r5, #1
	cmp	r3, r1
	bne.n	.L_02000950
	movs	r5, #0
.L_02000950:
	cmp	r5, #10
	bne.n	.L_02000956
	b.n	.L_020007ae
.L_02000956:
	ldr	r3, [pc, #452]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0200092a
.L_0200095e:
	cmp	r4, #0
	bne.n	.L_02000986
	adds	r0, r6, #0
	bl 0x0200939c
	ldr	r0, [pc, #436]
	adds	r1, r6, #0
.L_0200096c:
	movs	r2, #0
	movs	r3, #4
	bl 0x020093ac
	ldr	r7, [pc, #400]
	movs	r5, #1
.L_02000978:
	movs	r0, #1
	bl 0x02009324
	ldr	r3, [r7, #0]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_02000978
.L_02000986:
	movs	r0, #10
	bl 0x02009324
	bl 0x02009374
	movs	r0, #10
	bl 0x02009324
	adds	r0, r6, #0
	bl 0x0200939c
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200938c
	b.n	.L_020004a8
.L_020009a6:
	movs	r0, #113
	bl 0x020094cc
	mov	r0, sl
	bl 0x0200939c
	mov	r0, sl
	movs	r1, #2
	bl 0x0200938c
	movs	r1, #2
	ldr	r0, [sp, #16]
	bl 0x0200938c
	movs	r1, #2
	ldr	r0, [sp, #12]
	bl 0x0200938c
	b.n	.L_020009d2
.L_020009cc:
	cmp	r6, #0
	beq.n	.L_020009d2
	b.n	.L_02000c8a
.L_020009d2:
	movs	r0, #1
	bl 0x020093d4
	adds	r6, r0, #0
	movs	r0, #1
	negs	r0, r0
	cmp	r6, r0
.L_020009e0:
	bne.n	.L_020009e4
	b.n	.L_020007dc
.L_020009e4:
	add	r5, sp, #348
	adds	r2, r5, #0
	adds	r1, r6, #0
	movs	r0, #0
	bl 0x02008de4
	adds	r1, r5, #0
	mov	r9, r0
	bl 0x020092c8
	mov	r3, r9
	lsls	r0, r0, #16
	adds	r3, #1
	asrs	r2, r0, #16
	mov	r1, r9
	lsrs	r0, r0, #24
	strb	r0, [r5, r1]
	strb	r2, [r5, r3]
	movs	r2, #2
	add	r9, r2
	mov	r1, r9
	add	r2, sp, #28
	adds	r0, r5, #0
	bl 0x020091e4
	mov	r9, r0
	bl 0x02009354
	movs	r6, #2
	movs	r2, #20
	movs	r1, #4
	movs	r3, #12
	movs	r0, #5
	str	r6, [sp, #0]
	bl 0x02009384
	movs	r3, #0
	movs	r1, #50
	mov	sl, r0
	mov	r0, r9
	mov	fp, r3
	bl 0x02009314
	adds	r0, #1
	mov	r8, r0
	movs	r1, #0
	movs	r2, #10
	movs	r3, #4
	movs	r0, #10
	str	r6, [sp, #0]
	bl 0x02009384
	ldr	r5, [pc, #212]
	str	r0, [sp, #12]
	ldr	r1, [sp, #12]
	adds	r0, r5, #0
	movs	r2, #6
	movs	r3, #4
	bl 0x020093ac
	mov	r0, r8
	movs	r7, #1
	cmp	r0, #1
	bne.n	.L_02000a82
	movs	r1, #16
	movs	r2, #20
	movs	r3, #3
	movs	r0, #5
	str	r6, [sp, #0]
	bl 0x02009384
	str	r0, [sp, #16]
	ldr	r1, [sp, #16]
	subs	r0, r5, #2
	movs	r2, #80
	movs	r3, #0
	bl 0x020093b4
	b.n	.L_02000a9e
.L_02000a82:
	movs	r1, #16
	movs	r2, #28
	movs	r3, #3
	movs	r0, #1
	str	r6, [sp, #0]
	bl 0x02009384
	str	r0, [sp, #16]
	ldr	r1, [sp, #16]
	subs	r0, r5, #1
	movs	r2, #0
	movs	r3, #0
	bl 0x020093b4
.L_02000a9e:
	ldr	r0, [pc, #136]
	bl 0x0200942c
	mov	r0, sl
	bl 0x0200939c
.L_02000aaa:
	ldr	r0, [pc, #128]
	bl 0x02009434
	ldr	r1, [pc, #84]
	ldr	r2, [r1, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02000abe
	b.n	.L_020009a6
.L_02000abe:
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02000ada
	add	fp, r3
	movs	r7, #1
	cmp	fp, r8
	bne.n	.L_02000ad2
	b.n	.L_02000474
.L_02000ad2:
	movs	r0, #111
	bl 0x020094cc
	b.n	.L_02000b58
.L_02000ada:
	ldr	r3, [pc, #44]
	ldr	r2, [r3, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02000b30
	mov	r0, r8
	cmp	r0, #1
	ble.n	.L_02000b30
	movs	r0, #111
	bl 0x020094cc
	mov	r0, fp
	add	r0, r8
	subs	r0, #1
	b.n	.L_02000b4e
	.2byte 0x0000
	.4byte 0x03001f64
	.4byte 0x04000128
	.4byte 0x02002024
	.4byte 0x03001c94
	.4byte 0x00000c85
	.4byte 0x00001004
	.4byte 0x02000000
	.4byte 0x000927bf
	.4byte 0x02002080
	.4byte 0x00000c87
	.4byte 0x00000c82
	.4byte 0x06006000
	.2byte 0x2500
	.2byte 0x0600
.L_02000b30:
	ldr	r1, [pc, #420]
	ldr	r2, [r1, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02000b58
	mov	r2, r8
	cmp	r2, #1
	ble.n	.L_02000b58
	movs	r0, #111
	bl 0x020094cc
	mov	r0, fp
	add	r0, r8
	adds	r0, #1
.L_02000b4e:
	mov	r1, r8
	bl 0x0200931c
	movs	r7, #1
	mov	fp, r0
.L_02000b58:
	cmp	r7, #1
	beq.n	.L_02000b5e
	b.n	.L_02000c82
.L_02000b5e:
	mov	r0, sl
	bl 0x0200939c
	movs	r7, #0
	movs	r5, #2
.L_02000b68:
	adds	r2, r5, #0
	mov	r0, sl
	movs	r1, #0
	movs	r3, #18
	adds	r7, #1
	str	r5, [sp, #0]
	bl 0x020093a4
	adds	r5, #2
	cmp	r7, #4
	bne.n	.L_02000b68
	mov	r3, r8
	cmp	r3, #1
	ble.n	.L_02000bf2
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_02000bb4
	negs	r3, r3
	adds	r5, r3, #0
	movs	r6, #0
	adds	r5, #18
.L_02000b92:
	ldr	r0, [pc, #328]
	adds	r1, r7, r0
	cmp	r7, fp
	bne.n	.L_02000b9e
	ldr	r2, [pc, #324]
	adds	r1, r7, r2
.L_02000b9e:
	movs	r3, #1
	adds	r2, r5, #0
	mov	r0, sl
	negs	r3, r3
	adds	r7, #1
	str	r6, [sp, #0]
	adds	r5, #1
	bl 0x020093e4
	cmp	r7, r8
	bne.n	.L_02000b92
.L_02000bb4:
	mov	r3, r8
	movs	r2, #17
	subs	r2, r2, r3
	movs	r3, #1
	movs	r5, #0
	mov	r0, sl
	ldr	r1, [pc, #288]
	negs	r3, r3
	str	r5, [sp, #0]
	bl 0x020093e4
	movs	r3, #1
	mov	r0, sl
	ldr	r1, [pc, #280]
	movs	r2, #18
	negs	r3, r3
	str	r5, [sp, #0]
	bl 0x020093e4
	ldr	r3, [pc, #272]
	mov	r2, sl
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #268]
	ldrh	r3, [r2, #14]
	adds	r1, r1, r0
	lsrs	r3, r3, #2
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r1, #0]
	orrs	r2, r3
	strb	r2, [r1, #0]
.L_02000bf2:
	movs	r3, #50
	mov	r0, fp
	muls	r0, r3
	adds	r6, r0, #0
	adds	r6, #50
	cmp	r6, r9
	ble.n	.L_02000c02
	mov	r6, r9
.L_02000c02:
	adds	r7, r0, #0
	movs	r4, #0
	cmp	r7, r6
	beq.n	.L_02000c80
.L_02000c0a:
	add	r3, sp, #28
	ldrb	r3, [r3, r7]
	movs	r0, #63
	ands	r0, r3
	add	r1, sp, #24
	str	r4, [sp, #4]
	bl 0x020082f4
	adds	r0, r7, #0
	movs	r1, #10
	bl 0x0200931c
	ldr	r4, [sp, #4]
	cmp	r0, #4
	ble.n	.L_02000c4a
	adds	r0, r4, #0
	movs	r1, #10
	bl 0x0200931c
	ldr	r4, [sp, #4]
	adds	r5, r0, #0
	movs	r1, #10
	adds	r0, r4, #0
	bl 0x02009314
	lsls	r2, r5, #1
	adds	r3, r0, #0
	adds	r2, r2, r5
	lsls	r2, r2, #2
	lsls	r3, r3, #4
	adds	r2, #18
	b.n	.L_02000c6c
.L_02000c4a:
	adds	r0, r4, #0
	movs	r1, #10
	str	r4, [sp, #4]
	bl 0x0200931c
	ldr	r4, [sp, #4]
	adds	r5, r0, #0
	movs	r1, #10
	adds	r0, r4, #0
	bl 0x02009314
	lsls	r2, r5, #1
	adds	r3, r0, #0
	adds	r2, r2, r5
	lsls	r2, r2, #2
	lsls	r3, r3, #4
	adds	r2, #8
.L_02000c6c:
	adds	r3, #2
	add	r0, sp, #24
	mov	r1, sl
	bl 0x020093bc
	ldr	r4, [sp, #4]
	adds	r7, #1
	adds	r4, #1
	cmp	r7, r6
	bne.n	.L_02000c0a
.L_02000c80:
	movs	r7, #0
.L_02000c82:
	movs	r0, #1
	bl 0x02009324
	b.n	.L_02000aaa
.L_02000c8a:
	movs	r0, #150
	lsls	r0, r0, #1
	bl 0x02009324
	bl 0x020084a8
.L_02000c96:
	ldr	r3, [pc, #92]
	movs	r0, #184
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #88]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	strh	r2, [r3, #0]
	movs	r0, #30
	bl 0x02009494
	movs	r0, #17
	bl 0x020094cc
	bl 0x020094b4
	bl 0x020094bc
	movs	r0, #60
	bl 0x02009494
	movs	r0, #0
	movs	r3, #137
	lsls	r3, r3, #2
	add	sp, r3
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x03001c94
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x0000f128
	.4byte 0x0000f129
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0x03001ebc
	.2byte 0x03e7
	.2byte 0x0000
	.section .text.x02008de4,"ax",%progbits
	.p2align 2
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #64
	movs	r0, #11
	str	r1, [sp, #28]
	mov	fp, r2
	str	r0, [sp, #24]
	cmp	r1, #1
	beq.n	.L_02000e18
	cmp	r1, #1
	bgt.n	.L_02000e0a
	cmp	r1, #0
	beq.n	.L_02000e12
	b.n	.L_02000e22
.L_02000e0a:
	ldr	r1, [sp, #28]
	cmp	r1, #2
	beq.n	.L_02000e1e
	b.n	.L_02000e22
.L_02000e12:
	movs	r2, #173
	str	r2, [sp, #24]
	b.n	.L_02000e22
.L_02000e18:
	movs	r3, #39
	str	r3, [sp, #24]
	b.n	.L_02000e22
.L_02000e1e:
	movs	r4, #9
	str	r4, [sp, #24]
.L_02000e22:
	ldr	r0, [sp, #24]
	movs	r6, #0
	mov	r9, r6
	cmp	r0, #0
	beq.n	.L_02000e3e
	movs	r2, #0
	mov	r3, fp
.L_02000e30:
	strb	r2, [r3, #0]
	movs	r1, #1
	ldr	r4, [sp, #24]
	add	r9, r1
	adds	r3, #1
	cmp	r9, r4
	bne.n	.L_02000e30
.L_02000e3e:
	mov	r0, sp
	movs	r6, #0
	adds	r0, #32
	str	r6, [sp, #20]
	str	r6, [sp, #16]
	str	r6, [sp, #12]
	str	r6, [sp, #8]
	str	r0, [sp, #4]
	mov	r9, r6
	movs	r2, #0
	adds	r3, r0, #0
.L_02000e54:
	movs	r1, #1
	add	r9, r1
	mov	r4, r9
	stmia	r3!, {r2}
	cmp	r4, #8
	bne.n	.L_02000e54
	movs	r6, #0
	ldr	r5, [pc, #452]
	mov	r9, r6
	movs	r6, #1
.L_02000e68:
	ldrh	r0, [r5, #0]
	adds	r5, #2
	bl 0x02009454
	cmp	r0, #0
	beq.n	.L_02000e84
	ldr	r1, [sp, #8]
	adds	r3, r6, #0
	mov	r0, r9
	lsls	r3, r0
	orrs	r1, r3
	lsls	r3, r1, #24
	lsrs	r3, r3, #24
	str	r3, [sp, #8]
.L_02000e84:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #6
	bne.n	.L_02000e68
	movs	r4, #0
	ldr	r7, [sp, #4]
	mov	r9, r4
.L_02000e94:
	ldr	r2, [pc, #404]
	mov	r6, r9
	lsls	r3, r6, #2
	ldr	r0, [r2, r3]
	bl 0x0200943c
	mov	ip, r0
	mov	r1, ip
	adds	r1, #16
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	ldr	r0, [pc, #388]
	ldrh	r2, [r1, #0]
	cmp	r3, r0
	ble.n	.L_02000eb6
	strh	r0, [r1, #0]
	adds	r2, r0, #0
.L_02000eb6:
	lsls	r3, r2, #16
	cmp	r3, #0
	bge.n	.L_02000ec0
	movs	r3, #0
	strh	r3, [r1, #0]
.L_02000ec0:
	movs	r4, #2
	ldrsh	r3, [r1, r4]
	ldrh	r2, [r1, #2]
	cmp	r3, r0
	ble.n	.L_02000ece
	strh	r0, [r1, #2]
	adds	r2, r0, #0
.L_02000ece:
	lsls	r3, r2, #16
	cmp	r3, #0
	bge.n	.L_02000ed8
	movs	r3, #0
	strh	r3, [r1, #2]
.L_02000ed8:
	ldrh	r3, [r1, #8]
	ldr	r2, [pc, #344]
	cmp	r3, r2
	bls.n	.L_02000ee2
	strh	r2, [r1, #8]
.L_02000ee2:
	ldrh	r3, [r1, #10]
	cmp	r3, r2
	bls.n	.L_02000eea
	strh	r2, [r1, #10]
.L_02000eea:
	ldrh	r3, [r1, #12]
	cmp	r3, r2
	bls.n	.L_02000ef2
	strh	r2, [r1, #12]
.L_02000ef2:
	ldrb	r3, [r1, #14]
	cmp	r3, #99
	bls.n	.L_02000efc
	movs	r3, #99
	strb	r3, [r1, #14]
.L_02000efc:
	movs	r3, #0
	ldrsh	r2, [r1, r3]
	movs	r4, #2
	ldrsh	r3, [r1, r4]
	lsls	r2, r2, #21
	lsls	r3, r3, #10
	orrs	r2, r3
	ldrh	r3, [r1, #8]
.L_02000f0c:
	orrs	r2, r3
	str	r2, [r7, #0]
	ldrh	r3, [r1, #12]
	ldrh	r2, [r1, #10]
	lsls	r3, r3, #12
	lsls	r2, r2, #22
	orrs	r2, r3
	ldrb	r3, [r1, #14]
	lsls	r3, r3, #4
	orrs	r2, r3
	mov	r6, r9
	str	r2, [r7, #4]
	lsls	r0, r6, #3
	mov	r6, ip
	ldrb	r2, [r6, #15]
	adds	r3, r2, #0
	cmp	r3, #99
	bls.n	.L_02000f36
	movs	r3, #99
	strb	r3, [r6, #15]
	movs	r2, #99
.L_02000f36:
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_02000f42
	movs	r3, #1
	mov	r1, ip
	strb	r3, [r1, #15]
.L_02000f42:
	mov	r2, ip
	ldrb	r3, [r2, #15]
	mov	r4, r9
	subs	r2, r0, r4
	ldr	r6, [sp, #20]
	lsls	r3, r2
	orrs	r6, r3
	mov	r2, ip
	str	r6, [sp, #20]
	movs	r5, #0
	adds	r2, #248
	movs	r1, #0
.L_02000f5a:
	ldmia	r2!, {r3}
	ldr	r0, [sp, #16]
	lsls	r3, r1
	adds	r0, r0, r3
	adds	r5, #1
	str	r0, [sp, #16]
	adds	r1, #7
	cmp	r5, #4
	bne.n	.L_02000f5a
	ldr	r1, [pc, #200]
	ldr	r2, [pc, #204]
	movs	r3, #1
	mov	r0, ip
	movs	r5, #0
	mov	r8, r1
	mov	lr, r2
	mov	sl, r3
	adds	r0, #216
.L_02000f7e:
	ldrh	r3, [r0, #0]
	mov	r1, r8
	movs	r4, #0
	ands	r1, r3
	mov	r2, lr
.L_02000f88:
	ldrh	r3, [r2, #0]
	adds	r2, #2
	cmp	r1, r3
	bne.n	.L_02000f9e
	ldr	r6, [sp, #12]
	mov	r3, sl
	lsls	r3, r4
	orrs	r6, r3
	lsls	r3, r6, #24
	lsrs	r3, r3, #24
	str	r3, [sp, #12]
.L_02000f9e:
	adds	r4, #1
	cmp	r4, #8
	bne.n	.L_02000f88
	adds	r5, #1
	adds	r0, #2
	cmp	r5, #15
	bne.n	.L_02000f7e
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r7, #8
	cmp	r1, #4
	beq.n	.L_02000fba
	b.n	.L_02000e94
.L_02000fba:
	ldr	r2, [sp, #28]
	cmp	r2, #0
	beq.n	.L_02000fc2
	b.n	.L_02001108
.L_02000fc2:
	movs	r3, #39
	movs	r6, #0
	mov	sl, r3
	mov	r9, r6
.L_02000fca:
	mov	r4, r9
	ldr	r3, [pc, #92]
	lsls	r2, r4, #2
	ldr	r0, [r3, r2]
	bl 0x0200943c
	mov	r5, sl
	adds	r4, r0, #0
	movs	r0, #216
	movs	r7, #0
	mov	r8, r0
	add	r5, fp
.L_02000fe2:
	mov	r1, r8
	ldrh	r0, [r1, r4]
	str	r4, [sp, #0]
	bl 0x02009444
	ldr	r4, [sp, #0]
	mov	r2, r8
	ldrh	r1, [r2, r4]
	ldr	r3, [pc, #48]
	ands	r1, r3
	adds	r0, r6, #1
	ldrb	r3, [r5, #0]
	adds	r2, r1, #0
	asrs	r2, r0
	adds	r3, r3, r2
	strb	r3, [r5, #0]
	movs	r3, #7
	subs	r3, r3, r6
	lsls	r1, r3
	ldrb	r3, [r5, #1]
	adds	r3, r3, r1
	strb	r3, [r5, #1]
	adds	r6, r0, #0
	movs	r3, #1
	adds	r5, #1
	add	sl, r3
	cmp	r6, #7
	bne.n	.L_02001040
	movs	r6, #0
	adds	r5, #1
	add	sl, r3
	b.n	.L_02001040
	.2byte 0x0000
	.4byte 0x000001ff
	.4byte 0x020096d0
	.4byte 0x020096c0
	.4byte 0x000007cf
	.4byte 0x000003e7
	.4byte 0x000001ff
	.2byte 0x96dc
	.2byte 0x0200
.L_02001040:
	movs	r0, #2
	adds	r7, #1
	add	r8, r0
	cmp	r7, #15
	bne.n	.L_02000fe2
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #4
	bne.n	.L_02000fca
	movs	r3, #107
	movs	r6, #1
	movs	r4, #0
	mov	sl, r3
	negs	r6, r6
	mov	r9, r4
.L_02001060:
	ldr	r3, [pc, #368]
	mov	r0, r9
	lsls	r2, r0, #2
	ldr	r0, [r3, r2]
	bl 0x0200943c
	ldr	r2, [pc, #360]
	mov	r8, r0
	movs	r1, #0
	mov	r0, sl
	mov	lr, r1
	mov	ip, r2
	add	r0, fp
.L_0200107a:
	mov	r3, ip
	mov	r1, r8
	ldrh	r4, [r3, #0]
	movs	r5, #0
	movs	r7, #0
	adds	r1, #216
.L_02001086:
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #336]
	ands	r3, r2
	adds	r1, #2
	cmp	r3, r4
	bne.n	.L_0200109a
	movs	r3, #248
	lsls	r3, r3, #8
	ands	r3, r2
	lsrs	r5, r3, #11
.L_0200109a:
	adds	r7, #1
	cmp	r7, #15
	bne.n	.L_02001086
	lsls	r2, r5, #16
	cmp	r6, #0
	bge.n	.L_020010be
	lsrs	r1, r2, #16
	negs	r3, r6
	adds	r2, r1, #0
	asrs	r2, r3
	ldrb	r3, [r0, #0]
	movs	r4, #1
	adds	r3, r3, r2
	strb	r3, [r0, #0]
	add	sl, r4
	adds	r0, #1
	adds	r6, #8
	b.n	.L_020010c0
.L_020010be:
	lsrs	r1, r2, #16
.L_020010c0:
	ldrb	r3, [r0, #0]
	lsls	r1, r6
	adds	r3, r3, r1
	movs	r1, #5
	subs	r6, #5
	negs	r1, r1
	strb	r3, [r0, #0]
	cmp	r6, r1
	bne.n	.L_020010da
	movs	r2, #1
	adds	r0, #1
	add	sl, r2
	movs	r6, #3
.L_020010da:
	movs	r4, #1
	add	lr, r4
	movs	r3, #2
	mov	r1, lr
	add	ip, r3
	cmp	r1, #23
	bne.n	.L_0200107a
	add	r9, r4
	mov	r2, r9
	cmp	r2, #4
	bne.n	.L_02001060
	ldr	r2, [pc, #236]
	mov	r1, fp
	ldrh	r3, [r2, #18]
	adds	r1, #165
	strb	r3, [r1, #0]
	ldr	r3, [r2, #16]
	adds	r1, #1
	lsrs	r3, r3, #8
	strb	r3, [r1, #0]
	ldr	r3, [r2, #16]
	adds	r1, #1
	strb	r3, [r1, #0]
.L_02001108:
	ldr	r3, [sp, #28]
	cmp	r3, #2
	beq.n	.L_0200117c
	ldr	r4, [sp, #28]
	negs	r3, r3
	orrs	r3, r4
	lsrs	r3, r3, #31
	adds	r3, #8
	movs	r6, #0
	mov	r1, fp
	ldr	r4, [sp, #4]
	mov	r9, r6
	adds	r0, r3, r1
.L_02001122:
	ldr	r2, [r4, #0]
	lsrs	r3, r2, #24
	strb	r3, [r0, #0]
	lsrs	r3, r2, #16
	strb	r3, [r0, #1]
	lsrs	r3, r2, #8
	strb	r3, [r0, #2]
.L_02001130:
	strb	r2, [r0, #3]
	ldr	r1, [r4, #4]
	lsrs	r3, r1, #24
	strb	r3, [r0, #4]
	lsrs	r3, r1, #16
	strb	r3, [r0, #5]
.L_0200113c:
	lsrs	r3, r1, #8
	strb	r1, [r0, #7]
	strb	r3, [r0, #6]
	ldr	r2, [r4, #8]
	lsrs	r3, r2, #28
	orrs	r1, r3
	lsrs	r3, r2, #20
	strb	r3, [r0, #8]
	lsrs	r3, r2, #12
	strb	r3, [r0, #9]
	lsrs	r3, r2, #4
	lsls	r2, r2, #4
	strb	r2, [r0, #11]
	strb	r3, [r0, #10]
	strb	r1, [r0, #7]
	ldr	r1, [r4, #12]
	lsrs	r3, r1, #28
	orrs	r2, r3
	strb	r2, [r0, #11]
	lsrs	r3, r1, #20
	movs	r2, #1
	strb	r3, [r0, #12]
	add	r9, r2
	lsrs	r3, r1, #12
	strb	r3, [r0, #13]
	lsrs	r1, r1, #4
	mov	r3, r9
	strb	r1, [r0, #14]
	adds	r4, #16
	adds	r0, #15
	cmp	r3, #2
	bne.n	.L_02001122
.L_0200117c:
	add	r4, sp, #20
	ldrb	r4, [r4, #0]
	mov	r6, fp
	strb	r4, [r6, #0]
	ldr	r6, [sp, #20]
	mov	r0, fp
	lsrs	r3, r6, #8
	strb	r3, [r0, #1]
	lsrs	r3, r6, #16
	strb	r3, [r0, #2]
	lsrs	r2, r6, #20
	movs	r3, #240
	ands	r2, r3
	ldr	r3, [sp, #16]
	movs	r1, #15
	ands	r3, r1
	orrs	r2, r3
	strb	r2, [r0, #3]
	ldr	r1, [sp, #16]
	lsrs	r3, r1, #4
	strb	r3, [r0, #4]
	lsrs	r3, r1, #12
	strb	r3, [r0, #5]
	lsrs	r3, r1, #20
	strb	r3, [r0, #6]
	add	r2, sp, #8
	ldrb	r2, [r2, #0]
	strb	r2, [r0, #7]
	ldr	r3, [sp, #28]
	cmp	r3, #0
	beq.n	.L_020011c0
	add	r4, sp, #12
	ldrb	r4, [r4, #0]
	strb	r4, [r0, #8]
.L_020011c0:
	ldr	r0, [sp, #24]
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x020096c0
	.4byte 0x020096ec
	.4byte 0x000001ff
	.2byte 0x0240
	.2byte 0x0200
	.section .rodata,"a",%progbits
	.global Clear_CodeSequence
Clear_CodeSequence:
	.2byte 0x0004, 0x0004, 0x0004, 0x0000
	.global Clear_ExtraCodeSequence
Clear_ExtraCodeSequence:
	.2byte 0x0040, 0x0080, 0x0040, 0x0080, 0x0020, 0x0010, 0x0020, 0x0010
	.2byte 0x0040, 0x0010, 0x0080, 0x0020, 0x0040, 0x0004, 0x0000, 0x0000
	.4byte 0x8fc22100
	.4byte 0x250d09f0
	.4byte 0x3f08b7c2
	.4byte 0xf0c97c32
	.4byte 0x0ca7c327
	.4byte 0x907c32bf
	.4byte 0x5fc3d5f8
	.4byte 0xfc3d9f0f
	.4byte 0xb9ddf0f6
	.4byte 0x1f0c3fc2
	.4byte 0xf0c4fc31
	.4byte 0xd45fc315
	.4byte 0xfc7c32f5
	.4byte 0x78652df0
	.4byte 0x6d4b4f21
	.4byte 0x0ccfc331
	.4byte 0xf7ef535f
	.4byte 0xc3e304f0
	.4byte 0x5e7f0f97
	.4byte 0x6df51936
	.4byte 0x0751d1d4
	.4byte 0xc79c1b9b
	.4byte 0xc339f0cd
	.4byte 0x33df0cef
	.4byte 0xe9bd4ffc
	.4byte 0xd1d7ac75
	.4byte 0x863e025e
	.4byte 0x7fa5ab0f
	.4byte 0x1a7efff8
	.4byte 0x87821ebc
	.4byte 0x78be1e1f
	.4byte 0x93e1e3f8
	.4byte 0xb97bc1cb
	.4byte 0x7ebe1f9a
	.4byte 0x031efbf8
	.4byte 0x9f1e98b6
	.4byte 0xe1e59f81
	.4byte 0x1e7f879b
	.4byte 0xe9f87a3e
	.4byte 0x68ebf37a
	.4byte 0x0fbf63a8
	.4byte 0x9f9f83c1
	.4byte 0x7fc1e6f8
	.4byte 0xe60cc1f3
	.4byte 0x65f0d8c1
	.4byte 0x9f0d9fc3
	.4byte 0xc95afc36
	.4byte 0xe5fc3950
	.4byte 0x6fc399f0
	.4byte 0x6a59df0e
	.4byte 0x351f0d3c
	.4byte 0x55f0d4fc
	.4byte 0x09f55fc3
	.4byte 0xb41e6208
	.4byte 0x7c36ff0d
	.4byte 0xc373f0dc
	.4byte 0x39fbd5d7
	.4byte 0xa3f0e87c
	.4byte 0x7f0e97c3
	.4byte 0xd564025a
	.4byte 0x574755b7
	.4byte 0x676f41e7
	.4byte 0x37707982
	.4byte 0x7bf0de7c
	.4byte 0xff0df7c3
	.4byte 0x1d6a6f57
	.4byte 0xd6b475ab
	.4byte 0xe027d5fe
	.4byte 0x1da27e1f
	.4byte 0x083c79ce
	.4byte 0xf8707e1c
	.4byte 0x870fe1c2
	.4byte 0x5e072c4f
	.4byte 0x7e1d8f87
	.4byte 0xe1daf876
	.4byte 0xd410fb6f
	.4byte 0xc1a818f2
	.4byte 0xbe1c583c
	.4byte 0xe1c7f871
	.4byte 0xac9f8723
	.4byte 0xa68eb72f
	.4byte 0x14fb763a
	.4byte 0x90ed8394
	.4byte 0x270fc791
	.4byte 0x09f0a7c2
	.4byte 0xfbc22b0d
	.4byte 0x00000000
	.global Clear_ScriptTable
Clear_ScriptTable:
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x40000000
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Clear_MessageTable
Clear_MessageTable:
	.4byte 0x000001ff
	.global Clear_ActorTable
Clear_ActorTable:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Clear_EffectTable
Clear_EffectTable:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Clear_BlendFrame
Clear_BlendFrame:
	.2byte 0x0000
	.global Clear_CodeUnlocked
Clear_CodeUnlocked:
	.2byte 0x0000
	.global Clear_ExtraCodeUnlocked
Clear_ExtraCodeUnlocked:
	.2byte 0x0000
	.global Clear_CodeProgress
Clear_CodeProgress:
	.2byte 0x0000
	.global Clear_ExtraCodeProgress
Clear_ExtraCodeProgress:
	.2byte 0x0000
	.global Clear_CodeHeld
Clear_CodeHeld:
	.2byte 0x0000
	.global Clear_ExtraCodeHeld
Clear_ExtraCodeHeld:
	.2byte 0x0000
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x09510941
	.4byte 0x08d108b3
	.4byte 0x0868081e
	.4byte 0x00c900c8
	.4byte 0x00cb00ca
	.4byte 0x00cd00cc
	.4byte 0x00cf00ce
	.4byte 0x00b500b4
	.4byte 0x00b700b6
	.4byte 0x00bb00ba
	.4byte 0x00bd00bc
	.4byte 0x00c000bf
	.4byte 0x00c200c1
	.4byte 0x00c400c3
	.4byte 0x00e300e2
	.4byte 0x00e500e4
	.4byte 0x00ee00ec
	.4byte 0x00f000ef
	.2byte 0x00f1
