@ MIDI key to frequency: clamps the key, reads its octave and semitone from a
@ key table with add r4, pc and the semitone's frequency pair from a second
@ table with add r3, pc, then switches to ARM with a mid-function bx pc,
@ where umlalne tests the flags a Thumb lsls set before the switch, and
@ returns from ARM after umull. It saves r4 in ip without the stack and its
@ clamp block sits before its entry. Neither approved compiler emits an
@ add rd, pc address form or changes state inside a function. Both tables
@ (96 and 180 bytes from 0x081c0624) are data in MIDI_KEY_FREQUENCY_TABLES.S;
@ TBS keeps its key table as data at 0x080fb830. The zero halfword before
@ the entry (0x081c05ee) and the nop after the bx pc (0x081c0612) never run
@ and carry no credit. TLA revision of the sound driver's key-to-frequency
@ step.
@ credit: handwritten — MIDI key to frequency (TLA revision)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081c05e8
.L_081c05e8:
	movs	r1, #178
	movs	r2, #255
	b.n	.L_081c05f8
AlchemyUncredited_081c05ee:
	.2byte	0x0000
AlchemyUncreditedEnd_081c05ee:
	.global SoundDriver_MidiKeyToFrequency
	.thumb_func
SoundDriver_MidiKeyToFrequency:
	ldr	r0, [r0, #4]
	mov	ip, r4
	cmp	r1, #179
	bge.n	.L_081c05e8
.L_081c05f8:
	add	r4, pc, #136
	ldrb	r4, [r1, r4]
	lsls	r1, r4, #28
	lsrs	r1, r1, #25
	add	r3, pc, #32
	adds	r1, r1, r3
	ldmia	r1, {r1, r3}
	lsrs	r4, r4, #4
	lsrs	r1, r4
	lsrs	r3, r4
	movs	r4, #0
	lsls	r2, r2, #24
	bx	pc
AlchemyUncredited_081c0612:
	nop
	.arm
AlchemyUncreditedEnd_081c0612:
	umlalne	r4, r1, r3, r2
	umull	r4, r0, r1, r0
	mov	r4, ip
	bx	lr
