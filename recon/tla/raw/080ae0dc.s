.syntax unified
	.thumb
	.global Func_080ae0dc
	.thumb_func
Func_080ae0dc:
	ldr r3, .L_080ae0ec
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #5
	str r2, [r3]
	bx lr
	.2byte 0x0000
.L_080ae0ec:
	.4byte gPartyState
