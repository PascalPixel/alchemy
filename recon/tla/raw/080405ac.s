.syntax unified
	.thumb
	.global Func_080405ac
	.thumb_func
Func_080405ac:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r5, [r3]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #164
	adds r0, r5, r2
	bl Func_08108030
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #116
	adds r3, r5, r2
	ldrh r3, [r3]
	adds r2, #156
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r0, [r5, r3]
	bl Func_080450fc
	pop {r5, pc}
	.2byte 0x0000
