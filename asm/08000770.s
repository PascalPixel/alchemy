@ 起動時に03000000へ転送される常駐処理。
@ 割込み入口、固定小数点演算、初期化処理を実行時番地で再構築する。
.syntax unified
	.arm
	.align 2

	.global IwramIrqMain
IwramIrqMain:
	mov	r3, #0x04000000
	ldr	r2, [r3, #0x200]!
	ldrh	r1, [r3, #8]
	mrs	r0, SPSR
	push	{r0, r1, r2, r3, r4, r5, lr}
	mov	r0, #1
	strh	r0, [r3, #8]
	and	r1, r2, r2, lsr #16
	ands	r0, r1, #2
	ldrne	ip, IwramIrqHBlank
	bne	.L_irq_dispatch
	ands	r0, r1, #1
	ldrne	ip, IwramIrqVBlank
	bne	.L_irq_dispatch
	ands	r0, r1, #4
	ldrne	ip, IwramIrqVCount
	bne	.L_irq_dispatch
	ands	r0, r1, #0x40
	ldrne	ip, IwramIrqTimer3
	bne	.L_irq_dispatch
	ands	r0, r1, #0x80
	ldrne	ip, IwramIrqSerial
	bne	.L_irq_dispatch
	ands	r0, r1, #0x10
	ldrne	ip, IwramIrqTimer1
	bne	.L_irq_dispatch
	ands	r0, r1, #0x20
	ldrne	ip, IwramIrqTimer2
	bne	.L_irq_dispatch
	ands	r0, r1, #0x1000
	ldrne	ip, IwramIrqKeypad
	bne	.L_irq_dispatch
	ands	r0, r1, #0x2000
	ldr	ip, IwramIrqGamePak
.L_irq_dispatch:
	strh	r0, [r3, #2]
	mov	r1, #0x20c0
	bic	r2, r2, r0
	and	r1, r1, r2
	strh	r1, [r3]
	mrs	r3, CPSR
	bic	r3, r3, #0xdf
	orr	r3, r3, #0x1f
	msr	CPSR_fc, r3
	mov	r5, lr
	add	lr, pc, #0
	bx	ip
	mov	lr, r5
	mrs	r3, CPSR
	bic	r3, r3, #0xdf
	orr	r3, r3, #0x92
	msr	CPSR_fc, r3
	pop	{r0, r1, r2, r3, r4, r5, lr}
	strh	r2, [r3]
	strh	r1, [r3, #8]
	msr	SPSR_fc, r0
	bx	lr

@ 起動後に常駐側から割込み先十四本を転送する表。
IwramIrqVBlank:
	.4byte 0
IwramIrqHBlank:
	.4byte 0
IwramIrqVCount:
	.4byte 0
IwramIrqTimer0:
	.4byte 0
IwramIrqTimer1:
	.4byte 0
IwramIrqTimer2:
	.4byte 0
IwramIrqTimer3:
	.4byte 0
IwramIrqSerial:
	.4byte 0
IwramIrqDma0:
	.4byte 0
IwramIrqDma1:
	.4byte 0
IwramIrqDma2:
	.4byte 0
IwramIrqDma3:
	.4byte 0
IwramIrqKeypad:
	.4byte 0
IwramIrqGamePak:
	.4byte 0

	.global IwramMulQ16ReturnIp
IwramMulQ16ReturnIp:
	smull	r2, r0, r1, r0
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	add	ip, ip, #1
	bx	ip

	.global IwramMulQ16
IwramMulQ16:
	smull	r2, r0, r1, r0
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	bx	lr

	.global IwramRatioMulQ14
IwramRatioMulQ14:
	stmfd	sp!, {lr}
	mov	r4, r1
	mov	r1, r0
	mov	r0, #0x40000000
	bl	IwramSignedDivide
	smull	r3, r0, r4, r0
	ldmfd	sp!, {lr}
	lsl	r0, r0, #18
	orr	r0, r0, r3, lsr #14
	bx	lr

	.global IwramClearWords
IwramClearWords:
	mov	r2, #0
	push	{r5, r6, r7, r8, r9}
	mov	r3, r2
	mov	r4, r2
	mov	r5, r2
	mov	r6, r2
	mov	r7, r2
	mov	r8, r2
	mov	r9, r2
	ands	ip, r1, #0xe0
	rsb	ip, ip, #0xe0
	add	pc, pc, ip, lsr #3
.L_clear_256:
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	subs	r1, r1, #0x100
	bpl	.L_clear_256
	ands	r1, r1, #0x1c
	beq	.L_clear_done
.L_clear_word:
	stmia	r0!, {r2}
	subs	r1, r1, #4
	bgt	.L_clear_word
.L_clear_done:
	pop	{r5, r6, r7, r8, r9}
	bx	lr

	.global IwramSqrt
IwramSqrt:
	movs	r1, r0
	mov	r0, #0
	bxmi	lr
	mov	r2, #0x8000
.L_sqrt_coarse:
	add	r0, r0, r2
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2
	add	r0, r0, r2, lsr #1
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2, lsr #1
	lsrs	r2, r2, #2
	bne	.L_sqrt_coarse
	bx	lr

	.global IwramExpandBitRuns
IwramExpandBitRuns:
	add	ip, r0, #28
.L_expand_halfword:
	mov	r4, r1
	ldrh	r3, [r0], #2
	bic	r3, r3, #3
	lsls	r3, r3, #17
	bcc	.L_expand_zero
.L_expand_store:
	strb	r2, [r4], #1
.L_expand_shift:
	lsls	r3, r3, #1
	bcs	.L_expand_store
.L_expand_zero:
	add	r4, r4, #1
	bne	.L_expand_shift
	add	r1, r1, #16
	cmp	r0, ip
	bne	.L_expand_halfword
	bx	lr

	.global IwramTransformVector
IwramTransformVector:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	ldm	r0, {r2, r3, r4}
	add	r0, pc, #0xf0
	ldmia	r0!, {r5, r6, r7}
	smull	r8, r9, r2, r5
	smull	sl, fp, r2, r6
	smull	ip, lr, r2, r7
	ldmia	r0!, {r5, r6, r7}
	smlal	r8, r9, r3, r5
	smlal	sl, fp, r3, r6
	smlal	ip, lr, r3, r7
	ldmia	r0!, {r5, r6, r7}
	smlal	r8, r9, r4, r5
	smlal	sl, fp, r4, r6
	smlal	ip, lr, r4, r7
	lsr	r8, r8, #16
	orr	r8, r8, r9, lsl #16
	lsr	sl, sl, #16
	orr	sl, sl, fp, lsl #16
	lsr	ip, ip, #16
	orr	ip, ip, lr, lsl #16
	ldmia	r0!, {r5, r6, r7}
	add	r5, r5, r8
	add	r6, r6, sl
	add	r7, r7, ip
	stmia	r1!, {r5, r6, r7}
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr

	.global IwramTransformMatrix
IwramTransformMatrix:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	sub	sp, sp, #0x24
	mov	r4, sp
.L_matrix_row:
	add	r1, pc, #0x7c
	ldmia	r0!, {r2, r3}
	ldmia	r1!, {r5, r6, r7}
	smull	r8, r9, r5, r2
	smull	sl, fp, r6, r2
	smull	ip, lr, r7, r2
	ldmia	r1!, {r5, r6, r7}
	smlal	r8, r9, r5, r3
	smlal	sl, fp, r6, r3
	smlal	ip, lr, r7, r3
	ldmia	r1!, {r5, r6, r7}
	ldr	r2, [r0], #4
	smlal	r8, r9, r5, r2
	smlal	sl, fp, r6, r2
	smlal	ip, lr, r7, r2
	lsr	r8, r8, #16
	orr	r8, r8, r9, lsl #16
	lsr	sl, sl, #16
	orr	sl, sl, fp, lsl #16
	lsr	ip, ip, #16
	orr	ip, ip, lr, lsl #16
	sub	r2, r4, sp
	cmp	r2, #0x24
	stmiane	r4!, {r8, sl, ip}
	bne	.L_matrix_row
	ldmia	r1!, {r5, r6, r7}
	add	ip, ip, r7
	add	fp, sl, r6
	add	sl, r8, r5
	pop	{r0, r2, r3, r4, r5, r6, r7, r8, r9}
	stmdb	r1, {r0, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr

@ 三行の単位変換係数と平行移動成分。
.L_unit_matrix:
	.4byte 0x00010000, 0, 0, 0
	.4byte 0x00010000, 0, 0, 0
	.4byte 0x00010000, 0, 0, 0

@ コンパイラ実行時系と同型の符号付き除算。
	.global IwramSignedDivide
IwramSignedDivide:
	eor	ip, r0, r1
	movs	r2, r1
	rsbmi	r2, r2, #0
	movs	r1, r0
	rsbmi	r1, r1, #0
	lsrs	r0, ip, #32
	bcc	.L_divide_core
	mov	ip, lr
	bl	.L_divide_core
	rsb	r0, r0, #0
	bx	ip

@ コンパイラ実行時系と同型の符号付き剰余。
	.global IwramSignedRemainder
IwramSignedRemainder:
	stmfd	sp!, {lr}
	eor	ip, r0, r1
	movs	r2, r1
	rsbmi	r2, r2, #0
	movs	r1, r0
	rsbmi	r1, r1, #0
	mov	r0, #0
	bl	.L_divide_core
	mov	r0, r1
	movs	ip, ip
	rsbmi	r0, r0, #0
	ldmfd	sp!, {lr}
	bx	lr

@ コンパイラ実行時系と同型の符号なし剰余。
	.global IwramUnsignedRemainder
IwramUnsignedRemainder:
	mov	ip, lr
	bl	IwramUnsignedDivide
	mov	r0, r1
	bx	ip

@ コンパイラ実行時系と同型の符号なし除算。
	.global IwramUnsignedDivide
IwramUnsignedDivide:
	mov	r2, r1
	mov	r1, r0
	mov	r0, #0
.L_divide_core:
	rsbs	r3, r2, r1, lsr #28
	bcc	.L_divide_24
	rsbs	r3, r2, r1, lsr #31
	orrcs	r0, r0, #0x80000000
	subcs	r1, r1, r2, lsl #31
	rsbs	r3, r2, r1, lsr #30
	orrcs	r0, r0, #0x40000000
	subcs	r1, r1, r2, lsl #30
	rsbs	r3, r2, r1, lsr #29
	orrcs	r0, r0, #0x20000000
	subcs	r1, r1, r2, lsl #29
	rsbs	r3, r2, r1, lsr #28
	orrcs	r0, r0, #0x10000000
	subcs	r1, r1, r2, lsl #28
.L_divide_24:
	rsbs	r3, r2, r1, lsr #24
	bcc	.L_divide_20
	rsbs	r3, r2, r1, lsr #27
	orrcs	r0, r0, #0x08000000
	subcs	r1, r1, r2, lsl #27
	rsbs	r3, r2, r1, lsr #26
	orrcs	r0, r0, #0x04000000
	subcs	r1, r1, r2, lsl #26
	rsbs	r3, r2, r1, lsr #25
	orrcs	r0, r0, #0x02000000
	subcs	r1, r1, r2, lsl #25
	rsbs	r3, r2, r1, lsr #24
	orrcs	r0, r0, #0x01000000
	subcs	r1, r1, r2, lsl #24
.L_divide_20:
	rsbs	r3, r2, r1, lsr #20
	bcc	.L_divide_16
	rsbs	r3, r2, r1, lsr #23
	orrcs	r0, r0, #0x00800000
	subcs	r1, r1, r2, lsl #23
	rsbs	r3, r2, r1, lsr #22
	orrcs	r0, r0, #0x00400000
	subcs	r1, r1, r2, lsl #22
	rsbs	r3, r2, r1, lsr #21
	orrcs	r0, r0, #0x00200000
	subcs	r1, r1, r2, lsl #21
	rsbs	r3, r2, r1, lsr #20
	orrcs	r0, r0, #0x00100000
	subcs	r1, r1, r2, lsl #20
.L_divide_16:
	rsbs	r3, r2, r1, lsr #16
	bcc	.L_divide_12
	rsbs	r3, r2, r1, lsr #19
	orrcs	r0, r0, #0x00080000
	subcs	r1, r1, r2, lsl #19
	rsbs	r3, r2, r1, lsr #18
	orrcs	r0, r0, #0x00040000
	subcs	r1, r1, r2, lsl #18
	rsbs	r3, r2, r1, lsr #17
	orrcs	r0, r0, #0x00020000
	subcs	r1, r1, r2, lsl #17
	rsbs	r3, r2, r1, lsr #16
	orrcs	r0, r0, #0x00010000
	subcs	r1, r1, r2, lsl #16
.L_divide_12:
	rsbs	r3, r2, r1, lsr #12
	bcc	.L_divide_8
	rsbs	r3, r2, r1, lsr #15
	orrcs	r0, r0, #0x00008000
	subcs	r1, r1, r2, lsl #15
	rsbs	r3, r2, r1, lsr #14
	orrcs	r0, r0, #0x00004000
	subcs	r1, r1, r2, lsl #14
	rsbs	r3, r2, r1, lsr #13
	orrcs	r0, r0, #0x00002000
	subcs	r1, r1, r2, lsl #13
	rsbs	r3, r2, r1, lsr #12
	orrcs	r0, r0, #0x00001000
	subcs	r1, r1, r2, lsl #12
.L_divide_8:
	rsbs	r3, r2, r1, lsr #8
	bcc	.L_divide_4
	rsbs	r3, r2, r1, lsr #11
	orrcs	r0, r0, #0x00000800
	subcs	r1, r1, r2, lsl #11
	rsbs	r3, r2, r1, lsr #10
	orrcs	r0, r0, #0x00000400
	subcs	r1, r1, r2, lsl #10
	rsbs	r3, r2, r1, lsr #9
	orrcs	r0, r0, #0x00000200
	subcs	r1, r1, r2, lsl #9
	rsbs	r3, r2, r1, lsr #8
	orrcs	r0, r0, #0x00000100
	subcs	r1, r1, r2, lsl #8
.L_divide_4:
	rsbs	r3, r2, r1, lsr #4
	bcc	.L_divide_low
	rsbs	r3, r2, r1, lsr #7
	orrcs	r0, r0, #0x80
	subcs	r1, r1, r2, lsl #7
	rsbs	r3, r2, r1, lsr #6
	orrcs	r0, r0, #0x40
	subcs	r1, r1, r2, lsl #6
	rsbs	r3, r2, r1, lsr #5
	orrcs	r0, r0, #0x20
	subcs	r1, r1, r2, lsl #5
	rsbs	r3, r2, r1, lsr #4
	orrcs	r0, r0, #0x10
	subcs	r1, r1, r2, lsl #4
.L_divide_low:
	rsbs	r3, r2, r1
	bxcc	lr
	rsbs	r3, r2, r1, lsr #3
	orrcs	r0, r0, #8
	subcs	r1, r1, r2, lsl #3
	rsbs	r3, r2, r1, lsr #2
	orrcs	r0, r0, #4
	subcs	r1, r1, r2, lsl #2
	rsbs	r3, r2, r1, lsr #1
	orrcs	r0, r0, #2
	subcs	r1, r1, r2, lsl #1
	rsbs	r3, r2, r1
	orrcs	r0, r0, #1
	subcs	r1, r1, r2
	bx	lr

@ 制御語と後方参照を展開する常駐解凍処理。
	.global IwramDecompress
IwramDecompress:
	ldrb	r2, [r0], #1
	ldrb	r3, [r0], #1
	orrs	r2, r2, r3, lsl #8
	bxeq	lr
	push	{r5, r6, r7}
	mov	r6, r0
	add	r2, r2, r0
	sub	r2, r2, #2
.L_decompress_flag_reload:
	ldrb	r3, [r2], #1
	orr	r3, r3, #0x100
.L_decompress_flag:
	lsrs	r3, r3, #1
	bcc	.L_decompress_backref
	beq	.L_decompress_flag_reload
	ldrb	r4, [r0], #1
	subs	r5, r4, #0xdf
	addhi	r1, r1, r5
	strbls	r4, [r1], #1
	b	.L_decompress_flag
.L_decompress_backref:
	ldrb	r4, [r2], #1
	ldrb	ip, [r2], #1
	orrs	r4, ip, r4, lsl #8
	beq	.L_decompress_done
	bic	ip, r4, #0xf000
	sub	ip, r6, ip
	lsrs	r4, r4, #12
	ldrbeq	r4, [r2], #1
	addeq	r4, r4, #16
	add	r4, r4, #1
.L_decompress_copy:
	ldrb	r5, [ip], #1
	subs	r7, r5, #0xdf
	addhi	r1, r1, r7
	strbls	r5, [r1], #1
	subs	r4, r4, #1
	bpl	.L_decompress_copy
	b	.L_decompress_flag
.L_decompress_done:
	pop	{r5, r6, r7}
	mov	r0, #0
	bx	lr

@ 音源ごとの状態更新と標本合成を行う常駐音響処理。
@ サム入口からアーム合成核へ切り替わる。由来は命令列だけでは確定しない。
	.thumb
	.align 2
	.global IwramSoundMixEntry
	.thumb
IwramSoundMixEntry:
	str	r0, [sp, #4]
	ldr	r3, [r4, #36]
	ldrb	r6, [r4, #0]
	movs	r0, #199
	tst	r0, r6
	beq.n	.L_audio_069a
	lsls	r0, r6, #25
	bcc.n	.L_audio_0688
	bmi.n	.L_audio_0696
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
	beq.n	.L_audio_06d6
	movs	r0, #16
	orrs	r6, r0
	b.n	.L_audio_06d6
.L_audio_0688:
	ldrb	r5, [r4, #9]
	lsls	r0, r6, #29
	bpl.n	.L_audio_069c
	ldrb	r0, [r4, #13]
	subs	r0, #1
	strb	r0, [r4, #13]
	bhi.n	.L_audio_06e4
.L_audio_0696:
	movs	r0, #0
	strb	r0, [r4, #0]
.L_audio_069a:
	b.n	.L_audio_0adc
.L_audio_069c:
	lsls	r0, r6, #25
	bpl.n	.L_audio_06b8
	ldrb	r0, [r4, #7]
	muls	r5, r0
	lsrs	r5, r5, #8
	ldrb	r0, [r4, #12]
	cmp	r5, r0
	bhi.n	.L_audio_06e4
.L_audio_06ac:
	ldrb	r5, [r4, #12]
	cmp	r5, #0
	beq.n	.L_audio_0696
	movs	r0, #4
	orrs	r6, r0
	b.n	.L_audio_06e2
.L_audio_06b8:
	movs	r2, #3
	ands	r2, r6
	cmp	r2, #2
	bne.n	.L_audio_06d2
	ldrb	r0, [r4, #5]
	muls	r5, r0
	lsrs	r5, r5, #8
	ldrb	r0, [r4, #6]
	cmp	r5, r0
	bhi.n	.L_audio_06e4
	adds	r5, r0, #0
	beq.n	.L_audio_06ac
	b.n	.L_audio_06e0
.L_audio_06d2:
	cmp	r2, #3
	bne.n	.L_audio_06e4
.L_audio_06d6:
	ldrb	r0, [r4, #4]
	adds	r5, r5, r0
	cmp	r5, #255
	bcc.n	.L_audio_06e2
	movs	r5, #255
.L_audio_06e0:
	subs	r6, #1
.L_audio_06e2:
	strb	r6, [r4, #0]
.L_audio_06e4:
	strb	r5, [r4, #9]
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
	beq.n	.L_audio_0706
	adds	r3, #8
	ldmia	r3!, {r0, r1}
	adds	r3, r0, r3
	str	r3, [sp, #12]
	subs	r0, r1, r0
.L_audio_0706:
	str	r0, [sp, #16]
	ldr	r5, [pc, #12]
	ldr	r2, [r4, #24]
	ldr	r3, [r4, #40]
	ldrb	r0, [r4, #1]
	lsrs	r1, r0, #4
	add	r1, pc, #8
	bx	r1
	movs r0, r0
@ 合成作業域の先頭番地。
.L_audio_workspace_pointer:
	.4byte 0x03000df8

@ 標本補間と四語単位の混合を行うアーム合成核。
	.global IwramSoundMixArm
	.arm
IwramSoundMixArm:
	mov	r8, #352
.L_audio_0720:
	orr	fp, sl, fp, lsl #16
.L_audio_0724:
	bcs	.L_audio_pcm_block
	push	{r4, r9, ip}
	movs	r2, r2
	orreq	r0, r0, #64, 0
	strbeq	r0, [r4, #1]
	add	r4, r4, #28, 0
	ldm	r4, {r7, lr}
	mul	r4, ip, lr
	ldrsb	r6, [r3], #1
	ldrsb	ip, [r3]
	ands	r0, r0, #64, 0
	bne	.L_audio_wave_mode
	sub	ip, ip, r6
	lsrs	fp, fp, #1
	adc	fp, fp, #32768
	bic	fp, fp, #65280
	mov	r1, r7
	umlal	r1, r0, r4, r8
	lsr	r1, r1, #23
	orr	r0, r1, r0, lsl #9
	cmp	r2, r0
	ble	.L_audio_091c
	sub	r2, r2, r0
	ldr	sl, [pc, #324]
	add	sl, sl, r0
	cmp	sl, sp
	add	sl, r3, r0
	add	r9, pc, #516
	stm	r9, {r2, sl, sp}
	cmpcc	r0, #1024
	bcs	.L_audio_07c8
	bic	r1, r3, #3, 0
	mov	r9, #67108864
	add	r9, r9, #212, 0
	add	r0, r0, #7, 0
	lsr	r0, r0, #2
	sub	sp, sp, r0, lsl #2
	and	r3, r3, #3, 0
	add	r3, r3, sp
	orr	lr, r0, #-2080374784
	stm	r9, {r1, sp, lr}
.L_audio_07c8:
	subs	r4, r4, #8388608
	lslpl	fp, fp, #1
	add	r0, pc, #252, 0
	addpl	r0, r0, #24, 0
	subspl	r4, r4, #8388608
	addpl	r0, r0, #24, 0
	addpl	r4, r4, #8388608
	ldr	r2, [pc, #228]
	cmp	r0, r2
	beq	.L_audio_081c
	str	r0, [pc, #216]
	stmfd	sp!, {r6}
	ldm	r0, {r0, r1, r2, r6, r9, sl}
	add	lr, pc, #36, 0
.L_audio_0800:
	stmia	lr!, {r0, r1}
	add	lr, lr, #4, 0
	stm	lr, {r2, r6, r9, sl}
	add	lr, lr, #24, 0
	adds	r5, r5, #1073741824
	bcc	.L_audio_0800
	ldmfd	sp!, {r6}
.L_audio_081c:
	mov	r2, #-16777216
.L_audio_0820:
	ldm	r5, {r0, r1, sl, lr}
	mul	r9, r7, ip
@ 実行時に補間命令へ置換される自己書換え雛形。
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	mlane	r0, fp, r9, r0
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	mlane	r1, fp, r9, r1
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	mlane	sl, fp, r9, sl
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	mlane	lr, fp, r9, lr
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	bic	r7, r7, r2, asr #1
	stmia	r5!, {r0, r1, sl, lr}
	subs	r8, r8, #4, 0
	bgt	.L_audio_0820
	add	ip, pc, #212, 0
	ldm	ip, {r2, r3, sp}
	b	.L_audio_096c
.L_audio_stack_floor:
	.4byte 0x03007900
.L_audio_template_cache:
	.4byte 0x00000000
	asr	r9, r9, #22
	adds	r9, r9, r6, lsl #1
	adds	r7, r7, r4
	addpl	r6, ip, r6
	ldrsbpl	ip, [r3, #1]!
	subspl	ip, ip, r6
	adds	r9, r6, r9, asr #23
	add	r6, ip, r6
	adds	r7, r7, r4
	ldrsbpl	r6, [r3, #1]!
	ldrsb	ip, [r3, #1]!
	subs	ip, ip, r6
	adds	r9, r6, r9, asr #23
	add	r7, r7, r4
	lsr	ip, r7, #23
	ldrsb	r6, [r3, ip]!
	ldrsb	ip, [r3, #1]!
	subs	ip, ip, r6
.L_audio_091c:
	add	r5, r5, r8, lsl #2
.L_audio_0920:
	mul	r9, r7, ip
	asr	r9, r9, #22
	adds	r9, r9, r6, lsl #1
	ldrne	r0, [r5, -r8, lsl #2]
	mlane	r0, fp, r9, r0
	strne	r0, [r5, -r8, lsl #2]
	add	r7, r7, r4
	lsrs	r9, r7, #23
	beq	.L_audio_0964
	subs	r2, r2, r7, lsr #23
	blle	.L_audio_refill_source
	subs	r9, r9, #1, 0
	addeq	r6, ip, r6
	ldrsbne	r6, [r3, r9]!
	ldrsb	ip, [r3, #1]!
	sub	ip, ip, r6
	bic	r7, r7, #1065353216
.L_audio_0964:
	subs	r8, r8, #1, 0
	bgt	.L_audio_0920
.L_audio_096c:
	sub	r3, r3, #1, 0
	pop	{r4, r9, ip}
	str	r7, [r4, #28]
	b	.L_audio_0acc
.L_audio_refill_source:
	add	r3, sp, #24, 0
	ldm	r3, {r3, r6}
	cmp	r6, #0, 0
	rsbne	r9, r2, #0, 0
	addne	r2, r6, r2
	addne	pc, lr, #8, 0
	pop	{r4, r9, ip}
	b	.L_audio_09bc
@ 終端分岐間の予約語。
	.space 0x0c, 0
.L_audio_channel_end:
	ldr	r2, [sp, #24]
	movs	r6, r2
	ldrne	r3, [sp, #20]
	movne	pc, lr
	pop	{r4, r9}
.L_audio_09bc:
	strb	r6, [r4]
	b	.L_audio_0ad4
.L_audio_09c4:
	lsls	r6, sl, #24
	asrs	r6, r6, #24
.L_audio_09cc:
	lsls	r6, sl, #16
	asrs	r6, r6, #24
.L_audio_09d4:
	lsls	r6, sl, #8
	asrs	r6, r6, #24
.L_audio_09dc:
	asrs	r6, sl, #24
.L_audio_09e0:
	ldm	r3!, {sl}
	lsls	r6, sl, #24
.L_audio_09e8:
	asrs	r6, r6, #24
	lsls	r6, sl, #16
.L_audio_09f0:
	asrs	r6, r6, #24
	lsls	r6, sl, #8
.L_audio_09f8:
	asrs	r6, r6, #24
.L_audio_pcm_block:
	push	{r4, r9}
.L_audio_0a00:
	mov	lr, r2
	cmp	r2, r8
	movgt	lr, r8
	sub	lr, lr, #1, 0
	lsrs	lr, lr, #2
	beq	.L_audio_0a9c
	sub	r8, r8, lr, lsl #2
	sub	r2, r2, lr, lsl #2
	add	r1, pc, #52, 0
	sub	r0, pc, #104, 0
	lsl	r9, r3, #30
	add	r0, r0, r9, lsr #27
	ldm	r0!, {r6, r7, r9, sl}
	stm	r1, {r6, r7}
	add	r1, r1, #12, 0
	stm	r1, {r9, sl}
	add	r1, r1, #12, 0
	ldm	r0, {r6, r7, r9, sl}
	stm	r1, {r6, r7}
	add	r1, r1, #12, 0
	stm	r1, {r9, sl}
	ldm	r3!, {sl}
.L_audio_0a58:
	ldm	r5, {r0, r1, r7, r9}
	nop
	nop
	mlane	r0, fp, r6, r0
	nop
	nop
	mlane	r1, fp, r6, r1
	nop
	nop
	mlane	r7, fp, r6, r7
	nop
	nop
	mlane	r9, fp, r6, r9
	stmia	r5!, {r0, r1, r7, r9}
	subs	lr, lr, #1, 0
	bne	.L_audio_0a58
	sub	r3, r3, #4, 0
.L_audio_0a9c:
	mov	r1, #4, 0
.L_audio_0aa0:
	ldr	r0, [r5]
	ldrsb	r6, [r3], #1
	mla	r0, fp, r6, r0
	str	r0, [r5], #4
	subs	r2, r2, #1, 0
	bleq	.L_audio_channel_end
	subs	r1, r1, #1, 0
	bgt	.L_audio_0aa0
	subs	r8, r8, #4, 0
	bgt	.L_audio_0a00
	pop	{r4, r9}
.L_audio_0acc:
	str	r2, [r4, #24]
	str	r3, [r4, #40]
.L_audio_0ad4:
	add	r0, pc, #1, 0
	bx	r0

@ 音源反復を進め、全音源の処理後にアーム初期化部へ渡す。
	.thumb
.L_audio_0adc:
	ldr	r0, [sp, #4]
	subs	r0, #1
	ble.n	.L_audio_0ae6
	adds	r4, #64
	b.n	IwramSoundMixEntry
.L_audio_0ae6:
	ldr	r5, [sp, #8]
	add	r0, pc, #116
	bx	r0

@ 符号付き標本の振幅形式をそろえる補助処理。
	.arm
	.global IwramSoundNormalizeSample
	.global IwramSoundNormalizeChannels
	.global IwramSoundRenderFrame
IwramSoundNormalizeSample:
	ror	r7, r6, #16
	adds	r7, r7, r7
	bvc	.L_audio_0b04
	orr	r6, r6, #16384
	bic	r6, r6, #16320
	submi	r6, r6, #64, 0
.L_audio_0b04:
	adds	r7, r6, r6
	bxvc	lr
	orr	r6, r6, #1073741824
	bic	r6, r6, #1069547520
	submi	r6, r6, #4194304
	bx	lr
IwramSoundNormalizeChannels:
	mov	r6, r1
	bl	IwramSoundNormalizeSample
	mov	r1, r6
	mov	r6, r2
	bl	IwramSoundNormalizeSample
	mov	r2, r6
	mov	r6, r3
	bl	IwramSoundNormalizeSample
	mov	r3, r6
	mov	r6, r4
	bl	IwramSoundNormalizeSample
	mov	r4, r6
	mov	lr, #-2147483648
	orr	lr, lr, lr, lsr #16
	str	lr, [pc]
	b	.L_audio_0ba0
.L_audio_sign_mask:
	.4byte 0x00000000
IwramSoundRenderFrame:
	add	r0, pc, #656
	mov	r9, #352
	mov	r8, #-16777216
	orr	r8, r8, r8, lsr #16
	mov	sl, #-2147483648
	orr	lr, sl, sl, lsr #16
.L_audio_0b78:
	ldm	r0, {r1, r2, r3, r4}
	eor	r6, r1, r1, lsl #1
	ands	r6, r6, lr
	eoreq	r6, r2, r2, lsl #1
	andseq	r6, r6, lr
	eoreq	r6, r3, r3, lsl #1
	andseq	r6, r6, lr
	eoreq	r6, r4, r4, lsl #1
	andseq	r6, r6, lr
	bne	IwramSoundNormalizeChannels
.L_audio_0ba0:
	and	r6, r8, r1, ror #15
	and	r7, r8, r2, ror #15
	orr	r6, r7, r6, lsr #8
	and	fp, r8, r3, ror #15
	and	r7, r8, r4, ror #15
	orr	r7, r7, fp, lsr #8
	bic	fp, r6, sl, asr #15
	orr	fp, fp, r7, lsl #16
	and	r7, r7, sl, asr #15
	orr	r7, r7, r6, lsr #16
	ldr	ip, [r5, #1584]
	str	fp, [r5, #1584]
	ldr	fp, [r5]
	str	r7, [r5], #4
	asrs	r6, r4, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40, 0
	ror	r4, r4, #16
	asrs	r7, r4, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40, 0
	adds	r4, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r2, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40, 0
	ror	r2, r2, #16
	asrs	r7, r2, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40, 0
	adds	r2, r7, r6, lsl #16
	ror	fp, fp, #8
	ror	ip, ip, #8
	asrs	r6, r3, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40, 0
	ror	r3, r3, #16
	asrs	r7, r3, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40, 0
	adds	r3, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r1, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40, 0
	ror	r1, r1, #16
	asrs	r7, r1, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40, 0
	adds	r1, r7, r6, lsl #16
	stmia	r0!, {r1, r2, r3, r4}
	subs	r9, r9, #4, 0
	bgt	.L_audio_0b78
	add	sp, sp, #28, 0
	pop	{r8, r9, sl, fp}
	pop	{r5, r6, r7, lr}
	bx	lr
.L_audio_wave_mode:
	cmp	ip, #0, 0
	bne	.L_audio_noise_mode
	ldrb	r6, [r3, #2]
	add	r2, r2, r6, lsl #24
	ldrb	r6, [r3, #4]
	adds	r6, r2, r6, lsl #24
	mvnmi	r6, r6
	lsr	sl, r6, #8
	ldrb	r1, [r3, #3]
	ldrb	r0, [r3, #1]
	lsl	r0, r0, #24
	mla	r6, sl, r1, r0
.L_audio_0cbc:
	ldm	r5, {r0, r1, sl, lr}
	cmp	r7, r6
	addcc	r0, r0, fp, lsl #6
	subcs	r0, r0, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	r1, r1, fp, lsl #6
	subcs	r1, r1, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	sl, sl, fp, lsl #6
	subcs	sl, sl, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	lr, lr, fp, lsl #6
	subcs	lr, lr, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	stmia	r5!, {r0, r1, sl, lr}
	subs	r8, r8, #4, 0
	bgt	.L_audio_0cbc
	b	.L_audio_096c
.L_audio_noise_mode:
	subs	ip, ip, #1, 0
	bne	.L_audio_square_mode
	mov	r6, #768
	lsr	fp, fp, #1
	bic	fp, fp, #65280
	mov	ip, #112, 0
.L_audio_0d28:
	ldm	r5, {r0, r1, sl, lr}
	adds	r7, r7, r4, lsl #3
	rsb	r9, ip, r7, lsr #24
	lsl	r6, r7, #1
	sub	r9, r9, r6, lsr #27
	adds	r2, r9, r2, asr #1
	mlane	r0, fp, r2, r0
	adds	r7, r7, r4, lsl #3
	rsb	r9, ip, r7, lsr #24
	lsl	r6, r7, #1
	sub	r9, r9, r6, lsr #27
	adds	r2, r9, r2, asr #1
	mlane	r1, fp, r2, r1
	adds	r7, r7, r4, lsl #3
	rsb	r9, ip, r7, lsr #24
	lsl	r6, r7, #1
	sub	r9, r9, r6, lsr #27
	adds	r2, r9, r2, asr #1
	mlane	sl, fp, r2, sl
	adds	r7, r7, r4, lsl #3
	rsb	r9, ip, r7, lsr #24
	lsl	r6, r7, #1
	sub	r9, r9, r6, lsr #27
	adds	r2, r9, r2, asr #1
	mlane	lr, fp, r2, lr
	stmia	r5!, {r0, r1, sl, lr}
	subs	r8, r8, #4, 0
	bgt	.L_audio_0d28
	b	.L_audio_096c
.L_audio_square_mode:
	mov	r6, #128, 0
	mov	ip, #384
.L_audio_0da4:
	ldm	r5, {r0, r1, sl, lr}
	adds	r7, r7, r4, lsl #3
	rsbpl	r9, r6, r7, asr #23
	submi	r9, ip, r7, lsr #23
	mla	r0, fp, r9, r0
	adds	r7, r7, r4, lsl #3
	rsbpl	r9, r6, r7, asr #23
	submi	r9, ip, r7, lsr #23
	mla	r1, fp, r9, r1
	adds	r7, r7, r4, lsl #3
	rsbpl	r9, r6, r7, asr #23
	submi	r9, ip, r7, lsr #23
	mla	sl, fp, r9, sl
	adds	r7, r7, r4, lsl #3
	rsbpl	r9, r6, r7, asr #23
	submi	r9, ip, r7, lsr #23
	mla	lr, fp, r9, lr
	stmia	r5!, {r0, r1, sl, lr}
	subs	r8, r8, #4, 0
	bgt	.L_audio_0da4
	b	.L_audio_096c

@ 合成中に命令雛形と一時標本を置く常駐作業域。
.L_audio_workspace:
	.space 0x590, 0

@ 二百五十六バイト単位で語列を転送する常駐処理。
	.global IwramCopyWords
IwramCopyWords:
	push	{r5, r6, r7, r8, r9}
	ands	ip, r2, #224, 0
	rsb	ip, ip, #240, 0
	add	pc, pc, ip, lsr #2
.L_audio_1398:
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	subs	r2, r2, #256
	bpl	.L_audio_1398
	ands	r2, r2, #28, 0
	beq	.L_audio_13f8
.L_audio_13e8:
	ldm	r1!, {r3}
	stmia	r0!, {r3}
	subs	r2, r2, #4, 0
	bgt	.L_audio_13e8
.L_audio_13f8:
	pop	{r5, r6, r7, r8, r9}
	bx	lr
