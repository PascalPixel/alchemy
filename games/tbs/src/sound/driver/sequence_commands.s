@ C2: MusicPlayer2000 command family returns decoded bytes in r3 and multiple
@ nested-call setter entries return through ip. Source's full .global list
@ supplies the individual setter names.
.syntax unified
	.thumb
	.global Sound_LoadCommandTable
	.thumb_func
Sound_LoadCommandTable:
	mov	ip, lr
	movs	r1, #36
	ldr	r2, .L_080f9a80_source
.L_080f9a80_loop:
	ldr	r3, [r2, #0]
	bl	Sound_CheckReadAddress
	stmia	r0!, {r3}
	adds	r2, #4
	subs	r1, #1
	bgt.n	.L_080f9a80_loop
	bx	ip
AlchemyUncredited_080f9a96:
	.2byte 0

	.global Sound_ReadCheckedByte
	.thumb_func
Sound_ReadCheckedByte:
AlchemyUncreditedEnd_080f9a96:
	ldrb	r3, [r2, #0]
	.global Sound_CheckReadAddress
	.thumb_func
Sound_CheckReadAddress:
	push	{r0}
	lsrs	r0, r2, #25
	bne.n	.L_080f9a80_return
	ldr	r0, .L_080f9a80_source
	cmp	r2, r0
	bcc.n	.L_080f9a80_clear
	lsrs	r0, r2, #14
	beq.n	.L_080f9a80_return
.L_080f9a80_clear:
	movs	r3, #0
.L_080f9a80_return:
	pop	{r0}
	bx	lr
	.align 2
.L_080f9a80_source:
	.4byte Sound_CommandTableTemplate

	.global MusicTrack_ReadCheckedCommandByte
	.thumb_func
MusicTrack_ReadCheckedCommandByte:
	ldr	r2, [r1, #64]
	.global MusicTrack_ReadCheckedByteAt
	.thumb_func
MusicTrack_ReadCheckedByteAt:
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	b.n	Sound_CheckReadAddress
AlchemyUncredited_080f9abe:
	.2byte 0

.global MusicTrack_Goto
	.thumb_func
MusicTrack_Goto:
AlchemyUncreditedEnd_080f9abe:
	push	{lr}
.L_080f9ac0_jump:
	ldr	r2, [r1, #64]
	ldrb	r0, [r2, #3]
	lsls	r0, r0, #8
	ldrb	r3, [r2, #2]
	orrs	r0, r3
	lsls	r0, r0, #8
	ldrb	r3, [r2, #1]
	orrs	r0, r3
	lsls	r0, r0, #8
	bl	Sound_ReadCheckedByte
	orrs	r0, r3
	str	r0, [r1, #64]
	pop	{r0}
	bx	r0

.global MusicTrack_CallPattern
	.thumb_func
MusicTrack_CallPattern:
	ldrb	r2, [r1, #2]
	cmp	r2, #3
	bcs.n	.L_080f9ae0_full
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r2, [r1, #64]
	adds	r2, #4
	str	r2, [r3, #68]
	ldrb	r2, [r1, #2]
	adds	r2, #1
	strb	r2, [r1, #2]
	b.n	MusicTrack_Goto
.L_080f9ae0_full:
	b.n	MusicTrack_FinishSequence
AlchemyUncredited_080f9afa:
	.2byte 0

.global MusicTrack_ReturnFromPattern
	.thumb_func
MusicTrack_ReturnFromPattern:
AlchemyUncreditedEnd_080f9afa:
	ldrb	r2, [r1, #2]
	cmp	r2, #0
	beq.n	.L_080f9afc_0
	subs	r2, #1
	strb	r2, [r1, #2]
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r2, [r3, #68]
	str	r2, [r1, #64]
.L_080f9afc_0:
	bx	lr

	.global MusicTrack_Repeat
	.thumb_func
MusicTrack_Repeat:
	push	{lr}
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080f9b10_repeat
	adds	r2, #1
	str	r2, [r1, #64]
	b.n	.L_080f9ac0_jump
.L_080f9b10_repeat:
	ldrb	r3, [r1, #3]
	adds	r3, #1
	strb	r3, [r1, #3]
	mov	ip, r3
	bl	MusicTrack_ReadCheckedCommandByte
	cmp	ip, r3
	bcs.n	.L_080f9b10_advance
	b.n	.L_080f9ac0_jump
.L_080f9b10_advance:
	movs	r3, #0
	strb	r3, [r1, #3]
	adds	r2, #5
	str	r2, [r1, #64]
	pop	{r0}
	bx	r0
AlchemyUncredited_080f9b3e:
	.2byte 0

.global MusicTrack_SetPriorityFromCommand
	.thumb_func
MusicTrack_SetPriorityFromCommand:
AlchemyUncreditedEnd_080f9b3e:
	mov	ip, lr
	bl	MusicTrack_ReadCheckedCommandByte
	strb	r3, [r1, #29]
	bx	ip

AlchemyUncredited_080f9b4a:
	.balign 4, 0
.global MusicPlayer_SetTempoFromCommand
	.thumb_func
MusicPlayer_SetTempoFromCommand:
AlchemyUncreditedEnd_080f9b4a:
	mov	ip, lr
	bl	MusicTrack_ReadCheckedCommandByte
	lsls	r3, r3, #1
	strh	r3, [r0, #28]
	ldrh	r2, [r0, #30]
	muls	r3, r2
	lsrs	r3, r3, #8
	strh	r3, [r0, #32]
	bx	ip

.global MusicTrack_SetKeyOffsetFromCommand
	.thumb_func
MusicTrack_SetKeyOffsetFromCommand:
	mov	ip, lr
	bl	MusicTrack_ReadCheckedCommandByte
	strb	r3, [r1, #10]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip

AlchemyUncredited_080f9b72:
	.balign 4, 0
.global MusicTrack_SetVoiceFromCommand
	.thumb_func
MusicTrack_SetVoiceFromCommand:
AlchemyUncreditedEnd_080f9b72:
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
	bl	Sound_CheckReadAddress
	str	r3, [r1, #36]
	ldr	r3, [r2, #4]
	bl	Sound_CheckReadAddress
	str	r3, [r1, #40]
	ldr	r3, [r2, #8]
	bl	Sound_CheckReadAddress
	str	r3, [r1, #44]
	bx	ip

AlchemyUncredited_080f9ba2:
	.balign 4, 0
.global MusicTrack_SetVolumeFromCommand
	.thumb_func
MusicTrack_SetVolumeFromCommand:
AlchemyUncreditedEnd_080f9ba2:
	mov	ip, lr
	bl	MusicTrack_ReadCheckedCommandByte
	strb	r3, [r1, #18]
	ldrb	r3, [r1, #0]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip

AlchemyUncredited_080f9bb6:
	.balign 4, 0
.global MusicTrack_SetPanFromCommand
	.thumb_func
MusicTrack_SetPanFromCommand:
AlchemyUncreditedEnd_080f9bb6:
	mov	ip, lr
	bl	MusicTrack_ReadCheckedCommandByte
	subs	r3, #64
	strb	r3, [r1, #20]
	ldrb	r3, [r1, #0]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip

.global MusicTrack_SetBendFromCommand
	.thumb_func
MusicTrack_SetBendFromCommand:
	mov	ip, lr
	bl	MusicTrack_ReadCheckedCommandByte
	subs	r3, #64
	strb	r3, [r1, #14]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip

.global MusicTrack_SetBendRangeFromCommand
	.thumb_func
MusicTrack_SetBendRangeFromCommand:
	mov	ip, lr
	bl	MusicTrack_ReadCheckedCommandByte
	strb	r3, [r1, #15]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip

AlchemyUncredited_080f9bf2:
	.balign 4, 0
.global MusicTrack_SetLfoDelayFromCommand
	.thumb_func
MusicTrack_SetLfoDelayFromCommand:
AlchemyUncreditedEnd_080f9bf2:
	mov	ip, lr
	bl	MusicTrack_ReadCheckedCommandByte
	strb	r3, [r1, #27]
	bx	ip

AlchemyUncredited_080f9bfe:
	.balign 4, 0
.global MusicTrack_SetModulationTargetFromCommand
	.thumb_func
MusicTrack_SetModulationTargetFromCommand:
AlchemyUncreditedEnd_080f9bfe:
	mov	ip, lr
	bl	MusicTrack_ReadCheckedCommandByte
	ldrb	r0, [r1, #24]
	cmp	r0, r3
	beq.n	.L_080f9c00_0
	strb	r3, [r1, #24]
	ldrb	r3, [r1, #0]
	movs	r2, #15
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_080f9c00_0:
	bx	ip

.global MusicTrack_SetTuneFromCommand
	.thumb_func
MusicTrack_SetTuneFromCommand:
	mov	ip, lr
	bl	MusicTrack_ReadCheckedCommandByte
	subs	r3, #64
	strb	r3, [r1, #12]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip

.global MusicTrack_WriteSoundRegisterFromCommand
	.thumb_func
MusicTrack_WriteSoundRegisterFromCommand:
	mov	ip, lr
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	adds	r2, #1
	ldr	r0, [pc, #8]
	adds	r0, r0, r3
	bl	MusicTrack_ReadCheckedByteAt
	strb	r3, [r0, #0]
	bx	ip
	.4byte 0x04000060
