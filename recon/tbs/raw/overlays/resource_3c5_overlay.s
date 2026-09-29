.syntax unified
	.thumb
	.section .text.x0200a8a0,"ax",%progbits
	.balign 4
	.global BabiIriguchi_SetupScene
	.thumb_func
BabiIriguchi_SetupScene:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #1
	sub	sp, #8
	bl 0x0200ad80
	ldr	r3, [pc, #84]
	movs	r2, #224
	ldr	r1, [r3, #0]
	movs	r3, #129
	lsls	r2, r2, #1
	lsls	r3, r3, #2
	str	r3, [r1, r2]
	ldr	r6, [pc, #72]
	ldr	r3, [pc, #76]
	ldrsh	r1, [r6, r2]
	cmp	r1, r3
	beq.n	.L_020028dc
	adds	r2, #130
	adds	r3, r6, r2
	movs	r0, #144
	movs	r2, #1
	strh	r2, [r3, #0]
	lsls	r0, r0, #2
	ldr	r2, [pc, #60]
	adds	r3, r6, r0
	strh	r2, [r3, #0]
	mov	ip, r1
	b.n	.L_02002914
.L_020028dc:
	movs	r0, #12
	bl 0x0200ae50
	adds	r1, r0, #0
	ldr	r3, [r1, #8]
	asrs	r2, r3, #20
	cmp	r2, #20
	beq.n	.L_020028ee
	b.n	.L_02002d3c
.L_020028ee:
	ldr	r3, [r1, #16]
	asrs	r0, r3, #20
	cmp	r0, #12
	beq.n	.L_020028f8
	b.n	.L_02002d3c
.L_020028f8:
	str	r2, [sp, #0]
	str	r0, [sp, #4]
	movs	r1, #12
	movs	r0, #38
	b.n	.L_02002af2
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x000000b1
	.2byte 0x00b0
	.2byte 0x0000
.L_02002914:
	cmp	ip, r2
	beq.n	.L_0200291a
	b.n	.L_02002a7a
.L_0200291a:
	movs	r0, #8
	movs	r1, #6
	bl 0x0200aec8
	movs	r0, #9
	movs	r1, #6
	bl 0x0200aec8
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #5
	bne.n	.L_02002950
	ldr	r0, [pc, #180]
	bl 0x0200ae18
	cmp	r0, #0
	bne.n	.L_02002950
	movs	r1, #156
	movs	r2, #164
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200ae88
.L_02002950:
	bl 0x0200a820
	movs	r0, #9
	bl 0x0200ae50
	adds	r5, r0, #0
	movs	r0, #9
	bl 0x0200ae50
	ldr	r3, [r0, #20]
	movs	r0, #192
	str	r3, [r5, #12]
	lsls	r0, r0, #2
	bl 0x0200ae18
	cmp	r0, #0
	beq.n	.L_0200298a
	movs	r0, #10
	movs	r1, #4
	bl 0x0200ae90
	movs	r0, #10
	bl 0x0200ae50
	movs	r3, #254
	adds	r0, #89
	strb	r3, [r0, #0]
	bl 0x0200a7f4
.L_0200298a:
	movs	r0, #11
	bl 0x0200ae50
	adds	r1, r0, #0
	adds	r2, r1, #0
	movs	r3, #0
	adds	r2, #89
	strb	r3, [r2, #0]
	subs	r2, #54
	strb	r3, [r2, #0]
	adds	r2, #59
	strh	r3, [r2, #0]
	ldr	r2, [r1, #80]
	ldrb	r3, [r2, #9]
	movs	r6, #12
	orrs	r3, r6
	strb	r3, [r2, #9]
	ldr	r3, [r1, #80]
	ldr	r5, [pc, #60]
	adds	r3, #38
	strb	r5, [r3, #0]
	movs	r3, #192
	ldr	r2, [r1, #80]
	lsls	r3, r3, #8
	strh	r3, [r2, #30]
	movs	r1, #0
	movs	r0, #11
	bl 0x0200ae90
	movs	r0, #12
	bl 0x0200ae50
	adds	r1, r0, #0
	adds	r3, r1, #0
	adds	r3, #89
	strb	r5, [r3, #0]
	adds	r2, r1, #0
	subs	r3, #54
	strb	r5, [r3, #0]
	adds	r2, #94
	movs	r3, #30
	strh	r3, [r2, #0]
	ldr	r2, [r1, #80]
	ldrb	r3, [r2, #9]
	orrs	r3, r6
	strb	r3, [r2, #9]
	ldr	r3, [r1, #80]
	adds	r3, #38
	b.n	.L_020029f4
	.4byte 0x00000000
	.2byte 0x0109
	.2byte 0x0000
.L_020029f4:
	strb	r5, [r3, #0]
	movs	r3, #128
	ldr	r2, [r1, #80]
	lsls	r3, r3, #7
	strh	r3, [r2, #30]
	movs	r1, #0
	movs	r0, #12
	bl 0x0200ae90
	movs	r0, #13
	bl 0x0200ae50
	adds	r1, r0, #0
	adds	r3, r1, #0
	adds	r3, #89
	strb	r5, [r3, #0]
	adds	r2, r1, #0
	subs	r3, #54
	strb	r5, [r3, #0]
	adds	r2, #94
	movs	r3, #60
	strh	r3, [r2, #0]
	ldr	r2, [r1, #80]
	ldrb	r3, [r2, #9]
	orrs	r3, r6
	strb	r3, [r2, #9]
	ldr	r3, [r1, #80]
	adds	r3, #38
	strb	r5, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #8
	ldr	r3, [r1, #80]
	mov	r8, r2
	mov	r0, r8
	strh	r0, [r3, #30]
	movs	r1, #0
	movs	r0, #13
	bl 0x0200ae90
	movs	r0, #14
	bl 0x0200ae50
	adds	r1, r0, #0
	adds	r3, r1, #0
	adds	r3, #89
	strb	r5, [r3, #0]
	adds	r2, r1, #0
	subs	r3, #54
	strb	r5, [r3, #0]
	adds	r2, #94
	movs	r3, #90
	strh	r3, [r2, #0]
	ldr	r2, [r1, #80]
	ldrb	r3, [r2, #9]
	orrs	r3, r6
	strb	r3, [r2, #9]
	ldr	r3, [r1, #80]
	adds	r3, #38
	strb	r5, [r3, #0]
	ldr	r3, [r1, #80]
	mov	r2, r8
	strh	r2, [r3, #30]
	movs	r0, #14
	movs	r1, #0
	bl 0x0200ae90
	b.n	.L_02002d3c
.L_02002a7a:
	ldr	r3, [pc, #720]
	cmp	ip, r3
	bne.n	.L_02002b7e
	movs	r0, #225
	lsls	r0, r0, #1
	adds	r3, r6, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #10
	cmp	r3, #7
	bls.n	.L_02002a92
	b.n	.L_02002d3c
.L_02002a92:
	ldr	r2, [pc, #700]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200aabc
	.4byte 0x0200aac4
	.4byte 0x0200ad24
	.4byte 0x0200ad24
	.4byte 0x0200aafc
	.4byte 0x0200ad24
	.4byte 0x0200ab28
	.4byte 0x0200ab54
	.4byte 0x01002098
	.4byte 0xf9aef000
	.4byte 0x01002098
	.4byte 0xf9a6f000
	.4byte 0xd1002800
	.4byte 0x2301e134
	.4byte 0x93002203
	.4byte 0x20789201
	.4byte 0x226d2107
	.4byte 0xf0002307
	.4byte 0x232df971
	.4byte 0x93002209
	.4byte 0x202d9201
	.2byte 0x2108
.L_02002af2:
	movs	r2, #1
	movs	r3, #1
	bl 0x0200add8
	b.n	.L_02002d3c
	.4byte 0x229120dc
	.4byte 0x21000452
	.4byte 0x044023df
	.4byte 0xff74f7fd
	.4byte 0x220d231b
	.4byte 0x92019300
	.4byte 0x210d2016
	.4byte 0x23012201
	.4byte 0xf95cf000
	.4byte 0xf7fe200e
	.4byte 0xe109f9c3
	.4byte 0x229120e0
	.4byte 0x21000452
	.4byte 0x044023df
	.4byte 0xff5ef7fd
	.4byte 0x220a231c
	.4byte 0x92019300
	.4byte 0x210c2016
	.4byte 0x23012201
	.4byte 0xf946f000
	.4byte 0xf7fe2010
	.4byte 0xe0f3f9ad
	.4byte 0x4a7f20e8
	.4byte 0x23df2100
	.4byte 0xf7fd0400
	.4byte 0x230eff49
	.4byte 0x93002221
	.4byte 0x20169201
	.4byte 0x2201210c
	.4byte 0xf0002301
	.4byte 0x2011f931
	.4byte 0xf998f7fe
	.2byte 0xe0de
.L_02002b7e:
	ldr	r3, [pc, #472]
	cmp	r1, r3
	beq.n	.L_02002b86
	b.n	.L_02002d34
.L_02002b86:
	movs	r0, #8
	bl 0x0200ae50
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200ae50
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #128
	ands	r5, r3
	movs	r2, #128
	strb	r5, [r0, #0]
	lsls	r1, r1, #9
	movs	r0, #8
	lsls	r2, r2, #8
	bl 0x0200ae58
	movs	r1, #128
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200ae58
	ldr	r0, [pc, #408]
	bl 0x0200ae18
	cmp	r0, #0
	bne.n	.L_02002be8
	movs	r0, #225
	lsls	r0, r0, #1
	adds	r3, r6, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	bne.n	.L_02002be2
	ldr	r0, [pc, #388]
	bl 0x0200ae20
	b.n	.L_02002be8
.L_02002be2:
	ldr	r0, [pc, #380]
	bl 0x0200ae28
.L_02002be8:
	ldr	r0, [pc, #376]
	bl 0x0200ae18
	cmp	r0, #0
	bne.n	.L_02002c6a
	movs	r0, #10
	ldr	r1, [pc, #368]
	ldr	r2, [pc, #368]
	bl 0x0200ae88
	movs	r1, #140
	movs	r2, #148
	movs	r0, #11
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200ae88
	movs	r1, #156
	movs	r2, #248
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200ae88
	movs	r1, #148
	movs	r2, #248
	movs	r0, #13
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200ae88
	movs	r1, #160
	movs	r2, #148
	movs	r0, #14
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200ae88
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #192
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #192
	movs	r0, #13
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #128
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r0, #5
	bl 0x0200ae30
	b.n	.L_02002cc4
.L_02002c6a:
	ldr	r0, [pc, #256]
	bl 0x0200ae18
	cmp	r0, #0
	beq.n	.L_02002cc4
	movs	r1, #156
	movs	r2, #156
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200ae88
	movs	r1, #176
	movs	r0, #10
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #176
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #176
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #176
	movs	r0, #13
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #176
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r0, #5
	bl 0x0200ae30
.L_02002cc4:
	ldr	r0, [pc, #168]
	bl 0x0200ae18
	cmp	r0, #0
	beq.n	.L_02002d14
	movs	r1, #140
	movs	r2, #240
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200ae88
	movs	r1, #164
	movs	r2, #240
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200ae88
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aef0
	movs	r3, #17
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #81
	movs	r1, #14
	movs	r2, #4
	movs	r3, #1
	bl 0x0200add8
.L_02002d14:
	ldr	r3, [pc, #92]
	movs	r0, #225
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_02002d3c
	ldr	r0, [pc, #52]
	bl 0x0200ae18
	cmp	r0, #0
	bne.n	.L_02002d3c
	bl 0x02008d4c
	b.n	.L_02002d3c
.L_02002d34:
	movs	r0, #12
	movs	r1, #2
	bl 0x0200ae90
.L_02002d3c:
	movs	r0, #0
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x000000af
	.4byte 0x0200aa9c
	.4byte 0x02520000
	.4byte 0x000000ae
	.4byte 0x00000109
	.4byte 0x00000301
	.4byte 0x00000988
	.4byte 0xffc00000
	.4byte 0x00000989
	.4byte 0x00000985
	.4byte 0x02000240
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
	.global StagedActor_DirectionSteps
StagedActor_DirectionSteps:
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
	.global StagedActor_FootprintKinds
StagedActor_FootprintKinds:
	.4byte 0x000000cf
	.4byte 0x000000cd
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0000012a
	.4byte 0x00000129
	.global StagedActor_FootprintBounds
StagedActor_FootprintBounds:
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global gEffectScripts
gEffectScripts:
	.4byte 0x0200b08c
	.4byte 0x0200b0c4
	.4byte 0x0200b0fc
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.global gBabiIriguchiEntrances3
gBabiIriguchiEntrances3:
	.4byte 0xffff0000
	.4byte 0x00000088
	.4byte 0x40000178
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000c8
	.4byte 0xc00001a8
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0002
	.4byte 0x000000c8
	.4byte 0x400000c8
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0003
	.4byte 0x000000d8
	.4byte 0x400000b8
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0004
	.4byte 0x00000068
	.4byte 0x40000138
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0005
	.4byte 0x00000128
	.4byte 0x40000138
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0006
	.4byte 0x00000138
	.4byte 0x40000058
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEntrances2
gBabiIriguchiEntrances2:
	.4byte 0xffff0000
	.4byte 0x00000088
	.4byte 0x40000178
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000e8
	.4byte 0x400001e8
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff0002
	.4byte 0x00000118
	.4byte 0x400001e8
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff0003
	.4byte 0x000000f8
	.4byte 0x40000288
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff0004
	.4byte 0x000001a8
	.4byte 0x40000078
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0005
	.4byte 0x000001c8
	.4byte 0x40000078
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0006
	.4byte 0x000001f8
	.4byte 0x40000078
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0007
	.4byte 0x000001d8
	.4byte 0xc0000120
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0008
	.4byte 0x00000078
	.4byte 0x40000078
	.4byte 0x00300000
	.4byte 0x01200040
	.4byte 0x00000140
	.4byte 0xffff0009
	.4byte 0x000000b8
	.4byte 0xc0000120
	.4byte 0x00300000
	.4byte 0x01200040
	.4byte 0x00000140
	.4byte 0xffff000a
	.4byte 0x000002d8
	.4byte 0x400000a8
	.4byte 0x02700000
	.4byte 0x03600060
	.4byte 0x00000100
	.4byte 0xffff000b
	.4byte 0x00000308
	.4byte 0x400000b8
	.4byte 0x02700000
	.4byte 0x03600060
	.4byte 0x00000100
	.4byte 0xffff000c
	.4byte 0x000001e8
	.4byte 0x400000b8
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff000d
	.4byte 0x000000f8
	.4byte 0x40000248
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff000e
	.4byte 0x000001b8
	.4byte 0x400000d8
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff000f
	.4byte 0x000000d8
	.4byte 0x40000248
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff0010
	.4byte 0x000001c8
	.4byte 0x400000a8
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0011
	.4byte 0x000000e8
	.4byte 0x40000218
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEntrances1
gBabiIriguchiEntrances1:
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0x40000078
	.4byte 0x00600000
	.4byte 0x01800030
	.4byte 0x000001b0
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0xc0000170
	.4byte 0x00600000
	.4byte 0x01800030
	.4byte 0x000001b0
	.4byte 0xffff0003
	.4byte 0x000000a8
	.4byte 0x40000158
	.4byte 0x00600000
	.4byte 0x01800030
	.4byte 0x000001b0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEntrancesOther
gBabiIriguchiEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x00000148
	.4byte 0xc00000f0
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00000190
	.4byte 0xffff0001
	.4byte 0x00000070
	.4byte 0xc0000168
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00000190
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0x40000020
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00000190
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiRegions3
gBabiIriguchiRegions3:
	.4byte 0xffec0060
	.4byte 0x00700140
	.4byte 0x0150fffc
	.4byte 0x0004ffff
	.4byte 0xffec0120
	.4byte 0x01300140
	.4byte 0x0150fffc
	.4byte 0x0005ffff
	.4byte 0xffec0130
	.4byte 0x01400060
	.4byte 0x0070fffc
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiExits
gBabiIriguchiExits:
	.4byte 0x000000b0
	.4byte 0x001020b1
	.4byte 0x002070af
	.4byte 0x003090af
	.4byte 0x004010ad
	.4byte 0x005030ad
	.4byte 0x006020ad
	.4byte 0x000000af
	.4byte 0x001040af
	.4byte 0x002030b0
	.4byte 0x003080af
	.4byte 0x004010af
	.4byte 0x005020af
	.4byte 0x006020b0
	.4byte 0x007050af
	.4byte 0x008060af
	.4byte 0x0090b0af
	.4byte 0x00a030af
	.4byte 0x00b0c0af
	.4byte 0x00c0d0af
	.4byte 0x00d0e0af
	.4byte 0x00e0f0af
	.4byte 0x00f100af
	.4byte 0x010110af
	.4byte 0x011030ae
	.4byte 0x012020ae
	.4byte 0x000000ae
	.4byte 0x001010ac
	.4byte 0x0020a0af
	.4byte 0x000000b1
	.4byte 0x0011f002
	.4byte 0x002010b0
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiPlacements3
gBabiIriguchiPlacements3:
	.4byte 0xffff00fd
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0xffff00fd
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00820000
	.4byte 0x00004000
	.4byte 0xffff0098
	.4byte 0x0200b140
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff0074
	.4byte 0x0200b140
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x0200b140
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0072
	.4byte 0x0200b140
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiPlacements2
gBabiIriguchiPlacements2:
	.4byte 0x0072005d
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00002000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiPlacements1
gBabiIriguchiPlacements1:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffff0128
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00028000
	.4byte 0xffff0128
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00020000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00018000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00018000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00018000
	.4byte 0xffff0046
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiPlacementsOther
gBabiIriguchiPlacementsOther:
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00008000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff0100
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEvents3
gBabiIriguchiEvents3:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x020090f5
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte 0x0200910d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000026fa
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000026fb
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000026fc
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000026fd
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000026fe
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000026ff
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002700
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002701
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02009031
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x02009031
	.4byte 0x00000c15
	.4byte 0x0300000a
	.4byte 0x0200a7f5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEvents2
gBabiIriguchiEvents2:
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000021
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x00000012
	.4byte 0x00004602
	.4byte 0xffff0019
	.4byte 0x02009185
	.4byte 0x00004602
	.4byte 0xffff001a
	.4byte 0x0200919d
	.4byte 0x00004602
	.4byte 0xffff001b
	.4byte 0x020091c5
	.4byte 0x00004602
	.4byte 0xffff001c
	.4byte 0x020091fd
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008cc1
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte 0x02009215
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEvents1
gBabiIriguchiEvents1:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x0988000a
	.4byte 0x02009239
	.4byte 0x00000002
	.4byte 0x13010014
	.4byte 0x0200a549
	.4byte 0x00000002
	.4byte 0x03010015
	.4byte 0x0200a549
	.4byte 0x00000002
	.4byte 0xffff0016
	.4byte 0x0200a4d1
	.4byte 0x00000000
	.4byte 0x0989000a
	.4byte 0x00002725
	.4byte 0x00000000
	.4byte 0x0989000b
	.4byte 0x00002726
	.4byte 0x00000000
	.4byte 0x0989000c
	.4byte 0x00002727
	.4byte 0x00000000
	.4byte 0x0989000d
	.4byte 0x00002728
	.4byte 0x00000000
	.4byte 0x0989000e
	.4byte 0x00002729
	.4byte 0x00008d15
	.4byte 0x0989000a
	.4byte 0x0000272a
	.4byte 0x00008d15
	.4byte 0x0989000b
	.4byte 0x0000272b
	.4byte 0x00008d15
	.4byte 0x0989000c
	.4byte 0x0000272c
	.4byte 0x00008d15
	.4byte 0x0989000d
	.4byte 0x0000272d
	.4byte 0x00008d15
	.4byte 0x0989000e
	.4byte 0x0000272e
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000274c
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000274d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000274e
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000274f
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002750
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002751
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002752
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002753
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002754
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002755
	.4byte 0x00000003
	.4byte 0xffff003c
	.4byte 0x0200a5c9
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200a46d
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200a4b1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEventsOther
gBabiIriguchiEventsOther:
	.4byte 0x00000202
	.4byte 0xffff0032
	.4byte 0x0200a6fd
	.4byte 0x00000202
	.4byte 0xffff0033
	.4byte 0x0200a739
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x0200a6b9
	.4byte 0x10009315
	.4byte 0xffff000c
	.4byte 0x0200a6a9
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte 0x0200a6b9
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000026b7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000026b8
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000026b9
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000026ba
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000026c5
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000026c6
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000026c7
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000026c8
	.4byte 0x00000413
	.4byte 0x0fbc0064
	.4byte 0x001000c4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
