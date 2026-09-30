.syntax unified
	.thumb
	.global Func_08016d34
	.thumb_func
Func_08016d34:
	adds r4, r0, #0
	movs r3, #7
	ldr r2, .L_08016d58
	ands r3, r4
	movs r1, #1
	lsls r1, r3
	lsls r3, r4, #20
	lsrs r4, r3, #23
	ldrb r0, [r2, r4]
	adds r3, r1, #0
	eors r3, r0
	strb r3, [r2, r4]
	ldrb r3, [r2, r4]
	ands r3, r1
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	bx lr
.L_08016d58:
	.4byte GameFlagBytes
