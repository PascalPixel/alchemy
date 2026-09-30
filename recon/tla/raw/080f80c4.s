.syntax unified
	.thumb
	.global Func_080f80c4
	.thumb_func
Func_080f80c4:
	push {lr}
	movs r0, #169
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #179
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Map_EnableUpdateCallbackFar
	bl Scheduler_EnableUnmaskedOverlayCallbacks
	pop {pc}
