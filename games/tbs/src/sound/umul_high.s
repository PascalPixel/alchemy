@ C2: Audio_UmulHigh32:
@ Thumb entry switches into ARM in the same function before umull; Nintendo
@ library umul3232H32.
@ C2: AudioEngine_RunMixerTick:
@ Nintendo MusicPlayer2000 SoundMain entry sets up one frame then bx
@ transfers into SoundDriver_Mixer, which consumes its live stack and saved
@ register state.
.syntax unified
	.thumb
	.global Audio_UmulHigh32
	.thumb_func
Audio_UmulHigh32:
	add	r2, pc, #0
	bx	r2
	.arm
	umull	r2, r3, r0, r1
	add	r0, r3, #0
	bx	lr
.syntax unified
	.thumb
	.global AudioEngine_RunMixerTick
	.thumb_func
AudioEngine_RunMixerTick:
	ldr	r0, [pc, #104]
	ldr	r0, [r0, #0]
	ldr	r2, [pc, #104]
	ldr	r3, [r0, #0]
	cmp	r2, r3
	beq.n	.L0
	bx	lr
.L0:
	adds	r3, #1
	str	r3, [r0, #0]
	push	{r4, r5, r6, r7, lr}
	mov	r1, r8
	mov	r2, r9
	mov	r3, sl
	mov	r4, fp
	push	{r0, r1, r2, r3, r4}
	sub	sp, #24
	ldrb	r1, [r0, #12]
	cmp	r1, #0
	beq.n	.L1
	ldr	r2, [pc, #80]
	ldrb	r2, [r2, #0]
	cmp	r2, #160
	bcs.n	.L2
	adds	r2, #228
.L2:
	adds	r1, r1, r2
.L1:
	str	r1, [sp, #20]
	ldr	r3, [r0, #32]
	cmp	r3, #0
	beq.n	.L3
	ldr	r0, [r0, #36]
	bl	Mixer_CallViaR3Rom
	ldr	r0, [sp, #24]
.L3:
	ldr	r3, [r0, #40]
	bl	Mixer_CallViaR3Rom
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #16]
	mov	r8, r3
	ldr	r5, [pc, #44]
	adds	r5, r5, r0
	ldrb	r4, [r0, #4]
	subs	r7, r4, #1
	bls.n	.L4
	ldrb	r1, [r0, #11]
	subs	r1, r1, r7
	mov	r2, r8
	muls	r2, r1
	adds	r5, r5, r2
.L4:
	str	r5, [sp, #8]
	ldr	r6, [pc, #24]
	ldr	r3, [pc, #12]
	bx	r3
AlchemyUncredited_umul_high_1:
	.2byte	0x0000
AlchemyUncreditedEnd_umul_high_1:
	.4byte 0x03007ff0
	.4byte 0x68736d53
	.4byte SoundDriver_Mixer
	.4byte 0x04000006
	.4byte 0x00000350
	.4byte 0x00000630
