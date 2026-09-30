.syntax unified
	.thumb
	.global Func_0813bb38
	.thumb_func
Func_0813bb38:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #92]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #144
	adds r5, r4, r1
	ldr r3, [r5]
	adds r1, #4
	adds r3, #1
	str r3, [r5]
	adds r2, r4, r1
	ldr r2, [r2]
	cmp r3, r2
	bne .L_0813bb9c
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #208
	adds r1, r4, r3
	ldr r3, [r1]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #40
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #212
	adds r0, r4, r3
	ldr r3, [r0]
	adds r2, #4
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #152
	adds r2, r4, r3
	ldr r2, [r2]
	ldr r3, [r1]
	adds r3, r3, r2
	str r3, [r1]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #156
	adds r2, r4, r1
	ldr r3, [r0]
	ldr r2, [r2]
	adds r3, r3, r2
	str r3, [r0]
	movs r3, #0
	str r3, [r5]
.L_0813bb9c:
	pop {r5, pc}
	.2byte 0x0000
