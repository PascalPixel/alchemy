@ A copy of the sequence command-byte reader in the leftover block of
@ LEFTOVER_HANDLE_NOTE.S. Its only callers are the two setters of
@ LEFTOVER_COMMAND_BYTE_SETTERS.S (bl at 0x081c0402 and 0x081c0416). It
@ advances the track's pointer at +0x40 and returns the byte in r3, a private
@ return register the setters read after their bl. These 10 bytes equal TBS
@ SOUND/DRIVER/COMMAND_BYTE_READER.S (0x080fa1c8) byte for byte.
@ credit: library — TBS/TLA common sound command-byte reader and setters (reader)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081c03f4
	.global MusicTrack_ReadCommandByteLeftover
	.thumb_func
MusicTrack_ReadCommandByteLeftover:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	bx	lr
