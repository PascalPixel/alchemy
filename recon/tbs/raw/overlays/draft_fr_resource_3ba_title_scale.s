/* NONMATCHING: French Colosso title scale script, 2026-10-01.
 * The complete 126-byte script retains the generic single-sprite scale
 * range. Ten halfwords need the two-sprite French range: 0x180 to 0x200.
 */
	.section .rodata,"a",%progbits

	.global KorosseoKawa_ModeRecordFour
KorosseoKawa_ModeRecordFour:
	.2byte 0x1000
	.ifdef TBS_EDITION_ES
	.2byte 0x0180
	.else
	.ifdef TBS_EDITION_IT
	.2byte 0x0180
	.else
	.2byte 0x0200
	.endif
	.endif
	.2byte 1
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x20000006
	.4byte 0x001e0000
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0xffff0006
