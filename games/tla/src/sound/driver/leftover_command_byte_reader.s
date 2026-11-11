@ Why assembly: MusicTrack_ReadCommandByteLeftover takes track in r1 and
@ returns the decoded byte in r3 while updating r2/r3, rather than a standard
@ r0 return. Its bytes are ld_r3_tp_adr_i_unchecked's (command_byte_reader.s).
.syntax unified
	.thumb
	.global MusicTrack_ReadCommandByteLeftover
	.thumb_func
MusicTrack_ReadCommandByteLeftover:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	bx	lr
	.2byte	0x0000
