@ IWRAM sound mixer. The sound setup copies it with Bios_CpuSet (literal pool
@ at 0x081c1f90: source 0x081c1311 with bit 0 cleared, destination 0x03006000,
@ control 0x04000100 = 256 words), so 1,024 bytes reach IWRAM: these 932 bytes
@ and the 92 after them. It switches between Thumb and ARM inside the routine
@ (add r1, pc, #4; bx r1), saves r4 and ip without lr and ends by unwinding
@ the frame MIXER_ENTRY.S built, with one pop {r0-r7}; neither approved
@ compiler emits any of these. The code is position-independent: these bytes
@ equal TBS SOUND/DRIVER/MIXER.S (0x080f9674), which runs at 0x03007000.
@ The zero halfwords of alignment at 0x081c131a, 0x081c13c2 and 0x081c14a6
@ never run and carry no credit.
@ credit: library — TBS/TLA common sound-driver mixer and clear-buffer module (mixer)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081c1310
	.set AlchemyRunAddress, 0x03006000
	.global SoundDriver_Mixer
	.thumb_func
SoundDriver_Mixer:
	ldrb	r3, [r0, #5]
	cmp	r3, #0
	beq .L_03006060
	add	r1, pc, #4
	bx	r1
AlchemyUncredited_081c131a:
	.arm
AlchemyUncreditedEnd_081c131a:
	cmp	r4, #2
	addeq	r7, r0, #848
	addne	r7, r5, r8
	mov	r4, r8
.L_0300601c:
	ldrsb	r0, [r5, r6]
	ldrsb	r1, [r5]
	add	r0, r0, r1
	ldrsb	r1, [r7, r6]
	add	r0, r0, r1
	ldrsb	r1, [r7], #1
	add	r0, r0, r1
	mul	r1, r0, r3
	asr	r0, r1, #9
	tst	r0, #128
	addne	r0, r0, #1
	strb	r0, [r5, r6]
	strb	r0, [r5], #1
	subs	r4, r4, #1
	bgt .L_0300601c
	add	r0, pc, #47
	bx	r0
	.thumb
.L_03006060:
	movs	r0, #0
	mov	r1, r8
	adds	r6, r6, r5
	lsrs	r1, r1, #3
	bcc .L_0300606e
	stmia	r5!, {r0}
	stmia	r6!, {r0}
.L_0300606e:
	lsrs	r1, r1, #1
	bcc .L_0300607a
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	stmia	r5!, {r0}
	stmia	r6!, {r0}
.L_0300607a:
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	subs	r1, #1
	bgt .L_0300607a
	ldr	r4, [sp, #24]
	ldr	r0, [r4, #24]
	mov	ip, r0
	ldrb	r0, [r4, #6]
	adds	r4, #80
.L_03006098:
	str	r0, [sp, #4]
	ldr	r3, [r4, #36]
	ldr	r0, [sp, #20]
	cmp	r0, #0
	beq .L_030060b8
	ldr	r1, [pc, #16]
	ldrb	r1, [r1, #0]
	cmp	r1, #160
	bcs .L_030060ac
	adds	r1, #228
.L_030060ac:
	cmp	r1, r0
	bcc .L_030060b8
	b .L_0300638a
AlchemyUncredited_081c13c2:
	.2byte	0x0000
AlchemyUncreditedEnd_081c13c2:
	.4byte	0x04000006
.L_030060b8:
	ldrb	r6, [r4, #0]
	movs	r0, #199
	tst	r0, r6
	bne .L_030060c2
	b .L_03006380
.L_030060c2:
	movs	r0, #128
	tst	r0, r6
	beq .L_030060f2
	movs	r0, #64
	tst	r0, r6
	bne .L_03006102
	movs	r6, #3
	strb	r6, [r4, #0]
	adds	r0, r3, #0
	adds	r0, #16
	str	r0, [r4, #40]
	ldr	r0, [r3, #12]
	str	r0, [r4, #24]
	movs	r5, #0
	strb	r5, [r4, #9]
	str	r5, [r4, #28]
	ldrb	r2, [r3, #3]
	movs	r0, #192
	tst	r0, r2
	beq .L_0300614a
	movs	r0, #16
	orrs	r6, r0
	strb	r6, [r4, #0]
	b .L_0300614a
.L_030060f2:
	ldrb	r5, [r4, #9]
	movs	r0, #4
	tst	r0, r6
	beq .L_03006108
	ldrb	r0, [r4, #13]
	subs	r0, #1
	strb	r0, [r4, #13]
	bhi .L_03006158
.L_03006102:
	movs	r0, #0
	strb	r0, [r4, #0]
	b .L_03006380
.L_03006108:
	movs	r0, #64
	tst	r0, r6
	beq .L_03006128
	ldrb	r0, [r4, #7]
	muls	r5, r0
	lsrs	r5, r5, #8
	ldrb	r0, [r4, #12]
	cmp	r5, r0
	bhi .L_03006158
.L_0300611a:
	ldrb	r5, [r4, #12]
	cmp	r5, #0
	beq .L_03006102
	movs	r0, #4
	orrs	r6, r0
	strb	r6, [r4, #0]
	b .L_03006158
.L_03006128:
	movs	r2, #3
	ands	r2, r6
	cmp	r2, #2
	bne .L_03006146
	ldrb	r0, [r4, #5]
	muls	r5, r0
	lsrs	r5, r5, #8
	ldrb	r0, [r4, #6]
	cmp	r5, r0
	bhi .L_03006158
	adds	r5, r0, #0
	beq .L_0300611a
	subs	r6, #1
	strb	r6, [r4, #0]
	b .L_03006158
.L_03006146:
	cmp	r2, #3
	bne .L_03006158
.L_0300614a:
	ldrb	r0, [r4, #4]
	adds	r5, r5, r0
	cmp	r5, #255
	bcc .L_03006158
	movs	r5, #255
	subs	r6, #1
	strb	r6, [r4, #0]
.L_03006158:
	strb	r5, [r4, #9]
	ldr	r0, [sp, #24]
	ldrb	r0, [r0, #7]
	adds	r0, #1
	muls	r0, r5
	lsrs	r5, r0, #4
	ldrb	r0, [r4, #2]
	muls	r0, r5
	lsrs	r0, r0, #8
	strb	r0, [r4, #10]
	ldrb	r0, [r4, #3]
	muls	r0, r5
	lsrs	r0, r0, #8
	strb	r0, [r4, #11]
	movs	r0, #16
	ands	r0, r6
	str	r0, [sp, #16]
	beq .L_0300618c
	adds	r0, r3, #0
	adds	r0, #16
	ldr	r1, [r3, #8]
	adds	r0, r0, r1
	str	r0, [sp, #12]
	ldr	r0, [r3, #12]
	subs	r0, r0, r1
	str	r0, [sp, #16]
.L_0300618c:
	ldr	r5, [sp, #8]
	ldr	r2, [r4, #24]
	ldr	r3, [r4, #40]
	add	r0, pc, #4
	bx	r0
AlchemyUncredited_081c14a6:
	.arm
AlchemyUncreditedEnd_081c14a6:
	str	r8, [sp]
	ldrb	sl, [r4, #10]
	ldrb	fp, [r4, #11]
	lsl	sl, sl, #16
	lsl	fp, fp, #16
	ldrb	r0, [r4, #1]
	tst	r0, #8
	beq .L_030062d8
.L_030061b8:
	cmp	r2, #4
	ble .L_03006228
	subs	r2, r2, r8
	movgt	lr, #0
	bgt .L_030061e4
	mov	lr, r8
	add	r2, r2, r8
	sub	r8, r2, #4
	sub	lr, lr, r8
	ands	r2, r2, #3
	moveq	r2, #4
.L_030061e4:
	ldr	r6, [r5]
	ldr	r7, [r5, #1584]
.L_030061ec:
	ldrsb	r0, [r3], #1
	mul	r1, sl, r0
	bic	r1, r1, #16711680
	add	r6, r1, r6, ror #8
	mul	r1, fp, r0
	bic	r1, r1, #16711680
	add	r7, r1, r7, ror #8
	adds	r5, r5, #1073741824
	bcc .L_030061ec
	str	r7, [r5, #1584]
	str	r6, [r5], #4
	subs	r8, r8, #4
	bgt .L_030061e4
	adds	r8, r8, lr
	beq .L_0300636c
.L_03006228:
	ldr	r6, [r5]
	ldr	r7, [r5, #1584]
.L_03006230:
	ldrsb	r0, [r3], #1
	mul	r1, sl, r0
	bic	r1, r1, #16711680
	add	r6, r1, r6, ror #8
	mul	r1, fp, r0
	bic	r1, r1, #16711680
	add	r7, r1, r7, ror #8
	subs	r2, r2, #1
	beq .L_030062a0
.L_03006254:
	adds	r5, r5, #1073741824
	bcc .L_03006230
	str	r7, [r5, #1584]
	str	r6, [r5], #4
	subs	r8, r8, #4
	bgt .L_030061b8
	b .L_0300636c
.L_03006270:
	ldr	r0, [sp, #24]
	cmp	r0, #0
	beq .L_03006294
	ldr	r3, [sp, #20]
	rsb	r9, r2, #0
.L_03006284:
	adds	r2, r0, r2
	bgt .L_0300633c
	sub	r9, r9, r0
	b .L_03006284
.L_03006294:
	pop	{r4, ip}
	mov	r2, #0
	b .L_030062b0
.L_030062a0:
	ldr	r2, [sp, #16]
	cmp	r2, #0
	ldrne	r3, [sp, #12]
	bne .L_03006254
.L_030062b0:
	strb	r2, [r4]
	lsr	r0, r5, #30
	bic	r5, r5, #-1073741824
	rsb	r0, r0, #3
	lsl	r0, r0, #3
	ror	r6, r6, r0
	ror	r7, r7, r0
	str	r7, [r5, #1584]
	str	r6, [r5], #4
	b .L_03006374
.L_030062d8:
	push	{r4, ip}
	ldr	lr, [r4, #28]
	ldr	r1, [r4, #32]
	mul	r4, ip, r1
	ldrsb	r0, [r3]
	ldrsb	r1, [r3, #1]!
	sub	r1, r1, r0
.L_030062f4:
	ldr	r6, [r5]
	ldr	r7, [r5, #1584]
.L_030062fc:
	mul	r9, lr, r1
	add	r9, r0, r9, asr #23
	mul	ip, sl, r9
	bic	ip, ip, #16711680
	add	r6, ip, r6, ror #8
	mul	ip, fp, r9
	bic	ip, ip, #16711680
	add	r7, ip, r7, ror #8
	add	lr, lr, r4
	lsrs	r9, lr, #23
	beq .L_03006348
	bic	lr, lr, #1065353216
	subs	r2, r2, r9
	ble .L_03006270
	subs	r9, r9, #1
	addeq	r0, r0, r1
.L_0300633c:
	ldrsbne	r0, [r3, r9]!
	ldrsb	r1, [r3, #1]!
	sub	r1, r1, r0
.L_03006348:
	adds	r5, r5, #1073741824
	bcc .L_030062fc
	str	r7, [r5, #1584]
	str	r6, [r5], #4
	subs	r8, r8, #4
	bgt .L_030062f4
	sub	r3, r3, #1
	pop	{r4, ip}
	str	lr, [r4, #28]
.L_0300636c:
	str	r2, [r4, #24]
	str	r3, [r4, #40]
.L_03006374:
	ldr	r8, [sp]
	add	r0, pc, #1
	bx	r0
	.thumb
.L_03006380:
	ldr	r0, [sp, #4]
	subs	r0, #1
	ble .L_0300638a
	adds	r4, #64
	b .L_03006098
.L_0300638a:
	ldr	r0, [sp, #24]
	ldr	r3, [pc, #16]
	str	r3, [r0, #0]
	add	sp, #28
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r3}
	bx	r3
	.align 2
.L_030063a0:
	.4byte 0x68736d53
