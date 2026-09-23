@ Sound sequence command interpreter: mov ip, lr frames, flag branches on
@ lsrs with no compare, and a private continuation entered by bl and by
@ branches from other entries. These 452 bytes equal TBS
@ SOUND/DRIVER/SEQUENCE_COMMANDS.S (0x080f9a80) apart from one relocated
@ literal word, the command table address.
@ The zero halfwords of alignment at 0x081c1732, 0x081c175a, 0x081c1796,
@ 0x081c17da, 0x081c17e6, 0x081c180e, 0x081c183e, 0x081c1852, 0x081c188e and
@ 0x081c189a never run and carry no credit.
@ credit: library — TBS/TLA common sound sequence command interpreter
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081c171c
	.global MusicTrack_SequenceCommands
	.thumb_func
MusicTrack_SequenceCommands:
	mov	ip, lr
	movs	r1, #36
	ldr	r2, [pc, #40]
.L_081c1722:
	ldr	r3, [r2, #0]
	bl	.L_081c1736
	stmia	r0!, {r3}
	adds	r2, #4
	subs	r1, #1
	bgt.n	.L_081c1722
	bx	ip
AlchemyUncredited_081c1732:
	.2byte	0x0000
.L_081c1734:
AlchemyUncreditedEnd_081c1732:
	ldrb	r3, [r2, #0]
.L_081c1736:
	push	{r0}
	lsrs	r0, r2, #25
	bne.n	.L_081c1748
	ldr	r0, [pc, #12]
	cmp	r2, r0
	bcc.n	.L_081c1746
	lsrs	r0, r2, #14
	beq.n	.L_081c1748
.L_081c1746:
	movs	r3, #0
.L_081c1748:
	pop	{r0}
	bx	lr
	.4byte	0x081c343c
.L_081c1750:
	ldr	r2, [r1, #64]
.L_081c1752:
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	b.n	.L_081c1736
AlchemyUncredited_081c175a:
	.2byte	0x0000
.L_081c175c:
AlchemyUncreditedEnd_081c175a:
	push	{lr}
.L_081c175e:
	ldr	r2, [r1, #64]
	ldrb	r0, [r2, #3]
	lsls	r0, r0, #8
	ldrb	r3, [r2, #2]
	orrs	r0, r3
	lsls	r0, r0, #8
	ldrb	r3, [r2, #1]
	orrs	r0, r3
	lsls	r0, r0, #8
	bl	.L_081c1734
	orrs	r0, r3
	str	r0, [r1, #64]
	pop	{r0}
	bx	r0
	ldrb	r2, [r1, #2]
	cmp	r2, #3
	bcs.n	.L_081c1794
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r2, [r1, #64]
	adds	r2, #4
	str	r2, [r3, #68]
	ldrb	r2, [r1, #2]
	adds	r2, #1
	strb	r2, [r1, #2]
	b.n	.L_081c175c
.L_081c1794:
	b.n	Func_081c16ec
AlchemyUncredited_081c1796:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1796:
	ldrb	r2, [r1, #2]
	cmp	r2, #0
	beq.n	.L_081c17aa
	subs	r2, #1
	strb	r2, [r1, #2]
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r2, [r3, #68]
	str	r2, [r1, #64]
.L_081c17aa:
	bx	lr
	push	{lr}
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_081c17bc
	adds	r2, #1
	str	r2, [r1, #64]
	b.n	.L_081c175e
.L_081c17bc:
	ldrb	r3, [r1, #3]
	adds	r3, #1
	strb	r3, [r1, #3]
	mov	ip, r3
	bl	.L_081c1750
	cmp	ip, r3
	bcs.n	.L_081c17ce
	b.n	.L_081c175e
.L_081c17ce:
	movs	r3, #0
	strb	r3, [r1, #3]
	adds	r2, #5
	str	r2, [r1, #64]
	pop	{r0}
	bx	r0
AlchemyUncredited_081c17da:
	.2byte	0x0000
AlchemyUncreditedEnd_081c17da:
	mov	ip, lr
	bl	.L_081c1750
	strb	r3, [r1, #29]
	bx	ip
AlchemyUncredited_081c17e6:
	.2byte	0x0000
AlchemyUncreditedEnd_081c17e6:
	mov	ip, lr
	bl	.L_081c1750
	lsls	r3, r3, #1
	strh	r3, [r0, #28]
	ldrh	r2, [r0, #30]
	muls	r3, r2
	lsrs	r3, r3, #8
	strh	r3, [r0, #32]
	bx	ip
	mov	ip, lr
	bl	.L_081c1750
	strb	r3, [r1, #10]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
AlchemyUncredited_081c180e:
	.2byte	0x0000
AlchemyUncreditedEnd_081c180e:
	mov	ip, lr
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	adds	r2, #1
	str	r2, [r1, #64]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #2
	ldr	r3, [r0, #48]
	adds	r2, r2, r3
	ldr	r3, [r2, #0]
	bl	.L_081c1736
	str	r3, [r1, #36]
	ldr	r3, [r2, #4]
	bl	.L_081c1736
	str	r3, [r1, #40]
	ldr	r3, [r2, #8]
	bl	.L_081c1736
	str	r3, [r1, #44]
	bx	ip
AlchemyUncredited_081c183e:
	.2byte	0x0000
AlchemyUncreditedEnd_081c183e:
	mov	ip, lr
	bl	.L_081c1750
	strb	r3, [r1, #18]
	ldrb	r3, [r1, #0]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
AlchemyUncredited_081c1852:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1852:
	mov	ip, lr
	bl	.L_081c1750
	subs	r3, #64
	strb	r3, [r1, #20]
	ldrb	r3, [r1, #0]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
	mov	ip, lr
	bl	.L_081c1750
	subs	r3, #64
	strb	r3, [r1, #14]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
	mov	ip, lr
	bl	.L_081c1750
	strb	r3, [r1, #15]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
AlchemyUncredited_081c188e:
	.2byte	0x0000
AlchemyUncreditedEnd_081c188e:
	mov	ip, lr
	bl	.L_081c1750
	strb	r3, [r1, #27]
	bx	ip
AlchemyUncredited_081c189a:
	.2byte	0x0000
AlchemyUncreditedEnd_081c189a:
	mov	ip, lr
	bl	.L_081c1750
	ldrb	r0, [r1, #24]
	cmp	r0, r3
	beq.n	.L_081c18b2
	strb	r3, [r1, #24]
	ldrb	r3, [r1, #0]
	movs	r2, #15
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_081c18b2:
	bx	ip
	mov	ip, lr
	bl	.L_081c1750
	subs	r3, #64
	strb	r3, [r1, #12]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
	mov	ip, lr
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	adds	r2, #1
	ldr	r0, [pc, #8]
	adds	r0, r0, r3
	bl	.L_081c1752
	strb	r3, [r0, #0]
	bx	ip
	.4byte	0x04000060
