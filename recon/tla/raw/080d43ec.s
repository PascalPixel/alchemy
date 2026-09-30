.syntax unified
	.thumb
	.global Func_080d43ec
	.thumb_func
Func_080d43ec:
	push {r5, r6, lr}
	adds r6, r1, #0
	movs r1, #213
	adds r5, r0, #0
	lsls r1, r1, #4
	movs r0, #108
	bl Runtime_AllocateBlock
	movs r3, #230
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r3, [r0]
	str r5, [r3, #48]
	str r6, [r3, #52]
	pop {r5, r6, pc}
	.2byte 0x0000
