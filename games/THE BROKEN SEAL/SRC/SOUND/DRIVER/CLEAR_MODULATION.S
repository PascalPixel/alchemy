@ Clears a track's modulation: zeroes the bytes at offsets 22 and 26 of the
@ track in r1 and sets the volume (12) or pitch (3) update bits in its flags
@ according to the modulation type at offset 24. It reuses the zero register
@ r2 for the type load and the flag bits, where old_agbcc gives each value its
@ own pseudo and allocates r0 (eight differing halfwords under the agbcc
@ route, four bytes longer under the game route). It lies inside the
@ MusicPlayer2000 driver's assembly object between RELEASE_KEY.S and
@ COMMAND_BYTE_READER.S; the setters in COMMAND_BYTE_SETTERS.S and the note
@ routine in HANDLE_NOTE.S call it.
@ The zero halfword of alignment at 0x080fa1c6 never runs and carries no
@ credit.
@ credit: library — MusicPlayer2000 sound driver assembly object, track modulation clear
.syntax unified
	.thumb
	.global MusicTrack_ClearModulationState
	.global Func_080fa1ac
	.thumb_func
MusicTrack_ClearModulationState:
Func_080fa1ac:
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
