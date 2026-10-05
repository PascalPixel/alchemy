.syntax unified
	.thumb
	.global Event_SetSceneEvents
	.thumb_func
Event_SetSceneEvents:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	str r0, [r3, #16]
	bx lr
	.2byte 0x0000
