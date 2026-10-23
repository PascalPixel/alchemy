@ C2: Thumb-to-ARM mode switch within the live mixer; fp holds volume/sample
@ data and code is patched through PC-relative stores later in the file.
.syntax unified
	.thumb_func
	.arm

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
	beq.n	.L_audio_skip_channel
	lsls	r0, r6, #25
	bcc.n	.L_audio_envelope
	bmi.n	.L_audio_stop_channel
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
	beq.n	.L_audio_attack
	movs	r0, #16
	orrs	r6, r0
	b.n	.L_audio_attack
.L_audio_envelope:
	ldrb	r5, [r4, #9]
	lsls	r0, r6, #29
	bpl.n	.L_audio_release
	ldrb	r0, [r4, #13]
	subs	r0, #1
	strb	r0, [r4, #13]
	bhi.n	.L_audio_volume
.L_audio_stop_channel:
	movs	r0, #0
	strb	r0, [r4, #0]
.L_audio_skip_channel:
	b.n	IwramSoundMixNextChannel
.L_audio_release:
	lsls	r0, r6, #25
	bpl.n	.L_audio_decay
	ldrb	r0, [r4, #7]
	muls	r5, r0
	lsrs	r5, r5, #8
	ldrb	r0, [r4, #12]
	cmp	r5, r0
	bhi.n	.L_audio_volume
.L_audio_echo:
	ldrb	r5, [r4, #12]
	cmp	r5, #0
	beq.n	.L_audio_stop_channel
	movs	r0, #4
	orrs	r6, r0
	b.n	.L_audio_store_status
.L_audio_decay:
	movs	r2, #3
	ands	r2, r6
	cmp	r2, #2
	bne.n	.L_audio_attack_check
	ldrb	r0, [r4, #5]
	muls	r5, r0
	lsrs	r5, r5, #8
	ldrb	r0, [r4, #6]
	cmp	r5, r0
	bhi.n	.L_audio_volume
	adds	r5, r0, #0
	beq.n	.L_audio_echo
	b.n	.L_audio_next_phase
.L_audio_attack_check:
	cmp	r2, #3
	bne.n	.L_audio_volume
.L_audio_attack:
	ldrb	r0, [r4, #4]
	adds	r5, r5, r0
	cmp	r5, #255
	bcc.n	.L_audio_store_status
	movs	r5, #255
.L_audio_next_phase:
	subs	r6, #1
.L_audio_store_status:
	strb	r6, [r4, #0]
.L_audio_volume:
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
	beq.n	.L_audio_loop_length
	adds	r3, #8
	ldmia	r3!, {r0, r1}
	adds	r3, r0, r3
	str	r3, [sp, #12]
	subs	r0, r1, r0
.L_audio_loop_length:
	str	r0, [sp, #16]
	ldr	r5, [pc, #12]
	ldr	r2, [r4, #24]
	ldr	r3, [r4, #40]
	ldrb	r0, [r4, #1]
	lsrs	r1, r0, #4
	add	r1, pc, #8
	bx	r1
AlchemyUncredited_sound_mixer_1:
	.2byte	0x0000
.L_audio_workspace_pointer:
AlchemyUncreditedEnd_sound_mixer_1:
	.4byte IwramSoundMixWorkspace

	.global IwramSoundMixArm
	.arm
IwramSoundMixArm:
	mov	r8, #352
.L_audio_pack_volumes:
	orr	fp, sl, fp, lsl #16
.L_audio_select_mixer:
	bcs	IwramSoundMixBlock
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
	bne	IwramSoundSynthWave
	sub	ip, ip, r6
	lsrs	fp, fp, #1
	adc	fp, fp, #32768
	bic	fp, fp, #65280
	mov	r1, r7
	umlal	r1, r0, r4, r8
	lsr	r1, r1, #23
	orr	r0, r1, r0, lsl #9
	cmp	r2, r0
	ble	IwramSoundMixInterpolated
	sub	r2, r2, r0
	ldr	sl, [pc, #324]
	add	sl, sl, r0
	cmp	sl, sp
	add	sl, r3, r0
	add	r9, pc, #516
	stm	r9, {r2, sl, sp}
	cmpcc	r0, #1024
	bcs	.L_audio_pick_kernel
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
.L_audio_pick_kernel:
	subs	r4, r4, #8388608
	lslpl	fp, fp, #1
	add	r0, pc, #252, 0
	addpl	r0, r0, #24, 0
	subspl	r4, r4, #8388608
	addpl	r0, r0, #24, 0
	addpl	r4, r4, #8388608
	ldr	r2, [pc, #228]
	cmp	r0, r2
	beq	.L_audio_kernel_ready
	str	r0, [pc, #216]
	stmfd	sp!, {r6}
	ldm	r0, {r0, r1, r2, r6, r9, sl}
	add	lr, pc, #36, 0
.L_audio_patch_kernel:
	stmia	lr!, {r0, r1}
	add	lr, lr, #4, 0
	stm	lr, {r2, r6, r9, sl}
	add	lr, lr, #24, 0
	adds	r5, r5, #1073741824
	bcc	.L_audio_patch_kernel
	ldmfd	sp!, {r6}
.L_audio_kernel_ready:
	mov	r2, #-16777216
.L_audio_mix_four:
	ldm	r5, {r0, r1, sl, lr}
	mul	r9, r7, ip
AlchemyUncredited_sound_mixer_2:
	.4byte 0xfedcba98
	.4byte 0xfedcba98
AlchemyUncreditedEnd_sound_mixer_2:
	mlane	r0, fp, r9, r0
AlchemyUncredited_sound_mixer_3:
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
AlchemyUncreditedEnd_sound_mixer_3:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_sound_mixer_4:
	.4byte 0xfedcba98
	.4byte 0xfedcba98
AlchemyUncreditedEnd_sound_mixer_4:
	mlane	r1, fp, r9, r1
AlchemyUncredited_sound_mixer_5:
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
AlchemyUncreditedEnd_sound_mixer_5:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_sound_mixer_6:
	.4byte 0xfedcba98
	.4byte 0xfedcba98
AlchemyUncreditedEnd_sound_mixer_6:
	mlane	sl, fp, r9, sl
AlchemyUncredited_sound_mixer_7:
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
AlchemyUncreditedEnd_sound_mixer_7:
	bic	r7, r7, r2, asr #1
	mulne	r9, r7, ip
AlchemyUncredited_sound_mixer_8:
	.4byte 0xfedcba98
	.4byte 0xfedcba98
AlchemyUncreditedEnd_sound_mixer_8:
	mlane	lr, fp, r9, lr
AlchemyUncredited_sound_mixer_9:
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
	.4byte 0xfedcba98
AlchemyUncreditedEnd_sound_mixer_9:
	bic	r7, r7, r2, asr #1
	stmia	r5!, {r0, r1, sl, lr}
	subs	r8, r8, #4, 0
	bgt	.L_audio_mix_four
	add	ip, pc, #212, 0
	ldm	ip, {r2, r3, sp}
	b	IwramSoundMixDone
.L_audio_stack_floor:
	.4byte 0x03007900
