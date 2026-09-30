.syntax unified
	.thumb
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	movs r3, #7
	ands r3, r0
	ldr r1, .L_08016d30
	movs r2, #1
	lsls r2, r3
	lsls r3, r0, #20
	lsrs r0, r3, #23
	ldrb r3, [r1, r0]
	bics r3, r2
	strb r3, [r1, r0]
	bx lr
	.2byte 0x0000
.L_08016d30:
	.4byte GameFlagBytes
