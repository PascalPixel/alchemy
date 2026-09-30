.syntax unified
	.thumb
	.global Func_080cef68
	.thumb_func
Func_080cef68:
	push	{lr}
	bl	ObjectTable_Get
	movs	r1, #5
	bl	Object_SetMode
	movs	r0, #125
	bl	Audio_PlayCue
	movs	r0, #12
	bl	WaitFrames
	pop	{pc}
	.2byte 0x0000
