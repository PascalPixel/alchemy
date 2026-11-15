@ Why assembly: bcc consumes the carry of lsls r0, r6, #25; fp (r11) holds a
@ channel volume, not a frame pointer; and add r1, pc, #8 / bx r1 switches to
@ the ARM code that follows below, a mode switch inside a
@ function.
@ Why assembly: stm lr, {r0, r1} and stm lr, {r2, r8, r9, sl} copy one of
@ resident_mixer_channel.s's three templates into the lanes' instruction slots
@ before they run, stores into code; lsrs fp, fp, #1 / adc fp, fp, #0x8000
@ consumes a shift's carry; fp (r11) holds the packed volumes, not a frame
@ pointer; movs r2, r2 copies and tests without a shift; and push {r4, r9, ip}
@ saves without lr.
	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb_func
	.arm
	.thumb
	.global IwramSoundMixEntry
IwramSoundMixEntry:
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
	b.n	IwramSoundMixNextChannel
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
	.4byte	IwramSoundMixWorkspace
AlchemyUncredited_08000e34:
	.4byte	0x00000000
AlchemyUncreditedEnd_08000e34:

	.section .text.resident_mixer_lanes,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.arm
	mov	r8, #0x210
	orrs	fp, sl, fp, lsl #16
	beq	IwramSoundMixSkipChannel
	ldr	r6, [pc, #1160]
	add	r6, r6, #1
	str	r6, [pc, #1152]
	tst	r0, #8
	bne	IwramSoundMixBlock
	push	{r4, r9, ip}
	movs	r2, r2
	orreq	r0, r0, #64
	strbeq	r0, [r4, #1]
	add	r4, r4, #28
	ldm	r4, {r7, lr}
	mul	r4, ip, lr
	ldrsb	r6, [r3], #1
	ldrsb	ip, [r3]
	ands	r0, r0, #64
	bne	IwramSoundSynthWave
	sub	ip, ip, r6
	lsrs	fp, fp, #1
	adc	fp, fp, #0x8000
	bic	fp, fp, #0xff00
	mov	r1, r7
	umlal	r1, r0, r4, r8
	lsr	r1, r1, #23
	orr	r0, r1, r0, lsl #9
	cmp	r2, r0
	ble	IwramSoundMixInterpolated
	sub	r2, r2, r0
	ldr	sl, [pc, #500]
	add	sl, sl, r0
	cmp	sl, sp
	add	sl, r3, r0
	add	r9, pc, #0x1d8
	stm	r9, {r2, sl, sp}
	cmpcc	r0, #0x400
	bcs	.L_03000940
	bic	r1, r3, #3
	mov	r9, #0x4000000
	add	r9, r9, #212
	add	r0, r0, #7
	lsr	r0, r0, #2
	sub	sp, sp, r0, lsl #2
	and	r3, r3, #3
	add	r3, r3, sp
	orr	lr, r0, #0x84000000
	stm	r9, {r1, sp, lr}
.L_03000940:
	subs	r4, r4, #0x800000
	lslpl	fp, fp, #1
	add	r0, pc, #0x1ac
	addpl	r0, r0, #24
	subspl	r4, r4, #0x800000
	addpl	r0, r0, #24
	addpl	r4, r4, #0x800000
	ldr	r2, [pc, #404]
	cmp	r0, r2
	beq	.L_030009a0
	str	r0, [pc, #392]
	ldm	r0, {r0, r1, r2, r8, r9, sl}
	add	lr, pc, #52
.L_03000974:
	stm	lr, {r0, r1}
	add	lr, lr, #152
	stm	lr, {r0, r1}
	sub	lr, lr, #140
	stm	lr, {r2, r8, r9, sl}
	add	lr, lr, #152
	stm	lr, {r2, r8, r9, sl}
	sub	lr, lr, #128
	adds	r5, r5, #0x40000000
	bcc	.L_03000974
	mov	r8, #0x210
.L_030009a0:
	mov	r2, #0xff000000
.L_030009a4:
	ldm	r5, {r0, r1, sl, lr}
	mul	r9, r7, ip
@ Each 0xfedcba98 run below is an instruction slot the stm lr loop above fills.
AlchemyUncredited_08000f64:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08000f64:
	mlane	r0, fp, r9, r0
AlchemyUncredited_08000f70:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08000f70:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_08000f88:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08000f88:
	mlane	r1, fp, r9, r1
AlchemyUncredited_08000f94:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08000f94:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_08000fac:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08000fac:
	mlane	sl, fp, r9, sl
AlchemyUncredited_08000fb8:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08000fb8:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_08000fd0:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08000fd0:
	mlane	lr, fp, r9, lr
AlchemyUncredited_08000fdc:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08000fdc:
	bic	r7, r7, r2, asr #1
	stmia	r5!, {r0, r1, sl, lr}
	ldm	r5, {r0, r1, sl, lr}
	mulne	r9, r7, ip
AlchemyUncredited_08000ffc:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08000ffc:
	mlane	r0, fp, r9, r0
AlchemyUncredited_08001008:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08001008:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_08001020:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08001020:
	mlane	r1, fp, r9, r1
AlchemyUncredited_0800102c:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_0800102c:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_08001044:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08001044:
	mlane	sl, fp, r9, sl
AlchemyUncredited_08001050:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08001050:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_08001068:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08001068:
	mlane	lr, fp, r9, lr
AlchemyUncredited_08001074:
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
	.4byte	0xfedcba98
AlchemyUncreditedEnd_08001074:
	bic	r7, r7, r2, asr #1
	stmia	r5!, {r0, r1, sl, lr}
	subs	r8, r8, #8
	bgt	.L_030009a4
	add	ip, pc, #4
	ldm	ip, {r2, r3, sp}
	b	IwramSoundMixDone
@ r2, sl and sp as stm r9, {r2, sl, sp} saved them; ldm ip reloads them.
AlchemyUncredited_080010a0:
	.4byte	0x00000000
	.4byte	0x00000000
	.4byte	0x00000000
AlchemyUncreditedEnd_080010a0:
	.4byte	0x03007900
