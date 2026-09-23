.syntax unified
	.thumb
	.set sub_08002096, 0x08002096
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014dac, 0x08014dac
	.set sub_0801596c, 0x0801596c
	.set sub_08015e44, 0x08015e44
	.set sub_08016054, 0x08016054
	.set sub_0801613c, 0x0801613c
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_0803a3b8, 0x0803a3b8
	.set sub_0803a69c, 0x0803a69c
	.set sub_0803a7ac, 0x0803a7ac
	.set sub_0803ce1c, 0x0803ce1c
	.set sub_08042e28, 0x08042e28
	.set sub_08042eec, 0x08042eec
	.set sub_08042f10, 0x08042f10
	.set sub_08042ff0, 0x08042ff0
	.set sub_08043230, 0x08043230
	.set sub_08043cd8, 0x08043cd8
	.set sub_0804d9e0, 0x0804d9e0
	.set sub_080ad100, 0x080ad100
	.set sub_080ad250, 0x080ad250
	.set sub_080ad2c8, 0x080ad2c8
	.set sub_080c8628, 0x080c8628
	.set sub_080c8648, 0x080c8648
	.set sub_081c0010, 0x081c0010
	.global Func_080432a4
	.thumb_func
Func_080432a4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	bl	sub_0801596c
	movs	r1, #9
	negs	r1, r1
	movs	r6, #0
	mov	r8, r1
	cmp	r0, #0
	bne.n	.L_0804333a
	bl	sub_08016054
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	ldr	r2, [pc, #68]
	ldr	r7, [r3, #0]
	ldr	r1, [pc, #68]
	ldr	r3, [pc, #68]
	strh	r6, [r2, #0]
	mov	sl, r3
	strh	r6, [r1, #0]
	strh	r6, [r3, #0]
	movs	r3, #192
	mov	lr, r2
	lsls	r3, r3, #6
	ldr	r2, [pc, #40]
	adds	r3, #108
	mov	r8, r0
	movs	r5, #0
	mov	ip, r1
	adds	r0, r7, r3
.L_080432e8:
	movs	r3, #1
	ldrsb	r3, [r0, r3]
	lsls	r4, r5, #6
	cmp	r3, #0
	beq.n	.L_080432f8
	mov	r1, lr
	strh	r2, [r1, #0]
	adds	r6, #1
.L_080432f8:
	movs	r3, #2
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L_08043318
	mov	r3, ip
	strh	r2, [r3, #0]
	b.n	.L_08043318
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x02003860
	.4byte 0x020036d8
	.2byte 0x5350
	.2byte 0x0200
.L_08043318:
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r1, #88
	adds	r3, r4, r1
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_08043332
	movs	r3, #3
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	bne.n	.L_08043332
	mov	r3, sl
	strh	r2, [r3, #0]
.L_08043332:
	adds	r5, #1
	adds	r0, #64
	cmp	r5, #2
	ble.n	.L_080432e8
.L_0804333a:
	bl	sub_0801613c
	mov	r1, r8
	cmp	r1, #0
	beq.n	.L_0804334e
	cmp	r6, r8
	bne.n	.L_0804334e
	mov	r0, r8
	adds	r0, #100
	b.n	.L_08043350
.L_0804334e:
	mov	r0, r8
.L_08043350:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
.L_08043358:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #0
	ldr	r5, [pc, #368]
	sub	sp, #28
	mov	r8, r0
	bl	sub_080ad2c8
	ldr	r2, [pc, #364]
	ldr	r3, [pc, #364]
	str	r0, [r2, #0]
	movs	r0, #128
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #360]
	str	r1, [r2, #4]
	str	r1, [r3, #0]
	ldr	r3, [pc, #360]
	lsls	r0, r0, #2
	ldrb	r1, [r3, #0]
	adds	r0, #74
	adds	r3, r2, r0
	strb	r1, [r3, #0]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r2, r2, r1
	ldr	r0, [r2, #0]
	bl	sub_08016ca4
	adds	r7, r5, #0
	adds	r6, r0, #0
	adds	r1, r5, #0
	subs	r7, #16
	adds	r2, r6, #0
	movs	r5, #11
.L_0804339e:
	ldrb	r3, [r2, #0]
	subs	r5, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #1
	cmp	r5, #0
	bge.n	.L_0804339e
	ldrb	r3, [r6, #15]
	ldr	r5, [pc, #296]
	strb	r3, [r7, #28]
	movs	r2, #240
	ldr	r3, [r5, #4]
	lsls	r2, r2, #1
	str	r3, [r7, #32]
	adds	r3, r5, r2
	adds	r2, #2
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl	sub_080c8648
	strh	r0, [r7, #30]
	movs	r0, #42
	adds	r0, #255
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	movs	r0, #0
	strb	r3, [r7, #29]
	ldr	r3, [r5, #16]
	movs	r5, #0
	str	r3, [r7, #36]
	bl	sub_080ad250
	adds	r3, r7, #0
	adds	r3, #40
	strb	r0, [r3, #0]
	movs	r0, #1
	bl	sub_080ad250
	adds	r3, r7, #0
	adds	r3, #41
	strb	r0, [r3, #0]
	movs	r0, #2
	bl	sub_080ad250
	adds	r3, r7, #0
	adds	r3, #42
	strb	r0, [r3, #0]
	movs	r0, #3
	bl	sub_080ad250
	adds	r3, r7, #0
	adds	r3, #43
	mov	r6, sp
	strb	r0, [r3, #0]
	adds	r0, r6, #0
	bl	sub_080ad100
	ldrh	r3, [r6, r5]
	cmp	r3, #255
	beq.n	.L_08043438
	adds	r1, r7, #0
	adds	r0, r6, #0
	adds	r1, #44
	movs	r2, #0
.L_08043424:
	ldrh	r3, [r2, r0]
	adds	r5, #1
	strb	r3, [r1, #0]
	adds	r2, #2
	adds	r1, #1
	cmp	r5, #3
	bgt.n	.L_08043438
	ldrh	r3, [r2, r6]
	cmp	r3, #255
	bne.n	.L_08043424
.L_08043438:
	movs	r1, #1
	adds	r2, r5, #0
	negs	r1, r1
	adds	r3, r1, #0
	adds	r2, #44
	strb	r3, [r7, r2]
	movs	r0, #147
	ldr	r2, [pc, #144]
	lsls	r0, r0, #1
	adds	r0, #255
	adds	r3, r2, r0
	ldrb	r1, [r3, #0]
	adds	r3, r7, #0
	adds	r3, #52
	strb	r1, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #38
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	adds	r1, r7, #0
	adds	r1, #53
	strb	r3, [r1, #0]
	movs	r3, #152
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r2, r3
	ldrb	r3, [r2, #0]
	adds	r2, r7, #0
	adds	r2, #49
	strb	r3, [r2, #0]
	adds	r3, r7, #0
	adds	r3, #50
	movs	r0, #0
	strb	r0, [r3, #0]
	movs	r5, #48
	adds	r6, r3, #0
.L_08043482:
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08043492
	ldrb	r3, [r6, #0]
	adds	r3, #1
	strb	r3, [r6, #0]
.L_08043492:
	adds	r5, #1
	cmp	r5, #127
	ble.n	.L_08043482
	movs	r0, #34
	bl	sub_08016ce4
	negs	r3, r0
	orrs	r3, r0
	adds	r2, r7, #0
	lsrs	r3, r3, #31
	adds	r2, #51
	strb	r3, [r2, #0]
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #56]
	ldr	r3, [r3, #0]
	movs	r1, #242
	movs	r5, #0
	lsls	r1, r1, #2
	strh	r3, [r7, #54]
	b.n	.L_080434c0
.L_080434ba:
	ldmia	r2!, {r3}
	adds	r5, #1
	add	r8, r3
.L_080434c0:
	cmp	r5, r1
	blt.n	.L_080434ba
	mov	r0, r8
	str	r0, [r7, #60]
	add	sp, #28
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000000
	.4byte 0x02000240
	.4byte 0x0300117c
	.4byte 0x02001000
	.4byte 0x03001200
	.2byte 0x0040
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r6, #0
	bl	sub_0801596c
	cmp	r0, #0
	beq.n	.L_08043506
	ldr	r0, [pc, #44]
	movs	r1, #1
	bl	sub_0803a7ac
	subs	r6, #9
	b.n	.L_0804351e
.L_08043506:
	ldr	r1, [pc, #36]
	adds	r0, r5, #0
	bl	sub_08042f10
	cmp	r0, #0
	beq.n	.L_0804351e
	ldr	r0, [pc, #28]
	movs	r1, #1
	bl	sub_0803a7ac
	movs	r6, #3
	negs	r6, r6
.L_0804351e:
	bl	sub_0801613c
	adds	r0, r6, #0
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x0000000b
	.4byte 0x02000000
	.2byte 0x000c
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #192
	movs	r2, #0
	lsls	r0, r0, #6
	mov	sl, r2
	bl	sub_08014dac
	ldr	r3, [pc, #152]
	adds	r7, r0, #0
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	mov	r8, r3
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_080435da
	bl	sub_0801596c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_08043570
	ldr	r0, [pc, #128]
	movs	r1, #1
	bl	sub_0803a7ac
	movs	r2, #9
	b.n	.L_080435ca
.L_08043570:
	mov	r2, r8
	movs	r3, #0
	ldrsh	r0, [r2, r3]
	adds	r1, r7, #0
	bl	sub_08015e44
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_08043590
	ldr	r0, [pc, #104]
	movs	r1, #1
	bl	sub_0803a7ac
	movs	r3, #2
	negs	r3, r3
	mov	sl, r3
.L_08043590:
	ldr	r1, [pc, #92]
	ldr	r3, [pc, #96]
	adds	r0, r7, r1
	movs	r2, #16
	subs	r0, r0, r3
	ldr	r3, [pc, #92]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2000
	bl	sub_08042e28
	mov	r3, r8
	adds	r5, r0, #0
	adds	r1, r7, #0
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_08042f10
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_08042eec
	cmp	r6, #0
	beq.n	.L_080435ce
	ldr	r0, [pc, #40]
	movs	r1, #1
	bl	sub_0803a7ac
	movs	r2, #3
.L_080435ca:
	negs	r2, r2
	mov	sl, r2
.L_080435ce:
	bl	sub_0801613c
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, sl
.L_080435da:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x020036d0
	.4byte 0x0000000b
	.4byte 0x0000000c
	.4byte 0x02000504
	.4byte 0x02000000
	.2byte 0x0730
	.2byte 0x0300
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r6, #0
	cmp	r5, #2
	bls.n	.L_08043608
	movs	r5, #2
.L_08043608:
	bl	sub_0801596c
	cmp	r0, #0
	beq.n	.L_08043618
	ldr	r0, [pc, #100]
	movs	r1, #1
	movs	r6, #9
	b.n	.L_0804365e
.L_08043618:
	bl	sub_08016054
	ldr	r3, [pc, #92]
	movs	r0, #85
	strh	r5, [r3, #0]
	bl	sub_081c0010
	movs	r1, #13
	ldr	r0, [pc, #84]
	bl	sub_0803a7ac
	bl	.L_08043358
	bl	sub_080c8628
	adds	r0, r5, #0
	ldr	r1, [pc, #72]
	bl	sub_08042f10
	adds	r5, r0, #0
	b.n	.L_08043648
.L_08043642:
	movs	r0, #1
	bl	sub_08013560
.L_08043648:
	bl	sub_0803a3b8
	cmp	r0, #0
	beq.n	.L_08043642
	bl	sub_0803ce1c
	cmp	r5, #0
	beq.n	.L_08043666
	ldr	r0, [pc, #44]
	movs	r1, #1
	movs	r6, #3
.L_0804365e:
	bl	sub_0803a7ac
	negs	r6, r6
	b.n	.L_0804366e
.L_08043666:
	ldr	r0, [pc, #36]
	movs	r1, #9
	bl	sub_0803a7ac
.L_0804366e:
	bl	sub_0801613c
	adds	r0, r6, #0
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x0000000b
	.4byte 0x020036d0
	.4byte 0x0000001d
	.4byte 0x02000000
	.4byte 0x0000000c
	.2byte 0x0019
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r7, #0
	bl	sub_0801596c
	cmp	r0, #0
	beq.n	.L_080436a8
	ldr	r0, [pc, #188]
	movs	r1, #1
	bl	sub_0803a7ac
	subs	r7, #9
	b.n	.L_08043752
.L_080436a8:
	bl	sub_08016054
	ldr	r3, [pc, #176]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #0
	bl	sub_08043cd8
	movs	r2, #1
	adds	r5, r0, #0
	negs	r2, r2
	cmp	r5, r2
	bne.n	.L_080436c6
	adds	r7, r5, #0
	b.n	.L_08043752
.L_080436c6:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	movs	r1, #192
	ldr	r2, [r3, #0]
	lsls	r1, r1, #6
	lsls	r3, r5, #6
	adds	r1, #88
	adds	r3, r3, r1
	ldrb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_08043710
	ldr	r0, [pc, #132]
	movs	r1, #13
	bl	sub_0803a7ac
	b.n	.L_080436ee
.L_080436e8:
	movs	r0, #1
	bl	sub_08013560
.L_080436ee:
	bl	sub_0803a3b8
	cmp	r0, #0
	beq.n	.L_080436e8
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	movs	r3, #1
	bl	sub_0804d9e0
	cmp	r0, #0
	beq.n	.L_0804370c
	bl	sub_0803ce1c
	b.n	.L_08043752
.L_0804370c:
	bl	sub_0803ce1c
.L_08043710:
	ldr	r3, [pc, #76]
	movs	r0, #0
	strh	r5, [r3, #0]
	bl	sub_08042e28
	adds	r6, r0, #0
	bl	.L_08043358
	bl	sub_080c8628
	ldr	r1, [pc, #64]
	adds	r0, r5, #0
	bl	sub_08042f10
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08042eec
	bl	sub_0803ce1c
	cmp	r5, #0
	beq.n	.L_0804374a
	ldr	r0, [pc, #44]
	movs	r1, #1
	movs	r7, #3
	bl	sub_0803a7ac
	negs	r7, r7
	b.n	.L_08043752
.L_0804374a:
	ldr	r0, [pc, #36]
	movs	r1, #9
	bl	sub_0803a7ac
.L_08043752:
	bl	sub_0801613c
	adds	r0, r7, #0
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000000b
	.4byte 0x020036d0
	.4byte 0x00000015
	.4byte 0x02000000
	.4byte 0x0000000c
	.2byte 0x0019
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	movs	r6, #0
	bl	sub_0801596c
	cmp	r0, #0
	beq.n	.L_0804378e
	ldr	r0, [pc, #104]
	movs	r1, #1
	bl	sub_0803a7ac
	subs	r6, #9
	b.n	.L_080437e2
.L_0804378e:
	bl	sub_08016054
	ldr	r7, [pc, #92]
	movs	r1, #0
	ldrsh	r0, [r7, r1]
	adds	r1, r5, #0
	bl	sub_08043cd8
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L_080437ac
	adds	r6, r5, #0
	b.n	.L_080437e2
.L_080437ac:
	ldr	r1, [pc, #68]
	adds	r0, r5, #0
	bl	sub_08015e44
	cmp	r0, #0
	beq.n	.L_080437c6
	ldr	r0, [pc, #60]
	movs	r1, #1
	movs	r6, #2
	bl	sub_0803a7ac
	negs	r6, r6
	b.n	.L_080437e2
.L_080437c6:
	ldr	r3, [pc, #52]
	ldr	r1, [pc, #52]
	ldr	r2, [r3, #4]
	str	r2, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #74
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	ldr	r2, [pc, #40]
	strb	r3, [r2, #0]
	ldr	r3, [pc, #40]
	strh	r6, [r3, #0]
	strh	r5, [r7, #0]
.L_080437e2:
	bl	sub_0801613c
	adds	r0, r6, #0
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000000b
	.4byte 0x020036d0
	.4byte 0x02000000
	.4byte 0x0000000d
	.4byte 0x02000240
	.4byte 0x0300117c
	.4byte 0x03001200
	.2byte 0x1218
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r7, #0
	bl	sub_0801596c
	cmp	r0, #0
	beq.n	.L_08043828
	ldr	r0, [pc, #148]
	movs	r1, #1
	bl	sub_0803a7ac
	subs	r7, #9
	b.n	.L_080438a6
.L_08043828:
	bl	sub_08016054
	movs	r0, #0
	movs	r1, #2
	bl	sub_08043cd8
	movs	r3, #1
	adds	r6, r0, #0
	negs	r3, r3
	cmp	r6, r3
	bne.n	.L_08043842
	adds	r7, r6, #0
	b.n	.L_080438a6
.L_08043842:
	ldr	r3, [pc, #116]
	adds	r0, r6, #0
	mov	r8, r3
	mov	r1, r8
	bl	sub_08015e44
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0804385c
	ldr	r0, [pc, #100]
	movs	r1, #1
	movs	r7, #2
	b.n	.L_08043896
.L_0804385c:
	bl	sub_08043230
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r6, r0, #0
	adds	r3, #255
	cmp	r6, r3
	bne.n	.L_08043874
	ldr	r0, [pc, #80]
	movs	r1, #1
	movs	r7, #5
	b.n	.L_08043896
.L_08043874:
	movs	r0, #2
	bl	sub_08042e28
	mov	r1, r8
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08042f10
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_08042eec
	cmp	r6, #0
	beq.n	.L_0804389e
	ldr	r0, [pc, #44]
	movs	r1, #1
	movs	r7, #3
.L_08043896:
	bl	sub_0803a7ac
	negs	r7, r7
	b.n	.L_080438a6
.L_0804389e:
	ldr	r0, [pc, #36]
	movs	r1, #1
	bl	sub_0803a7ac
.L_080438a6:
	bl	sub_0801613c
	adds	r0, r7, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000000b
	.4byte 0x02000000
	.4byte 0x0000000d
	.4byte 0x0000000e
	.2byte 0x001b
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r7, #0
	bl	sub_0801596c
	cmp	r0, #0
	beq.n	.L_080438e0
	ldr	r0, [pc, #144]
	movs	r1, #1
	bl	sub_0803a7ac
	subs	r7, #9
	b.n	.L_08043960
.L_080438e0:
	bl	sub_08016054
	movs	r0, #0
	movs	r1, #3
	bl	sub_08043cd8
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L_080438fa
	adds	r7, r5, #0
	b.n	.L_08043960
.L_080438fa:
	movs	r1, #8
	movs	r2, #1
	movs	r3, #2
	ldr	r0, [pc, #104]
	bl	sub_0803a69c
	b.n	.L_0804390e
.L_08043908:
	movs	r0, #1
	bl	sub_08013560
.L_0804390e:
	bl	sub_0803a3b8
	cmp	r0, #0
	beq.n	.L_08043908
	movs	r0, #1
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	bl	sub_0804d9e0
	cmp	r0, #0
	beq.n	.L_0804392c
	bl	sub_0803ce1c
	b.n	.L_08043960
.L_0804392c:
	bl	sub_0803ce1c
	movs	r0, #1
	bl	sub_08042e28
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_08042ff0
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08042eec
	cmp	r5, #0
	beq.n	.L_08043958
	ldr	r0, [pc, #36]
	movs	r1, #1
	movs	r7, #4
	bl	sub_0803a7ac
	negs	r7, r7
	b.n	.L_08043960
.L_08043958:
	ldr	r0, [pc, #24]
	movs	r1, #1
	bl	sub_0803a7ac
.L_08043960:
	bl	sub_0801613c
	adds	r0, r7, #0
	pop	{r5, r6, r7, pc}
	.4byte 0x0000000b
	.4byte 0x00000017
	.4byte 0x0000000e
	.2byte 0x001a
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r7, [pc, #64]
	ldr	r6, [pc, #64]
	movs	r5, #0
.L_08043980:
	ldr	r3, [r7, #0]
	lsls	r2, r5, #3
	adds	r3, r3, r2
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #8
	bl	sub_08002096
	cmp	r0, #0
	bge.n	.L_0804399c
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	adds	r0, r0, r3
.L_0804399c:
	asrs	r3, r0, #14
	lsls	r1, r3, #1
	adds	r2, r3, #0
	adds	r1, #22
	adds	r2, #16
	adds	r3, #20
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #0]
	adds	r6, #2
	adds	r5, #1
	cmp	r5, #3
	ble.n	.L_08043980
	pop	{r5, r6, r7, pc}
	.4byte 0x03001100
	.2byte 0x01d0
	.2byte 0x0500
	push	{lr}
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #8]
	bl	sub_080145a8
	pop	{pc}
	movs	r0, r0
	.2byte 0x3979
	.2byte 0x0804
	push	{lr}
	ldr	r0, [pc, #8]
	bl	sub_08014644
	pop	{pc}
	movs	r0, r0
	.2byte 0x3979
	.2byte 0x0804
	push	{r5, r6, r7, lr}
	adds	r5, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #60]
	movs	r7, #12
	ldrsh	r3, [r0, r7]
	ldr	r4, [sp, #16]
	adds	r3, r1, r3
	adds	r1, r3, #1
	movs	r7, #14
	ldrsh	r3, [r0, r7]
	adds	r3, r2, r3
	adds	r2, r3, #1
	cmp	r1, #0
	bge.n	.L_08043a0c
	adds	r5, r5, r1
	movs	r1, #0
.L_08043a0c:
	adds	r3, r1, r5
	cmp	r3, #29
	ble.n	.L_08043a16
	movs	r3, #30
	subs	r5, r3, r1
.L_08043a16:
	cmp	r2, #0
	bge.n	.L_08043a1e
	adds	r4, r4, r2
	movs	r2, #0
.L_08043a1e:
	adds	r3, r2, r4
	cmp	r3, #29
	ble.n	.L_08043a28
	movs	r3, #20
	subs	r4, r3, r2
.L_08043a28:
	cmp	r5, #0
	ble.n	.L_08043a60
	cmp	r4, #0
	ble.n	.L_08043a60
	lsls	r3, r1, #1
	lsls	r2, r2, #6
	adds	r3, r6, r3
	adds	r1, r2, r3
.L_08043a38:
	adds	r2, r1, #0
	adds	r3, r5, #0
	adds	r2, #8
	cmp	r3, #0
	beq.n	.L_08043a54
	ldr	r0, [pc, #12]
.L_08043a44:
	subs	r3, #1
	strh	r0, [r2, #0]
	adds	r2, #2
	cmp	r3, #0
	bne.n	.L_08043a44
	b.n	.L_08043a54
	.2byte 0xe006
	.2byte 0x0000
.L_08043a54:
	subs	r4, #1
	adds	r1, #64
	cmp	r4, #0
	bne.n	.L_08043a38
	movs	r3, #1
	strb	r3, [r6, #3]
.L_08043a60:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
