@ Why assembly: MusicPlayer2000's ld_r3_tp_adr_i_unchecked takes track in r1
@ and returns the decoded byte in r3 while updating r2/r3, rather than a
@ standard r0 return.
.syntax unified
	.thumb
	.global ld_r3_tp_adr_i_unchecked
	.thumb_func
ld_r3_tp_adr_i_unchecked:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	bx	lr
	.2byte	0x0000
