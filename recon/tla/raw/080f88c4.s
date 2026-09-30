.syntax unified
	.thumb
	.global Func_080f88c4
	.thumb_func
Func_080f88c4:
	push {lr}
	movs r0, #0
	movs r2, #0
	bl RenderOutput_CreateFar + 0x18
	pop {pc}
