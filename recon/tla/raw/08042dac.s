.syntax unified
	.thumb
	.global Func_08042dac
	.thumb_func
Func_08042dac:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #64]
	movs r1, #1
	ldr r0, [r3]
	bl UiWork_Finalize
	movs r0, #64
	bl Runtime_ReleaseHeapBlock
	pop {pc}
