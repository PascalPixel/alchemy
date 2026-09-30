.syntax unified
	.thumb
	.global UiWindow_OpenMode1AndWaitFrame
	.thumb_func
UiWindow_OpenMode1AndWaitFrame:
	push {lr}
	movs r0, #1
	bl Func_08042690
	movs r0, #1
	bl WaitFrames
	pop {pc}
