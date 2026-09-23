@ Thumb entry of the per-channel sound mixing in TLA's resident runtime,
@ entered at 0x030007b5 by SOUND/CHANNEL_UPDATE_ENTRY.S. It tests channel
@ flags with tst and branches on flags left by lsls (lsls r0, r6, #25;
@ bcc; bmi), then switches to ARM inside the routine (add r1, pc, #8;
@ bx r1) into IWRAM_RUNTIME_MIX_LANES.S. Neither approved compiler emits
@ a Thumb tst or a state change inside a function. Between its literal
@ 0x03006fc0 (ldr r5, [pc, #8]) and the ARM code that bx r1 enters
@ (0x08000e38) sits a zero word that nothing reads or runs; filler carries
@ no credit.
@ credit: handwritten — TLA IWRAM runtime sound mixer entry
.syntax unified
	.thumb_func
	.set	.L_03000cfc, 0x03000cfc
	.arm
	.set AlchemyLoadAddress, 0x08000d6c
	.set AlchemyRunAddress, 0x030007b4
	.thumb
.L_030007b4:
	str	r0, [sp, #4]
	ldr	r3, [r4, #36]
	ldrb	r6, [r4, #0]
	movs	r0, #199
	tst	r0, r6
	beq.n	.L_030007f6
	lsls	r0, r6, #25
	bcc.n	.L_030007e4
	bmi.n	.L_030007f2
	movs	r6, #3
	adds	r0, r3, #0
	adds	r0, #16
	str	r0, [r4, #40]
	ldr	r0, [r3, #12]
	str	r0, [r4, #24]
	movs	r5, #0
	strb	r5, [r4, #9]
	str	r5, [r4, #28]
	ldrb	r2, [r3, #3]
	lsrs	r0, r2, #6
	beq.n	.L_03000836
	movs	r0, #16
	orrs	r6, r0
	b.n	.L_03000836
.L_030007e4:
	ldrb	r5, [r4, #9]
	lsls	r0, r6, #29
	bpl.n	.L_030007f8
	ldrb	r0, [r4, #13]
	subs	r0, #1
	strb	r0, [r4, #13]
	bhi.n	.L_03000844
.L_030007f2:
	movs	r0, #0
	strb	r0, [r4, #0]
.L_030007f6:
	b.n	.L_03000cfc
.L_030007f8:
	lsls	r0, r6, #25
	bpl.n	.L_03000818
	ldrb	r0, [r4, #7]
	subs	r0, #255
	subs	r0, #1
	adds	r5, r5, r0
	bmi.n	.L_0300080c
	ldrb	r0, [r4, #12]
	cmp	r5, r0
	bhi.n	.L_03000844
.L_0300080c:
	ldrb	r5, [r4, #12]
	cmp	r5, #0
	beq.n	.L_030007f2
	movs	r0, #4
	orrs	r6, r0
	b.n	.L_03000842
.L_03000818:
	movs	r2, #3
	ands	r2, r6
	cmp	r2, #2
	bne.n	.L_03000832
	ldrb	r0, [r4, #5]
	muls	r5, r0
	lsrs	r5, r5, #8
	ldrb	r0, [r4, #6]
	cmp	r5, r0
	bhi.n	.L_03000844
	adds	r5, r0, #0
	beq.n	.L_0300080c
	b.n	.L_03000840
.L_03000832:
	cmp	r2, #3
	bne.n	.L_03000844
.L_03000836:
	ldrb	r0, [r4, #4]
	adds	r5, r5, r0
	cmp	r5, #255
	bcc.n	.L_03000842
	movs	r5, #255
.L_03000840:
	subs	r6, #1
.L_03000842:
	strb	r6, [r4, #0]
.L_03000844:
	strb	r5, [r4, #9]
	lsrs	r0, r5, #3
	adds	r5, r5, r0
	ldrb	r0, [r4, #2]
	muls	r0, r5
	lsrs	r0, r0, #9
	mov	sl, r0
	ldrb	r0, [r4, #3]
	muls	r0, r5
	lsrs	r0, r0, #9
	mov	fp, r0
	movs	r0, #16
	ands	r0, r6
	beq.n	.L_0300086a
	adds	r3, #8
	ldmia	r3!, {r0, r1}
	adds	r3, r0, r3
	str	r3, [sp, #12]
	subs	r0, r1, r0
.L_0300086a:
	str	r0, [sp, #16]
	ldr	r5, [pc, #8]
	ldr	r2, [r4, #24]
	ldr	r3, [r4, #40]
	ldrb	r0, [r4, #1]
	add	r1, pc, #8
	bx	r1
	.4byte	0x03006fc0
AlchemyUncredited_08000e34:
	.4byte	0x00000000
AlchemyUncreditedEnd_08000e34:
