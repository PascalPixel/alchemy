.syntax unified
	.thumb
	.global EventRuntime_ShowMessageAndWait
	.thumb_func
EventRuntime_ShowMessageAndWait:
	push {lr}
	bl Func_080d3fb0
	pop {pc}
