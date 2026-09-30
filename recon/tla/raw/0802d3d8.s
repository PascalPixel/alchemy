.syntax unified
	.thumb
	.global Func_0802d3d8
	.thumb_func
Func_0802d3d8:
	ldr r3, .L_0802d3e8
	lsls r1, r1, #4
	subs r1, r1, r2
	adds r1, #15
	ldrb r3, [r3, r1]
	ldrsb r0, [r0, r3]
	lsls r0, r0, #19
	bx lr
.L_0802d3e8:
	.4byte Data_0802eec4
