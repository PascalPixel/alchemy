.syntax unified
	.thumb
	.global Func_08027e0c
	.thumb_func
Func_08027e0c:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_08026e60
	ldrh r3, [r5, #4]
	movs r0, #1
	adds r3, #1
	strh r3, [r5, #4]
	pop {r5, pc}
	.2byte 0x0000
