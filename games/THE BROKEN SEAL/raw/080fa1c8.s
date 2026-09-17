@ Retired from C: the callers at 0x080fa1d4 and 0x080fa1e8 read the fetched
@ command byte from r3, a return register C cannot express. Disassembled from
@ the audited extent at 0x080fa1c8 (10 bytes).
.syntax unified
	.thumb
	.global MusicTrack_ReadCommandByte
	.global Func_080fa1c8
	.thumb_func
MusicTrack_ReadCommandByte:
Func_080fa1c8:
	ldr	r2, [r1, #64]	@ 0x40
	adds	r3, r2, #1
	str	r3, [r1, #64]	@ 0x40
	ldrb	r3, [r2, #0]
	bx	lr
