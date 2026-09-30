.syntax unified
	.thumb
	.global Func_0813fa44
	.thumb_func
Func_0813fa44:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #92]
	ldr r0, [r3, #36]
	ldr r3, [r5, #8]
	cmp r3, #0
	ble .L_0813fa84
	ldr r2, [r5, #12]
	movs r3, #160
	lsls r3, r3, #3
	adds r2, #1
	adds r3, #108
	adds r0, r0, r3
	lsls r3, r2, #4
	adds r3, r3, r2
	lsls r3, r3, #4
	str r2, [r5, #12]
	adds r3, r3, r2
	movs r1, #160
	movs r2, #128
	lsls r3, r3, #2
	lsls r1, r1, #19
	lsls r2, r2, #9
	subs r2, r2, r3
	adds r1, #192
	movs r3, #128
	bl Func_08118088 + 0x68
	ldr r3, [r5, #8]
	subs r3, #1
	str r3, [r5, #8]
.L_0813fa84:
	pop {r5, pc}
	.2byte 0x0000
