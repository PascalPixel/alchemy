.syntax unified
	.thumb
	.global Func_081263f0
	.thumb_func
Func_081263f0:
	push {lr}
	movs r0, #40
	bl Runtime_ReleaseHeapBlock
	pop {pc}
	.2byte 0x0000
