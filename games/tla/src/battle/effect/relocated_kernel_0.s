@ Two hand-written ARM kernels, each copied into a heap block and run there:
@ Data_081380ac (its size is Data_081380acCodeSize, which RamCode_RunKernel0
@ copies) and Data_0813847c after it.
.syntax unified
	.thumb
	.global Data_081380ac
Data_081380ac:
	.global Func_081380ac
	.thumb_func
Func_081380ac:
	bx	pc
AlchemyUncredited_081380ae:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_081380ae:
	push	{r5, r6, r7, r8, r9, sl, lr}
	mov	r3, #50331648
	add	r0, r0, r0, lsl #1
	sub	sp, sp, #44
	ldr	r3, [r3, #100]
	mov	r7, r1
	ldr	r8, [pc, #768]
	mov	r5, r3
	add	r6, r3, r0, lsl #2
	cmp	r3, r6
	str	r3, [sp, #40]
	beq	.L_0813810c
.L_081380e0:
	mov	r0, r5
	mov	r1, r5
	mov	lr, pc
	bx	r8
	ldm	r5, {r2, r3}
	add	r3, r3, #4194304
	str	r3, [r5, #4]
	add	r2, r2, #4194304
	str	r2, [r5], #12
	cmp	r5, r6
	bne	.L_081380e0
.L_0813810c:
	mov	r3, #28672
	mov	r0, #50331648
	ldr	r1, [r0, #92]
	add	r3, r3, #128
	add	r2, r1, r3
	str	r2, [sp, #36]
	rsb	r2, r7, r7, lsl #3
	add	r3, r3, r2, lsl #2
	add	r1, r1, r3
	str	r1, [sp, #32]
	ldr	r3, [sp, #36]
	ldr	r0, [r0, #96]
	cmp	r3, r1
	str	r0, [sp, #28]
	beq	.L_0813846c
.L_08138148:
	ldr	r2, [sp, #36]
	ldr	r3, [r2]
	ldr	r2, [sp, #40]
	ldr	r5, [r2, r3, lsl #2]
	add	r3, r2, r3, lsl #2
	ldr	sl, [r3, #4]
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #4]
	ldr	r3, [sp, #40]
	ldr	r9, [r3, r2, lsl #2]
	add	r2, r3, r2, lsl #2
	ldr	r6, [r2, #4]
	ldr	r2, [sp, #36]
	ldr	r3, [r2, #8]
	ldr	r2, [sp, #40]
	ldr	r2, [r2, r3, lsl #2]
	str	r2, [sp, #24]
	ldr	r2, [sp, #40]
	asr	sl, sl, #16
	add	r3, r2, r3, lsl #2
	ldr	r4, [r3, #4]
	asr	r6, r6, #16
	asr	r4, r4, #16
	rsb	r0, r5, r9
	rsb	r3, sl, r4
	smull	r1, r2, r0, r3
	ldr	r3, [sp, #24]
	rsb	ip, r5, r3
	rsb	r3, r6, sl
	smlals	r1, r2, ip, r3
	addgt	r2, r2, #1
	cmp	r2, #0
	ldr	r2, [sp, #36]
	add	r2, r2, #28
	str	r2, [sp, #8]
	ble	.L_08138458
	cmp	sl, r6
	ble	.L_081381f8
	eor	r5, r5, r9
	eor	r9, r9, r5
	eor	r5, r5, r9
	eor	sl, sl, r6
	eor	r6, r6, sl
	eor	sl, sl, r6
.L_081381f8:
	cmp	r6, r4
	ble	.L_08138240
	ldr	r3, [sp, #24]
	eor	r9, r9, r3
	eor	r3, r3, r9
	eor	r9, r9, r3
	str	r3, [sp, #24]
	eor	r6, r6, r4
	eor	r4, r4, r6
	eor	r6, r6, r4
	cmp	sl, r6
	ble	.L_08138240
	eor	r5, r5, r9
	eor	r9, r9, r5
	eor	r5, r5, r9
	eor	sl, sl, r6
	eor	r6, r6, sl
	eor	sl, sl, r6
.L_08138240:
	cmp	sl, r6
	beq	.L_08138268
	rsb	r0, r5, r9
	ldr	r2, [pc, #384]
	rsb	r1, sl, r6
	str	r4, [sp]
	mov	lr, pc
	bx	r2
	str	r0, [sp, #20]
	ldr	r4, [sp]
.L_08138268:
	ldr	r3, [sp, #24]
	str	r4, [sp]
	rsb	r1, sl, r4
	ldr	r2, [pc, #344]
	rsb	r0, r5, r3
	mov	lr, pc
	bx	r2
	str	r0, [sp, #16]
	mov	r8, sl
	ldr	r4, [sp]
	mov	r7, r5
	ldr	r3, [sp, #20]
	ldr	r2, [sp, #36]
	rsb	sl, r6, r4
	ldrb	r2, [r2, #24]
	cmp	r3, r0
	str	r2, [sp, #12]
	ble	.L_081382cc
	eor	r3, r3, r0
	mov	r2, r0
	eor	r2, r2, r3
	str	r2, [sp, #16]
	eor	r3, r3, r2
	str	r3, [sp, #20]
	rsb	sl, sl, #0
.L_081382cc:
	ldr	r3, [sp, #36]
	cmp	r8, r6
	add	r3, r3, #28
	str	r3, [sp, #8]
	beq	.L_08138360
.L_081382e0:
	asr	r0, r5, #16
	asr	r3, r7, #16
	cmp	r0, r3
	ldr	r2, [sp, #20]
	mov	r1, r3
	ldr	r3, [sp, #16]
	add	r4, r5, r2
	add	r7, r7, r3
	add	r2, r8, #1
	str	r2, [sp, #4]
	and	r2, r8, #7
	asr	r3, r8, #3
	lsl	r3, r3, #10
	orr	r3, r3, r2, lsl #3
	ldr	r2, [sp, #28]
	add	ip, r2, r3
	beq	.L_08138350
	mov	lr, r1
.L_08138328:
	asr	r3, r0, #3
	and	r2, r0, #7
	orr	r3, r2, r3, lsl #6
	ldrb	r1, [ip, r3]
	ldr	r2, [sp, #12]
	cmp	r1, r2
	strbcc	r2, [ip, r3]
	add	r0, r0, #1
	cmp	r0, lr
	bne	.L_08138328
.L_08138350:
	ldr	r8, [sp, #4]
	mov	r5, r4
	cmp	r8, r6
	bne	.L_081382e0
.L_08138360:
	cmp	sl, #0
	beq	.L_08138458
	movlt	r7, r5
	ldrlt	r3, [sp, #20]
	rsblt	sl, sl, #0
	strlt	r3, [sp, #16]
	mov	r5, r9
	ldr	r2, [sp, #24]
	mov	r1, sl
	ldr	r3, [pc, #72]
	rsb	r0, r5, r2
	mov	lr, pc
	bx	r3
	add	sl, sl, r8
	cmp	r5, r7
	str	r0, [sp, #20]
	ble	.L_08138450
	eor	r5, r5, r7
	eor	r7, r7, r5
	eor	r5, r5, r7
	mov	r2, r0
	ldr	r3, [sp, #16]
	eor	r2, r2, r3
	eor	r3, r3, r2
	str	r3, [sp, #16]
	eor	r2, r2, r3
	str	r2, [sp, #20]
	b	.L_08138450
	.4byte	IwramTransformVector
	.4byte	IwramSignedDivide
.L_081383d8:
	asr	r0, r5, #16
	asr	r3, r7, #16
	cmp	r0, r3
	ldr	r2, [sp, #20]
	mov	r1, r3
	ldr	r3, [sp, #16]
	add	r4, r5, r2
	add	r7, r7, r3
	add	r2, r8, #1
	str	r2, [sp, #4]
	and	r2, r8, #7
	asr	r3, r8, #3
	lsl	r3, r3, #10
	orr	r3, r3, r2, lsl #3
	ldr	r2, [sp, #28]
	add	ip, r2, r3
	beq	.L_08138448
	mov	lr, r1
.L_08138420:
	asr	r3, r0, #3
	and	r2, r0, #7
	orr	r3, r2, r3, lsl #6
	ldrb	r1, [ip, r3]
	ldr	r2, [sp, #12]
	cmp	r1, r2
	strbcc	r2, [ip, r3]
	add	r0, r0, #1
	cmp	r0, lr
	bne	.L_08138420
.L_08138448:
	ldr	r8, [sp, #4]
	mov	r5, r4
.L_08138450:
	cmp	r8, sl
	bne	.L_081383d8
.L_08138458:
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #32]
	str	r3, [sp, #36]
	cmp	r3, r2
	bne	.L_08138148
.L_0813846c:
	add	sp, sp, #44
	pop	{r5, r6, r7, r8, r9, sl, lr}
	bx	lr
	.global Data_081380acCodeSize
	.set Data_081380acCodeSize, . - Data_081380ac
	.4byte	gModelTransformWork
	.global Data_0813847c
Data_0813847c:
	push	{r5, r6, r7, r8, r9, sl, lr}
	sub	sp, sp, #144
	str	r0, [sp, #140]
	str	r1, [sp, #136]
	str	r3, [sp, #132]
	mov	r3, #50331648
	ldr	r3, [r3, #96]
	mov	r6, r2
	str	r3, [sp, #128]
	ldr	r1, [sp, #132]
	ldr	r2, [r0]
	rsb	r8, r1, #8
	lsls	r3, r2, #8
	beq	.L_08138a08
	mov	r0, #-2147483648
	asr	r0, r0, #15
	str	r0, [sp, #60]
.L_081384c0:
	mov	r3, #2032
	add	r3, r3, #8
	ldr	r0, [sp, #136]
	and	r7, r3, r2, lsl #3
	ldr	r1, [r0, r7]
	and	r7, r3, r2, asr #5
	ldr	r7, [r0, r7]
	str	r7, [sp, #124]
	and	r7, r3, r2, asr #13
	ldr	r2, [sp, #140]
	ldr	r3, [sp, #124]
	ldr	r7, [r0, r7]
	str	r7, [sp, #120]
	ldr	ip, [r2, #4]
	cmp	r1, r3
	ldrh	r4, [r2, #8]
	ble	.L_08138518
	eor	r1, r1, r3
	eor	r3, r3, r1
	eor	r1, r1, r3
	str	r3, [sp, #124]
	ror	ip, ip, #16
.L_08138518:
	ldr	r0, [sp, #124]
	ldr	r2, [sp, #120]
	cmp	r0, r2
	ble	.L_08138564
	eor	r0, r0, r2
	eor	r2, r2, r0
	str	r2, [sp, #120]
	eor	r0, r0, r2
	str	r0, [sp, #124]
	lsl	r3, r4, #16
	ror	r4, ip, #16
	orr	ip, r3, r4, lsr #16
	cmp	r1, r0
	ble	.L_08138564
	eor	r1, r1, r0
	eor	r0, r0, r1
	eor	r1, r1, r0
	str	r0, [sp, #124]
	ror	ip, ip, #16
.L_08138564:
	asr	r3, r1, #23
	cmp	r3, #0
	ldr	r3, [sp, #132]
	ldr	r0, [sp, #140]
	rsb	r3, r3, #8
	str	r3, [sp, #52]
	add	r0, r0, #12
	str	r0, [sp, #56]
	bgt	.L_081389f0
	ldr	r2, [sp, #120]
	asr	r3, r2, #16
	asr	r2, r1, #16
	str	r3, [sp, #12]
	subs	sl, r3, r2
	str	r2, [sp, #24]
	beq	.L_081389f0
	lsl	r9, r1, #16
	ldr	r3, [sp, #124]
	lsl	r0, ip, #16
	str	r0, [sp, #72]
	and	r1, r0, #-16777216
	str	r1, [sp, #80]
	ldr	r0, [sp, #120]
	ldr	r5, [pc, #692]
	str	r4, [sp, #4]
	mov	r1, sl
	str	ip, [sp]
	rsb	r3, r2, r3, asr #16
	lsl	r2, ip, #24
	str	r3, [sp, #116]
	and	r3, r2, #-16777216
	lsl	r7, r0, #16
	str	r2, [sp, #68]
	rsb	r0, r9, r7
	str	r3, [sp, #76]
	mov	lr, pc
	bx	r5
	str	r0, [sp, #112]
	ldr	r4, [sp, #4]
	ldr	r1, [sp, #80]
	lsl	r0, r4, #16
	and	r0, r0, #-16777216
	rsb	r0, r1, r0
	mov	r1, sl
	mov	lr, pc
	bx	r5
	ldr	r4, [sp, #4]
	ldr	r2, [sp, #76]
	mov	r1, sl
	str	r0, [sp, #108]
	rsb	r0, r2, r4, lsl #24
	mov	lr, pc
	bx	r5
	ldr	r3, [sp, #116]
	str	r0, [sp, #104]
	str	r9, [sp, #28]
	str	r7, [sp, #20]
	ldr	ip, [sp]
	cmp	r3, #0
	beq	.L_08138680
	ldr	r9, [sp, #112]
	ldr	r0, [sp, #28]
	mla	r9, r3, r9, r0
	ldr	r1, [sp, #108]
	ldr	r2, [sp, #80]
	mla	r1, r3, r1, r2
	ldr	r0, [sp, #104]
	str	r1, [sp, #80]
	ldr	r1, [sp, #76]
	mla	r0, r3, r0, r1
	str	r0, [sp, #76]
.L_08138680:
	ldr	r2, [sp, #124]
	ldr	r3, [sp, #80]
	and	r0, ip, #-16777216
	str	ip, [sp]
	lsl	sl, r2, #16
	rsb	r7, r9, sl
	asr	r7, r7, #16
	mov	r5, r7
	rsb	r0, r3, r0
	ldr	r2, [pc, #464]
	mov	r1, r5
	mov	lr, pc
	bx	r2
	ldr	ip, [sp]
	mov	r9, r0
	ldr	r3, [sp, #76]
	mov	r1, r5
	ldr	r2, [pc, #432]
	lsl	r0, ip, #8
	and	r0, r0, #-16777216
	rsb	r0, r3, r0
	mov	lr, pc
	bx	r2
	mov	r2, #-2147483648
	asr	r2, r2, #15
	and	r0, r0, r2
	add	r9, r0, r9, asr #16
	ldr	r0, [sp, #72]
	ldr	r1, [sp, #68]
	cmn	r7, #1
	ldr	r7, [sp, #28]
	str	r2, [sp, #8]
	and	r3, r0, #-16777216
	orr	r3, r3, #8388608
	str	r3, [sp, #88]
	orr	r1, r1, #8388608
	str	r1, [sp, #92]
	ldr	r1, [sp, #116]
	movgt	r5, #1
	mvnle	r5, #0
	ldr	r3, [pc, #340]
	rsb	r0, r7, sl
	str	r7, [sp, #100]
	mov	lr, pc
	bx	r3
	str	r0, [sp, #96]
	str	r9, [sp, #84]
	cmp	r5, #0
	ldr	r2, [sp, #8]
	sublt	r7, r7, #65536
	rsblt	r9, r9, #0
	strlt	r7, [sp, #100]
	blt	.L_08138770
	ldr	r0, [sp, #88]
	and	r3, r9, r2
	ldr	r1, [sp, #92]
	add	r0, r0, r3
	str	r0, [sp, #88]
	add	r1, r1, r9, lsl #16
	str	r1, [sp, #92]
.L_08138770:
	ldr	r4, [sp, #24]
	ldr	r2, [sp, #132]
	ldr	r3, [sp, #140]
	rsb	r2, r2, #8
	str	r2, [sp, #52]
	add	r3, r3, #12
	str	r3, [sp, #56]
.L_0813878c:
	ldr	r0, [sp, #124]
	asr	r3, r0, #23
	cmp	r3, #0
	movgt	r0, #8388608
	cmp	r4, r0, asr #16
	str	r0, [sp, #124]
	beq	.L_0813898c
	ldr	r1, [sp, #84]
	lsl	r3, r1, #16
	asr	r3, r3, #16
	str	r3, [sp, #64]
.L_081387b8:
	ldr	r2, [sp, #124]
	ldr	r3, [sp, #100]
	ldr	r0, [sp, #112]
	ldr	r1, [sp, #96]
	asr	r2, r2, #16
	str	r2, [sp, #16]
	add	r3, r3, r0
	str	r3, [sp, #48]
	add	r1, r7, r1
	str	r1, [sp, #32]
	ldr	r2, [sp, #92]
	cmp	r4, #0
	ldr	r3, [sp, #104]
	mov	sl, #65280
	ldr	r0, [sp, #88]
	asr	sl, sl, r8
	ldr	r1, [sp, #108]
	add	r2, r2, r3
	str	r2, [sp, #44]
	add	r0, r0, r1
	str	r0, [sp, #40]
	add	r2, r4, #1
	str	r2, [sp, #36]
	blt	.L_08138960
	ldr	r3, [sp, #100]
	ldr	ip, [sp, #64]
	and	r1, r3, #65280
	mul	ip, r1, ip
	ldr	r0, [sp, #84]
	asr	r3, r0, #16
	mul	r2, r1, r3
	ldr	r1, [sp, #100]
	ldr	r3, [sp, #88]
	asr	r0, r1, #16
	lsrs	r1, r1, #23
	ldr	r1, [sp, #92]
	rsb	ip, ip, r3
	ldr	r3, [sp, #60]
	rsb	r2, r2, r1
	and	r2, r2, r3
	orr	r2, r2, ip, lsr #16
	beq	.L_0813888c
	teq	r0, r5
	bpl	.L_08138960
	cmp	r0, #0
	bge	.L_08138880
	rsb	r3, r0, #0
	mov	r0, #0
	b	.L_08138888
	.4byte	IwramSignedDivide
.L_08138880:
	sub	r3, r0, #127
	mov	r0, #127
.L_08138888:
	mla	r2, r3, r9, r2
.L_0813888c:
	asr	r3, r7, #16
	mov	ip, r3
	lsrs	r1, r7, #23
	beq	.L_081388a8
	cmn	r3, #1
	movgt	ip, #128
	mvnle	ip, #0
.L_081388a8:
	rsb	r3, r5, r0
	cmp	r3, ip
	ldr	r3, [sp, #124]
	ldr	r1, [sp, #100]
	asr	r3, r3, #16
	str	r3, [sp, #16]
	ldr	r3, [sp, #112]
	add	r1, r1, r3
	str	r1, [sp, #48]
	ldr	r1, [sp, #96]
	ldr	r3, [sp, #92]
	add	r7, r7, r1
	ldr	r1, [sp, #104]
	add	r3, r3, r1
	str	r3, [sp, #44]
	ldr	r3, [sp, #88]
	ldr	r1, [sp, #108]
	str	r7, [sp, #32]
	add	r3, r3, r1
	str	r3, [sp, #40]
	add	r3, r4, #1
	str	r3, [sp, #36]
	beq	.L_08138960
	and	r3, r4, #7
	asr	r1, r4, #3
	lsl	r1, r1, #10
	orr	r1, r1, r3, lsl #3
	ldr	r3, [sp, #128]
	cmp	r0, ip
	add	r1, r3, r1
	beq	.L_08138960
.L_08138924:
	and	r3, sl, r2, lsr r8
	orr	r3, r3, r2, lsr #24
	ldrb	r7, [r6, r3]
	cmp	r7, #0
	blt	.L_08138950
	asr	r3, r0, #3
	and	lr, r0, #7
	orr	r3, lr, r3, lsl #6
	ldrb	r4, [r1, r3]
	cmp	r4, r7
	strblt	r7, [r1, r3]
.L_08138950:
	add	r2, r2, r9
	add	r0, r0, r5
	cmp	r0, ip
	bne	.L_08138924
.L_08138960:
	ldr	r0, [sp, #48]
	ldr	r7, [sp, #32]
	ldr	r1, [sp, #44]
	ldr	r2, [sp, #40]
	ldr	r4, [sp, #36]
	ldr	r3, [sp, #16]
	str	r0, [sp, #100]
	str	r1, [sp, #92]
	cmp	r4, r3
	str	r2, [sp, #88]
	bne	.L_081387b8
.L_0813898c:
	asr	r3, r4, #7
	cmp	r3, #0
	bgt	.L_081389f0
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #124]
	subs	r0, r0, r1, asr #16
	str	r0, [sp, #116]
	beq	.L_081389f0
	ldr	r2, [sp, #20]
	lsl	r7, r1, #16
	ldr	r1, [sp, #116]
	str	r4, [sp, #4]
	ldr	r3, [pc, #40]
	rsb	r0, r7, r2
	mov	lr, pc
	bx	r3
	str	r0, [sp, #96]
	sub	r3, r7, #65536
	ldr	r0, [sp, #120]
	cmp	r5, #0
	ldr	r4, [sp, #4]
	movlt	r7, r3
	str	r0, [sp, #124]
	b	.L_0813878c
	.4byte	IwramSignedDivide
.L_081389f0:
	ldr	r1, [sp, #56]
	str	r1, [sp, #140]
	ldr	r2, [r1]
	ldr	r8, [sp, #52]
	lsls	r3, r2, #8
	bne	.L_081384c0
.L_08138a08:
	add	sp, sp, #144
	pop	{r5, r6, r7, r8, r9, sl, lr}
	bx	lr
	andeq	r4, r0, #120, 14
	push	{r5, r6, r7, r8, r9, sl, lr}
	sub	sp, sp, #156
	str	r0, [sp, #152]
	str	r1, [sp, #148]
	str	r3, [sp, #144]
	mov	r3, #50331648
	ldr	r3, [r3, #96]
	mov	r6, r2
	str	r3, [sp, #140]
	ldr	r1, [sp, #144]
	ldr	r2, [r0]
	rsb	r8, r1, #8
	lsls	r3, r2, #8
	beq	.L_08138fbc
	mov	r4, #-2147483648
	asr	r4, r4, #15
	str	r4, [sp, #64]
.L_08138a5c:
	mov	r3, #2032
	add	r3, r3, #8
	ldr	r0, [sp, #148]
	and	r7, r3, r2, lsl #3
	ldr	r1, [r0, r7]
	and	r7, r3, r2, asr #5
	ldr	r7, [r0, r7]
	str	r7, [sp, #136]
	and	r7, r3, r2, asr #13
	ldr	r2, [sp, #152]
	ldr	r3, [sp, #136]
	ldr	r7, [r0, r7]
	str	r7, [sp, #132]
	ldr	ip, [r2, #4]
	cmp	r1, r3
	ldrh	r4, [r2, #8]
	ble	.L_08138ab4
	eor	r1, r1, r3
	eor	r3, r3, r1
	eor	r1, r1, r3
	str	r3, [sp, #136]
	ror	ip, ip, #16
.L_08138ab4:
	ldr	r0, [sp, #136]
	ldr	r2, [sp, #132]
	cmp	r0, r2
	ble	.L_08138b00
	eor	r0, r0, r2
	eor	r2, r2, r0
	str	r2, [sp, #132]
	eor	r0, r0, r2
	str	r0, [sp, #136]
	lsl	r3, r4, #16
	ror	r4, ip, #16
	orr	ip, r3, r4, lsr #16
	cmp	r1, r0
	ble	.L_08138b00
	eor	r1, r1, r0
	eor	r0, r0, r1
	eor	r1, r1, r0
	str	r0, [sp, #136]
	ror	ip, ip, #16
.L_08138b00:
	asr	r3, r1, #23
	cmp	r3, #0
	ldr	r3, [sp, #144]
	ldr	r0, [sp, #152]
	rsb	r3, r3, #8
	str	r3, [sp, #52]
	add	r0, r0, #12
	str	r0, [sp, #56]
	bgt	.L_08138fa4
	ldr	r2, [sp, #132]
	asr	r3, r2, #16
	asr	r2, r1, #16
	str	r3, [sp, #12]
	subs	sl, r3, r2
	str	r2, [sp, #24]
	beq	.L_08138fa4
	lsl	r9, r1, #16
	ldr	r3, [sp, #136]
	lsl	r0, ip, #16
	str	r0, [sp, #84]
	and	r1, r0, #-16777216
	str	r1, [sp, #92]
	ldr	r0, [sp, #132]
	ldr	r5, [pc, #712]
	str	r4, [sp, #4]
	mov	r1, sl
	str	ip, [sp]
	rsb	r3, r2, r3, asr #16
	lsl	r2, ip, #24
	str	r3, [sp, #128]
	and	r3, r2, #-16777216
	str	r2, [sp, #80]
	lsl	r0, r0, #16
	str	r0, [sp, #76]
	rsb	r0, r9, r0
	str	r3, [sp, #88]
	mov	lr, pc
	bx	r5
	str	r0, [sp, #124]
	ldr	r4, [sp, #4]
	ldr	r1, [sp, #92]
	lsl	r0, r4, #16
	and	r0, r0, #-16777216
	rsb	r0, r1, r0
	mov	r1, sl
	mov	lr, pc
	bx	r5
	ldr	r4, [sp, #4]
	ldr	r2, [sp, #88]
	mov	r1, sl
	str	r0, [sp, #120]
	rsb	r0, r2, r4, lsl #24
	mov	lr, pc
	bx	r5
	ldr	r3, [sp, #128]
	str	r0, [sp, #116]
	str	r9, [sp, #28]
	ldr	r4, [sp, #76]
	ldr	ip, [sp]
	cmp	r3, #0
	str	r4, [sp, #20]
	beq	.L_08138c24
	ldr	r9, [sp, #124]
	ldr	r0, [sp, #28]
	mla	r9, r3, r9, r0
	ldr	r1, [sp, #120]
	ldr	r2, [sp, #92]
	mla	r1, r3, r1, r2
	ldr	r4, [sp, #116]
	ldr	r0, [sp, #88]
	mla	r4, r3, r4, r0
	str	r1, [sp, #92]
	str	r4, [sp, #88]
.L_08138c24:
	ldr	r1, [sp, #136]
	str	ip, [sp]
	and	r0, ip, #-16777216
	ldr	r3, [pc, #500]
	lsl	sl, r1, #16
	rsb	r2, r9, sl
	asr	r2, r2, #16
	str	r2, [sp, #72]
	mov	r5, r2
	ldr	r2, [sp, #92]
	mov	r1, r5
	rsb	r0, r2, r0
	mov	lr, pc
	bx	r3
	ldr	ip, [sp]
	mov	r9, r0
	ldr	r4, [sp, #88]
	mov	r1, r5
	ldr	r2, [pc, #440]
	lsl	r0, ip, #8
	and	r0, r0, #-16777216
	rsb	r0, r4, r0
	mov	lr, pc
	bx	r2
	mov	r2, #-2147483648
	asr	r2, r2, #15
	ldr	r3, [sp, #72]
	and	r0, r0, r2
	ldr	r4, [sp, #84]
	add	r9, r0, r9, asr #16
	ldr	r0, [sp, #80]
	ldr	r7, [sp, #28]
	ldr	r1, [sp, #128]
	str	r2, [sp, #8]
	cmn	r3, #1
	and	r3, r4, #-16777216
	orr	r3, r3, #8388608
	str	r3, [sp, #100]
	orr	r0, r0, #8388608
	str	r0, [sp, #104]
	movgt	r5, #1
	mvnle	r5, #0
	ldr	r3, [pc, #344]
	rsb	r0, r7, sl
	str	r7, [sp, #112]
	mov	lr, pc
	bx	r3
	str	r0, [sp, #108]
	str	r9, [sp, #96]
	cmp	r5, #0
	ldr	r2, [sp, #8]
	sublt	r7, r7, #65536
	rsblt	r9, r9, #0
	strlt	r7, [sp, #112]
	blt	.L_08138d1c
	ldr	r4, [sp, #100]
	and	r3, r9, r2
	ldr	r0, [sp, #104]
	add	r4, r4, r3
	str	r4, [sp, #100]
	add	r0, r0, r9, lsl #16
	str	r0, [sp, #104]
.L_08138d1c:
	ldr	r4, [sp, #24]
	ldr	r1, [sp, #144]
	ldr	r2, [sp, #152]
	rsb	r1, r1, #8
	str	r1, [sp, #52]
	add	r2, r2, #12
	str	r2, [sp, #56]
.L_08138d38:
	ldr	r0, [sp, #136]
	asr	r3, r0, #23
	cmp	r3, #0
	movgt	r0, #8388608
	cmp	r4, r0, asr #16
	str	r0, [sp, #136]
	beq	.L_08138f3c
	ldr	r1, [sp, #96]
	lsl	r3, r1, #16
	asr	r3, r3, #16
	str	r3, [sp, #68]
.L_08138d64:
	ldr	lr, [sp, #184]
	ldr	r2, [sp, #136]
	ldr	r3, [sp, #112]
	ldr	r0, [sp, #124]
	ldr	r1, [sp, #108]
	asr	r2, r2, #16
	str	r2, [sp, #16]
	add	r3, r3, r0
	str	r3, [sp, #48]
	add	r1, r7, r1
	str	r1, [sp, #32]
	ldr	r2, [sp, #104]
	cmp	r4, #0
	ldr	r3, [sp, #116]
	mov	sl, #65280
	ldr	r0, [sp, #100]
	asr	sl, sl, r8
	ldr	r1, [sp, #120]
	add	r2, r2, r3
	str	r2, [sp, #40]
	add	r0, r0, r1
	str	r0, [sp, #36]
	add	r2, r4, #1
	str	r2, [sp, #44]
	blt	.L_08138f10
	ldr	r3, [sp, #112]
	ldr	ip, [sp, #68]
	and	r1, r3, #65280
	mul	ip, r1, ip
	ldr	r0, [sp, #96]
	asr	r3, r0, #16
	mul	r2, r1, r3
	ldr	r1, [sp, #112]
	ldr	r3, [sp, #100]
	asr	r0, r1, #16
	lsrs	r1, r1, #23
	ldr	r1, [sp, #104]
	rsb	ip, ip, r3
	ldr	r3, [sp, #64]
	rsb	r2, r2, r1
	and	r2, r2, r3
	orr	r2, r2, ip, lsr #16
	beq	.L_08138e3c
	teq	r0, r5
	bpl	.L_08138f10
	cmp	r0, #0
	bge	.L_08138e30
	rsb	r3, r0, #0
	mov	r0, #0
	b	.L_08138e38
	.4byte	IwramSignedDivide
.L_08138e30:
	sub	r3, r0, #127
	mov	r0, #127
.L_08138e38:
	mla	r2, r3, r9, r2
.L_08138e3c:
	asr	r3, r7, #16
	mov	ip, r3
	lsrs	r1, r7, #23
	beq	.L_08138e58
	cmn	r3, #1
	movgt	ip, #128
	mvnle	ip, #0
.L_08138e58:
	rsb	r3, r5, r0
	cmp	r3, ip
	ldr	r3, [sp, #136]
	ldr	r1, [sp, #112]
	asr	r3, r3, #16
	str	r3, [sp, #16]
	ldr	r3, [sp, #124]
	add	r1, r1, r3
	str	r1, [sp, #48]
	ldr	r1, [sp, #108]
	ldr	r3, [sp, #104]
	add	r7, r7, r1
	ldr	r1, [sp, #116]
	add	r3, r3, r1
	str	r3, [sp, #40]
	ldr	r3, [sp, #100]
	ldr	r1, [sp, #120]
	str	r7, [sp, #32]
	add	r3, r3, r1
	str	r3, [sp, #36]
	add	r3, r4, #1
	str	r3, [sp, #44]
	beq	.L_08138f10
	and	r3, r4, #7
	asr	r1, r4, #3
	lsl	r1, r1, #10
	orr	r1, r1, r3, lsl #3
	ldr	r4, [sp, #140]
	cmp	r0, ip
	add	r1, r4, r1
	beq	.L_08138f10
.L_08138ed4:
	and	r3, sl, r2, lsr r8
	orr	r3, r3, r2, lsr #24
	ldrb	r7, [r6, r3]
	adds	r7, r7, lr
	bmi	.L_08138f00
	asr	r3, r0, #3
	and	r4, r0, #7
	orr	r3, r4, r3, lsl #6
	ldrb	r4, [r1, r3]
	cmp	r4, r7
	strblt	r7, [r1, r3]
.L_08138f00:
	add	r2, r2, r9
	add	r0, r0, r5
	cmp	r0, ip
	bne	.L_08138ed4
.L_08138f10:
	ldr	r0, [sp, #48]
	ldr	r7, [sp, #32]
	ldr	r1, [sp, #40]
	ldr	r2, [sp, #36]
	ldr	r4, [sp, #44]
	ldr	r3, [sp, #16]
	str	r0, [sp, #112]
	str	r1, [sp, #104]
	cmp	r4, r3
	str	r2, [sp, #100]
	bne	.L_08138d64
.L_08138f3c:
	asr	r3, r4, #7
	cmp	r3, #0
	bgt	.L_08138fa4
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #136]
	subs	r0, r0, r1, asr #16
	str	r0, [sp, #128]
	beq	.L_08138fa4
	ldr	r2, [sp, #20]
	lsl	sl, r1, #16
	ldr	r1, [sp, #128]
	str	r4, [sp, #4]
	ldr	r3, [pc, #44]
	rsb	r0, sl, r2
	mov	lr, pc
	bx	r3
	mov	r7, sl
	str	r0, [sp, #108]
	sub	r3, r7, #65536
	ldr	r0, [sp, #132]
	cmp	r5, #0
	ldr	r4, [sp, #4]
	movlt	r7, r3
	str	r0, [sp, #136]
	b	.L_08138d38
	.4byte	IwramSignedDivide
.L_08138fa4:
	ldr	r1, [sp, #56]
	str	r1, [sp, #152]
	ldr	r2, [r1]
	ldr	r8, [sp, #52]
	lsls	r3, r2, #8
	bne	.L_08138a5c
.L_08138fbc:
	add	sp, sp, #156
	pop	{r5, r6, r7, r8, r9, sl, lr}
	bx	lr
