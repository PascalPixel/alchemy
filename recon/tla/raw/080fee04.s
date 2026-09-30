.syntax unified
	.thumb
	.global Func_080fee04
	.thumb_func
Func_080fee04:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	adds r0, r5, #0
	bl Func_080f88d0
	movs r1, #2
	movs r2, #2
	bl Func_08104fe0
	adds r3, r5, #0
	movs r2, #0
	adds r3, #244
	str r2, [r5, #44]
	str r2, [r5, #40]
	str r2, [r5, #48]
	str r2, [r5, #36]
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	adds r2, r5, #0
	adds r2, #246
	movs r3, #8
	strb r3, [r2]
	adds r2, #1
	movs r3, #2
	strb r3, [r2]
	pop {r5, pc}
