.syntax unified
	.thumb
	.global Func_080cefe0
	.thumb_func
Func_080cefe0:
	push	{lr}
	bl	ObjectTable_Get
	movs	r1, #6
	bl	Object_SetMode
	movs	r0, #124
	bl	Audio_PlayCue
	movs	r0, #12
	bl	WaitFrames
	pop	{pc}
	.2byte 0x0000
