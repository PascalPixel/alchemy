.syntax unified
	.thumb
	.global Func_0801613c
	.thumb_func
Func_0801613c:
	push {lr}
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Runtime_SetIrqHandler
	movs r0, #204
	bl Runtime_ReleaseHeapBlock
	pop {pc}
