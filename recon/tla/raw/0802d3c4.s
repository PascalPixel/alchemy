.syntax unified
	.thumb
	.global Func_0802d3c4
	.thumb_func
Func_0802d3c4:
	ldr r3, .L_0802d3d4
	lsls r1, r1, #4
	adds r2, r2, r1
	ldrb r3, [r3, r2]
	ldrsb r0, [r0, r3]
	lsls r0, r0, #19
	bx lr
	.2byte 0x0000
.L_0802d3d4:
	.4byte Data_0802eec4
