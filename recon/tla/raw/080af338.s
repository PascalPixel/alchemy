.syntax unified
	.thumb
	.global Func_080af338
	.thumb_func
Func_080af338:
	push {lr}
	bl Item_GetDirect
	ldrh r0, [r0, #40]
	bl Func_080af43c
	ldrb r0, [r0]
	pop {pc}
