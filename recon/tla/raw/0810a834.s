.syntax unified
	.thumb
	.global Func_0810a834
	.thumb_func
Func_0810a834:
	ldr r2, .L_0810a844
	ldr r3, .L_0810a848
	ands r0, r2
	movs r2, #175
	lsls r2, r2, #2
	adds r3, r3, r2
	strh r0, [r3]
	bx lr
.L_0810a844:
	.4byte 0x000001ff
.L_0810a848:
	.4byte gPartyState
