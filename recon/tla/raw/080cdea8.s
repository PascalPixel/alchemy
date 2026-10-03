.syntax unified
	.thumb
	.global EventRuntime_RunMessage
	.thumb_func
EventRuntime_RunMessage:
	push {r5, r6, lr}
	adds r5, r1, #0
	adds r6, r0, #0
	bl EventRuntime_Begin
	adds r0, r5, #0
	bl EventRuntime_SetMessage
	adds r0, r6, #0
	movs r1, #0
	bl EventRuntime_ShowMessageAndWait
	bl EventRuntime_End
	pop {r5, r6, pc}
	.2byte 0x0000
