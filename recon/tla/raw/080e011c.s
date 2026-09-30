.syntax unified
	.thumb
	.global Func_080e011c
	.thumb_func
Func_080e011c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #1
	ldr r3, [r3, #20]
	adds r3, #91
	strb r2, [r3]
	bl Func_080e0134
	pop {pc}
