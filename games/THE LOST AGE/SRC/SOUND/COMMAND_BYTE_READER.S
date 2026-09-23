@ Reads the next sequence command byte, advancing the track's pointer at
@ +0x40, and returns it in r3 rather than r0: a private return register the
@ setters in COMMAND_BYTE_SETTERS.S read after their bl. These 10 bytes equal
@ TBS SOUND/DRIVER/COMMAND_BYTE_READER.S (0x080fa1c8).
@ credit: library — TBS/TLA common sound command-byte reader and setters (reader)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081c1e64
	.global MusicTrack_ReadCommandByte
	.thumb_func
MusicTrack_ReadCommandByte:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	bx	lr
