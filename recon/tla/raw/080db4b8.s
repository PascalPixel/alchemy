.syntax unified
	.thumb
	.global Func_080db4b8
	.thumb_func
Func_080db4b8:
	push	{r5, r6, r7, lr}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r6, [r3, #0]
	ldr	r3, [r2, #108]
	movs	r1, #30
	ldrsh	r2, [r6, r1]
	movs	r1, #26
	ldrsh	r5, [r6, r1]
	cmp	r2, #1
	bne.n	.L_080db4d8
	bl	BattleFx_RunItemBreakSequence
	b.n	.L_080db668
.L_080db4d8:
	cmp	r2, #7
	bne.n	.L_080db4e2
	bl	0x080ddea8
	b.n	.L_080db668
.L_080db4e2:
	cmp	r2, #11
	bne.n	.L_080db4ec
	bl	0x080df6f4
	b.n	.L_080db668
.L_080db4ec:
	cmp	r2, #4
	bne.n	.L_080db4f6
	bl	0x080de21c
	b.n	.L_080db668
.L_080db4f6:
	cmp	r2, #5
	bne.n	.L_080db500
	bl	0x080dede8
	b.n	.L_080db668
.L_080db500:
	cmp	r2, #14
	bne.n	.L_080db50a
	bl	0x080dfd00
	b.n	.L_080db668
.L_080db50a:
	cmp	r2, #6
	bne.n	.L_080db514
	bl	0x080de688
	b.n	.L_080db668
.L_080db514:
	cmp	r2, #3
	bne.n	.L_080db51e
	bl	0x080de9f8
	b.n	.L_080db668
.L_080db51e:
	cmp	r2, #12
	bne.n	.L_080db528
	bl	0x080ddb3c
	b.n	.L_080db668
.L_080db528:
	cmp	r2, #13
	bne.n	.L_080db532
	bl	0x080e03cc
	b.n	.L_080db668
.L_080db532:
	cmp	r2, #15
	bne.n	.L_080db53c
	bl	0x080e0dd4
	b.n	.L_080db668
.L_080db53c:
	cmp	r2, #16
	bne.n	.L_080db546
	bl	0x080e1214
	b.n	.L_080db668
.L_080db546:
	cmp	r2, #10
	bne.n	.L_080db550
	bl	Func_080decb8
	b.n	.L_080db668
.L_080db550:
	cmp	r2, #8
	bne.n	.L_080db55a
	bl	0x080dd820
	b.n	.L_080db668
.L_080db55a:
	cmp	r2, #17
	bne.n	.L_080db564
	bl	0x080e09c0
	b.n	.L_080db668
.L_080db564:
	cmp	r2, #29
	bne.n	.L_080db56e
	bl	Func_080e1f2c
	b.n	.L_080db668
.L_080db56e:
	cmp	r2, #25
	bne.n	.L_080db578
	bl	0x080e3074
	b.n	.L_080db668
.L_080db578:
	cmp	r2, #26
	bne.n	.L_080db582
	bl	0x080e3d04
	b.n	.L_080db668
.L_080db582:
	cmp	r2, #21
	bne.n	.L_080db58c
	bl	0x080e572c
	b.n	.L_080db668
.L_080db58c:
	cmp	r2, #18
	bne.n	.L_080db596
	bl	0x080e5d5c
	b.n	.L_080db668
.L_080db596:
	cmp	r2, #20
	bne.n	.L_080db5a0
	bl	0x080e68d0
	b.n	.L_080db668
.L_080db5a0:
	cmp	r2, #19
	bne.n	.L_080db5aa
	bl	0x080e9090
	b.n	.L_080db668
.L_080db5aa:
	cmp	r2, #27
	bne.n	.L_080db5b4
	bl	0x080e97c8
	b.n	.L_080db668
.L_080db5b4:
	cmp	r2, #24
	bne.n	.L_080db5be
	bl	0x080e9af4
	b.n	.L_080db668
.L_080db5be:
	cmp	r2, #23
	bne.n	.L_080db5c8
	bl	0x080e4a34
	b.n	.L_080db668
.L_080db5c8:
	cmp	r2, #30
	bne.n	.L_080db5d2
	bl	Func_080e781c
	b.n	.L_080db668
.L_080db5d2:
	cmp	r2, #22
	bne.n	.L_080db5dc
	bl	0x080e7844
	b.n	.L_080db668
.L_080db5dc:
	cmp	r2, #28
	bne.n	.L_080db5e6
	bl	Func_080e82cc
	b.n	.L_080db668
.L_080db5e6:
	cmp	r2, #9
	bne.n	.L_080db644
	ldr	r3, [pc, #128]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #106
	adds	r7, r3, r2
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_080db60c
	bl	Func_080e035c
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r7, #0]
.L_080db60c:
	movs	r1, #9
	movs	r2, #24
	ldrsh	r0, [r6, r2]
	bl	Func_080cdbf8
	bl	0x080e03ac
	adds	r5, r0, #0
	bl	Func_080cce94
	cmp	r0, #0
	beq.n	.L_080db63e
	adds	r1, r5, #0
	movs	r3, #24
	ldrsh	r0, [r6, r3]
	bl	Func_080dc62c
	adds	r0, r5, #0
	bl	0x080e011c
	adds	r0, r5, #0
	bl	Func_080e0308
	strh	r5, [r7, #0]
	b.n	.L_080db668
.L_080db63e:
	bl	0x080e0134
	b.n	.L_080db668
.L_080db644:
	cmp	r2, #2
	bne.n	.L_080db668
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080db65e
	bl	0x080dd950
.L_080db65e:
	movs	r2, #24
	ldrsh	r0, [r6, r2]
	adds	r1, r5, #0
	bl	Func_080dc978
.L_080db668:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r6, [r3, #0]
	ldr	r0, [r2, #108]
	movs	r2, #30
	ldrsh	r1, [r6, r2]
	adds	r2, r6, #0
	movs	r3, #26
	ldrsh	r5, [r6, r3]
	adds	r2, #32
	movs	r3, #0
	strb	r3, [r2, #0]
	cmp	r1, #1
	bne.n	.L_080db69a
	adds	r0, r5, #0
	bl	Func_080dd054
	b.n	.L_080db840
.L_080db69a:
	cmp	r1, #7
	bne.n	.L_080db6a6
	adds	r0, r5, #0
	bl	0x080ddda0
	b.n	.L_080db840
.L_080db6a6:
	cmp	r1, #11
	bne.n	.L_080db6b2
	adds	r0, r5, #0
	bl	0x080df1fc
	b.n	.L_080db840
.L_080db6b2:
	cmp	r1, #4
	bne.n	.L_080db6be
	adds	r0, r5, #0
	bl	0x080de214
	b.n	.L_080db840
.L_080db6be:
	cmp	r1, #5
	bne.n	.L_080db6ca
	adds	r0, r5, #0
	bl	0x080dede0
	b.n	.L_080db840
.L_080db6ca:
	cmp	r1, #6
	bne.n	.L_080db6d6
	adds	r0, r5, #0
	bl	0x080de654
	b.n	.L_080db840
.L_080db6d6:
	cmp	r1, #12
	bne.n	.L_080db6e2
	adds	r0, r5, #0
	bl	Func_080dda30
	b.n	.L_080db840
.L_080db6e2:
	cmp	r1, #3
	bne.n	.L_080db6ee
	adds	r0, r5, #0
	bl	0x080de9ec
	b.n	.L_080db840
.L_080db6ee:
	cmp	r1, #14
	bne.n	.L_080db6fa
	adds	r0, r5, #0
	bl	0x080dfcf8
	b.n	.L_080db840
.L_080db6fa:
	cmp	r1, #13
	bne.n	.L_080db706
	adds	r0, r5, #0
	bl	Func_080e03c4
	b.n	.L_080db840
.L_080db706:
	cmp	r1, #16
	bne.n	.L_080db710
	bl	0x080e1214
	b.n	.L_080db840
.L_080db710:
	cmp	r1, #17
	bne.n	.L_080db71c
	adds	r0, r5, #0
	bl	0x080e0978
	b.n	.L_080db840
.L_080db71c:
	cmp	r1, #10
	bne.n	.L_080db726
	bl	Func_080decb8
	b.n	.L_080db840
.L_080db726:
	cmp	r1, #15
	bne.n	.L_080db730
	bl	0x080e0dd4
	b.n	.L_080db840
.L_080db730:
	cmp	r1, #8
	bne.n	.L_080db73a
	bl	0x080dd820
	b.n	.L_080db840
.L_080db73a:
	cmp	r1, #23
	bne.n	.L_080db746
	adds	r0, r5, #0
	bl	0x080e4a2c
	b.n	.L_080db840
.L_080db746:
	cmp	r1, #26
	bne.n	.L_080db752
	adds	r0, r5, #0
	bl	0x080e3cfc
	b.n	.L_080db840
.L_080db752:
	cmp	r1, #25
	bne.n	.L_080db75e
	adds	r0, r5, #0
	bl	Func_080e306c
	b.n	.L_080db840
.L_080db75e:
	cmp	r1, #20
	bne.n	.L_080db76a
	adds	r0, r5, #0
	bl	Func_080e68c8
	b.n	.L_080db840
.L_080db76a:
	cmp	r1, #19
	bne.n	.L_080db776
	adds	r0, r5, #0
	bl	0x080e8db0
	b.n	.L_080db840
.L_080db776:
	cmp	r1, #27
	bne.n	.L_080db782
	adds	r0, r5, #0
	bl	0x080e92b4
	b.n	.L_080db840
.L_080db782:
	cmp	r1, #24
	bne.n	.L_080db78e
	adds	r0, r5, #0
	bl	Func_080e9aec
	b.n	.L_080db840
.L_080db78e:
	cmp	r1, #30
	bne.n	.L_080db79a
	adds	r0, r5, #0
	bl	Func_080e7818
	b.n	.L_080db840
.L_080db79a:
	cmp	r1, #22
	bne.n	.L_080db7a6
	adds	r0, r5, #0
	bl	0x080e783c
	b.n	.L_080db840
.L_080db7a6:
	cmp	r1, #21
	bne.n	.L_080db7b2
	adds	r0, r5, #0
	bl	0x080e5724
	b.n	.L_080db840
.L_080db7b2:
	cmp	r1, #18
	bne.n	.L_080db7be
	adds	r0, r5, #0
	bl	0x080e5d54
	b.n	.L_080db840
.L_080db7be:
	cmp	r1, #28
	bne.n	.L_080db7c8
	bl	Func_080e82cc
	b.n	.L_080db840
.L_080db7c8:
	cmp	r1, #2
	bne.n	.L_080db80c
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080db7e2
	bl	0x080dd950
.L_080db7e2:
	ldr	r3, [pc, #96]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #106
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	movs	r1, #26
	ldrsh	r3, [r6, r1]
	cmp	r2, r3
	beq.n	.L_080db800
	ldr	r3, [r6, #20]
	movs	r2, #1
	adds	r3, #91
	strb	r2, [r3, #0]
.L_080db800:
	movs	r2, #24
	ldrsh	r0, [r6, r2]
	adds	r1, r5, #0
	bl	Func_080dc978
	b.n	.L_080db840
.L_080db80c:
	cmp	r1, #9
	bne.n	.L_080db840
	ldr	r3, [pc, #48]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #106
	adds	r6, r3, r1
	movs	r2, #0
	ldrsh	r0, [r6, r2]
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_080db832
	bl	Func_080e035c
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r6, #0]
.L_080db832:
	adds	r0, r5, #0
	bl	Func_080e0308
	strh	r5, [r6, #0]
	adds	r0, r5, #0
	bl	0x080e011c
.L_080db840:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r5, [r3, #0]
	movs	r1, #30
	ldrsh	r3, [r5, r1]
	cmp	r3, #2
	bne.n	.L_080db87c
	bl	Func_080dca50
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #106
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	movs	r1, #26
	ldrsh	r3, [r5, r1]
	cmp	r2, r3
	beq.n	.L_080db87c
	ldr	r3, [r5, #20]
	movs	r2, #0
	adds	r3, #91
	strb	r2, [r3, #0]
.L_080db87c:
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r2, #30
	ldrsh	r3, [r3, r2]
	subs	r3, #8
	cmp	r3, #20
	bhi.n	.L_080db916
	ldr	r2, [pc, #124]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080db8f4
	.4byte 0x080db916
	.4byte 0x080db8fa
	.4byte 0x080db916
	.4byte 0x080db916
	.4byte 0x080db916
	.4byte 0x080db916
	.4byte 0x080db916
	.4byte 0x080db900
	.4byte 0x080db916
	.4byte 0x080db916
	.4byte 0x080db916
	.4byte 0x080db916
	.4byte 0x080db916
	.4byte 0x080db916
	.4byte 0x080db906
	.4byte 0x080db90c
	.4byte 0x080db916
	.4byte 0x080db916
	.4byte 0x080db916
	.2byte 0xb912
	.2byte 0x080d
	bl	0x080dd950
	b.n	.L_080db916
	bl	Func_080debd8
	b.n	.L_080db916
	bl	0x080e11bc
	b.n	.L_080db916
	bl	Func_080e4730
	b.n	.L_080db916
	bl	0x080e9db4
	b.n	.L_080db916
	bl	0x080e87c4
.L_080db916:
	pop	{pc}
	.2byte 0xb8a0
	.2byte 0x080d
	.global Func_080db91c
	.thumb_func
Func_080db91c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r1, [r3, #0]
	sub	sp, #4
	cmp	r1, #1
	bne.n	.L_080db968
	ldr	r2, [r0, #80]
	cmp	r2, #0
	beq.n	.L_080db968
	ldrb	r3, [r2, #17]
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_080db968
	ldrb	r1, [r2, #27]
	cmp	r1, #0
	beq.n	.L_080db964
	ldr	r3, [pc, #44]
	adds	r7, r2, #0
	ldr	r3, [r3, #0]
	adds	r7, #40
	mov	r8, r3
	adds	r6, r1, #0
.L_080db94e:
	mov	r0, r8
	movs	r1, #6
	str	r2, [sp, #0]
	bl	0x0800206c
	ldmia	r7!, {r5}
	subs	r6, #1
	strb	r0, [r5, #5]
	ldr	r2, [sp, #0]
	cmp	r6, #0
	bne.n	.L_080db94e
.L_080db964:
	movs	r3, #1
	strb	r3, [r2, #25]
.L_080db968:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, lr}
	adds	r6, r0, #0
	sub	sp, #12
	adds	r0, r1, #0
	adds	r1, r2, #0
	cmp	r6, #0
	beq.n	.L_080db9a2
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	Func_0801489c
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	adds	r0, r6, #0
	bl	Object_SetPosition
.L_080db9a2:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
