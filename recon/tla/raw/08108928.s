.syntax unified
	.thumb
	.global Func_08108928
	.thumb_func
Func_08108928:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #108]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #34
	adds r0, r0, r3
	movs r1, #1
	bl Func_080c8378
	movs r0, #16
	bl Func_080c8390
	pop {pc}
	.2byte 0x0000
