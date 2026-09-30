.syntax unified
	.thumb
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	movs r3, #7
	ands r3, r0
	ldr r1, .L_08016d14
	movs r2, #1
	lsls r2, r3
	lsls r3, r0, #20
	lsrs r0, r3, #23
	ldrb r3, [r1, r0]
	orrs r2, r3
	strb r2, [r1, r0]
	bx lr
	.2byte 0x0000
.L_08016d14:
	.4byte GameFlagBytes
