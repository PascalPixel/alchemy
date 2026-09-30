.syntax unified
	.thumb
	.global Func_081c1000
	.thumb_func
Func_081c1000:
	push {lr}
	bl AudioEngine_SuspendDirectSound
	pop {pc}
