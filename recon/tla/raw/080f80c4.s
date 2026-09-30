.syntax unified
	.thumb
	.global Func_080f80c4
	.thumb_func
Func_080f80c4:
	push {lr}
	movs r0, #169
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #179
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_080202d8
	bl Func_080146d4
	pop {pc}
