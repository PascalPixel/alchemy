.syntax unified
	.thumb
	.global EventRuntime_SetCountdown
	.thumb_func
EventRuntime_SetCountdown:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r3, r2
	strh r0, [r3]
	bx lr
