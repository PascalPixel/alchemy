.syntax unified
	.thumb
	.global Func_0802d370
	.thumb_func
Func_0802d370:
	adds r3, r0, #0
	movs r0, #0
	ldrsb r0, [r3, r0]
	movs r4, #1
	ldrsb r4, [r3, r4]
	ldr r3, .L_0802d390
	lsls r2, r2, #4
	adds r1, r1, r2
	ldrb r3, [r3, r1]
	lsls r0, r0, #19
	lsls r4, r4, #19
	subs r4, r4, r0
	muls r3, r4
	adds r0, r0, r3
	bx lr
	.2byte 0x0000
.L_0802d390:
	.4byte Data_0802edc4
