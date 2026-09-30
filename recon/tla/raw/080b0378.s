.syntax unified
	.thumb
	.global BattleRandom16
	.thumb_func
BattleRandom16:
	ldr r1, .L_080b0394
	ldr r3, .L_080b0398
	ldr r2, [r1]
	adds r0, r2, #0
	muls r0, r3
	movs r3, #192
	lsls r3, r3, #6
	adds r3, #57
	adds r0, r0, r3
	str r0, [r1]
	lsls r0, r0, #8
	lsrs r0, r0, #16
	bx lr
	.2byte 0x0000
.L_080b0394:
	.4byte Data_020054c8
.L_080b0398:
	.4byte 0x41c64e6d
