.syntax unified
	.thumb
	.global EventRuntime_ResolveAllPendingActions
	.thumb_func
EventRuntime_ResolveAllPendingActions:
	push {lr}
	movs r0, #0
	bl EventRuntime_ResolvePendingActions
	pop {pc}
	.2byte 0x0000
