.syntax unified
	.thumb
	.global Func_08108630
	.thumb_func
Func_08108630:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #220
	adds r5, r5, r3
	adds r7, r0, #0
	ldr r0, [r5]
	ldrb r6, [r0, #5]
	bl UiIcon_PrepareObjectFar
	adds r2, r7, #0
	movs r1, #5
	movs r0, #7
	bl Func_08038390
	ldr r3, [r5]
	adds r7, r0, #0
	strb r6, [r3, #5]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
