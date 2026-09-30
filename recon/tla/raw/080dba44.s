.syntax unified
	.thumb
	.global Func_080dba44
	.thumb_func
Func_080dba44:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r0, [r3]
	movs r3, #249
	lsls r3, r3, #3
	adds r0, r0, r3
	bl Func_080eb01c
	pop {pc}
	.2byte 0x0000
