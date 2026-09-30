.syntax unified
	.thumb
	.global Func_081c1188
	.thumb_func
Func_081c1188:
	movs r3, #7
	ands r3, r0
	movs r2, #1
	lsls r2, r3
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r0
	asrs r0, r3, #3
	ldr r3, .L_081c11a8
	ldrb r3, [r3, r0]
	ands r3, r2
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	bx lr
.L_081c11a8:
	.4byte Data_02002f20
