.syntax unified
	.thumb
	.global Func_080cefb4
	.thumb_func
Func_080cefb4:
	push {lr}
	bl ObjectTable_Get
	movs r1, #4
	bl Object_SetMode
	movs r0, #124
	bl Audio_PlayCue
	movs r0, #12
	bl WaitFrames
	pop {pc}
	.2byte 0x0000
