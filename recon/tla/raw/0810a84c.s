.syntax unified
	.thumb
	.global Func_0810a84c
	.thumb_func
Func_0810a84c:
	ldr r3, .L_0810a860
	movs r2, #175
	lsls r2, r2, #2
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	bx lr
.L_0810a860:
	.4byte gPartyState
