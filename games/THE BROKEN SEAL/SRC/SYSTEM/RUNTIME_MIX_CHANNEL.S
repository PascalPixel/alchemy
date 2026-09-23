@ Channel end, block mixing and sample normalisation of the resident mixer.
@ It returns conditionally with movne pc, lr, and each time before its block
@ loop it copies two-word templates over the eight nop words of that loop
@ (add r1, pc, #52; sub r0, pc, #104; stm r1, {r6, r7}), so the loop runs
@ the copies and never the stored nops, which carry no credit.
@ IwramSoundNormalizeSample branches on overflow (adds r7, r7, r7; bvc) and
@ returns with bxvc lr. Rewriting its own loop is beyond both approved
@ compilers.
@ credit: handwritten — IWRAM runtime channel mixing
.syntax unified
	.thumb_func
	.set	IwramSoundMixEntry, 0x03000658
	.set	.L_audio_0ba0, 0x03000ba0
	.arm
	.set AlchemyLoadAddress, 0x08001118
	.set AlchemyRunAddress, 0x030009a8
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
AlchemyUncredited_080011cc:
	nop
	nop
AlchemyUncreditedEnd_080011cc:
	mlane	r0, fp, r6, r0
AlchemyUncredited_080011d8:
	nop
	nop
AlchemyUncreditedEnd_080011d8:
	mlane	r1, fp, r6, r1
AlchemyUncredited_080011e4:
	nop
	nop
AlchemyUncreditedEnd_080011e4:
	mlane	r7, fp, r6, r7
AlchemyUncredited_080011f0:
	nop
	nop
AlchemyUncreditedEnd_080011f0:
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
