@ C2: MusicPlayer2000's shared call_r3 helper supplies the bx r3 hook entry.
@ Mixer-frame setup transfers into a shared live-stack IWRAM mixer rather
@ than returning through an ordinary call frame.
.syntax unified
.text
	.thumb
	.global SoundDriver_EnterMixer
	.thumb_func
SoundDriver_EnterMixer:
	ldr	r0, [pc, #108]
	ldr	r0, [r0, #0]
	ldr	r2, [pc, #100]
	ldr	r3, [r0, #0]
	cmp	r2, r3
	beq SoundDriver_StartMixFrame
	bx	lr
	.global MusicDriver_CallHook
	.thumb_func
MusicDriver_CallHook:
	bx	r3
.syntax unified
	.thumb
	.global SoundDriver_StartMixFrame
	.thumb_func
SoundDriver_StartMixFrame:
	push	{r5, r6, r7, lr}
	mov	r1, r8
	mov	r2, r9
	mov	r3, sl
	mov	r4, fp
	push	{r0, r1, r2, r3, r4}
	sub	sp, #24
	ldr	r3, [r0, #32]
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [r0, #36]
	bl	MusicDriver_CallHook
	ldr	r0, [sp, #24]
.L0:
	ldr	r3, [r0, #40]
	bl	MusicDriver_CallHook
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #16]
	str	r3, [sp, #0]
	ldr	r5, [pc, #40]
	adds	r5, r5, r0
	ldrb	r4, [r0, #4]
	subs	r7, r4, #1
	bls.n	.L1
	ldrb	r1, [r0, #11]
	subs	r1, r1, r7
	adds	r2, r3, #0
	muls	r2, r1
	adds	r5, r5, r2
.L1:
	str	r5, [sp, #8]
	ldr	r4, [sp, #24]
	ldr	r0, [r4, #20]
	mov	r9, r0
	ldr	r0, [r4, #24]
	mov	ip, r0
	ldrb	r0, [r4, #6]
	adds	r4, #80
	ldr	r3, [pc, #0]
	bx	r3
	.4byte IwramSoundMixEntry
	.4byte 0x00000350
AlchemyUncredited_08000764:
	.4byte 0x04000006
AlchemyUncreditedEnd_08000764:
	.4byte 0x68736d53
	.4byte 0x03007ff0
