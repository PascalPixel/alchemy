.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_080ad3a8, 0x080ad3a8
	.set sub_080af054, 0x080af054
	.set sub_080b0288, 0x080b0288
	.set sub_080b0378, 0x080b0378
	.set sub_080b0460, 0x080b0460
	.global Func_080b04bc
	.thumb_func
Func_080b04bc:
	push	{r5, r6, lr}
	movs	r2, #42
	adds	r5, r0, #0
	adds	r2, #255
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L_080b0512
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_080af054
	adds	r6, r0, #0
	movs	r0, #1
	cmp	r6, #0
	beq.n	.L_080b0512
	ldrh	r3, [r6, #14]
	cmp	r3, #0
	beq.n	.L_080b0512
	adds	r0, r5, #0
	bl	sub_080b0460
	ldrb	r2, [r6, #11]
	movs	r1, #100
	lsls	r3, r2, #2
	adds	r3, r3, r2
	adds	r0, r0, r3
	lsls	r0, r0, #16
	bl	sub_08002054
	adds	r5, r0, #0
	bl	sub_080b0378
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r0, r3
	cmp	r5, r0
	ble.n	.L_080b0510
	ldrh	r0, [r6, #14]
	b.n	.L_080b0512
.L_080b0510:
	movs	r0, #1
.L_080b0512:
	pop	{r5, r6, pc}
	push	{lr}
	subs	r0, #8
	cmp	r0, #77
	bls.n	.L_080b051e
	b.n	.L_080b069c
.L_080b051e:
	ldr	r2, [pc, #388]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080b0690
	.4byte 0x080b0690
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b0660
	.4byte 0x080b0660
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b0664
	.4byte 0x080b0664
	.4byte 0x080b0670
	.4byte 0x080b0670
	.4byte 0x080b067c
	.4byte 0x080b0680
	.4byte 0x080b0668
	.4byte 0x080b0698
	.4byte 0x080b066c
	.4byte 0x080b0670
	.4byte 0x080b0674
	.4byte 0x080b0678
	.4byte 0x080b0690
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b067c
	.4byte 0x080b0690
	.4byte 0x080b069c
	.4byte 0x080b0680
	.4byte 0x080b0694
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b0684
	.4byte 0x080b0688
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b068c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b069c
	.4byte 0x080b0690
	.4byte 0x080b069c
	.4byte 0x080b0698
	.4byte 0x080b0694
	.4byte 0x080b069c
	.2byte 0x0698
	.2byte 0x080b
	movs	r0, #70
	b.n	.L_080b06a0
	movs	r0, #75
	b.n	.L_080b06a0
	movs	r0, #30
	b.n	.L_080b06a0
	movs	r0, #45
	b.n	.L_080b06a0
	movs	r0, #55
	b.n	.L_080b06a0
	movs	r0, #25
	b.n	.L_080b06a0
	movs	r0, #20
	b.n	.L_080b06a0
	movs	r0, #65
	b.n	.L_080b06a0
	movs	r0, #35
	b.n	.L_080b06a0
	movs	r0, #60
	b.n	.L_080b069e
	movs	r0, #90
	b.n	.L_080b069e
	movs	r0, #70
	b.n	.L_080b069e
	movs	r0, #60
	b.n	.L_080b06a0
	movs	r0, #50
	b.n	.L_080b06a0
	movs	r0, #40
	b.n	.L_080b06a0
.L_080b069c:
	movs	r0, #100
.L_080b069e:
	negs	r0, r0
.L_080b06a0:
	pop	{pc}
	movs	r0, r0
	.2byte 0x0528
	.2byte 0x080b
	push	{lr}
	ldr	r2, [pc, #24]
	movs	r1, #0
.L_080b06ae:
	ldrh	r3, [r2, #0]
	adds	r2, #2
	cmp	r0, r3
	bne.n	.L_080b06ba
	movs	r0, #1
	b.n	.L_080b06c2
.L_080b06ba:
	adds	r1, #1
	cmp	r1, #46
	bls.n	.L_080b06ae
	movs	r0, #0
.L_080b06c2:
	pop	{pc}
	.2byte 0x6b18
	.2byte 0x080c
	push	{lr}
	ldr	r2, [pc, #24]
	movs	r1, #0
.L_080b06ce:
	ldrh	r3, [r2, #0]
	adds	r2, #2
	cmp	r0, r3
	bne.n	.L_080b06da
	movs	r0, #1
	b.n	.L_080b06e2
.L_080b06da:
	adds	r1, #1
	cmp	r1, #28
	bls.n	.L_080b06ce
	movs	r0, #0
.L_080b06e2:
	pop	{pc}
	.2byte 0x6b76
	.2byte 0x080c
	push	{r5, lr}
	adds	r5, r1, #0
	movs	r1, #42
	adds	r2, r0, #0
	adds	r1, #255
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b0718
	adds	r1, #33
	adds	r3, r2, r1
	ldrh	r0, [r3, #0]
	bl	sub_080ad3a8
	movs	r2, #0
	adds	r0, #62
.L_080b0708:
	ldrb	r3, [r0, #0]
	cmp	r3, r5
	beq.n	.L_080b0730
	adds	r2, #1
	adds	r0, #1
	cmp	r2, #2
	ble.n	.L_080b0708
	b.n	.L_080b073a
.L_080b0718:
	movs	r1, #42
	adds	r1, #255
	adds	r3, r2, r1
	ldrb	r0, [r3, #0]
	bl	sub_080b0288
	movs	r2, #0
	adds	r0, #80
.L_080b0728:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	cmp	r3, r5
	bne.n	.L_080b0734
.L_080b0730:
	movs	r0, #1
	b.n	.L_080b073c
.L_080b0734:
	adds	r2, #1
	cmp	r2, #2
	ble.n	.L_080b0728
.L_080b073a:
	movs	r0, #0
.L_080b073c:
	pop	{r5, pc}
	.align 2, 0
