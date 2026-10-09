@ C2: MusicPlayer2000 ld_r3_tp_adr_i_unchecked takes track in r1 and returns
@ the decoded byte in r3 while updating r2/r3, rather than a standard r0
@ return.
.syntax unified
	.thumb

.global MusicTrack_ReadCommandByte
.thumb_func
MusicTrack_ReadCommandByte:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	bx	lr
