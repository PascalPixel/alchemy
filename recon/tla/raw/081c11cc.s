.syntax unified
	.thumb
	.global Func_081c11cc
	.thumb_func
Func_081c11cc:
	movs r3, #7
	ands r3, r0
	movs r2, #1
	lsls r2, r3
	movs r3, #192
	lsls r3, r3, #2
	ldr r1, .L_081c11e8
	adds r3, #255
	ands r3, r0
	asrs r0, r3, #3
	ldrb r3, [r1, r0]
	bics r3, r2
	strb r3, [r1, r0]
	bx lr
.L_081c11e8:
	.4byte Data_02002f20
