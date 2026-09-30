.syntax unified
	.thumb
	.global Sys_Free
	.thumb_func
Sys_Free:
	movs r4, #3
	lsls r4, r4, #24
	movs r1, #4
	lsrs r2, r0, #22
	ands r2, r1
	str r0, [r2, r4]
	bx lr
	.2byte 0x0000
	.4byte 0xef030000
	.4byte 0xeafffffc
	.4byte 0xe3a0c301
	.4byte 0xe28c3c02
	.4byte 0xe5932000
	.4byte 0xe0021822
	.4byte 0xe3a02000
	.4byte 0xe2110a02
	.4byte 0x1543017c
	.4byte 0x1afffffe
