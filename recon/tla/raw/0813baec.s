.syntax unified
	.thumb
	.global Func_0813baec
	.thumb_func
Func_0813baec:
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
	bne .L_0813bb32
	movs r2, #238
	ldr r1, .L_0813bb34
	lsls r2, r2, #7
	adds r2, #152
	adds r3, r4, r2
	ldr r2, [r3]
	ldrh r3, [r1, #4]
	movs r0, #0
	adds r3, r3, r2
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #156
	strh r3, [r1, #4]
	adds r3, r4, r2
	ldr r2, [r3]
	ldrh r3, [r1, #6]
	str r0, [r5]
	adds r3, r3, r2
	strh r3, [r1, #6]
.L_0813bb32:
	pop {r5, pc}
.L_0813bb34:
	.4byte Data_03001120
