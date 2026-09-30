.syntax unified
	.thumb
	.global Func_080ca494
	.thumb_func
Func_080ca494:
	ldr r3, .L_080ca4a4
	movs r2, #251
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bx lr
	.2byte 0x0000
.L_080ca4a4:
	.4byte gPartyState
