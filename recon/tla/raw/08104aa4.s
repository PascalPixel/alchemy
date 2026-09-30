.syntax unified
	.thumb
	.global Func_08104aa4
	.thumb_func
Func_08104aa4:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #238
	adds r3, r5, r2
	ldrh r0, [r3]
	bl Func_08014274
	movs r3, #158
	lsls r3, r3, #3
	adds r5, r5, r3
	ldrh r0, [r5]
	bl Func_08014274
	pop {r5, pc}
	.2byte 0x0000
