.syntax unified
	.thumb
	.global AudioEngine_SuspendDirectSoundWrapper
	.thumb_func
AudioEngine_SuspendDirectSoundWrapper:
	push {lr}
	bl AudioEngine_SuspendDirectSound
	pop {pc}
