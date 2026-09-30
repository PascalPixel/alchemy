.syntax unified
	.thumb
	.global Event_RunObjectHookAndWait
	.thumb_func
Event_RunObjectHookAndWait:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_080d2394
	adds r0, r5, #0
	bl Func_080caa4c
	movs r0, #1
	bl WaitFrames
	bl Func_080cdf5c
	bl ObjectTable_Get
	pop {r5, pc}
	.2byte 0x0000
