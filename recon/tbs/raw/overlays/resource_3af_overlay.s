.syntax unified
	.thumb
	.section .text.x020080c4,"ax",%progbits
	.p2align 2
	.global FuneKanpan_UpdateHoverGullA
	.thumb_func
FuneKanpan_UpdateHoverGullA:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r6, r5, #0
	adds	r6, #98
	ldrb	r1, [r6, #0]
	mov	r8, r1
	cmp	r1, #0
	bne.n	.L_020000da
	b.n	.L_0200021a
.L_020000da:
	adds	r3, r1, #0
	subs	r3, #1
	cmp	r3, #7
	bls.n	.L_020000e4
	b.n	.L_020002b2
.L_020000e4:
	ldr	r2, [pc, #488]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200810c
	.4byte 0x020081d4
	.4byte 0x0200812c
	.4byte 0x020081d4
	.4byte 0x020081aa
	.4byte 0x020081d4
	.4byte 0x020081dc
	.4byte 0x02008214
	.4byte 0x02db2380
	.4byte 0x2380632b
	.4byte 0x636b029b
	.4byte 0x22a02186
	.4byte 0x049b23ad
	.4byte 0x04491c28
	.4byte 0xf0040352
	.4byte 0xe053f8b1
	.4byte 0x6bab2280
	.4byte 0x42930612
	.4byte 0xe0bcd000
	.4byte 0x429a6bea
	.4byte 0xe0b8d000
	.4byte 0x42936c2b
	.4byte 0xe0b4d000
	.4byte 0x20927833
	.4byte 0x70333301
	.4byte 0xf984f004
	.4byte 0x33631c2b
	.4byte 0x2b00781b
	.4byte 0x21d0d006
	.4byte 0x02092015
	.4byte 0xf0042200
	.4byte 0xe005f935
	.4byte 0x201521b0
	.4byte 0x22000209
	.4byte 0xf92ef004
	.4byte 0xf878f004
	.4byte 0x0c000080
	.4byte 0xd0062800
	.4byte 0xf0042015
	.4byte 0x2380f8c1
	.4byte 0x6283029b
	.4byte 0x2015e08f
	.4byte 0x2200494f
	.4byte 0xf928f004
	.4byte 0xf0042015
	.4byte 0x23c0f8b5
	.4byte 0x628302db
	.4byte 0x1c2be083
	.4byte 0x781b3363
	.4byte 0xd0072b00
	.4byte 0x1c28218d
	.4byte 0x22000449
	.4byte 0xf0044b46
	.4byte 0xe007f865
	.4byte 0x23a721fe
	.4byte 0x04091c28
	.4byte 0x049b2200
	.4byte 0xf85cf004
	.4byte 0x33017833
	.4byte 0xe06a7033
	.4byte 0x6bab2180
	.4byte 0x428b0609
	.4byte 0x6bead165
	.4byte 0xd162429a
	.4byte 0x42936c2b
	.4byte 0x2380d15f
	.4byte 0x632b029b
	.4byte 0x025b2380
	.4byte 0x1c2b636b
	.4byte 0x33642200
	.4byte 0x3302801a
	.4byte 0x7833801a
	.4byte 0x70333301
	.4byte 0xe04e64ea
	.4byte 0x70332300
	.2byte 0xe04b
.L_0200021a:
	adds	r7, r5, #0
	adds	r7, #100
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_02000240
	bl 0x0200c26c
	ldr	r3, [r5, #76]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	ldr	r1, [pc, #168]
	subs	r3, r3, r0
	str	r3, [r5, #76]
	cmp	r3, r1
	bge.n	.L_0200025a
	mov	r2, r8
	strh	r2, [r7, #0]
	b.n	.L_0200025a
.L_02000240:
	bl 0x0200c26c
	ldr	r3, [r5, #76]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	movs	r1, #128
	adds	r3, r3, r0
	lsls	r1, r1, #7
	str	r3, [r5, #76]
	cmp	r3, r1
	ble.n	.L_0200025a
	movs	r3, #1
	strh	r3, [r7, #0]
.L_0200025a:
	ldr	r1, [pc, #132]
	ldr	r2, [r5, #8]
	adds	r3, r2, r1
	ldr	r1, [pc, #128]
	cmp	r3, r1
	bhi.n	.L_0200026c
	ldr	r3, [r5, #76]
	adds	r3, r2, r3
	str	r3, [r5, #8]
.L_0200026c:
	adds	r7, r5, #0
	adds	r7, #102
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_02000292
	bl 0x0200c26c
	ldr	r3, [r5, #12]
	lsls	r0, r0, #15
	lsrs	r0, r0, #16
	ldr	r1, [pc, #100]
	subs	r3, r3, r0
	adds	r3, r3, r1
	str	r3, [r5, #12]
	cmp	r3, #0
	bge.n	.L_020002b2
	movs	r3, #0
	b.n	.L_020002b0
.L_02000292:
	bl 0x0200c26c
	ldr	r3, [r5, #12]
	lsls	r0, r0, #15
	lsrs	r0, r0, #16
	movs	r2, #128
	adds	r3, r3, r0
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r3, r3, r2
	lsls	r1, r1, #12
	str	r3, [r5, #12]
	cmp	r3, r1
	ble.n	.L_020002b2
	movs	r3, #1
.L_020002b0:
	strh	r3, [r7, #0]
.L_020002b2:
	bl 0x0200c26c
	movs	r3, #100
	muls	r3, r0
	lsrs	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_020002c4
	movs	r3, #1
	strb	r3, [r6, #0]
.L_020002c4:
	movs	r0, #1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x020080ec
	.4byte 0x00000103
	.4byte 0x02920000
	.4byte 0xffffc000
	.4byte 0xff07ffff
	.4byte 0x002bfffe
	.2byte 0x8000
	.2byte 0xffff
	.section .text.x020082ec,"ax",%progbits
	.global FuneKanpan_UpdateHoverGullB
	.thumb_func
FuneKanpan_UpdateHoverGullB:
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r6, r5, #0
	adds	r6, #98
	ldrb	r1, [r6, #0]
	mov	r8, r1
	cmp	r1, #0
	bne.n	.L_02000302
	b.n	.L_02000444
.L_02000302:
	adds	r3, r1, #0
	subs	r3, #1
	cmp	r3, #7
	bls.n	.L_0200030c
	b.n	.L_020004dc
.L_0200030c:
	ldr	r2, [pc, #492]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x02008334
	.4byte 0x020083fe
	.4byte 0x02008354
	.4byte 0x020083fe
	.4byte 0x020083d2
	.4byte 0x020083fe
	.4byte 0x02008406
	.4byte 0x0200843e
	.4byte 0x02db2380
	.4byte 0x2380632b
	.4byte 0x636b029b
	.4byte 0x22a02180
	.4byte 0x049b23a0
	.4byte 0x04491c28
	.4byte 0xf0030352
	.4byte 0xe054ff9d
	.4byte 0x6bab2280
	.4byte 0x42930612
	.4byte 0xe0bdd000
	.4byte 0x429a6bea
	.4byte 0xe0b9d000
	.4byte 0x42936c2b
	.4byte 0xe0b5d000
	.4byte 0x20927833
	.4byte 0x70333301
	.4byte 0xf870f004
	.4byte 0x33631c2b
	.4byte 0x2b00781b
	.4byte 0x21d0d006
	.4byte 0x02092016
	.4byte 0xf0042200
	.4byte 0xe005f821
	.4byte 0x201621b0
	.4byte 0x22000209
	.4byte 0xf81af004
	.4byte 0xff64f003
	.4byte 0x0c000080
	.4byte 0xd0062800
	.4byte 0xf0032016
	.4byte 0x2380ffad
	.4byte 0x6283029b
	.4byte 0x2016e090
	.4byte 0x22004950
	.4byte 0xf814f004
	.4byte 0xf0032016
	.4byte 0x23c0ffa1
	.4byte 0x628302db
	.4byte 0x1c2be084
	.4byte 0x781b3363
	.4byte 0xd0082b00
	.4byte 0x23962184
	.4byte 0x04491c28
	.4byte 0x049b2200
	.4byte 0xff50f003
	.4byte 0x21f2e007
	.4byte 0x1c282397
	.4byte 0x22000409
	.4byte 0xf003049b
	.4byte 0x7833ff47
	.4byte 0x70333301
	.4byte 0x2180e06a
	.4byte 0x06096bab
	.4byte 0xd165428b
	.4byte 0x429a6bea
	.4byte 0x6c2bd162
	.4byte 0xd15f4293
	.4byte 0x029b2380
	.4byte 0x2380632b
	.4byte 0x636b025b
	.4byte 0x22001c2b
	.4byte 0x801a3364
	.4byte 0x801a3302
	.4byte 0x33017833
	.4byte 0x64ea7033
	.4byte 0x2300e04e
	.2byte 0x7033
	.2byte 0xe04b
.L_02000444:
	adds	r7, r5, #0
	adds	r7, #100
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_0200046a
	bl 0x0200c26c
	ldr	r3, [r5, #76]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	ldr	r1, [pc, #168]
	subs	r3, r3, r0
	str	r3, [r5, #76]
	cmp	r3, r1
	bge.n	.L_02000484
	mov	r2, r8
	strh	r2, [r7, #0]
	b.n	.L_02000484
.L_0200046a:
	bl 0x0200c26c
	ldr	r3, [r5, #76]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	movs	r1, #128
	adds	r3, r3, r0
	lsls	r1, r1, #7
	str	r3, [r5, #76]
	cmp	r3, r1
	ble.n	.L_02000484
	movs	r3, #1
	strh	r3, [r7, #0]
.L_02000484:
	ldr	r1, [pc, #128]
	ldr	r2, [r5, #8]
	adds	r3, r2, r1
	ldr	r1, [pc, #128]
	cmp	r3, r1
	bhi.n	.L_02000496
	ldr	r3, [r5, #76]
	adds	r3, r2, r3
	str	r3, [r5, #8]
.L_02000496:
	adds	r7, r5, #0
	adds	r7, #102
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_020004bc
	bl 0x0200c26c
	ldr	r3, [r5, #12]
	lsls	r0, r0, #15
	lsrs	r0, r0, #16
	ldr	r1, [pc, #96]
	subs	r3, r3, r0
	adds	r3, r3, r1
	str	r3, [r5, #12]
	cmp	r3, #0
	bge.n	.L_020004dc
	movs	r3, #0
	b.n	.L_020004da
.L_020004bc:
	bl 0x0200c26c
	ldr	r3, [r5, #12]
	lsls	r0, r0, #15
	lsrs	r0, r0, #16
	movs	r2, #128
	adds	r3, r3, r0
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r3, r3, r2
	lsls	r1, r1, #12
	str	r3, [r5, #12]
	cmp	r3, r1
	ble.n	.L_020004dc
	movs	r3, #1
.L_020004da:
	strh	r3, [r7, #0]
.L_020004dc:
	bl 0x0200c26c
	movs	r3, #100
	muls	r3, r0
	lsrs	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_020004ee
	movs	r3, #1
	strb	r3, [r6, #0]
.L_020004ee:
	movs	r0, #1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x02008314
	.4byte 0x00000103
	.4byte 0xffffc000
	.4byte 0xff17ffff
	.4byte 0x0027fffe
	.2byte 0x8000
	.2byte 0xffff
	.section .rodata,"a",%progbits
	.global FuneKanpan_PresentationActionsA
FuneKanpan_PresentationActionsA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00ca0000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global FuneKanpan_PresentationActionsB
FuneKanpan_PresentationActionsB:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a20000
	.4byte 0x00000000
	.4byte 0x029a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000010
	.global FuneKanpan_RandomActorActions
FuneKanpan_RandomActorActions:
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global FuneKanpan_DeckEventActions
FuneKanpan_DeckEventActions:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x008c0000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00000001
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02560000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000022
	.4byte 0x020080b5
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02540000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global FuneKanpan_LeadActionsA
FuneKanpan_LeadActionsA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00e40000
	.4byte 0x00000000
	.4byte 0x02090000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x02090000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global FuneKanpan_LeadActionsB
FuneKanpan_LeadActionsB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global FuneKanpan_CrewActionsA
FuneKanpan_CrewActionsA:
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000017
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte FuneKanpan_UpdateHoverGullA
	.global FuneKanpan_CrewActionsB
FuneKanpan_CrewActionsB:
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000017
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte FuneKanpan_UpdateHoverGullB
	.global FuneKanpan_CrewActionsC
FuneKanpan_CrewActionsC:
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008515
	.global FuneKanpan_CrewActionsD
FuneKanpan_CrewActionsD:
	.4byte 0x00000022
	.4byte 0x02008571
	.global FuneKanpan_CrewActionsE
FuneKanpan_CrewActionsE:
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000b333
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000b333
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000f5c
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000147a
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global FuneKanpan_SailorActions
FuneKanpan_SailorActions:
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global FuneKanpan_DeckActionsB
FuneKanpan_DeckActionsB:
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008599
	.global FuneKanpan_DeckActionsA
FuneKanpan_DeckActionsA:
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008725
	.global FuneKanpan_DeckActionsC
FuneKanpan_DeckActionsC:
	.4byte 0x00000022
	.4byte 0x0200889d
	.global FuneKanpan_RosterActions
FuneKanpan_RosterActions:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00520000
	.4byte 0x00000000
	.4byte 0x02900000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global FuneKanpan_LeaveActions
FuneKanpan_LeaveActions:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00ba0000
	.4byte 0x00000000
	.4byte 0x025c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x025c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global FuneKanpan_LeadActionsC
FuneKanpan_LeadActionsC:
	.4byte 0x00000015
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x020088e1
	.global FuneKanpan_SceneTableA
FuneKanpan_SceneTableA:
	.4byte 0xffff0000
	.4byte 0x000000b3
	.4byte 0x4000028d
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000d8
	.4byte 0x40000258
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000000e8
	.4byte 0x00000298
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000000d8
	.4byte 0x40000328
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000400d8
	.4byte 0x40000288
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0xfffc0138
	.4byte 0x80000290
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0xfffc0078
	.4byte 0x00000290
	.4byte 0x00410000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x000000e8
	.4byte 0x00000298
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x000000d4
	.4byte 0x00000209
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x000000c0
	.4byte 0x0000028a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x000000e8
	.4byte 0x0000028e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000e
	.4byte 0x000000d8
	.4byte 0x00000291
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000f
	.4byte 0x000000d8
	.4byte 0x40000280
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0010
	.4byte 0x000000d8
	.4byte 0xc000021c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0011
	.4byte 0x000000d8
	.4byte 0xc000021c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0012
	.4byte 0x000000c4
	.4byte 0x4000028c
	.4byte 0x00410000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0013
	.4byte 0x000000d4
	.4byte 0x40000209
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_SceneTableB
FuneKanpan_SceneTableB:
	.4byte 0x001800d0
	.4byte 0x00e0027a
	.4byte 0x028a0028
	.4byte 0x0005ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_SceneTableC
FuneKanpan_SceneTableC:
	.4byte 0x0000006d
	.4byte 0x0010106f
	.4byte 0x0020306f
	.4byte 0x0030506f
	.4byte 0x0040206b
	.4byte 0x0050106e
	.4byte 0x00601070
	.4byte 0x0074a002
	.4byte 0x00a0a06e
	.4byte 0x00b0e06f
	.4byte 0x00c0f06f
	.4byte 0x00d1106f
	.4byte 0x00e1306f
	.4byte 0x00f46002
	.4byte 0x0101606f
	.4byte 0x0111806f
	.4byte 0x000001ff
	.global gFuneKanpanPlacements
gFuneKanpanPlacements:
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x010a0000
	.4byte 0x00000000
	.4byte 0x02bf0000
	.4byte 0x00010000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanPlacementsFlag911
gFuneKanpanPlacementsFlag911:
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x00b70000
	.4byte 0x00000000
	.4byte 0x02d00000
	.4byte 0x0001d000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x0002b000
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x00c60000
	.4byte 0x00000000
	.4byte 0x02c20000
	.4byte 0x00015000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00ac0000
	.4byte 0x00000000
	.4byte 0x02c20000
	.4byte 0x00000000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00023000
	.4byte 0xffff0084
	.4byte 0x00000001
	.4byte 0x00e60000
	.4byte 0x00000000
	.4byte 0x025a0000
	.4byte 0x00003000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00ec0000
	.4byte 0x00000000
	.4byte 0x026d0000
	.4byte 0x0000b000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x022c0000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanPlacementsFlag927
gFuneKanpanPlacementsFlag927:
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02910000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02910000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02910000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02910000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanPlacementsFlag928
gFuneKanpanPlacementsFlag928:
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_CrewScript
FuneKanpan_CrewScript:
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x0002b000
	.4byte 0xffff0043
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x027a0000
	.4byte 0x0000b000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x02090000
	.4byte 0x0000b000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000d000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_ClosingCrewScript
FuneKanpan_ClosingCrewScript:
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00025000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_CrewScriptB
FuneKanpan_CrewScriptB:
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_CrewScriptC
FuneKanpan_CrewScriptC:
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c6
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00d6
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
	.global FuneKanpan_CrewScriptD
FuneKanpan_CrewScriptD:
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00c5
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
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
	.global FuneKanpan_CrewScriptE
FuneKanpan_CrewScriptE:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002b000
	.4byte 0xffff0043
	.4byte 0x00000001
	.4byte 0x00b20000
	.4byte 0x00000000
	.4byte 0x02820000
	.4byte 0x0000b000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x02160000
	.4byte 0x0000b000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000d000
	.4byte 0xffff00c2
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanPlacementsFlag93e
gFuneKanpanPlacementsFlag93e:
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00bc0000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00003000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x00a20000
	.4byte 0x00000000
	.4byte 0x027a0000
	.4byte 0x00013000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00a20000
	.4byte 0x00000000
	.4byte 0x02a40000
	.4byte 0x0001d000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0084
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanEvents
gFuneKanpanEvents:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008c29
	.4byte 0x00008602
	.4byte 0xffff0002
	.4byte 0x02008c29
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008c29
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x09230014
	.4byte 0x00001d24
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001d8f
	.4byte 0x00008d15
	.4byte 0x09230014
	.4byte 0x00001d25
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001d90
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001d4a
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008a49
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008af1
	.4byte 0x00000000
	.4byte 0x09220019
	.4byte 0x00001d38
	.4byte 0x00000000
	.4byte 0x09250019
	.4byte 0x00001d71
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001e0a
	.4byte 0x00000000
	.4byte 0x0922001a
	.4byte 0x00001d39
	.4byte 0x00000000
	.4byte 0x0925001a
	.4byte 0x00001d72
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00001e0b
	.4byte 0x00000000
	.4byte 0x0922001b
	.4byte 0x00001d3a
	.4byte 0x00000000
	.4byte 0x0925001b
	.4byte 0x00001d73
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00001e0c
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001d4c
	.4byte 0x00008d15
	.4byte 0x09220015
	.4byte 0x00001d3b
	.4byte 0x00008d15
	.4byte 0x09250015
	.4byte 0x00001d74
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001e0d
	.4byte 0x00008d15
	.4byte 0x09220018
	.4byte 0x00001d3c
	.4byte 0x00008d15
	.4byte 0x09250018
	.4byte 0x00001d75
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001e0e
	.4byte 0x00008d15
	.4byte 0x09220019
	.4byte 0x00001d3d
	.4byte 0x00008d15
	.4byte 0x09250019
	.4byte 0x00001d76
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001e0f
	.4byte 0x00008d15
	.4byte 0x0922001a
	.4byte 0x00001d3e
	.4byte 0x00008d15
	.4byte 0x0925001a
	.4byte 0x00001d77
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001e10
	.4byte 0x00008d15
	.4byte 0x0922001b
	.4byte 0x00001d3f
	.4byte 0x00008d15
	.4byte 0x0925001b
	.4byte 0x00001d78
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00001e11
	.4byte 0x00000002
	.4byte 0x0920000a
	.4byte 0x02008ca1
	.4byte 0x00000002
	.4byte 0x0923000b
	.4byte 0x020090a1
	.4byte 0x00000002
	.4byte 0x0923000c
	.4byte 0x020091c9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanEventsFlag928
gFuneKanpanEventsFlag928:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008c29
	.4byte 0x00008602
	.4byte 0xffff0002
	.4byte 0x02008c29
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008c29
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001e77
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001eb0
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001eb1
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001e94
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001eb4
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001eb5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanEventsFlag93e
gFuneKanpanEventsFlag93e:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008c29
	.4byte 0x00008602
	.4byte 0xffff0002
	.4byte 0x02008c29
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008c29
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001eff
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008b99
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001f03
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001f04
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001f05
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001f06
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanEventsFlag8a0
gFuneKanpanEventsFlag8a0:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008c29
	.4byte 0x00008602
	.4byte 0xffff0002
	.4byte 0x02008c29
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008c29
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001f54
	.4byte 0x00000000
	.4byte 0x09030016
	.4byte 0x0200bc89
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001f5c
	.4byte 0x00000000
	.4byte 0x09030015
	.4byte 0x0200bc89
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001f5d
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001f5e
	.4byte 0x00008d15
	.4byte 0x09030016
	.4byte 0x00001f5f
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001f61
	.4byte 0x00008d15
	.4byte 0x09030015
	.4byte 0x00001f60
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001f62
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_FlagGroupEntries
FuneKanpan_FlagGroupEntries:
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
