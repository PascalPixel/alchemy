@ C2: SoundDriver_ClearBuffer:
@ Four stmia r0! of zero registers have no paired load/copy; r4 is saved
@ through ip.
@ C2: SoundNote_Detach to MusicTrack_FinishSequence:
@ MusicPlayer2000 library assembly (public pret m4a_1.s: RealClearChain,
@ with ply_fine after it). Credited on library provenance, not on a
@ handwritten-instruction claim: both routines are ordinary Thumb, and neither
@ SoundNote_Detach nor MusicTrack_FinishSequence carries an instruction-level
@ tell.
@ C2: Sound_LoadCommandTable to MusicTrack_WriteSoundRegisterFromCommand:
@ MusicPlayer2000 library assembly (public pret m4a_1.s: ply_pend and the
@ command family around it). MusicTrack_ReturnFromPattern (ply_pend) is
@ ordinary Thumb, credited on library provenance, not on a
@ handwritten-instruction claim.
@ Its neighbours in this file do carry instruction-level tells:
@ Sound_LoadCommandTable and the setter entries keep the return address with
@ mov ip, lr across a bl and return with bx ip, never saving lr;
@ Sound_ReadCheckedByte falls through into Sound_CheckReadAddress, which
@ returns the decoded byte in r3 and preserves r0 with push {r0} / pop {r0}.
@ Source's full .global list supplies the individual setter names.
@ C2: Sound_TickDmaRestartTimer to SoundDriver_CallViaR3:
@ DMA timer branches on carry from lsls; MusicPlayer_Tick's library
@ dispatcher and call-via-r3 entry share the complete MusicPlayer2000 assembly
@ module.
@ C2: MusicTrack_Stop:
@ Nintendo MusicPlayer2000 TrackStop, checked against pret's public
@ m4a_1.s. This ordinary Thumb routine is library assembly, not claimed
@ handwritten game code.
@ C2: SoundDriver_CalculateStereoVolume:
@ Caller supplies the channel in r4 and track in r5; the routine reads them
@ without an argument move or save. Equivalent Nintendo MusicPlayer2000
@ ChnVolSetAsm is public in pret m4a_1.s.
@ C2: MusicTrack_HandleNote:
@ Nintendo MusicPlayer2000 ply_note, checked against pret's public m4a_1.s.
@ The frame and r8-r11 save/restore match the library; this is library
@ provenance rather than a handwritten game-code assertion.
@ C2: MusicTrack_ReleaseKey:
@ push saves r4/r5 without lr; Nintendo library ply_endtie.
@ C2: MusicTrack_ClearModulationState:
@ MusicPlayer2000 library assembly (public pret m4a_1.s: clear_modM).
@ Credited on library provenance, not on a handwritten-instruction claim:
@ the routine is ordinary Thumb, and it is the only one in this file.
@ C2: MusicTrack_ReadCommandByte:
@ MusicPlayer2000 ld_r3_tp_adr_i_unchecked takes track in r1 and returns
@ the decoded byte in r3 while updating r2/r3, rather than a standard r0
@ return.
@ C2: MusicTrack_SetLfoSpeedFromCommand to MusicTrack_SetModulationFromCommand:
@ Both nested-call entries return through ip saved from lr rather than a
@ procedure-standard return.
.syntax unified
	.thumb
	.global SoundDriver_ClearBuffer
	.thumb_func
SoundDriver_ClearBuffer:
	mov	ip, r4
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	mov	r4, ip
	bx	lr
AlchemyUncredited_080f9a2e:
	.2byte	0x0000
AlchemyUncreditedEnd_080f9a2e:
.syntax unified
	.thumb
	.global SoundNote_Detach
	.thumb_func
SoundNote_Detach:
	ldr	r3, [r0, #44]
	cmp	r3, #0
	beq.n	.L_080f9a4e
	ldr	r1, [r0, #52]
	ldr	r2, [r0, #48]
	cmp	r2, #0
	beq.n	.L_080f9a42
	str	r1, [r2, #52]
	b.n	.L_080f9a44
.L_080f9a42:
	str	r1, [r3, #32]
.L_080f9a44:
	cmp	r1, #0
	beq.n	.L_080f9a4a
	str	r2, [r1, #48]
.L_080f9a4a:
	movs	r1, #0
	str	r1, [r0, #44]
.L_080f9a4e:
	bx	lr
	.global MusicTrack_FinishSequence
	.thumb_func
MusicTrack_FinishSequence:
	push	{r4, r5, lr}
	adds	r5, r1, #0
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_080f9a74
.L_080f9a5a:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_080f9a68
	movs	r0, #64
	orrs	r1, r0
	strb	r1, [r4, #0]
.L_080f9a68:
	adds	r0, r4, #0
	bl	SoundNote_Detach
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_080f9a5a
.L_080f9a74:
	movs	r0, #0
	strb	r0, [r5, #0]
	pop	{r4, r5}
	pop	{r0}
	bx	r0
AlchemyUncredited_080f9a7e:
	.2byte	0x0000
AlchemyUncreditedEnd_080f9a7e:
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
.syntax unified
	.thumb
	.global Sound_TickDmaRestartTimer
	.thumb_func
Sound_TickDmaRestartTimer:
	ldr r0, [pc, #680]
	ldr r0, [r0, #0]
	ldr r2, [pc, #680]
	ldr r3, [r0, #0]
	subs r3, r3, r2
	cmp r3, #1
	bhi .L_080f9c84
	ldrb r1, [r0, #4]
	subs r1, #1
	strb r1, [r0, #4]
	bgt .L_080f9c84
	ldrb r1, [r0, #11]
	strb r1, [r0, #4]
	ldr r2, [pc, #40]
	ldr r1, [r2, #8]
	lsls r1, r1, #7
	bcc .L_080f9c6a
	ldr r1, [pc, #36]
	str r1, [r2, #8]
.L_080f9c6a:
	ldr r1, [r2, #20]
	lsls r1, r1, #7
	bcc .L_080f9c74
	ldr r1, [pc, #24]
	str r1, [r2, #20]
.L_080f9c74:
	movs r1, #4
	lsls r1, r1, #8
	strh r1, [r2, #10]
	strh r1, [r2, #22]
	movs r1, #182
	lsls r1, r1, #8
	strh r1, [r2, #10]
	strh r1, [r2, #22]
.L_080f9c84:
	bx lr
AlchemyUncredited_080f9c86:
	.balign 4, 0
	.global Update_Pool1
Update_Pool1:
AlchemyUncreditedEnd_080f9c86:
	.4byte 0x040000bc
	.4byte 0x84400004
.syntax unified
	.thumb
	.global MusicPlayer_Tick
	.thumb_func
MusicPlayer_Tick:
	ldr	r2, [pc, #608]
	ldr	r3, [r0, #52]
	cmp	r2, r3
	beq.n	.L_080f9c9a
	bx	lr
.L_080f9c9a:
	adds	r3, #1
	str	r3, [r0, #52]
	push	{r0, lr}
	ldr	r3, [r0, #56]
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [r0, #60]
	bl	.L1
.L0:
	pop	{r0}
	push	{r4, r5, r6, r7}
	mov	r4, r8
	mov	r5, r9
	mov	r6, sl
	mov	r7, fp
	push	{r4, r5, r6, r7}
	adds	r7, r0, #0
	ldr	r0, [r7, #4]
	cmp	r0, #0
	bge.n	.L2
	b.n	.L3
.L2:
	ldr	r0, [pc, #552]
	ldr	r0, [r0, #0]
	mov	r8, r0
	adds	r0, r7, #0
	bl	MusicPlayer_StepFade
	ldr	r0, [r7, #4]
	cmp	r0, #0
	bge.n	.L4
	b.n	.L3
.L4:
	ldrh	r0, [r7, #34]
	ldrh	r1, [r7, #32]
	adds	r0, r0, r1
	b.n	.L5
.L28:
	ldrb	r6, [r7, #8]
	ldr	r5, [r7, #44]
	movs	r3, #1
	movs	r4, #0
.L25:
	ldrb	r0, [r5, #0]
	movs	r1, #128
	tst	r1, r0
	bne.n	.L6
	b.n	.L7
.L6:
	mov	sl, r3
	orrs	r4, r3
	mov	fp, r4
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L8
.L11:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L9
	ldrb	r0, [r4, #16]
	cmp	r0, #0
	beq.n	.L10
	subs	r0, #1
	strb	r0, [r4, #16]
	bne.n	.L10
	movs	r0, #64
	orrs	r1, r0
	strb	r1, [r4, #0]
	b.n	.L10
.L9:
	adds	r0, r4, #0
	bl	AudioCommand_InvokeSlot34
.L10:
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L11
.L8:
	ldrb	r3, [r5, #0]
	movs	r0, #64
	tst	r0, r3
	beq.n	.L12
	adds	r0, r5, #0
	bl	AudioCommand_InvokeSlot35
	movs	r0, #128
	strb	r0, [r5, #0]
	movs	r0, #2
	strb	r0, [r5, #15]
	movs	r0, #64
	strb	r0, [r5, #19]
	movs	r0, #22
	strb	r0, [r5, #25]
	movs	r0, #1
	adds	r1, r5, #6
	strb	r0, [r1, #30]
	b.n	.L12
.L18:
	ldr	r2, [r5, #64]
	ldrb	r1, [r2, #0]
	cmp	r1, #128
	bcs.n	.L13
	ldrb	r1, [r5, #7]
	b.n	.L14
.L13:
	adds	r2, #1
	str	r2, [r5, #64]
	cmp	r1, #189
	bcc.n	.L14
	strb	r1, [r5, #7]
.L14:
	cmp	r1, #207
	bcc.n	.L15
	mov	r0, r8
	ldr	r3, [r0, #56]
	adds	r0, r1, #0
	subs	r0, #207
	adds	r1, r7, #0
	adds	r2, r5, #0
	bl	.L1
	b.n	.L12
.L15:
	cmp	r1, #176
	bls.n	.L16
	adds	r0, r1, #0
	subs	r0, #177
	strb	r0, [r7, #10]
	mov	r3, r8
	ldr	r3, [r3, #52]
	lsls	r0, r0, #2
	ldr	r3, [r3, r0]
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	.L1
	ldrb	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L17
	b.n	.L12
.L16:
	ldr	r0, [pc, #336]
	subs	r1, #128
	adds	r1, r1, r0
	ldrb	r0, [r1, #0]
	strb	r0, [r5, #1]
.L12:
	ldrb	r0, [r5, #1]
	cmp	r0, #0
	beq.n	.L18
	subs	r0, #1
	strb	r0, [r5, #1]
	ldrb	r1, [r5, #25]
	cmp	r1, #0
	beq.n	.L17
	ldrb	r0, [r5, #23]
	cmp	r0, #0
	beq.n	.L17
	ldrb	r0, [r5, #28]
	cmp	r0, #0
	beq.n	.L19
	subs	r0, #1
	strb	r0, [r5, #28]
	b.n	.L17
.L19:
	ldrb	r0, [r5, #26]
	adds	r0, r0, r1
	strb	r0, [r5, #26]
	adds	r1, r0, #0
	subs	r0, #64
	lsls	r0, r0, #24
	bpl.n	.L20
	lsls	r2, r1, #24
	asrs	r2, r2, #24
	b.n	.L21
.L20:
	movs	r0, #128
	subs	r2, r0, r1
.L21:
	ldrb	r0, [r5, #23]
	muls	r0, r2
	asrs	r2, r0, #6
	ldrb	r0, [r5, #22]
	eors	r0, r2
	lsls	r0, r0, #24
	beq.n	.L17
	strb	r2, [r5, #22]
	ldrb	r0, [r5, #0]
	ldrb	r1, [r5, #24]
	cmp	r1, #0
	bne.n	.L22
	movs	r1, #12
	b.n	.L23
.L22:
	movs	r1, #3
.L23:
	orrs	r0, r1
	strb	r0, [r5, #0]
.L17:
	mov	r3, sl
	mov	r4, fp
.L7:
	subs	r6, #1
	ble.n	.L24
	movs	r0, #80
	adds	r5, r5, r0
	lsls	r3, r3, #1
	b.n	.L25
.L24:
	ldr	r0, [r7, #12]
	adds	r0, #1
	str	r0, [r7, #12]
	cmp	r4, #0
	bne.n	.L26
	movs	r0, #128
	lsls	r0, r0, #24
	str	r0, [r7, #4]
	b.n	.L3
.L26:
	str	r4, [r7, #4]
	ldrh	r0, [r7, #34]
	subs	r0, #150
.L5:
	strh	r0, [r7, #34]
	cmp	r0, #150
	bcc.n	.L27
	b.n	.L28
.L27:
	ldrb	r2, [r7, #8]
	ldr	r5, [r7, #44]
.L37:
	ldrb	r0, [r5, #0]
	movs	r1, #128
	tst	r1, r0
	beq.n	.L29
	movs	r1, #15
	tst	r1, r0
	beq.n	.L29
	mov	r9, r2
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	MusicTrack_CalcOutput
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L30
.L36:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	bne.n	.L31
	adds	r0, r4, #0
	bl	AudioCommand_InvokeSlot34
	b.n	.L32
.L31:
	ldrb	r0, [r4, #1]
	movs	r6, #7
	ands	r6, r0
	ldrb	r3, [r5, #0]
	movs	r0, #3
	tst	r0, r3
	beq.n	.L33
	bl	SoundDriver_CalculateStereoVolume
	cmp	r6, #0
	beq.n	.L33
	ldrb	r0, [r4, #29]
	movs	r1, #1
	orrs	r0, r1
	strb	r0, [r4, #29]
.L33:
	ldrb	r3, [r5, #0]
	movs	r0, #12
	tst	r0, r3
	beq.n	.L32
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r2, r1, r0
	bpl.n	.L34
	movs	r2, #0
.L34:
	cmp	r6, #0
	beq.n	.L35
	mov	r0, r8
	ldr	r3, [r0, #48]
	adds	r1, r2, #0
	ldrb	r2, [r5, #9]
	adds	r0, r6, #0
	bl	.L1
	str	r0, [r4, #32]
	ldrb	r0, [r4, #29]
	movs	r1, #2
	orrs	r0, r1
	strb	r0, [r4, #29]
	b.n	.L32
.L35:
	adds	r1, r2, #0
	ldrb	r2, [r5, #9]
	ldr	r0, [r4, #36]
	bl	Pcm_KeyToFrequency
	str	r0, [r4, #32]
.L32:
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L36
.L30:
	ldrb	r0, [r5, #0]
	movs	r1, #240
	ands	r0, r1
	strb	r0, [r5, #0]
	mov	r2, r9
.L29:
	subs	r2, #1
	ble.n	.L3
	movs	r0, #80
	adds	r5, r5, r0
	bgt.n	.L37
.L3:
	ldr	r0, [pc, #24]
	str	r0, [r7, #52]
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r3}
.L1:
	.global SoundDriver_CallViaR3
SoundDriver_CallViaR3:
	bx	r3
AlchemyUncredited_080f9eea:
	.2byte	0x0000
AlchemyUncreditedEnd_080f9eea:
	.4byte Sound_ClockLengths
	.4byte 0x03007ff0
	.4byte 0x68736d53
.syntax unified
	.thumb
	.global MusicTrack_Stop
	.thumb_func
MusicTrack_Stop:
	push	{r4, r5, r6, lr}
	adds	r5, r1, #0
	ldrb	r1, [r5, #0]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L_080f9f30
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_080f9f2e
	movs	r6, #0
.L_080f9f0c:
	ldrb	r0, [r4, #0]
	cmp	r0, #0
	beq.n	.L_080f9f26
	ldrb	r0, [r4, #1]
	movs	r3, #7
	ands	r0, r3
	beq.n	.L_080f9f24
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #44]
	bl	SoundDriver_CallViaR3
.L_080f9f24:
	strb	r6, [r4, #0]
.L_080f9f26:
	str	r6, [r4, #44]
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_080f9f0c
.L_080f9f2e:
	str	r4, [r5, #32]
.L_080f9f30:
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
AlchemyUncredited_080f9f36:
	.2byte	0x0000
AlchemyUncreditedEnd_080f9f36:
	.4byte	0x03007ff0
.syntax unified
	.thumb
	.global SoundDriver_CalculateStereoVolume
	.thumb_func
SoundDriver_CalculateStereoVolume:
	ldrb	r1, [r4, #18]
	movs	r0, #20
	ldrsb	r2, [r4, r0]
	movs	r3, #128
	adds	r3, r3, r2
	muls	r3, r1
	ldrb	r0, [r5, #16]
	muls	r0, r3
	asrs	r0, r0, #14
	cmp	r0, #255
	bls.n	.L_tail0
	movs	r0, #255
.L_tail0:
	strb	r0, [r4, #2]
	movs	r3, #127
	subs	r3, r3, r2
	muls	r3, r1
	ldrb	r0, [r5, #17]
	muls	r0, r3
	asrs	r0, r0, #14
	cmp	r0, #255
	bls.n	.L_tail1
	movs	r0, #255
.L_tail1:
	strb	r0, [r4, #3]
	bx	lr
.syntax unified
	.thumb
	.global MusicTrack_HandleNote
	.thumb_func
MusicTrack_HandleNote:
	push	{r4, r5, r6, r7, lr}
	mov	r4, r8
	mov	r5, r9
	mov	r6, sl
	mov	r7, fp
	push	{r4, r5, r6, r7}
	sub	sp, #24
	str	r1, [sp, #0]
	adds	r5, r2, #0
	ldr	r1, [pc, #484]
	ldr	r1, [r1, #0]
	str	r1, [sp, #4]
	ldr	r1, [pc, #480]
	adds	r0, r0, r1
	ldrb	r0, [r0, #0]
	strb	r0, [r5, #4]
	ldr	r3, [r5, #64]
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_080f9fb2
	strb	r0, [r5, #5]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_080f9fb0
	strb	r0, [r5, #6]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_080f9fb0
	ldrb	r1, [r5, #4]
	adds	r1, r1, r0
	strb	r1, [r5, #4]
	adds	r3, #1
.L_080f9fb0:
	str	r3, [r5, #64]
.L_080f9fb2:
	movs	r0, #0
	str	r0, [sp, #20]
	adds	r4, r5, #0
	adds	r4, #36
	ldrb	r2, [r4, #0]
	movs	r0, #192
	tst	r0, r2
	beq.n	.L_080fa004
	ldrb	r3, [r5, #5]
	movs	r0, #64
	tst	r0, r2
	beq.n	.L_080f9fd2
	ldr	r1, [r5, #44]
	adds	r1, r1, r3
	ldrb	r0, [r1, #0]
	b.n	.L_080f9fd4
.L_080f9fd2:
	adds	r0, r3, #0
.L_080f9fd4:
	lsls	r1, r0, #1
	adds	r1, r1, r0
	lsls	r1, r1, #2
	ldr	r0, [r5, #40]
	adds	r1, r1, r0
	mov	r9, r1
	mov	r6, r9
	ldrb	r1, [r6, #0]
	movs	r0, #192
	tst	r0, r1
	beq.n	.L_080f9fec
	b.n	.L_080fa152
.L_080f9fec:
	movs	r0, #128
	tst	r0, r2
	beq.n	.L_080fa008
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L_080fa000
	subs	r1, #192
	lsls	r1, r1, #1
	str	r1, [sp, #20]
.L_080fa000:
	ldrb	r3, [r6, #1]
	b.n	.L_080fa008
.L_080fa004:
	mov	r9, r4
	ldrb	r3, [r5, #5]
.L_080fa008:
	str	r3, [sp, #8]
	ldr	r6, [sp, #0]
	ldrb	r1, [r6, #9]
	ldrb	r0, [r5, #29]
	adds	r0, r0, r1
	cmp	r0, #255
	bls.n	.L_080fa018
	movs	r0, #255
.L_080fa018:
	str	r0, [sp, #16]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	movs	r6, #7
	ands	r6, r0
	str	r6, [sp, #12]
	beq.n	.L_080fa058
	ldr	r0, [sp, #4]
	ldr	r4, [r0, #28]
	cmp	r4, #0
	bne.n	.L_080fa030
	b.n	.L_080fa152
.L_080fa030:
	subs	r6, #1
	lsls	r0, r6, #6
	adds	r4, r4, r0
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_080fa0ac
	movs	r0, #64
	tst	r0, r1
	bne.n	.L_080fa0ac
	ldrb	r1, [r4, #19]
	ldr	r0, [sp, #16]
	cmp	r1, r0
	bcc.n	.L_080fa0ac
	beq.n	.L_080fa050
	b.n	.L_080fa152
.L_080fa050:
	ldr	r0, [r4, #44]
	cmp	r0, r5
	bcs.n	.L_080fa0ac
	b.n	.L_080fa152
.L_080fa058:
	ldr	r6, [sp, #16]
	adds	r7, r5, #0
	movs	r2, #0
	mov	r8, r2
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #6]
	adds	r4, #80
.L_080fa066:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_080fa0ac
	movs	r0, #64
	tst	r0, r1
	beq.n	.L_080fa080
	cmp	r2, #0
	bne.n	.L_080fa084
	adds	r2, #1
	ldrb	r6, [r4, #19]
	ldr	r7, [r4, #44]
	b.n	.L_080fa09e
.L_080fa080:
	cmp	r2, #0
	bne.n	.L_080fa0a0
.L_080fa084:
	ldrb	r0, [r4, #19]
	cmp	r0, r6
	bcs.n	.L_080fa090
	adds	r6, r0, #0
	ldr	r7, [r4, #44]
	b.n	.L_080fa09e
.L_080fa090:
	bhi.n	.L_080fa0a0
	ldr	r0, [r4, #44]
	cmp	r0, r7
	bls.n	.L_080fa09c
	adds	r7, r0, #0
	b.n	.L_080fa09e
.L_080fa09c:
	bcc.n	.L_080fa0a0
.L_080fa09e:
	mov	r8, r4
.L_080fa0a0:
	adds	r4, #64
	subs	r3, #1
	bgt.n	.L_080fa066
	mov	r4, r8
	cmp	r4, #0
	beq.n	.L_080fa152
.L_080fa0ac:
	adds	r0, r4, #0
	bl	AudioCommand_InvokeSlot34
	movs	r1, #0
	str	r1, [r4, #48]
	ldr	r3, [r5, #32]
	str	r3, [r4, #52]
	cmp	r3, #0
	beq.n	.L_080fa0c0
	str	r4, [r3, #48]
.L_080fa0c0:
	str	r4, [r5, #32]
	str	r5, [r4, #44]
	ldrb	r0, [r5, #27]
	strb	r0, [r5, #28]
	cmp	r0, r1
	beq.n	.L_080fa0d2
	adds	r1, r5, #0
	bl	MusicTrack_ClearModulationState
.L_080fa0d2:
	ldr	r0, [sp, #0]
	adds	r1, r5, #0
	bl	MusicTrack_CalcOutput
	ldr	r0, [r5, #4]
	str	r0, [r4, #16]
	ldr	r0, [sp, #16]
	strb	r0, [r4, #19]
	ldr	r0, [sp, #8]
	strb	r0, [r4, #8]
	ldr	r0, [sp, #20]
	strb	r0, [r4, #20]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	strb	r0, [r4, #1]
	ldr	r7, [r6, #4]
	str	r7, [r4, #36]
	ldr	r0, [r6, #8]
	str	r0, [r4, #4]
	ldrh	r0, [r5, #30]
	strh	r0, [r4, #12]
	bl	SoundDriver_CalculateStereoVolume
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r3, r1, r0
	bpl.n	.L_080fa10c
	movs	r3, #0
.L_080fa10c:
	ldr	r6, [sp, #12]
	cmp	r6, #0
	beq.n	.L_080fa13a
	mov	r6, r9
	ldrb	r0, [r6, #2]
	strb	r0, [r4, #30]
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	bne.n	.L_080fa126
	movs	r0, #112
	tst	r0, r1
	bne.n	.L_080fa128
.L_080fa126:
	movs	r1, #8
.L_080fa128:
	strb	r1, [r4, #31]
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	ldr	r0, [sp, #12]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #48]
	bl	SoundDriver_CallViaR3
	b.n	.L_080fa144
.L_080fa13a:
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	adds	r0, r7, #0
	bl	Pcm_KeyToFrequency
.L_080fa144:
	str	r0, [r4, #32]
	movs	r0, #128
	strb	r0, [r4, #0]
	ldrb	r1, [r5, #0]
	movs	r0, #240
	ands	r0, r1
	strb	r0, [r5, #0]
.L_080fa152:
	add	sp, #24
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r0}
	bx	r0
AlchemyUncredited_080fa162:
	.2byte	0x0000
AlchemyUncreditedEnd_080fa162:
	.4byte	0x03007ff0
	.4byte	Sound_ClockLengths
.syntax unified
	.thumb
	.global MusicTrack_ReleaseKey
	.thumb_func
MusicTrack_ReleaseKey:
	push	{r4, r5}
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	cmp	r3, #128
	bcs.n	.L_080fa17e
	strb	r3, [r1, #5]
	adds	r2, #1
	str	r2, [r1, #64]
	b.n	.L_080fa180
.L_080fa17e:
	ldrb	r3, [r1, #5]
.L_080fa180:
	ldr	r1, [r1, #32]
	cmp	r1, #0
	beq.n	.L_080fa1a8
	movs	r4, #131
	movs	r5, #64
.L_080fa18a:
	ldrb	r2, [r1, #0]
	tst	r2, r4
	beq.n	.L_080fa1a2
	tst	r2, r5
	bne.n	.L_080fa1a2
	ldrb	r0, [r1, #17]
	cmp	r0, r3
	bne.n	.L_080fa1a2
	movs	r0, #64
	orrs	r2, r0
	strb	r2, [r1, #0]
	b.n	.L_080fa1a8
.L_080fa1a2:
	ldr	r1, [r1, #52]
	cmp	r1, #0
	bne.n	.L_080fa18a
.L_080fa1a8:
	pop	{r4, r5}
	bx	lr
.syntax unified
	.thumb
	.global MusicTrack_ClearModulationState
	.thumb_func
MusicTrack_ClearModulationState:
	movs	r2, #0
	strb	r2, [r1, #22]
	strb	r2, [r1, #26]
	ldrb	r2, [r1, #24]
	cmp	r2, #0
	bne.n	.L_080fa1bc
	movs	r2, #12
	b.n	.L_080fa1be
.L_080fa1bc:
	movs	r2, #3
.L_080fa1be:
	ldrb	r3, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	lr
AlchemyUncredited_080fa1c6:
	.2byte	0x0000
AlchemyUncreditedEnd_080fa1c6:
.syntax unified
	.section .text.command_byte_reader,"ax",%progbits
	.thumb

.global MusicTrack_ReadCommandByte
.thumb_func
MusicTrack_ReadCommandByte:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	bx	lr
.syntax unified
	.section .text.command_byte_setters,"ax",%progbits
	.thumb

	.global MusicTrack_SetLfoSpeedFromCommand
	.thumb_func
MusicTrack_SetLfoSpeedFromCommand:
	mov	ip, lr
	bl	MusicTrack_ReadCommandByte
	strb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_080fa1d4_0
	bl	MusicTrack_ClearModulationState
.L_080fa1d4_0:
	bx	ip
AlchemyUncredited_080fa1e6:
	.balign 4, 0

AlchemyUncreditedEnd_080fa1e6:
	.global MusicTrack_SetModulationFromCommand
	.thumb_func
MusicTrack_SetModulationFromCommand:
	mov	ip, lr
	bl	MusicTrack_ReadCommandByte
	strb	r3, [r1, #23]
	cmp	r3, #0
	bne.n	.L_080fa1e8_0
	bl	MusicTrack_ClearModulationState
.L_080fa1e8_0:
	bx	ip
AlchemyUncredited_080fa1fa:
	.2byte 0
AlchemyUncreditedEnd_080fa1fa:
