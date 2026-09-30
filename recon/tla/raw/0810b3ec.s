.syntax unified
	.thumb
	.global Func_0810b3ec
	.thumb_func
Func_0810b3ec:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_08108148
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #6
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3]
	adds r0, r5, #0
	bl Func_0810b378
	adds r5, r0, #0
	bl Func_0810824c
	adds r0, r5, #0
	pop {r5, pc}
