.syntax unified
	.thumb
	.global UiWindow_OpenMode1AndWaitFrame
	.thumb_func
UiWindow_OpenMode1AndWaitFrame:
	push {lr}
	movs r0, #1
	bl UiWindow_CreateWithLayoutBounds
	movs r0, #1
	bl WaitFrames
	pop {pc}
