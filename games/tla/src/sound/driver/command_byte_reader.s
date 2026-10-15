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
	.2byte	0x0000
