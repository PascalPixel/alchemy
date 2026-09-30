.syntax unified
	.thumb
	.global Func_0802d400
	.thumb_func
Func_0802d400:
	ldr r3, .L_0802d410
	lsls r2, r2, #4
	subs r2, r2, r1
	adds r2, #15
	ldrb r3, [r3, r2]
	ldrsb r0, [r0, r3]
	lsls r0, r0, #19
	bx lr
.L_0802d410:
	.4byte Data_0802eec4
