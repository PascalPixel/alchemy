.syntax unified
	.thumb
	.balign 4
	.global Func_081c1000
	.thumb_func
Func_081c1000:
	push	{lr}
	bl	AudioEngine_SuspendDirectSound
	pop	{pc}
	svc	27
	bx	lr
	svc	35
	bx	lr
	bx	lr
	.2byte 0x0000